/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * but WITHOUT ANY WARRANTY; without even the warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "../platforms.h"

#ifdef ARDUINO_ARCH_AT32

#include "../../inc/MarlinConfig.h"

#include <SPI.h>

// The AT32 Arduino core leaves the global SPI instance commented out, expecting
// the application to instantiate it. Marlin's hardware-SPI path (spiInit / spiRec /
// spiSend) references the global `SPI`, so define it here against SPI1.
SPIClass SPI(SPI_CLASS_1_SPI);

void spiBegin() {
  #if PIN_EXISTS(SD_SS)
    OUT_WRITE(SD_SS_PIN, HIGH);
  #endif
}

// ------------------------
// Hardware SPI
// ------------------------

/**
 * Initialize hardware SPI
 * Set SCK rate to appropriate speed for spiRate [0,6]
 */
void spiInit(uint8_t spiRate) {
  uint32_t clock;
  switch (spiRate) {
    case 0: clock =  8000000; break;  // ~8-10 MHz
    case 1: clock =  4000000; break;  // ~4-5 MHz
    case 2: clock =  2000000; break;  // ~2-2.5 MHz
    case 3: clock =  1000000; break;  // ~1-1.25 MHz
    case 4: clock =   500000; break;  // ~500-625 kHz
    case 5: clock =   250000; break;  // ~250-312 kHz
    case 6: clock =   125000; break;  // ~125-156 kHz
    default: clock =  8000000; break;
  }
  SPI.setClock(clock);
  SPI.begin();
}

/** SPI receive a byte */
uint8_t spiRec() {
  return SPI.transfer(0xFF);
}

/** SPI send a byte */
void spiSend(uint8_t data) {
  SPI.transfer(data);
}

/** SPI read data into buffer */
void spiRead(uint8_t *buf, uint16_t nbyte) {
  if (nbyte == 0) return;
  SPI.write(buf, nbyte);
}

/** SPI send token and data block */
void spiSendBlock(uint8_t token, const uint8_t *buf) {
  SPI.transfer(token);
  if (buf) {
    for (uint16_t i = 0; i < 512; i++) {
      SPI.transfer(buf[i]);
    }
  }
  else {
    for (uint16_t i = 0; i < 512; i++) {
      SPI.transfer(0xFF);
    }
  }
}

#endif // __HAL_AT32__
