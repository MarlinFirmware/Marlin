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
#pragma once

/**
 * queue.h - The G-code command queue, which holds commands before they
 *           go to the parser and dispatcher.
 */

#include "../inc/MarlinConfig.h"

class GCodeQueue {
public:
  /**
   * GCode line number handling. Hosts may include line numbers when sending
   * commands to Marlin, and lines will be checked for sequentiality.
   * M110 N<int> sets the current line number.
   */

  static long last_N[NUM_SERIAL];

  /**
   * GCode Command Queue
   * A simple ring buffer of BUFSIZE command strings.
   *
   * Commands are copied into this buffer by the command injectors
   * (immediate, serial, sd card) and they are processed sequentially by
   * the main loop. The gcode.process_next_command method parses the next
   * command and hands off execution to individual handler functions.
   */
  static uint8_t length,  // Count of commands in the queue
                 index_r; // Ring buffer read position

  static char command_buffer[BUFSIZE][MAX_CMD_SIZE];

  /**
   * The port that the command was received on
   */
  #if HAS_MULTI_SERIAL
    static int16_t port[BUFSIZE];
  #endif

  static int16_t command_port() {
    return TERN0(HAS_MULTI_SERIAL, port[index_r]);
  }

  GCodeQueue();

  /**
   * Clear the Marlin command queue
   */
  static void clear();

  /**
   * Next Injected Command (PROGMEM) pointer. (nullptr == empty)
   * Internal commands are enqueued ahead of serial / SD commands.
   */
  static PGM_P injected_commands_P;

  /**
   * Injected Commands (SRAM)
   */
  static char injected_commands[64];

  /**
   * Enqueue command(s) to run from PROGMEM. Drained by process_injected_command_P().
   * Don't inject comments or use leading spaces!
   * Aborts the current PROGMEM queue so only use for one or two commands.
   */
  static inline void inject_P(PGM_P const pgcode) { injected_commands_P = pgcode; }

  /**
   * Enqueue command(s) to run from SRAM. Drained by process_injected_command().
   * Aborts the current SRAM queue so only use for one or two commands.
   */
  static inline void inject(char * const gcode) {
    strncpy(injected_commands, gcode, sizeof(injected_commands) - 1);
  }

  /**
   * Enqueue and return only when commands are actually enqueued
   */
  static void enqueue_one_now(const char* cmd);

  /**
   * Attempt to enqueue a single G-code command
   * and return 'true' if successful.
   */
  static bool enqueue_one_P(PGM_P const pgcode);

  /**
   * Enqueue from program memory and return only when commands are actually enqueued
   */
  static void enqueue_now_P(PGM_P const cmd);

  /**
   * Check whether there are any commands yet to be executed
   */
  static bool has_commands_queued();

  /**
   * Get the next command in the queue, optionally log it to SD, then dispatch it
   */
  static void advance();

  /**
   * Add to the circular command queue the next command from:
   *  - The command-injection queue (injected_commands_P)
   *  - The active serial input (usually USB)
   *  - The SD card file being actively printed
   */
  static void get_available_commands();

  /**
   * Send an "ok" message to the host, indicating
   * that a command was successfully processed.
   *
   * If ADVANCED_OK is enabled also include:
   *   N<int>  Line number of the command, if any
   *   P<int>  Planner space remaining
   *   B<int>  Block queue space remaining
   */
  static void ok_to_send();

  /**
   * Clear the serial line and request a resend of
   * the next expected line number.
   */
  static void flush_and_request_resend();

private:

  static uint8_t index_w;  // Ring buffer write position

  static void get_serial_commands();

  #if ENABLED(SDSUPPORT)
    static void get_sdcard_commands();
  #endif

  static void _commit_command(bool say_ok
    #if HAS_MULTI_SERIAL
      , int16_t p=-1
    #endif
  );

  static bool _enqueue(const char* cmd, bool say_ok=false
    #if HAS_MULTI_SERIAL
      , int16_t p=-1
    #endif
  );

  // Process the next "immediate" command (PROGMEM)
  static bool process_injected_command_P();

  // Process the next "immediate" command (SRAM)
  static bool process_injected_command();

  /**
   * Enqueue with Serial Echo
   * Return true on success
   */
  static bool enqueue_one(const char* cmd);

  static void gcode_line_error(PGM_P const err, const int8_t pn);

};

extern GCodeQueue queue;
