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
 * lcd/extui/dgus/dgus_extui.cpp
 */

#include "../../../inc/MarlinConfigPre.h"

#if HAS_DGUS_LCD_CLASSIC

#include "../ui_api.h"
#include "DGUSDisplay.h"
#include "DGUSDisplayDef.h"
#include "DGUSScreenHandler.h"

namespace ExtUI {

  #if DGUS_LCD_UI_CREALITY_TOUCH

    // The CR-6 touch screen has dedicated screens for some errors
    static DGUS_ScreenID killScreenFor(FSTR_P const error) {
      PGM_P const err = FTOP(error);
      if (strcmp_P(err, GET_TEXT(MSG_ERR_MAXTEMP)) == 0 || strcmp_P(err, GET_TEXT(MSG_ERR_THERMAL_RUNAWAY)) == 0)
        return DGUS_SCREEN_THERMAL_RUNAWAY;
      if (strcmp_P(err, GET_TEXT(MSG_ERR_HEATING_FAILED)) == 0)
        return DGUS_SCREEN_HEATING_FAILED;
      if (strcmp_P(err, GET_TEXT(MSG_ERR_MINTEMP)) == 0)
        return DGUS_SCREEN_THERMISTOR_ERROR;
      return DGUS_SCREEN_KILL;
    }

    // Confirm without a popup, e.g. while in the feed (load filament) workflow
    static void onCrealityUserConfirmed() {
      TERN_(M600_PURGE_MORE_RESUMABLE, setPauseMenuResponse(PAUSE_RESPONSE_RESUME_PRINT));
      setUserConfirmed();
    }

  #endif

  void onStartup() {
    dgus.initDisplay();
    screen.updateScreenVPData();
  }

  void onIdle() { screen.loop(); }

  void onPrinterKilled(FSTR_P const error, FSTR_P const) {
    #if DGUS_LCD_UI_MKS
      screen.sendInfoScreenMKS(GET_TEXT_F(MSG_HALTED), error, nullptr, GET_TEXT_F(MSG_PLEASE_RESET), mks_language_index);
    #elif DGUS_LCD_UI_CREALITY_TOUCH
      screen.sendInfoScreen(GET_TEXT_F(MSG_HALTED), error, GET_TEXT_F(MSG_PLEASE_RESET), FPSTR(NUL_STR));
    #else
      screen.sendInfoScreen(GET_TEXT_F(MSG_HALTED), error, FPSTR(NUL_STR), GET_TEXT_F(MSG_PLEASE_RESET));
    #endif

    #if DGUS_LCD_UI_CREALITY_TOUCH
      screen.gotoScreen(killScreenFor(error));
      screen.killScreenCalled();
    #else
      screen.gotoScreen(DGUS_SCREEN_KILL);
    #endif
    while (!screen.loop());  // Wait while anything is left to be sent
  }

  void onMediaMounted() { TERN_(HAS_MEDIA, screen.sdCardInserted()); }
  void onMediaError()   { TERN_(HAS_MEDIA, screen.sdCardError()); }
  void onMediaRemoved() { TERN_(HAS_MEDIA, screen.sdCardRemoved()); }

  void onUserConfirmRequired(const char * const msg) {
    if (msg) {
      #if DGUS_LCD_UI_CREALITY_TOUCH
        // In the feed (load filament) workflow assume immediate confirmation
        if (screen.getCurrentScreen() == DGUS_SCREEN_FEED) {
          onCrealityUserConfirmed();
          return;
        }
        // The screen change hook confirms when the popup is closed
        screen.setStatusMessage(msg);
        screen.sendInfoScreen_P(PSTR("Confirmation required"), msg, NUL_STR, PSTR("Ok"), true, false, true, true);
        screen.gotoScreen(screen.isPrintJobActive() ? DGUS_SCREEN_PRINT_PAUSED : DGUS_SCREEN_POPUP);
      #else
        #if DGUS_LCD_UI_MKS
          screen.sendInfoScreenMKS(F("Please confirm."), nullptr, msg, nullptr, mks_language_index);
        #else
          screen.sendInfoScreen(F("Please confirm."), nullptr, msg, nullptr, true, false, false, false);
        #endif
        screen.setupConfirmAction(setUserConfirmed);
        screen.gotoScreen(DGUS_SCREEN_POPUP);
      #endif
    }
    else if (screen.getCurrentScreen() == DGUS_SCREEN_POPUP) {
      #if DGUS_LCD_UI_CREALITY_TOUCH
        screen.setStatusMessage_P(nullptr);
      #else
        screen.setupConfirmAction(nullptr);
      #endif
      screen.popToOldScreen();
    }
  }

