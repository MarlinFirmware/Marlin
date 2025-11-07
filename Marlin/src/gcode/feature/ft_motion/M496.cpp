/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(FTM_RESONANCE_TEST)

#include "../../gcode.h"
#include "../../../module/ft_motion/trajectory_resonance.h"
#include "../../../module/ft_motion.h"

/**
 * M496: Abort the resonance test (via Emergency Parser)
 */
void GcodeSuite::M496() {
  if (ftMotion.getTrajectoryType() == TrajectoryType::RESONANCE) {
    if (ftMotion.rtg->isActive()) {
      ftMotion.rtg->abort();
      EmergencyParser::rt_stop_by_M496 = false;
      #if DISABLED(MARLIN_SMALL_BUILD)
        SERIAL_ECHOLN(F("Resonance Test"), F(" aborted."));
      #endif
      return;
    }
  }
  #if DISABLED(MARLIN_SMALL_BUILD)
    SERIAL_ECHOLN(F("No active "), F("Resonance Test"), F(" to abort."));
  #endif
}

#endif // FTM_RESONANCE_TEST
