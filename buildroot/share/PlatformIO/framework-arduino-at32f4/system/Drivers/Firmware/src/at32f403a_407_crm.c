/**
  **************************************************************************
  * @file     at32f403a_407_crm.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the crm firmware library
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

/** @defgroup CRM
  * @brief CRM driver modules
  * @{
  */

#ifdef CRM_MODULE_ENABLED

/** @defgroup CRM_private_functions
  * @{
  */

/**
  * @brief  reset the crm register
  * @param  none
  * @retval none
  */
void crm_reset(void)
{
  /* reset the crm clock configuration to the default reset state(for debug purpose) */
  /* set hicken bit */
  CRM->ctrl_bit.hicken = TRUE;

  /* wait hick stable */
  while(CRM->ctrl_bit.hickstbl != SET);

  /* hick used as system clock */
  CRM->cfg_bit.sclksel = CRM_SCLK_HICK;

  /* wait sclk switch status */
  while(CRM->cfg_bit.sclksts != CRM_SCLK_HICK);

  /* reset cfg register, include sclk switch, ahbdiv, apb1div, apb2div, adcdiv,
     clkout pllrcs, pllhextdiv, pllmult, usbdiv and pllrange bits */
  CRM->cfg = 0;

  /* reset hexten, hextbyps, cfden and pllen bits */
  CRM->ctrl &= ~(0x010D0000U);

  /* reset clkout[3], usbbufs, hickdiv, clkoutdiv */
  CRM->misc1 = 0;

  /* disable all interrupts enable and clear pending bits  */
  CRM->clkint = 0x009F0000;
}

/**
  * @brief  enable or disable crm low speed external crystal bypass
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_lext_bypass(confirm_state new_state)
{
  CRM->bpdc_bit.lextbyps = new_state;
}

/**
  * @brief  enable or disable crm high speed external crystal bypass
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_hext_bypass(confirm_state new_state)
{
  CRM->ctrl_bit.hextbyps = new_state;
}

/**
  * @brief  get crm flag status
  * @param  flag
  *         this parameter can be one of the following values:
  *         - CRM_HICK_STABLE_FLAG
  *         - CRM_HEXT_STABLE_FLAG
  *         - CRM_PLL_STABLE_FLAG
  *         - CRM_LEXT_STABLE_FLAG
  *         - CRM_LICK_STABLE_FLAG
  *         - CRM_NRST_RESET_FLAG
  *         - CRM_POR_RESET_FLAG
  *         - CRM_SW_RESET_FLAG
  *         - CRM_WDT_RESET_FLAG
  *         - CRM_WWDT_RESET_FLAG
  *         - CRM_LOWPOWER_RESET_FLAG
  *         interrupt flag:
  *         - CRM_LICK_READY_INT_FLAG
  *         - CRM_LEXT_READY_INT_FLAG
  *         - CRM_HICK_READY_INT_FLAG
  *         - CRM_HEXT_READY_INT_FLAG
  *         - CRM_PLL_READY_INT_FLAG
  *         - CRM_CLOCK_FAILURE_INT_FLAG
  * @retval flag_status (SET or RESET)
  */
