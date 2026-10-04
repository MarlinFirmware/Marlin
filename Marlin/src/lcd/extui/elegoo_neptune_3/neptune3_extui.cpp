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
 * lcd/extui/elegoo_neptune_3/neptune3_extui.cpp
 *
 * Elegoo Neptune 3 Pro / Plus / Max TJC touch screen
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ELEGOO_NEPTUNE_3_TFT)

#include "../ui_api.h"
#include "neptune3_tft.h"

namespace ExtUI {

  void onStartup() { neptune3.startup(); }
  void onIdle() { neptune3.idleLoop(); }
  void onPrinterKilled(FSTR_P const error, FSTR_P const) { neptune3.printerKilled(error); }

  void onMediaMounted() { neptune3.mediaMounted(); }
  void onMediaError() { neptune3.mediaError(); }
  void onMediaRemoved() { neptune3.mediaRemoved(); }

  void onHeatingError(const heater_id_t heater) { neptune3.heatingError(heater); }
  void onMinTempError(const heater_id_t heater) { neptune3.minTempError(heater); }
  void onMaxTempError(const heater_id_t heater) { neptune3.maxTempError(heater); }

  void onPrintTimerStarted() { neptune3.printTimerStarted(); }

  void onUserConfirmRequired(const char * const) { neptune3.userConfirmRequired(); }
  void onUserConfirmRequired(const int, const char * const, FSTR_P const) { neptune3.userConfirmRequired(); }
  void onUserConfirmRequired(const int, FSTR_P const, FSTR_P const) { neptune3.userConfirmRequired(); }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void onPauseMode(const PauseMessage message, const PauseMode mode/*=PAUSE_MODE_SAME*/, const uint8_t/*=motion.extruder*/) {
      if (mode != PAUSE_MODE_SAME) pause_mode = mode;
      neptune3.pauseMode(message);
    }
  #endif

  void onStatusChanged(const char * const msg) { neptune3.statusChanged(msg); }

  void onHomingStart() { neptune3.homingStart(); }
  void onHomingDone() { neptune3.homingDone(); }

  void onPrintDone() { neptune3.printDone(); }

  void onFactoryReset() { neptune3.factoryReset(); }
  void onStoreSettings(char *buff) { neptune3.storeSettings(buff); }
  void onLoadSettings(const char *buff) { neptune3.loadSettings(buff); }
  void onPostprocessSettings() { neptune3.postprocessSettings(); }

  #if HAS_LEVELING
    void onLevelingStart() { neptune3.levelingStart(); }
    void onLevelingDone() { neptune3.levelingDone(); }
  #endif

  #if HAS_MESH
    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) { neptune3.meshUpdate(xpos, ypos, zval); }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onSetPowerLoss(const bool onoff) { neptune3.setPowerLoss(onoff); }
    void onPowerLossResume() { neptune3.powerLossResume(); }
  #endif
}

#endif // ELEGOO_NEPTUNE_3_TFT
