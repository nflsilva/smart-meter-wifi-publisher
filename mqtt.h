#pragma once

#include <ESP8266WiFi.h>
#include <map>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "secrets.h"

class MQTTResult {
  public:
  bool success;
  String message;
};

class MQTTContext {
  
  private:
  WiFiClient* wifiClient;
  Adafruit_MQTT_Client* mqtt;
  std::map<const char*, Adafruit_MQTT_Publish*> publishers;

  void setupWiFi();
  void setupClient();
  
  public:
  MQTTContext();
  MQTTResult mqttConnect();
  MQTTResult mqttPublish(const char* topic, const char* data);
  MQTTResult mqttPing();
  
};
