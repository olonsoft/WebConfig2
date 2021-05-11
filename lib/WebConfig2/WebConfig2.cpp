#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <Ticker.h>
#include <WebConfig2.h>
#include <helper.h>
#include <helper_wifi.h>

// include html and css pages
#include "admin.html.h"
#include "common.js.h"
#include "favicon.ico.h"
#include "general.html.h"
#include "info.html.h"
#include "mqtt.html.h"
#include "ntp.html.h"
#include "settings.html.h"
#include "style.css.h"
#include "update.html.h"
#include "wifi.html.h"

#define WEBCONFIG_STR "[\e[32mWEBCONF\e[m] "

String networkConfigJson = "[]";
bool httpAuthenticated = false;

Ticker timeoutTicker;

AsyncWebServer server(80);
DNSServer dns;

WebConfig2::WebConfig2() {
  _server = &server;
  _dnsServer = &dns;
  _apPass = "";
}

WebConfig2::WebConfig2(AsyncWebServer &server, DNSServer &dns) {
  _server = &server;
  _dnsServer = &dns;
  _apPass = "";
}  // WebConfig2

void WebConfig2::onTimeoutEvent() {
  // timeoutTicker.detach();
  char buffer[80];
  snprintf_P(buffer, sizeof(buffer), PSTR("%sCaptive portal timeout.\n"),
             WEBCONFIG_STR);
  sendStatusCallback(WEB_CONFIG_TIMEOUT, buffer);
}

void WebConfig2::renewTimeout() {
  timeoutTicker.detach();
  if (wifiIsAP(WiFi.getMode())) {
    TLOGDEBUGF_P(PSTR("%sRenew Timeout on AP.\n"), WEBCONFIG_STR);

#ifdef ESP8266
    timeoutTicker.once(_timeoutSecs,
                       std::bind(&WebConfig2::onTimeoutEvent, this));
#else
    timeoutTicker.once(
        _timeoutSecs, +[](WebConfig2 *f) { f->onTimeoutEvent(); }, this);
#endif
  }
}

void WebConfig2::sendStatusCallback(webconfig_status_t wcStatus,
                                    char *message) {
  if (_webconfigStatusCallbackF) _webconfigStatusCallbackF(wcStatus, message);
}

void WebConfig2::onStatusChange(TWebconfigStatusCallbackF fn) {
  _webconfigStatusCallbackF = fn;
}

// returns a json array with found networks
String WebConfig2::scanWiFiJson(bool removeDuplicates) {
  // todo
  // https://github.com/me-no-dev/ESPAsyncWebServer#scanning-for-available-wifi-networks
  // scan for wifi networks
  String json = "[]";
  int n = WiFi.scanComplete();
  if (n == WIFI_SCAN_RUNNING) {
    return json;
  }
  if (n == WIFI_SCAN_FAILED) {
    WiFi.scanNetworks(true);
    return json;
  }
  if (n >= 0) {
    TLOGDEBUGF_P(PSTR("%sNetworks found: %d\n"), WEBCONFIG_STR, n);
    // build array of indices
    int ix[n];
    for (int i = 0; i < n; i++) ix[i] = i;

    // sort by signal strength
    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++)
        if (WiFi.RSSI(ix[j]) > WiFi.RSSI(ix[i])) std::swap(ix[i], ix[j]);

    // remove duplicates ( must be RSSI sorted )
    if (removeDuplicates) {
      String cssid;
      for (int i = 0; i < n; i++) {
        if (ix[i] == -1) continue;
        cssid = WiFi.SSID(ix[i]);
        for (int j = i + 1; j < n; j++) {
          if (cssid == WiFi.SSID(ix[j])) {
            TLOGDEBUGF_P(PSTR("%sDUP AP: %s\n"), WEBCONFIG_STR, WiFi.SSID(ix[j]).c_str());            
            ix[j] = -1;  // set dup aps to index -1
          }
        }
      }
    }

    json = "[";

    bool first = true;
    for (int i = 0; i < n; i++) {
      if (ix[i] == -1) continue;
      if (!first) json += ",";
      json += "{";
      json += "\"ssid\":\"" + WiFi.SSID(ix[i]) + "\"";
      json += ",\"rssi\":" + String(WiFi.RSSI(ix[i]));
      json += ",\"bssid\":\"" + WiFi.BSSIDstr(ix[i]) + "\"";
      // json += ",\"channel\":" + String(WiFi.channel(ix[i]));
      json += ",\"secure\":" + String(WiFi.encryptionType(ix[i]));      // todo : <---- is different on ESP32 vs ESP8266
      // json += ",\"hidden\":" + String(WiFi.isHidden(ix[i]) ? "true" :
      // "false");
      json += "}";
      first = false;
    }
    json += "]";
    WiFi.scanDelete();
    if (WiFi.scanComplete() == WIFI_SCAN_FAILED) {
      WiFi.scanNetworks(true);
    }
  }
  // send string to client
  TLOGDEBUGF_P(PSTR("%s%s\n"), WEBCONFIG_STR, json.c_str());  
  return json;
  // todo: delete last scan from memory
}

