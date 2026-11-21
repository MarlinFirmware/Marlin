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
 * lcd/extui/dgus/creality/MeshValidationHandler.h
 *
 * Mesh validation (G26) screen for the Creality CR-6 touch screen,
 * ported from the CR6Community firmware.
 */

#include "../DGUSScreenHandler.h"

class MeshValidationHandler {
public:
  static void init();

  static void handleTemperature(DGUS_VP_Variable &var, void *val_ptr);
  static void handleStartOrCancelButton(DGUS_VP_Variable &var, void *val_ptr);

  // Called from ExtUI::onMeshUpdate for G26_START and G26_FINISH
  static void onMeshValidationStart();
  static void onMeshValidationFinish();

public: // Needed for VP auto-upload
  static celsius_t nozzle_temperature;
  static celsius_t bed_temperature;

  static bool is_running;

private:
  static void start();
  static void cancel();
  static void onQueueDone();

  static void validateTemperatures();

  static feedRate_t prev_feedrate;
  static bool is_cancelling, started_from_screen, pattern_started;
};
