/* ==================================================
** Libraries
**
** =============================================== */


#include "PZEM017.h"


/* ==================================================
** Static function declaration
**
** =============================================== */


static void printHex(int num, int precision);


/* ==================================================
** Static function definition
**
** =============================================== */


static void printHex(int num, int precision) {
     char tmp[16];
     char format[128];

     sprintf(format, "%%.%dX", precision);

     sprintf(tmp, format, num);
     Serial.println(tmp);
}


/* ==================================================
** PZEM017: Constructors
**
** =============================================== */


/** Software Serial constructor
 * @param RX    RX pin
 * @param TX    TX pin
 * @param addr  Slave address of device
*/
#if defined(PZEM017_SOFTSERIAL)
PZEM017::PZEM017(uint8_t RX, uint8_t TX, uint8_t addr)
{
    SoftwareSerial *port = new SoftwareSerial(RX, TX);
    port->begin(PZEM017_BAUND_RATE, PZEM017_SERIAL_FORMAT);
    this->_serial = port;
    this->_isSoft = true;
    this->init(addr);
}
#endif


/** Hardware serial constructor
 *
 * @param port Hardware serial to use
 * @param addr Slave address of device
*/
PZEM017::PZEM017(HardwareSerial* port, uint8_t addr)
{
    port->begin(PZEM017_BAUND_RATE,PZEM017_SERIAL_FORMAT);
    this->_serial = port;
    this->_isSoft = false;
    init(addr);
}


/* ==================================================
** PZEM017: Destructors
**
** =============================================== */


// Destructor deleting software serial
PZEM017::~PZEM017()
{
    if(_isSoft)
        delete this->_serial;
}


/* ==================================================
** PZEM017: Getters
**
** =============================================== */


/** Get line voltage in Volts
 * @return current L-N volage
*/
float PZEM017::get_voltage()
{
    if(!upd_measure()) // Update vales if necessary
        return NAN; // Update did not work, return NAN

    return _measureValues.voltage;
}


/** Get line in Amps
 * @return line current
*/
float PZEM017::get_current()
{
    if(!upd_measure())// Update vales if necessary
        return NAN; // Update did not work, return NAN

    return _measureValues.current;
}


/** Get Active power in W
 * @return active power in W
*/
float PZEM017::get_power()
{
    if(!upd_measure()) // Update vales if necessary
        return NAN; // Update did not work, return NAN

    return _measureValues.power;
}


/** Get Active energy in kWh since last reset
 * @return active energy in Wh
*/
float PZEM017::get_energy()
{
    if(!upd_measure()) // Update vales if necessary
        return NAN; // Update did not work, return NAN

    return _measureValues.energy;
}


/** Get threshold of low voltage alarm
 * @return voltage of low voltage alarm in Volt
*/
float PZEM017::get_threshold_LVA()
{
    if(!upd_parameters()) // Update vales if necessary
        return NAN; // Update did not work, return NAN

    return _parameterValues.threshold_LVA;
}


/** Get threshold of high voltage alarm
 * @return voltage of high voltage alarm in Volt
*/
float PZEM017::get_threshold_HVA()
{
    if(!upd_parameters()) // Update vales if necessary
        return NAN; // Update did not work, return NAN

    return _parameterValues.threshold_HVA;
}


/** Get the current device address
 * @return address
*/
uint8_t PZEM017::get_address()
{
    if(!upd_parameters()){
        return NAN;
    }
    
    _addr = _parameterValues.address;
    return _addr;
}


/** Get the shunt type
 * @return address
*/
uint16_t PZEM017::get_shuntType()
{
    if(!upd_parameters()){
        return NAN;
    }

    return _parameterValues.shuntType;
}


/** Get the timeout each update (ms)
 * @return address
*/
uint64_t PZEM017::get_timeoutUpdate()
{
    return _timeout_upd;
}


/* ==================================================
** PZEM017: Setters
**
** =============================================== */


/** Set a new device address and update the device
 * 
 * WARNING - should be used to set up devices once.
 * Code initializtion will still have old address on next run!
 *
 * @param[in] addr      New device address 0x01-0xF7 (1 - 247)
 * @param[in] slaveAddr Slave address. Default is current local address
 * 
 * @return true: success | false: fail
*/
bool PZEM017::set_address(uint8_t addr, uint16_t slaveAddr)
{
    if(addr < PZEM017_ADDR_MIN || addr > PZEM017_ADDR_MAX) // sanity check
        return false;

    // Write the new address to the address register
    if(!sendCmd_writeParameter(PZEM017_WREG_ADDR, addr, slaveAddr))
        return false;

    // If successful, update the current slave address
    _addr = addr; 
    return true;
}


