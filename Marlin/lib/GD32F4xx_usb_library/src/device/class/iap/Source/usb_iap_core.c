/*!
    \file    usb_iap_core.c
    \brief   IAP driver

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

#include "usb_iap_core.h"
#include "flash_operation.h"
#include <string.h>

#define USBD_VID                     0x28E9U
#define USBD_PID                     0x0228U

/* Note:it should use the C99 standard when compiling the below codes */
/* USB standard device descriptor */
__ALIGN_BEGIN const usb_desc_dev iap_dev_desc __ALIGN_END =
{
    .header = 
     {
         .bLength          = USB_DEV_DESC_LEN, 
         .bDescriptorType  = USB_DESCTYPE_DEV
     },
    .bcdUSB                = 0x0200U,
    .bDeviceClass          = 0x00U,
    .bDeviceSubClass       = 0x00U,
    .bDeviceProtocol       = 0x00U,
    .bMaxPacketSize0       = USB_FS_EP0_MAX_LEN,
    .idVendor              = USBD_VID,
    .idProduct             = USBD_PID,
    .bcdDevice             = 0x0100U,
    .iManufacturer         = STR_IDX_MFC,
    .iProduct              = STR_IDX_PRODUCT,
    .iSerialNumber         = STR_IDX_SERIAL,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};

__ALIGN_BEGIN const usb_hid_desc_config_set iap_config_desc __ALIGN_END = 
{
    .config = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_config), 
             .bDescriptorType = USB_DESCTYPE_CONFIG 
         },
        .wTotalLength         = USB_DESC_LEN_IAP_CONFIG_SET,
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x00U,
        .bmAttributes         = 0x80U,
        .bMaxPower            = 0x32U
    },

    .hid_itf = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_itf), 
             .bDescriptorType = USB_DESCTYPE_ITF 
         },
        .bInterfaceNumber     = 0x00U,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x02U,
        .bInterfaceClass      = USB_HID_CLASS,
        .bInterfaceSubClass   = 0x00U,
        .bInterfaceProtocol   = 0x00U,
        .iInterface           = 0x00U
    },

    .hid_vendor = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_hid), 
             .bDescriptorType = USB_DESCTYPE_HID 
         },
        .bcdHID               = 0x0111U,
        .bCountryCode         = 0x00U,
        .bNumDescriptors      = 0x01U,
        .bDescriptorType      = USB_DESCTYPE_REPORT,
        .wDescriptorLength    = USB_DESC_LEN_IAP_REPORT,
    },

    .hid_epin = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_ep), 
             .bDescriptorType = USB_DESCTYPE_EP 
         },
        .bEndpointAddress     = IAP_IN_EP,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = IAP_IN_PACKET,
        .bInterval            = 0x01U
    },

    .hid_epout = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_ep), 
             .bDescriptorType = USB_DESCTYPE_EP 
         },
        .bEndpointAddress     = IAP_OUT_EP,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = IAP_OUT_PACKET,
        .bInterval            = 0x01U
    }
};

/* USB language ID Descriptor */
static __ALIGN_BEGIN const usb_desc_LANGID usbd_language_id_desc __ALIGN_END = 
{
    .header = 
     {
         .bLength         = sizeof(usb_desc_LANGID), 
         .bDescriptorType = USB_DESCTYPE_STR
     },
    .wLANGID              = ENG_LANGID
};

/* USB manufacture string */
static __ALIGN_BEGIN const usb_desc_str manufacturer_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(10U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'i', 'g', 'a', 'D', 'e', 'v', 'i', 'c', 'e'}
};

/* USB product string */
static __ALIGN_BEGIN const usb_desc_str product_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(12U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', '-', 'U', 'S', 'B', '_', 'I', 'A', 'P'}
};

/* USBD serial string */
static __ALIGN_BEGIN usb_desc_str serial_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(2U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     }
};

void *const usbd_iap_strings[] = 
{
    [STR_IDX_LANGID]  = (uint8_t *)&usbd_language_id_desc,
    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string
};

