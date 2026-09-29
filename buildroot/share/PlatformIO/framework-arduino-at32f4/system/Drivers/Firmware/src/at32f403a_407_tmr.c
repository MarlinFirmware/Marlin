/**
  **************************************************************************
  * @file     at32f403a_407_tmr.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the tmr firmware library
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

/** @defgroup TMR
  * @brief TMR driver modules
  * @{
  */

#ifdef TMR_MODULE_ENABLED

/** @defgroup TMR_private_functions
  * @{
  */

/**
  * @brief  tmr reset by crm reset register
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @retval none
  */
void tmr_reset(tmr_type *tmr_x)
{
  if(tmr_x == TMR1)
  {
    crm_periph_reset(CRM_TMR1_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR1_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR2)
  {
    crm_periph_reset(CRM_TMR2_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR2_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR3)
  {
    crm_periph_reset(CRM_TMR3_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR3_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR4)
  {
    crm_periph_reset(CRM_TMR4_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR4_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR5)
  {
    crm_periph_reset(CRM_TMR5_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR5_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR6)
  {
    crm_periph_reset(CRM_TMR6_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR6_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR7)
  {
    crm_periph_reset(CRM_TMR7_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR7_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR8)
  {
    crm_periph_reset(CRM_TMR8_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR8_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR9)
  {
    crm_periph_reset(CRM_TMR9_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR9_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR10)
  {
    crm_periph_reset(CRM_TMR10_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR10_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR11)
  {
    crm_periph_reset(CRM_TMR11_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR11_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR12)
  {
    crm_periph_reset(CRM_TMR12_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR12_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR13)
  {
    crm_periph_reset(CRM_TMR13_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR13_PERIPH_RESET, FALSE);
  }
  else if(tmr_x == TMR14)
  {
    crm_periph_reset(CRM_TMR14_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_TMR14_PERIPH_RESET, FALSE);
  }
}

/**
  * @brief  enable or disable tmr counter
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_counter_enable(tmr_type *tmr_x, confirm_state new_state)
{
  /* tmr counter enable  */
  tmr_x->ctrl1_bit.tmren = new_state;
}

/**
  * @brief  init tmr output default para
  * @param  tmr_output_struct
  *         - to the structure of tmr_output_config_type
  * @retval none
  */
void tmr_output_default_para_init(tmr_output_config_type *tmr_output_struct)
{
  tmr_output_struct->oc_mode = TMR_OUTPUT_CONTROL_OFF;
  tmr_output_struct->oc_idle_state = FALSE;
  tmr_output_struct->occ_idle_state = FALSE;
  tmr_output_struct->oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct->occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct->oc_output_state = FALSE;
  tmr_output_struct->occ_output_state = FALSE;
}

/**
  * @brief  init tmr input default para
  * @param  tmr_input_struct
  *         - to the structure of tmr_input_config_type
  * @retval none
  */
void tmr_input_default_para_init(tmr_input_config_type *tmr_input_struct)
{
  tmr_input_struct->input_channel_select = TMR_SELECT_CHANNEL_1;
  tmr_input_struct->input_polarity_select = TMR_INPUT_RISING_EDGE;
  tmr_input_struct->input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
  tmr_input_struct->input_filter_value = 0x0;
}

/**
  * @brief  init tmr brkdt default para
  * @param  tmr_brkdt_struct
  *         - to the structure of tmr_brkdt_config_type
  * @retval none
  */
void tmr_brkdt_default_para_init(tmr_brkdt_config_type *tmr_brkdt_struct)
{
  tmr_brkdt_struct->deadtime = 0x0;
  tmr_brkdt_struct->brk_polarity = TMR_BRK_INPUT_ACTIVE_LOW;
  tmr_brkdt_struct->wp_level = TMR_WP_OFF;
  tmr_brkdt_struct->auto_output_enable = FALSE ;
  tmr_brkdt_struct->fcsoen_state = FALSE ;
  tmr_brkdt_struct->fcsodis_state = FALSE ;
  tmr_brkdt_struct->brk_enable = FALSE ;
}

/**
  * @brief  init tmr base
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  tmr_pr (for 16 bit tmr 0x0000~0xFFFF,
  *                  for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @param  tmr_div (timer div value:0x0000~0xFFFF)
  * @retval none
  */
void tmr_base_init(tmr_type* tmr_x, uint32_t tmr_pr, uint32_t tmr_div)
{
  /* set the pr value */
  tmr_x->pr = tmr_pr;

  /* set the div value */
  tmr_x->div = tmr_div;

  /* trigger the overflow event to immediately reload pr value and div value */
  tmr_x->swevt_bit.ovfswtr = TRUE;
}

/**
  * @brief  set tmr clock source division
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_clock_div
  *         this parameter can be one of the following values:
  *         - TMR_CLOCK_DIV1
  *         - TMR_CLOCK_DIV2
  *         - TMR_CLOCK_DIV4
  * @retval none
  */
void tmr_clock_source_div_set(tmr_type *tmr_x, tmr_clock_division_type tmr_clock_div)
{
  /* set tmr clock source division */
  tmr_x->ctrl1_bit.clkdiv = tmr_clock_div;
}

/**
  * @brief  set tmr counter count direction
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_cnt_dir
  *         this parameter can be one of the following values:
  *         - TMR_COUNT_UP
  *         - TMR_COUNT_DOWN
  *         - TMR_COUNT_TWO_WAY_1
  *         - TMR_COUNT_TWO_WAY_2
  *         - TMR_COUNT_TWO_WAY_3
  * @retval none
  */
void tmr_cnt_dir_set(tmr_type *tmr_x, tmr_count_mode_type tmr_cnt_dir)
{
  /* set the cnt direct */
  tmr_x->ctrl1_bit.cnt_dir = tmr_cnt_dir;
}

/**
  * @brief  set the repetition counter register(rpr) value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8
  * @param  tmr_rpr_value (0x00~0xFF)
  * @retval none
  */
void tmr_repetition_counter_set(tmr_type *tmr_x, uint8_t tmr_rpr_value)
{
  /* set the repetition counter value */
  if((tmr_x == TMR1) || (tmr_x == TMR8))

  {
    tmr_x->rpr_bit.rpr = tmr_rpr_value;
  }
}

/**
  * @brief  set tmr counter value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  tmr_cnt_value (for 16 bit tmr 0x0000~0xFFFF,
  *                        for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @retval none
  */
void tmr_counter_value_set(tmr_type *tmr_x, uint32_t tmr_cnt_value)
{
  /* set the tmr counter value */
  tmr_x->cval = tmr_cnt_value;
}

/**
  * @brief  get tmr counter value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @retval tmr counter value
  */
uint32_t tmr_counter_value_get(tmr_type *tmr_x)
{
  return tmr_x->cval;
}

/**
  * @brief  set tmr div value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  tmr_div_value (for 16 bit tmr 0x0000~0xFFFF,
  *                        for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @retval none
  */
void tmr_div_value_set(tmr_type *tmr_x, uint32_t tmr_div_value)
{
  /* set the tmr div value */
  tmr_x->div = tmr_div_value;
}

/**
  * @brief  get tmr div value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @retval tmr div value
  */
uint32_t tmr_div_value_get(tmr_type *tmr_x)
{
  return tmr_x->div;
}

/**
  * @brief  config tmr output channel
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  tmr_output_struct
  *         - to the structure of tmr_output_config_type
  * @retval none
  */
void tmr_output_channel_config(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                               tmr_output_config_type *tmr_output_struct)
{
  uint16_t channel_index = 0, channel_c_index = 0, channel = 0;

  /* get channel idle state bit position in ctrl2 register */
  channel_index = (uint16_t)(tmr_output_struct->oc_idle_state << (8 + tmr_channel));

  /* get channel complementary idle state bit position in ctrl2 register */
  channel_c_index = (uint16_t)(tmr_output_struct->occ_idle_state << (9 + tmr_channel));

  if((tmr_x == TMR1) || (tmr_x == TMR8))
  {
    /* set output channel complementary idle state */
    tmr_x->ctrl2 &= ~channel_c_index;
    tmr_x->ctrl2 |= channel_c_index;
  }

  /* set output channel idle state */
  tmr_x->ctrl2 &= ~channel_index;
  tmr_x->ctrl2 |= channel_index;

  /* set channel output mode */
  channel = tmr_channel;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cm1_output_bit.c1octrl = tmr_output_struct->oc_mode;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cm1_output_bit.c2octrl = tmr_output_struct->oc_mode;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cm2_output_bit.c3octrl = tmr_output_struct->oc_mode;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cm2_output_bit.c4octrl = tmr_output_struct->oc_mode;
      break;

    default:
      break;
  }

  /* get channel polarity bit position in cctrl register */
  channel_index = (uint16_t)(tmr_output_struct->oc_polarity << ((tmr_channel * 2) + 1));

  /* get channel complementary polarity bit position in cctrl register */
  channel_c_index = (uint16_t)(tmr_output_struct->occ_polarity << ((tmr_channel * 2) + 3));

  if((tmr_x == TMR1) || (tmr_x == TMR8))
  {
    /* set output channel complementary polarity */
    tmr_x->cctrl &= ~channel_c_index;
    tmr_x->cctrl |= channel_c_index;
  }

  /* set output channel polarity */
  tmr_x->cctrl &= ~channel_index;
  tmr_x->cctrl |= channel_index;

  /* get channel enable bit position in cctrl register */
  channel_index = (uint16_t)(tmr_output_struct->oc_output_state << (tmr_channel * 2));

  /* get channel complementary enable bit position in cctrl register */
  channel_c_index = (uint16_t)(tmr_output_struct->occ_output_state << ((tmr_channel * 2) + 2));

  if((tmr_x == TMR1) || (tmr_x == TMR8))
  {
    /* set output channel complementary enable bit */
    tmr_x->cctrl &= ~channel_c_index;
    tmr_x->cctrl |= channel_c_index;
  }

  /* set output channel enable bit */
  tmr_x->cctrl &= ~channel_index;
  tmr_x->cctrl |= channel_index;
}

/**
  * @brief  select tmr output channel mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  oc_mode
  *         this parameter can be one of the following values:
  *         - TMR_OUTPUT_CONTROL_OFF
  *         - TMR_OUTPUT_CONTROL_HIGH
  *         - TMR_OUTPUT_CONTROL_LOW
  *         - TMR_OUTPUT_CONTROL_SWITCH
  *         - TMR_OUTPUT_CONTROL_FORCE_HIGH
  *         - TMR_OUTPUT_CONTROL_FORCE_LOW
  *         - TMR_OUTPUT_CONTROL_PWM_MODE_A
  *         - TMR_OUTPUT_CONTROL_PWM_MODE_B
  * @retval none
  */
void tmr_output_channel_mode_select(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                                    tmr_output_control_mode_type oc_mode)
{
  uint16_t channel;

  channel = tmr_channel;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cm1_output_bit.c1octrl = oc_mode;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cm1_output_bit.c2octrl = oc_mode;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cm2_output_bit.c3octrl = oc_mode;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cm2_output_bit.c4octrl = oc_mode;
      break;

    default:
      break;
  }
}
/**
  * @brief  set tmr period value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  tmr_pr_value: timer period register value of counter
  *                       (for 16 bit tmr 0x0000~0xFFFF,
  *                       for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @retval none
  */
void tmr_period_value_set(tmr_type *tmr_x, uint32_t tmr_pr_value)
{
  /* set tmr period value */
  tmr_x->pr = tmr_pr_value;
}

/**
  * @brief  get tmr period value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @retval timer period register value of counter
  *         (for 16 bit tmr 0x0000~0xFFFF, for 32 bit tmr
  *         0x0000_0000~0xFFFF_FFFF)
  */
uint32_t tmr_period_value_get(tmr_type *tmr_x)
{
  return tmr_x->pr;
}

/**
  * @brief  set tmr channel value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  tmr_channel_value (for 16 bit tmr 0x0000~0xFFFF,
  *                       for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @retval none
  */
void tmr_channel_value_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                           uint32_t tmr_channel_value)
{
  uint16_t channel;

  channel = tmr_channel;

  /* set tmr channel value */
  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->c1dt = tmr_channel_value;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->c2dt = tmr_channel_value;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->c3dt = tmr_channel_value;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->c4dt = tmr_channel_value;
      break;

    default:
      break;
  }
}

/**
  * @brief  get tmr channel value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @retval tmr channel value
  */
uint32_t tmr_channel_value_get(tmr_type *tmr_x, tmr_channel_select_type tmr_channel)
{
  uint32_t cc_value_get = 0;
  uint16_t channel;

  channel = tmr_channel;

  /* get tmr channel value */
  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      cc_value_get = tmr_x->c1dt;
      break;

    case TMR_SELECT_CHANNEL_2:
      cc_value_get = tmr_x->c2dt;
      break;

    case TMR_SELECT_CHANNEL_3:
      cc_value_get = tmr_x->c3dt;
      break;

    case TMR_SELECT_CHANNEL_4:
      cc_value_get = tmr_x->c4dt;
      break;

    default:
      break;
  }

  return cc_value_get;
}
/**
  * @brief  enable tmr period buffer
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_period_buffer_enable(tmr_type *tmr_x, confirm_state new_state)
{
  /* tmr period buffer set */
  tmr_x->ctrl1_bit.prben = new_state;
}

/**
  * @brief  enable tmr output channel buffer
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_output_channel_buffer_enable(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                                   confirm_state new_state)
{
  uint16_t channel;

  channel = tmr_channel;

  /* get tmr channel value */
  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cm1_output_bit.c1oben = new_state;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cm1_output_bit.c2oben = new_state;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cm2_output_bit.c3oben = new_state;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cm2_output_bit.c4oben = new_state;
      break;

    default:
      break;
  }
}

/**
  * @brief  set tmr output channel immediately
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_output_channel_immediately_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                                        confirm_state new_state)
{
  uint16_t channel;

  channel = tmr_channel;

  /* get tmr channel value */
  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cm1_output_bit.c1oien = new_state;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cm1_output_bit.c2oien = new_state;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cm2_output_bit.c3oien = new_state;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cm2_output_bit.c4oien = new_state;
      break;

    default:
      break;
  }
}

/**
  * @brief  set tmr output channel switch
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_output_channel_switch_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                                   confirm_state new_state)
{
  uint16_t channel;

  channel = tmr_channel;

  /* get tmr channel value */
  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cm1_output_bit.c1osen = new_state;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cm1_output_bit.c2osen = new_state;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cm2_output_bit.c3osen = new_state;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cm2_output_bit.c4osen = new_state;
      break;

    default:
      break;
  }
}

