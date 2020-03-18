/**
  ******************************************************************************
  * @file    stm32f10x_flash.h
  * @author  MCD Application Team
  * @version V3.6.1
  * @date    05-March-2012
  * @brief   This file contains all the functions prototypes for the FLASH
  *          firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_FLASH_H
#define __STM32F10x_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/** @defgroup FLASH_Exported_Types
  * @{
  */

/**
  * @brief  FLASH Status (updated with stm32duino flash_stm32.h values)
  */

typedef enum {
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_ERROR_OPT,
  FLASH_COMPLETE,
  FLASH_TIMEOUT,
  FLASH_BAD_ADDRESS
} FLASH_Status;

/**
  * @}
  */

/** @defgroup FLASH_Exported_Constants
  * @{
  */

/** @defgroup Flash_Latency
  * @{
  */

#define FLASH_Latency_0                ((uint32_t)0x00000000)  /*!< FLASH Zero Latency cycle */
#define FLASH_Latency_1                ((uint32_t)0x00000001)  /*!< FLASH One Latency cycle */
#define FLASH_Latency_2                ((uint32_t)0x00000002)  /*!< FLASH Two Latency cycles */
#define IS_FLASH_LATENCY(LATENCY) (((LATENCY) == FLASH_Latency_0) || \
                                   ((LATENCY) == FLASH_Latency_1) || \
                                   ((LATENCY) == FLASH_Latency_2))
/**
  * @}
  */

/** @defgroup Half_Cycle_Enable_Disable
  * @{
  */

#define FLASH_HalfCycleAccess_Enable   ((uint32_t)0x00000008)  /*!< FLASH Half Cycle Enable */
#define FLASH_HalfCycleAccess_Disable  ((uint32_t)0x00000000)  /*!< FLASH Half Cycle Disable */
#define IS_FLASH_HALFCYCLEACCESS_STATE(STATE) (((STATE) == FLASH_HalfCycleAccess_Enable) || \
                                               ((STATE) == FLASH_HalfCycleAccess_Disable))
/**
  * @}
  */

/** @defgroup Prefetch_Buffer_Enable_Disable
  * @{
  */

#define FLASH_PrefetchBuffer_Enable    ((uint32_t)0x00000010)  /*!< FLASH Prefetch Buffer Enable */
#define FLASH_PrefetchBuffer_Disable   ((uint32_t)0x00000000)  /*!< FLASH Prefetch Buffer Disable */
#define IS_FLASH_PREFETCHBUFFER_STATE(STATE) (((STATE) == FLASH_PrefetchBuffer_Enable) || \
                                              ((STATE) == FLASH_PrefetchBuffer_Disable))
/**
  * @}
  */

/** @defgroup Option_Bytes_Write_Protection
  * @{
  */

/* Values to be used with STM32 Low and Medium density devices */
#define FLASH_WRProt_Pages0to3         ((uint32_t)0x00000001) /*!< STM32 Low and Medium density devices: Write protection of page 0 to 3 */
#define FLASH_WRProt_Pages4to7         ((uint32_t)0x00000002) /*!< STM32 Low and Medium density devices: Write protection of page 4 to 7 */
#define FLASH_WRProt_Pages8to11        ((uint32_t)0x00000004) /*!< STM32 Low and Medium density devices: Write protection of page 8 to 11 */
#define FLASH_WRProt_Pages12to15       ((uint32_t)0x00000008) /*!< STM32 Low and Medium density devices: Write protection of page 12 to 15 */
#define FLASH_WRProt_Pages16to19       ((uint32_t)0x00000010) /*!< STM32 Low and Medium density devices: Write protection of page 16 to 19 */
#define FLASH_WRProt_Pages20to23       ((uint32_t)0x00000020) /*!< STM32 Low and Medium density devices: Write protection of page 20 to 23 */
#define FLASH_WRProt_Pages24to27       ((uint32_t)0x00000040) /*!< STM32 Low and Medium density devices: Write protection of page 24 to 27 */
#define FLASH_WRProt_Pages28to31       ((uint32_t)0x00000080) /*!< STM32 Low and Medium density devices: Write protection of page 28 to 31 */

