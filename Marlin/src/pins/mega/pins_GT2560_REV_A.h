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
 * Geeetech GT2560 Revision A board pin assignments, based on the work of
 * George Robles (https://georges3dprinters.com) and
 * Richard Smith <galorin@gmail.com>
 * ATmega2560
 */

#define ALLOW_MEGA1280
#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "GT2560 Rev.A"
#endif
#define DEFAULT_MACHINE_NAME "Prusa i3 Pro B"

#define AVR_CHIPOSCILLATOR_FREQ 16000000

// See page 327 of the ATmega2560 technical reference manual (DS40002211A).
// For CPU speed calculations it is necessary to give the programmed "fuse bits", that are internally programmed
// MCU configuration data structures not accessible by the MCU instruction pipeline itself.
#define AVR_EFUSE_VALUE 0xFD
#define AVR_HFUSE_VALUE 0xD8
#define AVR_LFUSE_VALUE 0xFF

//
// Limit Switches
//
#define X_MIN_PIN                             PinA0
#define X_MAX_PIN                             PinA2
#define Y_MIN_PIN                             PinA4
#define Y_MAX_PIN                             PinA6
#define Z_MIN_PIN                             PinC7

#if ENABLED(BLTOUCH)
  #if MB(GT2560_REV_A_PLUS)
    #define SERVO0_PIN                        PinB5
    #define Z_MAX_PIN                         PinC5
  #else
    #define SERVO0_PIN                        PinC5
    #define Z_MAX_PIN                         -1
  #endif
#else
  #define Z_MAX_PIN                           PinC5
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinA3
#define X_DIR_PIN                             PinA1
#define X_ENABLE_PIN                          PinA5

#define Y_STEP_PIN                            PinC6
#define Y_DIR_PIN                             PinC4
#define Y_ENABLE_PIN                          PinA7

#define Z_STEP_PIN                            PinC0
#define Z_DIR_PIN                             PinG2
#define Z_ENABLE_PIN                          PinC2

#define E0_STEP_PIN                           PinL6
#define E0_DIR_PIN                            PinL4
#define E0_ENABLE_PIN                         PinG0

#define E1_STEP_PIN                           PinL0
#define E1_DIR_PIN                            PinL2
#define E1_ENABLE_PIN                         PinL1

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinH5
#define TEMP_1_PIN                            PinH6
#define TEMP_BED_PIN                          PinB4

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE4
#define HEATER_1_PIN                          PinE5
#define HEATER_BED_PIN                        PinG5
#ifndef FAN_PIN
  #define FAN_PIN                             PinH4
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7
#define PS_ON_PIN                             PinB6
#define SUICIDE_PIN                           PinF0  // Must be enabled at startup to keep power flowing
#define KILL_PIN                              -1

#if HAS_WIRED_LCD

  #define BEEPER_PIN                          PinD3

  #if IS_NEWPANEL

    #if ENABLED(MKS_MINI_12864)
      #define DOGLCD_A0                       PinE3
      #define DOGLCD_CS                       PinD0
      #define BTN_EN1                         PinG1
      #define BTN_EN2                         PinL7
    #elif ENABLED(FYSETC_MINI_12864)
      // Disconnect EXP2-1 and EXP2-2, otherwise future firmware upload won't work.
      #define DOGLCD_A0                       PinD1
      #define DOGLCD_CS                       PinH0

      #define NEOPIXEL_PIN                    PinD0
      #define BTN_EN1                         PinL7
      #define BTN_EN2                         PinG1

      #define LCD_RESET_PIN                   PinH1

      #define LCD_CONTRAST_INIT              220

      #define LCD_BACKLIGHT_PIN               -1
    #else
      #define LCD_PINS_RS                     PinD1
      #define LCD_PINS_ENABLE                 PinH0
      #define LCD_PINS_D4                     PinH1
      #define LCD_PINS_D5                     PinD0
      #define LCD_PINS_D6                     PinE3
      #define LCD_PINS_D7                     PinH3
      #define BTN_EN1                         PinL7
      #define BTN_EN2                         PinG1
    #endif

    #define BTN_ENC                           PinD2
    #define SD_DETECT_PIN                     PinD7

  #else                                           // !IS_NEWPANEL

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

    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif

    #define SD_DETECT_PIN                     -1

  #endif // !IS_NEWPANEL

#endif // HAS_WIRED_LCD
