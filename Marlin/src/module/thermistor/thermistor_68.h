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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#define REVERSE_TEMP_SENSOR_RANGE_68 1

//PT100 amplifier board from Dyze Design
const temp_entry_t temptable_68[] PROGMEM = {
  { OV( 186), 0 },
  { OV(200.2), 20 },
  { OV(214.3), 40 },
  { OV(228.2), 60 },
  { OV(242.1), 80 },
  { OV(255.8), 100 },
  { OV(269.4), 120 },
  { OV(282.9), 140 },
  { OV(296.2), 160 },
  { OV(309.5), 180 },
  { OV(322.6), 200 },
  { OV(335.6), 220 },
  { OV(348.5), 240 },
  { OV(361.3), 260 },
  { OV(  374), 280 },
  { OV(386.5), 300 },
  { OV(  399), 320 },
  { OV(411.2), 340 },
  { OV(423.4), 360 },
  { OV(435.5), 380 },
  { OV(447.5), 400 },
  { OV(459.4), 420 },
  { OV(471.2), 440 },
  { OV(482.9), 460 },
  { OV(494.4), 480 },
  { OV(505.9), 500 },
  { OV(517.2), 520 },
  { OV(528.5), 540 },
  { OV(539.6), 560 },
  { OV(550.6), 580 },
  { OV(561.6), 600 }
};