/* Values to be used with STM32 Medium-density devices */
#define FLASH_WRProt_Pages32to35       ((uint32_t)0x00000100) /*!< STM32 Medium-density devices: Write protection of page 32 to 35 */
#define FLASH_WRProt_Pages36to39       ((uint32_t)0x00000200) /*!< STM32 Medium-density devices: Write protection of page 36 to 39 */
#define FLASH_WRProt_Pages40to43       ((uint32_t)0x00000400) /*!< STM32 Medium-density devices: Write protection of page 40 to 43 */
#define FLASH_WRProt_Pages44to47       ((uint32_t)0x00000800) /*!< STM32 Medium-density devices: Write protection of page 44 to 47 */
#define FLASH_WRProt_Pages48to51       ((uint32_t)0x00001000) /*!< STM32 Medium-density devices: Write protection of page 48 to 51 */
#define FLASH_WRProt_Pages52to55       ((uint32_t)0x00002000) /*!< STM32 Medium-density devices: Write protection of page 52 to 55 */
#define FLASH_WRProt_Pages56to59       ((uint32_t)0x00004000) /*!< STM32 Medium-density devices: Write protection of page 56 to 59 */
#define FLASH_WRProt_Pages60to63       ((uint32_t)0x00008000) /*!< STM32 Medium-density devices: Write protection of page 60 to 63 */
#define FLASH_WRProt_Pages64to67       ((uint32_t)0x00010000) /*!< STM32 Medium-density devices: Write protection of page 64 to 67 */
#define FLASH_WRProt_Pages68to71       ((uint32_t)0x00020000) /*!< STM32 Medium-density devices: Write protection of page 68 to 71 */
#define FLASH_WRProt_Pages72to75       ((uint32_t)0x00040000) /*!< STM32 Medium-density devices: Write protection of page 72 to 75 */
#define FLASH_WRProt_Pages76to79       ((uint32_t)0x00080000) /*!< STM32 Medium-density devices: Write protection of page 76 to 79 */
#define FLASH_WRProt_Pages80to83       ((uint32_t)0x00100000) /*!< STM32 Medium-density devices: Write protection of page 80 to 83 */
#define FLASH_WRProt_Pages84to87       ((uint32_t)0x00200000) /*!< STM32 Medium-density devices: Write protection of page 84 to 87 */
#define FLASH_WRProt_Pages88to91       ((uint32_t)0x00400000) /*!< STM32 Medium-density devices: Write protection of page 88 to 91 */
#define FLASH_WRProt_Pages92to95       ((uint32_t)0x00800000) /*!< STM32 Medium-density devices: Write protection of page 92 to 95 */
#define FLASH_WRProt_Pages96to99       ((uint32_t)0x01000000) /*!< STM32 Medium-density devices: Write protection of page 96 to 99 */
#define FLASH_WRProt_Pages100to103     ((uint32_t)0x02000000) /*!< STM32 Medium-density devices: Write protection of page 100 to 103 */
#define FLASH_WRProt_Pages104to107     ((uint32_t)0x04000000) /*!< STM32 Medium-density devices: Write protection of page 104 to 107 */
#define FLASH_WRProt_Pages108to111     ((uint32_t)0x08000000) /*!< STM32 Medium-density devices: Write protection of page 108 to 111 */
#define FLASH_WRProt_Pages112to115     ((uint32_t)0x10000000) /*!< STM32 Medium-density devices: Write protection of page 112 to 115 */
#define FLASH_WRProt_Pages116to119     ((uint32_t)0x20000000) /*!< STM32 Medium-density devices: Write protection of page 115 to 119 */
#define FLASH_WRProt_Pages120to123     ((uint32_t)0x40000000) /*!< STM32 Medium-density devices: Write protection of page 120 to 123 */
#define FLASH_WRProt_Pages124to127     ((uint32_t)0x80000000) /*!< STM32 Medium-density devices: Write protection of page 124 to 127 */

