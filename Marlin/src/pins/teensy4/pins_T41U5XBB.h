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

/****************************************************************************************
* Teensy 4.0 and Teensy 4.1 (IMXRT1062) Breadboard pin assignments
* Requires the Teensyduino software with Teensy 4.0 or Teensy 4.1 selected in Arduino IDE!
* https://www.pjrc.com/teensy/teensyduino.html
****************************************************************************************/

#if !IS_32BIT_TEENSY
  #error "Oops! Select 'Teensy 4.0' or 'Teensy 4.1' in 'Tools > Board.'"
#endif

#if IS_TEENSY40
  #define BOARD_INFO_NAME "Teensy4.0"
#elif IS_TEENSY41
  #define BOARD_INFO_NAME "Teensy4.1"
#endif

#define AT90USB          1286  // Disable MarlinSerial etc.
#define USBCON         //1286  // Disable MarlinSerial etc.
/*

   plan for Teensy4.0 and Teensy4.1:
                                                     USB
                                          GND |-----#####-----| VIN (3.65 TO 5.5V)
                          CS1     RX1  PWM  0 |     #####     | GND
                          MISO1   TX1  PWM  1 |               | 3.3V
      X_STEP_PIN                       PWM  2 |               | 23  A9 PWM              
      X_DIR_PIN                        PWM  3 |               | 22  A8 PWM              Z_STOP
      Y_STEP_PIN                       PWM  4 |               | 21  A7      RX5         Y_STOTP
      Y_DIR_PIN                        PWM  5 |               | 20  A6      TX5         X_STOP
      Z_STEP_PIN                       PWM  6 |               | 19  A5 PWM        SCL0  TEMP_0_PIN
      Z_DIR_PIN                   RX2  PWM  7 |               | 18  A4 PWM        SDA0  TEMP_1_PIN
      E0_STEP_PIN                 TX2  PWM  8 |               | 17  A3      RX4   SDA1  
      E0_DIR_PIN                       PWM  9 |               | 16  A2      TX4   SCL1  
      X_ENABLE_PIN                     PWM 10 |               | 15  A1 PWM  RX3         TEMP_BED_PIN
      MOSI_PIN            MOSI0        PWM 11 |               | 14  A0 PWM  TX3         
      MISO_PIN            MISO0        PWM 12 |               | 13 LED PWM  SCK0        
                                         3.3V |               | GND
      SCL                              PWM 24 |               | 41 A17                  
      SDA                              PWM 25 |               | 40 A16                  Y_ENABLE_PIN
      E1_STEP_PIN         MOSI1            26 |               | 39 A15      MISO1       Z_ENABLE_PIN
      E1_DIR_PIN          SCK1             27 | *  *  *  *  * | 38 A14                  E0_ENABLE_PIN
                                  RX7  PWM 28 |               | 37     PWM              E1_ENABLE_PIN  
                                  TX7  PWM 29 |               | 36     PWM              
                                           30 |               | 35          TX8         
      HEATER_0_PIN                         31 |    SDCARD     | 34          RX8         
      HEATER_0_PIN                         32 |_______________| 33     PWM              HEATER_BED_PIN

 
*/

//
// Limit Switches
//
#define X_STOP_PIN                            20
#define Y_STOP_PIN                            21
#define Z_STOP_PIN                            22

//
// Steppers
//
#define X_STEP_PIN                             2
#define X_DIR_PIN                              3
#define X_ENABLE_PIN                          10
//#define X_CS_PIN                              30

#define Y_STEP_PIN                             4
#define Y_DIR_PIN                              5
#define Y_ENABLE_PIN                          40
//#define Y_CS_PIN                              31

#define Z_STEP_PIN                             6
#define Z_DIR_PIN                              7
#define Z_ENABLE_PIN                          39
//#define Z_CS_PIN                              32

#define E0_STEP_PIN                            8
#define E0_DIR_PIN                             9
#define E0_ENABLE_PIN                         38

#define E1_STEP_PIN                           26
#define E1_DIR_PIN                            27
#define E1_ENABLE_PIN                         37

#define HEATER_0_PIN                          31
#define HEATER_1_PIN                          32
#define HEATER_BED_PIN                        33

#define TEMP_0_PIN                             5  // Extruder / Analog pin numbering: 2 => A2
#define TEMP_1_PIN                             4
#define TEMP_BED_PIN                          15  // Bed / Analog pin numbering

#define LED_PIN                               13
