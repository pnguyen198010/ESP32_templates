/* ==================================================
** Libraries
**
** =============================================== */


#include "RTDB_service.deps.h"
#include "RTDB_service.const.h"
#include "RTDB_service.h"

#include "utils/logger/Logger.h"
#include "services/relay/relay_service.h"
#include "services/NTP/NTP_service.h"
#include "services/PZEM017/PZEM017_service.h"
#include "services/wifi/wifi_service.h"


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

static FirebaseAuth auth;
static FirebaseConfig config;

static FirebaseData fbdo;
static FirebaseJson json;


/* ==================================================
** Static function declaration
**
** =============================================== */


static bool get_sheetSize(uint32_t* size);

static void upd_json();
static void upd_webData();
static void upd_sheetData(uint32_t size);
static void upd_sheetSize(uint32_t size);

static void evt_tokenStatus(TokenInfo info);


/* ==================================================
** Static function definition
**
** =============================================== */


bool get_sheetSize(uint32_t* size){
    Log.print("\n");
    Log.inf("[RTDB] Get sheet size... start");

    Log.inf("[RTDB] Get size...");
    if(!Firebase.RTDB.getInt(&fbdo, RTDB_PATH_SHEET_SIZE, size)){
      Log.err("[RTDB] Get sheet size... %s", fbdo.errorReason().c_str());

      if(fbdo.errorCode() != FIREBASE_ERROR_PATH_NOT_EXIST){
        Log.inf("[RTDB] Get sheet size... done");
        Log.print("\n");
        return false;
      }

      *size = 0;
    }
    
    else{
      Log.inf("[RTDB] Get size... ok");
      Log.inf("[RTDB] Sheet size: %d", *size);
    }

    Log.inf("[RTDB] Get sheet size... done");
    Log.print("\n");
    return true;
}


void upd_json(){

    json.set(RTDB_PATH_SOURCE_VOLTAGE, String(PZEM017_get_sourceVoltage(), 2));
    json.set(RTDB_PATH_SOURCE_CURRENT, String(PZEM017_get_sourceCurrent(), 2));
    json.set(RTDB_PATH_SOURCE_POWER,   String(PZEM017_get_sourcePower(),   2));
    json.set(RTDB_PATH_SOURCE_ENERGY,  String(PZEM017_get_sourceEnergy(),  2));

    json.set(RTDB_PATH_LOAD_VOLTAGE, String(PZEM017_get_loadVoltage(), 2));
    json.set(RTDB_PATH_LOAD_CURRENT, String(PZEM017_get_loadCurrent(), 2));
    json.set(RTDB_PATH_LOAD_POWER,   String(PZEM017_get_loadPower(),   2));
    json.set(RTDB_PATH_LOAD_ENERGY,  String(PZEM017_get_loadEnergy(),  2));

    unsigned long epochTime = NTP_get_epochTime();
    json.set(RTDB_PATH_TIMESTAMP, String(epochTime));

    Log.inf("[RTDB][upd_json] source: %.2f | %.2f | %.2f | %.2f", 
        PZEM017_get_sourceVoltage(),
        PZEM017_get_sourceCurrent(),
        PZEM017_get_sourcePower(),
        PZEM017_get_sourceEnergy()
    );
    
    Log.inf("[RTDB][upd_json] load: %.2f | %.2f | %.2f | %.2f", 
        PZEM017_get_loadVoltage(),
        PZEM017_get_loadCurrent(),
        PZEM017_get_loadPower(),
        PZEM017_get_loadEnergy()
    );

}


