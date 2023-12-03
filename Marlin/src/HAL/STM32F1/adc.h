/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 *
 * adc.h - Define enumerated indices for enabled ADC Features
 */

#include "../../inc/MarlinConfig.h"

enum ADCIndex : uint8_t {
  OPTITEM(HAS_TEMP_ADC_0,        TEMP_0           )
  OPTITEM(HAS_TEMP_ADC_1,        TEMP_1           )
  OPTITEM(HAS_TEMP_ADC_2,        TEMP_2           )
  OPTITEM(HAS_TEMP_ADC_3,        TEMP_3           )
  OPTITEM(HAS_TEMP_ADC_4,        TEMP_4           )
  OPTITEM(HAS_TEMP_ADC_5,        TEMP_5           )
  OPTITEM(HAS_TEMP_ADC_6,        TEMP_6           )
  OPTITEM(HAS_TEMP_ADC_7,        TEMP_7           )
  OPTITEM(HAS_TEMP_ADC_BED,      TEMP_BED         )
  OPTITEM(HAS_TEMP_ADC_CHAMBER,  TEMP_CHAMBER     )
  OPTITEM(HAS_TEMP_ADC_PROBE,    TEMP_PROBE       )
  OPTITEM(HAS_TEMP_ADC_COOLER,   TEMP_COOLER      )
  OPTITEM(HAS_TEMP_ADC_BOARD,    TEMP_BOARD       )
  OPTITEM(HAS_TEMP_ADC_SOC,      TEMP_SOC         )
  OPTITEM(FILAMENT_WIDTH_SENSOR, FILWIDTH         )
  OPTITEM(HAS_ADC_BUTTONS,       ADC_KEY          )
  OPTITEM(HAS_JOY_ADC_X,         JOY_X            )
  OPTITEM(HAS_JOY_ADC_Y,         JOY_Y            )
  OPTITEM(HAS_JOY_ADC_Z,         JOY_Z            )
  OPTITEM(POWER_MONITOR_CURRENT, POWERMON_CURRENT )
  OPTITEM(POWER_MONITOR_VOLTAGE, POWERMON_VOLTAGE )
  ADC_COUNT
};

extern uint16_t adc_results[ADC_COUNT];
