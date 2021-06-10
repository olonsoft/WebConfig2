#include "../../src/defaults.h"
#include <ArduinoJson.h>
#include <helper_general.h>
#include <helper_debug.h>
#include <Settings.h>

#define C_DEVICENAME   "devicename"
#define C_ADMINPASS    "adminpass"

#define C_SSID         "ssid"
#define C_PASSWORD     "pass"
#define C_DHCP         "dhcp"
#define C_IP           "ip"
#define C_NETMASK      "netmask"
#define C_GATEWAY      "gateway"
#define C_DNS          "dns"

#define C_BROKER1      "broker1"
#define C_MQTT_PORT1   "mqttport1"
#define C_MQTT_USER1   "mqttuser1"
#define C_MQTT_PASS1   "mqttpass1"

#define C_BROKER2      "broker2"
#define C_MQTT_PORT2   "mqttPort2"
#define C_MQTT_USER2   "mqttUser2"
#define C_MQTT_PASS2   "mqttPass2"

#define C_MQTT_G_TOPIC "mqttglobaltopic"
#define C_STATUS_INT   "statusupdateinterval"
#define C_NTP_SERVER   "ntpserver"
#define C_TIMEZONE     "timezone"
#define C_FOTA_URL     "FOTAconfurl"
#define C_FOTA_U_INT   "FOTAcheckinterval"

app_settings_t current_settings = {
  // * common settings
  DEFAULT_APP_NAME,      // deviceName
  CAPTIVE_PORTAL_PSW,    // adminPass must be >= 8chars
  DEFAULT_SSID,          // ssid
  DEFAULT_WIFI_PSW,      // pass
  DEFAULT_USE_DHCP,      // dhcp
  DEFAULT_IP,            // ip
  DEFAULT_SUBNET,        // netmask
  DEFAULT_GW,            // gateway
  DEFAULT_DNS,           // dns

  DEFAULT_MQTT_SERVER1,   // broker
  DEFAULT_MQTT_PORT1,     // mqttport
  DEFAULT_MQTT_USER1,     // mqttUser
  DEFAULT_MQTT_PASSWORD1, // mqttPass
  
  DEFAULT_MQTT_SERVER2,   // backup broker
  DEFAULT_MQTT_PORT2,     // backup mqttport
  DEFAULT_MQTT_USER2,     // backup mqttUser
  DEFAULT_MQTT_PASSWORD2, // backup mqttPass
  
  DEFAULT_MQTT_TOPIC,     // mqtt globalTopic
  MQTT_STATUS_UPDATE_INTERVAL_MINUTES,             
  DEFAULT_NTP_HOST,       // ntpServer
  DEFAULT_TIME_ZONE,      // timeZone
  FOTA_URL,               // FOTA url
  FOTA_CHECK_INTERVAL_MINUTES                     
  // * below, set application specific settings
};

app_settings_t new_settings;

bool formatFS() {
  ESP_FS.end();
  ESP_FS.format();
  return helper_general::beginFileSystem();
}

void copySettings(app_settings_t settingsFrom, app_settings_t &settingsTo) {
  String json= settings2Json(settingsFrom);
  json2Settings(json, settingsTo);
}

bool loadSettings(app_settings_t &settings){
  if (!helper_general::beginFileSystem()) return false;
  
  if (!ESP_FS.exists(SETTINGS_FILE)) {
    LOGWARNF_P(PSTR("%sConfig file does not exist. Creating default. [%s]\n"),
                  SETTINGS_STR, SETTINGS_FILE);
    return (saveSettings(settings));      
  }

  File f = ESP_FS.open(SETTINGS_FILE, "r");
  if (!f) {
    LOGERRORF_P(PSTR("%sFailed to open config file [%s].\n"),
                  SETTINGS_STR, SETTINGS_FILE);
    f.close();
    return false;
  }

  size_t size = f.size();
  if (size > JSON_SIZE) {
    LOGERRORF_P(PSTR("%sConfig file size is too large.\n"), SETTINGS_STR); 
    return false;
  }

  String json = "";
  while (f.available()){
          json += char(f.read());
  }
  
  f.close();

  return (json2Settings(json, settings));
}

bool saveSettings(app_settings_t settings) {
  if (!helper_general::beginFileSystem()) return false;
  
  File f = ESP_FS.open(SETTINGS_FILE, "w");
  if (!f) {
    LOGERRORF_P(PSTR("%sFailed to open config file for writing. [%s].\n"),
                  SETTINGS_STR, SETTINGS_FILE);
    f.close();
    return false;
  }

  int bytesWritten = f.print(settings2Json(settings));
  f.flush();
  f.close();
  return (bytesWritten > 0);
}

