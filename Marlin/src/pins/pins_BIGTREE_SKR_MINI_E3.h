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

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

/**
 * Marlin for STM32F103RC
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "BIGTREE SKR MINI E3"
#endif

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Release PB3/PB4 (E0 STP/DIR) from JTAG pins
//
#define DISABLE_JTAG

//
// Servos
//
#define SERVO0_PIN         PA1

//
// Limit Switches
//
#define X_MAX_PIN          PC0
#define Y_MAX_PIN          PC1
#define Z_MAX_PIN          PC2
#define X_MIN_PIN          PC0
#define Y_MIN_PIN          PC1
#define Z_MIN_PIN          PC2

//
// Z Probe must be this pins
//
#define Z_MIN_PROBE_PIN    PC14

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PC15
#endif

//
// Steppers
//
#define X_ENABLE_PIN       PB14
#define X_STEP_PIN         PB13
#define X_DIR_PIN          PB12

#define Y_ENABLE_PIN       PB11
#define Y_STEP_PIN         PB10
#define Y_DIR_PIN          PB2

#define Z_ENABLE_PIN       PB1
#define Z_STEP_PIN         PB0
#define Z_DIR_PIN          PC5

#define E0_ENABLE_PIN      PD2
#define E0_STEP_PIN        PB3
#define E0_DIR_PIN         PB4

#if HAS_DRIVER(TMC2209)
    /**
   * TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  #define X_HARDWARE_SERIAL  Serial4
  #define Y_HARDWARE_SERIAL  Serial4
  #define Z_HARDWARE_SERIAL  Serial4
  #define E0_HARDWARE_SERIAL Serial4
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         PA0   // ANALOG NUMBERING
#define TEMP_BED_PIN       PC3   // ANALOG NUMBERING

//
// Heaters / Fans
//
#define HEATER_0_PIN       PC8   // EXTRUDER 1
#define HEATER_BED_PIN     PC9   // BED
#define FAN_PIN            PA8

//
// USB connect control
//
#define USB_CONNECT        PC13
#define USB_CONNECT_INVERTING false

#define SD_DETECT_PIN      PC4

#define SS_PIN             PA4   // Chip select for SD card used by Marlin
#define SCK_PIN            PA5
#define MISO_PIN           PA6
#define MOSI_PIN           PA7
#define SDSS               SS_PIN
/*
|                    _____
|                5V | · · | GND 
|     (LCD_EN) PB7  | · · | PB8  (LCD_RS)
|     (LCD_D4) PB9  | · · | PA10 (BTN_EN2)
|             RESET | · · | PA9  (BTN_EN1)
|    (BTN_ENC) PB6  | · · | PB5  (BEEPER)
|                    ￣￣
|                    EXP1
*/
#if ENABLED(ULTRA_LCD)
  #define BEEPER_PIN       PB5
  #define BTN_ENC          PB6

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS    PB8

    #define BTN_EN1        PA9
    #define BTN_EN2        PA10

    #define LCD_PINS_ENABLE PB7
    #define LCD_PINS_D4    PB9
  #else
    #error "only CR10_STOCKDISPLAY supported!"
  #endif

#endif // ULTRA_LCD

#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 DELAY_NS(125)
#endif