/** Set Low Voltage alarm threshold in volts
 *
 * @param[in] Volt Alarm Threshold: 1 - 350V
 * @return true: success | false: fail
*/
bool PZEM017::set_threshold_LVA(uint16_t volt)
{
    if(volt < PZEM017_THRESHOLD_LVA_MIN || volt > PZEM017_THRESHOLD_LVA_MAX){
        Serial.println("Error: Invalid low voltage threshold (1V - 350V)");
        return false;
    }

    uint16_t LSB = volt / PZEM017_RESOLUTION_VOLTAGE;
    
    Serial.print("set_threshold_LVA: ");
    Serial.print(volt);
    Serial.print("V -> ");
    Serial.print(LSB);
    Serial.print("LSB");
    Serial.println();

    // Write the watts threshold to the Alarm register
    if(!sendCmd_writeParameter(PZEM017_WREG_THRESHOLD_LVA, LSB))
    {
        Serial.println("Error: set_threshold_LVA");
        return false;
    }

    return true;
}


/** Set High Voltage alarm threshold in volts
 *
 * @param[in] Volt Alarm Threshold: 5 - 350V
 * @return true: success | false: fail
*/
bool PZEM017::set_threshold_HVA(uint16_t volt)
{
    if(volt < PZEM017_THRESHOLD_HVA_MIN || volt > PZEM017_THRESHOLD_HVA_MAX){
        Serial.println("Error: Invalid high voltage threshold (5V - 350V)");
        return false;
    }

    uint16_t LSB = volt / PZEM017_RESOLUTION_VOLTAGE;
    
    Serial.print("set_threshold_HVA: ");
    Serial.print(volt);
    Serial.print("V -> ");
    Serial.print(LSB);
    Serial.print("LSB");
    Serial.println();

    // Write the watts threshold to the Alarm register
    if(!sendCmd_writeParameter(PZEM017_WREG_THRESHOLD_HVA, volt))
    {
        Serial.println("Error: set_threshold_HVA");
        return false;
    }

    return true;
}


/** Set current range in Ampes
 *
 * @param[in] shuntType current value: 
 *    PZEM017_RANGE_CURRENT_100A :  0x0000 : 100A 
 *  | PZEM017_RANGE_CURRENT_50A  :  0x0001 : 50A 
 *  | PZEM017_RANGE_CURRENT_200A :  0x0002 : 200A 
 *  | PZEM017_RANGE_CURRENT_300A :  0x0003 : 300A
 * 
 * @return true: success | false: fail
*/
bool PZEM017::set_shuntType(uint16_t shuntType)
{
    shuntType = shuntType < PZEM017_RANGE_CURRENT_100A ? PZEM017_RANGE_CURRENT_100A : shuntType;
    shuntType = shuntType > PZEM017_RANGE_CURRENT_300A ? PZEM017_RANGE_CURRENT_300A : shuntType;

    if(!sendCmd_writeParameter(PZEM017_WREG_CURRENT_RANGE, shuntType))
    return false;

    Serial.println("Set Shunt Value");
    return true;
}


/** Set timeout per update (ms)
 * @return true: success | false: fail
*/
bool PZEM017::set_timeoutUpdate(uint64_t timeout)
{
    _timeout_upd = timeout;

    Serial.println("set_timeoutUpdate");
    return true;
}


/** Set last two bytes of buffer to CRC16 of the buffer up to byte len-2
 * 
 * Buffer must be able to hold at least 3 bytes
 *
 * @param[out] data Memory buffer containing the frame to checksum and write CRC to
 * @param[in] len  Length of the respBuffer including 2 bytes for CRC
 *
*/
void PZEM017::set_CRC(uint8_t *buf, uint16_t len){

    // If the buffer length is not enough space for CRC
    if(len <= 2)
        return;

    uint16_t crc = CRC16(buf, len - 2); // CRC of data

    // Write high and low byte to last two positions
    // buf[len - 2] = (crc >> 8) & 0xFF; // High byte first
    // buf[len - 1] = crc & 0xFF; // Low byte second

    buf[len - 2] = crc & 0xFF; // Low byte first
    buf[len - 1] = (crc >> 8) & 0xFF; // High byte second

    #ifdef DEBUGMODE
        Serial.println("CRC:");
        printHex((crc & 0xFF),2);
        printHex(((crc >> 8) & 0xFF),2);
    #endif
}


