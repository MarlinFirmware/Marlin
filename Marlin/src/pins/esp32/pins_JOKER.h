/**
 * JIOKER
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
 * MRR ESPE pin assignments
 *
 * 3D printer control board based on the ESP32 microcontroller.
 * Supports 5 stepper drivers (using I2S stepper stream), heated bed,
 * single hotend, and LCD controller.
 */

#include "env_validate.h"

//#if EXTRUDERS > 2 || E_STEPPERS > 2
  //#error "MRR ESPE supports up to 2 E steppers."
//#elif HAS_MULTI_HOTEND
  //#error "MRR ESPE only supports 1 hotend / E stepper."
//#endif

#define BOARD_INFO_NAME      "MRR ESPE"
#define BOARD_WEBSITE_URL    "github.com/maplerainresearch/MRR_ESPE"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//
// Limit Switches
//
#define X_STOP_PIN                            15
#define Y_STOP_PIN                            16
#define Z_STOP_PIN                            17

//
// Enable I2S stepper stream
//
#define I2S_STEPPER_STREAM
#define I2S_WS                                26
#define I2S_BCK                               25
#define I2S_DATA                              27
#if ENABLED(LIN_ADVANCE)
  #error "I2S stream is currently incompatible with LIN_ADVANCE."
#endif

//
// Steppers
//
#define X_STEP_PIN                           128
#define X_DIR_PIN                            129
#define X_ENABLE_PIN                         136
//#define X_CS_PIN                            -1

#define Y_STEP_PIN                           130
#define Y_DIR_PIN                            131
#define Y_ENABLE_PIN                X_ENABLE_PIN
//#define Y_CS_PIN                            -1

#define Z_STEP_PIN                           132
#define Z_DIR_PIN                            133
#define Z_ENABLE_PIN                         137
//#define Z_CS_PIN                             -1  // SS_PIN

#define E0_STEP_PIN                          134
#define E0_DIR_PIN                           135
#define E0_ENABLE_PIN                        138
//#define E0_CS_PIN                           -1

//
// Temperature Sensors
//
#define TEMP_0_PIN                            36  // Analog Input
#define TEMP_BED_PIN                          39  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                         141  
#define FAN_PIN                              143  
#define HEATER_BED_PIN                       140  

//#define CONTROLLER_FAN_PIN                   -1
#define E0_AUTO_FAN_PIN                    142  // need to update Configuration_adv.h @section extruder

//
// MicroSD card
//
#define SD_MOSI_PIN                           23
#define SD_MISO_PIN                           19
#define SD_SCK_PIN                            18
#define SDSS                                   5
#define USES_SHARED_SPI                           // SPI is shared by SD card with TMC SPI drivers

//////////////////////////
// LCDs and Controllers //
//////////////////////////

#if HAS_WIRED_LCD

  //#define LCD_PINS_RS                         13
  //#define LCD_PINS_ENABLE                     17
  //#define LCD_PINS_D4                         16

  #if ENABLED(CR10_STOCKDISPLAY)

    #define BEEPER_PIN                       151

  #elif IS_RRD_FG_SC

    #define BEEPER_PIN                       151

    //#define LCD_PINS_D5                    -1
    //#define LCD_PINS_D6                    -1
    //#define LCD_PINS_D7                    -1

  #else

    //#error "Only CR10_STOCKDISPLAY and REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER are currently supported. Comment out this line to continue."

  #endif

  #define BTN_EN1                             2
  #define BTN_EN2                             4
  #define BTN_ENC                             12
  #define BEEPER_PIN                          139
#endif // HAS_MARLINUI_U8GLIB

// Hardware serial pins
// Add the following to Configuration.h or Configuration_adv.h to assign
// specific pins to hardware Serial1 and Serial2.
// Note: Serial2 can be defined using HARDWARE_SERIAL2_RX and HARDWARE_SERIAL2_TX but
// MRR ESPA does not have enough spare pins for such reassignment.
//#define HARDWARE_SERIAL1_RX                 21
//#define HARDWARE_SERIAL1_TX                 22
//#define HARDWARE_SERIAL2_RX                  2
//#define HARDWARE_SERIAL2_TX                  4

//
// Filament Runout Sensor -1关闭同时注释Configuration.h 1288 
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      13
#endif
//
// Servos 3Douch探针
//
#define SERVO0_PIN                            14
//探针独立，不用于Z限位
//Configuration.h里929行取消注释//#define Z_MIN_PROBE_PIN 35 // Pin 35 is the RAMPS default

//
//拓展板
//
//+2只有2个MAX限位，+5的限位和热敏只能6选3，开启需注释掉46行Limit Switches
#ifndef X_STOP_PIN
  #ifndef X_MIN_PIN
    #define X_MIN_PIN                          15
  #endif
  #ifndef X_MAX_PIN
    #define X_MAX_PIN                          -1//32
  #endif
#endif
#ifndef Y_STOP_PIN
  #ifndef Y_MIN_PIN
    #define Y_MIN_PIN                          16
  #endif
  #ifndef Y_MAX_PIN
    #define Y_MAX_PIN                          -1//33
  #endif
#endif
#ifndef Z_STOP_PIN
  #ifndef Z_MIN_PIN
    #define Z_MIN_PIN                          17
  #endif
  #ifndef Z_MAX_PIN
    #define Z_MAX_PIN                          -1//34
  #endif
#endif

//Steppers
//
//JOKER+2
//
//#define E1_STEP_PIN                          144  
//#define E1_DIR_PIN                           145  
//#define E1_ENABLE_PIN                        146  

//#define Z2_STEP_PIN                          147  
//#define Z2_DIR_PIN                           148  
//#define Z2_ENABLE_PIN                        149  

//#define TEMP_1_PIN                            34  // Analog Input
//#define HEATER_1_PIN                         150
//#define FAN1_PIN                             151

//
//JOKER+5
//
//#define E1_STEP_PIN                          144  
//#define E1_DIR_PIN                           145  
//#define E1_ENABLE_PIN                        146  

//#define Z2_STEP_PIN                          147  
//#define Z2_DIR_PIN                           148  
//#define Z2_ENABLE_PIN                        149  

//#define Z3_STEP_PIN                          150  
//#define Z3_DIR_PIN                           151  
//#define Z3_ENABLE_PIN                        152  

//#define E2_STEP_PIN                          153  
//#define E2_DIR_PIN                           154  
//#define E2_ENABLE_PIN                        155  

//#define E3_STEP_PIN                          156  
//#define E3_DIR_PIN                           157  
//#define E3_ENABLE_PIN                        158  

//和限位6选3
//#define TEMP_1_PIN                            32  // Analog Input
//#define TEMP_2_PIN                            33  // Analog Input
//#define TEMP_3_PIN                            34  // Analog Input

//#define HEATER_1_PIN                       159  
//#define HEATER_2_PIN                       160  
//#define HEATER_3_PIN                       161  

//#define FAN1_PIN                             162
//#define FAN1_PIN                             163
//#define FAN1_PIN                             164

//#define E1_AUTO_FAN_PIN                    165
//#define E1_AUTO_FAN_PIN                    166
//#define E1_AUTO_FAN_PIN                    167