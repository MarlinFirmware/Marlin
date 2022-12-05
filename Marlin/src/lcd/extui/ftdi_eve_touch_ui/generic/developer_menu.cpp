/**********************
 * developer_menu.cpp *
 **********************/

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

#include "../config.h"
#include "../screens.h"

#ifdef FTDI_DEVELOPER_MENU

#include "../archim2-flash/flash_storage.h"

using namespace FTDI;
using namespace Theme;

void DeveloperMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .font(font_medium)
       .tag(0);

    #ifdef SPI_FLASH_SS
      constexpr bool has_flash = true;
    #else
      constexpr bool has_flash = false;
    #endif

    #if ENABLED(SDSUPPORT)
      constexpr bool has_media = true;
    #else
      constexpr bool has_media = false;
    #endif

    cmd.cmd(COLOR_RGB(bg_text_enabled));
    #if ENABLED(TOUCH_UI_PORTRAIT)
      #define GRID_COLS 1
      #define GRID_ROWS 10
      cmd.font(font_large)         .text  ( BTN_POS(1,1), BTN_SIZE(1,1), F("Developer Menu"))
         .colors(normal_btn)
         .tag(2).font(font_medium) .button(BTN_POS(1,2), BTN_SIZE(1,1), F("Show All Widgets"))
         .tag(3)                   .button(BTN_POS(1,3), BTN_SIZE(1,1), F("Stress Test"))
         .tag(4)                   .button(BTN_POS(1,4), BTN_SIZE(1,1), F("Show Touch Registers"))
         .tag(5)                   .button(BTN_POS(1,5), BTN_SIZE(1,1), F("Play Song"))
         .tag(6).enabled(has_media).button(BTN_POS(1,6), BTN_SIZE(1,1), F("Play Video from Media"))
         .tag(7).enabled(has_flash).button(BTN_POS(1,7), BTN_SIZE(1,1), F("Play Video from SPI Flash"))
         .tag(8).enabled(has_flash).button(BTN_POS(1,8), BTN_SIZE(1,1), F("Load Video to SPI Flash"))
         .tag(9).enabled(has_flash).button(BTN_POS(1,9), BTN_SIZE(1,1), F("Erase SPI Flash"))

         .tag(1).colors(action_btn)
                                   .button(BTN_POS(1,10), BTN_SIZE(1,1), F("Back"));
    #else
      #define GRID_COLS 2
      #define GRID_ROWS 6
      cmd.font(font_medium)        .text  ( BTN_POS(1,1), BTN_SIZE(2,1), F("Developer Menu"))
         .colors(normal_btn)
         .tag(2).font(font_small)  .button(BTN_POS(1,2), BTN_SIZE(1,1), F("Show All Widgets"))
         .tag(3)                   .button(BTN_POS(1,3), BTN_SIZE(1,1), F("Show Touch Registers"))
         .tag(9)                   .button(BTN_POS(1,4), BTN_SIZE(1,1), F("Show Pin States"))
         .tag(4)                   .button(BTN_POS(1,5), BTN_SIZE(1,1), F("Play Song"))
         .tag(5).enabled(has_media).button(BTN_POS(2,2), BTN_SIZE(1,1), F("Play Video from Media"))
         .tag(6).enabled(has_flash).button(BTN_POS(2,3), BTN_SIZE(1,1), F("Play Video from SPI Flash"))
         .tag(7).enabled(has_flash).button(BTN_POS(2,4), BTN_SIZE(1,1), F("Load Video to SPI Flash"))
         .tag(8).enabled(has_flash).button(BTN_POS(2,5), BTN_SIZE(1,1), F("Erase SPI Flash"))
         .tag(1).colors(action_btn)
                                   .button(BTN_POS(1,6), BTN_SIZE(2,1), F("Back"));
    #endif
  }
}

bool DeveloperMenu::onTouchEnd(uint8_t tag) {
  using namespace Theme;
  switch (tag) {
    case 1: GOTO_PREVIOUS();                            break;
    case 2: GOTO_SCREEN(WidgetsScreen);                 break;
    case 3:
      PUSH_SCREEN(StressTestScreen);
      AlertDialogBox::show(F("Please do not run this test unattended as it may cause your printer to malfunction."));
      current_screen.forget();
      break;
    case 4: GOTO_SCREEN(TouchRegistersScreen);          break;
    case 5: sound.play(js_bach_joy, PLAY_ASYNCHRONOUS); break;
    #if ENABLED(SDSUPPORT)
    case 6:
        if (!MediaPlayerScreen::playCardMedia())
          AlertDialogBox::showError(F("Cannot open STARTUP.AVI"));
        break;
    #endif
    #ifdef SPI_FLASH_SS
      case 7:
        if (!MediaPlayerScreen::playBootMedia())
          AlertDialogBox::showError(F("No boot media available"));
        break;
      case 8:
      {
        SpinnerDialogBox::show(F("Saving..."));
        UIFlashStorage::error_t res = UIFlashStorage::write_media_file(F("STARTUP.AVI"));
        SpinnerDialogBox::hide();
        reset_menu_timeout();
        switch (res) {
          case UIFlashStorage::SUCCESS:
            AlertDialogBox::show(F("File copied!"));
            break;

          case UIFlashStorage::READ_ERROR:
            AlertDialogBox::showError(F("Failed to read file"));
            break;

          case UIFlashStorage::VERIFY_ERROR:
            AlertDialogBox::showError(F("Failed to verify file"));
            break;

          case UIFlashStorage::FILE_NOT_FOUND:
            AlertDialogBox::showError(F("Cannot open STARTUP.AVI"));
            break;

          case UIFlashStorage::WOULD_OVERWRITE:
            AlertDialogBox::showError(F("Cannot overwrite existing media."));
            break;
        }
        break;
      }
      case 9: GOTO_SCREEN(ConfirmEraseFlashDialogBox); break;
    #endif
    case 10: GOTO_SCREEN(EndstopStatesScreen); break;
    default: return false;
  }
  return true;
}

#endif // FTDI_DEVELOPER_MENU
