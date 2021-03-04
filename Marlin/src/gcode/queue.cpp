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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * queue.cpp - The G-code command queue
 */

#include "queue.h"
GCodeQueue queue;

#include "gcode.h"

#include "../lcd/marlinui.h"
#include "../sd/cardreader.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../module/temperature.h"
#include "../MarlinCore.h"
#include "../core/bug_on.h"

#if ENABLED(PRINTER_EVENT_LEDS)
  #include "../feature/leds/printer_event_leds.h"
#endif

#if HAS_ETHERNET
  #include "../feature/ethernet.h"
#endif

#if ENABLED(BINARY_FILE_TRANSFER)
  #include "../feature/binary_stream.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif

#if ENABLED(GCODE_REPEAT_MARKERS)
  #include "../feature/repeat.h"
#endif

// Frequently used G-code strings
PGMSTR(G28_STR, "G28");

GCodeQueue::SerialState GCodeQueue::serial_state[NUM_SERIAL] = { 0 };
GCodeQueue::RingBuffer GCodeQueue::ring_buffer = { 0 };

#if NO_TIMEOUTS > 0
  static millis_t last_command_time = 0;
#endif

/**
 * Serial command injection
 */

/**
 * Next Injected PROGMEM Command pointer. (nullptr == empty)
 * Internal commands are enqueued ahead of serial / SD commands.
 */
PGM_P GCodeQueue::injected_commands_P; // = nullptr

/**
 * Injected SRAM Commands
 */
char GCodeQueue::injected_commands[64]; // = { 0 }


void GCodeQueue::RingBuffer::commit_command(bool skip_ok
  #if HAS_MULTI_SERIAL
    , serial_index_t serial_ind/*=-1*/
  #endif
) {
  commands[index_w].skip_ok = skip_ok;
  TERN_(HAS_MULTI_SERIAL, commands[index_w].port = serial_ind);
  TERN_(POWER_LOSS_RECOVERY, recovery.commit_sdpos(index_w));
  advance_pos(index_w, 1);
}

/**
 * Copy a command from RAM into the main command buffer.
 * Return true if the command was successfully added.
 * Return false for a full buffer, or if the 'command' is a comment.
 */
bool GCodeQueue::RingBuffer::enqueue(const char* cmd, bool skip_ok/*=true*/
  #if HAS_MULTI_SERIAL
    , serial_index_t serial_ind/*=-1*/
  #endif
) {
  if (*cmd == ';' || length >= BUFSIZE) return false;
  strcpy(commands[index_w].buffer, cmd);
  commit_command(skip_ok
    #if HAS_MULTI_SERIAL
      , serial_ind
    #endif
  );
  return true;
}

/**
 * Enqueue with Serial Echo
 * Return true if the command was consumed
 */
bool GCodeQueue::enqueue_one(const char* cmd) {
  //SERIAL_ECHOLNPAIR("enqueue_one(\"", cmd, "\")");

  if (*cmd == 0 || ISEOL(*cmd)) return true;

  if (ring_buffer.enqueue(cmd)) {
    SERIAL_ECHO_MSG(STR_ENQUEUEING, cmd, "\"");
    return true;
  }
  return false;
}

/**
 * Process the next "immediate" command from PROGMEM.
 * Return 'true' if any commands were processed.
 */
bool GCodeQueue::process_injected_command_P() {
  if (!injected_commands_P) return false;

  char c;
  size_t i = 0;
  while ((c = pgm_read_byte(&injected_commands_P[i])) && c != '\n') i++;

  // Extract current command and move pointer to next command
  char cmd[i + 1];
  memcpy_P(cmd, injected_commands_P, i);
  cmd[i] = '\0';
  injected_commands_P = c ? injected_commands_P + i + 1 : nullptr;

  // Execute command if non-blank
  if (i) {
    parser.parse(cmd);
    gcode.process_parsed_command();
  }
  return true;
}

/**
 * Process the next "immediate" command from SRAM.
 * Return 'true' if any commands were processed.
 */
