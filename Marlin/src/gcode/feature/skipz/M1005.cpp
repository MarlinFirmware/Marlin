/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(START_PRINT_FROM_Z)

#include "../../gcode.h"
#include "../../../feature/skip_to_z.h"
#include "../../../sd/cardreader.h"
#include "../../../core/serial.h"

/**
 * M1005: Start the currently-open SD file from a target Z height.
 *
 *   Z<height>  Target Z (mm). Required. The G-code is pre-scanned and
 *              printing begins from the first line whose motion brings
 *              Z >= the given height.
 *
 * Used internally by openAndPrintFile() when the user has set a non-zero
 * "Print from Z" value via the LCD menu. The injected sequence is:
 *
 *   M23 <filename>
 *   M1005 Z<value>
 *
 * If the scan succeeds, this command populates the Power-Loss Recovery
 * info struct and invokes recovery.resume() to perform heating, homing,
 * Z-lift, and continue the print at the discovered byte offset.
 */
void GcodeSuite::M1005() {
  if (!parser.seenval('Z')) {
    SERIAL_ECHO_MSG("M1005: Z required");
    return;
  }

  const float skip_to = parser.value_float();
  if (skip_to <= 0.0f) {
    SERIAL_ECHO_MSG("M1005: Z must be > 0");
    return;
  }

  if (!card.isFileOpen()) {
    SERIAL_ECHO_MSG("M1005: no SD file open (need M23 first)");
    return;
  }

  SERIAL_ECHOLNPGM("M1005: scanning for Z>=", skip_to);

  const SkipToZ::Status st = SkipToZ::prepare(skip_to);
  switch (st) {
    case SkipToZ::OK:
      SERIAL_ECHOLNPGM("M1005: target reached, resuming");
      break;
    case SkipToZ::ERR_NOT_FOUND:
      SERIAL_ECHO_MSG("M1005: target Z not found in file");
      card.closefile();
      break;
    case SkipToZ::ERR_NO_FILE:
      SERIAL_ECHO_MSG("M1005: no file to scan");
      break;
    case SkipToZ::ERR_READ:
      SERIAL_ECHO_MSG("M1005: SD read error during scan");
      card.closefile();
      break;
  }
}

#endif // START_PRINT_FROM_Z
