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
#pragma once
#include "HardwareSerial.h"
#include "../../core/serial_hook.h"

// optionally set uart IRQ priority to reduce overflow errors
// #define UART_IRQ_PRIO 1

struct MarlinSerial : public HardwareSerial
{
  MarlinSerial(struct usart_dev *usart_device, uint8 tx_pin, uint8 rx_pin) : HardwareSerial(usart_device, tx_pin, rx_pin) {}

#ifdef UART_IRQ_PRIO
  void setPriority()
  {
    NVIC_SetPriority(c_dev()->RX_IRQ, UART_IRQ_PRIO);
    NVIC_SetPriority(c_dev()->TX_IRQ, UART_IRQ_PRIO);
  }

  void begin(uint32 baud)
  {
    HardwareSerial::begin(baud);
    setPriority();
  }

  void begin(uint32 baud, uint8_t config)
  {
    HardwareSerial::begin(baud, config);
    setPriority();
  }
#endif
};

typedef Serial1Class<MarlinSerial> MSerialT;

extern MSerialT MSerial1;
extern MSerialT MSerial2;
