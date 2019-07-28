/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_GAMES

#include "game.h"

int MarlinGame::score;
uint8_t MarlinGame::game_state;
millis_t MarlinGame::next_frame;

bool MarlinGame::game_frame() {
  static int8_t slew;
  if (ui.first_page) slew = 2;
  ui.refresh(LCDVIEW_CALL_NO_REDRAW); // Refresh as often as possible
  return (game_state && slew-- > 0);
}

void MarlinGame::draw_game_over() {
  constexpr int8_t gowide = (MENU_FONT_WIDTH) * 9,
                   gohigh = MENU_FONT_ASCENT - 3,
                       lx = (LCD_PIXEL_WIDTH - gowide) / 2,
                       ly = (LCD_PIXEL_HEIGHT + gohigh) / 2;
  if (PAGE_CONTAINS(ly - gohigh - 1, ly + 1)) {
    u8g.setColorIndex(0);
    u8g.drawBox(lx - 1, ly - gohigh - 1, gowide + 2, gohigh + 2);
    u8g.setColorIndex(1);
    if (ui.get_blink()) {
      lcd_moveto(lx, ly);
      lcd_put_u8str_P(PSTR("GAME OVER"));
    }
  }
}

void MarlinGame::init_game(const uint8_t init_state, const screenFunc_t screen) {
  score = 0;
  game_state = init_state;
  ui.goto_screen(screen);
  ui.defer_status_screen();
}

void MarlinGame::exit_game() {
  ui.goto_previous_screen_no_defer();
}

#endif // HAS_GAMES
