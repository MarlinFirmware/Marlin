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
 * Geeetech HJC2560-C Rev 2.x board pin assignments
 * ATmega2560
 */

#include "env_validate.h"

#define DEFAULT_MACHINE_NAME "ADIMLab Gantry v2"
#define BOARD_INFO_NAME      "HJC2560-C"

//
// Servos
//
//#ifndef SERVO0_PIN
//  #define SERVO0_PIN       PinB5
//#endif

//
// Limit Switches
//
#define X_STOP_PIN                            PinA0
#define Y_STOP_PIN                            PinA4
#define Z_STOP_PIN                            PinA7
//#define EXP_STOP_PIN                        PinA6

//
// Steppers
//
#define X_STEP_PIN                            PinA3
#define X_DIR_PIN                             PinA1
#define X_ENABLE_PIN                          PinA5

#define Y_STEP_PIN                            PinC5
#define Y_DIR_PIN                             PinC4
#define Y_ENABLE_PIN                          PinC6

#define Z_STEP_PIN                            PinC2
#define Z_DIR_PIN                             PinC1
#define Z_ENABLE_PIN                          PinC3

#define E0_STEP_PIN                           PinL7
#define E0_DIR_PIN                            PinL6
#define E0_ENABLE_PIN                         PinC0

#define E1_STEP_PIN                           PinL0
#define E1_DIR_PIN                            PinL2
#define E1_ENABLE_PIN                         PinL1

#define MOTOR_CURRENT_PWM_XY_PIN              PinL5
#define MOTOR_CURRENT_PWM_Z_PIN               PinL4
#define MOTOR_CURRENT_PWM_E_PIN               PinL3
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE            2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  { 1300, 1300, 1250 }

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinH5  // Analog Input
#define TEMP_1_PIN                            PinH6  // Analog Input
#define TEMP_BED_PIN                          PinB4  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE4
#define HEATER_1_PIN                          PinE5
#define HEATER_BED_PIN                        PinG5

#ifndef FAN_PIN
  #define FAN_PIN                             PinH4  //默认不使用PWM_FAN冷却喷嘴，如果需要，则取消注释
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define SD_DETECT_PIN                         PinG2
//#define LED_PIN                             PinH5

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      PinH5  // 8 默认挤出机风扇作为Case LED，如果需要PWM FAN,则需要将FAN_PIN置为7，LED_PIN置为8
#endif

//#define SAFETY_TRIGGERED_PIN                PinA6  // PIN to detect the safety circuit has triggered
//#define MAIN_VOLTAGE_MEASURE_PIN            PinJ1  // ANALOG PIN to measure the main voltage, with a 100k - 4k7 resitor divider.

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER
  #define SPINDLE_DIR_PIN                     PinH1
  #define SPINDLE_LASER_ENA_PIN               PinH0  // Pin should have a pullup!
  #define SPINDLE_LASER_PWM_PIN               PinH6  // Hardware PWM
#endif

//
// LCD / Controller
//
#if HAS_WIRED_LCD

  #define BEEPER_PIN                          PinD3

  #if IS_NEWPANEL

    #define LCD_PINS_RS                       PinD1  // LCD_CS
    #define LCD_PINS_ENABLE                   PinJ0  // LCD_SDA
    #define LCD_PINS_D4                       PinJ1  // LCD_SCK

    #if ENABLED(HJC_LCD_SMART_CONTROLLER)
      #define LCD_BACKLIGHT_PIN               PinE3  // LCD_Backlight
      //#ifndef LCD_CONTRAST_PIN
      //  #define LCD_CONTRAST_PIN            PinE3  // LCD_Contrast
      //#endif
      #ifndef FIL_RUNOUT_PIN
        #define FIL_RUNOUT_PIN                PinA2  // Filament runout
      #endif
    #else
      #define LCD_PINS_D5                     PinD0
      #define LCD_PINS_D6                     PinE3
      #define LCD_PINS_D7                     PinH3
    #endif

    #define BTN_EN1                           PinG0
    #define BTN_EN2                           PinG1
    #define BTN_ENC                           PinD2

    #define SD_DETECT_PIN                     PinG2

  #else

    // Buttons attached to a shift register
    #define SHIFT_CLK_PIN                     PinD7
    #define SHIFT_LD_PIN                      PinL7
    #define SHIFT_OUT_PIN                     PinG1
    #define SHIFT_EN_PIN                      PinH0

    #define LCD_PINS_RS                       PinH1
    #define LCD_PINS_ENABLE                   PinE3
    #define LCD_PINS_D4                       PinH3
    #define LCD_PINS_D5                       PinD0
    #define LCD_PINS_D6                       PinD1
    #define LCD_PINS_D7                       PinD2

  #endif // !IS_NEWPANEL

#endif // HAS_WIRED_LCD
