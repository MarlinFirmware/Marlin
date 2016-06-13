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
 * Gen6 pin assignments
 */

#ifndef __AVR_ATmega644P__
  #ifndef __AVR_ATmega1284P__
    #error "Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu."
  #endif
#endif

//x axis pins
#define X_STEP_PIN      15
#define X_DIR_PIN       18
#define X_ENABLE_PIN    19
#define X_STOP_PIN      20

//y axis pins
#define Y_STEP_PIN      23
#define Y_DIR_PIN       22
#define Y_ENABLE_PIN    24
#define Y_STOP_PIN      25

//z axis pins
#define Z_STEP_PIN      27
#define Z_DIR_PIN       28
#define Z_ENABLE_PIN    29
#define Z_STOP_PIN      30

//extruder pins
#define E0_STEP_PIN      4    //Edited @ EJE Electronics 20100715
#define E0_DIR_PIN       2    //Edited @ EJE Electronics 20100715
#define E0_ENABLE_PIN    3    //Added @ EJE Electronics 20100715
#define TEMP_0_PIN      5     //changed @ rkoeppl 20110410

#define HEATER_0_PIN    14    //changed @ rkoeppl 20110410

#if !MB(GEN6)
  #define HEATER_BED_PIN   1    //changed @ rkoeppl 20110410
  #define TEMP_BED_PIN     0    //changed @ rkoeppl 20110410
#endif

#define SDSS          17
//our pin for debugging.

#define DEBUG_PIN        0

//our RS485 pins
#define TX_ENABLE_PIN 12
#define RX_ENABLE_PIN 13
