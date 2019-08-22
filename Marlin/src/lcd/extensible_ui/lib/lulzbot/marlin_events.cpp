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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "compat.h"

#if ENABLED(LULZBOT_TOUCH_UI)

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

  void onPrinterKilled(PGM_P lcd_msg) {
    KillScreen::show(progmem_str(lcd_msg));
  }

  void onMediaInserted() {
    if (AT_SCREEN(StatusScreen))
      StatusScreen::setStatusMessage(F(MSG_MEDIA_INSERTED));
    sound.play(media_inserted, PLAY_ASYNCHRONOUS);
  }

  void onMediaRemoved() {
    if (AT_SCREEN(StatusScreen))
      StatusScreen::setStatusMessage(F(MSG_MEDIA_REMOVED));
    sound.play(media_removed, PLAY_ASYNCHRONOUS);
    if (AT_SCREEN(FilesScreen)) {
      GOTO_SCREEN(StatusScreen)
    }
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
    InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FAILED);
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
    #ifdef LULZBOT_EEPROM_BACKUP_SIZE
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
}

#endif // LULZBOT_TOUCH_UI
