/**
  **************************************************************************
  * @file     at32f403a_407_acc.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the acc firmware library
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

/** @defgroup ACC
  * @brief ACC driver modules
  * @{
  */

#ifdef ACC_MODULE_ENABLED

/** @defgroup ACC_private_functions
  * @{
  */

/**
  * @brief  enable or disable the acc calibration mode.
  * @param  acc_trim: specifies the acc calibration type.
  *         this parameter can be one of the following values:
  *         - ACC_CAL_HICKCAL
  *         - ACC_CAL_HICKTRIM
  * @param  new_state: specifies the acc calibration to be enabled or disabled.(TRUE or FALSE)
  * @retval none
  */
void acc_calibration_mode_enable(uint16_t acc_trim, confirm_state new_state)
{
  if(acc_trim == ACC_CAL_HICKCAL)
  {
    ACC->ctrl1_bit.entrim = FALSE;
  }
  else
  {
    ACC->ctrl1_bit.entrim = TRUE;
  }
  ACC->ctrl1_bit.calon = new_state;
}

/**
  * @brief  store calibration step data in acc's ctrl1 register.
  * @param  step_value:  value to be stored in the acc's ctrl1 register
  * @retval none
  */
void acc_step_set(uint8_t step_value)
{
  ACC->ctrl1_bit.step = step_value;
}

/**
  * @brief  enable or disable the specified acc interrupts.
  * @param  acc_int: specifies the acc interrupt sources to be enabled or disabled.
  *         this parameter can be one of the following values:
  *         - ACC_CALRDYIEN_INT
  *         - ACC_EIEN_INT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void acc_interrupt_enable(uint16_t acc_int, confirm_state new_state)
{
  if(acc_int == ACC_CALRDYIEN_INT)
  {
    ACC->ctrl1_bit.calrdyien = new_state;
  }
  else
  {
    ACC->ctrl1_bit.eien = new_state;
  }
}

/**
  * @brief  return the current acc hicktrim value.
  * @param  none
  * @retval 8-bit hicktrim value.
  */
uint8_t acc_hicktrim_get(void)
{
  return ((uint8_t)(ACC->ctrl2_bit.hicktrim));
}

/**
  * @brief  return the current acc hickcal value.
  * @param  none
  * @retval 8-bit hicktrim value.
  */
uint8_t acc_hickcal_get(void)
{
  return ((uint8_t)(ACC->ctrl2_bit.hickcal));
}

/**
  * @brief  wtire the value to  acc c1 register.
  * @param  acc_c1_value
  * @retval none.
  */
void acc_write_c1(uint16_t acc_c1_value)
{
  ACC->c1 = acc_c1_value;
}

/**
  * @brief  wtire the value to  acc c2 register.
  * @param  acc_c2_value
  * @retval none.
  */
void acc_write_c2(uint16_t acc_c2_value)
{
  ACC->c2 = acc_c2_value;
}

/**
  * @brief  wtire the value to  acc c3 register.
  * @param  acc_c3_value
  * @retval none.
  */
void acc_write_c3(uint16_t acc_c3_value)
{
  ACC->c3 = acc_c3_value;
}

/**
  * @brief  return the current acc c1 value.
  * @param  none
  * @retval 16-bit c1 value.
  */
uint16_t acc_read_c1(void)
{
  return ((uint16_t)(ACC->c1));
}

/**
  * @brief  return the current acc c2 value.
  * @param  none
  * @retval 16-bit c2 value.
  */
uint16_t acc_read_c2(void)
{
  return ((uint16_t)(ACC->c2));
}

/**
  * @brief  return the current acc c3 value.
  * @param  none
  * @retval 16-bit c3 value.
  */
uint16_t acc_read_c3(void)
{
  return ((uint16_t)(ACC->c3));
}

/**
  * @brief  check whether the specified acc flag is set or not.
  * @param  acc_flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - ACC_RSLOST_FLAG
  *         - ACC_CALRDY_FLAG
  * @retval  flag_status (SET or RESET)
  */
flag_status acc_flag_get(uint16_t acc_flag)
{
  if(acc_flag == ACC_CALRDY_FLAG)
    return (flag_status)(ACC->sts_bit.calrdy);
  else
    return (flag_status)(ACC->sts_bit.rslost);
}

/**
  * @brief  clear the specified acc flag is set or not.
  * @param  acc_flag: specifies the flag to check.
  *         this parameter can be any combination of the following values:
  *         - ACC_RSLOST_FLAG
  *         - ACC_CALRDY_FLAG
  * @retval  none
  */
void acc_flag_clear(uint16_t acc_flag)
{
  ACC->sts = ~acc_flag;
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
