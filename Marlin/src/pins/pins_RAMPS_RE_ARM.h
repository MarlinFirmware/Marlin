/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Re-ARM with RAMPS v1.4 pin assignments
 *
 * Applies to the following boards:
 *
 *  RAMPS_14_EFB (Hotend, Fan, Bed)
 *  RAMPS_14_EEB (Hotend0, Hotend1, Bed)
 *  RAMPS_14_EFF (Hotend, Fan0, Fan1)
 *  RAMPS_14_EEF (Hotend0, Hotend1, Fan)
 *  RAMPS_14_SF  (Spindle, Controller Fan)
 *
 */

//#if !defined(TARGET_LPC1768)
#if DISABLED(IS_REARM)
  #error "Oops!  Make sure you have Re-Arm selected."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "Re-ARM RAMPS 1.4"
#endif

#define LARGE_FLASH true

// unused
#define D57               57
#define D58               58

//
// Servos
//
#define SERVO0_PIN         11
#define SERVO1_PIN          6  // also on J5-1
#define SERVO2_PIN          5
#define SERVO3_PIN          4  // 5V output - PWM capable

//
// Limit Switches
//
#define X_MIN_PIN           3  //10k pullup to 3.3V, 1K series
#define X_MAX_PIN           2  //10k pullup to 3.3V, 1K series
#define Y_MIN_PIN          14  //10k pullup to 3.3V, 1K series
#define Y_MAX_PIN          15  //10k pullup to 3.3V, 1K series
#define Z_MIN_PIN          18  //10k pullup to 3.3V, 1K series
#define Z_MAX_PIN          19  //10k pullup to 3.3V, 1K series
//#define Z_PROBE_PIN         1  // AUX-1


//
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define E2_STEP_PIN        36
#define E2_DIR_PIN         34
#define E2_ENABLE_PIN      30

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_0_PIN         0  //A0 (T0) - D67 - TEMP_0_PIN
#define TEMP_BED_PIN       1  //A1 (T1) - D68 - TEMP_BED_PIN
#define TEMP_1_PIN         2  //A2 (T2) - D69 - TEMP_1_PIN
#define TEMP_2_PIN         3  //A3 - D63 - J5-3 & AUX-2
#define TEMP_3_PIN         4  //A4 - D37 - BUZZER_PIN
//#define TEMP_4_PIN         5  //A5 - D49 - SD_DETECT_PIN
//#define ??               6  //A6 - D0  - RXD0 - J4-4 & AUX-1
#define FILWIDTH_PIN       7  //A7 - D1  - TXD0 - J4-5 & AUX-1


//
// Augmentation for auto-assigning RAMPS plugs
//
#if DISABLED(IS_RAMPS_EEB) && DISABLED(IS_RAMPS_EEF) && DISABLED(IS_RAMPS_EFB) && DISABLED(IS_RAMPS_EFF) && DISABLED(IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
  #if HOTENDS > 1
    #if TEMP_SENSOR_BED
      #define IS_RAMPS_EEB
    #else
      #define IS_RAMPS_EEF
    #endif
  #elif TEMP_SENSOR_BED
    #define IS_RAMPS_EFB
  #else
    #define IS_RAMPS_EFF
  #endif
#endif

//
// Heaters / Fans
//
#ifndef MOSFET_D_PIN
  #define MOSFET_D_PIN  -1
#endif
#ifndef RAMPS_D8_PIN
  #define RAMPS_D8_PIN   8
#endif
#ifndef RAMPS_D9_PIN
  #define RAMPS_D9_PIN   9
#endif
#ifndef RAMPS_D10_PIN
  #define RAMPS_D10_PIN 10
#endif

#define HEATER_0_PIN     RAMPS_D10_PIN

#if ENABLED(IS_RAMPS_EFB)                      // Hotend, Fan, Bed
  #define FAN_PIN        RAMPS_D9_PIN
  #define HEATER_BED_PIN RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_EEF)                    // Hotend, Hotend, Fan
  #define HEATER_1_PIN   RAMPS_D9_PIN
  #define FAN_PIN        RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_EEB)                    // Hotend, Hotend, Bed
  #define HEATER_1_PIN   RAMPS_D9_PIN
  #define HEATER_BED_PIN RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_EFF)                    // Hotend, Fan, Fan
  #define FAN_PIN        RAMPS_D9_PIN
  #define FAN1_PIN       RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_SF)                     // Spindle, Fan
  #define FAN_PIN        RAMPS_D8_PIN
#else                                          // Non-specific are "EFB" (i.e., "EFBF" or "EFBE")
  #define FAN_PIN        RAMPS_D9_PIN
  #define HEATER_BED_PIN RAMPS_D8_PIN
  #if HOTENDS == 1
    #define FAN1_PIN     MOSFET_D_PIN
  #else
    #define HEATER_1_PIN MOSFET_D_PIN
  #endif
#endif

