/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * extui_dgus_lcd.cpp
 *
 * DGUS implementation for Marlin by coldtobi, Feb-May 2019
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD)

#include "extensible_ui/ui_api.h"
#include "extensible_ui/lib/dgus/DGUSDisplay.h"
#include "extensible_ui/lib/dgus/DGUSDisplayDefinition.h"

namespace ExtUI {

  void onStartup() {
    dgusdisplay.InitDisplay();
    ScreenHandler.UpdateScreenVPData();
  }

  void onIdle() { ScreenHandler.loop(); }

  void onPrinterKilled(PGM_P error, PGM_P component) {
    extern const char NUL_STR[];
    ScreenHandler.sendinfoscreen(GET_TEXT(MSG_HALTED), error, NUL_STR, GET_TEXT(MSG_PLEASE_RESET), true, true, true, true);
    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_KILL);
    while (!ScreenHandler.loop());  // Wait while anything is left to be sent
  }

  void onMediaInserted() { ScreenHandler.SDCardInserted(); }
  void onMediaError()    { ScreenHandler.SDCardError(); }
  void onMediaRemoved()  { ScreenHandler.SDCardRemoved(); }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {}
  void onPrintTimerStarted() {}
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() {}
  void onFilamentRunout(const extruder_t extruder) {}

  void onUserConfirmRequired(const char * const msg) {
    if (msg) {
      ScreenHandler.sendinfoscreen(PSTR("Please confirm."), nullptr, msg, nullptr, true, true, false, true);
      ScreenHandler.SetupConfirmAction(ExtUI::setUserConfirmed);
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
    }
    else if (ScreenHandler.getCurrentScreen() == DGUSLCD_SCREEN_POPUP ) {
      ScreenHandler.SetupConfirmAction(nullptr);
      ScreenHandler.PopToOldScreen();
    }
  }

  void onStatusChanged(const char * const msg) { ScreenHandler.setstatusmessage(msg); }

  void onFactoryReset() {}
  void onLoadSettings() {}
  void onStoreSettings() {}
}

#endif // DGUS_LCD
