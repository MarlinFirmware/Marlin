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
 * Configuration_adv.h
 */
CONFIG_VERSION(02020000)

/**
 * Your version of GCC probably handles __has_include so sub-configs are optional.
 * Simply delete the files you don't need and leave this file untouched.
 * Don't change the order of these includes before checking for dependencies.
 */
#if __has_include("config/laser-cooler.h")
  #include "config/laser-cooler.h"
#endif
#if __has_include("config/pid-scaling.h")
  #include "config/pid-scaling.h"
#endif
#if __has_include("config/autotemp.h")
  #include "config/autotemp.h"
#endif
#if __has_include("config/caselight.h")
  #include "config/caselight.h"
#endif
#if __has_include("config/closed-loop.h")
  #include "config/closed-loop.h"
#endif
#if __has_include("config/idex.h")
  #include "config/idex.h"
#endif
#if __has_include("config/multi-stepper.h")
  #include "config/multi-stepper.h"
#endif
#if __has_include("config/ext-solenoid.h")
  #include "config/ext-solenoid.h"
#endif
#if __has_include("config/z-align.h")
  #include "config/z-align.h"
#endif
#if __has_include("config/assisted-tramming.h")
  #include "config/assisted-tramming.h"
#endif
#if __has_include("config/ft-motion.h")
  #include "config/ft-motion.h"
#endif
#if __has_include("config/input-shaping.h")
  #include "config/input-shaping.h"
#endif
#if __has_include("config/multi-nozzle.h")
  #include "config/multi-nozzle.h"
#endif
#if __has_include("config/backlash.h")
  #include "config/backlash.h"
#endif
#if __has_include("config/calibration-gcode.h")
  #include "config/calibration-gcode.h"
#endif
#if __has_include("config/motor-current.h")
  #include "config/motor-current.h"
#endif
#if __has_include("config/probe-wizard.h")
  #include "config/probe-wizard.h"
#endif
#if __has_include("config/twist.h")
  #include "config/twist.h"
#endif
#if __has_include("config/marlinui.h")
  #include "config/marlinui.h"
#endif
#if __has_include("config/bootscreen.h")
  #include "config/bootscreen.h"
#endif
#if __has_include("config/led-menu.h")
  #include "config/led-menu.h"
#endif
#if __has_include("config/progress.h")
  #include "config/progress.h"
#endif
#if __has_include("config/media-adv.h")
  #include "config/media-adv.h"
#endif
#if __has_include("config/lcd-adv.h")
  #include "config/lcd-adv.h"
#endif
#if __has_include("config/touch-ui-ftdi-eve.h")
  #include "config/touch-ui-ftdi-eve.h"
#endif
#if __has_include("config/babystepping.h")
  #include "config/babystepping.h"
#endif
#if __has_include("config/linear-advance.h")
  #include "config/linear-advance.h"
#endif
#if __has_include("config/leveling-adv.h")
  #include "config/leveling-adv.h"
#endif
#if __has_include("config/probe-temp.h")
  #include "config/probe-temp.h"
#endif
#if __has_include("config/extras-adv.h")
  #include "config/extras-adv.h" // may combine with config-extras.h
#endif
#if __has_include("config/arcs.h")
  #include "config/arcs.h"
#endif
#if __has_include("config/probe-target.h")
  #include "config/probe-target.h"
#endif
#if __has_include("config/direct-stepping.h")
  #include "config/direct-stepping.h"
#endif
#if __has_include("config/serial-adv.h")
  #include "config/serial-adv.h"
#endif
#if __has_include("config/bluetooth.h")
  #include "config/bluetooth.h"
#endif
#if __has_include("config/toolchange.h")
  #include "config/toolchange.h"
#endif
#if __has_include("config/trinamic.h")
  #include "config/trinamic.h"
#endif
#if __has_include("config/i2cbus.h")
  #include "config/i2cbus.h"
#endif
#if __has_include("config/photo.h")
  #include "config/photo.h"
#endif
#if __has_include("config/laser-spindle.h")
  #include "config/laser-spindle.h"
#endif
#if __has_include("config/fil-width.h")
  #include "config/fil-width.h"
#endif
#if __has_include("config/power-monitor.h")
  #include "config/power-monitor.h"
#endif
#if __has_include("config/cnc.h")
  #include "config/cnc.h"
#endif
#if __has_include("config/volumetric.h")
  #include "config/volumetric.h"
#endif
#if __has_include("config/reporting.h")
  #include "config/reporting.h"
#endif
#if __has_include("config/optimize.h")
  #include "config/optimize.h"
#endif
#if __has_include("config/feedrate.h")
  #include "config/feedrate.h"
#endif
#if __has_include("config/gcode.h")
  #include "config/gcode.h"
#endif
#if __has_include("config/meatpack.h")
  #include "config/meatpack.h"
#endif
#if __has_include("config/custom-menu.h")
  #include "config/custom-menu.h"
#endif
#if __has_include("config/custom-buttons.h")
  #include "config/custom-buttons.h"
#endif
#if __has_include("config/cancel-objects.h")
  #include "config/cancel-objects.h"
#endif
#if __has_include("config/joystick.h")
  #include "config/joystick.h"
#endif
#if __has_include("config/gantry-calib.h")
  #include "config/gantry-calib.h"
#endif
#if __has_include("config/freeze.h")
  #include "config/freeze.h"
#endif
#if __has_include("config/max7219.h")
  #include "config/max7219.h"
#endif
#if __has_include("config/nanodlp.h")
  #include "config/nanodlp.h"
#endif
#if __has_include("config/ethernet.h")
  #include "config/ethernet.h"
#endif
#if __has_include("config/wifi.h")
  #include "config/wifi.h"
#endif
