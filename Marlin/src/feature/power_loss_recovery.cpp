/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
const char PrintJobRecovery::filename[5] = "/PLR";
uint8_t PrintJobRecovery::queue_index_r;
uint32_t PrintJobRecovery::cmd_sdpos, // = 0
         PrintJobRecovery::sdpos[BUFSIZE];

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

#define DEBUG_OUT ENABLED(DEBUG_POWER_LOSS_RECOVERY)
#include "../core/debug_out.h"

PrintJobRecovery recovery;

#ifndef POWER_LOSS_PURGE_LEN
  #define POWER_LOSS_PURGE_LEN 0
#endif
#ifndef POWER_LOSS_RETRACT_LEN
  #define POWER_LOSS_RETRACT_LEN 0
#endif
#ifndef POWER_LOSS_ZRAISE
  #define POWER_LOSS_ZRAISE 2
#endif

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
    if (!card.isMounted()) card.mount();
    if (card.isMounted()) {
      load();
      if (!valid()) return purge();
      queue.inject_P(PSTR("M1000 S"));
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
  debug(PSTR("Load"));
}

/**
 * Set info fields that won't change
 */
void PrintJobRecovery::prepare() {
  card.getAbsFilename(info.sd_filename);  // SD filename
  cmd_sdpos = 0;
}

/**
 * Save the current machine state to the power-loss recovery file
 */
void PrintJobRecovery::save(const bool force/*=false*/) {

  #if SAVE_INFO_INTERVAL_MS > 0
    static millis_t next_save_ms; // = 0
    millis_t ms = millis();
  #endif

  #ifndef POWER_LOSS_MIN_Z_CHANGE
    #define POWER_LOSS_MIN_Z_CHANGE 0.05  // Vase-mode-friendly out of the box
  #endif

  // Did Z change since the last call?
  if (force
    #if DISABLED(SAVE_EACH_CMD_MODE)      // Always save state when enabled
      #if SAVE_INFO_INTERVAL_MS > 0       // Save if interval is elapsed
        || ELAPSED(ms, next_save_ms)
      #endif
      // Save if Z is above the last-saved position by some minimum height
      || current_position.z > info.current_position.z + POWER_LOSS_MIN_Z_CHANGE
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
    info.current_position = current_position;
    #if HAS_HOME_OFFSET
      info.home_offset = home_offset;
    #endif
    #if HAS_POSITION_SHIFT
      info.position_shift = position_shift;
    #endif
    info.feedrate = uint16_t(feedrate_mm_s * 60.0f);

    #if EXTRUDERS > 1
      info.active_extruder = active_extruder;
    #endif

    #if DISABLED(NO_VOLUMETRICS)
      info.volumetric_enabled = parser.volumetric_enabled;
      #if EXTRUDERS > 1
        for (int8_t e = 0; e < EXTRUDERS; e++) info.filament_size[e] = planner.filament_size[e];
      #else
        if (parser.volumetric_enabled) info.filament_size = planner.filament_size[active_extruder];
      #endif
    #endif

    #if EXTRUDERS
      HOTEND_LOOP() info.target_temperature[e] = thermalManager.temp_hotend[e].target;
    #endif

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

    // Relative axis modes
    info.axis_relative = gcode.axis_relative;

    // Elapsed print job time
    info.print_job_elapsed = print_job_timer.duration();

    write();
  }
}

#if PIN_EXISTS(POWER_LOSS)

  void PrintJobRecovery::_outage() {
    #if ENABLED(BACKUP_POWER_SUPPLY)
      static bool lock = false;
      if (lock) return; // No re-entrance from idle() during raise_z()
      lock = true;
    #endif
    if (IS_SD_PRINTING()) save(true);
    #if ENABLED(BACKUP_POWER_SUPPLY)
      raise_z();
    #endif

    kill(GET_TEXT(MSG_OUTAGE_RECOVERY));
  }

  #if ENABLED(BACKUP_POWER_SUPPLY)

    void PrintJobRecovery::raise_z() {
      // Disable all heaters to reduce power loss
      thermalManager.disable_all_heaters();
      quickstop_stepper();
      // Raise Z axis
      gcode.process_subcommands_now_P(PSTR("G91\nG0 Z" STRINGIFY(POWER_LOSS_ZRAISE)));
      planner.synchronize();
    }

  #endif

#endif

/**
 * Save the recovery info the recovery file
 */
void PrintJobRecovery::write() {

  debug(PSTR("Write"));

  open(false);
  file.seekSet(0);
  const int16_t ret = file.write(&info, sizeof(info));
  if (ret == -1) DEBUG_ECHOLNPGM("Power-loss file write failed.");
  if (!file.close()) DEBUG_ECHOLNPGM("Power-loss file close failed.");
}

/**
 * Resume the saved print job
 */
