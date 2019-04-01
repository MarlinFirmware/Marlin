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

/**
 * power_loss_recovery.cpp - Resume an SD print after power-loss
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(POWER_LOSS_RECOVERY)

#include "power_loss_recovery.h"
#include "../core/macros.h"

bool PrintJobRecovery::enabled; // Initialized by settings.load()

SdFile PrintJobRecovery::file;
job_recovery_info_t PrintJobRecovery::info;

#include "../sd/cardreader.h"
#include "../lcd/ultralcd.h"
#include "../gcode/queue.h"
#include "../gcode/gcode.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../module/printcounter.h"
#include "../module/temperature.h"
#include "../core/serial.h"

#if ENABLED(FWRETRACT)
  #include "fwretract.h"
#endif

PrintJobRecovery recovery;

/**
 * Clear the recovery info
 */
void PrintJobRecovery::init() { memset(&info, 0, sizeof(info)); }

/**
 * Enable or disable then call changed()
 */
void PrintJobRecovery::enable(const bool onoff) {
  enabled = onoff;
  changed();
}

/**
 * The enabled state was changed:
 *  - Enabled: Purge the job recovery file
 *  - Disabled: Write the job recovery file
 */
void PrintJobRecovery::changed() {
  if (!enabled)
    purge();
  else if (IS_SD_PRINTING())
    save(true);
}

/**
 * Check for Print Job Recovery during setup()
 *
 * If a saved state exists send 'M1000 S' to initiate job recovery.
 */
void PrintJobRecovery::check() {
  if (enabled) {
    if (!card.isDetected()) card.initsd();
    if (card.isDetected()) {
      load();
      if (!valid()) return purge();
      enqueue_and_echo_commands_P(PSTR("M1000 S"));
    }
  }
}

/**
 * Delete the recovery file and clear the recovery data
 */
void PrintJobRecovery::purge() {
  init();
  card.removeJobRecoveryFile();
}

/**
 * Load the recovery data, if it exists
 */
void PrintJobRecovery::load() {
  if (exists()) {
    open(true);
    (void)file.read(&info, sizeof(info));
    close();
  }
  #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
    debug(PSTR("Load"));
  #endif
}

/**
 * Save the current machine state to the power-loss recovery file
 */
void PrintJobRecovery::save(const bool force/*=false*/, const bool save_queue/*=true*/) {

  #if SAVE_INFO_INTERVAL_MS > 0
    static millis_t next_save_ms; // = 0
    millis_t ms = millis();
  #endif

  // Did Z change since the last call?
  const float zmoved = current_position[Z_AXIS] - info.current_position[Z_AXIS];

  if (force
    #if DISABLED(SAVE_EACH_CMD_MODE)      // Always save state when enabled
      #if PIN_EXISTS(POWER_LOSS)          // Save if power loss pin is triggered
        || READ(POWER_LOSS_PIN) == POWER_LOSS_STATE
      #endif
      #if SAVE_INFO_INTERVAL_MS > 0       // Save if interval is elapsed
        || ELAPSED(ms, next_save_ms)
      #endif
      || zmoved > 0                       // Z moved up (including Z-hop)
      || zmoved < -5                      // Z moved down a lot (for some reason)
    #endif
  ) {

    #if SAVE_INFO_INTERVAL_MS > 0
      next_save_ms = ms + SAVE_INFO_INTERVAL_MS;
    #endif

    // Set Head and Foot to matching non-zero values
    if (!++info.valid_head) ++info.valid_head; // non-zero in sequence
    //if (!IS_SD_PRINTING()) info.valid_head = 0;
    info.valid_foot = info.valid_head;

    // Machine state
    COPY(info.current_position, current_position);
    info.feedrate = uint16_t(feedrate_mm_s * 60.0f);

    #if HOTENDS > 1
      info.active_hotend = active_extruder;
    #endif

    HOTEND_LOOP() info.target_temperature[e] = thermalManager.temp_hotend[e].target;

    #if HAS_HEATED_BED
      info.target_temperature_bed = thermalManager.temp_bed.target;
    #endif

    #if FAN_COUNT
      COPY(info.fan_speed, thermalManager.fan_speed);
    #endif

    #if HAS_LEVELING
      info.leveling = planner.leveling_active;
      info.fade = (
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          planner.z_fade_height
        #else
          0
        #endif
      );
    #endif

    #if ENABLED(GRADIENT_MIX)
      memcpy(&info.gradient, &mixer.gradient, sizeof(info.gradient));
    #endif

    #if ENABLED(FWRETRACT)
      COPY(info.retract, fwretract.current_retract);
      info.retract_hop = fwretract.current_hop;
    #endif

    //relative mode
    info.relative_mode = relative_mode;
    info.relative_modes_e = gcode.axis_relative_modes[E_AXIS];

    // Commands in the queue
    info.commands_in_queue = save_queue ? commands_in_queue : 0;
    info.cmd_queue_index_r = cmd_queue_index_r;
    COPY(info.command_queue, command_queue);

    // Elapsed print job time
    info.print_job_elapsed = print_job_timer.duration();

    // SD file position
    card.getAbsFilename(info.sd_filename);
    info.sdpos = card.getIndex();

    write();

    // KILL now if the power-loss pin was triggered
    #if PIN_EXISTS(POWER_LOSS)
      if (READ(POWER_LOSS_PIN) == POWER_LOSS_STATE) kill(PSTR(MSG_OUTAGE_RECOVERY));
    #endif
  }
}

