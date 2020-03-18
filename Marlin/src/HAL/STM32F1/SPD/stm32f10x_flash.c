/**
  ******************************************************************************
  * @file    stm32f10x_flash.c
  * @author  MCD Application Team
  * @version V3.6.1
  * @date    05-March-2012
  * @brief   This file provides all the FLASH firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#if defined(__STM32F1__) && defined(OVERCLOCK)

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_flash.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup FLASH
  * @brief FLASH driver modules
  * @{
  */

/** @defgroup FLASH_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup FLASH_Private_Defines
  * @{
  */

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask         ((uint32_t)0x00000038)
#define ACR_HLFCYA_Mask          ((uint32_t)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((uint32_t)0xFFFFFFEF)

/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((uint32_t)0x00000020)

/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE)
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)

/* FLASH Mask */
#define RDPRT_Mask               ((uint32_t)0x00000002)
#define WRP0_Mask                ((uint32_t)0x000000FF)
#define WRP1_Mask                ((uint32_t)0x0000FF00)
#define WRP2_Mask                ((uint32_t)0x00FF0000)
#define WRP3_Mask                ((uint32_t)0xFF000000)
#define OB_USER_BFB2             ((uint16_t)0x0008)

/* FLASH BANK address */
#define FLASH_BANK1_END_ADDRESS   ((uint32_t)0x807FFFF)

/* Delay definition */
#define EraseTimeout          ((uint32_t)0x000B0000)
#define ProgramTimeout        ((uint32_t)0x00002000)
/**
  * @}
  */

/** @defgroup FLASH_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup FLASH_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup FLASH_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup FLASH_Private_Functions
  * @{
  */

#ifdef ARDUINO_ARCH_STM32F1
static FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);
#endif

