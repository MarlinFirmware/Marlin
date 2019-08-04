/**
  ******************************************************************************
  * @file    EEPROM/EEPROM_Emulation/src/eeprom.c
  * @author  MCD Application Team
  * @version V1.2.6
  * @date    04-November-2016
  * @brief   This file provides all the EEPROM emulation firmware functions.
  ******************************************************************************
  * @attention
  *
  * Copyright © 2016 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/** @addtogroup EEPROM_Emulation
  * @{
  */
#if defined(STM32GENERIC) && (defined(STM32F4) || defined(STM32F7))

/* Includes ------------------------------------------------------------------*/
#include "eeprom_emul.h"

/* Private variables ---------------------------------------------------------*/

/* Global variable used to store variable value in read sequence */
uint16_t DataVar = 0;

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static HAL_StatusTypeDef EE_Format(void);
static uint16_t EE_FindValidPage(uint8_t Operation);
static uint16_t EE_VerifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data);
static uint16_t EE_PageTransfer(uint16_t VirtAddress, uint16_t Data);
static uint16_t EE_VerifyPageFullyErased(uint32_t Address);

 /**
  * @brief  Restore the pages to a known good state in case of page's status
  *   corruption after a power loss.
  * @param  None.
  * @retval - Flash error code: on write Flash error
  *         - FLASH_COMPLETE: on success
  */
