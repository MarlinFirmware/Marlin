/**********************************************************************
* $Id$		lpc17xx_clkpwr.h			2010-05-21
*//**
* @file		lpc17xx_clkpwr.h
* @brief	Contains all macro definitions and function prototypes
* 			support for Clock and Power Control firmware library on LPC17xx
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
/** @defgroup CLKPWR CLKPWR (Clock Power)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_CLKPWR_H_
#define LPC17XX_CLKPWR_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup CLKPWR_Public_Macros CLKPWR Public Macros
 * @{
 */

/**********************************************************************
 * Peripheral Clock Selection Definitions
 **********************************************************************/
/** Peripheral clock divider bit position for WDT */
#define	CLKPWR_PCLKSEL_WDT  		((uint32_t)(0))
/** Peripheral clock divider bit position for TIMER0 */
#define	CLKPWR_PCLKSEL_TIMER0  		((uint32_t)(2))
/** Peripheral clock divider bit position for TIMER1 */
#define	CLKPWR_PCLKSEL_TIMER1  		((uint32_t)(4))
/** Peripheral clock divider bit position for UART0 */
#define	CLKPWR_PCLKSEL_UART0  		((uint32_t)(6))
/** Peripheral clock divider bit position for UART1 */
#define	CLKPWR_PCLKSEL_UART1  		((uint32_t)(8))
/** Peripheral clock divider bit position for PWM1 */
#define	CLKPWR_PCLKSEL_PWM1  		((uint32_t)(12))
/** Peripheral clock divider bit position for I2C0 */
#define	CLKPWR_PCLKSEL_I2C0  		((uint32_t)(14))
/** Peripheral clock divider bit position for SPI */
#define	CLKPWR_PCLKSEL_SPI  		((uint32_t)(16))
/** Peripheral clock divider bit position for SSP1 */
#define	CLKPWR_PCLKSEL_SSP1  		((uint32_t)(20))
/** Peripheral clock divider bit position for DAC */
#define	CLKPWR_PCLKSEL_DAC  		((uint32_t)(22))
/** Peripheral clock divider bit position for ADC */
#define	CLKPWR_PCLKSEL_ADC  		((uint32_t)(24))
/** Peripheral clock divider bit position for CAN1 */
#define	CLKPWR_PCLKSEL_CAN1 		((uint32_t)(26))
/** Peripheral clock divider bit position for CAN2 */
#define	CLKPWR_PCLKSEL_CAN2 		((uint32_t)(28))
/** Peripheral clock divider bit position for ACF */
#define	CLKPWR_PCLKSEL_ACF  		((uint32_t)(30))
/** Peripheral clock divider bit position for QEI */
#define	CLKPWR_PCLKSEL_QEI  		((uint32_t)(32))
/** Peripheral clock divider bit position for PCB */
#define	CLKPWR_PCLKSEL_PCB  		((uint32_t)(36))
/** Peripheral clock divider bit position for  I2C1 */
#define	CLKPWR_PCLKSEL_I2C1  		((uint32_t)(38))
/** Peripheral clock divider bit position for SSP0 */
#define	CLKPWR_PCLKSEL_SSP0  		((uint32_t)(42))
/** Peripheral clock divider bit position for TIMER2 */
#define	CLKPWR_PCLKSEL_TIMER2  		((uint32_t)(44))
/** Peripheral clock divider bit position for  TIMER3 */
#define	CLKPWR_PCLKSEL_TIMER3  		((uint32_t)(46))
/** Peripheral clock divider bit position for UART2 */
#define	CLKPWR_PCLKSEL_UART2  		((uint32_t)(48))
/** Peripheral clock divider bit position for UART3 */
#define	CLKPWR_PCLKSEL_UART3  		((uint32_t)(50))
/** Peripheral clock divider bit position for I2C2 */
#define	CLKPWR_PCLKSEL_I2C2  		((uint32_t)(52))
/** Peripheral clock divider bit position for I2S */
#define	CLKPWR_PCLKSEL_I2S  		((uint32_t)(54))
/** Peripheral clock divider bit position for RIT */
#define	CLKPWR_PCLKSEL_RIT  		((uint32_t)(58))
/** Peripheral clock divider bit position for SYSCON */
#define	CLKPWR_PCLKSEL_SYSCON  		((uint32_t)(60))
/** Peripheral clock divider bit position for MC */
#define	CLKPWR_PCLKSEL_MC  			((uint32_t)(62))

/** Macro for Peripheral Clock Selection register bit values
 * Note: When CCLK_DIV_8, Peripheral�s clock is selected to
 * PCLK_xyz = CCLK/8 except for CAN1, CAN2, and CAN filtering
 * when �11�selects PCLK_xyz = CCLK/6 */
