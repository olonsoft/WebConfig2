#pragma once

#include <arduino.h>
#include <functional>

typedef struct
{   // common settings
    char deviceName[32];    
    char adminPass[64];

    char ssid[32];
    char pass[64];
    bool dhcp;
    char ip[16];
    char netmask[16];
    char gateway[16];
    char dns[16];
    
    char broker1[80];
    uint16_t mqttPort1;
    char mqttUser1[32];
    char mqttPass1[64];

    char broker2[80];               // backup broker
    uint16_t mqttPort2;             // backup mqttPort
    char mqttUser2[32];             // backup mqttUser
    char mqttPass2[64];             // backup mqttPass
    
    char mqttGlobalTopic[64];
    
    uint16_t statusUpdateInterval;  // in minutes
    char ntpServer[64];    
    char timeZone[51];              // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)   
    char firmwareConfURL[80];
    uint16_t firmwareUpdateCheckInterval; // in minutes

    // * application specific settings
    
} app_settings_t;

extern app_settings_t current_settings;
extern app_settings_t new_settings;

// ssid, password, dhcp, ip, netmask, gateway, dns
typedef std::function<void(const char*, const char*, bool, 
                           const char*, const char*, const char*, 
                           const char*)> TAddNetworkCallback;

bool   loadSettings(app_settings_t &settings);
bool   saveSettings(app_settings_t settings);
bool   json2Settings(String json, app_settings_t &settings);
String settings2Json(app_settings_t settings);
void   copySettings(app_settings_t settingsFrom, app_settings_t &settingsTo);
bool   deleteSettingsFile();
String loadNetworksConfigJson(app_settings_t settings);
bool   saveNetworksConfigJson(String json);
bool   loadNetworks(TAddNetworkCallback cb);
bool   deleteNetworksConfigJson();
bool   formatFS();
