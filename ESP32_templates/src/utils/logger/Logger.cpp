/* ==================================================
** Libraries
**
** =============================================== */


#include "Logger.h"


/* ==================================================
** Logger: Constructors
**
** =============================================== */


Logger::Logger(
    HardwareSerial  *LOG_PORT, 
    uint32_t        MONITOR_SPEED,
    bool            flg_enable
)
    : flg_init(false)
    , flg_upd(false)
    , flg_enable(flg_enable)
    , LOG_PORT(LOG_PORT)
    , MONITOR_SPEED(MONITOR_SPEED)
{

    // if (!flg_enable) {
    //     return;
    // }
    
    // if(!(*LOG_PORT)){
    //     LOG_PORT->begin(MONITOR_SPEED);
    // }

    // while(!(*LOG_PORT));
    // LOG_PORT->print("\n\n\r");

    // flg_init = true;
}


Logger::Logger(bool flg_enable)
    : flg_init(false)
    , flg_upd(false)
    , flg_enable(flg_enable)
    , LOG_PORT(LOG_PORT_DEFAULT)
    , MONITOR_SPEED(MONITOR_SPEED_DEFAULT)
{

    // if (!flg_enable) {
    //     return;
    // }
    
    // if(!(*LOG_PORT)){
    //     LOG_PORT->begin(MONITOR_SPEED);
    // }
    
    // while(!(*LOG_PORT));
    // LOG_PORT->print("\n\n\r");

    // flg_init = true;
}


/* ==================================================
** Logger: Methods
**
** =============================================== */


void Logger::init(){

    if(flg_init){
        return;
    }

    flg_init = true;
    
    LOG_PORT->begin(MONITOR_SPEED);
    while(!(*LOG_PORT));
    LOG_PORT->print("\n\n\r");

}


void Logger::print(const char *fmt) {
    if(!flg_enable) {
        return;
    }

    if(flg_upd){
        LOG_PORT->println();
        flg_upd = false;
    }

    LOG_PORT->print(fmt);
}


void Logger::printf(const char *fmt, ...) {
    if(!flg_enable) {
        return;
    }

    if(flg_upd){
        LOG_PORT->println();
        flg_upd = false;
    }

    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;

    va_start(arg, fmt);
    va_copy(copy, arg);

    int len = vsnprintf(temp, sizeof(loc_buf), fmt, copy);
    va_end(copy);

    if(len < 0) {
        va_end(arg);
        return;
    };

    if((unsigned long)len >= sizeof(loc_buf)){
        temp = (char*) malloc(len+1);
        if(temp == NULL) {
            va_end(arg);
            return;
        }
        len = vsnprintf(temp, len+1, fmt, arg);
    }

    va_end(arg);
    len = LOG_PORT->write((uint8_t*)temp, len);

    if(temp != loc_buf){
        free(temp);
    }
}


void Logger::printf(const char *fmt, va_list &args) {
    if(!flg_enable) {
        return;
    }

    if(flg_upd){
        LOG_PORT->println();
        flg_upd = false;
    }

    char loc_buf[64];
    char * temp = loc_buf;

    va_list copy;
    va_copy(copy, args);

    int len = vsnprintf(temp, sizeof(loc_buf), fmt, copy);
    va_end(copy);

    if(len < 0) {
        va_end(args);
        return;
    };

    if((unsigned long)len >= sizeof(loc_buf)){
        temp = (char*) malloc(len+1);
        if(temp == NULL) {
            va_end(args);
            return;
        }
        len = vsnprintf(temp, len+1, fmt, args);
    }

    va_end(args);
    len = LOG_PORT->write((uint8_t*)temp, len);

    if(temp != loc_buf){
        free(temp);
    }
}


void Logger::inf(const char *fmt, ...) {
    if(!flg_enable) {
        return;
    }

    // Kiểm tra xem đã cập nhật chưa
    if (flg_upd) {
        // Cập nhật trạng thái
        flg_upd = false;

        // In xuống dòng
        LOG_PORT->println();
    }

    // In thời gian
    printf("I (%.3Lf) ", (long double)millis() / 1000);

    // In thông tin ra serial
    va_list args;
    va_start(args, fmt);

    // In nội dung thông tin
    printf(fmt, args);

    // In xuống dòng
    LOG_PORT->println();
    
    // Dọn dẹp bộ nhớ
    va_end(args);
}


void Logger::err(const char *fmt, ...) {
    if(!flg_enable) {
        return;
    }

    // Kiểm tra xem đã cập nhật chưa
    if (flg_upd) {
        // Cập nhật trạng thái
        flg_upd = false;

        // In xuống dòng
        LOG_PORT->println();
    }

    // In thời gian
    printf("E (%.3Lf) ", (long double)millis() / 1000);

    // In thông tin ra serial
    va_list args;
    va_start(args, fmt);

    // In nội dung thông tin
    printf(fmt, args);

    // In xuống dòng
    LOG_PORT->println();
    
    // Dọn dẹp bộ nhớ
    va_end(args);
}


void Logger::upd(const char *fmt, ...) {
    if(!flg_enable) {
        return;
    }

    flg_upd = true;

    // In thời gian
    printf("U (%.3Lf) ", (long double)millis() / 1000);

    // In thông tin ra serial
    va_list args;
    va_start(args, fmt);

    // In nội dung thông tin
    printf(fmt, args);

    // Con trỏ quay về đầu dòng
    LOG_PORT->print("\r");
    
    // Dọn dẹp bộ nhớ
    va_end(args);
}


void Logger::enable() {
    flg_enable = true;
}


void Logger::disable() {
    flg_enable = false;
}