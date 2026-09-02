/**
  **************************************************************************
  * @file     audio_class.c
  * @brief    usb audio class type
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
#include "usbd_core.h"
#include "audio_class.h"
#include "audio_desc.h"
#include "audio_codec.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @defgroup USB_audio_class
  * @brief usb device class audio demo
  * @{
  */

/** @defgroup USB_audio_class_private_functions
  * @{
  */

static usb_sts_type class_init_handler(void *udev);
static usb_sts_type class_clear_handler(void *udev);
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup);
static usb_sts_type class_ept0_tx_handler(void *udev);
static usb_sts_type class_ept0_rx_handler(void *udev);
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_sof_handler(void *udev);
static usb_sts_type class_event_handler(void *udev, usbd_event_type event);

static void audio_req_get_cur(void *udev, usb_setup_type *setup);
static void audio_req_set_cur(void *udev, usb_setup_type *setup);
static void audio_req_get_min(void *udev, usb_setup_type *setup);
static void audio_req_get_max(void *udev, usb_setup_type *setup);
static void audio_req_get_res(void *udev, usb_setup_type *setup);
static void audio_get_interface(void *udev, usb_setup_type *setup);
static void audio_set_interface(void *udev, usb_setup_type *setup);

usb_audio_type audio_struct = {0, 0, 0, 0, 0, 0x1400, 0, 0, 0, {0x0000, 0x1400, 0x33}, {0x0000, 0x1400, 0x33}, 0, 0};

/* usb device class handler */
usbd_class_handler audio_class_handler =
{
  class_init_handler,
  class_clear_handler,
  class_setup_handler,
  class_ept0_tx_handler,
  class_ept0_rx_handler,
  class_in_handler,
  class_out_handler,
  class_sof_handler,
  class_event_handler,
  &audio_struct
};

/**
  * @brief  initialize usb custom hid endpoint
  * @param  udev: usb device core handler type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_init_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  /* enable microphone in endpoint double buffer mode */
  usbd_ept_dbuffer_enable(pudev, USBD_AUDIO_MIC_IN_EPT);

  /* open microphone in endpoint */
  usbd_ept_open(pudev, USBD_AUDIO_MIC_IN_EPT, EPT_ISO_TYPE, AUDIO_MIC_IN_MAXPACKET_SIZE);

  /* enable speaker out endpoint double buffer mode */
  usbd_ept_dbuffer_enable(pudev, USBD_AUDIO_SPK_OUT_EPT);

  /* open speaker out endpoint */
  usbd_ept_open(pudev, USBD_AUDIO_SPK_OUT_EPT, EPT_ISO_TYPE, AUDIO_SPK_OUT_MAXPACKET_SIZE);

  /* enable speaker feedback endpoint double buffer mode */
  usbd_ept_dbuffer_enable(pudev, USBD_AUDIO_FEEDBACK_EPT);

  /* open speaker feedback endpoint */
  usbd_ept_open(pudev, USBD_AUDIO_FEEDBACK_EPT, EPT_ISO_TYPE, AUDIO_FEEDBACK_MAXPACKET_SIZE);

  /* start receive speaker out data */
  usbd_ept_recv(pudev, USBD_AUDIO_SPK_OUT_EPT, paudio->audio_spk_data, AUDIO_SPK_OUT_MAXPACKET_SIZE);

  return status;
}

/**
  * @brief  clear endpoint or other state
  * @param  udev: usb device core handler type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_clear_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;

  /* close in endpoint */
  usbd_ept_close(pudev, USBD_AUDIO_MIC_IN_EPT);

  /* close in endpoint */
  usbd_ept_close(pudev, USBD_AUDIO_FEEDBACK_EPT);

  /* close out endpoint */
  usbd_ept_close(pudev, USBD_AUDIO_SPK_OUT_EPT);

  return status;
}