/**
 * Save the recovery info the recovery file
 */
void PrintJobRecovery::write() {

  #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
    debug(PSTR("Write"));
  #endif

  open(false);
  file.seekSet(0);
  const int16_t ret = file.write(&info, sizeof(info));
  close();

  #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
    if (ret == -1) SERIAL_ECHOLNPGM("Power-loss file write failed.");
  #else
    UNUSED(ret);
  #endif
}

/**
 * Resume the saved print job
 */
void PrintJobRecovery::resume() {

  #define RECOVERY_ZRAISE 2

  #if HAS_LEVELING
    // Make sure leveling is off before any G92 and G28
    gcode.process_subcommands_now_P(PSTR("M420 S0 Z0"));
  #endif

  // Set Z to 0, raise Z by 2mm, and Home (XY only for Cartesian) with no raise
  // (Only do simulated homing in Marlin Dev Mode.)
  gcode.process_subcommands_now_P(PSTR("G92.0 Z0\nG1 Z" STRINGIFY(RECOVERY_ZRAISE) "\nG28 R0"
    #if ENABLED(MARLIN_DEV_MODE)
      " S"
    #elif !IS_KINEMATIC
      " X Y"
    #endif
  ));

  // Pretend that all axes are homed
  axis_homed = axis_known_position = xyz_bits;

  char cmd[40], str_1[16], str_2[16];

  // Select the previously active tool (with no_move)
  #if EXTRUDERS > 1
    sprintf_P(cmd, PSTR("T%i S"), info.active_hotend);
    gcode.process_subcommands_now(cmd);
  #endif

  #if HAS_HEATED_BED
    const int16_t bt = info.target_temperature_bed;
    if (bt) {
      // Restore the bed temperature
      sprintf_P(cmd, PSTR("M190 S%i"), bt);
      gcode.process_subcommands_now(cmd);
    }
  #endif

  // Restore all hotend temperatures
  HOTEND_LOOP() {
    const int16_t et = info.target_temperature[e];
    if (et) {
      #if HOTENDS > 1
        sprintf_P(cmd, PSTR("T%i"), e);
        gcode.process_subcommands_now(cmd);
      #endif
      sprintf_P(cmd, PSTR("M109 S%i"), et);
      gcode.process_subcommands_now(cmd);
    }
  }

  // Restore print cooling fan speeds
  FANS_LOOP(i) {
    uint8_t f = info.fan_speed[i];
    if (f) {
      sprintf_P(cmd, PSTR("M106 P%i S%i"), i, f);
      gcode.process_subcommands_now(cmd);
    }
  }

  // Restore retract and hop state
  #if ENABLED(FWRETRACT)
    for (uint8_t e = 0; e < EXTRUDERS; e++) {
      if (info.retract[e] != 0.0)
        fwretract.current_retract[e] = info.retract[e];
        fwretract.retracted[e] = true;
    }
    fwretract.current_hop = info.retract_hop;
  #endif

  #if HAS_LEVELING
    // Restore leveling state before 'G92 Z' to ensure
    // the Z stepper count corresponds to the native Z.
    if (info.fade || info.leveling) {
      dtostrf(info.fade, 1, 1, str_1);
      sprintf_P(cmd, PSTR("M420 S%i Z%s"), int(info.leveling), str_1);
      gcode.process_subcommands_now(cmd);
    }
  #endif

  #if ENABLED(GRADIENT_MIX)
    memcpy(&mixer.gradient, &info.gradient, sizeof(info.gradient));
  #endif

  // Restore Z (plus raise) and E positions with G92.0
  dtostrf(info.current_position[Z_AXIS] + RECOVERY_ZRAISE, 1, 3, str_1);
  dtostrf(info.current_position[E_AXIS]
    #if ENABLED(SAVE_EACH_CMD_MODE)
      - 5 // Extra extrusion on restart
    #endif
    , 1, 3, str_2
  );
  sprintf_P(cmd, PSTR("G92.0 Z%s E%s"), str_1, str_2);
  gcode.process_subcommands_now(cmd);

  // Move back to the saved XY
  dtostrf(info.current_position[X_AXIS], 1, 3, str_1);
  dtostrf(info.current_position[Y_AXIS], 1, 3, str_2);
  sprintf_P(cmd, PSTR("G1 X%s Y%s F3000"), str_1, str_2);
  gcode.process_subcommands_now(cmd);

  // Move back to the saved Z
  dtostrf(info.current_position[Z_AXIS], 1, 3, str_1);
  sprintf_P(cmd, PSTR("G1 Z%s F200"), str_1);
  gcode.process_subcommands_now(cmd);

  // Restore the feedrate
  sprintf_P(cmd, PSTR("G1 F%d"), info.feedrate);
  gcode.process_subcommands_now(cmd);

  //relative mode
  if (info.relative_mode) relative_mode = true;
  if (info.relative_modes_e) gcode.axis_relative_modes[E_AXIS] = true;

  // Process commands from the old pending queue
  uint8_t c = info.commands_in_queue, r = info.cmd_queue_index_r;
  for (; c--; r = (r + 1) % BUFSIZE)
    gcode.process_subcommands_now(info.command_queue[r]);

  // Resume the SD file from the last position
  char *fn = info.sd_filename;
  while (*fn == '/') fn++;
  sprintf_P(cmd, PSTR("M23 %s"), fn);
  gcode.process_subcommands_now(cmd);
  sprintf_P(cmd, PSTR("M24 S%ld T%ld"), info.sdpos, info.print_job_elapsed);
  gcode.process_subcommands_now(cmd);
}

