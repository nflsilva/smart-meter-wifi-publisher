#pragma once

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <map>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "secrets.h"

class MQTTResult {
  public:
  bool success;
  char* message;
};

class MQTTConnection {
  
  private:
  WiFiClient* wifiClient;
  Adafruit_MQTT_Client* mqtt;
  std::map<const char*, Adafruit_MQTT_Publish*> publishers;

  void setupWiFi();
  void setupClient();
  
  public:
  MQTTConnection();
  MQTTResult mqttConnect();
  MQTTResult mqttPublish(const char* topic, const unsigned char* data);
  MQTTResult mqttPing();
  
};
