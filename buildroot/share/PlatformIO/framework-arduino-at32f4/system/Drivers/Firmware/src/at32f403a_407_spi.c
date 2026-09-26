/**
  **************************************************************************
  * @file     at32f403a_407_spi.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the spi firmware library
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

/** @defgroup SPI
  * @brief SPI driver modules
  * @{
  */

#ifdef SPI_MODULE_ENABLED

/** @defgroup SPI_private_functions
  * @{
  */

/**
  * @brief  spi reset by crm reset register
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @retval none
  */
void spi_i2s_reset(spi_type *spi_x)
{
  if(spi_x == SPI1)
  {
    crm_periph_reset(CRM_SPI1_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_SPI1_PERIPH_RESET, FALSE);
  }
  else if(spi_x == SPI2)
  {
    crm_periph_reset(CRM_SPI2_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_SPI2_PERIPH_RESET, FALSE);
  }
  else if(spi_x == SPI3)
  {
    crm_periph_reset(CRM_SPI3_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_SPI3_PERIPH_RESET, FALSE);
  }
  else if(spi_x == SPI4)
  {
    crm_periph_reset(CRM_SPI4_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_SPI4_PERIPH_RESET, FALSE);
  }
}

/**
  * @brief  spi init config with its default value.
  * @param  spi_init_struct : pointer to a spi_init_type structure which will
  *         be initialized.
  * @retval none
  */
void spi_default_para_init(spi_init_type* spi_init_struct)
{
  spi_init_struct->transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct->master_slave_mode = SPI_MODE_SLAVE;
  spi_init_struct->mclk_freq_division = SPI_MCLK_DIV_2;
  spi_init_struct->first_bit_transmission = SPI_FIRST_BIT_MSB;
  spi_init_struct->frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct->clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct->clock_phase = SPI_CLOCK_PHASE_1EDGE;
  spi_init_struct->cs_mode_selection = SPI_CS_SOFTWARE_MODE;
}

/**
  * @brief  spi init config with its setting value.
  * @param  spi_init_struct : pointer to a spi_init_type structure which will be initialized.
  * @retval none
  */
void spi_init(spi_type* spi_x, spi_init_type* spi_init_struct)
{
  spi_x->i2sctrl_bit.i2smsel = FALSE;
  if(spi_init_struct->transmission_mode == SPI_TRANSMIT_FULL_DUPLEX)
  {
    spi_x->ctrl1_bit.slben = FALSE;
    spi_x->ctrl1_bit.slbtd = FALSE;
    spi_x->ctrl1_bit.ora = FALSE;
  }
  else if(spi_init_struct->transmission_mode == SPI_TRANSMIT_SIMPLEX_RX)
  {
    spi_x->ctrl1_bit.slben = FALSE;
    spi_x->ctrl1_bit.slbtd = FALSE;
    spi_x->ctrl1_bit.ora = TRUE;
  }
  else if(spi_init_struct->transmission_mode == SPI_TRANSMIT_HALF_DUPLEX_RX)
  {
    spi_x->ctrl1_bit.slben = TRUE;
    spi_x->ctrl1_bit.slbtd = FALSE;
    spi_x->ctrl1_bit.ora = FALSE;
  }
  else if(spi_init_struct->transmission_mode == SPI_TRANSMIT_HALF_DUPLEX_TX)
  {
    spi_x->ctrl1_bit.slben = TRUE;
    spi_x->ctrl1_bit.slbtd = TRUE;
    spi_x->ctrl1_bit.ora = FALSE;
  }

  spi_x->ctrl1_bit.swcsen = spi_init_struct->cs_mode_selection;
  if((spi_init_struct->master_slave_mode == SPI_MODE_MASTER) && (spi_init_struct->cs_mode_selection == SPI_CS_SOFTWARE_MODE))
  {
    spi_x->ctrl1_bit.swcsil = TRUE;
  }
  else
  {
    spi_x->ctrl1_bit.swcsil = FALSE;
  }
  spi_x->ctrl1_bit.msten = spi_init_struct->master_slave_mode;
  if(spi_init_struct->mclk_freq_division > SPI_MCLK_DIV_256)
  {
    spi_x->ctrl2_bit.mdiv_h = 1;
    spi_x->ctrl1_bit.mdiv_l = spi_init_struct->mclk_freq_division & 0x7;
  }
  else
  {
    spi_x->ctrl2_bit.mdiv_h = 0;
    spi_x->ctrl1_bit.mdiv_l = spi_init_struct->mclk_freq_division;
  }
  spi_x->ctrl1_bit.ltf = spi_init_struct->first_bit_transmission;
  spi_x->ctrl1_bit.fbn = spi_init_struct->frame_bit_num;
  spi_x->ctrl1_bit.clkpol = spi_init_struct->clock_polarity;
  spi_x->ctrl1_bit.clkpha = spi_init_struct->clock_phase;
}

