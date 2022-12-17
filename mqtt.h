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

  public:
  MQTTConnection(WiFiClient* wifiClient);
  void mqttConnect();
  void mqttPublish(String topic, char* data);
  void mqttPublish(String topic, JsonDocument* json);
  bool mqttIsConnected();

};
