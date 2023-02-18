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

/**
 * M36: Return file information in JSON format (for PanelDue)
 *
 *  [P"<filename>"] - File to query. Omit to query the currently printing file.
 *
 * Response fields:
 *   err          - 0 = success, 1 = file not found / not printing
 *   fileName     - Name of the file (as passed, or current file)
 *   size         - File size in bytes
 *   printDuration- Elapsed print time in seconds (only when printing that file)
 *
 * Note: Fields requiring GCode file parsing (height, layerHeight, filament,
 * generatedBy, etc.) are not supported in Marlin and are omitted.
 */

#include "../../inc/MarlinConfig.h"

#if ALL(PANELDUE, HAS_MEDIA)

#include "../gcode.h"
#include "../../sd/cardreader.h"
#include "../../module/printcounter.h"

void GcodeSuite::M36() {
  char filename[MAXPATHNAMELENGTH] = {};
  const bool specific = parser.seenval('P');

  if (specific) {
    // Copy the quoted/unquoted string parameter
    const char *p = parser.value_string();
    if (p) strncpy(filename, p, sizeof(filename) - 1);
  }

  SERIAL_CHAR('{');

  // If a specific file was requested, open it to get its size
  if (specific && filename[0]) {
    MediaFile *dirPtr;
    const char *fname = card.diveToFile(false, dirPtr, filename);
    if (fname && fname[0]) {
      MediaFile f;
      if (f.open(dirPtr, fname, O_READ)) {
        const uint32_t fsize = f.fileSize();
        f.close();
        SERIAL_ECHOPGM("\"err\":0");
        SERIAL_ECHO(F(",\"fileName\":\""), filename, C('"'));
        SERIAL_ECHOPGM(",\"size\":", fsize);
      }
      else {
        SERIAL_ECHOPGM("\"err\":1");
        SERIAL_ECHO(F(",\"fileName\":\""), filename, C('"'));
      }
    }
    else {
      SERIAL_ECHOPGM("\"err\":1");
      SERIAL_ECHO(F(",\"fileName\":\""), filename, C('"'));
    }
  }
  else {
    // Query the currently printing file
    if (card.isStillPrinting() || card.isPaused()) {
      SERIAL_ECHOPGM("\"err\":0");
      SERIAL_ECHOPGM(",\"fileName\":\"");
      card.printSelectedFilename();
      SERIAL_CHAR('"');
      SERIAL_ECHOPGM(",\"size\":", card.getFileSize());
      SERIAL_ECHOPGM(",\"printDuration\":", print_job_timer.duration());
    }
    else {
      SERIAL_ECHOPGM("\"err\":1,\"fileName\":\"\"");
    }
  }

  SERIAL_CHAR('}');
  SERIAL_EOL();
}

#endif // PANELDUE && HAS_MEDIA
