/* ==================================================
** Libraries
**
** =============================================== */


#include "blynk_service.const.h"
#include "blynk_service.deps.h"
#include "blynk_service.h"

#include "utils/logger/Logger.h"
#include "services/PZEM017/PZEM017_service.h"
#include "services/relay/relay_service.h"


/* ==================================================
** Macro definition
**
** =============================================== */


//


/* ==================================================
** Type definition
**
** =============================================== */


//


/* ==================================================
** Extern variables
**
** =============================================== */


//


/* ==================================================
** Static variables
**
** =============================================== */


static Logger Log(true);
static BlynkTimer timer;


/* ==================================================
** Static function declaration
**
** =============================================== */


//


/* ==================================================
** Static function definition
**
** =============================================== */


BLYNK_CONNECTED() {
  Blynk.syncVirtual(BLYNK_VPIN_RELAY_SOURCE);
  Blynk.syncVirtual(BLYNK_VPIN_RELAY_LOAD);
}


BLYNK_WRITE(BLYNK_VPIN_RELAY_SOURCE) {
    int state = param.asInt();
    Relay_set_powerState_source(state);
    Log.inf("[Blynk] Power of source relay: %s\n", state ? "on" : "off");
}

BLYNK_WRITE(BLYNK_VPIN_RELAY_LOAD) {
    int state = param.asInt();
    Relay_set_powerState_load(state);
    Log.inf("[Blynk] Power of load relay: %s\n", state ? "on" : "off");
}

void send_Blynk() {

  Log.print("\n");
  Log.inf("[Blynk] send data... start");

  // PZEM_source
  // voltage_0, current_0, power_0, energy_0;
  Blynk.virtualWrite(BLYNK_VPIN_VOLTAGE_SOURCE, isnan(PZEM017_get_sourceVoltage()) ? 0 : PZEM017_get_sourceVoltage());
  Blynk.virtualWrite(BLYNK_VPIN_CURRENT_SOURCE, isnan(PZEM017_get_sourceCurrent()) ? 0 : PZEM017_get_sourceCurrent());
  Blynk.virtualWrite(BLYNK_VPIN_POWER_SOURCE,   isnan(PZEM017_get_sourcePower()) ? 0 : PZEM017_get_sourcePower());
  Blynk.virtualWrite(BLYNK_VPIN_ENERGY_SOURCE,  isnan(PZEM017_get_sourceEnergy()) ? 0 : PZEM017_get_sourceEnergy());

  // PZEM_load
  // voltage_1, current_1, power_1, energy_1;
  Blynk.virtualWrite(BLYNK_VPIN_VOLTAGE_LOAD, isnan(PZEM017_get_loadVoltage()) ? 0 : PZEM017_get_loadVoltage());
  Blynk.virtualWrite(BLYNK_VPIN_CURRENT_LOAD, isnan(PZEM017_get_loadCurrent()) ? 0 : PZEM017_get_loadCurrent());
  Blynk.virtualWrite(BLYNK_VPIN_POWER_LOAD,   isnan(PZEM017_get_loadPower()) ? 0 : PZEM017_get_loadPower());
  Blynk.virtualWrite(BLYNK_VPIN_ENERGY_LOAD,  isnan(PZEM017_get_loadEnergy()) ? 0 : PZEM017_get_loadEnergy());

  Log.inf("[Blynk] send data... done");
  Log.print("\n");
}


/* ==================================================
** Extern function definition
**
** =============================================== */


void Blynk_init(){
    timer.setInterval(BLYNK_INTERVAL, send_Blynk);
    Blynk.config(BLYNK_AUTH_TOKEN);
}


void Blynk_run(){
    Blynk.run();
    timer.run();
}

