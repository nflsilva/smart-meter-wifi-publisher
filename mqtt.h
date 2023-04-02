#pragma once

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <map>
#include <ArduinoMqttClient.h>

#include "secrets.h"

class MQTTConnection {
  
  private:
  MqttClient* mqtt;
  WiFiClient* wifiClient;
  bool hasValidServer;
  void mqttPublish(String topic, char* data);
  bool mqttIsConnected();

  public:
  MQTTConnection(WiFiClient* wifiClient);
  bool mqttConnect();
  void mqttPublish(String topic, JsonDocument* json);

};
