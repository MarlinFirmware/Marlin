/****************
 * ftdi_basic.h *
 ****************/

/****************************************************************************
 *   Written By Mark Pelletier  2019 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2019 - Aleph Objects, Inc.                  *
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

#ifndef __MARLIN_FIRMWARE__
  #define FTDI_BASIC
#endif

#ifdef FTDI_BASIC
  #include "registers_ft800.h"
  #include "registers_ft810.h"
  #include "constants.h"
  #include "boards.h"
  #include "commands.h"
  #include "spi.h"
  #include "display_list.h"
  #include "resolutions.h"
#endif