/* ==================================================
** PZEM017: Updaters
**
** =============================================== */


/** Update current slave parameters
 * @return true: success | false: fail
*/
bool PZEM017::upd_parameters()
{
    // Serial.println("upd_parameters:");

    static uint8_t responseParameter[PZEM017_SIZE_REPLY_RPARAMETER];

    // If we read before the update time limit, do not update
    if(_millis_lastParameterUpdate + _timeout_upd > millis()){
        return true;
    }

    _millis_lastParameterUpdate = millis();

    sendCmd_readParameter(PZEM017_WREG_FIRST, PZEM017_NUM_SLAVE_PARAMETER);

    if(recieve(responseParameter,PZEM017_SIZE_REPLY_RPARAMETER) != PZEM017_SIZE_REPLY_RPARAMETER){
        return false;
    }

    // Update threshold of high voltage alarm
    _parameterValues.threshold_HVA = ((uint32_t)responseParameter[3] << 8 |
                                       (uint32_t)responseParameter[4])
                                     * PZEM017_RESOLUTION_THRESHOLD_HVA;

    // Update threshold of low voltage alarm
    _parameterValues.threshold_LVA = ((uint32_t)responseParameter[5] << 8 | 
                                       (uint32_t)responseParameter[6])
                                     * PZEM017_RESOLUTION_THRESHOLD_LVA;

    // Update slave address
    _parameterValues.address   = ((uint32_t)responseParameter[7] << 8 | 
                                       (uint32_t)responseParameter[8]);

    // Update current range
    _parameterValues.shuntType =  ((uint32_t)responseParameter[9] << 8 |
                                    (uint32_t)responseParameter[10]);

    // Serial.print("HighVoltageThreshold: "); 
    // Serial.print(_parameterValues.threshold_HVA); 
    // Serial.println("V");
    
    // Serial.print("LowVoltageThreshold: "); 
    // Serial.print(_parameterValues.threshold_LVA); 
    // Serial.println("V"); 
    
    // Serial.print("MODBUS-RTU Adress: "); 
    // printHex(_parameterValues.address,4); 
    // Serial.println("");

    // Serial.print("Current Shunt: "); 
    // printHex(_parameterValues.shuntType,4); 
    // Serial.println("");                                       
    return true;
}


/** Read all registers of measure results and update the local values
 * @return true: success | false: fail
*/
bool PZEM017::upd_measure()
{
    //static uint8_t buffer[] = {0x00, CMD_RIR, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00};
    static uint8_t response[PZEM017_SIZE_REPLY_MEASURES];

    // If we read before the update time limit, do not update
    if(_millis_lastMeasureUpdate + _timeout_upd > millis()){
        return true;
    }

    // Record current time as _millis_lastUpdate
    _millis_lastMeasureUpdate = millis();

    // Read 10 registers starting at 0x00 (no check)
    sendCmd_readMeasure(PZEM017_RREG_FIRST, PZEM017_NUM_MEASUREMENT_RESULT);
    
    if(recieve(response, PZEM017_SIZE_REPLY_MEASURES) != PZEM017_SIZE_REPLY_MEASURES){
        return false;
    }

    // if(){ // Something went wrong
    //     return false;
    // }


    // Update voltage (V)
    _measureValues.voltage = ((uint32_t)response[3] << 8 |
                              (uint32_t)response[4])
                            * PZEM017_RESOLUTION_VOLTAGE;

    // Update current (A)
    _measureValues.current = ((uint32_t)response[5] << 8 |
                              (uint32_t)response[6])
                            * PZEM017_RESOLUTION_CURRENT;

    // Update power (W)
    _measureValues.power   = ((uint32_t)response[7]  << 8  |
                              (uint32_t)response[8]        |
                              (uint32_t)response[9]  << 24 |
                              (uint32_t)response[10] << 16 )
                            * PZEM017_RESOLUTION_POWER;

    // Update energy (Wh)
    _measureValues.energy  = ((uint32_t)response[11] << 8  |
                              (uint32_t)response[12]       |
                              (uint32_t)response[13] << 24 |
                              (uint32_t)response[14] << 16 )
                            * PZEM017_RESOLUTION_ENGERY;

    // Uddate status of high voltage alarm
    _measureValues.highVoltAlarm = ((uint32_t)response[15] << 8 | 
                                   (uint32_t)response[16]);

    // Uddate status of low voltage alarm
    _measureValues.lowVoltAlarm  = ((uint32_t)response[17] << 8 |
                                   (uint32_t)response[18]);


    return true;
}