flag_status crm_flag_get(uint32_t flag)
{
  flag_status status = RESET;
  if((CRM_REG(flag) & CRM_REG_BIT(flag)) != CRM_REG_BIT(flag))
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
  * @brief  wait for hext stable
  * @param  none
  * @retval error_status (ERROR or SUCCESS)
  */
error_status crm_hext_stable_wait(void)
{
  uint32_t stable_cnt = 0;
  error_status status = ERROR;

  while((crm_flag_get(CRM_HEXT_STABLE_FLAG) != SET) && (stable_cnt < HEXT_STARTUP_TIMEOUT))
  {
    stable_cnt ++;
  }

  if(crm_flag_get(CRM_HEXT_STABLE_FLAG) != SET)
  {
    status = ERROR;
  }
  else
  {
    status = SUCCESS;
  }

  return status;
}

/**
  * @brief  set the hick trimming value
  * @param  trim_value (0x00~0x3F)
  * @retval none
  */
void crm_hick_clock_trimming_set(uint8_t trim_value)
{
  CRM->ctrl_bit.hicktrim = trim_value;
}

/**
  * @brief  set the crm calibration value
  * @param  cali_value (0x00~0xFF)
  * @retval none
  */
void crm_hick_clock_calibration_set(uint8_t cali_value)
{
  /* enable write hick calibration */
  CRM->misc1_bit.hickcal_key = 0x5A;

  /* write hick calibration value */
  CRM->ctrl_bit.hickcal = cali_value;

  /* disable write hick calibration */
  CRM->misc1_bit.hickcal_key = 0x0;
}

/**
  * @brief  enable or disable the peripheral clock
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_DMA1_PERIPH_CLOCK         - CRM_DMA2_PERIPH_CLOCK         - CRM_CRC_PERIPH_CLOCK         - CRM_XMC_PERIPH_CLOCK
  *         - CRM_SDIO1_PERIPH_CLOCK        - CRM_SDIO2_PERIPH_CLOCK        - CRM_EMAC_PERIPH_CLOCK        - CRM_EMACTX_PERIPH_CLOCK
  *         - CRM_EMACRX_PERIPH_CLOCK       - CRM_EMACPTP_PERIPH_CLOCK      - CRM_IOMUX_PERIPH_CLOCK       - CRM_GPIOA_PERIPH_CLOCK
  *         - CRM_GPIOB_PERIPH_CLOCK        - CRM_GPIOC_PERIPH_CLOCK        - CRM_GPIOD_PERIPH_CLOCK       - CRM_GPIOE_PERIPH_CLOCK
  *         - CRM_ADC1_PERIPH_CLOCK         - CRM_ADC2_PERIPH_CLOCK         - CRM_TMR1_PERIPH_CLOCK        - CRM_SPI1_PERIPH_CLOCK
  *         - CRM_TMR8_PERIPH_CLOCK         - CRM_USART1_PERIPH_CLOCK       - CRM_ADC3_PERIPH_CLOCK        - CRM_TMR9_PERIPH_CLOCK
  *         - CRM_TMR10_PERIPH_CLOCK        - CRM_TMR11_PERIPH_CLOCK        - CRM_ACC_PERIPH_CLOCK         - CRM_I2C3_PERIPH_CLOCK
  *         - CRM_USART6_PERIPH_CLOCK       - CRM_UART7_PERIPH_CLOCK        - CRM_UART8_PERIPH_CLOCK       - CRM_TMR2_PERIPH_CLOCK
  *         - CRM_TMR3_PERIPH_CLOCK         - CRM_TMR4_PERIPH_CLOCK         - CRM_TMR5_PERIPH_CLOCK        - CRM_TMR6_PERIPH_CLOCK
  *         - CRM_TMR7_PERIPH_CLOCK         - CRM_TMR12_PERIPH_CLOCK        - CRM_TMR13_PERIPH_CLOCK       - CRM_TMR14_PERIPH_CLOCK
  *         - CRM_WWDT_PERIPH_CLOCK         - CRM_SPI2_PERIPH_CLOCK         - CRM_SPI3_PERIPH_CLOCK        - CRM_SPI4_PERIPH_CLOCK
  *         - CRM_USART2_PERIPH_CLOCK       - CRM_USART3_PERIPH_CLOCK       - CRM_UART4_PERIPH_CLOCK       - CRM_UART5_PERIPH_CLOCK
  *         - CRM_I2C1_PERIPH_CLOCK         - CRM_I2C2_PERIPH_CLOCK         - CRM_USB_PERIPH_CLOCK         - CRM_CAN1_PERIPH_CLOCK
  *         - CRM_CAN2_PERIPH_CLOCK         - CRM_BPR_PERIPH_CLOCK          - CRM_PWC_PERIPH_CLOCK         - CRM_DAC_PERIPH_CLOCK
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_periph_clock_enable(crm_periph_clock_type value, confirm_state new_state)
{
  /* enable periph clock */
  if(TRUE == new_state)
  {
    CRM_REG(value) |= CRM_REG_BIT(value);
  }
  /* disable periph clock */
  else
  {
    CRM_REG(value) &= ~(CRM_REG_BIT(value));
  }
}

