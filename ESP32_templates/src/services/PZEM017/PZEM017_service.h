#ifndef PZEM017_service_h
#define PZEM017_service_h


void PZEM017_init();
void PZEM017_read();

float PZEM017_get_sourceVoltage();
float PZEM017_get_sourceCurrent();
float PZEM017_get_sourcePower();
float PZEM017_get_sourceEnergy();

float PZEM017_get_loadVoltage();
float PZEM017_get_loadCurrent();
float PZEM017_get_loadPower();
float PZEM017_get_loadEnergy();


#endif