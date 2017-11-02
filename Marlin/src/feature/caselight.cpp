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

#include "../inc/MarlinConfig.h"

#if HAS_CASE_LIGHT

uint8_t case_light_brightness = CASE_LIGHT_DEFAULT_BRIGHTNESS;
bool case_light_on = CASE_LIGHT_DEFAULT_ON;

/**
 * The following are needed because ARM chips ignore a "WRITE(CASE_LIGHT_PIN,x)" command to the pins that
 * are directly controlled by the PWM module. In order to turn them off the brightness level needs to be
 * set to off.  Since we can't use the pwm register to save the last brightness level we need a variable
 * to save it.
 */
uint8_t case_light_brightness_sav;   // saves brighness info so can restore when "M355 S1" received
bool case_light_arg_flag;  // flag to notify if S or P arguement type

#ifndef INVERT_CASE_LIGHT
  #define INVERT_CASE_LIGHT false
#endif

void update_case_light() {
  SET_OUTPUT(CASE_LIGHT_PIN);

  if (!(case_light_arg_flag && !case_light_on))
    case_light_brightness_sav = case_light_brightness;  // save brightness except if this is an S0 arguement
  if (case_light_arg_flag && case_light_on)
    case_light_brightness = case_light_brightness_sav;  // restore last brightens if this is an S1 arguement

  if (case_light_on) {
    if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN))
      analogWrite(CASE_LIGHT_PIN, INVERT_CASE_LIGHT ? 255 - case_light_brightness : case_light_brightness);
    else
      WRITE(CASE_LIGHT_PIN, INVERT_CASE_LIGHT ? LOW : HIGH);
  }
  else {
    if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN))
      analogWrite(CASE_LIGHT_PIN, INVERT_CASE_LIGHT ? 255 : 0);
    else
      WRITE(CASE_LIGHT_PIN, INVERT_CASE_LIGHT ? HIGH : LOW);
  }
}

#endif // HAS_CASE_LIGHT
