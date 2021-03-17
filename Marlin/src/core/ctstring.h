/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2021 X-Ryl669
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

#include <stddef.h>

// This file contains few compile time string helpers functions to help reduce the binary
// and runtime resources when working with string
namespace CompileTimeString {
  // Hash methods
  //////////////////////////////////////////////////////////

  // When to use those ?
  //
  // Typically, when doing a long chain of
  // if (strcmp(txt, "ABC") == 0) {}
  // else if (strcmp(txt, "CDE") == 0) {}
  // etc...
  // You're:
  //   1. Storing the "ABC" / "CDE" string in the firmware
  //   2. Using a O(N*M) algorithm (you are parsing the txt string M times with size N) while a O(1) is only required
  //   3. Preventing the compiler to use a jump table (so larger code size is generated for all cases)

  // So instead, do this:
  // switch (constHash(txt)) {
  //  case "ABC"_hash: {}
  //  case "CDE"_hash: {}
  // In that case, you're only paying for a O(N) algorithm here (computing the hash of txt)
  // and then a O(1) for the switch/case jump table.
  // This works because the case's hash are computed at compile time and only the hash is stored in the binary

  // When not to use those ?
  //
  // If you have single:
  // if (strcmp(A, B) == 0)
  // Then adding the hash computation into the mix will actually slow things down, but you'll still gain
  // the string storage size in flash. So it's worth it only if the string is larger than a machine word here (likely 4 bytes)

  // Compute the hash of a string at compile time
  size_t constexpr constHash(const char * const input) {
    return *input ? static_cast<size_t>(*input) + 257 * constHash(input + 1) : 5381;
  }

  // String searching
  //////////////////////////////////////////////////////////

  // When to use those ?
  //
  // When you need to extract a small portion of a string that's known at compile time (can be automatically generated)
  // Instead of doing runtime string operations, the compiler can deal with minimal string parsing at compile time
  // Simple compile-time parser to find the position of the end of a string
  constexpr const char* findStringEnd(const char * const str) {
    return *str ? findStringEnd(str + 1) : str;
  }

  // Check whether a string contains a slash
  constexpr bool containsSlash(const char * const str) {
    return *str == '/' ? true : (*str ? containsSlash(str + 1) : false);
  }
  // Find the last position of the slash
  constexpr const char* findLastSlashPos(const char * const str) {
    return *str == '/' ? (str + 1) : findLastSlashPos(str - 1);
  }
  // Compile-time evaluation of the last part of a file path
  // Typically used to shorten the path to file in compiled strings
  // CompileTimeString::baseName(__FILE__) returns "macros.h" and not /path/to/Marlin/src/core/macros.h
  constexpr const char* baseName(const char * const str) {
    return containsSlash(str) ? findLastSlashPos(findStringEnd(str)) : str;
  }
}


// Compute the hash of a string at compile time
size_t constexpr operator "" _hash(const char * const str, size_t len) {
  return CompileTimeString::constHash( str );
}

#define HASH(X)       CompileTimeString::constHash(X)
#define ONLY_FILENAME CompileTimeString::baseName(__FILE__)
