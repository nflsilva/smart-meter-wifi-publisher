#include "eredesMeter.h"

EredesMeterConnection::EredesMeterConnection() {
  serialConnection = new SoftwareSerial(EREDES_RX_PIN, EREDES_TX_PIN);
  pinMode(EREDES_RX_PIN, INPUT);
  pinMode(EREDES_TX_PIN, OUTPUT);
  serialConnection->begin(EREDES_BAUD);
};

void EredesMeterConnection::writeRequest(byte* request, uint8_t length) {
  if(serialConnection == NULL || request == NULL) return;
  serialConnection->write(request, length);
  delay(500);
};

void EredesMeterConnection::readResponse(byte* responseData, uint8_t length) {

    if(serialConnection->available() <= 0) return;
    
    byte address = serialConnection->read();
    byte function = serialConnection->read();
    for(uint8_t i=0;i<length;i++){
      responseData[i] = serialConnection->read();
    }
    byte crc0 = serialConnection->read();
    byte crc1 = serialConnection->read();

    // Fast debug
    Serial.print(address, HEX);
    Serial.print(" ");
    Serial.print(function, HEX);
    Serial.print(" ");
    for(uint8_t i=0;i<length;i++){
      Serial.print(responseData[i], DEC);
      Serial.print(".");
    }
    Serial.print(" ");
    Serial.print(crc0, HEX);
    Serial.print(" ");
    Serial.println(crc1, HEX);
  
};

void EredesMeterConnection::getClock(ClockResponse* response) {

  byte request[8] = { 0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x60, 0x0a };
  writeRequest(request, 8);

  byte responseData[EREDES_CLOCK_RESPONSE_LEN];
  readResponse(responseData, EREDES_CLOCK_RESPONSE_LEN);

  response->year = responseData[1] << 8 | responseData[2];
  response->month = responseData[3];
  response->dayMonth = responseData[4];
  response->dayWeek = responseData[5];
  response->hours = responseData[6];
  response->minutes = responseData[7];
  response->seconds = responseData[8];
  response->hseconds = responseData[9];
  response->deviation = responseData[10] << 8 | responseData[11];
  response->hseconds = responseData[12];
};

void EredesMeterConnection::getVoltageAndCurrent(VoltageCurrentResponse* response) {

  byte request[8] = { 0x01, 0x04, 0x00, 0x6c, 0x00, 0x02, 0xb1, 0xd6 };
  writeRequest(request, 8);
  
  byte responseData[EREDES_VOLTAGE_CURRENT_RESPONSE_LEN];
  readResponse(responseData, EREDES_VOLTAGE_CURRENT_RESPONSE_LEN);

  response->voltage = responseData[1] << 8 | responseData[2];
  response->current =  responseData[3] << 8 | responseData[4];
};
