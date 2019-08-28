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
 * RUMBA pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#elif HOTENDS > 3 || E_STEPPERS > 3
  #error "RUMBA supports up to 3 hotends / E-steppers. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Rumba"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#endif

//
// Servos
//
#define SERVO0_PIN          5

//
// Limit Switches
//
#define X_MIN_PIN          37
#define X_MAX_PIN          36
#define Y_MIN_PIN          35
#define Y_MAX_PIN          34
#define Z_MIN_PIN          33
#define Z_MAX_PIN          32

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  32
#endif

//
// Steppers
//
#define X_STEP_PIN         17
#define X_DIR_PIN          16
#define X_ENABLE_PIN       48

#define Y_STEP_PIN         54
#define Y_DIR_PIN          47
#define Y_ENABLE_PIN       55

#define Z_STEP_PIN         57
#define Z_DIR_PIN          56
#define Z_ENABLE_PIN       62

#ifndef E0_STEP_PIN
  #define E0_STEP_PIN      23
  #define E0_DIR_PIN       22
  #define E0_ENABLE_PIN    24
#endif

#ifndef E1_STEP_PIN
  #define E1_STEP_PIN      26
  #define E1_DIR_PIN       25
  #define E1_ENABLE_PIN    27
#endif

#if E1_STEP_PIN != 29
  #define E2_STEP_PIN      29
  #define E2_DIR_PIN       28
  #define E2_ENABLE_PIN    39
#endif

//
// Temperature Sensors
//
#ifndef TEMP_0_PIN
  #if TEMP_SENSOR_0 == -1
    #define TEMP_0_PIN      6   // Analog Input (connector *K1* on RUMBA thermocouple ADD ON is used)
  #else
    #define TEMP_0_PIN     15   // Analog Input (default connector for thermistor *T0* on rumba board is used)
  #endif
#endif

#ifndef TEMP_1_PIN
  #if TEMP_SENSOR_1 == -1
    #define TEMP_1_PIN      5   // Analog Input (connector *K2* on RUMBA thermocouple ADD ON is used)
  #else
    #define TEMP_1_PIN     14   // Analog Input (default connector for thermistor *T1* on rumba board is used)
  #endif
#endif

#if TEMP_SENSOR_2 == -1
  #define TEMP_2_PIN        7   // Analog Input (connector *K3* on RUMBA thermocouple ADD ON is used <-- this can't be used when TEMP_SENSOR_BED is defined as thermocouple)
#else
  #define TEMP_2_PIN       13   // Analog Input (default connector for thermistor *T2* on rumba board is used)
#endif

// Optional for extruder 4 or chamber:
//#define TEMP_X_PIN         12   // Analog Input (default connector for thermistor *T3* on rumba board is used)
//#define TEMP_CHAMBER_PIN   12   // Analog Input (default connector for thermistor *T3* on rumba board is used)

#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN      7   // Analog Input (connector *K3* on RUMBA thermocouple ADD ON is used <-- this can't be used when TEMP_SENSOR_2 is defined as thermocouple)
#else
  #define TEMP_BED_PIN     11   // Analog Input (default connector for thermistor *THB* on rumba board is used)
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN        2
#define HEATER_1_PIN        3
#define HEATER_2_PIN        6
#define HEATER_3_PIN        8
#define HEATER_BED_PIN      9

#ifndef FAN_PIN
  #define FAN_PIN           7
#endif
#define FAN1_PIN            8

//
// Misc. Functions
//
#define LED_PIN            13
#define PS_ON_PIN          45
#define KILL_PIN           46
#define CASE_LIGHT_PIN     45

//
// M3/M4/M5 - Spindle/Laser Control
//
#ifndef SPINDLE_LASER_PWM_PIN
  #define SPINDLE_LASER_PWM_PIN 4   // Hardware PWM. Pin 4 interrupts OC0* and OC1* always in use?
#endif
#ifndef SPINDLE_LASER_ENA_PIN
  #define SPINDLE_LASER_ENA_PIN    14   // Pullup!
#endif
#ifndef SPINDLE_DIR_PIN
  #define SPINDLE_DIR_PIN  15
#endif

//
// LCD / Controller
//
#if EITHER(MKS_12864OLED, MKS_12864OLED_SSD1306)
  #define LCD_PINS_DC      38   // Set as output on init
  #define LCD_PINS_RS      41   // Pull low for 1s to init
  // DOGM SPI LCD Support
  #define DOGLCD_CS        19
  #define DOGLCD_MOSI      42
  #define DOGLCD_SCK       18
  #define DOGLCD_A0        LCD_PINS_DC
#elif ENABLED(FYSETC_MINI_12864)
  #define DOGLCD_CS        42
  #define DOGLCD_A0        19
  #define DOGLCD_MOSI      51
  #define DOGLCD_SCK       52

  //#define FORCE_SOFT_SPI    // Use this if default of hardware SPI causes display problems
                              //   results in LCD soft SPI mode 3, SD soft SPI mode 0

  #define LCD_RESET_PIN  18   // Must be high or open for LCD to operate normally.

  #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
    #ifndef RGB_LED_R_PIN
      #define RGB_LED_R_PIN 41
    #endif
    #ifndef RGB_LED_G_PIN
      #define RGB_LED_G_PIN 38
    #endif
    #ifndef RGB_LED_B_PIN
      #define RGB_LED_B_PIN 40
    #endif
  #elif ENABLED(FYSETC_MINI_12864_2_1)
    #define NEOPIXEL_PIN   25
  #endif

#else
  #define LCD_PINS_RS      19
  #define LCD_PINS_ENABLE  42
  #define LCD_PINS_D4      18
  #define LCD_PINS_D5      38
  #define LCD_PINS_D6      41
#endif

#define LCD_PINS_D7        40

//
// Beeper, SD Card, Encoder
//
#define BEEPER_PIN         44

#if ENABLED(SDSUPPORT)
  #define SDSS             53
  #define SD_DETECT_PIN    49
#endif

#if ENABLED(NEWPANEL)
  #define BTN_EN1          11
  #define BTN_EN2          12
  #define BTN_ENC          43
#endif
