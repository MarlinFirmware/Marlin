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

// Generic Silicon Heat Pad with NTC 100K thermistor ( Beta 25/50 3950K)
//
// Many of the generic silicon heat pads use the MGB18-104F39050L32 Thermistor   It is used for various
// wattage and voltage heat pads.  This table is correct if this part is used.   It has been
// optimized to provide good granularity around the 60 C. and 110 C. which corrisponds to bed temperatures
// for PLA and ABS.  If you are printing higher temperature filament such as nylon you can uncomment
// the higher earlier entries in the table to give better accuracy.  But for speed reasons, if these
// temperatures are not going to be used, it is better to leave them commented out.

const short temptable_75[][2] PROGMEM = { // Generic Silicon Heat Pad with NTC 100K MGB18-104F39050L32 thermistor
    { (short) ( 111.06 * OVERSAMPLENR ),  200 }, // v=0.542 r=571.747 res=0.501 degC/count
//  { (short) ( 174.87 * OVERSAMPLENR ),  175 }, // v=0.854 r=967.950 res=0.311 degC/count  These values are valid.  But they serve no
//  { (short) ( 191.64 * OVERSAMPLENR ),  170 }, // v=0.936 r=1082.139 res=0.284 degC/count  purpose.  It is better to delete them so
//  { (short) ( 209.99 * OVERSAMPLENR ),  165 }, // v=1.025 r=1212.472 res=0.260 degC/count  the search is quicker and get to the meaningful
//  { (short) ( 230.02 * OVERSAMPLENR ),  160 }, // v=1.123 r=1361.590 res=0.239 degC/count  part of the table sooner.
//  { (short) ( 251.80 * OVERSAMPLENR ),  155 }, // v=1.230 r=1532.621 res=0.220 degC/count
    { (short) ( 275.43 * OVERSAMPLENR ),  150 }, // v=1.345 r=1729.283 res=0.203 degC/count
//  { (short) ( 300.92 * OVERSAMPLENR ),  145 }, // v=1.469 r=1956.004 res=0.189 degC/coun
    { (short) ( 328.32 * OVERSAMPLENR ),  140 }, // v=1.603 r=2218.081 res=0.176 degC/count
    { (short) ( 388.65 * OVERSAMPLENR ),  130 }, // v=1.898 r=2874.980 res=0.156 degC/count
    { (short) ( 421.39 * OVERSAMPLENR ),  125 }, // v=2.058 r=3286.644 res=0.149 degC/count
    { (short) ( 455.65 * OVERSAMPLENR ),  120 }, // v=2.225 r=3768.002 res=0.143 degC/count
    { (short) ( 491.17 * OVERSAMPLENR ),  115 }, // v=2.398 r=4332.590 res=0.139 degC/count
    { (short) ( 527.68 * OVERSAMPLENR ),  110 }, // v=2.577 r=4996.905 res=0.136 degC/count
    { (short) ( 564.81 * OVERSAMPLENR ),  105 }, // v=2.758 r=5781.120 res=0.134 degC/count
    { (short) ( 602.19 * OVERSAMPLENR ),  100 }, // v=2.940 r=6710.000 res=0.134 degC/count
    { (short) ( 676.03 * OVERSAMPLENR ),   90 }, // v=3.301 r=9131.018 res=0.138 degC/count
    { (short) ( 745.85 * OVERSAMPLENR ),   80 }, // v=3.642 r=12602.693 res=0.150 degC/count
    { (short) ( 778.31 * OVERSAMPLENR ),   75 }, // v=3.800 r=14889.001 res=0.159 degC/count
    { (short) ( 808.75 * OVERSAMPLENR ),   70 }, // v=3.949 r=17658.700 res=0.171 degC/count
    { (short) ( 836.94 * OVERSAMPLENR ),   65 }, // v=4.087 r=21028.040 res=0.185 degC/count
    { (short) ( 862.74 * OVERSAMPLENR ),   60 }, // v=4.213 r=25144.568 res=0.204 degC/count
    { (short) ( 886.08 * OVERSAMPLENR ),   55 }, // v=4.327 r=30196.449 res=0.227 degC/count
    { (short) ( 906.97 * OVERSAMPLENR ),   50 }, // v=4.429 r=36424.838 res=0.255 degC/count
    { (short) ( 941.65 * OVERSAMPLENR ),   40 }, // v=4.598 r=53745.337 res=0.333 degC/count
    { (short) ( 967.76 * OVERSAMPLENR ),   30 }, // v=4.725 r=80880.630 res=0.452 degC/count
    { (short) ( 978.03 * OVERSAMPLENR ),   25 }, // v=4.776 r=100000.000 res=0.535 degC/count
    { (short) ( 981.68 * OVERSAMPLENR ),   23 }, // v=4.793 r=109024.395 res=0.573 degC/count
    { (short) ( 983.41 * OVERSAMPLENR ),   22 }, // v=4.802 r=113875.430 res=0.594 degC/count
    { (short) ( 985.08 * OVERSAMPLENR ),   21 }, // v=4.810 r=118968.955 res=0.616 degC/count
    { (short) ( 986.70 * OVERSAMPLENR ),   20 }, // v=4.818 r=124318.354 res=0.638 degC/count
    { (short) ( 993.94 * OVERSAMPLENR ),   15 }, // v=4.853 r=155431.302 res=0.768 degC/count
    { (short) ( 999.96 * OVERSAMPLENR ),   10 }, // v=4.883 r=195480.023 res=0.934 degC/count
    { (short) (1008.95 * OVERSAMPLENR ),    0 }  // v=4.926 r=314997.575 res=1.418 degC/count
};


