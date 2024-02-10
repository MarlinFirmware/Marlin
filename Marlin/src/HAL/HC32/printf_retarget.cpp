/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_HC32
#ifdef REDIRECT_PRINTF_TO_SERIAL

#ifndef __GNUC__
  #error "only GCC is supported"
#endif

#include "../../inc/MarlinConfig.h"

/**
 * @brief implementation of _write that redirects everything to the host serial(s)
 * @param file file descriptor. don't care
 * @param ptr pointer to the data to write
 * @param len length of the data to write
 * @return number of bytes written
 */
extern "C" int _write(int file, char *ptr, int len) {
  //SERIAL_ECHO_START(); // echo:
  for (int i = 0; i < len; i++) SERIAL_CHAR(ptr[i]);
  return len;
}

/**
 * @brief implementation of _isatty that always returns 1
 * @param file file descriptor. don't care
 * @return everything is a tty. there are no files to be had
 */
extern "C" int _isatty(int file) {
  return 1;
}

#endif // REDIRECT_PRINTF_TO_SERIAL
#endif // ARDUINO_ARCH_HC32
