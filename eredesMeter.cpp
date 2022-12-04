#include "eredesMeter.h"

EredesMeterConnection::EredesMeterConnection() {
  serialConnection = new SoftwareSerial(EREDES_RX_PIN, EREDES_TX_PIN);
  pinMode(EREDES_RX_PIN, INPUT);
  pinMode(EREDES_TX_PIN, OUTPUT);
  serialConnection->begin(EREDES_BAUD);
};

void EredesMeterConnection::writeRequest(byte* request) {
  if(serialConnection == NULL || request == NULL) return;
  serialConnection->write(request, 8);
  delay(500);
};

void EredesMeterConnection::readResponse(MODBUSMessage* message) {
  
  message->size = 0;
  while(serialConnection->available() > 0) {
    message->data[message->size] = serialConnection->read();
    message->size += 1;
  }
  
#if DEBUG
  this->debugPrint(message);
#endif
};

void EredesMeterConnection::debugPrint(MODBUSMessage* message) {
  // Fast debug
  for(uint8_t i=0;i<message->size;i++){
    Serial.print(message->data[i], HEX);
  }
  Serial.print("\n");
}

uint16_t EredesMeterConnection::computeCRC(byte* request, uint16_t lenght) {
  uint16_t crc = 0xffff;
 
  for (uint8_t pos = 0; pos < lenght; pos++) {
    crc ^= (uint16_t)request[pos];
 
    for (uint8_t i = 8; i != 0; i--) {
      if ((crc & 0x0001) != 0) { 
        crc >>= 1; 
        crc ^= 0xa001;
      }
      else {
        crc >>= 1;
      }
    }
  }
  return crc;  
};

void EredesMeterConnection::buildRequest(byte* request, uint16_t start, uint16_t length) {
  request[2] = start >> 8;
  request[3] = start;
  request[4] = length >> 8;
  request[5] = length;
  uint16_t crc = computeCRC(requestBuffer, 6);
  requestBuffer[6] = crc;
  requestBuffer[7] = crc >> 8;
}

void EredesMeterConnection::handleException(StaticJsonDocument<JSON_SIZE>* result, MODBUSMessage* messageBuffer, std::initializer_list<String> names) {
  String fieldName = "e";
  for (String name : names) {
    fieldName += "-" + name;
  }
  (*result)[fieldName] = messageBuffer->data[2];
}

void EredesMeterConnection::handleCRCError(StaticJsonDocument<JSON_SIZE>* result, MODBUSMessage* messageBuffer, uint16_t actual, uint16_t expected, std::initializer_list<String> names) {
  String fieldName = "t";
  for (String name : names) {
    fieldName += "-" + name;
  }
  (*result)[fieldName + "-e"] = expected;
  (*result)[fieldName + "-a"] = actual;
  (*result)[fieldName + "-ml"] = messageBuffer->size;
}

void EredesMeterConnection::handlePrimiteTypes(StaticJsonDocument<JSON_SIZE>* result, MODBUSMessage* messageBuffer, uint16_t scalar, EredesType type, std::initializer_list<String> names) {
  uint16_t b = 0;
  for (String name : names) {
    if(name != "") {
      uint32_t value = 0;
      for(uint8_t i = 0; i < type; i++) {
        value |= (messageBuffer->data[3 + b * type + i] << 8 * (type - i - 1)); 
      }
      (*result)[name] = (double)value / scalar;
    }
    b++;
  }
}

void EredesMeterConnection::readRegisters(StaticJsonDocument<JSON_SIZE>* result, uint16_t start, uint16_t scalar, EredesType type, std::initializer_list<String> names) {
  
  buildRequest(requestBuffer, start, names.size());
  writeRequest(requestBuffer);
  
  MODBUSMessage messageBuffer;
  readResponse(&messageBuffer);

  /* validate crc; send expected and actual;
  uint16_t expectedCRC = computeCRC(messageBuffer.data, messageBuffer.size - 2);
  uint16_t actualCRC = messageBuffer.data[messageBuffer.size - 1] << 8 |  messageBuffer.data[messageBuffer.size - 2];
  if(expectedCRC != actualCRC) {
    handleCRCError(result, &messageBuffer, actualCRC, expectedCRC, names);
    return;
  }*/
  
  // handle exception; just send the exception code; 
  byte functionCode = messageBuffer.data[1];
  if(functionCode != 0x4) {
    handleException(result, &messageBuffer, names);
    return;
  }

  handlePrimiteTypes(result, &messageBuffer, scalar, type, names);
}
