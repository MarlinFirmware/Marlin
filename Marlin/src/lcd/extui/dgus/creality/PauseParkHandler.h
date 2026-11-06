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
 * lcd/extui/dgus/creality/PauseParkHandler.h
 *
 * Pause an SD print and park the nozzle without holding the G-code queue.
 *
 * M125 (and M25 with PARK_HEAD_ON_PAUSE) wait inside the running command until
 * the user resumes. Commands injected by the screen, such as Load / Unload
 * filament from the Tune menu, cannot run during that wait. The CR-6 touch
 * screen expects to use them while paused, so it parks with ordinary queued
 * moves instead. PARK_HEAD_ON_PAUSE still parks for M600 and filament runout.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if ALL(HAS_MEDIA, NOZZLE_PARK_FEATURE)
  #define CREALITY_PARK_ON_PAUSE 1
#endif

#if CREALITY_PARK_ON_PAUSE

#include "../DGUSScreenHandler.h"
#include "../../../../gcode/gcode.h"

class PauseParkHandler {
public:
  // Start a pause. Returns false if the stock pause should be used instead.
  static bool pause();

  // Start a resume. Returns false if the print was not paused by pause().
  static bool resume();

  // M600 and M24 requests, from G-code or the host. Return true if handled here.
  static bool onFilamentChange();
  static bool onResumeRequest();

  // Parking or unparking is in progress
  static bool isBusy() { return working; }

  // Forget everything, e.g., when the print is stopped
  static void reset();

private:
  static void onPaused();
  static void onParked();
  static void resumeHeated();
  static void resumeMoved();
  static void resumeZ();
  static void onResumed();

  static void enterMoveModes();
  static void leaveMoveModes();

  static bool working, parked, retracted, modes_saved;
  static float saved_x, saved_y, saved_z, saved_e;
  static celsius_t saved_temp;
  static relative_t saved_axis_relative;
  static feedRate_t saved_feedrate;
};

#endif // CREALITY_PARK_ON_PAUSE
