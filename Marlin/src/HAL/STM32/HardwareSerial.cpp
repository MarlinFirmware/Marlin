/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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

// IRON, HARDWARESERIAL2 CLASS, ADAPTATION FROM ARDUINO'S HARDWARESERIAL

#include "../../inc/MarlinConfig.h"

#if defined(HAL_UART_MODULE_ENABLED) && !defined(HAL_UART_MODULE_ONLY)

#include <stdio.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "uart.h"

#define PIN_SERIAL1_TX          PA9
#define PIN_SERIAL1_RX          PA10
#define PIN_SERIAL2_TX          2 // PIN_A2
#define PIN_SERIAL2_RX          3 // PIN_A3
#define PIN_SERIAL3_TX          26// PIN_PB10
#define PIN_SERIAL3_RX          27// PIN_PB11
#define PIN_SERIAL4_TX          42// PIN_PC10
#define PIN_SERIAL4_RX          43// PIN_PC11
#define PIN_SERIAL5_TX          44// PIN_PC12
#define PIN_SERIAL5_RX          50// PIN_PD2

// GET FROM INCLUDE FILE!!!
#define RCC_AHB1Periph_DMA1 ((uint32_t)0x00200000)
#define RCC_AHB1Periph_DMA2 ((uint32_t)0x00400000)

void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState) {
  // Check the parameters
  assert_param(IS_RCC_AHB1_CLOCK_PERIPH(RCC_AHB1Periph));

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
    RCC->AHB1ENR |= RCC_AHB1Periph;
  else
    RCC->AHB1ENR &= ~RCC_AHB1Periph;
}

// ------------------------------------------------------------------------

// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
#ifdef USING_HW_SERIAL1
  HAL_HardwareSerial HSerial1(USART1);
  void serialEvent1() __attribute__((weak));
#endif

#ifdef USING_HW_SERIAL2
  HAL_HardwareSerial HSerial2(USART2);
  void serialEvent2() __attribute__((weak));
#endif

#ifdef USING_HW_SERIAL3
  HAL_HardwareSerial Serial3(USART3);
  void serialEvent3() __attribute__((weak));
#endif

#ifdef USING_HW_SERIAL4
  #ifdef USART4
    HAL_HardwareSerial HSerial4(USART4);
  #else
    HAL_HardwareSerial HSerial4(UART4);
  #endif
  void serialEvent4() __attribute__((weak));
#endif

#ifdef USING_HW_SERIAL5
  #ifdef USART5
    HAL_HardwareSerial HSerial5(USART5);
  #else
    HAL_HardwareSerial HSerial5(UART5);
  #endif
  void serialEvent5() __attribute__((weak));
#endif

// Constructors ////////////////////////////////////////////////////////////////

HAL_HardwareSerial::HAL_HardwareSerial(void *peripheral) {
  if (peripheral == USART1) {
    setRx(PIN_SERIAL1_RX);
    setTx(PIN_SERIAL1_TX);
    RX_DMA = { USART1, RCC_AHB1Periph_DMA2, 4, DMA2_Stream2 };
    _uart_index = 0;
  }
  else if (peripheral == USART2) {
    setRx(PIN_SERIAL2_RX);
    setTx(PIN_SERIAL2_TX);
    RX_DMA = { USART2, RCC_AHB1Periph_DMA1, 4, DMA1_Stream5 };
    _uart_index = 1;
  }
  else if (peripheral == USART3) {
    setRx(PIN_SERIAL3_RX);
    setTx(PIN_SERIAL3_TX);
    RX_DMA = { USART3, RCC_AHB1Periph_DMA1, 4, DMA1_Stream1 };
    _uart_index = 2;
  }

  #ifdef USART4
    else if (peripheral == USART4) {
      RX_DMA = { USART4, RCC_AHB1Periph_DMA1, 4, DMA1_Stream2 };
      setRx(PIN_SERIAL4_RX);
      setTx(PIN_SERIAL4_TX);
      _uart_index = 3;
    }
  #endif

  #ifdef UART4
    else if (peripheral == UART4) {
      RX_DMA = { UART4, RCC_AHB1Periph_DMA1, 4, DMA1_Stream2 };
      setRx(PIN_SERIAL4_RX);
      setTx(PIN_SERIAL4_TX);
      _uart_index = 3;
    }
  #endif

  else { // else get the pins of the first peripheral occurence in PinMap
    _serial.pin_rx = pinmap_pin(peripheral, PinMap_UART_RX);
    _serial.pin_tx = pinmap_pin(peripheral, PinMap_UART_TX);
  }

  init(_serial.pin_rx, _serial.pin_tx);
}

void HAL_HardwareSerial::setRx(uint32_t _rx) {
  _serial.pin_rx = digitalPinToPinName(_rx);
}

void HAL_HardwareSerial::setTx(uint32_t _tx) {
  _serial.pin_tx = digitalPinToPinName(_tx);
}

void HAL_HardwareSerial::init(PinName _rx, PinName _tx) {
  _serial.pin_rx  = _rx;
  _serial.rx_buff = _rx_buffer;
  _serial.rx_head = _serial.rx_tail = 0;

  _serial.pin_tx  = _tx;
  _serial.tx_buff = _tx_buffer;
  _serial.tx_head = _serial.tx_tail = 0;
}

/**
 * @brief  Read receive byte from uart
 * @param  obj : pointer to serial_t structure
 * @retval last character received
 */

// Actual interrupt handlers //////////////////////////////////////////////////////////////

