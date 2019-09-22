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

// 100k thermistor supplied with RPW-Ultra hotend, 4.7k pullup

const short temptable_512[][2] PROGMEM = {
  { OV(26),  300 },
  { OV(28),  295 },
  { OV(30),  290 },
  { OV(32),  285 },
  { OV(34),  280 },
  { OV(37),  275 },
  { OV(39),  270 },
  { OV(42),  265 },
  { OV(46),  260 },
  { OV(49),  255 },
  { OV(53),  250 }, // 256.5
  { OV(57),  245 },
  { OV(62),  240 },
  { OV(67),  235 },
  { OV(73),  230 },
  { OV(79),  225 },
  { OV(86),  220 },
  { OV(94),  215 },
  { OV(103), 210 },
  { OV(112), 205 },
  { OV(123), 200 },
  { OV(135), 195 },
  { OV(148), 190 },
  { OV(162), 185 },
  { OV(178), 180 },
  { OV(195), 175 },
  { OV(215), 170 },
  { OV(235), 165 },
  { OV(258), 160 },
  { OV(283), 155 },
  { OV(310), 150 }, // 2040.6
  { OV(338), 145 },
  { OV(369), 140 },
  { OV(401), 135 },
  { OV(435), 130 },
  { OV(470), 125 },
  { OV(505), 120 },
  { OV(542), 115 },
  { OV(579), 110 },
  { OV(615), 105 },
  { OV(651), 100 },
  { OV(686),  95 },
  { OV(720),  90 },
  { OV(751),  85 },
  { OV(781),  80 },
  { OV(809),  75 },
  { OV(835),  70 },
  { OV(858),  65 },
  { OV(880),  60 },
  { OV(899),  55 },
  { OV(915),  50 },
  { OV(930),  45 },
  { OV(944),  40 },
  { OV(955),  35 },
  { OV(965),  30 }, // 78279.3
  { OV(974),  25 },
  { OV(981),  20 },
  { OV(988),  15 },
  { OV(993),  10 },
  { OV(998),   5 },
  { OV(1002),  0 },
};
