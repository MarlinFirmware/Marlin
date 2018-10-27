/**********************************************************************
* $Id$		lpc17xx_iap.h			2012-04-18
*//**
* @file		lpc17xx_iap.h
* @brief	Contains all functions support for IAP
*			on lpc17xx
* @version	1.0
* @date		18. April. 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
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

#ifndef _LPC17xx_IAP_H
#define _LPC17xx_IAP_H
#include "lpc_types.h"

/** @defgroup IAP  	IAP (In Application Programming)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

/** @defgroup IAP_Public_Macros IAP Public Macros
 * @{
 */

/** IAP entry location */
#define IAP_LOCATION              (0x1FFF1FF1UL)

/**
 * @}
 */

/** @defgroup IAP_Public_Types IAP Public Types
 * @{
 */

/**
 * @brief IAP command code definitions
 */
typedef enum
{
    IAP_PREPARE = 50,       // Prepare sector(s) for write operation
    IAP_COPY_RAM2FLASH = 51,     // Copy RAM to Flash
    IAP_ERASE = 52,              // Erase sector(s)
    IAP_BLANK_CHECK = 53,        // Blank check sector(s)
    IAP_READ_PART_ID = 54,       // Read chip part ID
    IAP_READ_BOOT_VER = 55,      // Read chip boot code version
    IAP_COMPARE = 56,            // Compare memory areas
    IAP_REINVOKE_ISP = 57,       // Reinvoke ISP
    IAP_READ_SERIAL_NUMBER = 58, // Read serial number
}  IAP_COMMAND_CODE;

/**
 * @brief IAP status code definitions
 */
typedef enum
{
    CMD_SUCCESS,	             // Command is executed successfully.
    INVALID_COMMAND,             // Invalid command.
    SRC_ADDR_ERROR,              // Source address is not on a word boundary.
    DST_ADDR_ERROR,              // Destination address is not on a correct boundary.
    SRC_ADDR_NOT_MAPPED,         // Source address is not mapped in the memory map.
    DST_ADDR_NOT_MAPPED,         // Destination address is not mapped in the memory map.
    COUNT_ERROR,	               // Byte count is not multiple of 4 or is not a permitted value.
    INVALID_SECTOR,	           // Sector number is invalid.
    SECTOR_NOT_BLANK,	           // Sector is not blank.
    SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION,	// Command to prepare sector for write operation was not executed.
    COMPARE_ERROR,               // Source and destination data is not same.
    BUSY,		                   // Flash programming hardware interface is busy.
} IAP_STATUS_CODE;

/**
 * @brief IAP write length definitions
 */
typedef enum {
  IAP_WRITE_256  = 256,
  IAP_WRITE_512  = 512,
  IAP_WRITE_1024 = 1024,
  IAP_WRITE_4096 = 4096,
} IAP_WRITE_SIZE;

/**
 * @brief IAP command structure
 */
typedef struct {
    uint32_t cmd;   // Command
    uint32_t param[4];      // Parameters
    uint32_t status;        // status code
    uint32_t result[4];     // Result
} IAP_COMMAND_Type;

/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup IAP_Public_Functions IAP Public Functions
 * @{
 */

/**  Get sector number of an address */
uint32_t GetSecNum (uint32_t adr);
/**  Prepare sector(s) for write operation */
IAP_STATUS_CODE PrepareSector(uint32_t start_sec, uint32_t end_sec);
/**  Copy RAM to Flash */
IAP_STATUS_CODE CopyRAM2Flash(uint8_t * dest, uint8_t* source, IAP_WRITE_SIZE size);
/**  Prepare sector(s) for write operation */
IAP_STATUS_CODE EraseSector(uint32_t start_sec, uint32_t end_sec);
/**  Blank check sectors */
IAP_STATUS_CODE BlankCheckSector(uint32_t start_sec, uint32_t end_sec,
                                 uint32_t *first_nblank_loc,
								 uint32_t *first_nblank_val);
/**  Read part identification number */
IAP_STATUS_CODE ReadPartID(uint32_t *partID);
/**  Read boot code version */
IAP_STATUS_CODE ReadBootCodeVer(uint8_t *major, uint8_t* minor);
/**  Read Device serial number */
IAP_STATUS_CODE ReadDeviceSerialNum(uint32_t *uid);
/**  Compare memory */
IAP_STATUS_CODE Compare(uint8_t *addr1, uint8_t *addr2, uint32_t size);
/**  Invoke ISP */
void InvokeISP(void);

/**
 * @}
 */

/**
 * @}
 */

#endif /*_LPC17xx_IAP_H*/

