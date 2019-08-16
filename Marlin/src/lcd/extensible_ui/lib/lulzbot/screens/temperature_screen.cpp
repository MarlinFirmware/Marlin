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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void TemperatureScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0).color(temp).units(PSTR("C"));
  w.heading(         PSTR("Temperature:"));
  w.button(30, PSTR("Cooldown (All Off)"));
  #ifndef LULZBOT_DISABLE_TOOLHEAD_HEATER
    #if HOTENDS == 1
      w.adjuster(   2, PSTR("Hot End:"),   getTargetTemp_celsius(E0));
    #else
      w.adjuster(   2, PSTR("Hot End 1:"), getTargetTemp_celsius(E0));
      w.adjuster(   4, PSTR("Hot End 2:"), getTargetTemp_celsius(E1));
      #if HOTENDS > 2
        w.adjuster( 6, PSTR("Hot End 3:"), getTargetTemp_celsius(E2));
      #endif
      #if HOTENDS > 3
        w.adjuster( 8, PSTR("Hot End 4:"), getTargetTemp_celsius(E3));
      #endif
    #endif
  #endif
  #if HAS_HEATED_BED
    w.adjuster(    20, PSTR("Bed:"),     getTargetTemp_celsius(BED));
  #endif
  #if FAN_COUNT > 0
    w.color(fan_speed).units(PSTR("%"));
    w.adjuster(    10, PSTR("Fan Speed:"), getTargetFan_percent(FAN0));
  #endif
  w.increments();
}

bool TemperatureScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case 20: UI_DECREMENT(TargetTemp_celsius, BED); break;
    case 21: UI_INCREMENT(TargetTemp_celsius, BED); break;
    #ifndef LULZBOT_DISABLE_TOOLHEAD_HEATER
    case  2: UI_DECREMENT(TargetTemp_celsius, E0); break;
    case  3: UI_INCREMENT(TargetTemp_celsius, E0); break;
    #endif
    #if HOTENDS > 1
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
    #if FAN_COUNT > 0
    case 10: UI_DECREMENT(TargetFan_percent, FAN0);      break;
    case 11: UI_INCREMENT(TargetFan_percent, FAN0);      break;
    #endif
    case 30:
      setTargetTemp_celsius(0,E0);
      #if HOTENDS > 1
        setTargetTemp_celsius(0,E1);
        #if HOTENDS > 2
          setTargetTemp_celsius(0,E2);
          #if HOTENDS > 3
            setTargetTemp_celsius(0,E4);
          #endif
        #endif
      #endif
      #if HAS_HEATED_BED
        setTargetTemp_celsius(0,BED);
      #endif
      #if FAN_COUNT > 0
        setTargetFan_percent(0,FAN0);
      #endif
      break;
    default:
      return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI
