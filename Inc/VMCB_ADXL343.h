/*
    This library is supposed to configure an ADXL343 device to detect a (free) fall.

    As with all my other software i want to state the following seperately from the License:
        I do not care how, when, why or what for this piece of code is used. Just keep in mind that
        I do NOT provide any warranty whatsoever. Use at own risk.
        2022-LS
*/
#ifndef VMCB_ADXL343
#define VMCB_ADXL343

#include <stdint.h>



/*
    errorcode : |b7|b6|b5|b4|b3|b2|b1|b0|
                |RW|func-id |sub-id     |
    RW:         error in i2c read/write 
    func-id :   ID of upper level function
                0x0X 
                0x1X ADX_init
                0x2X ADX_FFint_init
                0x3X ADX_checkConnection
                0x4X ADX_clearFFInterrupt
    sub-id  :   id of step in function   

    
*/



#define ADX_ADR (0x53 << 1)
#define DEVID 0xE5

typedef unsigned char ADX_REGISTER;
//-----------------------------------------------Address_Map-------------------------------------------------
typedef enum 
{
    aDEVID,                      //R ,Reset: 1110 0101, Device ID
    //Reserved 0x1-0x1C
    aTHRESH_TAP = 0x1D,          //RW, Reset:0000 0000, Tap Threshold
    aOFSX,                       //RW, Reset:0000 0000, X-Axis Offset
    aOFSY,                       //RW, Reset:0000 0000, Y-Axis Offset
    aOFSZ,                       //RW, Reset:0000 0000, Z-Axis Offset
    aTAP_DUR,                    //RW, Reset:0000 0000, Tap Duration
    aTAP_LAT,                    //RW, Reset:0000 0000, Tap Latency
    aTAP_WIN,                    //RW, Reset:0000 0000, Tap Window
    aTHRESH_ACT,                 //RW, Reset:0000 0000, Activity Threshold
    aTHRESH_INACT,               //RW, Reset:0000 0000, Inactivity Threshold
    aTIME_INACT,                 //RW, Reset:0000 0000, Inactivity Time
    aACT_INACT_CTL,              //RW, Reset:0000 0000, Axis enable control for (In-)/Activity detection
    aTHRESH_FF,                  //RW, Reset:0000 0000, Freefall Threshold
    aTIME_FF,                    //RW, Reset:0000 0000, Freefall Time
    aTAP_AXES,                   //RW, Reset:0000 0000, Axis control for sincle/double Tap
    aACT_TAP_STATUS,             //R , Reset:0000 0000, Source of single/double Tap
    aBW_RATE,                    //RW, Reset:0000 1010, Data rate and power mode control
    aPOWER_CTL,                  //RW, Reset:0000 0000, Power saving control
    aINT_ENABLE,                 //RW, Reset:0000 0000, Interrupt enable
    aINT_MAP,                    //RW, Reset:0000 0000, Interrupt Mapping
    aINT_SOURCE,                 //R , Reset:0000 0010, Interrupt Source
    aDATA_FORMAT,                //RW, Reset:0000 0000, Data format control
    aDATA_X0,                    //R , Reset:0000 0000, Data X-Axis 1/2
    aDATA_X1,                    //R , Reset:0000 0000, Data X-Axis 2/2
    aDATA_Y0,                    //R , Reset:0000 0000, Data Y-Axis 1/2
    aDATA_Y1,                    //R , Reset:0000 0000, Data Y-Axis 2/2
    aDATA_Z0,                    //R , Reset:0000 0000, Data Z-Axis 1/2
    aDATA_Z1,                    //R , Reset:0000 0000, Data Z-Axis 2/2
    aFIFO_CTL,                   //RW, Reset:0000 0000, FIFO control
    aFIFO_STAT                   //R , Reset:0000 0000, FIFO status
}ADX_Address;

