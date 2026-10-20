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
 * lcd/extui/paneldue/paneldue_extui.cpp
 *
 * ExtUI callbacks for the PanelDue. See paneldue.cpp for the protocol.
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(PANELDUE)

#include "../ui_api.h"
#include "../../marlinui.h"
#include "paneldue.h"

// The PanelDue buzzer is used when there's no BEEPER_PIN
void MarlinUI::buzz(const long duration, const uint16_t freq/*=0*/) {
  if (sound_on && freq && duration) paneldue.beep(freq, duration);
}

namespace ExtUI {
  void onStartup() { paneldue.startup(); }
  void onIdle() { paneldue.idle(); }
  void onPrinterKilled(FSTR_P const error, FSTR_P const component) { paneldue.killed(error, component); }

  void onMediaMounted() { paneldue.media_changed(); }
  void onMediaError()   {}
  void onMediaRemoved() { paneldue.media_changed(); }

  void onHeatingError(const heater_id_t heater_id) {}
  void onMinTempError(const heater_id_t heater_id) {}
  void onMaxTempError(const heater_id_t heater_id) {}

  void onPlayTone(const uint16_t frequency, const uint16_t duration/*=0*/) { paneldue.beep(frequency, duration); }
  void onPrintTimerStarted() {}
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() { paneldue.user_confirm(nullptr); }

  #if HAS_FILAMENT_SENSOR
    void onFilamentRunout(const extruder_t extruder) {}
  #endif

  void onUserConfirmRequired(const char * const msg) { paneldue.user_confirm(msg); }

  // For fancy LCDs include an icon ID, message, and translated button title
  void onUserConfirmRequired(const int, const char * const cstr, FSTR_P const) { onUserConfirmRequired(cstr); }
  void onUserConfirmRequired(const int, FSTR_P const fstr, FSTR_P const) { onUserConfirmRequired(fstr); }

  void onStatusChanged(const char * const msg) { paneldue.status_message(msg); }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void onPauseMode(
      const PauseMessage message,
      const PauseMode mode/*=PAUSE_MODE_SAME*/,
      const uint8_t extruder/*=motion.extruder*/
    ) {
      paneldue.pause_option(message == PAUSE_MESSAGE_OPTION);
      if (message == PAUSE_MESSAGE_STATUS) paneldue.user_confirm(nullptr);
      stdOnPauseMode(message, mode, extruder);
    }
  #endif

  void onHomingStart() {}
  void onHomingDone() {}

  void onPrintDone() {}

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

  void onSettingsStored(const bool success) {
    // Called after the entire EEPROM has been written,
    // whether successful or not.
  }

  void onSettingsLoaded(const bool success) {
    // Called after the entire EEPROM has been read,
    // whether successful or not.
  }

  #if HAS_LEVELING
    void onLevelingStart() {}
    void onLevelingDone() {}
    #if ENABLED(PREHEAT_BEFORE_LEVELING)
      celsius_t getLevelingBedTemp() { return LEVELING_BED_TEMP; }
    #endif
  #endif

  #if HAS_MESH
    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
      // Called when any mesh points are updated
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const probe_state_t state) {
      // Called to indicate a special condition
    }
  #endif

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    void onSetMinExtrusionTemp(const celsius_t) {}
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onSetPowerLoss(const bool onoff) {
      // Called when power-loss is enabled/disabled
    }
    void onPowerLoss() {
      // Called when power-loss state is detected
    }
    void onPowerLossResume() {
      // Called on resume from power-loss
    }
  #endif

  #if HAS_PID_HEATING
    void onPIDTuning(const pidresult_t rst) {
      // Called for temperature PID tuning result
      switch (rst) {
        case PID_STARTED:
        case PID_BED_STARTED:
        case PID_CHAMBER_STARTED: break;
        case PID_BAD_HEATER_ID:   break;
        case PID_TEMP_TOO_HIGH:   break;
        case PID_TUNING_TIMEOUT:  break;
        case PID_DONE:            break;
      }
    }
    void onStartM303(const int count, const heater_id_t hid, const celsius_t temp) {
      // Called by M303 to update the UI
    }
  #endif

  #if ENABLED(MPC_AUTOTUNE)
    void onMPCTuning(const mpcresult_t rst) {
      // Called for temperature MPC tuning result
      switch (rst) {
        case MPC_STARTED:     break;
        case MPC_TEMP_ERROR:  break;
        case MPC_INTERRUPTED: break;
        case MPC_DONE:        break;
      }
    }
  #endif

  #if ENABLED(PLATFORM_M997_SUPPORT)
    void onFirmwareFlash() {}
  #endif

  void onSteppersDisabled() {}
  void onSteppersEnabled() {}
  void onAxisDisabled(const axis_t) {}
  void onAxisEnabled(const axis_t) {}

} // ExtUI

#endif // PANELDUE
