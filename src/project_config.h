#pragma once

#ifndef DEFAULT_APP_NAME
#define DEFAULT_APP_NAME                    "MyApp"             // Enter your device friendly name
#endif

#ifndef APP_VERSION
#define APP_VERSION                         "1.0.0"
#endif

#ifndef FS_VERSION
#define FS_VERSION                          "1.0.0"
#endif

#ifndef DEFAULT_APP_AUTHOR
#define DEFAULT_APP_AUTHOR                  "dimitris19@gmail.com"
#endif

#ifndef DEFAULT_APP_WEBSITE
#define DEFAULT_APP_WEBSITE                 "https://github.com/olonsoft"
#endif

#define DEVICE_ID_STR                       "{id}"

#ifndef DEFAULT_APP_NAME_ID
#define DEFAULT_APP_NAME_ID                 DEFAULT_APP_NAME "_" DEVICE_ID_STR // Device name with mac id
#endif

#ifndef CAPTIVE_PORTAL_PSW
#define CAPTIVE_PORTAL_PSW                  "12345678"          // must be >= 8chars
#endif

// Wifi AP Mode (when your Wifi Network is not reachable) ----------------------------------------
#ifndef DEFAULT_AP_IP
#define DEFAULT_AP_IP                       192,168,4,1         // Enter IP address (comma separated) for AP (config) mode
#endif
#ifndef DEFAULT_AP_SUBNET
#define DEFAULT_AP_SUBNET                   255,255,255,0       // Enter IP address (comma separated) for AP (config) mode
#endif
#ifndef DEFAULT_AP_KEY
#define DEFAULT_AP_KEY                      "12345678"          // Enter network WPA key for AP (config) mode
#endif

// Wifi hardcoded clients  ------------------------------------------------------------------------

#ifndef WIFI_MAX_NETWORKS
#define WIFI_MAX_NETWORKS                   4                    // Max number of WIFI connection configurations
#endif

#ifndef DEFAULT_WIFI1_SSID
#define DEFAULT_WIFI1_SSID                  "wifiSSID1"          // Enter your Wifi network SSID
#endif
#ifndef DEFAULT_WIFI1_PSW
#define DEFAULT_WIFI1_PSW                   "wifiPASS1"          // Enter your Wifi network WPA key
#endif
#ifndef DEFAULT_WIFI1_IP
#define DEFAULT_WIFI1_IP                    ""                  // IP address (Empty = DHCP)
#endif
#ifndef DEFAULT_WIFI1_DNS
#define DEFAULT_WIFI1_DNS                   "192.168.1.1"       // Enter your DNS
#endif
#ifndef DEFAULT_WIFI1_GW
#define DEFAULT_WIFI1_GW                    "192.168.1.1"       // Enter your Gateway
#endif
#ifndef DEFAULT_WIFI1_SUBNET
#define DEFAULT_WIFI1_SUBNET                "255.255.255.0"     // Enter your Subnet
#endif

#ifndef DEFAULT_WIFI2_SSID
#define DEFAULT_WIFI2_SSID                  "wifiSSID2"          // Enter your Wifi network SSID
#endif
#ifndef DEFAULT_WIFI2_PSW
#define DEFAULT_WIFI2_PSW                   "wifiPASS2"          // Enter your Wifi network WPA key
#endif
#ifndef DEFAULT_WIFI2_IP
#define DEFAULT_WIFI2_IP                    ""                  // IP address (Empty = DHCP)
#endif
#ifndef DEFAULT_WIFI2_DNS
#define DEFAULT_WIFI2_DNS                   "192.168.1.1"       // Enter your DNS
#endif
#ifndef DEFAULT_WIFI2_GW
#define DEFAULT_WIFI2_GW                    "192.168.1.1"       // Enter your Gateway
#endif
#ifndef DEFAULT_WIFI2_SUBNET
#define DEFAULT_WIFI2_SUBNET                "255.255.255.0"     // Enter your Subnet
#endif

#ifndef DEFAULT_WIFI3_SSID
#define DEFAULT_WIFI3_SSID                  "wifiSSID3"          // Enter your Wifi network SSID
#endif
#ifndef DEFAULT_WIFI3_PSW
#define DEFAULT_WIFI3_PSW                   "wifiPASS3"          // Enter your Wifi network WPA key
#endif
#ifndef DEFAULT_WIFI3_IP
#define DEFAULT_WIFI3_IP                    ""                  // IP address (Empty = DHCP)
#endif
#ifndef DEFAULT_WIFI3_DNS
#define DEFAULT_WIFI3_DNS                   "192.168.1.1"       // Enter your DNS
#endif
#ifndef DEFAULT_WIFI3_GW
#define DEFAULT_WIFI3_GW                    "192.168.1.1"       // Enter your Gateway
#endif
#ifndef DEFAULT_WIFI3_SUBNET
#define DEFAULT_WIFI3_SUBNET                "255.255.255.0"     // Enter your Subnet
#endif

