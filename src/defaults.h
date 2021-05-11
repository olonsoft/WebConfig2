#pragma once

#ifndef DEFAULT_APP_NAME
#define DEFAULT_APP_NAME   "MySmartHomeApp-{id}"          // Enter your device friendly name
#endif

#ifndef APP_VERSION
#define APP_VERSION "1.0.0"
#endif

#ifndef FS_VERSION
#define FS_VERSION  "1.0.0"
#endif

// to print color ansi chars on platformio terminal 
// set monitor_flags  = --raw in platformio.ini
// https://en.wikipedia.org/wiki/ANSI_escape_code
#ifndef TERMINAL_IN_COLOR
#define TERMINAL_IN_COLOR
#endif

#ifndef TERMINAL_TIME_PRINT
#define TERMINAL_TIME_PRINT
#endif

#ifndef APP_DEBUG_PORT
#define APP_DEBUG_PORT Serial
#endif

#ifndef SERIAL_BAUDRATE
#define SERIAL_BAUDRATE         115200          // Default baudrate
#endif

#ifndef APP_LOG_LEVEL
#define APP_LOG_LEVEL 4
#endif

// define application debug messages 

#ifndef TERMINAL_IN_COLOR
#define TERMINAL_IN_COLOR
#endif

// TODO : move to each app header
#ifdef TERMINAL_IN_COLOR
#define MQTT_STR        "[\e[32mMQTT\e[m] "
#define SETTINGS_STR    "[\e[32mSETT\e[m] "
#else
#define MQTT_STR        "[MQTT] "
#define SETTINGS_STR    "[SETT] "
#endif
// Todo end

#ifndef ENABLE_DEBUG_PRINT
#define ENABLE_DEBUG_PRINT
#endif

#ifndef CAPTIVE_PORTAL_PSW
#define CAPTIVE_PORTAL_PSW  "12345678"          // must be >= 8chars
#endif


// Wifi AP Mode (when your Wifi Network is not reachable) ----------------------------------------
#ifndef DEFAULT_AP_IP
#define DEFAULT_AP_IP       192,168,4,1         // Enter IP address (comma separated) for AP (config) mode
#endif
#ifndef DEFAULT_AP_SUBNET
#define DEFAULT_AP_SUBNET   255,255,255,0       // Enter IP address (comma separated) for AP (config) mode
#endif
#ifndef DEFAULT_AP_KEY
#define DEFAULT_AP_KEY      "12345678"          // Enter network WPA key for AP (config) mode
#endif

// Wifi Client Mode -----------------------------------------------------------------------------
#ifndef DEFAULT_SSID
#define DEFAULT_SSID        "mywifi"            // Enter your Wifi network SSID
#endif
#ifndef DEFAULT_WIFI_PSW
#define DEFAULT_WIFI_PSW    "mywifipass"        // Enter your Wifi network WPA key
#endif
#ifndef DEFAULT_USE_DHCP
#define DEFAULT_USE_DHCP    true                // (true|false)
#endif
#ifndef DEFAULT_IP
#define DEFAULT_IP          "192.168.1.1"       // Enter your IP address if not use dhcp
#endif
#ifndef DEFAULT_DNS
#define DEFAULT_DNS         "192.168.1.1"       // Enter your DNS
#endif
#ifndef DEFAULT_GW
#define DEFAULT_GW          "192.168.1.1"       // Enter your Gateway
#endif
#ifndef DEFAULT_SUBNET
#define DEFAULT_SUBNET      "255.255.255.0"     // Enter your Subnet
#endif
#ifndef DEFAULT_ADMIN_USERNAME
#define DEFAULT_ADMIN_USERNAME  "admin"
#endif
#ifndef DEFAULT_ADMIN_PASS
#define DEFAULT_ADMIN_PASS     "12345678"
#endif

#ifndef DEFAULT_WIFI_CONNECTION_TIMEOUT
#define DEFAULT_WIFI_CONNECTION_TIMEOUT  10000  // minimum timeout in ms for WiFi to be connected.
#endif

// -- MQTT defaults ------------------------
#ifndef DEFAULT_MQTT_TOPIC
#define DEFAULT_MQTT_TOPIC      "dimitris/devices"      // Enter your pub
#endif

#ifndef DEFAULT_MQTT_SERVER1
#define DEFAULT_MQTT_SERVER1    "test.mosquitto.org"    // Enter your MQTT Server IP address or host name
#endif

#ifndef DEFAULT_MQTT_PORT1
#define DEFAULT_MQTT_PORT1       1883                  // Enter MQTT Server port
#endif

#ifndef DEFAULT_MQTT_USER1
#define DEFAULT_MQTT_USER1       ""                   // Enter MQTT Server user
#endif

#ifndef DEFAULT_MQTT_PASSWORD1
#define DEFAULT_MQTT_PASSWORD1   ""                   // Enter MQTT Server password
#endif

#ifndef DEFAULT_MQTT_SERVER2
#define DEFAULT_MQTT_SERVER2     "mqtt.local"        // Enter your MQTT Server IP address or host name
#endif