/* ==================================================
** PZEM017: Reseters
**
** =============================================== */


/** Reset the Energy counter on the device
 * @return success
*/
bool PZEM017::rst_energy(){
    
    if(!sendCmd_rst()){
        return false;
    }

    return true;
}


/* ==================================================
** PZEM017: Checkers
**
** =============================================== */


/** Check high voltage alarm
 * @return true: Alarm is on | false: Alarm is off
*/
bool PZEM017::check_highVoltAlarm()
{
    if(!upd_measure()){
        return NAN;
    }

    return _measureValues.highVoltAlarm==PZEM017_STATUS_ON;
}


/** Check low voltage alarm
 * @return true: Alarm is on | false: Alarm is off
*/
bool PZEM017::check_lowVoltAlarm()
{
    if(!upd_measure()){
        return NAN;
    }

    return _measureValues.lowVoltAlarm==PZEM017_STATUS_ON;
}


/**
 * PZEM017::check_CRC
 *
 * Performs CRC check of the buffer up to len-2 and compares check sum to last two bytes
 *
 * @param[in] data Memory buffer containing the frame to check
 * @param[in] len  Length of the respBuffer including 2 bytes for CRC
 *
 * @return is the buffer check sum valid
*/
bool PZEM017::check_CRC(const uint8_t *buf, uint16_t len){
    if(len <= 2) // Sanity check
        return false;

    uint16_t crc = CRC16(buf, len - 2); // Compute CRC of data
    return ((uint16_t)buf[len-2]  | (uint16_t)buf[len-1] << 8) == crc;
}


/* ==================================================
** PZEM017: Methods
**
** =============================================== */


/** Initialization common to all consturctors
 *
 * @param[in] addr  device address: 0x01 - 0xF8 (1 - 248)
 * @return          success
*/
void PZEM017::init(uint8_t addr){
    // Sanity check of address
    if (addr < PZEM017_ADDR_MIN || addr > PZEM017_ADDR_GENERAL){
        addr = PZEM017_ADDR_DEFAULT;
    }
    _addr = addr;

    _millis_lastMeasureUpdate = 0;
    _millis_lastMeasureUpdate -= PZEM017_TIMEOUT_UPDATE;

    _millis_lastParameterUpdate = 0;
    _millis_lastParameterUpdate -= PZEM017_TIMEOUT_UPDATE;

    _timeout_upd = PZEM017_TIMEOUT_UPDATE;
}


/** Sent 8 bytes command
 * 
 * Read register: register address of measurement result
 *
 * @param[in] cmd        - Following function codes: PZEM017_CMD_RHR, PZEM017_CMD_RIR, PZEM017_CMD_WSR
 * @param[in] addr       - Address of reading register of holding register
 * @param[in] val        - Number of address to read or new value to write
 * @param[in] check      - perform a simple read check after write. Default is false
 * @param[in] slave_addr - Slave address. Default is current local address
 *
 * @return success
*/
bool PZEM017::sendCmd_8B(uint8_t cmd, uint16_t addr, uint16_t val, bool check, uint16_t slave_addr){

    uint8_t sendBuffer[8]; // Send buffer
    uint8_t respBuffer[8]; // Response buffer (only used when check is true)

    if((slave_addr == 0xFFFF) ||
       (slave_addr < PZEM017_ADDR_MIN) ||
       (slave_addr > PZEM017_ADDR_GENERAL)){
        slave_addr = _addr;
    }

    sendBuffer[0] = slave_addr;              // Set slave address
    sendBuffer[1] = cmd;                     // Set command

    sendBuffer[2] = (addr >> 8) & 0xFF;     // Set high byte of register address
    sendBuffer[3] = (addr) & 0xFF;          // Set low byte =//=

    sendBuffer[4] = (val >> 8) & 0xFF;       // Set high byte of register value
    sendBuffer[5] = (val) & 0xFF;            // Set low byte =//=

    set_CRC(sendBuffer, 8);                   // Set CRC of frame

    _serial->write(sendBuffer, 8); // send frame

    if(check) {
        if(!recieve(respBuffer, 8)){ // if check enabled, read the response
            return false;
        }

        // Check if response is same as send
        for(uint8_t i = 0; i < 8; i++){
            if(sendBuffer[i] != respBuffer[i])
                return false;
        }
    }

    return true;
}


