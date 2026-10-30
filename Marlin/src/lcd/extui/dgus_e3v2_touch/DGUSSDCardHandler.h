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

#include "config/DGUS_Addr.h"

/**
 * The stock screen has twenty filename slots, four on each of FILE1-FILE5,
 * and pages through them itself. Load the first twenty files of the root
 * folder once, keep the long name for display and the short name to print.
 */
class DGUS_SDCardHandler {
public:
  static char filenames[DGUS_FILE_COUNT][DGUS_FILENAME_LEN + 1];
  static char shortnames[DGUS_FILE_COUNT][13];
  static uint8_t fileCount;
  static int8_t selected;               // 0-based index, -1 for none

  static void reset();
  static bool select(const uint8_t index);  // 0-based
  static const char* selectedShortName() { return selected < 0 ? nullptr : shortnames[selected]; }
  static const char* selectedName() { return selected < 0 ? nullptr : filenames[selected]; }
};

extern DGUS_SDCardHandler dgus_sdcard_handler;
