#include "mqtt.h"

MQTTConnection::MQTTConnection(WiFiClient* wifiClient) {
  mqtt = new MqttClient(wifiClient);
  mqtt->setId("clientId");
  mqtt->setUsernamePassword(MQTT_USERNAME, MQTT_KEY);
};

void MQTTConnection::mqttConnect() {
  
  if (mqtt->connected()) {
    return;
  }
  
  while (!mqtt->connect(MQTT_SERVER, MQTT_SERVERPORT)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqtt->connectError());
    delay(1000);
  }
};

void MQTTConnection::mqttPublish(const char* topic, const char* data) {
  mqtt->beginMessage(topic);
  mqtt->print(data);
  mqtt->endMessage();
  delay(100);
};
