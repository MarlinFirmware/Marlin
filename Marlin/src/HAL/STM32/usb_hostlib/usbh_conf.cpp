/**
  ******************************************************************************
  * @file    usbh_conf.c
  * @author  MCD Application Team
  * @brief   This file implements the board support package for the USB host library
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../../../inc/MarlinConfig.h"
#include "STM32_USB_Host_Library/Core/Inc/usbh_core.h"
#include "usbh_pins.h"

#if ENABLED(USB_HOST_MSC_FLASH_SUPPORT)

HCD_HandleTypeDef Marlinhcd;

/**
  * @brief  SOF callback.
  * @param  hhcd HCD handle
  * @retval None
  */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_IncTimer((USBH_HandleTypeDef *)(hhcd->pData));
}

/**
  * @brief Connection Event callback.
  * @param  hhcd HCD handle
  * @retval None
  */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_DbgLog("Connected!\r\n");
  USBH_LL_Connect((USBH_HandleTypeDef *)(hhcd->pData));
}

/**
  * @brief  Disconnection Event callback.
  * @param  hhcd HCD handle
  * @retval None
  */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_DbgLog("Disconnected!\r\n");
  USBH_LL_Disconnect((USBH_HandleTypeDef *)(hhcd->pData));
}

/**
  * @brief  Port Enabled  Event callback.
  * @param  hhcd HCD handle
  * @retval None
  */
void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_DbgLog("Prot enabled!\r\n");
  USBH_LL_PortEnabled((USBH_HandleTypeDef *)(hhcd->pData));
}

/**
  * @brief  Port Disabled  Event callback.
  * @param  hhcd HCD handle
  * @retval None
  */
void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_DbgLog("Prot disabled!\r\n");
  USBH_LL_PortDisabled((USBH_HandleTypeDef *)(hhcd->pData));
}

/**
  * @brief  Notify URB state change callback.
  * @param  hhcd HCD handle
  * @param  chnum Channel number.
  *         This parameter can be a value from 1 to 15
  * @param  urb_state:
  *          This parameter can be one of these values:
  *            URB_IDLE/
  *            URB_DONE/
  *            URB_NOTREADY/
  *            URB_NYET/
  *            URB_ERROR/
  *            URB_STALL/
  * @retval None
  */
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);
  UNUSED(chnum);
  UNUSED(urb_state);
}

/**
  * @brief  Initialize the HCD MSP.
  * @param  hhcd HCD handle
  * @retval None
  */
void  HAL_HCD_MspInit(HCD_HandleTypeDef *hhcd)
{
  if(hhcd->Instance==USB_OTG_FS)
  {
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  }
  else if (hhcd->Instance == USB_OTG_HS)
  {
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = (GPIO_PIN_14 | GPIO_PIN_15);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_OTG_HS_FS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
  }
}

/**
  * @brief  DeInitialize the HCD MSP.
  * @param  hhcd HCD handle
  * @retval None
  */
void  HAL_HCD_MspDeInit(HCD_HandleTypeDef *hhcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hhcd);
}

// OTG host IRQ
extern "C" USB_OTG_ISR()
{
  HAL_HCD_IRQHandler(&Marlinhcd);
}

/**
  * @brief  USBH_LL_Init
  *         Initialize the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_LL_Init(USBH_HandleTypeDef *phost)
{
  Marlinhcd.Instance = USB_OTG_INSTANCE;
  Marlinhcd.Init.Host_channels = 12;
  Marlinhcd.Init.dma_enable = DISABLE;
  Marlinhcd.Init.low_power_enable = DISABLE;
  Marlinhcd.Init.phy_itface = HCD_PHY_EMBEDDED;
  Marlinhcd.Init.Sof_enable = DISABLE;
  Marlinhcd.Init.speed = HCD_SPEED_FULL;
  Marlinhcd.Init.vbus_sensing_enable = DISABLE;
  Marlinhcd.Init.lpm_enable = DISABLE;

  Marlinhcd.pData = phost;
  phost->pData = &Marlinhcd;

  HAL_HCD_Init(&Marlinhcd);

  USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&Marlinhcd));

  return USBH_OK;
}

/**
  * @brief  USBH_LL_DeInit
  *         De-Initialize the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
  HAL_HCD_DeInit((HCD_HandleTypeDef *)(phost->pData));

  return USBH_OK;
}

/**
  * @brief  USBH_LL_Start
  *         Start the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_LL_Start(USBH_HandleTypeDef *phost)
{
  HAL_HCD_Start((HCD_HandleTypeDef *)(phost->pData));

  return USBH_OK;
}

/**
  * @brief  USBH_LL_Stop
  *         Stop the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
  HAL_HCD_Stop((HCD_HandleTypeDef *)(phost->pData));

  return USBH_OK;
}

/**
  * @brief  USBH_LL_GetSpeed
  *         Return the USB Host Speed from the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Speeds
  */
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
  USBH_SpeedTypeDef speed;

  switch (HAL_HCD_GetCurrentSpeed((HCD_HandleTypeDef *)(phost->pData)))
  {
  case 0:
    speed = USBH_SPEED_HIGH;
    USBH_DbgLog("USB Host [HS]\r\n");
    break;

  case 1:
    speed = USBH_SPEED_FULL;
    USBH_DbgLog("USB Host [FS]\r\n");
    break;

  case 2:
    speed = USBH_SPEED_LOW;
    USBH_DbgLog("USB Host [LS]\r\n");
    break;

  default:
    speed = USBH_SPEED_FULL;
    USBH_DbgLog("USB Host [FS]\r\n");
    break;
  }

  return  speed;
}

