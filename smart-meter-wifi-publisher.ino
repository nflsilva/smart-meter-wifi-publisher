#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <string>

#include "secrets.h"
#include "eredesMeter.h"
#include "mqtt.h"
#include "wifi.h"

// USB
#define USB_BAUD 115200

struct Context {
  WiFiClient* wifiClient = NULL;
  MQTTConnection* mqttConnection = NULL;
  EredesMeterConnection* meterConnection = NULL;
  uint32_t* valueBuffer = NULL; 
} context;

void setupWiFi() {
  context.wifiClient = createWiFiConnection();
}

void setupSerial() {
  Serial.begin(USB_BAUD);
}

void setupOTA() {
  
  ArduinoOTA.setPort(8080);
  ArduinoOTA.setHostname("tasduino");
  
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
  
  ArduinoOTA.begin();
  
}

void setup() {

  setupWiFi();
  delay(100);
  
  setupSerial();
  delay(100);

  context.mqttConnection = new MQTTConnection(context.wifiClient);
  delay(100);

  context.meterConnection = new EredesMeterConnection();
  context.valueBuffer = new uint32_t[512];
  delay(100);

  setupOTA();
  delay(100);
}

void sendConsumptionStatus() {

  DynamicJsonDocument json(1024);
  char data[1024];
  
  json["vol"] = (double)context.valueBuffer[0];
  json["cur"] = (double)context.valueBuffer[1];
  
  serializeJsonPretty(json, data);
  
  context.mqttConnection->mqttConnect();
  
  Serial.print("Consumption: "); Serial.println(data);
  context.mqttConnection->mqttPublish("tele/powermeter/consumption", data);
  
}

void loop() {

  context.meterConnection->readRegisters(context.valueBuffer, 0x006c, 2, Long);
  sendConsumptionStatus();
  
  // 150sec = 2.5min
  for(int s=0; s < 150; s++) {
    ArduinoOTA.handle();
    delay(100);
  }

}
