#include <Arduino.h>
#include "../../src/defaults.h"
#include <helper_general.h>
#include <helper_wifi.h>

#include <WiFiConnection2.h>
#include <Settings.h>
#include <vector>
#include <TaskQueue.h>


#define WIFI_STR "[\e[32mWIFI2\e[m] "

uint16_t  _scanNetworksFound          = 0;
bool      wifiScanRemoveDuplicates    = true;
uint8_t   wifiConnectionRetries       = 3;
uint16_t  _wifiRetryConnectionCount   = 0;
uint8_t   _idToConnect                = 0;
uint8_t   _validNetworks              = 0;
String    _wifisJson;
bool      _scanNeeded                 = true;
bool      _connectAfterScan           = true;
bool      _doWiFiConnectCalled        = false;
String    wifiConnecting              = "";
bool      _forcedDisconnect           = false;

conn2_wifi_status_t _status;

#ifdef ESP8266
WiFiEventHandler onStationModeConnectedHandler,
                 onStationModeDisconnectedHandler,
                 onStationModeGotIPHandler;
#endif

std::vector<conn2_network_t>      _network_list;
std::vector<TWiFiStatusCallbackF> _wifi_status_callbacks;


// ===========================================

#ifdef ESP32
#include <esp_event.h>

enum WiFiDisconnectReason  // it does not exist for ESP32. Declare it.
{ WIFI_DISCONNECT_REASON_UNSPECIFIED = 1,
  WIFI_DISCONNECT_REASON_AUTH_EXPIRE = 2,
  WIFI_DISCONNECT_REASON_AUTH_LEAVE = 3,
  WIFI_DISCONNECT_REASON_ASSOC_EXPIRE = 4,
  WIFI_DISCONNECT_REASON_ASSOC_TOOMANY = 5,
  WIFI_DISCONNECT_REASON_NOT_AUTHED = 6,
  WIFI_DISCONNECT_REASON_NOT_ASSOCED = 7,
  WIFI_DISCONNECT_REASON_ASSOC_LEAVE = 8,
  WIFI_DISCONNECT_REASON_ASSOC_NOT_AUTHED = 9,
  WIFI_DISCONNECT_REASON_DISASSOC_PWRCAP_BAD = 10,      /* 11h */
  WIFI_DISCONNECT_REASON_DISASSOC_SUPCHAN_BAD = 11,     /* 11h */
  WIFI_DISCONNECT_REASON_IE_INVALID = 13,               /* 11i */
  WIFI_DISCONNECT_REASON_MIC_FAILURE = 14,              /* 11i */
  WIFI_DISCONNECT_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,   /* 11i */
  WIFI_DISCONNECT_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16, /* 11i */
  WIFI_DISCONNECT_REASON_IE_IN_4WAY_DIFFERS = 17,       /* 11i */
  WIFI_DISCONNECT_REASON_GROUP_CIPHER_INVALID = 18,     /* 11i */
  WIFI_DISCONNECT_REASON_PAIRWISE_CIPHER_INVALID = 19,  /* 11i */
  WIFI_DISCONNECT_REASON_AKMP_INVALID = 20,             /* 11i */
  WIFI_DISCONNECT_REASON_UNSUPP_RSN_IE_VERSION = 21,    /* 11i */
  WIFI_DISCONNECT_REASON_INVALID_RSN_IE_CAP = 22,       /* 11i */
  WIFI_DISCONNECT_REASON_802_1X_AUTH_FAILED = 23,       /* 11i */
  WIFI_DISCONNECT_REASON_CIPHER_SUITE_REJECTED = 24,    /* 11i */

  WIFI_DISCONNECT_REASON_BEACON_TIMEOUT = 200,
  WIFI_DISCONNECT_REASON_NO_AP_FOUND = 201,
  WIFI_DISCONNECT_REASON_AUTH_FAIL = 202,
  WIFI_DISCONNECT_REASON_ASSOC_FAIL = 203,
  WIFI_DISCONNECT_REASON_HANDSHAKE_TIMEOUT = 204 };
#endif