/**
  * @brief  enable or disable tmr one cycle mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8, TMR9, TMR12
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_one_cycle_mode_enable(tmr_type *tmr_x, confirm_state new_state)
{
  /* tmr one cycle mode enable */
  tmr_x->ctrl1_bit.ocmen = new_state;
}

/**
  * @brief  enable or disable  tmr 32 bit function(plus mode)
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR2, TMR5
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_32_bit_function_enable (tmr_type *tmr_x, confirm_state new_state)
{
  /* tmr 32 bit function(plus mode) enable,only for TMR2/TMR5 */
  if((tmr_x == TMR2) || (tmr_x == TMR5))
  {
    tmr_x->ctrl1_bit.pmen = new_state;
  }
}

/**
  * @brief  select tmr the overflow event sources
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_overflow_request_source_set(tmr_type *tmr_x, confirm_state new_state)
{
  tmr_x->ctrl1_bit.ovfs = new_state;
}

/**
  * @brief  enable or disable tmr overflow event generation
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_overflow_event_disable(tmr_type *tmr_x, confirm_state new_state)
{
  tmr_x->ctrl1_bit.ovfen = new_state;
}

/**
  * @brief  init tmr input channel
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  input_struct
  *         - to the structure of tmr_input_config_type
  * @param  divider_factor
  *         this parameter can be one of the following values:
  *         - TMR_CHANNEL_INPUT_DIV_1
  *         - TMR_CHANNEL_INPUT_DIV_2
  *         - TMR_CHANNEL_INPUT_DIV_4
  *         - TMR_CHANNEL_INPUT_DIV_8
  * @retval none
  */
