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

  { OV( 17.5), 850 },
  { OV( 17.9), 500 },
  { OV( 21.7), 480 },
  { OV( 26.6), 460 },
  { OV( 34.0), 430 },
  { OV( 36.0), 426 },
  { OV( 37 ), 422 },
  { OV( 38 ), 418},
  { OV( 40.4), 414 },
  { OV( 43.0), 410 },
  { OV( 45.6), 406 },
  { OV( 48.0), 402 },
  { OV( 50.6), 398 },
  { OV( 53.0), 394 },
  { OV( 56 ), 390 },
  { OV( 58 ), 386 },
  { OV( 61 ), 382 },
  { OV( 64 ), 378 },
  { OV( 68 ), 374 },
  { OV( 72 ), 370 },
  { OV( 75 ), 366 },
  { OV( 79 ), 362 },
  { OV( 83 ), 358 },
  { OV( 88 ), 354 },
  { OV( 93 ), 350 },
  { OV( 97 ), 346 },
  { OV( 103 ), 342 },
  { OV( 109 ), 338 },
  { OV( 115 ), 334 },
  { OV( 121 ), 330 },
  { OV( 128 ), 326 },
  { OV( 135 ), 322 },
  { OV( 143 ), 318 },
  { OV( 151 ), 314 },
  { OV( 160 ), 310 },
  { OV( 168 ), 306 },
  { OV( 177 ), 302 },
  { OV( 188 ), 298 },
  { OV( 198 ), 294 },
  { OV( 209 ), 290 },
  { OV( 222 ), 286 },
  { OV( 235 ), 282 },
  { OV( 248 ), 278 },
  { OV( 262 ), 274 },
  { OV( 276 ), 270 },
  { OV( 291 ), 266 },
  { OV( 306 ), 262 },
  { OV( 323 ), 258 },
  { OV( 340 ), 254 },
  { OV( 357 ), 250 },
  { OV( 378 ), 246 },
  { OV( 397 ), 242 },
  { OV( 417 ), 238 },
  { OV( 437 ), 234 },
  { OV( 458 ), 230 },
  { OV( 481 ), 226 },
  { OV( 502 ), 222 },
  { OV( 525 ), 218 },
  { OV( 547 ), 214 },
  { OV( 570 ), 210 },
  { OV( 594 ), 206 },
  { OV( 615 ), 202 },
  { OV( 637 ), 198 },
  { OV( 660 ), 194 },
  { OV( 683 ), 190 },
  { OV( 705 ), 186 },
  { OV( 727 ), 182 },
  { OV( 747 ), 178 },
  { OV( 767 ), 174 },
  { OV( 787 ), 170 },
  { OV( 805 ), 166 },
  { OV( 822 ), 162 },
  { OV( 839 ), 158 },
  { OV( 854 ), 154 },
  { OV( 870 ), 150 },
  { OV( 883 ), 146 },
  { OV( 898 ), 142 },
  { OV( 909 ), 138 },
  { OV( 919 ), 134 },
  { OV( 931 ), 130 },
  { OV( 940 ), 126 },
  { OV( 949 ), 122 },
  { OV( 957 ), 118 },
  { OV( 964 ), 114 },
  { OV( 971 ), 110 },
  { OV( 977 ), 106 },
  { OV( 982 ), 102 },
  { OV( 997 ), 93 },
  { OV(1002.2), 86 },
  { OV(1006.6), 80 },
  { OV(1015.8), 60 },
  { OV(1019.8), 36 },
  { OV(1020.9), 23 },
  { OV( 1022), -1 }
};
