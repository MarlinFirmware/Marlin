/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


/**
 * 2 wire I2C COM driver
 *
 * Handles both hardware I2C and software I2C.
 *
 * The WIRE library is used for the hardware I2C.  That means that
 * ANY hardware SDA and SLC pairs can be selected and the WIRE library
 * will automatically enable the correct hardware.
 *
 * The SlowSoftWire library is used for the software I2C. That means that
 * ANY pin can be selected for SDA and SLC.
 *
 * Selection of the WIRE library or the SoftWire library is automatically done
 * at run time.
 *
 * This driver requires the SDA and SLC pins be named:
 *   DOGLCD_SDA
 *   DOGLCD_SCL
 *
 * This allows independence from other I2C devices (mostly EEPROMs) that
 * usually use I2C_SDA_PIN and I2C_SLC_PIN.
 *
 */


#include "../../inc/MarlinConfigPre.h"

#if (defined(ARDUINO_ARCH_STM32) && (defined(U8GLIB_SH1106) || defined(IS_U8GLIB_SSD1306) || defined(U8GLIB_SSD1309)))

#include <U8glib-HAL.h>

#include "../../MarlinCore.h"  // so can get SDA & SCL pins
#include <Wire.h>

#include <SlowSoftI2CMaster.h>
#include <SlowSoftWire.h>


/*
  BUFFER_LENGTH is defined in libraries\Wire\utility\WireBase.h
  Default value is 32
  Increate this value to 144 to send U8G_COM_MSG_WRITE_SEQ in single block
*/

#if !defined(BUFFER_LENGTH) || BUFFER_LENGTH >= 144
  #define BUFFER_LENGTH 32
#endif
#define I2C_MAX_LENGTH (BUFFER_LENGTH - 1)

#ifndef MASTER_ADDRESS
  #define MASTER_ADDRESS 0x01
#endif

static uint8_t control;
static uint8_t msgInitCount = 0; // Ignore all messages until 2nd U8G_COM_MSG_INIT

static uint8_t I2C_initialized = 0;  // flag to only run init/linking code once
static uint8_t HARD_I2C = 0;         // 1 - hard I2C, 0 - soft I2C

TwoWire Wire2;  // Create an object of TwoWire
SlowSoftWire I2C_soft = SlowSoftWire((uint8_t)DOGLCD_SDA, (uint8_t)DOGLCD_SCL);

uint8_t u8g_com_stm32duino_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  I2C_TypeDef *i2cInstance;
  if(!I2C_initialized) {  // init runtime linkages
      I2C_initialized = 1;  //only do this once
      i2cInstance      = (I2C_TypeDef *)pinmap_peripheral(digitalPinToPinName(DOGLCD_SDA), PinMap_I2C_SDA);
      if (i2cInstance != (I2C_TypeDef *)pinmap_peripheral(digitalPinToPinName(DOGLCD_SCL), PinMap_I2C_SCL)) i2cInstance = NP;

      if (i2cInstance) {  // found hard I2C controller
        HARD_I2C = 1;
      }
  }


  if (msgInitCount) {
    if (msg == U8G_COM_MSG_INIT) msgInitCount--;
    if (msgInitCount) return -1;
  }

  if (HARD_I2C) {  // found hard I2C controller

    switch (msg)
    {
      case U8G_COM_MSG_INIT:
        Wire2.setClock(400000);
        Wire2.setSCL(DOGLCD_SCL);
        Wire2.setSDA(DOGLCD_SDA);
        Wire2.begin(MASTER_ADDRESS, 0); // start as master
        break;

      case U8G_COM_MSG_ADDRESS:           /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
        control = arg_val ? 0x40 : 0x00;
        break;

      case U8G_COM_MSG_WRITE_BYTE:
        Wire2.beginTransmission(0x3c);
        Wire2.write(control);
        Wire2.write(arg_val);
        Wire2.endTransmission();
        break;

      case U8G_COM_MSG_WRITE_SEQ:
      {
        uint8_t* dataptr = (uint8_t*)arg_ptr;
        #ifdef I2C_MAX_LENGTH
          while (arg_val > 0) {
            Wire2.beginTransmission(0x3c);
            Wire2.write(control);
            if (arg_val <= I2C_MAX_LENGTH) {
              Wire2.write(dataptr, arg_val);
              arg_val = 0;
            }
            else {
              Wire2.write(dataptr, I2C_MAX_LENGTH);
              arg_val -= I2C_MAX_LENGTH;
              dataptr += I2C_MAX_LENGTH;
            }
            Wire2.endTransmission();
          }
        #else
          Wire2.beginTransmission(0x3c);
          Wire2.write(control);
          Wire2.write(dataptr, arg_val);
          Wire2.endTransmission();
        #endif // I2C_MAX_LENGTH
        break;
      }

    }
  return 1;
  }

  else {    // use soft I2C

    switch (msg)
    {
      case U8G_COM_MSG_INIT:
        I2C_soft.setClock(400000);
        I2C_soft.begin(); // start as master
        break;

      case U8G_COM_MSG_ADDRESS:           /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
        control = arg_val ? 0x40 : 0x00;
        break;

      case U8G_COM_MSG_WRITE_BYTE:
        I2C_soft.beginTransmission((uint8_t)0x3c);
        I2C_soft.write((uint8_t)control);
        I2C_soft.write((uint8_t)arg_val);
        I2C_soft.endTransmission();
        break;

      case U8G_COM_MSG_WRITE_SEQ:
      {
        uint8_t* dataptr = (uint8_t*)arg_ptr;
        #ifdef I2C_MAX_LENGTH
          while (arg_val > 0) {
            I2C_soft.beginTransmission((uint8_t)0x3c);
            I2C_soft.write((uint8_t)control);
            if (arg_val <= I2C_MAX_LENGTH) {
              I2C_soft.write((const uint8_t *)dataptr, (size_t)arg_val);
              arg_val = 0;
            }
            else {
              I2C_soft.write((const uint8_t *)dataptr, I2C_MAX_LENGTH);
              arg_val -= I2C_MAX_LENGTH;
              dataptr += I2C_MAX_LENGTH;
            }
            I2C_soft.endTransmission();
          }
        #else
          I2C_soft.beginTransmission((uint8_t)0x3c);
          I2C_soft.write((uint8_t)control);
          I2C_soft.write((const uint8_t *)dataptr, (size_t)arg_val);
          I2C_soft.endTransmission();
        #endif // I2C_MAX_LENGTH
        break;
      }

    }
    return 1;
  }
}

#endif // ARDUINO_ARCH_STM32
