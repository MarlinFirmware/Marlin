/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
#pragma once

//
// Voxelab Aquila V1.0.1 (HC32F460) as found in the Voxelab Aquila X2
//
#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Aquila V1.0.1 only supports one hotend and E-stepper"
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Aquila V1.0.1"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Aquila"
#endif

//
// on-board crystal oscillator
//
#ifndef BOARD_XTAL_FREQUENCY
  #define BOARD_XTAL_FREQUENCY 8000000 // 8 MHz XTAL
#endif

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM
  // #define SDCARD_EEPROM_EMULATION
  #undef NO_EEPROM_SELECTED
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA PA11
  #define IIC_EEPROM_SCL PA12
  #define MARLIN_EEPROM_SIZE 0x800 // 2Kb (24C16)
#elif ENABLED(SDCARD_EEPROM_EMULATION)
  #define MARLIN_EEPROM_SIZE 0x800 // 2K
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN PB0 // BLTouch OUT
#endif

//
// Limit Switches
//
#ifndef X_STOP_PIN
  #define X_STOP_PIN PA5
#endif
#ifndef Y_STOP_PIN
  #define Y_STOP_PIN PA6
#endif
#ifndef Z_STOP_PIN
  #define Z_STOP_PIN PA7
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN PB1 // BLTouch IN
#endif

//
// Filament Runout Sensor
//
// Note: normally high, pulled low when filament is present
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN PA4
#endif

//
// Steppers
//
#ifndef X_ENABLE_PIN
  #define X_ENABLE_PIN PC3 // all steppers share enable pins
#endif
#ifndef X_STEP_PIN
  #define X_STEP_PIN PC2
#endif
#ifndef X_DIR_PIN
  #define X_DIR_PIN PB9
#endif

#ifndef Y_ENABLE_PIN
  #define Y_ENABLE_PIN X_ENABLE_PIN
#endif
#ifndef Y_STEP_PIN
  #define Y_STEP_PIN PB8
#endif
#ifndef Y_DIR_PIN
  #define Y_DIR_PIN PB7
#endif

#ifndef Z_ENABLE_PIN
  #define Z_ENABLE_PIN X_ENABLE_PIN
#endif
#ifndef Z_STEP_PIN
  #define Z_STEP_PIN PB6
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN PB5
#endif

#ifndef E0_ENABLE_PIN
  #define E0_ENABLE_PIN X_ENABLE_PIN
#endif
#ifndef E0_STEP_PIN
  #define E0_STEP_PIN PB4
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN PB3
#endif

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
// #define DISABLE_DEBUG
// #define DISABLE_JTAG

//
// Temperature Sensors
//
#define TEMP_0_PIN PC5   // HEATER1 ADC1_IN15
#define TEMP_BED_PIN PC4 // HOT BED ADC1_IN14

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN PA1 // HEATER1
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN PA2 // HOT BED
#endif

#ifndef FAN_PIN
  #define FAN0_PIN PA0 // FAN0
#endif
#define FAN_SOFT_PWM_REQUIRED

//
// SD Card
//
#define SD_DETECT_PIN PA10
#define SDCARD_CONNECTION ONBOARD
#define ONBOARD_SDIO
#define NO_SD_HOST_DRIVE // This board's SD card is only seen by the printer

//
// Screen Pins
//
// Screen Port Pinout:
// TODO: check screen port pinout
//    ------
// ? | 1  2 | ?
// ? | 3  4 | ?
// ?   5  6 | ?
// ? | 7  8 | ?
// ? | 9 10 | ?
//    ------

#if ANY(HAS_DWIN_E3V2, IS_DWIN_MARLINUI)  
  #define BTN_ENC PB14
  #define BTN_EN1 PB15
  #define BTN_EN2 PB12
  
  #ifndef BEEPER_PIN
    #define BEEPER_PIN PB13
  #endif
#endif

//
// SDIO Pins
//
#define BOARD_SDIO_D0 PC8
#define BOARD_SDIO_D1 PC9
#define BOARD_SDIO_D2 PC10
#define BOARD_SDIO_D3 PC11
#define BOARD_SDIO_CLK PC12
#define BOARD_SDIO_CMD PD2
#define BOARD_SDIO_DET PA10

//
// USART Pins
//
#define BOARD_USART1_TX_PIN PC0
#define BOARD_USART1_RX_PIN PC1

#define BOARD_USART2_TX_PIN PA9
#define BOARD_USART2_RX_PIN PA15

#define BOARD_USART3_TX_PIN PE5
#define BOARD_USART3_RX_PIN PE4

// on-board LED (?)
// #define LED PA3
