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

#ifndef TARGET_STM32F1
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "BIGTREE SKR mini V1.1"
#endif

//#define DISABLE_DEBUG
#define DISABLE_JTAG

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Limit Switches
//
#define X_MIN_PIN          PC2
#define X_MAX_PIN          PA2
#define Y_MIN_PIN          PC1
#define Y_MAX_PIN          PA1
#define Z_MIN_PIN          PC0
#define Z_MAX_PIN          PC3

//
// Steppers
//

#define X_STEP_PIN         PC6
#define X_DIR_PIN          PC7
#define X_ENABLE_PIN       PB15

#define Y_STEP_PIN         PB13
#define Y_DIR_PIN          PB14
#define Y_ENABLE_PIN       PB12

#define Z_STEP_PIN         PB10
#define Z_DIR_PIN          PB11
#define Z_ENABLE_PIN       PB2

#define E0_STEP_PIN        PC5
#define E0_DIR_PIN         PB0
#define E0_ENABLE_PIN      PC4

#if ENABLED(TMC_USE_SW_SPI)
  #define TMC_SW_SCK       PB3
  #define TMC_SW_MISO      PB4
  #define TMC_SW_MOSI      PB5
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       PA8
#define FAN_PIN            PC8
#define HEATER_BED_PIN     PC9

//
// Temperature Sensors
//
#define TEMP_BED_PIN       PB1   // Analog Input
#define TEMP_0_PIN         PA0   // Analog Input

//
// LCD Pins
//

/**
 *                _____                                             _____
 *            NC | · · | GND                                    5V | · · | GND
 *         RESET | · · | PB9 (SD_DETECT)             (LCD_D7) PC14 | · · | PC15 (LCD_D6)
 *  (MOSI)   PB5 | · · | PB8 (BTN_EN2)               (LCD_D5)  PB7 | · · | PC13 (LCD_D4)
 * (SD_SS)  PA15 | · · | PD2 (BTN_EN1)               (LCD_RS) PC12 | · · | PB6  (LCD_EN)
 *   (SCK)   PB3 | · · | PB4 (MISO)                 (BTN_ENC) PC11 | · · | PC10 (BEEPER)
 *                -----                                             -----
 *                EXP2                                              EXP1
 */

#if HAS_SPI_LCD
  #define BEEPER_PIN       PC10
  #define BTN_ENC          PC11
  #define LCD_PINS_RS      PC12

  #define BTN_EN1          PD2
  #define BTN_EN2          PB8

  #define LCD_PINS_ENABLE  PB6

  #if ENABLED(FYSETC_MINI_12864)

    #define LCD_BACKLIGHT_PIN -1
    #define LCD_RESET_PIN  PC13
    #define DOGLCD_A0      PC12
    #define DOGLCD_CS      PB6
    #define DOGLCD_SCK     PB3
    #define DOGLCD_MOSI    PB5

    #define FORCE_SOFT_SPI   // SPI MODE3

    #define LED_PIN        PB7   // red pwm
    //#define LED_PIN        PC15   // green
    //#define LED_PIN        PC14   // blue

    //#if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
    //  #ifndef RGB_LED_R_PIN
    //    #define RGB_LED_R_PIN PB7
    //  #endif
    //  #ifndef RGB_LED_G_PIN
    //    #define RGB_LED_G_PIN PC15
    //  #endif
    //  #ifndef RGB_LED_B_PIN
    //    #define RGB_LED_B_PIN PC14
    //  #endif
    //#elif ENABLED(FYSETC_MINI_12864_2_1)
    //  #define NEOPIXEL_PIN    PB7
    //#endif

  #else // !FYSETC_MINI_12864

    #define LCD_PINS_D4    PC13
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5  PB7
      #define LCD_PINS_D6  PC15
      #define LCD_PINS_D7  PC14
    #endif

  #endif // !FYSETC_MINI_12864

#endif // HAS_SPI_LCD

//
// SD Card
//

// By default the onboard SD is enabled.
// To disable it and use an external SD (connected to LCD)
// enable STM32_SD_LCD.

//#define STM32_SD_LCD

#if ENABLED(STM32_SD_LCD)
  #define ENABLE_SPI3
  #define SD_DETECT_PIN    PB9
  #define SCK_PIN          PB3
  #define MISO_PIN         PB4
  #define MOSI_PIN         PB5
  #define SS_PIN           PA15
#else
  #define SDCARD_CONNECTION ONBOARD
  #define ENABLE_SPI1
  #define SD_DETECT_PIN    PA3
  #define SCK_PIN          PA5
  #define MISO_PIN         PA6
  #define MOSI_PIN         PA7
  #define SS_PIN           PA4
#endif

#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 DELAY_NS(125)
#endif