#if ENABLED(DEBUG_POWER_LOSS_RECOVERY)

  void PrintJobRecovery::debug(PGM_P const prefix) {
    serialprintPGM(prefix);
    SERIAL_ECHOLNPAIR(" Job Recovery Info...\nvalid_head:", int(info.valid_head), " valid_foot:", int(info.valid_foot));
    if (info.valid_head) {
      if (info.valid_head == info.valid_foot) {
        SERIAL_ECHOPGM("current_position: ");
        LOOP_XYZE(i) {
          SERIAL_ECHO(info.current_position[i]);
          if (i < E_AXIS) SERIAL_CHAR(',');
        }
        SERIAL_EOL();
        SERIAL_ECHOLNPAIR("feedrate: ", info.feedrate);

        #if HOTENDS > 1
          SERIAL_ECHOLNPAIR("active_hotend: ", int(info.active_hotend));
        #endif

        SERIAL_ECHOPGM("target_temperature: ");
        HOTEND_LOOP() {
          SERIAL_ECHO(info.target_temperature[e]);
          if (e < HOTENDS - 1) SERIAL_CHAR(',');
        }
        SERIAL_EOL();

        #if HAS_HEATED_BED
          SERIAL_ECHOLNPAIR("target_temperature_bed: ", info.target_temperature_bed);
        #endif

        #if FAN_COUNT
          SERIAL_ECHOPGM("fan_speed: ");
          FANS_LOOP(i) {
            SERIAL_ECHO(int(info.fan_speed[i]));
            if (i < FAN_COUNT - 1) SERIAL_CHAR(',');
          }
          SERIAL_EOL();
        #endif

        #if HAS_LEVELING
          SERIAL_ECHOLNPAIR("leveling: ", int(info.leveling), "\n fade: ", int(info.fade));
        #endif
        #if ENABLED(FWRETRACT)
          SERIAL_ECHOPGM("retract: ");
          for (int8_t e = 0; e < EXTRUDERS; e++) {
            SERIAL_ECHO(info.retract[e]);
            if (e < EXTRUDERS - 1) SERIAL_CHAR(',');
          }
          SERIAL_EOL();
          SERIAL_ECHOLNPAIR("retract_hop: ", info.retract_hop);
        #endif
        SERIAL_ECHOLNPAIR("cmd_queue_index_r: ", int(info.cmd_queue_index_r));
        SERIAL_ECHOLNPAIR("commands_in_queue: ", int(info.commands_in_queue));
        for (uint8_t i = 0; i < info.commands_in_queue; i++) SERIAL_ECHOLNPAIR("> ", info.command_queue[i]);
        SERIAL_ECHOLNPAIR("sd_filename: ", info.sd_filename);
        SERIAL_ECHOLNPAIR("sdpos: ", info.sdpos);
        SERIAL_ECHOLNPAIR("print_job_elapsed: ", info.print_job_elapsed);
      }
      else
        SERIAL_ECHOLNPGM("INVALID DATA");
    }
    SERIAL_ECHOLNPGM("---");
  }

#endif // DEBUG_POWER_LOSS_RECOVERY

#endif // POWER_LOSS_RECOVERY
