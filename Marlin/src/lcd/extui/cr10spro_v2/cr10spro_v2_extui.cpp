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
 * lcd/extui/cr10spro_v2/cr10spro_v2_extui.cpp
 *
 * ExtUI callbacks for the Creality CR-10S Pro V2 stock touchscreen
 */

#include "../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CR10SPROV2

#include "cr10spro_v2_rts.h"

namespace ExtUI {

  static constexpr uint8_t settings_version = 2;

  void onStartup() { rts.onStartup(); }
  void onIdle() { rts.onIdle(); }

  void onPrinterKilled(FSTR_P const error, FSTR_P const) {
    PGM_P const e = FTOP(error);
    if (e == GET_TEXT(MSG_ERR_HEATING_FAILED))
      rts.gotoPage(RTS::PAGE_ERR_HEATING);
    else if (e == GET_TEXT(MSG_ERR_MAXTEMP) || e == GET_TEXT(MSG_ERR_MINTEMP) || e == GET_TEXT(MSG_ERR_REDUNDANT_TEMP))
      rts.gotoPage(RTS::PAGE_ERR_TEMP);
    else if (e == GET_TEXT(MSG_ERR_THERMAL_RUNAWAY) || e == GET_TEXT(MSG_ERR_TEMP_MALFUNCTION))
      rts.gotoPage(RTS::PAGE_ERR_RUNAWAY);
    // The screen has no page for other errors (M112, homing or probe failure)
  }

  void onMediaMounted() { rts.mediaInserted(); }
  void onMediaError() { rts.mediaRemoved(); }
  void onMediaRemoved() { rts.mediaRemoved(); }

  // Error pages are shown by onPrinterKilled, which knows the cause
  void onHeatingError(const heater_id_t) {}
  void onMinTempError(const heater_id_t) {}
  void onMaxTempError(const heater_id_t) {}

  void onPlayTone(const uint16_t, const uint16_t/*=0*/) {}

  void onPrintTimerStarted() { rts.printStarted(); }
  void onPrintTimerPaused() { rts.printPaused(); }
  void onPrintTimerStopped() { rts.printStopped(); }
  void onPrintDone() { rts.printFinished(); }

  void onFilamentRunout(const extruder_t) { rts.filamentRunout(); }

  void onUserConfirmRequired(const char * const msg) {
    if (!msg) return;
    if (strcmp_P(msg, GET_TEXT(MSG_REHEATDONE)) == 0)
      rts.gotoPage(RTS::PAGE_REHEAT_DONE);
    else if (strcmp_P(msg, GET_TEXT(MSG_NOZZLE_PARKED)) == 0)
      rts.gotoPage(RTS::PAGE_PAUSED);
  }

  void onUserConfirmRequired(const int, const char * const cstr, FSTR_P const) { onUserConfirmRequired(cstr); }
  void onUserConfirmRequired(const int, FSTR_P const fstr, FSTR_P const) { onUserConfirmRequired(fstr); }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void onPauseMode(const PauseMessage message, const PauseMode, const uint8_t) { rts.pauseMessage(message); }
  #endif

  void onStatusChanged(const char * const) {}

  void onHomingStart() { rts.homingStarted(); }
  void onHomingDone() { rts.homingFinished(); }

  void onSteppersDisabled() {}
  void onSteppersEnabled() {}
  void onAxisDisabled(const axis_t) {}
  void onAxisEnabled(const axis_t) {}

  void onFactoryReset() {
    RTS::settings.version       = settings_version;
    RTS::settings.chinese       = false;
    RTS::settings.volume        = 0x80;
    RTS::settings.energy_saving = false;
    RTS::settings.leveling      = false;
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

  void onPostprocessSettings() {}
  void onSettingsStored(const bool) {}

  void onSettingsLoaded(const bool) {
    // Restore the leveling state the screen last set. G28 keeps it with RESTORE_LEVELING_AFTER_G28.
    if (RTS::settings.leveling != getLevelingActive() && !isPrinting())
      setLevelingActive(RTS::settings.leveling);
    rts.sendLanguage();
    rts.sendVolume();
    rts.sendZOffset();
    rts.sendMesh();
  }

  #if HAS_LEVELING
    void onLevelingStart() {}
    void onLevelingDone() { rts.levelingFinished(); }
    #if ENABLED(PREHEAT_BEFORE_LEVELING)
      celsius_t getLevelingBedTemp() { return LEVELING_BED_TEMP; }
    #endif
  #endif

  #if HAS_MESH
    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) { rts.meshPointProbed(xpos, ypos, zval); }
    void onMeshUpdate(const int8_t, const int8_t, const probe_state_t) {}
  #endif

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    void onSetMinExtrusionTemp(const celsius_t) {}
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onSetPowerLoss(const bool) {}
    void onPowerLoss() {}
    void onPowerLossResume() { rts.powerLossDetected(); }
  #endif

  #if HAS_PID_HEATING
    void onPIDTuning(const pidresult_t rst) {
      if (rst == PID_TUNING_TIMEOUT) rts.gotoPage(RTS::PAGE_ERR_HEATING);
    }
    void onStartM303(const int, const heater_id_t, const celsius_t) {}
  #endif

  #if ENABLED(MPC_AUTOTUNE)
    void onMPCTuning(const mpcresult_t) {}
  #endif

  #if ENABLED(PLATFORM_M997_SUPPORT)
    void onFirmwareFlash() {}
  #endif

} // ExtUI

#endif // DGUS_LCD_UI_CR10SPROV2
