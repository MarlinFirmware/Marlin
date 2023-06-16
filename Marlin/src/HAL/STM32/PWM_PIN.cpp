/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Some CPUs can map the same PWM channel onto multiple physical pins.
 * If that happens then the pins behave as one.
 *
 * This can be avoided via manual methods.  This file is a way to
 * automatically keep this from happening.
 *
 * The array "used_PWM_channels" is used to keep track of what channels
 * are in use.  If a request for a PWM channel comes through then the
 * array is scanned for the channel associated with that pin.  If the
 * channel isn't in the array then the channel info and the pin number
 * is saved and TRUE is returned.  If the channel is in the array but
 * assigned to a different pin then FALSE is returned. If channel is in
 * the array and assigned to the pin then TRUE is returned. FALSE is
 * returned if there isn't a PWM channel associated with the pin.
 *
 */

#include "../platforms.h"

#ifdef HAL_STM32
#if defined(STM32G0B1xx) || defined(STM32H7xx) || defined(STM32F7xx)

#include "../../inc/MarlinConfig.h"

#include "PinNames.h"
#include <stdint.h>
#include "HAL.h"
#include "../../core/serial.h"

uint32_t pinmap_function(PinName pin, const PinMap *map);


typedef struct {
  uint32_t* timer;
  uint8_t channel;
  pin_t pin;
} PWM_chan_t;

#define MAX_PWM 32
PWM_chan_t used_PWM_channels[MAX_PWM] = {0};



void PIN_NUM_TO_ASCII(pin_t pin, char *array) {
  array[0] = 'P';
  array[1] = 'A' + pin/16;
  uint8_t temp = pin % 16;
  if (temp > 9) {
    array[2] = '1';
    array[3] = '0' + temp - 10;
    array[4] = '\0';
  }
  else {
    array[2] = '0' + temp;
    array[3] = '\0';
  }
}

void TIM_TO_ASCII(uint32_t* tim, uint8_t chan, char *array)  {
  uint8_t index;
  uint32_t timer = (uint32_t)tim;
  array[0] = 'T';
  array[1] = 'I';
  array[2] = 'M';
  switch (timer) {
    #ifdef TIM1_BASE
      case TIM1_BASE  : array[3] = '1' ; index = 4; break;
    #endif
    #ifdef TIM2_BASE
      case TIM2_BASE  : array[3] = '2' ; index = 4; break;
    #endif
    #ifdef TIM3_BASE
      case TIM3_BASE  : array[3] = '3' ; index = 4; break;
    #endif
    #ifdef TIM4_BASE
      case TIM4_BASE  : array[3] = '4' ; index = 4; break;
    #endif
    #ifdef TIM5_BASE
      case TIM5_BASE  : array[3] = '5' ; index = 4; break;
    #endif
    #ifdef TIM6_BASE
      case TIM6_BASE  : array[3] = '6' ; index = 4; break;
    #endif
    #ifdef TIM7_BASE
      case TIM7_BASE  : array[3] = '7' ; index = 4; break;
    #endif
    #ifdef TIM8_BASE
      case TIM8_BASE  : array[3] = '8' ; index = 4; break;
    #endif
    #ifdef TIM9_BASE
      case TIM9_BASE  : array[3] = '9' ; index = 4; break;
    #endif
    #ifdef TIM10_BASE
      case TIM10_BASE : array[3] = '1' ; array[3] = '0' ; index = 5; break;
    #endif
    #ifdef TIM11_BASE
      case TIM11_BASE : array[3] = '1' ; array[3] = '1' ; index = 5; break;
    #endif
    #ifdef TIM12_BASE
      case TIM12_BASE : array[3] = '1' ; array[3] = '2' ; index = 5; break;
    #endif
    #ifdef TIM13_BASE
      case TIM13_BASE : array[3] = '1' ; array[3] = '3' ; index = 5; break;
    #endif
    #ifdef TIM14_BASE
      case TIM14_BASE : array[3] = '1' ; array[3] = '4' ; index = 5; break;
    #endif
    #ifdef TIM15_BASE
      case TIM15_BASE : array[3] = '1' ; array[3] = '5' ; index = 5; break;
    #endif
    #ifdef TIM16_BASE
      case TIM16_BASE : array[3] = '1' ; array[3] = '6' ; index = 5; break;
    #endif
    #ifdef TIM17_BASE
      case TIM17_BASE : array[3] = '1' ; array[3] = '7' ; index = 5; break;
    #endif
  }
  array[index] = '_';
  array[index+1] = chan + '0';
  array[index+2] = '\0';
}

// return true if a PWM channel is available for the pin

uint8_t digitalPinHasAvailablPWM(pin_t pin) {
  PinName pin_name = digitalPinToPinName(pin);
  if (digitalPinHasPWM(pin_name)) {
    uint32_t* timer = (uint32_t*)pinmap_peripheral(pin_name, PinMap_PWM);
    uint8_t channel = STM_PIN_CHANNEL(pinmap_function(pin_name, PinMap_PWM));
    uint8_t i;
    for (i = 0; i < MAX_PWM; i++) {  // scan used PWM table
      if ((timer == used_PWM_channels[i].timer) && (channel == used_PWM_channels[i].channel))
        if (used_PWM_channels[i].pin == pin) {
          return true;  //  this pin is already set up
        }
        else {
          char ascii_array[6];
          SERIAL_ECHOLNPGM("WARNING: Attempt to assign one PWM channel to two pins.");
          PIN_NUM_TO_ASCII(used_PWM_channels[i].pin, ascii_array);
          SERIAL_ECHOPGM("pins: ", ascii_array);
          PIN_NUM_TO_ASCII(pin, ascii_array);
          SERIAL_ECHOLNPGM(" & ", ascii_array);
          TIM_TO_ASCII(timer, channel, ascii_array);
          SERIAL_ECHOLNPGM("Timer & channel: ", ascii_array);
          return false;   // PWM already in use so this pin can't have it
        }
    }
    for (i = 0; (i < MAX_PWM) && (used_PWM_channels[i].timer != 0) ; i++) {}  //find first open slot
    if (i < MAX_PWM) {
      used_PWM_channels[i].timer = timer;                                     // save new pin's info
      used_PWM_channels[i].channel = channel;
      used_PWM_channels[i].pin = pin;
      return true;
    }
}
  return false;
}

#endif  //STM32G0B1xx, STM32H7xx, STM32F7xx
#endif  //HAL_STM32