/** Sent command: Read measurement results
 * 
 * Read register: register address of measurement result
 *
 * @param[in] rReg       - Start read register. Default is the first read register
 * @param[in] numRReg    - Number of read register. Default is the number of read register
 * @param[in] slave_addr - Slave address. Default is current local address
 *
 * @return success
*/
bool PZEM017::sendCmd_readMeasure(uint16_t rReg, uint16_t numRReg, uint16_t slave_addr){

    if(numRReg < 1){
        numRReg = 1;
    }

    if(PZEM017_NUM_MEASUREMENT_RESULT-rReg < numRReg){
        numRReg = PZEM017_NUM_MEASUREMENT_RESULT-rReg;
    }
    
    return sendCmd_8B(PZEM017_CMD_RIR, rReg, numRReg, false, slave_addr);
}


/** Sent command: Read slave parameters
 * 
 * Write register: register address of slave parameter
 *
 * @param[in] wReg       - Start write register. Default is the first write register
 * @param[in] numWReg    - Number of write register. Default is the number of write register
 * @param[in] check      - perform a simple read check after write. Default is false
 * @param[in] slave_addr - Slave address. Default is current local address
 *
 * @return success
*/
bool PZEM017::sendCmd_readParameter(uint16_t wReg, uint16_t numWReg, uint16_t slave_addr){

    if(numWReg < 1){
        numWReg = 1;
    }

    if(PZEM017_NUM_SLAVE_PARAMETER-wReg < numWReg){
        numWReg = PZEM017_NUM_SLAVE_PARAMETER-wReg;
    }
    
    return sendCmd_8B(PZEM017_CMD_RHR, wReg, numWReg, false, slave_addr);
}


/** Sent command: Write (modify) slave parameters
 * 
 * Write register: register address of slave parameter
 *
 * @param[in] wReg       - Write register that you want to modify
 * @param[in] value      - New value for write register
 * @param[in] slave_addr - Slave address. Default is current local address
 *
 * @return success
*/
bool PZEM017::sendCmd_writeParameter(uint16_t wReg, uint16_t value, uint16_t slave_addr){
    return sendCmd_8B(PZEM017_CMD_WSR, wReg, value, true, slave_addr);
}


/** Sent command: Calibrate the device
 * 
 * Only success when device matches the conditions
 *
 * @return success
*/
bool PZEM017::sendCmd_cal(){


    uint8_t sendBuffer[PZEM017_SIZE_CAL]; // Send buffer
    uint8_t respBuffer[PZEM017_SIZE_REPLY_CAL];  // Response buffer (only used when check is true)

    sendBuffer[0] = PZEM017_ADDR_GENERAL;   // Set slave address
    sendBuffer[1] = PZEM017_CMD_CAL;        // Set command

    sendBuffer[2] = 0x37;   
    sendBuffer[3] = 0x21; 

    set_CRC(sendBuffer, PZEM017_SIZE_CAL); // Set CRC of frame
    #ifdef DEBUGMODE
        Serial.println("WriteBuffer: ");
        for(uint8_t u = 0; u < PZEM017_SIZE_CAL; u++){
            printHex(sendBuffer[u],2);
        //Serial.println(sendBuffer[u],HEX);
        }
    #endif
    
    _serial->write(sendBuffer, PZEM017_SIZE_CAL); // send frame

    return true;
}

/** Sent command: Reset energy
 * 
 * @param[in] slave_addr - Slave address. Default is current local address
 *
 * @return true: success | false: fail
*/
bool PZEM017::sendCmd_rst(uint16_t slave_addr){

    if( (slave_addr == 0xFFFF)
     || (slave_addr < PZEM017_ADDR_MIN)
     || (slave_addr > PZEM017_ADDR_GENERAL)
    ){
        slave_addr = _addr;
    }

    uint8_t sendBuffer[PZEM017_SIZE_RST]; // Send buffer
    uint8_t respBuffer[PZEM017_SIZE_REPLY_ERR];  // Response buffer (only used when check is true)

    sendBuffer[0] = _addr;            // Set slave address
    sendBuffer[1] = PZEM017_CMD_RST;  // Set command

    set_CRC(sendBuffer, PZEM017_SIZE_RST); // Set CRC of frame

    #ifdef DEBUGMODE
        Serial.println("WriteBuffer: ");
        for(uint8_t u = 0; u < PZEM017_SIZE_RST; u++){
            printHex(sendBuffer[u],2);
        //Serial.println(sendBuffer[u],HEX);
        }
    #endif

    _serial->write(sendBuffer, PZEM017_SIZE_RST); // send frame

    uint16_t length = recieve(respBuffer, PZEM017_SIZE_REPLY_ERR);
    if(length == 0 || length == PZEM017_SIZE_REPLY_ERR){
        return false;
    }

    return true;
}