/**
@code

 This driver provides functions to configure and program the Flash memory of all STM32F10x devices,
 including the latest STM32F10x_XL density devices.

 STM32F10x_XL devices feature up to 1 Mbyte with dual bank architecture for read-while-write (RWW) capability:
    - bank1: fixed size of 512 Kbytes (256 pages of 2Kbytes each)
    - bank2: up to 512 Kbytes (up to 256 pages of 2Kbytes each)
 While other STM32F10x devices features only one bank with memory up to 512 Kbytes.

 In version V3.3.0, some functions were updated and new ones were added to support
 STM32F10x_XL devices. Thus some functions manages all devices, while other are
 dedicated for XL devices only.

 The table below presents the list of available functions depending on the used STM32F10x devices.

   ***************************************************
   * Legacy functions used for all STM32F10x devices *
   ***************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |STM32F10x_XL|Other STM32F10x|    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_SetLatency                    |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_HalfCycleAccessCmd            |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_PrefetchBufferCmd             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_Unlock                        |    Yes     |      Yes      | - For STM32F10X_XL devices: unlock Bank1 and Bank2.            |
   |                                    |            |               | - For other devices: unlock Bank1 and it is equivalent         |
   |                                    |            |               |   to FLASH_UnlockBank1 function.                               |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_Lock                          |    Yes     |      Yes      | - For STM32F10X_XL devices: lock Bank1 and Bank2.              |
   |                                    |            |               | - For other devices: lock Bank1 and it is equivalent           |
   |                                    |            |               |   to FLASH_LockBank1 function.                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ErasePage                     |    Yes     |      Yes      | - For STM32F10x_XL devices: erase a page in Bank1 and Bank2    |
   |                                    |            |               | - For other devices: erase a page in Bank1                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EraseAllPages                 |    Yes     |      Yes      | - For STM32F10x_XL devices: erase all pages in Bank1 and Bank2 |
   |                                    |            |               | - For other devices: erase all pages in Bank1                  |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EraseOptionBytes              |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramWord                   |    Yes     |      Yes      | Updated to program up to 1MByte (depending on the used device) |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramHalfWord               |    Yes     |      Yes      | Updated to program up to 1MByte (depending on the used device) |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramOptionByteData         |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EnableWriteProtection         |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ReadOutProtection             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_UserOptionByteConfig          |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetUserOptionByte             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetWriteProtectionOptionByte  |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetReadOutProtectionStatus    |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetPrefetchBufferStatus       |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ITConfig                      |    Yes     |      Yes      | - For STM32F10x_XL devices: enable Bank1 and Bank2's interrupts|
   |                                    |            |               | - For other devices: enable Bank1's interrupts                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetFlagStatus                 |    Yes     |      Yes      | - For STM32F10x_XL devices: return Bank1 and Bank2's flag status|
   |                                    |            |               | - For other devices: return Bank1's flag status                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ClearFlag                     |    Yes     |      Yes      | - For STM32F10x_XL devices: clear Bank1 and Bank2's flag       |
   |                                    |            |               | - For other devices: clear Bank1's flag                        |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetStatus                     |    Yes     |      Yes      | - Return the status of Bank1 (for all devices)                 |
   |                                    |            |               |   equivalent to FLASH_GetBank1Status function                  |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_WaitForLastOperation          |    Yes     |      Yes      | - Wait for Bank1 last operation (for all devices)              |
   |                                    |            |               |   equivalent to: FLASH_WaitForLastBank1Operation function      |
   +----------------------------------------------------------------------------------------------------------------------------------+

   ************************************************************************************************************************
   * New functions used for all STM32F10x devices to manage Bank1:                                                        *
   *   - These functions are mainly useful for STM32F10x_XL density devices, to have separate control for Bank1 and bank2 *
   *   - For other devices, these functions are optional (covered by functions listed above)                              *
   ************************************************************************************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |STM32F10x_XL|Other STM32F10x|    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_UnlockBank1                  |    Yes     |      Yes      | - Unlock Bank1                                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_LockBank1                     |    Yes     |      Yes      | - Lock Bank1                                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_EraseAllBank1Pages           |    Yes     |      Yes      | - Erase all pages in Bank1                                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_GetBank1Status               |    Yes     |      Yes      | - Return the status of Bank1                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_WaitForLastBank1Operation    |    Yes     |      Yes      | - Wait for Bank1 last operation                                |
   +----------------------------------------------------------------------------------------------------------------------------------+

   *****************************************************************************
   * New Functions used only with STM32F10x_XL density devices to manage Bank2 *
   *****************************************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |STM32F10x_XL|Other STM32F10x|    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_UnlockBank2                  |    Yes     |      No       | - Unlock Bank2                                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_LockBank2                     |    Yes     |      No       | - Lock Bank2                                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_EraseAllBank2Pages           |    Yes     |      No       | - Erase all pages in Bank2                                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_GetBank2Status               |    Yes     |      No       | - Return the status of Bank2                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_WaitForLastBank2Operation    |    Yes     |      No       | - Wait for Bank2 last operation                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_BootConfig                   |    Yes     |      No       | - Configure to boot from Bank1 or Bank2                        |
   +----------------------------------------------------------------------------------------------------------------------------------+
@endcode
*/


/**
  * @brief  Sets the code latency value.
  * @note   This function can be used for all STM32F10x devices.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *   This parameter can be one of the following values:
  *     @arg FLASH_Latency_0: FLASH Zero Latency cycle
  *     @arg FLASH_Latency_1: FLASH One Latency cycle
  *     @arg FLASH_Latency_2: FLASH Two Latency cycles
  * @retval None
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));

  /* Read the ACR register */
  tmpreg = FLASH->ACR;

  /* Sets the Latency value */
  tmpreg &= ACR_LATENCY_Mask;
  tmpreg |= FLASH_Latency;

  /* Write the ACR register */
  FLASH->ACR = tmpreg;
}

/**
  * @brief  Enables or disables the Half cycle flash access.
  * @note   This function can be used for all STM32F10x devices.
  * @param  FLASH_HalfCycleAccess: specifies the FLASH Half cycle Access mode.
  *   This parameter can be one of the following values:
  *     @arg FLASH_HalfCycleAccess_Enable: FLASH Half Cycle Enable
  *     @arg FLASH_HalfCycleAccess_Disable: FLASH Half Cycle Disable
  * @retval None
  */
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess)
{
  /* Check the parameters */
  assert_param(IS_FLASH_HALFCYCLEACCESS_STATE(FLASH_HalfCycleAccess));

  /* Enable or disable the Half cycle access */
  FLASH->ACR &= ACR_HLFCYA_Mask;
  FLASH->ACR |= FLASH_HalfCycleAccess;
}

