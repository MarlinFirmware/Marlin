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
 * AZTEEG_X3_PRO (Arduino Mega) pin assignments
 */

#include "pins_RAMPS_14.h"

#undef FAN_PIN
#define FAN_PIN             6 //Part Cooling System
#define BEEPER_PIN         33
#define CONTROLLERFAN_PIN   4 //Pin used for the fan to cool motherboard (-1 to disable)
//Fans/Water Pump to cool the hotend cool side.
#define EXTRUDER_0_AUTO_FAN_PIN   5
#define EXTRUDER_1_AUTO_FAN_PIN   5
#define EXTRUDER_2_AUTO_FAN_PIN   5
#define EXTRUDER_3_AUTO_FAN_PIN   5
//
//This section is to swap the MIN and MAX pins because the X3 Pro comes with only
//MIN endstops soldered onto the board. Delta code wants the homing endstops to be
//the MAX so I swapped them here.
//
#if ENABLED(DELTA)
  #undef X_MIN_PIN
  #undef X_MAX_PIN
  #undef Y_MIN_PIN
  #undef Y_MAX_PIN
  #undef Z_MIN_PIN
  #undef Z_MAX_PIN

  #define X_MIN_PIN         2
  #define X_MAX_PIN         3
  #define Y_MIN_PIN        15
  #define Y_MAX_PIN        14
  #define Z_MIN_PIN        19
  #define Z_MAX_PIN        18
#endif
//
#if ENABLED(Z_MIN_PROBE_ENDSTOP)
  //#undef Z_MIN_PIN
  //#define Z_MIN_PIN        15
  // Define a pin to use as the signal pin on Arduino for the Z probe endstop.
  #define Z_MIN_PROBE_PIN  19
#endif
//
#define E2_STEP_PIN        23
#define E2_DIR_PIN         25
#define E2_ENABLE_PIN      40

#define E3_STEP_PIN        27
#define E3_DIR_PIN         29
#define E3_ENABLE_PIN      41

#define E4_STEP_PIN        43
#define E4_DIR_PIN         37
#define E4_ENABLE_PIN      42

#undef HEATER_1_PIN
#undef HEATER_2_PIN
#undef HEATER_3_PIN
#define HEATER_1_PIN        9
#define HEATER_2_PIN       16
#define HEATER_3_PIN       17
#define HEATER_4_PIN        4
#define HEATER_5_PIN        5
#define HEATER_6_PIN        6
#define HEATER_7_PIN       11

#undef TEMP_2_PIN
#undef TEMP_3_PIN
#define TEMP_2_PIN         12   // ANALOG NUMBERING
#define TEMP_3_PIN         11   // ANALOG NUMBERING
#define TEMP_4_PIN         10   // ANALOG NUMBERING
#define TC1                 4   // ANALOG NUMBERING Thermo couple on Azteeg X3Pro
#define TC2                 5   // ANALOG NUMBERING Thermo couple on Azteeg X3Pro

//
//These Servo pins are for when they are defined. Tested for usage with bed leveling
//on a Delta with 1 servo. Running through the Z servo endstop in code.
//Physical wire attachment was done on EXT1 on the GND, 5V, and D47 pins.
//
#define SERVO0_PIN         47

//LCD Pins//

#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #define BEEPER_PIN       33
  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0        44
  #define DOGLCD_CS        45
  #define LCD_SCREEN_ROT_180

  //The encoder and click button
  #define BTN_EN1          22
  #define BTN_EN2           7
  #define BTN_ENC          39  //the click switch

  #define SDSS             53
  #define SD_DETECT_PIN 49

  #define KILL_PIN         31
#endif

#if ENABLED(TEMP_STAT_LEDS)
  #define STAT_LED_RED     32
  #define STAT_LED_BLUE    35
#endif
