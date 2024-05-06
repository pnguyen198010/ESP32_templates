/* ==================================================
** Libraries
**
** =============================================== */


#include "wifi_service.deps.h"
#include "wifi_service.const.h"
#include "wifi_service.h"

#include "utils/logger/Logger.h"


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

static String SSID = "";
static String PASS = "";


/* ==================================================
** Static function declaration
**
** =============================================== */


static void wait();
static void connect(const char* ssid=WIFI_DEFAULT_SSID, const char* pass=WIFI_DEFAULT_PASS);

static bool is_validSSID(const char* ssid);
static bool is_validPass(const char* pass);
static bool is_sameWiFi(const char* ssid, const char* pass);


/* ==================================================
** Static function definition
**
** =============================================== */


// Wait for conncting
static void wait(){
    Log.print("\n");
    Log.inf("[WiFi] Connecting to '%s'", SSID.c_str());

    for (uint8_t i = 0; i < WIFI_NUM_TRY && WiFi.status() != WL_CONNECTED; ++i){
        delay(1000);
    }

    if(WiFi.status() != WL_CONNECTED){
        Log.inf("[WiFi] Connecting failed\n");
        return;
    }

    Log.inf("[WiFi] Connecting successed\n");
}


static void connect(const char* ssid, const char* pass)
{
    if (is_validSSID(ssid) == false){
        Log.err("[WiFi] SSID is empty");
        return;
    }

    if (is_validPass(pass) == false){
        Log.err("[WiFi] Password is less than 8 characters");
        return;
    }

    if(is_sameWiFi(ssid, pass)){

        if(WiFi.status() == WL_CONNECTED){
            // Log.inf("[WiFi] Connected to '%s'", ssid);
            return;
        }

        wait();
        return;
    }


    SSID = ssid;
    PASS = pass;

    WiFi.mode(WiFi.getMode() == WIFI_AP ? WIFI_AP_STA : WIFI_STA);
    WiFi.begin(ssid, pass);
    wait();
}


static bool is_validSSID(const char* ssid)
{
    return strlen(ssid)==0 ? false : true;
}


bool is_validPass(const char* pass)
{
    size_t len = strlen(pass);

    if(len == 0)                    {return true;}
    if(len >= WIFI_LENGTH_PASS_MIN) {return true;}

    return false;
}


static bool is_sameWiFi(const char* ssid, const char* pass){

    if (SSID != ssid){
        return false;
    }

    if(PASS != pass){
        return false;
    }

    return true;
}


void WiFiEvent(WiFiEvent_t event)
{
    // Log.printf("[WiFi-event] event: %d\n", event);

    switch (event) {
        case ARDUINO_EVENT_WIFI_READY: 
            // Log.print("[WiFi] interface ready\n");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            Log.print("[WiFi] Completed scan for access points\n");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            Log.print("[WiFi] clients started\n");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Log.print("[WiFi] clients stopped\n");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Log.print("[WiFi] Connected to access point\n");
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Log.print("[WiFi] Disconnected from WiFi access point\n");
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Log.print("[WiFi] Authentication mode of access point has changed\n");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Log.print("[WiFi] Obtained IP address: ");
            Log.print(WiFi.localIP().toString().c_str());
            Log.print("\n");
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Log.print("[WiFi] Lost IP address and IP address is reset to 0\n");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            Log.print("[WiFi] WiFi Protected Setup (WPS): succeeded in enrollee mode\n");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Log.print("[WiFi] WiFi Protected Setup (WPS): failed in enrollee mode\n");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Log.print("[WiFi] WiFi Protected Setup (WPS): timeout in enrollee mode\n");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            Log.print("[WiFi] WiFi Protected Setup (WPS): pin code in enrollee mode\n");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            Log.print("[WiFi] access point started\n");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Log.print("[WiFi] access point  stopped\n");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Log.print("[WiFi] Client connected\n");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Log.print("[WiFi] Client disconnected\n");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Log.print("[WiFi] Assigned IP address to client\n");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Log.print("[WiFi] Received probe request\n");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Log.print("[WiFi] AP IPv6 is preferred\n");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Log.print("[WiFi] STA IPv6 is preferred\n");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            Log.print("[WiFi] Ethernet IPv6 is preferred\n");
            break;
        case ARDUINO_EVENT_ETH_START:
            Log.print("[WiFi] Ethernet started\n");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Log.print("[WiFi] Ethernet stopped\n");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Log.print("[WiFi] Ethernet connected\n");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Log.print("[WiFi] Ethernet disconnected\n");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Log.print("[WiFi] Obtained IP address\n");
            break;
        default: break;
    }}



/* ==================================================
** Extern function definition
**
** =============================================== */


#ifdef ESP32
void WiFi_init(){
    Log.print("\n");
    Log.inf("[WiFi] init...");

    // Delete old config
    WiFi.disconnect(true);   delay(1000);
    WiFi.onEvent(WiFiEvent);

    connect();

    Log.inf("[WiFi] init... ok\n");
}
#endif


#ifdef ESP32
void WiFi_reconnect(){
    connect(SSID.c_str(), PASS.c_str());
}
#endif


#ifdef ESP32
void WiFi_connect(const char* ssid, const char* pass){
    connect(ssid, pass);
}
#endif