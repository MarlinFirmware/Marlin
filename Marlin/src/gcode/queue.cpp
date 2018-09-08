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
 * queue.cpp - The G-code command queue
 */

#include "queue.h"
#include "gcode.h"

#include "../lcd/ultralcd.h"
#include "../sd/cardreader.h"
#include "../module/planner.h"
#include "../module/temperature.h"
#include "../Marlin.h"

#if HAS_COLOR_LEDS
  #include "../feature/leds/leds.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/power_loss_recovery.h"
#endif

/**
 * GCode line number handling. Hosts may opt to include line numbers when
 * sending commands to Marlin, and lines will be checked for sequentiality.
 * M110 N<int> sets the current line number.
 */
long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

/**
 * GCode Command Queue
 * A simple ring buffer of BUFSIZE command strings.
 *
 * Commands are copied into this buffer by the command injectors
 * (immediate, serial, sd card) and they are processed sequentially by
 * the main loop. The gcode.process_next_command method parses the next
 * command and hands off execution to individual handler functions.
 */
uint8_t commands_in_queue = 0, // Count of commands in the queue
        cmd_queue_index_r = 0, // Ring buffer read position
        cmd_queue_index_w = 0; // Ring buffer write position

char command_queue[BUFSIZE][MAX_CMD_SIZE];

/*
 * The port that the command was received on
 */
#if NUM_SERIAL > 1
  int16_t command_queue_port[BUFSIZE];
#endif

/**
 * Serial command injection
 */

// Number of characters read in the current line of serial input
static int serial_count[NUM_SERIAL] = { 0 };

bool send_ok[BUFSIZE];

/**
 * Next Injected Command pointer. NULL if no commands are being injected.
 * Used by Marlin internally to ensure that commands initiated from within
 * are enqueued ahead of any pending serial or sd card commands.
 */
static const char *injected_commands_P = NULL;

void queue_setup() {
  // Send "ok" after commands by default
  for (uint8_t i = 0; i < COUNT(send_ok); i++) send_ok[i] = true;
}

/**
 * Clear the Marlin command queue
 */
void clear_command_queue() {
  cmd_queue_index_r = cmd_queue_index_w = commands_in_queue = 0;
}

/**
 * Once a new command is in the ring buffer, call this to commit it
 */
inline void _commit_command(bool say_ok
  #if NUM_SERIAL > 1
    , int16_t port = -1
  #endif
) {
  send_ok[cmd_queue_index_w] = say_ok;
  #if NUM_SERIAL > 1
    command_queue_port[cmd_queue_index_w] = port;
  #endif
  if (++cmd_queue_index_w >= BUFSIZE) cmd_queue_index_w = 0;
  commands_in_queue++;
}

/**
 * Copy a command from RAM into the main command buffer.
 * Return true if the command was successfully added.
 * Return false for a full buffer, or if the 'command' is a comment.
 */
inline bool _enqueuecommand(const char* cmd, bool say_ok=false
  #if NUM_SERIAL > 1
    , int16_t port = -1
  #endif
) {
  if (*cmd == ';' || commands_in_queue >= BUFSIZE) return false;
  strcpy(command_queue[cmd_queue_index_w], cmd);
  _commit_command(say_ok
    #if NUM_SERIAL > 1
      , port
    #endif
  );
  return true;
}

/**
 * Enqueue with Serial Echo
 */
bool enqueue_and_echo_command(const char* cmd) {

  //SERIAL_ECHO("enqueue_and_echo_command(\"");
  //SERIAL_ECHO(cmd);
  //SERIAL_ECHO("\") \n");

  if (*cmd == 0 || *cmd == '\n' || *cmd == '\r') {
    //SERIAL_ECHO("Null command found...   Did not queue!\n");
    return true;
  }

  if (_enqueuecommand(cmd)) {
    SERIAL_ECHO_START();
    SERIAL_ECHOPAIR(MSG_ENQUEUEING, cmd);
    SERIAL_CHAR('"');
    SERIAL_EOL();
    return true;
  }
  return false;
}

/**
 * Inject the next "immediate" command, when possible, onto the front of the queue.
 * Return true if any immediate commands remain to inject.
 */
static bool drain_injected_commands_P() {
  if (injected_commands_P != NULL) {
    size_t i = 0;
    char c, cmd[60];
    strncpy_P(cmd, injected_commands_P, sizeof(cmd) - 1);
    cmd[sizeof(cmd) - 1] = '\0';
    while ((c = cmd[i]) && c != '\n') i++; // find the end of this gcode command
    cmd[i] = '\0';
    if (enqueue_and_echo_command(cmd))     // success?
      injected_commands_P = c ? injected_commands_P + i + 1 : NULL; // next command or done
  }
  return (injected_commands_P != NULL);    // return whether any more remain
}