/**
  * @brief  usb device class setup request handler
  * @param  udev: usb device core handler type
  * @param  setup: setup packet
  * @retval status of usb_sts_type
  */
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  uint16_t len;

  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    /* class request */
    case USB_REQ_TYPE_CLASS:
      switch(setup->bRequest)
      {
        case AUDIO_REQ_GET_CUR:
          audio_req_get_cur(pudev, setup);
          break;

        case AUDIO_REQ_SET_CUR:
          audio_req_set_cur(pudev, setup);
          break;

        case AUDIO_REQ_GET_MIN:
          audio_req_get_min(pudev, setup);
          break;

        case AUDIO_REQ_GET_MAX:
          audio_req_get_max(pudev, setup);
          break;

        case AUDIO_REQ_GET_RES:
          audio_req_get_res(pudev, setup);
          break;

        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
    /* standard request */
    case USB_REQ_TYPE_STANDARD:
      switch(setup->bRequest)
      {
        case USB_STD_REQ_GET_DESCRIPTOR:
          if((setup->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
          {
            usbd_desc_t *config = pudev->desc_handler->get_device_configuration();
            len = MIN(AUDIO_DESCRIPTOR_SIZE, setup->wLength);
            usbd_ctrl_send(pudev, config->descriptor + 18, len);
          }
          else
          {
            usbd_ctrl_unsupport(pudev);
          }
          break;
        case USB_STD_REQ_GET_INTERFACE:
          audio_get_interface(udev, setup);
          break;

        case USB_STD_REQ_SET_INTERFACE:
          audio_set_interface(udev, setup);
          usbd_ctrl_send_status(pudev);
          break;
        case USB_STD_REQ_CLEAR_FEATURE:
          break;
        case USB_STD_REQ_SET_FEATURE:
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;

    default:
      usbd_ctrl_unsupport(pudev);
      break;
  }
  return status;
}

/**
  * @brief  usb device endpoint 0 in status stage complete
  * @param  udev: usb device core handler type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_tx_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */

  return status;
}

/**
  * @brief  usb device endpoint 0 out status stage complete
  * @param  udev: usb device core handler type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_rx_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  uint32_t recv_len = usbd_get_recv_len(pudev, 0);
  /* ...user code... */
  if( paudio->audio_cmd == AUDIO_REQ_SET_CUR)
  {
    /* class process */
    switch( paudio->request_no)
    {
      case AUDIO_VOLUME_CONTROL:
        if( paudio->interface == AUDIO_SPK_FEATURE_UNIT_ID)
        {
           paudio->spk_volume = (uint16_t)( paudio->g_audio_cur[0] |
                                               ( paudio->g_audio_cur[1] << 8));
          /* set volume */
          audio_codec_set_spk_volume(paudio->spk_volume*100/paudio->spk_volume_limits[1]);
        }
        else
        {
          paudio->mic_volume = (uint16_t)(paudio->g_audio_cur[0] |
                                               (paudio->g_audio_cur[1] << 8));

          audio_codec_set_mic_volume(paudio->mic_volume*256/paudio->mic_volume_limits[1]);
        }
        break;
      case AUDIO_MUTE_CONTROL:
        if(paudio->interface == AUDIO_SPK_FEATURE_UNIT_ID)
        {
          paudio->spk_mute = paudio->g_audio_cur[0];
          audio_codec_set_spk_mute(paudio->spk_mute);
        }
        else
        {
          paudio->mic_mute = paudio->g_audio_cur[0];
          audio_codec_set_mic_mute(paudio->mic_mute);
        }

        break;
      case AUDIO_FREQ_SET_CONTROL:
        if(paudio->enpd == USBD_AUDIO_MIC_IN_EPT)
        {
         paudio->mic_freq = (paudio->g_audio_cur[0] |
                            (paudio->g_audio_cur[1] << 8) |
                            (paudio->g_audio_cur[2] << 16));
          audio_codec_set_mic_freq(paudio->mic_freq);
        }
        else
        {
          paudio->spk_freq = (paudio->g_audio_cur[0] |
                            (paudio->g_audio_cur[1] << 8) |
                            (paudio->g_audio_cur[2] << 16));
          audio_codec_set_spk_freq(paudio->spk_freq);
        }
        break;
      default:
        break;
    }
  }

  return status;
}

