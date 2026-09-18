/**
  **************************************************************************
  * @file     at32f403a_407_crc.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the crc firmware library
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

/** @defgroup CRC
  * @brief CRC driver modules
  * @{
  */

#ifdef CRC_MODULE_ENABLED

/** @defgroup CRC_private_functions
  * @{
  */

/**
  * @brief  reset the crc data register.
  * @param  none
  * @retval none
  */
void crc_data_reset(void)
{
  /* reset crc generator */
  CRC->ctrl_bit.rst = 0x1;
}

/**
  * @brief  compute the 32-bit crc of a given data word(32-bit).
  * @param  data: data word(32-bit) to compute its crc
  * @retval 32-bit crc
  */
uint32_t crc_one_word_calculate(uint32_t data)
{
  CRC->dt = data;
  return (CRC->dt);
}

/**
  * @brief  compute the 32-bit crc of a given buffer of data word(32-bit).
  * @param  pbuffer: pointer to the buffer containing the data to be computed
  * @param  length: length of the buffer to be computed
  * @retval 32-bit crc
  */
uint32_t crc_block_calculate(uint32_t *pbuffer, uint32_t length)
{
  uint32_t index = 0;

  for(index = 0; index < length; index++)
  {
    CRC->dt = pbuffer[index];
  }

  return (CRC->dt);
}

/**
  * @brief  return the current crc value.
  * @param  none
  * @retval 32-bit crc
  */
uint32_t crc_data_get(void)
{
  return (CRC->dt);
}

/**
  * @brief  store a 8-bit data in the common data register.
  * @param  cdt_value: 8-bit value to be stored in the common data register
  * @retval none
  */
void crc_common_data_set(uint8_t cdt_value)
{
  CRC->cdt_bit.cdt = cdt_value;
}

/**
  * @brief  return the 8-bit data stored in the common data register
  * @param  none
  * @retval 8-bit value of the common data register
  */
uint8_t crc_common_date_get(void)
{
  return (CRC->cdt_bit.cdt);
}

/**
  * @brief  set the 32-bit initial data of crc
  * @param  value: initial data
  * @retval none
  */
void crc_init_data_set(uint32_t value)
{
  CRC->idt = value;
}

/**
  * @brief  control the reversal of the bit order in the input data
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRC_REVERSE_INPUT_NO_AFFECTE
  *         - CRC_REVERSE_INPUT_BY_BYTE
  *         - CRC_REVERSE_INPUT_BY_HALFWORD
  *         - CRC_REVERSE_INPUT_BY_WORD
  * @retval none.
  */
void crc_reverse_input_data_set(crc_reverse_input_type value)
{
  CRC->ctrl_bit.revid = value;
}

/**
  * @brief  control the reversal of the bit order in the output data
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRC_REVERSE_OUTPUT_NO_AFFECTE
  *         - CRC_REVERSE_OUTPUT_DATA
  * @retval none.
  */
void crc_reverse_output_data_set(crc_reverse_output_type value)
{
  CRC->ctrl_bit.revod = value;
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
