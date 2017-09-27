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

#include "../../core/macros.h"
#include "../HAL.h"
#include "HardwareSerial.h"
#define UART3 3
HardwareSerial Serial3 = HardwareSerial(UART3);

volatile uint32_t UART0Status, UART1Status, UART2Status, UART3Status;
volatile uint8_t UART0TxEmpty = 1, UART1TxEmpty = 1, UART2TxEmpty=1, UART3TxEmpty=1;
volatile uint8_t UART0Buffer[UARTRXQUEUESIZE], UART1Buffer[UARTRXQUEUESIZE], UART2Buffer[UARTRXQUEUESIZE], UART3Buffer[UARTRXQUEUESIZE];
volatile uint32_t UART0RxQueueWritePos = 0, UART1RxQueueWritePos = 0, UART2RxQueueWritePos = 0, UART3RxQueueWritePos = 0;
volatile uint32_t UART0RxQueueReadPos = 0, UART1RxQueueReadPos = 0, UART2RxQueueReadPos = 0, UART3RxQueueReadPos = 0;
volatile uint8_t dummy;

void HardwareSerial::begin(uint32_t baudrate) {
  uint32_t Fdiv, pclkdiv, pclk;

  if (PortNum == 0) {
    LPC_PINCON->PINSEL0 &= ~0x000000F0;
    LPC_PINCON->PINSEL0 |= 0x00000050;  /* RxD0 is P0.3 and TxD0 is P0.2 */
    /* By default, the PCLKSELx value is zero, thus, the PCLK for
       all the peripherals is 1/4 of the SystemFrequency. */
    /* Bit 6~7 is for UART0 */
    pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
    switch (pclkdiv) {
      case 0x00:
      default:
        pclk = SystemCoreClock / 4;
        break;
      case 0x01:
        pclk = SystemCoreClock;
        break;
      case 0x02:
        pclk = SystemCoreClock / 2;
        break;
      case 0x03:
        pclk = SystemCoreClock / 8;
        break;
    }

    LPC_UART0->LCR = 0x83;       /* 8 bits, no Parity, 1 Stop bit */
    Fdiv = ( pclk / 16 ) / baudrate ;   /*baud rate */
    LPC_UART0->DLM = Fdiv / 256;
    LPC_UART0->DLL = Fdiv % 256;
    LPC_UART0->LCR = 0x03;      /* DLAB = 0 */
    LPC_UART0->FCR = 0x07;       /* Enable and reset TX and RX FIFO. */

    NVIC_EnableIRQ(UART0_IRQn);

    LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;   /* Enable UART0 interrupt */
  }
  else if (PortNum == 1) {
    LPC_PINCON->PINSEL4 &= ~0x0000000F;
    LPC_PINCON->PINSEL4 |= 0x0000000A;  /* Enable RxD1 P2.1, TxD1 P2.0 */

    /* By default, the PCLKSELx value is zero, thus, the PCLK for
    all the peripherals is 1/4 of the SystemFrequency. */
    /* Bit 8,9 are for UART1 */
    pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
    switch (pclkdiv) {
      case 0x00:
      default:
        pclk = SystemCoreClock / 4;
        break;
      case 0x01:
        pclk = SystemCoreClock;
        break;
      case 0x02:
        pclk = SystemCoreClock / 2;
        break;
      case 0x03:
        pclk = SystemCoreClock / 8;
        break;
    }

    LPC_UART1->LCR = 0x83;       /* 8 bits, no Parity, 1 Stop bit */
    Fdiv = ( pclk / 16 ) / baudrate ;   /*baud rate */
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;      /* DLAB = 0 */
    LPC_UART1->FCR = 0x07;       /* Enable and reset TX and RX FIFO. */

    NVIC_EnableIRQ(UART1_IRQn);

    LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;   /* Enable UART1 interrupt */
  }
  else if (PortNum == 2) {
    //LPC_PINCON->PINSEL4 &= ~0x000F0000;  /*Pinsel4 Bits 16-19*/
    //LPC_PINCON->PINSEL4 |=  0x000A0000;  /* RxD2 is P2.9 and TxD2 is P2.8, value 10*/
    LPC_PINCON->PINSEL0 &= ~0x00F00000;  /*Pinsel0 Bits 20-23*/
    LPC_PINCON->PINSEL0 |=  0x00500000;  /* RxD2 is P0.11 and TxD2 is P0.10, value 01*/

    LPC_SC->PCONP |= 1 << 24; //Enable PCUART2
    /* By default, the PCLKSELx value is zero, thus, the PCLK for
      all the peripherals is 1/4 of the SystemFrequency. */
    /* Bit 6~7 is for UART3 */
    pclkdiv = (LPC_SC->PCLKSEL1 >> 16) & 0x03;
    switch (pclkdiv) {
      case 0x00:
      default:
          pclk = SystemCoreClock / 4;
          break;
      case 0x01:
          pclk = SystemCoreClock;
          break;
      case 0x02:
          pclk = SystemCoreClock / 2;
          break;
      case 0x03:
          pclk = SystemCoreClock / 8;
          break;
    }
    LPC_UART2->LCR = 0x83;        /* 8 bits, no Parity, 1 Stop bit */
    Fdiv = (pclk / 16) / baudrate; /*baud rate */
    LPC_UART2->DLM = Fdiv >> 8;
    LPC_UART2->DLL = Fdiv & 0xFF;
    LPC_UART2->LCR = 0x03;        /* DLAB = 0 */
    LPC_UART2->FCR = 0x07;        /* Enable and reset TX and RX FIFO. */

    NVIC_EnableIRQ(UART2_IRQn);

    LPC_UART2->IER = IER_RBR | IER_THRE | IER_RLS;    /* Enable UART3 interrupt */
  }
  else if (PortNum == 3) {
    LPC_PINCON->PINSEL0 &= ~0x0000000F;
    LPC_PINCON->PINSEL0 |=  0x0000000A;  /* RxD3 is P0.1 and TxD3 is P0.0 */
    LPC_SC->PCONP |= 1 << 4 | 1 << 25; //Enable PCUART1
    /* By default, the PCLKSELx value is zero, thus, the PCLK for
      all the peripherals is 1/4 of the SystemFrequency. */
    /* Bit 6~7 is for UART3 */
    pclkdiv = (LPC_SC->PCLKSEL1 >> 18) & 0x03;
    switch (pclkdiv) {
      case 0x00:
      default:
        pclk = SystemCoreClock / 4;
        break;
      case 0x01:
        pclk = SystemCoreClock;
        break;
      case 0x02:
        pclk = SystemCoreClock / 2;
        break;
      case 0x03:
        pclk = SystemCoreClock / 8;
        break;
    }
    LPC_UART3->LCR = 0x83;        /* 8 bits, no Parity, 1 Stop bit */
    Fdiv = (pclk / 16) / baudrate ; /*baud rate */
    LPC_UART3->DLM = Fdiv >> 8;
    LPC_UART3->DLL = Fdiv & 0xFF;
    LPC_UART3->LCR = 0x03;        /* DLAB = 0 */
    LPC_UART3->FCR = 0x07;        /* Enable and reset TX and RX FIFO. */

    NVIC_EnableIRQ(UART3_IRQn);

    LPC_UART3->IER = IER_RBR | IER_THRE | IER_RLS;    /* Enable UART3 interrupt */
  }
}

