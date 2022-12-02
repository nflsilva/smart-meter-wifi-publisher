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

void EredesMeterConnection::computeRequestCRC(byte* request) {
  uint16_t crc = 0xFFFF;
 
  for (uint8_t pos = 0; pos < 6; pos++) {
    crc ^= (uint16_t)request[pos];
 
    for (uint8_t i = 8; i != 0; i--) {
      if ((crc & 0x0001) != 0) { 
        crc >>= 1; 
        crc ^= 0xA001;
      }
      else {
        crc >>= 1;
      }
    }
  }

  requestBuffer[6] = crc;
  requestBuffer[7] = crc >> 8;
  
};

void EredesMeterConnection::readRegisters(StaticJsonDocument<JSON_SIZE>* result, uint16_t start, uint16_t length, EredesType type, String* names) {

  requestBuffer[2] = start >> 8;
  requestBuffer[3] = start;
  requestBuffer[4] = length >> 8;
  requestBuffer[5] = length;
  computeRequestCRC(requestBuffer);

  writeRequest(requestBuffer);
  
  MODBUSMessage messageBuffer;
  readResponse(&messageBuffer);

  // handle exception; just send the content 
  byte functionCode = messageBuffer.data[1];
  if(functionCode != 0x4) {
    String fieldName = "exception";
    for(uint16_t i = 0; i <length; i++) {
      fieldName += "-" + names[i];
    }
    (*result)[fieldName] = messageBuffer.data[2];
    return;
  }
    
  for(uint16_t b = 0; b < length; b++) {
    uint32_t value = 0;
    for(uint8_t i = 0; i < type; i++) {
      value |= (messageBuffer.data[3 + b * type + i] << 8 * (type - i - 1)); 
    }
    (*result)[names[b]] = value;
  }
  
}
