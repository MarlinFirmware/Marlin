/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../../Marlin/src/core/macros.h"

DeclareTest("_MIN/_MAX", []() -> bool {
  if (_MIN(3, 4, 5) != 3) return False;
  if (_MIN(3.4, 12, 1234U) != 3.4) return False;
  if (_MAX(3, 5, 4) != 5) return False;
  if (_MAX(3, 4, 5) == _MIN(3, 4, 5)) return False;
  return true;
});

DeclareTest("WITHIN", []() -> bool {
  if (WITHIN(3, 4, 5)) return False;
  if (!WITHIN(4, 3, 5)) return False;
  return true;
});

DeclareTest("NUMERIC", []() -> bool {
  unsigned char c = 0;
  for (c = 0; c != '0'; c++)
    if (NUMERIC(c)) return False;
  for (; c <= '9'; c++)
    if (!NUMERIC(c)) return False;
  for (; c < 255; c++)
    if (NUMERIC(c)) return False;

  return true;
});

DeclareTest("DECIMAL", []() -> bool {
  unsigned char c = 0;
  for (c = 0; c < '.'; c++)
    if (DECIMAL(c)) return False;

  if (!DECIMAL(c)) return False;
  c++; // Don't put in macro or it's evaluated twice!
  if (DECIMAL(c)) return False;
  c++;

  for (; c <= '9'; c++)
    if (!DECIMAL(c)) return False;
  for (; c < 255; c++)
    if (DECIMAL(c)) return False;

  return true;
});

DeclareTest("HEXCHR", []() -> bool {
  const char hexChars[] = "0123456789abcdef";
  const char hexChars2[] = "0123456789ABCDEF";

  for (const char * c = hexChars; *c; c++)
    if (HEXCHR(*c) != (c - hexChars)) return False(c);

  for (const char * c = hexChars2; *c; c++)
    if (HEXCHR(*c) != (c - hexChars2)) return False(c);

  if (HEXCHR('G') != -1) return False;
  return true;
});

DeclareTest("NUMERIC_SIGNED", []() -> bool {
  if (!NUMERIC_SIGNED('+')) return False;
  if (!NUMERIC_SIGNED('-')) return False;
  return true;
});

DeclareTest("DECIMAL_SIGNED", []() -> bool {
  if (!DECIMAL_SIGNED('+')) return False;
  if (!DECIMAL_SIGNED('-')) return False;
  return true;
});
