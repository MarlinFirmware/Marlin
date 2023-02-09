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
 * RUMBA pin assignments
 * Schematic: https://green-candy.osdn.jp/external/MarlinFW/board_schematics/RAMPS/RUMBA/RRD-RUMBA_SCHEMATICS.png
 * Origin: https://reprap.org/wiki/File:RRD-RUMBA_SCHEMATICS.png
 * ATmega2560
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "RUMBA supports up to 3 hotends / E steppers."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Rumba"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#endif

#ifndef AVR_CHIPOSCILLATOR_FREQ
  #define AVR_CHIPOSCILLATOR_FREQ 16000000
#endif

//
// Servos
//
#define SERVO0_PIN                            PinE3

//
// Limit Switches
//
#ifndef X_MIN_PIN
  #define X_MIN_PIN                           PinC0
#endif
#ifndef X_MAX_PIN
  #define X_MAX_PIN                           PinC1
#endif
#ifndef Y_MIN_PIN
  #define Y_MIN_PIN                           PinC2
#endif
#ifndef Y_MAX_PIN
  #define Y_MAX_PIN                           PinC3
#endif
#ifndef Z_MIN_PIN
  #define Z_MIN_PIN                           PinC4
#endif
#ifndef Z_MAX_PIN
  #define Z_MAX_PIN                           PinC5
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC5
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinH0
#define X_DIR_PIN                             PinH1
#define X_ENABLE_PIN                          PinL1

#define Y_STEP_PIN                            PinF0
#define Y_DIR_PIN                             PinL2
#define Y_ENABLE_PIN                          PinF1

#define Z_STEP_PIN                            PinF3
#define Z_DIR_PIN                             PinF2
#define Z_ENABLE_PIN                          PinK0

#ifndef E0_STEP_PIN
  #define E0_STEP_PIN                         PinA1
  #define E0_DIR_PIN                          PinA0
  #define E0_ENABLE_PIN                       PinA2
#endif

#ifndef E1_STEP_PIN
  #define E1_STEP_PIN                         PinA4
  #define E1_DIR_PIN                          PinA3
  #define E1_ENABLE_PIN                       PinA5
#endif

#if E1_STEP_PIN != PinA7
  #define E2_STEP_PIN                         PinA7
  #define E2_DIR_PIN                          PinA6
  #define E2_ENABLE_PIN                       PinG2
#endif

//
// Temperature Sensors
//
#ifndef TEMP_0_PIN
  #if TEMP_SENSOR_0 == -1
    #define TEMP_0_PIN                        PinH3  // Analog Input (connector *K1* on RUMBA thermocouple ADD ON is used)
  #else
    #define TEMP_0_PIN                        PinJ0  // Analog Input (default connector for thermistor *T0* on rumba board is used)
  #endif
#endif

#ifndef TEMP_1_PIN
  #if TEMP_SENSOR_1 == -1
    #define TEMP_1_PIN                        PinE3  // Analog Input (connector *K2* on RUMBA thermocouple ADD ON is used)
  #else
    #define TEMP_1_PIN                        PinJ1  // Analog Input (default connector for thermistor *T1* on rumba board is used)
  #endif
#endif

#if TEMP_SENSOR_2 == -1
  #define TEMP_2_PIN                          PinH4  // Analog Input (connector *K3* on RUMBA thermocouple ADD ON is used <-- this can't be used when TEMP_SENSOR_BED is defined as thermocouple)
#else
  #define TEMP_2_PIN                          PinB7  // Analog Input (default connector for thermistor *T2* on rumba board is used)
#endif

// Optional for extruder 4 or chamber:
//#define TEMP_X_PIN                          PinB6  // Analog Input (default connector for thermistor *T3* on rumba board is used)

#ifndef TEMP_CHAMBER_PIN
  //#define TEMP_CHAMBER_PIN                  PinB6  // Analog Input (default connector for thermistor *T3* on rumba board is used)
#endif

#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN                        PinH4  // Analog Input (connector *K3* on RUMBA thermocouple ADD ON is used <-- this can't be used when TEMP_SENSOR_2 is defined as thermocouple)
#else
  #define TEMP_BED_PIN                        PinB5  // Analog Input (default connector for thermistor *THB* on rumba board is used)
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE4
#define HEATER_1_PIN                          PinE5
#define HEATER_2_PIN                          PinH3
#define HEATER_3_PIN                          PinH5
#define HEATER_BED_PIN                        PinH6

#ifndef FAN_PIN
  #define FAN_PIN                             PinH4
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                            PinH5
#endif

//
// Misc. Functions
//
#define LED_PIN                               PinB7
#define PS_ON_PIN                             PinL4
#define KILL_PIN                              PinL3

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinL4
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#ifndef SPINDLE_LASER_PWM_PIN
  #define SPINDLE_LASER_PWM_PIN               PinG5  // Hardware PWM. Pin 4 interrupts OC0* and OC1* always in use?
#endif
#ifndef SPINDLE_LASER_ENA_PIN
  #define SPINDLE_LASER_ENA_PIN               PinJ1  // Pullup!
#endif
#ifndef SPINDLE_DIR_PIN
  #define SPINDLE_DIR_PIN                     PinJ0
#endif

//
// LCD / Controller
//
#if EITHER(MKS_12864OLED, MKS_12864OLED_SSD1306)
  #define LCD_PINS_DC                         PinD7  // Set as output on init
  #define LCD_PINS_RS                         PinG0  // Pull low for 1s to init
  // DOGM SPI LCD Support
  #define DOGLCD_CS                           PinD2
  #define DOGLCD_MOSI                         PinL7
  #define DOGLCD_SCK                          PinD3
  #define DOGLCD_A0                  LCD_PINS_DC
#elif ENABLED(FYSETC_MINI_12864)
  #define DOGLCD_CS                           PinL7
  #define DOGLCD_A0                           PinD2
  #define DOGLCD_MOSI                         PinB2
  #define DOGLCD_SCK                          PinB1

  //#define FORCE_SOFT_SPI                        // Use this if default of hardware SPI causes display problems
                                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

  #define LCD_RESET_PIN                       PinD3  // Must be high or open for LCD to operate normally.

  #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
    #ifndef RGB_LED_R_PIN
      #define RGB_LED_R_PIN                   PinG0
    #endif
    #ifndef RGB_LED_G_PIN
      #define RGB_LED_G_PIN                   PinD7
    #endif
    #ifndef RGB_LED_B_PIN
      #define RGB_LED_B_PIN                   PinG1
    #endif
  #elif ENABLED(FYSETC_MINI_12864_2_1)
    #define NEOPIXEL_PIN                      PinD7
  #endif

#else
  #define LCD_PINS_RS                         PinD2
  #define LCD_PINS_ENABLE                     PinL7
  #define LCD_PINS_D4                         PinD3
  #define LCD_PINS_D5                         PinD7
  #define LCD_PINS_D6                         PinG0
#endif

#define LCD_PINS_D7                           PinG1

//
// Beeper, SD Card, Encoder
//
#define BEEPER_PIN                            PinL5

#if ENABLED(SDSUPPORT)
  #define SDSS                                PinB0
  #define SD_DETECT_PIN                       PinL0
#endif

#if IS_NEWPANEL
  #define BTN_EN1                             PinB5
  #define BTN_EN2                             PinB6
  #define BTN_ENC                             PinL6
#endif
