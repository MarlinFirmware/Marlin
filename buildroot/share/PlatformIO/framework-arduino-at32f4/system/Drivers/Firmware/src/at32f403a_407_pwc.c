/**
  **************************************************************************
  * @file     at32f403a_407_pwc.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the pwc firmware library
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

/** @defgroup PWC
  * @brief PWC driver modules
  * @{
  */

#ifdef PWC_MODULE_ENABLED

/** @defgroup PWC_private_functions
  * @{
  */

/**
  * @brief  deinitialize the pwc peripheral registers to their default reset values.
  * @param  none
  * @retval none
  */
void pwc_reset(void)
{
  crm_periph_reset(CRM_PWC_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_PWC_PERIPH_RESET, FALSE);
}

/**
  * @brief  enable or disable access to the battery powered domain.
  * @param  new_state: new state of battery powered domain access.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void pwc_battery_powered_domain_access(confirm_state new_state)
{
  PWC->ctrl_bit.bpwen = new_state;
}

/**
  * @brief  select the voltage threshold detected by the power voltage detector.
  * @param  pvm_voltage: select pwc pvm voltage
  *         this parameter can be one of the following values:
  *         - PWC_PVM_VOLTAGE_2V3
  *         - PWC_PVM_VOLTAGE_2V4
  *         - PWC_PVM_VOLTAGE_2V5
  *         - PWC_PVM_VOLTAGE_2V6
  *         - PWC_PVM_VOLTAGE_2V7
  *         - PWC_PVM_VOLTAGE_2V8
  *         - PWC_PVM_VOLTAGE_2V9
  * @retval none
  */
void pwc_pvm_level_select(pwc_pvm_voltage_type pvm_voltage)
{
  PWC->ctrl_bit.pvmsel = pvm_voltage;
}

/**
  * @brief  enable or disable pwc power voltage monitor (pvm)
  * @param  new_state: new state of pvm.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void pwc_power_voltage_monitor_enable(confirm_state new_state)
{
  PWC->ctrl_bit.pvmen = new_state;
}

/**
  * @brief  enable or disable pwc standby wakeup pin
  * @param  pin_num: choose the wakeup pin.
  *         this parameter can be be any combination of the following values:
  *         - PWC_WAKEUP_PIN_1
  * @param  new_state: new state of the standby wakeup pin.
  *         this parameter can be one of the following values:
  *         - TRUE <wakeup pin is used for wake up cpu from standby mode>
  *         - FALSE <wakeup pin is used for general purpose I/O>
  * @retval none
  */
void pwc_wakeup_pin_enable(uint32_t pin_num, confirm_state new_state)
{
  if(new_state == TRUE)
  {
    PWC->ctrlsts |= pin_num;
  }
  else
  {
    PWC->ctrlsts &= ~pin_num;
  }
}

/**
  * @brief  clear flag of pwc
  * @param  pwc_flag: select the pwc flag.
  *         this parameter can be any combination of the following values:
  *         - PWC_WAKEUP_FLAG
  *         - PWC_STANDBY_FLAG
  *         - note:"PWC_PVM_OUTPUT_FLAG" cannot be choose!this bit is readonly bit,it means the voltage monitoring output state
  * @retval none
  */
void pwc_flag_clear(uint32_t pwc_flag)
{
  if(pwc_flag & PWC_STANDBY_FLAG)
    PWC->ctrl_bit.clsef = TRUE;
  if(pwc_flag & PWC_WAKEUP_FLAG)
    PWC->ctrl_bit.clswef = TRUE;
}

/**
  * @brief  get flag of pwc
  * @param  pwc_flag: select the pwc flag.
  *         this parameter can be one of the following values:
  *         - PWC_WAKEUP_FLAG
  *         - PWC_STANDBY_FLAG
  *         - PWC_PVM_OUTPUT_FLAG
  * @retval state of select flag(SET or RESET).
  */
flag_status pwc_flag_get(uint32_t pwc_flag)
{
  flag_status status = RESET;
  if ((PWC->ctrlsts & pwc_flag) == RESET)
  {
    status = RESET;
  }
  else
  {
    status = SET;
  }
  return status;
}

/**
  * @brief  enter pwc sleep mode
  * @param  sleep_mode_enter: choose the instruction to enter sleep mode.
  *         this parameter can be one of the following values:
  *         - PWC_SLEEP_ENTER_WFI
  *         - PWC_SLEEP_ENTER_WFE
  * @retval none
  */
void pwc_sleep_mode_enter(pwc_sleep_enter_type pwc_sleep_enter)
{
  SCB->SCR &= (uint32_t)~0x4;
  if(pwc_sleep_enter == PWC_SLEEP_ENTER_WFE)
  {
    __SEV();
    __WFE();
    __WFE();
  }
  else if(pwc_sleep_enter == PWC_SLEEP_ENTER_WFI)
  {
    __WFI();
  }
}

/**
  * @brief  enter pwc deep-sleep mode
  * @param  pwc_deep_sleep_enter: choose the instruction to enter deep sleep mode.
  *         this parameter can be one of the following values:
  *         - PWC_DEEP_SLEEP_ENTER_WFI
  *         - PWC_DEEP_SLEEP_ENTER_WFE
  * @retval none
  */
void pwc_deep_sleep_mode_enter(pwc_deep_sleep_enter_type pwc_deep_sleep_enter)
{
  SCB->SCR |= 0x04;
  if(pwc_deep_sleep_enter == PWC_DEEP_SLEEP_ENTER_WFE)
  {
    __SEV();
    __WFE();
    __WFE();
  }
  else if(pwc_deep_sleep_enter == PWC_DEEP_SLEEP_ENTER_WFI)
  {
    __WFI();
  }
  SCB->SCR &= (uint32_t)~0x4;
}

/**
  * @brief  regulate low power consumption in the deep sleep mode
  * @param  pwc_regulator: set the regulator state.
  *         this parameter can be one of the following values:
  *         - PWC_REGULATOR_ON
  *         - PWC_REGULATOR_LOW_POWER
  * @retval none
  */
void pwc_voltage_regulate_set(pwc_regulator_type pwc_regulator)
{
  PWC->ctrl_bit.vrsel = pwc_regulator;
}

/**
  * @brief  enter pwc standby mode
  * @param  none
  * @retval none
  */
void pwc_standby_mode_enter(void)
{
  PWC->ctrl_bit.clswef = TRUE;
  PWC->ctrl_bit.lpsel = TRUE;
  SCB->SCR |= 0x04;
#if defined (__CC_ARM)
  __force_stores();
#endif
  __WFI();
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