/**
  * @brief  spi next transmit crc for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @retval none
  */
void spi_crc_next_transmit(spi_type* spi_x)
{
  spi_x->ctrl1_bit.ntc = TRUE;
}

/**
  * @brief  set the crc polynomial value for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  crc_poly: crc polynomial value.
  * @retval none
  */
void spi_crc_polynomial_set(spi_type* spi_x, uint16_t crc_poly)
{
  spi_x->cpoly_bit.cpoly = crc_poly;
}

/**
  * @brief  return the crc polynomial register value for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @retval the select crc polynomial register value
  */
uint16_t spi_crc_polynomial_get(spi_type* spi_x)
{
  return spi_x->cpoly_bit.cpoly;
}

/**
  * @brief  enable or disable the hardware crc calculation for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of crc calculation.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_crc_enable(spi_type* spi_x, confirm_state new_state)
{
  spi_x->ctrl1_bit.ccen = new_state;
}

/**
  * @brief  return the transmit or the receive crc value for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  crc_direction: select transmit or receive crc value to be read
  *         - SPI_CRC_RX
  *         - SPI_CRC_TX
  * @retval the select crc register value
  */
uint16_t spi_crc_value_get(spi_type* spi_x, spi_crc_direction_type crc_direction)
{
  if(crc_direction == SPI_CRC_RX)
    return  spi_x->rcrc_bit.rcrc;
  else
    return  spi_x->tcrc_bit.tcrc;
}

/**
  * @brief  enable or disable the hardware cs output for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of spi master cs output.
  *         this parameter can be: TRUE or FALSE.
  *         note:the bit only use in spi master mode
  * @retval none
  */
void spi_hardware_cs_output_enable(spi_type* spi_x, confirm_state new_state)
{
  spi_x->ctrl2_bit.hwcsoe = new_state;
}

/**
  * @brief  set the software cs internal level for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  level: set the state of spi cs level.
  *         this parameter can be one of the following values:
  *         - SPI_SWCS_INTERNAL_LEVEL_LOW
  *         - SPI_SWCS_INTERNAL_LEVEL_HIGHT
  *         note:the bit only use when swcsen bit is set.
  *         note:when use this bit,io operation on the cs pin are invalid.
  * @retval none
  */
void spi_software_cs_internal_level_set(spi_type* spi_x, spi_software_cs_level_type level)
{
  spi_x->ctrl1_bit.swcsil = level;
}

/**
  * @brief  set the data frame bit num for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  bit_num: set the data frame size
  *         - SPI_FRAME_8BIT
  *         - SPI_FRAME_16BIT
  * @retval none
  */
void spi_frame_bit_num_set(spi_type* spi_x, spi_frame_bit_num_type bit_num)
{
  spi_x->ctrl1_bit.fbn = bit_num;
}

