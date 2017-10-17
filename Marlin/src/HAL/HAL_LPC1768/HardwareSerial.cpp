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

HardwareSerial Serial = HardwareSerial(LPC_UART0);
HardwareSerial Serial1 = HardwareSerial((LPC_UART_TypeDef *) LPC_UART1);
HardwareSerial Serial2 = HardwareSerial(LPC_UART2);
HardwareSerial Serial3 = HardwareSerial(LPC_UART3);

void HardwareSerial::begin(uint32_t baudrate) {

  UART_CFG_Type UARTConfigStruct;
  PINSEL_CFG_Type PinCfg;
  UART_FIFO_CFG_Type FIFOConfig;

  if (UARTx == LPC_UART0) {
    /*
    * Initialize UART0 pin connect
    */
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 2;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 3;
    PINSEL_ConfigPin(&PinCfg);
  }
  else if ((LPC_UART1_TypeDef *) UARTx == LPC_UART1) {
    /*
    * Initialize UART1 pin connect
    */
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 15;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 16;
    PINSEL_ConfigPin(&PinCfg);
  }
  else if (UARTx == LPC_UART2) {
    /*
    * Initialize UART2 pin connect
    */
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 10;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 11;
    PINSEL_ConfigPin(&PinCfg);
  }
  else if (UARTx == LPC_UART3) {
    /*
    * Initialize UART2 pin connect
    */
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
  
  // Enable and reset the TX and RC FIFOs
  UART_FIFOConfigStructInit(&FIFOConfig);
  UART_FIFOConfig(UARTx, &FIFOConfig);

	// Enable UART Transmit
  UART_TxCmd(UARTx, ENABLE);

  // Configure Interrupts
  UART_IntConfig(UARTx, UART_INTCFG_RBR, ENABLE);
  UART_IntConfig(UARTx, UART_INTCFG_RLS, ENABLE);
  
  if (UARTx == LPC_UART0)
    NVIC_EnableIRQ(UART0_IRQn);
  else if ((LPC_UART1_TypeDef *) UARTx == LPC_UART1)
    NVIC_EnableIRQ(UART1_IRQn);
  else if (UARTx == LPC_UART2)
    NVIC_EnableIRQ(UART2_IRQn);
  else if (UARTx == LPC_UART3)
    NVIC_EnableIRQ(UART3_IRQn);
}

int HardwareSerial::peek() {
  if (RxQueueReadPos == RxQueueWritePos)
    return -1;

  // Read from "head"
  return RxBuffer[RxQueueReadPos];
}

int HardwareSerial::read() {
  int rx = peek();

  if (rx != -1)
    RxQueueReadPos = (RxQueueReadPos + 1) % UARTRXQUEUESIZE;

  return rx;
}

size_t HardwareSerial::write(uint8_t send) {
  return UART_Send(UARTx, &send, 1, BLOCKING);
}

int HardwareSerial::available() {
  return (RxQueueWritePos + UARTRXQUEUESIZE - RxQueueReadPos) % UARTRXQUEUESIZE;
}

void HardwareSerial::flush() {
  RxQueueWritePos = 0;
  RxQueueReadPos = 0;
}

void HardwareSerial::printf(const char *format, ...) {
  static char RxBuffer[256];
  va_list vArgs;
  va_start(vArgs, format);
  int length = vsnprintf((char *) RxBuffer, 256, (char const *) format, vArgs);
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
  IIRValue &= UART_IIR_INTID_MASK;		/* check bit 1~3, interrupt identification */

  if ( IIRValue == UART_IIR_INTID_RLS )		/* Receive Line Status */
  {
    LSRValue = UART_GetLineStatus(UARTx);

    /* Receive Line Status */
    if ( LSRValue & (UART_LSR_OE|UART_LSR_PE|UART_LSR_FE|UART_LSR_RXFE|UART_LSR_BI) )
    {
      /* There are errors or break interrupt */
      /* Read LSR will clear the interrupt */
      Status = LSRValue;
      byte = UART_ReceiveByte(UARTx);		/* Dummy read on RX to clear
                                           interrupt, then bail out */
      return;
    }
  }
  
  if ( IIRValue == UART_IIR_INTID_RDA )	/* Receive Data Available */
  {
    /* Clear the FIFO */
    while ( UART_Receive((LPC_UART_TypeDef *)LPC_UART0, &byte, 1, NONE_BLOCKING) ) {
      if ((RxQueueWritePos+1) % UARTRXQUEUESIZE != RxQueueReadPos)
      {
        RxBuffer[RxQueueWritePos] = byte;
        RxQueueWritePos = (RxQueueWritePos+1) % UARTRXQUEUESIZE;
      }
    }
  }
  else if ( IIRValue == UART_IIR_INTID_CTI )	/* Character timeout indicator */
  {
    /* Character Time-out indicator */
    Status |= 0x100;		/* Bit 9 as the CTI error */
  }
}

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
** Function name:		UART0_IRQHandler
**
** Descriptions:		UART0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART0_IRQHandler (void)
{
  Serial.IRQHandler();
}

/*****************************************************************************
** Function name:		UART1_IRQHandler
**
** Descriptions:		UART1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART1_IRQHandler (void)
{
  Serial1.IRQHandler();
}

/*****************************************************************************
** Function name:		UART2_IRQHandler
**
** Descriptions:		UART2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART2_IRQHandler (void)
{
  Serial2.IRQHandler();
}

/*****************************************************************************
** Function name:		UART3_IRQHandler
**
** Descriptions:		UART3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART3_IRQHandler (void)
{
  Serial3.IRQHandler();
}

#ifdef __cplusplus
}
#endif

#endif // TARGET_LPC1768
