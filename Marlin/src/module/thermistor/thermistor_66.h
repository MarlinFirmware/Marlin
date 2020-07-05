/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// R25 = 2.5 MOhm, beta25 = 4500 K, 4.7 kOhm pull-up, DyzeDesign 500 °C Thermistor
const temp_entry_t temptable_66[] PROGMEM = {
  { OV(  17.5), 850 },
  { OV(  17.9), 500 },
  { OV(  21.7), 480 },
  { OV(  26.6), 460 },
  { OV(  33.1), 440 },
  { OV(  41.0), 420 },
  { OV(  52.3), 400 },
  { OV(  67.7), 380 },
  { OV(  86.5), 360 },
  { OV( 112.0), 340 },
  { OV( 147.2), 320 },
  { OV( 194.0), 300 },
  { OV( 254.3), 280 },
  { OV( 330.2), 260 },
  { OV( 427.9), 240 },
  { OV( 533.4), 220 },
  { OV( 646.5), 200 },
  { OV( 754.4), 180 },
  { OV( 844.3), 160 },
  { OV( 911.7), 140 },
  { OV( 958.6), 120 },
  { OV( 988.8), 100 },
  { OV(1006.6),  80 },
  { OV(1015.8),  60 },
  { OV(1021.3),  30 },
  { OV(  1022),  25 },
  { OV(  1023),  20 }
};