/**
  * @brief  set the data transmission direction in single line bidirectiona half duplex mode of the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  direction: data transfer direction
  *         this parameter can be one of the following values:
  *         - SPI_HALF_DUPLEX_DIRECTION_RX
  *         - SPI_HALF_DUPLEX_DIRECTION_TX
  * @retval none
  */
void spi_half_duplex_direction_set(spi_type* spi_x, spi_half_duplex_direction_type direction)
{
    spi_x->ctrl1_bit.slbtd = direction;
}

/**
  * @brief  enable or disable spi.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of spi.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_enable(spi_type* spi_x, confirm_state new_state)
{
  spi_x->ctrl1_bit.spien = new_state;
}

/**
  * @brief  i2s init config with its default value.
  * @param  i2s_init_struct : pointer to a i2s_init_type structure which will
  *         be initialized.
  * @retval none
  */
void i2s_default_para_init(i2s_init_type* i2s_init_struct)
{
  i2s_init_struct->operation_mode = I2S_MODE_SLAVE_TX;
  i2s_init_struct->audio_protocol = I2S_AUDIO_PROTOCOL_PHILLIPS;
  i2s_init_struct->audio_sampling_freq = I2S_AUDIO_FREQUENCY_DEFAULT;
  i2s_init_struct->data_channel_format = I2S_DATA_16BIT_CHANNEL_16BIT;
  i2s_init_struct->clock_polarity = I2S_CLOCK_POLARITY_LOW;
  i2s_init_struct->mclk_output_enable = FALSE;
}

/**
  * @brief  i2s init config with its setting value.
  * @param  i2s_init_struct : pointer to a i2s_init_type structure which will be initialized.
  * @retval none
  */
