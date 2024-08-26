/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../../HAL/platforms.h"

#ifdef HAL_STM32

#include "../../../inc/MarlinConfigPre.h"

#if ALL(HAS_TFT_LVGL_UI, MKS_WIFI_MODULE)

#include "tft_lvgl_configuration.h"

#include "draw_ui.h"
#include "wifiSerial.h"

WifiSerial WifiSerial1(USART1);

void WifiSerial::setRx(uint32_t _rx) { _serial.pin_rx = digitalPinToPinName(_rx); }
void WifiSerial::setTx(uint32_t _tx) { _serial.pin_tx = digitalPinToPinName(_tx); }
void WifiSerial::setRx(PinName _rx)  { _serial.pin_rx = _rx; }
void WifiSerial::setTx(PinName _tx)  { _serial.pin_tx = _tx; }

void WifiSerial::init(PinName _rx, PinName _tx) {
  _serial.pin_rx  = (_rx == _tx) ? NC : _rx;
  _serial.pin_tx  = _tx;
  _serial.rx_buff = wifiRxBuf;
  _serial.rx_head = 0;
  _serial.rx_tail = 0;
  _serial.tx_buff = wifiTxBuf;
  _serial.tx_head = 0;
  _serial.tx_tail = 0;
}

WifiSerial::WifiSerial(void *peripheral) {
  // If PIN_SERIALy_RX is not defined assume half-duplex
  _serial.pin_rx = NC;
  if (false) {
    // for else if / else below...
  }
  // If Serial is defined in variant set
  // the Rx/Tx pins for com port if defined
  #if defined(Serial) && defined(PIN_SERIAL_TX)
    else if ((void *)this == (void *)&Serial) {
      #ifdef PIN_SERIAL_RX
        setRx(PIN_SERIAL_RX);
      #endif
      setTx(PIN_SERIAL_TX);
    }
  #endif
  #if defined(PIN_SERIAL1_TX) && defined(USART1_BASE)
    else if (peripheral == USART1) {
      #ifdef PIN_SERIAL1_RX
        setRx(PIN_SERIAL1_RX);
      #endif
      setTx(PIN_SERIAL1_TX);
    }
  #endif
  #if defined(PIN_SERIAL2_TX) && defined(USART2_BASE)
    else if (peripheral == USART2) {
      #ifdef PIN_SERIAL2_RX
        setRx(PIN_SERIAL2_RX);
      #endif
      setTx(PIN_SERIAL2_TX);
    }
  #endif
  #if defined(PIN_SERIAL3_TX) && defined(USART3_BASE)
    else if (peripheral == USART3) {
      #ifdef PIN_SERIAL3_RX
        setRx(PIN_SERIAL3_RX);
      #endif
      setTx(PIN_SERIAL3_TX);
    }
  #endif
  #ifdef PIN_SERIAL4_TX
    else if (false
      #ifdef USART4_BASE
        || peripheral == USART4
      #elif defined(UART4_BASE)
        || peripheral == UART4
      #endif
    ) {
      #ifdef PIN_SERIAL4_RX
        setRx(PIN_SERIAL4_RX);
      #endif
      setTx(PIN_SERIAL4_TX);
    }
  #endif
  #ifdef PIN_SERIAL5_TX
    else if (false
      #ifdef USART5_BASE
        || peripheral == USART5
      #elif defined(UART5_BASE)
        || peripheral == UART5
      #endif
    ) {
      #ifdef PIN_SERIAL5_RX
        setRx(PIN_SERIAL5_RX);
      #endif
      setTx(PIN_SERIAL5_TX);
    }
  #endif
  #if defined(PIN_SERIAL6_TX) && defined(USART6_BASE)
    else if (peripheral == USART6) {
      #ifdef PIN_SERIAL6_RX
        setRx(PIN_SERIAL6_RX);
      #endif
        setTx(PIN_SERIAL6_TX);
    }
  #endif
  #ifdef PIN_SERIAL7_TX
    else if (false
      #ifdef USART7_BASE
        || peripheral == USART7
      #elif defined(UART7_BASE)
        || peripheral == UART7
      #endif
    ) {
      #ifdef PIN_SERIAL7_RX
        setRx(PIN_SERIAL7_RX);
      #endif
      setTx(PIN_SERIAL7_TX);
    }
  #endif
  #ifdef PIN_SERIAL8_TX
    else if (false
      #ifdef USART8_BASE
        || peripheral == USART8
      #elif defined(UART8_BASE)
        || peripheral == UART8
      #endif
    ) {
      #ifdef PIN_SERIAL8_RX
        setRx(PIN_SERIAL8_RX);
      #endif
      setTx(PIN_SERIAL8_TX);
    }
  #endif
  #if defined(PIN_SERIAL9_TX) && defined(UART9_BASE)
    else if (peripheral == UART9) {
      #ifdef PIN_SERIAL9_RX
        setRx(PIN_SERIAL9_RX);
      #endif
      setTx(PIN_SERIAL9_TX);
    }
  #endif
  #ifdef PIN_SERIAL10_TX
    else if (false
      #ifdef USART10_BASE
        || peripheral == USART10
      #elif defined(UART10_BASE)
        || peripheral == UART10
      #endif
    ) {
      #ifdef PIN_SERIAL10_RX
        setRx(PIN_SERIAL10_RX);
      #endif
      setTx(PIN_SERIAL10_TX);
    }
  #endif
  #if defined(PIN_SERIALLP1_TX) && defined(LPUART1_BASE)
    else if (peripheral == LPUART1) {
      #ifdef PIN_SERIALLP1_RX
        setRx(PIN_SERIALLP1_RX);
      #endif
      setTx(PIN_SERIALLP1_TX);
    }
  #endif
  // else get the pins of the first peripheral occurrence in PinMap
  else {
    _serial.pin_rx = pinmap_pin(peripheral, PinMap_UART_RX);
    _serial.pin_tx = pinmap_pin(peripheral, PinMap_UART_TX);
  }
  //if (halfDuplex == HALF_DUPLEX_ENABLED) _serial.pin_rx = NC;
  init(_serial.pin_rx, _serial.pin_tx);
}