/* Values to be used with STM32 High-density and STM32F10X Connectivity line devices */
#define FLASH_WRProt_Pages0to1         ((uint32_t)0x00000001) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 0 to 1 */
#define FLASH_WRProt_Pages2to3         ((uint32_t)0x00000002) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 2 to 3 */
#define FLASH_WRProt_Pages4to5         ((uint32_t)0x00000004) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 4 to 5 */
#define FLASH_WRProt_Pages6to7         ((uint32_t)0x00000008) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 6 to 7 */
#define FLASH_WRProt_Pages8to9         ((uint32_t)0x00000010) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 8 to 9 */
#define FLASH_WRProt_Pages10to11       ((uint32_t)0x00000020) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 10 to 11 */
#define FLASH_WRProt_Pages12to13       ((uint32_t)0x00000040) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 12 to 13 */
#define FLASH_WRProt_Pages14to15       ((uint32_t)0x00000080) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 14 to 15 */
#define FLASH_WRProt_Pages16to17       ((uint32_t)0x00000100) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 16 to 17 */
#define FLASH_WRProt_Pages18to19       ((uint32_t)0x00000200) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 18 to 19 */
#define FLASH_WRProt_Pages20to21       ((uint32_t)0x00000400) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 20 to 21 */
#define FLASH_WRProt_Pages22to23       ((uint32_t)0x00000800) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 22 to 23 */
#define FLASH_WRProt_Pages24to25       ((uint32_t)0x00001000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 24 to 25 */
#define FLASH_WRProt_Pages26to27       ((uint32_t)0x00002000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 26 to 27 */
#define FLASH_WRProt_Pages28to29       ((uint32_t)0x00004000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 28 to 29 */
#define FLASH_WRProt_Pages30to31       ((uint32_t)0x00008000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 30 to 31 */
#define FLASH_WRProt_Pages32to33       ((uint32_t)0x00010000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 32 to 33 */
#define FLASH_WRProt_Pages34to35       ((uint32_t)0x00020000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 34 to 35 */
#define FLASH_WRProt_Pages36to37       ((uint32_t)0x00040000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 36 to 37 */
#define FLASH_WRProt_Pages38to39       ((uint32_t)0x00080000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 38 to 39 */
#define FLASH_WRProt_Pages40to41       ((uint32_t)0x00100000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 40 to 41 */
#define FLASH_WRProt_Pages42to43       ((uint32_t)0x00200000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 42 to 43 */
#define FLASH_WRProt_Pages44to45       ((uint32_t)0x00400000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 44 to 45 */
#define FLASH_WRProt_Pages46to47       ((uint32_t)0x00800000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 46 to 47 */
#define FLASH_WRProt_Pages48to49       ((uint32_t)0x01000000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 48 to 49 */
#define FLASH_WRProt_Pages50to51       ((uint32_t)0x02000000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 50 to 51 */
#define FLASH_WRProt_Pages52to53       ((uint32_t)0x04000000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 52 to 53 */
#define FLASH_WRProt_Pages54to55       ((uint32_t)0x08000000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 54 to 55 */
#define FLASH_WRProt_Pages56to57       ((uint32_t)0x10000000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 56 to 57 */
#define FLASH_WRProt_Pages58to59       ((uint32_t)0x20000000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 58 to 59 */
#define FLASH_WRProt_Pages60to61       ((uint32_t)0x40000000) /*!< STM32 High-density, XL-density and Connectivity line devices:
                                                                   Write protection of page 60 to 61 */
#define FLASH_WRProt_Pages62to127      ((uint32_t)0x80000000) /*!< STM32 Connectivity line devices: Write protection of page 62 to 127 */
#define FLASH_WRProt_Pages62to255      ((uint32_t)0x80000000) /*!< STM32 Medium-density devices: Write protection of page 62 to 255 */
#define FLASH_WRProt_Pages62to511      ((uint32_t)0x80000000) /*!< STM32 XL-density devices: Write protection of page 62 to 511 */

#define FLASH_WRProt_AllPages          ((uint32_t)0xFFFFFFFF) /*!< Write protection of all Pages */

#define IS_FLASH_WRPROT_PAGE(PAGE) (((PAGE) != 0x00000000))

#define IS_FLASH_ADDRESS(ADDRESS) (((ADDRESS) >= 0x08000000) && ((ADDRESS) < 0x080FFFFF))

#define IS_OB_DATA_ADDRESS(ADDRESS) (((ADDRESS) == 0x1FFFF804) || ((ADDRESS) == 0x1FFFF806))

/**
  * @}
  */

/** @defgroup Option_Bytes_IWatchdog
  * @{
  */

#define OB_IWDG_SW                     ((uint16_t)0x0001)  /*!< Software IWDG selected */
#define OB_IWDG_HW                     ((uint16_t)0x0000)  /*!< Hardware IWDG selected */
#define IS_OB_IWDG_SOURCE(SOURCE) (((SOURCE) == OB_IWDG_SW) || ((SOURCE) == OB_IWDG_HW))

/**
  * @}
  */

/** @defgroup Option_Bytes_nRST_STOP
  * @{
  */

#define OB_STOP_NoRST                  ((uint16_t)0x0002) /*!< No reset generated when entering in STOP */
#define OB_STOP_RST                    ((uint16_t)0x0000) /*!< Reset generated when entering in STOP */
#define IS_OB_STOP_SOURCE(SOURCE) (((SOURCE) == OB_STOP_NoRST) || ((SOURCE) == OB_STOP_RST))

