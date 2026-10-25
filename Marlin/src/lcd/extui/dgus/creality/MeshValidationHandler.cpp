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
 * lcd/extui/dgus/creality/MeshValidationHandler.cpp
 *
 * Print the G26 mesh validation pattern from the touch screen. G26 started
 * by G-code (e.g., from a host) also shows this screen while it runs.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH && ENABLED(G26_MESH_VALIDATION)

#include "MeshValidationHandler.h"
#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"

#include "../../ui_api.h"
#include "../../../marlinui.h"
#include "../../../../MarlinCore.h"
#include "../../../../module/motion.h"
#include "../../../../module/temperature.h"

#ifndef G26_XY_FEEDRATE
  #define G26_XY_FEEDRATE (PLANNER_XY_FEEDRATE_MM_S / 3.0)
#endif
#ifndef G26_RETRACT_MULTIPLIER
  #define G26_RETRACT_MULTIPLIER 1.0
#endif

celsius_t MeshValidationHandler::nozzle_temperature;
celsius_t MeshValidationHandler::bed_temperature;
bool MeshValidationHandler::is_running;
bool MeshValidationHandler::started_from_screen;
bool MeshValidationHandler::pattern_started;
bool MeshValidationHandler::is_cancelling;
feedRate_t MeshValidationHandler::prev_feedrate;

void MeshValidationHandler::init() {
  if (is_running) return;

  // Set to PLA pre-heat temps by default
  nozzle_temperature = TERN(HAS_PREHEAT, ui.material_preset[0].hotend_temp, 200);
  bed_temperature = TERN(HAS_PREHEAT, ui.material_preset[0].bed_temp, 60);

  validateTemperatures();
}

void MeshValidationHandler::handleTemperature(DGUS_VP_Variable &var, void *val_ptr) {
  screen.setValueDirectly<int16_t>(var, val_ptr);
  validateTemperatures();
}

void MeshValidationHandler::validateTemperatures() {
  // The ranges accepted by G26
  LIMIT(nozzle_temperature, _MAX(EXTRUDE_MINTEMP, 165), thermalManager.hotend_max_target(0));
  #if HAS_HEATED_BED
    LIMIT(bed_temperature, 40, BED_MAX_TARGET);
  #endif
}

void MeshValidationHandler::handleStartOrCancelButton(DGUS_VP_Variable &var, void *val_ptr) {
  if (!is_running)
    start();
  else
    cancel();
}

void MeshValidationHandler::start() {
  if (!ExtUI::getLevelingIsValid()) {
    screen.setStatusMessage(F("Please level bed first"));
    return;
  }

  if (screen.hasCurrentSynchronousOperation() || screen.isPrintJobActive() || ExtUI::commandsInQueue()) {
    screen.setStatusMessage(F("Printer is busy"));
    return;
  }

  // Block
  screen.setSynchronousOperationStart();
  is_running = true;
  started_from_screen = true;
  pattern_started = false;

  // Home if necessary, then move X to 0 so the nozzle droops at the side of the bed.
  // G26 with temperatures, for the full bed (R), retract multiplier (Q), prime 2mm (P), from the bed center
  MString<64> cmd;
  if (motion.homing_needed()) cmd.set(F("G28\n"));
  cmd.append(F("G90\nG0 X0\nG26"));
  #if HAS_HEATED_BED
    cmd.append(F(" B"), int(bed_temperature));
  #endif
  cmd.append(F(" H"), int(nozzle_temperature), F(" R Q"), p_float_t(G26_RETRACT_MULTIPLIER, 1), F(" P2 X"), int(X_BED_SIZE / 2), F(" Y"), int(Y_BED_SIZE / 2));
  DEBUG_ECHOLNPGM("Injecting command: ", &cmd);
  ExtUI::injectCommands(cmd);
  screen.runWhenQueueDone(onQueueDone);

  // Set feedrate
  prev_feedrate = ExtUI::getFeedrate_mm_s();
  ExtUI::setFeedrate_mm_s(G26_XY_FEEDRATE);

  screen.setStatusMessage(F("Starting..."));
}

/**
 * Mainline G26 checks for a cancel only with a MarlinUI click, so the pattern can't be
 * stopped from the touch screen. During heat-up the wait can be ended (as with M108),
 * after which G26 leaves without printing the pattern.
 */
void MeshValidationHandler::cancel() {
  if (is_cancelling) return;

  if (!marlin.is_heating()) {
    screen.setStatusMessage(F("Can only cancel while heating"));
    return;
  }

  is_cancelling = true;
  marlin.end_waiting();

  screen.setStatusMessage(F("Cancelling..."));
}

void MeshValidationHandler::onMeshValidationStart() {
  // Note: also called when G26 is invoked by G-code
  if (screen.getCurrentScreen() != DGUS_SCREEN_MESH_VALIDATION)
    screen.gotoScreen(DGUS_SCREEN_MESH_VALIDATION);

  // Set state
  screen.setSynchronousOperationStart();
  is_running = true;
  pattern_started = true;
}

void MeshValidationHandler::onMeshValidationFinish() {
  // Started from the screen: finish once G26 has returned (onQueueDone)
  if (started_from_screen) return;

  // Invoked by G-code: pop back if we came here on G26_START
  if (pattern_started) screen.popToOldScreen();

  is_running = false;
  pattern_started = false;
  is_cancelling = false;

  screen.setSynchronousOperationFinish();
}

void MeshValidationHandler::onQueueDone() {
  if (!started_from_screen) return;

  ExtUI::setFeedrate_mm_s(prev_feedrate);

  if (pattern_started && !is_cancelling) {
    // Disable steppers
    ExtUI::injectCommands(F("M84"));
    screen.setStatusMessage(F("Mesh validation pattern printed"));
  }
  else {
    // Park and disable steppers
    ExtUI::injectCommands(F("G0 X5 F2000\n" TERN_(NOZZLE_PARK_FEATURE, "G27\n") "M84"));
    if (is_cancelling)
      screen.setStatusMessage(F("Canceled mesh validation pattern"));
    else
      screen.setStatusMessage(F("Mesh validation failed"));
  }

  // Reset state
  is_running = false;
  started_from_screen = false;
  pattern_started = false;
  is_cancelling = false;

  screen.setSynchronousOperationFinish();
}

#endif // DGUS_LCD_UI_CREALITY_TOUCH && G26_MESH_VALIDATION
