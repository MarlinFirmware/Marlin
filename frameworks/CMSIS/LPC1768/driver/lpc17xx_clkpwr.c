/**********************************************************************
* $Id$		lpc17xx_clkpwr.c				2010-06-18
*//**
* @file		lpc17xx_clkpwr.c
* @brief	Contains all functions support for Clock and Power Control
* 			firmware library on LPC17xx
* @version	3.0
* @date		18. June. 2010
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
/** @addtogroup CLKPWR
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_clkpwr.h"


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup CLKPWR_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Set value of each Peripheral Clock Selection
 * @param[in]	ClkType	Peripheral Clock Selection of each type,
 * 				should be one of the following:
 *				- CLKPWR_PCLKSEL_WDT   		: WDT
				- CLKPWR_PCLKSEL_TIMER0   	: Timer 0
				- CLKPWR_PCLKSEL_TIMER1   	: Timer 1
				- CLKPWR_PCLKSEL_UART0   	: UART 0
				- CLKPWR_PCLKSEL_UART1  	: UART 1
				- CLKPWR_PCLKSEL_PWM1   	: PWM 1
				- CLKPWR_PCLKSEL_I2C0   	: I2C 0
				- CLKPWR_PCLKSEL_SPI   		: SPI
				- CLKPWR_PCLKSEL_SSP1   	: SSP 1
				- CLKPWR_PCLKSEL_DAC   		: DAC
				- CLKPWR_PCLKSEL_ADC   		: ADC
				- CLKPWR_PCLKSEL_CAN1  		: CAN 1
				- CLKPWR_PCLKSEL_CAN2  		: CAN 2
				- CLKPWR_PCLKSEL_ACF   		: ACF
				- CLKPWR_PCLKSEL_QEI 		: QEI
				- CLKPWR_PCLKSEL_PCB   		: PCB
				- CLKPWR_PCLKSEL_I2C1   	: I2C 1
				- CLKPWR_PCLKSEL_SSP0   	: SSP 0
				- CLKPWR_PCLKSEL_TIMER2   	: Timer 2
				- CLKPWR_PCLKSEL_TIMER3   	: Timer 3
				- CLKPWR_PCLKSEL_UART2   	: UART 2
				- CLKPWR_PCLKSEL_UART3   	: UART 3
				- CLKPWR_PCLKSEL_I2C2   	: I2C 2
				- CLKPWR_PCLKSEL_I2S   		: I2S
				- CLKPWR_PCLKSEL_RIT   		: RIT
				- CLKPWR_PCLKSEL_SYSCON   	: SYSCON
				- CLKPWR_PCLKSEL_MC 		: MC

 * @param[in]	DivVal	Value of divider, should be:
 * 				- CLKPWR_PCLKSEL_CCLK_DIV_4 : PCLK_peripheral = CCLK/4
 * 				- CLKPWR_PCLKSEL_CCLK_DIV_1 : PCLK_peripheral = CCLK/1
 *				- CLKPWR_PCLKSEL_CCLK_DIV_2 : PCLK_peripheral = CCLK/2
 *
 * @return none
 **********************************************************************/
void CLKPWR_SetPCLKDiv (uint32_t ClkType, uint32_t DivVal)
{
	uint32_t bitpos;

	bitpos = (ClkType < 32) ? (ClkType) : (ClkType - 32);

	/* PCLKSEL0 selected */
	if (ClkType < 32)
	{
		/* Clear two bit at bit position */
		LPC_SC->PCLKSEL0 &= (~(CLKPWR_PCLKSEL_BITMASK(bitpos)));

		/* Set two selected bit */
		LPC_SC->PCLKSEL0 |= (CLKPWR_PCLKSEL_SET(bitpos, DivVal));
	}
	/* PCLKSEL1 selected */
	else
	{
		/* Clear two bit at bit position */
		LPC_SC->PCLKSEL1 &= ~(CLKPWR_PCLKSEL_BITMASK(bitpos));

		/* Set two selected bit */
		LPC_SC->PCLKSEL1 |= (CLKPWR_PCLKSEL_SET(bitpos, DivVal));
	}
}


