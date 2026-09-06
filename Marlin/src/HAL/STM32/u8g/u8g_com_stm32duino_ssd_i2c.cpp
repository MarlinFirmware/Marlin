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
 * Handles both Hardware and Software I2C so any pins can be used as SDA and SLC.
 * Wire library is used for Hardware I2C.
 * SlowSoftWire is used for Software I2C.
 *
 * Wire / SoftWire library selection can be done automatically at runtime.
 *
 * SDA and SLC pins must be named DOGLCD_SDA_PIN, DOGLCD_SCL_PIN to distinguish
 * from other I2C devices (e.g., EEPROM) that use I2C_SDA_PIN, I2C_SLC_PIN.
 */
#ifdef ARDUINO_ARCH_STM32

#include "../../../inc/MarlinConfig.h"

#if HAS_U8GLIB_I2C_OLED

#include <U8glib-HAL.h>

#if ENABLED(U8G_USES_HW_I2C)
  #include <Wire.h>
  #ifndef MASTER_ADDRESS
    #define MASTER_ADDRESS 0x01
  #endif
#endif

#if ENABLED(U8G_USES_SW_I2C)
  #include <SlowSoftI2CMaster.h>
  #include <SlowSoftWire.h>
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

uint8_t u8g_com_stm32duino_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  // Hardware I2C flag
  #ifdef COMPILE_TIME_I2C_IS_HARDWARE
    constexpr bool isHardI2C = ENABLED(COMPILE_TIME_I2C_IS_HARDWARE);
  #else
    static bool isHardI2C = false;
    static bool i2c_initialized = false;    // Flag to only run init/linking code once
    if (!i2c_initialized) {                 // Init runtime linkages
      i2c_initialized = true;               // Only do this once
      I2C_TypeDef *i2cInstance1 = (I2C_TypeDef *)pinmap_peripheral(digitalPinToPinName(DOGLCD_SDA_PIN), PinMap_I2C_SDA);
      I2C_TypeDef *i2cInstance2 = (I2C_TypeDef *)pinmap_peripheral(digitalPinToPinName(DOGLCD_SCL_PIN), PinMap_I2C_SCL);
      isHardI2C = (i2cInstance1 && (i2cInstance1 == i2cInstance2)); // Found hardware I2C controller
    }
  #endif

  static uint8_t msgInitCount = 0;          // Ignore all messages until 2nd U8G_COM_MSG_INIT
  if (msgInitCount) {
    if (msg == U8G_COM_MSG_INIT) msgInitCount--;
    if (msgInitCount) return -1;
  }

  static uint8_t control;
  if (isHardI2C) {                          // Found hardware I2C controller
    #if ENABLED(U8G_USES_HW_I2C)
      static TwoWire wire2;                 // A TwoWire object for use below
      switch (msg) {
        case U8G_COM_MSG_INIT:
          wire2.setClock(400000);
          wire2.setSCL(DOGLCD_SCL_PIN);
          wire2.setSDA(DOGLCD_SDA_PIN);
          wire2.begin(MASTER_ADDRESS, 0);   // Start as master
          break;

        case U8G_COM_MSG_ADDRESS:           // Define cmd (arg_val = 0) or data mode (arg_val = 1)
          control = arg_val ? 0x40 : 0x00;
          break;

        case U8G_COM_MSG_WRITE_BYTE:
          wire2.beginTransmission(0x3C);
          wire2.write(control);
          wire2.write(arg_val);
          wire2.endTransmission();
          break;

        case U8G_COM_MSG_WRITE_SEQ: {
          uint8_t* dataptr = (uint8_t*)arg_ptr;
          #ifdef I2C_MAX_LENGTH
            while (arg_val > 0) {
              wire2.beginTransmission(0x3C);
              wire2.write(control);
              if (arg_val <= I2C_MAX_LENGTH) {
                wire2.write(dataptr, arg_val);
                arg_val = 0;
              }
              else {
                wire2.write(dataptr, I2C_MAX_LENGTH);
                arg_val -= I2C_MAX_LENGTH;
                dataptr += I2C_MAX_LENGTH;
              }
              wire2.endTransmission();
            }
          #else
            wire2.beginTransmission(0x3C);
            wire2.write(control);
            wire2.write(dataptr, arg_val);
            wire2.endTransmission();
          #endif // I2C_MAX_LENGTH
          break;
        }
      }
    #endif // U8G_USES_HW_I2C
  }
  else {    // Software I2C
    #if ENABLED(U8G_USES_SW_I2C)
      static SlowSoftWire sWire = SlowSoftWire(DOGLCD_SDA_PIN, DOGLCD_SCL_PIN);

      switch (msg) {
        case U8G_COM_MSG_INIT:
          sWire.setClock(400000);
          sWire.begin();                    // Start as master
          break;

        case U8G_COM_MSG_ADDRESS:           // Define cmd (arg_val = 0) or data mode (arg_val = 1)
          control = arg_val ? 0x40 : 0x00;
          break;

        case U8G_COM_MSG_WRITE_BYTE:
          sWire.beginTransmission((uint8_t)0x3C);
          sWire.write((uint8_t)control);
          sWire.write((uint8_t)arg_val);
          sWire.endTransmission();
          break;

        case U8G_COM_MSG_WRITE_SEQ: {
          uint8_t* dataptr = (uint8_t*)arg_ptr;
          #ifdef I2C_MAX_LENGTH
            while (arg_val > 0) {
              sWire.beginTransmission((uint8_t)0x3C);
              sWire.write((uint8_t)control);
              if (arg_val <= I2C_MAX_LENGTH) {
                sWire.write((const uint8_t *)dataptr, (size_t)arg_val);
                arg_val = 0;
              }
              else {
                sWire.write((const uint8_t *)dataptr, I2C_MAX_LENGTH);
                arg_val -= I2C_MAX_LENGTH;
                dataptr += I2C_MAX_LENGTH;
              }
              sWire.endTransmission();
            }
          #else
            sWire.beginTransmission((uint8_t)0x3C);
            sWire.write((uint8_t)control);
            sWire.write((const uint8_t *)dataptr, (size_t)arg_val);
            sWire.endTransmission();
          #endif // I2C_MAX_LENGTH
          break;
        }
      }
    #endif // U8G_USES_SW_I2C
  }

  return 1;
}

#endif // HAS_U8GLIB_I2C_OLED
#endif // ARDUINO_ARCH_STM32
