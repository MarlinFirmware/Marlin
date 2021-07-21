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

// Zonestar (Z8XM2) Heated Bed thermistor. Added By AvanOsch
// These are taken from the Zonestar settings in original Repetier firmware: Z8XM2_ZRIB_LCD12864_V51.zip
constexpr temp_entry_t temptable_503[] PROGMEM = {
   { OV(  12), 300 },
   { OV(  27), 270 },
   { OV(  47), 250 },
   { OV(  68), 230 },
   { OV(  99), 210 },
   { OV( 120), 200 },
   { OV( 141), 190 },
   { OV( 171), 180 },
   { OV( 201), 170 },
   { OV( 261), 160 },
   { OV( 321), 150 },
   { OV( 401), 140 },
   { OV( 451), 130 },
   { OV( 551), 120 },
   { OV( 596), 110 },
   { OV( 626), 105 },
   { OV( 666), 100 },
   { OV( 697), 90 },
   { OV( 717), 85 },
   { OV( 798), 69 },
   { OV( 819), 65 },
   { OV( 870), 55 },
   { OV( 891), 51 },
   { OV( 922), 39 },
   { OV( 968), 28 },
   { OV( 980), 23 },
   { OV( 991), 17 },
   { OV( 1001), 9 },
   { OV(1021), -27 },
   { OV(1023), -200}
};
