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

//
// Job Recovery Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(POWER_LOSS_RECOVERY)

#include "menu.h"
#include "../../gcode/queue.h"
#include "../../sd/cardreader.h"
#include "../../feature/power_loss_recovery.h"

static void lcd_power_loss_recovery_resume() {
  char cmd[20];

  // Return to status now
  ui.return_to_status();

  // Turn leveling off and home
  enqueue_and_echo_commands_P(PSTR("M420 S0\nG28 R0"
    #if ENABLED(MARLIN_DEV_MODE)
      " S"
    #elif !IS_KINEMATIC
      " X Y"
    #endif
  ));

  #if HAS_HEATED_BED
    const int16_t bt = job_recovery_info.target_temperature_bed;
    if (bt) {
      // Restore the bed temperature
      sprintf_P(cmd, PSTR("M190 S%i"), bt);
      enqueue_and_echo_command(cmd);
    }
  #endif

  // Restore all hotend temperatures
  HOTEND_LOOP() {
    const int16_t et = job_recovery_info.target_temperature[e];
    if (et) {
      #if HOTENDS > 1
        sprintf_P(cmd, PSTR("T%i"), e);
        enqueue_and_echo_command(cmd);
      #endif
      sprintf_P(cmd, PSTR("M109 S%i"), et);
      enqueue_and_echo_command(cmd);
    }
  }

  #if HOTENDS > 1
    sprintf_P(cmd, PSTR("T%i"), job_recovery_info.active_hotend);
    enqueue_and_echo_command(cmd);
  #endif

  // Restore print cooling fan speeds
  for (uint8_t i = 0; i < FAN_COUNT; i++) {
    uint8_t f = job_recovery_info.fan_speed[i];
    if (f) {
      sprintf_P(cmd, PSTR("M106 P%i S%i"), i, f);
      enqueue_and_echo_command(cmd);
    }
  }

  // Start draining the job recovery command queue
  job_recovery_phase = JOB_RECOVERY_YES;
}

static void lcd_power_loss_recovery_cancel() {
  card.removeJobRecoveryFile();
  card.autostart_index = 0;
  ui.return_to_status();
}

void menu_job_recovery() {
  ui.defer_status_screen(true);
  START_MENU();
  STATIC_ITEM(MSG_POWER_LOSS_RECOVERY);
  MENU_ITEM(function, MSG_RESUME_PRINT, lcd_power_loss_recovery_resume);
  MENU_ITEM(function, MSG_STOP_PRINT, lcd_power_loss_recovery_cancel);
  END_MENU();
}

#endif // HAS_LCD_MENU && POWER_LOSS_RECOVERY
