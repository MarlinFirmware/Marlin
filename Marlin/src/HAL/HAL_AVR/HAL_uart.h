/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2018 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * UART support
  */

#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#if ENABLED(USE_UART)

#include "../../inc/MarlinConfigPre.h"
#include <Arduino.h>


class MarlinUART {

  public:
    MarlinUART(HardwareSerial *serial) {
      this->serial = serial;
    }

    void begin(unsigned long baud) { serial->begin(baud); }
    void end() { serial->end(); };
    int available(void) { return serial->available(void);};
    int peek(void) { return serial->peek(void); };
    int read(void) { return serial->read(void); };
    int availableForWrite(void) { return serial->availableFroWrite(void); };
    void flush(void) { serial->flush(void); };
    size_t write(uint8_t v) { return serial->write(v); };
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }


  private:
    HardwareSerial * serial;
}

#if USE_UART == 1
    MarlinUART(&Serial1) marlinUART;
#endif

#if USE_UART == 2
    MarlinUART(&Serial2) marlinUART;
#endif

#if USE_UART == 3
    MarlinUART(&Serial3) marlinUART;
#endif



#endif // ENABLED(USE_UART)

#endif // _HAL_UART_H_
