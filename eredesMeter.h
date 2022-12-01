#pragma once

#include <SoftwareSerial.h>
#include <stdint.h>
#include <stddef.h>

#include "secrets.h"

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

enum EredesType { Integer = 1, Long = 2, Double = 4 };

class MODBUSMessage {
  public:
  byte data[256];
  uint8_t size;
  byte getAddress();
  byte getFunction();
  uint16_t getCRC(); 
};
 
class EredesMeterConnection {
  
  private:
  byte requestBuffer[8] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  MODBUSMessage* messageBuffer;
  SoftwareSerial* serialConnection;
  void writeRequest(byte* request);
  void readResponse(MODBUSMessage* message);
  void debugPrint(MODBUSMessage* message);
  void computeRequestCRC(byte* request);
  
  public:
  EredesMeterConnection();
  void readRegisters(void* result, uint16_t start, uint16_t length, EredesType type);
  
};