int HardwareSerial::read() {
  uint8_t rx;
  if (PortNum == 0) {
    if (UART0RxQueueReadPos == UART0RxQueueWritePos) return -1;
    // Read from "head"
    rx = UART0Buffer[UART0RxQueueReadPos]; // grab next byte
    UART0RxQueueReadPos = (UART0RxQueueReadPos + 1) % UARTRXQUEUESIZE;
    return rx;
  }
  if (PortNum == 1) {
    if (UART1RxQueueReadPos == UART1RxQueueWritePos) return -1;
    rx = UART1Buffer[UART1RxQueueReadPos];
    UART1RxQueueReadPos = (UART1RxQueueReadPos + 1) % UARTRXQUEUESIZE;
    return rx;
  }
  if (PortNum == 2) {
    if (UART2RxQueueReadPos == UART2RxQueueWritePos) return -1;
    rx = UART2Buffer[UART2RxQueueReadPos];
    UART2RxQueueReadPos = (UART2RxQueueReadPos + 1) % UARTRXQUEUESIZE;
    return rx;
  }
  if (PortNum == 3) {
    if (UART3RxQueueReadPos == UART3RxQueueWritePos) return -1;
    rx = UART3Buffer[UART3RxQueueReadPos];
    UART3RxQueueReadPos = (UART3RxQueueReadPos + 1) % UARTRXQUEUESIZE;
    return rx;
  }
  return 0;
}

size_t HardwareSerial::write(uint8_t send) {
  if (PortNum == 0) {
    /* THRE status, contain valid data */
    while (!(UART0TxEmpty & 0x01));
    LPC_UART0->THR = send;
    UART0TxEmpty = 0; /* not empty in the THR until it shifts out */
  }
  else if (PortNum == 1) {
    while (!(UART1TxEmpty & 0x01));
    LPC_UART1->THR = send;
    UART1TxEmpty = 0;
  }
  else if (PortNum == 2) {
    while (!(UART2TxEmpty & 0x01));
    LPC_UART2->THR = send;
    UART2TxEmpty = 0;
  }
  else if (PortNum == 3) {
    while (!(UART3TxEmpty & 0x01));
    LPC_UART3->THR = send;
    UART3TxEmpty = 0;
  }
  return 0;
}

