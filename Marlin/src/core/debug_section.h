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

#include "serial.h"
#include "../module/motion.h"

class SectionLog {
public:
  SectionLog(PGM_P const msg=nullptr, bool inbug=true) {
    the_msg = msg;
    if ((debug = inbug)) echo_msg(PSTR(">>>"));
  }

  ~SectionLog() { if (debug) echo_msg(PSTR("<<<")); }

private:
  PGM_P the_msg;
  bool debug;

  void echo_msg(PGM_P const pre) {
    serialprintPGM(pre);
    if (the_msg) {
      SERIAL_CHAR(' ');
      serialprintPGM(the_msg);
    }
    SERIAL_CHAR(' ');
    print_xyz(current_position);
  }
};
