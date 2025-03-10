/*!
    \file    usbh_hid_mouse.c
    \brief   USB host HID mouse driver

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

#include "usbh_hid_mouse.h"
#include "usbh_hid_parser.h"

hid_mouse_info   mouse_info;
uint32_t         mouse_report_data[1];

/* structures defining how to access items in a hid mouse report */
/* access button 1 state. */
static const hid_report_item prop_b1 =
{
    (uint8_t *)(void *)mouse_report_data + 0, /* data */
    1,     /* size */
    0,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed? */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min value device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

/* access button 2 state. */
static const hid_report_item prop_b2 = 
{
    (uint8_t *)(void *)mouse_report_data + 0, /* data */
    1,     /* size */
    1,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed? */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min value device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

/* access button 3 state. */
static const hid_report_item prop_b3 = 
{
    (uint8_t *)(void *)mouse_report_data + 0, /* data */
    1,     /* size */
    2,     /* shift */
    0,     /* count (only for array items) */
    0,     /* signed? */
    0,     /* min value read can return */
    1,     /* max value read can return */
    0,     /* min vale device can report */
    1,     /* max value device can report */
    1      /* resolution */
};

/* access x coordinate change. */
static const hid_report_item prop_x = 
{
    (uint8_t *)(void *)mouse_report_data + 1, /* data */
    8,     /* size */
    0,     /* shift */
    0,     /* count (only for array items) */
    1,     /* signed? */
    0,     /* min value read can return */
    0xFFFF,/* max value read can return */
    0,     /* min vale device can report */
    0xFFFF,/* max value device can report */
    1      /* resolution */
};

/* access y coordinate change. */
static const hid_report_item prop_y = 
{
    (uint8_t *)(void *)mouse_report_data + 2, /* data */
    8,     /* size */
    0,     /* shift */
    0,     /* count (only for array items) */
    1,     /* signed? */
    0,     /* min value read can return */
    0xFFFF,/* max value read can return */
    0,     /* min vale device can report */
    0xFFFF,/* max value device can report */
    1      /* resolution */
};

/* local function prototypes ('static') */
static usbh_status usbh_hid_mouse_decode(usb_core_driver *udev, usbh_host *uhost);

/*!
    \brief      initialize the mouse function
    \param[in]  udev: pointer to USB core instance
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
usbh_status usbh_hid_mouse_init (usb_core_driver *udev, usbh_host *uhost)
{
    usbh_hid_handler *hid = (usbh_hid_handler *)uhost->active_class->class_data;

    mouse_info.x = 0U;
    mouse_info.y = 0U;
    mouse_info.buttons[0] = 0U;
    mouse_info.buttons[1] = 0U;
    mouse_info.buttons[2] = 0U;

    mouse_report_data[0] = 0U;

    if(hid->len > sizeof(mouse_report_data)) {
        hid->len = sizeof(mouse_report_data);
    }

    hid->pdata = (uint8_t *)(void *)mouse_report_data;

    usbh_hid_fifo_init(&hid->fifo, uhost->dev_prop.data, HID_QUEUE_SIZE * sizeof(mouse_report_data));

    usr_mouse_init();

    return USBH_OK;
}

/*!
    \brief      get mouse information
    \param[in]  udev: pointer to USB core instance
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     mouse information
*/
hid_mouse_info *usbh_hid_mouse_info_get (usb_core_driver *udev, usbh_host *uhost)
{
    if(usbh_hid_mouse_decode(udev, uhost)== USBH_OK) {
        return &mouse_info;
    } else {
        return NULL;
    }
}

/*!
    \brief      decode mouse data
    \param[in]  udev: pointer to USB core instance
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
void usbh_hid_mouse_machine (usb_core_driver *udev, usbh_host *uhost)
{
    hid_mouse_info *m_pinfo = NULL;

    m_pinfo = usbh_hid_mouse_info_get(udev, uhost);

    if (NULL != m_pinfo) {
        /* handle mouse data position */
        usr_mouse_process_data(&mouse_info);
    }
}

/*!
    \brief      decode mouse information
    \param[in]  udev: pointer to USB core instance
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
static usbh_status usbh_hid_mouse_decode(usb_core_driver *udev, usbh_host *uhost)
{
    usbh_hid_handler *hid = (usbh_hid_handler *)uhost->active_class->class_data;

    if (0U == hid->len) {
        return USBH_FAIL;
    }

    /* fill report */
    if (usbh_hid_fifo_read(&hid->fifo, &mouse_report_data, hid->len) == hid->len) {
        /* decode report */
        mouse_info.x = (uint8_t)hid_item_read((hid_report_item *)&prop_x, 0U);
        mouse_info.y = (uint8_t)hid_item_read((hid_report_item *)&prop_y, 0U);

        mouse_info.buttons[0] = (uint8_t)hid_item_read((hid_report_item *)&prop_b1, 0U);
        mouse_info.buttons[1] = (uint8_t)hid_item_read((hid_report_item *)&prop_b2, 0U);
        mouse_info.buttons[2] = (uint8_t)hid_item_read((hid_report_item *)&prop_b3, 0U);

        return USBH_OK;
    }

    return USBH_FAIL;
}
