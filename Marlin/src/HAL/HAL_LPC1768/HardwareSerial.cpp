/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"
#include "HardwareSerial.h"

#if SERIAL_PORT == 0 || SERIAL_PORT_2 == 0
  HardwareSerial Serial = HardwareSerial(LPC_UART0);
#elif SERIAL_PORT == 1 || SERIAL_PORT_2 == 1
  HardwareSerial Serial1 = HardwareSerial((LPC_UART_TypeDef *) LPC_UART1);
#elif SERIAL_PORT == 2 || SERIAL_PORT_2 == 2
  HardwareSerial Serial2 = HardwareSerial(LPC_UART2);
#elif SERIAL_PORT == 3 || SERIAL_PORT_2 == 3
  HardwareSerial Serial3 = HardwareSerial(LPC_UART3);
#endif

void HardwareSerial::begin(uint32_t baudrate) {

  UART_CFG_Type UARTConfigStruct;
  PINSEL_CFG_Type PinCfg;
  UART_FIFO_CFG_Type FIFOConfig;

  if (Baudrate == baudrate) return; // No need to re-initialize

  if (UARTx == LPC_UART0) {
    // Initialize UART0 pin connect
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 2;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 3;
    PINSEL_ConfigPin(&PinCfg);
  } else if ((LPC_UART1_TypeDef *) UARTx == LPC_UART1) {
    // Initialize UART1 pin connect
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 15;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 16;
    PINSEL_ConfigPin(&PinCfg);
  } else if (UARTx == LPC_UART2) {
    // Initialize UART2 pin connect
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 10;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 11;
    PINSEL_ConfigPin(&PinCfg);
  } else if (UARTx == LPC_UART3) {
    // Initialize UART2 pin connect
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 0;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);
  }

  /* Initialize UART Configuration parameter structure to default state:
   * Baudrate = 9600bps
   * 8 data bit
   * 1 Stop bit
   * None parity
   */
  UART_ConfigStructInit(&UARTConfigStruct);

  // Re-configure baudrate
  UARTConfigStruct.Baud_rate = baudrate;

  // Initialize eripheral with given to corresponding parameter
  UART_Init(UARTx, &UARTConfigStruct);

  // Enable and reset the TX and RX FIFOs
  UART_FIFOConfigStructInit(&FIFOConfig);
  UART_FIFOConfig(UARTx, &FIFOConfig);

  // Enable UART Transmit
  UART_TxCmd(UARTx, ENABLE);

  // Configure Interrupts
  UART_IntConfig(UARTx, UART_INTCFG_RBR, ENABLE);
  UART_IntConfig(UARTx, UART_INTCFG_RLS, ENABLE);

  if (UARTx == LPC_UART0) NVIC_EnableIRQ(UART0_IRQn);
  else if ((LPC_UART1_TypeDef *) UARTx == LPC_UART1) NVIC_EnableIRQ(UART1_IRQn);
  else if (UARTx == LPC_UART2) NVIC_EnableIRQ(UART2_IRQn);
  else if (UARTx == LPC_UART3) NVIC_EnableIRQ(UART3_IRQn);

  RxQueueWritePos = RxQueueReadPos = 0;
  #if TX_BUFFER_SIZE > 0
    TxQueueWritePos = TxQueueReadPos = 0;
  #endif

  // Save the configured baudrate
  Baudrate = baudrate;
}

int HardwareSerial::peek() {
  int byte = -1;

  // Temporarily lock out UART receive interrupts during this read so the UART receive
  // interrupt won't cause problems with the index values
  UART_IntConfig(UARTx, UART_INTCFG_RBR, DISABLE);

  if (RxQueueReadPos != RxQueueWritePos)
    byte = RxBuffer[RxQueueReadPos];

  // Re-enable UART interrupts
  UART_IntConfig(UARTx, UART_INTCFG_RBR, ENABLE);

  return byte;
}

int HardwareSerial::read() {
  int byte = -1;

  // Temporarily lock out UART receive interrupts during this read so the UART receive
  // interrupt won't cause problems with the index values
  UART_IntConfig(UARTx, UART_INTCFG_RBR, DISABLE);

  if (RxQueueReadPos != RxQueueWritePos) {
    byte = RxBuffer[RxQueueReadPos];
    RxQueueReadPos = (RxQueueReadPos + 1) % RX_BUFFER_SIZE;
  }

  // Re-enable UART interrupts
  UART_IntConfig(UARTx, UART_INTCFG_RBR, ENABLE);

  return byte;
}

