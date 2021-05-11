#pragma once

#include <arduino.h>
#include <AsyncMqttClient.h>

typedef enum {
    MQTT_DISCONNECTED, 
    MQTT_CONNECTING,
    MQTT_CONNECTED
} mqttConnectionStatus_t;

extern mqttConnectionStatus_t   mqttStatus;
extern uint16_t                 mqttDisconnections;
extern String                   mqttGlobalTopic;
extern String                   mqttDeviceTopic;

// change connection message
typedef std::function<void(mqttConnectionStatus_t, char*)> TMqttConnectionChangeMessage;

// incoming message. (topic, message, qos, dup, retain, len)
typedef std::function<void(char*, char*, uint8_t, bool, bool, uint32_t)> TMqttMessage;


void mqttServerSetup(String broker, uint16_t port, String user, String password,
               String globalTopic, String deviceName);
void mqttBackupServerSetup(String broker, uint16_t port, String user, String password);

void mqttConnect();
void mqttDisconnect();
void MQTTSend(String topic, String message, bool retain);
void MQTTSendStatus(String message);
void MQTTSendSystem(String message);
void MQTTSendData(String message);

void onMQTTConnectionChange(TMqttConnectionChangeMessage fn);
extern TMqttConnectionChangeMessage _onMQTTConnectionChange;
void onMQTTMessage(TMqttMessage fn);
extern TMqttMessage _onMQTTMessage;
