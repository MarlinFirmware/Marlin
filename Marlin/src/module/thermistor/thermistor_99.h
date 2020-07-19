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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

// 100k bed thermistor with a 10K pull-up resistor - made by $ buildroot/share/scripts/createTemperatureLookupMarlin.py --rp=10000

const temp_entry_t temptable_99[] PROGMEM = {
  { OV(  5.81), 350 }, // v=0.028   r=    57.081  res=13.433 degC/count
  { OV(  6.54), 340 }, // v=0.032   r=    64.248  res=11.711 degC/count
  { OV(  7.38), 330 }, // v=0.036   r=    72.588  res=10.161 degC/count
  { OV(  8.36), 320 }, // v=0.041   r=    82.336  res= 8.772 degC/count
  { OV(  9.51), 310 }, // v=0.046   r=    93.780  res= 7.535 degC/count
  { OV( 10.87), 300 }, // v=0.053   r=   107.281  res= 6.439 degC/count
  { OV( 12.47), 290 }, // v=0.061   r=   123.286  res= 5.473 degC/count
  { OV( 14.37), 280 }, // v=0.070   r=   142.360  res= 4.627 degC/count
  { OV( 16.64), 270 }, // v=0.081   r=   165.215  res= 3.891 degC/count
  { OV( 19.37), 260 }, // v=0.095   r=   192.758  res= 3.253 degC/count
  { OV( 22.65), 250 }, // v=0.111   r=   226.150  res= 2.705 degC/count
  { OV( 26.62), 240 }, // v=0.130   r=   266.891  res= 2.236 degC/count
  { OV( 31.46), 230 }, // v=0.154   r=   316.931  res= 1.839 degC/count
  { OV( 37.38), 220 }, // v=0.182   r=   378.822  res= 1.504 degC/count
  { OV( 44.65), 210 }, // v=0.218   r=   455.939  res= 1.224 degC/count
  { OV( 53.64), 200 }, // v=0.262   r=   552.778  res= 0.991 degC/count
  { OV( 64.78), 190 }, // v=0.316   r=   675.386  res= 0.799 degC/count
  { OV( 78.65), 180 }, // v=0.384   r=   831.973  res= 0.643 degC/count
  { OV( 95.94), 170 }, // v=0.468   r=  1033.801  res= 0.516 degC/count
  { OV(117.52), 160 }, // v=0.574   r=  1296.481  res= 0.414 degC/count
  { OV(144.42), 150 }, // v=0.705   r=  1641.900  res= 0.333 degC/count
  { OV(177.80), 140 }, // v=0.868   r=  2101.110  res= 0.269 degC/count
  { OV(218.89), 130 }, // v=1.069   r=  2718.725  res= 0.220 degC/count
  { OV(268.82), 120 }, // v=1.313   r=  3559.702  res= 0.183 degC/count
  { OV(328.35), 110 }, // v=1.603   r=  4719.968  res= 0.155 degC/count
  { OV(397.44), 100 }, // v=1.941   r=  6343.323  res= 0.136 degC/count
  { OV(474.90),  90 }, // v=2.319   r=  8648.807  res= 0.124 degC/count
  { OV(558.03),  80 }, // v=2.725   r= 11975.779  res= 0.118 degC/count
  { OV(642.76),  70 }, // v=3.138   r= 16859.622  res= 0.119 degC/count
  { OV(724.25),  60 }, // v=3.536   r= 24161.472  res= 0.128 degC/count
  { OV(797.93),  50 }, // v=3.896   r= 35295.361  res= 0.146 degC/count
  { OV(860.51),  40 }, // v=4.202   r= 52635.209  res= 0.178 degC/count
  { OV(910.55),  30 }, // v=4.446   r= 80262.251  res= 0.229 degC/count
  { OV(948.36),  20 }, // v=4.631   r=125374.433  res= 0.313 degC/count
  { OV(975.47),  10 }, // v=4.763   r=201020.458  res= 0.449 degC/count
  { OV(994.02),   0 }  // v=4.854   r=331567.870  res= 0.676 degC/count
};