/**
  * @brief  USBH_LL_ResetPort
  *         Reset the Host Port of the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost)
{
  HAL_HCD_ResetPort((HCD_HandleTypeDef *)(phost->pData));
  USBH_DbgLog("USB Reset Port\r\n");

  return USBH_OK;
}

/**
  * @brief  USBH_LL_GetLastXferSize
  *         Return the last transferred packet size.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval Packet Size
  */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  return HAL_HCD_HC_GetXferCount((HCD_HandleTypeDef *)(phost->pData), pipe);
}

/**
  * @brief  USBH_LL_OpenPipe
  *         Open a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe_num: Pipe index
  * @param  epnum: Endpoint Number
  * @param  dev_address: Device USB address
  * @param  speed: Device Speed
  * @param  ep_type: Endpoint Type
  * @param  mps: Endpoint Max Packet Size
  * @retval USBH Status
  */
USBH_StatusTypeDef   USBH_LL_OpenPipe(USBH_HandleTypeDef *phost,
                                      uint8_t pipe_num,
                                      uint8_t epnum,
                                      uint8_t dev_address,
                                      uint8_t speed,
                                      uint8_t ep_type,
                                      uint16_t mps)
{
  HAL_HCD_HC_Init((HCD_HandleTypeDef *)(phost->pData), pipe_num, epnum, dev_address, speed, ep_type, mps);

  return USBH_OK;
}

/**
  * @brief  USBH_LL_ClosePipe
  *         Close a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe_num: Pipe index
  * @retval USBH Status
  */
USBH_StatusTypeDef   USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  HAL_HCD_HC_Halt((HCD_HandleTypeDef *)(phost->pData), pipe);

  return USBH_OK;
}

/**
  * @brief  USBH_LL_SubmitURB
  *         Submit a new URB to the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *         This parameter can be a value from 1 to 15
  * @param  direction : Channel number
  *          This parameter can be one of the these values:
  *           0 : Output
  *           1 : Input
  * @param  ep_type : Endpoint Type
  *          This parameter can be one of the these values:
  *            @arg EP_TYPE_CTRL: Control type
  *            @arg EP_TYPE_ISOC: Isochronous type
  *            @arg EP_TYPE_BULK: Bulk type
  *            @arg EP_TYPE_INTR: Interrupt type
  * @param  token : Endpoint Type
  *          This parameter can be one of the these values:
  *            @arg 0: PID_SETUP
  *            @arg 1: PID_DATA
  * @param  pbuff : pointer to URB data
  * @param  length : Length of URB data
  * @param  do_ping : activate do ping protocol (for high speed only)
  *          This parameter can be one of the these values:
  *           0 : do ping inactive
  *           1 : do ping active
  * @retval Status
  */

USBH_StatusTypeDef   USBH_LL_SubmitURB(USBH_HandleTypeDef *phost,
                                       uint8_t pipe,
                                       uint8_t direction,
                                       uint8_t ep_type,
                                       uint8_t token,
                                       uint8_t *pbuff,
                                       uint16_t length,
                                       uint8_t do_ping)
{
  HAL_HCD_HC_SubmitRequest((HCD_HandleTypeDef *)(phost->pData), pipe, direction, ep_type, token, pbuff, length, do_ping);

  return USBH_OK;
}

/**
  * @brief  USBH_LL_GetURBState
  *         Get a URB state from the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *         This parameter can be a value from 1 to 15
  * @retval URB state
  *          This parameter can be one of the these values:
  *            @arg URB_IDLE
  *            @arg URB_DONE
  *            @arg URB_NOTREADY
  *            @arg URB_NYET
  *            @arg URB_ERROR
  *            @arg URB_STALL
  */
USBH_URBStateTypeDef  USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  return (USBH_URBStateTypeDef) HAL_HCD_HC_GetURBState((HCD_HandleTypeDef *)(phost->pData), pipe);
}

/**
  * @brief  USBH_LL_DriverVBUS
  *         Drive VBUS.
  * @param  phost: Host handle
  * @param  state : VBUS state
  *          This parameter can be one of the these values:
  *           0 : VBUS Active
  *           1 : VBUS Inactive
  * @retval Status
  */

USBH_StatusTypeDef  USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phost);
  UNUSED(state);

  return USBH_OK;
}

/**
  * @brief  USBH_LL_SetToggle
  *         Set toggle for a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  pipe_num: Pipe index
  * @param  toggle: toggle (0/1)
  * @retval Status
  */
USBH_StatusTypeDef   USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)
{
  if (Marlinhcd.hc[pipe].ep_is_in)
  {
    Marlinhcd.hc[pipe].toggle_in = toggle;
  }
  else
  {
    Marlinhcd.hc[pipe].toggle_out = toggle;
  }

  return USBH_OK;
}

/**
  * @brief  USBH_LL_GetToggle
  *         Return the current toggle of a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval toggle (0/1)
  */
uint8_t  USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phost);
  UNUSED(pipe);

  uint8_t toggle = 0U;

  if (Marlinhcd.hc[pipe].ep_is_in)
  {
    toggle = Marlinhcd.hc[pipe].toggle_in;
  }
  else
  {
    toggle = Marlinhcd.hc[pipe].toggle_out;
  }

  return toggle;
}
/**
  * @brief  USBH_Delay
  *         Delay routine for the USB Host Library
  * @param  Delay: Delay in ms
  * @retval None
  */
void  USBH_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#endif // USB_HOST_MSC_FLASH_SUPPORT