/**
  * @brief  Enables or disables the Prefetch Buffer.
  * @note   This function can be used for all STM32F10x devices.
  * @param  FLASH_PrefetchBuffer: specifies the Prefetch buffer status.
  *   This parameter can be one of the following values:
  *     @arg FLASH_PrefetchBuffer_Enable: FLASH Prefetch Buffer Enable
  *     @arg FLASH_PrefetchBuffer_Disable: FLASH Prefetch Buffer Disable
  * @retval None
  */
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer)
{
  /* Check the parameters */
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));

  /* Enable or disable the Prefetch Buffer */
  FLASH->ACR &= ACR_PRFTBE_Mask;
  FLASH->ACR |= FLASH_PrefetchBuffer;
}

/**
  * @brief  Unlocks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function unlocks Bank1 and Bank2.
  *         - For all other devices it unlocks Bank1 and it is equivalent
  *           to FLASH_UnlockBank1 function..
  * @param  None
  * @retval None
  */
#ifndef ARDUINO_ARCH_STM32F1
void FLASH_Unlock(void)
{
  /* Authorize the FPEC of Bank1 Access */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;

#ifdef STM32F10X_XL
  /* Authorize the FPEC of Bank2 Access */
  FLASH->KEYR2 = FLASH_KEY1;
  FLASH->KEYR2 = FLASH_KEY2;
#endif /* STM32F10X_XL */
}
#endif

/**
  * @brief  Unlocks the FLASH Bank1 Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function unlocks Bank1.
  *         - For all other devices it unlocks Bank1 and it is
  *           equivalent to FLASH_Unlock function.
  * @param  None
  * @retval None
  */
void FLASH_UnlockBank1(void)
{
  /* Authorize the FPEC of Bank1 Access */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

#ifdef STM32F10X_XL
/**
  * @brief  Unlocks the FLASH Bank2 Program Erase Controller.
  * @note   This function can be used only for STM32F10X_XL density devices.
  * @param  None
  * @retval None
  */
void FLASH_UnlockBank2(void)
{
  /* Authorize the FPEC of Bank2 Access */
  FLASH->KEYR2 = FLASH_KEY1;
  FLASH->KEYR2 = FLASH_KEY2;
}
#endif /* STM32F10X_XL */

/**
  * @brief  Locks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function Locks Bank1 and Bank2.
  *         - For all other devices it Locks Bank1 and it is equivalent
  *           to FLASH_LockBank1 function.
  * @param  None
  * @retval None
  */
#ifndef ARDUINO_ARCH_STM32F1
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank1 */
  FLASH->CR |= CR_LOCK_Set;

#ifdef STM32F10X_XL
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank2 */
  FLASH->CR2 |= CR_LOCK_Set;
#endif /* STM32F10X_XL */
}
#endif

/**
  * @brief  Locks the FLASH Bank1 Program Erase Controller.
  * @note   this function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function Locks Bank1.
  *         - For all other devices it Locks Bank1 and it is equivalent
  *           to FLASH_Lock function.
  * @param  None
  * @retval None
  */
void FLASH_LockBank1(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank1 */
  FLASH->CR |= CR_LOCK_Set;
}

#ifdef STM32F10X_XL
/**
  * @brief  Locks the FLASH Bank2 Program Erase Controller.
  * @note   This function can be used only for STM32F10X_XL density devices.
  * @param  None
  * @retval None
  */
void FLASH_LockBank2(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank2 */
  FLASH->CR2 |= CR_LOCK_Set;
}
#endif /* STM32F10X_XL */

/**
  * @brief  Erases a specified FLASH page.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
#ifndef ARDUINO_ARCH_STM32F1
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Page_Address));

#ifdef STM32F10X_XL
  if(Page_Address < FLASH_BANK1_END_ADDRESS)
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to erase the page */
      FLASH->CR|= CR_PER_Set;
      FLASH->AR = Page_Address;
      FLASH->CR|= CR_STRT_Set;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank1Operation(EraseTimeout);

      /* Disable the PER Bit */
      FLASH->CR &= CR_PER_Reset;
    }
  }
  else
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to erase the page */
      FLASH->CR2|= CR_PER_Set;
      FLASH->AR2 = Page_Address;
      FLASH->CR2|= CR_STRT_Set;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(EraseTimeout);

      /* Disable the PER Bit */
      FLASH->CR2 &= CR_PER_Reset;
    }
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address;
    FLASH->CR|= CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    /* Disable the PER Bit */
    FLASH->CR &= CR_PER_Reset;
  }
