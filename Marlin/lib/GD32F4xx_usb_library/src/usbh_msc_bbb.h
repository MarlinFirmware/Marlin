/*!
    \file    usbh_msc_bbb.h
    \brief   header file for usbh_msc_bbb.c

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

#ifndef __USBH_MSC_BBB_H
#define __USBH_MSC_BBB_H

#include "gdusbh_enum.h"
#include "msc_bbb.h"

typedef union {
    msc_bbb_cbw field;

    uint8_t CBWArray[31];
}usbh_cbw_pkt;

typedef union {
    msc_bbb_csw field;

    uint8_t CSWArray[13];
}usbh_csw_pkt;

enum usbh_msc_state {
    USBH_MSC_BOT_INIT_STATE = 0U,
    USBH_MSC_BOT_RESET,
    USBH_MSC_GET_MAX_LUN,
    USBH_MSC_TEST_UNIT_READY,
    USBH_MSC_READ_CAPACITY10,
    USBH_MSC_MODE_SENSE6,
    USBH_MSC_REQUEST_SENSE,
    USBH_MSC_BOT_USB_TRANSFERS,
    USBH_MSC_DEFAULT_APPLI_STATE,
    USBH_MSC_CTRL_ERROR_STATE,
    USBH_MSC_UNRECOVERED_STATE
};

typedef enum
{
    BOT_OK = 0U,
    BOT_FAIL,
    BOT_PHASE_ERROR,
    BOT_BUSY
} bot_status;

typedef enum
{
    BOT_CMD_IDLE = 0U,
    BOT_CMD_SEND,
    BOT_CMD_WAIT,
} bot_cmd_state;

/* csw status definitions */
typedef enum
{
    BOT_CSW_CMD_PASSED = 0U,
    BOT_CSW_CMD_FAILED,
    BOT_CSW_PHASE_ERROR,
} bot_csw_status;

typedef enum
{
    BOT_SEND_CBW = 1U,
    BOT_SEND_CBW_WAIT,
    BOT_DATA_IN,
    BOT_DATA_IN_WAIT,
    BOT_DATA_OUT,
    BOT_DATA_OUT_WAIT,
    BOT_RECEIVE_CSW,
    BOT_RECEIVE_CSW_WAIT,
    BOT_ERROR_IN,
    BOT_ERROR_OUT,
    BOT_UNRECOVERED_ERROR
} bot_state;

typedef struct
{
    uint8_t                *pbuf;
    uint32_t                data[16];
    bot_state               state;
    bot_state               prev_state;
    bot_cmd_state           cmd_state;
    usbh_cbw_pkt            cbw;
    usbh_csw_pkt            csw;
} bot_handle;

#define USBH_MSC_BOT_CBW_TAG                0x20304050U

#define USBH_MSC_CSW_MAX_LENGTH             63U

#define USBH_MSC_SEND_CSW_DISABLE           0U
#define USBH_MSC_SEND_CSW_ENABLE            1U

#define USBH_MSC_DIR_IN                     0U
#define USBH_MSC_DIR_OUT                    1U
#define USBH_MSC_BOTH_DIR                   2U

#define USBH_MSC_PAGE_LENGTH                512U

#define CBW_CB_LENGTH                       16U
#define CBW_LENGTH                          10U
#define CBW_LENGTH_TEST_UNIT_READY          0U

#define MAX_BULK_STALL_COUNT_LIMIT          0x04U   /*!< If STALL is seen on Bulk 
                                                      Endpoint continously, this means 
                                                      that device and Host has phase error
                                                      Hence a Reset is needed */

/* function declarations */
/* initialize the mass storage parameters */
void usbh_msc_bot_init (usbh_host *uhost);
/* manage the different states of BOT transfer and updates the status to upper layer */
usbh_status usbh_msc_bot_process (usbh_host *uhost, uint8_t lun);
/* manages the different error handling for stall */
usbh_status usbh_msc_bot_abort (usbh_host *uhost, uint8_t direction);
/* reset MSC bot request structure */
usbh_status usbh_msc_bot_reset (usbh_host *uhost);
/* decode the CSW received by the device and updates the same to upper layer */
bot_csw_status usbh_msc_csw_decode (usbh_host *uhost);

#endif /* __USBH_MSC_BBB_H */
