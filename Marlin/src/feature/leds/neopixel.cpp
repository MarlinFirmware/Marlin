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

/**
 * Marlin RGB LED general support
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(NEOPIXEL_LED)

#include "neopixel.h"

#if EITHER(NEOPIXEL_STARTUP_TEST, NEOPIXEL2_STARTUP_TEST)
  #include "../../core/utility.h"
#endif

Marlin_NeoPixel neo;
int8_t Marlin_NeoPixel::neoindex;

Adafruit_NeoPixel Marlin_NeoPixel::adaneo1(NEOPIXEL_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE + NEO_KHZ800)
  #if CONJOINED_NEOPIXEL
    , Marlin_NeoPixel::adaneo2(NEOPIXEL_PIXELS, NEOPIXEL2_PIN, NEOPIXEL2_TYPE + NEO_KHZ800)
  #endif
;

#ifdef NEOPIXEL_BKGD_LED_INDEX

  void Marlin_NeoPixel::set_color_background() {
    uint8_t background_color[4] = NEOPIXEL_BKGD_COLOR;
    set_pixel_color(NEOPIXEL_BKGD_LED_INDEX, adaneo1.Color(background_color[0], background_color[1], background_color[2], background_color[3]));
  }

#endif

void Marlin_NeoPixel::set_color(const uint32_t color) {
  if (neoindex >= 0) {
    set_pixel_color(neoindex, color);
    neoindex = -1;
  }
  else {
    for (uint16_t i = 0; i < pixels(); ++i) {
      #ifdef NEOPIXEL_BKGD_LED_INDEX
        if (i == NEOPIXEL_BKGD_LED_INDEX && TERN(NEOPIXEL_BKGD_ALWAYS_ON, true, color != 0x000000)) {
          set_color_background();
          continue;
        }
      #endif
      set_pixel_color(i, color);
    }
  }
  show();
}

void Marlin_NeoPixel::set_color_startup(const uint32_t color) {
  for (uint16_t i = 0; i < pixels(); ++i)
    set_pixel_color(i, color);
  show();
}

void Marlin_NeoPixel::init() {
  neoindex = -1;                       // -1 .. NEOPIXEL_PIXELS-1 range
  set_brightness(NEOPIXEL_BRIGHTNESS); //  0 .. 255 range
  begin();
  show();  // initialize to all off

  #if ENABLED(NEOPIXEL_STARTUP_TEST)
    set_color_startup(adaneo1.Color(255, 0, 0, 0));  // red
    safe_delay(500);
    set_color_startup(adaneo1.Color(0, 255, 0, 0));  // green
    safe_delay(500);
    set_color_startup(adaneo1.Color(0, 0, 255, 0));  // blue
    safe_delay(500);
  #endif

  #ifdef NEOPIXEL_BKGD_LED_INDEX
    set_color_background();
  #endif

  #if ENABLED(LED_USER_PRESET_STARTUP)
    set_color(adaneo1.Color(LED_USER_PRESET_RED, LED_USER_PRESET_GREEN, LED_USER_PRESET_BLUE, LED_USER_PRESET_WHITE));
  #else
    set_color(adaneo1.Color(0, 0, 0, 0));
  #endif
}

#if 0
bool Marlin_NeoPixel::set_led_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w, const uint8_t p) {
  const uint32_t color = adaneo1.Color(r, g, b, w);
  set_brightness(p);
  #if DISABLED(NEOPIXEL_IS_SEQUENTIAL)
    set_color(color);
    return false;
  #else
    static uint16_t nextLed = 0;
    set_pixel_color(nextLed, color);
    show();
    if (++nextLed >= pixels()) nextLed = 0;
    return true;
  #endif
}
#endif

#if ENABLED(NEOPIXEL2_SEPARATE)

  Marlin_NeoPixel2 neo2;

  int8_t Marlin_NeoPixel2::neoindex;
  Adafruit_NeoPixel Marlin_NeoPixel2::adaneo(NEOPIXEL2_PIXELS, NEOPIXEL2_PIN, NEOPIXEL2_TYPE);

  void Marlin_NeoPixel2::set_color(const uint32_t color) {
    if (neoindex >= 0) {
      set_pixel_color(neoindex, color);
      neoindex = -1;
    }
    else {
      for (uint16_t i = 0; i < pixels(); ++i)
        set_pixel_color(i, color);
    }
    show();
  }

  void Marlin_NeoPixel2::set_color_startup(const uint32_t color) {
    for (uint16_t i = 0; i < pixels(); ++i)
      set_pixel_color(i, color);
    show();
  }

  void Marlin_NeoPixel2::init() {
    neoindex = -1;                        // -1 .. NEOPIXEL2_PIXELS-1 range
    set_brightness(NEOPIXEL2_BRIGHTNESS); //  0 .. 255 range
    begin();
    show();  // initialize to all off

    #if ENABLED(NEOPIXEL2_STARTUP_TEST)
      set_color_startup(adaneo.Color(255, 0, 0, 0));  // red
      safe_delay(500);
      set_color_startup(adaneo.Color(0, 255, 0, 0));  // green
      safe_delay(500);
      set_color_startup(adaneo.Color(0, 0, 255, 0));  // blue
      safe_delay(500);
    #endif

    #if ENABLED(NEO2_USER_PRESET_STARTUP)
      set_color(adaneo.Color(NEO2_USER_PRESET_RED, NEO2_USER_PRESET_GREEN, NEO2_USER_PRESET_BLUE, NEO2_USER_PRESET_WHITE));
    #else
      set_color(adaneo.Color(0, 0, 0, 0));
    #endif
  }

#endif // NEOPIXEL2_SEPARATE

#endif // NEOPIXEL_LED
