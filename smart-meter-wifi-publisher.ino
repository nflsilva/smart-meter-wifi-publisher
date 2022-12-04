// USB
#define USB_BAUD 115200

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <string>

#include "secrets.h"
#include "eredesMeter.h"
#include "mqtt.h"
#include "wifi.h"

struct Context {
  WiFiClient* wifiClient = NULL;
  MQTTConnection* mqttConnection = NULL;
  EredesMeterConnection* meterConnection = NULL;
  StaticJsonDocument<JSON_SIZE> consumptionJson;
  StaticJsonDocument<JSON_SIZE> statusJson;
} context;

void setupSerial() {
  Serial.begin(USB_BAUD);
}

void setupWiFi() {
  context.wifiClient = createWiFiConnection();
}

void setupContext() {
  context.mqttConnection = new MQTTConnection(context.wifiClient);
  context.meterConnection = new EredesMeterConnection();
  context.statusJson["ver"] = VERSION;
}

void setupOTA() {
  
  ArduinoOTA.setPort(OTA_PORT);
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  
#if DEBUG
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
#endif
 
  ArduinoOTA.begin();
  
}

void setup() {

  setupSerial();
  setupWiFi();
  setupContext();
  setupOTA();
}

void publishMeterData() {
  context.consumptionJson.clear();
  context.statusJson.clear();

  context.meterConnection->readRegisters(&context.consumptionJson, 0x006c, 10, Long, { "vol", "cur" });
  context.meterConnection->readRegisters(&context.consumptionJson, 0x007f, 10, Long, { "fre" });
  context.meterConnection->readRegisters(&context.consumptionJson, 0x007b, 1000, Long, { "pf" });
  context.meterConnection->readRegisters(&context.consumptionJson, 0x0026, 1000, Double, { "vaz", "pon", "che" });
  context.meterConnection->readRegisters(&context.consumptionJson, 0x000b, 1, Integer, { "tar" });
  context.meterConnection->readRegisters(&context.consumptionJson, 0x002c, 1000, Double, { "tim" });
  context.meterConnection->readRegisters(&context.consumptionJson, 0x0033, 1000, Double, { "tex" });
  context.mqttConnection->mqttPublish("tele/powermeter/consumption", &context.consumptionJson);
  
  context.meterConnection->readRegisters(&context.statusJson, 0x0001, 1, Integer, { "", "", "", "", "", "hou", "min", "sec" });
  context.statusJson["mem"] = ESP.getFreeHeap();
  context.statusJson["net"] = WiFi.RSSI();
  context.mqttConnection->mqttPublish("tele/powermeter/status", &context.statusJson);
}

void loop() {
  
  //if(context.mqttConnection->mqttIsConnected()) {
    publishMeterData();
  //}
  
  // 150sec = 2.5min
  for(int s=0; s < 150; s++) {
    ArduinoOTA.handle();
    delay(100);
  }
  
}