bool WebConfig2::saveNetworkConfig(String data) {
  return saveNetworksConfigJson(data);
}

bool WebConfig2::checkAuth(AsyncWebServerRequest *request) {  
  if (!httpAuthenticated) {
    return request->authenticate("admin", new_settings.adminPass);
  } else {
    return true;
  }
}

void WebConfig2::setupWebServerHandlers() {

  _server->reset();

  //================================================================================

  _server->rewrite("/", "/admin.html");  // todo change
// Android captive portal. Maybe not needed.
// Might be handled by notFound handler.

  _server->rewrite("/generate_204", "/admin.html");  
                                    
// Microsoft captive portal. Maybe not needed.    
// Might be handled by notFound handler.                                
  _server->rewrite("/fwlink", "/admin.html");  
                                  

  //================================================================================

  _server->onNotFound([this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn not found.\n"), WEBCONFIG_STR);
    // todo onNotFound send home page
    request->send_P(200, "text/html", admin_html);  
  });

  //================================================================================

  _server->on("/favicon.ico", [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn favicon.ico\n"), WEBCONFIG_STR);
    // request->send(404);
    request->send(200, "text/html", "");
    // request->send_P(200, "image/x-icon", favicon_8b_ico);
    renewTimeout();  // all the pages call favicon.ico No need to add this to
                     // every page.
  });

  //================================================================================

  _server->on("/style.css", [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn style.css\n"), WEBCONFIG_STR);
    request->send_P(200, "text/css", style_css);
    renewTimeout();  // all the pages call style.css No need to add this to
                     // every page.
  });

  //================================================================================

  _server->on("/common.js", [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn common.js\n"), WEBCONFIG_STR);
    request->send_P(200, "text/javascript", common_js);
  });

  //================================================================================

  _server->on("/admin.html", [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn admin.html\n"), WEBCONFIG_STR);
    request->send_P(200, "text/html", admin_html);
  });

  //================================================================================
  _server->on("/general.html", HTTP_GET,
              [this](AsyncWebServerRequest *request) {
                if (!checkAuth(request))
                  return request->requestAuthentication();
                  TLOGDEBUGF_P(PSTR("%sOn general.html\n"), WEBCONFIG_STR);
                request->send_P(200, "text/html", general_html);
              });

  _server->on("/json.json", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn json.json\n"), WEBCONFIG_STR);
    String s = settings2Json(new_settings);
    request->send(200, "text/json", s);
  });

  //================================================================================
  _server->on("/wifi.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn wifi.html\n"), WEBCONFIG_STR);
    scanWiFiJson(true);
    request->send_P(200, "text/html", wifi_html);
  });

  //================================================================================
  _server->on("/mqtt.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn mqtt.html\n"), WEBCONFIG_STR);
    request->send_P(200, "text/html", mqtt_html);
  });

  //================================================================================
  _server->on("/ntp.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn ntp.html\n"), WEBCONFIG_STR);
    request->send_P(200, "text/html", ntp_html);
  });

  //===========================================================================

  _server->on("/update.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn update.html\n"), WEBCONFIG_STR);
    request->send_P(200, "text/html", update_html);
  });
  
  //===========================================================================
  
  _server->on("/scanwifi.json", HTTP_GET,
              [this](AsyncWebServerRequest *request) {
                if (!checkAuth(request))
                  return request->requestAuthentication();
                  TLOGDEBUGF_P(PSTR("%sOn scanwifi.json\n"), WEBCONFIG_STR);
                request->send(200, "text/json", scanWiFiJson(true));
              });  //_server->on /wifi/list

//===========================================================================
// send networkConfigJson String to web
//=====================================
  _server->on("/network.json", HTTP_GET,
              [this](AsyncWebServerRequest *request) {
                if (!checkAuth(request))
                  return request->requestAuthentication();  
                  TLOGDEBUGF_P(PSTR("%sOn network.json Send: %s\n"), WEBCONFIG_STR, networkConfigJson.c_str());              
                request->send(200, "text/json", networkConfigJson);
              });

//===========================================================================

  _server->on("/settings.html", HTTP_GET,
              [this](AsyncWebServerRequest *request) {
                if (!checkAuth(request))
                  return request->requestAuthentication();
                  TLOGDEBUGF_P(PSTR("%sOn settings.html\n"), WEBCONFIG_STR);
                request->send_P(200, "text/html", settings_html);
              });

