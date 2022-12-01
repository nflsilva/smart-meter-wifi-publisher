#include "eredesMeter.h"

byte MODBUSMessage::getAddress() {
  return this->data[0];
};

byte MODBUSMessage::getFunction() {
  return this->data[1];
};

uint16_t MODBUSMessage::getCRC() {
  if(size < 2) return 0;
  return this->data[size-2];
};

EredesMeterConnection::EredesMeterConnection() {
  messageBuffer = new MODBUSMessage();
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
  Serial.print(message->getAddress(), HEX);
  Serial.print(" ");
  Serial.print(message->getFunction(), HEX);
  Serial.print(" ");
  /*
  Serial.print(length, DEC);
  Serial.print(" ");
  for(uint8_t i=0;i<length;i++){
    Serial.print(data[i], DEC);
    Serial.print(".");
  }
  Serial.print(" ");*/
  Serial.println(message->getCRC(), HEX);
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

void EredesMeterConnection::readRegisters(void* result, uint16_t start, uint16_t length, EredesType type) {

  requestBuffer[2] = start >> 8;
  requestBuffer[3] = start;
  requestBuffer[4] = length >> 8;
  requestBuffer[5] = length;
  
  computeRequestCRC(requestBuffer);

  writeRequest(requestBuffer);
  readResponse(messageBuffer);

  // handle exception; return if there's any;
  
  switch(type) {
    case Integer:
    break;
    case Long:
    break;
    case Double:
    break;
    default:
    break;
  
    
  }
  
  for(uint16_t b = 0; b < length; b++) {
    //result[b] = messageBuffer->data[3 + b] 
  }
  
}
