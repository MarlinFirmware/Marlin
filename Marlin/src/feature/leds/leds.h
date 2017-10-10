/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Marlin general RGB LED support
 */

#ifndef __LEDS_H__
#define __LEDS_H__

#if ENABLED(NEOPIXEL_LED)
  #include <Adafruit_NeoPixel.h>
  #include "neopixel.h"
#endif

#if ENABLED(BLINKM)
  #include "blinkm.h"
#endif

#if ENABLED(PCA9632)
  #include "pca9632.h"
#endif

#if ENABLED(NEOPIXEL_LED) 
  #if NEOPIXEL_TYPE == NEO_RGB || NEOPIXEL_TYPE == NEO_RBG || NEOPIXEL_TYPE == NEO_GRB || NEOPIXEL_TYPE == NEO_GBR || NEOPIXEL_TYPE == NEO_BRG || NEOPIXEL_TYPE == NEO_BGR
    #define NEO_WHITE 255, 255, 255
  #else
    #define NEO_WHITE 0, 0, 0, 255
  #endif
#endif

#if ENABLED(RGB_LED) || ENABLED(BLINKM) || ENABLED(PCA9632)
  #define LED_WHITE 255, 255, 255
#elif ENABLED(RGBW_LED)
  #define LED_WHITE 0, 0, 0, 255
#endif

#if ENABLED(NEOPIXEL_LED)
  #define LED_BRIGHTNESS pixels.getBrightness()
#else
  #define LED_BRIGHTNESS 255
#endif

void set_led_color(
  const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w = 0, const uint8_t p = 255
);

#endif // __LEDS_H__
