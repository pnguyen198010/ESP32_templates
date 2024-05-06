#ifndef PZEM017_const_h
#define PZEM017_const_h


/* ==================================================
** Libraries
**
** =============================================== */


#include <Arduino.h>


/* ==================================================
** Current range
**
** =============================================== */


#define PZEM017_RANGE_CURRENT_50A   0x0001
#define PZEM017_RANGE_CURRENT_100A  0x0000
#define PZEM017_RANGE_CURRENT_200A  0x0002
#define PZEM017_RANGE_CURRENT_300A  0x0003


/* ==================================================
** Voltage function description
**
** =============================================== */


// Unit: V
#define PZEM017_RANGE_VOLTAGE_MIN   0.05f
// Unit: V
#define PZEM017_RANGE_VOLTAGE_MAX   300.0f
// Unit: V / LSB
#define PZEM017_RESOLUTION_VOLTAGE  0.01f


/* ==================================================
** Current function description
**
** =============================================== */


// Unit: A
#define PZEM017_RANGE_CURRENT_MIN   0.02f
// Unit: A
#define PZEM017_RANGE_CURRENT_MAX   300.0f
// Unit: A / LSB
#define PZEM017_RESOLUTION_CURRENT  0.01f


/* ==================================================
** Power function description
**
** =============================================== */


// Unit: kW
#define PZEM017_RANGE_POWER_MIN   0.2f
// Unit: kW
#define PZEM017_RANGE_POWER_MAX   90.0f
// Unit: W / LSB
#define PZEM017_RESOLUTION_POWER    0.1f


/* ==================================================
** Energy function description
**
** =============================================== */


// Unit: kWh
#define PZEM017_RANGE_ENERGY_MIN   0.0f
// Unit: kWh
#define PZEM017_RANGE_ENERGY_MAX   9999.0f
// Unit: Wh / LSB
#define PZEM017_RESOLUTION_ENGERY   1.0f


/* ==================================================
** High voltage alarm
**
** =============================================== */


// The default threshold of high voltage alarm
#define PZEM017_THRESHOLD_HVA_DEFAULT    300
// The maxium threshold of high voltage alarm
#define PZEM017_THRESHOLD_HVA_MAX        350
// The minium threshold of high voltage alarm
#define PZEM017_THRESHOLD_HVA_MIN        5
// Unit: V / LSB
#define PZEM017_RESOLUTION_THRESHOLD_HVA 0.01f


/* ==================================================
** Low voltage alarm
**
** =============================================== */


// The default threshold of low voltage alarm
#define PZEM017_THRESHOLD_LVA_DEFAULT   7
// The maxium threshold of low voltage alarm
#define PZEM017_THRESHOLD_LVA_MAX       350
// The minium threshold of low voltage alarm
#define PZEM017_THRESHOLD_LVA_MIN       1
// Unit: V / LSB
#define PZEM017_RESOLUTION_THRESHOLD_LVA 0.01f


/* ==================================================
** Alarm status
**
** =============================================== */


// Alarm low / high votage
#define PZEM017_STATUS_ON    0xFFFF
// Not alarm low / high votage
#define PZEM017_STATUS_OFF   0x0000


/* ==================================================
** Physical layer protocol
**
** =============================================== */


#define PZEM017_BAUND_RATE      9600

// 8 data bits, no parity, 2 stop bits
#define PZEM017_SERIAL_FORMAT   SERIAL_8N2


/* ==================================================
** Slave address
**
** =============================================== */


// Slave address range: 0x01 - 0xF7 (247)
#define PZEM017_ADDR_DEFAULT     0x01
// The maxium address of slave
#define PZEM017_ADDR_MAX         0xF7
// The minium address of slave
#define PZEM017_ADDR_MIN         0x01
// The slave does not need to reply the master
#define PZEM017_ADDR_BROADCAST   0x00
/** Can be only used in single-slave environment
 * Can be used for calibration
*/
#define PZEM017_ADDR_GENERAL     0xF8


