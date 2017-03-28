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

// 100k EPCOS (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee.
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: Twice the resolution and better linearity from 150C to 200C
const short temptable_51[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 350 },
  {  190 * OVERSAMPLENR, 250 }, // top rating 250C
  {  203 * OVERSAMPLENR, 245 },
  {  217 * OVERSAMPLENR, 240 },
  {  232 * OVERSAMPLENR, 235 },
  {  248 * OVERSAMPLENR, 230 },
  {  265 * OVERSAMPLENR, 225 },
  {  283 * OVERSAMPLENR, 220 },
  {  302 * OVERSAMPLENR, 215 },
  {  322 * OVERSAMPLENR, 210 },
  {  344 * OVERSAMPLENR, 205 },
  {  366 * OVERSAMPLENR, 200 },
  {  390 * OVERSAMPLENR, 195 },
  {  415 * OVERSAMPLENR, 190 },
  {  440 * OVERSAMPLENR, 185 },
  {  467 * OVERSAMPLENR, 180 },
  {  494 * OVERSAMPLENR, 175 },
  {  522 * OVERSAMPLENR, 170 },
  {  551 * OVERSAMPLENR, 165 },
  {  580 * OVERSAMPLENR, 160 },
  {  609 * OVERSAMPLENR, 155 },
  {  638 * OVERSAMPLENR, 150 },
  {  666 * OVERSAMPLENR, 145 },
  {  695 * OVERSAMPLENR, 140 },
  {  722 * OVERSAMPLENR, 135 },
  {  749 * OVERSAMPLENR, 130 },
  {  775 * OVERSAMPLENR, 125 },
  {  800 * OVERSAMPLENR, 120 },
  {  823 * OVERSAMPLENR, 115 },
  {  845 * OVERSAMPLENR, 110 },
  {  865 * OVERSAMPLENR, 105 },
  {  884 * OVERSAMPLENR, 100 },
  {  901 * OVERSAMPLENR,  95 },
  {  917 * OVERSAMPLENR,  90 },
  {  932 * OVERSAMPLENR,  85 },
  {  944 * OVERSAMPLENR,  80 },
  {  956 * OVERSAMPLENR,  75 },
  {  966 * OVERSAMPLENR,  70 },
  {  975 * OVERSAMPLENR,  65 },
  {  982 * OVERSAMPLENR,  60 },
  {  989 * OVERSAMPLENR,  55 },
  {  995 * OVERSAMPLENR,  50 },
  { 1000 * OVERSAMPLENR,  45 },
  { 1004 * OVERSAMPLENR,  40 },
  { 1007 * OVERSAMPLENR,  35 },
  { 1010 * OVERSAMPLENR,  30 },
  { 1013 * OVERSAMPLENR,  25 },
  { 1015 * OVERSAMPLENR,  20 },
  { 1017 * OVERSAMPLENR,  15 },
  { 1018 * OVERSAMPLENR,  10 },
  { 1019 * OVERSAMPLENR,   5 },
  { 1020 * OVERSAMPLENR,   0 },
  { 1021 * OVERSAMPLENR,  -5 }
};