  // For fancy LCDs include an icon ID, message, and translated button title
  void onUserConfirmRequired(const int, const char * const cstr, FSTR_P const) {
    onUserConfirmRequired(cstr);
  }
  void onUserConfirmRequired(const int, FSTR_P const fstr, FSTR_P const) {
    onUserConfirmRequired(fstr);
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void onPauseMode(
      const PauseMessage message,
      const PauseMode mode/*=PAUSE_MODE_SAME*/,
      const uint8_t extruder/*=motion.extruder*/
    ) {
      stdOnPauseMode(message, mode, extruder);
    }
  #endif

  void onStatusChanged(const char * const msg) { screen.setStatusMessage(msg); }

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {
      // Called on resume from power-loss
      #if DGUS_LCD_UI_CREALITY_TOUCH
        screen.onPowerLossResume();
      #else
        IF_DISABLED(DGUS_LCD_UI_MKS, screen.gotoScreen(DGUS_SCREEN_POWER_LOSS));
      #endif
    }
  #endif

  #if HAS_PID_HEATING && DISABLED(DGUS_LCD_UI_CREALITY_TOUCH)
    void onPIDTuning(const pidresult_t rst) {
      // Called for temperature PID tuning result
      switch (rst) {
        case PID_STARTED:
        case PID_BED_STARTED:
        case PID_CHAMBER_STARTED:
          screen.setStatusMessage(GET_TEXT_F(MSG_PID_AUTOTUNE));
          break;
        case PID_BAD_HEATER_ID:
          screen.setStatusMessage(GET_TEXT_F(MSG_PID_BAD_HEATER_ID));
          break;
        case PID_TEMP_TOO_HIGH:
          screen.setStatusMessage(GET_TEXT_F(MSG_PID_TEMP_TOO_HIGH));
          break;
        case PID_TUNING_TIMEOUT:
          screen.setStatusMessage(GET_TEXT_F(MSG_PID_TIMEOUT));
          break;
        case PID_DONE:
          screen.setStatusMessage(GET_TEXT_F(MSG_PID_AUTOTUNE_DONE));
          break;
      }
      screen.gotoScreen(DGUS_SCREEN_MAIN);
    }
  #endif

  #if ENABLED(MPC_AUTOTUNE)
    void onMPCTuning(const mpcresult_t rst) {
      // Called for temperature MPC tuning result
      switch (rst) {
        case MPC_STARTED:
          screen.setStatusMessage(GET_TEXT_F(MSG_MPC_AUTOTUNE));
          break;
        case MPC_TEMP_ERROR:
          //screen.setStatusMessage(GET_TEXT_F(MSG_MPC_TEMP_ERROR));
          break;
        case MPC_INTERRUPTED:
          //screen.setStatusMessage(GET_TEXT_F(MSG_MPC_INTERRUPTED));
          break;
        case MPC_DONE:
          //screen.setStatusMessage(GET_TEXT_F(MSG_MPC_AUTOTUNE_DONE));
          break;
      }
      screen.gotoScreen(DGUS_SCREEN_MAIN);
    }
  #endif
}

#endif // HAS_DGUS_LCD_CLASSIC
