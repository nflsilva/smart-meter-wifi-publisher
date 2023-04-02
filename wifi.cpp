#include "wifi.h"

void startWiFiConnection() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.print("WiFi connected. IP address: "); 
  Serial.println(WiFi.localIP());
}

WiFiClient* createWiFiConnection() {

  IPAddress staticIP(WIFI_STATIC_IP);
  IPAddress gateway(WIFI_GATEWAY);
  IPAddress subnet(WIFI_NETWORK);
  IPAddress primaryDNS(WIFI_DNS1);
  IPAddress secondaryDNS(WIFI_DNS2);

  WiFiClient* wifiClient = new WiFiClient();

  if (WIFI_USE_STATIC_IP && !WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  startWiFiConnection();
  
  return wifiClient;
};

bool ensureWiFiConnection() {
  int wifiStatus = WiFi.status();
  if(wifiStatus == WL_CONNECTED) return false;

  WiFi.disconnect();
  startWiFiConnection();
  return true;
}
