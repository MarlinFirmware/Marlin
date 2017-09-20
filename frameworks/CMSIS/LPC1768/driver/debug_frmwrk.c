/**********************************************************************
* $Id$		debug_frmwrk.c				2010-05-21
*//**
* @file		debug_frmwrk.c
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

#include "debug_frmwrk.h"
#include "lpc17xx_pinsel.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */

#ifdef _DBGFWK
/* Debug framework */

void (*_db_msg)(LPC_UART_TypeDef *UARTx, const void *s);
void (*_db_msg_)(LPC_UART_TypeDef *UARTx, const void *s);
void (*_db_char)(LPC_UART_TypeDef *UARTx, uint8_t ch);
void (*_db_dec)(LPC_UART_TypeDef *UARTx, uint8_t decn);
void (*_db_dec_16)(LPC_UART_TypeDef *UARTx, uint16_t decn);
void (*_db_dec_32)(LPC_UART_TypeDef *UARTx, uint32_t decn);
void (*_db_hex)(LPC_UART_TypeDef *UARTx, uint8_t hexn);
void (*_db_hex_16)(LPC_UART_TypeDef *UARTx, uint16_t hexn);
void (*_db_hex_32)(LPC_UART_TypeDef *UARTx, uint32_t hexn);
uint8_t (*_db_get_char)(LPC_UART_TypeDef *UARTx);


/*********************************************************************//**
 * @brief		Puts a character to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	ch		Character to put
 * @return		None
 **********************************************************************/
void UARTPutChar (LPC_UART_TypeDef *UARTx, uint8_t ch)
{
	UART_Send(UARTx, &ch, 1, BLOCKING);
}


/*********************************************************************//**
 * @brief		Get a character to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @return		character value that returned
 **********************************************************************/
uint8_t UARTGetChar (LPC_UART_TypeDef *UARTx)
{
	uint8_t tmp = 0;
	UART_Receive(UARTx, &tmp, 1, BLOCKING);
	return(tmp);
}


/*********************************************************************//**
 * @brief		Puts a string to UART port
 * @param[in]	UARTx 	Pointer to UART peripheral
 * @param[in]	str 	string to put
 * @return		None
 **********************************************************************/
void UARTPuts(LPC_UART_TypeDef *UARTx, const void *str)
{
	uint8_t *s = (uint8_t *) str;

	while (*s)
	{
		UARTPutChar(UARTx, *s++);
	}
}


/*********************************************************************//**
 * @brief		Puts a string to UART port and print new line
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	str		String to put
 * @return		None
 **********************************************************************/
void UARTPuts_(LPC_UART_TypeDef *UARTx, const void *str)
{
	UARTPuts (UARTx, str);
	UARTPuts (UARTx, "\n\r");
}


/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec(LPC_UART_TypeDef *UARTx, uint8_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	UARTPutChar(UARTx, '0'+c3);
	UARTPutChar(UARTx, '0'+c2);
	UARTPutChar(UARTx, '0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec16(LPC_UART_TypeDef *UARTx, uint16_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	uint8_t c4=(decnum/1000)%10;
	uint8_t c5=(decnum/10000)%10;
	UARTPutChar(UARTx, '0'+c5);
	UARTPutChar(UARTx, '0'+c4);
	UARTPutChar(UARTx, '0'+c3);
	UARTPutChar(UARTx, '0'+c2);
	UARTPutChar(UARTx, '0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec32(LPC_UART_TypeDef *UARTx, uint32_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	uint8_t c4=(decnum/1000)%10;
	uint8_t c5=(decnum/10000)%10;
	uint8_t c6=(decnum/100000)%10;
	uint8_t c7=(decnum/1000000)%10;
	uint8_t c8=(decnum/10000000)%10;
	uint8_t c9=(decnum/100000000)%10;
	uint8_t c10=(decnum/1000000000)%10;
	UARTPutChar(UARTx, '0'+c10);
	UARTPutChar(UARTx, '0'+c9);
	UARTPutChar(UARTx, '0'+c8);
	UARTPutChar(UARTx, '0'+c7);
	UARTPutChar(UARTx, '0'+c6);
	UARTPutChar(UARTx, '0'+c5);
	UARTPutChar(UARTx, '0'+c4);
	UARTPutChar(UARTx, '0'+c3);
	UARTPutChar(UARTx, '0'+c2);
	UARTPutChar(UARTx, '0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex (LPC_UART_TypeDef *UARTx, uint8_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts(UARTx, "0x");
	i = 1;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar(UARTx, (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}


/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (16-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex16 (LPC_UART_TypeDef *UARTx, uint16_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts(UARTx, "0x");
	i = 3;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar(UARTx, (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}

/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (32-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex32 (LPC_UART_TypeDef *UARTx, uint32_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts(UARTx, "0x");
	i = 7;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar(UARTx, (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}

///*********************************************************************//**
// * @brief		print function that supports format as same as printf()
// * 				function of <stdio.h> library
// * @param[in]	None
// * @return		None
// **********************************************************************/
//void  _printf (const  char *format, ...)
//{
//    static  char  buffer[512 + 1];
//            va_list     vArgs;
//            char	*tmp;
//    va_start(vArgs, format);
//    vsprintf((char *)buffer, (char const *)format, vArgs);
//    va_end(vArgs);
//
//    _DBG(buffer);
//}

/*********************************************************************//**
 * @brief		Initialize Debug frame work through initializing UART port
 * @param[in]	None
 * @return		None
 **********************************************************************/
void debug_frmwrk_init(void)
{
	UART_CFG_Type UARTConfigStruct;
	PINSEL_CFG_Type PinCfg;

#if (USED_UART_DEBUG_PORT==0)
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

#elif (USED_UART_DEBUG_PORT==1)
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
#endif

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	// Re-configure baudrate to 115200bps
	UARTConfigStruct.Baud_rate = 115200;

	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)DEBUG_UART_PORT, &UARTConfigStruct);

	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)DEBUG_UART_PORT, ENABLE);

	_db_msg	= UARTPuts;
	_db_msg_ = UARTPuts_;
	_db_char = UARTPutChar;
	_db_hex = UARTPutHex;
	_db_hex_16 = UARTPutHex16;
	_db_hex_32 = UARTPutHex32;
	_db_dec = UARTPutDec;
	_db_dec_16 = UARTPutDec16;
	_db_dec_32 = UARTPutDec32;
	_db_get_char = UARTGetChar;
}
#endif /*_DBGFWK */


/* --------------------------------- End Of File ------------------------------ */