usb_desc iap_desc = {
    .dev_desc    = (uint8_t *)&iap_dev_desc,
    .config_desc = (uint8_t *)&iap_config_desc,
    .strings     = usbd_iap_strings
};

/* local function prototypes ('static') */
static uint8_t iap_init (usb_dev *udev, uint8_t config_index);
static uint8_t iap_deinit (usb_dev *udev, uint8_t config_index);
static uint8_t iap_req_handler (usb_dev *udev, usb_req *req);
static uint8_t iap_data_out (usb_dev *udev, uint8_t ep_num);

/* IAP requests management functions */
static void iap_req_erase     (usb_dev *udev);
static void iap_req_dnload    (usb_dev *udev);
static void iap_req_optionbyte(usb_dev *udev, uint8_t option_num);
static void iap_req_leave     (usb_dev *udev);
static void iap_address_send  (usb_dev *udev);

usb_class_core iap_class = {
    .init            = iap_init,
    .deinit          = iap_deinit,
    .req_proc        = iap_req_handler,
    .data_out        = iap_data_out
};

/* USB custom HID device report descriptor */
__ALIGN_BEGIN const uint8_t iap_report_desc[USB_DESC_LEN_IAP_REPORT] __ALIGN_END =
{
    0x05, 0x01,     /* USAGE_PAGE (Generic Desktop) */
    0x09, 0x00,     /* USAGE (Custom Device)        */
    0xa1, 0x01,     /* COLLECTION (Application)     */

    /* IAP command and data */
    0x85, 0x01,     /* REPORT_ID (0x01)          */
    0x09, 0x01,     /* USAGE (IAP command)       */
    0x15, 0x00,     /* LOGICAL_MINIMUM (0)       */
    0x25, 0xff,     /* LOGICAL_MAXIMUM (255)     */
    0x75, 0x08,     /* REPORT_SIZE (8)           */
#ifdef USE_USB_FS
    0x95, REPORT_OUT_COUNT,
#else
    #ifdef USE_ULPI_PHY
        0x96, BYTE_LOW(REPORT_OUT_COUNT), BYTE_HIGH(REPORT_OUT_COUNT),
    #elif defined(USE_EMBEDDED_PHY)
        0x95, REPORT_OUT_COUNT,
    #endif
#endif
    0x91, 0x82,     /* OUTPUT (Data,Var,Abs,Vol) */

    /* device status and option byte */  
    0x85, 0x02,     /* REPORT_ID (0x02)               */
    0x09, 0x02,     /* USAGE (Status and option byte) */
    0x15, 0x00,     /* LOGICAL_MINIMUM (0)            */
    0x25, 0xff,     /* LOGICAL_MAXIMUM (255)          */
    0x75, 0x08,     /* REPORT_SIZE (8)                */
    0x95, REPORT_IN_COUNT,     /* REPORT_COUNT (23)         */
    0x81, 0x82,     /* INPUT (Data,Var,Abs,Vol)       */

    0xc0            /* END_COLLECTION            */
};

/*!
    \brief      send IAP report
    \param[in]  udev: pointer to USB device instance
    \param[in]  report: pointer to HID report
    \param[in]  len: data length
    \param[out] none
    \retval     USB device operation status
*/
uint8_t iap_report_send (usb_dev *udev, uint8_t *report, uint32_t len)
{
    usbd_ep_send (udev, IAP_IN_EP, report, len);

    return USBD_OK;
}

/*!
    \brief      initialize the IAP device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t iap_init (usb_dev *udev, uint8_t config_index)
{
    static __ALIGN_BEGIN usbd_iap_handler iap_handler __ALIGN_END;

    /* initialize TX endpoint */
    usbd_ep_setup(udev, &(iap_config_desc.hid_epin));

    /* initialize RX endpoint */
    usbd_ep_setup(udev, &(iap_config_desc.hid_epout));

    /* unlock the internal flash */
    fmc_unlock();

    memset((void *)&iap_handler, 0U, sizeof(usbd_iap_handler));

    /* prepare receive data */
    usbd_ep_recev(udev, IAP_OUT_EP, iap_handler.report_buf, IAP_OUT_PACKET);

    iap_handler.base_address = APP_LOADED_ADDR;

    udev->dev.class_data[USBD_IAP_INTERFACE] = (void *)&iap_handler;

    return USBD_OK;
}

