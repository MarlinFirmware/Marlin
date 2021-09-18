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

#include "../../../inc/MarlinConfigPre.h"

#include <lvgl.h>

// Functions for MKS_TEST
#if BOTH(MKS_TEST, SDSUPPORT)
  void mks_hardware_test();
  void mks_test_get();
  void mks_gpio_test();
  extern uint8_t mks_test_flag;
#else
  #define mks_test_flag 0
#endif

// String display and assets
void disp_string(uint16_t x, uint16_t y, const char * string, uint16_t charColor, uint16_t bkColor);
void disp_assets_update();
void disp_assets_update_progress(const char *msg);
