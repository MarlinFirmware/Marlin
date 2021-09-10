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

#define DEBUG_OUT 1
#include "../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "ui_api.h"
#include "../marlinui.h"
#include "dgus_creality/DGUSDisplay.h"
#include "dgus_creality/DGUSDisplayDef.h"
#include "dgus_creality/DGUSScreenHandler.h"
#include "dgus_creality/creality_touch/PIDHandler.h"
#include "dgus_creality/creality_touch/MeshValidationHandler.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
#endif

extern const char NUL_STR[];

namespace ExtUI {

  void onStartup() {
    ScreenHandler.Init();
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
    
    ScreenHandler.KillScreenCalled();
    while (!ScreenHandler.loop());  // Wait while anything is left to be sent
}

  void onMediaInserted() { TERN_(SDSUPPORT, ScreenHandler.SDCardInserted()); }
  void onMediaError()    { TERN_(SDSUPPORT, ScreenHandler.SDCardError()); }
  void onMediaRemoved()  { TERN_(SDSUPPORT, ScreenHandler.SDCardRemoved()); }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    if (ScreenHandler.getCurrentScreen() == DGUSLCD_SCREEN_FEED) {
        // We're in the feed (load filament) workflow - no beep - there is no confirmation
        return;
    }

    ScreenHandler.Buzzer(frequency, duration);
  }

bool hasPrintTimer = false;

  void onPrintTimerStarted() {
    hasPrintTimer = true;

    if (!IS_SD_FILE_OPEN() && !(PrintJobRecovery::valid() && PrintJobRecovery::exists())) {
      ScreenHandler.SetPrintingFromHost();
    }
    
#if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    ui.progress_reset();
#endif

    ScreenHandler.SetViewMeshLevelState();
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
    #ifdef FILAMENT_RUNOUT_SCRIPT
    // Only navigate to filament runout screen when we don't use M600 for changing the filament - otherwise it gets confusing for the user
    if (strcmp_P(FILAMENT_RUNOUT_SCRIPT, PSTR("M600")) != 0) {
      ScreenHandler.FilamentRunout();
    }
    #endif
  }

  void onUserConfirmed() {
    DEBUG_ECHOLN("User confirmation invoked");

#if M600_PURGE_MORE_RESUMABLE
    setPauseMenuResponse(PAUSE_RESPONSE_RESUME_PRINT);
#endif
    ExtUI::setUserConfirmed();
  }

  void onUserConfirmRequired(const char * const msg) {
    if (msg) {
      DEBUG_ECHOLNPAIR("User confirmation requested: ", msg);

      if (ScreenHandler.getCurrentScreen() == DGUSLCD_SCREEN_FEED) {
        // We're in the feed (load filament) workflow - immediately assume confirmed
        onUserConfirmed();
        return;
      }

      ScreenHandler.setstatusmessagePGM(msg);
      ScreenHandler.sendinfoscreen(PSTR("Confirmation required"), msg, NUL_STR, PSTR("Ok"), true, true, false, true);

      if (ExtUI::isPrinting()) {
        ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_PAUSED);
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
    ScreenHandler.StoreSettings(buff);
  }

  void onLoadSettings(const char *buff) {
    ScreenHandler.LoadSettings(buff);
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
    void onMeshLevelingStart() {
      ScreenHandler.OnMeshLevelingStart();
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
      ScreenHandler.OnMeshLevelingUpdate(xpos, ypos, zval);
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const ExtUI::probe_state_t state) {
      ScreenHandler.OnMeshLevelingUpdate(xpos, ypos, 0);
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
      switch (rst) {
        case PID_STARTED:
          // It has no use switching to the PID screen. It really isn't that informative.
          break;
        case PID_BAD_EXTRUDER_NUM:
          PIDHandler::result_message = GET_TEXT(MSG_PID_BAD_EXTRUDER_NUM);
          ScreenHandler.setstatusmessagePGM(PIDHandler::result_message);
          break;
        case PID_TEMP_TOO_HIGH:
          PIDHandler::result_message = GET_TEXT(MSG_PID_TEMP_TOO_HIGH);
          ScreenHandler.setstatusmessagePGM(PIDHandler::result_message);
          break;
        case PID_TUNING_TIMEOUT:
          PIDHandler::result_message = GET_TEXT(MSG_PID_TIMEOUT);
          ScreenHandler.setstatusmessagePGM(PIDHandler::result_message);
        break;
        case PID_DONE:
          PIDHandler::result_message = GET_TEXT(MSG_PID_AUTOTUNE_DONE);
          ScreenHandler.setstatusmessagePGM(PIDHandler::result_message);
        break;
      }
    }
  #endif

  void onSteppersDisabled() {
  }

  void onSteppersEnabled() {
  }

  void onMeshValidationStarting() {
    MeshValidationHandler::OnMeshValidationStart();
  }

  void onMeshValidationFinished() {
    MeshValidationHandler::OnMeshValidationFinish();
  }
}
#endif // HAS_DGUS_LCD
