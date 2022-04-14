/*******************************
 * interface_settings_screen.h *
 *******************************/

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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#define FTDI_INTERFACE_SETTINGS_SCREEN
#define FTDI_INTERFACE_SETTINGS_SCREEN_CLASS InterfaceSettingsScreen

struct InterfaceSettingsScreenData {
  uint8_t volume;
  uint8_t brightness;
};

class InterfaceSettingsScreen : public BaseScreen, public CachedScreen<INTERFACE_SETTINGS_SCREEN_CACHE> {
  private:
    struct persistent_data_t {
      uint32_t touch_transform_a;
      uint32_t touch_transform_b;
      uint32_t touch_transform_c;
      uint32_t touch_transform_d;
      uint32_t touch_transform_e;
      uint32_t touch_transform_f;
      uint16_t passcode;
      uint8_t  display_brightness;
      int8_t   display_h_offset_adj;
      int8_t   display_v_offset_adj;
      uint8_t  sound_volume;
      uint8_t  bit_flags;
      uint8_t  event_sounds[InterfaceSoundsScreen::NUM_EVENTS];
    };

  public:
    #ifdef ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE
      static bool backupEEPROM();
    #endif

    static void saveSettings(char *);
    static void loadSettings(const char *);
    static void defaultSettings();
    static void failSafeSettings();

    static void onStartup();
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};