uint16_t EE_Initialize(void) {
  /* Get Page0 and Page1 status */
  uint16_t PageStatus0 = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS),
           PageStatus1 = (*(__IO uint16_t*)PAGE1_BASE_ADDRESS);

  FLASH_EraseInitTypeDef pEraseInit;
  pEraseInit.TypeErase = TYPEERASE_SECTORS;
  pEraseInit.Sector = PAGE0_ID;
  pEraseInit.NbSectors = 1;
  pEraseInit.VoltageRange = VOLTAGE_RANGE;

  /* Check for invalid header states and repair if necessary */
  uint32_t SectorError;
  switch (PageStatus0) {
    case ERASED:
      if (PageStatus1 == VALID_PAGE) { /* Page0 erased, Page1 valid */
        /* Erase Page0 */
        if (!EE_VerifyPageFullyErased(PAGE0_BASE_ADDRESS)) {
          /* As the last operation, simply return the result */
          return HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
        }
      }
      else if (PageStatus1 == RECEIVE_DATA) { /* Page0 erased, Page1 receive */
        /* Erase Page0 */
        if (!EE_VerifyPageFullyErased(PAGE0_BASE_ADDRESS)) {
          HAL_StatusTypeDef fStat = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
          /* If erase operation was failed, a Flash error code is returned */
          if (fStat != HAL_OK) return fStat;
        }
        /* Mark Page1 as valid */
        /* As the last operation, simply return the result */
        return HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, PAGE1_BASE_ADDRESS, VALID_PAGE);
      }
      else { /* First EEPROM access (Page0&1 are erased) or invalid state -> format EEPROM */
        /* Erase both Page0 and Page1 and set Page0 as valid page */
        /* As the last operation, simply return the result */
        return EE_Format();
      }
      break;

    case RECEIVE_DATA:
      if (PageStatus1 == VALID_PAGE) { /* Page0 receive, Page1 valid */
        /* Transfer data from Page1 to Page0 */
        int16_t x = -1;
        for (uint16_t VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++) {
          if (( *(__IO uint16_t*)(PAGE0_BASE_ADDRESS + 6)) == VirtAddVarTab[VarIdx])
            x = VarIdx;
          if (VarIdx != x) {
            /* Read the last variables' updates */
            uint16_t ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
            /* In case variable corresponding to the virtual address was found */
            if (ReadStatus != 0x1) {
              /* Transfer the variable to the Page0 */
              uint16_t EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
              /* If program operation was failed, a Flash error code is returned */
              if (EepromStatus != HAL_OK) return EepromStatus;
            }
          }
        }
        /* Mark Page0 as valid */
        HAL_StatusTypeDef FlashStatus = HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, PAGE0_BASE_ADDRESS, VALID_PAGE);
        /* If program operation was failed, a Flash error code is returned */
        if (FlashStatus != HAL_OK) return FlashStatus;
        pEraseInit.Sector = PAGE1_ID;
        pEraseInit.NbSectors = 1;
        pEraseInit.VoltageRange = VOLTAGE_RANGE;
        /* Erase Page1 */
        if (!EE_VerifyPageFullyErased(PAGE1_BASE_ADDRESS)) {
          /* As the last operation, simply return the result */
          return HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
        }
      }
      else if (PageStatus1 == ERASED) { /* Page0 receive, Page1 erased */
        pEraseInit.Sector = PAGE1_ID;
        pEraseInit.NbSectors = 1;
        pEraseInit.VoltageRange = VOLTAGE_RANGE;
        /* Erase Page1 */
        if (!EE_VerifyPageFullyErased(PAGE1_BASE_ADDRESS)) {
          HAL_StatusTypeDef fStat = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
          /* If erase operation was failed, a Flash error code is returned */
          if (fStat != HAL_OK) return fStat;
        }
        /* Mark Page0 as valid */
        /* As the last operation, simply return the result */
        return HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, PAGE0_BASE_ADDRESS, VALID_PAGE);
      }
      else { /* Invalid state -> format eeprom */
        /* Erase both Page0 and Page1 and set Page0 as valid page */
        /* As the last operation, simply return the result */
        return EE_Format();
      }
      break;

    case VALID_PAGE:
      if (PageStatus1 == VALID_PAGE) { /* Invalid state -> format eeprom */
        /* Erase both Page0 and Page1 and set Page0 as valid page */
        FlashStatus = EE_Format();
        /* If erase/program operation was failed, a Flash error code is returned */
        if (FlashStatus != HAL_OK) return FlashStatus;
      }
      else if (PageStatus1 == ERASED) { /* Page0 valid, Page1 erased */
        pEraseInit.Sector = PAGE1_ID;
        pEraseInit.NbSectors = 1;
        pEraseInit.VoltageRange = VOLTAGE_RANGE;
        /* Erase Page1 */
        if (!EE_VerifyPageFullyErased(PAGE1_BASE_ADDRESS)) {
          FlashStatus = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
          /* If erase operation was failed, a Flash error code is returned */
          if (FlashStatus != HAL_OK) return FlashStatus;
        }
      }
      else { /* Page0 valid, Page1 receive */
        /* Transfer data from Page0 to Page1 */
        int16_t x = -1;
        for (uint16_t VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++) {
          if ((*(__IO uint16_t*)(PAGE1_BASE_ADDRESS + 6)) == VirtAddVarTab[VarIdx])
            x = VarIdx;

          if (VarIdx != x) {
            /* Read the last variables' updates */
            uint16_t ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
            /* In case variable corresponding to the virtual address was found */
            if (ReadStatus != 0x1) {
              /* Transfer the variable to the Page1 */
              uint16_t EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
              /* If program operation was failed, a Flash error code is returned */
              if (EepromStatus != HAL_OK) return EepromStatus;
            }
          }
        }
        /* Mark Page1 as valid */
        FlashStatus = HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, PAGE1_BASE_ADDRESS, VALID_PAGE);
        /* If program operation was failed, a Flash error code is returned */
        if (FlashStatus != HAL_OK) return FlashStatus;
        pEraseInit.Sector = PAGE0_ID;
        pEraseInit.NbSectors = 1;
        pEraseInit.VoltageRange = VOLTAGE_RANGE;
        /* Erase Page0 */
        if (!EE_VerifyPageFullyErased(PAGE0_BASE_ADDRESS)) {
          /* As the last operation, simply return the result */
          return HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
        }
      }
      break;

    default:  /* Any other state -> format eeprom */
      /* Erase both Page0 and Page1 and set Page0 as valid page */
      /* As the last operation, simply return the result */
      return EE_Format();
  }

  return HAL_OK;
}

