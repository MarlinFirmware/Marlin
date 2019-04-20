/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

//
// Prefix header for split-up "basic" configuration
//

#if __has_include("../../config/hardware.h")
  #include "../../config/hardware.h"
#endif
#if __has_include("../../config/motion.h")
  #include "../../config/motion.h"
#endif
#if __has_include("../../config/delta.h")
  #include "../../config/delta.h"
#endif
#if __has_include("../../config/scara.h")
  #include "../../config/scara.h"
#endif
#if __has_include("../../config/core.h")
  #include "../../config/core.h"
#endif
#if __has_include("../../config/extruders.h")
  #include "../../config/extruders.h"
#endif
#if __has_include("../../config/psu.h")
  #include "../../config/psu.h"
#endif
#if __has_include("../../config/endstops.h")
  #include "../../config/endstops.h"
#endif
#if __has_include("../../config/drivers.h")
  #include "../../config/drivers.h"
#endif
#if __has_include("../../config/probe.h")
  #include "../../config/probe.h"
#endif
#if __has_include("../../config/bed-leveling.h")
  #include "../../config/bed-leveling.h"
#endif
#if __has_include("../../config/skew.h")
  #include "../../config/skew.h"
#endif
#if __has_include("../../config/temperature.h")
  #include "../../config/temperature.h"
#endif
#if __has_include("../../config/runout.h")
  #include "../../config/runout.h"
#endif
#if __has_include("../../config/nozzle-park.h")
  #include "../../config/nozzle-park.h"
#endif
#if __has_include("../../config/nozzle-clean.h")
  #include "../../config/nozzle-clean.h"
#endif
#if __has_include("../../config/lcd-basic.h")
  #include "../../config/lcd-basic.h"
#endif
#if __has_include("../../config/leds.h")
  #include "../../config/leds.h"
#endif
#if __has_include("../../config/eeprom.h")
  #include "../../config/eeprom.h"
#endif
#if __has_include("../../config/servos.h")
  #include "../../config/servos.h"
#endif
