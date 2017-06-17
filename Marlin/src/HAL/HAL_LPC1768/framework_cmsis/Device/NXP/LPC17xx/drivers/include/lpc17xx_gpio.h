/**********************************************************************
* $Id$		lpc17xx_gpio.h				2010-06-18
*//**
* @file		lpc17xx_gpio.h
* @brief	Contains all macro definitions and function prototypes
* 			support for GPDMA firmware library on LPC17xx
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
/** @defgroup GPIO GPIO (General Purpose Input/Output)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_GPIO_H_
#define LPC17XX_GPIO_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup GPIO_Public_Macros GPIO Public Macros
 * @{
 */

/** Fast GPIO port 0 byte accessible definition */
#define GPIO0_Byte	((GPIO_Byte_TypeDef *)(LPC_GPIO0_BASE))
/** Fast GPIO port 1 byte accessible definition */
#define GPIO1_Byte	((GPIO_Byte_TypeDef *)(LPC_GPIO1_BASE))
/** Fast GPIO port 2 byte accessible definition */
#define GPIO2_Byte	((GPIO_Byte_TypeDef *)(LPC_GPIO2_BASE))
/** Fast GPIO port 3 byte accessible definition */
#define GPIO3_Byte	((GPIO_Byte_TypeDef *)(LPC_GPIO3_BASE))
/** Fast GPIO port 4 byte accessible definition */
#define GPIO4_Byte	((GPIO_Byte_TypeDef *)(LPC_GPIO4_BASE))


/** Fast GPIO port 0 half-word accessible definition */
#define GPIO0_HalfWord	((GPIO_HalfWord_TypeDef *)(LPC_GPIO0_BASE))
/** Fast GPIO port 1 half-word accessible definition */
#define GPIO1_HalfWord	((GPIO_HalfWord_TypeDef *)(LPC_GPIO1_BASE))
/** Fast GPIO port 2 half-word accessible definition */
#define GPIO2_HalfWord	((GPIO_HalfWord_TypeDef *)(LPC_GPIO2_BASE))
/** Fast GPIO port 3 half-word accessible definition */
#define GPIO3_HalfWord	((GPIO_HalfWord_TypeDef *)(LPC_GPIO3_BASE))
/** Fast GPIO port 4 half-word accessible definition */
#define GPIO4_HalfWord	((GPIO_HalfWord_TypeDef *)(LPC_GPIO4_BASE))

/**
 * @}
 */

/* Public Types --------------------------------------------------------------- */
/** @defgroup GPIO_Public_Types GPIO Public Types
 * @{
 */

/**
 * @brief Fast GPIO port byte type definition
 */
typedef struct {
	__IO uint8_t FIODIR[4];		/**< FIO direction register in byte-align */
	   uint32_t RESERVED0[3];	/**< Reserved */
	__IO uint8_t FIOMASK[4];	/**< FIO mask register in byte-align */
	__IO uint8_t FIOPIN[4];		/**< FIO pin register in byte align */
	__IO uint8_t FIOSET[4];		/**< FIO set register in byte-align */
	__O  uint8_t FIOCLR[4];		/**< FIO clear register in byte-align */
} GPIO_Byte_TypeDef;


/**
 * @brief Fast GPIO port half-word type definition
 */
typedef struct {
	__IO uint16_t FIODIRL;		/**< FIO direction register lower halfword part */
	__IO uint16_t FIODIRU;		/**< FIO direction register upper halfword part */
	   uint32_t RESERVED0[3];	/**< Reserved */
	__IO uint16_t FIOMASKL;		/**< FIO mask register lower halfword part */
	__IO uint16_t FIOMASKU;		/**< FIO mask register upper halfword part */
	__IO uint16_t FIOPINL;		/**< FIO pin register lower halfword part */
	__IO uint16_t FIOPINU;		/**< FIO pin register upper halfword part */
	__IO uint16_t FIOSETL;		/**< FIO set register lower halfword part */
	__IO uint16_t FIOSETU;		/**< FIO set register upper halfword part */
	__O  uint16_t FIOCLRL;		/**< FIO clear register lower halfword part */
	__O  uint16_t FIOCLRU;		/**< FIO clear register upper halfword part */
} GPIO_HalfWord_TypeDef;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup GPIO_Public_Functions GPIO Public Functions
 * @{
 */

/* GPIO style ------------------------------- */
void GPIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir);
void GPIO_SetValue(uint8_t portNum, uint32_t bitValue);
void GPIO_ClearValue(uint8_t portNum, uint32_t bitValue);
uint32_t GPIO_ReadValue(uint8_t portNum);
void GPIO_IntCmd(uint8_t portNum, uint32_t bitValue, uint8_t edgeState);
FunctionalState GPIO_GetIntStatus(uint8_t portNum, uint32_t pinNum, uint8_t edgeState);
void GPIO_ClearInt(uint8_t portNum, uint32_t bitValue);

/* FIO (word-accessible) style ------------------------------- */
void FIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir);
void FIO_SetValue(uint8_t portNum, uint32_t bitValue);
void FIO_ClearValue(uint8_t portNum, uint32_t bitValue);
uint32_t FIO_ReadValue(uint8_t portNum);
void FIO_SetMask(uint8_t portNum, uint32_t bitValue, uint8_t maskValue);
void FIO_IntCmd(uint8_t portNum, uint32_t bitValue, uint8_t edgeState);
FunctionalState FIO_GetIntStatus(uint8_t portNum, uint32_t pinNum, uint8_t edgeState);
void FIO_ClearInt(uint8_t portNum, uint32_t pinNum);

/* FIO (halfword-accessible) style ------------------------------- */
void FIO_HalfWordSetDir(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue, uint8_t dir);
void FIO_HalfWordSetMask(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue, uint8_t maskValue);
void FIO_HalfWordSetValue(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue);
void FIO_HalfWordClearValue(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue);
uint16_t FIO_HalfWordReadValue(uint8_t portNum, uint8_t halfwordNum);

/* FIO (byte-accessible) style ------------------------------- */
void FIO_ByteSetDir(uint8_t portNum, uint8_t byteNum, uint8_t bitValue, uint8_t dir);
void FIO_ByteSetMask(uint8_t portNum, uint8_t byteNum, uint8_t bitValue, uint8_t maskValue);
void FIO_ByteSetValue(uint8_t portNum, uint8_t byteNum, uint8_t bitValue);
void FIO_ByteClearValue(uint8_t portNum, uint8_t byteNum, uint8_t bitValue);
uint8_t FIO_ByteReadValue(uint8_t portNum, uint8_t byteNum);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_GPIO_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
