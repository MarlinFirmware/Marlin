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
 * Ported sys0724 & Vynt
 */

/**
 * Arduino Mega? or Due with RuRAMPS4DUE pin assignments
 *
 * Applies to the following boards:
 *  RURAMPS4DUE      (Hotend0, Hotend1, Hotend2, Fan0, Fan1, Bed)
 *
 *  Differences between
 *     RADDS | RuRAMPS4DUE
 *           |
 */

#ifndef __SAM3X8E__
  #error "Oops!  Make sure you have 'Arduino Due' selected from the 'Tools -> Boards' menu."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME       "RuRAMPS4Due"
#endif

//
// Limit Switches
//
#define X_MIN_PIN          45
#define X_MAX_PIN          39
#define Y_MIN_PIN          46
#define Y_MAX_PIN          41
#define Z_MIN_PIN          47
#define Z_MAX_PIN          43

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  43
#endif

//
// Steppers
//
#define X_STEP_PIN         37 // Support Extension Board
#define X_DIR_PIN          36
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         32 // Support Extension Board
#define Y_DIR_PIN          35
#define Y_ENABLE_PIN       34

#define Z_STEP_PIN         30 // Support Extension Board
#define Z_DIR_PIN           2
#define Z_ENABLE_PIN       33

#define E0_STEP_PIN        29
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      31

#define E1_STEP_PIN        22
#define E1_DIR_PIN         24
#define E1_ENABLE_PIN      26

#define E2_STEP_PIN        25
#define E2_DIR_PIN         23
#define E2_ENABLE_PIN      27

#define E3_STEP_PIN        15 // Only For Extension Board
#define E3_DIR_PIN         14
#define E3_ENABLE_PIN      61

//#define X_CS_PIN           -1
//#define Y_CS_PIN           -1
//#define Z_CS_PIN           -1
//#define E0_CS_PIN          -1
//#define E1_CS_PIN          -1
//#define E2_CS_PIN          -1
//#define E3_CS_PIN          -1

// For Future: Microstepping pins - Mapping not from fastio.h (?)
//#define E3_MS1_PIN         ?
//#define E3_MS2_PIN         ?
//#define E3_MS3_PIN         ?
//#define Z2_MS1_PIN         ? // shared with E3_MS1_PIN
//#define Z2_MS2_PIN         ? // shared with E3_MS2_PIN
//#define Z2_MS3_PIN         ? // shared with E3_MS3_PIN

#if !ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
  #define Z_PROBE_PIN      49
#endif // else Z_PROBE_PIN = Z_MIN_PIN

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #ifndef FIL_RUNOUT_PIN
    #define FIL_RUNOUT_PIN   Y_MIN_PIN
  #endif
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       13
#define HEATER_1_PIN       12
#define HEATER_2_PIN       11
#define HEATER_BED_PIN      7  // BED H1

#define FAN_PIN             9
#define FAN1_PIN            8
#define CONTROLLER_FAN_PIN -1

//
// Temperature Sensors
//
#define TEMP_0_PIN          0  // ANALOG A0
#define TEMP_1_PIN          1  // ANALOG A1
#define TEMP_2_PIN          2  // ANALOG A2
#define TEMP_3_PIN          3  // ANALOG A2
#define TEMP_BED_PIN        4  // ANALOG A3
//Thermocouple Use Analog Pins
#if ENABLED(VER_WITH_THERMOCOUPLE) // If Nead, define is in Configuration.h
  #define TEMP_4_PIN        5  // A5
  #define TEMP_5_PIN        6  // A6 (Marlin 2.0 not support)
#endif

// SPI for Max6675 or Max31855 Thermocouple
//#if DISABLED(SDSUPPORT)
//  #define MAX6675_SS        53
//#else
//  #define MAX6675_SS        49
//#endif

//
// Servos
//
#define SERVO0_PIN          5
#define SERVO1_PIN          3
#define SERVO2_PIN         -1
#define SERVO3_PIN         -1

//
// Misc. Functions
//
#define SDSS                4 // 4,10,52 if using HW SPI.
#define LED_PIN            -1 // 13 - HEATER_0_PIN
#define PS_ON_PIN          -1 // 65

// MKS TFT / Nextion Use internal USART-1
#define TFT_LCD_MODULE_COM        1
#define TFT_LCD_MODULE_BAUDRATE   115600

// ESP WiFi Use internal USART-2
#define ESP_WIFI_MODULE_COM       2
#define ESP_WIFI_MODULE_BAUDRATE  115600
#define ESP_WIFI_MODULE_RESET_PIN -1
#define PIGGY_GPIO_PIN            -1

//
// EEPROM
//
#define E2END 0x8000  // 32Kb (24lc256)
#define I2C_EEPROM    // EEPROM on I2C-0
//#define EEPROM_SD   // EEPROM on SDCARD
//#define SPI_EEPROM  // EEPROM on SPI-0
//#define SPI_CHAN_EEPROM1        ?
//#define SPI_EEPROM1_CS          ?
// 2K EEPROM
//#define SPI_EEPROM2_CS          ?
// 32Mb FLASH
//#define SPI_FLASH_CS            ?

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)
  // RADDS LCD panel
  #if ENABLED(RADDS_DISPLAY) || ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
    #define LCD_PINS_RS     63
    #define LCD_PINS_ENABLE 64
    #define LCD_PINS_D4     48
    #define LCD_PINS_D5     50
    #define LCD_PINS_D6     52
    #define LCD_PINS_D7     53

    #define BEEPER_PIN      62

    #define BTN_EN1         44
    #define BTN_EN2         42
    #define BTN_ENC         40

    #define SD_DETECT_PIN   51

  #elif ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    #define LCD_PINS_RS     52
    #define LCD_PINS_ENABLE 53
    #define LCD_PINS_D4     48
    #define LCD_PINS_D5     50
    #define LCD_PINS_D6     52
    #define LCD_PINS_D7     53

    #define BEEPER_PIN      62

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
     #define BTN_EN1        44
     #define BTN_EN2        42
     #define BTN_ENC        40
     #define SD_DETECT_PIN  51
    #endif

  #elif ENABLED(SSD1306_OLED_I2C_CONTROLLER)
    #define BTN_EN1         44
    #define BTN_EN2         42
    #define BTN_ENC         40
    #define BEEPER_PIN      62
    #define LCD_SDSS        10
    #define SD_DETECT_PIN   51

  #elif ENABLED(SPARK_FULL_GRAPHICS)
    //http://doku.radds.org/dokumentation/other-electronics/sparklcd/
    #error "Oops! SPARK_FULL_GRAPHICS not supported with this board"
    //#define LCD_PINS_D4     29//?
    //#define LCD_PINS_ENABLE 27//?
    //#define LCD_PINS_RS     25//?
    //#define BTN_EN1         35//?
    //#define BTN_EN2         33//?
    //#define BTN_ENC         37//?
  #endif // SPARK_FULL_GRAPHICS

#endif // ULTRA_LCD