void i2s_init(spi_type* spi_x, i2s_init_type* i2s_init_struct)
{
  crm_clocks_freq_type clocks_freq;
  uint32_t i2s_sclk_index = 0;
  uint32_t i2sdiv_index = 2, i2sodd_index = 0, frequency_index = 0;

  /* i2s audio frequency config */
  if(i2s_init_struct->audio_sampling_freq == I2S_AUDIO_FREQUENCY_DEFAULT)
  {
    i2sodd_index = 0;
    i2sdiv_index = 2;
  }
  else
  {
    crm_clocks_freq_get(&clocks_freq);
    i2s_sclk_index = clocks_freq.sclk_freq;
    if((i2s_init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PCM_SHORT) || (i2s_init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PCM_LONG))
    {
      if(i2s_init_struct->mclk_output_enable == TRUE)
      {
        frequency_index = (((i2s_sclk_index / 128) * 10) / i2s_init_struct->audio_sampling_freq) + 5;
      }
      else
      {
        if(i2s_init_struct->data_channel_format == I2S_DATA_16BIT_CHANNEL_16BIT)
          frequency_index = (((i2s_sclk_index / 16) * 10) / i2s_init_struct->audio_sampling_freq) + 5;
        else
          frequency_index = (((i2s_sclk_index / 32) * 10) / i2s_init_struct->audio_sampling_freq) + 5;
      }
    }
    else
    {
      if(i2s_init_struct->mclk_output_enable == TRUE)
      {
        frequency_index = (((i2s_sclk_index / 256) * 10) / i2s_init_struct->audio_sampling_freq) + 5;
      }
      else
      {
        if(i2s_init_struct->data_channel_format == I2S_DATA_16BIT_CHANNEL_16BIT)
          frequency_index = (((i2s_sclk_index / 32) * 10) / i2s_init_struct->audio_sampling_freq) + 5;
        else
          frequency_index = (((i2s_sclk_index / 64) * 10) / i2s_init_struct->audio_sampling_freq) + 5;
      }
    }
  }
  frequency_index = frequency_index / 10;
  i2sodd_index = frequency_index & (uint16_t)0x0001;
  i2sdiv_index = (frequency_index - i2sodd_index) / 2;
  if((i2sdiv_index < 2) || (i2sdiv_index > 0x03FF))
  {
    i2sodd_index = 0;
    i2sdiv_index = 2;
  }
  spi_x->i2sclk_bit.i2sodd = i2sodd_index;
  if(i2sdiv_index > 0x00FF)
  {
    spi_x->i2sclk_bit.i2sdiv_h = (i2sdiv_index >> 8) & 0x0003;
    spi_x->i2sclk_bit.i2sdiv_l = i2sdiv_index & 0x00FF;
  }
  else
  {
    spi_x->i2sclk_bit.i2sdiv_h = 0;
    spi_x->i2sclk_bit.i2sdiv_l = i2sdiv_index;
  }

  /* i2s audio_protocol set*/
  if(i2s_init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PCM_LONG)
  {
    spi_x->i2sctrl_bit.pcmfssel = 1;
    spi_x->i2sctrl_bit.stdsel = 3;
  }
  else if(i2s_init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PCM_SHORT)
  {
    spi_x->i2sctrl_bit.pcmfssel = 0;
    spi_x->i2sctrl_bit.stdsel = 3;
  }
  else if(i2s_init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_LSB)
  {
    spi_x->i2sctrl_bit.pcmfssel = 0;
    spi_x->i2sctrl_bit.stdsel = 2;
  }
  else if(i2s_init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_MSB)
  {
    spi_x->i2sctrl_bit.pcmfssel = 0;
    spi_x->i2sctrl_bit.stdsel = 1;
  }
  else if(i2s_init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PHILLIPS)
  {
    spi_x->i2sctrl_bit.pcmfssel = 0;
    spi_x->i2sctrl_bit.stdsel = 0;
  }

  /* i2s data_channel_format set*/
  if(i2s_init_struct->data_channel_format == I2S_DATA_16BIT_CHANNEL_16BIT)
  {
    spi_x->i2sctrl_bit.i2scbn = 0;
    spi_x->i2sctrl_bit.i2sdbn = 0;
  }
  else if(i2s_init_struct->data_channel_format == I2S_DATA_16BIT_CHANNEL_32BIT)
  {
    spi_x->i2sctrl_bit.i2scbn = 1;
    spi_x->i2sctrl_bit.i2sdbn = 0;
  }
  else if(i2s_init_struct->data_channel_format == I2S_DATA_24BIT_CHANNEL_32BIT)
  {
    spi_x->i2sctrl_bit.i2scbn = 1;
    spi_x->i2sctrl_bit.i2sdbn = 1;
  }
  else if(i2s_init_struct->data_channel_format == I2S_DATA_32BIT_CHANNEL_32BIT)
  {
    spi_x->i2sctrl_bit.i2scbn = 1;
    spi_x->i2sctrl_bit.i2sdbn = 2;
  }

  spi_x->i2sctrl_bit.i2sclkpol = i2s_init_struct->clock_polarity;
  spi_x->i2sclk_bit.i2smclkoe = i2s_init_struct->mclk_output_enable;
  spi_x->i2sctrl_bit.opersel = i2s_init_struct->operation_mode;
  spi_x->i2sctrl_bit.i2smsel = TRUE;
}

