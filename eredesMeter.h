#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct TimeResponse {
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} TimeResponse_t;

void parseTimeResponse(uint8_t* data, TimeResponse_t* response);
