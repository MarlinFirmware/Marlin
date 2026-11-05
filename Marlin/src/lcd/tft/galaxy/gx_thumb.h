/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * NOVIN3DP Galaxy UI - G-code QOI thumbnails ("; thumbnail_QOI begin WxH LEN")
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(NOVIN3DP_GALAXY_UI)

#include <stdint.h>

// Queue a thumbnail into the current canvas at (x, y). A file icon is drawn if the
// file has no QOI thumbnail of exactly this size. 'job' uses the directory of the
// file that is being printed instead of the current working directory.
void gx_thumb(const int16_t x, const int16_t y, const uint8_t size, const char *dosname, const bool job=false);

// Remember the directory and names of the file that is about to be printed
void gx_thumb_set_job(const char *dosname, const char *longname);
bool gx_thumb_has_job();
bool gx_job_file_ok();          // The last job's file can be opened
const char* gx_job_name();
const char* gx_job_dosname();

#endif