/*********************************************************************//**
 * @brief		Get current value of each Peripheral Clock Selection
 * @param[in]	ClkType	Peripheral Clock Selection of each type,
 * 				should be one of the following:
 *				- CLKPWR_PCLKSEL_WDT   		: WDT
				- CLKPWR_PCLKSEL_TIMER0   	: Timer 0
				- CLKPWR_PCLKSEL_TIMER1   	: Timer 1
				- CLKPWR_PCLKSEL_UART0   	: UART 0
				- CLKPWR_PCLKSEL_UART1  	: UART 1
				- CLKPWR_PCLKSEL_PWM1   	: PWM 1
				- CLKPWR_PCLKSEL_I2C0   	: I2C 0
				- CLKPWR_PCLKSEL_SPI   		: SPI
				- CLKPWR_PCLKSEL_SSP1   	: SSP 1
				- CLKPWR_PCLKSEL_DAC   		: DAC
				- CLKPWR_PCLKSEL_ADC   		: ADC
				- CLKPWR_PCLKSEL_CAN1  		: CAN 1
				- CLKPWR_PCLKSEL_CAN2  		: CAN 2
				- CLKPWR_PCLKSEL_ACF   		: ACF
				- CLKPWR_PCLKSEL_QEI 		: QEI
				- CLKPWR_PCLKSEL_PCB   		: PCB
				- CLKPWR_PCLKSEL_I2C1   	: I2C 1
				- CLKPWR_PCLKSEL_SSP0   	: SSP 0
				- CLKPWR_PCLKSEL_TIMER2   	: Timer 2
				- CLKPWR_PCLKSEL_TIMER3   	: Timer 3
				- CLKPWR_PCLKSEL_UART2   	: UART 2
				- CLKPWR_PCLKSEL_UART3   	: UART 3
				- CLKPWR_PCLKSEL_I2C2   	: I2C 2
				- CLKPWR_PCLKSEL_I2S   		: I2S
				- CLKPWR_PCLKSEL_RIT   		: RIT
				- CLKPWR_PCLKSEL_SYSCON   	: SYSCON
				- CLKPWR_PCLKSEL_MC 		: MC

 * @return		Value of Selected Peripheral Clock Selection
 **********************************************************************/
uint32_t CLKPWR_GetPCLKSEL (uint32_t ClkType)
{
	uint32_t bitpos, retval;

	if (ClkType < 32)
	{
		bitpos = ClkType;
		retval = LPC_SC->PCLKSEL0;
	}
	else
	{
		bitpos = ClkType - 32;
		retval = LPC_SC->PCLKSEL1;
	}

	retval = CLKPWR_PCLKSEL_GET(bitpos, retval);
	return retval;
}



/*********************************************************************//**
 * @brief 		Get current value of each Peripheral Clock
 * @param[in]	ClkType	Peripheral Clock Selection of each type,
 * 				should be one of the following:
 *				- CLKPWR_PCLKSEL_WDT   		: WDT
				- CLKPWR_PCLKSEL_TIMER0   	: Timer 0
				- CLKPWR_PCLKSEL_TIMER1   	: Timer 1
				- CLKPWR_PCLKSEL_UART0   	: UART 0
				- CLKPWR_PCLKSEL_UART1  	: UART 1
				- CLKPWR_PCLKSEL_PWM1   	: PWM 1
				- CLKPWR_PCLKSEL_I2C0   	: I2C 0
				- CLKPWR_PCLKSEL_SPI   		: SPI
				- CLKPWR_PCLKSEL_SSP1   	: SSP 1
				- CLKPWR_PCLKSEL_DAC   		: DAC
				- CLKPWR_PCLKSEL_ADC   		: ADC
				- CLKPWR_PCLKSEL_CAN1  		: CAN 1
				- CLKPWR_PCLKSEL_CAN2  		: CAN 2
				- CLKPWR_PCLKSEL_ACF   		: ACF
				- CLKPWR_PCLKSEL_QEI 		: QEI
				- CLKPWR_PCLKSEL_PCB   		: PCB
				- CLKPWR_PCLKSEL_I2C1   	: I2C 1
				- CLKPWR_PCLKSEL_SSP0   	: SSP 0
				- CLKPWR_PCLKSEL_TIMER2   	: Timer 2
				- CLKPWR_PCLKSEL_TIMER3   	: Timer 3
				- CLKPWR_PCLKSEL_UART2   	: UART 2
				- CLKPWR_PCLKSEL_UART3   	: UART 3
				- CLKPWR_PCLKSEL_I2C2   	: I2C 2
				- CLKPWR_PCLKSEL_I2S   		: I2S
				- CLKPWR_PCLKSEL_RIT   		: RIT
				- CLKPWR_PCLKSEL_SYSCON   	: SYSCON
				- CLKPWR_PCLKSEL_MC 		: MC

 * @return		Value of Selected Peripheral Clock
 **********************************************************************/
uint32_t CLKPWR_GetPCLK (uint32_t ClkType)
{
	uint32_t retval, div;

	retval = SystemCoreClock;
	div = CLKPWR_GetPCLKSEL(ClkType);

	switch (div)
	{
	case 0:
		div = 4;
		break;

	case 1:
		div = 1;
		break;

	case 2:
		div = 2;
		break;

	case 3:
		div = 8;
		break;
	}
	retval /= div;

	return retval;
}



