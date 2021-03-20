/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * blinkm.cpp - Control a BlinkM over i2c
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(BLINKM)

#include "blinkm.h"
#include "leds.h"
#include <Wire.h>

void blinkm_set_led_color(const LEDColor &color) {
  Wire.begin();
  Wire.beginTransmission(I2C_ADDRESS(0x09));
  Wire.write('o');                    //to disable ongoing script, only needs to be used once
  Wire.write('n');
  Wire.write(color.r);
  Wire.write(color.g);
  Wire.write(color.b);
  Wire.endTransmission();
}

#endif // BLINKM