//-----------------------------------------------Register_Offset_List----------------------------------------
enum ADX_OFFS_ACT_INACT_CTL
{
    oINACT_Z_ENABLE,
    oINACT_Y_ENABLE,
    oINACT_X_ENABLE,
    oINACT_ACDC,
    oACT_Z_ENABLE,
    oACT_Y_ENABLE,
    oACT_X_ENABLE,
    oACT_ACDC,
};
enum ADX_OFFS_TAP_AXES
{
    oTAP_Z_ENABLE,
    oTAP_Y_ENABLE,
    oTAP_X_ENABLE,
    oTAP_SUPPRESS
};
enum ADX_OFFS_ACT_TAP_STATUS
{
    oTAP_Z_SOURCE,
    oTAP_Y_SOURCE,
    oTAP_X_SOURCE,
    oASLEEP,
    oACT_Z_SOURCE,
    oACT_Y_SOURCE,
    oACT_X_SOURCE
};
enum ADX_OFFS_BW_RATE
{
    oRATE,//4Bit
    oLOW_POWER = 4
};
enum ADX_OFFS_POWER_CTL
{
    oWAKEUP,//2Bit
    oSLEEP = 2,
    oMEASURE,
    oAUTO_SLEEP,
    oLINK
};
enum ADX_OFFS_INT_all
{
    oOVERRUN,
    oWATERMARK,
    oFREEFALL,
    oINACTIVITY,
    oACTIVITY,
    oDOUBLETAP,
    oSINGLETAP,
    oDATAREADY
};
enum ADX_OFFS_DATA_FORMAT
{
    oRANGE,//2Bit
    oJUSTIFY = 2,
    oFULL_RES,
    oINT_INVERT = 5,
    oSPI,
    oSELF_TEST
};
enum ADX_OFFS_FIFO_CTL
{
    oSAMPLES,//5Bits
    oTRIGGER,
    oFIFO_MODE//2Bits
};
enum ADX_OFFS_FIFO_SATUS
{
    oENTRIES,//6Bits
    oFIFO_TRIG=7
};

typedef enum
{
    bADX_rate_100mHz,
    bADX_rate_200mHz,
    bADX_rate_390mHz,
    bADX_rate_780mHz,
    bADX_rate_1560mHz,
    bADX_rate_3130mHz,
    bADX_rate_6250mHz,
    bADX_rate_12500mHz,
    bADX_rate_25Hz,
    bADX_rate_50Hz,
    bADX_rate_100Hz,
    bADX_rate_200Hz,
    bADX_rate_400Hz,
    bADX_rate_800Hz,
    bADX_rate_1600Hz,
    bADX_rate_3200Hz
}
ADX_BIT_Rate;
typedef enum
{
    bADX_SleepFreq_8Hz,
    bADX_SleepFreq_4Hz,
    bADX_SleepFreq_2Hz,
    bADX_SleepFreq_1Hz,
}
ADX_BIT_Wakeup;
typedef enum
{
    bADX_range_2g,
    bADX_range_4g,
    bADX_range_8g,
    bADX_range_16g
}
ADX_BIT_Range;
typedef enum
{
    bADX_FIFO_bypass,
    bADX_FIFO_fifo,
    bADX_FIFO_stream,
    bADX_FIFO_trigger
}
ADX_BIT_FIFOmode;
typedef enum
{
    ADX_INT_pin1,
    ADX_INT_pin2
}
ADX_INT_PIN;
/**
 * @brief Generate the ADX_RETURN and ADX_STATE. ADX_RETURN is to be usead as the default Return-Value-Datatype for this lib.
 * The value-parameter has the double-dole of return parameter and error code depending on ADX_STATUS state
 * 
 */
typedef enum {ADX_OK, ADX_ERR} ADX_STATE;
typedef struct {ADX_STATE state; uint8_t value;}ADX_RETURN;

/**
 * @brief Write a byte to the ADXL. This function needs to be implemented externally. This was done in order to fit with every framework and driverset.
 * 
 * @param adr Address of writing
 * @param data Data to be written
 * @return ADX_RETURN returns OK or ERR with error code
 */
extern ADX_RETURN ADX_i2cWriteReg(ADX_Address adr, uint8_t* data);
/**
 * @brief Read a byte from the ADXL. This function needs to be implemented externally. This was done in order to fit with every framework and driverset.
 * 
 * @param adr 
 * @return ADX_RETURN 
 */
extern ADX_RETURN ADX_i2cReadReg(ADX_Address adr);

ADX_RETURN ADX_init(ADX_BIT_Range range, ADX_BIT_Rate rate);
ADX_RETURN ADX_FFint_init(ADX_INT_PIN intpin, ADX_REGISTER threshold, ADX_REGISTER duration);
ADX_RETURN ADX_checkConnection();
ADX_RETURN ADX_clearFFInterrupt();


#endif