/**
 * Record one or many commands to run from program memory.
 * Aborts the current queue, if any.
 * Note: drain_injected_commands_P() must be called repeatedly to drain the commands afterwards
 */
void enqueue_and_echo_commands_P(const char * const pgcode) {
  injected_commands_P = pgcode;
  (void)drain_injected_commands_P(); // first command executed asap (when possible)
}

#if HAS_QUEUE_NOW
  /**
   * Enqueue and return only when commands are actually enqueued
   */
  void enqueue_and_echo_command_now(const char* cmd) {
    while (!enqueue_and_echo_command(cmd)) idle();
  }
  #if HAS_LCD_QUEUE_NOW
    /**
     * Enqueue from program memory and return only when commands are actually enqueued
     */
    void enqueue_and_echo_commands_now_P(const char * const pgcode) {
      enqueue_and_echo_commands_P(pgcode);
      while (drain_injected_commands_P()) idle();
    }
  #endif
#endif

/**
 * Send an "ok" message to the host, indicating
 * that a command was successfully processed.
 *
 * If ADVANCED_OK is enabled also include:
 *   N<int>  Line number of the command, if any
 *   P<int>  Planner space remaining
 *   B<int>  Block queue space remaining
 */
void ok_to_send() {
  #if NUM_SERIAL > 1
    const int16_t port = command_queue_port[cmd_queue_index_r];
    if (port < 0) return;
  #endif
  if (!send_ok[cmd_queue_index_r]) return;
  SERIAL_PROTOCOLPGM_P(port, MSG_OK);
  #if ENABLED(ADVANCED_OK)
    char* p = command_queue[cmd_queue_index_r];
    if (*p == 'N') {
      SERIAL_PROTOCOL_P(port, ' ');
      SERIAL_ECHO_P(port, *p++);
      while (NUMERIC_SIGNED(*p))
        SERIAL_ECHO_P(port, *p++);
    }
    SERIAL_PROTOCOLPGM_P(port, " P"); SERIAL_PROTOCOL_P(port, int(BLOCK_BUFFER_SIZE - planner.movesplanned() - 1));
    SERIAL_PROTOCOLPGM_P(port, " B"); SERIAL_PROTOCOL_P(port, BUFSIZE - commands_in_queue);
  #endif
  SERIAL_EOL_P(port);
}

/**
 * Send a "Resend: nnn" message to the host to
 * indicate that a command needs to be re-sent.
 */
void flush_and_request_resend() {
  #if NUM_SERIAL > 1
    const int16_t port = command_queue_port[cmd_queue_index_r];
    if (port < 0) return;
  #endif
  SERIAL_FLUSH_P(port);
  SERIAL_PROTOCOLPGM_P(port, MSG_RESEND);
  SERIAL_PROTOCOLLN_P(port, gcode_LastN + 1);
  ok_to_send();
}

void gcode_line_error(const char* err, uint8_t port) {
  SERIAL_ERROR_START_P(port);
  serialprintPGM_P(port, err);
  SERIAL_ERRORLN_P(port, gcode_LastN);
  flush_and_request_resend();
  serial_count[port] = 0;
}

static bool serial_data_available() {
  return (MYSERIAL0.available() ? true :
    #if NUM_SERIAL > 1
      MYSERIAL1.available() ? true :
    #endif
    false);
}

static int read_serial(const int index) {
  switch (index) {
    case 0: return MYSERIAL0.read();
    #if NUM_SERIAL > 1
      case 1: return MYSERIAL1.read();
    #endif
    default: return -1;
  }
}

/**
 * Get all commands waiting on the serial port and queue them.
 * Exit when the buffer is full or when no more characters are
 * left on the serial port.
 */
