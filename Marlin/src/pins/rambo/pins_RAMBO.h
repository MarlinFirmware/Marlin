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
 * IMPORTANT NOTE:
 * Rambo users should be sure to compile Marlin using either the RAMBo
 * board type if using the Arduino IDE - available via the link below - or
 * the 'rambo' environment if using platformio, by specifying '-e rambo' on
 * the command line or by changing the value of the 'env_default' variable to
 * 'rambo' in the supplied platformio.ini.
 *
 * If you don't compile using the proper board type, the RAMBo's extended
 * pins will likely be unavailable and accessories/addons may not work.
 *
 * Instructions for installing the Arduino RAMBo board type for the
 * Arduino IDE are available at:
 * https://reprap.org/wiki/Rambo_firmware
 */

/**
 * Rambo pin assignments
 * Schematic (1.1b): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/RAMBo/Rambo1-1-schematic.png
 * Origin (1.1b): https://www.reprap.org/wiki/File:Rambo1-1-schematic.png
 */

#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Rambo"
#endif

#ifndef AVR_CHIPOSCILLATOR_FREQ
  #define AVR_CHIPOSCILLATOR_FREQ 16000000
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                          PinA0  // Motor header MX1
#endif
#define SERVO1_PIN                            PinA1  // Motor header MX2
#ifndef SERVO2_PIN
  #define SERVO2_PIN                          PinA2  // Motor header MX3
#endif
#define SERVO3_PIN                            PinE3  // PWM header pin 5

//
// Limit Switches
//
#define X_MIN_PIN                             PinB6
#define X_MAX_PIN                             PinA2
#define Y_MIN_PIN                             PinB5
#define Y_MAX_PIN                             PinA1
#ifndef Z_MIN_PIN
  #define Z_MIN_PIN                           PinB4
#endif
#define Z_MAX_PIN                             PinC7

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC7
#endif

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinE3
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinC0
#define X_DIR_PIN                             PinL1
#define X_ENABLE_PIN                          PinA7

#define Y_STEP_PIN                            PinC1
#define Y_DIR_PIN                             PinL0
#define Y_ENABLE_PIN                          PinA6

#define Z_STEP_PIN                            PinC2
#define Z_DIR_PIN                             PinL2
#define Z_ENABLE_PIN                          PinA5

#define E0_STEP_PIN                           PinC3
#define E0_DIR_PIN                            PinL6
#define E0_ENABLE_PIN                         PinA4

#define E1_STEP_PIN                           PinC4
#define E1_DIR_PIN                            PinL7
#define E1_ENABLE_PIN                         PinA3

// Microstepping pins - Mapping not from fastio.h (?)
#define X_MS1_PIN                             PinG1
#define X_MS2_PIN                             PinG0
#define Y_MS1_PIN                             PinK7
#define Y_MS2_PIN                             PinG2
#define Z_MS1_PIN                             PinK6
#define Z_MS2_PIN                             PinK5
#define E0_MS1_PIN                            PinK3
#define E0_MS2_PIN                            PinK4
#define E1_MS1_PIN                            PinK1
#define E1_MS2_PIN                            PinK2

#define DIGIPOTSS_PIN                         PinD7
#define DIGIPOT_CHANNELS { 4, 5, 3, 0, 1 }        // X Y Z E0 E1 digipot channels to stepper driver mapping
#ifndef DIGIPOT_MOTOR_CURRENT
  #define DIGIPOT_MOTOR_CURRENT { 135,135,135,135,135 }   // Values 0-255 (RAMBO 135 = ~0.75A, 185 = ~1A)
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinE0  // Analog Input
#define TEMP_1_PIN                            PinE1  // Analog Input
#define TEMP_BED_PIN                          PinE4  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinH6
#define HEATER_1_PIN                          PinH4
#define HEATER_2_PIN                          PinH3
#define HEATER_BED_PIN                        PinE5