void tmr_input_channel_init(tmr_type *tmr_x, tmr_input_config_type *input_struct,
                            tmr_channel_input_divider_type divider_factor)
{
  uint16_t channel = 0;

  /* get channel selected */
  channel = input_struct->input_channel_select;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cctrl_bit.c1p        = (uint32_t)input_struct->input_polarity_select;
      tmr_x->cctrl_bit.c1cp       = (input_struct->input_polarity_select & 0x2) >> 1;
      tmr_x->cm1_input_bit.c1c    = input_struct->input_mapped_select;
      tmr_x->cm1_input_bit.c1df   = input_struct->input_filter_value;
      tmr_x->cm1_input_bit.c1idiv = divider_factor;
      tmr_x->cctrl_bit.c1en       = TRUE;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cctrl_bit.c2p        = (uint32_t)input_struct->input_polarity_select;
      tmr_x->cctrl_bit.c2cp       = (input_struct->input_polarity_select & 0x2) >> 1;
      tmr_x->cm1_input_bit.c2c    = input_struct->input_mapped_select;
      tmr_x->cm1_input_bit.c2df   = input_struct->input_filter_value;
      tmr_x->cm1_input_bit.c2idiv = divider_factor;
      tmr_x->cctrl_bit.c2en       = TRUE;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cctrl_bit.c3p        = (uint32_t)input_struct->input_polarity_select;
      tmr_x->cctrl_bit.c3cp       = (input_struct->input_polarity_select & 0x2) >> 1;
      tmr_x->cm2_input_bit.c3c    = input_struct->input_mapped_select;
      tmr_x->cm2_input_bit.c3df   = input_struct->input_filter_value;
      tmr_x->cm2_input_bit.c3idiv = divider_factor;
      tmr_x->cctrl_bit.c3en       = TRUE;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cctrl_bit.c4p        = (uint32_t)input_struct->input_polarity_select;
      tmr_x->cm2_input_bit.c4c    = input_struct->input_mapped_select;
      tmr_x->cm2_input_bit.c4df   = input_struct->input_filter_value;
      tmr_x->cm2_input_bit.c4idiv = divider_factor;
      tmr_x->cctrl_bit.c4en       = TRUE;
      break;

    default:
      break;
  }
}

