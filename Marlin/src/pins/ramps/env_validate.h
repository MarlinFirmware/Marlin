/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#ifndef ENV_VALIDATE_H
#define ENV_VALIDATE_H

#if ENABLED(ALLOW_SAM3X8E)
  #if NOT_TARGET(__SAM3X8E__, __AVR_ATmega2560__)
    #error "Oops! Select 'Arduino Due' or 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
  #endif
#elif ENABLED(REQUIRE_MEGA2560) && NOT_TARGET(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#elif DISABLED(REQUIRE_MEGA2560) && NOT_TARGET(__AVR_ATmega1280__, __AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560 or 1280' in 'Tools > Board.'"
#endif

#undef ALLOW_SAM3X8E
#undef REQUIRE_MEGA2560

#endif
