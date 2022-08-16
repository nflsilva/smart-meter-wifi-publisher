#include "mqtt.h"

MQTTContext::MQTTContext() {
  setupWiFi();
  setupClient();
};

void MQTTContext::setupWiFi() {

  IPAddress staticIP(192, 168, 0, 6);
  IPAddress gateway(0, 0, 0, 0);
  IPAddress subnet(192, 168, 0, 1);
  IPAddress dns1(0, 0, 0, 0);
  IPAddress dns2(0, 0, 0, 0);
  
  wifiClient = new WiFiClient();
  
  Serial.print("Connecting to ");Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("WiFi connected. IP address: "); Serial.println(WiFi.localIP());
};

void MQTTContext::setupClient() {
  mqtt = new Adafruit_MQTT_Client(wifiClient, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
}

MQTTResult MQTTContext::mqttConnect() {
  
  MQTTResult result;
  result.success = true;
  
  if (mqtt->connected()) {
    return result;
  }

  Serial.print("Connecting to MQTT... ");
  
  int8_t ret;
  uint8_t retries = 3;
  while ((ret = mqtt->connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt->connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt->disconnect();
       delay(5000);
       retries--;
       if (retries == 0) {
         Serial.println("Failed MQTT connection. Please reset.");
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
  return result;
}

MQTTResult MQTTContext::mqttPublish(const char* topic, const char* data) {
  
  MQTTResult r;
  Adafruit_MQTT_Publish* publisher = publishers[topic];
  
  if(publisher == NULL) {
    publisher = new Adafruit_MQTT_Publish(mqtt, topic);
    publishers[topic] = publisher;
  }
  
  r.success = publisher->publish(data);
  if(!mqtt->ping()) {
    mqtt->disconnect();
  }

  return r;
};

MQTTResult MQTTContext::mqttPing() {
  MQTTResult r;
  r.success = true;
  if(!mqtt->ping()) {
    mqtt->disconnect();
    r.success = false;
  }
  return r;
};