/* ==================================================
** Function codes (Command codes)
**
** =============================================== */


/** Command: Read holding register
 * Read slave parameters
**/
#define PZEM017_CMD_RHR   0x03
/** Command: Read input register
 * Read measurement result
**/
#define PZEM017_CMD_RIR   0x04
/** Command: Write single register
 * Modify slave parameters
**/
#define PZEM017_CMD_WSR   0x06
/** Command: Calibration
 * 
 * Only for internal use
 * Address can be only 0xF8
 * Used for factory calibration
 * Return to factory maintenance occasions
 * 
 * After the function code to increase 16-bit password
 * The default password is 0x3721
*/
#define PZEM017_CMD_CAL   0x41
// Command: Reset energy
#define PZEM017_CMD_RST   0x42
// Command: Reply error of reading
#define PZEM017_CMD_ERR_R 0x84
// Command: Reply error of writing (modify)
#define PZEM017_CMD_ERR_W 0x86
// Command: Reply error of calibrating
#define PZEM017_CMD_ERR_CAL 0xC1
// Command: Reply error of reseting energy
#define PZEM017_CMD_ERR_RST 0xC2


/* ==================================================
** Password
**
** =============================================== */


#define PZEM017_PASSWORD_CAL 0x3721


/* ==================================================
** Abnormal code
**
** =============================================== */


// Error: Illegal function
#define PZEM017_ERR_FUNC    0x01
// Error: Illegal address
#define PZEM017_ERR_ADDR    0x02
// Error: Illegal data
#define PZEM017_ERR_DATA    0x03
// Error: Slave error
#define PZEM017_ERR_SLAVE   0x04


/* ==================================================
** Register address: Read the measurement result
**
** =============================================== */


// Number of measurement results: 8
#define PZEM017_NUM_MEASUREMENT_RESULT  0x08

// Read register address: voltage value
#define PZEM017_RREG_VOLTAGE            0x0000
// Read register address: current value
#define PZEM017_RREG_CURRENT            0x0001
// Read register address: power value low 16 bits
#define PZEM017_RREG_POWER_LOW          0x0002
// Read register address: power value high 16 bits
#define PZEM017_RREG_POWER_HIGH         0x0003
// Read register address: energy value low 16 bits
#define PZEM017_RREG_ENERGY_LOW         0x0004
// Read register address: energy value high 16 bits
#define PZEM017_RREG_ENERGY_HIGH        0x0005
// Read register address: high voltage alarm status
#define PZEM017_RREG_ALARM_VOLTAGE_LOW  0x0006
// Read register address: low voltage alarm status
#define PZEM017_RREG_ALARM_VOLTAGE_HIGH 0x0007

// The first register address of measurement result
#define PZEM017_RREG_FIRST  PZEM017_RREG_VOLTAGE


/* ==================================================
** Register address: Read and modify the slave parameters
**
** =============================================== */


// Number of slave parameters: 4
#define PZEM017_NUM_SLAVE_PARAMETER 0x04

/** Read and modify register address: threshold of high voltage alarm
 * Range:      5 - 350V
 * Default:    300V
 * Resolution: 0.01V / 1LBS
**/
#define PZEM017_WREG_THRESHOLD_HVA  0x0000
/** Read and modify register address: threshold of low voltage alarm
 * Range:      1 - 350V
 * Default:    7V
 * Resolution: 0.01V / 1LBS
**/
#define PZEM017_WREG_THRESHOLD_LVA  0x0001
/** Read and modify register address: Modbus rtu address of slave
 * Range: 0x01 - 0xF7
**/
#define PZEM017_WREG_ADDR           0x0002
/** Read and modify register address: The current range (only for PZEM-017)
 * PZEM017_RANGE_CURRENT_100A: 0x0001：100A
 * PZEM017_RANGE_CURRENT_50A : 0x0000：50A
 * PZEM017_RANGE_CURRENT_200A: 0x0002：200A
 * PZEM017_RANGE_CURRENT_300A: 0x0003：300A
*/
#define PZEM017_WREG_CURRENT_RANGE  0x0003

