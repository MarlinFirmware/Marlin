/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

/**
 * feature/skip_to_z.h - Manual "Start Print From Z" support.
 *
 * Pre-scans an SD G-code file, simulating machine state without motion, to
 * locate the byte offset where a chosen Z height is first reached. The
 * captured state is dumped into the Power-Loss Recovery info struct and the
 * standard PLR resume() path is used to physically position the printer and
 * begin printing from that offset.
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(START_PRINT_FROM_Z)

namespace SkipToZ {

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
   * until a motion command brings Z >= skip_to. On OK, populates
   * PrintJobRecovery::info with the simulated state (current_position,
   * feedrate, target temperatures, fan speeds, active tool, abs/rel modes,
   * sd_filename, sdpos = byte offset of next-to-execute line) so that
   * recovery.resume() will continue printing from that point.
   *
   * Blocks during the scan but periodically calls idle() to keep the
   * watchdog, thermal manager and UI alive.
   */
  Status prepare(const float skip_to);

} // namespace SkipToZ

#endif // START_PRINT_FROM_Z