/**
  * @brief  tmr channel enable
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_1C
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_2C
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_3C
  *         - TMR_SELECT_CHANNEL_4
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_channel_enable(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, confirm_state new_state)
{
  uint16_t channel;

  channel = tmr_channel;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cctrl_bit.c1en         = new_state;
      break;

    case TMR_SELECT_CHANNEL_1C:
      tmr_x->cctrl_bit.c1cen        = new_state;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cctrl_bit.c2en         = new_state;
      break;

    case TMR_SELECT_CHANNEL_2C:
      tmr_x->cctrl_bit.c2cen        = new_state;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cctrl_bit.c3en         = new_state;
      break;

    case TMR_SELECT_CHANNEL_3C:
      tmr_x->cctrl_bit.c3cen        = new_state;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cctrl_bit.c4en         = new_state;
      break;

    default:
      break;
  }
}

/**
  * @brief  set tmr input channel filter
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  filter_value (0x0~0xf)
  * @retval none
  */
void tmr_input_channel_filter_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                                  uint16_t filter_value)
{
  uint16_t channel;

  channel = tmr_channel;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cm1_input_bit.c1df = filter_value;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cm1_input_bit.c2df = filter_value;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cm2_input_bit.c3df = filter_value;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cm2_input_bit.c4df = filter_value;
      break;

    default:
      break;
  }
}

/**
  * @brief  config tmr pwm input
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  input_struct
  *         - to the structure of tmr_input_config_type
  * @param  divider_factor
  *         this parameter can be one of the following values:
  *         - TMR_CHANNEL_INPUT_DIV_1
  *         - TMR_CHANNEL_INPUT_DIV_2
  *         - TMR_CHANNEL_INPUT_DIV_4
  *         - TMR_CHANNEL_INPUT_DIV_8
  * @retval none
  */