bool json2Settings(String json, app_settings_t &settings) {

  // const size_t capacity = ESP.getFreeHeap() - 3096;
  DynamicJsonDocument doc(JSON_SIZE);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    LOGERRORF_P(PSTR("%sFailed to parse json. Error: %s\n"),
                  SETTINGS_STR, CSTR(error));
    return false;
  }  
    // todo: check if values are null before copying    
  strlcpy(settings.deviceName,  doc[C_DEVICENAME] | settings.deviceName, sizeof(settings.deviceName));
  strlcpy(settings.adminPass,   doc[C_ADMINPASS]  | settings.adminPass,  sizeof(settings.adminPass));

  strlcpy(settings.ssid,        doc[C_SSID]       | settings.ssid,       sizeof(settings.ssid));
  strlcpy(settings.pass,        doc[C_PASSWORD]   | settings.pass,       sizeof(settings.pass));
          settings.dhcp =       doc[C_DHCP]       | settings.dhcp;
  strlcpy(settings.ip,          doc[C_IP]         | settings.ip,         sizeof(settings.ip));
  strlcpy(settings.netmask,     doc[C_NETMASK]    | settings.netmask,    sizeof(settings.netmask));
  strlcpy(settings.gateway,     doc[C_GATEWAY]    | settings.gateway,    sizeof(settings.gateway));
  strlcpy(settings.dns,         doc[C_DNS]        | settings.dns,        sizeof(settings.dns));

  strlcpy(settings.broker1,     doc[C_BROKER1]     | settings.broker1,     sizeof(settings.broker1));
          settings.mqttPort1 =  doc[C_MQTT_PORT1]  | settings.mqttPort1;
  strlcpy(settings.mqttUser1,   doc[C_MQTT_USER1]  | settings.mqttUser1,   sizeof(settings.mqttUser1));
  strlcpy(settings.mqttPass1,   doc[C_MQTT_PASS1]  | settings.mqttPass1,   sizeof(settings.mqttPass1));
  
  strlcpy(settings.broker2,     doc[C_BROKER2]    | settings.broker2,    sizeof(settings.broker2));
          settings.mqttPort2 =  doc[C_MQTT_PORT2] | settings.mqttPort2;
  strlcpy(settings.mqttUser2,   doc[C_MQTT_USER2] | settings.mqttUser2,  sizeof(settings.mqttUser2));
  strlcpy(settings.mqttPass2,   doc[C_MQTT_PASS2] | settings.mqttPass2,  sizeof(settings.mqttPass2));
  
  strlcpy(settings.mqttGlobalTopic, doc[C_MQTT_G_TOPIC] | settings.mqttGlobalTopic, sizeof(settings.mqttGlobalTopic));
  
  settings.statusUpdateInterval = doc[C_STATUS_INT]   | settings.statusUpdateInterval;

  strlcpy(settings.ntpServer,     doc[C_NTP_SERVER]   | settings.ntpServer, sizeof(settings.ntpServer));
  strlcpy(settings.timeZone,      doc[C_TIMEZONE]     | settings.timeZone, sizeof(settings.timeZone));

  strlcpy(settings.firmwareConfURL,      doc[C_FOTA_URL]   | settings.firmwareConfURL, sizeof(settings.firmwareConfURL));
  settings.firmwareUpdateCheckInterval = doc[C_FOTA_U_INT] | settings.firmwareUpdateCheckInterval;
  
  /* application specific settings
  settings.baudrate = doc[F("baudrate")] | settings.baudrate;
  settings.dataInterval = doc[F("datainterval")] | settings.dataInterval;
  */
 return true;
}

String settings2Json(app_settings_t settings) {
  DynamicJsonDocument doc(JSON_SIZE);
  doc[C_DEVICENAME]   = settings.deviceName;
  doc[C_ADMINPASS]    = settings.adminPass;
  doc[C_SSID]         = settings.ssid;
  doc[C_PASSWORD]     = settings.pass;
  doc[C_DHCP]         = settings.dhcp;
  doc[C_IP]           = settings.ip;
  doc[C_NETMASK]      = settings.netmask;
  doc[C_GATEWAY]      = settings.gateway;
  doc[C_DNS]          = settings.dns;

  doc[C_BROKER1]      = settings.broker1;
  doc[C_MQTT_PORT1]   = settings.mqttPort1;
  doc[C_MQTT_USER1]   = settings.mqttUser1;
  doc[C_MQTT_PASS1]   = settings.mqttPass1;

  doc[C_BROKER2]      = settings.broker2;
  doc[C_MQTT_PORT2]   = settings.mqttPort2;
  doc[C_MQTT_USER2]   = settings.mqttUser2;
  doc[C_MQTT_PASS2]   = settings.mqttPass2;

  doc[C_MQTT_G_TOPIC] = settings.mqttGlobalTopic;

  doc[C_STATUS_INT]   = settings.statusUpdateInterval;
  doc[C_NTP_SERVER]   = settings.ntpServer;
  doc[C_TIMEZONE]     = settings.timeZone;
  doc[C_FOTA_URL]     = settings.firmwareConfURL;
  doc[C_FOTA_U_INT]   = settings.firmwareUpdateCheckInterval;
    
  /* application specific settings
  doc[F("baudrate")] = settings.baudrate;
  doc[F("datainterval")] = settings.dataInterval;
  */
    
  //serializeJsonPretty(doc, Serial);
  String s;
  serializeJson(doc, s);
  return s;
}