void PrintJobRecovery::resume() {

  const uint32_t resume_sdpos = info.sdpos; // Get here before the stepper ISR overwrites it

  #if HAS_LEVELING
    // Make sure leveling is off before any G92 and G28
    gcode.process_subcommands_now_P(PSTR("M420 S0 Z0"));
  #endif

  // Reset E, raise Z, home XY...
  gcode.process_subcommands_now_P(PSTR("G92.9 E0"
    #if Z_HOME_DIR > 0

      // If Z homing goes to max, just reset E and home all
      "\n"
      "G28R0"
      #if ENABLED(MARLIN_DEV_MODE)
        "S"
      #endif

    #else // "G92.9 E0 ..."

      // Set Z to 0, raise Z by RECOVERY_ZRAISE, and Home (XY only for Cartesian)
      // with no raise. (Only do simulated homing in Marlin Dev Mode.)
      #if ENABLED(BACKUP_POWER_SUPPLY)
        "Z" STRINGIFY(POWER_LOSS_ZRAISE)    // Z-axis was already raised at outage
      #else
        "Z0\n"                              // Set Z=0
        "G1Z" STRINGIFY(POWER_LOSS_ZRAISE)  // Raise Z
      #endif
      "\n"

      "G28R0"
      #if ENABLED(MARLIN_DEV_MODE)
        "S"
      #elif !IS_KINEMATIC
        "XY"
      #endif
    #endif
  ));

  // Pretend that all axes are homed
  axis_homed = axis_known_position = xyz_bits;

  char cmd[MAX_CMD_SIZE+16], str_1[16], str_2[16];

  // Select the previously active tool (with no_move)
  #if EXTRUDERS > 1
    sprintf_P(cmd, PSTR("T%i S"), info.active_extruder);
    gcode.process_subcommands_now(cmd);
  #endif

  // Recover volumetric extrusion state
  #if DISABLED(NO_VOLUMETRICS)
    #if EXTRUDERS > 1
      for (int8_t e = 0; e < EXTRUDERS; e++) {
        dtostrf(info.filament_size[e], 1, 3, str_1);
        sprintf_P(cmd, PSTR("M200 T%i D%s"), e, str_1);
        gcode.process_subcommands_now(cmd);
      }
      if (!info.volumetric_enabled) {
        sprintf_P(cmd, PSTR("M200 T%i D0"), info.active_extruder);
        gcode.process_subcommands_now(cmd);
      }
    #else
      if (info.volumetric_enabled) {
        dtostrf(info.filament_size, 1, 3, str_1);
        sprintf_P(cmd, PSTR("M200 D%s"), str_1);
        gcode.process_subcommands_now(cmd);
      }
    #endif
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
  #if HOTENDS
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
  #endif

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
      if (info.retract[e] != 0.0) {
        fwretract.current_retract[e] = info.retract[e];
        fwretract.retracted[e] = true;
      }
    }
    fwretract.current_hop = info.retract_hop;
  #endif

  #if HAS_LEVELING
    // Restore leveling state before 'G92 Z' to ensure
    // the Z stepper count corresponds to the native Z.
    if (info.fade || info.leveling) {
      sprintf_P(cmd, PSTR("M420 S%i Z%s"), int(info.leveling), dtostrf(info.fade, 1, 1, str_1));
      gcode.process_subcommands_now(cmd);
    }
  #endif

  #if ENABLED(GRADIENT_MIX)
    memcpy(&mixer.gradient, &info.gradient, sizeof(info.gradient));
  #endif

  // Extrude and retract to clean the nozzle
  #if POWER_LOSS_PURGE_LEN
    //sprintf_P(cmd, PSTR("G1 E%d F200"), POWER_LOSS_PURGE_LEN);
    //gcode.process_subcommands_now(cmd);
    gcode.process_subcommands_now_P(PSTR("G1 E" STRINGIFY(POWER_LOSS_PURGE_LEN) " F200"));
  #endif

  #if POWER_LOSS_RETRACT_LEN
    sprintf_P(cmd, PSTR("G1 E%d F3000"), POWER_LOSS_PURGE_LEN - (POWER_LOSS_RETRACT_LEN));
    gcode.process_subcommands_now(cmd);
  #endif

  // Move back to the saved XY
  sprintf_P(cmd, PSTR("G1 X%s Y%s F3000"),
    dtostrf(info.current_position.x, 1, 3, str_1),
    dtostrf(info.current_position.y, 1, 3, str_2)
  );
  gcode.process_subcommands_now(cmd);

  // Move back to the saved Z
  dtostrf(info.current_position.z, 1, 3, str_1);
  #if Z_HOME_DIR > 0
    sprintf_P(cmd, PSTR("G1 Z%s F200"), str_1);
  #else
    gcode.process_subcommands_now_P(PSTR("G1 Z0 F200"));
    sprintf_P(cmd, PSTR("G92.9 Z%s"), str_1);
  #endif
  gcode.process_subcommands_now(cmd);

  // Un-retract
  #if POWER_LOSS_PURGE_LEN
    //sprintf_P(cmd, PSTR("G1 E%d F3000"), POWER_LOSS_PURGE_LEN);
    //gcode.process_subcommands_now(cmd);
    gcode.process_subcommands_now_P(PSTR("G1 E" STRINGIFY(POWER_LOSS_PURGE_LEN) " F3000"));
  #endif

  // Restore the feedrate
  sprintf_P(cmd, PSTR("G1 F%d"), info.feedrate);
  gcode.process_subcommands_now(cmd);

  // Restore E position with G92.9
  sprintf_P(cmd, PSTR("G92.9 E%s"), dtostrf(info.current_position.e, 1, 3, str_1));
  gcode.process_subcommands_now(cmd);

  // Relative axis modes
  gcode.axis_relative = info.axis_relative;

  #if HAS_HOME_OFFSET
    home_offset = info.home_offset;
  #endif
  #if HAS_POSITION_SHIFT
    position_shift = info.position_shift;
  #endif
  #if HAS_HOME_OFFSET || HAS_POSITION_SHIFT
    LOOP_XYZ(i) update_workspace_offset((AxisEnum)i);
  #endif

  // Resume the SD file from the last position
  char *fn = info.sd_filename;
  extern const char M23_STR[];
  sprintf_P(cmd, M23_STR, fn);
  gcode.process_subcommands_now(cmd);
  sprintf_P(cmd, PSTR("M24 S%ld T%ld"), resume_sdpos, info.print_job_elapsed);
  gcode.process_subcommands_now(cmd);
}

