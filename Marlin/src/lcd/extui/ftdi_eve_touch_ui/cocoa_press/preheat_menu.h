/******************************
 * cocoa_press/preheat_menu.h *
 ******************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2020 - Cocoa Press                          *
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

#define COCOA_PREHEAT_MENU
#define COCOA_PREHEAT_MENU_CLASS PreheatMenu

class PreheatMenu : public BaseScreen, public CachedScreen<PREHEAT_MENU_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};
