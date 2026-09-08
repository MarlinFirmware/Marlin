/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define BOARD_INFO_NAME "MKS Monster8 V2"

//
// Limit Switches
//
#define X_STOP_PIN                          PA14
#define Y_STOP_PIN                          PA15

//
// Steppers
//
#define E4_ENABLE_PIN                       PB6   // Driver7

//
// Misc. Functions
//
#define PW_DET                              PA13  // MT_DET
#define PW_OFF                              PB12  // Z+
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA13  // MT_DET
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PB12  // Z+
#endif

//
// MKS WIFI MODULE
//
// Two jumper blocks left of the U-Disk port share these signals with the USB
// host, marked 'WiFi (L)' and 'OTA (R)' on the silkscreen. Jumper them left for
// the WiFi module, right for a USB flash drive - they can't both be connected:
//
//        1x3  - CS select
//   WIFI_CS      PD11      E4_UART_CS
//      1           2            3
//
//        2x3  - data select
//   WIFI_IO1   OTG_HS_DP      OTG_DP
//      5           3            1
//   WIFI_IO0   OTG_HS_DM      OTG_DM
//      6           4            2
//
// With the blocks set right the module still powers up and talks on its own
// UART, but WIFI_IO0 never reaches it, so it can't be put into its bootloader
// and 'MksWifi.bin' updates fail to connect.
//
#if ENABLED(MKS_WIFI_MODULE)
  #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
    #error "MKS_WIFI_MODULE and USB_FLASH_DRIVE_SUPPORT share jumpered pins on MKS Monster8 V2. Enable only one."
  #endif
  #define WIFI_SERIAL_PORT                     1  // USART1
  #define WIFI_IO0_PIN                      PB14  // MKS ESP WIFI IO0 PIN
  #define WIFI_IO1_PIN                      PB15  // MKS ESP WIFI IO1 PIN
  #define WIFI_RESET_PIN                    PD14  // MKS ESP WIFI RESET PIN
#endif

// The FYSETC_MINI_12864_2_1 uses one of the EXP pins
#define BOARD_NEOPIXEL_PIN                  PC5

#include "pins_MKS_MONSTER8_common.h"
