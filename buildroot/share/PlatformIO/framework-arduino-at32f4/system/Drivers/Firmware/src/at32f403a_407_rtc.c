/**
  **************************************************************************
  * @file     at32f403a_407_rtc.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the rtc firmware library
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

/** @defgroup RTC
  * @brief RTC driver modules
  * @{
  */

#ifdef RTC_MODULE_ENABLED

/** @defgroup RTC_private_functions
  * @{
  */

/**
  * @brief  rtc counter set
  * @param  counter_value (0x0000_0000 ~ 0xFFFF_FFFF)
  * @retval none
  */
void rtc_counter_set(uint32_t counter_value)
{
  /* enter rtc config mode */
  RTC->ctrll = 0x003F;

  /* set rtc counter */
  RTC->cnth_bit.cnt = (uint16_t)(counter_value >> 16);
  RTC->cntl_bit.cnt = (uint16_t)(counter_value & 0x0000FFFF);

  /* exit rtc config mode */
  RTC->ctrll = 0x000F;
}

/**
  * @brief  rtc counter get
  * @param  none
  * @retval rtc counter
  */
uint32_t rtc_counter_get(void)
{
  uint32_t cnt = 0;

  cnt = RTC->cnth;
  cnt = (cnt << 16) | RTC->cntl;

  return cnt;
}

/**
  * @brief  rtc divider set
  * @param  div_value (0x0000_0000 ~ 0xFFFF_FFFF)
  * @retval none
  */
void rtc_divider_set(uint32_t div_value)
{
  /* enter rtc config mode */
  RTC->ctrll = 0x003F;

  /* set rtc divider */
  RTC->divh_bit.div = (uint16_t)(div_value >> 16);
  RTC->divl_bit.div = (uint16_t)(div_value & 0x0000FFFF);

  /* exit rtc config mode */
  RTC->ctrll = 0x000F;
}

/**
  * @brief  rtc divider get
  * @param  none
  * @retval rtc counter
  */
uint32_t rtc_divider_get(void)
{
  uint32_t div = 0;

  div = RTC->divcnth;
  div = (div << 16) | RTC->divcntl;

  return div;
}

/**
  * @brief  rtc alarm value set
  * @param  alarm_value (0x0000_0000 ~ 0xFFFF_FFFF)
  * @retval none
  */
void rtc_alarm_set(uint32_t alarm_value)
{
  /* enter rtc config mode */
  RTC->ctrll = 0x003F;

  /* set rtc alarm value */
  RTC->tah_bit.ta = (uint16_t)(alarm_value >> 16);
  RTC->tal_bit.ta = (uint16_t)(alarm_value & 0x0000FFFF);

  /* exit rtc config mode */
  RTC->ctrll = 0x000F;
}

/**
  * @brief  rtc interrupt enable
  * @param  source
  *         this parameter can be any combination of the following values:
  *         - RTC_TS_INT: time second interrupt.
  *         - RTC_TA_INT: time alarm interrupt.
  *         - RTC_OVF_INT: overflow interrupt.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void rtc_interrupt_enable(uint16_t source, confirm_state new_state)
{
  if(new_state == FALSE)
  {
    RTC->ctrlh &= ~source;
  }
  else
  {
    RTC->ctrlh |= source;
  }
}

/**
  * @brief  rtc flag get
  * @param  flag
  *         this parameter can be one of the following values:
  *         - RTC_TS_FLAG: time second flag.
  *         - RTC_TA_FLAG: time alarm flag.
  *         - RTC_OVF_FLAG: overflow flag.
  *         - RTC_UPDF_FLAG: rtc update finish flag.
  *         - RTC_CFGF_FLAG: rtc configuration finish flag.
  * @retval state of rtc flag
  */
flag_status rtc_flag_get(uint16_t flag)
{
  flag_status status = RESET;

  if ((RTC->ctrll & flag) != (uint16_t)RESET)
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
  * @brief  rtc flag clear
  * @param  interrupt_flag
  *         this parameter can be any combination of the following values:
  *         - RTC_TS_FLAG: time second flag.
  *         - RTC_TA_FLAG: time alarm flag.
  *         - RTC_OVF_FLAG: overflow flag.
  *         - RTC_CFGF_FLAG: rtc configuration finish flag.
  * @retval none
  */
void rtc_flag_clear(uint16_t flag)
{
  RTC->ctrll = ~(flag | 0x10) | (RTC->ctrll_bit.cfgen << 4);
}

/**
  * @brief  rtc wait configuration finish
  * @param  none
  * @retval none
  */
void rtc_wait_config_finish(void)
{
  while (RTC->ctrll_bit.cfgf == 0);
}

/**
  * @brief  rtc wait update finish
  * @param  none
  * @retval none
  */
void rtc_wait_update_finish(void)
{
  while (RTC->ctrll_bit.updf == 0);
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