int HardwareSerial::available() {
  if (PortNum == 0)
    return (UART0RxQueueWritePos + UARTRXQUEUESIZE - UART0RxQueueReadPos) % UARTRXQUEUESIZE;
  if (PortNum == 1)
    return (UART1RxQueueWritePos + UARTRXQUEUESIZE - UART1RxQueueReadPos) % UARTRXQUEUESIZE;
  if (PortNum == 2)
    return (UART2RxQueueWritePos + UARTRXQUEUESIZE - UART2RxQueueReadPos) % UARTRXQUEUESIZE;
  if (PortNum == 3)
    return (UART3RxQueueWritePos + UARTRXQUEUESIZE - UART3RxQueueReadPos) % UARTRXQUEUESIZE;
  return 0;
}

void HardwareSerial::flush() {
  if (PortNum == 0)
    UART0RxQueueWritePos = UART0RxQueueReadPos = 0;
  if (PortNum == 1)
    UART1RxQueueWritePos = UART1RxQueueReadPos = 0;
  if (PortNum == 2)
    UART2RxQueueWritePos = UART2RxQueueReadPos = 0;
  if (PortNum == 3)
    UART3RxQueueWritePos = UART3RxQueueReadPos = 0;
}

void HardwareSerial::printf(const char *format, ...) {
  static char buffer[256];
  va_list vArgs;
  va_start(vArgs, format);
  int length = vsnprintf((char *) buffer, 256, (char const *) format, vArgs);
  va_end(vArgs);
  if (length > 0 && length < 256)
    for (int i = 0; i < length; ++i)
      write(buffer[i]);
}

/*****************************************************************************
** Function name:       UARTn_IRQHandler
**
** Descriptions:        UARTn interrupt handler
**
** parameters:          None
** Returned value:      None
**
*****************************************************************************/
#define DEFINE_UART_HANDLER(NUM)                                                                    \
  void UART3_IRQHandler(void) {                                                                     \
    uint8_t IIRValue, LSRValue;                                                                     \
    uint8_t Dummy = Dummy;                                                                          \
    IIRValue = LPC_UART ##NUM## ->IIR;                                                              \
    IIRValue >>= 1;                                                                                 \
    IIRValue &= 0x07;                                                                               \
    switch (IIRValue) {                                                                             \
      case IIR_RLS:                                                                                 \
        LSRValue = LPC_UART ##NUM## ->LSR;                                                          \
        if (LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI)) {                                    \
          UART ##NUM## Status = LSRValue;                                                           \
          Dummy = LPC_UART ##NUM## ->RBR;                                                           \
          return;                                                                                   \
        }                                                                                           \
        if (LSRValue & LSR_RDR) {                                                                   \
          if ((UART ##NUM## RxQueueWritePos+1) % UARTRXQUEUESIZE != UART ##NUM## RxQueueReadPos) {  \
            UART ##NUM## Buffer[UART ##NUM## RxQueueWritePos] = LPC_UART ##NUM## ->RBR;             \
            UART ##NUM## RxQueueWritePos = (UART ##NUM## RxQueueWritePos+1) % UARTRXQUEUESIZE;      \
          }                                                                                         \
        }                                                                                           \
        break;                                                                                      \
      case IIR_RDA:                                                                                 \
        if ((UART ##NUM## RxQueueWritePos+1) % UARTRXQUEUESIZE != UART ##NUM## RxQueueReadPos) {    \
          UART ##NUM## Buffer[UART ##NUM## RxQueueWritePos] = LPC_UART ##NUM## ->RBR;               \
          UART ##NUM## RxQueueWritePos = (UART ##NUM## RxQueueWritePos+1) % UARTRXQUEUESIZE;        \
        }                                                                                           \
        else                                                                                        \
          dummy = LPC_UART ##NUM## ->RBR;;                                                          \
        break;                                                                                      \
      case IIR_CTI:                                                                                 \
        UART ##NUM## Status |= 0x100;                                                               \
        break;                                                                                      \
      case IIR_THRE:                                                                                \
        LSRValue = LPC_UART ##NUM## ->LSR;                                                          \
        UART ##NUM## TxEmpty = (LSRValue & LSR_THRE) ? 1 : 0;                                       \
        break;                                                                                      \
    }                                                                                               \
  }                                                                                                 \
  typedef void _uart_ ## NUM

#ifdef __cplusplus
  extern "C" {
#endif

    DEFINE_UART_HANDLER(0);
    DEFINE_UART_HANDLER(1);
    DEFINE_UART_HANDLER(2);
    DEFINE_UART_HANDLER(3);

#ifdef __cplusplus
  }
#endif

#endif // TARGET_LPC1768
