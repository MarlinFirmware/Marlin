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

void lv_draw_wifi_tips();
void lv_clear_wifi_tips();

typedef enum {
  TIPS_TYPE_JOINING,
  TIPS_TYPE_TAILED_JOIN,
  TIPS_TYPE_WIFI_CONECTED
} TIPS_TYPE;
extern TIPS_TYPE wifi_tips_type;

typedef struct {
  unsigned char timer;
  uint16_t timer_count;
} TIPS_DISP;
extern TIPS_DISP tips_disp;

#define TIPS_TIMER_START 1
#define TIPS_TIMER_STOP 0

#ifdef __cplusplus
  } /* C-declarations for C++ */
#endif
