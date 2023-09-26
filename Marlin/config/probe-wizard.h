/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
CONFIG_VERSION(02020000)

#if HAS_BED_PROBE && ANY(HAS_MARLINUI_MENU, HAS_TFT_LVGL_UI)
  //#define PROBE_OFFSET_WIZARD       // Add a Probe Z Offset calibration option to the LCD menu
  #if ENABLED(PROBE_OFFSET_WIZARD)
    /**
     * Enable to init the Probe Z-Offset when starting the Wizard.
     * Use a height slightly above the estimated nozzle-to-probe Z offset.
     * For example, with an offset of -5, consider a starting height of -4.
     */
    //#define PROBE_OFFSET_WIZARD_START_Z -4.0

    // Set a convenient position to do the calibration (probing point and nozzle/bed-distance)
    //#define PROBE_OFFSET_WIZARD_XY_POS { X_CENTER, Y_CENTER }
  #endif
#endif
