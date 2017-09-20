/**********************************************************************
* $Id$		lpc17xx_pinsel.c				2010-05-21
*//**
* @file		lpc17xx_pinsel.c
* @brief	Contains all functions support for Pin connect block firmware
* 			library on LPC17xx
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

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup PINSEL
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_pinsel.h"

/* Public Functions ----------------------------------------------------------- */

static void set_PinFunc ( uint8_t portnum, uint8_t pinnum, uint8_t funcnum);
static void set_ResistorMode ( uint8_t portnum, uint8_t pinnum, uint8_t modenum);
static void set_OpenDrainMode( uint8_t portnum, uint8_t pinnum, uint8_t modenum);

/*********************************************************************//**
 * @brief 		Setup the pin selection function
 * @param[in]	portnum PORT number,
 * 				should be one of the following:
 * 				- PINSEL_PORT_0	: Port 0
 * 				- PINSEL_PORT_1	: Port 1
 * 				- PINSEL_PORT_2	: Port 2
 * 				- PINSEL_PORT_3	: Port 3
 *
 * @param[in]	pinnum	Pin number,
 * 				should be one of the following:
				- PINSEL_PIN_0 : Pin 0
				- PINSEL_PIN_1 : Pin 1
				- PINSEL_PIN_2 : Pin 2
				- PINSEL_PIN_3 : Pin 3
				- PINSEL_PIN_4 : Pin 4
				- PINSEL_PIN_5 : Pin 5
				- PINSEL_PIN_6 : Pin 6
				- PINSEL_PIN_7 : Pin 7
				- PINSEL_PIN_8 : Pin 8
				- PINSEL_PIN_9 : Pin 9
				- PINSEL_PIN_10 : Pin 10
				- PINSEL_PIN_11 : Pin 11
				- PINSEL_PIN_12 : Pin 12
				- PINSEL_PIN_13 : Pin 13
				- PINSEL_PIN_14 : Pin 14
				- PINSEL_PIN_15 : Pin 15
				- PINSEL_PIN_16 : Pin 16
				- PINSEL_PIN_17 : Pin 17
				- PINSEL_PIN_18 : Pin 18
				- PINSEL_PIN_19 : Pin 19
				- PINSEL_PIN_20 : Pin 20
				- PINSEL_PIN_21 : Pin 21
				- PINSEL_PIN_22 : Pin 22
				- PINSEL_PIN_23 : Pin 23
				- PINSEL_PIN_24 : Pin 24
				- PINSEL_PIN_25 : Pin 25
				- PINSEL_PIN_26 : Pin 26
				- PINSEL_PIN_27 : Pin 27
				- PINSEL_PIN_28 : Pin 28
				- PINSEL_PIN_29 : Pin 29
				- PINSEL_PIN_30 : Pin 30
				- PINSEL_PIN_31 : Pin 31

 * @param[in] 	funcnum Function number,
 * 				should be one of the following:
 *				- PINSEL_FUNC_0 : default function
 *				- PINSEL_FUNC_1 : first alternate function
 *				- PINSEL_FUNC_2 : second alternate function
 *				- PINSEL_FUNC_3 : third alternate function
 *
 * @return 		None
 **********************************************************************/
static void set_PinFunc ( uint8_t portnum, uint8_t pinnum, uint8_t funcnum)
{
	uint32_t pinnum_t = pinnum;
	uint32_t pinselreg_idx = 2 * portnum;
	uint32_t *pPinCon = (uint32_t *)&LPC_PINCON->PINSEL0;

	if (pinnum_t >= 16) {
		pinnum_t -= 16;
		pinselreg_idx++;
	}
	*(uint32_t *)(pPinCon + pinselreg_idx) &= ~(0x03UL << (pinnum_t * 2));
	*(uint32_t *)(pPinCon + pinselreg_idx) |= ((uint32_t)funcnum) << (pinnum_t * 2);
}

