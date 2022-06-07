/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

<<<<<<<< HEAD:Marlin/src/pins/stm32f1/pins_CREALITY_V24S1.h
/**
 * Creality v2.4.S1 (STM32F103RE / STM32F103RC) v101 as found in the Ender 7
 */

#define BOARD_INFO_NAME      "Creality v2.4.S1 V101"
#define DEFAULT_MACHINE_NAME "Creality3D"

//
// Heaters
//
#define HEATER_BED_PIN                      PA15  // HOT BED

#include "pins_CREALITY_V4.h"
========
#define ALLOW_STM32DUINO
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS Eagle supports up to 2 hotends / E-steppers."
#elif HAS_FSMC_TFT
  #error "MKS Eagle doesn't support FSMC-based TFT displays."
#endif

#define BOARD_INFO_NAME "MKS Eagle"

#include "pins_MKS_ROBIN_NANO_V3_common.h"
>>>>>>>> upstream/bugfix-2.0.x:Marlin/src/pins/stm32f4/pins_MKS_EAGLE.h
