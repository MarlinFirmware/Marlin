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

#include "../../inc/MarlinConfigPre.h"

#if HAS_GAME_MENU

#include "menu.h"
#include "game/game.h"

void menu_game() {
  START_MENU();
  MENU_BACK(MSG_MAIN);
  #if ENABLED(MARLIN_BRICKOUT)
    MENU_ITEM(submenu, MSG_BRICKOUT, brickout.enter_game);
  #endif
  #if ENABLED(MARLIN_INVADERS)
    MENU_ITEM(submenu, MSG_INVADERS, invaders.enter_game);
  #endif
  #if ENABLED(MARLIN_SNAKE)
    MENU_ITEM(submenu, MSG_SNAKE, snake.enter_game);
  #endif
  #if ENABLED(MARLIN_MAZE)
    MENU_ITEM(submenu, MSG_MAZE, maze.enter_game);
  #endif
  END_MENU();
}

#endif // HAS_GAME_MENU

