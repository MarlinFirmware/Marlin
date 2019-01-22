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

// 100k bed thermistor (Thermistor table 1 but -20 Deg)
const short temptable_99[][2] PROGMEM = {
  
{  174 ,  300 },
{  186 ,  295 },
{  199 ,  290 },
{  214 ,  285 },
{  230 ,  280 },
{  247 ,  275 },
{  266 ,  270 },
{  287 ,  265 },
{  309 ,  260 },
{  334 ,  255 },
{  362 ,  250 },
{  392 ,  245 },
{  426 ,  240 },
{  462 ,  235 },
{  503 ,  230 },
{  548 ,  225 },
{  598 ,  220 },
{  653 ,  215 },
{  714 ,  210 },
{  782 ,  205 },
{  858 ,  200 },
{  942 ,  195 },
{  1036 ,  190 },
{  1141 ,  185 },
{  1258 ,  180 },
{  1388 ,  175 },
{  1534 ,  170 },
{  1697 ,  165 },
{  1879 ,  160 },
{  2082 ,  155 },
{  2308 ,  150 },
{  2561 ,  145 },
{  2842 ,  140 },
{  3153 ,  135 },
{  3498 ,  130 },
{  3878 ,  125 },
{  4296 ,  120 },
{  4751 ,  115 },
{  5246 ,  110 },
{  5780 ,  105 },
{  6350 ,  100 },
{  6954 ,  95 },
{  7587 ,  90 },
{  8244 ,  85 },
{  8915 ,  80 },
{  9594 ,  75 },
{  10270 ,  70 },
{  10933 ,  65 },
{  11573 ,  60 },
{  12182 ,  55 },
{  12752 ,  50 },
{  13277 ,  45 },
{  13753 ,  40 },
{  14179 ,  35 },
{  14554 ,  30 },
{  14880 ,  25 },
{  15159 ,  20 },
{  15396 ,  15 },
{  15593 ,  10 },
{  15757 ,  5 },
{  15890 ,  0 },


};

