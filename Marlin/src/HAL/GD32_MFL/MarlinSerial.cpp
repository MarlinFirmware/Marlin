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

auto MarlinSerial::get_instance(usart::USART_Base Base, pin_size_t rxPin, pin_size_t txPin) -> MarlinSerial& {
  auto& serial = UsartSerial::get_instance(Base, rxPin, txPin);
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

/**
 * The MFL core only provides an IRQ handler for its DEFAULT_HARDWARE_SERIAL port.
 * Any other USART would fall through to the weak default handler, which hangs
 * forever on the first RX/TX interrupt. Provide handlers for the remaining ports.
 */
#define _USART_IRQ_HANDLER(U) \
  extern "C" void U##_IRQHandler() { \
    NVIC_ClearPendingIRQ(U##_IRQn); \
    UsartSerial::get_instance(usart::USART_Base::U##_BASE).handleInterrupt(); \
  }

#if USING_HW_SERIAL0 && !defined(USE_HARDWARE_SERIAL0)
  _USART_IRQ_HANDLER(USART0)
#endif
#if USING_HW_SERIAL1 && !defined(USE_HARDWARE_SERIAL1)
  _USART_IRQ_HANDLER(USART1)
#endif
#if USING_HW_SERIAL2 && !defined(USE_HARDWARE_SERIAL2)
  _USART_IRQ_HANDLER(USART2)
#endif
#if USING_HW_SERIAL3 && !defined(USE_HARDWARE_SERIAL3)
  _USART_IRQ_HANDLER(UART3)
#endif
#if USING_HW_SERIAL4 && !defined(USE_HARDWARE_SERIAL4)
  _USART_IRQ_HANDLER(UART4)
#endif

#if ENABLED(EMERGENCY_PARSER)

  #if DISABLED(SERIAL_DMA)
    // The RX callback has no context, so keep one instance per USART
    static MarlinSerial* eparser_serial[5] = { nullptr };

    template<uint8_t N>
    static void emergency_callback() {
      MarlinSerial * const s = eparser_serial[N];
      if (s) emergency_parser.update(s->emergency_state, s->get_last_data());
    }

    static void (* const eparser_callback[5])() = {
      emergency_callback<0>, emergency_callback<1>, emergency_callback<2>, emergency_callback<3>, emergency_callback<4>
    };
  #endif

  void MarlinSerial::register_emergency_callback(void (*callback)()) {
    usart_.register_interrupt_callback(usart::Interrupt_Type::INTR_RBNEIE, callback);
  }

#endif // EMERGENCY_PARSER

void MarlinSerial::begin(unsigned long baudrate, uint16_t config) {
  UsartSerial::begin(baudrate, config, ENABLED(SERIAL_DMA));
  #if ENABLED(EMERGENCY_PARSER)
    emergency_state = EmergencyParser::State::EP_RESET;
    #if ENABLED(SERIAL_DMA)
      rx_dma_pos = 0;
    #else
      const uint8_t index = static_cast<uint8_t>(base_);
      if (index < COUNT(eparser_serial)) {
        eparser_serial[index] = this;
        register_emergency_callback(eparser_callback[index]);
      }
    #endif
  #endif
}

/**
 * Called by UsartSerial::available/peek/read to sync the DMA RX buffer.
 * Bytes must remain in the buffer for the G-code reader, so the emergency
 * parser only scans (never consumes) the newly-received DMA data.
 */
void MarlinSerial::updateRxDmaBuffer() {
  UsartSerial::updateRxDmaBuffer();
  #if ALL(EMERGENCY_PARSER, SERIAL_DMA)
    const uint8_t * const buf = usart_.get_buffer_data(true);
    const size_t head = (USART_RX_BUFFER_SIZE - dma_.get_transfer_count()) % (USART_RX_BUFFER_SIZE);
    for (; rx_dma_pos != head; rx_dma_pos = (rx_dma_pos + 1) % (USART_RX_BUFFER_SIZE))
      emergency_parser.update(emergency_state, buf[rx_dma_pos]);
  #endif
}

#endif // ARDUINO_ARCH_MFL
