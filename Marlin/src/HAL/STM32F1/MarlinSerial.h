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

#include <HardwareSerial.h>
#include <libmaple/usart.h>
#include <WString.h>

#include "../../inc/MarlinConfigPre.h"
#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

// Increase priority of serial interrupts, to reduce overflow errors
#define UART_IRQ_PRIO 1

class MarlinSerial : public HardwareSerial {
public:
  #if ENABLED(EMERGENCY_PARSER)
    const bool ep_enabled;
    EmergencyParser::State emergency_state;
    inline bool emergency_parser_enabled() { return ep_enabled; }
  #endif

  MarlinSerial(struct usart_dev *usart_device, uint8 tx_pin, uint8 rx_pin, bool TERN_(EMERGENCY_PARSER, ep_capable)) :
    HardwareSerial(usart_device, tx_pin, rx_pin)
    #if ENABLED(EMERGENCY_PARSER)
      , ep_enabled(ep_capable)
      , emergency_state(EmergencyParser::State::EP_RESET)
    #endif
    { }

  #ifdef UART_IRQ_PRIO
    // Shadow the parent methods to set IRQ priority after begin()
    void begin(uint32 baud) {
      MarlinSerial::begin(baud, SERIAL_8N1);
    }

    void begin(uint32 baud, uint8_t config) {
      HardwareSerial::begin(baud, config);
      nvic_irq_set_priority(c_dev()->irq_num, UART_IRQ_PRIO);
    }
  #endif
};

extern MarlinSerial MSerial1;
extern MarlinSerial MSerial2;
extern MarlinSerial MSerial3;
#if EITHER(STM32_HIGH_DENSITY, STM32_XL_DENSITY)
  extern MarlinSerial MSerial4;
  extern MarlinSerial MSerial5;
#endif
