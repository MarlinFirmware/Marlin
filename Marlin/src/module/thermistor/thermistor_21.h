/**
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

#define REVERSE_TEMP_SENSOR_RANGE

// Pt100 with INA826 amp on 32 bit mcu based on "Pt100 with INA826 amp on Ultimaker v2.0 electronics"
const short temptable_21[][2] PROGMEM = {
  { OV(  0),    0 },
  { OV(344),    1 },
  { OV(358),   10 },
  { OV(371),   20 },
  { OV(383),   30 },
  { OV(397),   40 },
  { OV(409),   50 },
  { OV(423),   60 },
  { OV(435),   70 },
  { OV(447),   80 },
  { OV(461),   90 },
  { OV(473),  100 },
  { OV(485),  110 },
  { OV(498),  120 },
  { OV(511),  130 },
  { OV(523),  140 },
  { OV(535),  150 },
  { OV(547),  160 },
  { OV(559),  170 },
  { OV(571),  180 },
  { OV(583),  190 },
  { OV(595),  200 },
  { OV(608),  210 },
  { OV(620),  220 },
  { OV(632),  230 },
  { OV(642),  240 },
  { OV(655),  250 },
  { OV(667),  260 },
  { OV(677),  270 },
  { OV(689),  280 },
  { OV(702),  290 },
  { OV(712),  300 },
  { OV(724),  310 },
  { OV(735),  320 },
  { OV(747),  330 },
  { OV(758),  340 },
  { OV(768),  350 },
  { OV(780),  360 },
  { OV(791),  370 },
  { OV(802),  380 },
  { OV(814),  390 },
  { OV(824),  400 },
  { OV(930),  500 }
};
