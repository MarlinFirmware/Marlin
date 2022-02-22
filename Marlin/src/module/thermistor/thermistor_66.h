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

// R25 = 2.5 MOhm, beta25 = 4500 K, 4.7 kOhm pull-up, DyzeDesign 500 °C Thermistor
constexpr temp_entry_t temptable_66[] PROGMEM = {
  { OV(  17.5), 850 },
  { OV(  17.9), 500 },
  { OV(  21.7), 480 },
  { OV(  26.6), 460 },
  { OV(  34.0), 430 },
  { OV(  36.0), 426 },
  { OV(  37.0), 422 },
  { OV(  38.0), 418 },
  { OV(  40.4), 414 },
  { OV(  43.0), 410 },
  { OV(  45.6), 406 },
  { OV(  48.0), 402 },
  { OV(  50.6), 398 },
  { OV(  53.0), 394 },
  { OV(  56.0), 390 },
  { OV(  58.0), 386 },
  { OV(  61.0), 382 },
  { OV(  64.0), 378 },
  { OV(  68.0), 374 },
  { OV(  72.0), 370 },
  { OV(  75.0), 366 },
  { OV(  79.0), 362 },
  { OV(  83.0), 358 },
  { OV(  88.0), 354 },
  { OV(  93.0), 350 },
  { OV(  97.0), 346 },
  { OV( 103.0), 342 },
  { OV( 109.0), 338 },
  { OV( 115.0), 334 },
  { OV( 121.0), 330 },
  { OV( 128.0), 326 },
  { OV( 135.0), 322 },
  { OV( 143.0), 318 },
  { OV( 151.0), 314 },
  { OV( 160.0), 310 },
  { OV( 168.0), 306 },
  { OV( 177.0), 302 },
  { OV( 188.0), 298 },
  { OV( 198.0), 294 },
  { OV( 209.0), 290 },
  { OV( 222.0), 286 },
  { OV( 235.0), 282 },
  { OV( 248.0), 278 },
  { OV( 262.0), 274 },
  { OV( 276.0), 270 },
  { OV( 291.0), 266 },
  { OV( 306.0), 262 },
  { OV( 323.0), 258 },
  { OV( 340.0), 254 },
  { OV( 357.0), 250 },
  { OV( 378.0), 246 },
  { OV( 397.0), 242 },
  { OV( 417.0), 238 },
  { OV( 437.0), 234 },
  { OV( 458.0), 230 },
  { OV( 481.0), 226 },
  { OV( 502.0), 222 },
  { OV( 525.0), 218 },
  { OV( 547.0), 214 },
  { OV( 570.0), 210 },
  { OV( 594.0), 206 },
  { OV( 615.0), 202 },
  { OV( 637.0), 198 },
  { OV( 660.0), 194 },
  { OV( 683.0), 190 },
  { OV( 705.0), 186 },
  { OV( 727.0), 182 },
  { OV( 747.0), 178 },
  { OV( 767.0), 174 },
  { OV( 787.0), 170 },
  { OV( 805.0), 166 },
  { OV( 822.0), 162 },
  { OV( 839.0), 158 },
  { OV( 854.0), 154 },
  { OV( 870.0), 150 },
  { OV( 883.0), 146 },
  { OV( 898.0), 142 },
  { OV( 909.0), 138 },
  { OV( 919.0), 134 },
  { OV( 931.0), 130 },
  { OV( 940.0), 126 },
  { OV( 949.0), 122 },
  { OV( 957.0), 118 },
  { OV( 964.0), 114 },
  { OV( 971.0), 110 },
  { OV( 977.0), 106 },
  { OV( 982.0), 102 },
  { OV( 997.0),  93 },
  { OV(1002.2),  86 },
  { OV(1006.6),  80 },
  { OV(1015.8),  60 },
  { OV(1019.8),  36 },
  { OV(1020.9),  23 },
  { OV(1022.0),  -1 }
};
