/*!
    \file    usbh_core.c
    \brief   USB host core state machine driver

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

#include "drv_usb_hw.h"
#include "gdusbh_pipe.h"
#include "gdusbh_enum.h"
#include "gdusbh_core.h"
#include "drv_usbh_int.h"
#include <string.h>

// #include "../../../src/feature/powerloss.h"

/* local function prototypes ('static') */
static uint8_t usbh_sof           (usbh_host *uhost);
static uint8_t usbh_connect       (usbh_host *uhost);
static uint8_t usbh_disconnect    (usbh_host *uhost);
static uint8_t usbh_port_enabled  (usbh_host *uhost);
static uint8_t usbh_port_disabled (usbh_host *uhost);
static usbh_status usbh_enum_task (usbh_host *uhost);

#ifdef USB_LOW_PWR_ENABLE
static void usb_hwp_suspend(usb_core_driver *udev);
#endif

// extern uint8_t udiskMounted;
// extern uint16_t plr_num;
extern bool plr_flag;

usbh_int_cb usbh_int_op = 
{
    usbh_connect,
    usbh_disconnect,
    usbh_port_enabled,
    usbh_port_disabled,
    usbh_sof
};

usbh_int_cb *usbh_int_fop = &usbh_int_op;

/*!
    \brief      USB host stack initializations
    \param[in]  uhost: pointer to USB host
    \param[in]  user_cb: pointer to user callback
    \param[out] none
    \retval     none
*/
void usbh_init (usbh_host *uhost, usb_core_driver *udev, usb_core_enum usb_core, usbh_user_cb *user_cb)
{
    /* host deinitialization */
    usbh_deinit(uhost);

    uhost->usr_cb = user_cb;

    udev->host.connect_status = 0U;

    for (uint8_t i = 0U; i < USBFS_MAX_TX_FIFOS; i++) {
        udev->host.pipe[i].err_count = 0U;
        udev->host.pipe[i].pp_status = PIPE_IDLE;
        udev->host.backup_xfercount[i] = 0U;
    }

    udev->host.pipe[0].ep.mps = 8U;

    usb_basic_init (&udev->bp, &udev->regs, usb_core);

#ifndef DUAL_ROLE_MODE_ENABLED
    usb_globalint_disable(&udev->regs);

    usb_core_init (udev->bp, &udev->regs);

#ifndef USE_OTG_MODE
    usb_curmode_set (&udev->regs, HOST_MODE);
#endif /* USE_OTG_MODE */

    usb_host_init (udev);

    usb_globalint_enable(&udev->regs);
#endif /* DUAL_ROLE_MODE_ENABLED */

    /* link driver to the stack */
    udev->host.data = (void *)uhost;
    uhost->data = (void *)udev;

    /* upon initialize call usr call back */
    uhost->usr_cb->dev_init();
}

/*!
    \brief      USB host register device class
    \param[in]  uhost: pointer to USB host instance
    \param[in]  puclass: pointer to USB device class
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_class_register (usbh_host *uhost, usbh_class *puclass)
{
    usbh_status status = USBH_OK;

    if (NULL != puclass) {
        if (uhost->class_num < USBH_MAX_SUPPORTED_CLASS) {
            uhost->uclass[uhost->class_num++] = puclass;
        } else {
            status = USBH_FAIL;
        }
    } else {
        status = USBH_FAIL;
    }

    return status;
}

/*!
    \brief      de-initialize USB host
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_deinit(usbh_host *uhost)
{
    usb_core_driver *udev = (usb_core_driver *)uhost->data;

    /* software initialize */
    uhost->cur_state = HOST_DEFAULT;
    uhost->backup_state = HOST_DEFAULT;
    uhost->enum_state = ENUM_DEFAULT;

    uhost->control.ctl_state = CTL_IDLE;
    uhost->control.max_len = USB_FS_EP0_MAX_LEN;

    uhost->dev_prop.addr = USBH_DEV_ADDR_DEFAULT;
    uhost->dev_prop.speed = PORT_SPEED_FULL;
    uhost->dev_prop.cur_itf = 0xFFU;

    usbh_pipe_free(udev, uhost->control.pipe_in_num);
    usbh_pipe_free(udev, uhost->control.pipe_out_num);

    return USBH_OK;
}

