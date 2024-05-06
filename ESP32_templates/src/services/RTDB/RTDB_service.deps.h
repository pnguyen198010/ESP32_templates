#ifndef RTDB_deps_h
#define RTDB_deps_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#if (defined(ESP32) || defined(ESP8266))
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#endif

#endif