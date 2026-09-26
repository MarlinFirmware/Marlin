/**
  **************************************************************************
  * @file     at32f403a_407_wwdt.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the wwdt firmware library
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

/** @defgroup WWDT
  * @brief WWDT driver modules
  * @{
  */

#ifdef WWDT_MODULE_ENABLED

/** @defgroup WWDT_private_functions
  * @{
  */

/**
  * @brief  wwdt reset by crm reset register
  * @retval none
  */
void wwdt_reset(void)
{
  crm_periph_reset(CRM_WWDT_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_WWDT_PERIPH_RESET, FALSE);
}

/**
  * @brief  wwdt division set
  * @param  division
  *         this parameter can be one of the following values:
  *         - WWDT_PCLK1_DIV_4096        (wwdt counter clock = (pclk1/4096)/1)
  *         - WWDT_PCLK1_DIV_8192        (wwdt counter clock = (pclk1/4096)/2)
  *         - WWDT_PCLK1_DIV_16384       (wwdt counter clock = (pclk1/4096)/4)
  *         - WWDT_PCLK1_DIV_32768       (wwdt counter clock = (pclk1/4096)/8)
  * @retval none
  */
void wwdt_divider_set(wwdt_division_type division)
{
  WWDT->cfg_bit.div = division;
}

/**
  * @brief  wwdt reload counter interrupt flag clear
  * @param  none
  * @retval none
  */
void wwdt_flag_clear(void)
{
  WWDT->sts = 0;
}

/**
  * @brief  wwdt enable and the counter value load
  * @param  wwdt_cnt (0x40~0x7f)
  * @retval none
  */
void wwdt_enable(uint8_t wwdt_cnt)
{
  WWDT->ctrl = wwdt_cnt | WWDT_EN_BIT;
}

/**
  * @brief  wwdt reload counter interrupt enable
  * @param  none
  * @retval none
  */
void wwdt_interrupt_enable(void)
{
  WWDT->cfg_bit.rldien = TRUE;
}

/**
  * @brief  wwdt reload counter interrupt flag get
  * @param  none
  * @retval state of reload counter interrupt flag
  */
flag_status wwdt_flag_get(void)
{
  return (flag_status)WWDT->sts_bit.rldf;
}

/**
  * @brief  wwdt counter value set
  * @param  wwdt_cnt (0x40~0x7f)
  * @retval none
  */
void wwdt_counter_set(uint8_t wwdt_cnt)
{
  WWDT->ctrl_bit.cnt = wwdt_cnt;
}

/**
  * @brief  wwdt window counter value set
  * @param  window_cnt (0x40~0x7f)
  * @retval none
  */
void wwdt_window_counter_set(uint8_t window_cnt)
{
  WWDT->cfg_bit.win = window_cnt;
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