String getLastDisconnectReason(WiFiDisconnectReason lastDisconnectReason) {
  switch (lastDisconnectReason) {
    case WIFI_DISCONNECT_REASON_UNSPECIFIED:
      return F("Unspecified");
    case WIFI_DISCONNECT_REASON_AUTH_EXPIRE:
      return F("Auth expire");
    case WIFI_DISCONNECT_REASON_AUTH_LEAVE:
      return F("Auth leave");
    case WIFI_DISCONNECT_REASON_ASSOC_EXPIRE:
      return F("Assoc expire");
    case WIFI_DISCONNECT_REASON_ASSOC_TOOMANY:
      return F("Assoc toomany");
    case WIFI_DISCONNECT_REASON_NOT_AUTHED:
      return F("Not authed");
    case WIFI_DISCONNECT_REASON_NOT_ASSOCED:
      return F("Not assoced");
    case WIFI_DISCONNECT_REASON_ASSOC_LEAVE:
      return F("Assoc leave");
    case WIFI_DISCONNECT_REASON_ASSOC_NOT_AUTHED:
      return F("Assoc not authed");
    case WIFI_DISCONNECT_REASON_DISASSOC_PWRCAP_BAD:
      return F("Disassoc pwrcap bad");
    case WIFI_DISCONNECT_REASON_DISASSOC_SUPCHAN_BAD:
      return F("Disassoc supchan bad");
    case WIFI_DISCONNECT_REASON_IE_INVALID:
      return F("IE invalid");
    case WIFI_DISCONNECT_REASON_MIC_FAILURE:
      return F("Mic failure");
    case WIFI_DISCONNECT_REASON_4WAY_HANDSHAKE_TIMEOUT:
      return F("4way handshake timeout");
    case WIFI_DISCONNECT_REASON_GROUP_KEY_UPDATE_TIMEOUT:
      return F("Group key update timeout");
    case WIFI_DISCONNECT_REASON_IE_IN_4WAY_DIFFERS:
      return F("IE in 4way differs");
    case WIFI_DISCONNECT_REASON_GROUP_CIPHER_INVALID:
      return F("Group cipher invalid");
    case WIFI_DISCONNECT_REASON_PAIRWISE_CIPHER_INVALID:
      return F("Pairwise cipher invalid");
    case WIFI_DISCONNECT_REASON_AKMP_INVALID:
      return F("AKMP invalid");
    case WIFI_DISCONNECT_REASON_UNSUPP_RSN_IE_VERSION:
      return F("Unsupp RSN IE version");
    case WIFI_DISCONNECT_REASON_INVALID_RSN_IE_CAP:
      return F("Invalid RSN IE cap");
    case WIFI_DISCONNECT_REASON_802_1X_AUTH_FAILED:
      return F("802 1X auth failed");
    case WIFI_DISCONNECT_REASON_CIPHER_SUITE_REJECTED:
      return F("Cipher suite rejected");
    case WIFI_DISCONNECT_REASON_BEACON_TIMEOUT:
      return F("Beacon timeout");
    case WIFI_DISCONNECT_REASON_NO_AP_FOUND:
      return F("No AP found");
    case WIFI_DISCONNECT_REASON_AUTH_FAIL:
      return F("Auth fail");
    case WIFI_DISCONNECT_REASON_ASSOC_FAIL:
      return F("Assoc fail");
    case WIFI_DISCONNECT_REASON_HANDSHAKE_TIMEOUT:
      return F("Handshake timeout");
  }
  return F("Unknown");
}

WiFiDisconnectReason _lastDisconnectReason;

String WiFiGetAPssid() {
  return helper_general::addMacAddress(String(current_settings.deviceName));
}

String WiFiGetHostname() {
  String hostname(WiFiGetAPssid());
  hostname.replace(" ", "-");
  hostname.replace("_", "-"); // See RFC952
  return hostname;
}


String _isHiddenStr(bool isHidden) {
  return isHidden ? F("[Hidden]") : F("        ");
}


