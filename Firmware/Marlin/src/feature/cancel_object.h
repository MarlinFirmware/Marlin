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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <stdint.h>

class CancelObject {
public:
  static bool skipping;
  static int8_t object_count, active_object;
  static uint32_t canceled;
  static void set_active_object(const int8_t obj);
  static void cancel_object(const int8_t obj);
  static void uncancel_object(const int8_t obj);
  static void report();
  static inline bool is_canceled(const int8_t obj) { return TEST(canceled, obj); }
  static inline void clear_active_object() { set_active_object(-1); }
  static inline void cancel_active_object() { cancel_object(active_object); }
  static inline void reset() { canceled = 0x0000; object_count = 0; clear_active_object(); }
};

extern CancelObject cancelable;
