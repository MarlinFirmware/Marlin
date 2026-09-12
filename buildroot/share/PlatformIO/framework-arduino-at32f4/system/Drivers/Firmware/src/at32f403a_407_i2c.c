/**
  **************************************************************************
  * @file     at32f403a_407_i2c.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the i2c firmware library
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

/** @defgroup I2C
  * @brief I2C driver modules
  * @{
  */

#ifdef I2C_MODULE_ENABLED

/** @defgroup I2C_private_functions
  * @{
  */

/**
  * @brief  reset the i2c register
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval none
  */
void i2c_reset(i2c_type *i2c_x)
{
  if(i2c_x == I2C1)
  {
    crm_periph_reset(CRM_I2C1_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_I2C1_PERIPH_RESET, FALSE);
  }
  else if(i2c_x == I2C2)
  {
    crm_periph_reset(CRM_I2C2_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_I2C2_PERIPH_RESET, FALSE);
  }
  else if(i2c_x == I2C3)
  {
    crm_periph_reset(CRM_I2C3_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_I2C3_PERIPH_RESET, FALSE);
  }
}

/**
  * @brief  software reset.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_software_reset(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.reset = new_state;
}

/**
  * @brief  init i2c speed and duty cycle.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  duty
  *         this parameter can be one of the following values:
  *         - I2C_FSMODE_DUTY_2_1: duty cycle 2:1
  *         - I2C_FSMODE_DUTY_16_9: duty cycle 16:9
  * @param  speed: i2c scl clock speed, such as 100000
  * @retval none
  */
void i2c_init(i2c_type *i2c_x, i2c_fsmode_duty_cycle_type duty, uint32_t speed)
{
  uint32_t apb_freq = 0;
  uint16_t freq_mhz = 0, temp = 0;

  crm_clocks_freq_type clocks;

  /* disable i2c peripherals */
  i2c_x->ctrl1_bit.i2cen = FALSE;

  /* get system clock */
  crm_clocks_freq_get(&clocks);

  if((i2c_x == I2C1) || (i2c_x == I2C2))
  {
    apb_freq = clocks.apb1_freq;
  }
  else if(i2c_x == I2C3)
  {
    apb_freq = clocks.apb2_freq;
  }

  freq_mhz = (apb_freq / 1000000);

  /* set i2c input clock frequency */
  i2c_x->ctrl2_bit.clkfreq = freq_mhz;

  /* standard mode */
  if(speed <= 100000)
  {
    temp = (uint16_t)(apb_freq / (speed << 1));

    if (temp < 0x04)
    {
      temp = 0x04;
    }

    /* set scl clock */
    i2c_x->clkctrl_bit.speed = temp;

    /* disable fast mode */
    i2c_x->clkctrl_bit.speedmode = FALSE;

    /* set the maximum rise time */
    if((freq_mhz + 1) > 0x3F)
    {
      i2c_x->tmrise_bit.risetime = 0x3F;
    }
    else
    {
      i2c_x->tmrise_bit.risetime = (freq_mhz + 1);
    }
  }
  /* fast mode */
  else
  {
    if (duty == I2C_FSMODE_DUTY_2_1)
    {
      temp = (uint16_t)(apb_freq / (speed * 3));

      /* the ratio of high level to low level is 1:2 */
      i2c_x->clkctrl_bit.dutymode = I2C_FSMODE_DUTY_2_1;
    }
    else
    {
      temp = (uint16_t)(apb_freq / (speed * 25));

      /* the ratio of high level to low level is 9:16 */
      i2c_x->clkctrl_bit.dutymode = I2C_FSMODE_DUTY_16_9;
    }

    if (temp == 0)
    {
      temp = 0x0001;
    }

    /* set scl clock*/
    i2c_x->clkctrl_bit.speed = temp;

    /* set the mode to fast mode */
    i2c_x->clkctrl_bit.speedmode = TRUE;

    /* set the maximum rise time */
    i2c_x->tmrise_bit.risetime = (uint16_t)(((freq_mhz * (uint16_t)300) / (uint16_t)1000) + (uint16_t)1);
  }
}

/**
  * @brief  config own address1.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  mode
  *         this parameter can be one of the following values:
  *         - I2C_ADDRESS_MODE_7BIT: 7bit address.
  *         - I2C_ADDRESS_MODE_10BIT: 10bit address.
  * @param  address: own address1, such as 0xb0.
  * @retval none
  */
void i2c_own_address1_set(i2c_type *i2c_x, i2c_address_mode_type mode, uint16_t address)
{
  /* set address mode */
  i2c_x->oaddr1_bit.addr1mode = mode;

  /* set own address1 */
  i2c_x->oaddr1_bit.addr1 = address;
}

