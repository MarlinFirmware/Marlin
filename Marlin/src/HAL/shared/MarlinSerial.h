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

/**
 * HAL/shared/MarlinSerial.h
 */

#include "../../inc/MarlinConfigPre.h"

constexpr bool
  #if HAS_DGUS_LCD
    bDGUS_SERIAL_STATS_RX_BUFFER_OVERRUNS = (false
      #if ENABLED(DGUS_SERIAL_STATS_RX_BUFFER_OVERRUNS)
        || true
      #endif
    ),
  #endif
  bSERIAL_XON_XOFF = (false
    #if ENABLED(SERIAL_XON_XOFF)
      || true
    #endif
  ),
  bEMERGENCY_PARSER = (false
    #if ENABLED(EMERGENCY_PARSER)
      || true
    #endif
  ),
  bSERIAL_STATS_DROPPED_RX = (false
    #if ENABLED(SERIAL_STATS_DROPPED_RX)
      || true
    #endif
  ),
  bSERIAL_STATS_RX_BUFFER_OVERRUNS = (false
    #if ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS)
      || true
    #endif
  ),
  bSERIAL_STATS_RX_FRAMING_ERRORS = (false
    #if ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS)
      || true
    #endif
  ),
  bSERIAL_STATS_MAX_RX_QUEUED = (false
    #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
      || true
    #endif
  );
