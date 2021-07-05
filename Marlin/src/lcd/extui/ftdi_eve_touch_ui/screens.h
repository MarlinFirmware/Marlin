/*************
 * screens.h *
 *************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#include "config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "ftdi_eve_lib/ftdi_eve_lib.h"
#include "language/language.h"
#include "theme/theme.h"
#include "generic/string_format.h"

#ifndef BED_LEVELING_COMMANDS
  #define BED_LEVELING_COMMANDS "G29"
#endif

extern tiny_timer_t refresh_timer;

#if ENABLED(TOUCH_UI_LULZBOT_BIO)
  #include "bioprinter/screens.h"
#elif ENABLED(TOUCH_UI_COCOA_PRESS)
  #include "cocoapress/screens.h"
#elif ENABLED(TOUCH_UI_SYNDAVER_LEVEL)
  #include "syndaver_level/screens.h"
#else
  #include "generic/screens.h"
#endif

#endif // TOUCH_UI_FTDI_EVE
