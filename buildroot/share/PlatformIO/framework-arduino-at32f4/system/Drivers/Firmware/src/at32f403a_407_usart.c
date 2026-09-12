/**
  **************************************************************************
  * @file     at32f403a_407_usart.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the usart firmware library
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

/* includes ------------------------------------------------------------------*/
#include "at32f403a_407_conf.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @defgroup USART
  * @brief USART driver modules
  * @{
  */

#ifdef USART_MODULE_ENABLED

/** @defgroup USART_private_functions
  * @{
  */

/**
  * @brief  deinitialize the usart peripheral registers to their default reset values.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7,or UART8.
  * @retval none
  */
void usart_reset(usart_type* usart_x)
{
  if(usart_x == USART1)
  {
    crm_periph_reset(CRM_USART1_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_USART1_PERIPH_RESET, FALSE);
  }
  else if(usart_x == USART2)
  {
    crm_periph_reset(CRM_USART2_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_USART2_PERIPH_RESET, FALSE);
  }
  else if(usart_x == USART3)
  {
    crm_periph_reset(CRM_USART3_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_USART3_PERIPH_RESET, FALSE);
  }
  else if(usart_x == UART4)
  {
    crm_periph_reset(CRM_UART4_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_UART4_PERIPH_RESET, FALSE);
  }
  else if(usart_x == UART5)
  {
    crm_periph_reset(CRM_UART5_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_UART5_PERIPH_RESET, FALSE);
  }
  else if(usart_x == USART6)
  {
    crm_periph_reset(CRM_USART6_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_USART6_PERIPH_RESET, FALSE);
  }
  else if(usart_x == UART7)
  {
    crm_periph_reset(CRM_UART7_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_UART7_PERIPH_RESET, FALSE);
  }
  else if(usart_x == UART8)
  {
    crm_periph_reset(CRM_UART8_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_UART8_PERIPH_RESET, FALSE);
  }
}

/**
  * @brief  initialize the usart peripheral according to the specified parameters.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4 ,UART5, USART6, UART7 or UART8.
  * @param  baud_rate: configure the usart communication baud rate.
  * @param  data_bit: data bits transmitted or received in a frame
  *         this parameter can be one of the following values:
  *         - USART_DATA_8BITS
  *         - USART_DATA_9BITS.
  * @param  stop_bit: stop bits transmitted
  *         this parameter can be one of the following values:
  *         - USART_STOP_1_BIT
  *         - USART_STOP_0_5_BIT.
  *         - USART_STOP_2_BIT
  *         - USART_STOP_1_5_BIT.
  * @retval none
  */
void usart_init(usart_type* usart_x, uint32_t baud_rate, usart_data_bit_num_type data_bit, usart_stop_bit_num_type stop_bit)
{
  crm_clocks_freq_type clocks_freq;
  uint32_t apb_clock, temp_val;
  crm_clocks_freq_get(&clocks_freq);
  if((usart_x == USART1) || (usart_x == USART6) ||  (usart_x == UART7) || (usart_x == UART8))
  {
    apb_clock = clocks_freq.apb2_freq;
  }
  else
  {
    apb_clock = clocks_freq.apb1_freq;
  }
  temp_val = (apb_clock * 10 / baud_rate);
  if((temp_val % 10) < 5)
  {
    temp_val = (temp_val / 10);
  }
  else
  {
    temp_val = (temp_val / 10) + 1;
  }
  usart_x->baudr_bit.div = temp_val;
  usart_x->ctrl1_bit.dbn = data_bit;
  usart_x->ctrl2_bit.stopbn = stop_bit;
}

/**
  * @brief  usart parity selection config.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4 ,UART5, USART6, UART7 or UART8.
  * @param  parity: select the none, odd or even parity.
  *         this parameter can be one of the following values:
  *         - USART_PARITY_NONE
  *         - USART_PARITY_EVEN.
  *         - USART_PARITY_ODD
  * @retval none
  */
void usart_parity_selection_config(usart_type* usart_x, usart_parity_selection_type parity)
{
  if(parity == USART_PARITY_NONE)
  {
    usart_x->ctrl1_bit.psel = FALSE;
    usart_x->ctrl1_bit.pen = FALSE;
  }
  else if(parity == USART_PARITY_EVEN)
  {
    usart_x->ctrl1_bit.psel = FALSE;
    usart_x->ctrl1_bit.pen = TRUE;
  }
  else if(parity == USART_PARITY_ODD)
  {
    usart_x->ctrl1_bit.psel = TRUE;
    usart_x->ctrl1_bit.pen = TRUE;
  }
}

/**
  * @brief  enable or disable the specified usart peripheral.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  new_state: new state of the usart peripheral.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl1_bit.uen = new_state;
}

/**
  * @brief  usart transmitter enable.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4 ,UART5, USART6, UART7 or UART8.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void usart_transmitter_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl1_bit.ten = new_state;
}

/**
  * @brief  usart receiver enable.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4 ,UART5, USART6, UART7 or UART8.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void usart_receiver_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl1_bit.ren = new_state;
}

/**
  * @brief  usart clock config.
  * @note   clock config are not available for UART4, UART5, UART7 and UART8.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3 or USART6.
  * @param  clk_pol: polarity of the clock output on the ck pin.
  *         this parameter can be one of the following values:
  *         - USART_CLOCK_POLARITY_LOW
  *         - USART_CLOCK_POLARITY_HIGH
  * @param  clk_pha: phase of the clock output on the ck pin.
  *         this parameter can be one of the following values:
  *         - USART_CLOCK_PHASE_1EDGE
  *         - USART_CLOCK_PHASE_2EDGE
  * @param  clk_lb: whether the clock pulse of the last data bit transmitted (MSB) is outputted on the ck pin.
  *         this parameter can be one of the following values:
  *         - USART_CLOCK_LAST_BIT_NONE
  *         - USART_CLOCK_LAST_BIT_OUTPUT
  * @retval none
  */
void usart_clock_config(usart_type* usart_x, usart_clock_polarity_type clk_pol, usart_clock_phase_type clk_pha, usart_lbcp_type clk_lb)
{
  usart_x->ctrl2_bit.clkpol = clk_pol;
  usart_x->ctrl2_bit.clkpha = clk_pha;
  usart_x->ctrl2_bit.lbcp = clk_lb;
}

/**
  * @brief  usart enable the ck pin.
  * @note   clock enable are not available for UART4, UART5, UART7 and UART8.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3 or USART6.
  * @param  new_state: TRUE or FALSE
  * @retval none
  */
void usart_clock_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl2_bit.clken = new_state;
}

