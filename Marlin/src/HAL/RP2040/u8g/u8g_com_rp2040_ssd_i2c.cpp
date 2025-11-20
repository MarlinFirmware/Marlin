/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * 2-Wire I2C COM Driver
 *
 * Handles Hardware I2C on valid pin combinations.
 * Wire library is used for Hardware I2C.
 *
 * Hardware I2C uses pins defined in pins_arduino.h.
 */

#ifdef __PLAT_RP2040__

#include "../../../inc/MarlinConfig.h"

#if HAS_U8GLIB_I2C_OLED

#include <U8glib-HAL.h>

#include <Wire.h>
#ifndef MASTER_ADDRESS
  #define MASTER_ADDRESS 0x01
#endif

/**
 * BUFFER_LENGTH is defined in libraries\Wire\utility\WireBase.h
 * Default value is 32
 * Increase this value to 144 to send U8G_COM_MSG_WRITE_SEQ in single block
 */
#ifndef BUFFER_LENGTH
  #define BUFFER_LENGTH 32
#endif
#if BUFFER_LENGTH > 144
  #error "BUFFER_LENGTH should not be greater than 144."
#endif
#define I2C_MAX_LENGTH (BUFFER_LENGTH - 1)

uint8_t u8g_com_rp2040_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {

  // Hardware I2C flag
  //static bool i2c_initialized = false;  // Flag to only run init/linking code once
  //if (!i2c_initialized) {               // Init runtime linkages
  //  i2c_initialized = true;             // Only do this once
  //}

  static uint8_t control;
  // Use the global Wire instance (already initialized with correct pins for RP2040)
  switch (msg) {
    case U8G_COM_MSG_INIT:
      Wire.setClock(400000);
      // Wire already initialized in MarlinUI::init(), no need to call begin() again
      break;

    case U8G_COM_MSG_ADDRESS:           // Define cmd (arg_val = 0) or data mode (arg_val = 1)
      control = arg_val ? 0x40 : 0x00;
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      ::Wire.beginTransmission(0x3C);
      ::Wire.write(control);
      ::Wire.write(arg_val);
      ::Wire.endTransmission();
      break;

    case U8G_COM_MSG_WRITE_SEQ: {
      uint8_t* dataptr = (uint8_t*)arg_ptr;
      while (arg_val > 0) {
        ::Wire.beginTransmission(0x3C);
        ::Wire.write(control);
        if (arg_val <= I2C_MAX_LENGTH) {
          ::Wire.write(dataptr, arg_val);
          arg_val = 0;
        }
        else {
          ::Wire.write(dataptr, I2C_MAX_LENGTH);
          arg_val -= I2C_MAX_LENGTH;
          dataptr += I2C_MAX_LENGTH;
        }
        ::Wire.endTransmission();
      }
      break;
    }
  }
  return 1;
}

#endif // HAS_U8GLIB_I2C_OLED
#endif // __PLAT_RP2040__
