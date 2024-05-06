#ifndef Logger_h
#define Logger_h


/* ==================================================
** Libraries
**
** =============================================== */


#include "Logger.const.h"


/* ==================================================
** Class declaration
**
** =============================================== */


class Logger;


/* ==================================================
** Class definition
**
** =============================================== */


class Logger
{
    // Constructor
    public: Logger(
        HardwareSerial *LOG_PORT=LOG_PORT_DEFAULT, 
        uint32_t MONITOR_SPEED=MONITOR_SPEED_DEFAULT,
        bool flg_enable=true
    );

    public: Logger(bool flg_enable);

    // Methods
    public: void init();

    public: void print(const char *fmt);
    public: void printf(const char *fmt, ...);
    private: void printf(const char *fmt, va_list &args);

    public: void inf(const char *fmt, ...);
    public: void err(const char *fmt, ...);
    public: void upd(const char *fmt, ...);

    public: void enable();
    public: void disable();

    // Attributes
    private: bool flg_init;
    private: bool flg_upd;
    private: bool flg_enable;
    private: HardwareSerial *LOG_PORT;
    private: const uint32_t MONITOR_SPEED;
};


/* ==================================================
** Extern variables
**
** =============================================== */


//


#endif