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

// Unknown thermistor in 600W silicone heater pad
// Ordered from https://www.aliexpress.com/item/32859679498.html

// Values were determined by taping two type-5 thermistors to the printer bed:
// a 12x12x1/4" slab of cast-aluminum tooling plate with the aforementioned
// heater stuck underneath.  The thermistors were stuck to the side of the 
// bed with some kapton tape.  One was plugged into an SKR Pro 1.1 running
// Marlin.  The other type-5 thermistor and the unknown thermistor in the
// bed heater were connected to an Arduino Nano, programmed to write the temperature
// and raw ADC readings to its serial port once per second.  Readings were
// logged with Minicom while the bed temperature was adjusted and allowed
// to settle.
//
// All but the last two (0 and 10 degrees) were determined this way.  As
// a result, temperatures below 25 may be inaccurate...but the coldest I
// run my printer is 60 for PLA, so that shouldn't be an issue for this
// application.

const short temptable_76[][2] PROGMEM = {
  { OV( 634), 120 },
  { OV( 676), 115 },
  { OV( 717), 110 },
  { OV( 738), 105 },
  { OV( 796), 100 },
  { OV( 832),  95 },
  { OV( 845),  90 },
  { OV( 849),  85 },
  { OV( 867),  80 },
  { OV( 881),  75 },
  { OV( 906),  70 },
  { OV( 915),  65 },
  { OV( 926),  60 },
  { OV( 938),  55 },
  { OV( 947),  50 },
  { OV( 954),  45 },
  { OV( 961),  40 },
  { OV( 967),  35 },
  { OV( 974),  30 },
  { OV( 979),  25 },
  { OV(1000),  10 },
  { OV(1010),   0 }
};
