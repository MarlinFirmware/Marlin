/**
  **************************************************************************
  * @file     at32f403a_407_dac.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the dac firmware library
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

/** @defgroup DAC
  * @brief DAC driver modules
  * @{
  */

#ifdef DAC_MODULE_ENABLED

/** @defgroup DAC_private_functions
  * @{
  */

/**
  * @brief  dac reset
  * @param  none
  * @retval none
  */
void dac_reset(void)
{
  crm_periph_reset(CRM_DAC_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_DAC_PERIPH_RESET, FALSE);
}

/**
  * @brief  enable or disable dac
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void dac_enable(dac_select_type dac_select, confirm_state new_state)
{
  switch(dac_select)
  {
    case DAC1_SELECT:
      DAC->ctrl_bit.d1en = new_state;
      break;
    case DAC2_SELECT:
      DAC->ctrl_bit.d2en = new_state;
      break;
    default:
      break;
  }
}

/**
  * @brief  enable or disable dac output buffer
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void dac_output_buffer_enable(dac_select_type dac_select, confirm_state new_state)
{
  new_state = (confirm_state)!new_state;
  switch(dac_select)
  {
    case DAC1_SELECT:
      DAC->ctrl_bit.d1obdis = new_state;
      break;
    case DAC2_SELECT:
      DAC->ctrl_bit.d2obdis = new_state;
      break;
    default:
      break;
  }
}

/**
  * @brief  enable or disable dac trigger
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void dac_trigger_enable(dac_select_type dac_select, confirm_state new_state)
{
  switch(dac_select)
  {
    case DAC1_SELECT:
      DAC->ctrl_bit.d1trgen = new_state;
      break;
    case DAC2_SELECT:
      DAC->ctrl_bit.d2trgen = new_state;
      break;
    default:
      break;
  }
}

/**
  * @brief  select dac trigger
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @param  dac_trigger_source
  *         this parameter can be one of the following values:
  *         - DAC_TMR6_TRGOUT_EVENT
  *         - DAC_TMR8_TRGOUT_EVENT
  *         - DAC_TMR7_TRGOUT_EVENT
  *         - DAC_TMR5_TRGOUT_EVENT
  *         - DAC_TMR2_TRGOUT_EVENT
  *         - DAC_TMR4_TRGOUT_EVENT
  *         - DAC_EXTERNAL_INTERRUPT_LINE_9
  *         - DAC_SOFTWARE_TRIGGER
  * @retval none
  */
void dac_trigger_select(dac_select_type dac_select, dac_trigger_type dac_trigger_source)
{
  switch(dac_select)
  {
    case DAC1_SELECT:
      DAC->ctrl_bit.d1trgsel = dac_trigger_source;
      break;
    case DAC2_SELECT:
      DAC->ctrl_bit.d2trgsel = dac_trigger_source;
      break;
    default:
      break;
  }
}

/**
  * @brief  generate dac software trigger
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @retval none
  */
void dac_software_trigger_generate(dac_select_type dac_select)
{
  switch(dac_select)
  {
    case DAC1_SELECT:
      DAC->swtrg_bit.d1swtrg = TRUE;
      break;
    case DAC2_SELECT:
      DAC->swtrg_bit.d2swtrg = TRUE;
      break;
    default:
      break;
  }
}

/**
  * @brief  generate dac dual software trigger synchronously
  * @param  none
  * @retval none
  */
void dac_dual_software_trigger_generate(void)
{
  DAC->swtrg |= 0x03;
}

/**
  * @brief  generate dac wave
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @param  dac_wave
  *         this parameter can be one of the following values:
  *         - DAC_WAVE_GENERATE_NONE
  *         - DAC_WAVE_GENERATE_NOISE
  *         - DAC_WAVE_GENERATE_TRIANGLE
  * @retval none
  */
void dac_wave_generate(dac_select_type dac_select, dac_wave_type dac_wave)
{
  switch(dac_select)
  {
    case DAC1_SELECT:
      DAC->ctrl_bit.d1nm = dac_wave;
      break;
    case DAC2_SELECT:
      DAC->ctrl_bit.d2nm = dac_wave;
      break;
    default:
      break;
  }
}

