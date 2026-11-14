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
 * lcd/dogm/status_screen_PANCAKEBOT.cpp
 *
 * Status screen for the 128x32 PancakeBot Panel.
 *
 *  - An SD card icon and a batter bottle icon (full while printing).
 *  - A progress bar for the SD print, or the status message when idle.
 *  - After the feedrate is changed, the new speed is shown for a few seconds.
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(PANCAKEBOT_PANEL)

#include "marlinui_DOGM.h"
#include "status_pancakebot.h"

#include "../marlinui.h"
#include "../lcdprint.h"
#include "../../libs/numtostr.h"
#include "../../module/motion.h"
#include "../../sd/cardreader.h"

static_assert(sizeof(pancakebot_sd_bmp) == (PANCAKEBOT_ICON_BYTEWIDTH) * (PANCAKEBOT_ICON_HEIGHT), "Bad PancakeBot SD icon size.");
static_assert(sizeof(pancakebot_nosd_bmp) == (PANCAKEBOT_ICON_BYTEWIDTH) * (PANCAKEBOT_ICON_HEIGHT), "Bad PancakeBot No-SD icon size.");
static_assert(sizeof(pancakebot_bottle_full_bmp) == (PANCAKEBOT_ICON_BYTEWIDTH) * (PANCAKEBOT_ICON_HEIGHT), "Bad PancakeBot full bottle icon size.");
static_assert(sizeof(pancakebot_bottle_empty_bmp) == (PANCAKEBOT_ICON_BYTEWIDTH) * (PANCAKEBOT_ICON_HEIGHT), "Bad PancakeBot empty bottle icon size.");

#define PB_SPEED_VIEW_MS   5000     // How long to show a speed change
#define PB_BAR_X             41
#define PB_BAR_Y              7
#define PB_BAR_WIDTH         87
#define PB_BAR_HEIGHT        18
#define PB_TEXT_X            (PB_BAR_X + 4)
#define PB_TEXT_BASELINE     (PB_BAR_Y + PB_BAR_HEIGHT - 5)

void MarlinUI::draw_status_screen() {
  static int16_t old_feedrate = 100;
  static millis_t speed_view_end_ms = 0;

  // Notice when the speed is changed, and show it for a while
  if (first_page) {
    const millis_t ms = millis();
    if (old_feedrate != motion.feedrate_percentage) {
      old_feedrate = motion.feedrate_percentage;
      speed_view_end_ms = ms + (PB_SPEED_VIEW_MS);
    }
    else if (speed_view_end_ms && ELAPSED(ms, speed_view_end_ms))
      speed_view_end_ms = 0;
  }

  set_font(FONT_STATUSMENU);

  // Speed view
  if (speed_view_end_ms) {
    lcd_moveto(0, 14);
    lcd_put_u8str(GET_TEXT_F(MSG_SPEED));
    lcd_moveto(0, 30);
    lcd_put_u8str(i16tostr3rj(old_feedrate));
    lcd_put_lchar('%');
    return;
  }

  // Icons
  u8g.drawBitmapP(0, 0, PANCAKEBOT_ICON_BYTEWIDTH, PANCAKEBOT_ICON_HEIGHT, card.isMounted() ? pancakebot_sd_bmp : pancakebot_nosd_bmp);

  const bool printing = card.isStillPrinting();
  u8g.drawBitmapP(20, 0, PANCAKEBOT_ICON_BYTEWIDTH, PANCAKEBOT_ICON_HEIGHT, printing ? pancakebot_bottle_full_bmp : pancakebot_bottle_empty_bmp);

  // Progress bar frame
  u8g.drawFrame(PB_BAR_X, PB_BAR_Y, PB_BAR_WIDTH, PB_BAR_HEIGHT);

  if (printing)
    u8g.drawBox(PB_BAR_X + 1, PB_BAR_Y + 1, u8g_uint_t((PB_BAR_WIDTH - 2) * uint16_t(card.percentDone()) / 100), PB_BAR_HEIGHT - 2);
  else {
    lcd_moveto(PB_TEXT_X, PB_TEXT_BASELINE);
    draw_status_message(false);
  }
}

// Draw the status message (without scrolling) in the progress bar
void MarlinUI::draw_status_message(const bool) {
  lcd_put_u8str_max(status_message, PB_BAR_WIDTH - 6);
}

#endif // PANCAKEBOT_PANEL
