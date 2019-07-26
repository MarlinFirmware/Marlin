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

// As only one game is active at a time, we can pool some
// or all of the game data in a union to free up RAM that
// ordinarily be reserved.

typedef struct { int8_t x, y; } pos_t;
typedef struct { int8_t x, y, v; } laser_t;

#if ENABLED(MARLIN_BRICKOUT)
  #define BRICK_ROWS   4
  #define BRICK_COLS  16
#endif

#if ENABLED(MARLIN_INVADERS)
  #define INVASION_SIZE 3

  #if INVASION_SIZE == 3
    #define INVADER_COLS   5
  #elif INVASION_SIZE == 4
    #define INVADER_COLS   6
  #else
    #define INVADER_COLS   8
    #undef INVASION_SIZE
    #define INVASION_SIZE  5
  #endif

  #define INVADER_ROWS INVASION_SIZE
#endif

#define INVADER_ROWS INVASION_SIZE

union MarlinGameData {
  #if ENABLED(MARLIN_BRICKOUT)
    struct {
      uint8_t balls_left, brick_count;
      uint16_t bricks[BRICK_ROWS];
      int8_t paddle_x, hit_dir;
      fixed_t ballx, bally, ballh, ballv;
    } brickout;
  #endif
  #if ENABLED(MARLIN_INVADERS)
    struct {
      uint8_t cannons_left;
      int8_t cannon_x;
      laser_t bullet[10], laser, explod;
      int8_t invaders_x, invaders_y, invaders_dir, leftmost, rightmost, botmost;
      uint8_t invader_count, quit_count;
      uint8_t bugs[INVADER_ROWS], shooters[(INVADER_ROWS) * (INVADER_COLS)];
      int8_t ufox, ufov;
      uint8_t blink_count;
      bool game_blink;
    } invaders;
  #endif
  #if ENABLED(MARLIN_SNAKE)
    struct {
      int8_t snake_dir, foodx, foody, food_cnt, old_encoder;
      pos_t snake_tail[50];
      fixed_t snakex, snakey;
      uint8_t head_ind;
    } snake;
  #endif
  #if ENABLED(MARLIN_MAZE)
    struct {
    } maze;
  #endif
};

extern MarlinGameData marlin_game_data;
