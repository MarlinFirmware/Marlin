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
 * lcd/extui/dgus/creality/EstepsHandler.h
 *
 * E-steps calibration screens for the Creality CR-6 touch screen,
 * ported from the CR6Community firmware.
 */

#include "../DGUSScreenHandler.h"
#include "../../../../gcode/gcode.h"

class EstepsHandler {
public:
  static void init();

  static void handleStartButton(DGUS_VP_Variable &var, void *val_ptr);
  static void handleApplyButton(DGUS_VP_Variable &var, void *val_ptr);
  static void handleBackButton(DGUS_VP_Variable &var, void *val_ptr);

  static void handleRemainingFilament(DGUS_VP_Variable &var, void *val_ptr);

public: // Needed for VP auto-upload
  static float set_esteps;
  static float calculated_esteps;

  static float mark_filament_mm;
  static float remaining_filament;

  static float filament_to_extrude;
  static celsius_t calibration_temperature;

private:
  static void onHeated();
  static void onExtruded();
  static void saveSettingsAndReturn(const bool fullConfirm);

  static bool is_running;
  static relative_t saved_axis_relative;
  #if HAS_LIN_ADVANCE_K
    static float saved_advance_k;
  #endif
};
