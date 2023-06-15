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

#include "../../inc/MarlinConfig.h"

#include "PinNames.h"
#include <stdint.h>
#include "HAL.h"

uint32_t pinmap_function(PinName pin, const PinMap *map);


typedef struct {
  uint32_t* timer;
  uint8_t channel;
  pin_t pin;
} PWM_chan_t;

#define MAX_PWM 32
PWM_chan_t used_PWM_channels[MAX_PWM] = {0};

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

#endif  //HAL_STM32
