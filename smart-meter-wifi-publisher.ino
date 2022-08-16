#include <SoftwareSerial.h>
#include <string>

#include "secrets.h"
#include "eredesMeter.h"
#include "mqtt.h"

// USB
#define USB_BAUD 115200

// TTL
#define TTL_BAUD 9300
#define TTL_RX_PIN 4
#define TTL_TX_PIN 5
#define TTL_RESPONSE_LEN 14

SoftwareSerial ttlSerial = SoftwareSerial(TTL_RX_PIN, TTL_TX_PIN);
byte request[8] = { 0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x60, 0x0a };
byte response[TTL_RESPONSE_LEN];

MQTTContext* haContext = NULL;

void setupSerial() {
   Serial.begin(USB_BAUD);
}



void setupTTL() {
  pinMode(TTL_RX_PIN, INPUT);
  pinMode(TTL_TX_PIN, OUTPUT);
  ttlSerial.begin(TTL_BAUD);
}

void setup() {
  
  setupSerial();
  delay(100);

  haContext = new MQTTContext();
  delay(100);
  
  setupTTL();
  delay(100);

}

int n = 0;

void loop() {


  haContext->mqttConnect();

  auto data = "hello-" + std::to_string(n++);
  
  Serial.print("Publishing "); Serial.println(data.c_str());
  haContext->mqttPublish("/feeds/photocell", data.c_str());
  delay(1000);


  auto heap = ESP.getFreeHeap();

  Serial.println(("heap-" + std::to_string(heap)).c_str());
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
