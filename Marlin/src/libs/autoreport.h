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

#include "../inc/MarlinConfig.h"

template<serial_index_t AR_PORT_INDEX>
class AutoReporter {
public:
  millis_t next_report_ms;
  uint8_t report_interval;

  // Override this method
  inline void auto_report() { }

  inline void set_interval(uint8_t seconds, const uint8_t limit=60) {
    report_interval = _MIN(seconds, limit);
    next_report_ms = millis() + SEC_TO_MS(seconds);
  }

  inline void tick() {
    if (!report_interval) return;
    const millis_t ms = millis();
    if (ELAPSED(ms, next_report_ms)) {
      next_report_ms = ms + SEC_TO_MS(report_interval);
      PORT_REDIRECT(AR_PORT_INDEX);
      auto_report();
    }
  }
};