/**
  * @}
  */

/** @defgroup Option_Bytes_nRST_STDBY
  * @{
  */

#define OB_STDBY_NoRST                 ((uint16_t)0x0004) /*!< No reset generated when entering in STANDBY */
#define OB_STDBY_RST                   ((uint16_t)0x0000) /*!< Reset generated when entering in STANDBY */
#define IS_OB_STDBY_SOURCE(SOURCE) (((SOURCE) == OB_STDBY_NoRST) || ((SOURCE) == OB_STDBY_RST))

#ifdef STM32F10X_XL
/**
  * @}
  */
/** @defgroup FLASH_Boot
  * @{
  */
#define FLASH_BOOT_Bank1  ((uint16_t)0x0000) /*!< At startup, if boot pins are set in boot from user Flash position
                                                  and this parameter is selected the device will boot from Bank1(Default) */
#define FLASH_BOOT_Bank2  ((uint16_t)0x0001) /*!< At startup, if boot pins are set in boot from user Flash position
                                                  and this parameter is selected the device will boot from Bank 2 or Bank 1,
                                                  depending on the activation of the bank */
#define IS_FLASH_BOOT(BOOT) (((BOOT) == FLASH_BOOT_Bank1) || ((BOOT) == FLASH_BOOT_Bank2))
#endif
/**
  * @}
  */
/** @defgroup FLASH_Interrupts
  * @{
  */
#ifdef STM32F10X_XL
#define FLASH_IT_BANK2_ERROR                 ((uint32_t)0x80000400)  /*!< FPEC BANK2 error interrupt source */
#define FLASH_IT_BANK2_EOP                   ((uint32_t)0x80001000)  /*!< End of FLASH BANK2 Operation Interrupt source */

#define FLASH_IT_BANK1_ERROR                 FLASH_IT_ERROR          /*!< FPEC BANK1 error interrupt source */
#define FLASH_IT_BANK1_EOP                   FLASH_IT_EOP            /*!< End of FLASH BANK1 Operation Interrupt source */

#define FLASH_IT_ERROR                 ((uint32_t)0x00000400)  /*!< FPEC BANK1 error interrupt source */
#define FLASH_IT_EOP                   ((uint32_t)0x00001000)  /*!< End of FLASH BANK1 Operation Interrupt source */
#define IS_FLASH_IT(IT) ((((IT) & (uint32_t)0x7FFFEBFF) == 0x00000000) && (((IT) != 0x00000000)))
#else
#define FLASH_IT_ERROR                 ((uint32_t)0x00000400)  /*!< FPEC error interrupt source */
#define FLASH_IT_EOP                   ((uint32_t)0x00001000)  /*!< End of FLASH Operation Interrupt source */
#define FLASH_IT_BANK1_ERROR           FLASH_IT_ERROR          /*!< FPEC BANK1 error interrupt source */
#define FLASH_IT_BANK1_EOP             FLASH_IT_EOP            /*!< End of FLASH BANK1 Operation Interrupt source */

#define IS_FLASH_IT(IT) ((((IT) & (uint32_t)0xFFFFEBFF) == 0x00000000) && (((IT) != 0x00000000)))
#endif

/**
  * @}
  */

/** @defgroup FLASH_Flags
  * @{
  */
#ifdef STM32F10X_XL
#define FLASH_FLAG_BANK2_BSY                 ((uint32_t)0x80000001)  /*!< FLASH BANK2 Busy flag */
#define FLASH_FLAG_BANK2_EOP                 ((uint32_t)0x80000020)  /*!< FLASH BANK2 End of Operation flag */
#define FLASH_FLAG_BANK2_PGERR               ((uint32_t)0x80000004)  /*!< FLASH BANK2 Program error flag */
#define FLASH_FLAG_BANK2_WRPRTERR            ((uint32_t)0x80000010)  /*!< FLASH BANK2 Write protected error flag */

#define FLASH_FLAG_BANK1_BSY                 FLASH_FLAG_BSY       /*!< FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_EOP                 FLASH_FLAG_EOP       /*!< FLASH BANK1 End of Operation flag */
#define FLASH_FLAG_BANK1_PGERR               FLASH_FLAG_PGERR     /*!< FLASH BANK1 Program error flag */
#define FLASH_FLAG_BANK1_WRPRTERR            FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 Write protected error flag */

