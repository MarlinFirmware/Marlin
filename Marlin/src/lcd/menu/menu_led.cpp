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

//
// LED Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(LED_CONTROL_MENU)

#include "menu.h"
#include "../../feature/leds/leds.h"

#if ENABLED(LED_COLOR_PRESETS)

  void menu_led_presets() {
    START_MENU();
    #if LCD_HEIGHT > 2
      STATIC_ITEM(MSG_LED_PRESETS, true, true);
    #endif
    MENU_BACK(MSG_LED_CONTROL);
    MENU_ITEM(function, MSG_SET_LEDS_WHITE, leds.set_white);
    MENU_ITEM(function, MSG_SET_LEDS_RED, leds.set_red);
    MENU_ITEM(function, MSG_SET_LEDS_ORANGE, leds.set_orange);
    MENU_ITEM(function, MSG_SET_LEDS_YELLOW,leds.set_yellow);
    MENU_ITEM(function, MSG_SET_LEDS_GREEN, leds.set_green);
    MENU_ITEM(function, MSG_SET_LEDS_BLUE, leds.set_blue);
    MENU_ITEM(function, MSG_SET_LEDS_INDIGO, leds.set_indigo);
    MENU_ITEM(function, MSG_SET_LEDS_VIOLET, leds.set_violet);
    END_MENU();
  }

#endif

void menu_led_custom() {
  START_MENU();
  MENU_BACK(MSG_LED_CONTROL);
  MENU_ITEM_EDIT_CALLBACK(uint8, MSG_INTENSITY_R, &leds.color.r, 0, 255, leds.update, true);
  MENU_ITEM_EDIT_CALLBACK(uint8, MSG_INTENSITY_G, &leds.color.g, 0, 255, leds.update, true);
  MENU_ITEM_EDIT_CALLBACK(uint8, MSG_INTENSITY_B, &leds.color.b, 0, 255, leds.update, true);
  #if ENABLED(RGBW_LED) || ENABLED(NEOPIXEL_LED)
    MENU_ITEM_EDIT_CALLBACK(uint8, MSG_INTENSITY_W, &leds.color.w, 0, 255, leds.update, true);
    #if ENABLED(NEOPIXEL_LED)
      MENU_ITEM_EDIT_CALLBACK(uint8, MSG_LED_BRIGHTNESS, &leds.color.i, 0, 255, leds.update, true);
    #endif
  #endif
  END_MENU();
}

void menu_led() {
  START_MENU();
  MENU_BACK(MSG_MAIN);
  bool led_on = leds.lights_on;
  MENU_ITEM_EDIT_CALLBACK(bool, MSG_LEDS, &led_on, leds.toggle);
  MENU_ITEM(function, MSG_SET_LEDS_DEFAULT, leds.set_default);
  #if ENABLED(LED_COLOR_PRESETS)
    MENU_ITEM(submenu, MSG_LED_PRESETS, menu_led_presets);
  #endif
  MENU_ITEM(submenu, MSG_CUSTOM_LEDS, menu_led_custom);
  END_MENU();
}

#endif // HAS_LCD_MENU && LED_CONTROL_MENU
