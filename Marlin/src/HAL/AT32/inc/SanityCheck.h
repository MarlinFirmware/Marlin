/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 */
#pragma once

/**
 * Test AT32-specific configuration values for errors at compile-time.
 */

/**
 * Require pins to be defined as macros with numerical values
 */
#ifndef PA0
  #error "Your ARM platform pins are not defined as macros, only as enums! Provide pins_arduino.h and/or 'buildroot/share/PlatformIO/variants/*/variant.h' to define the pins."
#endif

// Require Arduino core version check if available
#if defined(ARDUINO_CORE_VERSION_INT) && ARDUINO_CORE_VERSION_INT < 100000
  #error "The AT32 HAL requires Arduino Core version >= 1.0.0. Consider updating the Arduino Core."
#endif

// Serial stats not supported on AT32
#if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
  #error "SERIAL_STATS_MAX_RX_QUEUED is not supported on AT32."
#elif ENABLED(SERIAL_STATS_DROPPED_RX)
  #error "SERIAL_STATS_DROPPED_RX is not supported on AT32."
#endif

// SDCARD_EEPROM_EMULATION requires SDSUPPORT
#if ENABLED(SDCARD_EEPROM_EMULATION) && !HAS_MEDIA
  #undef SDCARD_EEPROM_EMULATION
  #if USE_FALLBACK_EEPROM
    #warning "EEPROM type not specified. Fallback is SDCARD_EEPROM_EMULATION."
  #endif
  #error "SDCARD_EEPROM_EMULATION requires SDSUPPORT. Enable SDSUPPORT or choose another EEPROM emulation."
#endif

// TEMP_SENSOR_SOC requires TEMP_SOC_PIN
#if TEMP_SENSOR_SOC
  #ifndef TEMP_SOC_PIN
    #error "TEMP_SOC_PIN must be defined to use TEMP_SENSOR_SOC on AT32."
  #elif IS_GPIO_PIN(TEMP_SOC_PIN)
    #error "TEMP_SOC_PIN must not be a valid GPIO pin to avoid conflicts."
  #endif
#endif

// Emergency Parser needs at least one serial with HardwareSerial
#if ENABLED(EMERGENCY_PARSER) && ((SERIAL_PORT == -1 && !defined(SERIAL_PORT_2)) || (SERIAL_PORT_2 == -1 && !defined(SERIAL_PORT)))
  #error "EMERGENCY_PARSER requires HardwareSerial on AT32."
#endif

// Check for common serial pin conflicts
#define _CHECK_SERIAL_PIN(N) (( \
    BTN_EN1 == N || BTN_EN2 == N || DOGLCD_CS == N || HEATER_BED_PIN == N || FAN0_PIN == N \
  ))
#define CHECK_SERIAL_PIN(T,N) defined(UART##N##_##T##_PIN) && _CHECK_SERIAL_PIN(UART##N##_##T##_PIN)
#if SERIAL_IN_USE(1)
  #if CHECK_SERIAL_PIN(TX,1)
    #error "Serial Port 1 TX IO pins conflict with another pin on the board."
  #endif
  #if CHECK_SERIAL_PIN(RX,1)
    #error "Serial Port 1 RX IO pins conflict with another pin on the board."
  #endif
#endif
#if SERIAL_IN_USE(2)
  #if CHECK_SERIAL_PIN(TX,2)
    #error "Serial Port 2 TX IO pins conflict with another pin on the board."
  #endif
  #if CHECK_SERIAL_PIN(RX,2)
    #error "Serial Port 2 RX IO pins conflict with another pin on the board."
  #endif
#endif
#if SERIAL_IN_USE(3)
  #if CHECK_SERIAL_PIN(TX,3)
    #error "Serial Port 3 TX IO pins conflict with another pin on the board."
  #endif
  #if CHECK_SERIAL_PIN(RX,3)
    #error "Serial Port 3 RX IO pins conflict with another pin on the board."
  #endif
#endif
#undef CHECK_SERIAL_PIN
#undef _CHECK_SERIAL_PIN