/*!
    \brief      deinitialize the IAP device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t iap_deinit (usb_dev *udev, uint8_t config_index)
{
    /* deinitialize IAP endpoints */
    usbd_ep_clear (udev, IAP_IN_EP);
    usbd_ep_clear (udev, IAP_OUT_EP);

    /* lock the internal flash */
    fmc_lock();

    return USBD_OK;
}

/*!
    \brief      handle the IAP class-specific requests
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: device class-specific request
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t iap_req_handler (usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->dev.transc_in[0];

    usbd_iap_handler *iap = (usbd_iap_handler *)udev->dev.class_data[USBD_IAP_INTERFACE];

    switch (req->bRequest) {
    case GET_REPORT:
        /* no use for this driver */
        break;

    case GET_IDLE:
        transc->xfer_buf = (uint8_t *)&iap->idlestate;
        transc->remain_len = 1U;
        break;

    case GET_PROTOCOL:
        transc->xfer_buf = (uint8_t *)&iap->protocol;
        transc->remain_len = 1U;
        break;

    case SET_REPORT:
        iap->reportID = (uint8_t)(req->wValue);
        break;

    case SET_IDLE:
        iap->idlestate = (uint8_t)(req->wValue >> 8U);
        break;

    case SET_PROTOCOL:
        iap->protocol = (uint8_t)(req->wValue);
        break;

    case USB_GET_DESCRIPTOR:
        if (USB_DESCTYPE_REPORT == (req->wValue >> 8U)) {
            transc->remain_len = USB_MIN(USB_DESC_LEN_IAP_REPORT, req->wLength);
            transc->xfer_buf = (uint8_t *)iap_report_desc;
        }
        break;

    default:
        return USBD_FAIL;
    }

    return USBD_OK;
}

/*!
    \brief      handle data out stage
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint identifier
    \param[out] none
    \retval     none
*/
static uint8_t iap_data_out (usb_dev *udev ,uint8_t ep_num)
{
    usbd_iap_handler *iap = (usbd_iap_handler *)udev->dev.class_data[USBD_IAP_INTERFACE];

    if ((IAP_OUT_EP & 0x7FU) == ep_num) {
        if (0x01U == iap->report_buf[0]) {
            switch (iap->report_buf[1]) {
            case IAP_DNLOAD:
                iap_req_dnload(udev);
                break;

            case IAP_ERASE:
                iap_req_erase(udev);
                break;

            case IAP_OPTION_BYTE1:
                iap_req_optionbyte(udev, 0x01U);
                break;

            case IAP_LEAVE:
                iap_req_leave(udev);
                break;

            case IAP_GETBIN_ADDRESS:
                iap_address_send(udev);
                break;

            case IAP_OPTION_BYTE2:
                iap_req_optionbyte(udev, 0x02U);
                break;

            default:
                break;
            }
        }

        usbd_ep_recev(udev, IAP_OUT_EP, iap->report_buf, IAP_OUT_PACKET);
    }

    return USBD_OK;
}

