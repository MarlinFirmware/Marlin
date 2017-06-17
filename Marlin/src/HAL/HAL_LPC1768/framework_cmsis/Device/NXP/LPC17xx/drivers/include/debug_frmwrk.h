/**********************************************************************
* $Id$		debug_frmwrk.h		2010-05-21
*//**
* @file		debug_frmwrk.h
* @brief	Contains some utilities that used for debugging through UART
* @version	2.0
* @date		21. May. 2010
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2010, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#ifndef DEBUG_FRMWRK_H_
#define DEBUG_FRMWRK_H_

//#include <stdarg.h>
#include "lpc17xx_uart.h"

#define USED_UART_DEBUG_PORT	0

#if (USED_UART_DEBUG_PORT==0)
#define DEBUG_UART_PORT	LPC_UART0
#elif (USED_UART_DEBUG_PORT==1)
#define DEBUG_UART_PORT	LPC_UART1
#endif

#define _DBG(x)	 	_db_msg(DEBUG_UART_PORT, x)
#define _DBG_(x)	_db_msg_(DEBUG_UART_PORT, x)
#define _DBC(x)	 	_db_char(DEBUG_UART_PORT, x)
#define _DBD(x)	 	_db_dec(DEBUG_UART_PORT, x)
#define _DBD16(x)	 _db_dec_16(DEBUG_UART_PORT, x)
#define _DBD32(x)	 _db_dec_32(DEBUG_UART_PORT, x)
#define _DBH(x)	 	_db_hex(DEBUG_UART_PORT, x)
#define _DBH16(x)	 _db_hex_16(DEBUG_UART_PORT, x)
#define _DBH32(x)	 _db_hex_32(DEBUG_UART_PORT, x)
#define _DG			_db_get_char(DEBUG_UART_PORT)
//void  _printf (const  char *format, ...);

extern void (*_db_msg)(LPC_UART_TypeDef *UARTx, const void *s);
extern void (*_db_msg_)(LPC_UART_TypeDef *UARTx, const void *s);
extern void (*_db_char)(LPC_UART_TypeDef *UARTx, uint8_t ch);
extern void (*_db_dec)(LPC_UART_TypeDef *UARTx, uint8_t decn);
extern void (*_db_dec_16)(LPC_UART_TypeDef *UARTx, uint16_t decn);
extern void (*_db_dec_32)(LPC_UART_TypeDef *UARTx, uint32_t decn);
extern void (*_db_hex)(LPC_UART_TypeDef *UARTx, uint8_t hexn);
extern void (*_db_hex_16)(LPC_UART_TypeDef *UARTx, uint16_t hexn);
extern void (*_db_hex_32)(LPC_UART_TypeDef *UARTx, uint32_t hexn);
extern uint8_t (*_db_get_char)(LPC_UART_TypeDef *UARTx);

void UARTPutChar (LPC_UART_TypeDef *UARTx, uint8_t ch);
void UARTPuts(LPC_UART_TypeDef *UARTx, const void *str);
void UARTPuts_(LPC_UART_TypeDef *UARTx, const void *str);
void UARTPutDec(LPC_UART_TypeDef *UARTx, uint8_t decnum);
void UARTPutDec16(LPC_UART_TypeDef *UARTx, uint16_t decnum);
void UARTPutDec32(LPC_UART_TypeDef *UARTx, uint32_t decnum);
void UARTPutHex (LPC_UART_TypeDef *UARTx, uint8_t hexnum);
void UARTPutHex16 (LPC_UART_TypeDef *UARTx, uint16_t hexnum);
void UARTPutHex32 (LPC_UART_TypeDef *UARTx, uint32_t hexnum);
uint8_t UARTGetChar (LPC_UART_TypeDef *UARTx);
void debug_frmwrk_init(void);

#endif /* DEBUG_FRMWRK_H_ */
