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

#if defined(STM32GENERIC) && defined(STM32F4)

#include "../../inc/MarlinConfig.h"

#if ENABLED(USE_WATCHDOG)

  #include "watchdog_STM32F4.h"

  IWDG_HandleTypeDef hiwdg;

  void watchdog_init() {
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_32; //32kHz LSI clock and 32x prescalar = 1024Hz IWDG clock
    hiwdg.Init.Reload = 4095;           //4095 counts = 4 seconds at 1024Hz
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
      //Error_Handler();
    }
  }

  void watchdog_reset() {
    /* Refresh IWDG: reload counter */
    if (HAL_IWDG_Refresh(&hiwdg) != HAL_OK) {
      /* Refresh Error */
      //Error_Handler();
    }
    else {
      #if PIN_EXISTS(LED)
        TOGGLE(LED_PIN);  // heartbeat indicator
      #endif
    }
  }

#endif // USE_WATCHDOG

#endif // STM32GENERIC && STM32F4
