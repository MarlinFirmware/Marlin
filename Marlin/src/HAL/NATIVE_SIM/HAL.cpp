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

#ifdef __PLAT_NATIVE_SIM__

#ifndef HAS_LIBBSD

  #include "HAL.h"

  /**
   * Redirect missing strlcpy here
   */
  size_t MarlinHAL::_strlcpy(char *dst, const char *src, size_t dsize) {
    const char *osrc = src;
    size_t nleft = dsize;

    // Copy as many bytes as will fit.
    if (nleft != 0) while (--nleft != 0) if ((*dst++ = *src++) == '\0') break;

    // Not enough room in dst, add NUL and traverse rest of src.
    if (nleft == 0) {
      if (dsize != 0) *dst = '\0';    // NUL-terminate dst
      while (*src++) { /* nada */ }
    }

    return (src - osrc - 1); // count does not include NUL
  }

#endif // HAS_LIBBSD
#endif // __PLAT_NATIVE_SIM__
