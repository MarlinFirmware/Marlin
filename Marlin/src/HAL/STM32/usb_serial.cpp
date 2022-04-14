/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(EMERGENCY_PARSER) && USBD_USE_CDC

#include "usb_serial.h"
#include "../../feature/e_parser.h"

EmergencyParser::State emergency_state = EmergencyParser::State::EP_RESET;

int8_t (*USBD_CDC_Receive_original) (uint8_t *Buf, uint32_t *Len) = nullptr;

static int8_t USBD_CDC_Receive_hook(uint8_t *Buf, uint32_t *Len) {
  for (uint32_t i = 0; i < *Len; i++)
    emergency_parser.update(emergency_state, Buf[i]);
  return USBD_CDC_Receive_original(Buf, Len);
}

typedef struct _USBD_CDC_Itf {
  int8_t (* Init)(void);
  int8_t (* DeInit)(void);
  int8_t (* Control)(uint8_t cmd, uint8_t *pbuf, uint16_t length);
  int8_t (* Receive)(uint8_t *Buf, uint32_t *Len);
  int8_t (* Transferred)(void);
} USBD_CDC_ItfTypeDef;

extern USBD_CDC_ItfTypeDef USBD_CDC_fops;

void USB_Hook_init() {
  USBD_CDC_Receive_original = USBD_CDC_fops.Receive;
  USBD_CDC_fops.Receive = USBD_CDC_Receive_hook;
}

#endif // EMERGENCY_PARSER && USBD_USE_CDC
#endif // HAL_STM32
