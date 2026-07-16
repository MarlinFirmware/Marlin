/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "Arduino.h"
#include "i2c.h"

void I2Cx_Init(i2c_type *I2Cx, uint32_t baudRate)
{
    if(I2Cx == I2C1)
    {
        crm_periph_clock_enable(CRM_I2C1_PERIPH_CLOCK, TRUE);
        pinMode(PB6, OUTPUT_AF_OD);
        pinMode(PB7, OUTPUT_AF_OD);
    }
#ifdef CRM_I2C2_PERIPH_CLOCK
    else if(I2Cx == I2C2)
    {
        crm_periph_clock_enable(CRM_I2C2_PERIPH_CLOCK, TRUE);
        pinMode(PB10, OUTPUT_AF_OD);
        pinMode(PB11, OUTPUT_AF_OD);
    }
#endif
#ifdef CRM_I2C3_PERIPH_CLOCK
    else if(I2Cx == I2C3)
    {
        crm_periph_clock_enable(CRM_I2C3_PERIPH_CLOCK, TRUE);
        pinMode(PA8, OUTPUT_AF_OD);
        pinMode(PC9, OUTPUT_AF_OD);
    }
#endif
    else
    {
        return;
    }
	/* reset i2c peripheral */
	i2c_reset(I2Cx);
	/* i2c peripheral initialization */
	i2c_init(I2Cx, I2C_FSMODE_DUTY_2_1, baudRate);

    i2c_own_address1_set(I2Cx, I2C_ADDRESS_MODE_7BIT, 0);
	i2c_ack_enable(I2Cx, TRUE);
	i2c_master_receive_ack_set(I2Cx, I2C_MASTER_ACK_CURRENT);
    /* i2c peripheral enable */
    i2c_enable(I2Cx, TRUE);
}

void I2Cx_ClearADDRFlag(i2c_type* I2Cx)
{
  // Sadece register'ları oku (ADDR flag temizlenir)
  (void)I2Cx->sts1;
  (void)I2Cx->sts2;
}

void I2Cx_Read(i2c_type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, void *buf, uint32_t length)
{
    uint8_t *dat = (uint8_t *)buf;
    
	/* enable ack */
	i2c_ack_enable(I2Cx, TRUE);

	/* generate start condtion */
	i2c_start_generate(I2Cx);

    while(!i2c_flag_get(I2Cx, I2C_STARTF_FLAG));
    while(I2Cx->ctrl1 & I2C_CTRL1_STARTGEN);
    i2c_7bit_address_send(I2Cx, (slaveAddr << 1), I2C_DIRECTION_TRANSMIT);
    while(!i2c_flag_get(I2Cx, I2C_ADDR7F_FLAG));
    I2Cx_ClearADDRFlag(I2Cx);
    
	i2c_data_send(I2Cx, regAddr);
    while(!i2c_flag_get(I2Cx, I2C_TDC_FLAG));
    i2c_flag_clear(I2Cx, I2C_TDC_FLAG);
    
    i2c_start_generate(I2Cx);
    while(!i2c_flag_get(I2Cx, I2C_STARTF_FLAG));
    
    i2c_7bit_address_send(I2Cx, (slaveAddr << 1), I2C_DIRECTION_RECEIVE);
    while(!i2c_flag_get(I2Cx, I2C_ADDR7F_FLAG));
    I2Cx_ClearADDRFlag(I2Cx);
    
    while(length --)
    {
        if(!length)
        {
            i2c_ack_enable(I2Cx, FALSE);
            i2c_stop_generate(I2Cx);
        }
        while(!i2c_flag_get(I2Cx, I2C_RDBF_FLAG));
        *dat++ = i2c_data_receive(I2Cx); 
    }
    while(I2Cx->ctrl1 & I2C_CTRL1_STOPGEN);
}

void I2Cx_Write(i2c_type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, void *buf, uint32_t length)
{
    uint8_t *dat = (uint8_t *)buf;
    
	i2c_start_generate(I2Cx);
    while(!i2c_flag_get(I2Cx, I2C_STARTF_FLAG));
    
    i2c_7bit_address_send(I2Cx, (slaveAddr << 1), I2C_DIRECTION_TRANSMIT);
    while(!i2c_flag_get(I2Cx, I2C_ADDR7F_FLAG));
    I2Cx_ClearADDRFlag(I2Cx);
    
	i2c_data_send(I2Cx, regAddr);
    while(!i2c_flag_get(I2Cx, I2C_TDC_FLAG));
    i2c_flag_clear(I2Cx, I2C_TDC_FLAG);
    
    while(length --)
    {
        i2c_data_send(I2Cx, *dat++);
        while(!i2c_flag_get(I2Cx, I2C_TDC_FLAG));
        i2c_flag_clear(I2Cx, I2C_TDC_FLAG);
    }
    
    i2c_stop_generate(I2Cx);
    
    while(I2Cx->ctrl1 & I2C_CTRL1_STOPGEN);
//    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TDE));
//    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTFF));
}

uint8_t I2Cx_ReadReg(i2c_type *I2Cx, uint8_t slaveAddr, uint8_t regAddr)
{
    uint8_t retval = 0;
    I2Cx_Read(I2Cx, slaveAddr, regAddr, &retval, sizeof(uint8_t));
    return retval;
}

void I2Cx_WriteReg(i2c_type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, uint8_t value)
{
    I2Cx_Write(I2Cx, slaveAddr, regAddr, &value, sizeof(uint8_t));
}




