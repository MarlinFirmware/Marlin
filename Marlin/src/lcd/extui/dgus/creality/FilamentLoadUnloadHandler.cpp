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
 * lcd/extui/dgus/creality/FilamentLoadUnloadHandler.cpp
 *
 * Heat up if needed, then load or unload the given length of filament.
 * Uses M701/M702 when FILAMENT_LOAD_UNLOAD_GCODES is enabled, otherwise a
 * plain relative E move. The G-code is queued; each stage continues once
 * the queue is done.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH && HAS_EXTRUDERS

#include "FilamentLoadUnloadHandler.h"
#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"

#include "../../ui_api.h"
#include "../../../marlinui.h"
#include "../../../../module/temperature.h"

#if DISABLED(FILAMENT_LOAD_UNLOAD_GCODES)
  // Feedrates for the plain E move (mm/s)
  #ifdef FILAMENT_CHANGE_FAST_LOAD_FEEDRATE
    #define FEED_LOAD_FEEDRATE FILAMENT_CHANGE_FAST_LOAD_FEEDRATE
  #else
    #define FEED_LOAD_FEEDRATE 6
  #endif
  #ifdef FILAMENT_CHANGE_UNLOAD_FEEDRATE
    #define FEED_UNLOAD_FEEDRATE FILAMENT_CHANGE_UNLOAD_FEEDRATE
  #else
    #define FEED_UNLOAD_FEEDRATE 10
  #endif
#endif

#ifdef THERMAL_PROTECTION_HYSTERESIS
  #define FEED_HEAT_HYSTERESIS THERMAL_PROTECTION_HYSTERESIS
#else
  #define FEED_HEAT_HYSTERESIS 4
#endif

celsius_t FilamentLoadUnloadHandler::nozzle_temperature = 0;
float FilamentLoadUnloadHandler::length = 0;
bool FilamentLoadUnloadHandler::is_running = false;
bool FilamentLoadUnloadHandler::is_loading = false;
#if DISABLED(FILAMENT_LOAD_UNLOAD_GCODES)
  relative_t FilamentLoadUnloadHandler::saved_axis_relative;
#endif

void FilamentLoadUnloadHandler::init() {
  if (is_running) return;

  nozzle_temperature = TERN(HAS_PREHEAT, ui.material_preset[0].hotend_temp, 200);
  length = 150;

  if (screen.isPrintJobActive()) nozzle_temperature = ExtUI::getTargetTemp_celsius(ExtUI::E0);

  validateTemperatures();
}

void FilamentLoadUnloadHandler::handleTemperature(DGUS_VP_Variable &var, void *val_ptr) {
  screen.setValueDirectly<int16_t>(var, val_ptr);
  validateTemperatures();
}

void FilamentLoadUnloadHandler::validateTemperatures() {
  LIMIT(nozzle_temperature, EXTRUDE_MINTEMP, thermalManager.hotend_max_target(0));
}

void FilamentLoadUnloadHandler::handleLoadUnloadButton(DGUS_VP_Variable &var, void *val_ptr) {
  if (is_running || screen.hasCurrentSynchronousOperation()) {
    screen.setStatusMessage(F("Printer is busy"));
    return;
  }

  // Common for load/unload
  if (length < 0.1f) {
    screen.setStatusMessage(F("Invalid feed length"));
    return;
  }

  if (screen.isPrintJobActive() && !ExtUI::isPrintingPaused()) {
    screen.setStatusMessage(F("Please pause print first"));
    return;
  }

  if (ExtUI::commandsInQueue()) {
    screen.setStatusMessage(F("Printer is busy"));
    return;
  }

  switch (BE16_P(val_ptr)) {
    default: return;
    case FILCHANGE_ACTION_LOAD_BUTTON:   is_loading = true;  break;
    case FILCHANGE_ACTION_UNLOAD_BUTTON: is_loading = false; break;
  }

  // Synchronous operation - disable back button
  is_running = true;
  screen.setSynchronousOperationStart();

  // Heat if necessary
  const celsius_t target = _MIN(nozzle_temperature, thermalManager.hotend_max_target(0));
  if (ExtUI::getActualTemp_celsius(ExtUI::E0) < target - (FEED_HEAT_HYSTERESIS)) {
    screen.setStatusMessage(F("Heating up..."));

    MString<16> cmd;
    cmd.setf(F("M109 S%i"), int(target));
    ExtUI::injectCommands(cmd);
    screen.runWhenQueueDone(changeFilament);
  }
  else
    changeFilament();
}

void FilamentLoadUnloadHandler::changeFilament() {
  screen.setStatusMessage(F("Filament load/unload..."));

  MString<32> cmd;
  #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    cmd.set(is_loading ? F("M701 L") : F("M702 U"), p_float_t(length, 1));
  #else
    // The queue is empty, so the E mode can be changed now
    saved_axis_relative = gcode.axis_relative;
    gcode.set_e_relative();
    cmd.set(F("G1 E"), p_float_t(is_loading ? length : -length, 1), F(" F"), int(MMS_TO_MMM(is_loading ? FEED_LOAD_FEEDRATE : FEED_UNLOAD_FEEDRATE)));
  #endif
  DEBUG_ECHOLNPGM("Injecting command: ", &cmd);
  ExtUI::injectCommands(cmd);
  screen.runWhenQueueDone(onChangeDone);
}

void FilamentLoadUnloadHandler::onChangeDone() {
  #if DISABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    gcode.axis_relative = saved_axis_relative;
  #endif

  if (screen.lcd_settings.display_sound) screen.buzzer(500, 100);

  is_running = false;
  screen.setSynchronousOperationFinish();
  screen.setStatusMessage(F("Filament load/unload complete"));
}

#endif // DGUS_LCD_UI_CREALITY_TOUCH && HAS_EXTRUDERS
