/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * TFT_COLOR_UI G-code thumbnail preview header
 * Adapted from DWIN PROUI gcode_preview.h
 */

#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TFT_COLOR_UI) && HAS_GCODE_PREVIEW

/**
 * Class for handling G-code thumbnail preview functionality
 */
class Preview {
  public:
    /** Check if a preview is available for the current file */
    bool hasPreview();
    /** Draw the preview screen from SD card */
    void drawFromSD();
    /** Invalidate the current preview cache */
    void invalidate();
    /** Check if the preview is valid */
    bool valid();
    /** Show the thumbnail at specified coordinates */
    void show(int x, int y);

  private:
    /** Flag to track if preview has been processed */
    bool processed = false;
};

extern Preview preview;

#endif // ENABLED(TFT_COLOR_UI) && HAS_GCODE_PREVIEW