/**
  * @brief  enable or disable the specified usart interrupts.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  usart_int: specifies the USART interrupt sources to be enabled or disabled.
  *         this parameter can be one of the following values:
  *         - USART_IDLE_INT:  idle interrupt
  *         - USART_RDBF_INT:  rdbf interrupt
  *         - USART_TDC_INT:   tdc interrupt
  *         - USART_TDBE_INT:  tdbe interrupt
  *         - USART_PERR_INT:  perr interrupt
  *         - USART_BF_INT:    break frame interrupt
  *         - USART_ERR_INT:   err interrupt
  *         - USART_CTSCF_INT: ctscf interrupt
  * @param  new_state: new state of the specified usart interrupts.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_interrupt_enable(usart_type* usart_x, uint32_t usart_int, confirm_state new_state)
{
  if(new_state == TRUE)
    PERIPH_REG((uint32_t)usart_x, usart_int) |= PERIPH_REG_BIT(usart_int);
  else
    PERIPH_REG((uint32_t)usart_x, usart_int) &= ~PERIPH_REG_BIT(usart_int);
}

/**
  * @brief  enable or disable the usart's dma transmitter interface.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  new_state: new state of the dma request sources.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_dma_transmitter_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl3_bit.dmaten = new_state;
}

/**
  * @brief  enable or disable the usart's dma receiver interface.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  new_state: new state of the dma request sources.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_dma_receiver_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl3_bit.dmaren = new_state;
}

/**
  * @brief  set the wakeup id of the usart.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  usart_id: the matching id(0x0~0xF).
  * @retval none
  */