/*********************************************************************//**
 * @brief 		Setup resistor mode for each pin
 * @param[in]	portnum PORT number,
 * 				should be one of the following:
 * 				- PINSEL_PORT_0	: Port 0
 * 				- PINSEL_PORT_1	: Port 1
 * 				- PINSEL_PORT_2	: Port 2
 * 				- PINSEL_PORT_3	: Port 3
 * @param[in]	pinnum	Pin number,
 * 				should be one of the following:
				- PINSEL_PIN_0 : Pin 0
				- PINSEL_PIN_1 : Pin 1
				- PINSEL_PIN_2 : Pin 2
				- PINSEL_PIN_3 : Pin 3
				- PINSEL_PIN_4 : Pin 4
				- PINSEL_PIN_5 : Pin 5
				- PINSEL_PIN_6 : Pin 6
				- PINSEL_PIN_7 : Pin 7
				- PINSEL_PIN_8 : Pin 8
				- PINSEL_PIN_9 : Pin 9
				- PINSEL_PIN_10 : Pin 10
				- PINSEL_PIN_11 : Pin 11
				- PINSEL_PIN_12 : Pin 12
				- PINSEL_PIN_13 : Pin 13
				- PINSEL_PIN_14 : Pin 14
				- PINSEL_PIN_15 : Pin 15
				- PINSEL_PIN_16 : Pin 16
				- PINSEL_PIN_17 : Pin 17
				- PINSEL_PIN_18 : Pin 18
				- PINSEL_PIN_19 : Pin 19
				- PINSEL_PIN_20 : Pin 20
				- PINSEL_PIN_21 : Pin 21
				- PINSEL_PIN_22 : Pin 22
				- PINSEL_PIN_23 : Pin 23
				- PINSEL_PIN_24 : Pin 24
				- PINSEL_PIN_25 : Pin 25
				- PINSEL_PIN_26 : Pin 26
				- PINSEL_PIN_27 : Pin 27
				- PINSEL_PIN_28 : Pin 28
				- PINSEL_PIN_29 : Pin 29
				- PINSEL_PIN_30 : Pin 30
				- PINSEL_PIN_31 : Pin 31

 * @param[in] 	modenum: Mode number,
 * 				should be one of the following:
				- PINSEL_PINMODE_PULLUP	: Internal pull-up resistor
				- PINSEL_PINMODE_TRISTATE : Tri-state
				- PINSEL_PINMODE_PULLDOWN : Internal pull-down resistor

 * @return 		None
 **********************************************************************/
void set_ResistorMode ( uint8_t portnum, uint8_t pinnum, uint8_t modenum)
{
	uint32_t pinnum_t = pinnum;
	uint32_t pinmodereg_idx = 2 * portnum;
	uint32_t *pPinCon = (uint32_t *)&LPC_PINCON->PINMODE0;

	if (pinnum_t >= 16) {
		pinnum_t -= 16;
		pinmodereg_idx++ ;
	}

	*(uint32_t *)(pPinCon + pinmodereg_idx) &= ~(0x03UL << (pinnum_t * 2));
	*(uint32_t *)(pPinCon + pinmodereg_idx) |= ((uint32_t)modenum) << (pinnum_t * 2);
}

/*********************************************************************//**
 * @brief 		Setup Open drain mode for each pin
 * @param[in]	portnum PORT number,
 * 				should be one of the following:
 * 				- PINSEL_PORT_0	: Port 0
 * 				- PINSEL_PORT_1	: Port 1
 * 				- PINSEL_PORT_2	: Port 2
 * 				- PINSEL_PORT_3	: Port 3
 *
 * @param[in]	pinnum	Pin number,
 * 				should be one of the following:
				- PINSEL_PIN_0 : Pin 0
				- PINSEL_PIN_1 : Pin 1
				- PINSEL_PIN_2 : Pin 2
				- PINSEL_PIN_3 : Pin 3
				- PINSEL_PIN_4 : Pin 4
				- PINSEL_PIN_5 : Pin 5
				- PINSEL_PIN_6 : Pin 6
				- PINSEL_PIN_7 : Pin 7
				- PINSEL_PIN_8 : Pin 8
				- PINSEL_PIN_9 : Pin 9
				- PINSEL_PIN_10 : Pin 10
				- PINSEL_PIN_11 : Pin 11
				- PINSEL_PIN_12 : Pin 12
				- PINSEL_PIN_13 : Pin 13
				- PINSEL_PIN_14 : Pin 14
				- PINSEL_PIN_15 : Pin 15
				- PINSEL_PIN_16 : Pin 16
				- PINSEL_PIN_17 : Pin 17
				- PINSEL_PIN_18 : Pin 18
				- PINSEL_PIN_19 : Pin 19
				- PINSEL_PIN_20 : Pin 20
				- PINSEL_PIN_21 : Pin 21
				- PINSEL_PIN_22 : Pin 22
				- PINSEL_PIN_23 : Pin 23
				- PINSEL_PIN_24 : Pin 24
				- PINSEL_PIN_25 : Pin 25
				- PINSEL_PIN_26 : Pin 26
				- PINSEL_PIN_27 : Pin 27
				- PINSEL_PIN_28 : Pin 28
				- PINSEL_PIN_29 : Pin 29
				- PINSEL_PIN_30 : Pin 30
				- PINSEL_PIN_31 : Pin 31

 * @param[in]	modenum  Open drain mode number,
 * 				should be one of the following:
 * 				- PINSEL_PINMODE_NORMAL : Pin is in the normal (not open drain) mode
 * 				- PINSEL_PINMODE_OPENDRAIN : Pin is in the open drain mode
 *
 * @return 		None
 **********************************************************************/
