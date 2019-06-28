/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../inc/MarlinConfigPre.h"
#include "../shared/Marduino.h"

void u8g_SetPIOutput_DUE(u8g_t *u8g, uint8_t pin_index);
void u8g_SetPILevel_DUE(u8g_t *u8g, uint8_t pin_index, uint8_t level);

void u8g_spiSend_sw_DUE_mode_0(uint8_t val);
void u8g_spiSend_sw_DUE_mode_3(uint8_t val);

extern Pio *SCK_pPio, *MOSI_pPio;
extern uint32_t SCK_dwMask, MOSI_dwMask;
