/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * MKS Robin MINI (STM32F130VET6) board pin assignments
 */

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "MKS Robin mini supports up to 1 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "MKS Robin mini"

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

#define FLASH_EEPROM_EMULATION
// 2K in a AT24C16N
#define EEPROM_PAGE_SIZE      (uint16)0x800 // 2048
#define EEPROM_START_ADDRESS  ((uint32)(0x8000000 + 512 * 1024 - 2 * EEPROM_PAGE_SIZE))
#define E2END (EEPROM_PAGE_SIZE - 1)

//
// Note: MKS Robin mini board is using SPI2 interface.
//
#define SPI_MODULE 2

//
// Limit Switches
//
#define X_STOP_PIN                          PA15
#define Y_STOP_PIN                          PA12
#define Z_MIN_PIN                           PA11
#define Z_MAX_PIN                           PC4

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA4   // MT_DET
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PE4
#define X_STEP_PIN                          PE3
#define X_DIR_PIN                           PE2

#define Y_ENABLE_PIN                        PE1
#define Y_STEP_PIN                          PE0
#define Y_DIR_PIN                           PB9

#define Z_ENABLE_PIN                        PB8
#define Z_STEP_PIN                          PB5
#define Z_DIR_PIN                           PB4

#define E0_ENABLE_PIN                       PB3
#define E0_STEP_PIN                         PD6
#define E0_DIR_PIN                          PD3

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC3   // HEATER1
#define HEATER_BED_PIN                      PA0   // HOT BED

#define FAN_PIN                             PB1   // FAN

//
// Thermocouples
//
//#define MAX6675_SS_PIN                    PE5   // TC1 - CS1
//#define MAX6675_SS_PIN                    PE6   // TC2 - CS2

//
// Misc. Functions
//
#define POWER_LOSS_PIN                      PA2   // PW_DET
#define PS_ON_PIN                           PA3   // PW_OFF

//#define LED_PIN                           PB2

//
// LCD / Controller
//
#define BEEPER_PIN                          PC5
#define SD_DETECT_PIN                       PD12

/**
 * Note: MKS Robin TFT screens use various TFT controllers.
 * If the screen stays white, disable 'LCD_RESET_PIN'
 * to let the bootloader init the screen.
 */
#if ENABLED(FSMC_GRAPHICAL_TFT)
  #define FSMC_CS_PIN                       PD7   // NE4
  #define FSMC_RS_PIN                       PD11  // A0

  #define LCD_RESET_PIN                     PC6
  #define NO_LCD_REINIT                           // Suppress LCD re-initialization

  #define LCD_BACKLIGHT_PIN                 PD13

  #if ENABLED(TOUCH_BUTTONS)
    #define TOUCH_CS_PIN                    PC2
    #define TOUCH_SCK_PIN                   PB13
    #define TOUCH_MOSI_PIN                  PB15
    #define TOUCH_MISO_PIN                  PB14
  #endif
#endif

// Motor current PWM pins
#define MOTOR_CURRENT_PWM_XY_PIN            PA6
#define MOTOR_CURRENT_PWM_Z_PIN             PA7
#define MOTOR_CURRENT_PWM_E_PIN             PB0
#define MOTOR_CURRENT_PWM_RANGE 1500              // (255 * (1000mA / 65535)) * 257 = 1000 is equal 1.6v Vref in turn equal 1Amp
#define DEFAULT_PWM_MOTOR_CURRENT  { 1030, 1030, 1030 } // 1.05Amp per driver, here is XY, Z and E. This values determined empirically.

// This is a kind of workaround in case native marlin "digipot" interface won't work.
// Required to enable related code in STM32F1/HAL.cpp
//#ifndef MKS_ROBIN_MINI_VREF_PWM
//  #define MKS_ROBIN_MINI_VREF_PWM
//#endif

//#define VREF_XY_PIN                       PA6
//#define VREF_Z_PIN                        PA7
//#define VREF_E1_PIN                       PB0