// Only internal scope
void setAPinternal(bool enable) {
  if (enable) {
    // create and store unique AP SSID/PW to prevent ESP from starting AP mode with default SSID and No password!
    // setup ssid for AP Mode when needed
    String softAPSSID = WiFiGetAPssid();
    String pwd        = String(current_settings.adminPass);
    IPAddress apIP(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    if (!WiFi.softAPConfig(apIP, apIP, subnet)) {
      TLOGDEBUGF_P(PSTR("%s[AP] softAPConfig failed!\n"), WIFI_STR);
    }

    TLOGDEBUGF_P(PSTR("%s[AP] ssid: %s pass: %s\n"), WIFI_STR, softAPSSID.c_str(), pwd.c_str());

    if (WiFi.softAP(softAPSSID.c_str(), pwd.c_str())) {
      TLOGDEBUGF_P(PSTR("%sAP Mode enabled: ssid: %s\n"),
          WIFI_STR, softAPSSID.c_str());
    } else {
      TLOGDEBUGF_P(PSTR("%sError starting AP Mode. ssid: %s ip: %s\n"),
          WIFI_STR, softAPSSID.c_str(), apIP.toString().c_str());
    }
    #ifdef ESP32

    #else // ifdef ESP32 now ESP8266

    if (wifi_softap_dhcps_status() != DHCP_STARTED) {
      if (!wifi_softap_dhcps_start()) {
        TLOGDEBUGF_P(PSTR("%sAP Start DHCP failed.\n"), WIFI_STR);

      }
    }
    #endif // ifdef ESP8266
  } else {
    //if (dnsServerActive) {
    //  dnsServerActive = false;
    //  //dnsServer.stop();
    //}
  }
}

void setWiFiMode(WiFiMode_t wifimode) {
  //Serial.println(__FUNCTION__);
  const WiFiMode_t cur_mode = WiFi.getMode();

  if (cur_mode == wifimode) {
    return;
  }

  if (wifimode != WIFI_OFF) {
    #ifdef ESP32 // In ESP32 setHostName must be just before WiFi.mode()
      bool ret = WiFi.setHostname(WiFiGetHostname().c_str());
      TLOGDEBUGF_P(PSTR("%sSetting hostname: %s %s\n"),
            WIFI_STR, WiFiGetHostname().c_str(), helper_general::boolSuccess(ret).c_str());
    #endif

    #ifdef ESP8266
    // See: https://github.com/esp8266/Arduino/issues/6172#issuecomment-500457407
    WiFi.forceSleepWake(); // Make sure WiFi is really active.
    #endif // ifdef ESP8266
    delay(100);
  }

  TLOGDEBUGF_P(PSTR("%sSet WiFi to %s\n"), WIFI_STR, helper_wifi::getWifiModeString(wifimode).c_str());

  int retry = 2;
  while (!WiFi.mode(wifimode) && retry--) {
    TLOGDEBUGF_P(PSTR("%sRetry set mode...\n"), WIFI_STR);
    delay(100);
  }

  if (wifimode == WIFI_OFF) {
    delay(1000);
    #ifdef ESP8266
      WiFi.forceSleepBegin();
      delay(1);
    #endif // ifdef ESP8266
  } else {
    #ifdef ESP8266 // In ESP8266 setHostName must be just after WiFi.mode()
      bool ret = WiFi.hostname(WiFiGetHostname().c_str());
      TLOGDEBUGF_P(PSTR("%sSetting hostname: %s %s\n"),
            WIFI_STR, WiFiGetHostname().c_str(), helper_general::boolSuccess(ret).c_str());
    #endif
    delay(100); // Must allow for some time to init.
  }

  bool new_mode_AP_enabled = helper_wifi::wifiIsAP(wifimode);

  if (helper_wifi::wifiIsAP(cur_mode) && !new_mode_AP_enabled) {
    TLOGDEBUGF_P(PSTR("%sAP mode Disabled.\n"), WIFI_STR);
  }

  if (helper_wifi::wifiIsAP(cur_mode) != new_mode_AP_enabled) {
    // Mode has changed
    setAPinternal(new_mode_AP_enabled);
  }
  #ifdef FEATURE_MDNS
  MDNS.notifyAPChange();
  #endif
}

void setAP(bool enable) {
  WiFiMode_t wifimode = WiFi.getMode();

  switch (wifimode) {
    case WIFI_OFF:

      if (enable) { setWiFiMode(WIFI_AP); }
      break;
    case WIFI_STA:

      if (enable) { setWiFiMode(WIFI_AP_STA); }
      break;
    case WIFI_AP:

      if (!enable) { setWiFiMode(WIFI_OFF); }
      break;
    case WIFI_AP_STA:

      if (!enable) { setWiFiMode(WIFI_STA); }
      break;
    default:
      break;
  }
}


// ********************************************************************************
// Manage Wifi Modes
// ********************************************************************************

/*
 * set mode to Station
 * @param  {bool} enable : true/false
 */
void setSTA(bool enable) {
  switch (WiFi.getMode()) {
    case WIFI_OFF:

      if (enable) { setWiFiMode(WIFI_STA); }
      break;
    case WIFI_STA:

      if (!enable) { setWiFiMode(WIFI_OFF); }
      break;
    case WIFI_AP:

      if (enable) { setWiFiMode(WIFI_AP_STA); }
      break;
    case WIFI_AP_STA:

      if (!enable) { setWiFiMode(WIFI_AP); }
      break;
    default:
      break;
  }
}

// =========================================================
// callback when WiFi status change
// =========================================================

/*
 * Add callback function to be executed when WiFi status changes
 * @param  {TWiFiStatusCallbackF} fn : the function to execute
 */
void onWiFiStatusChange(TWiFiStatusCallbackF fn) {
  _wifi_status_callbacks.push_back(fn);
}

/*
 * Execute the callback functions on the list
 * @param  {conn2_wifi_status_t} status :
 * @param  {char*} parameter            :
 */
void doStatusCallbacks(conn2_wifi_status_t status, char* parameter) {
  for (unsigned char i = 0; i < _wifi_status_callbacks.size(); i++) {
    (_wifi_status_callbacks[i])(status, parameter);
  }
}

/*
 * clear wifi networks saved to networks list
 */
void clearWiFis() {
  for (uint8_t i = 0; i < _network_list.size(); i++) {
    conn2_network_t entry = _network_list[i];
    if (entry.ssid) free(entry.ssid);
    if (entry.pass) free(entry.pass);
    if (entry.enterprise_username) free(entry.enterprise_username);
    if (entry.enterprise_password) free(entry.enterprise_password);
  }
  _network_list.clear();
}

/*
 * reset all saved networks to priority 0. Needs scanning
 * to find again what available wifis match to saved.
 */
void resetSavedNetworks() {
  for (uint8_t i = 0; i < _network_list.size(); i++) {
    _network_list[i].priority = 0;
  }
  _scanNeeded = true;
}

// =============================================
// process functions
// =============================================

/*
 * Connect to wifi.
 * Checks if there is a valid network to connect and try to connect to it.
 */
void doWiFiConnect() {
  int idx = -1;
  if (_idToConnect >= 0) {
    for (uint8_t j = 0; j < _network_list.size(); j++) {
      if (_network_list[j].priority == _idToConnect) {
        idx = j;
        break;
      }
    }
  }

  if (idx >= 0) {

    conn2_network_t entry = _network_list[idx];
    setSTA(true);

    if (!entry.dhcp) {
      WiFi.config(entry.ip, entry.gw, entry.netmask, entry.dns);
    } else {
      // https://github.com/espressif/arduino-esp32/issues/2537#issuecomment-508558849
       WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
    }


    TLOGDEBUGF_P(PSTR("%sTrying wifi connection #%d (%d) to \"%s\"\n"),
        WIFI_STR, _idToConnect, idx, entry.ssid);

    String pass = (entry.pass) ? String(entry.pass) : ""; // needed for printf in case entry.pass is null

    char buffer[128];
    snprintf_P(
        buffer, sizeof(buffer),
        PSTR("%sTry #%d.%d Connecting to BSSID: %02X:%02X:%02X:%02X:%02X:%02X "
             "CH: %02d, RSSI: %3d, SEC: %s, SSID: %s, PASS: %s.\n"),
        WIFI_STR, _wifiRetryConnectionCount, entry.priority, entry.bssid[0],
        entry.bssid[1], entry.bssid[2], entry.bssid[3], entry.bssid[4],
        entry.bssid[5], entry.channel, entry.rssi,
        helper_wifi::getEncryptionTypeStr(entry.security).c_str(), entry.ssid, pass.c_str());

    doStatusCallbacks(STATUS_CONNECTING, buffer);
#ifdef CONN2_WIFI_ENABLE_ENTERPRISE
    /*
    Here is the work flow that prepares ESP266 station to connect to
    WPA2_Enterprise AP. • Call wifi_station_set_config to set the configuration
    of target AP. • Call wifi_station_set_wpa2_enterprise_auth(1); to enable
    WPA2_Enterprise authentication.
    - For EAP-TLS authentication, call wifi_station_set_enterprise_cert_key to
    set certificate and private key. wifi_station_set_enterprise_username is an
    optional choice, it can be called to set user name.
    - For EAP-TTLS or EAP-PEAP authentication, call
    wifi_station_set_enterprise_username and
    wifi_station_set_enterprise_password to set user name and password.
    wifi_station_set_enterprise_ca_cert is an optional choice, it can be called
    to set root certificate. • Call wifi_station_connect to connect to target
    AP. • After being connected to an AP, or failing to connect to AP and on
    stopped retries, please call the corresponding
    wifi_station_clear_enterprise_XXX APIs to release the resources.
    */
    if (entry.enterprise_username && entry.enterprise_password) {
      // Create config
      struct station_config wifi_config;
      memset(&wifi_config, 0, sizeof(wifi_config));
      strcpy((char*)wifi_config.ssid, entry.ssid);
      wifi_config.bssid_set = 0;
      *wifi_config.password = 0;

      // Set some defaults
      wifi_set_opmode(STATION_MODE);
      wifi_station_set_config_current(
          &wifi_config);  // wifi_station_set_config(&wifi_config);
      wifi_station_set_enterprise_disable_time_check(1);
      wifi_station_clear_cert_key();
      wifi_station_clear_enterprise_ca_cert();
      wifi_station_set_wpa2_enterprise_auth(1);

      // Set user/pass
      wifi_station_set_enterprise_identity((uint8*)entry.enterprise_username,
                                           strlen(entry.enterprise_username));
      wifi_station_set_enterprise_username((uint8*)entry.enterprise_username,
                                           strlen(entry.enterprise_username));
      wifi_station_set_enterprise_password((uint8*)entry.enterprise_password,
                                           strlen(entry.enterprise_password));

      // Connect, free resources after
      wifi_station_connect();
      wifi_station_clear_enterprise_identity();
      wifi_station_clear_enterprise_username();
      wifi_station_clear_enterprise_password();
      wifi_station_clear_cert_key();
      wifi_station_clear_enterprise_ca_cert();
    } else
#endif

    if (entry.channel == 0) {
      WiFi.begin(entry.ssid, entry.pass);
    } else {
      WiFi.begin(entry.ssid, entry.pass, entry.channel, entry.bssid);
    }
    _doWiFiConnectCalled = true;
    // todo start timeout timer
  } else {
    char buffer[80];
    snprintf_P(buffer, sizeof(buffer), PSTR("%sNo network to connect. (%d)\n"),
               WIFI_STR, _idToConnect);

    doStatusCallbacks(STATUS_NO_KNOWN_NETWORKS, buffer);
  }
}

/*
 * When WiFi scan is completed call this function to
 * process found networks.
 */
void processWiFiScan() {

  _validNetworks = 0;
  int ix[_scanNetworksFound];
  for (int i = 0; i < _scanNetworksFound; i++) ix[i] = i;

  // sort by RSSI (signal strength)
  for (int i = 0; i < _scanNetworksFound; i++)
    for (int j = i + 1; j < _scanNetworksFound; j++)
      if (WiFi.RSSI(ix[j]) > WiFi.RSSI(ix[i])) std::swap(ix[i], ix[j]);

  // remove duplicates ( must be RSSI sorted )
  if (wifiScanRemoveDuplicates) {
    String cssid;
    for (int i = 0; i < _scanNetworksFound; i++) {
      if (ix[i] == -1) continue;
      cssid = WiFi.SSID(ix[i]);
      for (int j = i + 1; j < _scanNetworksFound; j++) {
        if (cssid == WiFi.SSID(ix[j])) {
          Serial.println("DUP AP: " + WiFi.SSID(ix[j])); // debug (delete later)
          ix[j] = -1;  // set dup aps to index -1
        }
      }
    }
  }

  int _priority = 1;

  _wifisJson = "[";
  bool first = true;
  for (int i = 0; i < _scanNetworksFound; i++) {
    if (ix[i] == -1) continue;
    if (!first) _wifisJson += ",";
    _wifisJson += "{";
    _wifisJson += "\"ssid\":\""   + WiFi.SSID(ix[i])          + "\"";
    _wifisJson += ",\"rssi\":"    + String(WiFi.RSSI(ix[i]));
    _wifisJson += ",\"bssid\":\"" + WiFi.BSSIDstr(ix[i])      + "\"";
    _wifisJson += ",\"channel\":" + String(WiFi.channel(ix[i]));
    _wifisJson += ",\"secure\":"  + String(WiFi.encryptionType(ix[i]));
#ifdef ESP8266
    _wifisJson +=
        ",\"hidden\":" + String(WiFi.isHidden(ix[i]) ? "true" : "false");
#endif
    _wifisJson += "}";
    first = false;
    char buffer[128];
    snprintf_P(buffer, sizeof(buffer),
               PSTR("%s[%s] Ch: %02d RSSI: %3d Sec: %15s %s SSID: %s"),
               WIFI_STR, WiFi.BSSIDstr(ix[i]).c_str(),
               WiFi.channel(ix[i]), WiFi.RSSI(ix[i]),
               helper_wifi::getEncryptionTypeStr( WiFi.encryptionType(ix[i]) ).c_str(),
#ifdef ESP8266
               _isHiddenStr(WiFi.isHidden(ix[i])).c_str(),
#else
               String().c_str(),  // ESP32 does not have isHidden property
#endif
               WiFi.SSID(ix[i]).c_str() );
    LOGINFOLN(buffer);   // do not print time with message
    doStatusCallbacks(STATUS_FOUND_NETWORK, buffer);

    // scan the saved networks for valid to connect
    if (_connectAfterScan) {
      for (uint8_t j = 0; j < _network_list.size(); j++) {
        conn2_network_t* entry = &_network_list[j];

        if (WiFi.SSID(ix[i]).equals(entry->ssid) && (entry->priority == 0)) {
// Check security
#ifdef ESP8266
          if ((WiFi.encryptionType(ix[i]) != ENC_TYPE_NONE) &&
              (entry->pass == NULL))
            continue;
#else
          if ((WiFi.encryptionType(ix[i]) != WIFI_AUTH_OPEN) &&
              (entry->pass == NULL))
            continue;
#endif
          entry->rssi     = WiFi.RSSI(ix[i]);
          entry->security = WiFi.encryptionType(ix[i]);
          entry->channel  = WiFi.channel(ix[i]);
          entry->priority = _priority;
          memcpy((void*)&entry->bssid, (void*)WiFi.BSSID(ix[i]),
                 sizeof(entry->bssid));
          _validNetworks++;
          char buffer[80];
          snprintf_P(buffer, sizeof(buffer),
                     PSTR("\t↑ Connection try #%d.\n"), _priority);
          LOGINFOF_P(buffer);
          //doStatusCallbacks(STATUS_FOUND_NETWORK, buffer);
          _priority++;
        }
      }
    }
    //doStatusCallbacks(STATUS_FOUND_NETWORK, (char*)"\n");
  }
  _wifisJson += "]";
  char buffer[120];
  snprintf_P(buffer, sizeof(buffer), PSTR("%sScan processed.\n"), WIFI_STR);
  doStatusCallbacks(STATUS_SCAN_PROCESSED, buffer);
  if (_validNetworks > 0) {
    // now we are ready to connect to any of the found networks.
    // todo: if already connected, check if connected wifi has less rssi to a
    // todo: found one and if so, connect to the stronger one
    if (_connectAfterScan) {
      _wifiRetryConnectionCount++;
      _idToConnect = 1;
      // add connect function to tasks queue
      addToTaskQueue(doWiFiConnect);
    }
  } else {
    char buffer[80];
    snprintf_P(buffer, sizeof(buffer), PSTR("%sNo known networks found.\n"),
               WIFI_STR);
    doStatusCallbacks(STATUS_NO_KNOWN_NETWORKS, buffer);
  }
}


// =============================================
// safe functions to process events
// =============================================

/*
 * Callback function called when scan for networks completes.
 */
void onWiFiScanComplete() {
  char buffer[80];
  snprintf_P(buffer, sizeof(buffer),
             PSTR("%sScan Complete. %d network(s) found.\n"), WIFI_STR,
             _scanNetworksFound);

  _status = STATUS_SCAN_COMPLETE;
  doStatusCallbacks(STATUS_SCAN_COMPLETE, buffer);
  processWiFiScan();
}

void onWiFiConnect() {
  // do something here
  char buffer[128];
  _status = STATUS_CONNECTED;
  snprintf_P(buffer, sizeof(buffer),
              PSTR("%sConnected to: %s. Next step get IP.\n"), WIFI_STR,
              wifiConnecting.c_str());
  doStatusCallbacks(STATUS_CONNECTED, buffer);
}

void onWiFiGotIP(){
  _status = STATUS_GOT_IP;
  char buffer[128];
  snprintf_P(buffer, sizeof(buffer),
             PSTR("%sWiFi connected. Got IP: %s HostName: %s\n"), WIFI_STR,
             CSTR(WiFi.localIP().toString()), CSTR(WiFiGetHostname()));
  doStatusCallbacks(STATUS_GOT_IP, buffer);
  // do something here
}

void onWiFiDisconnect() {
  conn2_wifi_status_t previousStatus = _status;
  _status = STATUS_DISCONNECTED;
  char buffer[128];
  if (_forcedDisconnect) {
    _forcedDisconnect = false;
    snprintf_P(buffer, sizeof(buffer), PSTR("%sWiFi Disconnected (Forced).\n"),
               WIFI_STR);
    doStatusCallbacks(STATUS_FORCED_DISCONNECTION, buffer);
    return;
  }

  snprintf_P(buffer, sizeof(buffer), PSTR("%sWiFi Disconnected. Reason: %s\n"),
             WIFI_STR, CSTR(getLastDisconnectReason(_lastDisconnectReason)));
  doStatusCallbacks(STATUS_DISCONNECTED, buffer);

  // if last status was connected, set _retryConnectionCount = 0 and start
  // connecting from the first network.
  // fail only if are continuous disconnects.
  if (previousStatus == STATUS_GOT_IP) {
    _wifiRetryConnectionCount = 1;
    _idToConnect = 1;
    addToTaskQueue(doWiFiConnect);

  } else if (_wifiRetryConnectionCount <= wifiConnectionRetries) {
    if (_idToConnect < _validNetworks) {
      ++_idToConnect;
      addToTaskQueue(doWiFiConnect);
    } else {
      if (_wifiRetryConnectionCount < wifiConnectionRetries) {
        resetSavedNetworks(); // reset saved networks. Rescan is needed.

        addToTaskQueue(wifiConnect);
      } else {
        // no more networks to connect.
        // Exit to captive portal
        snprintf_P(buffer, sizeof(buffer),
                   PSTR("%sNo more networks to connect. Exit.\n"), WIFI_STR);
        doStatusCallbacks(STATUS_EXIT_TO_CAPTIVE_PORTAL, buffer);
      }
    }
  }
}

// =============================================
// called by WiFi events
// =============================================

void onWiFiScanCompleteEvent(int networksFound) {
  _scanNetworksFound = networksFound;
  addToTaskQueue(onWiFiScanComplete);
}

void onWiFiConnectEvent() {
  addToTaskQueue(onWiFiConnect);
}

void onWiFiGotIPEvent() {
  addToTaskQueue(onWiFiGotIP);
}

void onWiFiDisconnectEvent() {
  addToTaskQueue(onWiFiDisconnect);
}

#ifdef ESP8266

void onWiFiScanCompleteEventESP8266(int networksFound) {
  onWiFiScanCompleteEvent(networksFound);
}

void onWiFiConnectEventESP8266(const WiFiEventStationModeConnected& event) {
  wifiConnecting = String(event.ssid);
  TLOGDEBUGF_P(PSTR("%sESP8266 Connected.\n"), WIFI_STR);
  onWiFiConnectEvent();
}

void onWiFiGotIPEventESP8266(const WiFiEventStationModeGotIP& event) {
  TLOGDEBUGF_P(PSTR("%sESP8266 Got IP.\n"), WIFI_STR);
  onWiFiGotIPEvent();
}

void onWiFiDisconnectEventESP8266(const WiFiEventStationModeDisconnected& event) {
  if (_doWiFiConnectCalled) {
    _doWiFiConnectCalled = false;
    _lastDisconnectReason = event.reason;
    onWiFiDisconnectEvent();
  }
}

void onWiFiDHCPTimeoutEvent()    {}
void onConnectedAPmodeEvent()    {}
void onDisconnectedAPmodeEvent() {}

#endif // ESP8266

#ifdef ESP32

// * ESP32 WiFi callback events
void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
  TLOGDEBUGF_P(PSTR("EVENT: %d\n"), event);
  switch (event) {
    case SYSTEM_EVENT_STA_CONNECTED: {
      onWiFiConnectEvent();
      break;
    }
    case SYSTEM_EVENT_STA_DISCONNECTED: {
      // ignore disconnect events if no WiFi.begin(...) has called
      if (_doWiFiConnectCalled) {
        _doWiFiConnectCalled = false;
        _lastDisconnectReason =
          static_cast<WiFiDisconnectReason>(info.wifi_sta_disconnected.reason);
        onWiFiDisconnectEvent();
      }
      break;
    }

    case SYSTEM_EVENT_STA_LOST_IP:
    break;
    case SYSTEM_EVENT_STA_GOT_IP: {
      onWiFiGotIPEvent();
    } break;

    case SYSTEM_EVENT_AP_STACONNECTED:
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      break;
    case SYSTEM_EVENT_SCAN_DONE:
      onWiFiScanCompleteEvent(WiFi.scanComplete());
      break;
    default:
      break;
  }
}