#if ENABLED(DEBUG_POWER_LOSS_RECOVERY)

  void PrintJobRecovery::debug(PGM_P const prefix) {
    DEBUG_PRINT_P(prefix);
    DEBUG_ECHOLNPAIR(" Job Recovery Info...\nvalid_head:", int(info.valid_head), " valid_foot:", int(info.valid_foot));
    if (info.valid_head) {
      if (info.valid_head == info.valid_foot) {
        DEBUG_ECHOPGM("current_position: ");
        LOOP_XYZE(i) {
          if (i) DEBUG_CHAR(',');
          DEBUG_ECHO(info.current_position[i]);
        }
        DEBUG_EOL();

        #if HAS_HOME_OFFSET
          DEBUG_ECHOPGM("home_offset: ");
          LOOP_XYZ(i) {
            if (i) DEBUG_CHAR(',');
            DEBUG_ECHO(info.home_offset[i]);
          }
          DEBUG_EOL();
        #endif

        #if HAS_POSITION_SHIFT
          DEBUG_ECHOPGM("position_shift: ");
          LOOP_XYZ(i) {
            if (i) DEBUG_CHAR(',');
            DEBUG_ECHO(info.position_shift[i]);
          }
          DEBUG_EOL();
        #endif

        DEBUG_ECHOLNPAIR("feedrate: ", info.feedrate);

        #if EXTRUDERS > 1
          DEBUG_ECHOLNPAIR("active_extruder: ", int(info.active_extruder));
        #endif

        #if HOTENDS
          DEBUG_ECHOPGM("target_temperature: ");
          HOTEND_LOOP() {
            DEBUG_ECHO(info.target_temperature[e]);
            if (e < HOTENDS - 1) DEBUG_CHAR(',');
          }
          DEBUG_EOL();
        #endif

        #if HAS_HEATED_BED
          DEBUG_ECHOLNPAIR("target_temperature_bed: ", info.target_temperature_bed);
        #endif

        #if FAN_COUNT
          DEBUG_ECHOPGM("fan_speed: ");
          FANS_LOOP(i) {
            DEBUG_ECHO(int(info.fan_speed[i]));
            if (i < FAN_COUNT - 1) DEBUG_CHAR(',');
          }
          DEBUG_EOL();
        #endif

        #if HAS_LEVELING
          DEBUG_ECHOLNPAIR("leveling: ", int(info.leveling), "\n fade: ", int(info.fade));
        #endif
        #if ENABLED(FWRETRACT)
          DEBUG_ECHOPGM("retract: ");
          for (int8_t e = 0; e < EXTRUDERS; e++) {
            DEBUG_ECHO(info.retract[e]);
            if (e < EXTRUDERS - 1) DEBUG_CHAR(',');
          }
          DEBUG_EOL();
          DEBUG_ECHOLNPAIR("retract_hop: ", info.retract_hop);
        #endif
        DEBUG_ECHOLNPAIR("sd_filename: ", info.sd_filename);
        DEBUG_ECHOLNPAIR("sdpos: ", info.sdpos);
        DEBUG_ECHOLNPAIR("print_job_elapsed: ", info.print_job_elapsed);
      }
      else
        DEBUG_ECHOLNPGM("INVALID DATA");
    }
    DEBUG_ECHOLNPGM("---");
  }

#endif // DEBUG_POWER_LOSS_RECOVERY

#endif // POWER_LOSS_RECOVERY
