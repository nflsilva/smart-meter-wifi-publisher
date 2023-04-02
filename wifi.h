#pragma once

#include <ESP8266WiFi.h>

#include "secrets.h"

WiFiClient* createWiFiConnection();
bool ensureWiFiConnection();
