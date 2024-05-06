#ifndef wifi_service_deps_h
#define wifi_service_deps_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#ifdef ESP8266
#include "ESP8266WiFi.h"
#endif

#ifdef ESP32
#include "WiFi.h"
#endif


#endif