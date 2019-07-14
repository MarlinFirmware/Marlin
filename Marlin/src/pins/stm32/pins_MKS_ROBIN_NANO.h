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
#pragma once

/**
 * MKS Robin nano (STM32F130VET6) board pin assignments
 */

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS Robin nano supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "MKS Robin nano"

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

//
// Note: MKS Robin board is using SPI2 interface.
//
#define SPI_MODULE 2

//
// Limit Switches
//
#define X_STOP_PIN        PA15
#define Y_STOP_PIN        PA12
#define Z_MIN_PIN         PA11
#define Z_MAX_PIN         PC4

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PA4   // MT_DET
#endif

//
// Steppers
//
#define X_ENABLE_PIN       PE4
#define X_STEP_PIN         PE3
#define X_DIR_PIN          PE2

#define Y_ENABLE_PIN       PE1
#define Y_STEP_PIN         PE0
#define Y_DIR_PIN          PB9

#define Z_ENABLE_PIN       PB8
#define Z_STEP_PIN         PB5
#define Z_DIR_PIN          PB4

#define E0_ENABLE_PIN      PB3
#define E0_STEP_PIN        PD6
#define E0_DIR_PIN         PD3

#define E1_ENABLE_PIN      PA3
#define E1_STEP_PIN        PA6
#define E1_DIR_PIN         PA1

//
// Temperature Sensors
//
#define TEMP_0_PIN         PC1   // TH1
#define TEMP_1_PIN         PC2   // TH2
#define TEMP_BED_PIN       PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN       PC3   // HEATER1
#define HEATER_1_PIN       PB0   // HEATER2
#define HEATER_BED_PIN     PA0   // HOT BED

#define FAN_PIN            PB1   // FAN

//
// Thermocouples
//
//#define MAX6675_SS_PIN     PE5  // TC1 - CS1
//#define MAX6675_SS_PIN     PE6  // TC2 - CS2

//
// Misc. Functions
//
#define POWER_LOSS_PIN     PA2   // PW_DET
#define PS_ON_PIN          PA3   // PW_OFF

#define LED_PIN            PB2

//
// LCD / Controller
//
#define BEEPER_PIN         PC5
#define SD_DETECT_PIN      PD12

/**
 * Note: MKS Robin TFT screens use various TFT controllers.
 * If the screen stays white, disable 'LCD_RESET_PIN'
 * to let the bootloader init the screen.
 */
#if ENABLED(MKS_ROBIN_TFT)
  #define LCD_RESET_PIN      PF6
  #define NO_LCD_REINIT             // Suppress LCD re-initialization

  #define LCD_BACKLIGHT_PIN  PD13

  #if ENABLED(TOUCH_BUTTONS)
    #define BTN_ENC          PC13   // Not connected. TODO: Replace this hack to enable button code
    #define FSMC_CS_PIN      PD7    // NE4
    #define FSMC_RS_PIN      PD11   // A0
    #define TOUCH_CS_PIN     PA7
  #endif
#endif
