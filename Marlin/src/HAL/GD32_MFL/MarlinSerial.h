/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

#include <UsartSerial.hpp>

#include "../../core/serial_hook.h"

#define SERIAL_INDEX_MIN 0
#define SERIAL_INDEX_MAX 4

#include "../shared/serial_ports.h"

#if defined(LCD_SERIAL_PORT) && ANY(HAS_DGUS_LCD, EXTENSIBLE_UI)
  #define LCD_SERIAL_TX_BUFFER_FREE() LCD_SERIAL.availableForWrite()
#endif

using namespace arduino;

struct MarlinSerial : public UsartSerial {
  static MarlinSerial& get_instance(usart::USART_Base Base, pin_size_t rxPin = NO_PIN, pin_size_t txPin = NO_PIN);

  void begin(unsigned long baudrate, uint16_t config);
  inline void begin(unsigned long baudrate) { begin(baudrate, SERIAL_8N1); }
  void updateRxDmaBuffer();

  #if DISABLED(SERIAL_DMA)
    FORCE_INLINE static uint8_t buffer_overruns() { return 0; }
  #endif

  #if ENABLED(EMERGENCY_PARSER)
    EmergencyParser::State emergency_state;

    // Accessor method to get the last received byte
    uint8_t get_last_data() { return usart_.get_last_data(); }

    // Register the emergency callback
    void register_emergency_callback(void (*callback)());
  #endif

protected:
  using UsartSerial::UsartSerial;
};

typedef Serial1Class<MarlinSerial> MSerialT;
extern MSerialT MSerial0;
extern MSerialT MSerial1;
extern MSerialT MSerial2;
extern MSerialT MSerial3;
extern MSerialT MSerial4;
