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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * extui_dgus_lcd.cpp
 *
 * DGUS implementation for Marlin by coldtobi, Feb-May 2019
 */

#include "../inc/MarlinConfigPre.h"

#if HAS_DGUS_LCD

#include "extui/ui_api.h"

#include "extui/lib/dgus/DGUSScreenHandler.h"
#include "extui/lib/dgus/DGUSDisplayDef.h"

extern const char NUL_STR[];

namespace ExtUI {

  void onStartup() {
    dgus_screen_handler.Init();
  }

  void onIdle() {
    dgus_screen_handler.Loop();
  }

  void onPrinterKilled(PGM_P error, PGM_P component) {
    dgus_screen_handler.PrinterKilled(error, component);
  }

  void onMediaInserted() {
    #if ENABLED(SDSUPPORT)
      dgus_screen_handler.SDCardInserted();
    #endif
  }
  void onMediaError()    {
    #if ENABLED(SDSUPPORT)
      dgus_screen_handler.SDCardError();
    #endif
  }
  void onMediaRemoved()  {
    #if ENABLED(SDSUPPORT)
      dgus_screen_handler.SDCardRemoved();
    #endif
  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {}
  void onPrintTimerStarted() {}
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() {}
  void onFilamentRunout(const extruder_t extruder) {}

  void onUserConfirmRequired(const char * const msg) {
    dgus_screen_handler.UserConfirmRequired(msg);
  }

  void onStatusChanged(const char * const msg) {
    dgus_screen_handler.SetStatusMessage(msg);
  }

  void onFactoryReset() {}
  void onStoreSettings(char *buff) {
    // Called when saving to EEPROM (i.e. M500). If the ExtUI needs
    // permanent data to be stored, it can write up to eeprom_data_size bytes
    // into buff.

    // Example:
    //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
    //  memcpy(buff, &myDataStruct, sizeof(myDataStruct));
  }

  void onLoadSettings(const char *buff) {
    // Called while loading settings from EEPROM. If the ExtUI
    // needs to retrieve data, it should copy up to eeprom_data_size bytes
    // from buff

    // Example:
    //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
    //  memcpy(&myDataStruct, buff, sizeof(myDataStruct));
  }

  void onConfigurationStoreWritten(bool success) {
    // Called after the entire EEPROM has been written,
    // whether successful or not.
  }

  void onConfigurationStoreRead(bool success) {
    // Called after the entire EEPROM has been read,
    // whether successful or not.
  }

  void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
    // Called when any mesh points are updated
  }

  #if ENABLED(POWER_LOSS_RECOVERY)
    void OnPowerLossResume() {
      // Called on resume from power-loss
      dgus_screen_handler.PowerLossResume();
    }
  #endif


  #if HAS_PID_HEATING
    void OnPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
      dgus_screen_handler.PidTuning(rst);
    }
  #endif

}

#endif // HAS_DGUS_LCD
