#include "../../src/defaults.h"
#include <MQTT2.h>
#include <helper.h>

AsyncMqttClient mqttClient;

uint32_t mqttConnectionTime =   0; 

String    mqttGlobalTopic;
String    mqttDeviceTopic;
String    mqttLastWillTopic;

String    mqttBroker;
uint16_t  mqttPort;
String    mqttUser;
String    mqttPassword;

// backup mqtt 
String    bmqttBroker;
uint16_t  mqttPort2;
String    mqttUser2;
String    mqttPass2word;

String    mqttDeviceName;

mqttConnectionStatus_t mqttStatus = MQTT_DISCONNECTED;

uint16_t mqttDisconnections;

TMqttConnectionChangeMessage _onMQTTConnectionChange;
TMqttMessage _onMQTTMessage;

void mqttSubscribe() { 
  // subscribe to global topic (for all devices)
  String topic = addTrailingSlash(mqttGlobalTopic) + 
                        String(MQTT_TOPIC_COMMAND);
  mqttClient.subscribe(CSTR(topic), MQTT_QOS);
  
  // subscribe to this device's topic
  topic = addTrailingSlash(mqttDeviceTopic) + 
                 String(MQTT_TOPIC_COMMAND);
  mqttClient.subscribe(CSTR(topic), MQTT_QOS);

}

void onMqttConnect(bool sessionPresent) {
  if (_onMQTTConnectionChange) {
    char buffer[64];
    snprintf_P(buffer, sizeof(buffer), PSTR("%sConnected.\n"), MQTT_STR);    
    _onMQTTConnectionChange(MQTT_CONNECTED, buffer);
  }

  mqttSubscribe();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  char buffer[64];
  if (reason == AsyncMqttClientDisconnectReason::TCP_DISCONNECTED) {
    snprintf_P(buffer, sizeof(buffer), PSTR("%sTCP Disconnected\n"), MQTT_STR);
  }
  if (reason == AsyncMqttClientDisconnectReason::MQTT_IDENTIFIER_REJECTED) {
    snprintf_P(buffer, sizeof(buffer), PSTR("%sIdentifier Rejected\n"), MQTT_STR);      
  }
  if (reason == AsyncMqttClientDisconnectReason::MQTT_SERVER_UNAVAILABLE) {
    snprintf_P(buffer, sizeof(buffer), PSTR("%sServer unavailable\n"), MQTT_STR);
  }
  if (reason == AsyncMqttClientDisconnectReason::MQTT_MALFORMED_CREDENTIALS) {
    snprintf_P(buffer, sizeof(buffer), PSTR("%sMalformed credentials\n"), MQTT_STR);      
  }
  if (reason == AsyncMqttClientDisconnectReason::MQTT_NOT_AUTHORIZED) {
    snprintf_P(buffer, sizeof(buffer), PSTR("%sNot authorized\n"), MQTT_STR);
  }            
  if (reason == AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT) {
    snprintf_P(buffer, sizeof(buffer), PSTR("%sBad fingerprint\n"), MQTT_STR);
  }

  mqttDisconnections++;

  if (_onMQTTConnectionChange) {
    _onMQTTConnectionChange(MQTT_DISCONNECTED, buffer);
  }

}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  TLOGDEBUGF_P(PSTR("%sSubscribe acknowledged. PacketId: %lu QOS: %hu\n"), MQTT_STR, (unsigned long)packetId, qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  TLOGDEBUGF_P(PSTR("%sUnsubscribe acknowledged. PacketId: %lu\n"), MQTT_STR, (unsigned long)packetId);
}

void onMQTTMessage(TMqttMessage fn) {
  _onMQTTMessage = fn; 
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  if (len == 0) return;
  char message[len + 1];
  strlcpy(message, (char *) payload, len + 1);
  if (_onMQTTMessage) {
    _onMQTTMessage(topic, message, properties.qos, properties.dup, properties.retain, len+1);
  }
}

void onMqttPublish(uint16_t packetId) {
  TLOGDEBUGF_P(PSTR("%sPublish acknowledged. PacketId: %lu\n"), MQTT_STR, (unsigned long)packetId);
  // todo: add callback
}


