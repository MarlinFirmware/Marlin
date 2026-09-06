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
 * feature/mks_wifi/mks_wifi_ui.h
 *
 * Everything the MKS WiFi protocol needs from a user interface.
 *
 * With TFT_LVGL_UI these all forward to the LVGL screens, so the module
 * behaves exactly as it always has. With any other UI - or none at all - the
 * screen calls compile away and the state the protocol still needs (print
 * state, selected file, transfer settings, scratch buffers) lives here.
 */

#include "../../inc/MarlinConfigPre.h"
#include "wifi_module.h"

#if HAS_MKS_WIFI_UI

  #include "../../lcd/extui/mks_ui/draw_ui.h"
  #include "../../lcd/extui/mks_ui/SPI_TFT.h"

  // Screens
  #define MKSW_UI_CLEAR()         clear_cur_ui()
  #define MKSW_UI_RETURN()        draw_return_ui()
  #define MKSW_UI_DIALOG(T)       lv_draw_dialog(T)
  #define MKSW_UI_PRINTING()      lv_draw_printing()
  #define MKSW_UI_READY_PRINT()   lv_draw_ready_print()
  #define MKSW_UI_TASK()          lv_task_handler()
  #define MKSW_UI_BUSY_TYPING()   (disp_state == KEYBOARD_UI)
  #define MKSW_UI_LIST_REFRESH()  do{ if (disp_state == WIFI_LIST_UI) disp_wifi_list(); }while(0)

  // G-code preview thumbnails
  #define MKSW_UI_PREVIEW(F)      preview_gcode_prehandle(F)
  #define MKSW_UI_PREVIEW_DONE    gcode_preview_over
  #define MKSW_UI_SHOW_PREVIEW()  do{ if (gCfgItems.from_flash_pic) flash_preview_begin = true; else default_preview_flg = true; }while(0)
  #define MKSW_UI_CLEAR_ONCE()    (once_flag = false)

  // Elapsed print time reported to the host (M992)
  #define MKSW_PRINT_HOURS        print_time.hours
  #define MKSW_PRINT_MINUTES      print_time.minutes
  #define MKSW_PRINT_SECONDS      print_time.seconds

  // Print timer shown on the printing screen
  #define MKSW_UI_TIME_RESET()    reset_print_time()
  #define MKSW_UI_TIME_START()    start_print_time()
  #define MKSW_UI_TIME_STOP()     stop_print_time()

  // Settings persisted to the MKS SPI flash by the LVGL UI
  #define MKSW_SETTINGS_SAVE()    update_spi_flash()

  // State shared with the UI
  #define MKSW_PRINT_STATE        uiCfg.print_state
  #define MKSW_COMMAND_SEND       uiCfg.command_send
  #define MKSW_CONFIG_WIFI        uiCfg.configWifi
  #define MKSW_TOTAL_SEND         uiCfg.totalSend
  #define MKSW_WIFI_NAME          uiCfg.wifi_name
  #define MKSW_WIFI_KEY           uiCfg.wifi_key
  #define MKSW_CLOUD_HOSTURL      uiCfg.cloud_hostUrl
  #define MKSW_CLOUD_PORT         uiCfg.cloud_port
  #define MKSW_MODE_SEL           gCfgItems.wifi_mode_sel
  #define MKSW_WIFI_TYPE          gCfgItems.wifi_type
  #define MKSW_CLOUD_ENABLE       gCfgItems.cloud_enable
  #define MKSW_FILESYS_TYPE       gCfgItems.fileSysType
  #define MKSW_CUR_FILESIZE       gCfgItems.curFilesize
  #define MKSW_SEL_FILE           list_file.file_name[sel_id]
  #define MKSW_SEL_LONGNAME       list_file.long_name[sel_id]

  // Scratch buffers shared with the UI, so no extra RAM is used
  #define MKSW_DMA_BUF            bmp_public_buf
  #define MKSW_FILE_BUF           public_buf

