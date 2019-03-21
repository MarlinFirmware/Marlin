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

#include "../inc/MarlinConfigPre.h"

// BLTouch commands are sent as servo angles
typedef unsigned char BLTCommand;

#define BLTOUCH_DEPLOY    10
#define BLTOUCH_SW_MODE   60
#define BLTOUCH_STOW      90
#define BLTOUCH_SELFTEST 120
#define BLTOUCH_5V_MODE  140
#define BLTOUCH_OD_MODE  150
#define BLTOUCH_RESET    160

class BLTouch {
public:
  static void init();
  static void command(const BLTCommand cmd);
  static bool triggered();

  FORCE_INLINE static void reset()       { command(BLTOUCH_RESET); }
  FORCE_INLINE static void set_5V_mode() { command(BLTOUCH_5V_MODE); }
  FORCE_INLINE static void set_OD_mode() { command(BLTOUCH_OD_MODE); }
  FORCE_INLINE static void set_SW_mode() { command(BLTOUCH_SW_MODE); }

  FORCE_INLINE static bool deploy() { return set_deployed(true); }
  FORCE_INLINE static bool stow()   { return set_deployed(false); }

private:
  FORCE_INLINE static void _deploy()     { command(BLTOUCH_DEPLOY); }
  FORCE_INLINE static void _stow()       { command(BLTOUCH_STOW); }
  static bool set_deployed(const bool deploy);
};

#define BLTOUCH_ANGLES { BLTOUCH_DEPLOY, BLTOUCH_STOW }

extern BLTouch bltouch;
