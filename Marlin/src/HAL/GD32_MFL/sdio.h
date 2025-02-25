/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <SDIO.hpp>
#include <DMA.hpp>

#define SDIO_D0_PIN   40
#define SDIO_D1_PIN   41
#define SDIO_D2_PIN   42
#define SDIO_D3_PIN   43
#define SDIO_CK_PIN   44
#define SDIO_CMD_PIN  50

void sdio_mfl_init();
bool SDIO_SetBusWidth(sdio::Bus_Width width);
void DMA1_IRQHandler(dma::DMA_Channel channel);
