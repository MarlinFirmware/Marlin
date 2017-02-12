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
 * Arduino Mega with PICA pin assignments
 *
 * PICA is Power, Interface, and Control Adapter and is open source hardware.
 * See github.com/mjrice/PICA for schematics etc.
 *
 * Applies to the following boards:
 *
 *  PICA 
 *  PICA_REVB
 *
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "PICA"
#endif


/* note that these are the "pins" that correspond to the analog inputs on the arduino mega.  these are not the same as the physical pin numbers
AD0 = 54;   AD1 = 55;   AD2 = 56;   AD3 = 57;   
AD4 = 58;   AD5 = 59;   AD6 = 60;   AD7 = 61;   
AD8 = 62;   AD9 = 63;   AD10 = 64;  AD11 = 65;
AD12 = 66;  AD13 = 67;  AD14 = 68;  AD15 = 69;
*/
/*************************************
 * PICA Arduino shield configuration mjrice
 ************************************* */


#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

 #define KNOWN_BOARD 1
 #define LARGE_FLASH true

 #define X_DIR_PIN         54
 #define X_STEP_PIN        55
 #define Y_DIR_PIN         56
 #define Y_STEP_PIN        57
 #define Z_DIR_PIN         58
 #define Z_STEP_PIN        59
 #define X_ENABLE_PIN      60     
 #define Y_ENABLE_PIN      61
 #define Z_ENABLE_PIN      62

 #define X_MIN_PIN         14
 #define X_MAX_PIN         15

 #define Y_MIN_PIN          16
 #define Y_MAX_PIN          17

 #define Z_MIN_PIN          23
 #define Z_MAX_PIN          22

 #define E0_STEP_PIN         67 
 #define E0_DIR_PIN          24
 #define E0_ENABLE_PIN       26

 #define SDPOWER            -1
 #define LED_PIN            -1 
 #define PS_ON_PIN          -1
 #define KILL_PIN           -1

      
 #define SSR_PIN            6
 #define HEATER_BED_PIN     8 // heat bed
 
 #define HEATER_0_PIN       10 // extruder 
 #define HEATER_1_PIN       2 // second extruder
 #define FAN_PIN            9
 #define FAN_2_PIN          7

 #define TEMP_0_PIN         9   // ANALOG NUMBERING
 #define TEMP_1_PIN         10
 #define TEMP_BED_PIN       10
 #define TEMP_2_PIN         11
 #define TEMP_3_PIN         12

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS       66 // Do not use pin 53 if there is even the remote possibility of using Display/SD card
#else
  #define MAX6675_SS       66 // Do not use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#endif

 // LCD interface pins
 #define LCD_PINS_RS 33
 #define LCD_PINS_ENABLE 30
 #define LCD_PINS_D4 35
 #define LCD_PINS_D5 32
 #define LCD_PINS_D6 37
 #define LCD_PINS_D7 36

 #define BTN_EN1 47
 #define BTN_EN2 48
 #define BTN_ENC 31  //the click 

 #define BLEN_C 2
 #define BLEN_B 1
 #define BLEN_A 0

 #define SD_DETECT_PIN 49 
 #define BEEPER_PIN   29
 #define SDSS         53

#define LCD_SDSS 53

 //encoder rotation values
 #define encrot0 0
 #define encrot1 2
 #define encrot2 3
 #define encrot3 1
 


