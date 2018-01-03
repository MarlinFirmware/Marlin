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
#ifndef TMC_MACROS_H
#define TMC_MACROS_H

  // Trinamic Stepper Drivers
  #define HAS_TRINAMIC (ENABLED(HAVE_TMC2130) || ENABLED(HAVE_TMC2208) || ENABLED(IS_TRAMS))
  #define  X_IS_TRINAMIC (ENABLED( X_IS_TMC2130) || ENABLED( X_IS_TMC2208) || ENABLED(IS_TRAMS))
  #define X2_IS_TRINAMIC (ENABLED(X2_IS_TMC2130) || ENABLED(X2_IS_TMC2208))
  #define  Y_IS_TRINAMIC (ENABLED( Y_IS_TMC2130) || ENABLED( Y_IS_TMC2208) || ENABLED(IS_TRAMS))
  #define Y2_IS_TRINAMIC (ENABLED(Y2_IS_TMC2130) || ENABLED(Y2_IS_TMC2208))
  #define  Z_IS_TRINAMIC (ENABLED( Z_IS_TMC2130) || ENABLED( Z_IS_TMC2208) || ENABLED(IS_TRAMS))
  #define Z2_IS_TRINAMIC (ENABLED(Z2_IS_TMC2130) || ENABLED(Z2_IS_TMC2208))
  #define E0_IS_TRINAMIC (ENABLED(E0_IS_TMC2130) || ENABLED(E0_IS_TMC2208) || ENABLED(IS_TRAMS))
  #define E1_IS_TRINAMIC (ENABLED(E1_IS_TMC2130) || ENABLED(E1_IS_TMC2208))
  #define E2_IS_TRINAMIC (ENABLED(E2_IS_TMC2130) || ENABLED(E2_IS_TMC2208))
  #define E3_IS_TRINAMIC (ENABLED(E3_IS_TMC2130) || ENABLED(E3_IS_TMC2208))
  #define E4_IS_TRINAMIC (ENABLED(E4_IS_TMC2130) || ENABLED(E4_IS_TMC2208))

#endif