#ifndef FAN_PIN
  #define FAN_PIN 4      // IO pin. Buffer needed
#endif

//
// Misc. Functions
//
#define LED_PIN            13

// define digital pin 4 for the filament runout sensor. Use the RAMPS 1.4 digital input 4 on the servos connector
#define FIL_RUNOUT_PIN      4

#define PS_ON_PIN          12

#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT) && !defined(SPINDLE_LASER_ENABLE_PIN)
  #if !defined(NUM_SERVOS) || NUM_SERVOS < 4 // try to use servo connector
    #define CASE_LIGHT_PIN    4      // MUST BE HARDWARE PWM
  #endif
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if ENABLED(SPINDLE_LASER_ENABLE) && !PIN_EXISTS(SPINDLE_LASER_ENABLE)
  #if !defined(NUM_SERVOS) || NUM_SERVOS == 1 // must use servo connector
    #undef  SERVO1
    #undef  SERVO2
    #undef  SERVO3
    #define SPINDLE_LASER_ENABLE_PIN  6  // Pin should have a pullup/pulldown!
    #define SPINDLE_LASER_PWM_PIN     4  // MUST BE HARDWARE PWM
    #define SPINDLE_DIR_PIN           5
  #endif
#endif
//
// Průša i3 MK2 Multiplexer Support
//
#define E_MUX0_PIN          0   // Z_CS_PIN
#define E_MUX1_PIN          1   // E0_CS_PIN
#define E_MUX2_PIN         63   // E1_CS_PIN

/**
 * LCD / Controller
 *
 * All controllers can use J3 and J5 on the Re-ARM board.  Custom cabling will be required.
 */

/**
 * Smart LCD adapter
 *
 * The Smart LCD adapter can be used for the two 10 pin LCD controllers such as
 * REPRAP_DISCOUNT_SMART_CONTROLLER.  It can't be used for controllers that use
 * DOGLCD_A0, DOGLCD_CS, LCD_PINS_D5, LCD_PINS_D6 or LCD_PINS_D7. A custom cable
 * is needed to pick up 5V for the EXP1 connection.
 *
 * SD card on the LCD uses the same SPI signals as the LCD. This results in garbage/lines
 * on the LCD display during accesses of the SD card. The menus/code has been arranged so
 * that the garbage/lines are erased immediately after the SD card accesses are completed.
 */

