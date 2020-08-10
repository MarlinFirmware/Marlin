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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

//
// LED Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && EITHER(LED_CONTROL_MENU, CASE_LIGHT_MENU)

#include "menu.h"

#if ENABLED(LED_CONTROL_MENU)
  #include "../../feature/leds/leds.h"

  #if ENABLED(LED_COLOR_PRESETS)
    void menu_led_presets() {
      START_MENU();
      #if LCD_HEIGHT > 2
        STATIC_ITEM(MSG_LED_PRESETS, SS_DEFAULT|SS_INVERT);
      #endif
      BACK_ITEM(MSG_LED_CONTROL);
      ACTION_ITEM(MSG_SET_LEDS_WHITE, leds.set_white);
      ACTION_ITEM(MSG_SET_LEDS_RED, leds.set_red);
      ACTION_ITEM(MSG_SET_LEDS_ORANGE, leds.set_orange);
      ACTION_ITEM(MSG_SET_LEDS_YELLOW,leds.set_yellow);
      ACTION_ITEM(MSG_SET_LEDS_GREEN, leds.set_green);
      ACTION_ITEM(MSG_SET_LEDS_BLUE, leds.set_blue);
      ACTION_ITEM(MSG_SET_LEDS_INDIGO, leds.set_indigo);
      ACTION_ITEM(MSG_SET_LEDS_VIOLET, leds.set_violet);
      END_MENU();
    }

  #endif

  void menu_led_custom() {
    START_MENU();
    BACK_ITEM(MSG_LED_CONTROL);
    EDIT_ITEM(uint8, MSG_INTENSITY_R, &leds.color.r, 0, 255, leds.update, true);
    EDIT_ITEM(uint8, MSG_INTENSITY_G, &leds.color.g, 0, 255, leds.update, true);
    EDIT_ITEM(uint8, MSG_INTENSITY_B, &leds.color.b, 0, 255, leds.update, true);
    #if EITHER(RGBW_LED, NEOPIXEL_LED)
      EDIT_ITEM(uint8, MSG_INTENSITY_W, &leds.color.w, 0, 255, leds.update, true);
      #if ENABLED(NEOPIXEL_LED)
        EDIT_ITEM(uint8, MSG_LED_BRIGHTNESS, &leds.color.i, 0, 255, leds.update, true);
      #endif
    #endif
    END_MENU();
  }
#endif

#if ENABLED(CASE_LIGHT_MENU)
  #include "../../feature/caselight.h"

  #if DISABLED(CASE_LIGHT_NO_BRIGHTNESS)
    void menu_case_light() {
      START_MENU();
      BACK_ITEM(MSG_CONFIGURATION);
      EDIT_ITEM(percent, MSG_CASE_LIGHT_BRIGHTNESS, &caselight.brightness, 0, 255, caselight.update_brightness, true);
      EDIT_ITEM(bool, MSG_CASE_LIGHT, (bool*)&caselight.on, caselight.update_enabled);
      END_MENU();
    }
  #endif
#endif



void menu_led() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);
  #if ENABLED(LED_CONTROL_MENU)
    bool led_on = leds.lights_on;
    EDIT_ITEM(bool, MSG_LEDS, &led_on, leds.toggle);
    ACTION_ITEM(MSG_SET_LEDS_DEFAULT, leds.set_default);
    #if ENABLED(LED_COLOR_PRESETS)
      SUBMENU(MSG_LED_PRESETS, menu_led_presets);
    #endif
    SUBMENU(MSG_CUSTOM_LEDS, menu_led_custom);
  #endif
  //
  // Set Case light on/off/brightness
  //
  #if ENABLED(CASE_LIGHT_MENU)
    #if DISABLED(CASE_LIGHT_NO_BRIGHTNESS)
      if (TERN1(CASE_LIGHT_USE_NEOPIXEL, PWM_PIN(CASE_LIGHT_PIN)))
        SUBMENU(MSG_CASE_LIGHT, menu_case_light);
      else
    #endif
        EDIT_ITEM(bool, MSG_CASE_LIGHT, (bool*)&caselight.on, caselight.update_enabled);
  #endif
  END_MENU();
}

#endif // HAS_LCD_MENU && LED_CONTROL_MENU
