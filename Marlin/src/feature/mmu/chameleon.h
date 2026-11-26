/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * feature/mmu/chameleon.h - 3D Chameleon Mk4 filament changer
 *
 * The Chameleon has a single button. Its firmware measures how long the button
 * is held (one "pulse" per half second, with vibration feedback) to select a
 * command, then uses up to two more presses to unload and load filament
 * (about one inch per second while held). Marlin presses the button by
 * moving an axis into it.
 */

#include <stdint.h>

#define CHAMELEON_NO_TOOL -1

class Chameleon {
  public:
    static int8_t extruder;   // The filament the Chameleon has loaded (CHAMELEON_NO_TOOL if none or unknown)

    static void tool_change(const uint8_t new_tool);

  private:
    static void button(const bool pressed);
    static void hold(const uint16_t ms);
    static void pulses(const uint8_t count);
    static void go_standby();
};

extern Chameleon chameleon;
