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
extern "C" { /* C-declarations for C++ */
#endif

#define DIALOG_TYPE_STOP                             0
#define DIALOG_TYPE_PRINT_FILE                       1
#define DIALOG_TYPE_REPRINT_NO_FILE                  2

#define DIALOG_TYPE_M80_FAIL                         3  //**
#define DIALOG_TYPE_MESSAGE_ERR1                     4  //**

#define DIALOG_TYPE_UPDATE_ESP_FIRMARE               5
#define DIALOG_TYPE_UPDATE_ESP_DATA                  6
#define DIALOG_TYPE_UPLOAD_FILE                      7
#define DIALOG_TYPE_UNBIND                           8

#define DIALOG_TYPE_FILAMENT_LOAD_HEAT               9
#define DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED    10
#define DIALOG_TYPE_FILAMENT_LOADING                11
#define DIALOG_TYPE_FILAMENT_LOAD_COMPLETED         12
#define DIALOG_TYPE_FILAMENT_UNLOAD_HEAT            13
#define DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED  14
#define DIALOG_TYPE_FILAMENT_UNLOADING              15
#define DIALOG_TYPE_FILAMENT_UNLOAD_COMPLETED       16

#define DIALOG_TYPE_FILE_LOADING                    17  //**

#define DIALOG_TYPE_FILAMENT_NO_PRESS               18
#define DIALOG_TYPE_FINISH_PRINT                    19

#define WIFI_ENABLE_TIPS                            20

#define DIALOG_PAUSE_MESSAGE_PAUSING                21
#define DIALOG_PAUSE_MESSAGE_CHANGING               22
#define DIALOG_PAUSE_MESSAGE_UNLOAD                 23
#define DIALOG_PAUSE_MESSAGE_WAITING                24
#define DIALOG_PAUSE_MESSAGE_INSERT                 25
#define DIALOG_PAUSE_MESSAGE_LOAD                   26
#define DIALOG_PAUSE_MESSAGE_PURGE                  27
#define DIALOG_PAUSE_MESSAGE_RESUME                 28
#define DIALOG_PAUSE_MESSAGE_HEAT                   29
#define DIALOG_PAUSE_MESSAGE_HEATING                30
#define DIALOG_PAUSE_MESSAGE_OPTION                 31

#define DIALOG_STORE_EEPROM_TIPS                    32
#define DIALOG_READ_EEPROM_TIPS                     33
#define DIALOG_REVERT_EEPROM_TIPS                   34

#define BTN_OK_X      100
#define BTN_OK_Y      180
#define BTN_CANCEL_X  280
#define BTN_CANCEL_Y  180

extern void lv_draw_dialog(uint8_t type);
extern void lv_clear_dialog();

//extern void disp_temp_ready_print();
#ifdef __cplusplus
} /* C-declarations for C++ */
#endif
