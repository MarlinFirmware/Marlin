/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/sermoon_d1/sermoon_d1_extui.cpp
 *
 * ExtUI callbacks for the Creality Sermoon D1 stock touchscreen
 */

#include "../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_SERMOON_D1

#include "sermoon_d1_rts.h"

namespace ExtUI {

  static constexpr uint8_t settings_version = 1;

  void onStartup() { rts.onStartup(); }

  void onIdle() { rts.onIdle(); }

  // The screen has one error page, with a short message
  void onPrinterKilled(FSTR_P const error, FSTR_P const) { rts.showError(error); }

  void onMediaMounted() { rts.mediaInserted(); }
  void onMediaError() { rts.mediaRemoved(); }
  void onMediaRemoved() { rts.mediaRemoved(); }

  void onPrintTimerStarted() { rts.printStarted(); }
  void onPrintTimerPaused() { rts.printPaused(); }
  void onPrintTimerStopped() { rts.printStopped(); }
  void onPrintDone() { rts.printFinished(); }

  void onFilamentRunout(const extruder_t) { rts.filamentRunout(); }

  void onUserConfirmRequired(const char * const msg) {
    if (msg && strcmp_P(msg, GET_TEXT(MSG_NOZZLE_PARKED)) == 0)
      rts.gotoPage(RTS::PAGE_PAUSED);
  }
  void onUserConfirmRequired(const int, const char * const cstr, FSTR_P const) { onUserConfirmRequired(cstr); }
  void onUserConfirmRequired(const int, FSTR_P const fstr, FSTR_P const) { onUserConfirmRequired(fstr); }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void onPauseMode(const PauseMessage message, const PauseMode, const uint8_t) { rts.pauseMessage(message); }
  #endif

  void onHomingStart() { rts.homingStarted(); }
  void onHomingDone() { rts.homingFinished(); }

  void onSteppersDisabled() { rts.writeWord(VP_MOTOR_ICON, 0); }
  void onSteppersEnabled() { rts.writeWord(VP_MOTOR_ICON, 1); }

  void onFactoryReset() {
    RTS::settings.version  = settings_version;
    RTS::settings.language = 2;   // English
  }

  static_assert(sizeof(RTS::settings_t) <= eeprom_data_size, "Insufficient space in EEPROM for UI parameters");

  void onStoreSettings(char *buff) {
    memcpy(buff, &RTS::settings, sizeof(RTS::settings_t));
  }

  void onLoadSettings(const char *buff) {
    RTS::settings_t loaded;
    memcpy(&loaded, buff, sizeof(RTS::settings_t));
    if (loaded.version == settings_version)
      RTS::settings = loaded;
    else
      onFactoryReset();
  }

  void onSettingsLoaded(const bool) {
    rts.sendZOffset();
    rts.sendMesh();
  }

  #if HAS_LEVELING
    void onLevelingDone() { rts.levelingFinished(); }
    #if ENABLED(PREHEAT_BEFORE_LEVELING)
      celsius_t getLevelingBedTemp() { return LEVELING_BED_TEMP; }
    #endif
  #endif

  #if HAS_MESH
    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) { rts.meshPointProbed(xpos, ypos, zval); }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() { rts.powerLossDetected(); }
  #endif

  #if HAS_PID_HEATING
    void onStartM303(const int, const heater_id_t, const celsius_t) {}
  #endif

} // ExtUI

#endif // DGUS_LCD_UI_SERMOON_D1
