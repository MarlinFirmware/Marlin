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

#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include "softspi.h"

template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin>
class SPI {
  static SoftSPI<MisoPin, MosiPin, SckPin> softSPI;
  public:
    FORCE_INLINE static void init() { softSPI.begin(); }
    FORCE_INLINE static void send(uint8_t data) { softSPI.send(data); }
    FORCE_INLINE static uint8_t receive() { return softSPI.receive(); }
};


// Hardware SPI
template<>
class SPI<MISO_PIN, MOSI_PIN, SCK_PIN> {
  public:
    FORCE_INLINE static void init() {
        OUT_WRITE(SCK_PIN, LOW);
        OUT_WRITE(MOSI_PIN, HIGH);
        SET_INPUT(MISO_PIN);
        WRITE(MISO_PIN, HIGH);
    }
    FORCE_INLINE static uint8_t receive() {
      SPDR = 0;
      for (;!TEST(SPSR, SPIF););
      return SPDR;
    }

};

#endif // __SPI_H__