#endif  // ifdef ESP32



void wifiSetup() {
  Serial.println(__FUNCTION__);
  WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
  WiFi.disconnect(true);    // Delete SDK wifi config
  delay(200);
  setWiFiMode(WIFI_STA);


  if (!WiFi.getAutoConnect()) { WiFi.setAutoConnect(true); }

  // The WiFi.disconnect() ensures that the WiFi is working correctly. If this is not done before receiving WiFi connections,
  // those WiFi connections will take a long time to make or sometimes will not work at all.
//  WiFi.disconnect();
//  setWiFiMode(WIFI_OFF);

  TLOGDEBUGF_P(PSTR("%sSetup WiFi events.\n"), WIFI_STR);

#if defined(ESP32)
  WiFi.onEvent(onWiFiEvent);
#else
  // WiFi event handlers
  onStationModeConnectedHandler =
      WiFi.onStationModeConnected(onWiFiConnectEventESP8266);
	onStationModeDisconnectedHandler =
      WiFi.onStationModeDisconnected(onWiFiDisconnectEventESP8266);
	onStationModeGotIPHandler =
      WiFi.onStationModeGotIP(onWiFiGotIPEventESP8266);
  //stationModeDHCPTimeoutHandler    =
  //WiFi.onStationModeDHCPTimeout(onWiFiDHCPTimeoutEventESP8266);
  //APModeStationConnectedHandler    =
  //WiFi.onSoftAPModeStationConnected(onConnectedAPmodeEventESP8266);
  //APModeStationDisconnectedHandler =
  //WiFi.onSoftAPModeStationDisconnected(onDisconnectedAPmodeEventESP8266);
#endif
}

