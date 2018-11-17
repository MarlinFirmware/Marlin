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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(POWER_LOSS_RECOVERY)

#include "../../gcode.h"
#include "../../../feature/power_loss_recovery.h"
#include "../../../module/motion.h"
#include "../../../lcd/ultralcd.h"

void menu_job_recovery();

#if ENABLED(DEBUG_POWER_LOSS_RECOVERY)

  inline void plr_error(PGM_P const prefix) {
    SERIAL_ECHO_START();
    serialprintPGM(prefix);
    SERIAL_ECHOLNPGM(" Power-Loss Recovery Data");
  }

#endif

/**
 * M1000: Resume from power-loss (undocumented)
 *   - With 'S' go to the Resume/Cancel menu
 *   - With no parameters, run recovery commands
 */
void GcodeSuite::M1000() {

  if (recovery.valid()) {
    if (parser.seen('S'))
      ui.goto_screen(menu_job_recovery);
    else
      recovery.resume();
  }
  else {
    #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
      plr_error(recovery.info.valid_head ? PSTR("No") : PSTR("Invalid"));
    #endif
  }

}

#endif // POWER_LOSS_RECOVERY
