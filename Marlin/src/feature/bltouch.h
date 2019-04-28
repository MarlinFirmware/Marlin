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

#define BLTOUCH_DEPLOY          10
#define BLTOUCH_SW_MODE         60
#define BLTOUCH_STOW            90
#define BLTOUCH_SELFTEST       120
#define BLTOUCH_5V_MODE        140
#define BLTOUCH_OD_MODE        150
#define BLTOUCH_RESET          160

// The following delays are needed for some of the commands, instead of the BLTOUCH_DELAY (default 375)
#define BLTOUCH_RESET_DELAY    500
#define BLTOUCH_SET5V_DELAY   2000 // This is according to ANTClabs. Seems a bit high???
// Set the following to zero for highspeed probing mode. Works fine, but does not catch deploy or stow
// errors after the command - instead the error catching is deffered to the subsequent command. Printer
// won't stop, instead it will skip the failed probes. If you consider your probe to reliable beyond
// a doubt, you can run this way. Not recommended but YMMV
#if ENABLED(BLTOUCH_HS_MODE)
  #define BLTOUCH_DEPLOY_DELAY   0 // So we won't recognize DEPLOY fails right away
  #define BLTOUCH_STOW_DELAY     0 // So we won't recognize STOW fails right away
  #undef  BLTOUCH_DELAY
  #define BLTOUCH_DELAY        150 // Override
#else
  #define BLTOUCH_DEPLOY_DELAY 750 // So that we can recognize DEPLOY fails
  #define BLTOUCH_STOW_DELAY   750 // So that we can recognize STOW fails
#endif

class BLTouch {
public:
  static void command(const BLTCommand cmd);
  static void init();
  static bool triggered();

  // Native BLTouch commands ("Underscore"...)
  FORCE_INLINE static void _reset()       { command(BLTOUCH_RESET); }

  FORCE_INLINE static void _selftest()    { command(BLTOUCH_SELFTEST); }

  FORCE_INLINE static void _set_SW_mode() { command(BLTOUCH_SW_MODE); }
  FORCE_INLINE static void _set_5V_mode() { command(BLTOUCH_5V_MODE); }
  FORCE_INLINE static void _set_OD_mode() { command(BLTOUCH_OD_MODE); }

  FORCE_INLINE static void _deploy()      { command(BLTOUCH_DEPLOY); }
  FORCE_INLINE static void _stow()        { command(BLTOUCH_STOW); }

  // DEPLOY and STOW are wrapped for error handling
  FORCE_INLINE static bool deploy()       { return deploy_stow_wrapper(true); }
  FORCE_INLINE static bool stow()         { return deploy_stow_wrapper(false); }

private:
  static bool deploy_stow_wrapper(const bool deploy);
  static void reset();
  static void clear();
};

// This transfers the two needed angle values to the servo.cpp/servo.h routine
#define BLTOUCH_ANGLES { BLTOUCH_DEPLOY, BLTOUCH_STOW }

extern BLTouch bltouch;
