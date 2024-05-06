#ifndef PZEM017_deps_h
#define PZEM017_deps_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// #define PZEM017_NO_SWSERIAL
#if (not defined(PZEM017_NO_SWSERIAL)) && (defined(__AVR__) || defined(ESP8266) && (not defined(ESP32)))
#define PZEM017_SOFTSERIAL
#endif

#if defined(PZEM017_SOFTSERIAL)
#include <SoftwareSerial.h>
#endif

#endif