#ifndef DEFAULT_WIFI4_SSID
#define DEFAULT_WIFI4_SSID                  "wifiSSID4"          // Enter your Wifi network SSID
#endif
#ifndef DEFAULT_WIFI4_PSW
#define DEFAULT_WIFI4_PSW                   "wifiPASS4"          // Enter your Wifi network WPA key
#endif
#ifndef DEFAULT_WIFI4_IP
#define DEFAULT_WIFI4_IP                    ""                  // IP address (Empty = DHCP)
#endif
#ifndef DEFAULT_WIFI4_DNS
#define DEFAULT_WIFI4_DNS                   "192.168.1.1"       // Enter your DNS
#endif
#ifndef DEFAULT_WIFI4_GW
#define DEFAULT_WIFI4_GW                    "192.168.1.1"       // Enter your Gateway
#endif
#ifndef DEFAULT_WIFI4_SUBNET
#define DEFAULT_WIFI4_SUBNET                "255.255.255.0"     // Enter your Subnet
#endif

#ifndef DEFAULT_ADMIN_USERNAME
#define DEFAULT_ADMIN_USERNAME              "admin"
#endif
#ifndef DEFAULT_ADMIN_PASS
#define DEFAULT_ADMIN_PASS                  "12345678"
#endif

#ifndef DEFAULT_WIFI_CONNECTION_TIMEOUT
#define DEFAULT_WIFI_CONNECTION_TIMEOUT     10000  // minimum timeout in ms for WiFi to be connected.
#endif

#ifndef DEFAULT_WIFI_RECONNECT_INTERVAL
#define DEFAULT_WIFI_RECONNECT_INTERVAL     180000              // If could not connect to WIFI, retry after this time in ms
#endif

#ifndef LOOP_DELAY_TIME
#define LOOP_DELAY_TIME                     10  // Delay for the main loop, in millis [0-250]
                                                // Recommended minimum is 10, see:
                                                // https://github.com/xoseperez/espurna/issues/1541
                                                // https://github.com/xoseperez/espurna/issues/1631
                                                // https://github.com/esp8266/Arduino/issues/5825
#endif

// =========================================
// =            MQTT defaults              =
// =========================================

// strings for setings

#ifndef STR_BROKER
#define STR_BROKER                          "broker"
#endif

#ifndef STR_MQTT_PORT
#define STR_MQTT_PORT                       "mqttport"
#endif

#ifndef STR_MQTT_USER
#define STR_MQTT_USER                       "mqttuser"
#endif

#ifndef STR_MQTT_PASS
#define STR_MQTT_PASS                       "mqttpass"
#endif

// mqtt brokers info

#ifndef DEFAULT_MQTT_SERVER1
#define DEFAULT_MQTT_SERVER1                "mqtt.local"        // Enter your MQTT Server IP address or host name
#endif

#ifndef DEFAULT_MQTT_PORT1
#define DEFAULT_MQTT_PORT1                  1883                // Enter MQTT Server port
#endif

#ifndef DEFAULT_MQTT_USER1
#define DEFAULT_MQTT_USER1                  "mqttuser"          // Enter MQTT Server user
#endif


#ifndef DEFAULT_MQTT_PASSWORD1
#define DEFAULT_MQTT_PASSWORD1              "mqttpass"          // Enter MQTT Server password
#endif

#ifndef DEFAULT_MQTT_SERVER2
#define DEFAULT_MQTT_SERVER2                "mqtt.server2.com" // Enter your MQTT Server IP address or host name
#endif

#ifndef DEFAULT_MQTT_PORT2
#define DEFAULT_MQTT_PORT2                  12345              // Enter MQTT Server port
#endif

#ifndef DEFAULT_MQTT_USER2
#define DEFAULT_MQTT_USER2                  "mqttuser2"        // Enter MQTT Server user
#endif

#ifndef DEFAULT_MQTT_PASSWORD2
#define DEFAULT_MQTT_PASSWORD2              "mqttpass2"        // Enter MQTT Server password
#endif

// common mqtt settings

