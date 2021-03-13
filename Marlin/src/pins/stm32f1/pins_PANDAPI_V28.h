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
 * PANDAPI STM32F1 pin assignments
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "PandaPi V28"
#endif

#define BOARD_WEBSITE_URL    "github.com/markniu/PandaPi"
#define DEFAULT_MACHINE_NAME "PANDAPI 3D"



#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE                0x1000  // 4KB

//for DGUS_TFT_UI
//#define I2C_SCL_PIN                         PB6
//#define I2C_SDA_PIN                         PB7

//
// Servos
//

#define SERVO0_PIN       255// 0xBE

//
#define X_STOP_PIN          PA3  //1 
//#define X_MAX_PIN          1 

#define Y_STOP_PIN          PA4  //4 
//#define Y_MAX_PIN          4 

#define Z_STOP_PIN          PA4  //11 
//#define Z_MIN_PIN          11 



#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN    255
#endif



//
// Z Probe (when not Z_MIN_PIN)
//
//#ifndef Z_MIN_PROBE_PIN
//  #define Z_MIN_PROBE_PIN  7
//#endif

//
// Steppers
//
#define X_STEP_PIN         PB8  //7 
#define X_DIR_PIN          PB6  //2 
#define X_ENABLE_PIN       0 

#define Y_STEP_PIN         PB5  //12 
#define Y_DIR_PIN          PB4  //5 
#define Y_ENABLE_PIN       0 

#define Z_STEP_PIN         PB3  //14 
#define Z_DIR_PIN          PA15 //13 
#define Z_ENABLE_PIN       0 


#define E0_STEP_PIN        PB15 //10 
#define E0_DIR_PIN         PB14 //6 
#define E0_ENABLE_PIN      0 

#define E1_STEP_PIN        PB2  //26
#define E1_DIR_PIN         PA7  //25
#define E1_ENABLE_PIN      0

#define E2_STEP_PIN        PA6  //31
#define E2_DIR_PIN         PA5  //21
#define E2_ENABLE_PIN      0
/*
#define Z2_STEP_PIN        31
#define Z2_DIR_PIN         21
#define Z2_ENABLE_PIN      0
*/


#if   ENABLED(MAX31856_PANDAPI)
#define MAX31856_CLK_PIN  29
#define MAX31856_MISO_PIN 24
#define MAX31856_MOSI_PIN 28
#define MAX31856_CS_PIN   27
#endif



/**
 * Default pins for TMC software SPI
 */

#if HAS_TRINAMIC
  #define X_HARDWARE_SERIAL  Serial1
 //  #define X2_HARDWARE_SERIAL customizedSerial1
  #define Y_HARDWARE_SERIAL  Serial1
 //  #define Y2_HARDWARE_SERIAL customizedSerial1
  #define Z_HARDWARE_SERIAL  Serial1
  // #define Z2_HARDWARE_SERIAL customizedSerial1
  #define E0_HARDWARE_SERIAL Serial1
  #define E1_HARDWARE_SERIAL Serial1
  #define E2_HARDWARE_SERIAL Serial1




#endif



//
// Augmentation for auto-assigning RAMPS plugs
//
#if DISABLED(IS_RAMPS_EEB) && DISABLED(IS_RAMPS_EEF) && DISABLED(IS_RAMPS_EFB) && DISABLED(IS_RAMPS_EFF) && DISABLED(IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
  //#if HOTENDS > 1
  #if HAS_MULTI_HOTEND
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
  #define MOSFET_D_PIN     -1
#endif

// heat connector index
#define HOTBED_CODE       0  
#define HOTEND_0_CODE     1  
#define HOTEND_1_CODE     2  

#if ENABLED(MAX31856_PANDAPI)
#define HEATER_0_PIN     30
#else
/* U6 PI*/

/* U6 STM32*/
#define HEATER_0_PIN     PB12 //255
#endif
#define HEATER_1_PIN     PA12 //255
#define HEATER_BED_PIN   PB13 //255
#define TEMP_0_PIN       PB0  //255
#define TEMP_1_PIN       PB1  //255
#define TEMP_BED_PIN     255
#define TEMP_BOARD_PIN   255

#define FAN_PIN          PA8  //255
#define FAN1_PIN         PA9  //255
#define FAN2_PIN         PA10 //255
#define FAN3_PIN         255

#define CASE_LIGHT_PIN 255


//////////////////////////
// LCDs and Controllers //
//////////////////////////
#if ENABLED(ULTRA_LCD)
  // LCD Display output pins
  #ifdef REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
	#define LCD_PINS_RS      28 
	#define LCD_PINS_ENABLE  29 
	#define LCD_PINS_D4      27  
  #endif

	#define SD_DETECT_PIN   255   
   // LCD Display input pins
  #if ENABLED(NEWPANEL)
	#define BEEPER_PIN		24 
	#define BTN_ENC 		30 
	#define BTN_EN1 		22 
	#define BTN_EN2 		23 
  #endif // NEWPANEL

#endif // ULTRA_LCD
