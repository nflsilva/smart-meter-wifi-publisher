#pragma once

#include <SoftwareSerial.h>
#include <stdint.h>
#include <stddef.h>

#define EREDES_BAUD 9300
#define EREDES_RX_PIN 4
#define EREDES_TX_PIN 5

/*
 * r01 04 0001 0001
 * r01 04 006C 0002
 * 
 * r01 04 0016 0002
 * 
 * r01 04 0026 0003
 * r01 04 0079 0003
 * r01 04 007F 0002
 * r01 04 000B 0002
 * r01 44 0301 ??
 */

class InstantVoltageCurrentResponse {
  public:
  double voltage;
  double current;
  double frequency;
};

class TotalPowerResponse {
  public:
  double energyImport;
  double energyExport;
  double powerFactor;
};

class ClockResponse {
  public:
  uint16_t year;
  uint8_t month;
  uint8_t dayMonth;
  uint8_t dayWeek;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint8_t hseconds;
  int16_t deviation;
  uint8_t status;
};

class TariffResponse {
  public: 
  double vazio;
  double ponta;
  double cheias;
  double totalImport;
  double totalExport;
  uint8_t tariff;
};

class EredesMeterConnection {
  
  private:
  SoftwareSerial* serialConnection;
  void writeRequest(byte* request, uint8_t length);
  void readResponse(byte* data);
  
  public:
  EredesMeterConnection();
  void getClock(ClockResponse* response);
  void getVoltageAndCurrent(InstantVoltageCurrentResponse* response);
  void getTotalPower(TotalPowerResponse* response);
  void getTariff(TariffResponse* response);
  
};
