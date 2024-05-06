/* ==================================================
** Libraries
**
** =============================================== */


#include "relay_service.const.h"
#include "relay_service.h"

#include "common/pin_table.h"


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


//


/* ==================================================
** Static function declaration
**
** =============================================== */


static inline void relay_powerOff(uint8_t relayPin);
static inline void relay_powerOn(uint8_t relayPin);


/* ==================================================
** Static function definition
**
** =============================================== */


void relay_powerOff(uint8_t relayPin){
    digitalWrite(relayPin, RELAY_STATE_POWER_OFF);
}


void relay_powerOn(uint8_t relayPin){
    digitalWrite(relayPin, RELAY_STATE_POWER_ON);
}


/* ==================================================
** Extern function definition
**
** =============================================== */


void Relay_init(){
    pinMode(PIN_RELAY_SOURCE, OUTPUT);
    pinMode(PIN_RELAY_LOAD, OUTPUT);

    Relay_powerOn_source();
    Relay_powerOff_load();
}


void Relay_set_powerState_source(int state){
    if(state){
        relay_powerOn(PIN_RELAY_SOURCE);
        return;
    }

    relay_powerOff(PIN_RELAY_SOURCE);
}


void Relay_set_powerState_load(int state){
    if(state){
        relay_powerOn(PIN_RELAY_LOAD);
        return;
    }

    relay_powerOff(PIN_RELAY_LOAD);
}


void Relay_powerOff_source(){
    relay_powerOff(PIN_RELAY_SOURCE);
}


void Relay_powerOn_source(){
    relay_powerOn(PIN_RELAY_SOURCE);
}


void Relay_powerOff_load(){
    relay_powerOff(PIN_RELAY_LOAD);
}


void Relay_powerOn_load(){
    relay_powerOn(PIN_RELAY_LOAD);
}