void tmr_pwm_input_config(tmr_type *tmr_x, tmr_input_config_type *input_struct,
                          tmr_channel_input_divider_type divider_factor)
{
  uint16_t channel = 0;

  /* get channel selected */
  channel = input_struct->input_channel_select;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      if(input_struct->input_polarity_select == TMR_INPUT_RISING_EDGE)
      {
        /* set channel polarity */
        tmr_x->cctrl_bit.c1p          = TMR_INPUT_RISING_EDGE;
        tmr_x->cctrl_bit.c2p          = TMR_INPUT_FALLING_EDGE;
      }
      else if(input_struct->input_polarity_select == TMR_INPUT_FALLING_EDGE)
      {
        /* set channel polarity */
        tmr_x->cctrl_bit.c1p          = TMR_INPUT_FALLING_EDGE;
        tmr_x->cctrl_bit.c2p          = TMR_INPUT_RISING_EDGE;
      }

      if(input_struct->input_mapped_select == TMR_CC_CHANNEL_MAPPED_DIRECT)
      {
        /* ic1 is mapped on ti1 */
        tmr_x->cm1_input_bit.c1c = TMR_CC_CHANNEL_MAPPED_DIRECT;

        /* ic1 is mapped on ti2 */
        tmr_x->cm1_input_bit.c2c = TMR_CC_CHANNEL_MAPPED_INDIRECT;
      }
      else if(input_struct->input_mapped_select == TMR_CC_CHANNEL_MAPPED_INDIRECT)
      {
        /* ic1 is mapped on ti1 */
        tmr_x->cm1_input_bit.c1c = TMR_CC_CHANNEL_MAPPED_INDIRECT;

        /* ic1 is mapped on ti2 */
        tmr_x->cm1_input_bit.c2c = TMR_CC_CHANNEL_MAPPED_DIRECT;
      }

      /* set input ch1 and ch2 filter value*/
      tmr_x->cm1_input_bit.c1df = input_struct->input_filter_value;
      tmr_x->cm1_input_bit.c2df = input_struct->input_filter_value;

      /*set input ch1 and ch2 divider value*/
      tmr_x->cm1_input_bit.c1idiv = divider_factor;
      tmr_x->cm1_input_bit.c2idiv = divider_factor;

      tmr_x->cctrl_bit.c1en = TRUE;
      tmr_x->cctrl_bit.c2en = TRUE;
      break;

    case TMR_SELECT_CHANNEL_2:
      if(input_struct->input_polarity_select == TMR_INPUT_RISING_EDGE)
      {
        /* set channel polarity */
        tmr_x->cctrl_bit.c2p          = TMR_INPUT_RISING_EDGE;
        tmr_x->cctrl_bit.c1p          = TMR_INPUT_FALLING_EDGE;
      }
      else if(input_struct->input_polarity_select == TMR_INPUT_FALLING_EDGE)
      {
        /* set channel polarity */
        tmr_x->cctrl_bit.c2p          = TMR_INPUT_FALLING_EDGE;
        tmr_x->cctrl_bit.c1p          = TMR_INPUT_RISING_EDGE;
      }

      if(input_struct->input_mapped_select == TMR_CC_CHANNEL_MAPPED_DIRECT)
      {
        /* set mapped direct */
        tmr_x->cm1_input_bit.c2c = TMR_CC_CHANNEL_MAPPED_DIRECT;
        tmr_x->cm1_input_bit.c1c = TMR_CC_CHANNEL_MAPPED_INDIRECT;
      }
      else if(input_struct->input_mapped_select == TMR_CC_CHANNEL_MAPPED_INDIRECT)
      {
        /* set mapped direct */
        tmr_x->cm1_input_bit.c2c = TMR_CC_CHANNEL_MAPPED_INDIRECT;
        tmr_x->cm1_input_bit.c1c = TMR_CC_CHANNEL_MAPPED_DIRECT;
      }

      /* set input ch1 and ch2 filter value*/
      tmr_x->cm1_input_bit.c1df = input_struct->input_filter_value;
      tmr_x->cm1_input_bit.c2df = input_struct->input_filter_value;

      /*set input ch1 and ch2 divider value*/
      tmr_x->cm1_input_bit.c1idiv = divider_factor;
      tmr_x->cm1_input_bit.c2idiv = divider_factor;

      tmr_x->cctrl_bit.c1en = TRUE;
      tmr_x->cctrl_bit.c2en = TRUE;
      break;

    default:
      break;
  }
}

/**
  * @brief  select tmr channel1 input
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8
  * @param  ti1_connect
  *         this parameter can be one of the following values:
  *         - TMR_CHANEL1_CONNECTED_C1IRAW
  *         - TMR_CHANEL1_2_3_CONNECTED_C1IRAW_XOR
  * @retval none
  */
void tmr_channel1_input_select(tmr_type *tmr_x, tmr_channel1_input_connected_type ti1_connect)
{
  tmr_x->ctrl2_bit.c1insel = ti1_connect;
}

/**
  * @brief  set tmr input channel divider
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  divider_factor
  *         this parameter can be one of the following values:
  *         - TMR_CHANNEL_INPUT_DIV_1
  *         - TMR_CHANNEL_INPUT_DIV_2
  *         - TMR_CHANNEL_INPUT_DIV_4
  *         - TMR_CHANNEL_INPUT_DIV_8
  * @retval none
  */
void tmr_input_channel_divider_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                                   tmr_channel_input_divider_type divider_factor)
{
  uint16_t channel;

  channel = tmr_channel;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cm1_input_bit.c1idiv = divider_factor;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cm1_input_bit.c2idiv = divider_factor;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cm2_input_bit.c3idiv = divider_factor;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cm2_input_bit.c4idiv = divider_factor;
      break;

    default:
      break;
  }
}

/**
  * @brief  select tmr primary mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8
  * @param  primary_mode
  *         this parameter can be one of the following values:
  *         - TMR_PRIMARY_SEL_RESET
  *         - TMR_PRIMARY_SEL_ENABLE
  *         - TMR_PRIMARY_SEL_OVERFLOW
  *         - TMR_PRIMARY_SEL_COMPARE
  *         - TMR_PRIMARY_SEL_C1ORAW
  *         - TMR_PRIMARY_SEL_C2ORAW
  *         - TMR_PRIMARY_SEL_C3ORAW
  *         - TMR_PRIMARY_SEL_C4ORAW
  * @retval none
  */
void tmr_primary_mode_select(tmr_type *tmr_x, tmr_primary_select_type primary_mode)
{
  tmr_x->ctrl2_bit.ptos = primary_mode;
}

/**
  * @brief  select tmr subordinate mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12
  * @param  sub_mode
  *         this parameter can be one of the following values:
  *         - TMR_SUB_MODE_DIABLE
  *         - TMR_SUB_ENCODER_MODE_A
  *         - TMR_SUB_ENCODER_MODE_B
  *         - TMR_SUB_ENCODER_MODE_C
  *         - TMR_SUB_RESET_MODE
  *         - TMR_SUB_HANG_MODE
  *         - TMR_SUB_TRIGGER_MODE
  *         - TMR_SUB_EXTERNAL_CLOCK_MODE_A
  * @retval none
  */
void tmr_sub_mode_select(tmr_type *tmr_x, tmr_sub_mode_select_type sub_mode)
{
  tmr_x->stctrl_bit.smsel = sub_mode;
}

