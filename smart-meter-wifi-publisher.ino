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

  ClockResponse cr;
  context.meterConnection->getClock(&cr);

  InstantVoltageCurrentResponse vr;
  context.meterConnection->getVoltageAndCurrent(&vr);

  auto json = *context.responseJson; 
  json["heap"] = heap;
  json["year"] = cr.year;
  json["month"] = cr.month;
  json["day"] = cr.dayMonth;
  json["hours"] = cr.hours;
  json["minutes"] = cr.minutes;
  json["seconds"] = cr.seconds;

  json["voltage"] = vr.voltage;
  json["current"] = vr.current;
  
  String data;
  serializeJson(json, data);

  context.mqttConnection->mqttConnect();

  Serial.print("Publishing "); Serial.println(data.c_str());
  context.mqttConnection->mqttPublish("/feeds/test", data.c_str());
  delay(1000);
  
}