/**
  * @brief  enable or disable the peripheral reset
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_EMAC_PERIPH_RESET         - CRM_IOMUX_PERIPH_RESET        - CRM_EXINT_PERIPH_RESET       - CRM_GPIOA_PERIPH_RESET
  *         - CRM_GPIOB_PERIPH_RESET        - CRM_GPIOC_PERIPH_RESET        - CRM_GPIOD_PERIPH_RESET       - CRM_GPIOE_PERIPH_RESET
  *         - CRM_ADC1_PERIPH_RESET         - CRM_ADC2_PERIPH_RESET         - CRM_TMR1_PERIPH_RESET        - CRM_SPI1_PERIPH_RESET
  *         - CRM_TMR8_PERIPH_RESET         - CRM_USART1_PERIPH_RESET       - CRM_ADC3_PERIPH_RESET        - CRM_TMR9_PERIPH_RESET
  *         - CRM_TMR10_PERIPH_RESET        - CRM_TMR11_PERIPH_RESET        - CRM_ACC_PERIPH_RESET         - CRM_I2C3_PERIPH_RESET
  *         - CRM_USART6_PERIPH_RESET       - CRM_UART7_PERIPH_RESET        - CRM_UART8_PERIPH_RESET       - CRM_TMR2_PERIPH_RESET
  *         - CRM_TMR3_PERIPH_RESET         - CRM_TMR4_PERIPH_RESET         - CRM_TMR5_PERIPH_RESET        - CRM_TMR6_PERIPH_RESET
  *         - CRM_TMR7_PERIPH_RESET         - CRM_TMR12_PERIPH_RESET        - CRM_TMR13_PERIPH_RESET       - CRM_TMR14_PERIPH_RESET
  *         - CRM_WWDT_PERIPH_RESET         - CRM_SPI2_PERIPH_RESET         - CRM_SPI3_PERIPH_RESET        - CRM_SPI4_PERIPH_RESET
  *         - CRM_USART2_PERIPH_RESET       - CRM_USART3_PERIPH_RESET       - CRM_UART4_PERIPH_RESET       - CRM_UART5_PERIPH_RESET
  *         - CRM_I2C1_PERIPH_RESET         - CRM_I2C2_PERIPH_RESET         - CRM_USB_PERIPH_RESET         - CRM_CAN1_PERIPH_RESET
  *         - CRM_CAN2_PERIPH_RESET         - CRM_BPR_PERIPH_RESET          - CRM_PWC_PERIPH_RESET         - CRM_DAC_PERIPH_RESET
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_periph_reset(crm_periph_reset_type value, confirm_state new_state)
{
  /* enable periph reset */
  if(new_state == TRUE)
  {
    CRM_REG(value) |= (CRM_REG_BIT(value));
  }
  /* disable periph reset */
  else
  {
    CRM_REG(value) &= ~(CRM_REG_BIT(value));
  }
}

/**
  * @brief  enable or disable the peripheral clock in sleep mode
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_SRAM_PERIPH_CLOCK_SLEEP_MODE
  *         - CRM_FLASH_PERIPH_CLOCK_SLEEP_MODE
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_periph_sleep_mode_clock_enable(crm_periph_clock_sleepmd_type value, confirm_state new_state)
{
  /* enable periph clock in sleep mode */
  if(new_state == TRUE)
  {
    CRM_REG(value) |= (CRM_REG_BIT(value));
  }
  /* disable perph clock in sleep mode */
  else
  {
    CRM_REG(value) &= ~(CRM_REG_BIT(value));
  }

}

/**
  * @brief  enable or disable the crm clock source
  * @param  source
  *         this parameter can be one of the following values:
  *         - CRM_CLOCK_SOURCE_HICK
  *         - CRM_CLOCK_SOURCE_HEXT
  *         - CRM_CLOCK_SOURCE_PLL
  *         - CRM_CLOCK_SOURCE_LEXT
  *         - CRM_CLOCK_SOURCE_LICK
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_clock_source_enable(crm_clock_source_type source, confirm_state new_state)
{
  switch(source)
  {
    case CRM_CLOCK_SOURCE_HICK:
      CRM->ctrl_bit.hicken = new_state;
      break;
    case CRM_CLOCK_SOURCE_HEXT:
      CRM->ctrl_bit.hexten = new_state;
      break;
    case CRM_CLOCK_SOURCE_PLL:
      CRM->ctrl_bit.pllen = new_state;
      break;
    case CRM_CLOCK_SOURCE_LEXT:
      CRM->bpdc_bit.lexten = new_state;
      break;
    case CRM_CLOCK_SOURCE_LICK:
      CRM->ctrlsts_bit.licken = new_state;
      break;
    default:
      break;
  }
}

/**
  * @brief  clear the crm reset flags
  * @param  flag
  *         this parameter can be one of the following values:
  *         reset flag:
  *         - CRM_NRST_RESET_FLAG
  *         - CRM_POR_RESET_FLAG
  *         - CRM_SW_RESET_FLAG
  *         - CRM_WDT_RESET_FLAG
  *         - CRM_WWDT_RESET_FLAG
  *         - CRM_LOWPOWER_RESET_FLAG
  *         - CRM_ALL_RESET_FLAG
  *         interrupt flag:
  *         - CRM_LICK_READY_INT_FLAG
  *         - CRM_LEXT_READY_INT_FLAG
  *         - CRM_HICK_READY_INT_FLAG
  *         - CRM_HEXT_READY_INT_FLAG
  *         - CRM_PLL_READY_INT_FLAG
  *         - CRM_CLOCK_FAILURE_INT_FLAG
  * @retval none
  */