/**
  * @brief  select tmr channel dma
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12
  * @param  cc_dma_select
  *         this parameter can be one of the following values:
  *         - TMR_DMA_REQUEST_BY_CHANNEL
  *         - TMR_DMA_REQUEST_BY_OVERFLOW
  * @retval none
  */
void tmr_channel_dma_select(tmr_type *tmr_x, tmr_dma_request_source_type cc_dma_select)
{
  tmr_x->ctrl2_bit.drs = cc_dma_select;
}

/**
  * @brief  select tmr hall
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_hall_select(tmr_type *tmr_x,  confirm_state new_state)
{
  tmr_x->ctrl2_bit.ccfs = new_state;
}

/**
  * @brief  enable tmr channel buffer
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_channel_buffer_enable(tmr_type *tmr_x, confirm_state new_state)
{
  tmr_x->ctrl2_bit.cbctrl = new_state;
}

/**
  * @brief  select tmr sub-trigger
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12
  * @param  trigger_select
  *         this parameter can be one of the following values:
  *         - TMR_SUB_INPUT_SEL_IS0
  *         - TMR_SUB_INPUT_SEL_IS1
  *         - TMR_SUB_INPUT_SEL_IS2
  *         - TMR_SUB_INPUT_SEL_IS3
  *         - TMR_SUB_INPUT_SEL_C1INC
  *         - TMR_SUB_INPUT_SEL_C1DF1
  *         - TMR_SUB_INPUT_SEL_C2DF2
  *         - TMR_SUB_INPUT_SEL_EXTIN
  * @retval none
  */
void tmr_trigger_input_select(tmr_type *tmr_x, sub_tmr_input_sel_type trigger_select)
{
  tmr_x->stctrl_bit.stis = trigger_select;
}

/**
  * @brief  set tmr subordinate synchronization mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_sub_sync_mode_set(tmr_type *tmr_x, confirm_state new_state)
{
  tmr_x->stctrl_bit.sts = new_state;
}

/**
  * @brief  enable or disable tmr dma request
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  dma_request
  *         this parameter can be one of the following values:
  *         - TMR_OVERFLOW_DMA_REQUEST
  *         - TMR_C1_DMA_REQUEST
  *         - TMR_C2_DMA_REQUEST
  *         - TMR_C3_DMA_REQUEST
  *         - TMR_C4_DMA_REQUEST
  *         - TMR_HALL_DMA_REQUEST
  *         - TMR_TRIGGER_DMA_REQUEST
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_dma_request_enable(tmr_type *tmr_x, tmr_dma_request_type dma_request, confirm_state new_state)
{
  if(new_state == TRUE)
  {
    tmr_x->iden |= dma_request;
  }
  else if(new_state == FALSE)
  {
    tmr_x->iden &= ~dma_request;
  }
}

/**
  * @brief  enable or disable tmr interrupt
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  tmr_interrupt
  *         this parameter can be one of the following values:
  *         - TMR_OVF_INT
  *         - TMR_C1_INT
  *         - TMR_C2_INT
  *         - TMR_C3_INT
  *         - TMR_C4_INT
  *         - TMR_HALL_INT
  *         - TMR_TRIGGER_INT
  *         - TMR_BRK_INT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_interrupt_enable(tmr_type *tmr_x, uint32_t tmr_interrupt, confirm_state new_state)
{
  if(new_state == TRUE)
  {
    tmr_x->iden |= tmr_interrupt;
  }
  else if(new_state == FALSE)
  {
    tmr_x->iden &= ~tmr_interrupt;
  }
}

/**
  * @brief  get tmr flag
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  tmr_flag
  *         this parameter can be one of the following values:
  *         - TMR_OVF_FLAG
  *         - TMR_C1_FLAG
  *         - TMR_C2_FLAG
  *         - TMR_C3_FLAG
  *         - TMR_C4_FLAG
  *         - TMR_HALL_FLAG
  *         - TMR_TRIGGER_FLAG
  *         - TMR_BRK_FLAG
  *         - TMR_C1_RECAPTURE_FLAG
  *         - TMR_C2_RECAPTURE_FLAG
  *         - TMR_C3_RECAPTURE_FLAG
  *         - TMR_C4_RECAPTURE_FLAG
  * @retval state of tmr flag
  */
