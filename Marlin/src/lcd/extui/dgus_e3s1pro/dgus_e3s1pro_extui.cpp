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

/**
 * lcd/extui/dgus_e3s1pro/dgus_e3s1pro_extui.cpp
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_E3S1PRO)

#include "../ui_api.h"
#include "DGUSScreenHandler.h"

#define DEBUG_OUT 1
#include "../../../core/debug_out.h"

namespace ExtUI {

  void onStartup() {
    screen.init();
  }

  void onIdle() {
    static bool processing = false;

    // Prevent recursion
    if (!processing) {
      processing = true;
      screen.loop();
      processing = false;
    }
  }

  void onPrinterKilled(FSTR_P const error, FSTR_P const component) {
    screen.printerKilled(error, component);
  }

  void onMediaInserted() { TERN_(HAS_MEDIA, screen.sdCardInserted()); }
  void onMediaError()    { TERN_(HAS_MEDIA, screen.sdCardError()); }
  void onMediaRemoved()  { TERN_(HAS_MEDIA, screen.sdCardRemoved()); }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    screen.playTone(frequency, duration);
  }

  void onPrintTimerStarted() {
    screen.printTimerStarted();
  }

  void onPrintTimerPaused() {
    screen.printTimerPaused();
  }

  void onPrintTimerStopped() {
    screen.printTimerStopped();
  }

  void onFilamentRunout(const extruder_t extruder) {
    screen.filamentRunout(extruder);
  }

  void onUserConfirmRequired(const char * const msg) {
    screen.userConfirmRequired(msg);
  }

  void onStatusChanged(const char * const msg) {
    screen.setStatusMessage(msg);
  }

  void onHomingStart() {}

  void onHomingDone() {
    screen.homingDone();
  }

  void onPrintDone() {}

  void onFactoryReset() {
    screen.settingsReset();
  }

  void onStoreSettings(char *buff) {
    screen.storeSettings(buff);
  }

  void onLoadSettings(const char *buff) {
    screen.loadSettings(buff);
  }

  void onPostprocessSettings() {}

  void onSettingsStored(const bool success) {
    screen.configurationStoreWritten(success);
  }

  void onSettingsLoaded(const bool success) {
    screen.configurationStoreRead(success);
  }

  #if HAS_MESH
    void onLevelingStart() {
      screen.levelingStart();
    }

    void onLevelingDone() {
      screen.levelingEnd();
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const_float_t zval) {
      screen.meshUpdate(xpos, ypos);
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const probe_state_t state) { }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onSetPowerLoss(const bool onoff) {
      // Called when power-loss is enabled/disabled
    }
    void onPowerLoss() {
      // Called when power-loss state is detected
    }
    void onPowerLossResume() {
      // Called on resume from power-loss
      screen.powerLossResume();
    }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
      screen.pidTuning(rst);
    }
  #endif

  void onSteppersDisabled() {
    screen.steppersStatusChanged(false);
  }

  void onSteppersEnabled() {
    screen.steppersStatusChanged(true);
  }
}

#endif // DGUS_LCD_UI_RELOADED
