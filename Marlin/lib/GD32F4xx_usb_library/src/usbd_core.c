/*!
    \file    usbd_core.c
    \brief   USB device mode core functions

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

#include "gdusbd_core.h"
#include "gdusbd_enum.h"
#include "drv_usb_hw.h"

/* endpoint type */
const uint32_t ep_type[] = {
    [USB_EP_ATTR_CTL]  = (uint32_t)USB_EPTYPE_CTRL,
    [USB_EP_ATTR_BULK] = (uint32_t)USB_EPTYPE_BULK,
    [USB_EP_ATTR_INT]  = (uint32_t)USB_EPTYPE_INTR,
    [USB_EP_ATTR_ISO]  = (uint32_t)USB_EPTYPE_ISOC
};

/*!
    \brief      initializes the USB device-mode stack and load the class driver
    \param[in]  udev: pointer to USB core instance
    \param[in]  core: USB core type
    \param[in]  desc: pointer to USB descriptor
    \param[in]  class_core: class driver
    \param[out] none
    \retval     none
*/
void usbd_init (usb_core_driver *udev, usb_core_enum core, usb_desc *desc, usb_class_core *class_core)
{
    udev->dev.desc = desc;

    /* class callbacks */
    udev->dev.class_core = class_core;

    /* create serial string */
    serial_string_get(udev->dev.desc->strings[STR_IDX_SERIAL]);

    /* configure USB capabilities */
    (void)usb_basic_init (&udev->bp, &udev->regs, core);

    usb_globalint_disable(&udev->regs);

    /* initializes the USB core*/
    (void)usb_core_init (udev->bp, &udev->regs);

    /* set device disconnect */
    usbd_disconnect (udev);

#ifndef USE_OTG_MODE
    usb_curmode_set(&udev->regs, DEVICE_MODE);
#endif

    /* initializes device mode */
    (void)usb_devcore_init (udev);

    usb_globalint_enable(&udev->regs);

    /* set device connect */
    usbd_connect (udev);

    udev->dev.cur_status = (uint8_t)USBD_DEFAULT;
}

/*!
    \brief      endpoint initialization
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_desc: pointer to endpoint descriptor
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_setup (usb_core_driver *udev, const usb_desc_ep *ep_desc)
{
    usb_transc *transc;

    uint8_t ep_addr = ep_desc->bEndpointAddress;
    uint16_t max_len = ep_desc->wMaxPacketSize;

    /* set endpoint direction */
    if (EP_DIR(ep_addr)) {
        transc = &udev->dev.transc_in[EP_ID(ep_addr)];

        transc->ep_addr.dir = 1U;
    } else {
        transc = &udev->dev.transc_out[ep_addr];

        transc->ep_addr.dir = 0U;
    }

    transc->ep_addr.num = EP_ID(ep_addr);
    transc->max_len = max_len;
    transc->ep_type = (uint8_t)ep_type[ep_desc->bmAttributes & (uint8_t)USB_EPTYPE_MASK];

    /* active USB endpoint function */
    (void)usb_transc_active (udev, transc);

    return 0U;
}

/*!
    \brief      configure the endpoint when it is disabled
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_clear (usb_core_driver *udev, uint8_t ep_addr)
{
    usb_transc *transc;

    if (EP_DIR(ep_addr)) {
        transc = &udev->dev.transc_in[EP_ID(ep_addr)];
    } else {
        transc = &udev->dev.transc_out[ep_addr];
    }

    /* deactivate USB endpoint function */
    (void)usb_transc_deactivate (udev, transc);

    return 0U;
}

/*!
    \brief      endpoint prepare to receive data
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[in]  pbuf: user buffer address pointer
    \param[in]  len: buffer length
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_recev (usb_core_driver *udev, uint8_t ep_addr, uint8_t *pbuf, uint32_t len)
{
    usb_transc *transc = &udev->dev.transc_out[EP_ID(ep_addr)];

    /* setup the transfer */
    transc->xfer_buf = pbuf;
    transc->xfer_len = len;
    transc->xfer_count = 0U;

    if ((uint8_t)USB_USE_DMA == udev->bp.transfer_mode) {
        transc->dma_addr = (uint32_t)pbuf;
    }

    /* start the transfer */
    (void)usb_transc_outxfer (udev, transc);

    return 0U;
}

/*!
    \brief      endpoint prepare to transmit data
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[in]  pbuf: transmit buffer address pointer
    \param[in]  len: buffer length
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_send (usb_core_driver *udev, uint8_t ep_addr, uint8_t *pbuf, uint32_t len)
{
    usb_transc *transc = &udev->dev.transc_in[EP_ID(ep_addr)];

    /* setup the transfer */
    transc->xfer_buf = pbuf;
    transc->xfer_len = len;
    transc->xfer_count = 0U;

    if ((uint8_t)USB_USE_DMA == udev->bp.transfer_mode) {
        transc->dma_addr = (uint32_t)pbuf;
    }

    /* start the transfer */
    (void)usb_transc_inxfer (udev, transc);

    return 0U;
}

/*!
    \brief      set an endpoint to STALL status
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_stall (usb_core_driver *udev, uint8_t ep_addr)
{
    usb_transc *transc = NULL;

    if (EP_DIR(ep_addr)) {
        transc = &udev->dev.transc_in[EP_ID(ep_addr)];
    } else {
        transc = &udev->dev.transc_out[ep_addr];
    }

    transc->ep_stall = 1U;

    (void)usb_transc_stall (udev, transc);

    return (0U);
}

/*!
    \brief      clear endpoint STALLed status
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_stall_clear (usb_core_driver *udev, uint8_t ep_addr)
{
    usb_transc *transc = NULL;

    if (EP_DIR(ep_addr)) {
        transc = &udev->dev.transc_in[EP_ID(ep_addr)];
    } else {
        transc = &udev->dev.transc_out[ep_addr];
    }

    transc->ep_stall = 0U;

    (void)usb_transc_clrstall (udev, transc);

    return (0U);
}

/*!
    \brief      flush the endpoint FIFOs
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[out] none
    \retval     none
*/
uint32_t usbd_fifo_flush (usb_core_driver *udev, uint8_t ep_addr)
{
    if (EP_DIR(ep_addr)) {
        (void)usb_txfifo_flush (&udev->regs, EP_ID(ep_addr));
    } else {
        (void)usb_rxfifo_flush (&udev->regs);
    }

    return (0U);
}

/*!
    \brief      device connect
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
void usbd_connect (usb_core_driver *udev)
{
#ifndef USE_OTG_MODE
    /* connect device */
    usb_dev_connect (udev);

    usb_mdelay(3U);
#endif /* USE_OTG_MODE */
}

/*!
    \brief      device disconnect
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
void usbd_disconnect (usb_core_driver *udev)
{
#ifndef USE_OTG_MODE
    /* disconnect device for 3ms */
    usb_dev_disconnect (udev);

    usb_mdelay(3U);
#endif /* USE_OTG_MODE */
}
