/* ==================================================
** Libraries
**
** =============================================== */


#include "NTP_service.deps.h"
#include "NTP_service.const.h"
#include "NTP_service.h"

#include "utils/logger/logger.h"


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


// static Logger Log(true);

#ifdef ESP32
static WiFiUDP udp_ntp;
static NTPClient timeClient(udp_ntp, NTP_POOL_SERVER_NAME);
#endif


/* ==================================================
** Static function declaration
**
** =============================================== */


//


/* ==================================================
** Static function definition
**
** =============================================== */


//


/* ==================================================
** Extern function definition
**
** =============================================== */


void NTP_init(){
  timeClient.begin();
  timeClient.setTimeOffset(NTP_TIME_OFFSET);
}


unsigned long NTP_get_epochTime(){
  timeClient.update();
  return timeClient.getEpochTime();
}