/**
 * @brief  Verify if specified page is fully erased.
 * @param  Address: page address
 *   This parameter can be one of the following values:
 *     @arg PAGE0_BASE_ADDRESS: Page0 base address
 *     @arg PAGE1_BASE_ADDRESS: Page1 base address
 * @retval page fully erased status:
 *           - 0: if Page not erased
 *           - 1: if Page erased
 */
uint16_t EE_VerifyPageFullyErased(uint32_t Address) {
  uint32_t ReadStatus = 1;
  /* Check each active page address starting from end */
  while (Address <= PAGE0_END_ADDRESS) {
    /* Get the current location content to be compared with virtual address */
    uint16_t AddressValue = (*(__IO uint16_t*)Address);
    /* Compare the read address with the virtual address */
    if (AddressValue != ERASED) {
      /* In case variable value is read, reset ReadStatus flag */
      ReadStatus = 0;
      break;
    }
    /* Next address location */
    Address += 4;
  }
  /* Return ReadStatus value: (0: Page not erased, 1: Sector erased) */
  return ReadStatus;
}

/**
 * @brief  Returns the last stored variable data, if found, which correspond to
 *   the passed virtual address
 * @param  VirtAddress: Variable virtual address
 * @param  Data: Global variable contains the read variable value
 * @retval Success or error status:
 *           - 0: if variable was found
 *           - 1: if the variable was not found
 *           - NO_VALID_PAGE: if no valid page was found.
 */
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data) {
  uint16_t ReadStatus = 1;

  /* Get active Page for read operation */
  uint16_t ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

  /* Check if there is no valid page */
  if (ValidPage == NO_VALID_PAGE) return NO_VALID_PAGE;

  /* Get the valid Page start and end Addresses */
  uint32_t PageStartAddress = uint32_t(EEPROM_START_ADDRESS) + uint32_t(ValidPage * (PAGE_SIZE)),
           Address = PageStartAddress + PAGE_SIZE - 2;

  /* Check each active page address starting from end */
  while (Address > PageStartAddress + 2) {
    /* Get the current location content to be compared with virtual address */
    uint16_t AddressValue = (*(__IO uint16_t*)Address);

    /* Compare the read address with the virtual address */
    if (AddressValue == VirtAddress) {
      /* Get content of Address-2 which is variable value */
      *Data = (*(__IO uint16_t*)(Address - 2));
      /* In case variable value is read, reset ReadStatus flag */
      ReadStatus = 0;
      break;
    }
    else /* Next address location */
      Address -= 4;
  }
  /* Return ReadStatus value: (0: variable exist, 1: variable doesn't exist) */
  return ReadStatus;
}

/**
 * @brief  Writes/upadtes variable data in EEPROM.
 * @param  VirtAddress: Variable virtual address
 * @param  Data: 16 bit data to be written
 * @retval Success or error status:
 *           - FLASH_COMPLETE: on success
 *           - PAGE_FULL: if valid page is full
 *           - NO_VALID_PAGE: if no valid page was found
 *           - Flash error code: on write Flash error
 */
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data) {
  /* Write the variable virtual address and value in the EEPROM */
  uint16_t Status = EE_VerifyPageFullWriteVariable(VirtAddress, Data);

  /* In case the EEPROM active page is full */
  if (Status == PAGE_FULL) /* Perform Page transfer */
    Status = EE_PageTransfer(VirtAddress, Data);

  /* Return last operation status */
  return Status;
}

/**
 * @brief  Erases PAGE and PAGE1 and writes VALID_PAGE header to PAGE
 * @param  None
 * @retval Status of the last operation (Flash write or erase) done during
 *         EEPROM formating
 */
