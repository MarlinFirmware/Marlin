/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

//
// SD Card Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(SDSUPPORT)

#include "menu.h"
#include "../../sd/cardreader.h"

#if !PIN_EXISTS(SD_DETECT)
  void lcd_sd_refresh() {
    card.initsd();
    encoderTopLine = 0;
  }
#endif

void lcd_sd_updir() {
  ui.encoderPosition = card.updir() ? ENCODER_STEPS_PER_MENU_ITEM : 0;
  encoderTopLine = 0;
  screen_changed = true;
  ui.refresh();
}

#if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)

  uint32_t last_sdfile_encoderPosition = 0xFFFF;

  void MarlinUI::reselect_last_file() {
    if (last_sdfile_encoderPosition == 0xFFFF) return;
    //#if HAS_GRAPHICAL_LCD
    //  // This is a hack to force a screen update.
    //  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
    //  ui.synchronize();
    //  safe_delay(50);
    //  ui.synchronize();
    //  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
    //  ui.drawing_screen = screen_changed = true;
    //#endif

    goto_screen(menu_sdcard, last_sdfile_encoderPosition);
    last_sdfile_encoderPosition = 0xFFFF;

    defer_status_screen(true);

    //#if HAS_GRAPHICAL_LCD
    //  update();
    //#endif
  }
#endif

class menu_item_sdfile {
  public:
    static void action(CardReader &theCard) {
      #if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
        last_sdfile_encoderPosition = ui.encoderPosition;  // Save which file was selected for later use
      #endif
      card.openAndPrintFile(theCard.filename);
      ui.return_to_status();
      ui.reset_status();
    }
};

class menu_item_sdfolder {
  public:
    static void action(CardReader &theCard) {
      card.chdir(theCard.filename);
      encoderTopLine = 0;
      ui.encoderPosition = 2 * ENCODER_STEPS_PER_MENU_ITEM;
      screen_changed = true;
      #if HAS_GRAPHICAL_LCD
        ui.drawing_screen = false;
      #endif
      ui.refresh();
    }
};

void menu_sdcard() {
  ui.encoder_direction_menus();

  const uint16_t fileCnt = card.get_num_Files();

  START_MENU();
  MENU_BACK(MSG_MAIN);
  card.getWorkDirName();
  if (card.filename[0] == '/') {
    #if !PIN_EXISTS(SD_DETECT)
      MENU_ITEM(function, LCD_STR_REFRESH MSG_REFRESH, lcd_sd_refresh);
    #endif
  }
  else {
    MENU_ITEM(function, LCD_STR_FOLDER "..", lcd_sd_updir);
  }

  for (uint16_t i = 0; i < fileCnt; i++) {
    if (_menuLineNr == _thisItemNr) {
      const uint16_t nr =
        #if ENABLED(SDCARD_RATHERRECENTFIRST) && DISABLED(SDCARD_SORT_ALPHA)
          fileCnt - 1 -
        #endif
      i;

      card.getfilename_sorted(nr);

      if (card.filenameIsDir)
        MENU_ITEM(sdfolder, MSG_CARD_MENU, card);
      else
        MENU_ITEM(sdfile, MSG_CARD_MENU, card);
    }
    else {
      MENU_ITEM_DUMMY();
    }
  }
  END_MENU();
}

#endif // HAS_LCD_MENU && SDSUPPORT