void upd_webData(){
    Log.print("\n");
    Log.inf("[RTDB] Update web data... start");

    FirebaseJsonArray arr;
    arr.setFloatDigits(4);
    arr.setDoubleDigits(4);

    Log.inf("[RTDB] Get array...");
    if(!Firebase.RTDB.getArray(&fbdo, RTDB_PATH_WEB_DATA, &arr)){
        Log.err("[RTDB] Get array... error %d: %s",fbdo.errorCode(), fbdo.errorReason().c_str());

        if(fbdo.errorCode() != FIREBASE_ERROR_PATH_NOT_EXIST
        || fbdo.errorCode() == FIREBASE_ERROR_HTTP_CODE_UNDEFINED
        ){
            Log.inf("[RTDB] Update web data... done");
            Log.print("\n");
            return;
        }
    } 
    
    Log.inf("[RTDB] Get array... ok");
    Log.inf("[RTDB] Array size: %d", arr.size());

    if(arr.size() < 10){
      arr.add(json);
    }

    else{
      arr.remove(0);
      arr.add(json);
    }

    Log.inf("[RTDB] Set array... %s", Firebase.RTDB.setArray(&fbdo, RTDB_PATH_WEB_DATA, &arr) ? "ok" : fbdo.errorReason().c_str());
    
    Log.inf("[RTDB] Update web data... done");
    Log.print("\n");
}


void upd_sheetData(uint32_t size){
    Log.print("\n");
    Log.inf("[RTDB] Update sheet data... start");

    String path = String(RTDB_PATH_SHEET_DATA);
           path += "/";
           path += String(size);

    Log.inf("[RTDB] Set json... %s", Firebase.RTDB.setJSON(&fbdo, path, &json) ? "ok" : fbdo.errorReason().c_str());

    Log.inf("[RTDB] Update sheet data... done");
    Log.print("\n");
}


void upd_sheetSize(uint32_t size){
    Log.print("\n");
    Log.inf("[RTDB] Update sheet size... start");

    size += 1;
    Log.inf("[RTDB] Set int... %s", Firebase.RTDB.setInt(&fbdo, RTDB_PATH_SHEET_SIZE, size) ? "ok" : fbdo.errorReason().c_str());

    Log.inf("[RTDB] Update sheet size... done");
    Log.print("\n");
}


void evt_tokenStatus(TokenInfo info)
{
    /** firebase_auth_token_status enum
     * token_status_uninitialized,
     * token_status_on_initialize,
     * token_status_on_signing,
     * token_status_on_request,
     * token_status_on_refresh,
     * token_status_ready,
     * token_status_error
     */
    if (info.status == token_status_error)
    {
        Serial_Printf("Token info: type = %s, status = %s\n", getTokenType(info), getTokenStatus(info));
        Serial_Printf("Token error: %s\n", getTokenError(info).c_str());

        WiFi_reconnect();
    }
    else
    {
        Serial_Printf("Token info: type = %s, status = %s\n", getTokenType(info), getTokenStatus(info));
    }
}


/* ==================================================
** Extern function definition
**
** =============================================== */


void RTDB_init(){
    Log.print("\n");
    Log.inf("[RTDB] Init... start");

    auth.user.email    = RTDB_USER_EMAIL;
    auth.user.password = RTDB_USER_PASSWORD;

    config.api_key                    = RTDB_API_KEY;
    config.database_url               = RTDB_URL;
    config.max_token_generation_retry = RTDB_MAX_TOKEN_GENERATION_RETRY;
    config.token_status_callback      = evt_tokenStatus; // See addons/TokenHelper.h

    fbdo.setBSSLBufferSize(RTDB_SIZE_RESPONSE, 1024);
    json.setFloatDigits(4);
    json.setDoubleDigits(4);

    Firebase.reconnectWiFi(true);
    Firebase.begin(&config, &auth);

    Log.inf("[RTDB] Getting user UID");
    while(auth.token.uid == ""){
        delay(1000);
    }

    Log.inf("[RTDB] User UID: %s", auth.token.uid.c_str());
    Log.inf("[RTDB] Init... done");
    Log.print("\n");
}


void RTDB_run(){

    if(!Firebase.ready()){
        Log.upd("[RTDB] Firebase isn't ready");
        return;
    }

    static unsigned long millis_prev = 0 - millis();

    if(millis() - millis_prev < RTDB_TIMEOUT_UPD){
        return;
    }

    millis_prev = millis();

    upd_json();
    upd_webData();

    uint32_t size = 0;
    if(!get_sheetSize(&size)){
        return;
    }

    upd_sheetData(size);
    upd_sheetSize(size);

    #if true
    if(size % 20 == 19){
        Relay_powerOff_load();
    }

    else if(size % 20 == 9){
        Relay_powerOn_load();
    }
    #endif
}