/*!
    \brief      USB host core main state machine process
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
void usbh_core_task (usbh_host *uhost)
{
    volatile usbh_status status = USBH_FAIL;
    usb_core_driver *udev = (usb_core_driver *)uhost->data;

    /* check for host port events */
    if (((0U == udev->host.connect_status) || (0U == udev->host.port_enabled)) && (HOST_DEFAULT != uhost->cur_state)) {
        if (uhost->cur_state != HOST_DEV_DETACHED) {
            uhost->cur_state = HOST_DEV_DETACHED;
        }
    }

    switch (uhost->cur_state) {
    case HOST_DEFAULT:
        if (udev->host.connect_status) {
            uhost->cur_state = HOST_DETECT_DEV_SPEED;

            usb_mdelay (100U);

            usb_port_reset (udev);

            uhost->usr_cb->dev_reset();
        }
        break;

    case HOST_DETECT_DEV_SPEED:
        if (udev->host.port_enabled) {
            uhost->cur_state = HOST_DEV_ATTACHED;

            uhost->dev_prop.speed = usb_curspeed_get (udev);

            uhost->usr_cb->dev_speed_detected(uhost->dev_prop.speed);

            usb_mdelay (50U);
        }
        break;

    case HOST_DEV_ATTACHED:
        uhost->usr_cb->dev_attach();
        uhost->control.pipe_out_num = usbh_pipe_allocate(udev, 0x00U);
        uhost->control.pipe_in_num = usbh_pipe_allocate(udev, 0x80U);

        /* open IN control pipe */
        usbh_pipe_create (udev,
                          &uhost->dev_prop,
                          uhost->control.pipe_in_num,
                          USB_EPTYPE_CTRL,
                          (uint16_t)uhost->control.max_len);

        /* open OUT control pipe */
        usbh_pipe_create (udev,
                          &uhost->dev_prop,
                          uhost->control.pipe_out_num,
                          USB_EPTYPE_CTRL,
                          (uint16_t)uhost->control.max_len);

        uhost->cur_state = HOST_ENUM;
        break;

    case HOST_ENUM:
        /* check for enumeration status */
        if (USBH_OK == usbh_enum_task (uhost)) {
            /* the function shall return USBH_OK when full enumeration is complete */

            /* user callback for end of device basic enumeration */
            uhost->usr_cb->dev_enumerated();

#ifdef USB_LOW_PWR_ENABLE
            uhost->cur_state = HOST_SUSPENDED;
#else
            uhost->cur_state = HOST_SET_WAKEUP_FEATURE;
#endif
        }
        break;

    case HOST_SET_WAKEUP_FEATURE:
        if ((uhost->dev_prop.cfg_desc_set.cfg_desc.bmAttributes) & (1U << 5)) {
            if (usbh_setdevfeature(uhost, FEATURE_SELECTOR_REMOTEWAKEUP, 0U) == USBH_OK) {
                uhost->cur_state = HOST_CHECK_CLASS;
            }
        } else {
            uhost->cur_state = HOST_CHECK_CLASS;
        }
        break;

    case HOST_CHECK_CLASS:
        if (0U == uhost->class_num) {
            uhost->cur_state = HOST_ERROR;
        } else {
            uhost->active_class = NULL;

            uint8_t itf_class = uhost->dev_prop.cfg_desc_set.itf_desc_set[0][0].itf_desc.bInterfaceClass;

            for (uint8_t index = 0U; index < uhost->class_num; index++) {
                if ((uhost->uclass[index]->class_code == itf_class) || (0xFFU == itf_class)) {
                    uhost->active_class = uhost->uclass[index];
                }
            }

            if (uhost->active_class != NULL) {
                uhost->cur_state = HOST_USER_INPUT;
            } else {
                uhost->cur_state = HOST_ERROR;
            }
        }
        break;

    case HOST_USER_INPUT:
        /* the function should return user response true to move to class state */
        if (USBH_USER_RESP_OK == uhost->usr_cb->dev_user_input()) {
            if ((USBH_OK == uhost->active_class->class_init(uhost))) {
                uhost->cur_state = HOST_CLASS_ENUM;
            }
        }
        break;

#ifdef USB_LOW_PWR_ENABLE
    case HOST_SUSPENDED:
        if (USBH_OK == usbh_setdevfeature(uhost, FEATURE_SELECTOR_DEV, 0U)) {
            uhost->suspend_flag = 1;
            usb_hwp_suspend(uhost->data);
            uhost->usr_cb->dev_user_input();
            pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, WFI_CMD);
            uhost->cur_state = HOST_WAKEUP;
        }
        break;

    case HOST_WAKEUP:
        if (USBH_OK == usbh_clrdevfeature(uhost, FEATURE_SELECTOR_DEV, 0U)) {
            /* user callback for initialization */
            uhost->usr_cb->dev_init();

            uhost->cur_state = HOST_CHECK_CLASS;
        }
        break;
