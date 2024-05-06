#ifndef blynk_deps_h
#define blynk_deps_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#ifdef ESP32
#include <BlynkSimpleEsp32.h>
#endif


#endif