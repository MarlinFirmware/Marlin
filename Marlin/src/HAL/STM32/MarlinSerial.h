/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

#include "../../core/serial_hook.h"

typedef void (*usart_rx_callback_t)(serial_t * obj);

struct MarlinSerial : public HardwareSerial {
  MarlinSerial(void* peripheral, usart_rx_callback_t rx_callback) :
      HardwareSerial(peripheral), _rx_callback(rx_callback)
  { }

  void begin(unsigned long baud, uint8_t config);
  inline void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }

  void _rx_complete_irq(serial_t* obj);

protected:
  usart_rx_callback_t _rx_callback;
};

typedef Serial0Type<MarlinSerial> MSerialT;
extern MSerialT MSerial1;
extern MSerialT MSerial2;
extern MSerialT MSerial3;
extern MSerialT MSerial4;
extern MSerialT MSerial5;
extern MSerialT MSerial6;
extern MSerialT MSerial7;
extern MSerialT MSerial8;
extern MSerialT MSerial9;
extern MSerialT MSerial10;
extern MSerialT MSerialLP1;