/**
  * @brief  usb device transmision complete handler
  * @param  udev: usb device core handler type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  uint32_t len = 0;

  /* ...user code...
    trans next packet data
  */
  if((ept_num & 0x7F) == (USBD_AUDIO_MIC_IN_EPT & 0x7F))
  {
    len = audio_codec_mic_get_data(paudio->audio_mic_data);
    usbd_ept_send(udev, USBD_AUDIO_MIC_IN_EPT, paudio->audio_mic_data, len);
  }

  else if((ept_num & 0x7F) == (USBD_AUDIO_FEEDBACK_EPT & 0x7F))
  {
    len = audio_codec_spk_feedback(paudio->audio_feed_back);
    usbd_ept_send(udev, USBD_AUDIO_FEEDBACK_EPT, paudio->audio_feed_back, len);
  }

  return status;
}

/**
  * @brief  usb device endpoint receive data
  * @param  udev: usb device core handler type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  uint16_t g_rxlen;

  /* get endpoint receive data length  */
  g_rxlen = usbd_get_recv_len(pudev, ept_num);

  if((ept_num & 0x7F) == (USBD_AUDIO_SPK_OUT_EPT & 0x7F))
  {
    /* speaker data*/
    audio_codec_spk_fifo_write(paudio->audio_spk_data, g_rxlen);

    /* get next data */
    usbd_ept_recv(pudev, USBD_AUDIO_SPK_OUT_EPT, paudio->audio_spk_data, AUDIO_SPK_OUT_MAXPACKET_SIZE);
  }

  return status;
}

/**
  * @brief  usb device sof handler
  * @param  udev: usb device core handler type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_sof_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */

  return status;
}

/**
  * @brief  usb device event handler
  * @param  udev: usb device core handler type
  * @param  event: usb device event
  * @retval status of usb_sts_type
  */
usb_sts_type class_event_handler(void *udev, usbd_event_type event)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  switch(event)
  {
    case USBD_RESET_EVENT:

      /* ...user code... */

      break;
    case USBD_SUSPEND_EVENT:
      paudio->spk_alt_setting = 0;
      audio_codec_spk_alt_setting(paudio->spk_alt_setting);
      paudio->mic_alt_setting = 0;
      audio_codec_mic_alt_setting(paudio->spk_alt_setting);
      /* ...user code... */

      break;
    case USBD_WAKEUP_EVENT:
      /* ...user code... */

      break;
    default:
      break;
  }
  return status;
}

/**
  * @brief  usb audio request get cur
  * @param  udev: usb device core handler type
  * @param  setup: setup class
  * @retval none
  */
static void audio_req_get_cur(void *udev, usb_setup_type *setup)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  if(HBYTE(setup->wIndex) == AUDIO_SPK_FEATURE_UNIT_ID)
  {
    if(HBYTE(setup->wValue) == AUDIO_MUTE_CONTROL)
    {
      paudio->g_audio_cur[0] = paudio->spk_mute;
      usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
    }
    else
    {
      *((uint16_t *)paudio->g_audio_cur) = paudio->spk_volume;
      usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
    }
  }
  else
  {
    if(HBYTE(setup->wValue) == AUDIO_MUTE_CONTROL)
    {
      paudio->g_audio_cur[0] = paudio->mic_mute;
      usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
    }
    else
    {
      *((uint16_t *)paudio->g_audio_cur) = paudio->mic_volume;
      usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
    }

  }
}

/**
  * @brief  usb audio request set cur
  * @param  udev: usb device core handler type
  * @param  setup: setup class
  * @retval none
  */
static void audio_req_set_cur(void *udev, usb_setup_type *setup)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  if(setup->wLength > 0)
  {
    usbd_ctrl_recv(pudev, paudio->g_audio_cur, setup->wLength);

    paudio->audio_cmd = AUDIO_REQ_SET_CUR;
    paudio->audio_cmd_len = setup->wLength;

    switch(setup->bmRequestType & AUDIO_REQ_CONTROL_MASK)
    {
      case AUDIO_REQ_CONTROL_INTERFACE:
        paudio->interface = HBYTE(setup->wIndex);
        if(HBYTE(setup->wValue) == AUDIO_MUTE_CONTROL)
        {
          paudio->request_no = AUDIO_MUTE_CONTROL;
        }
        else
        {
          paudio->request_no = AUDIO_VOLUME_CONTROL;
        }
        break;
      case AUDIO_REQ_CONTROL_ENDPOINT:
        paudio->enpd = setup->wIndex;
        paudio->request_no = AUDIO_FREQ_SET_CONTROL;
        break;
      default:
        break;
    }
  }
}