int HAL_HardwareSerial::_tx_complete_irq(serial_t *obj) {
  // If interrupts are enabled, there must be more data in the output buffer. Send the next byte
  obj->tx_tail = (obj->tx_tail + 1) % TX_BUFFER_SIZE;

  if (obj->tx_head == obj->tx_tail) return -1;

  return 0;
}

// Public Methods //////////////////////////////////////////////////////////////

void HAL_HardwareSerial::begin(unsigned long baud, uint8_t config) {
  uint32_t databits = 0, stopbits = 0, parity = 0;

  _baud   = baud;
  _config = config;

  // Manage databits
  switch (config & 0x07) {
    case 0x02: databits = 6; break;
    case 0x04: databits = 7; break;
    case 0x06: databits = 8; break;
    default:   databits = 0; break;
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

  stopbits = (config & 0x08) == 0x08 ? UART_STOPBITS_2 : UART_STOPBITS_1;

  switch (databits) {
    #ifdef UART_WORDLENGTH_7B
      case 7: databits = UART_WORDLENGTH_7B; break;
    #endif
    case 8: databits = UART_WORDLENGTH_8B; break;
    case 9: databits = UART_WORDLENGTH_9B; break;
    default:
    case 0: Error_Handler(); break;
  }

  uart_init(&_serial, (uint32_t)baud, databits, parity, stopbits);

  // IRON, USING FREE RUNNING DMA READING, NO CALLBACK NEEDED
  //uart_attach_rx_callback(&_serial, _rx_complete_irq); // IRON, start the interrupt reading, indicate where to callback

  Serial_DMA_Read_Enable(); // IRON, START THE DMA READING PROCESS
}

void HAL_HardwareSerial::end() {
  flush();                              // Wait for transmission of outgoing data

  uart_deinit(&_serial);

  _serial.rx_head = _serial.rx_tail;    // Clear any received data
}

void HAL_HardwareSerial::update_rx_head() { // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS

  #if ENABLED(EMERGENCY_PARSER)
    static uint32_t flag = 0;
    while (flag != _serial.rx_head) { // send all available data to emergency parser immediately
      emergency_parser.update(static_cast<MSerialT*>(this)->emergency_state, _serial.rx_buff[flag++]);
      flag = flag % RX_BUFFER_SIZE;
    }
  #endif

  _serial.rx_head = RX_BUFFER_SIZE - RX_DMA.dma_streamRX->NDTR; // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
}

int HAL_HardwareSerial::available() {
  update_rx_head();  // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
  return ((unsigned int)(RX_BUFFER_SIZE + _serial.rx_head - _serial.rx_tail)) % RX_BUFFER_SIZE;
}

int HAL_HardwareSerial::peek() {
  update_rx_head();  // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
  if (_serial.rx_head == _serial.rx_tail) return -1;
  return _serial.rx_buff[_serial.rx_tail];
}

int HAL_HardwareSerial::read() {
  update_rx_head();                                   // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
  if (_serial.rx_head == _serial.rx_tail) return -1;  // No chars if the head isn't ahead of the tail

  unsigned char c = _serial.rx_buff[_serial.rx_tail];
  _serial.rx_tail = (rx_buffer_index_t)(_serial.rx_tail + 1) % RX_BUFFER_SIZE;
  return c;
}

size_t HAL_HardwareSerial::write(uint8_t c) {             // Interrupt based writing
  tx_buffer_index_t i = (_serial.tx_head + 1) % TX_BUFFER_SIZE;

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to empty it a bit
  while (i == _serial.tx_tail) { /* nada */ }             // NOP, let the interrupt free up space for us

  _serial.tx_buff[_serial.tx_head] = c;
  _serial.tx_head = i;

  if (!serial_tx_active(&_serial))
    uart_attach_tx_callback(&_serial, _tx_complete_irq);  // Write next byte, launch interrupt

  return 1;
}

void HAL_HardwareSerial::Serial_DMA_Read_Enable() {
  RCC_AHB1PeriphClockCmd(RX_DMA.dma_rcc, ENABLE);                   // Enable DMA clock

  RX_DMA.dma_streamRX->PAR  = (uint32_t)(&RX_DMA.uart->DR);         // RX peripheral address (usart)
  RX_DMA.dma_streamRX->M0AR = (uint32_t)_serial.rx_buff;            // RX destination address (memory)
  RX_DMA.dma_streamRX->NDTR = RX_BUFFER_SIZE;                       // RX buffer size

  RX_DMA.dma_streamRX->CR = (RX_DMA.dma_channel << 25);             // RX channel selection, set to 0 all the other CR bits

  // Primary serial port priority at highest level (TX higher than RX)
  RX_DMA.dma_streamRX->CR |= (3 << 16);                             // RX priority level: Very High

  //RX_DMA.dma_streamRX->CR &= ~(3 << 13);                          // RX memory data size: 8 bit
  //RX_DMA.dma_streamRX->CR &= ~(3 << 11);                          // RX peripheral data size: 8 bit
  RX_DMA.dma_streamRX->CR |= (1 << 10);                             // RX memory increment mode
  //RX_DMA.dma_streamRX->CR &= ~(1 << 9);                           // RX peripheral no increment mode
  RX_DMA.dma_streamRX->CR |= (1 << 8);                              // RX circular mode enabled
  //RX_DMA.dma_streamRX->CR &= ~(1 << 6);                           // RX data transfer direction: Peripheral-to-memory
  RX_DMA.uart->CR3        |= (1 << 6);                              // Enable DMA receiver (DMAR)
  RX_DMA.dma_streamRX->CR |= (1 << 0);                              // RX enable DMA
}

#endif // HAL_UART_MODULE_ENABLED && !HAL_UART_MODULE_ONLY