#endif

    case HOST_CLASS_ENUM:
        /* process class standard control requests state machine */
        status = uhost->active_class->class_requests(uhost);

        if (USBH_OK == status) {
            uhost->cur_state = HOST_CLASS_HANDLER;
            
            // udisk du
            // udiskMounted = 1;
            // delay(1000);
            // delay(1000);
        } else {
            usbh_error_handler (uhost, status);
        }
        break;

    case HOST_CLASS_HANDLER:
        /* process class state machine */
        status = uhost->active_class->class_machine(uhost);

        usbh_error_handler (uhost, status);
        break;

    case HOST_ERROR:
        /* initialize host for new enumeration */
        usbh_deinit (uhost);
        uhost->usr_cb->dev_deinit();
        uhost->active_class->class_deinit(uhost);
        break;

    case HOST_DEV_DETACHED:
        /* manage user disconnect operations*/
        uhost->usr_cb->dev_detach();

        /* re-initialize host for new enumeration */
        usbh_deinit(uhost);
        uhost->usr_cb->dev_deinit();
        uhost->active_class->class_deinit(uhost);
        usbh_pipe_delete(udev);
        uhost->cur_state = HOST_DEFAULT;

        // udisk du
        // udiskMounted = 2;
        // plr_num = 0;
        plr_flag = false;
        break;

    default:
        break;
    }
}

/*!
    \brief      handle the error on USB host side
    \param[in]  uhost: pointer to USB host
    \param[in]  err_type: type of error or busy/OK state
    \param[out] none
    \retval     none
*/
void usbh_error_handler (usbh_host *uhost, usbh_status err_type)
{
    /* error unrecovered or not supported device speed */
    if ((USBH_SPEED_UNKNOWN_ERROR == err_type) || (USBH_UNRECOVERED_ERROR == err_type)) {
        uhost->usr_cb->dev_error();

        uhost->cur_state = HOST_ERROR;
    } else if (USBH_APPLY_DEINIT == err_type) {
        uhost->cur_state = HOST_ERROR;

        /* user callback for initialization */
        uhost->usr_cb->dev_init();
    } else {
        /* no operation */
    }
}

/*!
    \brief      USB SOF callback function from the interrupt
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
static uint8_t usbh_sof (usbh_host *uhost)
{
    usb_core_driver *udev = (usb_core_driver *)uhost->data;

    /* this callback could be used to implement a scheduler process */
    uhost->control.timer = (uint16_t)usb_curframe_get(udev);

    if (uhost->active_class != NULL) {
        if (uhost->active_class->class_sof != NULL) {
            uhost->active_class->class_sof(uhost);
        }
    }

    return 0U;
}

