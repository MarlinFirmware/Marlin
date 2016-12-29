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

// PT100 with INA826 amp on Ultimaker v2.0 electronics
// The PT100 in the Ultimaker v2.0 electronics has a high sample value for a high temperature.
// This does not match the normal thermistor behaviour so we need to set the following defines
#if (THERMISTORHEATER_0 == 20)
  #define HEATER_0_RAW_HI_TEMP 16383
  #define HEATER_0_RAW_LO_TEMP 0
#endif
#if (THERMISTORHEATER_1 == 20)
  #define HEATER_1_RAW_HI_TEMP 16383
  #define HEATER_1_RAW_LO_TEMP 0
#endif
#if (THERMISTORHEATER_2 == 20)
  #define HEATER_2_RAW_HI_TEMP 16383
  #define HEATER_2_RAW_LO_TEMP 0
#endif
#if (THERMISTORHEATER_3 == 20)
  #define HEATER_3_RAW_HI_TEMP 16383
  #define HEATER_3_RAW_LO_TEMP 0
#endif
#if (THERMISTORBED == 20)
  #define HEATER_BED_RAW_HI_TEMP 16383
  #define HEATER_BED_RAW_LO_TEMP 0
#endif
const short temptable_20[][2] PROGMEM = {
  {   0 * OVERSAMPLENR,    0 },
  { 227 * OVERSAMPLENR,    1 },
  { 236 * OVERSAMPLENR,   10 },
  { 245 * OVERSAMPLENR,   20 },
  { 253 * OVERSAMPLENR,   30 },
  { 262 * OVERSAMPLENR,   40 },
  { 270 * OVERSAMPLENR,   50 },
  { 279 * OVERSAMPLENR,   60 },
  { 287 * OVERSAMPLENR,   70 },
  { 295 * OVERSAMPLENR,   80 },
  { 304 * OVERSAMPLENR,   90 },
  { 312 * OVERSAMPLENR,  100 },
  { 320 * OVERSAMPLENR,  110 },
  { 329 * OVERSAMPLENR,  120 },
  { 337 * OVERSAMPLENR,  130 },
  { 345 * OVERSAMPLENR,  140 },
  { 353 * OVERSAMPLENR,  150 },
  { 361 * OVERSAMPLENR,  160 },
  { 369 * OVERSAMPLENR,  170 },
  { 377 * OVERSAMPLENR,  180 },
  { 385 * OVERSAMPLENR,  190 },
  { 393 * OVERSAMPLENR,  200 },
  { 401 * OVERSAMPLENR,  210 },
  { 409 * OVERSAMPLENR,  220 },
  { 417 * OVERSAMPLENR,  230 },
  { 424 * OVERSAMPLENR,  240 },
  { 432 * OVERSAMPLENR,  250 },
  { 440 * OVERSAMPLENR,  260 },
  { 447 * OVERSAMPLENR,  270 },
  { 455 * OVERSAMPLENR,  280 },
  { 463 * OVERSAMPLENR,  290 },
  { 470 * OVERSAMPLENR,  300 },
  { 478 * OVERSAMPLENR,  310 },
  { 485 * OVERSAMPLENR,  320 },
  { 493 * OVERSAMPLENR,  330 },
  { 500 * OVERSAMPLENR,  340 },
  { 507 * OVERSAMPLENR,  350 },
  { 515 * OVERSAMPLENR,  360 },
  { 522 * OVERSAMPLENR,  370 },
  { 529 * OVERSAMPLENR,  380 },
  { 537 * OVERSAMPLENR,  390 },
  { 544 * OVERSAMPLENR,  400 },
  { 614 * OVERSAMPLENR,  500 },
  { 681 * OVERSAMPLENR,  600 },
  { 744 * OVERSAMPLENR,  700 },
  { 805 * OVERSAMPLENR,  800 },
  { 862 * OVERSAMPLENR,  900 },
  { 917 * OVERSAMPLENR, 1000 },
  { 968 * OVERSAMPLENR, 1100 }
};
