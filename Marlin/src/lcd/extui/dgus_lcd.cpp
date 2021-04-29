/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/dgus_lcd.cpp
 */

#include "../../inc/MarlinConfigPre.h"

#if HAS_DGUS_LCD

#include "ui_api.h"
#include "lib/dgus/DGUSDisplay.h"
#include "lib/dgus/DGUSDisplayDef.h"
#include "lib/dgus/DGUSScreenHandler.h"

namespace ExtUI {

  void onStartup() {
    dgusdisplay.InitDisplay();
    ScreenHandler.UpdateScreenVPData();
  }

  void onIdle() { ScreenHandler.loop(); }

  void onPrinterKilled(PGM_P const error, PGM_P const component) {
    ScreenHandler.sendinfoscreen(GET_TEXT(MSG_HALTED), error, NUL_STR, GET_TEXT(MSG_PLEASE_RESET), true, true, true, true);
    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_KILL);
    while (!ScreenHandler.loop());  // Wait while anything is left to be sent
  }

  void onMediaInserted() { TERN_(SDSUPPORT, ScreenHandler.SDCardInserted()); }
  void onMediaError()    { TERN_(SDSUPPORT, ScreenHandler.SDCardError()); }
  void onMediaRemoved()  { TERN_(SDSUPPORT, ScreenHandler.SDCardRemoved()); }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {}
  void onPrintTimerStarted() {}
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() {}
  void onFilamentRunout(const extruder_t extruder) {}

  void onUserConfirmRequired(const char * const msg) {
    if (msg) {
      ScreenHandler.sendinfoscreen(PSTR("Please confirm."), nullptr, msg, nullptr, true, true, false, true);
      ScreenHandler.SetupConfirmAction(setUserConfirmed);
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
    }
    else if (ScreenHandler.getCurrentScreen() == DGUSLCD_SCREEN_POPUP ) {
      ScreenHandler.SetupConfirmAction(nullptr);
      ScreenHandler.PopToOldScreen();
    }
  }

  void onStatusChanged(const char * const msg) { ScreenHandler.setstatusmessage(msg); }

  void onHomingStart() {}
  void onHomingComplete() {}
  void onPrintFinished() {}

  void onFactoryReset() {}

  void onStoreSettings(char *buff) {
    // Called when saving to EEPROM (i.e. M500). If the ExtUI needs
    // permanent data to be stored, it can write up to eeprom_data_size bytes
    // into buff.

    // Example:
    //  static_assert(sizeof(myDataStruct) <= eeprom_data_size);
    //  memcpy(buff, &myDataStruct, sizeof(myDataStruct));
  }

  void onLoadSettings(const char *buff) {
    // Called while loading settings from EEPROM. If the ExtUI
    // needs to retrieve data, it should copy up to eeprom_data_size bytes
    // from buff

    // Example:
    //  static_assert(sizeof(myDataStruct) <= eeprom_data_size);
    //  memcpy(&myDataStruct, buff, sizeof(myDataStruct));
  }

  void onPostprocessSettings() {
    // Called after loading or resetting stored settings
  }

  void onConfigurationStoreWritten(bool success) {
    // Called after the entire EEPROM has been written,
    // whether successful or not.
  }

  void onConfigurationStoreRead(bool success) {
    // Called after the entire EEPROM has been read,
    // whether successful or not.
  }

  #if HAS_MESH
    void onMeshLevelingStart() {}

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const_float_t zval) {
      // Called when any mesh points are updated
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const probe_state_t state) {
      // Called to indicate a special condition
    }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {
      // Called on resume from power-loss
      IF_DISABLED(DGUS_LCD_UI_MKS, ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POWER_LOSS));
    }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
      switch (rst) {
        case PID_BAD_EXTRUDER_NUM:
          ScreenHandler.setstatusmessagePGM(GET_TEXT(MSG_PID_BAD_EXTRUDER_NUM));
          break;
        case PID_TEMP_TOO_HIGH:
          ScreenHandler.setstatusmessagePGM(GET_TEXT(MSG_PID_TEMP_TOO_HIGH));
          break;
        case PID_TUNING_TIMEOUT:
          ScreenHandler.setstatusmessagePGM(GET_TEXT(MSG_PID_TIMEOUT));
          break;
        case PID_DONE:
          ScreenHandler.setstatusmessagePGM(GET_TEXT(MSG_PID_AUTOTUNE_DONE));
          break;
      }
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN);
    }
  #endif

  void onSteppersDisabled() {}
  void onSteppersEnabled()  {}
}

#endif // HAS_DGUS_LCD
