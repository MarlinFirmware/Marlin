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

#define DGUS_BED_SIZE_STR STRINGIFY(X_BED_SIZE) "x" STRINGIFY(Y_BED_SIZE) "mm"

#if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
  #ifndef DGUS_AUTOSCROLL_START_CYCLES
    // Additional refresh cycles where strings beginnings are shown
    #define DGUS_AUTOSCROLL_START_CYCLES 1
  #endif
  #ifndef DGUS_AUTOSCROLL_END_CYCLES
    // Additional refresh cycles where strings endings are shown
    #define DGUS_AUTOSCROLL_END_CYCLES 1
  #endif
#endif

#ifndef DGUS_DEFAULT_VOLUME
  #define DGUS_DEFAULT_VOLUME 50
#endif

#ifndef DGUS_DEFAULT_BRIGHTNESS
  #define DGUS_DEFAULT_BRIGHTNESS 100
#endif

#ifndef DGUS_STATUS_EXPIRATION_MS
  #define DGUS_STATUS_EXPIRATION_MS 30000
#endif

#ifndef BED_TRAMMING_Z_HOP
  #define BED_TRAMMING_Z_HOP 4.0
#endif

#ifndef BED_TRAMMING_HEIGHT
  #define BED_TRAMMING_HEIGHT 0.0
#endif

static_assert(BED_TRAMMING_Z_HOP >= 0, "BED_TRAMMING_Z_HOP must be >= 0. Please update your configuration.");
