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

/**
 * MKS Robin Lite 3 (STM32F103RCT6) board pin assignments
 */

#if NOT_TARGET(__STM32F1__)
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS Robin Lite3 supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "MKS Robin Lite3"
#endif
#define BOARD_WEBSITE_URL "github.com/makerbase-mks"

#define BOARD_NO_NATIVE_USB

//#define DISABLE_DEBUG
#define DISABLE_JTAG

//
// Servos
//
#define SERVO0_PIN                          PA3

//
// Limit Switches
//
#define X_STOP_PIN                          PA12
#define Y_STOP_PIN                          PA11
#define Z_MIN_PIN                           PC6
#define Z_MAX_PIN                           PB1

//
// Steppers
//
#define X_STEP_PIN                          PC0
#define X_DIR_PIN                           PB2
#define X_ENABLE_PIN                        PC13

#define Y_STEP_PIN                          PC2
#define Y_DIR_PIN                           PB9
#define Y_ENABLE_PIN                        PB12

#define Z_STEP_PIN                          PB7
#define Z_DIR_PIN                           PB6
#define Z_ENABLE_PIN                        PB8

#define E0_STEP_PIN                         PB4
#define E0_DIR_PIN                          PB3
#define E0_ENABLE_PIN                       PB5

#define E1_STEP_PIN                         PC12
#define E1_DIR_PIN                          PC11
#define E1_ENABLE_PIN                       PD2

//
// Heaters 0,1 / Fans / Bed
//
#define HEATER_0_PIN                        PC9
#define HEATER_1_PIN                        PC7
#define FAN_PIN                             PA8
#define HEATER_BED_PIN                      PC8

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PA1   // TB
#define TEMP_0_PIN                          PA0   // TH1
#define TEMP_1_PIN                          PA2   // TH2

#define FIL_RUNOUT_PIN                      PB10  // MT_DET

//
// LCD Pins
//
#if HAS_WIRED_LCD

  #define BEEPER_PIN                        PC1
  #define BTN_ENC                           PC3
  #define LCD_PINS_ENABLE                   PA4
  #define LCD_PINS_RS                       PA5
  #define BTN_EN1                           PB11
  #define BTN_EN2                           PB0

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)

    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN                   -1
    #define DOGLCD_A0                       PC4
    #define DOGLCD_CS                       PA7
    #define DOGLCD_SCK                      PB13
    #define DOGLCD_MOSI                     PB15

  #elif IS_TFTGLCD_PANEL

    #if ENABLED(TFTGLCD_PANEL_SPI)
      #define TFTGLCD_CS                    PB11
    #endif

  #else                                           // !MKS_MINI_12864

    #define LCD_PINS_D4                     PA6
    #if IS_ULTIPANEL
      #define LCD_PINS_D5                   PA7
      #define LCD_PINS_D6                   PC4
      #define LCD_PINS_D7                   PC5

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

  #endif // !MKS_MINI_12864

  #define BOARD_ST7920_DELAY_1     DELAY_NS(125)
  #define BOARD_ST7920_DELAY_2     DELAY_NS(125)
  #define BOARD_ST7920_DELAY_3     DELAY_NS(125)

#endif // HAS_WIRED_LCD

//
// SD Card
//
#define SD_DETECT_PIN                       PC10

//
// SPI
//
#define SPI_DEVICE                             2
#define SD_SCK_PIN                          PB13
#define SD_MISO_PIN                         PB14
#define SD_MOSI_PIN                         PB15
#define SD_SS_PIN                           PA15
