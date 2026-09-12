/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file Wire.cpp
 * @author Trystan Jones <crenn6977@gmail.com>
 * @brief Wire library, uses the WireBase to create the primary interface
 *        while keeping low level interactions invisible to the user.
 */

/*
 * Library updated by crenn to follow new Wire system.
 * Code was derived from the original Wire for maple code by leaflabs and the
 * modifications by gke and ala42.
 */
/*
 * Updated by Roger Clark. 20141111. Fixed issue when process() returned because of missing ACK (often caused by invalid device address being used), caused SCL to be left
 * LOW so that in the next call to process() , the first clock pulse was not sent, because SCL was LOW when it should have been high.
 */

#include "Wire.h"

#if WIRE_USE_FULL_SPEED_I2C
#  define I2C_DELAY(x)
#  define SET_SDA(state)    digitalWrite_##state(this->sda_pin)
#  define SET_SCL(state)    digitalWrite_##state(this->scl_pin)
#else
#  define I2C_DELAY(x)      delayMicroseconds(x)
#  define SET_SDA(state)    set_sda(state)
#  define SET_SCL(state)    set_scl(state)
#endif

#define I2C_WRITE 0
#define I2C_READ  1

// TODO: Add in Error Handling if pins is out of range for other Maples
// TODO: Make delays more capable
TwoWire::TwoWire(uint8_t scl, uint8_t sda, uint8_t delay) : i2c_delay(delay)
{
    this->scl_pin = scl;
    this->sda_pin = sda;
}

TwoWire::~TwoWire()
{
}

bool TwoWire::begin(uint8_t self_addr)
{
    WireBase::begin(self_addr);

    if (this->sda_pin == PB4 || this->scl_pin == PA8) {
        
        crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
        
        // JTAG close, SWD active
        gpio_pin_remap_config(SWJTAG_MUX_010, TRUE);
        gpio_pin_remap_config(I2C3_MUX, TRUE);
    }
    
    pinMode(this->scl_pin, OUTPUT_OPEN_DRAIN);
    pinMode(this->sda_pin, OUTPUT_OPEN_DRAIN);

    bool success = set_scl(HIGH, WIRE_BEGIN_TIMEOUT);
    set_sda(HIGH);

    return success;
}

/* low level conventions:
 * - SDA/SCL idle high (expected high)
 * - always start with i2c_delay rather than end
 */

void TwoWire::set_scl(bool state)
{
    I2C_DELAY(this->i2c_delay);
    digitalWrite(this->scl_pin, state);
    //Allow for clock stretching - dangerous currently
    if (state == HIGH)
    {
        while(digitalRead(this->scl_pin) == 0);
    }
}

bool TwoWire::set_scl(bool state, uint32_t timeout)
{
    I2C_DELAY(this->i2c_delay);
    digitalWrite(this->scl_pin, state);

    uint32_t start = millis();

    if (state == HIGH)
    {
        while(digitalRead(this->scl_pin) == LOW)
        {
            if(millis() - start >= timeout)
            {
                return false;
            }
        }
    }
    return true;
}

void TwoWire::set_sda(bool state)
{
    I2C_DELAY(this->i2c_delay);
    digitalWrite(this->sda_pin, state);
}

void TwoWire::i2c_start()
{
    SET_SDA(LOW);
    SET_SCL(LOW);
}

void TwoWire::i2c_stop()
{
    SET_SDA(LOW);
    SET_SCL(HIGH);
    SET_SDA(HIGH);
}

bool TwoWire::i2c_get_ack()
{
    SET_SCL(LOW);
    SET_SDA(HIGH);
    SET_SCL(HIGH);
    bool ret = !digitalRead(this->sda_pin);
    SET_SCL(LOW);
    return ret;
}

void TwoWire::i2c_send_ack()
{
    SET_SDA(LOW);
    SET_SCL(HIGH);
    SET_SCL(LOW);
}

void TwoWire::i2c_send_nack()
{
    SET_SDA(HIGH);
    SET_SCL(HIGH);
    SET_SCL(LOW);
}

uint8_t TwoWire::i2c_shift_in()
{
    uint8_t data = 0;

    SET_SDA(HIGH);

    int i;
    for (i = 0; i < 8; i++)
    {
        SET_SCL(HIGH);
        data |= digitalRead(this->sda_pin) << (7 - i);
        SET_SCL(LOW);
    }

    return data;
}

void TwoWire::i2c_shift_out(uint8_t val)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        set_sda(!!(val & (1 << (7 - i)) ) );
        SET_SCL(HIGH);
        SET_SCL(LOW);
    }
}

uint8_t TwoWire::process(void)
{
    itc_msg.xferred = 0;

    uint8_t sla_addr = (itc_msg.addr << 1);
    if (itc_msg.flags == I2C_MSG_READ)
    {
        sla_addr |= I2C_READ;
    }
    i2c_start();
    // shift out the address we're transmitting to
    i2c_shift_out(sla_addr);
    if (!i2c_get_ack())
    {
        i2c_stop();// Roger Clark. 20141110 added to set clock high again, as it will be left in a low state otherwise
        return ENACKADDR;
    }
    // Recieving
    if (itc_msg.flags == I2C_MSG_READ)
    {
        while (itc_msg.xferred < itc_msg.length)
        {
            itc_msg.data[itc_msg.xferred++] = i2c_shift_in();
            if (itc_msg.xferred < itc_msg.length)
            {
                i2c_send_ack();
            }
            else
            {
                i2c_send_nack();
            }
        }
    }
    // Sending
    else
    {
        for (uint8_t i = 0; i < itc_msg.length; i++)
        {
            i2c_shift_out(itc_msg.data[i]);
            if (!i2c_get_ack())
            {
                i2c_stop();// Roger Clark. 20141110 added to set clock high again, as it will be left in a low state otherwise
                return ENACKTRNS;
            }
            itc_msg.xferred++;
        }
    }
    i2c_stop();
    return SUCCESS;
}

// Declare the instance that the users of the library can use
// TwoWire Wire(WIRE_SCL_PIN, WIRE_SDA_PIN, WIRE_DELAY);
