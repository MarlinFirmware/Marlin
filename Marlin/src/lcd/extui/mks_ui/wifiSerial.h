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
#pragma once

#ifdef SERIAL_PORT_2
  #error "SERIAL_PORT_2 must be disabled with TFT_LVGL_UI* and MKS_WIFI_MODULE."
#endif

#define WIFI_BAUDRATE          115200
#define WIFI_UPLOAD_BAUDRATE  1958400
#define USART_SAFE_INSERT

#define WIFI_RX_BUF_SIZE  (1024)
#define WIFI_TX_BUF_SIZE  (64)

#include "tft_lvgl_configuration.h"

#ifdef __STM32F1__
  #include "wifiSerial_STM32F1.h"
#else
  #include "wifiSerial_STM32.h"
#endif

extern WifiSerial WifiSerial1;
#define WIFISERIAL WifiSerial1
