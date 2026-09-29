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

/**
 * feature/continue_from_z.h - Manual "Continue Print from Z" support.
 *
 * Pre-scans an SD G-code file, simulating machine state without motion, to
 * locate the byte offset where a chosen Z height is first reached. The
 * captured state is dumped into the Power-Loss Recovery info struct and the
 * standard PLR resume() path is used to physically position the printer and
 * begin printing from that offset.
 */

#include "../inc/MarlinConfigPre.h"

namespace ContinueFromZ {

  // Target Z height in mm. 0 = feature disabled (normal print).
  // Set from the menu, consumed (and cleared) by openAndPrintFile().
  extern float target_z;

  // Status codes returned from prepare().
  enum Status : uint8_t {
    OK,            // info populated; caller should call recovery.resume()
    ERR_NO_FILE,   // no SD file open
    ERR_NOT_FOUND, // scanned to EOF without reaching target Z
    ERR_READ       // SD read error
  };

  /**
   * Scan the currently-open SD file from offset 0, simulating G-code state,
   * until a motion command brings Z >= skip_to_z. On OK, populates
   * PrintJobRecovery::info with the simulated state (current_position,
   * feedrate, target temperatures, fan speeds, active tool, abs/rel modes,
   * sd_filename, sdpos = byte offset of next-to-execute line) so that
   * recovery.resume() will continue printing from that point.
   *
   * Blocks during the scan but periodically calls idle() to keep the
   * watchdog, thermal manager and UI alive.
   */
  Status prepare(const float skip_to_z);

} // namespace ContinueFromZ
