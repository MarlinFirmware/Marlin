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
 * RAMPS-FD v2
 *
 * EEPROM supported
 * Use 1k thermistor tables
 */

#define BOARD_NAME         "RAMPS-FD v2"

#include "pins_RAMPS_FD_V1.h"

#undef INVERTED_HEATER_PINS
#undef INVERTED_BED_PINS
#undef INVERTED_FAN_PINS

#define I2C_EEPROM

#ifndef PS_ON_PIN
  #define PS_ON_PIN        12
#endif

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN      5   // Analog Input on AUX2
#endif