/*!
    \brief      USB connect callback function from the interrupt
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
static uint8_t usbh_connect (usbh_host *uhost)
{
    usb_core_driver *udev = (usb_core_driver *)uhost->data;
    udev->host.connect_status = 1U;

    return 0U;
}

/*!
    \brief      USB disconnect callback function from the interrupt
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
static uint8_t usbh_disconnect (usbh_host *uhost)
{
    usb_core_driver *udev = (usb_core_driver *)uhost->data;
    udev->host.connect_status = 0U;

    return 0U;
}

/*!
    \brief      USB port enable callback function from the interrupt
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
static uint8_t usbh_port_enabled (usbh_host *uhost)
{
    usb_core_driver *udev = (usb_core_driver *)uhost->data;
    udev->host.port_enabled = 1U;

    return 0U;
}

/*!
    \brief      USB port disabled callback function from the interrupt
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
static uint8_t usbh_port_disabled (usbh_host *uhost)
{
    usb_core_driver *udev = (usb_core_driver *)uhost->data;
    udev->host.port_enabled = 0U;

    return 0U;
}

/*!
    \brief      handle the USB enumeration task
    \param[in]  uhost: pointer to host
    \param[out] none
    \retval     none
*/
static usbh_status usbh_enum_task (usbh_host *uhost)
{
    uint8_t str_buf[64];
    usbh_status status = USBH_BUSY;
    usb_core_driver *udev = (usb_core_driver *)uhost->data;

    static uint8_t index_mfc_str = 0U, index_prod_str = 0U, index_serial_str = 0U;

    switch (uhost->enum_state) {
    case ENUM_DEFAULT:
        /* get device descriptor for only 1st 8 bytes : to get ep0 max packet size */
        if (USBH_OK == usbh_devdesc_get (uhost, 8U)) {
            uhost->control.max_len = uhost->dev_prop.dev_desc.bMaxPacketSize0;

            /* modify control channels configuration for maximum packet size */
            usbh_pipe_update (udev,
                              uhost->control.pipe_out_num,
                              0U, 0U,
                              (uint16_t)uhost->control.max_len);

            usbh_pipe_update (udev,
                              uhost->control.pipe_in_num,
                              0U, 0U,
                              (uint16_t)uhost->control.max_len);

            uhost->enum_state = ENUM_GET_DEV_DESC;
        }
        break;

    case ENUM_GET_DEV_DESC:
        /* get full device descriptor */
        if (USBH_OK == usbh_devdesc_get (uhost, USB_DEV_DESC_LEN)) {
            uhost->usr_cb->dev_devdesc_assigned(&uhost->dev_prop.dev_desc);

            index_mfc_str = uhost->dev_prop.dev_desc.iManufacturer;
            index_prod_str = uhost->dev_prop.dev_desc.iProduct;
            index_serial_str = uhost->dev_prop.dev_desc.iSerialNumber;

            uhost->enum_state = ENUM_SET_ADDR;
        }
        break;

    case ENUM_SET_ADDR: 
        /* set address */
        if (USBH_OK == usbh_setaddress (uhost, USBH_DEV_ADDR)) {
            usb_mdelay (2U);

            uhost->dev_prop.addr = USBH_DEV_ADDR;

            /* user callback for device address assigned */
            uhost->usr_cb->dev_address_set();

            /* modify control channels to update device address */
            usbh_pipe_update (udev,
                              uhost->control.pipe_in_num,
                              uhost->dev_prop.addr,
                              0U, 0U);

            usbh_pipe_update (udev,
                              uhost->control.pipe_out_num,
                              uhost->dev_prop.addr,
                              0U, 0U);

            uhost->enum_state = ENUM_GET_CFG_DESC;
        }
        break;

    case ENUM_GET_CFG_DESC:
        /* get standard configuration descriptor */
        if (USBH_OK == usbh_cfgdesc_get (uhost, USB_CFG_DESC_LEN)) {
            uhost->enum_state = ENUM_GET_CFG_DESC_SET;
        }
        break;

    case ENUM_GET_CFG_DESC_SET:
        /* get full configure descriptor (config, interface, endpoints) */
        if (USBH_OK == usbh_cfgdesc_get (uhost, uhost->dev_prop.cfg_desc_set.cfg_desc.wTotalLength)) {
            /* user callback for configuration descriptors available */
            uhost->usr_cb->dev_cfgdesc_assigned (&uhost->dev_prop.cfg_desc_set.cfg_desc,
                                                  &uhost->dev_prop.cfg_desc_set.itf_desc_set[0][0].itf_desc,
                                                  &uhost->dev_prop.cfg_desc_set.itf_desc_set[0][0].ep_desc[0]);

            uhost->enum_state = ENUM_GET_STR_DESC;
        }
        break;

    case ENUM_GET_STR_DESC:
        if (index_mfc_str) {
            if (USBH_OK == usbh_strdesc_get (uhost, 
                                             uhost->dev_prop.dev_desc.iManufacturer, 
                                             str_buf,
                                             0xFFU)) {
                /* user callback for manufacturing string */
                uhost->usr_cb->dev_mfc_str(str_buf);

                index_mfc_str = 0U;
            }
        } else {
            if (index_prod_str) {
                /* check that product string is available */
                if (USBH_OK == usbh_strdesc_get (uhost, 
                                                 uhost->dev_prop.dev_desc.iProduct, 
                                                 str_buf, 
                                                 0xFFU)) {
                    uhost->usr_cb->dev_prod_str(str_buf);

                    index_prod_str = 0U;
                }
            } else {
                if (index_serial_str) {
                    if (USBH_OK == usbh_strdesc_get (uhost, 
                                                     uhost->dev_prop.dev_desc.iSerialNumber,
                                                     str_buf, 
                                                     0xFFU)) {
                        uhost->usr_cb->dev_seral_str(str_buf);
                        uhost->enum_state = ENUM_SET_CONFIGURATION;
                        index_serial_str = 0U;
                    }
                } else {
                    uhost->enum_state = ENUM_SET_CONFIGURATION;
                }
            }
        }
        break;

    case ENUM_SET_CONFIGURATION:
        if (USBH_OK == usbh_setcfg (uhost, (uint16_t)uhost->dev_prop.cfg_desc_set.cfg_desc.bConfigurationValue)) {
            uhost->enum_state = ENUM_DEV_CONFIGURED;
        }
        break;

    case ENUM_DEV_CONFIGURED:
        status = USBH_OK;
        break;

    default:
        break;
    }

    return status;
}

