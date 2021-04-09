/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

/**
 * creality_dwin.cpp
 *
 * DWIN implementation for Marlin by Jacob Myers, 2021
 */

#include "../../inc/MarlinConfigPre.h"

#if BOTH(CREALITY_DWIN_EXTUI, EXTENSIBLE_UI)

#include "lib/creality_dwin/creality_dwin.h"
#include "ui_api.h"

namespace ExtUI {
  void onStartup() { CrealityDWIN.Startup(); }
  void onIdle() { CrealityDWIN.Update(); }
  void onPrinterKilled(PGM_P const error, PGM_P const component) { CrealityDWIN.Draw_Popup((char*)"Printer Kill Reason:", error, (char*)"Restart Required", Wait, ICON_BLTouch); }
  void onMediaInserted() { CrealityDWIN.SDCardInsert(); }
  void onMediaError() {}
  void onMediaRemoved() {}
  void onPlayTone(const uint16_t frequency, const uint16_t duration) {}
  void onPrintTimerStarted() { CrealityDWIN.Start_Print(isPrintingFromMedia()); }
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() { CrealityDWIN.Stop_Print(); }
  void onFilamentRunout(const extruder_t extruder) { CrealityDWIN.Popup_Handler(Runout); }
  void onUserConfirmRequired(const char * const msg) { CrealityDWIN.Confirm_Handler(msg); }
  void onStatusChanged(const char * const msg) { CrealityDWIN.Update_Status(msg); }

  void onHomingStart() {}
  void onHomingComplete() {}
  void onPrintFinished() {}

  void onFactoryReset() {}

  void onStoreSettings(char *buff) {
    CrealityDWIN.Save_Settings();
    memcpy(buff, &CrealityDWIN.eeprom_settings, min(sizeof(CrealityDWIN.eeprom_settings), eeprom_data_size));
  }

  void onLoadSettings(const char *buff) {
    memcpy(&CrealityDWIN.eeprom_settings, buff, min(sizeof(CrealityDWIN.eeprom_settings), eeprom_data_size));
    CrealityDWIN.Load_Settings();
  }

  void onConfigurationStoreWritten(bool success) {}

  void onConfigurationStoreRead(bool success) {}

  #if HAS_MESH
    void onMeshLevelingStart() {}

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float &zval) {
      // Called when any mesh points are updated
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const probe_state_t state) {
      // Called to indicate a special condition
    }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() { CrealityDWIN.Popup_Handler(Resume); }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      switch (rst) {
        case PID_BAD_EXTRUDER_NUM:
          CrealityDWIN.Confirm_Handler((char*)"Bad extruder number");
          break;
        case PID_TEMP_TOO_HIGH:
          CrealityDWIN.Confirm_Handler((char*)"Temp too high");
          break;
        case PID_TUNING_TIMEOUT:
          CrealityDWIN.Confirm_Handler((char*)"PID Timout");
          break;
        case PID_DONE:
          CrealityDWIN.Confirm_Handler((char*)"PID Done");
          break;
      }
    }
  #endif

  void onSteppersDisabled() {}
  void onSteppersEnabled()  {}
}

#endif // CREALITY_DWIN_EXTUI && EXTENSIBLE_UI
