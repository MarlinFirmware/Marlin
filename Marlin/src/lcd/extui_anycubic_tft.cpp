/*********************
 * extui_anycubic_tftexample.cpp *
 *********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../inc/MarlinConfigPre.h"

#if BOTH(ANYCUBIC_TFT_MODEL, EXTENSIBLE_UI)

#include "extui/lib/anycubic/anycubic_TFT.h"
#include "extui/ui_api.h"

#include <Arduino.h>

namespace ExtUI {
  void onStartup() {
    /* Initialize the display module here. The following
     * routines are available for access to the GPIO pins:
     *
     *   SET_OUTPUT(pin)
     *   SET_INPUT_PULLUP(pin)
     *   SET_INPUT(pin)
     *   WRITE(pin,value)
     *   READ(pin)
     */
    AnycubicTFT.OnSetup();
  }

  void onIdle() {
    AnycubicTFT.OnCommandScan();
  }

  void onPrinterKilled(PGM_P const error, PGM_P const component) {
    AnycubicTFT.OnKillTFT();
  }

  void onMediaInserted() {
    AnycubicTFT.OnSDCardStateChange(true);
  };
  
  void onMediaError() {};

  void onMediaRemoved() {
    AnycubicTFT.OnSDCardStateChange(false);
  };

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    #if ENABLED(SPEAKER)
      ::tone(BEEPER_PIN, frequency, duration);
    #endif
  }
  void onPrintTimerStarted() {
    AnycubicTFT.OnPrintTimerStarted();
  }

  void onPrintTimerPaused() {
    AnycubicTFT.OnPrintTimerPaused();
  }

  void onPrintTimerStopped() {
    AnycubicTFT.OnPrintTimerStopped();
  }

  void onFilamentRunout(const extruder_t extruder) {
    AnycubicTFT.OnFilamentRunout();
  }

  void onUserConfirmRequired(const char * const msg) {
    AnycubicTFT.OnUserConfirmRequired(msg);
  }

  void onStatusChanged(const char * const msg) {}
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
    void onPowerLossResume() {
      // Called on resume from power-loss
    }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
    }
  #endif
}

#endif // EXTUI_EXAMPLE && EXTENSIBLE_UI


