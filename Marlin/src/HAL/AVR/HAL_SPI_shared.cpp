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

#ifdef __AVR__

#include "../../inc/MarlinConfig.h"

#include "registers.h"

inline bool _ATmega_isPinSPI(int pin) {
  return _ATmega_getPinFunctions(pin).hasFunc(eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::SPI_CS);
}

void spiSetupChipSelect(int pin) {
  // If the pin is not hardware SPI, then we disable peripherals that may conflict with it due to badly written bootloaders
  // or ArduinoCore.
  if (_ATmega_isPinSPI(pin) == false) {
    _ATmega_savePinAlternate(pin);
  }
  _ATmega_pinMode(pin, OUTPUT);
  _ATmega_digitalWrite(pin, HIGH);
}

#endif //__AVR__