#if ENABLED(ULTRA_LCD)

  #define BEEPER_PIN          37  // not 5V tolerant

  #define BTN_EN1             31  // J3-2 & AUX-4
  #define BTN_EN2             33  // J3-4 & AUX-4
  #define BTN_ENC             35  // J3-3 & AUX-4

  #define SD_DETECT_PIN       49  // not 5V tolerant   J3-1 & AUX-3
  #define KILL_PIN            41  // J5-4 & AUX-4
  #define LCD_PINS_RS         16  // J3-7 & AUX-4
  #define LCD_SDSS            16  // J3-7 & AUX-4
  #define LCD_BACKLIGHT_PIN   16  // J3-7 & AUX-4 - only used on DOGLCD controllers
  #define LCD_PINS_ENABLE     51  // (MOSI) J3-10 & AUX-3
  #define LCD_PINS_D4         52  // (SCK)  J3-9 & AUX-3

  #define DOGLCD_A0           59  // J3-8 & AUX-2
  #define DOGLCD_CS           63  // J5-3 & AUX-2

  #ifdef ULTIPANEL
    #define LCD_PINS_D5       71  // ENET_MDIO
    #define LCD_PINS_D6       73  // ENET_RX_ER
    #define LCD_PINS_D7       75  // ENET_RXD1
  #endif

  #if ENABLED(NEWPANEL)
    #if ENABLED(REPRAPWORLD_KEYPAD)
      #define SHIFT_OUT         51  // (MOSI) J3-10 & AUX-3
      #define SHIFT_CLK         52  // (SCK)  J3-9 & AUX-3
      #define SHIFT_LD          49  // not 5V tolerant   J3-1 & AUX-3
    #endif
  #else
    //#define SHIFT_CLK           31  // J3-2 & AUX-4
    //#define SHIFT_LD            33  // J3-4 & AUX-4
    //#define SHIFT_OUT           35  // J3-3 & AUX-4
    //#define SHIFT_EN            41  // J5-4 & AUX-4
  #endif

  #define SDCARD_SORT_ALPHA         // Using SORT feature to keep one directory level in RAM
                                    // When going up/down directory levels the SD card is
                                    // accessed but the garbage/lines are removed when the
                                    // LCD updates

  #define SDSORT_LIMIT       256    // Maximum number of sorted items (10-256). Costs 27 bytes each.
  #define FOLDER_SORTING     -1     // -1=above  0=none  1=below
  #define SDSORT_GCODE       false  // Allow turning sorting on/off with LCD and M34 g-code.
  #define SDSORT_USES_RAM    true   // Pre-allocate a static array for faster pre-sorting.
  #define SDSORT_USES_STACK  false  // Prefer the stack for pre-sorting to give back some SRAM. (Negated by next 2 options.)
  #define SDSORT_CACHE_NAMES true   // Keep sorted items in RAM longer for speedy performance. Most expensive option.
  #define SDSORT_DYNAMIC_RAM false  // Use dynamic allocation (within SD menus). Least expensive option. Set SDSORT_LIMIT before use!

  #if ENABLED(VIKI2) || ENABLED(miniVIKI)
    // #define LCD_SCREEN_ROT_180

    #undef  BEEPER_PIN
    #define BEEPER_PIN          37  // may change if cable changes

    #define BTN_EN1             31  // J3-2 & AUX-4
    #define BTN_EN2             33  // J3-4 & AUX-4
    #define BTN_ENC             35  // J3-3 & AUX-4

    #define SD_DETECT_PIN       49  // not 5V tolerant   J3-1 & AUX-3
    #define KILL_PIN            41  // J5-4 & AUX-4

    #undef  DOGLCD_CS
    #define DOGLCD_CS           16
    #undef  LCD_BACKLIGHT_PIN   //16  // J3-7 & AUX-4 - only used on DOGLCD controllers
    #undef  LCD_PINS_ENABLE     //51  // (MOSI) J3-10 & AUX-3
    #undef  LCD_PINS_D4         //52  // (SCK)  J3-9 & AUX-3

    #undef  LCD_PINS_D5         //59  // J3-8 & AUX-2
    #define DOGLCD_A0           59  // J3-8 & AUX-2
    #undef  LCD_PINS_D6         //63  // J5-3 & AUX-2
    #undef  LCD_PINS_D7         // 6  // (SERVO1) J5-1 & SERVO connector
    #define DOGLCD_SCK SCK_PIN
    #define DOGLCD_MOSI MOSI_PIN

    #define STAT_LED_BLUE_PIN   63  // may change if cable changes
    #define STAT_LED_RED_PIN     6  // may change if cable changes
  #endif

  //#define MISO_PIN            50  // system defined J3-10 & AUX-3
  //#define MOSI_PIN            51  // system defined J3-10 & AUX-3
  //#define SCK_PIN             52  // system defined J3-9 & AUX-3
  //#define SS_PIN              53  // system defined J3-5 & AUX-3 - sometimes called SDSS

  #if ENABLED(MINIPANEL)
    // GLCD features
    //#define LCD_CONTRAST   190
    // Uncomment screen orientation
    //#define LCD_SCREEN_ROT_90
    //#define LCD_SCREEN_ROT_180
    //#define LCD_SCREEN_ROT_270
  #endif

#endif // ULTRA_LCD

//
// Ethernet pins
//
#ifndef ULTIPANEL
  #define ENET_MDIO   71  // J12-4
  #define ENET_RX_ER  73  // J12-6
  #define ENET_RXD1   75  // J12-8
#endif
#define ENET_MOC      70  // J12-3
#define REF_CLK       72  // J12-5
#define ENET_RXD0     74  // J12-7
#define ENET_CRS      76  // J12-9
#define ENET_TX_EN    77  // J12-10
#define ENET_TXD0     78  // J12-11
#define ENET_TXD1     79  // J12-12

/**
 *  PWMS
 *
 *  There are 6 PWMS.  Each PWM can be assigned to one of two pins.
 *
 *  SERVO2 does NOT have a PWM assigned to it.
 *
 *  PWM1.1   DIO4    SERVO3_PIN       FIL_RUNOUT_PIN   5V output, PWM
 *  PWM1.1   DIO26   E0_STEP_PIN
 *  PWM1.2   DIO11   SERVO0_PIN
 *  PWM1.2   DIO54   X_STEP_PIN
 *  PWM1.3   DIO6    SERVO1_PIN       J5-1
 *  PWM1.3   DIO60   Y_STEP_PIN
 *  PWM1.4   DIO53   SDSS(SSEL0)      J3-5  AUX-3
 *  PWM1.4   DIO46   Z_STEP_PIN
 *  PWM1.5   DIO3    X_MIN_PIN        10K PULLUP TO 3.3v, 1K SERIES
 *  PWM1.5   DIO9    RAMPS_D9_PIN
 *  PWM1.6   DIO14   Y_MIN_PIN        10K PULLUP TO 3.3v, 1K SERIES
 *  PWM1.6   DIO10   RAMPS_D10_PIN
 */

/**
 *  The following pins are NOT available in a Re-ARM system
 *  7
 *  17
 *  22
 *  23
 *  25
 *  27
 *  29
 *  32
 *  39
 *  40
 *  42
 *  43
 *  44
 *  45
 *  47
 *  64
 *  65
 *  66
 */

 /**
  * special pins
  *   D37 - not 5V tolerant
  *   D49 - not 5V tolerant
  *   D57 - open collector
  *   D58 - open collector
  *
 */
