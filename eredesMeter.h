#pragma once

#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <stdint.h>
#include <initializer_list>

#include "secrets.h"

#define EREDES_BAUD 9300
#define EREDES_RX_PIN 4
#define EREDES_TX_PIN 5

enum EredesType { Integer = 1, Long = 2, Double = 4 };

struct MODBUSMessage {
  byte data[256];
  uint8_t size;
};

class EredesMeterConnection {
  
  private:
  byte requestBuffer[8] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  SoftwareSerial* serialConnection = NULL;
  void writeRequest(byte* request);
  void readResponse(MODBUSMessage* message);
  void debugPrint(MODBUSMessage* message);
  uint16_t computeCRC(byte* request, uint16_t lenght);
  void buildRequest(byte* request, uint16_t start, uint16_t length);
  void handleCRCError(StaticJsonDocument<JSON_SIZE>* result, MODBUSMessage* messageBuffer, uint16_t actual, uint16_t expected, std::initializer_list<String> names);
  void handleException(StaticJsonDocument<JSON_SIZE>* result, MODBUSMessage* messageBuffer, std::initializer_list<String> names);
  void handlePrimiteTypes(StaticJsonDocument<JSON_SIZE>* result, MODBUSMessage* messageBuffer, uint16_t scalar, EredesType type, std::initializer_list<String> names);
  
  public:
  EredesMeterConnection();
  void readRegisters(StaticJsonDocument<JSON_SIZE>* result, uint16_t start, uint16_t scalar, EredesType type, std::initializer_list<String> names);
  
};
