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

void lv_draw_wifi_list();
void lv_clear_wifi_list();
void disp_wifi_list();
void cutWifiName(char *name, int len, char *outStr);
void wifi_scan_handle();

#define NUMBER_OF_PAGE 5

#define WIFI_TOTAL_NUMBER 20
#define WIFI_NAME_BUFFER_SIZE 33

typedef struct {
  int8_t getNameNum;
  int8_t nameIndex;
  int8_t currentWifipage;
  int8_t getPage;
  int8_t RSSI[WIFI_TOTAL_NUMBER];
  uint8_t wifiName[WIFI_TOTAL_NUMBER][WIFI_NAME_BUFFER_SIZE];
  uint8_t wifiConnectedName[WIFI_NAME_BUFFER_SIZE];
} WIFI_LIST;
extern WIFI_LIST wifi_list;

typedef struct list_menu_disp {
  const char *title;
  const char *file_pages;
} list_menu_def;
extern list_menu_def list_menu;

typedef struct keyboard_menu_disp {
  const char *title;
  const char *apply;
  const char *password;
  const char *letter;
  const char *digital;
  const char *symbol;
  const char *space;
} keyboard_menu_def;
extern keyboard_menu_def keyboard_menu;

typedef struct tips_menu_disp {
  const char *joining;
  const char *failedJoin;
  const char *wifiConected;
} tips_menu_def;
extern tips_menu_def tips_menu;

#ifdef __cplusplus
  } /* C-declarations for C++ */
#endif