flag_status tmr_flag_get(tmr_type *tmr_x, uint32_t tmr_flag)
{
  flag_status status = RESET;

  if((tmr_x->ists & tmr_flag) != RESET)
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
  * @brief  clear tmr flag
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  tmr_flag
  *         this parameter can be any combination of the following values:
  *         - TMR_OVF_FLAG
  *         - TMR_C1_FLAG
  *         - TMR_C2_FLAG
  *         - TMR_C3_FLAG
  *         - TMR_C4_FLAG
  *         - TMR_HALL_FLAG
  *         - TMR_TRIGGER_FLAG
  *         - TMR_BRK_FLAG
  *         - TMR_C1_RECAPTURE_FLAG
  *         - TMR_C2_RECAPTURE_FLAG
  *         - TMR_C3_RECAPTURE_FLAG
  *         - TMR_C4_RECAPTURE_FLAG
  * @retval none
  */
void tmr_flag_clear(tmr_type *tmr_x, uint32_t tmr_flag)
{
  tmr_x->ists = ~tmr_flag;
}

/**
  * @brief  generate tmr event
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14
  * @param  tmr_event
  *         this parameter can be one of the following values:
  *         - TMR_OVERFLOW_SWTRIG
  *         - TMR_C1_SWTRIG
  *         - TMR_C2_SWTRIG
  *         - TMR_C3_SWTRIG
  *         - TMR_C4_SWTRIG
  *         - TMR_HALL_SWTRIG
  *         - TMR_TRIGGER_SWTRIG
  *         - TMR_BRK_SWTRIG
  * @retval none
  */
void tmr_event_sw_trigger(tmr_type *tmr_x, tmr_event_trigger_type tmr_event)
{
  tmr_x->swevt |= tmr_event;
}

/**
  * @brief  tmr output enable(oen),this function is important for advtm output enable
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_output_enable(tmr_type *tmr_x, confirm_state new_state)
{
  tmr_x->brk_bit.oen = new_state;
}

/**
  * @brief  set tmr select internal clock
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12
  * @retval none
  */
void tmr_internal_clock_set(tmr_type *tmr_x)
{
  tmr_x->stctrl_bit.smsel = TMR_SUB_MODE_DIABLE;
}

/**
  * @brief  set tmr output channel polarity
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  *         - TMR_SELECT_CHANNEL_1C
  *         - TMR_SELECT_CHANNEL_2C
  *         - TMR_SELECT_CHANNEL_3C
  * @param  oc_polarity
  *         this parameter can be one of the following values:
  *         - TMR_POLARITY_ACTIVE_HIGH
  *         - TMR_POLARITY_ACTIVE_LOW
  * @retval none
  */
void tmr_output_channel_polarity_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel,
                                     tmr_polarity_active_type oc_polarity)
{
  uint16_t channel;

  channel = tmr_channel;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cctrl_bit.c1p = (uint32_t)oc_polarity;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cctrl_bit.c2p = (uint32_t)oc_polarity;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cctrl_bit.c3p = (uint32_t)oc_polarity;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cctrl_bit.c4p = (uint32_t)oc_polarity;
      break;

    case TMR_SELECT_CHANNEL_1C:
      tmr_x->cctrl_bit.c1cp = (uint32_t)oc_polarity;
      break;

    case TMR_SELECT_CHANNEL_2C:
      tmr_x->cctrl_bit.c2cp = (uint32_t)oc_polarity;
      break;

    case TMR_SELECT_CHANNEL_3C:
      tmr_x->cctrl_bit.c3cp = (uint32_t)oc_polarity;
      break;

    default:
      break;
  }
}

/**
  * @brief  config tmr external clock
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8
  * @param  es_divide
  *         this parameter can be one of the following values:
  *         - TMR_ES_FREQUENCY_DIV_1
  *         - TMR_ES_FREQUENCY_DIV_2
  *         - TMR_ES_FREQUENCY_DIV_4
  *         - TMR_ES_FREQUENCY_DIV_8
  * @param  es_polarity
  *         this parameter can be one of the following values:
  *         - TMR_ES_POLARITY_NON_INVERTED
  *         - TMR_ES_POLARITY_INVERTED
  * @param  es_filter (0x0~0xf)
  * @retval none
  */
void tmr_external_clock_config(tmr_type *tmr_x, tmr_external_signal_divider_type es_divide,
                               tmr_external_signal_polarity_type  es_polarity, uint16_t es_filter)
{
  tmr_x->stctrl_bit.esdiv = es_divide;
  tmr_x->stctrl_bit.esp = es_polarity;
  tmr_x->stctrl_bit.esf = es_filter;
}

/**
  * @brief  config tmr external clock mode1
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12
  * @param  es_divide
  *         this parameter can be one of the following values:
  *         - TMR_ES_FREQUENCY_DIV_1
  *         - TMR_ES_FREQUENCY_DIV_2
  *         - TMR_ES_FREQUENCY_DIV_4
  *         - TMR_ES_FREQUENCY_DIV_8
  * @param  es_polarity
  *         this parameter can be one of the following values:
  *         - TMR_ES_POLARITY_NON_INVERTED
  *         - TMR_ES_POLARITY_INVERTED
  * @param  es_filter (0x0~0xf)
  * @retval none
  */
void tmr_external_clock_mode1_config(tmr_type *tmr_x, tmr_external_signal_divider_type es_divide,
                                     tmr_external_signal_polarity_type  es_polarity, uint16_t es_filter)
{
  tmr_external_clock_config(tmr_x, es_divide, es_polarity, es_filter);
  tmr_x->stctrl_bit.smsel = TMR_SUB_EXTERNAL_CLOCK_MODE_A;
  tmr_x->stctrl_bit.stis = TMR_SUB_INPUT_SEL_EXTIN;
}

/**
  * @brief  config tmr external clock mode2
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8
  * @param  es_divide
  *         this parameter can be one of the following values:
  *         - TMR_ES_FREQUENCY_DIV_1
  *         - TMR_ES_FREQUENCY_DIV_2
  *         - TMR_ES_FREQUENCY_DIV_4
  *         - TMR_ES_FREQUENCY_DIV_8
  * @param  es_polarity
  *         this parameter can be one of the following values:
  *         - TMR_ES_POLARITY_NON_INVERTED
  *         - TMR_ES_POLARITY_INVERTED
  * @param  es_filter (0x0~0xf)
  * @retval none
  */
void tmr_external_clock_mode2_config(tmr_type *tmr_x, tmr_external_signal_divider_type es_divide,
                                     tmr_external_signal_polarity_type  es_polarity, uint16_t es_filter)
{
  tmr_external_clock_config(tmr_x, es_divide, es_polarity, es_filter);
  tmr_x->stctrl_bit.ecmben = TRUE;
}

