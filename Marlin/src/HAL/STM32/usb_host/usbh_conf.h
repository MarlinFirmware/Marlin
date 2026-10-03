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
#pragma once

/**
 * HAL/STM32/usb_host/usbh_conf.h
 *
 * Configuration for ST's USB Host Library (STM32_USB_Host_Library), which STM32duino ships
 * without building it. buildroot/share/PlatformIO/scripts/stm32_usb_host.py adds this folder
 * and the library to the build for environments with USBHOST.
 */

#include "stm32_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Mass storage only: one device, one class
#define USBH_MAX_NUM_ENDPOINTS        2U
#define USBH_MAX_NUM_INTERFACES       2U
#define USBH_MAX_NUM_CONFIGURATION    2U
#define USBH_KEEP_CFG_DESCRIPTOR      2U
#define USBH_MAX_NUM_SUPPORTED_CLASS  2U
#define USBH_MAX_SIZE_CONFIGURATION   256U
#define USBH_MAX_DATA_BUFFER          512U
#define USBH_DEBUG_LEVEL              0U
#define USBH_USE_OS                   0U

// Host port identifiers passed to USBH_Init
#define HOST_HS                       0
#define HOST_FS                       1

#ifndef USBH_IRQ_PRIO
  #define USBH_IRQ_PRIO               0
#endif
#ifndef USBH_IRQ_SUBPRIO
  #define USBH_IRQ_SUBPRIO            0
#endif

#define USBH_malloc   malloc
#define USBH_free     free
#define USBH_memset   memset
#define USBH_memcpy   memcpy

#define USBH_UsrLog(...) do{}while(0)
#define USBH_ErrLog(...) do{}while(0)
#define USBH_DbgLog(...) do{}while(0)

#ifdef __cplusplus
}
#endif
