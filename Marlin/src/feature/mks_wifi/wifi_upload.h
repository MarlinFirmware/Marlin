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

#ifdef __cplusplus
  extern "C" {
#endif

#define ESP_FIRMWARE_FILE         "MksWifi.bin"
#define ESP_FIRMWARE_FILE_RENAME  "MKSWIFI.CUR"
#define ESP_WEB_FIRMWARE_FILE     "1:/MksWifi_Web.bin"
#define ESP_WEB_FILE              "1:/MksWifi_WebView.bin"

typedef enum {
  upload_idle,
  resetting,
  connecting,
  erasing,
  uploading,
  done
} UploadState;

typedef enum {
  success = 0,
  timeout,
  connected,
  badReply,
  fileRead,
  emptyFile,
  respHeader,
  slipFrame,
  slipState,
  slipData,
} EspUploadResult;

typedef struct {
  uint32_t fileSize;

  uint32_t uploadAddress;
  UploadState state;
  uint32_t retriesPerBaudRate;
  uint32_t connectAttemptNumber;
  millis_t lastAttemptTime;
  millis_t lastResetTime;
  uint32_t uploadBlockNumber;
  uint32_t uploadNextPercentToReport;
  EspUploadResult uploadResult;
} UPLOAD_STRUCT;

extern UPLOAD_STRUCT esp_upload;
int32_t wifi_upload(int type);

#ifdef __cplusplus
  } /* C-declarations for C++ */
#endif
