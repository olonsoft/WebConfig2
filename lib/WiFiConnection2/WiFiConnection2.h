#pragma once

#ifdef ESP32
//#include <ESPmDNS.h>
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
#endif

#ifdef CONN2_WIFI_ENABLE_ENTERPRISE
#include "wpa2_enterprise.h"
#endif


typedef enum {
  STATUS_SCANNING,
  STATUS_SCAN_COMPLETE,
  STATUS_SCAN_PROCESSED,
  STATUS_NO_NETWORKS,
  STATUS_NO_KNOWN_NETWORKS,
  STATUS_FOUND_NETWORK,
  STATUS_CONNECTING,    
  STATUS_CONNECT_FAILED,
  STATUS_CONNECTED,
  STATUS_GOT_IP,
  STATUS_EXIT_TO_CAPTIVE_PORTAL,
  STATUS_DISCONNECTED,
  STATUS_FORCED_DISCONNECTION,

  STATUS_ACCESSPOINT_CREATING,
  STATUS_ACCESSPOINT_CREATED,
  STATUS_ACCESSPOINT_FAILED,
  STATUS_ACCESSPOINT_DESTROYED,
  
  STATUS_HOSTNAME_ERROR,
  STATUS_TURNING_OFF,
  STATUS_TURNING_ON
} conn2_wifi_status_t;

typedef struct {
    char*     ssid;
    char*     pass;
    bool      dhcp;
    int       priority;
    IPAddress ip;
    IPAddress gw;
    IPAddress netmask;
    IPAddress dns;
    int32_t   rssi;
    uint8_t   security;
    uint8_t   channel;
    uint8_t   bssid[6];
    char*     enterprise_username;
    char*     enterprise_password;
} conn2_network_t;

extern bool    dnsServerActive;
extern bool    wifiScanRemoveDuplicates;
extern uint8_t wifiConnectionRetries;

typedef std::function<void(conn2_wifi_status_t, char *)> TWiFiStatusCallbackF;

void wifiSetup();
bool addWiFi(
        const char* ssid,
        const char* pass                = NULL,
        const char* ip                  = NULL,
        const char* netmask             = NULL,
        const char* gw                  = NULL,
        const char* dns                 = NULL,
        const char* enterprise_username = NULL,
        const char* enterprise_password = NULL
      );
void clearWiFis();
void setSTA(bool enable);
void setAP(bool enable);
void onWiFiStatusChange(TWiFiStatusCallbackF fn);
void wifiConnect();
void wifiDisconnect();
void resetSavedNetworks();
//void wifiLoop();
