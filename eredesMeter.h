#pragma once

#include <SoftwareSerial.h>
#include <stdint.h>
#include <stddef.h>

#define EREDES_BAUD 9300
#define EREDES_RX_PIN 4
#define EREDES_TX_PIN 5

#define EREDES_CLOCK_RESPONSE_LEN 13
#define EREDES_VOLTAGE_CURRENT_RESPONSE_LEN 5

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
/*
  H
  01
  04
  0C xx xx xx xx xx uu
  0C xx xx xx xx xx xx uu
  0C xx xx xx xx xx xx xx uu
  
  R
  01
  04
  12 7 230 8 17 3 22 19 6 0 128 0 128
  24
  72

  VC
  01
  04
  04 xx xx UU uu
  
  
*/
 

class VoltageCurrentResponse {
  public:
  uint16_t voltage;
  uint16_t current;
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

class EredesMeterConnection {
  
  private:
  SoftwareSerial* serialConnection;
  void writeRequest(byte* request, uint8_t length);
  void readResponse(byte* responseData, uint8_t length);
  
  public:
  EredesMeterConnection();
  void getClock(ClockResponse* response);
  void getVoltageAndCurrent(VoltageCurrentResponse* response);
  
};
