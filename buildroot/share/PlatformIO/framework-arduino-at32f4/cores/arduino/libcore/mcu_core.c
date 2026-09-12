/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mcu_core.h"
#include "../../../system/Drivers/usbd_drivers/inc/usbd_core.h"
#include "../../../system/Drivers/usbd_class/cdc/cdc_class.h"
#include "../../../system/Drivers/usbd_class/cdc/cdc_desc.h"
#include "../../../system/Drivers/usbd_drivers/inc/usbd_int.h"




usbd_core_type usb_core_dev;
uint8_t usb_buffer[256];

/* usart global struct define */
extern linecoding_type linecoding;
void usb_usart_config(linecoding_type linecoding);
void usart_gpio_config(void);

void usb_cdc_send(const uint8_t *buf, uint16_t len);

void usb_clock48m_select(usb_clk48_s clk_s) {

    //crm_usb_interrupt_remapping_set(CRM_USB_INT73_INT74);

  if(clk_s == USB_CLK_HICK)
  {
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

    /* enable the acc calibration ready interrupt */
    crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

    /* update the c1\c2\c3 value */
    acc_write_c1(7980);
    acc_write_c2(8000);
    acc_write_c3(8020);

    /* open acc calibration */
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
  }  else  {

     // Aşağıdaki iki satır 192 mhz ayarlar
     crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_PLL);
     crm_usb_clock_div_set(CRM_USB_DIV_4);
  }
  
}


void Core_Init(void)
{
  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  system_clock_config();
  SysTick_Config(system_core_clock / 1000);

#ifdef AT32_USB_CDC
  /* usart gpio config */
  usart_gpio_config();

  /* hardware usart config: usart2 */
  usb_usart_config(linecoding);

  /* select usb 48m clcok source */
  usb_clock48m_select(USB_CLK_HICK);
  //usb_clock48m_select(USB_CLK_HEXT);

  /* enable usb clock */
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);

  /* enable usb interrupt */
  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);

  /* usb core init */
  usbd_core_init(&usb_core_dev, USB, &cdc_class_handler, &cdc_desc_handler, 0);

  /* enable usb pull-up */
  usbd_connect(&usb_core_dev);
//    GPIO_JTAG_Disable();
#endif
    Delay_Init();
    ADCx_Init(ADC1);
}


#ifdef AT32_USB_CDC
void usb_cdc_send(const uint8_t *buf, uint16_t len)
{
  usb_vcp_send_data(&usb_core_dev, (uint8_t *)buf, len);
}
#endif


/* -------------------------------------------------- */


void usb_usart_config(linecoding_type linecoding)
{
  /* USB CDC kullanılıyor, USART köprüleme yok */
  (void)linecoding;
}


void usart_gpio_config(void)
{
  /* enable iomux periph clock */
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  /* enable gpioa periph clock */
  gpio_init_type gpio_init_struct;

  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  /* dp and dm */
  gpio_init_struct.gpio_pins = GPIO_PINS_12 | GPIO_PINS_11;
  gpio_init(GPIOA, &gpio_init_struct);
}


#ifdef AT32_USB_CDC
/**
  * @brief  this function handles usb interrupt.
  * @param  none
  * @retval none
  */
void USBFS_L_CAN1_RX0_IRQHandler(void)
{
  usbd_irq_handler(&usb_core_dev);
}
#endif

/**
  * @brief  usb delay millisecond function.
  * @param  ms: number of millisecond delay
  * @retval none
  */
void usb_delay_ms(uint32_t ms)
{
  /* user can define self delay function */
  delay_ms(ms);
}

/**
  * @brief  usb delay microsecond function.
  * @param  us: number of microsecond delay
  * @retval none
  */
void usb_delay_us(uint32_t us)
{
  delay_us(us);
}
