#include <Wrapper.h>
#include "../../src/defaults.h"
#include <CmdParser.h>
#include <helper_general.h>
#include <helper_wifi.h>
#include <MQTT2.h>
#include <Settings.h>
//#include <Ticker.h>

#include <time.h>           // time() ctime()
#ifdef ESP8266
  #include <coredecls.h>      // settimeofday_cb()
#else
  #include "esp_sntp.h"
#endif
//#include <TZ.h>
#include <TaskQueue.h>
#include <FOTA_ESP.h>


#define QUEUE_EXEC_DELAY 100

WebConfig2 Web2;

uint16_t wifiDisconnections = 0;


void startWeb(){
  if (helper_wifi::wifiConnected()) {
    Serial.println("SetSTA.");
    setSTA(true);
  } else {
    Serial.println("SetAP.");
    setAP(true);
  }
   Web2.begin();
}

//
// MQTT Status
//
bool MQTTConnected = false;

void sendMQTTSystemInfo() {
  MQTTSendSystem(CSTR(helper_general::getSystemInfoJson()));
}

void sendMQTTStatusInfo() {
  MQTTSendStatus(
      CSTR(helper_general::getSystemStatusJson(wifiDisconnections, mqttDisconnections)));
}

void sendMQTTData(){
  MQTTSendData("test data");
}

Task tMqttSendStatusInfo(current_settings.statusUpdateInterval * TASK_MINUTE,
                        TASK_FOREVER, &sendMQTTStatusInfo, &ts, false);


// ============================================================================
//      NTP sync
// ============================================================================
bool NTPSynced = false;
void NTPTrySync();        // forward declaration

