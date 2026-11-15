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
#include "../../gcode/queue.h"

mks_wifi_state_t mksWifi;

// Word-aligned: mksWifiFileBuf is written to the SD card 512 bytes at a time,
// and the STM32 SDIO DMA requires a word-aligned memory address.
alignas(4) uint8_t mksWifiDmaBuf[TRANS_RCV_FIFO_BLOCK_NUM * UDISKBUFLEN];
alignas(4) uint8_t mksWifiFileBuf[513];

/**
 * The LVGL screens and printer_state_polling() move 'print_state' along:
 * PAUSING to PAUSED, RESUMING to WORKING, and back to IDLE when a print
 * ends. Without them the WiFi M24/M25 handlers set it and nothing else does,
 * so a finished print stayed WORKING (M997 PRINTING; the module refuses
 * uploads and M23 until a WiFi M26) and a WiFi pause stuck at PAUSING.
 *
 * Take the state from the SD card instead. This also covers prints started
 * or paused from the display or the host.
 */
void mks_wifi_sync_print_state() {
  #if HAS_MEDIA
    // WiFi M24 on a paused print. Resume it the way M24 does, unparking the
    // head if it was parked.
    if (mksWifi.print_state == RESUMING && card.isPaused()) {
      queue.inject(F("M24"));
      mksWifi.print_state = WORKING;
      return;
    }

    if (card.isStillPrinting())   // Until M1001 runs at the end of the file
      mksWifi.print_state = WORKING;
    else if (card.isPaused())
      mksWifi.print_state = PAUSED;
    else
      mksWifi.print_state = IDLE;
  #endif
}

#endif // MKS_WIFI_MODULE && !HAS_TFT_LVGL_UI
