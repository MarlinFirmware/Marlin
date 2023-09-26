/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
CONFIG_VERSION(02020000)

//
// Custom Thermistor 1000 parameters
//
#if TEMP_SENSOR_0 == 1000
  #define HOTEND0_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define HOTEND0_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define HOTEND0_BETA                    3950 // Beta value
  #define HOTEND0_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_1 == 1000
  #define HOTEND1_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define HOTEND1_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define HOTEND1_BETA                    3950 // Beta value
  #define HOTEND1_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_2 == 1000
  #define HOTEND2_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define HOTEND2_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define HOTEND2_BETA                    3950 // Beta value
  #define HOTEND2_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_3 == 1000
  #define HOTEND3_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define HOTEND3_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define HOTEND3_BETA                    3950 // Beta value
  #define HOTEND3_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_4 == 1000
  #define HOTEND4_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define HOTEND4_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define HOTEND4_BETA                    3950 // Beta value
  #define HOTEND4_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_5 == 1000
  #define HOTEND5_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define HOTEND5_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define HOTEND5_BETA                    3950 // Beta value
  #define HOTEND5_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_6 == 1000
  #define HOTEND6_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define HOTEND6_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define HOTEND6_BETA                    3950 // Beta value
  #define HOTEND6_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_7 == 1000
  #define HOTEND7_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define HOTEND7_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define HOTEND7_BETA                    3950 // Beta value
  #define HOTEND7_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_BED == 1000
  #define BED_PULLUP_RESISTOR_OHMS        4700 // Pullup resistor
  #define BED_RESISTANCE_25C_OHMS       100000 // Resistance at 25C
  #define BED_BETA                        3950 // Beta value
  #define BED_SH_C_COEFF                     0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_CHAMBER == 1000
  #define CHAMBER_PULLUP_RESISTOR_OHMS    4700 // Pullup resistor
  #define CHAMBER_RESISTANCE_25C_OHMS   100000 // Resistance at 25C
  #define CHAMBER_BETA                    3950 // Beta value
  #define CHAMBER_SH_C_COEFF                 0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_COOLER == 1000
  #define COOLER_PULLUP_RESISTOR_OHMS     4700 // Pullup resistor
  #define COOLER_RESISTANCE_25C_OHMS    100000 // Resistance at 25C
  #define COOLER_BETA                     3950 // Beta value
  #define COOLER_SH_C_COEFF                  0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_PROBE == 1000
  #define PROBE_PULLUP_RESISTOR_OHMS      4700 // Pullup resistor
  #define PROBE_RESISTANCE_25C_OHMS     100000 // Resistance at 25C
  #define PROBE_BETA                      3950 // Beta value
  #define PROBE_SH_C_COEFF                   0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_BOARD == 1000
  #define BOARD_PULLUP_RESISTOR_OHMS      4700 // Pullup resistor
  #define BOARD_RESISTANCE_25C_OHMS     100000 // Resistance at 25C
  #define BOARD_BETA                      3950 // Beta value
  #define BOARD_SH_C_COEFF                   0 // Steinhart-Hart C coefficient
#endif

#if TEMP_SENSOR_REDUNDANT == 1000
  #define REDUNDANT_PULLUP_RESISTOR_OHMS  4700 // Pullup resistor
  #define REDUNDANT_RESISTANCE_25C_OHMS 100000 // Resistance at 25C
  #define REDUNDANT_BETA                  3950 // Beta value
  #define REDUNDANT_SH_C_COEFF               0 // Steinhart-Hart C coefficient
#endif