bool GCodeQueue::process_injected_command() {
  if (injected_commands[0] == '\0') return false;

  char c;
  size_t i = 0;
  while ((c = injected_commands[i]) && c != '\n') i++;

  // Execute a non-blank command
  if (i) {
    injected_commands[i] = '\0';
    parser.parse(injected_commands);
    gcode.process_parsed_command();
  }

  // Copy the next command into place
  for (
    uint8_t d = 0, s = i + !!c;                     // dst, src
    (injected_commands[d] = injected_commands[s]);  // copy, exit if 0
    d++, s++                                        // next dst, src
  );

  return true;
}

/**
 * Enqueue and return only when commands are actually enqueued.
 * Never call this from a G-code handler!
 */
void GCodeQueue::enqueue_one_now(const char* cmd) { while (!enqueue_one(cmd)) idle(); }

/**
 * Attempt to enqueue a single G-code command
 * and return 'true' if successful.
 */
bool GCodeQueue::enqueue_one_P(PGM_P const pgcode) {
  size_t i = 0;
  PGM_P p = pgcode;
  char c;
  while ((c = pgm_read_byte(&p[i])) && c != '\n') i++;
  char cmd[i + 1];
  memcpy_P(cmd, p, i);
  cmd[i] = '\0';
  return ring_buffer.enqueue(cmd);
}

/**
 * Enqueue from program memory and return only when commands are actually enqueued
 * Never call this from a G-code handler!
 */
void GCodeQueue::enqueue_now_P(PGM_P const pgcode) {
  size_t i = 0;
  PGM_P p = pgcode;
  for (;;) {
    char c;
    while ((c = pgm_read_byte(&p[i])) && c != '\n') i++;
    char cmd[i + 1];
    memcpy_P(cmd, p, i);
    cmd[i] = '\0';
    enqueue_one_now(cmd);
    if (!c) break;
    p += i + 1;
  }
}

/**
 * Send an "ok" message to the host, indicating
 * that a command was successfully processed.
 *
 * If ADVANCED_OK is enabled also include:
 *   N<int>  Line number of the command, if any
 *   P<int>  Planner space remaining
 *   B<int>  Block queue space remaining
 */
void GCodeQueue::RingBuffer::ok_to_send() {
  #if NO_TIMEOUTS > 0
    // Start counting from the last command's execution
    last_command_time = millis();
  #endif
  CommandLine &command = commands[index_r];
  #if HAS_MULTI_SERIAL
    const serial_index_t serial_ind = command.port;
    if (serial_ind < 0) return;
    PORT_REDIRECT(SERIAL_PORTMASK(serial_ind));   // Reply to the serial port that sent the command
  #endif
  if (command.skip_ok) return;
  SERIAL_ECHOPGM(STR_OK);
  #if ENABLED(ADVANCED_OK)
    char* p = command.buffer;
    if (*p == 'N') {
      SERIAL_CHAR(' ', *p++);
      while (NUMERIC_SIGNED(*p))
        SERIAL_CHAR(*p++);
    }
    SERIAL_ECHOPAIR_P(SP_P_STR, planner.moves_free(),
                      SP_B_STR, BUFSIZE - length);
  #endif
  SERIAL_EOL();
}

/**
 * Send a "Resend: nnn" message to the host to
 * indicate that a command needs to be re-sent.
 */
void GCodeQueue::flush_and_request_resend(const serial_index_t serial_ind) {
  #if HAS_MULTI_SERIAL
    if (serial_ind < 0) return;                   // Never mind. Command came from SD or Flash Drive
    PORT_REDIRECT(SERIAL_PORTMASK(serial_ind));   // Reply to the serial port that sent the command
  #endif
  SERIAL_FLUSH();
  SERIAL_ECHOPGM(STR_RESEND);
  SERIAL_ECHOLN(serial_state[serial_ind].last_N + 1);
}

// Multiserial already handle the dispatch to/from multiple port by itself
inline bool serial_data_available(uint8_t index = SERIAL_ALL) {
  if (index == SERIAL_ALL) {
    for (index = 0; index < NUM_SERIAL; index++) {
      const int a = SERIAL_IMPL.available(index);
      #if BOTH(RX_BUFFER_MONITOR, RX_BUFFER_SIZE)
        if (a > RX_BUFFER_SIZE - 2) {
          PORT_REDIRECT(SERIAL_PORTMASK(index));
          SERIAL_ERROR_MSG("RX BUF overflow, increase RX_BUFFER_SIZE: ", a);
        }
      #endif
      if (a > 0) return true;
    }
    return false;
  }
  const int a = SERIAL_IMPL.available(index);
  #if BOTH(RX_BUFFER_MONITOR, RX_BUFFER_SIZE)
    if (a > RX_BUFFER_SIZE - 2) {
      PORT_REDIRECT(SERIAL_PORTMASK(index));
      SERIAL_ERROR_MSG("RX BUF overflow, increase RX_BUFFER_SIZE: ", a);
    }
  #endif

  return a > 0;
}

