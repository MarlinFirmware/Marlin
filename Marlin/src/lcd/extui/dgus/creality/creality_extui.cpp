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
 * lcd/extui/dgus/creality/creality_extui.cpp
 *
 * ExtUI event handlers used only by the Creality CR-6 touch screen.
 * Handlers shared with the other classic DGUS displays are in ../dgus_extui.cpp.
 * Any handler not defined here falls back to the weak default in ui_api.cpp.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH

#include "../../ui_api.h"
#include "../../../marlinui.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"
#include "../../../../sd/cardreader.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if ENABLED(PIDTEMP)
  #include "PIDHandler.h"
#endif

#if ENABLED(G26_MESH_VALIDATION)
  #include "MeshValidationHandler.h"
#endif

// The board has no beeper, so M300 and UI feedback go to the touch screen's buzzer
void MarlinUI::buzz(const long duration, const uint16_t freq/*=0*/) {
  // No beep in the feed (load filament) workflow - there is no confirmation
  if (sound_on && screen.getCurrentScreen() != DGUS_SCREEN_FEED)
    screen.buzzer(freq, duration);
}

namespace ExtUI {

  void onPrintTimerStarted() {
    #if HAS_MEDIA
      if (!card.isFileOpen() && TERN1(POWER_LOSS_RECOVERY, !(recovery.valid() && recovery.exists())))
        screen.setPrintingFromHost();
    #endif

    // M109 starts the job timer too (PRINTJOB_TIMER_AUTOSTART). Stay on the screen of an
    // operation that heats up, such as filament load/unload, instead of showing a print.
    if (screen.hasCurrentSynchronousOperation()) return;

    TERN_(SET_PROGRESS_PERCENT, ui.progress_reset());

    screen.setPrintJobScreens(true);
    screen.setViewMeshLevelState();
    screen.gotoScreen(DGUS_SCREEN_PRINT_RUNNING);
  }

  void onPrintTimerPaused() {
    // Handle M25 Pause SD print, but only if we're not waiting on a user
    if (isPrintingFromMediaPaused() && screen.getCurrentScreen() == DGUS_SCREEN_PRINT_RUNNING && !awaitingUserConfirm())
      screen.gotoScreen(DGUS_SCREEN_PRINT_PAUSED);
  }

  void onPrintTimerStopped() {
    // Only a job that was shown as a print has a finish screen
    if (!screen.isPrintJobActive()) return;
    screen.setPrintJobScreens(false);
    screen.gotoScreen(DGUS_SCREEN_PRINT_FINISH);
  }

  void onSettingsStored(const bool success) {
    screen.setStatusMessage(success ? F("Settings saved") : F("Failed to save settings"));
  }

  void onPrintDone() { screen.onPrintDone(); }

  void onFilamentRunout(const extruder_t) {
    #ifdef FILAMENT_RUNOUT_SCRIPT
      // Only navigate to filament runout screen when we don't use M600 for changing the filament - otherwise it gets confusing for the user
      if (strcmp_P(FILAMENT_RUNOUT_SCRIPT, PSTR("M600")) != 0) screen.filamentRunout();
    #endif
  }

  void onHomingStart() { screen.onHomingStart(); }
  void onHomingDone()  { screen.onHomingDone(); }

  void onFactoryReset() { screen.onFactoryReset(); }

  void onStoreSettings(char *buff) { screen.storeSettings(buff); }
  void onLoadSettings(const char *buff) { screen.loadSettings(buff); }

  #if HAS_MESH
    void onLevelingStart() { screen.onLevelingStart(); }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
      screen.onLevelingUpdate(xpos, ypos, zval);
    }

    #if ENABLED(G26_MESH_VALIDATION)
      // Only the start and end of G26 are of interest
      void onMeshUpdate(const int8_t xpos, const int8_t ypos, const probe_state_t state) {
        switch (state) {
          case G26_START:  MeshValidationHandler::onMeshValidationStart(); break;
          case G26_FINISH: MeshValidationHandler::onMeshValidationFinish(); break;
          default: break;
        }
      }
    #endif
  #endif

  #if HAS_PID_HEATING
    void onPIDTuning(const pidresult_t rst) {
      // Called for temperature PID tuning result
      static bool tuning_failed = false;
      FSTR_P msg;
      switch (rst) {
        default: // It has no use switching to the PID screen. It really isn't that informative.
          tuning_failed = false;
          return;
        case PID_BAD_HEATER_ID:  msg = GET_TEXT_F(MSG_PID_BAD_HEATER_ID); break;
        case PID_TEMP_TOO_HIGH:  msg = GET_TEXT_F(MSG_PID_TEMP_TOO_HIGH); break;
        case PID_TUNING_TIMEOUT: msg = GET_TEXT_F(MSG_PID_TIMEOUT); break;
        case PID_DONE:
          // A failed tune also ends with PID_DONE. Keep the error message.
          if (tuning_failed) { tuning_failed = false; return; }
          msg = GET_TEXT_F(MSG_PID_AUTOTUNE_DONE);
          break;
      }
      tuning_failed = (rst != PID_DONE);
      screen.setStatusMessage(msg);
      TERN_(PIDTEMP, PIDHandler::onTuningResult(rst == PID_DONE));
    }
  #endif

}

#endif // DGUS_LCD_UI_CREALITY_TOUCH