/**
  * @brief  config own address2.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  address: specifies the 7bit i2c own address2, such as 0xa0.
  * @retval none.
  */
void i2c_own_address2_set(i2c_type *i2c_x, uint8_t address)
{
  i2c_x->oaddr2_bit.addr2 = (address >> 1);
}

/**
  * @brief  enable or disable own address2.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_own_address2_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->oaddr2_bit.addr2en = new_state;
}

/**
  * @brief  enable or disable the smbus mode
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_smbus_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.permode = new_state;
}

/**
  * @brief  enable or disable i2c periph
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.i2cen = new_state;
}

/**
  * @brief  config fast mode duty cycle
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  duty
  *         this parameter can be one of the following values:
  *         - I2C_FSMODE_DUTY_2_1: duty cycle 2:1
  *         - I2C_FSMODE_DUTY_16_9: duty cycle 16:9
  * @retval none
  */
void i2c_fast_mode_duty_set(i2c_type *i2c_x, i2c_fsmode_duty_cycle_type duty)
{
  i2c_x->clkctrl_bit.dutymode = duty;
}

/**
  * @brief  enable or disable clock stretch.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_clock_stretch_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.stretch = !new_state;
}

/**
  * @brief  enable or disable acknowledge.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none.
  */
void i2c_ack_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.acken = new_state;
}

/**
  * @brief  master receiving mode acknowledge control.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  pos
  *         this parameter can be one of the following values:
  *         - I2C_MASTER_ACK_CURRENT: acken bit acts on the current byte
  *         - I2C_MASTER_ACK_NEXT: acken bit acts on the next byte
  * @retval none
  */
void i2c_master_receive_ack_set(i2c_type *i2c_x, i2c_master_ack_type pos)
{
  i2c_x->ctrl1_bit.mackctrl = pos;
}

/**
  * @brief  pec position set.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  pos
  *         this parameter can be one of the following values:
  *         - I2C_PEC_POSITION_CURRENT: the current byte is pec
  *         - I2C_PEC_POSITION_NEXT: the next byte is pec
  * @retval none
  */
void i2c_pec_position_set(i2c_type *i2c_x, i2c_pec_position_type pos)
{
  i2c_x->ctrl1_bit.mackctrl = pos;
}

/**
  * @brief  enable or disable general call.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_general_call_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.gcaen = new_state;
}

/**
  * @brief  enable or disable arp mode.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_arp_mode_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.arpen = new_state;
}

/**
  * @brief  config smbus host or device.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  level
  *         this parameter can be one of the following values:
  *         - I2C_SMBUS_MODE_DEVICE: smbus device.
  *         - I2C_SMBUS_MODE_HOST: smbus host.
  * @retval none
  */
void i2c_smbus_mode_set(i2c_type *i2c_x, i2c_smbus_mode_set_type mode)
{
  i2c_x->ctrl1_bit.smbmode = mode;
}

/**
  * @brief  drive the smbus alert pin high or low.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  level
  *         this parameter can be one of the following values:
  *         - I2C_SMBUS_ALERT_LOW: smbus alert pin set low.
  *         - I2C_SMBUS_ALERT_HIGH: smbus alert pin set high.
  * @retval none
  */
void i2c_smbus_alert_set(i2c_type *i2c_x, i2c_smbus_alert_set_type level)
{
  i2c_x->ctrl1_bit.smbalert = level;
}

/**
  * @brief  enable or disable pec transfer.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_pec_transmit_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.pecten = new_state;
}

/**
  * @brief  enable or disable pec calcultetion.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_pec_calculate_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.pecen = new_state;
}

/**
  * @brief  get pec value.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval uint8_t: pec value.
  */
uint8_t i2c_pec_value_get(i2c_type *i2c_x)
{
  return i2c_x->sts2_bit.pecval;
}

/**
  * @brief  enable or disable if the next dma transfer will be the last one.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_dma_end_transfer_set(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl2_bit.dmaend = new_state;
}

/**
  * @brief  enable or disable dma requests.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_dma_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl2_bit.dmaen = new_state;
}

/**
  * @brief  enable or disable interrupt
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  source
  *         this parameter can be one of the following values:
  *         - I2C_DATA_INT: data interrupt.
  *         - I2C_EV_INT: event interrupt.
  *         - I2C_ERR_INT: error interrupt.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void i2c_interrupt_enable(i2c_type *i2c_x, uint16_t source, confirm_state new_state)
{
  if (new_state != FALSE)
  {
    i2c_x->ctrl2 |= source;
  }
  else
  {
    i2c_x->ctrl2 &= (uint16_t)~source;
  }
}

/**
  * @brief  generate start condition.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval none.
  */
