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

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("WiFi connected. IP address: "); Serial.println(WiFi.localIP());
};

void MQTTConnection::setupClient() {
  mqtt = new MqttClient(wifiClient);
  mqtt->setId("clientId");
  mqtt->setUsernamePassword(MQTT_USERNAME, MQTT_KEY);
}

MQTTResult MQTTConnection::mqttConnect() {
  
  MQTTResult result;
  result.success = true;
  
  if (mqtt->connected()) {
    return result;
  }
  
  if (!mqtt->connect(MQTT_SERVER, MQTT_SERVERPORT)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqtt->connectError());

    while (1);
  }
  
  return result;
}

MQTTResult MQTTConnection::mqttPublish(const char* topic, const char* data) {
  
  MQTTResult r;
  r.success = true;
  mqtt->beginMessage(topic);
  mqtt->print(data);
  mqtt->endMessage();

  delay(100);

  return r;
};

MQTTResult MQTTConnection::mqttPing() {
  MQTTResult r;
  r.success = true;

  return r;
};