static HAL_StatusTypeDef EE_Format(void) {
  FLASH_EraseInitTypeDef pEraseInit;
  pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
  pEraseInit.Sector = PAGE0_ID;
  pEraseInit.NbSectors = 1;
  pEraseInit.VoltageRange = VOLTAGE_RANGE;

  HAL_StatusTypeDef FlashStatus; // = HAL_OK

  /* Erase Page0 */
  if (!EE_VerifyPageFullyErased(PAGE0_BASE_ADDRESS)) {
    uint32_t SectorError;
    FlashStatus = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
    /* If erase operation was failed, a Flash error code is returned */
    if (FlashStatus != HAL_OK) return FlashStatus;
  }
  /* Set Page0 as valid page: Write VALID_PAGE at Page0 base address */
  FlashStatus = HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, PAGE0_BASE_ADDRESS, VALID_PAGE);
  /* If program operation was failed, a Flash error code is returned */
  if (FlashStatus != HAL_OK) return FlashStatus;

  pEraseInit.Sector = PAGE1_ID;
  /* Erase Page1 */
  if (!EE_VerifyPageFullyErased(PAGE1_BASE_ADDRESS)) {
    /* As the last operation, just return the result code */
    uint32_t SectorError;
    return HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
  }

  return HAL_OK;
}

/**
 * @brief  Find valid Page for write or read operation
 * @param  Operation: operation to achieve on the valid page.
 *   This parameter can be one of the following values:
 *     @arg READ_FROM_VALID_PAGE: read operation from valid page
 *     @arg WRITE_IN_VALID_PAGE: write operation from valid page
 * @retval Valid page number (PAGE or PAGE1) or NO_VALID_PAGE in case
 *   of no valid page was found
 */
static uint16_t EE_FindValidPage(uint8_t Operation) {
  /* Get Page0 and Page1 actual status */
  uint16_t PageStatus0 = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS),
           PageStatus1 = (*(__IO uint16_t*)PAGE1_BASE_ADDRESS);

  /* Write or read operation */
  switch (Operation) {
    case WRITE_IN_VALID_PAGE:   /* ---- Write operation ---- */
      if (PageStatus1 == VALID_PAGE) {
        /* Page0 receiving data */
        return (PageStatus0 == RECEIVE_DATA) ? PAGE0 : PAGE1;
      }
      else if (PageStatus0 == VALID_PAGE) {
        /* Page1 receiving data */
        return (PageStatus1 == RECEIVE_DATA) ? PAGE1 : PAGE0;
      }
      else
        return NO_VALID_PAGE;   /* No valid Page */

    case READ_FROM_VALID_PAGE:  /* ---- Read operation ---- */
      if (PageStatus0 == VALID_PAGE)
        return PAGE0;           /* Page0 valid */
      else if (PageStatus1 == VALID_PAGE)
        return PAGE1;           /* Page1 valid */
      else
        return NO_VALID_PAGE;   /* No valid Page */

    default:
      return PAGE0;             /* Page0 valid */
  }
}

/**
 * @brief  Verify if active page is full and Writes variable in EEPROM.
 * @param  VirtAddress: 16 bit virtual address of the variable
 * @param  Data: 16 bit data to be written as variable value
 * @retval Success or error status:
 *           - FLASH_COMPLETE: on success
 *           - PAGE_FULL: if valid page is full
 *           - NO_VALID_PAGE: if no valid page was found
 *           - Flash error code: on write Flash error
 */
