/* ==================================================
** Libraries
**
** =============================================== */


#include "PZEM017_service.h"
#include "components/PZEM017/PZEM017.h"

#include "utils/logger/Logger.h"
#include "services/relay/relay_service.h"


/* ==================================================
** Macro definition
**
** =============================================== */


// Number of PZEM017
#define PZEM017_NUM_DEVICES 2


#define PZEM017_ADDR_SOURCE 0x01

#if     PZEM017_ADDR_SOURCE == 0x01
#define PZEM017_ADDR_LOAD   0x02
#else
#define PZEM017_ADDR_LOAD   0x01
#endif



/* ==================================================
** Type definition
**
** =============================================== */


typedef struct {
    // In volt
    float voltage;
    // In Ampe
    float current;
    // In W
    float power;
    // In Wh
    float energy;
    
} pzem_data_t;


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

static PZEM017 pzem017_source (&Serial2, PZEM017_ADDR_SOURCE);
static PZEM017 pzem017_load (&Serial2, PZEM017_ADDR_LOAD);

static pzem_data_t source, load;

static unsigned long intv_waitPZEM017 = 0 - millis();


/* ==================================================
** Static function declaration
**
** =============================================== */


static uint8_t get_enablePZEM017();


/* ==================================================
** Static function definition
**
** =============================================== */


static uint8_t get_enablePZEM017(){
    
    const uint8_t min = 1;
    const uint8_t max = PZEM017_NUM_DEVICES;
    const unsigned long timeout = 300;

    static uint8_t enable = 1;

    if(millis() - intv_waitPZEM017 < timeout){
        return 0;
    }

    intv_waitPZEM017 = millis();

    enable += 1;
    enable = enable > max ? min : enable;

    return enable;
}


/* ==================================================
** Extern function definition
**
** =============================================== */


void PZEM017_init(){
    Log.print("\n");
    Log.inf("[PZEM017] init... start");

    Log.inf("[PZEM017] update parameters");
    pzem017_source.upd_parameters(); delay(200);
    pzem017_load.upd_parameters(); delay(200);

    Log.inf("[PZEM017] reset energey");
    pzem017_source.rst_energy(); delay(200);
    pzem017_load.rst_energy();

    Log.print("\n");
    Log.inf("[PZEM017] init... done");
}


void PZEM017_read(){
    
    uint8_t enable = get_enablePZEM017();

    if(enable == 1){
        source.voltage = pzem017_source.get_voltage();
        source.current = pzem017_source.get_current();
        source.power   = source.voltage*source.current;
        source.energy  = pzem017_source.get_energy();

        intv_waitPZEM017 = millis();
    }
    
    else if(enable == 2){
        load.voltage = pzem017_load.get_voltage();
        load.current = pzem017_load.get_current();
        load.power   = load.voltage*load.current;
        load.energy  = pzem017_load.get_energy();

        intv_waitPZEM017 = millis();
    }

    #if true
    if(!isnan(load.voltage) && load.voltage < 12){
        Relay_powerOff_source();
    }

    else{
        Relay_powerOn_source();
    }
    #endif
}


float PZEM017_get_sourceVoltage(){
    // source.voltage = pzem017_source.get_voltage();
    return source.voltage;
}


float PZEM017_get_sourceCurrent(){
    // source.current = pzem017_source.get_current();
    return source.current;
}


float PZEM017_get_sourcePower(){
    // source.power = pzem017_source.get_power();
    return source.power;
}


float PZEM017_get_sourceEnergy(){
    // source.energy = pzem017_source.get_energy();
    return source.energy;
}


float PZEM017_get_loadVoltage(){
    // load.voltage = pzem017_load.get_voltage();
    return load.voltage;
}


float PZEM017_get_loadCurrent(){
    // load.current = pzem017_load.get_current();
    return load.current;
}


float PZEM017_get_loadPower(){
    // load.power = pzem017_load.get_power();
    return load.power;
}


float PZEM017_get_loadEnergy(){
    // load.energy = pzem017_load.get_energy();
    return load.energy;
}

