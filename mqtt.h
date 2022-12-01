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

  public:
  MQTTConnection(WiFiClient* wifiClient);
  void mqttConnect();
  void mqttPublish(const char* topic, const char* data);

};