#ifdef ESP32
void NTPSynced_cb(struct timeval *tv) {
#else
void NTPSynced_cb() {
#endif
  NTPSynced = true;
  Serial.println("Time is SET.");
  #ifdef ESP32
    Serial.println(tv->tv_sec);
    Serial.println(ctime(&tv->tv_sec));
  #endif
}

// sync with ntp server might take more than 30seconds to complete.
// check every 500ms if synced
//Task tNTPSync(500 * TASK_MILLISECOND, TASK_FOREVER, &NTPTrySync, &ts, false);

bool NTPSet() {
  Serial.println("Set Time.");

#ifdef ESP8266
  settimeofday_cb(NTPSynced_cb);
  configTime( current_settings.timeZone, current_settings.ntpServer);
#else
  // * ESP32 Arduino does not have callback (yet) when time is set. UPDATE use the following:
  // https://techtutorialsx.com/2021/09/03/esp32-sntp-additional-features/
  sntp_set_time_sync_notification_cb(NTPSynced_cb);
  configTzTime( current_settings.timeZone, current_settings.ntpServer);
  //tNTPSync.enable();
#endif
//  configTime( 0, 0, current_settings.ntpServer); //sets TZ and starts NTP sync
//  https://github.com/esp8266/Arduino/pull/7144#issue-385658333
//  environment variable TZ may not be set up after configTime().
//  setenv("TZ", , 1);
//  tzset();
  return true;
}

// void NTPTrySync() {
//   NTPSynced = false;
//   time_t now;
//   tm info;
//   time(&now);
//   localtime_r(&now, &info);
//   if (info.tm_year > (2016 - 1900)) {
//     NTPSynced = true;
//     tNTPSync.disable();
//   }
//   Serial.print("Sync Time: ");
//   Serial.println(NTPSynced);
// }

// ============================================================================
// FOTA Firmware over the air
// ============================================================================
void onFOTAMessage(fota_t msg, char* txt) {
  TLOGDEBUGF(txt);
}

void FOTACheck() {
  FOTAClient.checkAndUpdateFOTA(true);
}

Task tFOTACheck(current_settings.firmwareUpdateCheckInterval * TASK_MINUTE, TASK_FOREVER, &FOTACheck, &ts, false);

// ============================================================================
//      WiFi
// ============================================================================
// @ callback. Do not call functions directly.
void wifiStatusChange(conn2_wifi_status_t status, char *message) {
  // todo process all statuses
  switch (status) {

    case STATUS_FOUND_NETWORK:

      break;

    case STATUS_DISCONNECTED:  // do not do any processing here.
      wifiDisconnections++;
      TLOGINFO(message);

      break;

    case STATUS_FORCED_DISCONNECTION:
      break;

    case STATUS_GOT_IP:
      TLOGINFO(message);
      NTPSet();
      // enable the task to check for fw updates.
      Serial.println(current_settings.firmwareUpdateCheckInterval);
      Serial.println(new_settings.firmwareUpdateCheckInterval);
      tFOTACheck.enable();
      // tNTPSync.enable();
      // connect to mqtt
      addToTaskQueue(mqttConnect);
      // start the webconfig in STA mode
      addToTaskQueue(startWeb);
      break;

    case STATUS_NO_NETWORKS:
    case STATUS_NO_KNOWN_NETWORKS:
    case STATUS_EXIT_TO_CAPTIVE_PORTAL:
      TLOGINFO(message);
      // run captive portal
      // start the webconfig in AP mode
      addToTaskQueue(startWeb);
      break;

    default:
      TLOGINFO(message);
      break;
  }
}
// ============================================================================
//      MQTT
// ============================================================================
// @ callback
void mqttConnectionChanged(mqttConnectionStatus_t status, char *message) {
  switch (status) {
    case MQTT_DISCONNECTED:
      TLOGDEBUG(message);
      MQTTConnected = false;
      tMqttSendStatusInfo.disable();
      if (helper_wifi::wifiConnected()) {
        addToTaskQueue(mqttConnect);
      }
      break;
    case MQTT_CONNECTING:
      TLOGDEBUG(message);
      break;
    case MQTT_CONNECTED:
      MQTTConnected = true;
      TLOGDEBUG(message);
      addToTaskQueue(sendMQTTSystemInfo);
      addToTaskQueue(sendMQTTStatusInfo);
      addToTaskQueue(sendMQTTData);  // demo data
      break;
    default:
      break;
  }
}

void commandGetSystemStatus() {
  MQTTSendStatus(
      CSTR(helper_general::getSystemStatusJson(wifiDisconnections, mqttDisconnections)));
}

void commandGetSystemInfo() {
  MQTTSendSystem(helper_general::getSystemInfoJson());
}

void commandGetSettings1(){
  MQTTSendStatus(settings2Json(current_settings));
}

void commandGetSettings2(){
  MQTTSendStatus(settings2Json(new_settings));
}

void commandGetSavedNetworks() {
  MQTTSendStatus(loadNetworksConfigJson(current_settings));
}

void commandDeleteNetworks() {
  bool result = deleteNetworksConfigJson();
  MQTTSendStatus("Delete networks file " +
                  helper_general::boolToString(result, SUCCESS_STR, FAIL_STR));
}

void commandDeleteSettings() {
  MQTTSendStatus(
      "{" + helper_general::jsonPair(PARSER_RESPONSE, "Deleting settings file...") + "}");
  bool result = deleteSettingsFile();
  MQTTSendStatus(
      "{" +
      helper_general::jsonPair(PARSER_RESPONSE,
                "Delete settings: " + helper_general::boolSuccess(result)) +
      "}");
}

void commandFormatFS() {
  MQTTSendStatus("{" + helper_general::jsonPair(PARSER_RESPONSE, "Formatting...") + "}");
  bool result = formatFS();
  MQTTSendStatus(
      "{" +
      helper_general::jsonPair(PARSER_RESPONSE,
                "Format: " + helper_general::boolSuccess(result)) +
      "}");
}

void commandSettingsModified() {
  MQTTSendStatus("{" + helper_general::jsonPair(PARSER_RESPONSE, "Settings modified.") +
                  "}");
}

void commandLoggedIn() {
  MQTTSendStatus("{" + helper_general::jsonPair(PARSER_RESPONSE, "Admin logged in.") + "}");
}

void commandLoggedOut() {
  MQTTSendStatus("{" + helper_general::jsonPair(PARSER_RESPONSE, "Admin logged out.") +
                  "}");
}

void commandSaveSettings() {
  MQTTSendStatus("{" + helper_general::jsonPair(PARSER_RESPONSE, "Saving new settings...") +
                  "}");
  bool result = saveSettings(new_settings);
  MQTTSendStatus(
      "{" +
      helper_general::jsonPair(PARSER_RESPONSE,
                "Format: " + helper_general::boolSuccess(result)) +
      "}");
}

void commandNoAdminRights() {
  MQTTSendStatus(
      "{" + helper_general::jsonPair(PARSER_RESPONSE, "No Admin rights to this command.") +
      "}");
}

void commandError() {
  MQTTSendStatus("{" + helper_general::jsonPair(PARSER_RESPONSE, "Command error!") + "}");
}

void commandReboot() {
  wifiDisconnect();
  delay(4000);
  ESP.restart();
  delay(1000);
}

/*
 * Message from mqtt broker has been received.
 ! add commands to execution queue. Do not call call directly
 ! within this callback.
 */
void mqttMessage(char *topic, char *message, uint8_t qos, bool dup, bool retain,
                 uint32_t len) {
  TLOGDEBUGF_P(PSTR("MQTT message received. Topic: [%s] qos: %hu Retain %d "
                       "Duplicate: %d len: %lu Payload: %s\n"),
                  topic, qos, retain, dup, (unsigned long)len, message);

  mqtt_command_t cmd;

  cmd = decodeCommand(message, new_settings);
  TLOGDEBUGLN(cmd);
  switch (cmd) {

    case system_status:
      addToTaskQueue(commandGetSystemStatus);
      break;

    case system_info:
      addToTaskQueue(commandGetSystemInfo);
      break;

    case settings_cur:
      addToTaskQueue(commandGetSettings1);
      break;

    case settings_new:
      addToTaskQueue(commandGetSettings2);
      break;

    case saved_networks:
      addToTaskQueue(commandGetSavedNetworks);
      break;

    case delete_networks:
      addToTaskQueue(commandDeleteNetworks);
      break;

    case delete_settings:
      addToTaskQueue(commandDeleteSettings);
      break;

    case format_fs:
      addToTaskQueue(commandFormatFS);
      break;

    case settings_modified:
      addToTaskQueue(commandSettingsModified);
      break;

    case logged_in:
      addToTaskQueue(commandLoggedIn);
      break;

    case logged_out:
      addToTaskQueue(commandLoggedOut);
      break;

    case save_settings:
      addToTaskQueue(commandSaveSettings);
      break;

    case no_admin_rights:
      addToTaskQueue(commandNoAdminRights);
      break;

    case cmd_error:
      addToTaskQueue(commandError);
      break;

    case scan_wifi:
      break;

    case add_wifi:
      break;

    case delete_wifi:
      break;

    case reset:
      break;

    case reboot:
      addToTaskQueue(commandReboot);
      break;

    case fota_update:
      break;

    default:
      break;
  }
}

void addNetworksCallback(const char *ssid, const char *password, bool dhcp,
                         const char *ip, const char *netmask,
                         const char *gateway, const char *dns) {
  TLOGDEBUGF_P(PSTR("[Add WiFi] ssid: %s, pass: %s, dhcp: %d, ip: %s, netmask: %s, gateway: %s, dns: %s\n"),
      ssid, password, dhcp, ip, netmask, gateway, dns);
  if (dhcp) {
    addWiFi(ssid, password);
  } else {
    addWiFi(ssid, password, ip, netmask, gateway, dns);
  }
}

void webConfigStatusChange(webconfig_status_t wcStatus, char *message) {
  TLOGDEBUG(message);
  switch (wcStatus) {
    case WEB_CONFIG_TIMEOUT:
      if (!helper_wifi::wifiConnected()) {
        // clear all (marked to connect) networks and force rescan on next connect
        resetSavedNetworks();
        addToTaskQueue(wifiConnect); // reconnect wifi
      }
      // ESP.restart();
      /* code */
      break;
    case WEB_CONFIG_STARTED:
      TLOGDEBUGLN("Web config started.");
      break;
    default:
      break;
  }
}

void initialize() {
  // load current_settings from fs
  loadSettings(current_settings);
  TLOGINFOF_P(PSTR("Current Settings: %s\n\n"), settings2Json(current_settings).c_str());

  // make a copy of current_settings to new_settings.
  copySettings(current_settings, new_settings);
  TLOGINFOF_P(PSTR("New Settings: %s\n\n"), settings2Json(new_settings).c_str());

  // load networks config file.
  // Holds all wifi networks that this system can connect.
  loadNetworks(addNetworksCallback);

  // setup Firmware Over the Air
  FOTAClient.setFOTAParameters(helper_general::addTrailingSlash(String(current_settings.firmwareConfURL)), helper_general::addMacAddress(String(current_settings.deviceName)), APP_VERSION, FS_VERSION); // todo: add defined
  FOTAClient.onMessage(onFOTAMessage);

  TLOGINFOLN("Setup MQTT");
  // mqtt setup
  mqttServerSetup(current_settings.broker1, current_settings.mqttPort1,
                  current_settings.mqttUser1, current_settings.mqttPass1,
                  current_settings.mqttGlobalTopic,
                  current_settings.deviceName);

  mqttBackupServerSetup(current_settings.broker2, current_settings.mqttPort2,
                        current_settings.mqttUser2, current_settings.mqttPass2);

  onMQTTConnectionChange(mqttConnectionChanged);
  onMQTTMessage(mqttMessage);

  // webconfig setup
  TLOGINFOLN("Setup WebConfig");
  // if there is no action within this seconds, exit webconfig.
  Web2.setTimeout(60);
  // setup callback to changes of webconfig
  Web2.onStatusChange(webConfigStatusChange);

  // wifi connection setup
  TLOGINFOLN("Setup WiFi");
  onWiFiStatusChange(wifiStatusChange);
  wifiSetup();
  //wifiConnect();
  addToTaskQueue(wifiConnect);
  //Task *t = new Task(100 * TASK_MILLISECOND,  TASK_ONCE, &wifiConnect, &ts, true, NULL, &onDisableDeleteTask);
}

void wifiLoop() {
}


Task tMqttSendData(new_settings.statusUpdateInterval * TASK_MINUTE, TASK_FOREVER, &sendMQTTStatusInfo, &ts, false);

/*
 * each time this function is called checks if there are tasks
 * in the queue and executes one each time.
 */
void tasksExecute() {
  static uint32_t lastTaskLoopMs = 0;
  if (millis() - lastTaskLoopMs > QUEUE_EXEC_DELAY) {
    lastTaskLoopMs = millis();
    executeTaskQueue();
  }
}

/*
 * this task is created in main file (tWrapper) and is called continuosly
 */
void wrapperLoop() {
  if (NTPSynced && MQTTConnected && !tMqttSendStatusInfo.isEnabled()) tMqttSendStatusInfo.enable();
  tasksExecute();
  wifiLoop();

  Web2.loop();
}