void crm_flag_clear(uint32_t flag)
{
  switch(flag)
  {
    case CRM_NRST_RESET_FLAG:
    case CRM_POR_RESET_FLAG:
    case CRM_SW_RESET_FLAG:
    case CRM_WDT_RESET_FLAG:
    case CRM_WWDT_RESET_FLAG:
    case CRM_LOWPOWER_RESET_FLAG:
    case CRM_ALL_RESET_FLAG:
      CRM->ctrlsts_bit.rstfc = TRUE;
      break;
    case CRM_LICK_READY_INT_FLAG:
      CRM->clkint_bit.lickstblfc = TRUE;
      break;
    case CRM_LEXT_READY_INT_FLAG:
      CRM->clkint_bit.lextstblfc = TRUE;
      break;
    case CRM_HICK_READY_INT_FLAG:
      CRM->clkint_bit.hickstblfc = TRUE;
      break;
    case CRM_HEXT_READY_INT_FLAG:
      CRM->clkint_bit.hextstblfc = TRUE;
      break;
    case CRM_PLL_READY_INT_FLAG:
      CRM->clkint_bit.pllstblfc = TRUE;
      break;
    case CRM_CLOCK_FAILURE_INT_FLAG:
      CRM->clkint_bit.cfdfc = TRUE;
      break;
    default:
      break;
  }
}

/**
  * @brief  select rtc clock
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_RTC_CLOCK_LEXT
  *         - CRM_RTC_CLOCK_LICK
  *         - CRM_RTC_CLOCK_HEXT_DIV
  * @retval none
  */
void crm_rtc_clock_select(crm_rtc_clock_type value)
{
  CRM->bpdc_bit.rtcsel = value;
}

/**
  * @brief  enable or disable rtc
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_rtc_clock_enable(confirm_state new_state)
{
  CRM->bpdc_bit.rtcen = new_state;
}

/**
  * @brief  set crm ahb division
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_AHB_DIV_1
  *         - CRM_AHB_DIV_2
  *         - CRM_AHB_DIV_4
  *         - CRM_AHB_DIV_8
  *         - CRM_AHB_DIV_16
  *         - CRM_AHB_DIV_64
  *         - CRM_AHB_DIV_128
  *         - CRM_AHB_DIV_256
  *         - CRM_AHB_DIV_512
  * @retval none
  */
void crm_ahb_div_set(crm_ahb_div_type value)
{
  CRM->cfg_bit.ahbdiv = value;
}

/**
  * @brief  set crm apb1 division
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_APB1_DIV_1
  *         - CRM_APB1_DIV_2
  *         - CRM_APB1_DIV_4
  *         - CRM_APB1_DIV_8
  *         - CRM_APB1_DIV_16
  * @retval none
  */
void crm_apb1_div_set(crm_apb1_div_type value)
{
  CRM->cfg_bit.apb1div = value;
}

/**
  * @brief  set crm apb2 division
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_APB2_DIV_1
  *         - CRM_APB2_DIV_2
  *         - CRM_APB2_DIV_4
  *         - CRM_APB2_DIV_8
  *         - CRM_APB2_DIV_16
  * @retval none
  */
void crm_apb2_div_set(crm_apb2_div_type value)
{
  CRM->cfg_bit.apb2div = value;
}