#else // !HAS_MKS_WIFI_UI

  #include "../../inc/MarlinConfig.h"
  #include "../../sd/cardreader.h"
  #include "../../module/printcounter.h"

  // Screens - nothing to draw
  #define MKSW_UI_CLEAR()         NOOP
  #define MKSW_UI_RETURN()        NOOP
  #define MKSW_UI_DIALOG(T)       NOOP
  #define MKSW_UI_PRINTING()      NOOP
  #define MKSW_UI_READY_PRINT()   NOOP
  #define MKSW_UI_TASK()          NOOP
  #define MKSW_UI_BUSY_TYPING()   false
  #define MKSW_UI_LIST_REFRESH()  NOOP

  // No thumbnails without the LVGL UI, so never divert the file read
  #define MKSW_UI_PREVIEW(F)      NOOP
  #define MKSW_UI_PREVIEW_DONE    false
  #define MKSW_UI_SHOW_PREVIEW()  NOOP
  #define MKSW_UI_CLEAR_ONCE()    NOOP

  // Elapsed print time comes straight from Marlin's own job timer
  #define MKSW_PRINT_HOURS        uint16_t(print_job_timer.duration() / 3600)
  #define MKSW_PRINT_MINUTES      uint8_t((print_job_timer.duration() / 60) % 60)
  #define MKSW_PRINT_SECONDS      uint8_t(print_job_timer.duration() % 60)

  // Marlin's own print job timer already runs, so these are no-ops
  #define MKSW_UI_TIME_RESET()    NOOP
  #define MKSW_UI_TIME_START()    NOOP
  #define MKSW_UI_TIME_STOP()     NOOP

  // Nothing to persist: without a UI these settings can't be changed at runtime
  #define MKSW_SETTINGS_SAVE()    NOOP

  // Print state, mirroring the LVGL 'print_state' values used by the protocol
  enum : uint8_t { IDLE, WORKING, PAUSING, PAUSED, REPRINTING, REPRINTED, RESUMING };

  // Volume the ESP module is browsing
  #define FILE_SYS_USB 0
  #define FILE_SYS_SD  1

  #define ESP_WIFI   0x02
  #define AP_MODEL   0x01
  #define STA_MODEL  0x02

  #define MKSW_MAX_DIR_LEVEL 10
  #define MKSW_MAX_PATH (FILENAME_LENGTH * MKSW_MAX_DIR_LEVEL + 1)
  #define MKSW_MAX_NAME (TERN(LONG_FILENAME_WRITE_SUPPORT, LONG_FILENAME_LENGTH, FILENAME_LENGTH * 2) + 1)

  typedef struct {
    uint8_t  print_state;
    bool     command_send, configWifi;
    uint32_t totalSend;
    uint8_t  wifi_name[32], wifi_key[64];
    uint8_t  cloud_hostUrl[96];
    uint16_t cloud_port;
    uint8_t  wifi_mode_sel, wifi_type, fileSysType;
    bool     cloud_enable;
    uint32_t curFilesize;
    char     file_name[MKSW_MAX_PATH];   // File the host asked to print
    char     long_name[MKSW_MAX_NAME];
  } mks_wifi_state_t;
  extern mks_wifi_state_t mksWifi;

  #define MKSW_PRINT_STATE        mksWifi.print_state
  #define MKSW_COMMAND_SEND       mksWifi.command_send
  #define MKSW_CONFIG_WIFI        mksWifi.configWifi
  #define MKSW_TOTAL_SEND         mksWifi.totalSend
  #define MKSW_WIFI_NAME          mksWifi.wifi_name
  #define MKSW_WIFI_KEY           mksWifi.wifi_key
  #define MKSW_CLOUD_HOSTURL      mksWifi.cloud_hostUrl
  #define MKSW_CLOUD_PORT         mksWifi.cloud_port
  #define MKSW_MODE_SEL           mksWifi.wifi_mode_sel
  #define MKSW_WIFI_TYPE          mksWifi.wifi_type
  #define MKSW_CLOUD_ENABLE       mksWifi.cloud_enable
  #define MKSW_FILESYS_TYPE       mksWifi.fileSysType
  #define MKSW_CUR_FILESIZE       mksWifi.curFilesize
  #define MKSW_SEL_FILE           mksWifi.file_name
  #define MKSW_SEL_LONGNAME       mksWifi.long_name

  // Buffers the LVGL UI would otherwise lend us
  extern uint8_t mksWifiDmaBuf[TRANS_RCV_FIFO_BLOCK_NUM * UDISKBUFLEN];
  extern uint8_t mksWifiFileBuf[513];
  #define MKSW_DMA_BUF            mksWifiDmaBuf
  #define MKSW_FILE_BUF           mksWifiFileBuf

#endif // !HAS_MKS_WIFI_UI