#define FLASH_FLAG_BSY                 ((uint32_t)0x00000001)  /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000020)  /*!< FLASH End of Operation flag */
#define FLASH_FLAG_PGERR               ((uint32_t)0x00000004)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_OPTERR              ((uint32_t)0x00000001)  /*!< FLASH Option Byte error flag */

#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0x7FFFFFCA) == 0x00000000) && ((FLAG) != 0x00000000))
#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_BSY) || ((FLAG) == FLASH_FLAG_EOP) || \
                                  ((FLAG) == FLASH_FLAG_PGERR) || ((FLAG) == FLASH_FLAG_WRPRTERR) || \
                                  ((FLAG) == FLASH_FLAG_OPTERR)|| \
                                  ((FLAG) == FLASH_FLAG_BANK1_BSY) || ((FLAG) == FLASH_FLAG_BANK1_EOP) || \
                                  ((FLAG) == FLASH_FLAG_BANK1_PGERR) || ((FLAG) == FLASH_FLAG_BANK1_WRPRTERR) || \
                                  ((FLAG) == FLASH_FLAG_BANK2_BSY) || ((FLAG) == FLASH_FLAG_BANK2_EOP) || \
                                  ((FLAG) == FLASH_FLAG_BANK2_PGERR) || ((FLAG) == FLASH_FLAG_BANK2_WRPRTERR))
#else
#define FLASH_FLAG_BSY                 ((uint32_t)0x00000001)  /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000020)  /*!< FLASH End of Operation flag */
#define FLASH_FLAG_PGERR               ((uint32_t)0x00000004)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_OPTERR              ((uint32_t)0x00000001)  /*!< FLASH Option Byte error flag */

#define FLASH_FLAG_BANK1_BSY                 FLASH_FLAG_BSY       /*!< FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_EOP                 FLASH_FLAG_EOP       /*!< FLASH BANK1 End of Operation flag */
#define FLASH_FLAG_BANK1_PGERR               FLASH_FLAG_PGERR     /*!< FLASH BANK1 Program error flag */
#define FLASH_FLAG_BANK1_WRPRTERR            FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 Write protected error flag */

#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFFFCA) == 0x00000000) && ((FLAG) != 0x00000000))
#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_BSY) || ((FLAG) == FLASH_FLAG_EOP) || \
                                  ((FLAG) == FLASH_FLAG_PGERR) || ((FLAG) == FLASH_FLAG_WRPRTERR) || \
								  ((FLAG) == FLASH_FLAG_BANK1_BSY) || ((FLAG) == FLASH_FLAG_BANK1_EOP) || \
                                  ((FLAG) == FLASH_FLAG_BANK1_PGERR) || ((FLAG) == FLASH_FLAG_BANK1_WRPRTERR) || \
                                  ((FLAG) == FLASH_FLAG_OPTERR))
#endif

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup FLASH_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup FLASH_Exported_Functions
  * @{
  */

/*------------ Functions used for all STM32F10x devices -----*/
void FLASH_SetLatency(uint32_t FLASH_Latency);
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess);
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer);
#ifndef ARDUINO_ARCH_STM32F1 /* already in maple flash_stm32 */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);
FLASH_Status FLASH_GetStatus(void);
void FLASH_Unlock(void);
void FLASH_Lock(void);
#endif
FLASH_Status FLASH_EraseAllPages(void);
FLASH_Status FLASH_EraseOptionBytes(void);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data);
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages);
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState);
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY);
uint32_t FLASH_GetUserOptionByte(void);
uint32_t FLASH_GetWriteProtectionOptionByte(void);
FlagStatus FLASH_GetReadOutProtectionStatus(void);
FlagStatus FLASH_GetPrefetchBufferStatus(void);
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);

/*------------ New function used for all STM32F10x devices -----*/
void FLASH_UnlockBank1(void);
void FLASH_LockBank1(void);
FLASH_Status FLASH_EraseAllBank1Pages(void);
FLASH_Status FLASH_GetBank1Status(void);
FLASH_Status FLASH_WaitForLastBank1Operation(uint32_t Timeout);

#ifdef STM32F10X_XL
/*---- New Functions used only with STM32F10x_XL density devices -----*/
void FLASH_UnlockBank2(void);
void FLASH_LockBank2(void);
FLASH_Status FLASH_EraseAllBank2Pages(void);
FLASH_Status FLASH_GetBank2Status(void);
FLASH_Status FLASH_WaitForLastBank2Operation(uint32_t Timeout);
FLASH_Status FLASH_BootConfig(uint16_t FLASH_BOOT);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_FLASH_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
