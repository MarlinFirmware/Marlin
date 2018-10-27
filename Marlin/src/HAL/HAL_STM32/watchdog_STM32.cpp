/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifdef ARDUINO_ARCH_STM32

#include "../../inc/MarlinConfig.h"

#if ENABLED(USE_WATCHDOG)

  #include "watchdog_STM32.h"
  #include <IWatchdog.h>

  void watchdog_init() { IWatchdog.begin(4000000); } // 4 sec timeout

  void watchdog_reset() { IWatchdog.reload(); }

#endif // USE_WATCHDOG
#endif // ARDUINO_ARCH_STM32
