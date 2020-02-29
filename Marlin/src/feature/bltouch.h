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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../inc/MarlinConfigPre.h"

// BLTouch commands are sent as servo angles
typedef unsigned char BLTCommand;

#define BLTOUCH_DEPLOY          10
#define BLTOUCH_SW_MODE         60
#define BLTOUCH_STOW            90
#define BLTOUCH_SELFTEST       120
#define BLTOUCH_MODE_STORE     130
#define BLTOUCH_5V_MODE        140
#define BLTOUCH_OD_MODE        150
#define BLTOUCH_RESET          160

/**
 * The following commands require different minimum delays.
 *
 * 500ms required for a reliable Reset.
 *
 * 750ms required for Deploy/Stow, otherwise the alarm state
 *       will not be seen until the following move command.
 */

#ifndef BLTOUCH_SET5V_DELAY
  #define BLTOUCH_SET5V_DELAY   150
#endif
#ifndef BLTOUCH_SETOD_DELAY
  #define BLTOUCH_SETOD_DELAY   150
#endif
#ifndef BLTOUCH_MODE_STORE_DELAY
  #define BLTOUCH_MODE_STORE_DELAY 150
#endif
#ifndef BLTOUCH_DEPLOY_DELAY
  #define BLTOUCH_DEPLOY_DELAY   750
#endif
#ifndef BLTOUCH_STOW_DELAY
  #define BLTOUCH_STOW_DELAY     750
#endif
#ifndef BLTOUCH_RESET_DELAY
  #define BLTOUCH_RESET_DELAY    500
#endif

class BLTouch {
public:
  static void init(const bool set_voltage=false);
  static bool last_written_mode; // Initialized by settings.load, 0 = Open Drain; 1 = 5V Drain

  // DEPLOY and STOW are wrapped for error handling - these are used by homing and by probing
  FORCE_INLINE static bool deploy()              { return deploy_proc(); }
  FORCE_INLINE static bool stow()                { return stow_proc(); }
  FORCE_INLINE static bool status()              { return status_proc(); }

  // Native BLTouch commands ("Underscore"...), used in lcd menus and internally
  FORCE_INLINE static void _reset()              { command(BLTOUCH_RESET, BLTOUCH_RESET_DELAY); }

  FORCE_INLINE static void _selftest()           { command(BLTOUCH_SELFTEST, BLTOUCH_DELAY); }

  FORCE_INLINE static void _set_SW_mode()        { command(BLTOUCH_SW_MODE, BLTOUCH_DELAY); }
  FORCE_INLINE static void _reset_SW_mode()      { if (triggered()) _stow(); else _deploy(); }

  FORCE_INLINE static void _set_5V_mode()        { command(BLTOUCH_5V_MODE, BLTOUCH_SET5V_DELAY); }
  FORCE_INLINE static void _set_OD_mode()        { command(BLTOUCH_OD_MODE, BLTOUCH_SETOD_DELAY); }
  FORCE_INLINE static void _mode_store()         { command(BLTOUCH_MODE_STORE, BLTOUCH_MODE_STORE_DELAY); }

  FORCE_INLINE static void _deploy()             { command(BLTOUCH_DEPLOY, BLTOUCH_DEPLOY_DELAY); }
  FORCE_INLINE static void _stow()               { command(BLTOUCH_STOW, BLTOUCH_STOW_DELAY); }

  FORCE_INLINE static void mode_conv_5V()        { mode_conv_proc(true); }
  FORCE_INLINE static void mode_conv_OD()        { mode_conv_proc(false); }

  static bool triggered();

private:
  FORCE_INLINE static bool _deploy_query_alarm() { return command(BLTOUCH_DEPLOY, BLTOUCH_DEPLOY_DELAY); }
  FORCE_INLINE static bool _stow_query_alarm()   { return command(BLTOUCH_STOW, BLTOUCH_STOW_DELAY); }

  static void clear();
  static bool command(const BLTCommand cmd, const millis_t &ms);
  static bool deploy_proc();
  static bool stow_proc();
  static bool status_proc();
  static void mode_conv_proc(const bool M5V);
};

extern BLTouch bltouch;
