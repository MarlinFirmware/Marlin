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
 * lcd/extui/dgus/creality/PIDHandler.cpp
 *
 * Hotend PID autotune screen. The tune runs as queued G-code (M106 + M303)
 * and finishes in ExtUI::onPIDTuning, so the display stays responsive.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH && ENABLED(PIDTEMP)

#include "PIDHandler.h"
#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"

#include "../../ui_api.h"
#include "../../../marlinui.h"
#include "../../../../module/temperature.h"
#include "../../../../module/settings.h"

uint16_t PIDHandler::cycles = 0;
celsius_t PIDHandler::calibration_temperature = 0;
bool PIDHandler::fan_on = false;
bool PIDHandler::is_running = false;
#if HAS_FAN
  uint8_t PIDHandler::prev_fan_percent = 0;
#endif

void PIDHandler::init() {
  cycles = 3;

  fan_on = TERN0(HAS_FAN, ExtUI::getTargetFan_percent(ExtUI::fan_t::FAN0) > 10);

  // Use the configured PLA temperature + 15 degrees
  calibration_temperature = TERN(HAS_PREHEAT, ui.material_preset[0].hotend_temp, 180) + 15;

  if (!is_running) screen.setStatusMessage(F("Ready"));
}

void PIDHandler::handleStartButton(DGUS_VP_Variable &var, void *val_ptr) {
  if (is_running || screen.hasCurrentSynchronousOperation() || screen.isPrintJobActive() || ExtUI::commandsInQueue()) {
    screen.setStatusMessage(F("Printer is busy"));
    return;
  }

  // Validate
  if (!WITHIN(calibration_temperature, EXTRUDE_MINTEMP, thermalManager.hotend_max_target(0))) {
    screen.setStatusMessage(F("Invalid temperature set"));
    return;
  }

  if (cycles < 3) { // M303 needs at least 3 cycles
    screen.setStatusMessage(F("Invalid number of cycles"));
    return;
  }

  // Synchronous operation - disable back button
  is_running = true;
  screen.setSynchronousOperationStart();

  screen.setStatusMessage(F("PID tuning. Please wait..."));

  MString<64> cmd;
  #if HAS_FAN
    // Set the fan and give it time to spin up
    prev_fan_percent = ExtUI::getTargetFan_percent(ExtUI::fan_t::FAN0);
    cmd.setf(F("M106 S%i\nG4 S2\n"), fan_on ? 255 : 0);
  #endif
  cmd.append(F("M303 E0 S"), int(calibration_temperature), F(" C"), int(cycles), F(" U1"));
  DEBUG_ECHOLNPGM("Executing: ", &cmd);
  ExtUI::injectCommands(cmd);
}

void PIDHandler::onTuningResult(const bool success) {
  if (!is_running) return; // Not started from the screen

  is_running = false;

  TERN_(HAS_FAN, ExtUI::setTargetFan_percent(prev_fan_percent, ExtUI::fan_t::FAN0));

  screen.buzzer(0, 250);
  if (success) settings.request_autosave(); // M303 U1 applied the new values
  screen.setSynchronousOperationFinish();
}

#endif // DGUS_LCD_UI_CREALITY_TOUCH && PIDTEMP