void WifiSerial::flush() {
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!_written) return;

  while ((_serial.tx_head != _serial.tx_tail)) {
    // nop, the interrupt handler will free up space for us
  }
  // If we get here, nothing is queued anymore (DRIE is disabled) and
  // the hardware finished transmission (TXC is set).
}

bool WifiSerial::isHalfDuplex() const { return _serial.pin_rx == NC; }

void WifiSerial::enableHalfDuplexRx() {
  if (isHalfDuplex()) {
    // In half-duplex mode we have to wait for all TX characters to
    // be transmitted before we can receive data.
    flush();
    if (!_rx_enabled) {
      _rx_enabled = true;
      uart_enable_rx(&_serial);
    }
  }
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void WifiSerial::_rx_complete_irq(serial_t *obj) {
  // No Parity error, read byte and store it in the buffer if there is room
  unsigned char c;

  if (uart_getc(obj, &c) == 0) {

    WRITE(WIFI_IO1_PIN, HIGH);

    rx_buffer_index_t i = uint16_t(obj->rx_head + 1) % WIFI_RX_BUF_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != obj->rx_tail) {
      obj->rx_buff[obj->rx_head] = c;
      obj->rx_head = i;
    }
  }
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

int WifiSerial::_tx_complete_irq(serial_t *obj) {
  // If interrupts are enabled, there must be more data in the output
  // buffer. Send the next byte
  obj->tx_tail = (obj->tx_tail + 1) % WIFI_TX_BUF_SIZE;

  return (obj->tx_head == obj->tx_tail) ? -1 : 0;
}

void WifiSerial::begin(unsigned long baud) { begin(baud, SERIAL_8N1); }

void WifiSerial::begin(unsigned long baud, byte config) {
  uint32_t databits = 0, stopbits = 0, parity = 0;

  _baud = baud;
  _config = config;

  // Manage databits
  switch (config & 0x07) {
    case 0x02: databits = 6; break;
    case 0x04: databits = 7; break;
    case 0x06: databits = 8; break;
    default: databits = 0; break;
  }

  if ((config & 0x30) == 0x30) {
    parity = UART_PARITY_ODD;
    databits++;
  }
  else if ((config & 0x20) == 0x20) {
    parity = UART_PARITY_EVEN;
    databits++;
  }
  else
    parity = UART_PARITY_NONE;

  stopbits = ((config & 0x08) == 0x08) ? UART_STOPBITS_2 : UART_STOPBITS_1;

  switch (databits) {
    #ifdef UART_WORDLENGTH_7B
      case 7: databits = UART_WORDLENGTH_7B; break;
    #endif
    case 8: databits = UART_WORDLENGTH_8B; break;
    case 9: databits = UART_WORDLENGTH_9B; break;
    default:
    case 0: Error_Handler(); break;
  }

  uart_init(&_serial, uint32_t(baud), databits, parity, stopbits);
  enableHalfDuplexRx();
  if (baud == WIFI_BAUDRATE)
    uart_attach_rx_callback(&_serial, _rx_complete_irq);
  else
    USART1->CR1 |= USART_CR1_RE;  // Preserve word length, etc. Use 'or' to preserve USART_CR1_M_8N1
}

void WifiSerial::end() {
  // wait for transmission of outgoing data
  flush();

  uart_deinit(&_serial);

  // clear any received data
  _serial.rx_head = _serial.rx_tail;
}

int WifiSerial::available() {
  return uint16_t(WIFI_RX_BUF_SIZE + _serial.rx_head - _serial.rx_tail) % WIFI_RX_BUF_SIZE;
}

//
// I/O
//
int WifiSerial::read() {
  enableHalfDuplexRx();
  // if the head isn't ahead of the tail, we don't have any characters
  if (_serial.rx_head == _serial.rx_tail) return -1;

  uint8_t c = _serial.rx_buff[_serial.rx_tail];
  _serial.rx_tail = (rx_buffer_index_t)(_serial.rx_tail + 1) % WIFI_RX_BUF_SIZE;
  return c;
}

int WifiSerial::write(uint8_t c) {
  _written = true;
  if (isHalfDuplex()) {
    if (_rx_enabled) {
      _rx_enabled = false;
      uart_enable_tx(&_serial);
    }
  }

  tx_buffer_index_t i = (_serial.tx_head + 1) % WIFI_TX_BUF_SIZE;

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to empty it a bit
  while (i == _serial.tx_tail) {
    // nop, the interrupt handler will free up space for us
  }

  _serial.tx_buff[_serial.tx_head] = c;
  _serial.tx_head = i;

  if (!serial_tx_active(&_serial))
    uart_attach_tx_callback(&_serial, _tx_complete_irq);

  return 1;
}

#endif // HAS_TFT_LVGL_UI && MKS_WIFI_MODULE
#endif // HAL_STM32
