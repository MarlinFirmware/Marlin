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
 * feature/mks_wifi/mks_wifi_ui.cpp
 *
 * State and buffers the MKS WiFi module borrows from the LVGL UI when it is
 * present, and has to own itself when it isn't.
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(MKS_WIFI_MODULE) && !HAS_TFT_LVGL_UI

#include "mks_wifi_ui.h"

mks_wifi_state_t mksWifi;

uint8_t mksWifiDmaBuf[TRANS_RCV_FIFO_BLOCK_NUM * UDISKBUFLEN];
uint8_t mksWifiFileBuf[513];

#endif // MKS_WIFI_MODULE && !HAS_TFT_LVGL_UI
