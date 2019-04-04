/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../../../inc/MarlinConfigPre.h"
#include "../../dogm/ultralcd_DOGM.h"
#include "../../lcdprint.h"
#include "../../ultralcd.h"

//#define MUTE_GAMES

#ifdef MUTE_GAMES
  #define _BUZZ(D,F) NOOP
#else
  #define _BUZZ(D,F) BUZZ(D,F)
#endif

// Simple 8:8 fixed-point
typedef int16_t fixed_t;
#define FTOP(F) fixed_t((F)*256.0f)
#define PTOF(P) (float(P)*(1.0f/256.0f))
#define BTOF(X) (fixed_t(X)<<8)
#define FTOB(X) int8_t(fixed_t(X)>>8)

#define SCREEN_M ((LCD_PIXEL_WIDTH) / 2)

#if HAS_GAME_MENU
  void menu_game();
#endif

class MarlinGame {
protected:
  static int score;
  static uint8_t game_state;
  static millis_t next_frame;

  static bool game_frame();
  static void draw_game_over();
  static void exit_game();
public:
  static void init_game(const uint8_t init_state, const screenFunc_t screen);
};

#if ENABLED(MARLIN_BRICKOUT)
  class BrickoutGame : MarlinGame { public: static void enter_game(); static void game_screen(); };
  extern BrickoutGame brickout;
#endif
#if ENABLED(MARLIN_INVADERS)
  class InvadersGame : MarlinGame { public: static void enter_game(); static void game_screen(); };
  extern InvadersGame invaders;
#endif
#if ENABLED(MARLIN_SNAKE)
  class SnakeGame : MarlinGame { public: static void enter_game(); static void game_screen(); };
  extern SnakeGame snake;
#endif
#if ENABLED(MARLIN_MAZE)
  class MazeGame : MarlinGame { public: static void enter_game(); static void game_screen(); };
  extern MazeGame maze;
#endif
