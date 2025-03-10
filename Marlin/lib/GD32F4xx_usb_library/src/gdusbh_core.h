/*!
    \file    usbh_core.h
    \brief   USB host core state machine header file

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

#ifndef __USBH_CORE_H
#define __USBH_CORE_H

#include "gdusbh_conf.h"
#include "drv_usb_host.h"

#define MSC_CLASS                                       0x08U
#define HID_CLASS                                       0x03U
#define MSC_PROTOCOL                                    0x50U
#define CBI_PROTOCOL                                    0x01U

#define USBH_MAX_ERROR_COUNT                            3U

#define USBH_DEV_ADDR_DEFAULT                           0U
#define USBH_DEV_ADDR                                   1U

typedef enum
{
    USBH_OK = 0U,
    USBH_BUSY,
    USBH_FAIL,
    USBH_NOT_SUPPORTED,
    USBH_UNRECOVERED_ERROR,
    USBH_SPEED_UNKNOWN_ERROR,
    USBH_APPLY_DEINIT
} usbh_status;

/* USB host global operation state */
typedef enum
{
    HOST_DEFAULT = 0U,
    HOST_DETECT_DEV_SPEED,
    HOST_DEV_ATTACHED,
    HOST_DEV_DETACHED,
    HOST_ENUM,
    HOST_SET_WAKEUP_FEATURE,
    HOST_CHECK_CLASS,
    HOST_CLASS_ENUM,
    HOST_CLASS_HANDLER,
    HOST_USER_INPUT,
    HOST_SUSPENDED,
    HOST_WAKEUP,
    HOST_ERROR
} usb_host_state;

/* USB host enumeration state */
typedef enum
{
    ENUM_DEFAULT = 0U,
    ENUM_GET_DEV_DESC,
    ENUM_SET_ADDR,
    ENUM_GET_CFG_DESC,
    ENUM_GET_CFG_DESC_SET,
    ENUM_GET_STR_DESC,
#ifdef USB_MTP
    ENUM_GET_MTP_STR,
#endif
    ENUM_SET_CONFIGURATION,
    ENUM_DEV_CONFIGURED
} usbh_enum_state;

/* USB host control transfer state */
typedef enum 
{
    CTL_IDLE = 0U,
    CTL_SETUP,
    CTL_SETUP_WAIT,
    CTL_DATA_IN,
    CTL_DATA_IN_WAIT,
    CTL_DATA_OUT,
    CTL_DATA_OUT_WAIT,
    CTL_STATUS_IN,
    CTL_STATUS_IN_WAIT,
    CTL_STATUS_OUT,
    CTL_STATUS_OUT_WAIT,
    CTL_ERROR,
    CTL_FINISH
} usbh_ctl_state;

/* user action state */
typedef enum
{
    USBH_USER_NO_RESP = 0U,
    USBH_USER_RESP_OK = 1U,
} usbh_user_status;

typedef enum
{
    USBH_PORT_EVENT = 1U,
    USBH_URB_EVENT,
    USBH_CONTROL_EVENT,
    USBH_CLASS_EVENT,
    USBH_STATE_CHANGED_EVENT,
}usbh_os_event;

/* control transfer information */
typedef struct _usbh_control
{
    uint8_t               pipe_in_num;
    uint8_t               pipe_out_num;
    uint8_t               max_len;
    uint8_t               error_count;

    uint8_t               *buf;
    uint16_t              ctl_len;
    uint16_t              timer;

    usb_setup             setup;
    usbh_ctl_state        ctl_state;
} usbh_control;

/* USB interface descriptor set */
typedef struct _usb_desc_itf_set
{
    usb_desc_itf itf_desc;
    usb_desc_ep  ep_desc[USBH_MAX_EP_NUM];
} usb_desc_itf_set;

/* USB configure descriptor set */
typedef struct _usb_desc_cfg_set
{
    usb_desc_config   cfg_desc;
    usb_desc_itf_set  itf_desc_set[USBH_MAX_INTERFACES_NUM][USBH_MAX_ALT_SETTING];
} usb_desc_cfg_set;

/* USB device property */
typedef struct
{
    uint8_t                   data[USBH_DATA_BUF_MAX_LEN]; /* if DMA is used, the data array must be located in the first position */
    uint8_t                   cur_itf;
    uint8_t                   addr;

    uint32_t                  speed;

    usb_desc_dev              dev_desc;
    usb_desc_cfg_set          cfg_desc_set;

#if (USBH_KEEP_CFG_DESCRIPTOR == 1U)
    uint8_t                   cfgdesc_rawdata[USBH_CFGSET_MAX_LEN];
#endif /* (USBH_KEEP_CFG_DESCRIPTOR == 1U) */
} usb_dev_prop;