// The first register address of slave parameter
#define PZEM017_WREG_FIRST  PZEM017_WREG_THRESHOLD_HVA


/* ==================================================
** Timeout
**
** =============================================== */


#define PZEM017_TIMEOUT_UPDATE  200
#define PZEM017_TIMEOUT_READ    200


/* ==================================================
** Sent command size
**
** =============================================== */


// Sent size (bytes) of read measurement result command
#define PZEM017_SIZE_MEASURES    8
// Sent size (bytes) of read slave parameter command
#define PZEM017_SIZE_RPARAMETER  8
// Sent size (bytes) of write (modify) slave parameter command
#define PZEM017_SIZE_WPARAMETER  8
// Sent size (bytes) of calibrate command
#define PZEM017_SIZE_CAL         6
// Sent size (bytes) of reset energy command
#define PZEM017_SIZE_RST         4


/* ==================================================
** Reply command size
**
** =============================================== */


/** Correct reply size (bytes) of update measure results
 * 
 * Bytes contains slave address  : 1
 * Bytes contains function code  : 1
 * Bytes contains number of bytes: 1
 * Bytes of each result register : 2 (low byte and high byte)
 * Number of result register     : 8
 * Bytes of CRC check            : 2 (low byte and high byte)
 * 
 * Bytes of correct reply: 1 + 1 + 1 + 2*8 + 2 = 21
**/
#define PZEM017_SIZE_REPLY_MEASURES 21

/** Correct reply size (bytes) of read slave parameters
 * 
 * Bytes contains slave address  : 1
 * Bytes contains function code  : 1
 * Bytes contains number of bytes: 1
 * Bytes of each result register : 2 (low byte and high byte)
 * Number of result register     : 4
 * Bytes of CRC check            : 2 (low byte and high byte)
 * 
 * Bytes of correct reply: 1 + 1 + 1 + 2*4 + 2 = 13
**/
#define PZEM017_SIZE_REPLY_RPARAMETER 13

/** Correct reply size (bytes) of write (modify) slave parameter
 * 
 * Bytes contains slave address    : 1
 * Bytes contains function code    : 1
 * Bytes contains number of bytes  : 1
 * Bytes contains parameter address: 1 (low byte)
 * Bytes contains parameter value  : 2 (low byte and high byte)
 * Bytes of CRC check              : 2 (low byte and high byte)
 * 
 * Bytes of correct reply: 1 + 1 + 1 + 1 + 2 + 2 = 8
**/
#define PZEM017_SIZE_REPLY_WPARAMETER 8

/** Correct reply size (bytes) of reset energy
 * 
 * Bytes contains slave address    : 1
 * Bytes contains function code    : 1
 * Bytes of CRC check              : 2 (low byte and high byte)
 * 
 * Bytes of correct reply: 1 + 1 + 2 = 4
**/
#define PZEM017_SIZE_REPLY_RST 4

/** Correct reply size (bytes) of calibrate
 * 
 * Correct reply: 
 * 0xF8 + 0x41 + 0x37 + 0x21 + CRC check high byte + CRC check low byte
 * 
 * Bytes of correct reply: 6
**/
#define PZEM017_SIZE_REPLY_CAL 6

/** Error reply size (bytes) of update measure results
 * 
 * Bytes contains slave address : 1
 * Bytes contains function code : 1
 * Bytes contains abnormal code : 1
 * Bytes of CRC check           : 2 (low byte and high byte)
 * 
 * Bytes of correct reply: 1 + 1 + 1 + 2 = 5
**/
#define PZEM017_SIZE_REPLY_ERR 5



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