bool deleteSettingsFile() {
  if (!helper_general::beginFileSystem()) return false;
  if (ESP_FS.exists(SETTINGS_FILE)) {
    LOGINFOF_P(PSTR("%sDeleting config file [%s]...."),
                  SETTINGS_STR, SETTINGS_FILE);        
    if (ESP_FS.remove(SETTINGS_FILE)) {
      LOGINFOF_P(PSTR(SUCCESS_STR "\n"));
      return true;
    } else {
      LOGERRORF_P(PSTR(FAIL_STR "\n"));
      return false;
    }
  }
  return false;          
}

String loadNetworksConfigJson(app_settings_t settings) {
  if (!helper_general::beginFileSystem()) return "[]";
  
  if (!ESP_FS.exists(NETWORKS_FILE)) {
    LOGWARNF_P(PSTR("%sNetworks file does not exist. Creating default. [%s]\n"),
                  SETTINGS_STR, NETWORKS_FILE);
    // write default wifi settings to file
    String s = "[{" + 
               helper_general::jsonPair(C_SSID,     String(settings.ssid)) + "," +               
               helper_general::jsonPair(C_PASSWORD, String(settings.pass)) + "," +
               helper_general::quotedText(C_DHCP) + ":" + helper_general::boolToString(settings.dhcp) + "," +               
               helper_general::jsonPair(C_IP,       String(settings.ip)) + "," +
               helper_general::jsonPair(C_NETMASK,  String(settings.netmask)) + "," +
               helper_general::jsonPair(C_GATEWAY,  String(settings.gateway)) + "," +
               helper_general::jsonPair(C_DNS,      String(settings.dns)) +
               "}]";
    TLOGDEBUG("CREATE new json -->"); LOGDEBUGLN(s);               
    saveNetworksConfigJson(s);
    return s;
  }

  File f = ESP_FS.open(NETWORKS_FILE, "r");
  if (!f) {
    LOGERRORF_P(PSTR("%sFailed to open networks file for reading [%s].\n"),
                  SETTINGS_STR, NETWORKS_FILE);
    f.close();
    return "[]";
  }

  String json = "";
  while (f.available()){
          json += char(f.read());
  }
  
  f.close();
  return json;
}

bool saveNetworksConfigJson(String json) {
  if (!helper_general::beginFileSystem()) return false;
  
  File f = ESP_FS.open(NETWORKS_FILE, "w");
  if (!f) {
    LOGERRORF_P(PSTR("%sFailed to open networks file for writing [%s].\n"),
                  SETTINGS_STR, NETWORKS_FILE);
    f.close();
    return false;
  }

  int bytesWritten = f.print(json);
  f.flush();
  f.close();
  
  return (bytesWritten > 0);
}

bool loadNetworks(TAddNetworkCallback cb) {
  if (cb) {
    String json = loadNetworksConfigJson(current_settings);   
    //const size_t capacity = ESP.getFreeHeap() - 3096;
    DynamicJsonDocument doc(JSON_SIZE);
    // Parse the JSON input
    DeserializationError err = deserializeJson(doc, json);
    if (err) {
      TLOGERRORF_P(PSTR("%sNetworks deserialization error: [%s]. Free Heap:%d json: %s\n"),
                  SETTINGS_STR, err.c_str(), ESP.getFreeHeap(), json.c_str());
      return false;
    }   
    JsonArray wifi = doc.as<JsonArray>();
    for (JsonObject wf : wifi) {
      TLOGDEBUGF_P(PSTR("%sLoad networks json: "), SETTINGS_STR);
      serializeJson(wf, Serial);
      LOGLN;
      cb(wf[C_SSID], wf[C_PASSWORD], wf[C_DHCP],
         wf[C_IP], wf[C_NETMASK], wf[C_GATEWAY], wf[C_DNS] );    
    }
    return true;
  } else {
    return false;
  }
}

bool deleteNetworksConfigJson() {
  if (!helper_general::beginFileSystem()) return false;
  if (ESP_FS.exists(NETWORKS_FILE)) {
    LOGINFOF_P(PSTR("%sDeleting networks file [%s]...."),
                  SETTINGS_STR, NETWORKS_FILE);        
    if (ESP_FS.remove(NETWORKS_FILE)) {
      LOGINFOF_P(PSTR(SUCCESS_STR "\n"));
      return true;
    } else {
      LOGERRORF_P(PSTR(FAIL_STR "\n"));
      return false;
    }
  }
  return false;
}