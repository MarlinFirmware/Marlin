/**
  **************************************************************************
  * @file     at32f403a_407_gpio.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains all the functions for the gpio firmware library
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

/** @defgroup GPIO
  * @brief GPIO driver modules
  * @{
  */

#ifdef GPIO_MODULE_ENABLED

/** @defgroup GPIO_private_functions
  * @{
  */

/**
  * @brief  reset the gpio register
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @retval none
  */
void gpio_reset(gpio_type *gpio_x)
{
  if(gpio_x == GPIOA)
  {
    crm_periph_reset(CRM_GPIOA_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_GPIOA_PERIPH_RESET, FALSE);
  }
  else if(gpio_x == GPIOB)
  {
    crm_periph_reset(CRM_GPIOB_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_GPIOB_PERIPH_RESET, FALSE);
  }
  else if(gpio_x == GPIOC)
  {
    crm_periph_reset(CRM_GPIOC_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_GPIOC_PERIPH_RESET, FALSE);
  }
  else if(gpio_x == GPIOD)
  {
    crm_periph_reset(CRM_GPIOD_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_GPIOD_PERIPH_RESET, FALSE);
  }
  else if(gpio_x == GPIOE)
  {
    crm_periph_reset(CRM_GPIOE_PERIPH_RESET, TRUE);
    crm_periph_reset(CRM_GPIOE_PERIPH_RESET, FALSE);
  }
}

/**
  * @brief  reset the mux functions (remap, event control
            and exint configuration) registers to their default values.
  * @param  none
  * @retval none
  */
void gpio_iomux_reset(void)
{
  crm_periph_reset(CRM_IOMUX_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_IOMUX_PERIPH_RESET, FALSE);
}

/**
  * @brief  initialize the gpio peripheral.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  gpio_init_struct: pointer to gpio init structure.
  * @retval none
  */
void gpio_init(gpio_type *gpio_x, gpio_init_type *gpio_init_struct)
{
  uint32_t temp;
  uint16_t pinx_value, pin_index;

  pin_index = (uint16_t)gpio_init_struct->gpio_pins;

  /* pinx_value indecate pin grounp bit[3:0] from modey[1:0] confy[1:0] */

  /* pin input analog config */
  if(gpio_init_struct->gpio_mode == GPIO_MODE_ANALOG)
  {
    pinx_value = 0x00;
  }
  /* pin input config */
  else if(gpio_init_struct->gpio_mode == GPIO_MODE_INPUT)
  {
    pinx_value = gpio_init_struct->gpio_pull & 0x0F;

    if(gpio_init_struct->gpio_pull == GPIO_PULL_UP)
    {
      gpio_x->scr = pin_index;
    }
    else if(gpio_init_struct->gpio_pull == GPIO_PULL_DOWN)
    {
      gpio_x->clr = pin_index;
    }
  }
  else
  {
    pinx_value = (gpio_init_struct->gpio_mode & 0x08) | (gpio_init_struct->gpio_out_type & 0x04) | \
                 (gpio_init_struct->gpio_drive_strength & 0x03);
  }

  /* pin 0~7 config */
  if(((uint32_t)pin_index & ((uint32_t)0x00FF)) != 0x00)
  {
    for(temp = 0; temp < 0x08; temp++)
    {
      if((1 << temp) & pin_index)
      {
        gpio_x->cfglr &= (uint32_t)~(0x0F << (temp * 4));
        gpio_x->cfglr |= (uint32_t)(pinx_value << (temp * 4));
      }
    }
  }

  /* pin 8~15 config */
  if(pin_index > 0x00ff)
  {
    pin_index = pin_index >> 8;

    for(temp = 0; temp < 0x8; temp++)
    {
      if((1 << temp) & pin_index)
      {
        gpio_x->cfghr &= (uint32_t)~(0xf << (temp * 4));
        gpio_x->cfghr |= (uint32_t)(pinx_value << (temp * 4));
      }
    }
  }
}

/**
  * @brief  fill each gpio_init_type member with its default value.
  * @param  gpio_init_struct : pointer to a gpio_init_type structure which will be initialized.
  * @retval none
  */
void gpio_default_para_init(gpio_init_type *gpio_init_struct)
{
  /* reset gpio init structure parameters values */
  gpio_init_struct->gpio_pins  = GPIO_PINS_ALL;
  gpio_init_struct->gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct->gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct->gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct->gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
}

