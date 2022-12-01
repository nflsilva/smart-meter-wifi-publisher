#include <ArduinoJson.h>
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
  InstantVoltageCurrentResponse vr;
  TotalPowerResponse tpr;
  TariffResponse trr;
  ClockResponse cr;
} context;

void setupWiFi() {
  context.wifiClient = createWiFiConnection();
}

void setupSerial() {
   Serial.begin(USB_BAUD);
}

void setup() {

  setupWiFi();
  delay(100);
  
  setupSerial();
  delay(100);

  context.mqttConnection = new MQTTConnection(context.wifiClient);
  delay(100);

  context.meterConnection = new EredesMeterConnection();
  delay(100);
}

void sendConsumptionStatus(){

  DynamicJsonDocument json(1024);
  
  context.meterConnection->getVoltageAndCurrent(&context.vr);
  delay(500);
  context.meterConnection->getTotalPower(&context.tpr);
  delay(500);
  context.meterConnection->getTariff(&context.trr);

  json["vol"] = context.vr.voltage;
  json["cur"] = context.vr.current;
  json["fre"] = context.vr.frequency;
  
  json["api"] = context.tpr.energyImport;
  json["ape"] = context.tpr.energyExport;
  json["pf"] = context.tpr.powerFactor;

  json["vaz"] = context.trr.vazio;
  json["pon"] = context.trr.ponta;
  json["che"] = context.trr.cheias;
  json["tar"] = context.trr.tariff;

  json["tim"] = context.trr.totalImport;
  json["tex"] = context.trr.totalExport;
  
  char data[1024];
  serializeJsonPretty(json, data);

  context.mqttConnection->mqttConnect();

  Serial.print("Consumption: "); Serial.println(data);
  context.mqttConnection->mqttPublish("tele/powermeter/consumption", data);
}

void sendMachineStatus(){

  DynamicJsonDocument json(512);
  
  context.meterConnection->getClock(&context.cr);
  delay(500);

  json["ver"] = VERSION;
  json["mem"] = ESP.getFreeHeap();
  json["net"] = WiFi.RSSI();
  json["yea"] = context.cr.year;
  json["mon"] = context.cr.month;
  json["day"] = context.cr.dayMonth;
  json["hou"] = context.cr.hours;
  json["min"] = context.cr.minutes;
  json["sec"] = context.cr.seconds;
  
  char data[512];
  serializeJson(json, data);

  context.mqttConnection->mqttConnect();

  Serial.print("Status: "); Serial.println(data);
  context.mqttConnection->mqttPublish("tele/powermeter/status", data);
}

void loop() {

  sendConsumptionStatus();
  
  sendMachineStatus();

  delay(6000);
}
