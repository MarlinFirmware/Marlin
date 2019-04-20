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
// Prefix header for split-up "advanced" configuration
//

#if __has_include("../../config/sd.h")
  #include "../../config/sd.h"
#endif
#if __has_include("../../config/lcd-advanced.h")
  #include "../../config/lcd-advanced.h"
#endif
#if __has_include("../../config/babystepping.h")
  #include "../../config/babystepping.h"
#endif
#if __has_include("../../config/lin-advance.h")
  #include "../../config/lin-advance.h"
#endif
#if __has_include("../../config/homing.h")
  #include "../../config/homing.h"
#endif
#if __has_include("../../config/serial.h")
  #include "../../config/serial.h"
#endif
#if __has_include("../../config/steppers.h")
  #include "../../config/steppers.h"
#endif
#if __has_include("../../config/steppers-tmc26x.h")
  #include "../../config/steppers-tmc26x.h"
#endif
#if __has_include("../../config/steppers-trinamic.h")
  #include "../../config/steppers-trinamic.h"
#endif
#if __has_include("../../config/steppers-l6470.h")
  #include "../../config/steppers-l6470.h"
#endif
#if __has_include("../../config/calibration.h")
  #include "../../config/calibration.h"
#endif
#if __has_include("../../config/hephestos2.h")
  #include "../../config/hephestos2.h"
#endif
#if __has_include("../../config/dual-axis.h")
  #include "../../config/dual-axis.h"
#endif
#if __has_include("../../config/idex.h")
  #include "../../config/idex.h"
#endif
#if __has_include("../../config/serial-adv.h")
  #include "../../config/serial-adv.h"
#endif
#if __has_include("../../config/temperature-adv.h")
  #include "../../config/temperature-adv.h"
#endif
#if __has_include("../../config/fans.h")
  #include "../../config/fans.h"
#endif
#if __has_include("../../config/case-light.h")
  #include "../../config/case-light.h"
#endif
#if __has_include("../../config/backlash.h")
  #include "../../config/backlash.h"
#endif
#if __has_include("../../config/closed-loop.h")
  #include "../../config/closed-loop.h"
#endif
#if __has_include("../../config/i2c-position-encoders.h")
  #include "../../config/i2c-position-encoders.h"
#endif
#if __has_include("../../config/max7219.h")
  #include "../../config/max7219.h"
#endif
#if __has_include("../../config/nanodlp.h")
  #include "../../config/nanodlp.h"
#endif
#if __has_include("../../config/wifi.h")
  #include "../../config/wifi.h"
#endif
#if __has_include("../../config/prusa-mmu2.h")
  #include "../../config/prusa-mmu2.h"
#endif
#if __has_include("../../config/printcounter.h")
  #include "../../config/printcounter.h"
#endif
#if __has_include("../../config/z-align.h")
  #include "../../config/z-align.h"
#endif
#if __has_include("../../config/advanced-pause.h")
  #include "../../config/advanced-pause.h"
#endif
#if __has_include("../../config/tool-change.h")
  #include "../../config/tool-change.h"
#endif
#if __has_include("../../config/firmware-retract.h")
  #include "../../config/firmware-retract.h"
#endif
#if __has_include("../../config/i2c-bus.h")
  #include "../../config/i2c-bus.h"
#endif
#if __has_include("../../config/photo.h")
  #include "../../config/photo.h"
#endif
#if __has_include("../../config/filament-width.h")
  #include "../../config/filament-width.h"
#endif
#if __has_include("../../config/custom-menus.h")
  #include "../../config/custom-menus.h"
#endif
#if __has_include("../../config/spindle-laser.h")
  #include "../../config/spindle-laser.h"
#endif
#if __has_include("../../config/arc-support.h")
  #include "../../config/arc-support.h"
#endif
#if __has_include("../../config/extras.h")
  #include "../../config/extras.h"
#endif