inline int read_serial(const uint8_t index) { return SERIAL_IMPL.read(index); }

void GCodeQueue::gcode_line_error(PGM_P const err, const serial_index_t serial_ind) {
  PORT_REDIRECT(SERIAL_PORTMASK(serial_ind)); // Reply to the serial port that sent the command
  SERIAL_ERROR_START();
  SERIAL_ECHOLNPAIR_P(err, serial_state[serial_ind].last_N);
  while (read_serial(serial_ind) != -1) { /* nada */ } // Clear out the RX buffer. Why don't use flush here ?
  flush_and_request_resend(serial_ind);
  serial_state[serial_ind].count = 0;
}

FORCE_INLINE bool is_M29(const char * const cmd) {  // matches "M29" & "M29 ", but not "M290", etc
  const char * const m29 = strstr_P(cmd, PSTR("M29"));
  return m29 && !NUMERIC(m29[3]);
}

#define PS_NORMAL 0
#define PS_EOL    1
#define PS_QUOTED 2
#define PS_PAREN  3
#define PS_ESC    4

inline void process_stream_char(const char c, uint8_t &sis, char (&buff)[MAX_CMD_SIZE], int &ind) {

  if (sis == PS_EOL) return;    // EOL comment or overflow

  #if ENABLED(PAREN_COMMENTS)
    else if (sis == PS_PAREN) { // Inline comment
      if (c == ')') sis = PS_NORMAL;
      return;
    }
  #endif

  else if (sis >= PS_ESC)       // End escaped char
    sis -= PS_ESC;

  else if (c == '\\') {         // Start escaped char
    sis += PS_ESC;
    if (sis == PS_ESC) return;  // Keep if quoting
  }

  #if ENABLED(GCODE_QUOTED_STRINGS)

    else if (sis == PS_QUOTED) {
      if (c == '"') sis = PS_NORMAL; // End quoted string
    }
    else if (c == '"')          // Start quoted string
      sis = PS_QUOTED;

  #endif

  else if (c == ';') {          // Start end-of-line comment
    sis = PS_EOL;
    return;
  }

  #if ENABLED(PAREN_COMMENTS)
    else if (c == '(') {        // Start inline comment
      sis = PS_PAREN;
      return;
    }
  #endif

  // Backspace erases previous characters
  if (c == 0x08) {
    if (ind) buff[--ind] = '\0';
  }
  else {
    buff[ind++] = c;
    if (ind >= MAX_CMD_SIZE - 1)
      sis = PS_EOL;             // Skip the rest on overflow
  }
}

/**
 * Handle a line being completed. For an empty line
 * keep sensor readings going and watchdog alive.
 */
inline bool process_line_done(uint8_t &sis, char (&buff)[MAX_CMD_SIZE], int &ind) {
  sis = PS_NORMAL;                    // "Normal" Serial Input State
  buff[ind] = '\0';                   // Of course, I'm a Terminator.
  const bool is_empty = (ind == 0);   // An empty line?
  if (is_empty)
    thermalManager.manage_heater();   // Keep sensors satisfied
  else
    ind = 0;                          // Start a new line
  return is_empty;                    // Inform the caller
}

/**
 * Get all commands waiting on the serial port and queue them.
 * Exit when the buffer is full or when no more characters are
 * left on the serial port.
 */