inline void get_serial_commands() {
  static char serial_line_buffer[NUM_SERIAL][MAX_CMD_SIZE];
  static bool serial_comment_mode[NUM_SERIAL] = { false };

  // If the command buffer is empty for too long,
  // send "wait" to indicate Marlin is still waiting.
  #if NO_TIMEOUTS > 0
    static millis_t last_command_time = 0;
    const millis_t ms = millis();
    if (commands_in_queue == 0 && !serial_data_available() && ELAPSED(ms, last_command_time + NO_TIMEOUTS)) {
      SERIAL_ECHOLNPGM(MSG_WAIT);
      last_command_time = ms;
    }
  #endif

  /**
   * Loop while serial characters are incoming and the queue is not full
   */
  while (commands_in_queue < BUFSIZE && serial_data_available()) {
    for (uint8_t i = 0; i < NUM_SERIAL; ++i) {
      int c;
      if ((c = read_serial(i)) < 0) continue;

      char serial_char = c;

      /**
       * If the character ends the line
       */
      if (serial_char == '\n' || serial_char == '\r') {

        serial_comment_mode[i] = false;                   // end of line == end of comment

        // Skip empty lines and comments
        if (!serial_count[i]) { thermalManager.manage_heater(); continue; }

        serial_line_buffer[i][serial_count[i]] = 0;       // Terminate string
        serial_count[i] = 0;                              // Reset buffer

        char* command = serial_line_buffer[i];

        while (*command == ' ') command++;                // Skip leading spaces
        char *npos = (*command == 'N') ? command : NULL;  // Require the N parameter to start the line

        if (npos) {

          bool M110 = strstr_P(command, PSTR("M110")) != NULL;

          if (M110) {
            char* n2pos = strchr(command + 4, 'N');
            if (n2pos) npos = n2pos;
          }

          gcode_N = strtol(npos + 1, NULL, 10);

          if (gcode_N != gcode_LastN + 1 && !M110)
            return gcode_line_error(PSTR(MSG_ERR_LINE_NO), i);

          char *apos = strrchr(command, '*');
          if (apos) {
            uint8_t checksum = 0, count = uint8_t(apos - command);
            while (count) checksum ^= command[--count];
            if (strtol(apos + 1, NULL, 10) != checksum)
              return gcode_line_error(PSTR(MSG_ERR_CHECKSUM_MISMATCH), i);
          }
          else
            return gcode_line_error(PSTR(MSG_ERR_NO_CHECKSUM), i);

          gcode_LastN = gcode_N;
        }
        #if ENABLED(SDSUPPORT)
          else if (card.saving)
            return gcode_line_error(PSTR(MSG_ERR_NO_CHECKSUM), i);
        #endif

        // Movement commands alert when stopped
        if (IsStopped()) {
          char* gpos = strchr(command, 'G');
          if (gpos) {
            const int codenum = strtol(gpos + 1, NULL, 10);
            switch (codenum) {
              case 0:
              case 1:
              case 2:
              case 3:
                SERIAL_ERRORLNPGM_P(i, MSG_ERR_STOPPED);
                LCD_MESSAGEPGM(MSG_STOPPED);
                break;
            }
          }
        }

        #if DISABLED(EMERGENCY_PARSER)
          // Process critical commands early
          if (strcmp(command, "M108") == 0) {
            wait_for_heatup = false;
            #if ENABLED(ULTIPANEL)
              wait_for_user = false;
            #endif
          }
          if (strcmp(command, "M112") == 0) kill(PSTR(MSG_KILLED));
          if (strcmp(command, "M410") == 0) quickstop_stepper();
        #endif

        #if defined(NO_TIMEOUTS) && NO_TIMEOUTS > 0
          last_command_time = ms;
        #endif

        // Add the command to the queue
        _enqueuecommand(serial_line_buffer[i], true
          #if NUM_SERIAL > 1
            , i
          #endif
        );
      }
      else if (serial_count[i] >= MAX_CMD_SIZE - 1) {
        // Keep fetching, but ignore normal characters beyond the max length
        // The command will be injected when EOL is reached
      }
      else if (serial_char == '\\') {  // Handle escapes
        // if we have one more character, copy it over
        if ((c = read_serial(i)) >= 0 && !serial_comment_mode[i])
          serial_line_buffer[i][serial_count[i]++] = (char)c;
      }
      else { // it's not a newline, carriage return or escape char
        if (serial_char == ';') serial_comment_mode[i] = true;
        if (!serial_comment_mode[i]) serial_line_buffer[i][serial_count[i]++] = serial_char;
      }
    } // for NUM_SERIAL
  } // queue has space, serial has data
}