#ifndef DEFAULT_MQTT_RETAIN
#define DEFAULT_MQTT_RETAIN                 false              // (true|false) Retain MQTT messages?
#endif

#ifndef DEFAULT_MQTT_DELAY
#define DEFAULT_MQTT_DELAY                  100                // Time in milliseconds to retain MQTT messages
#endif

#ifndef MQTT_TOPIC_LWT
#define MQTT_TOPIC_LWT                      "LWT"              // Default lwt topic
#endif

#ifndef MQTT_TOPIC_DATA
#define MQTT_TOPIC_DATA                     "data"             // app data topic
#endif

#ifndef MQTT_TOPIC_SYSTEM
#define MQTT_TOPIC_SYSTEM                   "system"           // system info topic
#endif

#ifndef MQTT_TOPIC_STATUS
#define MQTT_TOPIC_STATUS                   "status"           // status info topic
#endif

#ifndef MQTT_TOPIC_COMMAND
#define MQTT_TOPIC_COMMAND                  "command"          // user commands topic
#endif

#ifndef MQTT_QOS
#define MQTT_QOS                            0
#endif

#ifndef DEFAULT_MQTT_LWT_CONNECT_MESSAGE
#define DEFAULT_MQTT_LWT_CONNECT_MESSAGE    "online" // Default lwt message
#endif

#ifndef DEFAULT_MQTT_LWT_DISCONNECT_MESSAGE
#define DEFAULT_MQTT_LWT_DISCONNECT_MESSAGE "offline" // Default lwt message
#endif

#ifndef MQTT_RECONNECT_INITIAL_DELAY 
#define MQTT_RECONNECT_INITIAL_DELAY        1000 // When disconnected first retry connection after this ms
#endif 

#ifndef MQTT_RECONNECT_FINAL_DELAY 
#define MQTT_RECONNECT_FINAL_DELAY          30000 // This is the max reconnection delay1000
#endif 

#ifndef MQTT_RECONNECT_DELAY_MULTIPLIER
#define MQTT_RECONNECT_DELAY_MULTIPLIER     1.5f // on every disconnection multipy retry delay by this value
#endif 

#ifndef MQTT_NUM_DISCON_SWITCH
#define MQTT_NUM_DISCON_SWITCH              10    // number of disconnections to switch to next mqtt broker
#endif

#ifndef MQTT_DISCON_TIME
#define MQTT_DISCON_TIME                    20000 // to switch to next broker disconnections must occur within 20secs
#endif

#ifndef MQTT_STATUS_UPDATE_INTERVAL_MINUTES 
#define MQTT_STATUS_UPDATE_INTERVAL_MINUTES 5
#endif


// =========================================
// =            JSON defaults              =
// =========================================


#ifndef JSON_SIZE
#define JSON_SIZE 1024
#endif


// Default saved settings -----------------------

#ifndef DEFAULT_SERVER_USEDNS
#define DEFAULT_SERVER_USEDNS               false  // true: Use hostname.  false: use IP
#endif
#ifndef DEFAULT_PORT
#define DEFAULT_PORT                        8080   // Enter your Server port value
#endif

#ifndef DEFAULT_PIN_I2C_SDA
#define DEFAULT_PIN_I2C_SDA                 4
#endif
#ifndef DEFAULT_PIN_I2C_SCL
#define DEFAULT_PIN_I2C_SCL                 5
#endif
#ifndef DEFAULT_I2C_CLOCK_SPEED
#define DEFAULT_I2C_CLOCK_SPEED             400000    // Use 100 kHz if working with old I2C chips
#endif

#ifndef DEFAULT_PIN_STATUS_LED
#define DEFAULT_PIN_STATUS_LED              (-1)
#endif

#ifndef DEFAULT_PIN_STATUS_LED_ON
#define DEFAULT_PIN_STATUS_LED_ON           false // led will be on if pin is true or false
#endif


// =========================================
// =            NTP  defaults              =
// =========================================

#ifndef DEFAULT_NTP_HOST     
#define DEFAULT_NTP_HOST                    "gr.pool.ntp.org" 
#endif

#ifndef DEFAULT_TIME_ZONE
#define DEFAULT_TIME_ZONE                   "EET-2EEST-3,M3.5.0/03:00:00,M10.5.0/04:00:00" 
#endif

// --- Firmware over the air ---
#ifndef FOTA_URL
#define FOTA_URL                            "http://fw.crete.ovh"  
#endif

#ifndef FOTA_CHECK_INTERVAL_MINUTES
#define FOTA_CHECK_INTERVAL_MINUTES         10
#endif

