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
 * @file WireBase.h
 * @author Trystan Jones <crenn6977@gmail.com>
 * @brief Wire library, following the majority of the interface from Arduino.
 *        Provides a 'standard' interface to I2C (two-wire) communication for
 *        derived classes.
 */

/*
 * Library created by crenn to allow a system which would provide users the
 * 'standardised' Arduino method for interfacing with I2C devices regardless of
 * whether it is I2C hardware or emulating software.
 */

#ifndef _WIREBASE_H_
#define _WIREBASE_H_


#include "Arduino.h"

#ifndef WIRE_BUFF_SIZE
#  define WIRE_BUFF_SIZE 32
#endif

/* return codes from endTransmission() */
#define SUCCESS   0        /* transmission was successful */
#define EDATA     1        /* too much data */
#define ENACKADDR 2        /* received nack on transmit of address */
#define ENACKTRNS 3        /* received nack on transmit of data */
#define EOTHER    4        /* other error */

struct i2c_msg;

typedef struct i2c_msg
{
    uint16_t addr;                /**< Address */

#define I2C_MSG_READ            0x1
#define I2C_MSG_10BIT_ADDR      0x2
    /**
     * Bitwise OR of:
     * - I2C_MSG_READ (write is default)
     * - I2C_MSG_10BIT_ADDR (7-bit is default) */
    uint16_t flags;

    uint16_t length;              /**< Message length */
    uint16_t xferred;             /**< Messages transferred */
    uint8_t *data;                /**< Data */
} i2c_msg;

class WireBase   // Abstraction is awesome!
{
protected:
    i2c_msg itc_msg;
    uint8_t rx_buf[WIRE_BUFF_SIZE];      /* receive buffer */
    uint8_t rx_buf_idx;               /* first unread idx in rx_buf */
    uint8_t rx_buf_len;               /* number of bytes read */

    uint8_t tx_buf[WIRE_BUFF_SIZE];      /* transmit buffer */
    uint8_t tx_buf_idx;  // next idx available in tx_buf, -1 overflow
    boolean tx_buf_overflow;

    // Force derived classes to define process function
    virtual uint8_t process() = 0;
public:
    WireBase() {}
    virtual ~WireBase() {}

    /*
     * Initialises the class interface
     */
    // Allow derived classes to overwrite begin function
    void begin(uint8_t = 0x00);

    void setClock(uint32_t);

    /*
     * Sets up the transmission message to be processed
     */
    void beginTransmission(uint8_t);

    /*
     * Allow only 8 bit addresses to be used
     */
    void beginTransmission(int);

    /*
     * Call the process function to process the message if the TX
     * buffer has not overflowed.
     */
    uint8_t endTransmission(void);

    inline uint8_t endTransmission(uint8_t)
    {
        return endTransmission();
    }

    /*
     * Request bytes from a slave device and process the request,
     * storing into the receiving buffer.
     */
    uint8_t requestFrom(uint8_t, int);

    /*
     * Allow only 8 bit addresses to be used when requesting bytes
     */
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, bool);

    /*
     * Stack up bytes to be sent when transmitting
     */
    void write(uint8_t);

    /*
     * Stack up bytes from the array to be sent when transmitting
     */
    void write(uint8_t*, int);

    /*
     * Ensure that a sending data will only be 8-bit bytes
     */
    void write(int);

    /*
     * Ensure that an array sending data will only be 8-bit bytes
     */
    void write(int*, int);

    /*
     * Stack up bytes from a string to be sent when transmitting
     */
    void write(char*);

    /*
     * Return the amount of bytes that is currently in the receiving buffer
     */
    uint8_t available();

    /*
     * Return the value of byte in the receiving buffer that is currently being
     * pointed to
     */
    uint8_t read();
};

#endif // _WIREBASE_H_
