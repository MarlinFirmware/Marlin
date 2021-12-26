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
 * leds.cpp - Marlin RGB LED general support
 */

#include "../../inc/MarlinConfig.h"

#if HAS_COLOR_LEDS

#include "leds.h"

#if ENABLED(BLINKM)
  #include "blinkm.h"
#endif

#if ENABLED(PCA9632)
  #include "pca9632.h"
#endif

#if ENABLED(PCA9533)
  #include "pca9533.h"
#endif

#if ENABLED(CASE_LIGHT_USE_RGB_LED)
  #include "../../feature/caselight.h"
#endif

#if ENABLED(LED_COLOR_PRESETS)
  const LEDColor LEDLights::defaultLEDColor = LEDColor(
    LED_USER_PRESET_RED, LED_USER_PRESET_GREEN, LED_USER_PRESET_BLUE
    OPTARG(HAS_WHITE_LED, LED_USER_PRESET_WHITE)
    OPTARG(NEOPIXEL_LED, LED_USER_PRESET_BRIGHTNESS)
  );
#endif

#if ANY(LED_CONTROL_MENU, PRINTER_EVENT_LEDS, CASE_LIGHT_IS_COLOR_LED)
  LEDColor LEDLights::color;
  bool LEDLights::lights_on;
#endif

LEDLights leds;

void LEDLights::setup() {
  #if EITHER(RGB_LED, RGBW_LED)
    if (PWM_PIN(RGB_LED_R_PIN)) SET_PWM(RGB_LED_R_PIN); else SET_OUTPUT(RGB_LED_R_PIN);
    if (PWM_PIN(RGB_LED_G_PIN)) SET_PWM(RGB_LED_G_PIN); else SET_OUTPUT(RGB_LED_G_PIN);
    if (PWM_PIN(RGB_LED_B_PIN)) SET_PWM(RGB_LED_B_PIN); else SET_OUTPUT(RGB_LED_B_PIN);
    #if ENABLED(RGBW_LED)
      if (PWM_PIN(RGB_LED_W_PIN)) SET_PWM(RGB_LED_W_PIN); else SET_OUTPUT(RGB_LED_W_PIN);
    #endif
  #endif
  TERN_(NEOPIXEL_LED, neo.init());
  TERN_(PCA9533, PCA9533_init());
  TERN_(LED_USER_PRESET_STARTUP, set_default());
}

void LEDLights::set_color(const LEDColor &incol
  OPTARG(NEOPIXEL_IS_SEQUENTIAL, bool isSequence/*=false*/)
) {

  #if ENABLED(NEOPIXEL_LED)

    const uint32_t neocolor = LEDColorWhite() == incol
                            ? neo.Color(NEO_WHITE)
                            : neo.Color(incol.r, incol.g, incol.b OPTARG(HAS_WHITE_LED, incol.w));

    #if ENABLED(NEOPIXEL_IS_SEQUENTIAL)
      static uint16_t nextLed = 0;
      #ifdef NEOPIXEL_BKGD_INDEX_FIRST
        while (WITHIN(nextLed, NEOPIXEL_BKGD_INDEX_FIRST, NEOPIXEL_BKGD_INDEX_LAST)) {
          neo.reset_background_color();
          if (++nextLed >= neo.pixels()) { nextLed = 0; return; }
        }
      #endif
    #endif

    neo.set_brightness(incol.i);

    #if ENABLED(NEOPIXEL_IS_SEQUENTIAL)
      if (isSequence) {
        neo.set_pixel_color(nextLed, neocolor);
        neo.show();
        if (++nextLed >= neo.pixels()) nextLed = 0;
        return;
      }
    #endif

    neo.set_color(neocolor);

  #endif

  #if ENABLED(BLINKM)

    // This variant uses i2c to send the RGB components to the device.
    blinkm_set_led_color(incol);

  #endif

  #if EITHER(RGB_LED, RGBW_LED)

    // This variant uses 3-4 separate pins for the RGB(W) components.
    // If the pins can do PWM then their intensity will be set.
    #define _UPDATE_RGBW(C,c) do {                 \
      if (PWM_PIN(RGB_LED_##C##_PIN))              \
        set_pwm_duty(pin_t(RGB_LED_##C##_PIN), c); \
      else                                         \
        WRITE(RGB_LED_##C##_PIN, c ? HIGH : LOW);  \
    }while(0)
    #define UPDATE_RGBW(C,c) _UPDATE_RGBW(C, TERN1(CASE_LIGHT_USE_RGB_LED, caselight.on) ? incol.c : 0)
    UPDATE_RGBW(R,r); UPDATE_RGBW(G,g); UPDATE_RGBW(B,b);
    #if ENABLED(RGBW_LED)
      UPDATE_RGBW(W,w);
    #endif

  #endif

  // Update I2C LED driver
  TERN_(PCA9632, PCA9632_set_led_color(incol));
  TERN_(PCA9533, PCA9533_set_rgb(incol.r, incol.g, incol.b));

  #if EITHER(LED_CONTROL_MENU, PRINTER_EVENT_LEDS)
    // Don't update the color when OFF
    lights_on = !incol.is_off();
    if (lights_on) color = incol;
  #endif
}

#if ENABLED(LED_CONTROL_MENU)
  void LEDLights::toggle() { if (lights_on) set_off(); else update(); }
#endif

#ifdef LED_BACKLIGHT_TIMEOUT

  millis_t LEDLights::led_off_time; // = 0

  void LEDLights::update_timeout(const bool power_on) {
    if (lights_on) {
      const millis_t ms = millis();
      if (power_on)
        reset_timeout(ms);
      else if (ELAPSED(ms, led_off_time))
        set_off();
    }
  }

#endif

#if ENABLED(NEOPIXEL2_SEPARATE)

  #if ENABLED(NEO2_COLOR_PRESETS)
    const LEDColor LEDLights2::defaultLEDColor = LEDColor(
      NEO2_USER_PRESET_RED, NEO2_USER_PRESET_GREEN, NEO2_USER_PRESET_BLUE
      OPTARG(HAS_WHITE_LED2, NEO2_USER_PRESET_WHITE)
      OPTARG(NEOPIXEL_LED, NEO2_USER_PRESET_BRIGHTNESS)
    );
  #endif

  #if ENABLED(LED_CONTROL_MENU)
    LEDColor LEDLights2::color;
    bool LEDLights2::lights_on;
  #endif

  LEDLights2 leds2;

  void LEDLights2::setup() {
    neo2.init();
    TERN_(NEO2_USER_PRESET_STARTUP, set_default());
  }

  void LEDLights2::set_color(const LEDColor &incol) {
    const uint32_t neocolor = LEDColorWhite() == incol
                            ? neo2.Color(NEO2_WHITE)
                            : neo2.Color(incol.r, incol.g, incol.b OPTARG(HAS_WHITE_LED2, incol.w));
    neo2.set_brightness(incol.i);
    neo2.set_color(neocolor);

    #if ENABLED(LED_CONTROL_MENU)
      // Don't update the color when OFF
      lights_on = !incol.is_off();
      if (lights_on) color = incol;
    #endif
  }

  #if ENABLED(LED_CONTROL_MENU)
    void LEDLights2::toggle() { if (lights_on) set_off(); else update(); }
  #endif

#endif  // NEOPIXEL2_SEPARATE

#endif  // HAS_COLOR_LEDS
