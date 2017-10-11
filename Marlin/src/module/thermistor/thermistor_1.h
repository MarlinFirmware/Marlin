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

// 100k bed thermistor
const short temptable_1[][2] PROGMEM = {
  { OV(  5.50), 350 }, // v=0.017     r=54.019        res=13.335 degC/count
  { OV(  6.23), 340 }, // v=0.019     r=61.232        res=11.568 degC/count
  { OV(  7.09), 330 }, // v=0.022     r=69.676        res=9.987 degC/count
  { OV(  8.09), 320 }, // v=0.025     r=79.602        res=8.578 degC/count
  { OV(  9.27), 310 }, // v=0.029     r=91.325        res=7.330 degC/count
  { OV( 10.66), 300 }, // v=0.033     r=105.233       res=6.231 degC/count
  { OV( 12.32), 290 }, // v=0.038     r=121.816       res=5.268 degC/count
  { OV( 14.31), 280 }, // v=0.044     r=141.689       res=4.431 degC/count
  { OV( 16.68), 270 }, // v=0.052     r=165.633       res=3.707 degC/count
  { OV( 19.55), 260 }, // v=0.061     r=194.644       res=3.084 degC/count
  { OV( 23.02), 250 }, // v=0.071     r=230.000       res=2.553 degC/count
  { OV( 27.25), 240 }, // v=0.084     r=273.353       res=2.101 degC/count
  { OV( 32.41), 230 }, // v=0.100     r=326.853       res=1.721 degC/count
  { OV( 38.75), 220 }, // v=0.120     r=393.320       res=1.403 degC/count
  { OV( 46.57), 210 }, // v=0.144     r=476.481       res=1.138 degC/count
  { OV( 56.26), 200 }, // v=0.174     r=581.300       res=0.920 degC/count
  { OV( 68.28), 190 }, // v=0.212     r=714.447       res=0.741 degC/count
  { OV( 83.25), 180 }, // v=0.258     r=884.961       res=0.596 degC/count
  { OV(101.91), 170 }, // v=0.316     r=1105.209      res=0.479 degC/count
  { OV(125.15), 160 }, // v=0.388     r=1392.275      res=0.385 degC/count
  { OV(153.99), 150 }, // v=0.477     r=1770.000      res=0.311 degC/count
  { OV(189.58), 140 }, // v=0.588     r=2272.018      res=0.254 degC/count
  { OV(233.04), 130 }, // v=0.723     r=2946.313      res=0.209 degC/count
  { OV(285.30), 120 }, // v=0.885     r=3862.164      res=0.176 degC/count
  { OV(346.79), 110 }, // v=1.075     r=5120.859      res=0.151 degC/count
  { OV(417.09), 100 }, // v=1.293     r=6872.449      res=0.135 degC/count
  { OV(494.59),  90 }, // v=1.534     r=9342.364      res=0.125 degC/count
  { OV(576.34),  80 }, // v=1.787     r=12874.313     res=0.121 degC/count
  { OV(658.29),  70 }, // v=2.041     r=18000.621     res=0.124 degC/count
  { OV(736.03),  60 }, // v=2.282     r=25559.526     res=0.135 degC/count
  { OV(805.64),  50 }, // v=2.498     r=36894.461     res=0.155 degC/count
  { OV(864.50),  40 }, // v=2.680     r=54199.273     res=0.189 degC/count
  { OV(911.63),  30 }, // v=2.827     r=81128.828     res=0.243 degC/count
  { OV(947.53),  20 }, // v=2.938     r=123903.203    res=0.327 degC/count
  { OV(973.64),  10 }, // v=3.019     r=193352.221    res=0.462 degC/count
  { OV(991.88),   0 }  // v=3.075     r=308798.156    res=0.678 degC/count
};