/*********************************************************************//**
 * @brief 		Configure power supply for each peripheral according to NewState
 * @param[in]	PPType	Type of peripheral used to enable power,
 *     					should be one of the following:
 *     			-  CLKPWR_PCONP_PCTIM0 		: Timer 0
				-  CLKPWR_PCONP_PCTIM1 		: Timer 1
				-  CLKPWR_PCONP_PCUART0  	: UART 0
				-  CLKPWR_PCONP_PCUART1   	: UART 1
				-  CLKPWR_PCONP_PCPWM1 		: PWM 1
				-  CLKPWR_PCONP_PCI2C0 		: I2C 0
				-  CLKPWR_PCONP_PCSPI   	: SPI
				-  CLKPWR_PCONP_PCRTC   	: RTC
				-  CLKPWR_PCONP_PCSSP1 		: SSP 1
				-  CLKPWR_PCONP_PCAD   		: ADC
				-  CLKPWR_PCONP_PCAN1   	: CAN 1
				-  CLKPWR_PCONP_PCAN2   	: CAN 2
				-  CLKPWR_PCONP_PCGPIO 		: GPIO
				-  CLKPWR_PCONP_PCRIT 		: RIT
				-  CLKPWR_PCONP_PCMC 		: MC
				-  CLKPWR_PCONP_PCQEI 		: QEI
				-  CLKPWR_PCONP_PCI2C1   	: I2C 1
				-  CLKPWR_PCONP_PCSSP0 		: SSP 0
				-  CLKPWR_PCONP_PCTIM2 		: Timer 2
				-  CLKPWR_PCONP_PCTIM3 		: Timer 3
				-  CLKPWR_PCONP_PCUART2  	: UART 2
				-  CLKPWR_PCONP_PCUART3   	: UART 3
				-  CLKPWR_PCONP_PCI2C2 		: I2C 2
				-  CLKPWR_PCONP_PCI2S   	: I2S
				-  CLKPWR_PCONP_PCGPDMA   	: GPDMA
				-  CLKPWR_PCONP_PCENET 		: Ethernet
				-  CLKPWR_PCONP_PCUSB   	: USB
 *
 * @param[in]	NewState	New state of Peripheral Power, should be:
 * 				- ENABLE	: Enable power for this peripheral
 * 				- DISABLE	: Disable power for this peripheral
 *
 * @return none
 **********************************************************************/
void CLKPWR_ConfigPPWR (uint32_t PPType, FunctionalState NewState)
{
	if (NewState == ENABLE)
	{
		LPC_SC->PCONP |= PPType & CLKPWR_PCONP_BITMASK;
	}
	else if (NewState == DISABLE)
	{
		LPC_SC->PCONP &= (~PPType) & CLKPWR_PCONP_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Enter Sleep mode with co-operated instruction by the Cortex-M3.
 * @param[in]	None
 * @return		None
 **********************************************************************/
void CLKPWR_Sleep(void)
{
	LPC_SC->PCON = 0x00;
	/* Sleep Mode*/
	__WFI();
}


/*********************************************************************//**
 * @brief 		Enter Deep Sleep mode with co-operated instruction by the Cortex-M3.
 * @param[in]	None
 * @return		None
 **********************************************************************/
void CLKPWR_DeepSleep(void)
{
    /* Deep-Sleep Mode, set SLEEPDEEP bit */
	SCB->SCR = 0x4;
	LPC_SC->PCON = 0x00;
	/* Deep Sleep Mode*/
	__WFI();
}


/*********************************************************************//**
 * @brief 		Enter Power Down mode with co-operated instruction by the Cortex-M3.
 * @param[in]	None
 * @return		None
 **********************************************************************/
void CLKPWR_PowerDown(void)
{
    /* Deep-Sleep Mode, set SLEEPDEEP bit */
	SCB->SCR = 0x4;
	LPC_SC->PCON = 0x01;
	/* Power Down Mode*/
	__WFI();
}


/*********************************************************************//**
 * @brief 		Enter Deep Power Down mode with co-operated instruction by the Cortex-M3.
 * @param[in]	None
 * @return		None
 **********************************************************************/
void CLKPWR_DeepPowerDown(void)
{
    /* Deep-Sleep Mode, set SLEEPDEEP bit */
	SCB->SCR = 0x4;
	LPC_SC->PCON = 0x03;
	/* Deep Power Down Mode*/
	__WFI();
}

/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
