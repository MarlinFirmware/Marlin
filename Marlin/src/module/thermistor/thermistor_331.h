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

// R25 = 100 kOhm, beta25 = 4092 K, 4.7 kOhm pull-up, bed thermistor
const short temptable_331[][2] PROGMEM = {
  { OV(  23*0.654), 300 },
  { OV(  25*0.654), 295 },
  { OV(  27*0.654), 290 },
  { OV(  28*0.654), 285 },
  { OV(  31*0.654), 280 },
  { OV(  33*0.654), 275 },
  { OV(  35*0.654), 270 },
  { OV(  38*0.654), 265 },
  { OV(  41*0.654), 260 },
  { OV(  44*0.654), 255 },
  { OV(  48*0.654), 250 },
  { OV(  52*0.654), 245 },
  { OV(  56*0.654), 240 },
  { OV(  61*0.654), 235 },
  { OV(  66*0.654), 230 },
  { OV(  71*0.654), 225 },
  { OV(  78*0.654), 220 },
  { OV(  84*0.654), 215 },
  { OV(  92*0.654), 210 },
  { OV( 100*0.654), 205 },
  { OV( 109*0.654), 200 },
  { OV( 120*0.654), 195 },
  { OV( 131*0.654), 190 },
  { OV( 143*0.654), 185 },
  { OV( 156*0.654), 180 },
  { OV( 171*0.654), 175 },
  { OV( 187*0.654), 170 },
  { OV( 205*0.654), 165 },
  { OV( 224*0.654), 160 },
  { OV( 245*0.654), 155 },
  { OV( 268*0.654), 150 },
  { OV( 293*0.654), 145 },
  { OV( 320*0.654), 140 },
  { OV( 348*0.654), 135 },
  { OV( 379*0.654), 130 },
  { OV( 411*0.654), 125 },
  { OV( 445*0.654), 120 },
  { OV( 480*0.654), 115 },
  { OV( 516*0.654), 110 },
  { OV( 553*0.654), 105 },
  { OV( 591*0.654), 100 },
  { OV( 628*0.654),  95 },
  { OV( 665*0.654),  90 },
  { OV( 702*0.654),  85 },
  { OV( 737*0.654),  80 },
  { OV( 770*0.654),  75 },
  { OV( 801*0.654),  70 },
  { OV( 830*0.654),  65 },
  { OV( 857*0.654),  60 },
  { OV( 881*0.654),  55 },
  { OV( 903*0.654),  50 },
  { OV( 922*0.654),  45 },
  { OV( 939*0.654),  40 },
  { OV( 954*0.654),  35 },
  { OV( 966*0.654),  30 },
  { OV( 977*0.654),  25 },
  { OV( 985*0.654),  20 },
  { OV( 993*0.654),  15 },
  { OV( 999*0.654),  10 },
  { OV(1004*0.654),   5 },
  { OV(1008*0.654),   0 },
  { OV(1012*0.654),  -5 },
  { OV(1016*0.654), -10 },
  { OV(1020*0.654), -15 }
};
