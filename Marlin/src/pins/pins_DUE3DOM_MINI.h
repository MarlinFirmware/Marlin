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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * DUE3DOM MINI pin assignments
 */

#ifndef __SAM3X8E__
  #error "Oops!  Make sure you have 'Arduino Due' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME "DUE3DOM MINI"

//
// Servos
//
#define SERVO0_PIN          5
#define SERVO1_PIN          6
#define SERVO2_PIN          8   // 4-pin header FAN0
#define SERVO3_PIN         -1

//
// Limit Switches
//
#define X_MIN_PIN          38
#define X_MAX_PIN          -1
#define Y_MIN_PIN          34
#define Y_MAX_PIN          -1
#define Z_MIN_PIN          30
#define Z_MAX_PIN          -1

//
// Steppers
//
#define X_STEP_PIN         17
#define X_DIR_PIN          16
#define X_ENABLE_PIN       22

#define Y_STEP_PIN          2
#define Y_DIR_PIN           3
#define Y_ENABLE_PIN       26

#define Z_STEP_PIN         64
#define Z_DIR_PIN          63
#define Z_ENABLE_PIN       15

#define E0_STEP_PIN        61
#define E0_DIR_PIN         60
#define E0_ENABLE_PIN      62

//
// Temperature Sensors
//
#define TEMP_0_PIN          0   // Analog Input (HOTEND0 thermistor)
#define TEMP_1_PIN          2   // Analog Input (unused)
#define TEMP_2_PIN          5   // Analog Input (OnBoard thermistor beta 3950)
#define TEMP_BED_PIN        1   // Analog Input (BED thermistor)

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS       53
#else
  #define MAX6675_SS       53
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       13   // HOTEND0 MOSFET
#define HEATER_BED_PIN      7   // BED MOSFET

#ifndef FAN_PIN
  #define FAN_PIN          11   // FAN1 header on board - PRINT FAN
#endif
#define FAN1_PIN           12   // FAN2 header on board - CONTROLLER FAN
#define FAN2_PIN            9   // FAN3 header on board - EXTRUDER0 FAN
//#define FAN3_PIN          8   // FAN0 4-pin header on board

//
// Misc. Functions
//
#define SDSS                4
#define PS_ON_PIN          40

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)

  #define LCD_PINS_RS       42
  #define LCD_PINS_ENABLE   43
  #define LCD_PINS_D4       44
  #define LCD_PINS_D5       45
  #define LCD_PINS_D6       46
  #define LCD_PINS_D7       47

  #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)

    #define BEEPER_PIN      41

    #define BTN_EN1         50
    #define BTN_EN2         52
    #define BTN_ENC         48

    #define SDSS             4
    #define SD_DETECT_PIN   14

  #elif ENABLED(RADDS_DISPLAY)

    #define BEEPER_PIN      41

    #define BTN_EN1         50
    #define BTN_EN2         52
    #define BTN_ENC         48

    #define BTN_BACK        71

    #undef SDSS
    #define SDSS             4
    #define SD_DETECT_PIN   14

  #elif ENABLED(SSD1306_OLED_I2C_CONTROLLER)

    #define BTN_EN1         50
    #define BTN_EN2         52
    #define BTN_ENC         48
    #define BEEPER_PIN      41
    #define LCD_SDSS         4
    #define SD_DETECT_PIN   14

  #elif ENABLED(SPARK_FULL_GRAPHICS)

    #define LCD_PINS_D4     29
    #define LCD_PINS_ENABLE 27
    #define LCD_PINS_RS     25

    #define BTN_EN1         35
    #define BTN_EN2         33
    #define BTN_ENC         37

    #define BEEPER_PIN      -1

   #elif ENABLED(MINIPANEL)
    #define BTN_EN1         52
    #define BTN_EN2         50
    #define BTN_ENC         48
    #define LCD_SDSS        4
    #define SD_DETECT_PIN   14
    #define BEEPER_PIN      41
    #define DOGLCD_A0       46
    #define DOGLCD_CS       45

  #endif // SPARK_FULL_GRAPHICS
#endif // ULTRA_LCD
