/*********************************
 * interface_settings_screen.cpp *
 *********************************/

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

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens.h"
#include "screen_data.h"

#include "../archim2-flash/flash_storage.h"

#include "../../../../../module/configuration_store.h"

#if ENABLED(LULZBOT_PRINTCOUNTER)
  #include "../../../../../module/printcounter.h"
#endif

bool restoreEEPROM();

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

constexpr bool PERSISTENT_STORE_SUCCESS = false; // persistentStore uses true for error

void InterfaceSettingsScreen::onStartup() {
}

void InterfaceSettingsScreen::onEntry() {
  screen_data.InterfaceSettingsScreen.brightness = CLCD::get_brightness();
  screen_data.InterfaceSettingsScreen.volume     = SoundPlayer::get_volume();
  BaseScreen::onEntry();
}

void InterfaceSettingsScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if (what & BACKGROUND) {

    #define GRID_COLS 4
    #ifdef TOUCH_UI_PORTRAIT
      #define GRID_ROWS 7
    #else
      #define GRID_ROWS 6
    #endif

    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0)
       .font(font_medium)
       .text(BTN_POS(1,1), BTN_SIZE(4,1), GET_TEXT_F(MSG_INTERFACE))
    #undef EDGE_R
    #define EDGE_R 30
       .font(font_small)
       .tag(0)
    #if DISABLED(LCD_FYSETC_TFT81050)
       .text(BTN_POS(1,2), BTN_SIZE(2,1), GET_TEXT_F(MSG_LCD_BRIGHTNESS), OPT_RIGHTX | OPT_CENTERY)
    #endif
       .text(BTN_POS(1,3), BTN_SIZE(2,1), GET_TEXT_F(MSG_SOUND_VOLUME),   OPT_RIGHTX | OPT_CENTERY)
       .text(BTN_POS(1,4), BTN_SIZE(2,1), GET_TEXT_F(MSG_SCREEN_LOCK),    OPT_RIGHTX | OPT_CENTERY);
    #if DISABLED(TOUCH_UI_NO_BOOTSCREEN)
    cmd.text(BTN_POS(1,5), BTN_SIZE(2,1), GET_TEXT_F(MSG_BOOT_SCREEN),    OPT_RIGHTX | OPT_CENTERY);
    #endif
    #undef EDGE_R
  }

  if (what & FOREGROUND) {
    #ifdef TOUCH_UI_PORTRAIT
      constexpr uint8_t w = 2;
    #else
      constexpr uint8_t w = 1;
    #endif

    cmd.font(font_medium)
    #define EDGE_R 30
       .colors(ui_slider)
    #if DISABLED(LCD_FYSETC_TFT81050)
       .tag(2).slider(BTN_POS(3,2), BTN_SIZE(2,1), screen_data.InterfaceSettingsScreen.brightness, 128)
    #endif
       .tag(3).slider(BTN_POS(3,3), BTN_SIZE(2,1), screen_data.InterfaceSettingsScreen.volume,     0xFF)
       .colors(ui_toggle)
       .tag(4).toggle2(BTN_POS(3,4), BTN_SIZE(w,1), GET_TEXT_F(MSG_NO), GET_TEXT_F(MSG_YES), LockScreen::is_enabled())
    #if DISABLED(TOUCH_UI_NO_BOOTSCREEN)
       .tag(5).toggle2(BTN_POS(3,5), BTN_SIZE(w,1), GET_TEXT_F(MSG_NO), GET_TEXT_F(MSG_YES), UIData::animations_enabled())
    #endif
    #undef EDGE_R
    #define EDGE_R 0
    #ifdef TOUCH_UI_PORTRAIT
       .colors(normal_btn)
       .tag(6).button (BTN_POS(1,6), BTN_SIZE(4,1), GET_TEXT_F(MSG_SOUNDS))
       .colors(action_btn)
       .tag(1).button (BTN_POS(1,7), BTN_SIZE(4,1), GET_TEXT_F(MSG_BACK));
    #else
       .tag(6).button (BTN_POS(1,6), BTN_SIZE(2,1), GET_TEXT_F(MSG_SOUNDS))
       .colors(action_btn)
       .tag(1).button (BTN_POS(3,6), BTN_SIZE(2,1), GET_TEXT_F(MSG_BACK));
    #endif
  }
}

bool InterfaceSettingsScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); return true;
    case 4:
      if (!LockScreen::is_enabled())
        LockScreen::enable();
      else
        LockScreen::disable();
      break;
    case 5: UIData::enable_animations(!UIData::animations_enabled());; break;
    case 6: GOTO_SCREEN(InterfaceSoundsScreen); return true;
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

bool InterfaceSettingsScreen::onTouchStart(uint8_t tag) {
  #undef EDGE_R
  #define EDGE_R 30
  CommandProcessor cmd;
  switch (tag) {
    case 2: cmd.track_linear(BTN_POS(3,3), BTN_SIZE(2,1), 2).execute(); break;
    case 3: cmd.track_linear(BTN_POS(3,4), BTN_SIZE(2,1), 3).execute(); break;
    default: break;
  }
  #undef EDGE_R
  #define EDGE_R 0
  #undef GRID_COLS
  #undef GRID_ROWS
  return true;
}

void InterfaceSettingsScreen::onIdle() {
  if (refresh_timer.elapsed(TOUCH_UPDATE_INTERVAL)) {
    refresh_timer.start();

    uint16_t value;
    CommandProcessor cmd;
    switch (cmd.track_tag(value)) {
      case 2:
        screen_data.InterfaceSettingsScreen.brightness = _MAX(11, (value * 128UL) / 0xFFFF);
        CLCD::set_brightness(screen_data.InterfaceSettingsScreen.brightness);
        SaveSettingsDialogBox::settingsChanged();
        break;
      case 3:
        screen_data.InterfaceSettingsScreen.volume = value >> 8;
        SoundPlayer::set_volume(screen_data.InterfaceSettingsScreen.volume);
        SaveSettingsDialogBox::settingsChanged();
        break;
      default:
        return;
    }
    onRefresh();
  }
  BaseScreen::onIdle();
}

