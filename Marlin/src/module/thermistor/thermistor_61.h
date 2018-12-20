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
{  2 ,  420 }, // Guestimate to ensure we dont lose a reading and drop temps to -50 when over
{  193 ,  350 },
{  205 ,  345 },
{  218 ,  340 },
{  231 ,  335 },
{  246 ,  330 },
{  262 ,  325 },
{  279 ,  320 },
{  298 ,  315 },
{  319 ,  310 },
{  341 ,  305 },
{  365 ,  300 },
{  391 ,  295 },
{  419 ,  290 },
{  450 ,  285 },
{  484 ,  280 },
{  521 ,  275 },
{  562 ,  270 },
{  615 ,  265 },
{  654 ,  260 },
{  707 ,  255 },
{  765 ,  250 },
{  829 ,  245 },
{  899 ,  240 },
{  976 ,  235 },
{  1061 ,  230 },
{  1154 ,  225 },
{  1256 ,  220 },
{  1369 ,  215 },
{  1493 ,  210 },
{  1631 ,  205 },
{  1781 ,  200 },
{  1948 ,  195 },
{  2131 ,  190 },
{  2332 ,  185 },
{  2553 ,  180 },
{  2796 ,  175 },
{  3062 ,  170 },
{  3352 ,  165 },
{  3670 ,  160 },
{  4014 ,  155 },
{  4388 ,  150 },
{  4791 ,  145 },
{  5224 ,  140 },
{  5687 ,  135 },
{  6178 ,  130 },
{  6697 ,  125 },
{  7239 ,  120 },
{  7802 ,  115 },
{  8381 ,  110 },
{  8971 ,  105 },
{  9566 ,  100 },
{  10160 ,  95 },
{  10745 ,  90 },
{  11315 ,  85 },
{  11865 ,  80 },
{  12474 ,  75 },
{  12953 ,  70 },
{  13334 ,  65 },
{  13753 ,  60 },
{  14132 ,  55 },
{  14472 ,  50 },
{  14774 ,  45 },
{  15038 ,  40 },
{  15268 ,  35 },
{  15465 ,  30 },
{  15633 ,  25 },
{  15775 ,  20 },
{  15894 ,  15 },
{  15991 ,  10 },
{  16071 ,  5 },
{  16136 ,  0 },
{  16189 , -5 },
{  16230 , -10 },
{  16264 , -15 },
{  16290 , -20 },
{  16310 , -25 },
{  16325 , -30 },
{  16337 , -35 },
{  16346 , -40 },
{  16352 , -45 },
{  16357 , -50 }
};


