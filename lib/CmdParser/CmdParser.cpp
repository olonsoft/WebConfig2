#include <ArduinoJson.h>
#include <CmdParser.h>
#include <Settings.h>
#include <helper.h>

#define PARSER_STR "[CMD] "
#define PARSER_RESPONSE "Response"

#define CMD_LOGIN_TIMEOUT  120              // seconds auto logout after last command.

#define CMD                "cmd"
#define CMD_LOGIN          "login"           // {"login":"password"} -> needed for set commands
#define CMD_LOGOUT         "logout"          // {"cmd":"logout"}
#define CMD_SET            "set"             // {"cmd":"set","anysetting":"anyvalue"}
#define CMD_STATUS         "status"          // {"cmd":"status"} 
#define CMD_SYSTEM         "system"          // {"cmd":"system"} 
#define CMD_SCANWIFI       "scanwifi"        // {"cmd":"scanwifi"} -> scans wifi nwtworks
#define CMD_NETWORKS       "networks"        // {"cmd":"networks"} -> show saved networks
#define CMD_DELETENETWORKS "deletenetworks"  // {"cmd":"deletenetworks"} -> delete all saved networks.
#define CMD_ADDWIFI        "addwifi"         // {"cmd":"addwifi","ssid":"wifiname","password":"12345678","bssid":"12:13:14:15:16"}
#define CMD_DELETEWIFI     "deletewifi"      // {"cmd":"deletewifi","ssid":"value"} -> deletes wifi from networks file
#define CMD_SAVESETTINGS   "savesettings"    // {"cmd":"savesettings"} -> saves new_settings to file
#define CMD_DELETESETTINGS "deletesettings" 
#define CMD_SETTINGS_CUR   "settings"        // {"cmd":"settings"} -> show settings json
#define CMD_SETTINGS_NEW   "settings2"       // {"cmd":"settings2"} -> show modified settings json
#define CMD_RESET          "reset"           // {"cmd":"reset"} -> deletes settings configuration file
#define CMD_REBOOT         "reboot"          // {"cmd":"reboot"}
#define CMD_UPDATE         "update"          // {"cmd":"update"} -> will check for firmware update 
#define CMD_FORMATFS       "formatfs"        // {"cmd":"formatfs"} 

bool loggedIn = false;
uint32_t lastLoginTime = 0;

mqtt_command_t decodeCommand(char *json, app_settings_t &settings) {

  if (loggedIn && millis()-lastLoginTime > CMD_LOGIN_TIMEOUT * 1000UL) {
    loggedIn = false;
  }

  const size_t capacity = ESP.getFreeHeap() - 3096;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, json);
  char buffer[120];
  if (error) {    
    snprintf_P(buffer, sizeof(buffer), 
               PSTR("%sParse command failed. Error: %s\n"),
               PARSER_STR, CSTR(error));                      
    TLOGDEBUG(buffer);
    return cmd_error;
  }

  if (doc[CMD_LOGIN] && !strcmp(doc[CMD_LOGIN], settings.adminPass)) {
    loggedIn = true;
    lastLoginTime = millis();
    return logged_in;
  }

  if (!strcmp(doc[CMD] , CMD_LOGOUT)) {    
    loggedIn = false;
    lastLoginTime = 0;
    return logged_out;
  }

  if (doc[CMD] == nullptr) { return cmd_error; }
  // if new message is received while logged in,
  // update the timer.
  lastLoginTime = millis(); 

  // first check for commands that do not need login
  if (!strcmp(doc[CMD] , CMD_STATUS))       { return system_status; }  
  if (!strcmp(doc[CMD] , CMD_SYSTEM))       { return system_info; }  

  if (!loggedIn) {
    return no_admin_rights;
  }

  if (!strcmp(doc[CMD] , CMD_SCANWIFI))     { return scan_wifi; };
  if (!strcmp(doc[CMD] , CMD_NETWORKS))     { return saved_networks; }
  if (!strcmp(doc[CMD] , CMD_DELETENETWORKS)) { return delete_networks; }
  if (!strcmp(doc[CMD] , CMD_ADDWIFI))      { return add_wifi; }
  if (!strcmp(doc[CMD] , CMD_DELETEWIFI))   { return delete_wifi; }
  if (!strcmp(doc[CMD] , CMD_SAVESETTINGS)) { return save_settings; }
  if (!strcmp(doc[CMD] , CMD_DELETESETTINGS)) { return delete_settings; }  
  if (!strcmp(doc[CMD] , CMD_SETTINGS_CUR)) { return settings_cur; } 
  if (!strcmp(doc[CMD] , CMD_SETTINGS_NEW)) { return settings_new; }
  if (!strcmp(doc[CMD] , CMD_RESET))        { return reset; }
  if (!strcmp(doc[CMD] , CMD_REBOOT))       { return reboot; }
  if (!strcmp(doc[CMD] , CMD_UPDATE))       { return fota_update; }
  if (!strcmp(doc[CMD] , CMD_FORMATFS))     { return format_fs; }  
  if (!strcmp(doc[CMD] , CMD_SET)) {
    String jsonstr;
    serializeJson(doc, jsonstr);
    json2Settings(jsonstr, settings);
    return settings_modified;
  }

  snprintf_P(buffer, sizeof(buffer), 
               PSTR("%sUnknown command!\n"),
               PARSER_STR);  
  TLOGDEBUG(buffer);
  return cmd_error;
}