/**
  * @brief  usb audio request get min
  * @param  udev: usb device core handler type
  * @param  setup: setup class
  * @retval none
  */
static void audio_req_get_min(void *udev, usb_setup_type *setup)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  if(HBYTE(setup->wIndex) == AUDIO_SPK_FEATURE_UNIT_ID)
  {
    *((uint16_t *)paudio->g_audio_cur) = paudio->spk_volume_limits[0];
    usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
  }
  else
  {
    *((uint16_t *)paudio->g_audio_cur) = paudio->mic_volume_limits[0];
    usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
  }
}

/**
  * @brief  usb audio request get max
  * @param  udev: usb device core handler type
  * @param  setup: setup class
  * @retval none
  */
static void audio_req_get_max(void *udev, usb_setup_type *setup)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  if(HBYTE(setup->wIndex) == AUDIO_SPK_FEATURE_UNIT_ID)
  {
    *((uint16_t *)paudio->g_audio_cur) = paudio->spk_volume_limits[1];
    usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
  }
  else
  {
    *((uint16_t *)paudio->g_audio_cur) = paudio->mic_volume_limits[1];
    usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
  }
}

/**
  * @brief  usb audio request get res
  * @param  udev: usb device core handler type
  * @param  setup: setup class
  * @retval none
  */
static void audio_req_get_res(void *udev, usb_setup_type *setup)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  if(HBYTE(setup->wIndex) == AUDIO_SPK_FEATURE_UNIT_ID)
  {
    *((uint16_t *)paudio->g_audio_cur) = paudio->spk_volume_limits[2];
    usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
  }
  else
  {
    *((uint16_t *)paudio->g_audio_cur) = paudio->mic_volume_limits[2];
    usbd_ctrl_send(pudev, paudio->g_audio_cur, setup->wLength);
  }
}

/**
  * @brief  usb audio set interface
  * @param  udev: usb device core handler type
  * @param  setup: setup class
  * @retval none
  */
static void audio_set_interface(void *udev, usb_setup_type *setup)
{
  uint32_t len;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  if(LBYTE(setup->wIndex) == AUDIO_SPK_INTERFACE_NUMBER)
  {
    paudio->spk_alt_setting = setup->wValue;
    audio_codec_spk_alt_setting(paudio->spk_alt_setting);
    if(paudio->spk_alt_setting )
    {
      int len = audio_codec_spk_feedback(paudio->audio_feed_back);
      usbd_ept_recv(pudev, USBD_AUDIO_SPK_OUT_EPT, paudio->audio_spk_data, AUDIO_SPK_OUT_MAXPACKET_SIZE);
      usbd_ept_send(pudev, USBD_AUDIO_FEEDBACK_EPT, paudio->audio_feed_back, len);
    }

  }
  else if(LBYTE(setup->wIndex) == AUDIO_MIC_INTERFACE_NUMBER)
  {
    paudio->mic_alt_setting = setup->wValue;
    audio_codec_mic_alt_setting(paudio->mic_alt_setting);
    if(paudio->mic_alt_setting)
    {
      len = audio_codec_mic_get_data(paudio->audio_mic_data);
      usbd_ept_send(pudev, USBD_AUDIO_MIC_IN_EPT, paudio->audio_mic_data, len);
    }
  }

}


/**
  * @brief  usb audio get interface
  * @param  udev: usb device core handler type
  * @param  setup: setup class
  * @retval none
  */
static void audio_get_interface(void *udev, usb_setup_type *setup)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  usb_audio_type *paudio = (usb_audio_type *)pudev->class_handler->pdata;
  if(LBYTE(setup->wIndex) == AUDIO_SPK_INTERFACE_NUMBER)
  {
    usbd_ctrl_send(pudev, (uint8_t *)&paudio->spk_alt_setting, 1);
  }
  else if(LBYTE(setup->wIndex) == AUDIO_MIC_INTERFACE_NUMBER)
  {
    usbd_ctrl_send(pudev, (uint8_t *)&paudio->mic_alt_setting, 1);
  }

}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


