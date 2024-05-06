#ifndef PZEM017_h
#define PZEM017_h


/* ==================================================
** Libraries
**
** =============================================== */


#include "PZEM017.deps.h"
#include "PZEM017.const.h"


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
** Class declaration
**
** =============================================== */


class PZEM017;


/* ==================================================
** Class definition
**
** =============================================== */


class PZEM017
{

    // Constructors
    ///////////////
    #if defined(PZEM017_SOFTSERIAL)
    public: PZEM017(uint8_t RX, uint8_t TX, uint8_t addr=PZEM_DEFAULT_ADDR);
    #endif

    public: PZEM017(HardwareSerial* port, uint8_t addr=PZEM017_ADDR_DEFAULT);


    // Destructors
    ///////////////
    public: ~PZEM017();


    // Getters
    ///////////////
    public: float get_voltage();
    public: float get_current();
    public: float get_power();
    public: float get_energy();

    public: float    get_threshold_HVA();
    public: float    get_threshold_LVA();
    public: uint16_t get_shuntType();

    public: uint8_t get_address();
    public: uint64_t get_timeoutUpdate();


    // Setters
    ///////////////
    public: bool  set_address(uint8_t addr, uint16_t slaveAddr=0xFFFF);
    public: bool  set_threshold_LVA(uint16_t volt);
    public: bool  set_threshold_HVA(uint16_t volt);
    public: bool  set_shuntType(uint16_t shunt);

    public: bool  set_timeoutUpdate(uint64_t timeout);
    private: void set_CRC(uint8_t *buf, uint16_t len);           // Set the CRC for a buffer


    // Checkers
    ///////////////
    public: bool  check_highVoltAlarm();
    public: bool  check_lowVoltAlarm();
    private: bool check_CRC(const uint8_t *buf, uint16_t len);   // Check CRC of buffer


    // Updaters
    ///////////////
    public: bool  upd_parameters();
    private: bool upd_measure();    // Get most up to date values from device registers and cache them


    // Reseters
    ///////////////
    public: bool rst_energy();


    // Methods
    ///////////////
    private: void init(uint8_t addr); // Init common to all constructors
    public: void search();

    private: bool sendCmd_8B(
        uint8_t cmd, 
        uint16_t rAddr,
        uint16_t val, 
        bool check = false, 
        uint16_t slave_addr = 0xFFFF
    ); 

    private:  bool sendCmd_readMeasure(
        uint16_t rReg       = PZEM017_RREG_FIRST, 
        uint16_t numRReg    = PZEM017_NUM_MEASUREMENT_RESULT,
        uint16_t slave_addr = 0xFFFF
    );

    private:  bool sendCmd_readParameter(
        uint16_t wReg       = PZEM017_WREG_FIRST, 
        uint16_t numWReg    = PZEM017_NUM_SLAVE_PARAMETER,
        uint16_t slave_addr = 0xFFFF
    );

    private:  bool sendCmd_writeParameter(
        uint16_t wReg,
        uint16_t value,
        uint16_t slave_addr = 0xFFFF
    );

    private:  bool sendCmd_cal();
    private:  bool sendCmd_rst(uint16_t slave_addr = 0xFFFF);

    private: uint16_t recieve(uint8_t *resp, uint16_t len); // Receive len bytes into a buffer

    private: uint16_t CRC16(const uint8_t *data, uint16_t len); // Calculate CRC of buffer


    // Attributes
    ///////////////
    private: Stream*  _serial; // Serial interface
    private: bool     _isSoft; // Is serial interface software
    private: uint8_t  _addr;   // Device address

    private: uint64_t _millis_lastMeasureUpdate; // Last time values were updated
    private: uint64_t _millis_lastParameterUpdate;

    private: uint64_t _timeout_upd;

    private: struct {
        float voltage;
        float current;
        float power;
        float energy;
        uint16_t highVoltAlarm;
        uint16_t lowVoltAlarm;
    }  _measureValues; // Measured values

    private: struct {
        float threshold_HVA;
        float threshold_LVA;
        uint32_t address;
        uint32_t shuntType;
    }  _parameterValues; // Measured values


};


/* ==================================================
** Extern variables
**
** =============================================== */


//


#endif