//===========================================================================

  _server->on("/info.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn info.html\n"), WEBCONFIG_STR);
    request->send_P(200, "text/html", info_html);
  });

//===========================================================================

  _server->on("/info.json", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    TLOGDEBUGF_P(PSTR("%sOn info.json\n"), WEBCONFIG_STR);
    request->send(200, "text/json", getSystemInfoJson());
  });

//===========================================================================
// save wifi networks to networkConfigJson varialble
//==================================================
  AsyncCallbackJsonWebHandler *saveNetworkHandler =
      new AsyncCallbackJsonWebHandler(
          "/savenetwork.json",
          [this](AsyncWebServerRequest *request, JsonVariant &json) {
            if (!checkAuth(request)) return request->requestAuthentication();
            TLOGDEBUGF_P(PSTR("%sOn savenetwork.json\n"), WEBCONFIG_STR);
            networkConfigJson = "";
            serializeJson(json, networkConfigJson);
            // networkConfigJson = json.as<String>();
            TLOGDEBUGLN(networkConfigJson);
            //request->redirect("/admin.html");
          });

  _server->addHandler(saveNetworkHandler);

//===========================================================================
// on any other save, this is called
//==================================
  AsyncCallbackJsonWebHandler *saveSettingsHandler =
      new AsyncCallbackJsonWebHandler(
          "/savesettings.json",
          [this](AsyncWebServerRequest *request, JsonVariant &json) {
            if (!checkAuth(request)) return request->requestAuthentication();
            TLOGDEBUGF_P(PSTR("%sOn savesettings.json\n"), WEBCONFIG_STR);
            String s;
            serializeJson(json, s);
            json2Settings(s, new_settings);          
            serializeJsonPretty(json, Serial);
            //request->redirect("/admin.html");
          });

  _server->addHandler(saveSettingsHandler);

//===========================================================================
// on (main admin page SAVE)
//==========================
  _server->on("/save", [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    request->redirect("admin.html");
    TLOGDEBUGF_P(PSTR("%sSaving and rebooting...\n"), WEBCONFIG_STR);    
    saveSettings(new_settings);
    saveNetworksConfigJson(networkConfigJson);
    delay(1000);
    ESP.restart();    //todo add to task list
    delay(1000);
  });

//===========================================================================

  _server->on("/reset", [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    request->redirect("/");
    TLOGDEBUGF_P(PSTR("%sReseting to factory defaults...\n"), WEBCONFIG_STR);     
    deleteSettingsFile();
    deleteNetworksConfigJson();
    // always format on factory reset, in case of corrupt SPIFFS
    /*SPIFFS.end();
    Serial.println(F("RESET: formatting..."));
    SPIFFS.format();
    Serial.println(F("RESET: formatting done..."));
    if (!SPIFFS.begin())
    {
      serialPrintln(F("RESET: FORMAT SPIFFS FAILED!"));
      return;
    }
    */
    ESP.restart();
    delay(1000);
  });

//===========================================================================

  _server->on("/reboot", [this](AsyncWebServerRequest *request) {
    if (!checkAuth(request)) return request->requestAuthentication();
    // request->send(200, "text/html", Page_WaitAndRestart);
    request->redirect("/");
    TLOGDEBUGF_P(PSTR("%sRebooting...\n"), WEBCONFIG_STR);         
    delay(100);
    ESP.restart();
    delay(1000);
  });

//===========================================================================

  _server->begin();

}  // setupWiFiHandlers


void WebConfig2::begin() {


/*  
  if (wifiConnected()) {
    // wifi is already connected
    _dnsServer->stop();
  } else {
    startAP();  
  }
*/
  renewTimeout();
  networkConfigJson = loadNetworksConfigJson(current_settings);
  httpAuthenticated = false;
  setupWebServerHandlers();

  char buffer[80];
  snprintf_P(buffer, sizeof(buffer),
             PSTR("%sCaptive portal started. Mode: %s\n"), WEBCONFIG_STR,
             getWifiModeString(WiFi.getMode()).c_str());
  sendStatusCallback(WEB_CONFIG_STARTED, buffer);
}

String WebConfig2::getApSsid() {
  return _apSsid.length() ? _apSsid : getDefaultDeviceName();

}  // getApSsid

void WebConfig2::setApCredentials(const String &apSsid, const String &apPass) {
  if (apSsid.length())      _apSsid = apSsid;
  if (apPass.length() >= 8) _apPass = apPass;
}  // setApCredentials

void WebConfig2::setTimeout(uint16_t seconds) { _timeoutSecs = seconds; }

void WebConfig2::loop() {
  _dnsServer->processNextRequest();
}