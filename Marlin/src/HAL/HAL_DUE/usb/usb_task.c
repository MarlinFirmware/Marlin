/**
 * \file
 *
 * \brief Main functions for USB composite example
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifdef ARDUINO_ARCH_SAM

#include "conf_usb.h"
#include "udc.h"

static volatile bool main_b_msc_enable = false;
static volatile bool main_b_cdc_enable = false;

void HAL_init(void) {
	udd_disable();
	UDD_SetStack(&USBD_ISR);

	// Start USB stack to authorize VBus monitoring
	udc_start();
}

void HAL_idletask(void)
{

	// Attend SD card access from the USB MSD -- Priotize access to improve speed
	int delay = 2;
	while (main_b_msc_enable && --delay > 0 ) {
		if (udi_msc_process_trans()) {
			delay = 10000;
		}
		
		/* Reset the watchdog, just to be sure */
		REG_WDT_CR = WDT_CR_WDRSTT | WDT_CR_KEY(0xA5);
	}
}

/*! \brief Example of extra USB string management
 * This feature is available for single or composite device
 * which want implement additional USB string than
 * Manufacture, Product and serial number ID.
 *
 * return true, if the string ID requested is know and managed by this functions
 */
bool usb_task_extra_string(void)
{
	static uint8_t udi_cdc_name[] = "CDC interface";
	static uint8_t udi_msc_name[] = "MSC interface";

	struct extra_strings_desc_t{
		usb_str_desc_t header;
		le16_t string[Max(sizeof(udi_cdc_name)-1, sizeof(udi_msc_name)-1)];
	};
	static UDC_DESC_STORAGE struct extra_strings_desc_t extra_strings_desc = {
		.header.bDescriptorType = USB_DT_STRING
	};

	uint8_t i;
	uint8_t *str;
	uint8_t str_lgt=0;

	// Link payload pointer to the string corresponding at request
	switch (udd_g_ctrlreq.req.wValue & 0xff) {
	case UDI_CDC_IAD_STRING_ID:
		str_lgt = sizeof(udi_cdc_name)-1;
		str = udi_cdc_name;
		break;
	case UDI_MSC_STRING_ID:
		str_lgt = sizeof(udi_msc_name)-1;
		str = udi_msc_name;
		break;
	default:
		return false;
	}

	if (str_lgt!=0) {
		for( i=0; i<str_lgt; i++) {
			extra_strings_desc.string[i] = cpu_to_le16((le16_t)str[i]);
		}
		extra_strings_desc.header.bLength = 2+ (str_lgt)*2;
		udd_g_ctrlreq.payload_size = extra_strings_desc.header.bLength;
		udd_g_ctrlreq.payload = (uint8_t *) &extra_strings_desc;
	}

	// if the string is larger than request length, then cut it
	if (udd_g_ctrlreq.payload_size > udd_g_ctrlreq.req.wLength) {
		udd_g_ctrlreq.payload_size = udd_g_ctrlreq.req.wLength;
	}
	return true;
}

bool usb_task_msc_enable(void)
{
	main_b_msc_enable = true;
	return true;
}

void usb_task_msc_disable(void)
{
	main_b_msc_enable = false;
}

bool usb_task_msc_isenabled(void)
{
	return main_b_msc_enable;
}

bool usb_task_cdc_enable(uint8_t port)
{
	main_b_cdc_enable = true;
	return true;
}

void usb_task_cdc_disable(uint8_t port)
{
	main_b_cdc_enable = false;
}

bool usb_task_cdc_isenabled(void)
{
	return main_b_cdc_enable;
}

/*! \brief Called by CDC interface
 * Callback running when CDC device have received data
 */
void usb_task_cdc_rx_notify(uint8_t port)
{
}

/*! \brief Configures communication line
 *
 * \param cfg      line configuration
 */
void usb_task_cdc_config(uint8_t port, usb_cdc_line_coding_t * cfg)
{
}

void usb_task_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) {
	} else {
	}
}

#endif