/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if NOT_TARGET(STM32F4)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "STM32F401CCU6 boards support one hotend / E-steppers only."
#endif

#include "env_validate.h"

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "I3DBEE BP_01"
#endif
#define TEMP_TIMER 5
//#define DISABLE_DEBUG                      false  // DISABLE_(DEBUG|JTAG) is not supported for STM32F4.
//#define ALLOW_STM32F4
//#define BOARD_NO_NATIVE_USB

//
// EEPROM
//
  #define FLASH_EEPROM_EMULATION
  #define FLASH_EEPROM_LEVELING
  #define EEPROM_SIZE           0x4000                                           // 16kB
  #define FLASH_SECTOR          (FLASH_SECTOR_TOTAL - 1)
  #define FLASH_UNIT_SIZE       0x4000                                           // 16kB
/*
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
  #define SDCARD_EEPROM_EMULATION
#endif
*/

//
// Limit Switches
//
#ifndef X_STOP_PIN      
  #ifndef X_MIN_PIN     
    #define X_MIN_PIN      PB9
  #endif                
  #ifndef X_MAX_PIN     
    #define X_MAX_PIN      PB9
  #endif                
#endif                  
#ifndef Y_STOP_PIN      
  #ifndef Y_MIN_PIN     
    #define Y_MIN_PIN      PB3
  #endif                
  #ifndef Y_MAX_PIN     
    #define Y_MAX_PIN      PB3
  #endif                
#endif                  
#ifndef Z_STOP_PIN      
  #ifndef Z_MIN_PIN     
    #define Z_MIN_PIN      PA9
  #endif                
  #ifndef Z_MAX_PIN     
    #define Z_MAX_PIN      PA9
  #endif                
#endif   

//
// Steppers
//
// X & Y enable are the same
#define X_STEP_PIN         PB7
#define X_DIR_PIN          PB6
#define X_ENABLE_PIN       PB8

#define Y_STEP_PIN         PB5
#define Y_DIR_PIN          PB4
#define Y_ENABLE_PIN       X_ENABLE_PIN

#define Z_STEP_PIN         PA15
#define Z_DIR_PIN          PA10
#define Z_ENABLE_PIN       X_ENABLE_PIN

#define E0_STEP_PIN        PA8
#define E0_DIR_PIN         PB15
#define E0_ENABLE_PIN      X_ENABLE_PIN

//
// Temperature Sensors
//
#define TEMP_0_PIN         PB1   // Analog Input (HOTEND thermistor)
#define TEMP_BED_PIN       PB0  // Analog Input (BED thermistor)

//
// Heaters / Fans
//
#define HEATER_0_PIN       PA2   // HOTEND MOSFET
#define HEATER_BED_PIN     PA0  // BED MOSFET

#define FAN1_PIN            PA1   // FAN1 header on board - PRINT FAN


//Encoder
#define BTN_EN1           PC15
#define BTN_EN2           PC14
#define BTN_ENC           PC13

// SDCard
#define SDSS                                PA4
#define SD_DETECT_PIN                       -1
#define BEEPER_PIN                          PB10
#define KILL_PIN          -1

//
// SPI
//
#define SD_SCK_PIN                          PA5
#define SD_MISO_PIN                         PA6
#define SD_MOSI_PIN                         PA7




//LCD REPRAP_DISCOUNT_SMART_CONTROLLER
//
 #define LCD_PINS_RS         PB12
 #define LCD_PINS_EN         PB13
 #define LCD_PINS_D4         PB14
 #define LCD_PINS_D5         PA3
 //#define LCD_PINS_D6         PB1//
 //#define LCD_PINS_D7         PB2///<- Ты вонючий алкашь, что блядь это такое, сука, я тебя спрашиваю!



    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS              PB13
      #define DOGLCD_A0              PB12
      //#define LCD_BACKLIGHT_PIN           -1
      #define LCD_RESET_PIN          PB14  // Must be high or open for LCD to operate normally.
      #if ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN         PA3
      #endif
    #endif // !FYSETC_MINI_12864
 // GLCD features
 //
 //#define LCD_CONTRAST   190

 // Uncomment screen orientation
 //
 //#define LCD_SCREEN_ROT_90
 //#define LCD_SCREEN_ROT_180
 //#define LCD_SCREEN_ROT_270

//#define DISABLE_JTAG
//#define FAN_SOFT_PWM