/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Einstart-S pin assignments
 * PCB Silkscreen: 3DPrinterCon_v3.5
 */

#if ENABLED(TARGET_LPC1768)
  #error "Oops! Set MOTHERBOARD to an LPC1768-based board when building for LPC1768."
#elif defined(__STM32F1__)
  #error "Oops! Set MOTHERBOARD to an STM32F1-based board when building for STM32F1."
#endif

#if DISABLED(IS_RAMPS_SMART, IS_RAMPS_DUO, IS_RAMPS4DUE, TARGET_LPC1768)
  #if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
    #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
  #endif
#endif

#define BOARD_NAME "Einstart-S"

//
// Limit Switches
//
#define X_STOP_PIN         44   // 2560 PIN 40
#define Y_STOP_PIN         43   // 2560 PIN 41
#define Z_STOP_PIN         42   // 2560 PIN 42

//
// Steppers
//
#define X_STEP_PIN         76   // 2560 PIN 68
#define X_DIR_PIN          75   // 2560 PIN 67
#define X_ENABLE_PIN       73   // 2560 PIN 66

#define Y_STEP_PIN         31   // 2560 PIN 59
#define Y_DIR_PIN          32   // 2560 PIN 58
#define Y_ENABLE_PIN       72   // 2560 PIN 65

#define Z_STEP_PIN         34   // 2560 PIN 56
#define Z_DIR_PIN          35   // 2560 PIN 55
#define Z_ENABLE_PIN       33   // 2560 PIN 57

#define E0_STEP_PIN        36   // 2560 PIN 54
#define E0_DIR_PIN         37   // 2560 PIN 53
#define E0_ENABLE_PIN      30   // 2560 PIN 60

//
// Temperature Sensors
//
#define TEMP_0_PIN          0   // Analog Input
#define TEMP_BED_PIN        1   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN       83   // 2560 PIN 49
#define HEATER_BED_PIN     38   // 2560 PIN 50

#define FAN_PIN            82   // 2560 PIN 48

//
// Misc. Functions
//
#define SDSS               53   // 2560 PIN 19
#define LED_PIN             4   // 2560 PIN 1

//////////////////////////
// LCDs and Controllers //
//////////////////////////

//
// LCD Display output pins
//

// Requires #define U8GLIB_SH1106_EINSTART in Configuration.h
// u8glib constructor
// U8GLIB_SH1106_128X64 u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, LCD_PINS_DC, LCD_PINS_RS);

#define LCD_PINS_DC        78   // 2560 PIN 4
#define LCD_PINS_RS        79   // 2560 PIN 8
// DOGM SPI LCD Support
#define DOGLCD_CS           3   // 2560 PIN 7
#define DOGLCD_MOSI         2   // 2560 PIN 6
#define DOGLCD_SCK          5   // 2560 PIN 5
#define DOGLCD_A0           2   // 2560 PIN 6

//
// LCD Display input pins
//
#define BTN_UP             25   // 2560 PIN 75
#define BTN_DWN            26   // 2560 PIN 74
#define BTN_LFT            27   // 2560 PIN 73
#define BTN_RT             28   // 2560 PIN 72

// 'OK' button
#define BTN_ENC            29   // 2560 PIN 71

// Set Kill to right arrow, same as RIGID_PANEL
#define KILL_PIN           28   // 2560 PIN 72
