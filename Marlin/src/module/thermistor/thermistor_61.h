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

// R25 = 100 kOhm, beta25 = 3950 K, 4.7 kOhm pull-up,
// Formbot / Vivedino high temp 100k thermistor
// 100KR13950181203
// Generated with https://www.thingiverse.com/thing:103668
// Using table 1 with datasheet values
// Resistance         100k Ohms at 25deg. C
//Resistance Tolerance     + / -1%
//B Value             3950K at 25/50 deg. C
//B Value Tolerance         + / - 1%
const short temptable_61[][2] PROGMEM = {
{  OV(   5) ,  400 },
{  OV(  12) ,  350 },
{  OV(  13) ,  345 },
{  OV(  14) ,  340 },    // Only one of these OV(14) values should be in the table.
{  OV(  14) ,  335 },    // It can't map to two different temperatures.
{  OV(  15) ,  330 },
{  OV(  16) ,  325 },
{  OV(  17) ,  320 },
{  OV(  19) ,  315 },
{  OV(  20) ,  310 },
{  OV(  21) ,  305 },
{  OV(  23) ,  300 },
{  OV(  24) ,  295 },
{  OV(  26) ,  290 },
{  OV(  28) ,  285 },
{  OV(  30) ,  280 },
{  OV(  33) ,  275 },
{  OV(  35) ,  270 },
{  OV(  38) ,  265 },
{  OV(  41) ,  260 },
{  OV(  44) ,  255 },
{  OV(  48) ,  250 },
{  OV(  52) ,  245 },
{  OV(  56) ,  240 },
{  OV(  61) ,  235 },
{  OV(  66) ,  230 },
{  OV(  72) ,  225 },
{  OV(  79) ,  220 },
{  OV(  86) ,  215 },
{  OV(  93) ,  210 },
{  OV( 102) ,  205 },
{  OV( 111) ,  200 },
{  OV( 122) ,  195 },
{  OV( 133) ,  190 },
{  OV( 146) ,  185 },
{  OV( 160) ,  180 },
{  OV( 175) ,  175 },
{  OV( 191) ,  170 },
{  OV( 210) ,  165 },
{  OV( 229) ,  160 },
{  OV( 251) ,  155 },
{  OV( 274) ,  150 },
{  OV( 299) ,  145 },
{  OV( 327) ,  140 },
{  OV( 355) ,  135 },
{  OV( 386) ,  130 },
{  OV( 419) ,  125 },
{  OV( 452) ,  120 },
{  OV( 488) ,  115 },
{  OV( 524) ,  110 },
{  OV( 561) ,  105 },
{  OV( 598) ,  100 },
{  OV( 635) ,   95 },
{  OV( 672) ,   90 },
{  OV( 707) ,   85 },
{  OV( 742) ,   80 },
{  OV( 780) ,   75 },
{  OV( 810) ,   70 },
{  OV( 833) ,   65 },
{  OV( 860) ,   60 },
{  OV( 883) ,   55 },
{  OV( 905) ,   50 },
{  OV( 923) ,   45 },
{  OV( 940) ,   40 },
{  OV( 954) ,   35 },
{  OV( 967) ,   30 },
{  OV( 977) ,   25 },
{  OV( 986) ,   20 },
{  OV( 993) ,   15 },
{  OV( 999) ,   10 },
{  OV(1004) ,    5 },
{  OV(1009) ,    0 },
{  OV(1012) ,   -5 },
{  OV(1014) ,  -10 },
{  OV(1016) ,  -15 },
{  OV(1018) ,  -20 },
{  OV(1019) ,  -25 },
{  OV(1020) ,  -30 },
{  OV(1021) ,  -35 },
{  OV(1022) ,  -40 }
};


