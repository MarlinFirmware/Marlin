/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * Define USB HOST Pins: OTG_DM, OTG_DP
 */
#ifndef USB_OTG_DM_PIN
  #define USB_OTG_DM_PIN  PA11
#endif
#ifndef USB_OTG_DP_PIN
  #define USB_OTG_DP_PIN  PA12
#endif

#if USB_OTG_DM_PIN == PA11 && USB_OTG_DP_PIN == PA12
  #define USB_OTG_ID FS
#elif USB_OTG_DM_PIN == PB14 && USB_OTG_DP_PIN == PB15
  #define USB_OTG_ID HS  // HS port used for FS function
#else
  #error "USB OTG pins error!"
#endif

#define __USB_OTG_ISR(n) void OTG_##n##_IRQHandler(void)
#define _USB_OTG_ISR(n)  __USB_OTG_ISR(n)
#define USB_OTG_ISR()    _USB_OTG_ISR(USB_OTG_ID)

#define __USB_OTG_INSTANCE(n) USB_OTG_##n
#define _USB_OTG_INSTANCE(n)  __USB_OTG_INSTANCE(n)
#define USB_OTG_INSTANCE      _USB_OTG_INSTANCE(USB_OTG_ID)