void GCodeQueue::get_serial_commands() {
  #if ENABLED(BINARY_FILE_TRANSFER)
    if (card.flag.binary_mode) {
      /**
       * For binary stream file transfer, use serial_line_buffer as the working
       * receive buffer (which limits the packet size to MAX_CMD_SIZE).
       * The receive buffer also limits the packet size for reliable transmission.
       */
      binaryStream[card.transfer_port_index].receive(serial_state[card.transfer_port_index].line_buffer);
      return;
    }
  #endif

  // If the command buffer is empty for too long,
  // send "wait" to indicate Marlin is still waiting.
  #if NO_TIMEOUTS > 0
    const millis_t ms = millis();
    if (ring_buffer.empty() && !serial_data_available() && ELAPSED(ms, last_command_time + NO_TIMEOUTS)) {
      SERIAL_ECHOLNPGM(STR_WAIT);
      last_command_time = ms;
    }
  #endif

  // Loop while serial characters are incoming and the queue is not full
  for (bool hadData = true; hadData;) {
    // Unless a serial port has data, this will exit on next iteration
    hadData = false;

    LOOP_L_N(p, NUM_SERIAL) {
      // Check if the queue is full and exit if it is.
      if (ring_buffer.full()) return;

      // No data for this port ? Skip it
      if (!serial_data_available(p)) continue;

      // Ok, we have some data to process, let's make progress here
      hadData = true;

      const int c = read_serial(p);
      if (c < 0) {
        // This should never happen, let's log it
        PORT_REDIRECT(SERIAL_PORTMASK(p));     // Reply to the serial port that sent the command
        // Crash here to get more information why it failed
        BUG_ON("SP available but read -1");
        SERIAL_ERROR_MSG(STR_ERR_SERIAL_MISMATCH);
        SERIAL_FLUSH();
        continue;
      }

      const char serial_char = (char)c;
      SerialState &serial = serial_state[p];

      if (ISEOL(serial_char)) {

        // Reset our state, continue if the line was empty
        if (process_line_done(serial.input_state, serial.line_buffer, serial.count))
          continue;

        char* command = serial.line_buffer;

        while (*command == ' ') command++;                   // Skip leading spaces
        char *npos = (*command == 'N') ? command : nullptr;  // Require the N parameter to start the line

        if (npos) {

          const bool M110 = !!strstr_P(command, PSTR("M110"));

          if (M110) {
            char* n2pos = strchr(command + 4, 'N');
            if (n2pos) npos = n2pos;
          }

          const long gcode_N = strtol(npos + 1, nullptr, 10);

          if (gcode_N != serial.last_N + 1 && !M110) {
            // In case of error on a serial port, don't prevent other serial port from making progress
            gcode_line_error(PSTR(STR_ERR_LINE_NO), p);
            break;
          }

          char *apos = strrchr(command, '*');
          if (apos) {
            uint8_t checksum = 0, count = uint8_t(apos - command);
            while (count) checksum ^= command[--count];
            if (strtol(apos + 1, nullptr, 10) != checksum) {
              // In case of error on a serial port, don't prevent other serial port from making progress
              gcode_line_error(PSTR(STR_ERR_CHECKSUM_MISMATCH), p);
              break;
            }
          }
          else {
            // In case of error on a serial port, don't prevent other serial port from making progress
            gcode_line_error(PSTR(STR_ERR_NO_CHECKSUM), p);
            break;
          }

          serial.last_N = gcode_N;
        }
        #if ENABLED(SDSUPPORT)
          // Pronterface "M29" and "M29 " has no line number
          else if (card.flag.saving && !is_M29(command)) {
            gcode_line_error(PSTR(STR_ERR_NO_CHECKSUM), p);
            break;
          }
        #endif

        //
        // Movement commands give an alert when the machine is stopped
        //

        if (IsStopped()) {
          char* gpos = strchr(command, 'G');
          if (gpos) {
            switch (strtol(gpos + 1, nullptr, 10)) {
              case 0: case 1:
              #if ENABLED(ARC_SUPPORT)
                case 2: case 3:
              #endif
              #if ENABLED(BEZIER_CURVE_SUPPORT)
                case 5:
              #endif
                PORT_REDIRECT(SERIAL_PORTMASK(p));     // Reply to the serial port that sent the command
                SERIAL_ECHOLNPGM(STR_ERR_STOPPED);
                LCD_MESSAGEPGM(MSG_STOPPED);
                break;
            }
          }
        }

        #if DISABLED(EMERGENCY_PARSER)
          // Process critical commands early
          if (command[0] == 'M') switch (command[3]) {
            case '8': if (command[2] == '0' && command[1] == '1') { wait_for_heatup = false; TERN_(HAS_LCD_MENU, wait_for_user = false); } break;
            case '2': if (command[2] == '1' && command[1] == '1') kill(M112_KILL_STR, nullptr, true); break;
            case '0': if (command[1] == '4' && command[2] == '1') quickstop_stepper(); break;
          }
        #endif

        #if NO_TIMEOUTS > 0
          last_command_time = ms;
        #endif

        // Add the command to the queue
        ring_buffer.enqueue(serial.line_buffer, false
          #if HAS_MULTI_SERIAL
            , p
          #endif
        );
      }
      else
        process_stream_char(serial_char, serial.input_state, serial.line_buffer, serial.count);

    } // NUM_SERIAL loop
  } // queue has space, serial has data
}