#ifndef FAN_PIN
  #define FAN_PIN                             PinH5
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                            PinH3
#endif
#ifndef FAN2_PIN
  #define FAN2_PIN                            PinE4
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7
#define PS_ON_PIN                             PinG5

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinL3
#endif

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                        PinE5  // Analog Input
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN                 PinL4  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                 PinC6  // Pullup!
#define SPINDLE_DIR_PIN                       PinC5

//
// SPI for MAX Thermocouple
//
#ifndef TEMP_0_CS_PIN
  #define TEMP_0_CS_PIN                       PinC5  // SPINDLE_DIR_PIN / STAT_LED_BLUE_PIN
#endif

//
// M7/M8/M9 - Coolant Control
//
#define COOLANT_MIST_PIN                      PinA0
#define COOLANT_FLOOD_PIN                     PinL5

//
// Průša i3 MK2 Multiplexer Support
//
#if HAS_PRUSA_MMU1
  #define E_MUX0_PIN                          PinH0
  #define E_MUX1_PIN                          PinH1
  #define E_MUX2_PIN                          PinH2  // 84 in MK2 Firmware
#endif

//
// LCD / Controller
//
#if HAS_WIRED_LCD || TOUCH_UI_ULTIPANEL

  #define KILL_PIN                            PinE7

  #if IS_ULTIPANEL || TOUCH_UI_ULTIPANEL

    #define LCD_PINS_RS                       PinG4
    #define LCD_PINS_ENABLE                   PinG3
    #define LCD_PINS_D4                       PinJ2
    #define LCD_PINS_D5                       PinJ3
    #define LCD_PINS_D6                       PinJ7
    #define LCD_PINS_D7                       PinJ4

    #if EITHER(VIKI2, miniVIKI)
      #define BEEPER_PIN                      PinL5
      // NB: Panucatt's Viki 2.0 wiring diagram (v1.2) indicates that the
      //     beeper/buzzer is connected to pin 33; however, the pin used in the
      //     diagram is actually pin 44, so this is correct.

      #define DOGLCD_A0                       PinG4
      #define DOGLCD_CS                       PinG3

      #define BTN_EN1                         PinH7
      #define BTN_EN2                         PinH2
      #define BTN_ENC                         PinD6

      #define SD_DETECT_PIN                   -1  // Pin 72 if using easy adapter board

      #define STAT_LED_RED_PIN                PinA0
      #define STAT_LED_BLUE_PIN               PinC5

      #define LCD_SCREEN_ROTATE              180  // 0, 90, 180, 270

    #else                                         // !VIKI2 && !miniVIKI

      #define BEEPER_PIN                      PinE6  // AUX-4

      // AUX-2
      #ifndef BTN_EN1
        #define BTN_EN1                       PinJ5
      #endif
      #ifndef BTN_EN2
        #define BTN_EN2                       PinJ6
      #endif
      #define BTN_ENC                         PinE2

      #define SD_DETECT_PIN                   PinD4

    #endif // !VIKI2 && !miniVIKI

    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif

  #else                                           // !IS_NEWPANEL - old style panel with shift register

    // No Beeper added
    #define BEEPER_PIN                        PinC4

    // Buttons attached to a shift register
    // Not wired yet
    //#define SHIFT_CLK_PIN                   PinD7
    //#define SHIFT_LD_PIN                    PinL7
    //#define SHIFT_OUT_PIN                   PinG1
    //#define SHIFT_EN_PIN                    PinH0

    #define LCD_PINS_RS                       PinJ4
    #define LCD_PINS_ENABLE                   PinH0
    #define LCD_PINS_D4                       PinA1
    #define LCD_PINS_D5                       PinA3
    #define LCD_PINS_D6                       PinA5
    #define LCD_PINS_D7                       PinA7

  #endif // !IS_NEWPANEL

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                 0
  #define BOARD_ST7920_DELAY_2                 0
  #define BOARD_ST7920_DELAY_3                 0
#endif
