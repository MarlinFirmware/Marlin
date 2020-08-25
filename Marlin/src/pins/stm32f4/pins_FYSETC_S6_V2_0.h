/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "pins_FYSETC_S6.h"

//
// EEPROM Emulation
//
#if NO_EEPROM_SELECTED
  //#define FLASH_EEPROM_EMULATION
  //#define SRAM_EEPROM_EMULATION
  #define I2C_EEPROM
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#elif ENABLED(I2C_EEPROM)
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
#endif

//
// Steppers
//
#undef X_ENABLE_PIN
#define X_ENABLE_PIN       PE9

#if HAS_TMC220x
  //
  // TMC2208/TMC2209 stepper drivers
  //

  //
  // Software serial
  //
  #undef X_SERIAL_TX_PIN
  #define X_SERIAL_TX_PIN  PE8

  #undef Y_SERIAL_TX_PIN
  #undef Y_SERIAL_RX_PIN
  #define Y_SERIAL_TX_PIN  PC4
  #define Y_SERIAL_RX_PIN  PC4

  #undef Z_SERIAL_TX_PIN
  #define Z_SERIAL_TX_PIN  PD12

  #undef E0_SERIAL_TX_PIN
  #define E0_SERIAL_TX_PIN PA15

  #undef E1_SERIAL_TX_PIN
  #define E1_SERIAL_TX_PIN PC5

  #undef E2_SERIAL_TX_PIN
  #define E2_SERIAL_TX_PIN PE0
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#define TMC_USE_SW_SPI
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI    PE14
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO    PE13
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK     PE12
  #endif
#endif