#ifndef DEFAULT_USE_SERIAL
#define DEFAULT_USE_SERIAL                  true    // (true|false) Enable Logging to the Serial Port
#endif

#ifndef DEFAULT_SERIAL_BAUD
#define DEFAULT_SERIAL_BAUD                 115200  // Serial Port Baud Rate
#endif


/* Telegram notify */
#define CONFIG_TELEGRAM_TOKEN               "tgTOKEN"
#define CONFIG_TELEGRAM_CHATID              -12345678901233
#define CONFIG_TELEGRAM_DEVICE              "😀 ESP-43GFS6"
#define CONFIG_TELEGRAM_SEND_QUEUE_SIZE     16

/* rlog : debug log settings */
#define CONFIG_RLOG_PROJECT_LEVEL           RLOG_LEVEL_DEBUG
#define CONFIG_RLOG_PROJECT_COLORS          1
#define CONFIG_RLOG_SHOW_TIMESTAMP          1

/* rled32 : task settings */
#define CONFIG_LED_QUEUE_SIZE               3
#define CONFIG_LED_TASK_STACK_SIZE          2048
#define CONFIG_LED_TASK_PRIORITY            5
#define CONFIG_LED_TASK_CORE                1
#define CONFIG_LED_LOG_RU                   0

/* rled32 : system blink modes */

#define CONFIG_LEDSYS_NORMAL_QUANTITY       1
#define CONFIG_LEDSYS_NORMAL_DURATION       75
#define CONFIG_LEDSYS_NORMAL_INTERVAL       5000
#define CONFIG_LEDSYS_ERROR_QUANTITY        1
#define CONFIG_LEDSYS_ERROR_DURATION        1000
#define CONFIG_LEDSYS_ERROR_INTERVAL        1000
#define CONFIG_LEDSYS_WARNING_QUANTITY      1
#define CONFIG_LEDSYS_WARNING_DURATION      1000
#define CONFIG_LEDSYS_WARNING_INTERVAL      1000
#define CONFIG_LEDSYS_WIFI_ERROR_QUANTITY   1
#define CONFIG_LEDSYS_WIFI_ERROR_DURATION   500
#define CONFIG_LEDSYS_WIFI_ERROR_INTERVAL   500
#define CONFIG_LEDSYS_WIFI_INIT_QUANTITY    1
#define CONFIG_LEDSYS_WIFI_INIT_DURATION    250
#define CONFIG_LEDSYS_WIFI_INIT_INTERVAL    250
#define CONFIG_LEDSYS_INET_ERROR_QUANTITY   2
#define CONFIG_LEDSYS_INET_ERROR_DURATION   100
#define CONFIG_LEDSYS_INET_ERROR_INTERVAL   500
#define CONFIG_LEDSYS_MQTT_ERROR_QUANTITY   3
#define CONFIG_LEDSYS_MQTT_ERROR_DURATION   250
#define CONFIG_LEDSYS_MQTT_ERROR_INTERVAL   1000
#define CONFIG_LEDSYS_TG_ERROR_QUANTITY     4
#define CONFIG_LEDSYS_TG_ERROR_DURATION     250
#define CONFIG_LEDSYS_TG_ERROR_INTERVAL     1000
#define CONFIG_LEDSYS_PUB_ERROR_QUANTITY    5
#define CONFIG_LEDSYS_PUB_ERROR_DURATION    250
#define CONFIG_LEDSYS_PUB_ERROR_INTERVAL    1000

/* ********** Strings for dictionary settings file ***************** */

#define SETTINGS_FILE                       "/config.json"
#define SETTINGS_FILE_SIZE                  2048
#define NETWORKS_FILE                       "/networks.json"

// ---- wifi strings for settings file -----------------------------------------------------------
#define STR_SSID                            "ssid"
#define STR_PASSWORD                        "pass"
#define STR_DHCP                            "dhcp"
#define STR_IP                              "ip"
#define STR_NETMASK                         "netmask"
#define STR_GATEWAY                         "gateway"
#define STR_DNS                             "dns"

#define STR_STATUS_INT                      "statusupdateinterval"
#define STR_NTP_SERVER                      "ntpserver"
#define STR_TIMEZONE                        "timezone"
#define STR_FOTA_URL                        "FOTAconfurl"
#define STR_FOTA_U_INT                      "FOTAcheckinterval"

/* general strings */

#define STR_SUCCESS                         "Success"
#define STR_FAIL                            "Fail"