/**
  * @brief  enable or disable i2s.
  * @param  spi_x: select the i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of i2s.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void i2s_enable(spi_type* spi_x, confirm_state new_state)
{
  spi_x->i2sctrl_bit.i2sen = new_state;
}

/**
  * @brief  enable or disable the specified spi/i2s interrupts.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  spi_i2s_int: specifies the spi/i2s interrupt sources to be enabled or disabled.
  *         this parameter can be one of the following values:
  *         - SPI_I2S_ERROR_INT
  *         - SPI_I2S_RDBF_INT
  *         - SPI_I2S_TDBE_INT
  * @param  new_state: new state of the specified spi/i2s interrupts.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_i2s_interrupt_enable(spi_type* spi_x, uint32_t spi_i2s_int, confirm_state new_state)
{
  if(new_state != FALSE)
  {
    spi_x->ctrl2 |= spi_i2s_int;
  }
  else
  {
    spi_x->ctrl2 &= ~spi_i2s_int;
  }
}

/**
  * @brief  enable or disable the spi/i2s dma transmitter mode.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of the dma request.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_i2s_dma_transmitter_enable(spi_type* spi_x, confirm_state new_state)
{
  spi_x->ctrl2_bit.dmaten = new_state;
}

/**
  * @brief  enable or disable the spi/i2s dma receiver mode.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of the dma request.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_i2s_dma_receiver_enable(spi_type* spi_x, confirm_state new_state)
{
  spi_x->ctrl2_bit.dmaren = new_state;
}

/**
  * @brief  spi/i2s data transmit
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  tx_data: the data to be transmit.
  *         this parameter can be:
  *         - (0x0000~0xFFFF)
  * @retval none
  */
void spi_i2s_data_transmit(spi_type* spi_x, uint16_t tx_data)
{
  spi_x->dt = tx_data;
}

/**
  * @brief  spi/i2s data receive
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @retval the received data value
  */
uint16_t spi_i2s_data_receive(spi_type* spi_x)
{
  return (uint16_t)spi_x->dt;
}

/**
  * @brief  get flag of the specified spi/i2s peripheral.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  spi_i2s_flag: select the spi/i2s flag
  *         this parameter can be one of the following values:
  *         - SPI_I2S_RDBF_FLAG
  *         - SPI_I2S_TDBE_FLAG
  *         - I2S_ACS_FLAG    (this flag only use in i2s mode)
  *         - I2S_TUERR_FLAG  (this flag only use in i2s mode)
  *         - SPI_CCERR_FLAG  (this flag only use in spi mode)
  *         - SPI_MMERR_FLAG  (this flag only use in spi mode)
  *         - SPI_I2S_ROERR_FLAG
  *         - SPI_I2S_BF_FLAG
  * @retval the new state of spi/i2s flag
  */
flag_status spi_i2s_flag_get(spi_type* spi_x, uint32_t spi_i2s_flag)
{
  flag_status status = RESET;
  if ((spi_x->sts & spi_i2s_flag) == RESET)
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
  * @brief  clear flag of the specified spi/i2s peripheral.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  spi_i2s_flag: select the spi/i2s flag
  *         this parameter can be one of the following values:
  *         - SPI_CCERR_FLAG
  *         - SPI_I2S_RDBF_FLAG
  *         - I2S_TUERR_FLAG
  *         - SPI_MMERR_FLAG
  *         - SPI_I2S_ROERR_FLAG
  * @note
  *         SPI_I2S_TDBE_FLAG  this flag is cleared when the tx buffer already contain data to be transmit.
  *         I2S_ACS_FLAG       this flag cann't cleared by software,the flag indicate the channel side(not use in pcm standard mode).
  *         SPI_I2S_BF_FLAG    this flag cann't cleared by software, it's set and cleared by hardware.
  * @retval none
  */
void spi_i2s_flag_clear(spi_type* spi_x, uint32_t spi_i2s_flag)
{
  volatile uint32_t temp = 0;
  temp = temp;
  if(spi_i2s_flag == SPI_CCERR_FLAG)
    spi_x->sts = ~SPI_CCERR_FLAG;
  else if(spi_i2s_flag == SPI_I2S_RDBF_FLAG)
    temp = REG32(&spi_x->dt);
  else if(spi_i2s_flag == I2S_TUERR_FLAG)
    temp = REG32(&spi_x->sts);
  else if(spi_i2s_flag == SPI_MMERR_FLAG)
  {
    temp = REG32(&spi_x->sts);
    spi_x->ctrl1 = spi_x->ctrl1;
  }
  else if(spi_i2s_flag == SPI_I2S_ROERR_FLAG)
  {
    temp = REG32(&spi_x->dt);
    temp = REG32(&spi_x->sts);
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
