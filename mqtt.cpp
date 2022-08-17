#include "mqtt.h"

MQTTConnection::MQTTConnection() {
  setupWiFi();
  setupClient();
};

void MQTTConnection::setupWiFi() {

  IPAddress staticIP(WIFI_STATIC_IP);
  IPAddress gateway(WIFI_GATEWAY);
  IPAddress subnet(WIFI_NETWORK);
  IPAddress primaryDNS(WIFI_DNS1);
  IPAddress secondaryDNS(WIFI_DNS2);

  wifiClient = new WiFiClient();
  
  Serial.print("Connecting to ");Serial.println(WIFI_SSID);

  if (WIFI_USE_STATIC_IP && !WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("WiFi connected. IP address: "); Serial.println(WiFi.localIP());
};

void MQTTConnection::setupClient() {
  mqtt = new Adafruit_MQTT_Client(wifiClient, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
}

MQTTResult MQTTConnection::mqttConnect() {
  
  MQTTResult result;
  result.success = true;
  
  if (mqtt->connected()) {
    return result;
  }
  
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
  
  return result;
}

MQTTResult MQTTConnection::mqttPublish(const char* topic, const char* data) {
  
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

MQTTResult MQTTConnection::mqttPing() {
  MQTTResult r;
  r.success = true;
  if(!mqtt->ping()) {
    mqtt->disconnect();
    r.success = false;
  }
  return r;
};
