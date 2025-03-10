/*!
    \file    drv_usb_hw.h
    \brief   usb hardware configuration header file

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef __DRV_USB_HW_H
#define __DRV_USB_HW_H

#include "gdusb_conf.h"
#include "wiring_time.h"
/* function declarations */
/* configure USB clock */
void usb_rcu_config (void);
/* configure USB data line gpio */
void usb_gpio_config (void);
/* configure USB interrupt */
void usb_intr_config (void);
/* initializes delay unit using Timer2 */
void usb_timer_init (void);
/* delay in micro seconds */
// void usb_udelay (const uint32_t usec);
#define usb_udelay(x)   delay(1)
/* delay in milliseconds */
#define usb_mdelay(x)   delay(x)
// void usb_mdelay (const uint32_t msec);
/* configures system clock after wakeup from STOP mode */
void system_clk_config_stop(void);

/* configure the CTC peripheral */
#ifdef USE_IRC48M
    void ctc_config(void);
#endif /* USE_IRC48M */

#ifdef USE_HOST_MODE
    void systick_config(void);

    /* configure USB VBus */
    void usb_vbus_config (void);

    /* drive USB VBus */
    void usb_vbus_drive (uint8_t State);
#endif /* USE_HOST_MODE */

#endif /* __DRV_USB_HW_H */
