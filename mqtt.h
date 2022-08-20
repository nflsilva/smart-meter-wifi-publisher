#pragma once

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <map>
#include <ArduinoMqttClient.h>

#include "secrets.h"

class MQTTResult {
  public:
  bool success;
  char* message;
};

class MQTTConnection {
  
  private:
  WiFiClient* wifiClient;
  MqttClient* mqtt;

  void setupWiFi();
  void setupClient();
  
  public:
  MQTTConnection();
  MQTTResult mqttConnect();
  MQTTResult mqttPublish(const char* topic, const char* data);
  MQTTResult mqttPing();
  
};
