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

/**
 * power_loss_recovery.h - Resume an SD print after power-loss
 */

#ifndef _POWER_LOSS_RECOVERY_H_
#define _POWER_LOSS_RECOVERY_H_

#include "cardreader.h"
#include "types.h"
#include "MarlinConfig.h"

#define SAVE_INFO_INTERVAL_MS 0
//#define SAVE_EACH_CMD_MODE
//#define DEBUG_POWER_LOSS_RECOVERY

typedef struct {
  uint8_t valid_head;

  // Machine state
  float current_position[NUM_AXIS], feedrate;

  #if HOTENDS > 1
    uint8_t active_hotend;
  #endif

  int16_t target_temperature[HOTENDS];

  #if HAS_HEATED_BED
    int16_t target_temperature_bed;
  #endif

  #if FAN_COUNT
    int16_t fanSpeeds[FAN_COUNT];
  #endif

  #if HAS_LEVELING
    bool leveling;
    float fade;
  #endif

  // Command queue
  uint8_t cmd_queue_index_r, commands_in_queue;
  char command_queue[BUFSIZE][MAX_CMD_SIZE];

  // SD Filename and position
  char sd_filename[MAXPATHNAMELENGTH];
  uint32_t sdpos;

  // Job elapsed time
  millis_t print_job_elapsed;

  uint8_t valid_foot;
} job_recovery_info_t;

extern job_recovery_info_t job_recovery_info;

enum JobRecoveryPhase : unsigned char {
  JOB_RECOVERY_IDLE,
  JOB_RECOVERY_MAYBE,
  JOB_RECOVERY_YES,
  JOB_RECOVERY_DONE
};
extern JobRecoveryPhase job_recovery_phase;

#if HAS_LEVELING
  #define APPEND_CMD_COUNT 9
#else
  #define APPEND_CMD_COUNT 7
#endif

extern char job_recovery_commands[BUFSIZE + APPEND_CMD_COUNT][MAX_CMD_SIZE];
extern uint8_t job_recovery_commands_count;

void check_print_job_recovery();
void save_job_recovery_info();

#endif // _POWER_LOSS_RECOVERY_H_
