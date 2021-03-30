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
#pragma once

#include "../inc/MarlinConfigPre.h"

#if ENABLED(POSTMORTEM_DEBUGGING)
  void _bugOnImpl(const char * filename, const int line, const char * str, const long * values, const int len, bool crash);
  // Useful macro for stopping the CPU on an unexpected condition
  // This is used like SERIAL_ECHOPAIR, that is: a key-value call of the local variables you want
  // to dump to the serial port before stopping the CPU.
  #define BUG_ON(TXT, ...) do { long v[] = { ##__VA_ARGS__ }; _bugOnImpl(ONLY_FILENAME, __LINE__, TXT, v, sizeof(v)/sizeof(v[0]), true); } while(0)
#elif ENABLED(MARLIN_DEV_MODE)
  void _bugOnImpl(const char * filename, const int line, const char * str, const long * values, const int len, bool crash);
  // Don't stop the CPU here, but at least dump the bug on the serial port
  #define BUG_ON(TXT, ...) do { long v[] = { ##__VA_ARGS__ }; _bugOnImpl(ONLY_FILENAME, __LINE__, TXT, v, sizeof(v)/sizeof(v[0]), false); } while(0)
#else
  // Release mode, let's ignore the bug
  #define BUG_ON(TXT, ...) NOOP
#endif
