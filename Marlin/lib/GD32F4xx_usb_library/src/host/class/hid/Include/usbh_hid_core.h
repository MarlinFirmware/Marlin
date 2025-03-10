/*!
    \file    usbh_hid_core.h
    \brief   header file for the usbh_hid_core.c

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

#ifndef __USBH_HID_CORE_H
#define __USBH_HID_CORE_H

#include "usb_hid.h"
#include "gdusbh_enum.h"
#include "usbh_transc.h"

#define HID_MIN_POLL                                    10U
#define HID_REPORT_SIZE                                 16U
#define HID_MAX_USAGE                                   10U
#define HID_MAX_NBR_REPORT_FMT                          10U
#define HID_QUEUE_SIZE                                  10U

#define HID_ITEM_LONG                                   0xFEU

#define HID_ITEM_TYPE_MAIN                              0x00U
#define HID_ITEM_TYPE_GLOBAL                            0x01U
#define HID_ITEM_TYPE_LOCAL                             0x02U
#define HID_ITEM_TYPE_RESERVED                          0x03U

#define HID_MAIN_ITEM_TAG_INPUT                         0x08U
#define HID_MAIN_ITEM_TAG_OUTPUT                        0x09U
#define HID_MAIN_ITEM_TAG_COLLECTION                    0x0AU
#define HID_MAIN_ITEM_TAG_FEATURE                       0x0BU
#define HID_MAIN_ITEM_TAG_ENDCOLLECTION                 0x0CU

#define HID_GLOBAL_ITEM_TAG_USAGE_PAGE                  0x00U
#define HID_GLOBAL_ITEM_TAG_LOG_MIN                     0x01U
#define HID_GLOBAL_ITEM_TAG_LOG_MAX                     0x02U
#define HID_GLOBAL_ITEM_TAG_PHY_MIN                     0x03U
#define HID_GLOBAL_ITEM_TAG_PHY_MAX                     0x04U
#define HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT               0x05U
#define HID_GLOBAL_ITEM_TAG_UNIT                        0x06U
#define HID_GLOBAL_ITEM_TAG_REPORT_SIZE                 0x07U
#define HID_GLOBAL_ITEM_TAG_REPORT_ID                   0x08U
#define HID_GLOBAL_ITEM_TAG_REPORT_COUNT                0x09U
#define HID_GLOBAL_ITEM_TAG_PUSH                        0x0AU
#define HID_GLOBAL_ITEM_TAG_POP                         0x0BU

#define HID_LOCAL_ITEM_TAG_USAGE                        0x00U
#define HID_LOCAL_ITEM_TAG_USAGE_MIN                    0x01U
#define HID_LOCAL_ITEM_TAG_USAGE_MAX                    0x02U
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_INDEX             0x03U
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_MIN               0x04U
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_MAX               0x05U
#define HID_LOCAL_ITEM_TAG_STRING_INDEX                 0x07U
#define HID_LOCAL_ITEM_TAG_STRING_MIN                   0x08U
#define HID_LOCAL_ITEM_TAG_STRING_MAX                   0x09U
#define HID_LOCAL_ITEM_TAG_DELIMITER                    0x0AU

#define USB_HID_DESC_SIZE                               9U

/* states for HID state machine */
typedef enum {
    HID_INIT = 0U,
    HID_IDLE,
    HID_SEND_DATA,
    HID_BUSY,
    HID_GET_DATA,
    HID_SYNC,
    HID_POLL,
    HID_ERROR,
} hid_state;

typedef enum {
    HID_REQ_INIT = 0U,
    HID_REQ_IDLE,
    HID_REQ_GET_REPORT_DESC,
    HID_REQ_GET_HID_DESC,
    HID_REQ_SET_IDLE,
    HID_REQ_SET_PROTOCOL,
    HID_REQ_SET_REPORT,
} hid_ctlstate;

typedef enum
{
    HID_MOUSE    = 0x01U,
    HID_KEYBOARD = 0x02U,
    HID_UNKNOWN  = 0xFFU,
} hid_type;

typedef struct _hid_report_data
{
    uint8_t  ReportID;
    uint8_t  ReportType;
    uint16_t UsagePage;
    uint32_t Usage[HID_MAX_USAGE];
    uint32_t NbrUsage;
    uint32_t UsageMin;
    uint32_t UsageMax;
    int32_t  LogMin;
    int32_t  LogMax;
    int32_t  PhyMin;
    int32_t  PhyMax;
    int32_t  UnitExp;
    uint32_t Unit;
    uint32_t ReportSize;
    uint32_t ReportCnt;
    uint32_t Flag;
    uint32_t PhyUsage;
    uint32_t AppUsage;
    uint32_t LogUsage;
} hid_report_data;

typedef struct _hid_report_ID
{
    uint8_t  size;         /*!< report size return by the device ID */
    uint8_t  reportID;     /*!< report ID */
    uint8_t  type;         /*!< report type (INPUT/OUTPUT/FEATURE) */
} hid_report_ID;

typedef struct  _hid_collection
{
    uint32_t                usage;
    uint8_t                 type;
    struct _hid_collection  *next_ptr;
} hid_collection;

typedef struct _hid_appcollection
{
    uint32_t               usage;
    uint8_t                type;
    uint8_t                nbr_report_fmt;
    hid_report_data        report_data[HID_MAX_NBR_REPORT_FMT];
} hid_appcollection;

typedef struct
{
     uint8_t  *buf;
     uint16_t  head;
     uint16_t  tail;
     uint16_t  size;
     uint8_t   lock;
} data_fifo;

/* structure for HID process */
typedef struct _hid_process
{
    uint8_t              pipe_in;
    uint8_t              pipe_out;
    uint8_t              ep_addr;
    uint8_t              ep_in;
    uint8_t              ep_out;
    __IO uint8_t         data_ready;
    uint8_t              *pdata;
    uint16_t             len;
    uint16_t             poll;

    __IO uint32_t        timer;

    data_fifo            fifo;
    usb_desc_hid         hid_desc;
    hid_report_data      hid_report;

    hid_state            state; 
    hid_ctlstate         ctl_state;
    usbh_status          (*init)(usb_core_driver *udev, usbh_host *uhost);
    void                 (*machine)(usb_core_driver *udev, usbh_host *uhost);
} usbh_hid_handler;

extern usbh_class usbh_hid;

/* function declarations */
/* set HID report */
usbh_status usbh_set_report (usb_core_driver *udev,
                             usbh_host *uhost,
                             uint8_t  report_type,
                             uint8_t  report_ID,
                             uint8_t  report_len,
                             uint8_t *report_buf);
/* read data from FIFO */
uint16_t usbh_hid_fifo_read (data_fifo *fifo, void *buf, uint16_t nbytes);
/* write data to FIFO */
uint16_t usbh_hid_fifo_write (data_fifo *fifo, void *buf, uint16_t nbytes);
/* initialize FIFO */
void usbh_hid_fifo_init (data_fifo *fifo, uint8_t *buf, uint16_t size);

#endif /* __USBH_HID_CORE_H */