size_t HardwareSerial::write(uint8_t send) {
#if TX_BUFFER_SIZE > 0
  size_t bytes = 0;
  uint32_t fifolvl = 0;

  // If the Tx Buffer is full, wait for space to clear
  if ((TxQueueWritePos+1) % TX_BUFFER_SIZE == TxQueueReadPos) flushTX();

  // Temporarily lock out UART transmit interrupts during this read so the UART transmit interrupt won't
  // cause problems with the index values
  UART_IntConfig(UARTx, UART_INTCFG_THRE, DISABLE);

  // LPC17xx.h incorrectly defines FIFOLVL as a uint8_t, when it's actually a 32-bit register
  if ((LPC_UART1_TypeDef *) UARTx == LPC_UART1) {
    fifolvl = *(reinterpret_cast<volatile uint32_t *>(&((LPC_UART1_TypeDef *) UARTx)->FIFOLVL));
  } else fifolvl = *(reinterpret_cast<volatile uint32_t *>(&UARTx->FIFOLVL));

  // If the queue is empty and there's space in the FIFO, immediately send the byte
  if (TxQueueWritePos == TxQueueReadPos && fifolvl < UART_TX_FIFO_SIZE) {
    bytes = UART_Send(UARTx, &send, 1, BLOCKING);
  }
  // Otherwiise, write the byte to the transmit buffer
  else if ((TxQueueWritePos+1) % TX_BUFFER_SIZE != TxQueueReadPos) {
    TxBuffer[TxQueueWritePos] = send;
    TxQueueWritePos = (TxQueueWritePos+1) % TX_BUFFER_SIZE;
    bytes++;
  }

  // Re-enable the TX Interrupt
  UART_IntConfig(UARTx, UART_INTCFG_THRE, ENABLE);

  return bytes;
#else
  return UART_Send(UARTx, &send, 1, BLOCKING);
#endif
}

#if TX_BUFFER_SIZE > 0
  void HardwareSerial::flushTX() {
    // Wait for the tx buffer and FIFO to drain
    while (TxQueueWritePos != TxQueueReadPos && UART_CheckBusy(UARTx) == SET);
  }
#endif

int HardwareSerial::available() {
  return (RxQueueWritePos + RX_BUFFER_SIZE - RxQueueReadPos) % RX_BUFFER_SIZE;
}

void HardwareSerial::flush() {
  RxQueueWritePos = 0;
  RxQueueReadPos = 0;
}

void HardwareSerial::printf(const char *format, ...) {
  char RxBuffer[256];
  va_list vArgs;
  va_start(vArgs, format);
  int length = vsnprintf(RxBuffer, 256, format, vArgs);
  va_end(vArgs);
  if (length > 0 && length < 256) {
    for (int i = 0; i < length; ++i)
      write(RxBuffer[i]);
  }
}

void HardwareSerial::IRQHandler() {
  uint32_t IIRValue;
  uint8_t LSRValue, byte;

  IIRValue = UART_GetIntId(UARTx);
  IIRValue &= UART_IIR_INTID_MASK; // check bit 1~3, interrupt identification

  // Receive Line Status
  if (IIRValue == UART_IIR_INTID_RLS) {
    LSRValue = UART_GetLineStatus(UARTx);

    // Receive Line Status
    if (LSRValue & (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_RXFE | UART_LSR_BI)) {
      // There are errors or break interrupt
      // Read LSR will clear the interrupt
      Status = LSRValue;
      byte = UART_ReceiveByte(UARTx); // Dummy read on RX to clear interrupt, then bail out
      return;
    }
  }

  // Receive Data Available
  if (IIRValue == UART_IIR_INTID_RDA) {
    // Clear the FIFO
    while (UART_Receive(UARTx, &byte, 1, NONE_BLOCKING)) {
      if ((RxQueueWritePos + 1) % RX_BUFFER_SIZE != RxQueueReadPos) {
        RxBuffer[RxQueueWritePos] = byte;
        RxQueueWritePos = (RxQueueWritePos + 1) % RX_BUFFER_SIZE;
      } else
        break;
    }
    // Character timeout indicator
  } else if (IIRValue == UART_IIR_INTID_CTI) {
    // Character Time-out indicator
    Status |= 0x100; // Bit 9 as the CTI error
  }

  #if TX_BUFFER_SIZE > 0
    if (IIRValue == UART_IIR_INTID_THRE) {
      // Disable THRE interrupt
      UART_IntConfig(UARTx, UART_INTCFG_THRE, DISABLE);

      // Wait for FIFO buffer empty
      while (UART_CheckBusy(UARTx) == SET);

      // Transfer up to UART_TX_FIFO_SIZE bytes of data
      for (int i = 0; i < UART_TX_FIFO_SIZE && TxQueueWritePos != TxQueueReadPos; i++) {
        // Move a piece of data into the transmit FIFO
        if (UART_Send(UARTx, &TxBuffer[TxQueueReadPos], 1, NONE_BLOCKING)) {
          TxQueueReadPos = (TxQueueReadPos+1) % TX_BUFFER_SIZE;
        } else break;
      }

      // If there is no more data to send, disable the transmit interrupt - else enable it or keep it enabled
      if (TxQueueWritePos == TxQueueReadPos) {
        UART_IntConfig(UARTx, UART_INTCFG_THRE, DISABLE);
      } else UART_IntConfig(UARTx, UART_INTCFG_THRE, ENABLE);
    }
  #endif
}

#ifdef __cplusplus
extern "C" {
#endif

void UART0_IRQHandler(void) {
  #if SERIAL_PORT == 0 || SERIAL_PORT_2 == 0
    Serial.IRQHandler();
  #endif
}

void UART1_IRQHandler(void) {
  #if SERIAL_PORT == 1 || SERIAL_PORT_2 == 1
    Serial1.IRQHandler();
  #endif
}

void UART2_IRQHandler(void) {
  #if SERIAL_PORT == 2 || SERIAL_PORT_2 == 2
    Serial2.IRQHandler();
  #endif
}

void UART3_IRQHandler(void) {
  #if SERIAL_PORT == 3 || SERIAL_PORT_2 == 3
    Serial3.IRQHandler();
  #endif
}

#ifdef __cplusplus
}
#endif

#endif // TARGET_LPC1768
