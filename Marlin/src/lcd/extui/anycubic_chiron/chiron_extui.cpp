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
 * lcd/extui/anycubic_chiron/chiron_extui.cpp
 *
 * Anycubic Chiron TFT support for Marlin
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_CHIRON)

#include "../ui_api.h"
#include "chiron_tft.h"

using namespace Anycubic;

namespace ExtUI {

  void onStartup() { chiron.startup(); }

  void onIdle() { chiron.idleLoop(); }

  void onPrinterKilled(FSTR_P const error, FSTR_P const component) {
    chiron.printerKilled(error, component);
  }

  void onMediaMounted() { chiron.mediaEvent(AC_media_inserted); }
  void onMediaError()   { chiron.mediaEvent(AC_media_error);    }
  void onMediaRemoved() { chiron.mediaEvent(AC_media_removed);  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration/*=0*/) {
    #if ENABLED(SPEAKER)
      ::tone(BEEPER_PIN, frequency, duration);
    #endif
  }

  void onPrintTimerStarted() { chiron.timerEvent(AC_timer_started); }
  void onPrintTimerPaused()  { chiron.timerEvent(AC_timer_paused);  }
  void onPrintTimerStopped() { chiron.timerEvent(AC_timer_stopped); }

  void onFilamentRunout(const extruder_t)            { chiron.filamentRunout(); }

  void onUserConfirmRequired(const char * const msg) { chiron.confirmationRequest(msg); }

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

  void onStatusChanged(const char * const msg)       { chiron.statusChange(msg); }

  #if ENABLED(POWER_LOSS_RECOVERY)
    // Called on resume from power-loss
    void onPowerLossResume() { chiron.powerLossRecovery(); }
  #endif
}

#endif // ANYCUBIC_LCD_CHIRON
