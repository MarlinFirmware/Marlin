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

#include "../platforms.h"

#ifdef ARDUINO_ARCH_MFL

#include "../../inc/MarlinConfig.h"
#include "MarlinSerial.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

using namespace arduino;

MarlinSerial& MarlinSerial::get_instance(usart::USART_Base Base, pin_size_t rxPin, pin_size_t txPin) {
  UsartSerial& serial = UsartSerial::get_instance(Base, rxPin, txPin);
  return *reinterpret_cast<MarlinSerial*>(&serial);
}

#if USING_HW_SERIAL0
  MSerialT MSerial0(true, MarlinSerial::get_instance(usart::USART_Base::USART0_BASE, NO_PIN, NO_PIN));
#endif
#if USING_HW_SERIAL1
  MSerialT MSerial1(true, MarlinSerial::get_instance(usart::USART_Base::USART1_BASE, NO_PIN, NO_PIN));
#endif
#if USING_HW_SERIAL2
  MSerialT MSerial2(true, MarlinSerial::get_instance(usart::USART_Base::USART2_BASE, NO_PIN, NO_PIN));
#endif
#if USING_HW_SERIAL3
  MSerialT MSerial3(true, MarlinSerial::get_instance(usart::USART_Base::UART3_BASE, NO_PIN, NO_PIN));
#endif
#if USING_HW_SERIAL4
  MSerialT MSerial4(true, MarlinSerial::get_instance(usart::USART_Base::UART4_BASE, NO_PIN, NO_PIN));
#endif

#if ENABLED(EMERGENCY_PARSER)
  // This callback needs to access the specific MarlinSerial instance
  // We'll use a static pointer to track the current instance
  static MarlinSerial* current_serial_instance = nullptr;

  static void emergency_callback() {
    if (current_serial_instance) {
      uint8_t last_data = current_serial_instance->get_last_data();
      emergency_parser.update(current_serial_instance->emergency_state, last_data);
    }
  }

  void MarlinSerial::register_emergency_callback(void (*callback)()) {
    usart_.register_interrupt_callback(usart::Interrupt_Type::INTR_RBNEIE, callback);
  }
#endif

void MarlinSerial::begin(unsigned long baudrate, uint16_t config) {
  UsartSerial::begin(baudrate, config);
  #if DISABLED(SERIAL_DMA)
    #if ENABLED(EMERGENCY_PARSER)
      current_serial_instance = this;
      register_emergency_callback(emergency_callback);
    #endif
  #endif
}

void MarlinSerial::updateRxDmaBuffer() {
  #if ENABLED(EMERGENCY_PARSER)
    // Get the number of bytes available in the receive buffer
    size_t available_bytes = usart_.available_for_read(true);
    uint8_t data;

    // Process only the available data
    for (size_t i = 0; i < available_bytes; ++i) {
      if (usart_.read_rx_buffer(data)) {
        emergency_parser.update(emergency_state, data);
      }
    }
  #endif
  // Call the base class implementation to handle any additional updates
  UsartSerial::updateRxDmaBuffer();
}

#endif // ARDUINO_ARCH_MFL
