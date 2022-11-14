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

void EredesMeterConnection::readResponse(byte* data) {

    if(serialConnection->available() <= 0) return;
    
    byte address = serialConnection->read();
    byte function = serialConnection->read();
    uint8_t length = serialConnection->read();
    for(uint8_t i=0;i<length;i++){
      data[i] = serialConnection->read();
    }
    byte crc0 = serialConnection->read();
    byte crc1 = serialConnection->read();
    
    // Fast debug
    Serial.print(address, HEX);
    Serial.print(" ");
    Serial.print(function, HEX);
    Serial.print(" ");
    Serial.print(length, DEC);
    Serial.print(" ");
    for(uint8_t i=0;i<length;i++){
      Serial.print(data[i], DEC);
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

  byte responseData[252];
  readResponse(responseData);

  response->year = responseData[0] << 8 | responseData[1];
  response->month = responseData[2];
  response->dayMonth = responseData[3];
  response->dayWeek = responseData[4];
  response->hours = responseData[5];
  response->minutes = responseData[6];
  response->seconds = responseData[7];
  response->hseconds = responseData[8];
  response->deviation = responseData[9] << 8 | responseData[10];
  response->hseconds = responseData[11];
  
};

void EredesMeterConnection::getVoltageAndCurrent(InstantVoltageCurrentResponse* response) {

  byte responseData[252];

  byte request0[8] = { 0x01, 0x04, 0x00, 0x6c, 0x00, 0x02, 0xb1, 0xd6 };
  writeRequest(request0, 8);
  readResponse(responseData);
  response->voltage = (responseData[0] << 8 | responseData[1]) / 10.0;
  response->current = (responseData[2] << 8 | responseData[3]) / 10.0;

  delay(500);
  byte request1[8] = { 0x01, 0x04, 0x00, 0x7f, 0x00, 0x02, 0x40, 0x13 };
  writeRequest(request1, 8);
  readResponse(responseData);
  response->frequency = (responseData[0] << 8 | responseData[1]) / 10.0;
};

void EredesMeterConnection::getTotalPower(TotalPowerResponse* response) {
  
  byte request[8] = { 0x01, 0x04, 0x00, 0x16, 0x00, 0x03, 0x51, 0xcf };
  writeRequest(request, 8);
  
  byte responseData[252];
  readResponse(responseData);
  
  response->energyImport = ((responseData[0] << 24) | (responseData[1] << 16) | (responseData[2] << 8) | responseData[3]) / 1000.0;
  response->energyExport = ((responseData[4] << 24) | (responseData[5] << 16) | (responseData[6] << 8) | responseData[7]) / 1000.0;
  response->powerFactor = ((responseData[8] << 8) | responseData[9]) / 10.0f;

};

void EredesMeterConnection::getTariff(TariffResponse* response) {

  byte responseData[252];

  byte request0[8] = { 0x01, 0x04, 0x00, 0x26, 0x00, 0x03, 0x51, 0xc0 };
  writeRequest(request0, 8);
  readResponse(responseData);
  response->vazio = ((responseData[0] << 24) | (responseData[1] << 16) | (responseData[2] << 8) | responseData[3]) / 1000.0;
  response->ponta = ((responseData[4] << 24) | (responseData[5] << 16) | (responseData[6] << 8) | responseData[7]) / 1000.0;
  response->cheias = ((responseData[8] << 24) | (responseData[9] << 16) | (responseData[10] << 8) | responseData[11]) / 1000.0;
  
  delay(500);
  byte request1[8] = { 0x01, 0x04, 0x00, 0x0b, 0x00, 0x01, 0x040, 0x08 };
  writeRequest(request1, 8);
  readResponse(responseData);
  response->tariff = responseData[0];

  delay(500);
  byte request2[8] = { 0x01, 0x04, 0x00, 0x2c, 0x00, 0x01, 0xf0, 0x03 };
  writeRequest(request2, 8);
  readResponse(responseData);
  response->totalImport = ((responseData[0] << 24) | (responseData[1] << 16) | (responseData[2] << 8) | responseData[3]) / 1000.0;

  delay(500);
  byte request3[8] = { 0x01, 0x04, 0x00, 0x33, 0x00, 0x01, 0xc5, 0x50 };
  writeRequest(request3, 8);
  readResponse(responseData);
  response->totalExport = ((responseData[0] << 24) | (responseData[1] << 16) | (responseData[2] << 8) | responseData[3]) / 1000.0;

};
