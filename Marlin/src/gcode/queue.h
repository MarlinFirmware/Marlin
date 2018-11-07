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
#pragma once

/**
 * queue.h - The G-code command queue, which holds commands before they
 *           go to the parser and dispatcher.
 */

#include "../inc/MarlinConfig.h"

/**
 * GCode line number handling. Hosts may include line numbers when sending
 * commands to Marlin, and lines will be checked for sequentiality.
 * M110 N<int> sets the current line number.
 */
extern long gcode_LastN, Stopped_gcode_LastN;

/**
 * GCode Command Queue
 * A simple ring buffer of BUFSIZE command strings.
 *
 * Commands are copied into this buffer by the command injectors
 * (immediate, serial, sd card) and they are processed sequentially by
 * the main loop. The gcode.process_next_command method parses the next
 * command and hands off execution to individual handler functions.
 */
extern uint8_t commands_in_queue, // Count of commands in the queue
               cmd_queue_index_r; // Ring buffer read position

extern char command_queue[BUFSIZE][MAX_CMD_SIZE];

/*
 * The port that the command was received on
 */
#if NUM_SERIAL > 1
  extern int16_t command_queue_port[BUFSIZE];
#endif

/**
 * Initialization of queue for setup()
 */
void queue_setup();

/**
 * Clear the Marlin command queue
 */
void clear_command_queue();

/**
 * Clear the serial line and request a resend of
 * the next expected line number.
 */
void flush_and_request_resend();

/**
 * Send an "ok" message to the host, indicating
 * that a command was successfully processed.
 *
 * If ADVANCED_OK is enabled also include:
 *   N<int>  Line number of the command, if any
 *   P<int>  Planner space remaining
 *   B<int>  Block queue space remaining
 */
void ok_to_send();

/**
 * Record one or many commands to run from program memory.
 * Aborts the current queue, if any.
 * Note: drain_injected_commands_P() must be called repeatedly to drain the commands afterwards
 */
void enqueue_and_echo_commands_P(PGM_P const pgcode);

/**
 * Enqueue with Serial Echo
 */
bool enqueue_and_echo_command(const char* cmd);

#define HAS_LCD_QUEUE_NOW (ENABLED(MALYAN_LCD) || (HAS_LCD_MENU && (ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(PID_AUTOTUNE_MENU) || ENABLED(ADVANCED_PAUSE_FEATURE))))
#define HAS_QUEUE_NOW (ENABLED(SDSUPPORT) || HAS_LCD_QUEUE_NOW)

#if HAS_QUEUE_NOW
  /**
   * Enqueue and return only when commands are actually enqueued
   */
  void enqueue_and_echo_command_now(const char* cmd);
  #if HAS_LCD_QUEUE_NOW
    /**
     * Enqueue from program memory and return only when commands are actually enqueued
     */
    void enqueue_and_echo_commands_now_P(PGM_P const cmd);
  #endif
#endif

/**
 * Add to the circular command queue the next command from:
 *  - The command-injection queue (injected_commands_P)
 *  - The active serial input (usually USB)
 *  - The SD card file being actively printed
 */
void get_available_commands();

/**
 * Get the next command in the queue, optionally log it to SD, then dispatch it
 */
void advance_command_queue();
