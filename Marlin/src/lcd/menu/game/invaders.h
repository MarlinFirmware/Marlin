/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "types.h"

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

#define INVADER_COL_W   14
#define INVADER_H        8
#define INVADER_ROW_H   (INVADER_H + 2)

typedef struct { int8_t x, y, v; } laser_t;

typedef struct {
  pos_t pos;
  uint8_t cannons_left;
  int8_t cannon_x;
  laser_t bullet[10], laser, explod;
  int8_t dir, leftmost, rightmost, botmost;
  uint8_t count, quit_count, blink_count;
  uint8_t bugs[INVADER_ROWS], shooters[(INVADER_ROWS) * (INVADER_COLS)];
  int8_t ufox, ufov;
  bool game_blink;
  int8_t laser_col() { return ((laser.x - pos.x) / (INVADER_COL_W)); };
  int8_t laser_row() { return ((laser.y - pos.y + 2) / (INVADER_ROW_H)); };
} invaders_data_t;

class InvadersGame : MarlinGame { public: static void enter_game(), game_screen(); };

extern InvadersGame invaders;