/**
  * @brief  select dac mask amplitude
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @param  dac_mask_amplitude
  *         this parameter can be one of the following values:
  *         - DAC_LSFR_BIT0_AMPLITUDE_1
  *         - DAC_LSFR_BIT10_AMPLITUDE_3
  *         - DAC_LSFR_BIT20_AMPLITUDE_7
  *         - DAC_LSFR_BIT30_AMPLITUDE_15
  *         - DAC_LSFR_BIT40_AMPLITUDE_31
  *         - DAC_LSFR_BIT50_AMPLITUDE_63
  *         - DAC_LSFR_BIT60_AMPLITUDE_127
  *         - DAC_LSFR_BIT70_AMPLITUDE_255
  *         - DAC_LSFR_BIT80_AMPLITUDE_511
  *         - DAC_LSFR_BIT90_AMPLITUDE_1023
  *         - DAC_LSFR_BITA0_AMPLITUDE_2047
  *         - DAC_LSFR_BITB0_AMPLITUDE_4095
  * @retval none
  */
void dac_mask_amplitude_select(dac_select_type dac_select, dac_mask_amplitude_type dac_mask_amplitude)
{
  switch(dac_select)
  {
    case DAC1_SELECT:
      DAC->ctrl_bit.d1nbsel = dac_mask_amplitude;
      break;
    case DAC2_SELECT:
      DAC->ctrl_bit.d2nbsel = dac_mask_amplitude;
      break;
    default:
      break;
  }
}

/**
  * @brief  enable or disable dac dma
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void dac_dma_enable(dac_select_type dac_select, confirm_state new_state)
{
  switch(dac_select)
  {
    case DAC1_SELECT:
      DAC->ctrl_bit.d1dmaen = new_state;
      break;
    case DAC2_SELECT:
      DAC->ctrl_bit.d2dmaen = new_state;
      break;
    default:
      break;
  }
}

/**
  * @brief  get dac data output
  * @param  dac_select
  *         this parameter can be one of the following values:
  *         - DAC1_SELECT
  *         - DAC2_SELECT
  * @retval dac channel data output
  */
uint16_t dac_data_output_get(dac_select_type dac_select)
{
  uint16_t data_output =0 ;
  switch(dac_select)
  {
    case DAC1_SELECT:
      data_output = DAC->d1odt_bit.d1odt;
      break;
    case DAC2_SELECT:
      data_output = DAC->d2odt_bit.d2odt;
      break;
    default:
      break;
  }
  return data_output;
}

/**
  * @brief  set dac1 data
  * @param  dac1_aligned
  *         this parameter can be one of the following values:
  *         DAC1_12BIT_RIGHT
  *         DAC1_12BIT_LEFT
  *         DAC1_8BIT_RIGHT
  * @param  dac1_data :indecate from selected data holding register
  * @retval none
  */
void dac_1_data_set(dac1_aligned_data_type dac1_aligned, uint16_t dac1_data)
{
  *(__IO uint32_t *) dac1_aligned = dac1_data;
}

/**
  * @brief  set dac2 data
  * @param  dac2_aligned
  *         this parameter can be one of the following values:
  *         DAC2_12BIT_RIGHT
  *         DAC2_12BIT_LEFT
  *         DAC2_8BIT_RIGHT
  * @param  dac2_data :indecate from selected data holding register
  * @retval none
  */
void dac_2_data_set(dac2_aligned_data_type dac2_aligned, uint16_t dac2_data)
{
  *(__IO uint32_t *) dac2_aligned = dac2_data;
}

/**
  * @brief  set dac dual data
  * @param  dac_dual
  *         this parameter can be one of the following values:
  *         DAC_DUAL_12BIT_RIGHT
  *         DAC_DUAL_12BIT_LEFT
  *         DAC_DUAL_8BIT_RIGHT
  * @param  data1 :dac1 channel indecate from selected data holding register
  * @param  data2 :dac1 channel indecate from selected data holding register
  * @retval none
  */
void dac_dual_data_set(dac_dual_data_type dac_dual, uint16_t data1, uint16_t data2)
{
  switch(dac_dual)
  {
    case DAC_DUAL_12BIT_RIGHT:
      *(__IO uint32_t *) dac_dual = (uint32_t)(data1 | (data2 << 16));
      break;
    case DAC_DUAL_12BIT_LEFT:
      *(__IO uint32_t *) dac_dual = (uint32_t)(data1 | (data2 << 16));
      break;
    case DAC_DUAL_8BIT_RIGHT:
      *(__IO uint32_t *) dac_dual = (uint32_t)(data1 | (data2 << 8));
      break;
    default:
      break;
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
