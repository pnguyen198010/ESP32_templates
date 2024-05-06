#ifndef relay_service_h
#define relay_service_h


/* ==================================================
** Libraries
**
** =============================================== */


//


/* ==================================================
** Extern function declaration
**
** =============================================== */


void Relay_init();

void Relay_set_powerState_source(int state);
void Relay_set_powerState_load(int state);

void Relay_powerOff_source();
void Relay_powerOn_source();

void Relay_powerOff_load();
void Relay_powerOn_load();



/* ==================================================
** Extern variables
**
** =============================================== */


//


#endif