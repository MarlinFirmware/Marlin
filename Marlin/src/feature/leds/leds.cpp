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
 * Marlin RGB LED general support
 */

#include "../../inc/MarlinConfig.h"

#if HAS_COLOR_LEDS

#include "leds.h"

void set_led_color(
  const uint8_t r, const uint8_t g, const uint8_t b , const uint8_t w , const uint8_t p
) {

  #if ENABLED(NEOPIXEL_LED)
    if (w == 255 || (r == 255 && g == 255 && b == 255))
      neopixel_set_led_color(NEO_WHITE, p);
    else
      neopixel_set_led_color(r, g, b, w, p);
    return;
  #endif

  #if ENABLED(BLINKM)
    blinkm_set_led_color(r, g, b); // Use i2c to send the RGB components to the device.
  #endif

  #if ENABLED(RGB_LED) || ENABLED(RGBW_LED)
    // This variant uses 3 separate pins for the RGB components.
    // If the pins can do PWM then their intensity will be set.
    WRITE(RGB_LED_R_PIN, r ? HIGH : LOW);
    WRITE(RGB_LED_G_PIN, g ? HIGH : LOW);
    WRITE(RGB_LED_B_PIN, b ? HIGH : LOW);
    analogWrite(RGB_LED_R_PIN, r);
    analogWrite(RGB_LED_G_PIN, g);
    analogWrite(RGB_LED_B_PIN, b);

    #if ENABLED(RGBW_LED)
      WRITE(RGB_LED_W_PIN, w ? HIGH : LOW);
      analogWrite(RGB_LED_W_PIN, w);
    #endif
  #endif

  #if ENABLED(PCA9632)
    pca9632_set_led_color(r, g, b); // Update I2C LED driver
  #endif
}

#endif // HAS_COLOR_LEDS