#endif /* STM32F10X_XL */

  /* Return the Erase Status */
  return status;
}
#endif

/**
  * @brief  Erases all FLASH pages.
  * @note   This function can be used for all STM32F10x devices.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;

#ifdef STM32F10X_XL
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastBank1Operation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);

    /* Disable the MER Bit */
    FLASH->CR &= CR_MER_Reset;
  }
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
     FLASH->CR2 |= CR_MER_Set;
     FLASH->CR2 |= CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);

    /* Disable the MER Bit */
    FLASH->CR2 &= CR_MER_Reset;
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    /* Disable the MER Bit */
    FLASH->CR &= CR_MER_Reset;
  }
#endif /* STM32F10X_XL */

  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Erases all Bank1 FLASH pages.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function erases all Bank1 pages.
  *         - For all other devices it erases all Bank1 pages and it is equivalent
  *           to FLASH_EraseAllPages function.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllBank1Pages(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastBank1Operation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);

    /* Disable the MER Bit */
    FLASH->CR &= CR_MER_Reset;
  }
  /* Return the Erase Status */
  return status;
}

#ifdef STM32F10X_XL
/**
  * @brief  Erases all Bank2 FLASH pages.
  * @note   This function can be used only for STM32F10x_XL density devices.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllBank2Pages(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastBank2Operation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
     FLASH->CR2 |= CR_MER_Set;
     FLASH->CR2 |= CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);

    /* Disable the MER Bit */
    FLASH->CR2 &= CR_MER_Reset;
  }
  /* Return the Erase Status */
  return status;
}
#endif /* STM32F10X_XL */

/**
  * @brief  Erases the FLASH option bytes.
  * @note   This functions erases all option bytes except the Read protection (RDP).
  * @note   This function can be used for all STM32F10x devices.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseOptionBytes(void)
{
  uint16_t rdptmp = RDP_Key;

  FLASH_Status status = FLASH_COMPLETE;

  /* Get the actual read protection Option Byte value */
  if(FLASH_GetReadOutProtectionStatus() != RESET)
  {
    rdptmp = 0x00;
  }

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;

    /* if the previous operation is completed, proceed to erase the option bytes */
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit */
      FLASH->CR &= CR_OPTER_Reset;

      /* Enable the Option Bytes Programming operation */
      FLASH->CR |= CR_OPTPG_Set;
      /* Restore the last read protection Option Byte value */
      OB->RDP = (uint16_t)rdptmp;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);

      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if (status != FLASH_TIMEOUT)
      {
        /* Disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
  }
  /* Return the erase status */
  return status;
}