#if ENABLED(SDSUPPORT)

  /**
   * Get commands from the SD Card until the command buffer is full
   * or until the end of the file is reached. The special character '#'
   * can also interrupt buffering.
   */
  inline void get_sdcard_commands() {
    static bool stop_buffering = false,
                sd_comment_mode = false;

    if (!IS_SD_PRINTING) return;

    /**
     * '#' stops reading from SD to the buffer prematurely, so procedural
     * macro calls are possible. If it occurs, stop_buffering is triggered
     * and the buffer is run dry; this character _can_ occur in serial com
     * due to checksums, however, no checksums are used in SD printing.
     */

    if (commands_in_queue == 0) stop_buffering = false;

    uint16_t sd_count = 0;
    bool card_eof = card.eof();
    while (commands_in_queue < BUFSIZE && !card_eof && !stop_buffering) {
      const int16_t n = card.get();
      char sd_char = (char)n;
      card_eof = card.eof();
      if (card_eof || n == -1
          || sd_char == '\n' || sd_char == '\r'
          || ((sd_char == '#' || sd_char == ':') && !sd_comment_mode)
      ) {
        if (card_eof) {

          card.printingHasFinished();

          if (card.sdprinting)
            sd_count = 0; // If a sub-file was printing, continue from call point
          else {
            SERIAL_PROTOCOLLNPGM(MSG_FILE_PRINTED);
            #if ENABLED(PRINTER_EVENT_LEDS)
              LCD_MESSAGEPGM(MSG_INFO_COMPLETED_PRINTS);
              leds.set_green();
              #if HAS_RESUME_CONTINUE
                gcode.lights_off_after_print = true;
                enqueue_and_echo_commands_P(PSTR("M0 S"
                  #if ENABLED(NEWPANEL)
                    "1800"
                  #else
                    "60"
                  #endif
                ));
              #else
                safe_delay(2000);
                leds.set_off();
              #endif
            #endif // PRINTER_EVENT_LEDS
          }
        }
        else if (n == -1) {
          SERIAL_ERROR_START();
          SERIAL_ECHOLNPGM(MSG_SD_ERR_READ);
        }
        if (sd_char == '#') stop_buffering = true;

        sd_comment_mode = false; // for new command

        // Skip empty lines and comments
        if (!sd_count) { thermalManager.manage_heater(); continue; }

        command_queue[cmd_queue_index_w][sd_count] = '\0'; // terminate string
        sd_count = 0; // clear sd line buffer

        _commit_command(false);
      }
      else if (sd_count >= MAX_CMD_SIZE - 1) {
        /**
         * Keep fetching, but ignore normal characters beyond the max length
         * The command will be injected when EOL is reached
         */
      }
      else {
        if (sd_char == ';') sd_comment_mode = true;
        if (!sd_comment_mode) command_queue[cmd_queue_index_w][sd_count++] = sd_char;
      }
    }
  }

  #if ENABLED(POWER_LOSS_RECOVERY)

    inline bool drain_job_recovery_commands() {
      static uint8_t job_recovery_commands_index = 0; // Resets on reboot
      if (job_recovery_commands_count) {
        if (_enqueuecommand(job_recovery_commands[job_recovery_commands_index])) {
          ++job_recovery_commands_index;
          if (!--job_recovery_commands_count) job_recovery_phase = JOB_RECOVERY_DONE;
        }
        return true;
      }
      return false;
    }

  #endif

#endif // SDSUPPORT

/**
 * Add to the circular command queue the next command from:
 *  - The command-injection queue (injected_commands_P)
 *  - The active serial input (usually USB)
 *  - The SD card file being actively printed
 */
void get_available_commands() {

  // if any immediate commands remain, don't get other commands yet
  if (drain_injected_commands_P()) return;

  get_serial_commands();

  #if ENABLED(POWER_LOSS_RECOVERY)
    // Commands for power-loss recovery take precedence
    if (job_recovery_phase == JOB_RECOVERY_YES && drain_job_recovery_commands()) return;
  #endif

  #if ENABLED(SDSUPPORT)
    get_sdcard_commands();
  #endif
}

/**
 * Get the next command in the queue, optionally log it to SD, then dispatch it
 */
void advance_command_queue() {

  if (!commands_in_queue) return;

  #if ENABLED(SDSUPPORT)

    if (card.saving) {
      char* command = command_queue[cmd_queue_index_r];
      if (strstr_P(command, PSTR("M29"))) {
        // M29 closes the file
        card.closefile();
        SERIAL_PROTOCOLLNPGM(MSG_FILE_SAVED);

        #if !defined(__AVR__) || !defined(USBCON)
          #if ENABLED(SERIAL_STATS_DROPPED_RX)
            SERIAL_ECHOLNPAIR("Dropped bytes: ", customizedSerial.dropped());
          #endif

          #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
            SERIAL_ECHOLNPAIR("Max RX Queue Size: ", customizedSerial.rxMaxEnqueued());
          #endif
        #endif //  !defined(__AVR__) || !defined(USBCON)

        ok_to_send();
      }
      else {
        // Write the string from the read buffer to SD
        card.write_command(command);
        if (card.logging)
          gcode.process_next_command(); // The card is saving because it's logging
        else
          ok_to_send();
      }
    }
    else {
      gcode.process_next_command();
      #if ENABLED(POWER_LOSS_RECOVERY)
        if (card.cardOK && card.sdprinting) save_job_recovery_info();
      #endif
    }

  #else

    gcode.process_next_command();

  #endif // SDSUPPORT

  // The queue may be reset by a command handler or by code invoked by idle() within a handler
  if (commands_in_queue) {
    --commands_in_queue;
    if (++cmd_queue_index_r >= BUFSIZE) cmd_queue_index_r = 0;
  }

}