/* Peripheral clock divider is set to 4 from CCLK */
#define	CLKPWR_PCLKSEL_CCLK_DIV_4  ((uint32_t)(0))
/** Peripheral clock divider is the same with CCLK */
#define	CLKPWR_PCLKSEL_CCLK_DIV_1  ((uint32_t)(1))
/** Peripheral clock divider is set to 2 from CCLK */
#define	CLKPWR_PCLKSEL_CCLK_DIV_2  ((uint32_t)(2))


/********************************************************************
* Power Control for Peripherals Definitions
**********************************************************************/
/** Timer/Counter 0 power/clock control bit */
#define	 CLKPWR_PCONP_PCTIM0	((uint32_t)(1<<1))
/* Timer/Counter 1 power/clock control bit */
#define	 CLKPWR_PCONP_PCTIM1	((uint32_t)(1<<2))
/** UART0 power/clock control bit */
#define	 CLKPWR_PCONP_PCUART0  	((uint32_t)(1<<3))
/** UART1 power/clock control bit */
#define	 CLKPWR_PCONP_PCUART1  	((uint32_t)(1<<4))
/** PWM1 power/clock control bit */
#define	 CLKPWR_PCONP_PCPWM1	((uint32_t)(1<<6))
/** The I2C0 interface power/clock control bit */
#define	 CLKPWR_PCONP_PCI2C0	((uint32_t)(1<<7))
/** The SPI interface power/clock control bit */
#define	 CLKPWR_PCONP_PCSPI  	((uint32_t)(1<<8))
/** The RTC power/clock control bit */
#define	 CLKPWR_PCONP_PCRTC  	((uint32_t)(1<<9))
/** The SSP1 interface power/clock control bit */
#define	 CLKPWR_PCONP_PCSSP1	((uint32_t)(1<<10))
/** A/D converter 0 (ADC0) power/clock control bit */
#define	 CLKPWR_PCONP_PCAD  	((uint32_t)(1<<12))
/** CAN Controller 1 power/clock control bit */
#define	 CLKPWR_PCONP_PCAN1  	((uint32_t)(1<<13))
/** CAN Controller 2 power/clock control bit */
#define	 CLKPWR_PCONP_PCAN2 	((uint32_t)(1<<14))
/** GPIO power/clock control bit */
#define	CLKPWR_PCONP_PCGPIO 	((uint32_t)(1<<15))
/** Repetitive Interrupt Timer power/clock control bit */
#define	CLKPWR_PCONP_PCRIT 		((uint32_t)(1<<16))
/** Motor Control PWM */
#define CLKPWR_PCONP_PCMC 		((uint32_t)(1<<17))
/** Quadrature Encoder Interface power/clock control bit */
#define CLKPWR_PCONP_PCQEI 		((uint32_t)(1<<18))
/** The I2C1 interface power/clock control bit */
#define	 CLKPWR_PCONP_PCI2C1  	((uint32_t)(1<<19))
/** The SSP0 interface power/clock control bit */
#define	 CLKPWR_PCONP_PCSSP0	((uint32_t)(1<<21))
/** Timer 2 power/clock control bit */
#define	 CLKPWR_PCONP_PCTIM2	((uint32_t)(1<<22))
/** Timer 3 power/clock control bit */
#define	 CLKPWR_PCONP_PCTIM3	((uint32_t)(1<<23))
/** UART 2 power/clock control bit */
#define	 CLKPWR_PCONP_PCUART2  	((uint32_t)(1<<24))
/** UART 3 power/clock control bit */
#define	 CLKPWR_PCONP_PCUART3  	((uint32_t)(1<<25))
/** I2C interface 2 power/clock control bit */
#define	 CLKPWR_PCONP_PCI2C2	((uint32_t)(1<<26))
/** I2S interface power/clock control bit*/
#define	 CLKPWR_PCONP_PCI2S  	((uint32_t)(1<<27))
/** GP DMA function power/clock control bit*/
#define	 CLKPWR_PCONP_PCGPDMA  	((uint32_t)(1<<29))
/** Ethernet block power/clock control bit*/
#define	 CLKPWR_PCONP_PCENET	((uint32_t)(1<<30))
/** USB interface power/clock control bit*/
#define	 CLKPWR_PCONP_PCUSB  	((uint32_t)(1<<31))


/**
 * @}
 */
/* Private Macros ------------------------------------------------------------- */
/** @defgroup CLKPWR_Private_Macros CLKPWR Private Macros
 * @{
 */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for Clock Source Select Register
 **********************************************************************/
