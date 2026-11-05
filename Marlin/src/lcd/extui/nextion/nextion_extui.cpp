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
 * lcd/extui/nextion_lcd.cpp
 *
 * Nextion TFT support for Marlin
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(NEXTION_TFT)

#include "../ui_api.h"
#include "nextion_tft.h"

namespace ExtUI {

  void onStartup() { nextion.startup();  }
  void onIdle() { nextion.idleLoop(); }
  void onPrinterKilled(FSTR_P const error, FSTR_P const component) { nextion.printerKilled(error, component); }

  void onUserConfirmRequired(const char * const msg) { nextion.confirmationRequest(msg); }

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

  void onStatusChanged(const char * const msg) { nextion.statusChange(msg); }

  void onPrintDone() { nextion.PrintFinished(); }

  #if HAS_PID_HEATING
    void onPIDTuning(const pidresult_t rst) {
      // Called for temperature PID tuning result
      nextion.panelInfo(37);
    }
  #endif
}

#endif // NEXTION_TFT
