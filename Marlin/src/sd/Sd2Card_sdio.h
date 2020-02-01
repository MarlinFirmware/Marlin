/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../inc/MarlinConfig.h"

#if ENABLED(SDIO_SUPPORT)

bool SDIO_Init();
bool SDIO_ReadBlock(uint32_t block, uint8_t *dst);
bool SDIO_WriteBlock(uint32_t block, const uint8_t *src);

extern uint8_t _status_init;
extern uint8_t _status_W;

class Sd2Card {
  public:
//    bool init(uint8_t sckRateID = 0, uint8_t chipSelectPin = 0) { return SDIO_Init(); }
    bool init(uint8_t sckRateID = 0, uint8_t chipSelectPin = 0) {
      bool stat = SDIO_Init();
      SERIAL_ECHOLNPAIR("HAL_INIT: ", _status_init,
                       "    WIDE: ", _status_W,
                       "    init: ", stat);
      return stat;
    }

//    bool readBlock(uint32_t block, uint8_t *dst) { return SDIO_ReadBlock(block, dst); }
    bool readBlock(uint32_t block, uint8_t *dst) {
     for (uint16_t x = 0; x < 512; x++)
       *(dst + x) = 0;

      bool stat = SDIO_ReadBlock(block, dst);
      SERIAL_ECHOLNPAIR("read: ", stat,
      "   block: ", block);

      uint16_t zero_count = 0;
      for (uint16_t x = 0; x < 512; x++)
        if (!(*(dst + x))) zero_count++;
      SERIAL_ECHOLNPAIR("read: ", stat,
      "   block: ", block,
       "   count: ", zero_count);


//     for (uint8_t j = 0; j < 16; j++) {
//       for (uint8_t i = 0; i < 32; i++) {
//         uint8_t nibble = *(dst + i + j*16) & 0x0F;
//         nibble += '0';
//         SERIAL_ECHO(nibble);
//         nibble = *(dst + i + j*16) & 0xF0;
//         nibble = nibble >> 4;
//         nibble += '0';
//         SERIAL_ECHO(nibble);
//         SERIAL_ECHOPGM(" ");
//       }
//       SERIAL_EOL();
//     }
      return stat;
    }

    bool writeBlock(uint32_t block, const uint8_t *src) { return SDIO_WriteBlock(block, src); }
};

#endif // SDIO_SUPPORT