/**
  * @brief  config tmr encoder mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8
  * @param  encoder_mode
  *         this parameter can be one of the following values:
  *         - TMR_ENCODER_MODE_A
  *         - TMR_ENCODER_MODE_B
  *         - TMR_ENCODER_MODE_C
  * @param  ic1_polarity
  *         this parameter can be one of the following values:
  *         - TMR_INPUT_RISING_EDGE
  *         - TMR_INPUT_FALLING_EDGE
  *         - TMR_INPUT_BOTH_EDGE
  * @param  ic2_polarity
  *         this parameter can be one of the following values:
  *         - TMR_INPUT_RISING_EDGE
  *         - TMR_INPUT_FALLING_EDGE
  *         - TMR_INPUT_BOTH_EDGE
  * @retval none
  */
void tmr_encoder_mode_config(tmr_type *tmr_x, tmr_encoder_mode_type encoder_mode, tmr_input_polarity_type
                             ic1_polarity, tmr_input_polarity_type ic2_polarity)
{
  tmr_x->stctrl_bit.smsel = encoder_mode;

  /* set ic1 polarity */
  tmr_x->cctrl_bit.c1p =  (ic1_polarity & 0x1);
  tmr_x->cctrl_bit.c1cp = (ic1_polarity >> 1);
  /* set ic1 as input channel */
  tmr_x->cm1_input_bit.c1c = TMR_CC_CHANNEL_MAPPED_DIRECT;

  /* set ic2 polarity */
  tmr_x->cctrl_bit.c2p =  (ic2_polarity & 0x1);
  tmr_x->cctrl_bit.c2cp = (ic2_polarity >> 1);
  /* set ic2 as input channel */
  tmr_x->cm1_input_bit.c2c = TMR_CC_CHANNEL_MAPPED_DIRECT;
}

/**
  * @brief  set tmr force output
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_SELECT_CHANNEL_1
  *         - TMR_SELECT_CHANNEL_2
  *         - TMR_SELECT_CHANNEL_3
  *         - TMR_SELECT_CHANNEL_4
  * @param  force_output
  *         this parameter can be one of the following values:
  *         - TMR_FORCE_OUTPUT_HIGH
  *         - TMR_FORCE_OUTPUT_LOW
  * @retval none
  */
void tmr_force_output_set(tmr_type *tmr_x,  tmr_channel_select_type tmr_channel,
                          tmr_force_output_type force_output)
{
  uint16_t channel;

  channel = tmr_channel;

  switch(channel)
  {
    case TMR_SELECT_CHANNEL_1:
      tmr_x->cm1_output_bit.c1octrl = force_output;
      break;

    case TMR_SELECT_CHANNEL_2:
      tmr_x->cm1_output_bit.c2octrl = force_output;
      break;

    case TMR_SELECT_CHANNEL_3:
      tmr_x->cm2_output_bit.c3octrl = force_output;
      break;

    case TMR_SELECT_CHANNEL_4:
      tmr_x->cm2_output_bit.c4octrl = force_output;
      break;

    default:
      break;
  }
}

/**
  * @brief  config tmr dma control
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8
  * @param  dma_length
  *         this parameter can be one of the following values:
  *         - TMR_DMA_TRANSFER_1BYTE
  *         - TMR_DMA_TRANSFER_2BYTES
  *         - TMR_DMA_TRANSFER_3BYTES
  *         ...
  *         - TMR_DMA_TRANSFER_17BYTES
  *         - TMR_DMA_TRANSFER_18BYTES
  * @param  dma_base_address
  *         this parameter can be one of the following values:
  *         - TMR_CTRL1_ADDRESS
  *         - TMR_CTRL2_ADDRESS
  *         - TMR_STCTRL_ADDRESS
  *         - TMR_IDEN_ADDRESS
  *         - TMR_ISTS_ADDRESS
  *         - TMR_SWEVT_ADDRESS
  *         - TMR_CM1_ADDRESS
  *         - TMR_CM2_ADDRESS
  *         - TMR_CCTRL_ADDRESS
  *         - TMR_CVAL_ADDRESS
  *         - TMR_DIV_ADDRESS
  *         - TMR_PR_ADDRESS
  *         - TMR_RPR_ADDRESS
  *         - TMR_C1DT_ADDRESS
  *         - TMR_C2DT_ADDRESS
  *         - TMR_C3DT_ADDRESS
  *         - TMR_C4DT_ADDRESS
  *         - TMR_BRK_ADDRESS
  *         - TMR_DMACTRL_ADDRESS
  * @retval none
  */
void tmr_dma_control_config(tmr_type *tmr_x, tmr_dma_transfer_length_type dma_length,
                            tmr_dma_address_type dma_base_address)
{
  tmr_x->dmactrl_bit.dtb = dma_length;
  tmr_x->dmactrl_bit.addr = dma_base_address;
}

/**
  * @brief  config tmr break mode and dead-time
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8
  * @param  brkdt_struct
  *         - to the structure of tmr_brkdt_config_type
  * @retval none
  */
void tmr_brkdt_config(tmr_type *tmr_x, tmr_brkdt_config_type *brkdt_struct)
{
  tmr_x->brk_bit.brken = brkdt_struct->brk_enable;
  tmr_x->brk_bit.dtc = brkdt_struct->deadtime;
  tmr_x->brk_bit.fcsodis = brkdt_struct->fcsodis_state;
  tmr_x->brk_bit.fcsoen = brkdt_struct->fcsoen_state;
  tmr_x->brk_bit.brkv = brkdt_struct->brk_polarity;
  tmr_x->brk_bit.aoen = brkdt_struct->auto_output_enable;
  tmr_x->brk_bit.wpc = brkdt_struct->wp_level;
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
