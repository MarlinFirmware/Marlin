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
 * speed_dial.cpp - speed dial feature
 */

#include "../MarlinCore.h"
#include "../HAL/shared/Delay.h"
#include "../lcd/marlinui.h"
#include "../gcode/gcode.h"
#include "../module/stepper.h"
#include "speed_dial.h"

#if ENABLED(SPEED_DIAL_FEATURE)

raw_adc_t SpeedDial::current_speed_dial_raw   = HAL_ADC_RANGE;
uint8_t   SpeedDial::current_speed_dial       = 100;
uint8_t   SpeedDial::next_value_change        = SPEED_DIAL_DIAL_SENSITIVITY;

SpeedDial speedDial;

void SpeedDial::set(raw_adc_t value) {
  current_speed_dial_raw = value;

  #define SPEED_DIAL_MULTIPLIER           (SQRT(0x7FFFFFFF / SPEED_DIAL_BOARD_RESISTOR) / 100)
  #define SPEED_DIAL_MULTIPLIER_FINAL     (SPEED_DIAL_MULTIPLIER * 100)
  #define SPEED_DIAL_SMOOTH_VALUE_TOTAL   (SPEED_DIAL_SMOOTH_VALUE + 1)

  //get adc percentage
  int32_t speed_dial_adc_percent = current_speed_dial_raw * SPEED_DIAL_MULTIPLIER_FINAL / HAL_ADC_RANGE;
  if(speed_dial_adc_percent > SPEED_DIAL_MULTIPLIER_FINAL) speed_dial_adc_percent = SPEED_DIAL_MULTIPLIER_FINAL;
  if(speed_dial_adc_percent < 0) speed_dial_adc_percent = 0;

  #if SPEED_DIAL_INVERT
    speed_dial_adc_percent = SPEED_DIAL_MULTIPLIER_FINAL - speed_dial_adc_percent;
  #endif

  //dial position percentage
  int32_t speed_dial_percent = (speed_dial_adc_percent > 0 ? 
                                ((SPEED_DIAL_BOARD_RESISTOR * SPEED_DIAL_MULTIPLIER_FINAL / speed_dial_adc_percent) - SPEED_DIAL_BOARD_RESISTOR) * SPEED_DIAL_MULTIPLIER_FINAL / SPEED_DIAL_POT_RESISTOR :
                                SPEED_DIAL_MULTIPLIER_FINAL);
  if(speed_dial_percent > SPEED_DIAL_MULTIPLIER_FINAL) speed_dial_percent = SPEED_DIAL_MULTIPLIER_FINAL;
  if(speed_dial_percent < 0) speed_dial_percent = 0;

  #if SPEED_DIAL_INVERT
    speed_dial_percent = SPEED_DIAL_MULTIPLIER_FINAL - speed_dial_percent;
  #endif

  //get actual speed value
  int32_t speed_dial_value = (speed_dial_percent + (SPEED_DIAL_MULTIPLIER / 2)) * (SPEED_DIAL_MAX_SPEED - SPEED_DIAL_MIN_SPEED) / SPEED_DIAL_MULTIPLIER_FINAL + SPEED_DIAL_MIN_SPEED;

  //clamp speed value
  if(speed_dial_value > SPEED_DIAL_MAX_SPEED) speed_dial_value = SPEED_DIAL_MAX_SPEED;
  if(speed_dial_value < SPEED_DIAL_MIN_SPEED) speed_dial_value = SPEED_DIAL_MIN_SPEED;

  //store derived value
  current_speed_dial = speed_dial_value;

  //update stepper speed as per dial sensitivity
  if(SPEED_DIAL_DIAL_SENSITIVITY > 0) {
    for(uint8_t i=0; i < SPEED_DIAL_DIAL_SENSITIVITY; i++) {
      updateStepper();
    }
  } else {
    if(next_value_change) {
      next_value_change--;
    } else {
      updateStepper();

      next_value_change = SPEED_DIAL_DIAL_SENSITIVITY;
    }
  }
}

void SpeedDial::updateStepper() {
  if(current_speed_dial > stepper.current_speed_dial()) stepper.set_speed_dial(stepper.current_speed_dial() + 1);
  if(current_speed_dial < stepper.current_speed_dial()) stepper.set_speed_dial(stepper.current_speed_dial() - 1);
}

#endif
