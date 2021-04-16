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

#include "../inc/MarlinConfig.h"

#if ENABLED(CASE_LIGHT_ENABLE)

#include "caselight.h"

CaseLight caselight;

#if CASE_LIGHT_IS_COLOR_LED
  #include "leds/leds.h"
#endif

#if CASELIGHT_USES_BRIGHTNESS && !defined(CASE_LIGHT_DEFAULT_BRIGHTNESS)
  #define CASE_LIGHT_DEFAULT_BRIGHTNESS 0 // For use on PWM pin as non-PWM just sets a default
#endif

#if CASELIGHT_USES_BRIGHTNESS
  uint8_t CaseLight::brightness = CASE_LIGHT_DEFAULT_BRIGHTNESS;
#endif

bool CaseLight::on = CASE_LIGHT_DEFAULT_ON;

#if CASE_LIGHT_IS_COLOR_LED
  LEDColor CaseLight::color =
    #ifdef CASE_LIGHT_DEFAULT_COLOR
      CASE_LIGHT_DEFAULT_COLOR
    #else
      { 255, 255, 255, 255 }
    #endif
  ;
#endif

#ifndef INVERT_CASE_LIGHT
  #define INVERT_CASE_LIGHT false
#endif

void CaseLight::update(const bool sflag) {
  #if CASELIGHT_USES_BRIGHTNESS
    /**
     * The brightness_sav (and sflag) is needed because ARM chips ignore
     * a "WRITE(CASE_LIGHT_PIN,x)" command to the pins that are directly
     * controlled by the PWM module. In order to turn them off the brightness
     * level needs to be set to OFF. Since we can't use the PWM register to
     * save the last brightness level we need a variable to save it.
     */
    static uint8_t brightness_sav;  // Save brightness info for restore on "M355 S1"

    if (on || !sflag)
      brightness_sav = brightness;  // Save brightness except for M355 S0
    if (sflag && on)
      brightness = brightness_sav;  // Restore last brightness for M355 S1

    const uint8_t i = on ? brightness : 0, n10ct = INVERT_CASE_LIGHT ? 255 - i : i;
  #endif

  #if CASE_LIGHT_IS_COLOR_LED

    leds.set_color(
      MakeLEDColor(color.r, color.g, color.b, color.w, n10ct),
      false
    );

  #else // !CASE_LIGHT_IS_COLOR_LED

    #if CASELIGHT_USES_BRIGHTNESS
      if (pin_is_pwm())
        analogWrite(pin_t(CASE_LIGHT_PIN), (
          #if CASE_LIGHT_MAX_PWM == 255
            n10ct
          #else
            map(n10ct, 0, 255, 0, CASE_LIGHT_MAX_PWM)
          #endif
        ));
      else
    #endif
      {
        const bool s = on ? !INVERT_CASE_LIGHT : INVERT_CASE_LIGHT;
        WRITE(CASE_LIGHT_PIN, s ? HIGH : LOW);
      }

  #endif // !CASE_LIGHT_IS_COLOR_LED

  #if ENABLED(CASE_LIGHT_USE_RGB_LED)
    if (leds.lights_on) leds.update(); else leds.set_off();
  #endif
}

#endif // CASE_LIGHT_ENABLE