#ifndef DEFAULT_MQTT_PORT2
#define DEFAULT_MQTT_PORT2       1883                // Enter MQTT Server port
#endif

#ifndef DEFAULT_MQTT_USER2
#define DEFAULT_MQTT_USER2       "username"         // Enter MQTT Server user
#endif

#ifndef DEFAULT_MQTT_PASSWORD2
#define DEFAULT_MQTT_PASSWORD2   "password"         // Enter MQTT Server password
#endif

#ifndef DEFAULT_MQTT_RETAIN
#define DEFAULT_MQTT_RETAIN      false              // (true|false) Retain MQTT messages?
#endif

#ifndef DEFAULT_MQTT_DELAY
#define DEFAULT_MQTT_DELAY       100                // Time in milliseconds to retain MQTT messages
#endif

#ifndef MQTT_TOPIC_LWT
#define MQTT_TOPIC_LWT           "LWT"              // Default lwt topic
#endif

#ifndef MQTT_TOPIC_DATA
#define MQTT_TOPIC_DATA          "data"             // app data topic
#endif

#ifndef MQTT_TOPIC_SYSTEM
#define MQTT_TOPIC_SYSTEM        "system"           // system info topic
#endif

#ifndef MQTT_TOPIC_STATUS
#define MQTT_TOPIC_STATUS        "status"           // status info topic
#endif

#ifndef MQTT_TOPIC_COMMAND
#define MQTT_TOPIC_COMMAND       "command"          // user commands topic
#endif

#ifndef MQTT_QOS
#define MQTT_QOS                 0
#endif

#ifndef DEFAULT_MQTT_LWT_CONNECT_MESSAGE
#define DEFAULT_MQTT_LWT_CONNECT_MESSAGE     "online" // Default lwt message
#endif

#ifndef DEFAULT_MQTT_LWT_DISCONNECT_MESSAGE
#define DEFAULT_MQTT_LWT_DISCONNECT_MESSAGE  "offline" // Default lwt message
#endif

#ifndef MQTT_NUM_DISCON_SWITCH
#define MQTT_NUM_DISCON_SWITCH      10    // number of disconnections to switch to next mqtt broker
#endif

#ifndef MQTT_DISCON_TIME
#define MQTT_DISCON_TIME            20000 // to switch to next broker disconnections must occur within 20secs
#endif

#ifndef MQTT_STATUS_UPDATE_INTERVAL_MINUTES 
#define MQTT_STATUS_UPDATE_INTERVAL_MINUTES 5
#endif

// json -----------------------------------------
#ifndef JSON_SIZE
#define JSON_SIZE 1024
#endif


// Default saved settings -----------------------
#define SETTINGS_FILE               "/config.json"
#define NETWORKS_FILE               "/networks.json"


#ifndef DEFAULT_SERVER_USEDNS
#define DEFAULT_SERVER_USEDNS       false             // true: Use hostname.  false: use IP
#endif
#ifndef DEFAULT_PORT
#define DEFAULT_PORT                8080               // Enter your Server port value
#endif

#ifndef DEFAULT_PIN_I2C_SDA
#define DEFAULT_PIN_I2C_SDA         4
#endif
#ifndef DEFAULT_PIN_I2C_SCL
#define DEFAULT_PIN_I2C_SCL         5
#endif
#ifndef DEFAULT_I2C_CLOCK_SPEED
#define DEFAULT_I2C_CLOCK_SPEED     400000            // Use 100 kHz if working with old I2C chips
#endif

#ifndef DEFAULT_PIN_STATUS_LED
#define DEFAULT_PIN_STATUS_LED      (-1)
#endif
#ifndef DEFAULT_PIN_STATUS_LED_INVERSED
#define DEFAULT_PIN_STATUS_LED_INVERSED  true
#endif


// --- NTP --------------------------
#ifndef DEFAULT_NTP_HOST     
#define DEFAULT_NTP_HOST              "gr.pool.ntp.org" 
#endif

#ifndef DEFAULT_TIME_ZONE
#define DEFAULT_TIME_ZONE             "EET-2EEST-3,M3.5.0/03:00:00,M10.5.0/04:00:00" 
#endif

// --- Firmware over the air ---
#ifndef FOTA_URL
#define FOTA_URL                      "http://fw.crete.ovh"  
#endif

#ifndef FOTA_CHECK_INTERVAL_MINUTES
#define FOTA_CHECK_INTERVAL_MINUTES   10
#endif

#ifndef DEFAULT_USE_SERIAL
#define DEFAULT_USE_SERIAL            true    // (true|false) Enable Logging to the Serial Port
#endif

#ifndef DEFAULT_SERIAL_BAUD
#define DEFAULT_SERIAL_BAUD           115200  // Serial Port Baud Rate
#endif

#ifndef DEFAULT_SYSLOG_FACILITY
#define DEFAULT_SYSLOG_FACILITY      	0 	    // kern
#endif

#ifndef SUCCESS_STR
#define SUCCESS_STR                   "Success"
#endif

#ifndef FAIL_STR
#define FAIL_STR                      "Fail"
#endif