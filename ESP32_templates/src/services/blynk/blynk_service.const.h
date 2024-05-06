#ifndef blynk_service_const_h
#define blynk_service_const_h


/* ==================================================
** Libraries
**
** =============================================== */


//


/* ==================================================
** Macro definition
**
** =============================================== */


#define BLYNK_TEMPLATE_ID "TMPL6eRD-LVW6"
#define BLYNK_TEMPLATE_NAME "Battery collect"
#define BLYNK_AUTH_TOKEN "uZXZlGy-3uirppZcPdbQFvtdwp2wY1k-"

#define BLYNK_PRINT Serial

#define BLYNK_INTERVAL  1000L


#define BLYNK_VPIN_RELAY_SOURCE V1

#if     BLYNK_VPIN_RELAY_SOURCE == V1
#define BLYNK_VPIN_RELAY_LOAD   V2
#else
#define BLYNK_VPIN_RELAY_LOAD   V1
#endif


#define BLYNK_VPIN_VOLTAGE_SOURCE V3
#define BLYNK_VPIN_CURRENT_SOURCE V4
#define BLYNK_VPIN_POWER_SOURCE   V5
#define BLYNK_VPIN_ENERGY_SOURCE  V6


#if     BLYNK_VPIN_VOLTAGE_SOURCE == V3
#define BLYNK_VPIN_VOLTAGE_LOAD  V7
#else
#define BLYNK_VPIN_VOLTAGE_LOAD V3
#endif

#if     BLYNK_VPIN_CURRENT_SOURCE == V4
#define BLYNK_VPIN_CURRENT_LOAD  V8
#else
#define BLYNK_VPIN_CURRENT_LOAD V4
#endif

#if     BLYNK_VPIN_POWER_SOURCE == V5
#define BLYNK_VPIN_POWER_LOAD  V9
#else
#define BLYNK_VPIN_POWER_LOAD V5
#endif

#if     BLYNK_VPIN_ENERGY_SOURCE == V6
#define BLYNK_VPIN_ENERGY_LOAD  V10
#else
#define BLYNK_VPIN_ENERGY_LOAD V10
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