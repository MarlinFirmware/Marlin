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

#undef THERMISTOR_NAME

// Thermcouples
#if THERMISTOR_ID == -3
  #define THERMISTOR_NAME "MAX31855"
#elif THERMISTOR_ID == -2
  #define THERMISTOR_NAME "MAX6675"
#elif THERMISTOR_ID == -1
  #define THERMISTOR_NAME "AD595"

// Standard thermistors
#elif THERMISTOR_ID == 1
  #define THERMISTOR_NAME "EPCOS 100K"
#elif THERMISTOR_ID == 2
  #define THERMISTOR_NAME "ATC 204GT-2"
#elif THERMISTOR_ID == 3
  #define THERMISTOR_NAME "Mendel-parts"
#elif THERMISTOR_ID == 4
  #define THERMISTOR_NAME "Generic 10K"
#elif THERMISTOR_ID == 5
  #define THERMISTOR_NAME "ATC 104GT-2"
#elif THERMISTOR_ID == 6
  #define THERMISTOR_NAME "EPCOS (alt)"
#elif THERMISTOR_ID == 7
  #define THERMISTOR_NAME "Honeywell 104LAG"
#elif THERMISTOR_ID == 71
  #define THERMISTOR_NAME "Honeywell 104LAF"
#elif THERMISTOR_ID == 8
  #define THERMISTOR_NAME "E3104FHT"
#elif THERMISTOR_ID == 9
  #define THERMISTOR_NAME "GE AL03006"
#elif THERMISTOR_ID == 10
  #define THERMISTOR_NAME "RS 198-961"
#elif THERMISTOR_ID == 11
  #define THERMISTOR_NAME "1% beta 3950"
#elif THERMISTOR_ID == 12
  #define THERMISTOR_NAME "Unknown"
#elif THERMISTOR_ID == 13
  #define THERMISTOR_NAME "Hisens"
#elif THERMISTOR_ID == 20
  #define THERMISTOR_NAME "PT100 UltiMB"
#elif THERMISTOR_ID == 60
  #define THERMISTOR_NAME "Makers Tool"
#elif THERMISTOR_ID == 70
  #define THERMISTOR_NAME "Hephestos 2"
#elif THERMISTOR_ID == 75
  #define THERMISTOR_NAME "MGB18"

// Modified thermistors
#elif THERMISTOR_ID == 51
  #define THERMISTOR_NAME "EPCOS 1K"
#elif THERMISTOR_ID == 52
  #define THERMISTOR_NAME "ATC204GT-2 1K"
#elif THERMISTOR_ID == 55
  #define THERMISTOR_NAME "ATC104GT-2 1K"
#elif THERMISTOR_ID == 1047
  #define THERMISTOR_NAME "PT1000 4K7"
#elif THERMISTOR_ID == 1010
  #define THERMISTOR_NAME "PT1000 1K"
#elif THERMISTOR_ID == 147
  #define THERMISTOR_NAME "PT100 4K7"
#elif THERMISTOR_ID == 110
  #define THERMISTOR_NAME "PT100 1K"

// High Temperature thermistors
#elif THERMISTOR_ID == 66
  #define THERMISTOR_NAME "Dyze 4.7M"

// Dummies for dev testing
#elif THERMISTOR_ID == 998
  #define THERMISTOR_NAME "Dummy 1"
#elif THERMISTOR_ID == 999
  #define THERMISTOR_NAME "Dummy 2"

#endif // THERMISTOR_ID
