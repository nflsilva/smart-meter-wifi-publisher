#include "mqtt.h"

MQTTConnection::MQTTConnection(WiFiClient* wifiClient) {
  mqtt = new MqttClient(wifiClient);
  mqtt->setId("clientId");
  mqtt->setUsernamePassword(MQTT_USERNAME, MQTT_KEY);
  hasValidServer = false;
};

void MQTTConnection::mqttConnect() {
  
  if (mqtt->connected()) {
    return;
  }

  uint8_t tries = 3;
  while (tries > 0 ) {
    if(mqtt->connect(MQTT_SERVER, MQTT_SERVERPORT)) {
      hasValidServer = true;
      break;
    }
    tries--;
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqtt->connectError());
    delay(1000);
  }
};

bool MQTTConnection::mqttIsConnected() {
  return hasValidServer;
}

void MQTTConnection::mqttPublish(String topic, char* data) {
  mqtt->beginMessage(topic.c_str());
  mqtt->print(data);
  mqtt->endMessage();
  delay(100);
};


void MQTTConnection::mqttPublish(String topic, JsonDocument* json) {
  char data[JSON_SIZE];
  serializeJson(*json, data, sizeof(data));
  mqttConnect();
  Serial.print(topic + ": "); Serial.println(data);
  mqttPublish(topic, data);
};