/**
  * @brief  set crm adc division
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_ADC_DIV_2
  *         - CRM_ADC_DIV_4
  *         - CRM_ADC_DIV_6
  *         - CRM_ADC_DIV_8
  *         - CRM_ADC_DIV_12
  *         - CRM_ADC_DIV_16
  * @retval none
  */
void crm_adc_clock_div_set(crm_adc_div_type div_value)
{
  CRM->cfg_bit.adcdiv_l = div_value & 0x03;
  CRM->cfg_bit.adcdiv_h = (div_value >> 2) & 0x01;
}

/**
  * @brief  set crm usb division
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_USB_DIV_1_5
  *         - CRM_USB_DIV_1
  *         - CRM_USB_DIV_2_5
  *         - CRM_USB_DIV_2
  *         - CRM_USB_DIV_3_5
  *         - CRM_USB_DIV_3
  *         - CRM_USB_DIV_4
  * @retval none
  */
void crm_usb_clock_div_set(crm_usb_div_type div_value)
{
  CRM->cfg_bit.usbdiv_l = div_value & 0x03;
  CRM->cfg_bit.usbdiv_h = (div_value >> 2) & 0x01;
}

/**
  * @brief  enable or disable clock failure detection
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_clock_failure_detection_enable(confirm_state new_state)
{
  CRM->ctrl_bit.cfden = new_state;
}

/**
  * @brief  battery powered domain software reset
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_battery_powered_domain_reset(confirm_state new_state)
{
  CRM->bpdc_bit.bpdrst = new_state;
}

/**
  * @brief  config crm pll
  * @param  clock_source
  *         this parameter can be one of the following values:
  *         - CRM_PLL_SOURCE_HICK
  *         - CRM_PLL_SOURCE_HEXT
  *         - CRM_PLL_SOURCE_HEXT_DIV
  * @param  mult_value (CRM_PLL_MULT_2~64)
  * @param  pll_range
  *         this parameter can be one of the following values:
  *         - CRM_PLL_OUTPUT_RANGE_LE72MHZ
  *         - CRM_PLL_OUTPUT_RANGE_GT72MHZ
  * @retval none
  */
void crm_pll_config(crm_pll_clock_source_type clock_source, crm_pll_mult_type mult_value, crm_pll_output_range_type pll_range)
{
  /* config pll clock source */
  if(clock_source == CRM_PLL_SOURCE_HICK)
  {
    CRM->cfg_bit.pllrcs = FALSE;
  }
  else
  {
    CRM->cfg_bit.pllrcs = TRUE;
    if(CRM_PLL_SOURCE_HEXT == clock_source)
    {
      CRM->cfg_bit.pllhextdiv = FALSE;
    }
    else
    {
      CRM->cfg_bit.pllhextdiv = TRUE;
    }
  }

  /* config pll multiplication factor */
  CRM->cfg_bit.pllmult_l = (mult_value & 0x0F);
  CRM->cfg_bit.pllmult_h = ((mult_value & 0x30) >> 4);

  /* config pll output range */
  CRM->cfg_bit.pllrange = pll_range;
}

/**
  * @brief  select system clock source
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_SCLK_HICK
  *         - CRM_SCLK_HEXT
  *         - CRM_SCLK_PLL
  * @retval none
  */
void crm_sysclk_switch(crm_sclk_type value)
{
  CRM->cfg_bit.sclksel = value;
}

/**
  * @brief  indicate which clock source is used as system clock
  * @param  none
  * @retval crm_sclk
  *         this return can be one of the following values:
  *         - CRM_SCLK_HICK
  *         - CRM_SCLK_HEXT
  *         - CRM_SCLK_PLL
  */
crm_sclk_type crm_sysclk_switch_status_get(void)
{
  return (crm_sclk_type)CRM->cfg_bit.sclksts;
}

/**
  * @brief  get crm clocks freqency
  * @param  clocks
  *         - pointer to the crm_clocks_freq structure
  * @retval none
  */
