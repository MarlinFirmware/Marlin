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
 * Elefu RA Board Pin Assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         49
#define X_DIR_PIN          13
#define X_ENABLE_PIN       48
#define X_MIN_PIN          35
#define X_MAX_PIN          34

#define Y_STEP_PIN         11
#define Y_DIR_PIN           9
#define Y_ENABLE_PIN       12
#define Y_MIN_PIN          33
#define Y_MAX_PIN          32

#define Z_STEP_PIN          7
#define Z_DIR_PIN           6
#define Z_ENABLE_PIN        8
#define Z_MIN_PIN          31
#define Z_MAX_PIN          30

#define E2_STEP_PIN        43
#define E2_DIR_PIN         47
#define E2_ENABLE_PIN      42

#define E1_STEP_PIN        18
#define E1_DIR_PIN         19
#define E1_ENABLE_PIN      38

#define E0_STEP_PIN        40
#define E0_DIR_PIN         41
#define E0_ENABLE_PIN      37

#define SDPOWER            -1
#define LED_PIN            -1 //Use +12V Aux port for LED Ring

#define FAN_PIN            16 //5V PWM

#define PS_ON_PIN          10 //Set to -1 if using a manual switch on the PWRSW Connector
#define SLEEP_WAKE_PIN     26 //This feature still needs work

#define HEATER_0_PIN       45 //12V PWM1
#define HEATER_1_PIN       46 //12V PWM2
#define HEATER_2_PIN       17 //12V PWM3
#define HEATER_BED_PIN     44 //DOUBLE 12V PWM
#define TEMP_0_PIN          3 //ANALOG NUMBERING
#define TEMP_1_PIN          2 //ANALOG NUMBERING
#define TEMP_2_PIN          1 //ANALOG NUMBERING
#define TEMP_BED_PIN        0 //ANALOG NUMBERING

#define BEEPER_PIN         36

#define KILL_PIN           -1

// M240  Triggers a camera by emulating a Canon RC-1 Remote
// Data from: http://www.doc-diy.net/photo/rc-1_hacked/
#define PHOTOGRAPH_PIN     29

#if ENABLED(RA_CONTROL_PANEL)

  #define SDSS             53
  #define SD_DETECT_PIN    28

  #define BTN_EN1          14
  #define BTN_EN2          39
  #define BTN_ENC          15  //the click

  #define BLEN_C            2
  #define BLEN_B            1
  #define BLEN_A            0

#endif // RA_CONTROL_PANEL

#if ENABLED(RA_DISCO)
  //variables for which pins the TLC5947 is using
  #define TLC_CLOCK_PIN    25
  #define TLC_BLANK_PIN    23
  #define TLC_XLAT_PIN     22
  #define TLC_DATA_PIN     24

  //We also need to define pin to port number mapping for the 2560 to match the pins listed above. If you change the TLC pins, update this as well per the 2560 datasheet!
  //This currently only works with the RA Board.
  #define TLC_CLOCK_BIT 3 //bit 3 on port A
  #define TLC_CLOCK_PORT &PORTA //bit 3 on port A

  #define TLC_BLANK_BIT 1 //bit 1 on port A
  #define TLC_BLANK_PORT &PORTA //bit 1 on port A

  #define TLC_DATA_BIT 2 //bit 2 on port A
  #define TLC_DATA_PORT &PORTA //bit 2 on port A

  #define TLC_XLAT_BIT 0 //bit 0 on port A
  #define TLC_XLAT_PORT &PORTA //bit 0 on port A

  //change this to match your situation. Lots of TLCs takes up the arduino SRAM very quickly, so be careful
  //Leave it at at least 1 if you have enabled RA_LIGHTING
  //The number of TLC5947 boards chained together for use with the animation, additional ones will repeat the animation on them, but are not individually addressable and mimic those before them. You can leave the default at 2 even if you only have 1 TLC5947 module.
  #define NUM_TLCS 2

  //These TRANS_ARRAY values let you change the order the LEDs on the lighting modules will animate for chase functions.
  //Modify them according to your specific situation.
  //NOTE: the array should be 8 long for every TLC you have. These defaults assume (2) TLCs.
  #define TRANS_ARRAY {0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8} //forwards
  //#define TRANS_ARRAY {7, 6, 5, 4, 3, 2, 1, 0, 8, 9, 10, 11, 12, 13, 14, 15} //backwards
#endif // RA_DISCO