void i2c_start_generate(i2c_type *i2c_x)
{
  i2c_x->ctrl1_bit.genstart = TRUE;
}

/**
  * @brief  generate stop condition.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval none.
  */
void i2c_stop_generate(i2c_type *i2c_x)
{
  i2c_x->ctrl1_bit.genstop = TRUE;
}

/**
  * @brief  transmit the slave address.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  address: specifies the slave address which will be transmitted
  * @param  direction
  *         this parameter can be one of the following values:
  *         - I2C_DIRECTION_TRANSMIT: transmit mode.
  *         - I2C_DIRECTION_RECEIVE: receive mode.
  * @retval none.
  */
void i2c_7bit_address_send(i2c_type *i2c_x, uint8_t address, i2c_direction_type direction)
{
  if(direction == I2C_DIRECTION_TRANSMIT)
  {
    i2c_x->dt = address & 0xFE;
  }
  else
  {
    i2c_x->dt = address | 0x01;
  }
}

/**
  * @brief  send a byte through the i2c periph.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  data: byte to be transmitted.
  * @retval none
  */
void i2c_data_send(i2c_type *i2c_x, uint8_t data)
{
  i2c_x->dt = data;
}

/**
  * @brief  receive a byte through the i2c periph.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval uint8_t: received byte
  */
uint8_t i2c_data_receive(i2c_type *i2c_x)
{
  return (uint8_t)i2c_x->dt;
}

/**
  * @brief  get flag status
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  flag
  *         this parameter can be one of the following values:
  *         - I2C_STARTF_FLAG: start condition generation complete flag.
  *         - I2C_ADDR7F_FLAG: 0~7 bit address match flag.
  *         - I2C_TDC_FLAG: transmit data complete flag.
  *         - I2C_ADDRHF_FLAG: master 9~8 bit address header match flag.
  *         - I2C_STOPF_FLAG: stop condition generation complete flag.
  *         - I2C_RDBF_FLAG: receive data buffer full flag.
  *         - I2C_TDBE_FLAG: transmit data buffer empty flag.
  *         - I2C_BUSERR_FLAG: bus error flag.
  *         - I2C_ARLOST_FLAG: arbitration lost flag.
  *         - I2C_ACKFAIL_FLAG: acknowledge failure flag.
  *         - I2C_OUF_FLAG: overflow or underflow flag.
  *         - I2C_PECERR_FLAG: pec receive error flag.
  *         - I2C_TMOUT_FLAG: smbus timeout flag.
  *         - I2C_ALERTF_FLAG: smbus alert flag.
  *         - I2C_TRMODE_FLAG: transmission mode.
  *         - I2C_BUSYF_FLAG: bus busy flag transmission mode.
  *         - I2C_DIRF_FLAG: transmission direction flag.
  *         - I2C_GCADDRF_FLAG: general call address received flag.
  *         - I2C_DEVADDRF_FLAG: smbus device address received flag.
  *         - I2C_HOSTADDRF_FLAG: smbus host address received flag.
  *         - I2C_ADDR2_FLAG: own address 2 received flag.
  * @retval flag_status (SET or RESET)
  */
flag_status i2c_flag_get(i2c_type *i2c_x, uint32_t flag)
{
  __IO uint32_t reg = 0, value = 0;

  reg = flag >> 28;

  flag &= (uint32_t)0x00FFFFFF;

  if(reg == 0)
  {
    value = i2c_x->sts1;
  }
  else
  {
    flag = (uint32_t)(flag >> 16);

    value = i2c_x->sts2;
  }

  if((value & flag) != (uint32_t)RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  clear flag status
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  flag
  *         this parameter can be any combination of the following values:
  *         - I2C_BUSERR_FLAG: bus error flag.
  *         - I2C_ARLOST_FLAG: arbitration lost flag.
  *         - I2C_ACKFAIL_FLAG: acknowledge failure flag.
  *         - I2C_OUF_FLAG: overflow or underflow flag.
  *         - I2C_PECERR_FLAG: pec receive error flag.
  *         - I2C_TMOUT_FLAG: smbus timeout flag.
  *         - I2C_ALERTF_FLAG: smbus alert flag.
  * @retval none
  */
void i2c_flag_clear(i2c_type *i2c_x, uint32_t flag)
{
  i2c_x->sts1 = (uint16_t)~(flag & (uint32_t)0x00FFFFFF);
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
