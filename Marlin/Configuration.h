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
CONFIG_VERSION(02020000)

/**
 * Here are some useful links to help get your machine configured and calibrated:
 *
 * Example Configs:     https://github.com/MarlinFirmware/Configurations/branches/all
 *
 * Průša Calculator:    https://blog.prusa3d.com/calculator_3416/
 *
 * Calibration Guides:  https://reprap.org/wiki/Calibration
 *                      https://reprap.org/wiki/Triffid_Hunter%27s_Calibration_Guide
 *                      https://web.archive.org/web/20220907014303/https://sites.google.com/site/repraplogphase/calibration-of-your-reprap
 *                      https://youtu.be/wAL9d7FgInk
 *                      https://teachingtechyt.github.io/calibration.html
 *
 * Calibration Objects: https://www.thingiverse.com/thing:5573
 *                      https://www.thingiverse.com/thing:1278865
 */

/**
 * Your version of GCC probably handles __has_include so sub-configs are optional.
 * Simply delete the files you don't need and leave this file untouched.
 * Don't change the order of these includes before checking for dependencies.
 */
#if __has_include("config/machine.h")
  #include "config/machine.h"
#endif
#if __has_include("config/corexx.h")
  #include "config/corexx.h"
#endif
#if __has_include("config/beltprinter.h")
  #include "config/beltprinter.h"
#endif
#if __has_include("config/polargraph.h")
  #include "config/polargraph.h"
#endif
#if __has_include("config/delta.h")
  #include "config/delta.h"
#endif
#if __has_include("config/scara.h")
  #include "config/scara.h"
#endif
#if __has_include("config/tpara.h")
  #include "config/tpara.h"
#endif
#if __has_include("config/polar.h")
  #include "config/polar.h"
#endif
#if __has_include("config/robot-arm.h")
  #include "config/robot-arm.h"
#endif
#if __has_include("config/foamcutter.h")
  #include "config/foamcutter.h"
#endif
#if __has_include("config/geometry.h")
  #include "config/geometry.h"
#endif
#if __has_include("config/serial.h")
  #include "config/serial.h"
#endif
#if __has_include("config/stepper-drivers.h")
  #include "config/stepper-drivers.h"
#endif
#if __has_include("config/extruder.h")
  #include "config/extruder.h"
#endif
#if __has_include("config/singlenozzle.h")
  #include "config/singlenozzle.h"
#endif
#if __has_include("config/switching-extruder.h")
  #include "config/switching-extruder.h"
#endif
#if __has_include("config/switching-nozzle.h")
  #include "config/switching-nozzle.h"
#endif
#if __has_include("config/switching-tool.h")
  #include "config/switching-tool.h"
#endif
#if __has_include("config/parking-extruder.h")
  #include "config/parking-extruder.h"
#endif
#if __has_include("config/multi-material.h")
  #include "config/multi-material.h"
#endif
#if __has_include("config/psu-control.h")
  #include "config/psu-control.h"
#endif
#if __has_include("config/temperature.h")
  #include "config/temperature.h"
#endif
#if __has_include("config/custom-sensor.h")
  #include "config/custom-sensor.h"
#endif
#if __has_include("config/mpctemp.h")
  #include "config/mpctemp.h"
#endif
#if __has_include("config/preheat.h")
  #include "config/preheat.h"
#endif
#if __has_include("config/safety.h")
  #include "config/safety.h"
#endif
#if __has_include("config/delta.h")
  #include "config/delta.h"
#endif
#if __has_include("config/scara.h")
  #include "config/scara.h"
#endif
#if __has_include("config/tpara.h")
  #include "config/tpara.h"
#endif
#if __has_include("config/polar.h")
  #include "config/polar.h"
#endif
#if __has_include("config/endstops.h")
  #include "config/endstops.h"
#endif
#if __has_include("config/motion.h")
  #include "config/motion.h"
#endif
#if __has_include("config/jerk.h")
  #include "config/jerk.h"
#endif
#if __has_include("config/nozzle-park.h")
  #include "config/nozzle-park.h"
#endif
#if __has_include("config/nozzle-clean.h")
  #include "config/nozzle-clean.h"
#endif
#if __has_include("config/probe.h")
  #include "config/probe.h"
#endif
#if __has_include("config/bltouch.h")
  #include "config/bltouch.h"
#endif
#if __has_include("config/homing.h")
  #include "config/homing.h"
#endif
#if __has_include("config/leveling.h")
  #include "config/leveling.h"
#endif
#if __has_include("config/skew.h")
  #include "config/skew.h"
#endif
#if __has_include("config/eeprom.h")
  #include "config/eeprom.h"
#endif
#if __has_include("config/host.h")
  #include "config/host.h"
#endif
#if __has_include("config/units.h")
  #include "config/units.h"
#endif
#if __has_include("config/printcounter.h")
  #include "config/printcounter.h"
#endif
#if __has_include("config/security.h")
  #include "config/security.h"
#endif
#if __has_include("config/interface.h")
  #include "config/interface.h"
#endif
#if __has_include("config/lcd.h")
  #include "config/lcd.h"
#endif
#if __has_include("config/fans.h")
  #include "config/fans.h"
#endif
#if __has_include("config/extras.h")
  #include "config/extras.h"
#endif
#if __has_include("config/lights.h")
  #include "config/lights.h"
#endif
#if __has_include("config/servos.h")
  #include "config/servos.h"
#endif
#if __has_include("config/max31865.h")
  #include "config/max31865.h"
#endif
#if __has_include("config/hephestos.h")
  #include "config/hephestos.h"
#endif
#if __has_include("config/chamber.h")
  #include "config/chamber.h"
#endif
#if __has_include("config/runout.h")
  #include "config/runout.h"
#endif
#if __has_include("config/runaway.h")
  #include "config/runaway.h"
#endif
