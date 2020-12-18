/*******************
 * boot_screen.cpp *
 *******************/

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

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void TemperatureScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  #if TOUCH_UI_LCD_TEMP_SCALING == 10
    w.precision(1, DEFAULT_MIDRANGE)
  #else
    w.precision(0, getTargetTemp_celsius(E0) == 0 ? DEFAULT_HIGHEST : DEFAULT_MIDRANGE)
  #endif
   .color(temp).units(GET_TEXT_F(MSG_UNITS_C));
  w.heading(GET_TEXT_F(MSG_TEMPERATURE));
  w.button(30, GET_TEXT_F(MSG_COOLDOWN));
  #ifndef NO_TOOLHEAD_HEATER_GCODE
    #ifdef TOUCH_UI_COCOA_PRESS
      w.adjuster(   2, GET_TEXT_F(MSG_NOZZLE), getTargetTemp_celsius(E0));
      w.adjuster(   4, GET_TEXT_F(MSG_BODY), getTargetTemp_celsius(E1));
      #if ENABLED(COCOA_PRESS_EXTRA_HEATER)
        if (has_extra_heater())
          w.adjuster(6, GET_TEXT_F(MSG_EXTERNAL), getTargetTemp_celsius(E2));
      #endif
    #elif HOTENDS == 1
      w.adjuster(   2, GET_TEXT_F(MSG_NOZZLE),   getTargetTemp_celsius(E0));
    #else
      w.adjuster(   2, F(LCD_STR_E0), getTargetTemp_celsius(E0));
      w.adjuster(   4, F(LCD_STR_E1), getTargetTemp_celsius(E1));
      #if HOTENDS > 2
        w.adjuster( 6, F(LCD_STR_E2), getTargetTemp_celsius(E2));
      #endif
      #if HOTENDS > 3
        w.adjuster( 8, F(LCD_STR_E3), getTargetTemp_celsius(E3));
      #endif
    #endif
  #endif
  #if HAS_HEATED_BED
    w.adjuster(    20, GET_TEXT_F(MSG_BED),     getTargetTemp_celsius(BED));
  #endif
  #if HAS_HEATED_CHAMBER
    w.adjuster(    22, GET_TEXT_F(MSG_CHAMBER), getTargetTemp_celsius(CHAMBER));
  #endif
  #if HAS_FAN
    w.color(fan_speed).units(GET_TEXT_F(MSG_UNITS_PERCENT));
    w.adjuster(    10, GET_TEXT_F(MSG_FAN_SPEED), getTargetFan_percent(FAN0));
  #endif
  w.increments();
}

bool TemperatureScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case 20: UI_DECREMENT(TargetTemp_celsius, BED); break;
    case 21: UI_INCREMENT(TargetTemp_celsius, BED); break;
    case 22: UI_DECREMENT(TargetTemp_celsius, CHAMBER); break;
    case 23: UI_INCREMENT(TargetTemp_celsius, CHAMBER); break;
    #ifndef NO_TOOLHEAD_HEATER_GCODE
      case  2: UI_DECREMENT(TargetTemp_celsius, E0); break;
      case  3: UI_INCREMENT(TargetTemp_celsius, E0); break;
    #endif
    #if HAS_MULTI_HOTEND
      case  4: UI_DECREMENT(TargetTemp_celsius, E1); break;
      case  5: UI_INCREMENT(TargetTemp_celsius, E1); break;
    #endif
    #if HOTENDS > 2
      case  6: UI_DECREMENT(TargetTemp_celsius, E2); break;
      case  7: UI_INCREMENT(TargetTemp_celsius, E2); break;
    #endif
    #if HOTENDS > 3
      case  8: UI_DECREMENT(TargetTemp_celsius, E3); break;
      case  9: UI_INCREMENT(TargetTemp_celsius, E3); break;
    #endif
    #if HAS_FAN
      case 10: UI_DECREMENT(TargetFan_percent, FAN0); break;
      case 11: UI_INCREMENT(TargetFan_percent, FAN0); break;
    #endif
    case 30:
      #define _HOTEND_OFF(N) setTargetTemp_celsius(0, E##N);
      REPEAT(HOTENDS, _HOTEND_OFF);
      TERN_(HAS_HEATED_BED, setTargetTemp_celsius(0, BED));
      TERN_(HAS_HEATED_CHAMBER, setTargetTemp_celsius(0, CHAMBER));
      #if HAS_FAN
        setTargetFan_percent(0, FAN0);
      #endif
      break;
    default:
      return false;
  }
  return true;
}

#endif // TOUCH_UI_FTDI_EVE
