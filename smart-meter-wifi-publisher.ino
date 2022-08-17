#include <ArduinoJson.h>
#include <string>

#include "secrets.h"
#include "eredesMeter.h"
#include "mqtt.h"

// USB
#define USB_BAUD 115200

struct Context {
  MQTTConnection* mqttConnection = NULL;
  EredesMeterConnection* meterConnection = NULL;
  DynamicJsonDocument* responseJson = NULL;
} context;

void setupSerial() {
   Serial.begin(USB_BAUD);
}

void setup() {
  
  setupSerial();
  delay(100);

  context.mqttConnection = new MQTTConnection();
  delay(100);

  context.meterConnection = new EredesMeterConnection();
  delay(100);

  context.responseJson = new DynamicJsonDocument(512);

}

void loop() {

  auto heap = ESP.getFreeHeap();

  TimeResponse tr;
  context.meterConnection->getCurrentTime(&tr);

  auto json = *context.responseJson; 
  json["heap"] = heap;
  json["hours"] = tr.hours;
  json["minutes"] = tr.minutes;
  json["seconds"] = tr.minutes;
  
  String data;
  serializeJson(json, data);

  context.mqttConnection->mqttConnect();

  //Serial.print("Publishing "); Serial.println(data.c_str());
  context.mqttConnection->mqttPublish("/feeds/test", data.c_str());
  delay(1000);

/*

byte ttlRead() {
    if (ttlSerial != NULL && ttlSerial.available() > 0) {
        return ttlSerial.read();
    }
    return NULL;
}
  ttlSerial.write(request, 8);
  delay(5000);
  
  byte address = ttlRead();
  byte function = ttlRead();
  for(int i=0;i<TTL_RESPONSE_LEN;i++){
    response[i] = ttlRead();
  }
  byte crc = ttlRead();

  Serial.print(address, HEX);
  Serial.print(" ");
  Serial.print(function, HEX);
  Serial.print(" ");
  for(int i=0;i<TTL_RESPONSE_LEN;i++){
    Serial.print(response[i], DEC);
    Serial.print(".");
  }
  Serial.print(" ");
  Serial.println(crc, HEX);

  Serial.print("---");
  unsigned int h = response[i]
  Serial.printl(response
  */
  
}
