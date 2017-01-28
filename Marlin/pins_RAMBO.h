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
 * http://reprap.org/wiki/Rambo_firmware
 */

/**
 * Rambo pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME "Rambo"

#define LARGE_FLASH true

//
// Servos
//
#define SERVO0_PIN         22 // Motor header MX1
#define SERVO1_PIN         23 // Motor header MX2
#define SERVO2_PIN         24 // Motor header MX3
#define SERVO3_PIN          5 // PWM header pin 5

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  30
#endif

#if ENABLED(Z_PROBE_SLED)
  #define SLED_PIN         -1
#endif

//
// Limit Switches
//
#define X_MIN_PIN          12
#define X_MAX_PIN          24
#define Y_MIN_PIN          11
#define Y_MAX_PIN          23
#define Z_MIN_PIN          10
#define Z_MAX_PIN          30

//
// Steppers
//
#define X_STEP_PIN         37
#define X_DIR_PIN          48
#define X_ENABLE_PIN       29

#define Y_STEP_PIN         36
#define Y_DIR_PIN          49
#define Y_ENABLE_PIN       28

#define Z_STEP_PIN         35
#define Z_DIR_PIN          47
#define Z_ENABLE_PIN       27

#define E0_STEP_PIN        34
#define E0_DIR_PIN         43
#define E0_ENABLE_PIN      26

#define E1_STEP_PIN        33
#define E1_DIR_PIN         42
#define E1_ENABLE_PIN      25

// Microstepping pins - Mapping not from fastio.h (?)
#define X_MS1_PIN          40
#define X_MS2_PIN          41
#define Y_MS1_PIN          69
#define Y_MS2_PIN          39
#define Z_MS1_PIN          68
#define Z_MS2_PIN          67
#define E0_MS1_PIN         65
#define E0_MS2_PIN         66
#define E1_MS1_PIN         63
#define E1_MS2_PIN         64

#define DIGIPOTSS_PIN      38
#define DIGIPOT_CHANNELS {4,5,3,0,1} // X Y Z E0 E1 digipot channels to stepper driver mapping

//
// Temperature Sensors
//
#define TEMP_0_PIN          0   // Analog Input
#define TEMP_1_PIN          1   // Analog Input
#define TEMP_BED_PIN        2   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN        9
#define HEATER_1_PIN        7
#define HEATER_2_PIN        6
#define HEATER_BED_PIN      3

#define FAN_PIN             8
#define FAN1_PIN            6
#define FAN2_PIN            2

//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13
#define FILWIDTH_PIN        3   // Analog Input
#define PS_ON_PIN           4

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)

  #define KILL_PIN 80

  #if ENABLED(NEWPANEL)

    #define LCD_PINS_RS 70
    #define LCD_PINS_ENABLE 71
    #define LCD_PINS_D4 72
    #define LCD_PINS_D5 73
    #define LCD_PINS_D6 74
    #define LCD_PINS_D7 75

    #if ENABLED(VIKI2) || ENABLED(miniVIKI)
      #define BEEPER_PIN 44
      // NB: Panucatt's Viki 2.0 wiring diagram (v1.2) indicates that the
      //     beeper/buzzer is connected to pin 33; however, the pin used in the
      //     diagram is actually pin 44, so this is correct.

      #define DOGLCD_A0  70
      #define DOGLCD_CS  71
      #define LCD_SCREEN_ROT_180

      #define BTN_EN1 85
      #define BTN_EN2 84
      #define BTN_ENC 83

      #define SD_DETECT_PIN -1 // Pin 72 if using easy adapter board

      #define STAT_LED_RED_PIN 22
      #define STAT_LED_BLUE_PIN 32

    #else

      #define BEEPER_PIN 79 // AUX-4

      // AUX-2
      #define BTN_EN1 76
      #define BTN_EN2 77
      #define BTN_ENC 78

      #define SD_DETECT_PIN 81

    #endif // VIKI2/miniVIKI

  #else // !NEWPANEL - old style panel with shift register

    // No Beeper added
    #define BEEPER_PIN 33

    // buttons are attached to a shift register
    // Not wired yet
    //#define SHIFT_CLK 38
    //#define SHIFT_LD 42
    //#define SHIFT_OUT 40
    //#define SHIFT_EN 17

    #define LCD_PINS_RS 75
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 23
    #define LCD_PINS_D5 25
    #define LCD_PINS_D6 27
    #define LCD_PINS_D7 29

  #endif // !NEWPANEL

#endif // ULTRA_LCD