void usart_wakeup_id_set(usart_type* usart_x, uint8_t usart_id)
{
  usart_x->ctrl2_bit.id = usart_id;
}

/**
  * @brief  select the usart wakeup method in multi-processor communication.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  wakeup_mode: determines the way to wake up usart method.
  *         this parameter can be one of the following values:
  *         - USART_WAKEUP_BY_IDLE_FRAME
  *         - USART_WAKEUP_BY_MATCHING_ID
  * @retval none
  */
void usart_wakeup_mode_set(usart_type* usart_x, usart_wakeup_mode_type wakeup_mode)
{
  usart_x->ctrl1_bit.wum = wakeup_mode;
}

/**
  * @brief  config the usart in mute mode in multi-processor communication.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  new_state: new state of the usart mute mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_receiver_mute_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl1_bit.rm = new_state;
}

/**
  * @brief  set the usart break frame bit num.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  break_bit: specifies the break bit num.
  *         this parameter can be one of the following values:
  *         - USART_BREAK_10BITS
  *         - USART_BREAK_11BITS
  * @retval none
  */
void usart_break_bit_num_set(usart_type* usart_x, usart_break_bit_num_type break_bit)
{
  usart_x->ctrl2_bit.bfbn = break_bit;
}

/**
  * @brief  enable or disable the usart lin mode.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  new_state: new state of the usart lin mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_lin_mode_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl2_bit.linen = new_state;
}

/**
  * @brief  transmit single data through the usart peripheral.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  data: the data to transmit.
  * @retval none
  */
void usart_data_transmit(usart_type* usart_x, uint16_t data)
{
  usart_x->dt = (data & 0x01FF);
}

/**
  * @brief  return the most recent received data by the usart peripheral.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @retval the received data.
  */
uint16_t usart_data_receive(usart_type* usart_x)
{
  return (uint16_t)(usart_x->dt);
}

/**
  * @brief  transmit break characters.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @retval none
  */
void usart_break_send(usart_type* usart_x)
{
  usart_x->ctrl1_bit.sbf = TRUE;
}

/**
  * @brief  config the specified usart smartcard guard time.
  * @note   The guard time bits are not available for UART4, UART5, UART7 or UART8.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3 or USART6.
  * @param  guard_time_val: specifies the guard time (0x00~0xFF).
  * @retval none
  */
void usart_smartcard_guard_time_set(usart_type* usart_x, uint8_t guard_time_val)
{
  usart_x->gdiv_bit.scgt = guard_time_val;
}

/**
  * @brief  config the irda/smartcard division.
  * @note   the division are not available for UART4, UART5, UART7 or UART8.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3 or USART6.
  * @param  div_val: specifies the division.
  * @retval none
  */
void usart_irda_smartcard_division_set(usart_type* usart_x, uint8_t div_val)
{
  usart_x->gdiv_bit.isdiv = div_val;
}

/**
  * @brief  enable or disable the usart smart card mode.
  * @note   the smart card mode are not available for UART4, UART5, UART7 or UART8.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3 or USART6.
  * @param  new_state: new state of the smart card mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_smartcard_mode_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl3_bit.scmen = new_state;
}

/**
  * @brief  enable or disable nack transmission in smartcard mode.
  * @note   the smart card nack are not available for UART4, UART5, UART7 or UART8.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3 or USART6.
  * @param  new_state: new state of the nack transmission.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_smartcard_nack_set(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl3_bit.scnacken = new_state;
}

/**
  * @brief  enable or disable the usart single line bidirectional half-duplex communication.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  new_state: new state of the single line half-duplex select.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_single_line_halfduplex_select(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl3_bit.slben = new_state;
}

/**
  * @brief  enable or disable the usart's irda interface.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  new_state: new state of the irda mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_irda_mode_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl3_bit.irdaen = new_state;
}

/**
  * @brief  configure the usart's irda low power.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  new_state: new state of the irda mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void usart_irda_low_power_enable(usart_type* usart_x, confirm_state new_state)
{
  usart_x->ctrl3_bit.irdalp = new_state;
}

/**
  * @brief  configure the usart's hardware flow control.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3
  * @param  flow_state: specifies the hardware flow control.
  *         this parameter can be one of the following values:
  *         - USART_HARDWARE_FLOW_NONE
  *         - USART_HARDWARE_FLOW_RTS,
  *         - USART_HARDWARE_FLOW_CTS,
  *         - USART_HARDWARE_FLOW_RTS_CTS
  * @retval none
  */
