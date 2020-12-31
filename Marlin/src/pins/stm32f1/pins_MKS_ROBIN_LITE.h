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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#if NOT_TARGET(__STM32F1__)
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "MKS Robin Lite only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "MKS Robin Lite"
#endif
#define BOARD_WEBSITE_URL "github.com/makerbase-mks"

#define BOARD_NO_NATIVE_USB

//#define DISABLE_DEBUG
#define DISABLE_JTAG

//
// Limit Switches
//
#define X_STOP_PIN                          PC13
#define Y_STOP_PIN                          PC0
#define Z_MIN_PIN                           PC12
#define Z_MAX_PIN                           PB9

//
// Steppers
//
#define X_STEP_PIN                          PC6
#define X_DIR_PIN                           PB12
#define X_ENABLE_PIN                        PB10

#define Y_STEP_PIN                          PB11
#define Y_DIR_PIN                           PB2
#define Y_ENABLE_PIN                        PB10

#define Z_STEP_PIN                          PB1
#define Z_DIR_PIN                           PC5
#define Z_ENABLE_PIN                        PB10

#define E0_STEP_PIN                         PC4
#define E0_DIR_PIN                          PA5
#define E0_ENABLE_PIN                       PA4

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC9
#define FAN_PIN                             PA8
#define HEATER_BED_PIN                      PC8

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PA1
#define TEMP_0_PIN                          PA0

#define FIL_RUNOUT_PIN                      PB8   // MT_DET

//
// LCD Pins
//
#if HAS_WIRED_LCD
  #define BEEPER_PIN                        PD2
  #define BTN_ENC                           PB3
  #define LCD_PINS_RS                       PC3

  #define BTN_EN1                           PB5
  #define BTN_EN2                           PB4

  #define LCD_PINS_ENABLE                   PC2

  #if ENABLED(MKS_MINI_12864)

    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN                   -1
    #define DOGLCD_A0                       PC1
    #define DOGLCD_CS                       PC2
    #define DOGLCD_SCK                      PB13
    #define DOGLCD_MOSI                     PB15

  #else                                           // !MKS_MINI_12864

    #define LCD_PINS_D4                     PC1
    #if IS_ULTIPANEL
      #define LCD_PINS_D5                   -1
      #define LCD_PINS_D6                   -1
      #define LCD_PINS_D7                   -1
    #endif

  #endif // !MKS_MINI_12864

  #if HAS_MARLINUI_U8GLIB
    #ifndef BOARD_ST7920_DELAY_1
      #define BOARD_ST7920_DELAY_1 DELAY_NS(125)
    #endif
    #ifndef BOARD_ST7920_DELAY_2
      #define BOARD_ST7920_DELAY_2 DELAY_NS(125)
    #endif
    #ifndef BOARD_ST7920_DELAY_3
      #define BOARD_ST7920_DELAY_3 DELAY_NS(125)
    #endif
  #endif

#endif // HAS_WIRED_LCD

// Motor current PWM pins
#define MOTOR_CURRENT_PWM_XY_PIN            PB0
#define MOTOR_CURRENT_PWM_Z_PIN             PA7
#define MOTOR_CURRENT_PWM_E_PIN             PA6
#define MOTOR_CURRENT_PWM_RANGE (65535/10/3.3) // (255 * (1000mA / 65535)) * 257 = 1000 is equal 1.6v Vref in turn equal 1Amp
#define DEFAULT_PWM_MOTOR_CURRENT { 1000, 1000, 1000 } // 1.05Amp per driver, here is XY, Z and E. This values determined empirically.

//
// SD Card
//
#define SD_DETECT_PIN                       PC10

//
// SPI
//
#define SPI_DEVICE                             2
#define SCK_PIN                             PB13
#define MISO_PIN                            P1B4
#define MOSI_PIN                            P1B5
#define SS_PIN                              PA15
