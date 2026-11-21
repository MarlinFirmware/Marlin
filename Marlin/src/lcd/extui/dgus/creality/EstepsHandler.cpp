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
 * lcd/extui/dgus/creality/EstepsHandler.cpp
 *
 * E-steps calibration: raise Z, heat up, extrude a set length, lower Z, then
 * calculate new steps/mm from the filament left before a mark. The moves run
 * as queued G-code; each stage continues once the queue is done.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH && ALL(HAS_EXTRUDERS, EDITABLE_STEPS_PER_UNIT)

#include "EstepsHandler.h"
#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"

#include "../../ui_api.h"
#include "../../../marlinui.h"
#include "../../../../module/settings.h"

float EstepsHandler::set_esteps = 0;
float EstepsHandler::calculated_esteps = 0;
float EstepsHandler::remaining_filament = 0;
float EstepsHandler::mark_filament_mm = 0;
float EstepsHandler::filament_to_extrude = 0;
celsius_t EstepsHandler::calibration_temperature = 0;

bool EstepsHandler::is_running = false;
relative_t EstepsHandler::saved_axis_relative;
#if HAS_LIN_ADVANCE_K
  float EstepsHandler::saved_advance_k;
#endif

void EstepsHandler::init() {
  if (is_running) return;

  // Use steps
  set_esteps = ExtUI::getAxisSteps_per_mm(ExtUI::E0);
  calculated_esteps = 0;

  // Reset
  filament_to_extrude = 100;
  mark_filament_mm = 120;
  remaining_filament = 0;

  // Use the configured PLA temperature + 10 degrees
  calibration_temperature = TERN(HAS_PREHEAT, ui.material_preset[0].hotend_temp, 200) + 10;

  screen.setStatusMessage(F("Ready"));
}

void EstepsHandler::handleStartButton(DGUS_VP_Variable &var, void *val_ptr) {
  if (is_running || screen.hasCurrentSynchronousOperation() || screen.isPrintJobActive() || ExtUI::commandsInQueue()) {
    screen.setStatusMessage(F("Printer is busy"));
    return;
  }

  // Validate
  if (calibration_temperature < EXTRUDE_MINTEMP) {
    screen.setStatusMessage(F("Invalid temperature set"));
    return;
  }

  if (filament_to_extrude < 10) {
    screen.setStatusMessage(F("Invalid extrusion length set"));
    return;
  }

  if (mark_filament_mm < filament_to_extrude) {
    screen.setStatusMessage(F("Invalid mark length set"));
    return;
  }

  // Synchronous operation - disable back button
  is_running = true;
  screen.setSynchronousOperationStart();

  // Prepare: relative moves, no linear advance. The queue is empty, so this is safe to do now.
  saved_axis_relative = gcode.axis_relative;
  gcode.set_relative_mode(true);
  gcode.set_e_relative();

  #if HAS_LIN_ADVANCE_K
    saved_advance_k = ExtUI::getLinearAdvance_mm_mm_s(ExtUI::E0);
    ExtUI::setLinearAdvance_mm_mm_s(0, ExtUI::E0);
  #endif

  // Raise Z and heat up
  screen.setStatusMessage(F("Heating up..."));

  MString<32> cmd;
  cmd.setf(F("G0 Z5 F150\nM109 R%i"), int(calibration_temperature));
  ExtUI::injectCommands(cmd);
  screen.runWhenQueueDone(onHeated);
}

void EstepsHandler::onHeated() {
  screen.setStatusMessage(F("Extruding..."));

  MString<40> cmd(F("G1 E"), p_float_t(filament_to_extrude, 1), F(" F50\nG0 Z-5 F150"));
  ExtUI::injectCommands(cmd);
  screen.runWhenQueueDone(onExtruded);
}

void EstepsHandler::onExtruded() {
  // Restore the previous state
  gcode.axis_relative = saved_axis_relative;
  TERN_(HAS_LIN_ADVANCE_K, ExtUI::setLinearAdvance_mm_mm_s(saved_advance_k, ExtUI::E0));

  // Done
  screen.gotoScreen(DGUS_SCREEN_ESTEPS_CALIBRATION_RESULTS);
  screen.buzzer(0, 250);
  is_running = false;
  screen.setSynchronousOperationFinish();
  screen.setStatusMessage(F("Measure remaining filament"));
}

void EstepsHandler::handleApplyButton(DGUS_VP_Variable &var, void *val_ptr) {
  if (is_running) return;

  // Without a calculated value the user intends to set e-steps directly
  ExtUI::setAxisSteps_per_mm(ABS(calculated_esteps) < 1 ? set_esteps : calculated_esteps, ExtUI::E0);

  saveSettingsAndReturn(true);
}

void EstepsHandler::handleBackButton(DGUS_VP_Variable &var, void *val_ptr) {
  if (is_running) return;

  // User intends to set e-steps directly
  ExtUI::setAxisSteps_per_mm(set_esteps, ExtUI::E0);

  saveSettingsAndReturn(false);
}

void EstepsHandler::saveSettingsAndReturn(const bool fullConfirm) {
  settings.request_autosave();

  if (fullConfirm) screen.buzzer(0, 250);

  screen.popToOldScreen();
  if (fullConfirm) screen.gotoScreen(DGUS_SCREEN_MAIN);

  screen.setStatusMessage(F("New e-steps value saved"));
}

void EstepsHandler::handleRemainingFilament(DGUS_VP_Variable &var, void *val_ptr) {
  screen.setFloatAsIntFromDisplay<1>(var, val_ptr);

  // Calculate
  constexpr float precision = 0.01f;
  const float actualExtrusion = mark_filament_mm - remaining_filament;
  if (actualExtrusion < -precision) {
    screen.setStatusMessage(F("Mark filament further"));
    return;
  }

  if (actualExtrusion < precision) {
    screen.setStatusMessage(F("Invalid remaining length"));
    return;
  }

  if (ABS(actualExtrusion - filament_to_extrude) < precision) {
    screen.setStatusMessage(F("E-steps are correct"));
    calculated_esteps = set_esteps;
    return;
  }

  const float current_steps = ExtUI::getAxisSteps_per_mm(ExtUI::E0),
              new_steps = (current_steps * filament_to_extrude) / actualExtrusion;

  DEBUG_ECHOLNPGM("Current steps: ", current_steps, " Actual extrusion: ", actualExtrusion, " New steps: ", new_steps);

  calculated_esteps = new_steps;

  screen.setStatusMessage(F("Calculated new e-steps"));
}

#endif // DGUS_LCD_UI_CREALITY_TOUCH && HAS_EXTRUDERS && EDITABLE_STEPS_PER_UNIT
