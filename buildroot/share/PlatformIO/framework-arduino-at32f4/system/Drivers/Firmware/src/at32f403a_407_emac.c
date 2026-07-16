/**
  **************************************************************************
  * @file     at32f403a_407_emac.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the emac firmware library
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

/** @defgroup EMAC
  * @brief EMAC driver modules
  * @{
  */

#ifdef EMAC_MODULE_ENABLED

/** @defgroup EMAC_private_functions
  * @{
  */

#if defined (EMAC_BASE)
/**
  * @brief global pointers on tx and rx descriptor used to track transmit and receive descriptors
  */
emac_dma_desc_type  *dma_tx_desc_to_set;
emac_dma_desc_type  *dma_rx_desc_to_get;

/* emac private function */
static void emac_delay(uint32_t delay);

/**
  * @brief  deinitialize the emac peripheral registers to their default reset values.
  * @param  none
  * @retval none
  */
void emac_reset(void)
{
  crm_periph_reset(CRM_EMAC_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_EMAC_PERIPH_RESET, FALSE);
}

/**
  * @brief  initialize emac control structure
  * @param  emac_control_config_type
  * @retval none
  */
void emac_control_para_init(emac_control_config_type *control_para)
{
  control_para->auto_nego = EMAC_AUTO_NEGOTIATION_OFF;
  control_para->auto_pad_crc_strip = FALSE;
  control_para->back_off_limit = EMAC_BACKOFF_LIMIT_0;
  control_para->carrier_sense_disable = FALSE;
  control_para->deferral_check = FALSE;
  control_para->duplex_mode = EMAC_HALF_DUPLEX;
  control_para->fast_ethernet_speed = EMAC_SPEED_10MBPS;
  control_para->interframe_gap = EMAC_INTERFRAME_GAP_96BIT;
  control_para->ipv4_checksum_offload = FALSE;
  control_para->jabber_disable = FALSE;
  control_para->loopback_mode = FALSE;
  control_para->receive_own_disable = FALSE;
  control_para->retry_disable = FALSE;
  control_para->watchdog_disable = FALSE;
}

/**
  * @brief  according to hclk to set mdc clock frequency.
  * @param  none
  * @retval none
  */
void emac_clock_range_set(void)
{
  uint8_t bits_value = 0;
  crm_clocks_freq_type clocks_freq = {0};

  /* clear clock range bits */
  EMAC->miiaddr_bit.cr = bits_value;

  crm_clocks_freq_get(&clocks_freq);

  if((clocks_freq.ahb_freq >= EMAC_HCLK_BORDER_20MHZ) && (clocks_freq.ahb_freq < EMAC_HCLK_BORDER_35MHZ))
  {
    bits_value = EMAC_CLOCK_RANGE_20_TO_35;
  }
  else if((clocks_freq.ahb_freq >= EMAC_HCLK_BORDER_35MHZ) && (clocks_freq.ahb_freq < EMAC_HCLK_BORDER_60MHZ))
  {
    bits_value = EMAC_CLOCK_RANGE_35_TO_60;
  }
  else if((clocks_freq.ahb_freq >= EMAC_HCLK_BORDER_60MHZ) && (clocks_freq.ahb_freq < EMAC_HCLK_BORDER_100MHZ))
  {
    bits_value = EMAC_CLOCK_RANGE_60_TO_100;
  }
  else if((clocks_freq.ahb_freq >= EMAC_HCLK_BORDER_100MHZ) && (clocks_freq.ahb_freq < EMAC_HCLK_BORDER_150MHZ))
  {
    bits_value = EMAC_CLOCK_RANGE_100_TO_150;
  }
  else if((clocks_freq.ahb_freq >= EMAC_HCLK_BORDER_150MHZ) && (clocks_freq.ahb_freq <= EMAC_HCLK_BORDER_240MHZ))
  {
    bits_value = EMAC_CLOCK_RANGE_150_TO_240;
  }

  EMAC->miiaddr_bit.cr = bits_value;
}

/**
  * @brief  configure emac control setting.
  * @param  control_struct: control setting of mac control register.
  * @retval none
  */
void emac_control_config(emac_control_config_type *control_struct)
{
  emac_deferral_check_set(control_struct->deferral_check);
  emac_backoff_limit_set(control_struct->back_off_limit);
  emac_auto_pad_crc_stripping_set(control_struct->auto_pad_crc_strip);
  emac_retry_disable(control_struct->retry_disable);
  emac_ipv4_checksum_offload_set(control_struct->ipv4_checksum_offload);
  emac_loopback_mode_enable(control_struct->loopback_mode);
  emac_receive_own_disable(control_struct->receive_own_disable);
  emac_carrier_sense_disable(control_struct->carrier_sense_disable);
  emac_interframe_gap_set(control_struct->interframe_gap);
  emac_jabber_disable(control_struct->jabber_disable);
  emac_watchdog_disable(control_struct->watchdog_disable);
}

/**
  * @brief  reset emac dma
  * @param  none
  * @retval none
  */
void emac_dma_software_reset_set(void)
{
  EMAC_DMA->bm_bit.swr = 1;
}

/**
  * @brief  get emac dma reset status
  * @param  none
  * @retval TRUE of FALSE
  */
