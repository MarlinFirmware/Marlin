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

// TDK NTCG104LH104JT1 with 4K7 pull-up resistor
// Source: https://product.tdk.com/info/en/catalog/datasheets/503021/tpd_commercial_ntc-thermistor_ntcg_en.pdf
const short temptable_104[][2] PROGMEM = {
  { OV(  313), 125 },
  { OV(  347), 120 },
  { OV(  383), 115 },
  { OV(  422), 110 },
  { OV(  463), 105 },
  { OV(  506), 100 },
  { OV(  549),  95 },
  { OV(  594),  90 },
  { OV(  638),  85 },
  { OV(  681),  80 },
  { OV(  722),  75 },
  { OV(  762),  70 },
  { OV(  799),  65 },
  { OV(  833),  60 },
  { OV(  863),  55 },
  { OV(  890),  50 },
  { OV(  914),  45 },
  { OV(  934),  40 },
  { OV(  951),  35 },
  { OV(  966),  30 },
  { OV(  978),  25 },
  { OV(  988),  20 },
  { OV(  996),  15 },
  { OV( 1002),  10 },
  { OV( 1007),   5 } ,
  { OV( 1012),   0 } ,
  { OV( 1015),  -5 },
  { OV( 1017), -10 },
  { OV( 1019), -15 },
  { OV( 1020), -20 },
  { OV( 1021), -25 },
  { OV( 1022), -30 },
  { OV( 1023), -35 },
  { OV( 1023), -40 },
};