void InterfaceSettingsScreen::failSafeSettings() {
  // Reset settings that may make the printer interface
  // unusable.
  CLCD::mem_write_32(CLCD::REG::ROTATE, 0);
  CLCD::default_touch_transform();
  CLCD::default_display_orientation();
  CLCD::set_brightness(255);
  UIData::reset_persistent_data();
  CLCD::mem_write_16(CLCD::REG::HOFFSET, FTDI::Hoffset);
  CLCD::mem_write_16(CLCD::REG::VOFFSET, FTDI::Voffset);
}

void InterfaceSettingsScreen::defaultSettings() {
  LockScreen::passcode = 0;
  SoundPlayer::set_volume(255);
  CLCD::set_brightness(255);
  UIData::reset_persistent_data();
  InterfaceSoundsScreen::defaultSettings();
  CLCD::mem_write_16(CLCD::REG::HOFFSET, FTDI::Hoffset);
  CLCD::mem_write_16(CLCD::REG::VOFFSET, FTDI::Voffset);
}

void InterfaceSettingsScreen::saveSettings(char *buff) {
  static_assert(
    ExtUI::eeprom_data_size >= sizeof(persistent_data_t),
    "Insufficient space in EEPROM for UI parameters"
  );

  SERIAL_ECHOLNPGM("Writing setting to EEPROM");

  persistent_data_t eeprom;

  eeprom.passcode             = LockScreen::passcode;
  eeprom.sound_volume         = SoundPlayer::get_volume();
  eeprom.display_brightness   = CLCD::get_brightness();
  eeprom.bit_flags            = UIData::get_persistent_data();
  eeprom.touch_transform_a    = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_A);
  eeprom.touch_transform_b    = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_B);
  eeprom.touch_transform_c    = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_C);
  eeprom.touch_transform_d    = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_D);
  eeprom.touch_transform_e    = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_E);
  eeprom.touch_transform_f    = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_F);
  eeprom.display_h_offset_adj = CLCD::mem_read_16(CLCD::REG::HOFFSET) - FTDI::Hoffset;
  eeprom.display_v_offset_adj = CLCD::mem_read_16(CLCD::REG::VOFFSET) - FTDI::Voffset;
  for(uint8_t i = 0; i < InterfaceSoundsScreen::NUM_EVENTS; i++)
    eeprom.event_sounds[i] = InterfaceSoundsScreen::event_sounds[i];

  memcpy(buff, &eeprom, sizeof(eeprom));
}

void InterfaceSettingsScreen::loadSettings(const char *buff) {
  static_assert(
    ExtUI::eeprom_data_size >= sizeof(persistent_data_t),
    "Insufficient space in EEPROM for UI parameters"
  );

  persistent_data_t eeprom;
  memcpy(&eeprom, buff, sizeof(eeprom));

  SERIAL_ECHOLNPGM("Loading setting from EEPROM");

  LockScreen::passcode = eeprom.passcode;
  SoundPlayer::set_volume(eeprom.sound_volume);
  UIData::set_persistent_data(eeprom.bit_flags);
  CLCD::set_brightness(eeprom.display_brightness);
  CLCD::mem_write_32(CLCD::REG::TOUCH_TRANSFORM_A, eeprom.touch_transform_a);
  CLCD::mem_write_32(CLCD::REG::TOUCH_TRANSFORM_B, eeprom.touch_transform_b);
  CLCD::mem_write_32(CLCD::REG::TOUCH_TRANSFORM_C, eeprom.touch_transform_c);
  CLCD::mem_write_32(CLCD::REG::TOUCH_TRANSFORM_D, eeprom.touch_transform_d);
  CLCD::mem_write_32(CLCD::REG::TOUCH_TRANSFORM_E, eeprom.touch_transform_e);
  CLCD::mem_write_32(CLCD::REG::TOUCH_TRANSFORM_F, eeprom.touch_transform_f);
  CLCD::mem_write_16(CLCD::REG::HOFFSET,           eeprom.display_h_offset_adj + FTDI::Hoffset);
  CLCD::mem_write_16(CLCD::REG::VOFFSET,           eeprom.display_v_offset_adj + FTDI::Voffset);
  for(uint8_t i = 0; i < InterfaceSoundsScreen::NUM_EVENTS; i++)
    InterfaceSoundsScreen::event_sounds[i] = eeprom.event_sounds[i];

  TERN_(TOUCH_UI_DEVELOPER_MENU, StressTestScreen::startupCheck());
}

#ifdef ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE
  #include "../../../../../HAL/shared/eeprom_api.h"

  bool restoreEEPROM() {
    uint8_t data[ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE];

    bool success = UIFlashStorage::read_config_data(data, ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE);

    if (success)
      success = persistentStore.write_data(0, data, ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE) == PERSISTENT_STORE_SUCCESS;

    if (success)
      StatusScreen::setStatusMessage(GET_TEXT_F(MSG_EEPROM_RESTORED));
    else
      StatusScreen::setStatusMessage(GET_TEXT_F(MSG_EEPROM_RESET));

    return success;
  }

  bool InterfaceSettingsScreen::backupEEPROM() {
    uint8_t data[ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE];

    if (persistentStore.read_data(0, data, ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE) != PERSISTENT_STORE_SUCCESS)
      return false;

    UIFlashStorage::write_config_data(data, ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE);

    return true;
  }
#endif

#endif // TOUCH_UI_FTDI_EVE
