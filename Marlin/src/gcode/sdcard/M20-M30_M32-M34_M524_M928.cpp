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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "../gcode.h"
#include "../../sd/cardreader.h"
#include "../../module/printcounter.h"
#include "../../module/stepper.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/power_loss_recovery.h"
#endif

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../feature/pause.h"
#endif

#if ENABLED(PARK_HEAD_ON_PAUSE) || NUM_SERIAL > 1
  #include "../queue.h"
#endif

/**
 * M20: List SD card to serial output
 */
void GcodeSuite::M20() {
  #if NUM_SERIAL > 1
    const int16_t port = command_queue_port[cmd_queue_index_r];
  #endif

  SERIAL_PROTOCOLLNPGM_P(port, MSG_BEGIN_FILE_LIST);
  card.ls(
    #if NUM_SERIAL > 1
      port
    #endif
  );
  SERIAL_PROTOCOLLNPGM_P(port, MSG_END_FILE_LIST);
}

/**
 * M21: Init SD Card
 */
void GcodeSuite::M21() { card.initsd(); }

/**
 * M22: Release SD Card
 */
void GcodeSuite::M22() { card.release(); }

/**
 * M23: Open a file
 */
void GcodeSuite::M23() {
  #if ENABLED(POWER_LOSS_RECOVERY)
    card.removeJobRecoveryFile();
  #endif
  // Simplify3D includes the size, so zero out all spaces (#7227)
  for (char *fn = parser.string_arg; *fn; ++fn) if (*fn == ' ') *fn = '\0';
  card.openFile(parser.string_arg, true);
}

/**
 * M24: Start or Resume SD Print
 */
void GcodeSuite::M24() {
  #if ENABLED(PARK_HEAD_ON_PAUSE)
    resume_print();
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (parser.seenval('S')) card.setIndex(parser.value_long());
  #endif

  card.startFileprint();

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (parser.seenval('T'))
      print_job_timer.resume(parser.value_long());
    else
  #endif
      print_job_timer.start();
}

/**
 * M25: Pause SD Print
 */
void GcodeSuite::M25() {
  card.pauseSDPrint();
  print_job_timer.pause();

  #if ENABLED(PARK_HEAD_ON_PAUSE)
    enqueue_and_echo_commands_P(PSTR("M125")); // Must be enqueued with pauseSDPrint set to be last in the buffer
  #endif
}

/**
 * M26: Set SD Card file index
 */
void GcodeSuite::M26() {
  if (card.cardOK && parser.seenval('S'))
    card.setIndex(parser.value_long());
}

/**
 * M27: Get SD Card status
 *      OR, with 'S<seconds>' set the SD status auto-report interval. (Requires AUTO_REPORT_SD_STATUS)
 *      OR, with 'C' get the current filename.
 */
void GcodeSuite::M27() {
  #if NUM_SERIAL > 1
    const int16_t port = command_queue_port[cmd_queue_index_r];
  #endif

  if (parser.seen('C')) {
    SERIAL_ECHOPGM_P(port, "Current file: ");
    card.printFilename();
  }

  #if ENABLED(AUTO_REPORT_SD_STATUS)
    else if (parser.seenval('S'))
      card.set_auto_report_interval(parser.value_byte()
        #if NUM_SERIAL > 1
          , port
        #endif
      );
  #endif

  else
    card.getStatus(
      #if NUM_SERIAL > 1
        port
      #endif
    );
}

/**
 * M28: Start SD Write
 */
void GcodeSuite::M28() { card.openFile(parser.string_arg, false); }

/**
 * M29: Stop SD Write
 * Processed in write to file routine
 */
void GcodeSuite::M29() {
  // card.saving = false;
}

/**
 * M30 <filename>: Delete SD Card file
 */
void GcodeSuite::M30() {
  if (card.cardOK) {
    card.closefile();
    card.removeFile(parser.string_arg);
  }
}

/**
 * M32: Select file and start SD Print
 *
 * Examples:
 *
 *    M32 !PATH/TO/FILE.GCO#      ; Start FILE.GCO
 *    M32 P !PATH/TO/FILE.GCO#    ; Start FILE.GCO as a procedure
 *    M32 S60 !PATH/TO/FILE.GCO#  ; Start FILE.GCO at byte 60
 *
 */
void GcodeSuite::M32() {
  if (card.sdprinting) planner.synchronize();

  if (card.cardOK) {
    const bool call_procedure = parser.boolval('P');

    card.openFile(parser.string_arg, true, call_procedure);

    if (parser.seenval('S')) card.setIndex(parser.value_long());

    card.startFileprint();

    // Procedure calls count as normal print time.
    if (!call_procedure) print_job_timer.start();
  }
}

#if ENABLED(LONG_FILENAME_HOST_SUPPORT)

  /**
   * M33: Get the long full path of a file or folder
   *
   * Parameters:
   *   <dospath> Case-insensitive DOS-style path to a file or folder
   *
   * Example:
   *   M33 miscel~1/armchair/armcha~1.gco
   *
   * Output:
   *   /Miscellaneous/Armchair/Armchair.gcode
   */
  void GcodeSuite::M33() {
    card.printLongPath(parser.string_arg
      #if NUM_SERIAL > 1
        , command_queue_port[cmd_queue_index_r]
      #endif
    );
  }

#endif // LONG_FILENAME_HOST_SUPPORT

#if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)

  /**
   * M34: Set SD Card Sorting Options
   */
  void GcodeSuite::M34() {
    if (parser.seen('S')) card.setSortOn(parser.value_bool());
    if (parser.seenval('F')) {
      const int v = parser.value_long();
      card.setSortFolders(v < 0 ? -1 : v > 0 ? 1 : 0);
    }
    //if (parser.seen('R')) card.setSortReverse(parser.value_bool());
  }

#endif // SDCARD_SORT_ALPHA && SDSORT_GCODE

/**
 * M524: Abort the current SD print job (started with M24)
 */
void GcodeSuite::M524() {
  if (IS_SD_PRINTING()) card.abort_sd_printing = true;
}

/**
 * M928: Start SD Write
 */
void GcodeSuite::M928() {
  card.openLogFile(parser.string_arg);
}

#endif // SDSUPPORT
