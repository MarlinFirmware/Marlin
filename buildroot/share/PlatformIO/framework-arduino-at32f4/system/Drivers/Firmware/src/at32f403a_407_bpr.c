/**
  **************************************************************************
  * @file     at32f403a_407_bpr.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the bpr firmware library
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

/** @defgroup BPR
  * @brief BPR driver modules
  * @{
  */

#ifdef BPR_MODULE_ENABLED

/** @defgroup BPR_private_functions
  * @{
  */

/**
  * @brief  bpr reset by crm reset register
  * @param  none
  * @retval none
  */
void bpr_reset(void)
{
  crm_battery_powered_domain_reset(TRUE);
  crm_battery_powered_domain_reset(FALSE);
}

/**
  * @brief  bpr event flag get, for tamper event flag
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - BPR_TAMPER_INTERRUPT_FLAG: tamper interrupt flag
  *         - BPR_TAMPER_EVENT_FLAG:   tamper event flag
  * @retval state of tamper event flag
  */
flag_status bpr_flag_get(uint32_t flag)
{
  if(flag == BPR_TAMPER_INTERRUPT_FLAG)
  {
    return (flag_status)(BPR->ctrlsts_bit.tpif);
  }
  else
  {
    return (flag_status)(BPR->ctrlsts_bit.tpef);
  }
}

/**
  * @brief  clear bpr tamper flag
  * @param  flag: specifies the flag to clear.
  *         this parameter can be one of the following values:
  *         - BPR_TAMPER_INTERRUPT_FLAG: tamper interrupt flag
  *         - BPR_TAMPER_EVENT_FLAG:   tamper event flag
  * @retval none
  */
void bpr_flag_clear(uint32_t flag)
{
  if(flag == BPR_TAMPER_INTERRUPT_FLAG)
  {
    BPR->ctrlsts_bit.tpifclr = TRUE;
  }
  else
  {
    BPR->ctrlsts_bit.tpefclr = TRUE;
  }
}

/**
  * @brief  enable or disable bpr tamper interrupt
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void bpr_interrupt_enable(confirm_state new_state)
{
  BPR->ctrlsts_bit.tpien = new_state;
}

/**
  * @brief  read bpr bpr data
  * @param  bpr_data
  *         this parameter can be one of the following values:
  *         - BPR_DATA1
  *         - BPR_DATA2
  *         ...
  *         - BPR_DATA41
  *         - BPR_DATA42
  * @retval none
  */
uint16_t bpr_data_read(bpr_data_type bpr_data)
{
  return (*(__IO uint16_t *)(BPR_BASE + bpr_data));
}

/**
  * @brief  write bpr data
  * @param  bpr_data
  *         this parameter can be one of the following values:
  *         - BPR_DATA1
  *         - BPR_DATA2
  *         ...
  *         - BPR_DATA41
  *         - BPR_DATA42
  * @param  data_value (0x0000~0xFFFF)
  * @retval none
  */
void bpr_data_write(bpr_data_type bpr_data, uint16_t data_value)
{
  (*(__IO uint32_t *)(BPR_BASE + bpr_data)) = data_value;
}

/**
  * @brief  select bpr rtc output
  * @param  output_source
  *         this parameter can be one of the following values:
  *         - BPR_RTC_OUTPUT_NONE: output disable.
  *         - BPR_RTC_OUTPUT_CLOCK_CAL_BEFORE: output clock before calibration.
  *         - BPR_RTC_OUTPUT_ALARM: output alarm event with pluse mode.
  *         - BPR_RTC_OUTPUT_SECOND: output second event with pluse mode.
  *         - BPR_RTC_OUTPUT_CLOCK_CAL_AFTER: output clock after calibration.
  *         - BPR_RTC_OUTPUT_ALARM_TOGGLE: output alarm event with toggle mode.
  *         - BPR_RTC_OUTPUT_SECOND_TOGGLE: output second event with toggle mode.
  * @retval none
  */
void bpr_rtc_output_select(bpr_rtc_output_type output_source)
{
  /* clear cco,asoe,asos,ccos,togen bits */
  BPR->rtccal &= (uint32_t)~0x0F80;

  /* set output_source value */
  BPR->rtccal |= output_source;
}

/**
  * @brief  set rtc clock calibration value
  * @param  calibration_value (0x00~0x7f)
  * @retval none
  */
void bpr_rtc_clock_calibration_value_set(uint8_t calibration_value)
{
  /* set rtc clock calibration value */
  BPR->rtccal_bit.calval= calibration_value;
}

/**
  * @brief  enable or disable bpr tamper pin
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void bpr_tamper_pin_enable(confirm_state new_state)
{
  BPR->ctrl_bit.tpen = new_state;
}

/**
  * @brief  set bpr tamper pin active level
  * @param  active_level
  *         this parameter can be one of the following values:
  *         - BPR_TAMPER_PIN_ACTIVE_HIGH: tamper pin input active level is high.
  *         - BPR_TAMPER_PIN_ACTIVE_LOW: tamper pin input active level is low.
  * @retval none
  */
void bpr_tamper_pin_active_level_set(bpr_tamper_pin_active_level_type active_level)
{
  BPR->ctrl_bit.tpp = active_level;
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
