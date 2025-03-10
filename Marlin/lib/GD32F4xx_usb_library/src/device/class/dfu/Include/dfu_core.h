/*!
    \file    dfu_core.h
    \brief   the header file of USB DFU device class core functions

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

#ifndef __DFU_CORE_H
#define __DFU_CORE_H

#include "usbd_enum.h"

/* DFU class code */
#define USB_DFU_CLASS                 0xFEU

/* DFU subclass code */
#define USB_DFU_SUBCLASS_UPGRADE      0x01U

/* DFU protocol code */
#define USB_DFU_PROTOCL_RUNTIME       0x01U
#define USB_DFU_PROTOCL_DFU           0x02U

/* manifestation state */
#define MANIFEST_COMPLETE             0x00U
#define MANIFEST_IN_PROGRESS          0x01U

/* DFU attributes code */
#define USB_DFU_CAN_DOWNLOAD          0x01U
#define USB_DFU_CAN_UPLOAD            0x02U
#define USB_DFU_MANIFEST_TOLERANT     0x04U
#define USB_DFU_WILL_DETACH           0x08U

/* special commands with download request */
#define GET_COMMANDS                  0x00U
#define SET_ADDRESS_POINTER           0x21U
#define ERASE                         0x41U

/* memory operation command */
#define CMD_ERASE                     0U
#define CMD_WRITE                     1U

#define _BYTE1(x)                     (uint8_t)((x) & 0xFFU)                /*!< addressing cycle 1st byte */
#define _BYTE2(x)                     (uint8_t)(((x) & 0xFF00U) >> 8U)      /*!< addressing cycle 2nd byte */
#define _BYTE3(x)                     (uint8_t)(((x) & 0xFF0000U) >> 16U)   /*!< addressing cycle 3rd byte */

#define FLASH_ERASE_TIMEOUT           60U
#define FLASH_WRITE_TIMEOUT           80U

/* bit detach capable = bit 3 in bmAttributes field */
#define DFU_DETACH_MASK               (uint8_t)(0x10U)

#define USB_SERIAL_STR_LEN            0x06U

#define USB_DFU_CONFIG_DESC_SIZE      27U

#define DFU_DESC_TYPE                 0x21U

/* DFU device state defines */
typedef enum {
    STATE_APP_IDLE = 0x00U,
    STATE_APP_DETACH,
    STATE_DFU_IDLE,
    STATE_DFU_DNLOAD_SYNC,
    STATE_DFU_DNBUSY,
    STATE_DFU_DNLOAD_IDLE,
    STATE_DFU_MANIFEST_SYNC,
    STATE_DFU_MANIFEST,
    STATE_DFU_MANIFEST_WAIT_RESET,
    STATE_DFU_UPLOAD_IDLE,
    STATE_DFU_ERROR
} dfu_state;

/* DFU device status defines */
typedef enum {
    STATUS_OK = 0x00U,
    STATUS_ERR_TARGET,
    STATUS_ERR_FILE,
    STATUS_ERR_WRITE,
    STATUS_ERR_ERASE,
    STATUS_ERR_CHECK_ERASED,
    STATUS_ERR_PROG,
    STATUS_ERR_VERIFY,
    STATUS_ERR_ADDRESS,
    STATUS_ERR_NOTDONE,
    STATUS_ERR_FIRMWARE,
    STATUS_ERR_VENDOR,
    STATUS_ERR_USBR,
    STATUS_ERR_POR,
    STATUS_ERR_UNKNOWN,
    STATUS_ERR_STALLEDPKT
} dfu_status;

/* DFU class-specific requests */
typedef enum {
    DFU_DETACH = 0U,
    DFU_DNLOAD,
    DFU_UPLOAD,
    DFU_GETSTATUS,
    DFU_CLRSTATUS,
    DFU_GETSTATE,
    DFU_ABORT,
    DFU_REQ_MAX
} dfu_requests;

#pragma pack(1)

/* USB DFU function descriptor structure */
typedef struct
{
    usb_desc_header header;               /*!< descriptor header, including type and size */
    uint8_t bmAttributes;                 /*!< DFU attributes */
    uint16_t wDetachTimeOut;              /*!< time, in milliseconds, that the device will wait after receipt of the DFU_DETACH request. If */ 
    uint16_t wTransferSize;               /*!< maximum number of bytes that the device can accept per control-write transaction */
    uint16_t bcdDFUVersion;               /*!< numeric expression identifying the version of the DFU Specification release. */
} usb_desc_dfu_func;

#pragma pack()

/* USB configuration descriptor structure */
typedef struct
{
    usb_desc_config           config;
    usb_desc_itf              dfu_itf;
    usb_desc_dfu_func         dfu_func;
} usb_dfu_desc_config_set;

typedef struct
{
    uint8_t bStatus;
    uint8_t bwPollTimeout0;
    uint8_t bwPollTimeout1;
    uint8_t bwPollTimeout2;
    uint8_t bState;
    uint8_t iString;

    uint8_t manifest_state;
    uint32_t data_len;
    uint16_t block_num;
    uint32_t base_addr;

    uint8_t buf[TRANSFER_SIZE];
} usbd_dfu_handler;

typedef void (*app_func) (void);

extern usb_desc dfu_desc;
extern usb_class_core dfu_class;

#endif  /* DFU_CORE_H */
