/**
  **************************************************************************
  * @file     at32f403a_407_exint.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the exint firmware library
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f403a_407_conf.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @defgroup EXINT
  * @brief EXINT driver modules
  * @{
  */

#ifdef EXINT_MODULE_ENABLED

/** @defgroup EXINT_private_functions
  * @{
  */

/**
  * @brief  exint reset
  * @param  none
  * @retval none
  */
void exint_reset(void)
{
  EXINT->inten = 0x00000000;
  EXINT->polcfg1 = 0x00000000;
  EXINT->polcfg2 = 0x00000000;
  EXINT->evten = 0x00000000;
  EXINT->intsts = 0x000FFFFF;
}

/**
  * @brief  exint default para init
  * @param  exint_struct
  *         - to the structure of exint_init_type
  * @retval none
  */
void exint_default_para_init(exint_init_type *exint_struct)
{
  exint_struct->line_enable = FALSE;
  exint_struct->line_select = EXINT_LINE_NONE;
  exint_struct->line_polarity = EXINT_TRIGGER_FALLING_EDGE;
  exint_struct->line_mode = EXINT_LINE_EVENT;
}

/**
  * @brief  exint init
  * @param  exint_struct
  *         - to the structure of exint_init_type
  * @retval none
  */
void exint_init(exint_init_type *exint_struct)
{
  uint32_t line_index = 0;
  line_index = exint_struct->line_select;

  EXINT->inten &= ~line_index;
  EXINT->evten &= ~line_index;

  if(exint_struct->line_enable != FALSE)
  {
    if(exint_struct->line_mode == EXINT_LINE_INTERRUPUT)
    {
      EXINT->inten |= line_index;
    }
    else
    {
      EXINT->evten |= line_index;
    }

    EXINT->polcfg1 &= ~line_index;
    EXINT->polcfg2 &= ~line_index;
    if(exint_struct->line_polarity == EXINT_TRIGGER_RISING_EDGE)
    {
      EXINT->polcfg1 |= line_index;
    }
    else if(exint_struct->line_polarity == EXINT_TRIGGER_FALLING_EDGE)
    {
      EXINT->polcfg2 |= line_index;
    }
    else
    {
      EXINT->polcfg1 |= line_index;
      EXINT->polcfg2 |= line_index;
    }
  }
}

/**
  * @brief  clear exint flag
  * @param  exint_line
  *         this parameter can be any combination of the following values:
  *         - EXINT_LINE_0
  *         - EXINT_LINE_1
  *         ...
  *         - EXINT_LINE_18
  *         - EXINT_LINE_19
  * @retval none
  */
void exint_flag_clear(uint32_t exint_line)
{
  EXINT->intsts = exint_line;
}

/**
  * @brief  get exint flag
  * @param  exint_line
  *         this parameter can be one of the following values:
  *         - EXINT_LINE_0
  *         - EXINT_LINE_1
  *         ...
  *         - EXINT_LINE_18
  *         - EXINT_LINE_19
  * @retval the new state of exint flag(SET or RESET).
  */
flag_status exint_flag_get(uint32_t exint_line)
{
  flag_status status = RESET;
  uint32_t exint_flag =0;
  exint_flag = EXINT->intsts & exint_line;
  if((exint_flag != (uint16_t)RESET))
  {
    status = SET;
  }
  else
  {
    status = RESET;
  }
  return status;
}

/**
  * @brief  generate exint software interrupt event
  * @param  exint_line
  *         this parameter can be one of the following values:
  *         - EXINT_LINE_0
  *         - EXINT_LINE_1
  *         ...
  *         - EXINT_LINE_18
  *         - EXINT_LINE_19
  * @retval none
  */
void exint_software_interrupt_event_generate(uint32_t exint_line)
{
  EXINT->swtrg |= exint_line;
}

/**
  * @brief  enable or disable exint interrupt
  * @param  exint_line
  *         this parameter can be any combination of the following values:
  *         - EXINT_LINE_0
  *         - EXINT_LINE_1
  *         ...
  *         - EXINT_LINE_18
  *         - EXINT_LINE_19
  * @param  new_state: new state of exint interrupt.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void exint_interrupt_enable(uint32_t exint_line, confirm_state new_state)
{
  if(new_state == TRUE)
  {
    EXINT->inten |= exint_line;
  }
  else
  {
    EXINT->inten &= ~exint_line;
  }
}

/**
  * @brief  enable or disable exint event
  * @param  exint_line
  *         this parameter can be any combination of the following values:
  *         - EXINT_LINE_0
  *         - EXINT_LINE_1
  *         ...
  *         - EXINT_LINE_18
  *         - EXINT_LINE_19
  * @param  new_state: new state of exint event.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void exint_event_enable(uint32_t exint_line, confirm_state new_state)
{
  if(new_state == TRUE)
  {
    EXINT->evten |= exint_line;
  }
  else
  {
    EXINT->evten &= ~exint_line;
  }
}

/**
  * @}
  */

#endif

/**
  * @}
  */

/**
  * @}
  */