struct _usbh_host;

/* device class callbacks */
typedef struct
{
    uint8_t       class_code;       /*!< USB class type */

    usbh_status (*class_init)      (struct _usbh_host *phost);
    void        (*class_deinit)    (struct _usbh_host *phost);
    usbh_status (*class_requests)  (struct _usbh_host *phost);
    usbh_status (*class_machine)   (struct _usbh_host *phost);
    usbh_status (*class_sof)       (struct _usbh_host *uhost);

    void         *class_data;
} usbh_class;

/* user callbacks */
typedef struct
{
    void (*dev_init)                    (void);
    void (*dev_deinit)                  (void);
    void (*dev_attach)                  (void);
    void (*dev_reset)                   (void);
    void (*dev_detach)                  (void);
    void (*dev_over_currented)          (void);
    void (*dev_speed_detected)          (uint32_t dev_speed);
    void (*dev_devdesc_assigned)        (void *dev_desc);
    void (*dev_address_set)             (void);

    void (*dev_cfgdesc_assigned)        (usb_desc_config *cfg_desc, 
                                         usb_desc_itf *itf_desc,
                                         usb_desc_ep *ep_desc);

    void (*dev_mfc_str)                 (void *mfc_str);
    void (*dev_prod_str)                (void *prod_str);
    void (*dev_seral_str)               (void *serial_str);
    void (*dev_enumerated)              (void);
    usbh_user_status (*dev_user_input)  (void);
    int  (*dev_user_app)                (void);
    void (*dev_not_supported)           (void);
    void (*dev_error)                   (void);
} usbh_user_cb;

/* host information */
typedef struct _usbh_host
{
    usb_host_state                      cur_state;                          /*!< host state machine value */
    usb_host_state                      backup_state;                       /*!< backup of previous state machine value */
    usbh_enum_state                     enum_state;                         /*!< enumeration state machine */
    usbh_control                        control;                            /*!< USB host control state machine */
    usb_dev_prop                        dev_prop;                           /*!< USB device property */

    usbh_class                          *uclass[USBH_MAX_SUPPORTED_CLASS];  /*!< USB host supported class */
    usbh_class                          *active_class;                      /*!< USB active class */
    usbh_user_cb                        *usr_cb;                            /*!< USB user callback */

    uint8_t                             class_num;                          /*!< USB class number */

    void                                *data;                              /*!< used for... */

#ifdef USB_LOW_PWR_ENABLE
    uint8_t                             suspend_flag;  
#endif /* USB_LOW_PWR_ENABLE*/
} usbh_host;

/*!
    \brief      get USB URB state
    \param[in]  udev: pointer to USB core instance
    \param[in]  pp_num: pipe number
    \param[out] none
    \retval     none
*/
static inline usb_urb_state usbh_urbstate_get (usb_core_driver *udev, uint8_t pp_num)
{
    return udev->host.pipe[pp_num].urb_state;
}

/*!
    \brief      get USB transfer data count
    \param[in]  udev: pointer to USB core instance
    \param[in]  pp_num: pipe number
    \param[out] none
    \retval     none
*/
static inline uint32_t usbh_xfercount_get (usb_core_driver *udev, uint8_t pp_num)
{
    return udev->host.backup_xfercount[pp_num];
}

/* function declarations */
/* USB host stack initializations */
void usbh_init (usbh_host *uhost, usb_core_driver *udev, usb_core_enum usb_core, usbh_user_cb *user_cb);
/* USB host register device class */
usbh_status usbh_class_register (usbh_host *uhost, usbh_class *puclass);
/* de-initialize USB host */
usbh_status usbh_deinit (usbh_host *uhost);
/* USB host core main state machine process */
void usbh_core_task (usbh_host *uhost);
/* handle the error on USB host side */
void usbh_error_handler (usbh_host *uhost, usbh_status err_type);
#ifdef USB_LOW_PWR_ENABLE
/* handles the USB resume from suspend mode */
void usb_hwp_resume(usb_core_driver *udev);
#endif /* USB_LOW_PWR_ENABLE */

// extern uint8_t udiskMounted;

#endif /* __USBH_CORE_H */
