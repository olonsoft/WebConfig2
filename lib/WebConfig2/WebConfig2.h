#pragma once

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <AsyncTCP.h>
#include <WiFi.h>
#else
#error not supported platform
#endif
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <Settings.h>

typedef enum {
  WEB_CONFIG_STARTED,
  WEB_CONFIG_TIMEOUT,
} webconfig_status_t;

typedef std::function<void(webconfig_status_t, char *)>
    TWebconfigStatusCallbackF;

class WebConfig2 {
 public:
  WebConfig2();
  WebConfig2(AsyncWebServer &server, DNSServer &dns);

  void setApCredentials(const String &apSsid, const String &apPass = "");
  void setTimeout(uint16_t seconds);
  void begin();
  void onStatusChange(TWebconfigStatusCallbackF fn);  
  String getApSsid();
  // returns a json array with found networks
  String scanWiFiJson(bool removeDuplicates);
  void loop();
  DNSServer *_dnsServer;

 private:
  TWebconfigStatusCallbackF _webconfigStatusCallbackF;
  void sendStatusCallback(webconfig_status_t wcStatus, char *message);
  void onTimeoutEvent();
  void renewTimeout();
  void setupWebServerHandlers();
  // GlobalSettings *_settObj;
  // globalSettings_t _appSettings;
  AsyncWebServer *_server;
  uint16_t _timeoutSecs = 120;
  String _apSsid, _apPass;
  bool saveNetworkConfig(String data);
  bool checkAuth(AsyncWebServerRequest *request);
  unsigned long _connectStartTime;
};  // class
