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

#ifndef __STM32F1__
  #error "Oops!  Make sure you have an STM32F1 board selected from the 'Tools -> Boards' menu."
#endif

/**
 * 21017 Victor Perez Marlin for stm32f1 test
 */

#define DEFAULT_MACHINE_NAME "3D Printer"
//#define BOARD_NAME "Marlin for STM32"

#ifndef BOARD_NAME
  #define BOARD_NAME "FYSETC CHEETAH"
  #define DEFAULT_WEBSITE_URL "https://fysetc.com"
#endif

// Ignore temp readings during develpment.
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Servos
//
#define SERVO0_PIN         PA0

//
// Limit Switches
//
#define U_MIN_PIN          -1
#define V_MIN_PIN          -1
#define W_MIN_PIN          -1

#define X_STOP_PIN         PA1
#define Y_STOP_PIN         PB4
#define Z_STOP_PIN         PA15

//
// Steppers
//
#define X_STEP_PIN         PB8
#define X_DIR_PIN          PB9
#define X_ENABLE_PIN       PA8

#define Y_STEP_PIN         PB2
#define Y_DIR_PIN          PB3
#define Y_ENABLE_PIN       PB1

#define Z_STEP_PIN         PC0
#define Z_DIR_PIN          PC1
#define Z_ENABLE_PIN       PC2

#define E0_STEP_PIN        PC15
#define E0_DIR_PIN         PC14
#define E0_ENABLE_PIN      PC13

#define X_HARDWARE_SERIAL  MSerial2 // geo-f:need double check
#define Y_HARDWARE_SERIAL  MSerial2
#define Z_HARDWARE_SERIAL  MSerial2
#define E0_HARDWARE_SERIAL MSerial2

#define X_SLAVE_ADDR  0
#define Y_SLAVE_ADDR  1
#define Z_SLAVE_ADDR  2
#define E0_SLAVE_ADDR 3

#define TMC2209_XYZE_IN_ONE

//
// Misc. Functions
//
#define LED_PIN            -1

//
// Filament runout
//
#define FIL_RUNOUT_PIN     PB5

//
// Heaters / Fans
//
#define HEATER_0_PIN       PC6   // EXTRUDER 1 
//#define HEATER_1_PIN     PB1

#define HEATER_BED_PIN     PC7   // BED

#ifndef FAN_PIN
  #define FAN_PIN          PC8
#endif
#define FAN1_PIN           -1


//
// Temperature Sensors
//
#define TEMP_BED_PIN       PC5   // ANALOG NUMBERING
#define TEMP_0_PIN         PC4   // ANALOG NUMBERING
//#define TEMP_1_PIN         PA2   // ANALOG NUMBERING

//
// LCD Pins
//
//#define LCD_RESET_PIN			-1  
#define LCD_RED_PIN				PB0
#define LCD_GREEN_PIN			PB7
#define LCD_BLUE_PIN 			PB6

#define BEEPER_PIN        PC9


// Pins for DOGM SPI LCD Support
#define DOGLCD_A0         PB14
#define DOGLCD_CS         PB12

#define DOGLCD_SCK        PB13
#define DOGLCD_MOSI       PB15

#define LCD_PINS_RS       PB12  // CS    // SOFT SPI for ENDER3 LCD
#define LCD_PINS_D4       PB13 // SCLK
#define LCD_PINS_ENABLE   PB15 // DATA MOSI

#if ENABLED(FYSETC_MINI_12864) || ENABLED (U8GLIB_ST7920)
  #define FORCE_SOFT_SPI
#endif

//#define LCD_BACKLIGHT_PIN -1   // backlight LED on A11/D65
#define SDSS              PA4

//#define KILL_PIN          -1
// GLCD features
//#define LCD_CONTRAST   190
// Uncomment screen orientation
//#define LCD_SCREEN_ROT_90
//#define LCD_SCREEN_ROT_180
//#define LCD_SCREEN_ROT_270
// The encoder and click button
#define BTN_EN1           PC11
#define BTN_EN2           PC10
#define BTN_ENC           PC12
// not connected to a pin
#define SD_DETECT_PIN     PC3

#define RGB_LED_R_PIN PB0
#define RGB_LED_G_PIN PB7
#define RGB_LED_B_PIN PB6