void set_OpenDrainMode( uint8_t portnum, uint8_t pinnum, uint8_t modenum)
{
	uint32_t *pPinCon = (uint32_t *)&LPC_PINCON->PINMODE_OD0;

	if (modenum == PINSEL_PINMODE_OPENDRAIN){
		*(uint32_t *)(pPinCon + portnum) |= (0x01UL << pinnum);
	} else {
		*(uint32_t *)(pPinCon + portnum) &= ~(0x01UL << pinnum);
	}
}

/* End of Public Functions ---------------------------------------------------- */

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup PINSEL_Public_Functions
 * @{
 */
/*********************************************************************//**
 * @brief 		Configure trace function
 * @param[in] 	NewState State of the Trace function configuration,
 * 				should be one of the following:
 * 				- ENABLE : Enable Trace Function
 * 				- DISABLE : Disable Trace Function
 *
 * @return 		None
 **********************************************************************/
void PINSEL_ConfigTraceFunc(FunctionalState NewState)
{
	if (NewState == ENABLE) {
		LPC_PINCON->PINSEL10 |= (0x01UL << 3);
	} else if (NewState == DISABLE) {
		LPC_PINCON->PINSEL10 &= ~(0x01UL << 3);
	}
}

/*********************************************************************//**
 * @brief 		Setup I2C0 pins
 * @param[in]	i2cPinMode I2C pin mode,
 * 				should be one of the following:
 * 				- PINSEL_I2C_Normal_Mode : The standard drive mode
 * 				- PINSEL_I2C_Fast_Mode : Fast Mode Plus drive mode
 *
 * @param[in]	filterSlewRateEnable  should be:
 * 				- ENABLE: Enable filter and slew rate.
 * 				- DISABLE: Disable filter and slew rate.
 *
 * @return 		None
 **********************************************************************/
void PINSEL_SetI2C0Pins(uint8_t i2cPinMode, FunctionalState filterSlewRateEnable)
{
	uint32_t regVal;

	if (i2cPinMode == PINSEL_I2C_Fast_Mode){
		regVal = PINSEL_I2CPADCFG_SCLDRV0 | PINSEL_I2CPADCFG_SDADRV0;
	}

	if (filterSlewRateEnable == DISABLE){
		regVal = PINSEL_I2CPADCFG_SCLI2C0 | PINSEL_I2CPADCFG_SDAI2C0;
	}
	LPC_PINCON->I2CPADCFG = regVal;
}


/*********************************************************************//**
 * @brief 		Configure Pin corresponding to specified parameters passed
 * 				in the PinCfg
 * @param[in]	PinCfg	Pointer to a PINSEL_CFG_Type structure
 *                    that contains the configuration information for the
 *                    specified pin.
 * @return 		None
 **********************************************************************/
void PINSEL_ConfigPin(PINSEL_CFG_Type *PinCfg)
{
	set_PinFunc(PinCfg->Portnum, PinCfg->Pinnum, PinCfg->Funcnum);
	set_ResistorMode(PinCfg->Portnum, PinCfg->Pinnum, PinCfg->Pinmode);
	set_OpenDrainMode(PinCfg->Portnum, PinCfg->Pinnum, PinCfg->OpenDrain);
}


/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