/**
  * @brief  read the specified input port pin.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  pins: gpio pin number
  *         this parameter can be one of the following values:
  *         - GPIO_PINS_0
  *         - GPIO_PINS_1
  *         - GPIO_PINS_2
  *         - GPIO_PINS_3
  *         - GPIO_PINS_4
  *         - GPIO_PINS_5
  *         - GPIO_PINS_6
  *         - GPIO_PINS_7
  *         - GPIO_PINS_8
  *         - GPIO_PINS_9
  *         - GPIO_PINS_10
  *         - GPIO_PINS_11
  *         - GPIO_PINS_12
  *         - GPIO_PINS_13
  *         - GPIO_PINS_14
  *         - GPIO_PINS_15
  * @retval flag_status (SET or RESET)
  */
flag_status gpio_input_data_bit_read(gpio_type *gpio_x, uint16_t pins)
{
  flag_status status = RESET;

  if(pins != (pins & gpio_x->idt))
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
  * @brief  read the specified gpio input data port.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @retval gpio input data port value.
  */
uint16_t gpio_input_data_read(gpio_type *gpio_x)
{
  return ((uint16_t)(gpio_x->idt));
}

/**
  * @brief  read the specified output port pin.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  pins: gpio pin number
  *         this parameter can be one of the following values:
  *         - GPIO_PINS_0
  *         - GPIO_PINS_1
  *         - GPIO_PINS_2
  *         - GPIO_PINS_3
  *         - GPIO_PINS_4
  *         - GPIO_PINS_5
  *         - GPIO_PINS_6
  *         - GPIO_PINS_7
  *         - GPIO_PINS_8
  *         - GPIO_PINS_9
  *         - GPIO_PINS_10
  *         - GPIO_PINS_11
  *         - GPIO_PINS_12
  *         - GPIO_PINS_13
  *         - GPIO_PINS_14
  *         - GPIO_PINS_15
  * @retval flag_status (SET or RESET)
  */
flag_status gpio_output_data_bit_read(gpio_type *gpio_x, uint16_t pins)
{
  flag_status status = RESET;

  if((gpio_x->odt & pins) != RESET)
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
  * @brief  read the specified gpio ouput data port.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @retval gpio input data port value.
  */
uint16_t gpio_output_data_read(gpio_type *gpio_x)
{
  return ((uint16_t)(gpio_x->odt));
}

/**
  * @brief  set the selected data port bits.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  pins: gpio pin number
  *         parameter can be any combination of gpio_pin_x, gpio_pin_x as following values:
  *         - GPIO_PINS_0
  *         - GPIO_PINS_1
  *         - GPIO_PINS_2
  *         - GPIO_PINS_3
  *         - GPIO_PINS_4
  *         - GPIO_PINS_5
  *         - GPIO_PINS_6
  *         - GPIO_PINS_7
  *         - GPIO_PINS_8
  *         - GPIO_PINS_9
  *         - GPIO_PINS_10
  *         - GPIO_PINS_11
  *         - GPIO_PINS_12
  *         - GPIO_PINS_13
  *         - GPIO_PINS_14
  *         - GPIO_PINS_15
  *         - GPIO_PINS_ALL
  * @retval none
  */
void gpio_bits_set(gpio_type *gpio_x, uint16_t pins)
{
  gpio_x->scr = pins;
}

/**
  * @brief  clear the selected data port bits.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  pins: gpio pin number
  *         parameter can be any combination of gpio_pin_x, gpio_pin_x as following values:
  *         - GPIO_PINS_0
  *         - GPIO_PINS_1
  *         - GPIO_PINS_2
  *         - GPIO_PINS_3
  *         - GPIO_PINS_4
  *         - GPIO_PINS_5
  *         - GPIO_PINS_6
  *         - GPIO_PINS_7
  *         - GPIO_PINS_8
  *         - GPIO_PINS_9
  *         - GPIO_PINS_10
  *         - GPIO_PINS_11
  *         - GPIO_PINS_12
  *         - GPIO_PINS_13
  *         - GPIO_PINS_14
  *         - GPIO_PINS_15
  *         - GPIO_PINS_ALL
  * @retval none
  */
void gpio_bits_reset(gpio_type *gpio_x, uint16_t pins)
{
  gpio_x->clr = pins;
}

/**
  * @brief  set or clear the selected data port bit.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  pins: gpio pin number
  *         parameter can be any combination of gpio_pin_x, gpio_pin_x as following values:
  *         - GPIO_PINS_0
  *         - GPIO_PINS_1
  *         - GPIO_PINS_2
  *         - GPIO_PINS_3
  *         - GPIO_PINS_4
  *         - GPIO_PINS_5
  *         - GPIO_PINS_6
  *         - GPIO_PINS_7
  *         - GPIO_PINS_8
  *         - GPIO_PINS_9
  *         - GPIO_PINS_10
  *         - GPIO_PINS_11
  *         - GPIO_PINS_12
  *         - GPIO_PINS_13
  *         - GPIO_PINS_14
  *         - GPIO_PINS_15
  *         - GPIO_PINS_ALL
  * @param  bit_state: specifies the value to be written to the selected bit (TRUE or FALSE).
  * @retval none
  */
void gpio_bits_write(gpio_type *gpio_x, uint16_t pins, confirm_state bit_state)
{
  if(bit_state != FALSE)
  {
    gpio_x->scr = pins;
  }
  else
  {
    gpio_x->clr = pins;
  }
}

/**
  * @brief  write data to the specified gpio data port.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  port_value: specifies the value to be written to the port output data register.
  * @retval none
  */
void gpio_port_wirte(gpio_type *gpio_x, uint16_t port_value)
{
  gpio_x->odt = port_value;
}

/**
  * @brief  write protect gpio pins configuration registers.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  pins: gpio pin number
  *         this parameter can be any combination of the following:
  *         - GPIO_PINS_0
  *         - GPIO_PINS_1
  *         - GPIO_PINS_2
  *         - GPIO_PINS_3
  *         - GPIO_PINS_4
  *         - GPIO_PINS_5
  *         - GPIO_PINS_6
  *         - GPIO_PINS_7
  *         - GPIO_PINS_8
  *         - GPIO_PINS_9
  *         - GPIO_PINS_10
  *         - GPIO_PINS_11
  *         - GPIO_PINS_12
  *         - GPIO_PINS_13
  *         - GPIO_PINS_14
  *         - GPIO_PINS_15
  *         - GPIO_PINS_ALL
  * @retval none
  */
void gpio_pin_wp_config(gpio_type *gpio_x, uint16_t pins)
{
  uint32_t temp = 0x00010000;

  temp |= pins;
  /* set wpen bit */
  gpio_x->wpr = temp;
  /* reset wpen bit */
  gpio_x->wpr =  pins;
  /* set wpen bit */
  gpio_x->wpr = temp;
  /* read wpen bit*/
  temp = gpio_x->wpr;
  /* read wpen bit*/
  temp = gpio_x->wpr;
}

/**
  * @brief  enable or disable gpio pins huge driven.
  * @param  gpio_x: to select the gpio peripheral.
  *         this parameter can be one of the following values:
  *         GPIOA, GPIOB, GPIOC, GPIOD, GPIOE.
  * @param  pins: gpio pin number
  *         parameter can be any combination of gpio_pin_x, gpio_pin_x as following values:
  *         - GPIO_PINS_0
  *         - GPIO_PINS_1
  *         - GPIO_PINS_2
  *         - GPIO_PINS_3
  *         - GPIO_PINS_4
  *         - GPIO_PINS_5
  *         - GPIO_PINS_6
  *         - GPIO_PINS_7
  *         - GPIO_PINS_8
  *         - GPIO_PINS_9
  *         - GPIO_PINS_10
  *         - GPIO_PINS_11
  *         - GPIO_PINS_12
  *         - GPIO_PINS_13
  *         - GPIO_PINS_14
  *         - GPIO_PINS_15
  *         - GPIO_PINS_ALL
  * @param  new_state: new state of the slew rate.
  *         this parameter can be: true or false.
  * @retval none
  */
void gpio_pins_huge_driven_config(gpio_type *gpio_x, uint16_t pins, confirm_state new_state)
{
  if(new_state != FALSE)
  {
    gpio_x->hdrv |= pins;
  }
  else
  {
    gpio_x->hdrv &= ~pins;
  }
}

/**
  * @brief  select the gpio pin used as event output.
  * @param  gpio_port_source: select the gpio port to be used as source
  *         for event output.
  *         this parameter can be one of the following values:
  *         - GPIO_PORT_SOURCE_GPIOA
  *         - GPIO_PORT_SOURCE_GPIOB
  *         - GPIO_PORT_SOURCE_GPIOC
  *         - GPIO_PORT_SOURCE_GPIOD
  *         - GPIO_PORT_SOURCE_GPIOE
  * @param  gpio_pin_source: specifies the pin for the event output.
  *         this parameter can be one of the following values:
  *         - GPIO_PINS_SOURCE0
  *         - GPIO_PINS_SOURCE1
  *         - GPIO_PINS_SOURCE2
  *         - GPIO_PINS_SOURCE3
  *         - GPIO_PINS_SOURCE4
  *         - GPIO_PINS_SOURCE5
  *         - GPIO_PINS_SOURCE6
  *         - GPIO_PINS_SOURCE7
  *         - GPIO_PINS_SOURCE8
  *         - GPIO_PINS_SOURCE9
  *         - GPIO_PINS_SOURCE10
  *         - GPIO_PINS_SOURCE11
  *         - GPIO_PINS_SOURCE12
  *         - GPIO_PINS_SOURCE13
  *         - GPIO_PINS_SOURCE14
  *         - GPIO_PINS_SOURCE15
  * @retval none
  */
void gpio_event_output_config(gpio_port_source_type gpio_port_source, gpio_pins_source_type gpio_pin_source)
{
  uint32_t tmpreg = 0x00;

  tmpreg = IOMUX->evtout;

  /* clear the port[6:4] and pin[3:0] bits */
  tmpreg &= 0x80;
  tmpreg |= (uint32_t)gpio_port_source << 0x04;
  tmpreg |= gpio_pin_source;
  IOMUX->evtout = tmpreg;
}

/**
  * @brief  enable or disable the event output.
  * @param  confirm_state: new state of the event output.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void gpio_event_output_enable(confirm_state new_state)
{
  IOMUX->evtout_bit.evoen = new_state;
}

/**
  * @brief  iomux remap and debug i/o configuration.
  * @param  gpio_remap: select the pin to remap.
  *         this parameter can be one of the following values:
  *         - SPI1_MUX_01                   - SPI1_MUX_10                   - SPI1_MUX_11                  - I2C1_MUX
  *         - USART1_MUX                    - USART2_MUX                    - USART3_MUX_01                - USART3_MUX_11
  *         - TMR1_MUX_01                   - TMR1_MUX_11                   - TMR2_MUX_01                  - TMR2_MUX_10
  *         - TMR2_MUX_11                   - TMR3_MUX_10                   - TMR3_MUX_11                  - TMR4_MUX
  *         - CAN_MUX_10                    - CAN_MUX_11                    - PD01_MUX                     - TMR5CH4_MUX
  *         - ADC1_EXTRGPRE_MUX             - ADC1_EXTRGORD_MUX             - ADC2_EXTRGPRE_MUX            - ADC2_EXTRGORD_MUX
  *         - EMAC_MUX                      - CAN2_MUX                      - MII_RMII_SEL_MUX             - SWJTAG_MUX_001
  *         - SWJTAG_MUX_010                - SWJTAG_MUX_100                - SPI3_MUX                     - TMR2ITR1_MUX
  *         - PTP_PPS_MUX                   - TMR9_MUX                      - XMC_NADV_MUX                 - SPI4_MUX
  *         - I2C3_MUX                      - SDIO2_MUX01                   - SDIO2_MUX10                  - SDIO2_MUX11
  *         - EXT_SPIM_EN_MUX               - TMR9_GMUX                     - TMR1_GMUX_0001               - TMR1_GMUX_0011
  *         - TMR2_GMUX_01                  - TMR2_GMUX_10                  - TMR2_GMUX_11                 - TMR2ITR1_GMUX_10
  *         - TMR2ITR1_GMUX_11              - TMR3_GMUX_0010                - TMR3_GMUX_0011               - TMR4_GMUX_0001
  *         - TMR5CH4_GMUX                  - UART5_GMUX_0001               - I2C1_GMUX_0001               - I2C3_GMUX_0001
  *         - SPI1_GMUX_0001                - SPI1_GMUX_0010                - SPI1_GMUX_0011               - SPI2_GMUX_0001
  *         - SPI2_GMUX_0010                - SPI3_GMUX_0001                - SPI3_GMUX_0010               - SPI3_GMUX_0011
  *         - SPI4_GMUX_0001                - SPI4_GMUX_0010                - SPI4_GMUX_0011               - CAN1_GMUX_0010
  *         - CAN1_GMUX_0011                - CAN2_GMUX_0001                - SDIO2_GMUX_0001              - SDIO2_GMUX_0010
  *         - SDIO2_GMUX_0011               - USART1_GMUX_0001              - USART2_GMUX_0001             - USART3_GMUX_0001
  *         - USART3_GMUX_0011              - UART4_GMUX_0010               - EXT_SPIM_GMUX_1000           - EXT_SPIM_GMUX_1001
  *         - ADC1_ETP_GMUX                 - ADC1_ETO_GMUX                 - ADC2_ETP_GMUX                - ADC2_ETO_GMUX
  *         - SWJTAG_GMUX_001               - SWJTAG_GMUX_010               - SWJTAG_GMUX_100              - PD01_GMUX
  *         - XMC_GMUX_001                  - XMC_GMUX_010                  - XMC_NADV_GMUX                - EMAC_GMUX_01
  *         - MII_RMII_SEL_GMUX             - PTP_PPS_GMUX                  - USART6_GMUX                  - UART7_GMUX
  *         - UART8_GMUX
  * @param  new_state: (TRUE or FALSE)
  * @retval none
  */
void gpio_pin_remap_config(uint32_t gpio_remap, confirm_state new_state)
{
  uint32_t reg_addr, remap_mask;
  uint8_t bit_offset, bit_num, bit_val;

  /* get register address, bit offset, bit number and remap value */
  reg_addr = IOMUX_BASE + (gpio_remap >> 24);
  bit_offset = (gpio_remap >> 16) & 0xFF;
  bit_num = (gpio_remap >> 8) & 0xFF;
  bit_val = gpio_remap & 0xFF;

  /* get remap mask value */
  remap_mask = 0xFFFFFFFF << (32 - bit_num - bit_offset);
  remap_mask = remap_mask >> (32 - bit_num - bit_offset);
  remap_mask = remap_mask >> bit_offset;
  remap_mask = remap_mask << bit_offset;

  /* clear remap value */
  *(uint32_t*)reg_addr &= ~remap_mask;

  if(new_state != FALSE)
  {
    /* set remap value */
    *(uint32_t*)reg_addr |= (uint32_t)(bit_val << bit_offset);
  }
}

/**
  * @brief  select the gpio pin used as exint line.
  * @param  gpio_port_source: select the gpio port to be used as source for exint.
  *         this parameter can be one of the following values:
  *         - GPIO_PORT_SOURCE_GPIOA
  *         - GPIO_PORT_SOURCE_GPIOB
  *         - GPIO_PORT_SOURCE_GPIOC
  *         - GPIO_PORT_SOURCE_GPIOD
  *         - GPIO_PORT_SOURCE_GPIOE
  *         - GPIO_PORT_SOURCE_GPIOF
  *         - GPIO_PORT_SOURCE_GPIOG
  * @param  gpio_pin_source: specifies the pin for the event output.
  *         this parameter can be one of the following values:
  *         - GPIO_PINS_SOURCE0
  *         - GPIO_PINS_SOURCE1
  *         - GPIO_PINS_SOURCE2
  *         - GPIO_PINS_SOURCE3
  *         - GPIO_PINS_SOURCE4
  *         - GPIO_PINS_SOURCE5
  *         - GPIO_PINS_SOURCE6
  *         - GPIO_PINS_SOURCE7
  *         - GPIO_PINS_SOURCE8
  *         - GPIO_PINS_SOURCE9
  *         - GPIO_PINS_SOURCE10
  *         - GPIO_PINS_SOURCE11
  *         - GPIO_PINS_SOURCE12
  *         - GPIO_PINS_SOURCE13
  *         - GPIO_PINS_SOURCE14
  *         - GPIO_PINS_SOURCE15
  * @retval none
  */
void gpio_exint_line_config(gpio_port_source_type gpio_port_source, gpio_pins_source_type gpio_pin_source)
{
  uint32_t tmp = 0x00;
  tmp = ((uint32_t)0x0F) << (0x04 * (gpio_pin_source & (uint8_t)0x03));

  switch (gpio_pin_source >> 0x02)
  {
    case 0:
      IOMUX->exintc1 &= ~tmp;
      IOMUX->exintc1 |= (((uint32_t)gpio_port_source) << (0x04 * (gpio_pin_source & (uint8_t)0x03)));
      break;
    case 1:
      IOMUX->exintc2 &= ~tmp;
      IOMUX->exintc2 |= (((uint32_t)gpio_port_source) << (0x04 * (gpio_pin_source & (uint8_t)0x03)));
      break;
    case 2:
      IOMUX->exintc3 &= ~tmp;
      IOMUX->exintc3 |= (((uint32_t)gpio_port_source) << (0x04 * (gpio_pin_source & (uint8_t)0x03)));
      break;
    case 3:
      IOMUX->exintc4 &= ~tmp;
      IOMUX->exintc4 |= (((uint32_t)gpio_port_source) << (0x04 * (gpio_pin_source & (uint8_t)0x03)));
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