void usart_hardware_flow_control_set(usart_type* usart_x,usart_hardware_flow_control_type flow_state)
{
  if(flow_state == USART_HARDWARE_FLOW_NONE)
  {
    usart_x->ctrl3_bit.rtsen = FALSE;
    usart_x->ctrl3_bit.ctsen = FALSE;
  }
  else if(flow_state == USART_HARDWARE_FLOW_RTS)
  {
    usart_x->ctrl3_bit.rtsen = TRUE;
    usart_x->ctrl3_bit.ctsen = FALSE;
  }
  else if(flow_state == USART_HARDWARE_FLOW_CTS)
  {
    usart_x->ctrl3_bit.rtsen = FALSE;
    usart_x->ctrl3_bit.ctsen = TRUE;
  }
  else if(flow_state == USART_HARDWARE_FLOW_RTS_CTS)
  {
    usart_x->ctrl3_bit.rtsen = TRUE;
    usart_x->ctrl3_bit.ctsen = TRUE;
  }
}

/**
  * @brief  check whether the specified usart flag is set or not.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - USART_CTSCF_FLAG: cts change flag (not available for UART4,UART5,USART6,UART7 and UART8)
  *         - USART_BFF_FLAG:   break frame flag
  *         - USART_TDBE_FLAG:  transmit data buffer empty flag
  *         - USART_TDC_FLAG:   transmit data complete flag
  *         - USART_RDBF_FLAG:  receive data buffer full flag
  *         - USART_IDLEF_FLAG: idle flag
  *         - USART_ROERR_FLAG: receiver overflow error flag
  *         - USART_NERR_FLAG:  noise error flag
  *         - USART_FERR_FLAG:  framing error flag
  *         - USART_PERR_FLAG:  parity error flag
  * @retval the new state of usart_flag (SET or RESET).
  */
flag_status usart_flag_get(usart_type* usart_x, uint32_t flag)
{
  if(usart_x->sts & flag)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  clear the usart's pending flags.
  * @param  usart_x: select the usart or the uart peripheral.
  *         this parameter can be one of the following values:
  *         USART1, USART2, USART3, UART4, UART5, USART6, UART7 or UART8.
  * @param  flag: specifies the flag to clear.
  *         this parameter can be any combination of the following values:
  *         - USART_CTSCF_FLAG: (not available for UART4,UART5,USART6,UART7 and UART8).
  *         - USART_BFF_FLAG:
  *         - USART_TDC_FLAG:
  *         - USART_RDBF_FLAG:
  * @note
  *         - USART_PERR_FLAG, USART_FERR_FLAG, USART_NERR_FLAG, USART_ROERR_FLAG and USART_IDLEF_FLAG are cleared by software
  *           sequence: a read operation to usart sts register (usart_flag_get())
  *           followed by a read operation to usart dt register (usart_data_receive()).
  *         - USART_RDBF_FLAG can be also cleared by a read to the usart dt register(usart_data_receive()).
  *         - USART_TDC_FLAG can be also cleared by software sequence: a read operation to usart sts register (usart_flag_get())
  *           followed by a write operation to usart dt register (usart_data_transmit()).
  *         - USART_TDBE_FLAG is cleared only by a write to the usart dt register(usart_data_transmit()).
  * @retval none
  */
void usart_flag_clear(usart_type* usart_x, uint32_t flag)
{
  usart_x->sts = ~flag;
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