/**
  * @brief  Programs a word at a specified address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  __IO uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

#ifdef STM32F10X_XL
  if(Address < FLASH_BANK1_END_ADDRESS - 2)
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank1Operation(ProgramTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new first
        half word */
      FLASH->CR |= CR_PG_Set;

      *(__IO uint16_t*)Address = (uint16_t)Data;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);

      if(status == FLASH_COMPLETE)
      {
        /* if the previous operation is completed, proceed to program the new second
        half word */
        tmp = Address + 2;

        *(__IO uint16_t*) tmp = Data >> 16;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(ProgramTimeout);

        /* Disable the PG Bit */
        FLASH->CR &= CR_PG_Reset;
      }
      else
      {
        /* Disable the PG Bit */
        FLASH->CR &= CR_PG_Reset;
       }
    }
  }
  else if(Address == (FLASH_BANK1_END_ADDRESS - 1))
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank1Operation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new first
        half word */
      FLASH->CR |= CR_PG_Set;

      *(__IO uint16_t*)Address = (uint16_t)Data;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank1Operation(ProgramTimeout);

	  /* Disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
    else
    {
      /* Disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new second
      half word */
      FLASH->CR2 |= CR_PG_Set;
      tmp = Address + 2;

      *(__IO uint16_t*) tmp = Data >> 16;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

      /* Disable the PG Bit */
      FLASH->CR2 &= CR_PG_Reset;
    }
    else
    {
      /* Disable the PG Bit */
      FLASH->CR2 &= CR_PG_Reset;
    }
  }
  else
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new first
        half word */
      FLASH->CR2 |= CR_PG_Set;

      *(__IO uint16_t*)Address = (uint16_t)Data;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

      if(status == FLASH_COMPLETE)
      {
        /* if the previous operation is completed, proceed to program the new second
        half word */
        tmp = Address + 2;

        *(__IO uint16_t*) tmp = Data >> 16;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

        /* Disable the PG Bit */
        FLASH->CR2 &= CR_PG_Reset;
      }
      else
      {
        /* Disable the PG Bit */
        FLASH->CR2 &= CR_PG_Reset;
      }
    }
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first
    half word */
    FLASH->CR |= CR_PG_Set;

    *(__IO uint16_t*)Address = (uint16_t)Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new second
      half word */
      tmp = Address + 2;

      *(__IO uint16_t*) tmp = Data >> 16;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);

      /* Disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
    else
    {
      /* Disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
  }
#endif /* STM32F10X_XL */

  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a half word at a specified address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
#ifndef ARDUINO_ARCH_STM32F1
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

#ifdef STM32F10X_XL
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(Address < FLASH_BANK1_END_ADDRESS)
  {
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new data */
      FLASH->CR |= CR_PG_Set;

      *(__IO uint16_t*)Address = Data;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank1Operation(ProgramTimeout);

      /* Disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
  }
  else
  {
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new data */
      FLASH->CR2 |= CR_PG_Set;

      *(__IO uint16_t*)Address = Data;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

      /* Disable the PG Bit */
      FLASH->CR2 &= CR_PG_Reset;
    }
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR |= CR_PG_Set;

    *(__IO uint16_t*)Address = Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    /* Disable the PG Bit */
    FLASH->CR &= CR_PG_Reset;
  }
#endif  /* STM32F10X_XL */

  /* Return the Program Status */
  return status;
}
#endif

/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  *   This parameter can be 0x1FFFF804 or 0x1FFFF806.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_OB_DATA_ADDRESS(Address));
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    /* Enables the Option Bytes Programming operation */
    FLASH->CR |= CR_OPTPG_Set;
    *(__IO uint16_t*)Address = Data;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }
  /* Return the Option Byte Data Program Status */
  return status;
}

/**
  * @brief  Write protects the desired pages
  * @note   This function can be used for all STM32F10x devices.
  * @param  FLASH_Pages: specifies the address of the pages to be write protected.
  *   This parameter can be:
  *     @arg For @b STM32_Low-density_devices: value between FLASH_WRProt_Pages0to3 and FLASH_WRProt_Pages28to31
  *     @arg For @b STM32_Medium-density_devices: value between FLASH_WRProt_Pages0to3
  *       and FLASH_WRProt_Pages124to127
  *     @arg For @b STM32_High-density_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to255
  *     @arg For @b STM32_Connectivity_line_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to127
  *     @arg For @b STM32_XL-density_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages60to61 or FLASH_WRProt_Pages62to511
  *     @arg FLASH_WRProt_AllPages
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages)
{
  uint16_t WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF, WRP2_Data = 0xFFFF, WRP3_Data = 0xFFFF;

  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));

  FLASH_Pages = (uint32_t)(~FLASH_Pages);
  WRP0_Data = (uint16_t)(FLASH_Pages & WRP0_Mask);
  WRP1_Data = (uint16_t)((FLASH_Pages & WRP1_Mask) >> 8);
  WRP2_Data = (uint16_t)((FLASH_Pages & WRP2_Mask) >> 16);
  WRP3_Data = (uint16_t)((FLASH_Pages & WRP3_Mask) >> 24);

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTPG_Set;
    if(WRP0_Data != 0xFF)
    {
      OB->WRP0 = WRP0_Data;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP1_Data != 0xFF))
    {
      OB->WRP1 = WRP1_Data;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP2_Data != 0xFF))
    {
      OB->WRP2 = WRP2_Data;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }

    if((status == FLASH_COMPLETE)&& (WRP3_Data != 0xFF))
    {
      OB->WRP3 = WRP3_Data;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }

    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }
  /* Return the write protection operation Status */
  return status;
}