#if ENABLED(SDSUPPORT)

  /**
   * Get lines from the SD Card until the command buffer is full
   * or until the end of the file is reached. Because this method
   * always receives complete command-lines, they can go directly
   * into the main command queue.
   */
  inline void GCodeQueue::get_sdcard_commands() {
    static uint8_t sd_input_state = PS_NORMAL;

    if (!IS_SD_PRINTING()) return;

    int sd_count = 0;
    while (!ring_buffer.full() && !card.eof()) {
      const int16_t n = card.get();
      const bool card_eof = card.eof();
      if (n < 0 && !card_eof) { SERIAL_ERROR_MSG(STR_SD_ERR_READ); continue; }

      CommandLine &command = ring_buffer.commands[ring_buffer.index_w];
      const char sd_char = (char)n;
      const bool is_eol = ISEOL(sd_char);
      if (is_eol || card_eof) {


        // Reset stream state, terminate the buffer, and commit a non-empty command
        if (!is_eol && sd_count) ++sd_count;          // End of file with no newline
        if (!process_line_done(sd_input_state, command.buffer, sd_count)) {

          // M808 L saves the sdpos of the next line. M808 loops to a new sdpos.
          TERN_(GCODE_REPEAT_MARKERS, repeat.early_parse_M808(command.buffer));

          // Put the new command into the buffer (no "ok" sent)
          ring_buffer.commit_command(true);

          // Prime Power-Loss Recovery for the NEXT commit_command
          TERN_(POWER_LOSS_RECOVERY, recovery.cmd_sdpos = card.getIndex());
        }

        if (card.eof()) card.fileHasFinished();         // Handle end of file reached
      }
      else
        process_stream_char(sd_char, sd_input_state, command.buffer, sd_count);
    }
  }

#endif // SDSUPPORT

/**
 * Add to the circular command queue the next command from:
 *  - The command-injection queues (injected_commands_P, injected_commands)
 *  - The active serial input (usually USB)
 *  - The SD card file being actively printed
 */
void GCodeQueue::get_available_commands() {
  if (ring_buffer.full()) return;

  get_serial_commands();

  TERN_(SDSUPPORT, get_sdcard_commands());
}

/**
 * Get the next command in the queue, optionally log it to SD, then dispatch it
 */
void GCodeQueue::advance() {

  // Process immediate commands
  if (process_injected_command_P() || process_injected_command()) return;

  // Return if the G-code buffer is empty
  if (ring_buffer.empty()) return;

  #if ENABLED(SDSUPPORT)

    if (card.flag.saving) {
      char * const cmd = ring_buffer.peek_next_command_string();
      if (is_M29(cmd)) {
        // M29 closes the file
        card.closefile();
        SERIAL_ECHOLNPGM(STR_FILE_SAVED);

        #if !defined(__AVR__) || !defined(USBCON)
          #if ENABLED(SERIAL_STATS_DROPPED_RX)
            SERIAL_ECHOLNPAIR("Dropped bytes: ", MYSERIAL0.dropped());
          #endif
          #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
            SERIAL_ECHOLNPAIR("Max RX Queue Size: ", MYSERIAL0.rxMaxEnqueued());
          #endif
        #endif

        ok_to_send();
      }
      else {
        // Write the string from the read buffer to SD
        card.write_command(cmd);
        if (card.flag.logging)
          gcode.process_next_command(); // The card is saving because it's logging
        else
          ok_to_send();
      }
    }
    else
      gcode.process_next_command();

  #else

    gcode.process_next_command();

  #endif // SDSUPPORT

  // The queue may be reset by a command handler or by code invoked by idle() within a handler
  ring_buffer.advance_pos(ring_buffer.index_r, -1);
}