flag_status emac_dma_software_reset_get(void)
{
  if(EMAC_DMA->bm_bit.swr)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  enable emac and dma reception/transmission
  * @param  none
  * @retval none
  */
void emac_start(void)
{
  /* enable transmit state machine of the mac for transmission on the mii */
  emac_trasmitter_enable(TRUE);

  /* flush transmit fifo */
  emac_dma_operations_set(EMAC_DMA_OPS_FLUSH_TRANSMIT_FIFO, TRUE);

  /* enable receive state machine of the mac for reception from the mii */
  emac_receiver_enable(TRUE);

  /* start dma transmission */
  emac_dma_operations_set(EMAC_DMA_OPS_START_STOP_TRANSMIT, TRUE);

  /* start dma reception */
  emac_dma_operations_set(EMAC_DMA_OPS_START_STOP_RECEIVE, TRUE);
}

/**
  * @brief  stop emac and dma reception/transmission
  * @param  none
  * @retval none
  */
void emac_stop(void)
{
  /* stop dma transmission */
  emac_dma_operations_set(EMAC_DMA_OPS_START_STOP_TRANSMIT, FALSE);

  /* stop dma reception */
  emac_dma_operations_set(EMAC_DMA_OPS_START_STOP_RECEIVE, FALSE);

  /* stop receive state machine of the mac for reception from the mii */
  emac_receiver_enable(FALSE);

  /* flush transmit fifo */
  emac_dma_operations_set(EMAC_DMA_OPS_FLUSH_TRANSMIT_FIFO, TRUE);

  /* stop transmit state machine of the mac for transmission on the mii */
  emac_trasmitter_enable(FALSE);
}

/**
  * @brief  write phy data.
  * @param  address: phy address.
  * @param  reg: register of phy.
  * @param  data: value that wants to write to phy.
  * @retval SUCCESS or ERROR
  */
error_status emac_phy_register_write(uint8_t address, uint8_t reg, uint16_t data)
{
  uint32_t timeout = 0;

  EMAC->miidt_bit.md = data;

  EMAC->miiaddr_bit.pa = address;
  EMAC->miiaddr_bit.mii = reg;
  EMAC->miiaddr_bit.mw = 1;
  EMAC->miiaddr_bit.mb = 1;

  do
  {
    timeout++;
  } while((EMAC->miiaddr_bit.mb) && (timeout < PHY_TIMEOUT));

  if(timeout == PHY_TIMEOUT)
  {
    return ERROR;
  }
  return SUCCESS;
}

/**
  * @brief  read phy data
  * @param  address: phy address.
  * @param  reg: register of phy.
  * @param  data: value that is read from phy.
  * @retval SUCCESS or ERROR
  */
error_status emac_phy_register_read(uint8_t address, uint8_t reg, uint16_t *data)
{
  uint32_t timeout = 0;

  EMAC->miiaddr_bit.pa = address;
  EMAC->miiaddr_bit.mii = reg;
  EMAC->miiaddr_bit.mw = 0;
  EMAC->miiaddr_bit.mb = 1;

  do
  {
    timeout++;
    *data = EMAC->miidt_bit.md;
  } while((EMAC->miiaddr_bit.mb) && (timeout < PHY_TIMEOUT));

  if(timeout == PHY_TIMEOUT)
  {
    return ERROR;
  }

  *data = EMAC->miidt_bit.md;
  return SUCCESS;
}

/**
  * @brief  emac receiver enable.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_receiver_enable(confirm_state new_state)
{
  __IO uint32_t temp = 0;

  EMAC->ctrl_bit.re = new_state;

  temp = EMAC->ctrl;
  emac_delay(1);
  EMAC->ctrl = temp;
}

/**
  * @brief  emac transmitter enable.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_trasmitter_enable(confirm_state new_state)
{
  __IO uint32_t temp = 0;

  EMAC->ctrl_bit.te = new_state;

  temp = EMAC->ctrl;
  emac_delay(1);
  EMAC->ctrl = temp;
}

/**
  * @brief  emac defferal check enable, only avalible in half-duplex mode.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_deferral_check_set(confirm_state new_state)
{
  EMAC->ctrl_bit.dc = new_state;
}

/**
  * @brief  emac back-off limit, only avalible in half-duplex mode.
  * @param  slot_time: waiting time of retransmission after collision
  *         this parameter can be one of the following values:
  *         - EMAC_BACKOFF_LIMIT_0
  *         - EMAC_BACKOFF_LIMIT_1
  *         - EMAC_BACKOFF_LIMIT_2
  *         - EMAC_BACKOFF_LIMIT_3
  * @retval none
  */
void emac_backoff_limit_set(emac_bol_type slot_time)
{
  EMAC->ctrl_bit.bl = slot_time;
}

/**
  * @brief  set mac automatic pad/CRC stripping.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_auto_pad_crc_stripping_set(confirm_state new_state)
{
  EMAC->ctrl_bit.acs = new_state;
}

/**
  * @brief  transmittion retry disable.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_retry_disable(confirm_state new_state)
{
  EMAC->ctrl_bit.dr = new_state;
}

/**
  * @brief  set ipv4 checksum offload.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ipv4_checksum_offload_set(confirm_state new_state)
{
  EMAC->ctrl_bit.ipc = new_state;
}

/**
  * @brief  enable loopback mode.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_loopback_mode_enable(confirm_state new_state)
{
  EMAC->ctrl_bit.lm = new_state;
}

/**
  * @brief  receive own disable.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_receive_own_disable(confirm_state new_state)
{
  EMAC->ctrl_bit.dro = new_state;
}

/**
  * @brief  carrier sense disbale.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_carrier_sense_disable(confirm_state new_state)
{
  EMAC->ctrl_bit.dcs = new_state;
}

/**
  * @brief  set minimum interframe gap between frames during transmission.
  * @param  number: interframe gap number.
  *         this parameter can be one of the following values:
  *         - EMAC_FRAME_GAP_96BIT
  *         - EMAC_FRAME_GAP_88BIT
  *         - EMAC_FRAME_GAP_80BIT
  *         - EMAC_FRAME_GAP_72BIT
  *         - EMAC_FRAME_GAP_64BIT
  *         - EMAC_FRAME_GAP_56BIT
  *         - EMAC_FRAME_GAP_48BIT
  *         - EMAC_FRAME_GAP_40BIT
  * @retval none
  */
void emac_interframe_gap_set(emac_intergrame_gap_type number)
{
  EMAC->ctrl_bit.ifg = number;
}

/**
  * @brief  jabber disable.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_jabber_disable(confirm_state new_state)
{
  EMAC->ctrl_bit.jd = new_state;
}

/**
  * @brief  watchdog disable.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_watchdog_disable(confirm_state new_state)
{
  EMAC->ctrl_bit.wd = new_state;
}

/**
  * @brief  set mac fast emac speed.
  * @param  speed: mac bandwidth
  *         this parameter can be one of the following values:
  *         - EMAC_SPEED_10MBPS
  *         - EMAC_SPEED_100MBPS
  * @retval none
  */
void emac_fast_speed_set(emac_speed_type speed)
{
  EMAC->ctrl_bit.fes = speed;
}

/**
  * @brief  set duplex mode.
  * @param  duplex_mode: communication mode
  *         this parameter can be one of the following values:
  *         - EMAC_HALF_DUPLEX
  *         - EMAC_FULL_DUPLEX
  * @retval none
  */
void emac_duplex_mode_set(emac_duplex_type duplex_mode)
{
  EMAC->ctrl_bit.dm = duplex_mode;
}

/**
  * @brief  set mac promiscuous mode.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_promiscuous_mode_set(confirm_state new_state)
{
  EMAC->frmf_bit.pr = new_state;
}

/**
  * @brief  hash unicast.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_hash_unicast_set(confirm_state new_state)
{
  EMAC->frmf_bit.huc = new_state;
}

/**
  * @brief  hash multicast.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_hash_multicast_set(confirm_state new_state)
{
  EMAC->frmf_bit.hmc = new_state;
}

/**
  * @brief  destination address inverse filtering.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_dstaddr_inverse_filter_set(confirm_state new_state)
{
  EMAC->frmf_bit.daif = new_state;
}

/**
  * @brief  pass all multicasting frames.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_pass_all_multicasting_set(confirm_state new_state)
{
  EMAC->frmf_bit.pmc = new_state;
}

/**
  * @brief  broadcast frames disable.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_broadcast_frames_disable(confirm_state new_state)
{
  EMAC->frmf_bit.dbf = new_state;
}

/**
  * @brief  set mac how to pass control frames.
  * @param  condition: set what control frame can pass filter.
  *         this parameter can be one of the following values:
  *         - EMAC_CONTROL_FRAME_PASSING_NO
  *         - EMAC_CONTROL_FRAME_PASSING_ALL
  *         - EMAC_CONTROL_FRAME_PASSING_MATCH
  * @retval none
  */
void emac_pass_control_frames_set(emac_control_frames_filter_type condition)
{
  EMAC->frmf_bit.pcf = condition;
}

/**
  * @brief  source address inverse filtering.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_srcaddr_inverse_filter_set(confirm_state new_state)
{
  EMAC->frmf_bit.saif = new_state;
}

/**
  * @brief  source address filtering.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_srcaddr_filter_set(confirm_state new_state)
{
  EMAC->frmf_bit.saf = new_state;
}

/**
  * @brief  mac uses hash or perfect filter.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_hash_perfect_filter_set(confirm_state new_state)
{
  EMAC->frmf_bit.hpf = new_state;
}

/**
  * @brief  mac receives all frames.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_receive_all_set(confirm_state new_state)
{
  EMAC->frmf_bit.ra = new_state;
}

/**
  * @brief  hash table high 32-bit.
  * @param  high32bits: the highest 32-bit of hash table.
  * @retval none
  */
void emac_hash_table_high32bits_set(uint32_t high32bits)
{
  EMAC->hth_bit.hth = high32bits;
}

/**
  * @brief  hash table low 32-bit.
  * @param  low32bits: the lowest 32-bit of hash table.
  * @retval none
  */
void emac_hash_table_low32bits_set(uint32_t low32bits)
{
  EMAC->htl_bit.htl = low32bits;
}

/**
  * @brief  mii busy status.
  * @param  none
  * @retval SET or RESET
  */
flag_status emac_mii_busy_get(void)
{
  if(EMAC->miiaddr_bit.mb) {
    return SET;
  }
  else {
    return RESET;
  }
}

/**
  * @brief  tell phy that will be written.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_mii_write(confirm_state new_state)
{
  EMAC->miiaddr_bit.mw = new_state;
}

/**
  * @brief  set flow control busy in full-duplex mode, back pressure activate in half-duplex mode.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_fcb_bpa_set(confirm_state new_state)
{
  EMAC->fctrl_bit.fcbbpa = new_state;
}

/**
  * @brief  set transmit flow control.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_transmit_flow_control_enable(confirm_state new_state)
{
  EMAC->fctrl_bit.etf = new_state;
}

/**
  * @brief  set receive flow control.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_receive_flow_control_enable(confirm_state new_state)
{
  EMAC->fctrl_bit.erf = new_state;
}

/**
  * @brief  set unicast pause frame detect.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_unicast_pause_frame_detect(confirm_state new_state)
{
  EMAC->fctrl_bit.dup = new_state;
}

/**
  * @brief  set pause low threshold.
  * @param  pasue_threshold: pause slot time.
  *         this parameter can be one of the following values:
  *         - EMAC_PAUSE_4_SLOT_TIME
  *         - EMAC_PAUSE_28_SLOT_TIME
  *         - EMAC_PAUSE_144_SLOT_TIME
  *         - EMAC_PAUSE_256_SLOT_TIME
  * @retval none
  */
void emac_pause_low_threshold_set(emac_pause_slot_threshold_type pasue_threshold)
{
  EMAC->fctrl_bit.plt = pasue_threshold;
}

/**
  * @brief  set zero-quanta pause disable.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_zero_quanta_pause_disable(confirm_state new_state)
{
  EMAC->fctrl_bit.dzqp = new_state;
}

/**
  * @brief  set pause time.
  * @param  pause_time: time slots to pause transmit frame.
  * @retval none
  */
void emac_pause_time_set(uint16_t pause_time)
{
  EMAC->fctrl_bit.pt = pause_time;
}

/**
  * @brief  identify coming vlan frame field with setting value.
  * @param  identifier: it will be compared with coming frame.
  * @retval none
  */
void emac_vlan_tag_identifier_set(uint16_t identifier)
{
  EMAC->vlt_bit.vti = identifier;
}

/**
  * @brief  set 12-bit vlan identifier.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_vlan_tag_comparison_set(confirm_state new_state)
{
  EMAC->vlt_bit.etv = new_state;
}

/**
  * @brief  set wakeup frame.
  * @param  value: it will be written to eight non transparent registers.
  * @retval none
  */
void emac_wakeup_frame_set(uint32_t value)
{
  EMAC->rwff = value;
}

/**
  * @brief  get wakeup frame.
  * @param  none
  * @retval get value from eight non transparent registers.
  */
uint32_t emac_wakeup_frame_get(void)
{
  return (EMAC->rwff);
}

/**
  * @brief  all frame will be droppped except wakeup frame or magic packet.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_power_down_set(confirm_state new_state)
{
  EMAC->pmtctrlsts_bit.pd = new_state;
}

/**
  * @brief  magic packet enable
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_magic_packet_enable(confirm_state new_state)
{
  EMAC->pmtctrlsts_bit.emp = new_state;
}

/**
  * @brief  wakeup frame enable
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_wakeup_frame_enable(confirm_state new_state)
{
  EMAC->pmtctrlsts_bit.erwf = new_state;
}

/**
  * @brief  received magic packet
  * @param  none
  * @retval the new state of usart_flag (SET or RESET).
  */
flag_status emac_received_magic_packet_get(void)
{
  if(EMAC->pmtctrlsts_bit.rmp)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  received wakeup frame.
  * @param  none
  * @retval the new state of usart_flag (SET or RESET).
  */
flag_status emac_received_wakeup_frame_get(void)
{
  if(EMAC->pmtctrlsts_bit.rrwf)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  set unicast frame that passes DAF as wakeup frame.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_global_unicast_set(confirm_state new_state)
{
  EMAC->pmtctrlsts_bit.guc = new_state;
}

/**
  * @brief  reset wakeup frame filter resgister
  * @param new_state: TRUE or FALSE.
  * @retval none
  */
void emac_wakeup_frame_filter_reset(confirm_state new_state)
{
  EMAC->pmtctrlsts_bit.rwffpr = new_state;
}

/**
  * @brief  read interrupt status
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - EMAC_PMT_FLAG
  *         - EMAC_MMC_FLAG
  *         - EMAC_MMCR_FLAG
  *         - EMAC_MMCT_FLAG
  *         - EMAC_TST_FLAG
  * @retval the new state of usart_flag (SET or RESET).
  */
flag_status emac_interrupt_status_read(uint32_t flag)
{
  if(EMAC->ists & flag)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  set interrupt mask
  * @param  mask_type: mask the interrupt signal
  *         this parameter can be one of the following values:
  *         - EMAC_INTERRUPT_PMT_MASK
  *         - EMAC_INTERRUPT_TST_MASK
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_interrupt_mask_set(emac_interrupt_mask_type mask_type, confirm_state new_state)
{
  switch(mask_type)
  {
    case EMAC_INTERRUPT_PMT_MASK:
    {
      EMAC->imr_bit.pim = new_state;
      break;
    }
    case EMAC_INTERRUPT_TST_MASK:
    {
      EMAC->imr_bit.tim = new_state;
      break;
    }
  }
}

/**
  * @brief  set local mac address
  * @param  address: local address for mac0
  * @retval none
  */
void emac_local_address_set(uint8_t *address)
{
  EMAC->a0h_bit.ma0h = (uint32_t)(address[5] << 8 | address[4]);
  EMAC->a0l_bit.ma0l = (uint32_t)(address[3] << 24 | address[2] << 16 | address[1] << 8 | address[0]);
}

/**
  * @brief  set mac filter address
  * @param  mac: select which mac you want to set
  *         this parameter can be one of the following values:
  *         - EMAC_ADDRESS_FILTER_1
  *         - EMAC_ADDRESS_FILTER_2
  *         - EMAC_ADDRESS_FILTER_3
  * @retval none
  */
void emac_address_filter_set(emac_address_type mac, emac_address_filter_type filter, emac_address_mask_type mask_bit, confirm_state new_state)
{
  switch(mac)
  {
    case EMAC_ADDRESS_FILTER_1:
    {
      EMAC->a1h_bit.sa = filter;
      EMAC->a1h_bit.mbc = mask_bit;
      EMAC->a1h_bit.ae = new_state;
      break;
    }
    case EMAC_ADDRESS_FILTER_2:
    {
      EMAC->a2h_bit.sa = filter;
      EMAC->a2h_bit.mbc = mask_bit;
      EMAC->a2h_bit.ae = new_state;
      break;
    }
    case EMAC_ADDRESS_FILTER_3:
    {
      EMAC->a3h_bit.sa = filter;
      EMAC->a3h_bit.mbc = mask_bit;
      EMAC->a3h_bit.ae = new_state;
      break;
    }
  }
}

/**
  * @brief  set transmit/receive descriptor list address
  * @param  transfer_type: it will be transmit or receive
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_TRANSMIT
  *         - EMAC_DMA_RECEIVE
  * @param  dma_desc_tab: pointer on the first tx desc list
  * @param  buff: pointer on the first tx/rx buffer list
  * @param  buffer_count: number of the used Tx desc in the list
  * @retval none
  */
void emac_dma_descriptor_list_address_set(emac_dma_tx_rx_type transfer_type, emac_dma_desc_type *dma_desc_tab, uint8_t *buff, uint32_t buffer_count)
{
  uint32_t i = 0;
  emac_dma_desc_type *dma_descriptor;

  switch(transfer_type)
  {
    case EMAC_DMA_TRANSMIT:
    {
      dma_tx_desc_to_set = dma_desc_tab;
      for(i = 0; i < buffer_count; i++)
      {
        dma_descriptor = dma_desc_tab + i;

        dma_descriptor->status = EMAC_DMATXDESC_TCH;

        dma_descriptor->buf1addr = (uint32_t)(&buff[i * EMAC_MAX_PACKET_LENGTH]);

        if(i < (buffer_count - 1))
        {
          dma_descriptor->buf2nextdescaddr = (uint32_t)(dma_desc_tab + i + 1);
        }
        else
        {
          dma_descriptor->buf2nextdescaddr = (uint32_t) dma_desc_tab;
        }
      }
      EMAC_DMA->tdladdr_bit.stl = (uint32_t) dma_desc_tab;
      break;
    }
    case EMAC_DMA_RECEIVE:
    {
      dma_rx_desc_to_get = dma_desc_tab;
      for(i = 0; i < buffer_count; i++)
      {
        dma_descriptor = dma_desc_tab + i;

        dma_descriptor->status = EMAC_DMARXDESC_OWN;

        dma_descriptor->controlsize = EMAC_DMARXDESC_RCH | (uint32_t)EMAC_MAX_PACKET_LENGTH;

        dma_descriptor->buf1addr = (uint32_t)(&buff[i * EMAC_MAX_PACKET_LENGTH]);

        if(i < (buffer_count - 1))
        {
          dma_descriptor->buf2nextdescaddr = (uint32_t)(dma_desc_tab + i + 1);
        }
        else
        {
          dma_descriptor->buf2nextdescaddr = (uint32_t) dma_desc_tab;
        }
      }
      EMAC_DMA->rdladdr_bit.srl = (uint32_t) dma_desc_tab;
      break;
    }
  }
}

/**
  * @brief  enable or disable the specified dma rx descriptor receive interrupt
  * @param  dma_rx_desc: pointer on a rx desc.
  * @param  new_state: new state of the specified dma rx descriptor interrupt.
  *         this parameter can be one of the following values:
  *         - TRUE
  *         - FALSE.
  * @retval none
  */
void emac_dma_rx_desc_interrupt_config(emac_dma_desc_type *dma_rx_desc, confirm_state new_state)
{
  if (new_state != FALSE)
  {
    /* enable the dma rx desc receive interrupt */
    dma_rx_desc->controlsize &= (~(uint32_t)EMAC_DMARXDESC_DIC);
  }
  else
  {
    /* disable the dma rx desc receive interrupt */
    dma_rx_desc->controlsize |= EMAC_DMARXDESC_DIC;
  }
}

/**
  * @brief  get transmit/receive descriptor list address
  * @param  transfer_type: it will be transmit or receive
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_TRANSMIT
  *         - EMAC_DMA_RECEIVE
  * @retval transmit/receive descriptor list address
  */
uint32_t emac_dma_descriptor_list_address_get(emac_dma_tx_rx_type transfer_type)
{
  switch(transfer_type)
  {
    case EMAC_DMA_TRANSMIT:
    {
      return (EMAC_DMA->tdladdr_bit.stl);
    }
    case EMAC_DMA_RECEIVE:
    {
      return (EMAC_DMA->rdladdr_bit.srl);
    }
  }
  return 0;
}

/**
  * @brief  get the size of received the received packet.
  * @param  none
  * @retval received packet size
  */
uint32_t emac_received_packet_size_get(void)
{
  uint32_t frame_length = 0;
  if(((dma_rx_desc_to_get->status & EMAC_DMARXDESC_OWN) == (uint32_t)RESET) &&
     ((dma_rx_desc_to_get->status & EMAC_DMATXDESC_ES) == (uint32_t)RESET) &&
     ((dma_rx_desc_to_get->status & EMAC_DMARXDESC_LS) != (uint32_t)RESET) &&
     ((dma_rx_desc_to_get->status & EMAC_DMARXDESC_FS) != (uint32_t)RESET))
  {
    frame_length = emac_dmarxdesc_frame_length_get(dma_rx_desc_to_get);
  }

  return frame_length;
}

/**
  * @brief  get the specified dma rx descsriptor frame length.
  * @param  dma_rx_desc: pointer on a dma rx descriptor
  * @retval the rx descriptor received frame length.
  */
uint32_t emac_dmarxdesc_frame_length_get(emac_dma_desc_type *dma_rx_desc)
{
  return ((dma_rx_desc->status & EMAC_DMARXDESC_FL) >> EMAC_DMARXDESC_FRAME_LENGTHSHIFT);
}

/**
  * @brief  init emac dma parameters
  * @param  emac_dma_config_type
  * @retval none
  */
void emac_dma_para_init(emac_dma_config_type *control_para)
{
  control_para->aab_enable = FALSE;
  control_para->da_enable = FALSE;
  control_para->desc_skip_length = 0;
  control_para->dt_disable = FALSE;
  control_para->fb_enable = FALSE;
  control_para->fef_enable = FALSE;
  control_para->flush_rx_disable = FALSE;
  control_para->fugf_enable = FALSE;
  control_para->osf_enable = FALSE;
  control_para->priority_ratio = EMAC_DMA_1_RX_1_TX;
  control_para->rsf_enable = FALSE;
  control_para->rx_dma_pal = EMAC_DMA_PBL_1;
  control_para->rx_threshold = EMAC_DMA_RX_THRESHOLD_64_BYTES;
  control_para->tsf_enable = FALSE;
  control_para->tx_dma_pal = EMAC_DMA_PBL_1;
  control_para->tx_threshold = EMAC_DMA_TX_THRESHOLD_64_BYTES;
  control_para->usp_enable = FALSE;
}

/**
  * @brief  configure emac dma
  * @param  emac_dma_config_type
  * @retval none
  */
void emac_dma_config(emac_dma_config_type *control_para)
{
  EMAC_DMA->bm_bit.aab = control_para->aab_enable;
  EMAC_DMA->bm_bit.dsl = control_para->desc_skip_length;
  EMAC_DMA->bm_bit.rdp = control_para->rx_dma_pal;
  EMAC_DMA->bm_bit.pbl = control_para->tx_dma_pal;
  EMAC_DMA->bm_bit.fb = control_para->fb_enable;
  EMAC_DMA->bm_bit.usp = control_para->usp_enable;
  EMAC_DMA->bm_bit.da = control_para->da_enable;
  EMAC_DMA->bm_bit.pr = control_para->priority_ratio;

  EMAC_DMA->opm_bit.dt = control_para->dt_disable;
  EMAC_DMA->opm_bit.rsf = control_para->rsf_enable;
  EMAC_DMA->opm_bit.dfrf = control_para->flush_rx_disable;
  EMAC_DMA->opm_bit.tsf = control_para->tsf_enable;
  EMAC_DMA->opm_bit.ttc = control_para->tx_threshold;
  EMAC_DMA->opm_bit.fef = control_para->fef_enable;
  EMAC_DMA->opm_bit.fugf = control_para->fugf_enable;
  EMAC_DMA->opm_bit.rtc = control_para->rx_threshold;
  EMAC_DMA->opm_bit.osf = control_para->osf_enable;
}

/**
  * @brief  set rx tx priority
  * @param  ratio: rx tx priority ratio
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_1_RX_1_TX
  *         - EMAC_DMA_2_RX_1_TX
  *         - EMAC_DMA_3_RX_1_TX
  *         - EMAC_DMA_4_RX_1_TX
  * @param  new_state: TRUE or FALSE
  * @retval none
  */
void emac_dma_arbitation_set(emac_dma_rx_tx_ratio_type ratio, confirm_state new_state)
{
  EMAC_DMA->bm_bit.da = new_state;

  if(new_state)
  {
    EMAC_DMA->bm_bit.pr = ratio;
  }
}

/**
  * @brief  set descriptor skip mength
  * @param  length: descriptor skip length
  * @retval none
  */
void emac_dma_descriptor_skip_length_set(uint8_t length)
{
  EMAC_DMA->bm_bit.dsl = length;
}

/**
  * @brief  set programmable burst length
  * @param  tx_length: tx programmable burst length
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_PBL_1
  *         - EMAC_DMA_PBL_2
  *         - EMAC_DMA_PBL_4
  *         - EMAC_DMA_PBL_8
  *         - EMAC_DMA_PBL_16
  *         - EMAC_DMA_PBL_32
  * @param  rx_length: rx programmable burst length
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_PBL_1
  *         - EMAC_DMA_PBL_2
  *         - EMAC_DMA_PBL_4
  *         - EMAC_DMA_PBL_8
  *         - EMAC_DMA_PBL_16
  *         - EMAC_DMA_PBL_32
  * @param  new_state: TRUE or FALSE
  * @retval none
  */
void emac_dma_separate_pbl_set(emac_dma_pbl_type tx_length, emac_dma_pbl_type rx_length, confirm_state new_state)
{
  EMAC_DMA->bm_bit.usp = new_state;
  EMAC_DMA->bm_bit.pbl = tx_length;

  if(new_state)
  {
    EMAC_DMA->bm_bit.pbl = rx_length;
  }
}

/**
  * @brief  set 8 times programmable burst length
  * @param  new_state: TRUE or FALSE
  * @retval none
  */
void emac_dma_eight_pbl_mode_set(confirm_state new_state)
{
  EMAC_DMA->bm_bit.pblx8 = new_state;
}

/**
  * @brief  set address-aligned beats
  * @param  new_state: TRUE or FALSE
  * @retval none
  */
void emac_dma_address_aligned_beats_set(confirm_state new_state)
{
  EMAC_DMA->bm_bit.aab = new_state;
}

/**
  * @brief  set transmit/receive poll demand
  * @param  transfer_type: it will be transmit or receive
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_TRANSMIT
  *         - EMAC_DMA_RECEIVE
  * @param  value: it can be any number
  * @retval none
  */
void emac_dma_poll_demand_set(emac_dma_tx_rx_type transfer_type, uint32_t value)
{
  switch(transfer_type)
  {
    case EMAC_DMA_TRANSMIT:
    {
      EMAC_DMA->tpd_bit.tpd = value;
      break;
    }
    case EMAC_DMA_RECEIVE:
    {
      EMAC_DMA->rpd_bit.rpd = value;
      break;
    }
  }
}

/**
  * @brief  get transmit poll demand
  * @param  transfer_type: it will be transmit or receive
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_TRANSMIT
  *         - EMAC_DMA_RECEIVE
  * @retval current transmit descriptor
  */
uint32_t emac_dma_poll_demand_get(emac_dma_tx_rx_type transfer_type)
{
  switch(transfer_type)
  {
    case EMAC_DMA_TRANSMIT:
    {
      return (EMAC_DMA->tpd_bit.tpd);
    }
    case EMAC_DMA_RECEIVE:
    {
      return (EMAC_DMA->rpd_bit.rpd);
    }
  }
  return 0;
}

/**
  * @brief  get receive dma process status
  * @param  none
  * @retval every situation it describe in RM
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_RX_RESET_STOP_COMMAND
  *         - EMAC_DMA_RX_FETCH_DESCRIPTOR
  *         - EMAC_DMA_RX_WAITING_PACKET
  *         - EMAC_DMA_RX_DESCRIPTOR_UNAVAILABLE
  *         - EMAC_DMA_RX_CLOSE_DESCRIPTOR
  *         - EMAC_DMA_RX_FIFO_TO_HOST
  */
emac_dma_receive_process_status_type emac_dma_receive_status_get(void)
{
  switch(EMAC_DMA->sts_bit.rs)
  {
    case EMAC_DMA_RX_RESET_STOP_COMMAND:
    {
      return EMAC_DMA_RX_RESET_STOP_COMMAND;
    }

    case EMAC_DMA_RX_FETCH_DESCRIPTOR:
    {
      return EMAC_DMA_RX_FETCH_DESCRIPTOR;
    }

    case EMAC_DMA_RX_WAITING_PACKET:
    {
      return EMAC_DMA_RX_WAITING_PACKET;
    }

    case EMAC_DMA_RX_DESCRIPTOR_UNAVAILABLE:
    {
      return EMAC_DMA_RX_DESCRIPTOR_UNAVAILABLE;
    }

    case EMAC_DMA_RX_CLOSE_DESCRIPTOR:
    {
      return EMAC_DMA_RX_CLOSE_DESCRIPTOR;
    }

    case EMAC_DMA_RX_FIFO_TO_HOST:
    {
      return EMAC_DMA_RX_FIFO_TO_HOST;
    }
  }

  return EMAC_DMA_RX_RESET_STOP_COMMAND;
}

/**
  * @brief  get transmit dma process status
  * @param  none
  * @retval every situation it describe in RM
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_TX_RESET_STOP_COMMAND
  *         - EMAC_DMA_TX_FETCH_DESCRIPTOR
  *         - EMAC_DMA_TX_WAITING_FOR_STATUS
  *         - EMAC_DMA_TX_HOST_TO_FIFO
  *         - EMAC_DMA_TX_DESCRIPTOR_UNAVAILABLE
  *         - EMAC_DMA_TX_CLOSE_DESCRIPTOR
  */
emac_dma_transmit_process_status_type emac_dma_transmit_status_get(void)
{
  switch(EMAC_DMA->sts_bit.ts)
  {
    case EMAC_DMA_TX_RESET_STOP_COMMAND:
    {
      return EMAC_DMA_TX_RESET_STOP_COMMAND;
    }

    case EMAC_DMA_TX_FETCH_DESCRIPTOR:
    {
      return EMAC_DMA_TX_FETCH_DESCRIPTOR;
    }

    case EMAC_DMA_TX_WAITING_FOR_STATUS:
    {
      return EMAC_DMA_TX_WAITING_FOR_STATUS;
    }

    case EMAC_DMA_TX_HOST_TO_FIFO:
    {
      return EMAC_DMA_TX_HOST_TO_FIFO;
    }

    case EMAC_DMA_TX_DESCRIPTOR_UNAVAILABLE:
    {
      return EMAC_DMA_TX_DESCRIPTOR_UNAVAILABLE;
    }

    case EMAC_DMA_TX_CLOSE_DESCRIPTOR:
    {
      return EMAC_DMA_TX_CLOSE_DESCRIPTOR;
    }
  }

  return EMAC_DMA_TX_RESET_STOP_COMMAND;
}

/**
  * @brief  set dma operations
  * @param  ops: operations of dma
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_OPS_START_STOP_RECEIVE
  *         - EMAC_DMA_OPS_SECOND_FRAME
  *         - EMAC_DMA_OPS_FORWARD_UNDERSIZED
  *         - EMAC_DMA_OPS_FORWARD_ERROR
  *         - EMAC_DMA_OPS_START_STOP_TRANSMIT
  *         - EMAC_DMA_OPS_FLUSH_TRANSMIT_FIFO
  *         - EMAC_DMA_OPS_TRANSMIT_STORE_FORWARD
  *         - EMAC_DMA_OPS_RECEIVE_FLUSH_DISABLE
  *         - EMAC_DMA_OPS_RECEIVE_STORE_FORWARD
  *         - EMAC_DMA_OPS_DROP_ERROR_DISABLE
  * @param  new_state: TRUE or FALSE
  * @retval none
  */
void emac_dma_operations_set(emac_dma_operations_type ops, confirm_state new_state)
{
  __IO uint32_t temp = 0;
  switch(ops)
  {
    case EMAC_DMA_OPS_START_STOP_RECEIVE:
    {
      EMAC_DMA->opm_bit.ssr = new_state;
      break;
    }

    case EMAC_DMA_OPS_SECOND_FRAME:
    {
      EMAC_DMA->opm_bit.osf = new_state;
      break;
    }

    case EMAC_DMA_OPS_FORWARD_UNDERSIZED:
    {
      EMAC_DMA->opm_bit.fugf = new_state;
      break;
    }

    case EMAC_DMA_OPS_FORWARD_ERROR:
    {
      EMAC_DMA->opm_bit.fef = new_state;
      break;
    }

    case EMAC_DMA_OPS_START_STOP_TRANSMIT:
    {
      EMAC_DMA->opm_bit.sstc = new_state;
      break;
    }

    case EMAC_DMA_OPS_FLUSH_TRANSMIT_FIFO:
    {
      EMAC_DMA->opm_bit.ftf = new_state;
      temp = EMAC_DMA->opm;
      emac_delay(1);
      EMAC_DMA->opm = temp;
      break;
    }

    case EMAC_DMA_OPS_TRANSMIT_STORE_FORWARD:
    {
      EMAC_DMA->opm_bit.tsf = new_state;
      break;
    }

    case EMAC_DMA_OPS_RECEIVE_FLUSH_DISABLE:
    {
      EMAC_DMA->opm_bit.dfrf = new_state;
      break;
    }

    case EMAC_DMA_OPS_RECEIVE_STORE_FORWARD:
    {
      EMAC_DMA->opm_bit.rsf = new_state;
      break;
    }

    case EMAC_DMA_OPS_DROP_ERROR_DISABLE:
    {
      EMAC_DMA->opm_bit.dt = new_state;
      break;
    }
  }
}

/**
  * @brief  set receive dma threshold
  * @param  value: receive threshold
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_RX_THRESHOLD_64_BYTES
  *         - EMAC_DMA_RX_THRESHOLD_32_BYTES
  *         - EMAC_DMA_RX_THRESHOLD_96_BYTES
  *         - EMAC_DMA_RX_THRESHOLD_128_BYTES
  * @retval none
  */
void emac_dma_receive_threshold_set(emac_dma_receive_threshold_type value)
{
  EMAC_DMA->opm_bit.rtc = value;
}

/**
  * @brief  set transmit dma threshold
  * @param  value: transmit threshold
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_TX_THRESHOLD_64_BYTES
  *         - EMAC_DMA_TX_THRESHOLD_128_BYTES
  *         - EMAC_DMA_TX_THRESHOLD_192_BYTES
  *         - EMAC_DMA_TX_THRESHOLD_256_BYTES
  *         - EMAC_DMA_TX_THRESHOLD_40_BYTES
  *         - EMAC_DMA_TX_THRESHOLD_32_BYTES
  *         - EMAC_DMA_TX_THRESHOLD_24_BYTES
  *         - EMAC_DMA_TX_THRESHOLD_16_BYTES
  * @retval none
  */
void emac_dma_transmit_threshold_set(emac_dma_transmit_threshold_type value)
{
  EMAC_DMA->opm_bit.ttc = value;
}

/**
  * @brief  enable dma interrupt
  * @param  it: interrupt type
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_INTERRUPT_TX
  *         - EMAC_DMA_INTERRUPT_TX_STOP
  *         - EMAC_DMA_INTERRUPT_TX_UNAVAILABLE
  *         - EMAC_DMA_INTERRUPT_TX_JABBER
  *         - EMAC_DMA_INTERRUPT_RX_OVERFLOW
  *         - EMAC_DMA_INTERRUPT_TX_UNDERFLOW
  *         - EMAC_DMA_INTERRUPT_RX
  *         - EMAC_DMA_INTERRUPT_RX_UNAVAILABLE
  *         - EMAC_DMA_INTERRUPT_RX_STOP
  *         - EMAC_DMA_INTERRUPT_RX_TIMEOUT
  *         - EMAC_DMA_INTERRUPT_TX_EARLY
  *         - EMAC_DMA_INTERRUPT_FATAL_BUS_ERROR
  *         - EMAC_DMA_INTERRUPT_RX_EARLY
  *         - EMAC_DMA_INTERRUPT_ABNORMAL_SUMMARY
  *         - EMAC_DMA_INTERRUPT_NORMAL_SUMMARY
  * @param  new_state: TRUE or FALSE
  * @retval none
  */
void emac_dma_interrupt_enable(emac_dma_interrupt_type it, confirm_state new_state)
{
  switch(it)
  {
    case EMAC_DMA_INTERRUPT_TX:
    {
      EMAC_DMA->ie_bit.tie = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_TX_STOP:
    {
      EMAC_DMA->ie_bit.tse = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_TX_UNAVAILABLE:
    {
      EMAC_DMA->ie_bit.tue = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_TX_JABBER:
    {
      EMAC_DMA->ie_bit.tje = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_RX_OVERFLOW:
    {
      EMAC_DMA->ie_bit.ove = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_TX_UNDERFLOW:
    {
      EMAC_DMA->ie_bit.une = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_RX:
    {
      EMAC_DMA->ie_bit.rie = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_RX_UNAVAILABLE:
    {
      EMAC_DMA->ie_bit.rbue = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_RX_STOP:
    {
      EMAC_DMA->ie_bit.rse = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_RX_TIMEOUT:
    {
      EMAC_DMA->ie_bit.rwte = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_TX_EARLY:
    {
      EMAC_DMA->ie_bit.eie = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_FATAL_BUS_ERROR:
    {
      EMAC_DMA->ie_bit.fbee = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_RX_EARLY:
    {
      EMAC_DMA->ie_bit.ere = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_ABNORMAL_SUMMARY:
    {
      EMAC_DMA->ie_bit.aie = new_state;
      break;
    }
    case EMAC_DMA_INTERRUPT_NORMAL_SUMMARY:
    {
      EMAC_DMA->ie_bit.nie = new_state;
      break;
    }
  }
}

/**
  * @brief  get missed frames by the controller
  * @param  none
  * @retval missed frames by the controller
  */
uint16_t emac_dma_controller_missing_frame_get(void)
{
  uint16_t number = EMAC_DMA->mfbocnt_bit.mfc;
  return number;
}

/**
  * @brief  get overflow bit for missed frame counter
  * @param  none
  * @retval overflow bit for missed frame counter
  */
uint8_t emac_dma_missing_overflow_bit_get(void)
{
  uint8_t number = EMAC_DMA->mfbocnt_bit.obmfc;
  return number;
}

/**
  * @brief  get missed frames by the application
  * @param  none
  * @retval missed frames by the application
  */
uint16_t emac_dma_application_missing_frame_get(void)
{
  uint16_t number = EMAC_DMA->mfbocnt_bit.ofc;
  return number;
}

/**
  * @brief  get overflow bit for FIFO overflow counter
  * @param  none
  * @retval overflow bit for FIFO overflow counter
  */
uint8_t emac_dma_fifo_overflow_bit_get(void)
{
  uint8_t number = EMAC_DMA->mfbocnt_bit.obfoc;
  return number;
}

/**
  * @brief  get overflow bit for FIFO overflow counter
  * @param  transfer type: receive/transmit type
  *         this parameter can be one of the following values:
  *         - EMAC_DMA_TX_DESCRIPTOR
  *         - EMAC_DMA_RX_DESCRIPTOR
  *         - EMAC_DMA_TX_BUFFER
  *         - EMAC_DMA_RX_BUFFER
  * @retval memory address
  */
uint32_t emac_dma_tansfer_address_get(emac_dma_transfer_address_type transfer_type)
{
  uint32_t address = 0;

  switch(transfer_type)
  {
    case EMAC_DMA_TX_DESCRIPTOR:
    {
      address = EMAC_DMA->ctd_bit.htdap;
      break;
    }
    case EMAC_DMA_RX_DESCRIPTOR:
    {
      address = EMAC_DMA->crd_bit.hrdap;
      break;
    }
    case EMAC_DMA_TX_BUFFER:
    {
      address = EMAC_DMA->ctbaddr_bit.htbap;
      break;
    }
    case EMAC_DMA_RX_BUFFER:
    {
      address = EMAC_DMA->crbaddr_bit.hrbap;
      break;
    }
  }
  return address;
}

/**
  * @brief  reset all counter
  * @param  none
  * @retval none
  */
void emac_mmc_counter_reset(void)
{
  EMAC_MMC->ctrl_bit.rc = TRUE;
}

/**
  * @brief  counter stop counting from zero when it reaches maximum
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_mmc_rollover_stop(confirm_state new_state)
{
  EMAC_MMC->ctrl_bit.scr = new_state;
}

/**
  * @brief  enable reset on read
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_mmc_reset_on_read_enable(confirm_state new_state)
{
  EMAC_MMC->ctrl_bit.rr = new_state;
}

/**
  * @brief  freeze mmc counter
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_mmc_counter_freeze(confirm_state new_state)
{
  EMAC_MMC->ctrl_bit.fmc = new_state;
}

/**
  * @brief  interupt status of received frames
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - MMC_RX_CRC_ERROR
  *         - MMC_RX_ALIGN_ERROR
  *         - MMC_RX_GOOD_UNICAST
  * @retval the new state of usart_flag (SET or RESET).
  */
flag_status emac_mmc_received_status_get(uint32_t flag)
{
  if(EMAC_MMC->ri & flag)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  interupt status of transmit frames
  * @param  transmit_type: transmit type.
  *         this parameter can be one of the following values:
  *         - MMC_TX_SINGLE_COL
  *         - MMC_TX_MULTIPLE_COL
  *         - MMC_TX_GOOD_FRAMES
  * @retval the new state of usart_flag (SET or RESET).
  */
flag_status emac_mmc_transmit_status_get(uint32_t flag)
{
  if(EMAC_MMC->ti & flag)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  mask received mmc interrupt
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - MMC_RX_CRC_ERROR
  *         - MMC_RX_ALIGN_ERROR
  *         - MMC_RX_GOOD_UNICAST
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_mmc_received_interrupt_mask_set(uint32_t flag, confirm_state new_state)
{
  switch(flag)
  {
    case MMC_RX_CRC_ERROR:
    {
      EMAC_MMC->rim_bit.rcefcim = new_state;
      break;
    }
    case MMC_RX_ALIGN_ERROR:
    {
      EMAC_MMC->rim_bit.raefacim = new_state;
      break;
    }
    case MMC_RX_GOOD_UNICAST:
    {
      EMAC_MMC->rim_bit.rugfcim = new_state;
      break;
    }
  }
}

/**
  * @brief  mask transmit mmc interrupt
  * @param  transmit_type: transmit type.
  *         this parameter can be one of the following values:
  *         - MMC_TX_SINGLE_COL
  *         - MMC_TX_MULTIPLE_COL
  *         - MMC_TX_GOOD_FRAMES
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_mmc_transmit_interrupt_mask_set(uint32_t flag, confirm_state new_state)
{
  switch(flag)
  {
    case MMC_TX_SINGLE_COL:
    {
      EMAC_MMC->tim_bit.tscgfcim = new_state;
      break;
    }
    case MMC_TX_MULTIPLE_COL:
    {
      EMAC_MMC->tim_bit.tmcgfcim = new_state;
      break;
    }
    case MMC_TX_GOOD_FRAMES:
    {
      EMAC_MMC->tim_bit.tgfcim = new_state;
      break;
    }
  }
}

/**
  * @brief  get good frame numbers as single collision occurs.
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - MMC_TX_SINGLE_COL
  *         - MMC_TX_MULTIPLE_COL
  *         - MMC_TX_GOOD_FRAMES
  * @retval good frames
  */
uint32_t emac_mmc_transmit_good_frames_get(uint32_t flag)
{
  uint32_t good_frames = MMC_TX_GOOD_FRAMES;

  switch(flag)
  {
    case MMC_TX_SINGLE_COL:
    {
      good_frames = EMAC_MMC->tfscc_bit.tgfscc;
      break;
    }
    case MMC_TX_MULTIPLE_COL:
    {
      good_frames = EMAC_MMC->tfmscc_bit.tgfmscc;
      break;
    }
    case MMC_TX_GOOD_FRAMES:
    {
      good_frames = EMAC_MMC->tfcnt_bit.tgfc;
      break;
    }
  }
  return good_frames;
}

/**
  * @brief  get good frame numbers as single collision occurs.
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - MMC_RX_CRC_ERROR
  *         - MMC_RX_ALIGN_ERROR
  *         - MMC_RX_GOOD_UNICAST
  * @retval good frames
  */
uint32_t emac_mmc_received_error_frames_get(uint32_t flag)
{
  uint32_t error_frames = MMC_RX_GOOD_UNICAST;

  switch(flag)
  {
    case MMC_RX_CRC_ERROR:
    {
      error_frames = EMAC_MMC->rfcecnt_bit.rfcec;
      break;
    }
    case MMC_RX_ALIGN_ERROR:
    {
      error_frames = EMAC_MMC->rfaecnt_bit.rfaec;
      break;
    }
    case MMC_RX_GOOD_UNICAST:
    {
      error_frames = EMAC_MMC->rgufcnt_bit.rgufc;
      break;
    }
  }
  return error_frames;
}

/**
  * @brief  enable timestamp.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_timestamp_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.te = new_state;
}

/**
  * @brief  enable timestamp fine update.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_timestamp_fine_update_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.tfcu = new_state;
}

/**
  * @brief  initialize timestamp time system.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_timestamp_system_time_init(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.ti = new_state;
}

/**
  * @brief  update timestamp time system.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_timestamp_system_time_update(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.tu = new_state;
}

/**
  * @brief  enable timestamp interrupt trigger.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_interrupt_trigger_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.tite = new_state;
}

/**
  * @brief  update timestamp addend register.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_addend_register_update(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.aru = new_state;
}

/**
  * @brief  enable timestamp snapshot for all received frames.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_snapshot_received_frames_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.etaf = new_state;
}

/**
  * @brief  enable digital rollover.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_subsecond_rollover_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.tdbrc = new_state;
}

/**
  * @brief  enable packet snooping for version 2.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_psv2_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.eppv2f = new_state;
}

/**
  * @brief  enable snapshot over emac.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_snapshot_emac_frames_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.eppef = new_state;
}

/**
  * @brief  enable snapshot for ipv6 frames.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_snapshot_ipv6_frames_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.eppfsip6u = new_state;
}

/**
  * @brief  enable snapshot for ipv4 frames.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_snapshot_ipv4_frames_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.eppfsip4u = new_state;
}

/**
  * @brief  enable snapshot for event message.
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_snapshot_event_message_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.etsfem = new_state;
}

/**
  * @brief  enable snapshot for message relevant to master
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_snapshot_master_event_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.esfmrtm = new_state;
}

/**
  * @brief  set clock node type
  * @param  node: select ptp packets for taking snapshot
  *         this parameter can be one of the following values:
  *         - EMAC_PTP_NORMAL_CLOCK
  *         - EMAC_PTP_BOUNDARY_CLOCK
  *         - EMAC_PTP_END_TO_END_CLOCK
  *         - EMAC_PTP_PEER_TO_PEER_CLOCK
  * @retval none
  */
void emac_ptp_clock_node_set(emac_ptp_clock_node_type node)
{
  EMAC_PTP->tsctrl_bit.sppfts = node;
}

/**
  * @brief  enable ptp frame filtering mac address
  * @param  new_state: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_mac_address_filter_enable(confirm_state new_state)
{
  EMAC_PTP->tsctrl_bit.emafpff = new_state;
}

/**
  * @brief  set subsecond increment value
  * @param  value: add to subsecond value for every update
  * @retval none
  */
void emac_ptp_subsecond_increment_set(uint8_t value)
{
  EMAC_PTP->ssinc_bit.ssiv = value;
}

/**
  * @brief  get system time second
  * @param  none
  * @retval system time second
  */
uint32_t emac_ptp_system_second_get(void)
{
  uint32_t second = EMAC_PTP->tsh_bit.ts;
  return second;
}

/**
  * @brief  get system time subsecond
  * @param  none
  * @retval system time subsecond
  */
uint32_t emac_ptp_system_subsecond_get(void)
{
  uint32_t subsecond = EMAC_PTP->tsl_bit.tss;
  return subsecond;
}

/**
  * @brief  get system time sign
  * @param  none
  * @retval TRUE or FALSE
  */
confirm_state emac_ptp_system_time_sign_get(void)
{
  if(EMAC_PTP->tsl_bit.ast)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  set system time second
  * @param  second: system time second
  * @retval none
  */
void emac_ptp_system_second_set(uint32_t second)
{
  EMAC_PTP->tshud_bit.ts = second;
}

/**
  * @brief  set system time subsecond
  * @param  subsecond: system time subsecond
  * @retval none
  */
void emac_ptp_system_subsecond_set(uint32_t subsecond)
{
  EMAC_PTP->tslud_bit.tss = subsecond;
}

/**
  * @brief  set system time sign
  * @param  sign: TRUE or FALSE.
  * @retval none
  */
void emac_ptp_system_time_sign_set(confirm_state sign)
{
  if(sign)
  {
    EMAC_PTP->tslud_bit.ast = 1;
  }
  else
  {
    EMAC_PTP->tslud_bit.ast = 0;
  }
}

/**
  * @brief  set time stamp addend
  * @param  value: to achieve time synchronization
  * @retval none
  */
void emac_ptp_timestamp_addend_set(uint32_t value)
{
  EMAC_PTP->tsad_bit.tar = value;
}

/**
  * @brief  set target time stamp high
  * @param  value: to set target time second
  * @retval none
  */
void emac_ptp_target_second_set(uint32_t value)
{
  EMAC_PTP->tth_bit.ttsr = value;
}

/**
  * @brief  set target time stamp low
  * @param  value: to set target time nanosecond
  * @retval none
  */
void emac_ptp_target_nanosecond_set(uint32_t value)
{
  EMAC_PTP->ttl_bit.ttlr = value;
}

/**
  * @brief  set target time stamp low
  * @param  status: type of status
  *         this parameter can be one of the following values:
  *         - EMAC_PTP_SECOND_OVERFLOW
  *         - EMAC_PTP_TARGET_TIME_REACH
  * @retval TRUE or FALSE
  */
confirm_state emac_ptp_timestamp_status_get(emac_ptp_timestamp_status_type status)
{
  switch(status)
  {
    case EMAC_PTP_SECOND_OVERFLOW:
    {
      if(EMAC_PTP->tssr_bit.tso)
      {
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
    case EMAC_PTP_TARGET_TIME_REACH:
    {
      if(EMAC_PTP->tssr_bit.tttr)
      {
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
  }
  return FALSE;
}

/**
  * @brief  set pps frequency
  * @param  freq: pps frequency
  *         this parameter can be one of the following values:
  *         - EMAC_PTP_PPS_1HZ
  *         - EMAC_PTP_PPS_2HZ
  *         - EMAC_PTP_PPS_4HZ
  *         - EMAC_PTP_PPS_8HZ
  *         - EMAC_PTP_PPS_16HZ
  *         - EMAC_PTP_PPS_32HZ
  *         - EMAC_PTP_PPS_64HZ
  *         - EMAC_PTP_PPS_128HZ
  *         - EMAC_PTP_PPS_256HZ
  *         - EMAC_PTP_PPS_512HZ
  *         - EMAC_PTP_PPS_1024HZ
  *         - EMAC_PTP_PPS_2048HZ
  *         - EMAC_PTP_PPS_4096HZ
  *         - EMAC_PTP_PPS_8192HZ
  *         - EMAC_PTP_PPS_16384HZ
  *         - EMAC_PTP_PPS_32768HZ
  * @retval none
  */
void emac_ptp_pps_frequency_set(emac_ptp_pps_control_type freq)
{
  EMAC_PTP->ppscr_bit.pofc = freq;
}

/**
  * @brief  this is delay function base on system clock.
  * @param  delay: delay time
  * @retval none
  */
static void emac_delay(uint32_t delay)
{
  __IO uint32_t delay_time = delay * (system_core_clock / 8 / 1000);
  do
  {
    __NOP();
  }
  while(delay_time --);
}

/**
  * @brief  check whether the specified emac dma flag is set or not.
  * @param  dma_flag: specifies the emac dma flag to check.
  *         this parameter can be one of emac dma flag status:
  *         - EMAC_DMA_TI_FLAG
  *         - EMAC_DMA_TPS_FLAG
  *         - EMAC_DMA_TBU_FLAG
  *         - EMAC_DMA_TJT_FLAG
  *         - EMAC_DMA_OVF_FLAG
  *         - EMAC_DMA_UNF_FLAG
  *         - EMAC_DMA_RI_FLAG
  *         - EMAC_DMA_RBU_FLAG
  *         - EMAC_DMA_RPS_FLAG
  *         - EMAC_DMA_RWT_FLAG
  *         - EMAC_DMA_ETI_FLAG
  *         - EMAC_DMA_FBEI_FLAG
  *         - EMAC_DMA_ERI_FLAG
  *         - EMAC_DMA_AIS_FLAG
  *         - EMAC_DMA_NIS_FLAG
  * @retval the new state of dma_flag (SET or RESET).
  */
flag_status emac_dma_flag_get(uint32_t dma_flag)
{
  flag_status status = RESET;

  if(EMAC_DMA->sts & dma_flag)
    status = SET;
  /* return the new state (SET or RESET) */
  return status;
}

/**
  * @brief  clear the emac dma flag.
  * @param  dma_flag: specifies the emac dma flags to clear.
  *         this parameter can be any combination of the following values:
  *         - EMAC_DMA_TI_FLAG
  *         - EMAC_DMA_TPS_FLAG
  *         - EMAC_DMA_TBU_FLAG
  *         - EMAC_DMA_TJT_FLAG
  *         - EMAC_DMA_OVF_FLAG
  *         - EMAC_DMA_UNF_FLAG
  *         - EMAC_DMA_RI_FLAG
  *         - EMAC_DMA_RBU_FLAG
  *         - EMAC_DMA_RPS_FLAG
  *         - EMAC_DMA_RWT_FLAG
  *         - EMAC_DMA_ETI_FLAG
  *         - EMAC_DMA_FBEI_FLAG
  *         - EMAC_DMA_ERI_FLAG
  *         - EMAC_DMA_AIS_FLAG
  *         - EMAC_DMA_NIS_FLAG
  * @retval none
  */
void emac_dma_flag_clear(uint32_t dma_flag)
{
  EMAC_DMA->sts = dma_flag;
}

/**
  * @}
  */

#endif

/**
  * @}
  */

#endif

/**
  * @}
  */
