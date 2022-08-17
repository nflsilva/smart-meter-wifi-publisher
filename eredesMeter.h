#pragma once

#include <SoftwareSerial.h>
#include <stdint.h>
#include <stddef.h>

#define TTL_BAUD 9300
#define TTL_RX_PIN 4
#define TTL_TX_PIN 5
#define TTL_RESPONSE_LEN 13

class TimeResponse {
  public:
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
};

class EredesMeterConnection {
  
  private:
  SoftwareSerial* serialConnection;
  
  public:
  EredesMeterConnection();
  void getCurrentTime(TimeResponse* response);
  
};
