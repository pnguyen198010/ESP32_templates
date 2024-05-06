#ifndef relay_service_const_h
#define relay_service_const_h


/* ==================================================
** Libraries
**
** =============================================== */


#include <Arduino.h>


/* ==================================================
** Macro definition
**
** =============================================== */


#define RELAY_NO                0
#define RELAY_NC                1
#define RELAY_VCC               RELAY_NC
#define RELAY_TRIGGER_LEVEL     HIGH

// If there's current normally
#if RELAY_VCC == RELAY_NC
    // Don't need to trigger relay for power
    #define RELAY_STATE_POWER_ON      (!RELAY_TRIGGER_LEVEL)
    // Trigger relay to interrupt power
    #define RELAY_STATE_POWER_OFF     (RELAY_TRIGGER_LEVEL)

// If there's no current normally
#elif RELAY_VCC == RELAY_NO
    // Trigger relay for power
    #define RELAY_STATE_POWER_ON      (RELAY_TRIGGER_LEVEL)
    // Don't  need trigger relay to interrupt power
    #define RELAY_STATE_POWER_OFF     (!RELAY_TRIGGER_LEVEL)
#endif


/* ==================================================
** Type definition
**
** =============================================== */


//


/* ==================================================
** Extern function declaration
**
** =============================================== */


//


#endif