/*!
    \brief      handle the IAP_DNLOAD request
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void iap_req_dnload(usb_dev *udev)
{
    usbd_iap_handler *iap = (usbd_iap_handler *)udev->dev.class_data[USBD_IAP_INTERFACE];

    if (0U != iap->transfer_times) {
        if (1U == iap->transfer_times) {
            if (0U == iap->lps) {
                iap_data_write(&iap->report_buf[2], iap->base_address, TRANSFER_SIZE);
            } else {
                iap_data_write(&iap->report_buf[2], iap->base_address, iap->file_length % TRANSFER_SIZE);
                iap->lps = 0U;
            }

            iap->dev_status[0] = 0x02U;
            iap->dev_status[1] = 0x02U;
            iap_report_send (udev, iap->dev_status, IAP_IN_PACKET);
        } else {
            iap_data_write(&iap->report_buf[2], iap->base_address, TRANSFER_SIZE);

            iap->base_address += TRANSFER_SIZE;
        }

        iap->transfer_times--;
    }
}

/*!
    \brief      handle the IAP_ERASE request
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void iap_req_erase(usb_dev *udev)
{
    uint32_t addr = 0U;

    usbd_iap_handler *iap = (usbd_iap_handler *)udev->dev.class_data[USBD_IAP_INTERFACE];

    /* get base address to erase */
    iap->base_address  = iap->report_buf[2];
    iap->base_address |= iap->report_buf[3] << 8U;
    iap->base_address |= iap->report_buf[4] << 16U;
    iap->base_address |= iap->report_buf[5] << 24U;

    /* get file length */
    iap->file_length = iap->report_buf[7];
    iap->file_length |= iap->report_buf[8] << 8U;
    iap->file_length |= iap->report_buf[9] << 16U;
    iap->file_length |= iap->report_buf[10] << 24U;

    iap->lps = iap->file_length % TRANSFER_SIZE;
    if (0U == iap->lps) {
        iap->transfer_times = iap->file_length / TRANSFER_SIZE;
    } else {
        iap->transfer_times = iap->file_length / TRANSFER_SIZE + 1U;
    }

    /* check if the address is in protected area */
    if (IS_PROTECTED_AREA(iap->base_address)) {
        return;
    }

    addr = iap->base_address;

    /* unlock the flash program erase controller */
    fmc_unlock();

    flash_erase(addr, iap->file_length, iap->report_buf);
    
    fmc_lock();

    iap->dev_status[0] = 0x02U;
    iap->dev_status[1] = 0x01U;

    usbd_ep_send(udev, IAP_IN_EP, iap->dev_status, IAP_IN_PACKET);
}

/*!
    \brief      handle the IAP_OPTION_BYTE request
    \param[in]  udev: pointer to USB device instance
    \param[in]  option_num: number of option byte
    \param[out] none
    \retval     none
*/
static void iap_req_optionbyte(usb_dev *udev, uint8_t option_num)
{
    uint8_t i = 0U;
    uint32_t address = 0U;

    usbd_iap_handler *iap = (usbd_iap_handler *)udev->dev.class_data[USBD_IAP_INTERFACE];

    iap->option_byte[0]= 0x02U;

    if (0x01U == option_num) {
        address = OPT_BYTE_ADDR1;
#ifdef OPT_BYTE_ADDR2
    } else if (0x02U == option_num) {
        address = OPT_BYTE_ADDR2;
#endif
    } else {
        return;
    }

    for (i = 1U; i < 17U; i++) {
        iap->option_byte[i] = *(uint8_t *)address;
        address++;
    }

    iap_report_send (udev, iap->option_byte, IAP_IN_PACKET);
}

/*!
    \brief      handle the IAP_LEAVE request
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void iap_req_leave(usb_dev *udev)
{
    /* lock the internal flash */
    fmc_lock();

    /* generate system reset to allow jumping to the user code */
    NVIC_SystemReset();
}

/*!
    \brief      handle the IAP_SEND_ADDRESS request
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void iap_address_send(usb_dev *udev)
{
    usbd_iap_handler *iap = (usbd_iap_handler *)udev->dev.class_data[USBD_IAP_INTERFACE];

    iap->bin_addr[0] = 0x02U;

    iap->bin_addr[1] = (uint8_t)(APP_LOADED_ADDR);
    iap->bin_addr[2] = (uint8_t)(APP_LOADED_ADDR >> 8U);
    iap->bin_addr[3] = (uint8_t)(APP_LOADED_ADDR >> 16U);
    iap->bin_addr[4] = (uint8_t)(APP_LOADED_ADDR >> 24U);

    iap_report_send (udev, iap->bin_addr, IAP_IN_PACKET);
}