static uint16_t EE_VerifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data) {
  /* Get valid Page for write operation */
  uint16_t ValidPage = EE_FindValidPage(WRITE_IN_VALID_PAGE);

  /* Check if there is no valid page */
  if (ValidPage == NO_VALID_PAGE) return NO_VALID_PAGE;

  /* Get the valid Page start and end Addresses */
  uint32_t Address = uint32_t(EEPROM_START_ADDRESS) + uint32_t(ValidPage * (PAGE_SIZE)),
           PageEndAddress = Address + PAGE_SIZE - 1;

  /* Check each active page address starting from begining */
  while (Address < PageEndAddress) {
    /* Verify if Address and Address+2 contents are 0xFFFFFFFF */
    if ((*(__IO uint32_t*)Address) == 0xFFFFFFFF) {
      /* Set variable data */
      HAL_StatusTypeDef FlashStatus = HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, Address, Data);
      /* If program operation was failed, a Flash error code is returned */
      if (FlashStatus != HAL_OK) return FlashStatus;
      /* Set variable virtual address, return status */
      return HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, Address + 2, VirtAddress);
    }
    else /* Next address location */
      Address += 4;
  }

  /* Return PAGE_FULL in case the valid page is full */
  return PAGE_FULL;
}

/**
 * @brief  Transfers last updated variables data from the full Page to
 *   an empty one.
 * @param  VirtAddress: 16 bit virtual address of the variable
 * @param  Data: 16 bit data to be written as variable value
 * @retval Success or error status:
 *           - FLASH_COMPLETE: on success
 *           - PAGE_FULL: if valid page is full
 *           - NO_VALID_PAGE: if no valid page was found
 *           - Flash error code: on write Flash error
 */
static uint16_t EE_PageTransfer(uint16_t VirtAddress, uint16_t Data) {
  /* Get active Page for read operation */
  uint16_t ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);
  uint32_t NewPageAddress = EEPROM_START_ADDRESS;
  uint16_t OldPageId = 0;

  if (ValidPage == PAGE1) {     /* Page1 valid */
    /* New page address where variable will be moved to */
    NewPageAddress = PAGE0_BASE_ADDRESS;
    /* Old page ID where variable will be taken from */
    OldPageId = PAGE1_ID;
  }
  else if (ValidPage == PAGE0) { /* Page0 valid */
    /* New page address  where variable will be moved to */
    NewPageAddress = PAGE1_BASE_ADDRESS;
    /* Old page ID where variable will be taken from */
    OldPageId = PAGE0_ID;
  }
  else
    return NO_VALID_PAGE;       /* No valid Page */

  /* Set the new Page status to RECEIVE_DATA status */
  HAL_StatusTypeDef FlashStatus = HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, NewPageAddress, RECEIVE_DATA);
  /* If program operation was failed, a Flash error code is returned */
  if (FlashStatus != HAL_OK) return FlashStatus;

  /* Write the variable passed as parameter in the new active page */
  uint16_t EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddress, Data);
  /* If program operation was failed, a Flash error code is returned */
  if (EepromStatus != HAL_OK) return EepromStatus;

  /* Transfer process: transfer variables from old to the new active page */
  for (uint16_t VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++) {
    if (VirtAddVarTab[VarIdx] != VirtAddress) { /* Check each variable except the one passed as parameter */
      /* Read the other last variable updates */
      uint16_t ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
      /* In case variable corresponding to the virtual address was found */
      if (ReadStatus != 0x1) {
        /* Transfer the variable to the new active page */
        EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
        /* If program operation was failed, a Flash error code is returned */
        if (EepromStatus != HAL_OK) return EepromStatus;
      }
    }
  }

  FLASH_EraseInitTypeDef pEraseInit;
  pEraseInit.TypeErase = TYPEERASE_SECTORS;
  pEraseInit.Sector = OldPageId;
  pEraseInit.NbSectors = 1;
  pEraseInit.VoltageRange = VOLTAGE_RANGE;

  /* Erase the old Page: Set old Page status to ERASED status */
  uint32_t SectorError;
  FlashStatus = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
  /* If erase operation was failed, a Flash error code is returned */
  if (FlashStatus != HAL_OK) return FlashStatus;

  /* Set new Page status to VALID_PAGE status */
  /* As the last operation, just return the result code */
  return HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, NewPageAddress, VALID_PAGE);
}

#endif // STM32GENERIC && (STM32F4 || STM32F7)

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
