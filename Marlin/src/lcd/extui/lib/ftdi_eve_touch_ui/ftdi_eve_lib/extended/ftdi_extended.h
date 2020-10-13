/*******************
 * ftdi_extended.h *
 *******************/

/****************************************************************************
 *   Written By Mark Pelletier  2019 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 201( - Aleph Objects, Inc.                  *
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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

#include "../compat.h"
#include "../basic/ftdi_basic.h"

#ifndef __MARLIN_FIRMWARE__
  #define FTDI_EXTENDED
#endif

#ifdef FTDI_EXTENDED
  #include "unicode/font_size_t.h"
  #include "unicode/unicode.h"
  #include "unicode/standard_char_set.h"
  #include "unicode/western_char_set.h"
  #include "unicode/font_bitmaps.h"
  #include "rgb_t.h"
  #include "bitmap_info.h"
  #include "tiny_timer.h"
  #include "grid_layout.h"
  #include "dl_cache.h"
  #include "event_loop.h"
  #include "command_processor.h"
  #include "screen_types.h"
  #include "sound_player.h"
  #include "sound_list.h"
  #include "polygon.h"
  #include "text_box.h"
  #include "text_ellipsis.h"
#endif