/**
  * @brief  Enables or disables the read out protection.
  * @note   If the user has already programmed the other option bytes before calling
  *   this function, he must re-program them since this function erases all option bytes.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Newstate: new state of the ReadOut Protection.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit */
      FLASH->CR &= CR_OPTER_Reset;
      /* Enable the Option Bytes Programming operation */
      FLASH->CR |= CR_OPTPG_Set;
      if(NewState != DISABLE)
      {
        OB->RDP = 0x00;
      }
      else
      {
        OB->RDP = RDP_Key;
      }
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(EraseTimeout);

      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if(status != FLASH_TIMEOUT)
      {
        /* Disable the OPTER Bit */
        FLASH->CR &= CR_OPTER_Reset;
      }
    }
  }
  /* Return the protection operation Status */
  return status;
}

/**
  * @brief  Programs the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_STDBY.
  * @note   This function can be used for all STM32F10x devices.
  * @param  OB_IWDG: Selects the IWDG mode
  *   This parameter can be one of the following values:
  *     @arg OB_IWDG_SW: Software IWDG selected
  *     @arg OB_IWDG_HW: Hardware IWDG selected
  * @param  OB_STOP: Reset event when entering STOP mode.
  *   This parameter can be one of the following values:
  *     @arg OB_STOP_NoRST: No reset generated when entering in STOP
  *     @arg OB_STOP_RST: Reset generated when entering in STOP
  * @param  OB_STDBY: Reset event when entering Standby mode.
  *   This parameter can be one of the following values:
  *     @arg OB_STDBY_NoRST: No reset generated when entering in STANDBY
  *     @arg OB_STDBY_RST: Reset generated when entering in STANDBY
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* Authorize the small information block programming */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Enable the Option Bytes Programming operation */
    FLASH->CR |= CR_OPTPG_Set;

    OB->USER = OB_IWDG | (uint16_t)(OB_STOP | (uint16_t)(OB_STDBY | ((uint16_t)0xF8)));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }
  /* Return the Option Byte program Status */
  return status;
}

#ifdef STM32F10X_XL
/**
  * @brief  Configures to boot from Bank1 or Bank2.
  * @note   This function can be used only for STM32F10x_XL density devices.
  * @param  FLASH_BOOT: select the FLASH Bank to boot from.
  *   This parameter can be one of the following values:
  *     @arg FLASH_BOOT_Bank1: At startup, if boot pins are set in boot from user Flash
  *        position and this parameter is selected the device will boot from Bank1(Default).
  *     @arg FLASH_BOOT_Bank2: At startup, if boot pins are set in boot from user Flash
  *        position and this parameter is selected the device will boot from Bank2 or Bank1,
  *        depending on the activation of the bank. The active banks are checked in
  *        the following order: Bank2, followed by Bank1.
  *        The active bank is recognized by the value programmed at the base address
  *        of the respective bank (corresponding to the initial stack pointer value
  *        in the interrupt vector table).
  *        For more information, please refer to AN2606 from www.st.com.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_BootConfig(uint16_t FLASH_BOOT)
{
  FLASH_Status status = FLASH_COMPLETE;
  assert_param(IS_FLASH_BOOT(FLASH_BOOT));
  /* Authorize the small information block programming */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Enable the Option Bytes Programming operation */
    FLASH->CR |= CR_OPTPG_Set;

    if(FLASH_BOOT == FLASH_BOOT_Bank1)
    {
      OB->USER |= OB_USER_BFB2;
    }
    else
    {
      OB->USER &= (uint16_t)(~(uint16_t)(OB_USER_BFB2));
    }
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }
  /* Return the Option Byte program Status */
  return status;
}
#endif /* STM32F10X_XL */

/**
  * @brief  Returns the FLASH User Option Bytes values.
  * @note   This function can be used for all STM32F10x devices.
  * @param  None
  * @retval The FLASH User Option Bytes values:IWDG_SW(Bit0), RST_STOP(Bit1)
  *         and RST_STDBY(Bit2).
  */
