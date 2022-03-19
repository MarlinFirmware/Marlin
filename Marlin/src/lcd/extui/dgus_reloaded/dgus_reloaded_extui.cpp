/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/dgus_reloaded/dgus_reloaded_extui.cpp
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_RELOADED)

#include "../ui_api.h"
#include "DGUSScreenHandler.h"

namespace ExtUI {

  void onStartup() { dgus_screen_handler.Init(); }

  void onIdle() {
    static bool processing = false;

    // Prevent recursion
    if (!processing) {
      processing = true;
      dgus_screen_handler.Loop();
      processing = false;
    }
  }

  void onPrinterKilled(FSTR_P const error, FSTR_P const component) {
    dgus_screen_handler.PrinterKilled(error, component);
  }

  void onMediaInserted() { TERN_(SDSUPPORT, dgus_screen_handler.SDCardInserted()); }
  void onMediaError()    { TERN_(SDSUPPORT, dgus_screen_handler.SDCardError()); }
  void onMediaRemoved()  { TERN_(SDSUPPORT, dgus_screen_handler.SDCardRemoved()); }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    dgus_screen_handler.PlayTone(frequency, duration);
  }

  void onPrintTimerStarted() {
    dgus_screen_handler.PrintTimerStarted();
  }

  void onPrintTimerPaused() {
    dgus_screen_handler.PrintTimerPaused();
  }

  void onPrintTimerStopped() {
    dgus_screen_handler.PrintTimerStopped();
  }

  void onFilamentRunout(const extruder_t extruder) {
    dgus_screen_handler.FilamentRunout(extruder);
  }

  void onUserConfirmRequired(const char * const msg) {
    dgus_screen_handler.UserConfirmRequired(msg);
  }

  void onStatusChanged(const char * const msg) {
    dgus_screen_handler.SetStatusMessage(msg);
  }

  void onHomingStart() {}
  void onHomingDone() {}
  void onPrintDone() {}

  void onFactoryReset() {
    dgus_screen_handler.SettingsReset();
  }

  void onStoreSettings(char *buff) {
    dgus_screen_handler.StoreSettings(buff);
  }

  void onLoadSettings(const char *buff) {
    dgus_screen_handler.LoadSettings(buff);
  }

  void onPostprocessSettings() {}

  void onConfigurationStoreWritten(bool success) {
    dgus_screen_handler.ConfigurationStoreWritten(success);
  }

  void onConfigurationStoreRead(bool success) {
    dgus_screen_handler.ConfigurationStoreRead(success);
  }

  #if HAS_MESH
    void onLevelingStart() {}
    void onLevelingDone() {}

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const_float_t zval) {
      dgus_screen_handler.MeshUpdate(xpos, ypos);
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const probe_state_t state) {
      if (state == G29_POINT_FINISH)
        dgus_screen_handler.MeshUpdate(xpos, ypos);
    }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {
      // Called on resume from power-loss
      dgus_screen_handler.PowerLossResume();
    }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
      dgus_screen_handler.PidTuning(rst);
    }
  #endif

  void onSteppersDisabled() {}
  void onSteppersEnabled()  {}
}

#endif // DGUS_LCD_UI_RELOADED