/** Internal RC oscillator */
#define CLKPWR_CLKSRCSEL_CLKSRC_IRC			((uint32_t)(0x00))
/** Main oscillator */
#define CLKPWR_CLKSRCSEL_CLKSRC_MAINOSC		((uint32_t)(0x01))
/** RTC oscillator */
#define CLKPWR_CLKSRCSEL_CLKSRC_RTC			((uint32_t)(0x02))
/** Clock source selection bit mask */
#define CLKPWR_CLKSRCSEL_BITMASK			((uint32_t)(0x03))

/*********************************************************************//**
 * Macro defines for Clock Output Configuration Register
 **********************************************************************/
/* Clock Output Configuration register definition */
/** Selects the CPU clock as the CLKOUT source */
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_CPU		((uint32_t)(0x00))
/** Selects the main oscillator as the CLKOUT source */
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_MAINOSC	((uint32_t)(0x01))
/** Selects the Internal RC oscillator as the CLKOUT source */
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_RC		((uint32_t)(0x02))
/** Selects the USB clock as the CLKOUT source */
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_USB		((uint32_t)(0x03))
/** Selects the RTC oscillator as the CLKOUT source */
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_RTC		((uint32_t)(0x04))
/** Integer value to divide the output clock by, minus one */
#define CLKPWR_CLKOUTCFG_CLKOUTDIV(n)		((uint32_t)((n&0x0F)<<4))
/** CLKOUT enable control */
#define CLKPWR_CLKOUTCFG_CLKOUT_EN			((uint32_t)(1<<8))
/** CLKOUT activity indication */
#define CLKPWR_CLKOUTCFG_CLKOUT_ACT			((uint32_t)(1<<9))
/** Clock source selection bit mask */
#define CLKPWR_CLKOUTCFG_BITMASK			((uint32_t)(0x3FF))

/*********************************************************************//**
 * Macro defines for PPL0 Control Register
 **********************************************************************/
/** PLL 0 control enable */
#define CLKPWR_PLL0CON_ENABLE		((uint32_t)(0x01))
/** PLL 0 control connect */
#define CLKPWR_PLL0CON_CONNECT		((uint32_t)(0x02))
/** PLL 0 control bit mask */
#define CLKPWR_PLL0CON_BITMASK		((uint32_t)(0x03))

/*********************************************************************//**
 * Macro defines for PPL0 Configuration Register
 **********************************************************************/
/** PLL 0 Configuration MSEL field */
#define CLKPWR_PLL0CFG_MSEL(n)		((uint32_t)(n&0x7FFF))
/** PLL 0 Configuration NSEL field */
#define CLKPWR_PLL0CFG_NSEL(n)		((uint32_t)((n<<16)&0xFF0000))
/** PLL 0 Configuration bit mask */
#define CLKPWR_PLL0CFG_BITMASK		((uint32_t)(0xFF7FFF))


/*********************************************************************//**
 * Macro defines for PPL0 Status Register
 **********************************************************************/
/** PLL 0 MSEL value */
#define CLKPWR_PLL0STAT_MSEL(n)		((uint32_t)(n&0x7FFF))
/** PLL NSEL get value  */
#define CLKPWR_PLL0STAT_NSEL(n)		((uint32_t)((n>>16)&0xFF))
/** PLL status enable bit */
#define CLKPWR_PLL0STAT_PLLE		((uint32_t)(1<<24))
/** PLL status Connect bit */
#define CLKPWR_PLL0STAT_PLLC		((uint32_t)(1<<25))
/** PLL status lock */
#define CLKPWR_PLL0STAT_PLOCK		((uint32_t)(1<<26))

/*********************************************************************//**
 * Macro defines for PPL0 Feed Register
 **********************************************************************/
/** PLL0 Feed bit mask */
#define CLKPWR_PLL0FEED_BITMASK			((uint32_t)0xFF)

/*********************************************************************//**
 * Macro defines for PLL1 Control Register
 **********************************************************************/
/** USB PLL control enable */
#define CLKPWR_PLL1CON_ENABLE		((uint32_t)(0x01))
/** USB PLL control connect */
#define CLKPWR_PLL1CON_CONNECT		((uint32_t)(0x02))
/** USB PLL control bit mask */
#define CLKPWR_PLL1CON_BITMASK		((uint32_t)(0x03))

/*********************************************************************//**
 * Macro defines for PLL1 Configuration Register
 **********************************************************************/
/** USB PLL MSEL set value */
#define CLKPWR_PLL1CFG_MSEL(n)		((uint32_t)(n&0x1F))
/** USB PLL PSEL set value */
#define CLKPWR_PLL1CFG_PSEL(n)		((uint32_t)((n&0x03)<<5))
/** USB PLL configuration bit mask */
#define CLKPWR_PLL1CFG_BITMASK		((uint32_t)(0x7F))

