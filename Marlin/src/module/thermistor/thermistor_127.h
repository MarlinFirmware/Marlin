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

// Dariy & Simon table adjusted for stock TEVO Black Widow heater
const short temptable_127[][2] PROGMEM = {
{1 * OVERSAMPLENR, 938},
{23 * OVERSAMPLENR, 300},
{25 * OVERSAMPLENR, 295},
{26 * OVERSAMPLENR, 290},
{27 * OVERSAMPLENR, 285},
{28 * OVERSAMPLENR, 280},
{30 * OVERSAMPLENR, 275},
{32 * OVERSAMPLENR, 270},
{34 * OVERSAMPLENR, 265},
{36 * OVERSAMPLENR, 260},
{38 * OVERSAMPLENR, 255},
{40 * OVERSAMPLENR, 250},
{43 * OVERSAMPLENR, 245},
{46 * OVERSAMPLENR, 240},
{51 * OVERSAMPLENR, 235},
{56 * OVERSAMPLENR, 230},
{61 * OVERSAMPLENR, 225},
{65 * OVERSAMPLENR, 220},
{71 * OVERSAMPLENR, 215},
{80 * OVERSAMPLENR, 210},
{88 * OVERSAMPLENR, 205},
{94 * OVERSAMPLENR, 200},
{104 * OVERSAMPLENR, 195},
{114 * OVERSAMPLENR, 190},
{127 * OVERSAMPLENR, 185},
{139 * OVERSAMPLENR, 180},
{158 * OVERSAMPLENR, 175},
{169 * OVERSAMPLENR, 170},
{183 * OVERSAMPLENR, 165},
{199 * OVERSAMPLENR, 160},
{226 * OVERSAMPLENR, 155},
{247 * OVERSAMPLENR, 150},
{260 * OVERSAMPLENR, 145},
{300 * OVERSAMPLENR, 140},
{323 * OVERSAMPLENR, 135},
{354 * OVERSAMPLENR, 130},
{392 * OVERSAMPLENR, 125},
{418 * OVERSAMPLENR, 120},
{452 * OVERSAMPLENR, 115},
{480 * OVERSAMPLENR, 110},
{513 * OVERSAMPLENR, 105},
{546 * OVERSAMPLENR, 100},
{650 * OVERSAMPLENR, 95},
{669 * OVERSAMPLENR, 90},
{711 * OVERSAMPLENR, 85},
{751 * OVERSAMPLENR, 79},
{791 * OVERSAMPLENR, 72},
{811 * OVERSAMPLENR, 69},
{831 * OVERSAMPLENR, 65},
{835 * OVERSAMPLENR, 60},
{881 * OVERSAMPLENR, 55},
{901 * OVERSAMPLENR, 51},
{921 * OVERSAMPLENR, 45},
{941 * OVERSAMPLENR, 39},
{971 * OVERSAMPLENR, 28},
{981 * OVERSAMPLENR, 23},
{991 * OVERSAMPLENR, 17},
{1001 * OVERSAMPLENR, 9},
{1021 * OVERSAMPLENR, -27}
};