// =========================================================
// add wifi credentials to networks list for wifi connection
// =========================================================

/*
 * Add WiFi ssid to connection list.
 */
bool addWiFi(const char* ssid, const char* pass,
                                 const char* ip, const char* netmask,
                                 const char* gw, const char* dns,
                                 const char* enterprise_username,
                                 const char* enterprise_password) {
  conn2_network_t new_network;

  // Check SSID too long or missing
  if (!ssid || *ssid == 0x00 || strlen(ssid) > 31) {
    return false;
  }

  // Check PASS too long
  if (pass && strlen(pass) > 63) {
    return false;
  }

  // Copy network SSID
  new_network.ssid = strdup(ssid);
  if (!new_network.ssid) {
    return false;
  }

  // Copy network PASS
  if (pass && *pass != 0x00) {
    new_network.pass = strdup(pass);
    if (!new_network.pass) {
      free(new_network.ssid);
      return false;
    }
  } else {
    new_network.pass = NULL;
  }

  // Copy static config
  new_network.dhcp = true;
  if (ip && gw && netmask && *ip != 0x00 && *gw != 0x00 && *netmask != 0x00) {
    new_network.dhcp = false;
    new_network.ip.fromString(ip);
    new_network.gw.fromString(gw);
    new_network.netmask.fromString(netmask);
  }
  if (dns && *dns != 0x00) {
    new_network.dns.fromString(dns);
  }
  if (enterprise_username && enterprise_password &&
      *enterprise_username != 0x00 && *enterprise_password != 0x00) {
    new_network.enterprise_username = strdup(enterprise_username);
    new_network.enterprise_password = strdup(enterprise_password);
  }

  // Defaults
  new_network.rssi     = 0;
  new_network.security = 0;
  new_network.channel  = 0;
  new_network.priority = 0;

  _network_list.push_back(new_network);
  return true;
}

/*
 * Scan for WiFi networks asynchronously
 * @return {bool}  : true if success, false if scan is already in process.
 */
bool scanNetworks() {
  if (WiFi.scanComplete() == -1) return false; //Scanning still in progress
  bool show_hidden = true;

#ifdef ESP32
  WiFi.scanNetworks(true, show_hidden);
#else
  WiFi.scanNetworksAsync(onWiFiScanCompleteEventESP8266, show_hidden);
#endif

  _status   = STATUS_SCANNING;

  char buffer[120];
  snprintf_P(buffer, sizeof(buffer), PSTR("%sStart networks scanning.\n"),
             WIFI_STR);
  doStatusCallbacks(STATUS_SCANNING, buffer);

  return true;
}

void wifiConnect() {
  if (_scanNeeded) {
    _scanNeeded = false;
    _connectAfterScan = true;
    scanNetworks();
  } else {
    doWiFiConnect();
  };
}

void wifiDisconnect() {
  _forcedDisconnect = true;
  WiFi.disconnect();
}