/** Receive data from serial with buffer limit and timeout
 *
 * @param[out] resp Memory buffer to hold response. Must be at least `len` long
 * @param[in] len Max number of bytes to read
 *
 * @return number of bytes read
*/
uint16_t PZEM017::recieve(uint8_t *resp, uint16_t len)
{
    //Serial.println(*resp);
    #ifdef PZEM017_SOFTSERIAL
        if(_isSoft)
            ((SoftwareSerial *)_serial)->listen(); // Start software serial listen
    #endif

    unsigned long startTime = millis(); // Start time for Timeout
    uint8_t index = 0; // Bytes we have read
    while((index < len) && (millis() - startTime < PZEM017_TIMEOUT_READ))
    {
        if(_serial->available() > 0)
        {
            uint8_t c = (uint8_t)_serial->read();

            resp[index++] = c;
        }
        yield();	// do background netw tasks while blocked for IO (prevents ESP watchdog trigger)
    }
    #ifdef DEBUGMODE
        Serial.println("ReadBuffer: ");
        for(uint8_t z = 0; z < len; z++){
            printHex(resp[z],2);
        }
    #endif
    //Check CRC with the number of bytes read
    if(!check_CRC(resp, index)){
        // Serial.println("CRC Error");
        return 0;
    }

    return index;
}


// Pre computed CRC table
static const uint16_t crcTable[] PROGMEM = {
    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
};


/**
 * PZEM017::CRC16
 *
 * Calculate the CRC16-Modbus for a buffer
 * Based on https://www.modbustools.com/modbus_crc16.html
 *
 * @param[in] data Memory buffer containing the data to checksum
 * @param[in] len  Length of the respBuffer
 *
 * @return Calculated CRC
*/
uint16_t PZEM017::CRC16(const uint8_t *data, uint16_t len)
{
//     uint8_t nTemp; // CRC table index
//     uint16_t crc = 0xFFFF; // Default value
//   for (int pos = 0; pos < len; pos++) {
//     crc ^= (uint16_t)data[pos];          // XOR byte into least sig. byte of crc
  
//     for (int i = 8; i != 0; i--) {    // Loop over each bit
//       if ((crc & 0x0001) != 0) {      // If the LSB is set
//         crc >>= 1;                    // Shift right and XOR 0xA001
//         crc ^= 0xA001;
//       }
//       else                            // Else LSB is not set
//         crc >>= 1;                    // Just shift right
//     }
//   }
//   // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
//   return crc;  


    uint8_t nTemp; // CRC table index
    uint16_t crc = 0xFFFF; // Default value

    while (len--)
    {
        nTemp = *data++ ^ crc;
        crc >>= 8;
        crc ^= (uint16_t)pgm_read_word(&crcTable[nTemp]);
    }
    return crc;
}


/** Search for available devices. This should be used only for debugging!
 * 
 * Prints any found device addresses on the bus.
 * Can be disabled by defining PZEM017T_DISABLE_SEARCH
*/
void PZEM017::search(){
    #if ( not defined(PZEM017T_DISABLE_SEARCH))

    const uint8_t size_response = 1+1+1 +2*1 +2;
    static uint8_t response[size_response];
    uint8_t count = 0;

    for(uint16_t addr = PZEM017_ADDR_MIN; addr <= PZEM017_ADDR_GENERAL; addr++){
        Serial.print("Check ");
        Serial.print(addr);
        sendCmd_readMeasure(PZEM017_RREG_FIRST, 1, addr);

        count = recieve(response, size_response);
        Serial.print(" - ");
        Serial.println(count);
        
        if(count != size_response){ // Something went wrong
            continue;

        } else {
            Serial.print("Device on addr: ");
            Serial.println(addr);
        }
    }

    #endif
}