void crm_clocks_freq_get(crm_clocks_freq_type *clocks_struct)
{
  uint32_t hext_prediv = 0, pll_mult = 0, pll_mult_h = 0, pll_clock_source = 0, temp = 0, div_value = 0;
  crm_sclk_type sclk_source;

  static const uint8_t sclk_ahb_div_table[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
  static const uint8_t ahb_apb1_div_table[8] = {0, 0, 0, 0, 1, 2, 3, 4};
  static const uint8_t ahb_apb2_div_table[8] = {0, 0, 0, 0, 1, 2, 3, 4};
  static const uint8_t adc_div_table[8] = {2, 4, 6, 8, 2, 12, 8, 16};

  /* get sclk source */
  sclk_source = crm_sysclk_switch_status_get();

  switch(sclk_source)
  {
    case CRM_SCLK_HICK:
      if(((CRM->misc3_bit.hick_to_sclk) != RESET) && ((CRM->misc1_bit.hickdiv) != RESET))
        clocks_struct->sclk_freq = HICK_VALUE * 6;
      else
        clocks_struct->sclk_freq = HICK_VALUE;
      break;
    case CRM_SCLK_HEXT:
      clocks_struct->sclk_freq = HEXT_VALUE;
      break;
    case CRM_SCLK_PLL:
      pll_clock_source = CRM->cfg_bit.pllrcs;
      /* get multiplication factor */
      pll_mult = CRM->cfg_bit.pllmult_l;
      pll_mult_h = CRM->cfg_bit.pllmult_h;

      /* process high bits */
      if((pll_mult_h != 0U) || (pll_mult == 15U))
      {
          pll_mult += ((16U * pll_mult_h) + 1U);
      }
      else
      {
          pll_mult += 2U;
      }

      if (pll_clock_source == 0x00)
      {
        /* hick divided by 2 selected as pll clock entry */
        clocks_struct->sclk_freq = (HICK_VALUE >> 1) * pll_mult;
      }
      else
      {
        /* hext selected as pll clock entry */
        if (CRM->cfg_bit.pllhextdiv != RESET)
        {
          hext_prediv = CRM->misc3_bit.hextdiv;
          /* hext clock divided by 2 */
          clocks_struct->sclk_freq = (HEXT_VALUE / (hext_prediv + 2)) * pll_mult;
        }
        else
        {
          clocks_struct->sclk_freq = HEXT_VALUE * pll_mult;
        }
      }
      break;
    default:
      clocks_struct->sclk_freq = HICK_VALUE;
      break;
  }

  /* compute sclk, ahbclk, abp1clk apb2clk and adcclk frequencies */
  /* get ahb division */
  temp = CRM->cfg_bit.ahbdiv;
  div_value = sclk_ahb_div_table[temp];
  /* ahbclk frequency */
  clocks_struct->ahb_freq = clocks_struct->sclk_freq >> div_value;

  /* get apb1 division */
  temp = CRM->cfg_bit.apb1div;
  div_value = ahb_apb1_div_table[temp];
  /* apb1clk frequency */
  clocks_struct->apb1_freq = clocks_struct->ahb_freq >> div_value;

  /* get apb2 division */
  temp = CRM->cfg_bit.apb2div;
  div_value = ahb_apb2_div_table[temp];
  /* apb2clk frequency */
  clocks_struct->apb2_freq = clocks_struct->ahb_freq >> div_value;

  /* get adc division */
  temp = CRM->cfg_bit.adcdiv_h;
  temp = ((temp << 2) | (CRM->cfg_bit.adcdiv_l));
  div_value = adc_div_table[temp];
  /* adcclk clock frequency */
  clocks_struct->adc_freq = clocks_struct->apb2_freq / div_value;
}

/**
  * @brief  set crm clkout
  * @param  clkout
  *         this parameter can be one of the following values:
  *         - CRM_CLKOUT_NOCLK
  *         - CRM_CLKOUT_LICK
  *         - CRM_CLKOUT_LEXT
  *         - CRM_CLKOUT_SCLK
  *         - CRM_CLKOUT_HICK
  *         - CRM_CLKOUT_HEXT
  *         - CRM_CLKOUT_PLL_DIV_2
  *         - CRM_CLKOUT_PLL_DIV_4
  *         - CRM_CLKOUT_USB
  *         - CRM_CLKOUT_ADC
  * @retval none
  */
void crm_clock_out_set(crm_clkout_select_type clkout)
{
  CRM->cfg_bit.clkout_sel = clkout & 0x7;
  CRM->misc1_bit.clkout_sel = (clkout >> 3) & 0x1;
}

/**
  * @brief  config crm interrupt
  * @param  int
  *         this parameter can be any combination of the following values:
  *         - CRM_LICK_STABLE_INT
  *         - CRM_LEXT_STABLE_INT
  *         - CRM_HICK_STABLE_INT
  *         - CRM_HEXT_STABLE_INT
  *         - CRM_PLL_STABLE_INT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_interrupt_enable(uint32_t crm_int, confirm_state new_state)
{
  if(new_state == TRUE)
    CRM->clkint |= crm_int;
  else
    CRM->clkint &= ~crm_int;
}

/**
  * @brief  auto step clock switch enable
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_auto_step_mode_enable(confirm_state new_state)
{
  if(new_state == TRUE)
    CRM->misc3_bit.auto_step_en = CRM_AUTO_STEP_MODE_ENABLE;
  else
    CRM->misc3_bit.auto_step_en = CRM_AUTO_STEP_MODE_DISABLE;
}

/**
  * @brief  usbdev interrupt remapping control
  * @param  int_remap
  *         this parameter can be one of the following values:
  *         - CRM_USB_INT19_INT20
  *         - CRM_USB_INT73_INT74
  * @retval none
  */
void crm_usb_interrupt_remapping_set(crm_usb_int_map_type int_remap)
{
  CRM->intmap_bit.usbintmap = int_remap;
}

/**
  * @brief  config hick divider select
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_HICK48_DIV6
  *         - CRM_HICK48_NODIV
  * @retval none
  */
void crm_hick_divider_select(crm_hick_div_6_type value)
{
  CRM->misc1_bit.hickdiv = value;
}

/**
  * @brief  hick as system clock frequency select
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_HICK_SCLK_8MHZ
  *         - CRM_HICK_SCLK_48MHZ
  * @retval none
  */
void crm_hick_sclk_frequency_select(crm_hick_sclk_frequency_type value)
{
  crm_hick_divider_select(CRM_HICK48_NODIV);
  CRM->misc3_bit.hick_to_sclk = value;
}

/**
  * @brief  usb 48 mhz clock source select
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_USB_CLOCK_SOURCE_PLL
  *         - CRM_USB_CLOCK_SOURCE_HICK
  * @retval none
  */
void crm_usb_clock_source_select(crm_usb_clock_source_type value)
{
  if(value == CRM_USB_CLOCK_SOURCE_HICK)
  {
    crm_hick_sclk_frequency_select(CRM_HICK_SCLK_48MHZ);
  }
  CRM->misc3_bit.hick_to_usb = value;
}

/**
  * @brief  enable or disable clkout direct to tmr10 channel 1
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_clkout_to_tmr10_enable(confirm_state new_state)
{
  CRM->misc2_bit.clk_to_tmr = new_state;
}

/**
  * @brief  set hext clock division
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_HEXT_DIV_2
  *         - CRM_HEXT_DIV_3
  *         - CRM_HEXT_DIV_4
  *         - CRM_HEXT_DIV_5
  * @retval none
  */
void crm_hext_clock_div_set(crm_hext_div_type value)
{
  CRM->misc3_bit.hextdiv = value;
}

/**
  * @brief  set crm clkout division
  * @param  clkout_div
  *         this parameter can be one of the following values:
  *         - CRM_CLKOUT_DIV_1
  *         - CRM_CLKOUT_DIV_2
  *         - CRM_CLKOUT_DIV_4
  *         - CRM_CLKOUT_DIV_8
  *         - CRM_CLKOUT_DIV_16
  *         - CRM_CLKOUT_DIV_64
  *         - CRM_CLKOUT_DIV_128
  *         - CRM_CLKOUT_DIV_256
  *         - CRM_CLKOUT_DIV_512
  * @retval none
  */
void crm_clkout_div_set(crm_clkout_div_type clkout_div)
{
  CRM->misc1_bit.clkoutdiv = clkout_div;
}

#if defined (AT32F407xx)
/**
  * @brief  set emac output pulse width
  * @param  width
  *         this parameter can be one of the following values:
  *         - CRM_EMAC_PULSE_125MS
  *         - CRM_EMAC_PULSE_1SCLK
  * @retval none
  */
void crm_emac_output_pulse_set(crm_emac_output_pulse_type width)
{
  CRM->misc3_bit.emac_pps_sel = width;
}
#endif

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
