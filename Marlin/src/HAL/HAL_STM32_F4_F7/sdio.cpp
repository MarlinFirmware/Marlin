/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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

#include "../HAL.h"
#if HAL_IS_STM32_F4_F7

#ifdef __HAL_SD_ENABLE

#include "../../inc/MarlinConfig.h" // Allow pins/pins.h to set density
#include "bsp_sd.h"

bool SDIO_Init(void) {
  if (BSP_SD_Init() == MSD_OK) {
    return true;
  }
  return false;
}

bool SDIO_ReadBlock(uint32_t blockAddress, uint8_t *data) {
  return BSP_SD_ReadBlocks(data, blockAddress, 1) == MSD_OK;
}

bool SDIO_WriteBlock(uint32_t blockAddress, const uint8_t *data) {
  return BSP_SD_WriteBlocks(data, blockAddress, 1) == MSD_OK;
}

#endif // __HAL_SD_ENABLE
#endif
