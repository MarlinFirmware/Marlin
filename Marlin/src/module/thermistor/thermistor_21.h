/*
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

// PT100 with E3D INA826 amp on 3.3V SKR 1.3 (Smoothieboard)
/// *** SOME TEMPS EXTRAPOLATED *** See table below.
// The PT100 in the Ultimaker v2.0 electronics has a high sample value for a high temperature.
// This does not match the normal thermistor behavior so we need to set the following defines
#if THERMISTOR_HEATER_0 == 21
  #define HEATER_0_RAW_HI_TEMP 16383
  #define HEATER_0_RAW_LO_TEMP 0
#endif
#if THERMISTOR_HEATER_1 == 21
  #define HEATER_1_RAW_HI_TEMP 16383
  #define HEATER_1_RAW_LO_TEMP 0
#endif
#if THERMISTOR_HEATER_2 == 21
  #define HEATER_2_RAW_HI_TEMP 16383
  #define HEATER_2_RAW_LO_TEMP 0
#endif
#if THERMISTOR_HEATER_3 == 21
  #define HEATER_3_RAW_HI_TEMP 16383
  #define HEATER_3_RAW_LO_TEMP 0
#endif
#if THERMISTOR_HEATER_4 == 21
  #define HEATER_4_RAW_HI_TEMP 16383
  #define HEATER_4_RAW_LO_TEMP 0
#endif
#if THERMISTOR_HEATER_5 == 21
  #define HEATER_5_RAW_HI_TEMP 16383
  #define HEATER_5_RAW_LO_TEMP 0
#endif
#if THERMISTORBED == 21
  #define HEATER_BED_RAW_HI_TEMP 16383
  #define HEATER_BED_RAW_LO_TEMP 0
#endif
#if THERMISTORCHAMBER == 21
  #define HEATER_CHAMBER_RAW_HI_TEMP 16383
  #define HEATER_CHAMBER_RAW_LO_TEMP 0
#endif
const short temptable_21[][2] PROGMEM = {
  { OV(  0),    0 },  // NA
  { OV(227),    1 },  // EXTRAPOLATED
  { OV(236),   10 },  // EXTRAPOLATED
  { OV(245),   20 },  // EXTRAPOLATED
  { OV(253),   30 },  // EXTRAPOLATED
  { OV(262),   40 },  // MEASURED
  { OV(270),   50 },  // MEASURED
  { OV(275),   60 },  // MEASURED
  { OV(285),   70 },  // MEASURED
  { OV(294),   80 },  // MEASURED
  { OV(302),   90 },  // MEASURED
  { OV(310),  100 },  // MEASURED
  { OV(318),  110 },  // MEASURED
  { OV(326),  120 },  // MEASURED
  { OV(335),  130 },  // MEASURED
  { OV(342),  140 },  // MEASURED
  { OV(351),  150 },  // MEASURED
  { OV(359),  160 },  // MEASURED
  { OV(367),  170 },  // MEASURED
  { OV(375),  180 },  // MEASURED
  { OV(383),  190 },  // MEASURED
  { OV(391),  200 },  // MEASURED
  { OV(399),  210 },  // MEASURED
  { OV(407),  220 },  // MEASURED
  { OV(415),  230 },  // MEASURED
  { OV(423),  240 },  // MEASURED
  { OV(432),  250 },  // MEASURED
  { OV(440),  260 },  // MEASURED
  { OV(447),  270 },  // MEASURED
  { OV(454),  280 },  // MEASURED
  { OV(462),  290 },  // MEASURED
  { OV(470),  300 },  // MEASURED
  { OV(478),  310 },  // MEASURED
  { OV(485),  320 },  // MEASURED
  { OV(492),  330 },  // MEASURED
  { OV(499),  340 },  // MEASURED
  { OV(507),  350 },  // MEASURED
  { OV(514),  360 },  // MEASURED
  { OV(520),  370 },  // MEASURED
  { OV(522),  380 },  // MEASURED
  { OV(524),  390 },  // MEASURED
  { OV(526),  400 },  // MEASURED
  { OV(528),  410 },  // MEASURED
  { OV(531),  420 },  // MEASURED
  { OV(533),  430 },  // MEASURED
  { OV(535),  440 },  // MEASURED
  { OV(537),  450 },  // MEASURED
  { OV(614),  500 },  // ??
  { OV(681),  600 },  // ??
  { OV(744),  700 },  // ??
  { OV(805),  800 },  // ??
  { OV(862),  900 },  // ??
  { OV(917), 1000 },  // ??
  { OV(968), 1100 }   // ??
};
