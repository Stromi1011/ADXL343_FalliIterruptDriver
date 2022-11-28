#include <VMCB_ADXL343.h>

ADX_RETURN ADX_init(ADX_BIT_Range range, ADX_BIT_Rate rate)
{
    ADX_RETURN ret;
    uint8_t buf;
    //readRegister - clear b0,b1 - write b0,b1 from range parameter
    ret = ADX_i2cReadReg(aDATA_FORMAT);
    if (ret.state != ADX_OK) 
    {
        ret.value |= 0x10;
        return ret;
    }
    
    buf = ((ret.value) & 0xFC) | (range & 0xFC);
    ret = ADX_i2cWriteReg(aDATA_FORMAT,&buf); 
    if (ret.state != ADX_OK) 
    {
        ret.value |= 0x11;
        return ret;
    }

    //write lowPowerMode and rate
    buf=(0x01 << oLOW_POWER) | (rate & 0xF0);
    ret = ADX_i2cWriteReg(aBW_RATE, &buf);
    if (ret.state != ADX_OK) 
    {
        ret.value |= 0x12;
    }
    return ret;
}
ADX_RETURN ADX_FFint_init(ADX_INT_PIN intpin, ADX_REGISTER threshold, ADX_REGISTER duration)
{
    ADX_RETURN ret;
    uint8_t buf;
    //write free fall threshold
    buf = threshold;
    ret = ADX_i2cWriteReg(aTHRESH_FF,&buf);
    if (ret.state != ADX_OK) 
    {
        ret.value |= 0x20;
        return ret;
    }

    //write free fall duration
    buf = duration;
    ret = ADX_i2cWriteReg(aTIME_FF,&buf);
    if (ret.state != ADX_OK)if (ret.state != ADX_OK) 
    {
        ret.value |= 0x21;
        return ret;
    }
    //enable FF interrupt
    buf = (0x01 << oFREEFALL);
    ret = ADX_i2cWriteReg(aINT_ENABLE,&buf);
    if (ret.state != ADX_OK) 
    {
        ret.value |= 0x22;
        return ret;
    }

    //map FF to int-pin
    buf = (intpin << oFREEFALL);
    ret = ADX_i2cWriteReg(aINT_MAP, &buf);
    if (ret.state != ADX_OK) 
    {
        ret.value |= 0x23;
    }
    return ret;
}
ADX_RETURN ADX_checkConnection()
{

    ADX_RETURN readDevId = ADX_i2cReadReg(aDEVID);
    if(readDevId.state != ADX_OK)
    {
        //case: i2c read reg already noticed fault
        readDevId.value |=0x30;
    }
    else if(readDevId.value != DEVID)
    {
        //case: read was succsessfull but wrong
        readDevId.state = ADX_ERR;
        readDevId.value = 0x31;
    }
    return readDevId;
}
ADX_RETURN ADX_clearFFInterrupt()
{
    ADX_RETURN read = ADX_i2cReadReg(aINT_SOURCE);
    if (read.state != ADX_OK)
    {
        //case: read was not possible
        read.value |=0x40;
    }
    else if ((read.value & (0x01<<oFREEFALL)) != (0x01<<oFREEFALL))
    {
        //case: freefall int was not active
        read.value = 0x41;
        read.state = ADX_ERR;
    }
    return read;
}