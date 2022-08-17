#include "eredesMeter.h"

EredesMeterConnection::EredesMeterConnection() {
  serialConnection = new SoftwareSerial(TTL_RX_PIN, TTL_TX_PIN);
  pinMode(TTL_RX_PIN, INPUT);
  pinMode(TTL_TX_PIN, OUTPUT);
  serialConnection->begin(TTL_BAUD);
};

void EredesMeterConnection::getCurrentTime(TimeResponse* response) {

  if(serialConnection == NULL || response == NULL) {
    return;
  }
  
  byte request[8] = { 0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x60, 0x0a };
  byte responseData[TTL_RESPONSE_LEN];

  serialConnection->write(request, 8);
  delay(1000);

  if (serialConnection->available() > 0) {
    byte address = serialConnection->read();
    byte function = serialConnection->read();
    
    for(int i=0;i<TTL_RESPONSE_LEN;i++){
      responseData[i] = serialConnection->read();
    }
    byte crc0 = serialConnection->read();
    byte crc1 = serialConnection->read();

    Serial.print(address, HEX);
    Serial.print(" ");
    Serial.print(function, HEX);
    Serial.print(" ");
    for(int i=0;i<TTL_RESPONSE_LEN;i++){
      Serial.print(responseData[i], DEC);
      Serial.print(".");
    }
    Serial.print(" ");
    Serial.println(crc0, HEX);
    Serial.println(crc1, HEX);
    
  }
  
};