#ifdef USB_LOW_PWR_ENABLE

/*!
    \brief      handles the USB resume from suspend mode
    \param[in]  udev: pointer to selected USB device
    \param[out] none
    \retval     none
*/
void usb_hwp_resume(usb_core_driver *udev)
{
    __IO uint32_t hprt = 0U;

    /*  switch-on the clocks */
    *udev->regs.PWRCLKCTL &= ~PWRCLKCTL_SUCLK;

    *udev->regs.PWRCLKCTL &= ~PWRCLKCTL_SHCLK;

    hprt = usb_port_read(udev);

    hprt &= ~HPCS_PSP;
    hprt |= HPCS_PREM;

    *udev->regs.HPCS = hprt;

    usb_mdelay (20U);

    hprt &= ~HPCS_PREM;

    *udev->regs.HPCS = hprt;
}

/*!
    \brief      handles the USB enter to suspend mode
    \param[in]  udev: pointer to selected USB device
    \param[out] none
    \retval     none
*/
static void usb_hwp_suspend(usb_core_driver *udev)
{
    __IO uint32_t hprt = 0U;

    hprt = usb_port_read(udev);

    hprt |= HPCS_PSP;

    *udev->regs.HPCS = hprt;

    /*  switch-off the clocks */
    *udev->regs.PWRCLKCTL |= PWRCLKCTL_SUCLK;

    *udev->regs.PWRCLKCTL |= PWRCLKCTL_SHCLK;
}

#endif /* USB_LOW_PWR_ENABLE */
