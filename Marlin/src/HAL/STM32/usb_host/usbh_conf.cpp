/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * HAL/STM32/usb_host/usbh_conf.cpp
 *
 * Low-level driver connecting ST's USB Host Library to the HAL HCD (USB OTG host) driver.
 * STM32duino ships the library but not this layer. MARLIN_USBH_CONF is set by
 * stm32_usb_host.py when the framework doesn't provide its own (as the stm-flash-drive fork does).
 *
 * USE_USBHOST_HS puts the host on USB_OTG_HS (using its internal full-speed PHY), otherwise USB_OTG_FS.
 */

#include "../../platforms.h"

#if defined(HAL_STM32) && defined(USBHOST) && defined(MARLIN_USBH_CONF)

#include <Arduino.h>
#include "usbh_core.h"

#ifndef HAL_HCD_MODULE_ENABLED
  #error "USB host requires HAL_HCD_MODULE_ENABLED."
#endif

#ifdef USE_USBHOST_HS
  #ifndef USB_OTG_HS
    #error "USE_USBHOST_HS requires an MCU with USB_OTG_HS."
  #endif
  #define USBH_INSTANCE USB_OTG_HS
  #define USBH_IRQn     OTG_HS_IRQn
  #define USBH_CHANNELS 12
#else
  #ifndef USB_OTG_FS
    #error "USB host requires an MCU with USB_OTG_FS (or USE_USBHOST_HS)."
  #endif
  #define USBH_INSTANCE USB_OTG_FS
  #define USBH_IRQn     OTG_FS_IRQn
  #define USBH_CHANNELS 8
#endif

static HCD_HandleTypeDef hhcd;

static USBH_StatusTypeDef usbh_status(const HAL_StatusTypeDef s) {
  return s == HAL_OK ? USBH_OK : s == HAL_BUSY ? USBH_BUSY : USBH_FAIL;
}

#define PHOST(H) ((USBH_HandleTypeDef*)(H)->pData)
#define PHCD(P)  ((HCD_HandleTypeDef*)(P)->pData)

extern "C" {

//
// HCD MSP and callbacks (HAL -> library)
//

void HAL_HCD_MspInit(HCD_HandleTypeDef *h) {
  if (h->Instance != USBH_INSTANCE) return;
  #ifdef USE_USBHOST_HS
    for (const PinMap *map = PinMap_USB_OTG_HS; map->pin != NC; ++map) pin_function(map->pin, map->function);
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
  #else
    for (const PinMap *map = PinMap_USB_OTG_FS; map->pin != NC; ++map) pin_function(map->pin, map->function);
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
  #endif
  HAL_NVIC_SetPriority(USBH_IRQn, USBH_IRQ_PRIO, USBH_IRQ_SUBPRIO);
  HAL_NVIC_EnableIRQ(USBH_IRQn);
}

void HAL_HCD_MspDeInit(HCD_HandleTypeDef *h) {
  if (h->Instance != USBH_INSTANCE) return;
  HAL_NVIC_DisableIRQ(USBH_IRQn);
  #ifdef USE_USBHOST_HS
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
  #else
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
  #endif
}

void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *h)          { USBH_LL_IncTimer(PHOST(h)); }
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *h)      { USBH_LL_Connect(PHOST(h)); }
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *h)   { USBH_LL_Disconnect(PHOST(h)); }
void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *h)  { USBH_LL_PortEnabled(PHOST(h)); }
void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *h) { USBH_LL_PortDisabled(PHOST(h)); }
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef*, uint8_t, HCD_URBStateTypeDef) {} // Only used with an RTOS

//
// Low-level driver (library -> HAL)
//

USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost) {
  hhcd.pData = phost;
  phost->pData = &hhcd;

  hhcd.Instance                 = USBH_INSTANCE;
  hhcd.Init.Host_channels       = USBH_CHANNELS;
  hhcd.Init.speed               = HCD_SPEED_FULL;
  hhcd.Init.dma_enable          = DISABLE;
  hhcd.Init.phy_itface          = HCD_PHY_EMBEDDED;
  hhcd.Init.Sof_enable          = DISABLE;
  hhcd.Init.low_power_enable    = DISABLE;
  hhcd.Init.vbus_sensing_enable = DISABLE;
  hhcd.Init.use_external_vbus   = DISABLE;
  if (HAL_HCD_Init(&hhcd) != HAL_OK) return USBH_FAIL;

  USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&hhcd));
  return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)    { return usbh_status(HAL_HCD_DeInit(PHCD(phost))); }
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)     { return usbh_status(HAL_HCD_Start(PHCD(phost))); }
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)      { return usbh_status(HAL_HCD_Stop(PHCD(phost))); }
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost) { return usbh_status(HAL_HCD_ResetPort(PHCD(phost))); }

USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost) {
  switch (HAL_HCD_GetCurrentSpeed(PHCD(phost))) { // Port speed: 0 high, 1 full, 2 low
    case 0:  return USBH_SPEED_HIGH;
    case 2:  return USBH_SPEED_LOW;
    default: return USBH_SPEED_FULL;
  }
}

uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe) {
  return HAL_HCD_HC_GetXferCount(PHCD(phost), pipe);
}

USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t epnum,
                                    uint8_t dev_address, uint8_t speed, uint8_t ep_type, uint16_t mps) {
  return usbh_status(HAL_HCD_HC_Init(PHCD(phost), pipe, epnum, dev_address, speed, ep_type, mps));
}

USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe) {
  return usbh_status(HAL_HCD_HC_Halt(PHCD(phost), pipe));
}

USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t direction, uint8_t ep_type,
                                     uint8_t token, uint8_t *pbuff, uint16_t length, uint8_t do_ping) {
  return usbh_status(HAL_HCD_HC_SubmitRequest(PHCD(phost), pipe, direction, ep_type, token, pbuff, length, do_ping));
}

USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe) {
  return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState(PHCD(phost), pipe);
}

// VBUS is always on for the boards using this
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef*, uint8_t) {
  HAL_Delay(200);
  return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle) {
  HCD_HandleTypeDef * const h = PHCD(phost);
  if (h->hc[pipe].ep_is_in) h->hc[pipe].toggle_in = toggle; else h->hc[pipe].toggle_out = toggle;
  return USBH_OK;
}

uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe) {
  const HCD_HandleTypeDef * const h = PHCD(phost);
  return h->hc[pipe].ep_is_in ? h->hc[pipe].toggle_in : h->hc[pipe].toggle_out;
}

void USBH_Delay(uint32_t ms) { HAL_Delay(ms); }

//
// USB OTG interrupt
//
#ifdef USE_USBHOST_HS
  void OTG_HS_IRQHandler() { HAL_HCD_IRQHandler(&hhcd); }
#else
  void OTG_FS_IRQHandler() { HAL_HCD_IRQHandler(&hhcd); }
#endif

} // extern "C"

#endif // HAL_STM32 && USBHOST && MARLIN_USBH_CONF
