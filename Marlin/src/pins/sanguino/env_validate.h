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

#if ENABLED(ALLOW_MEGA644)
  #if NOT_TARGET(__AVR_ATmega644__, __AVR_ATmega644P__, __AVR_ATmega1284P__)
    #error "Oops! Select 'Sanguino' in 'Tools > Boards' and 'ATmega644', 'ATmega644P', or 'ATmega1284P' in 'Tools > Processor.'"
  #endif
#elif ENABLED(ALLOW_MEGA644P)
  #if NOT_TARGET(__AVR_ATmega644P__, __AVR_ATmega1284P__)
    #error "Oops! Select 'Sanguino' in 'Tools > Boards' and 'ATmega644P' or 'ATmega1284P' in 'Tools > Processor.'"
  #endif
#elif ENABLED(REQUIRE_MEGA644P)
  #if NOT_TARGET(__AVR_ATmega644P__)
    #error "Oops! Select 'Sanguino' in 'Tools > Board' and 'ATmega644P' in 'Tools > Processor.'"
  #endif
#elif NOT_TARGET(__AVR_ATmega1284P__)
  #error "Oops! Select 'Sanguino' in 'Tools > Board' and 'ATmega1284P' in 'Tools > Processor.' (For PlatformIO, use 'melzi' or 'melzi_optiboot.')"
#endif

#undef ALLOW_MEGA644
#undef ALLOW_MEGA644P
#undef REQUIRE_MEGA644P

#endif
