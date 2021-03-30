/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669 [https://blog.cyril.by]
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
#include "bug_on.h"

#if EITHER(POSTMORTEM_DEBUGGING, MARLIN_DEV_MODE)
  void _bugOnImpl(const char * filename, const int line, const char * str, const long * values, const int len, bool crash) {
    SERIAL_ECHOLNPAIR(filename, "(", line, ") BUG: ", str);
    for (int i = 0; i < len; i++) SERIAL_ECHO(values[i], PrintBase::Hex);
    SERIAL_FLUSHTX();
    if (crash) *(char*)0 = 42;
  }
#endif
