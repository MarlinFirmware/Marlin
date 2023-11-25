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
 * The SoftWire library is used for the software I2C. That means that
 * ANY pin can be selected for SDA and SLC.
 *
 * This driver requires the SDA and SLC pins be named:
 *   DOGLCD_SDA
 *   DOGLCD_SCL
 *
 * This allows independence from other I2C devices (mostly EEPROMs) that
 * usually use I2C_SDA_PIN and I2C_SLC_PIN.
 *
 * The following MUST be present in the platformio environment used to
 * compile Marlin. This required because
 *
 * The flag LCD_I2C_SOFT is used to select which I2C library is used.
 * It must be enabled in the platformio environment used to compile
 * Marlin.  Enabling it anywhere else results in LCD_I2C_SOFT not being
 * enabled during crucial portions of the compile process which results
 * in defaulting to hardwired operation.  The net result is the following
 * MUST be in the platformio environment in order to use the software I2C
 * system:
 *    build_flags = -DLCD_I2C_SOFT
 *    lib_deps    = stevemarple/SoftWire@^2.0.9
 *                  stevemarple/AsyncDelay@^1.1.2
 *
 */

#ifdef ARDUINO_ARCH_STM32

#include "../../inc/MarlinConfigPre.h"

#if ANY(U8GLIB_SH1106, IS_U8GLIB_SSD1306, U8GLIB_SSD1309)

#include <U8glib-HAL.h>

#include "../../MarlinCore.h"  // so can get SDA & SCL pins

/**
 * BUFFER_LENGTH is defined in libraries/Wire/utility/WireBase.h
 * Default value is 32
 * Increase this value to 144 to send U8G_COM_MSG_WRITE_SEQ in a single block
 */
#if !BUFFER_LENGTH || BUFFER_LENGTH >= 144
  #undef BUFFER_LENGTH
  #define BUFFER_LENGTH 32
#endif
#define I2C_MAX_LENGTH (BUFFER_LENGTH - 1)

#ifdef LCD_I2C_SOFT
  #include <SoftWire.h>
  #include <AsyncDelay.h>

  char swTxBuffer[BUFFER_LENGTH];
  char swRxBuffer[BUFFER_LENGTH];
  SoftWire sw(DOGLCD_SDA, DOGLCD_SCL);
  #define I2C_ITF sw

#elif !defined(HAL_I2C_MODULE_DISABLED)
  #include <Wire.h>
  #define I2C_ITF Wire
  #ifndef MASTER_ADDRESS
    #define MASTER_ADDRESS 0x01
  #endif
#else
  #error "Unsupported I2C configuration"
#endif

static uint8_t control;
static uint8_t msgInitCount = 2; // Ignore all messages until 2nd U8G_COM_MSG_INIT

uint8_t u8g_com_stm32duino_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  if (msgInitCount) {
    if (msg == U8G_COM_MSG_INIT) msgInitCount--;
    if (msgInitCount) return -1;
  }

  switch (msg) {
    case U8G_COM_MSG_INIT:
      #ifdef LCD_I2C_SOFT
        I2C_ITF.setClock(100000);
        I2C_ITF.setTxBuffer(swTxBuffer, BUFFER_LENGTH);
        I2C_ITF.setRxBuffer(swRxBuffer, BUFFER_LENGTH);
        I2C_ITF.begin();
      #else
        I2C_ITF.setClock(400000);
        I2C_ITF.setSCL(DOGLCD_SCL);
        I2C_ITF.setSDA(DOGLCD_SDA);
        I2C_ITF.begin(MASTER_ADDRESS, 0); // start as master
      #endif
      break;

    case U8G_COM_MSG_ADDRESS:           /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      control = arg_val ? 0x40 : 0x00;
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      I2C_ITF.beginTransmission(0x3c);
      I2C_ITF.write(control);
      I2C_ITF.write(arg_val);
      I2C_ITF.endTransmission();
      break;

    case U8G_COM_MSG_WRITE_SEQ: {
      uint8_t* dataptr = (uint8_t*)arg_ptr;
      #ifdef I2C_MAX_LENGTH
        while (arg_val > 0) {
          I2C_ITF.beginTransmission(0x3c);
          I2C_ITF.write(control);
          if (arg_val <= I2C_MAX_LENGTH) {
            I2C_ITF.write(dataptr, arg_val);
            arg_val = 0;
          }
          else {
            I2C_ITF.write(dataptr, I2C_MAX_LENGTH);
            arg_val -= I2C_MAX_LENGTH;
            dataptr += I2C_MAX_LENGTH;
          }
          I2C_ITF.endTransmission();
        }
      #else
        I2C_ITF.beginTransmission(0x3c);
        I2C_ITF.write(control);
        I2C_ITF.write(dataptr, arg_val);
        I2C_ITF.endTransmission();
      #endif // I2C_MAX_LENGTH
      break;
    }

  }
  return 1;
}

#endif // U8GLIB_SH1106 || IS_U8GLIB_SSD1306 || U8GLIB_SSD1309
#endif // ARDUINO_ARCH_STM32
