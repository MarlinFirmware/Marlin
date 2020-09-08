/*********************
 * marlin_events.cpp *
 *********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "compat.h"

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens/screens.h"

namespace ExtUI {
  using namespace Theme;
  using namespace FTDI;

  void onStartup() {
    EventLoop::setup();
  }

  void onIdle() {
    EventLoop::loop();
  }

  void onPrinterKilled(PGM_P const error, PGM_P const component) {
    char str[strlen_P(error) + strlen_P(component) + 3];
    sprintf_P(str, PSTR(S_FMT ": " S_FMT), error, component);
    KillScreen::show(str);
  }

  void onMediaInserted() {
    if (AT_SCREEN(StatusScreen))
      StatusScreen::setStatusMessage(GET_TEXT_F(MSG_MEDIA_INSERTED));
    sound.play(media_inserted, PLAY_ASYNCHRONOUS);
  }

  void onMediaRemoved() {
    if (isPrintingFromMedia()) {
      stopPrint();
      InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FAILED);
    }
    else
      sound.play(media_removed, PLAY_ASYNCHRONOUS);

    if (AT_SCREEN(StatusScreen) || isPrintingFromMedia())
      StatusScreen::setStatusMessage(GET_TEXT_F(MSG_MEDIA_REMOVED));

    if (AT_SCREEN(FilesScreen)) GOTO_SCREEN(StatusScreen);
  }

  void onMediaError() {
    sound.play(sad_trombone, PLAY_ASYNCHRONOUS);
    AlertDialogBox::showError(F("Unable to read media."));
  }

  void onStatusChanged(const char* lcd_msg) {
    StatusScreen::setStatusMessage(lcd_msg);
  }

  void onStatusChanged(progmem_str lcd_msg) {
    StatusScreen::setStatusMessage(lcd_msg);
  }

  void onPrintTimerStarted() {
    InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_STARTED);
  }

  void onPrintTimerStopped() {
    InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FINISHED);
  }

  void onPrintTimerPaused() {
  }

  void onFilamentRunout(const extruder_t extruder) {
    char lcd_msg[30];
    sprintf_P(lcd_msg, PSTR("Extruder %d Filament Error"), extruder + 1);
    StatusScreen::setStatusMessage(lcd_msg);
    InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FAILED, FTDI::PLAY_SYNCHRONOUS);
  }

  void onFactoryReset() {
    InterfaceSettingsScreen::defaultSettings();
  }

  void onStoreSettings(char *buff) {
    InterfaceSettingsScreen::saveSettings(buff);
  }

  void onLoadSettings(const char *buff) {
    InterfaceSettingsScreen::loadSettings(buff);
  }

  void onConfigurationStoreWritten(bool success) {
    #ifdef ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE
      if (success && InterfaceSettingsScreen::backupEEPROM()) {
        SERIAL_ECHOLNPGM("Made backup of EEPROM to SPI Flash");
      }
    #else
      UNUSED(success);
    #endif
  }

  void onConfigurationStoreRead(bool) {
  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    sound.play_tone(frequency, duration);
  }

  void onUserConfirmRequired(const char * const msg) {
    if (msg)
      ConfirmUserRequestAlertBox::show(msg);
    else
      ConfirmUserRequestAlertBox::hide();
  }

  #if HAS_LEVELING && HAS_MESH
    void onMeshUpdate(const int8_t x, const int8_t y, const float val) {
      BedMeshScreen::onMeshUpdate(x, y, val);
    }

    void onMeshUpdate(const int8_t x, const int8_t y, const ExtUI::probe_state_t state) {
      BedMeshScreen::onMeshUpdate(x, y, state);
    }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume() {
      // Called on resume from power-loss
    }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
      //SERIAL_ECHOLNPAIR("OnPidTuning:", rst);
      switch (rst) {
        case PID_BAD_EXTRUDER_NUM:
          StatusScreen::setStatusMessage(GET_TEXT_F(MSG_PID_BAD_EXTRUDER_NUM));
          break;
        case PID_TEMP_TOO_HIGH:
          StatusScreen::setStatusMessage(GET_TEXT_F(MSG_PID_TEMP_TOO_HIGH));
          break;
        case PID_TUNING_TIMEOUT:
          StatusScreen::setStatusMessage(GET_TEXT_F(MSG_PID_TIMEOUT));
          break;
        case PID_DONE:
          StatusScreen::setStatusMessage(GET_TEXT_F(MSG_PID_AUTOTUNE_DONE));
          break;
      }
      GOTO_SCREEN(StatusScreen);
    }
  #endif // HAS_PID_HEATING
}

#endif // TOUCH_UI_FTDI_EVE
