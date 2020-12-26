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
 * dgus_creality_lcd.cpp
 *
 * DGUS implementation written by coldtobi in 2019 for Marlin
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "ui_api.h"
#include "lib/dgus_creality/DGUSDisplay.h"
#include "lib/dgus_creality/DGUSDisplayDef.h"
#include "lib/dgus_creality/DGUSScreenHandler.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
#endif

extern const char NUL_STR[];

namespace ExtUI {

  void onStartup() {
    dgusdisplay.InitDisplay();
    ScreenHandler.UpdateScreenVPData();
  }

  void onIdle() { ScreenHandler.loop(); }

  void onPrinterKilled(PGM_P const error, PGM_P const component) {
    ScreenHandler.sendinfoscreen(GET_TEXT(MSG_HALTED), error, GET_TEXT(MSG_PLEASE_RESET), GET_TEXT(MSG_PLEASE_RESET), true, true, true, true);

    if (strcmp_P(error, GET_TEXT(MSG_ERR_MAXTEMP)) == 0 || strcmp_P(error, GET_TEXT(MSG_THERMAL_RUNAWAY)) == 0)     {
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_THERMAL_RUNAWAY);
    } else if (strcmp_P(error, GET_TEXT(MSG_HEATING_FAILED_LCD)) == 0) {
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_HEATING_FAILED);
    }else if (strcmp_P(error, GET_TEXT(MSG_ERR_MINTEMP)) == 0) {
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_THERMISTOR_ERROR);
    } else {
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_KILL);
    }

    while (!ScreenHandler.loop());  // Wait while anything is left to be sent
  }

  void onMediaInserted() { TERN_(SDSUPPORT, ScreenHandler.SDCardInserted()); }
  void onMediaError()    { TERN_(SDSUPPORT, ScreenHandler.SDCardError()); }
  void onMediaRemoved()  { TERN_(SDSUPPORT, ScreenHandler.SDCardRemoved()); }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    ScreenHandler.Buzzer(frequency, duration);
  }

bool hasPrintTimer = false;

  void onPrintTimerStarted() {
    hasPrintTimer = true;

    if (!ExtUI::isPrintingFromMedia() && !(PrintJobRecovery::valid() && PrintJobRecovery::exists())) {
      ScreenHandler.SetPrintingFromHost();
    }

    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_RUNNING);
  }

  void onPrintTimerPaused() {
    // Handle M28 Pause SD print - But only if we're not waiting on a user
    if (ExtUI::isPrintingFromMediaPaused() && ScreenHandler.getCurrentScreen() == DGUSLCD_SCREEN_PRINT_RUNNING && !ExtUI::isWaitingOnUser()) {
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_PAUSED);
    }
  }

  void onPrintTimerStopped() {
    hasPrintTimer = false;

    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_FINISH);
  }

  void onFilamentRunout(const extruder_t extruder) {
    // Only navigate to filament runout screen when we don't use M600 for changing the filament - otherwise it gets confusing for the user
    if (strcmp_P(FILAMENT_RUNOUT_SCRIPT, PSTR("M600")) != 0) {
      ScreenHandler.FilamentRunout();
    }
  }

  void onUserConfirmed() {
    DEBUG_ECHOLN("User confirmation invoked");

    ExtUI::setUserConfirmed();
  }

  void onUserConfirmRequired(const char * const msg) {
    if (msg) {
      DEBUG_ECHOLNPAIR("User confirmation requested: ", msg);

      ScreenHandler.setstatusmessagePGM(msg);
      ScreenHandler.sendinfoscreen(PSTR("Confirmation required"), msg, NUL_STR, PSTR("Ok"), true, true, false, true);

      if (ExtUI::isPrinting()) {
        ScreenHandler.GotoScreen(DGUSLCD_SCREEN_DIALOG_PAUSE);
      } else {
        ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
      }
    }
    else if (ScreenHandler.getCurrentScreen() == DGUSLCD_SCREEN_POPUP) {
      DEBUG_ECHOLNPAIR("User confirmation canceled");

      ScreenHandler.setstatusmessagePGM(nullptr);
      ScreenHandler.PopToOldScreen();
    }
  }

  void onStatusChanged(const char * const msg) { ScreenHandler.setstatusmessage(msg); }

  void onFactoryReset() {
    ScreenHandler.OnFactoryReset();
  }

   void onHomingStart() {
    ScreenHandler.OnHomingStart();
  }

  void onHomingComplete() {
    ScreenHandler.OnHomingComplete();
  }

  void onPrintFinished() {
    ScreenHandler.OnPrintFinished();
  }
  
  void onStoreSettings(char *buff) {
    // Called when saving to EEPROM (i.e. M500). If the ExtUI needs
    // permanent data to be stored, it can write up to eeprom_data_size bytes
    // into buff.

    // Example:
    //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
    //  memcpy(buff, &myDataStruct, sizeof(myDataStruct));

    ScreenHandler.StoreSettings(buff);
  }

  void onLoadSettings(const char *buff) {
    // Called while loading settings from EEPROM. If the ExtUI
    // needs to retrieve data, it should copy up to eeprom_data_size bytes
    // from buff

    // Example:
    //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
    //  memcpy(&myDataStruct, buff, sizeof(myDataStruct));

    ScreenHandler.LoadSettings(buff);
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
    void onMeshLevelingStart() {
      ScreenHandler.OnMeshLevelingStart();
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
      ScreenHandler.OnMeshLevelingUpdate(xpos, ypos);
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const ExtUI::probe_state_t state) {
      ScreenHandler.OnMeshLevelingUpdate(xpos, ypos);
    }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {
      // Called on resume from power-loss
      ScreenHandler.OnPowerlossResume();
    }
  #endif


  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
      // switch (rst) {
      //   case PID_BAD_EXTRUDER_NUM:
      //     ScreenHandler.setstatusmessagePGM(GET_TEXT(MSG_PID_BAD_EXTRUDER_NUM));
      //     break;
      //   case PID_TEMP_TOO_HIGH:
      //     ScreenHandler.setstatusmessagePGM(GET_TEXT(MSG_PID_TEMP_TOO_HIGH));
      //     break;
      //   case PID_TUNING_TIMEOUT:
      //     ScreenHandler.setstatusmessagePGM(GET_TEXT(MSG_PID_TIMEOUT));
      //     break;
      //   case PID_DONE:
      //     ScreenHandler.setstatusmessagePGM(GET_TEXT(MSG_PID_AUTOTUNE_DONE));
      //     break;
      // }
      // ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN);
    }
  #endif

  void onSteppersDisabled() {
    ScreenHandler.HandleStepperState(false);
  }

  void onSteppersEnabled() {
    ScreenHandler.HandleStepperState(true);
  }

}
#endif // HAS_DGUS_LCD
