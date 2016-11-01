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
 * Rambo pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME "Rambo"

#define LARGE_FLASH true

// Servo support
#define SERVO0_PIN 22 // Motor header MX1
#define SERVO1_PIN 23 // Motor header MX2
#define SERVO2_PIN 24 // Motor header MX3
#define SERVO3_PIN  5 // PWM header pin 5

#if ENABLED(Z_PROBE_SLED)
  #define SLED_PIN         -1
#endif

#define X_MIN_PIN 12
#define X_MAX_PIN 24
#define Y_MIN_PIN 11
#define Y_MAX_PIN 23
#define Z_MIN_PIN 10
#define Z_MAX_PIN 30
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN 10
#endif

#define X_STEP_PIN 37
#define X_DIR_PIN 48
#define X_ENABLE_PIN 29
#define X_MS1_PIN 40
#define X_MS2_PIN 41

#define Y_STEP_PIN 36
#define Y_DIR_PIN 49
#define Y_ENABLE_PIN 28
#define Y_MS1_PIN 69
#define Y_MS2_PIN 39

#define Z_STEP_PIN 35
#define Z_DIR_PIN 47
#define Z_ENABLE_PIN 27
#define Z_MS1_PIN 68
#define Z_MS2_PIN 67

#define HEATER_BED_PIN 3
#define TEMP_BED_PIN 2

#define HEATER_0_PIN  9
#define TEMP_0_PIN 0

#define HEATER_1_PIN 7
#define TEMP_1_PIN 1

#if ENABLED(BARICUDA)
  #define HEATER_2_PIN 6
#endif

#define E0_STEP_PIN         34
#define E0_DIR_PIN          43
#define E0_ENABLE_PIN       26
#define E0_MS1_PIN 65
#define E0_MS2_PIN 66

#define E1_STEP_PIN         33
#define E1_DIR_PIN          42
#define E1_ENABLE_PIN       25
#define E1_MS1_PIN 63
#define E1_MS2_PIN 64

#define DIGIPOTSS_PIN 38
#define DIGIPOT_CHANNELS {4,5,3,0,1} // X Y Z E0 E1 digipot channels to stepper driver mapping

#define SDSS               53
#define LED_PIN            13
#define FAN_PIN            8

#define FILWIDTH_PIN        3  // ANALOG NUMBERING

/**********************************************************
  Fan Pins
  Fan_0 8
  Fan_1 6
  Fan_2 2
***********************************************************/
#define PS_ON_PIN          4

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

      #define DOGLCD_A0  70
      #define DOGLCD_CS  71
      #define LCD_SCREEN_ROT_180

      #define BTN_EN1 85
      #define BTN_EN2 84
      #define BTN_ENC 83

      #define SD_DETECT_PIN -1 // Pin 72 if using easy adapter board

      #if ENABLED(TEMP_STAT_LEDS)
        #define STAT_LED_RED      22
        #define STAT_LED_BLUE     32
      #endif

    #else

      #define BEEPER_PIN 79 // AUX-4

      // AUX-2
      #define BTN_EN1 76
      #define BTN_EN2 77
      #define BTN_ENC 78

      #define SD_DETECT_PIN 81

    #endif // VIKI2/miniVIKI

  #else //!NEWPANEL - old style panel with shift register

    // No Beeper added
    #define BEEPER_PIN 33

    //buttons are attached to a shift register
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

