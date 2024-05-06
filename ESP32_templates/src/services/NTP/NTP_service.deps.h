#ifndef NTP_service_h
#define NTP_service_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#ifdef ESP32
#include <WiFiUdp.h>
#include <NTPClient.h>
#endif



#endif