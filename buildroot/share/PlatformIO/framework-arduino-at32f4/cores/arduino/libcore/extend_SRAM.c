/**
  ******************************************************************************
  * File   : SRAM/extend_SRAM/EXTEND_SRAM/extend_SRAM.c
  * Version: V1.2.7
  * Date   : 2020-11-10
  * Brief  : This file contains the function extend_SRAM_test used to extend SRAM size
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "extend_SRAM.h" 
#include "at32f403a_407.h"     

#define EXTEND_SRAM_224K

/* Copy to Startup file */
#if 0
; Reset handler
Reset_Handler   PROC
                EXPORT  Reset_Handler                       [WEAK]
                IMPORT  __main
                IMPORT  SystemInit
                IMPORT  extend_SRAM

                MOV32   R0, #0x20001000
                MOV     SP, R0
                LDR     R0, =extend_SRAM
                BLX     R0
                MOV32   R0, #0x08000000
                LDR     SP, [R0]

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP
#endif

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

 /** @addtogroup SRAM_Extended_SRAM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

#define TEST_RAM_SIZE  0x800

/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/
/**
  * @brief  To extend SRAM size
  * @param  None
  * @retval None
  */
void extend_sram(void)
{
/* Target set_SRAM_96K is selected */
#ifdef EXTEND_SRAM_96K
/* check if RAM has been set to 96K, if not, change EOPB0 */
  if(((UOPTB->EOPB0)&0xFF)!=0xFF)
  {
    /* Unlock Option Bytes Program Erase controller */
    FLASH_Unlock();
    /* Erase Option Bytes */
    FLASH_EraseUserOptionBytes();
    /* Change SRAM size to 96KB */
    FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0,0xFF);
    NVIC_SystemReset();
  }
#endif

/* Target set_SRAM_224K is selected */
#ifdef EXTEND_SRAM_224K
  /* check if ram has been set to expectant size, if not, change eopb0 */
  if(((USD->eopb0) & 0xFF) != 0xFE)
  {
    flash_unlock();
    /* erase user system data bytes */
    flash_user_system_data_erase();

    /* change sram size */
    flash_user_system_data_program((uint32_t)&USD->eopb0, 0xFE);

    /* system reset */
    nvic_system_reset();
  }
#endif
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