uint32_t FLASH_GetUserOptionByte(void)
{
  /* Return the User Option Byte */
  return (uint32_t)(FLASH->OBR >> 2);
}

/**
  * @brief  Returns the FLASH Write Protection Option Bytes Register value.
  * @note   This function can be used for all STM32F10x devices.
  * @param  None
  * @retval The FLASH Write Protection  Option Bytes Register value
  */
uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
  /* Return the Flash write protection Register value */
  return (uint32_t)(FLASH->WRPR);
}

/**
  * @brief  Checks whether the FLASH Read Out Protection Status is set or not.
  * @note   This function can be used for all STM32F10x devices.
  * @param  None
  * @retval FLASH ReadOut Protection Status(SET or RESET)
  */
FlagStatus FLASH_GetReadOutProtectionStatus(void)
{
  FlagStatus readoutstatus = RESET;
  if ((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  return readoutstatus;
}

/**
  * @brief  Checks whether the FLASH Prefetch Buffer status is set or not.
  * @note   This function can be used for all STM32F10x devices.
  * @param  None
  * @retval FLASH Prefetch Buffer Status (SET or RESET).
  */
FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
  FlagStatus bitstatus = RESET;

  if ((FLASH->ACR & ACR_PRFTBS_Mask) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the new state of FLASH Prefetch Buffer Status (SET or RESET) */
  return bitstatus;
}

/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices, enables or disables the specified FLASH interrupts
              for Bank1 and Bank2.
  *         - For other devices it enables or disables the specified FLASH interrupts for Bank1.
  * @param  FLASH_IT: specifies the FLASH interrupt sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_IT_ERROR: FLASH Error Interrupt
  *     @arg FLASH_IT_EOP: FLASH end of operation Interrupt
  * @param  NewState: new state of the specified Flash interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
#ifdef STM32F10X_XL
  /* Check the parameters */
  assert_param(IS_FLASH_IT(FLASH_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if((FLASH_IT & 0x80000000) != 0x0)
  {
    if(NewState != DISABLE)
    {
      /* Enable the interrupt sources */
      FLASH->CR2 |= (FLASH_IT & 0x7FFFFFFF);
    }
    else
    {
      /* Disable the interrupt sources */
      FLASH->CR2 &= ~(uint32_t)(FLASH_IT & 0x7FFFFFFF);
    }
  }
  else
  {
    if(NewState != DISABLE)
    {
      /* Enable the interrupt sources */
      FLASH->CR |= FLASH_IT;
    }
    else
    {
      /* Disable the interrupt sources */
      FLASH->CR &= ~(uint32_t)FLASH_IT;
    }
  }
#else
  /* Check the parameters */
  assert_param(IS_FLASH_IT(FLASH_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {
    /* Enable the interrupt sources */
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    /* Disable the interrupt sources */
    FLASH->CR &= ~(uint32_t)FLASH_IT;
  }
#endif /* STM32F10X_XL */
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices, this function checks whether the specified
  *           Bank1 or Bank2 flag is set or not.
  *         - For other devices, it checks whether the specified Bank1 flag is
  *           set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *     @arg FLASH_FLAG_BSY: FLASH Busy flag
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  *     @arg FLASH_FLAG_OPTERR:  FLASH Option Byte error flag
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;

#ifdef STM32F10X_XL
  /* Check the parameters */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
  if(FLASH_FLAG == FLASH_FLAG_OPTERR)
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
    if((FLASH_FLAG & 0x80000000) != 0x0)
    {
      if((FLASH->SR2 & FLASH_FLAG) != (uint32_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }
    }
    else
    {
      if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }
    }
  }
#else
  /* Check the parameters */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
  if(FLASH_FLAG == FLASH_FLAG_OPTERR)
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
   if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
#endif /* STM32F10X_XL */

  /* Return the new state of FLASH_FLAG (SET or RESET) */
  return bitstatus;
}

/**
  * @brief  Clears the FLASH's pending flags.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices, this function clears Bank1 or Bank2’s pending flags
  *         - For other devices, it clears Bank1’s pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
#ifdef STM32F10X_XL
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;

  if((FLASH_FLAG & 0x80000000) != 0x0)
  {
    /* Clear the flags */
    FLASH->SR2 = FLASH_FLAG;
  }
  else
  {
    /* Clear the flags */
    FLASH->SR = FLASH_FLAG;
  }

#else
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;

  /* Clear the flags */
  FLASH->SR = FLASH_FLAG;
#endif /* STM32F10X_XL */
}

/**
  * @brief  Returns the FLASH Status.
  * @note   This function can be used for all STM32F10x devices, it is equivalent
  *         to FLASH_GetBank1Status function.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP or FLASH_COMPLETE
  */
#ifndef ARDUINO_ARCH_STM32F1
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;

  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
  {
    flashstatus = FLASH_BUSY;
  }
  else
  {
    if((FLASH->SR & FLASH_FLAG_PGERR) != 0)
    {
      flashstatus = FLASH_ERROR_PG;
    }
    else
    {
      if((FLASH->SR & FLASH_FLAG_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}
#endif

/**
  * @brief  Returns the FLASH Bank1 Status.
  * @note   This function can be used for all STM32F10x devices, it is equivalent
  *         to FLASH_GetStatus function.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP or FLASH_COMPLETE
  */
FLASH_Status FLASH_GetBank1Status(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;

  if((FLASH->SR & FLASH_FLAG_BANK1_BSY) == FLASH_FLAG_BSY)
  {
    flashstatus = FLASH_BUSY;
  }
  else
  {
    if((FLASH->SR & FLASH_FLAG_BANK1_PGERR) != 0)
    {
      flashstatus = FLASH_ERROR_PG;
    }
    else
    {
      if((FLASH->SR & FLASH_FLAG_BANK1_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}

#ifdef STM32F10X_XL
/**
  * @brief  Returns the FLASH Bank2 Status.
  * @note   This function can be used for STM32F10x_XL density devices.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *        FLASH_ERROR_WRP or FLASH_COMPLETE
  */
FLASH_Status FLASH_GetBank2Status(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;

  if((FLASH->SR2 & (FLASH_FLAG_BANK2_BSY & 0x7FFFFFFF)) == (FLASH_FLAG_BANK2_BSY & 0x7FFFFFFF))
  {
    flashstatus = FLASH_BUSY;
  }
  else
  {
    if((FLASH->SR2 & (FLASH_FLAG_BANK2_PGERR & 0x7FFFFFFF)) != 0)
    {
      flashstatus = FLASH_ERROR_PG;
    }
    else
    {
      if((FLASH->SR2 & (FLASH_FLAG_BANK2_WRPRTERR & 0x7FFFFFFF)) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}
#endif /* STM32F10X_XL */

/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @note   This function can be used for all STM32F10x devices,
  *         it is equivalent to FLASH_WaitForLastBank1Operation.
  *         - For STM32F10X_XL devices this function waits for a Bank1 Flash operation
  *           to complete or a TIMEOUT to occur.
  *         - For all other devices it waits for a Flash operation to complete
  *           or a TIMEOUT to occur.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
#ifdef ARDUINO_ARCH_STM32F1
static
#endif
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check for the Flash Status */
  status = FLASH_GetBank1Status();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetBank1Status();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}

/**
  * @brief  Waits for a Flash operation on Bank1 to complete or a TIMEOUT to occur.
  * @note   This function can be used for all STM32F10x devices,
  *         it is equivalent to FLASH_WaitForLastOperation.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastBank1Operation(uint32_t Timeout)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check for the Flash Status */
  status = FLASH_GetBank1Status();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == FLASH_FLAG_BANK1_BSY) && (Timeout != 0x00))
  {
    status = FLASH_GetBank1Status();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}

#ifdef STM32F10X_XL
/**
  * @brief  Waits for a Flash operation on Bank2 to complete or a TIMEOUT to occur.
  * @note   This function can be used only for STM32F10x_XL density devices.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastBank2Operation(uint32_t Timeout)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check for the Flash Status */
  status = FLASH_GetBank2Status();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == (FLASH_FLAG_BANK2_BSY & 0x7FFFFFFF)) && (Timeout != 0x00))
  {
    status = FLASH_GetBank2Status();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}
#endif /* STM32F10X_XL */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __STM32F1__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