/*********************************************************************//**
 * Macro defines for PLL1 Status Register
 **********************************************************************/
/** USB PLL MSEL get value  */
#define CLKPWR_PLL1STAT_MSEL(n)		((uint32_t)(n&0x1F))
/** USB PLL PSEL get value  */
#define CLKPWR_PLL1STAT_PSEL(n)		((uint32_t)((n>>5)&0x03))
/** USB PLL status enable bit */
#define CLKPWR_PLL1STAT_PLLE		((uint32_t)(1<<8))
/** USB PLL status Connect bit */
#define CLKPWR_PLL1STAT_PLLC		((uint32_t)(1<<9))
/** USB PLL status lock */
#define CLKPWR_PLL1STAT_PLOCK		((uint32_t)(1<<10))

/*********************************************************************//**
 * Macro defines for PLL1 Feed Register
 **********************************************************************/
/** PLL1 Feed bit mask */
#define CLKPWR_PLL1FEED_BITMASK		((uint32_t)0xFF)

/*********************************************************************//**
 * Macro defines for CPU Clock Configuration Register
 **********************************************************************/
/** CPU Clock configuration bit mask */
#define CLKPWR_CCLKCFG_BITMASK		((uint32_t)(0xFF))

/*********************************************************************//**
 * Macro defines for USB Clock Configuration Register
 **********************************************************************/
/** USB Clock Configuration bit mask */
#define CLKPWR_USBCLKCFG_BITMASK	((uint32_t)(0x0F))

/*********************************************************************//**
 * Macro defines for IRC Trim Register
 **********************************************************************/
/** IRC Trim bit mask */
#define CLKPWR_IRCTRIM_BITMASK		((uint32_t)(0x0F))

/*********************************************************************//**
 * Macro defines for Peripheral Clock Selection Register 0 and 1
 **********************************************************************/
/** Peripheral Clock Selection 0 mask bit */
#define CLKPWR_PCLKSEL0_BITMASK		((uint32_t)(0xFFF3F3FF))
/** Peripheral Clock Selection 1 mask bit */
#define CLKPWR_PCLKSEL1_BITMASK		((uint32_t)(0xFCF3F0F3))
/** Macro to set peripheral clock of each type
 * p: position of two bits that hold divider of peripheral clock
 * n: value of divider of peripheral clock  to be set */
#define CLKPWR_PCLKSEL_SET(p,n)		_SBF(p,n)
/** Macro to mask peripheral clock of each type */
#define CLKPWR_PCLKSEL_BITMASK(p)	_SBF(p,0x03)
/** Macro to get peripheral clock of each type */
#define CLKPWR_PCLKSEL_GET(p, n)	((uint32_t)((n>>p)&0x03))

/*********************************************************************//**
 * Macro defines for Power Mode Control Register
 **********************************************************************/
/** Power mode control bit 0 */
#define CLKPWR_PCON_PM0			((uint32_t)(1<<0))
/** Power mode control bit 1 */
#define CLKPWR_PCON_PM1			((uint32_t)(1<<1))
/** Brown-Out Reduced Power Mode */
#define CLKPWR_PCON_BODPDM		((uint32_t)(1<<2))
/** Brown-Out Global Disable */
#define CLKPWR_PCON_BOGD		((uint32_t)(1<<3))
/** Brown Out Reset Disable */
#define CLKPWR_PCON_BORD		((uint32_t)(1<<4))
/** Sleep Mode entry flag */
#define CLKPWR_PCON_SMFLAG		((uint32_t)(1<<8))
/** Deep Sleep entry flag */
#define CLKPWR_PCON_DSFLAG		((uint32_t)(1<<9))
/** Power-down entry flag */
#define CLKPWR_PCON_PDFLAG		((uint32_t)(1<<10))
/** Deep Power-down entry flag */
#define CLKPWR_PCON_DPDFLAG		((uint32_t)(1<<11))

/*********************************************************************//**
 * Macro defines for Power Control for Peripheral Register
 **********************************************************************/
/** Power Control for Peripherals bit mask */
#define CLKPWR_PCONP_BITMASK	0xEFEFF7DE

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup CLKPWR_Public_Functions CLKPWR Public Functions
 * @{
 */

void CLKPWR_SetPCLKDiv (uint32_t ClkType, uint32_t DivVal);
uint32_t CLKPWR_GetPCLKSEL (uint32_t ClkType);
uint32_t CLKPWR_GetPCLK (uint32_t ClkType);
void CLKPWR_ConfigPPWR (uint32_t PPType, FunctionalState NewState);
void CLKPWR_Sleep(void);
void CLKPWR_DeepSleep(void);
void CLKPWR_PowerDown(void);
void CLKPWR_DeepPowerDown(void);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_CLKPWR_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