void mqttSetup(bool mainServer) {
  
  if (mainServer) {
    //sprintf(willTopic, "%s/%s", deviceTopic, MQTT_TOPIC_SYSTEM_INFO); 
    // todo if remotely update topics should restart or run MQTTSetup again
    TLOGDEBUGF_P(PSTR("%sSetup main broker.\n"), MQTT_STR);
    mqttClient.setServer(CSTR(mqttBroker), mqttPort);
    mqttClient.setCredentials(CSTR(mqttUser), CSTR(mqttPassword));
  } else {
    TLOGDEBUGF_P(PSTR("%sSetup backup broker.\n"), MQTT_STR);    
    mqttClient.setServer(CSTR(bmqttBroker), mqttPort2);
    mqttClient.setCredentials(CSTR(mqttUser2), CSTR(mqttPass2word));
  }

  mqttClient.setWill(CSTR(mqttLastWillTopic), MQTT_QOS, true, "Offline");
  mqttDisconnections = 0;
}

void mqttServerSetup(String broker, uint16_t port, String user, String password, 
               String globalTopic, String deviceName) {

  mqttBroker        = broker;
  mqttPort          = port;
  mqttUser          = user;
  mqttPassword      = password;

  mqttDeviceName    = getDeviceName(deviceName);
  mqttGlobalTopic   = addTrailingSlash(globalTopic);
  mqttDeviceTopic   = addTrailingSlash(mqttGlobalTopic) +
                      addTrailingSlash(mqttDeviceName);
  mqttLastWillTopic = addTrailingSlash(mqttDeviceTopic) +
                      String(MQTT_TOPIC_SYSTEM);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);

  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);

  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);  

  mqttSetup(true);
}

void mqttBackupServerSetup(String broker, uint16_t port, String user, String password) {
  bmqttBroker   = broker;
  mqttPort2     = port;
  mqttUser2     = user;
  mqttPass2word = password;
}


void mqttConnect() {
  static bool mainBroker = true;
  static uint8_t mqttConErrors = 0;
  if (millis()-mqttConnectionTime < MQTT_DISCON_TIME) {
    mqttConErrors++;
    if (mqttConErrors > MQTT_NUM_DISCON_SWITCH) {
      mainBroker = !mainBroker;
      mqttSetup(mainBroker);
      mqttConErrors = 0;
    }
  } else {
    mqttConErrors = 0;
  }
  
  if (_onMQTTConnectionChange) {
    char buffer[120];
    snprintf_P(buffer, sizeof(buffer), PSTR("%sConnecting to %s mqtt broker.\n"), MQTT_STR, mainBroker?"main":"backup");    
    _onMQTTConnectionChange(MQTT_CONNECTING, buffer);
  }

  mqttClient.connect();
  mqttConnectionTime = millis();
}

void mqttDisconnect() {
  mqttClient.disconnect();
}

void onMQTTConnectionChange(TMqttConnectionChangeMessage fn) {
  _onMQTTConnectionChange = fn;
}

void MQTTSend(String topic, String message, bool retain) {
  if (!mqttClient.connected()) return;
  mqttClient.publish(CSTR(topic), MQTT_QOS, retain, CSTR(message));
  TLOGDEBUGF_P(PSTR("%sSend Topic: [%s]. Message: %s\n"), MQTT_STR, topic.c_str(), message.c_str() );
}

void MQTTSendStatus(String message) {
  String _topic =  addTrailingSlash(mqttDeviceTopic) +
      String(MQTT_TOPIC_STATUS);
  MQTTSend(_topic, message, true);
}

void MQTTSendSystem(String message) {
  String _topic = addTrailingSlash(mqttDeviceTopic) + 
      String(MQTT_TOPIC_SYSTEM);
  MQTTSend(_topic, message, true);
}

void MQTTSendData(String message) {
  String _topic = addTrailingSlash(mqttDeviceTopic) +  
      String(MQTT_TOPIC_DATA);
  MQTTSend(_topic, message, false);
}
