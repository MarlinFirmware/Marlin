/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * queue.cpp - The G-code command queue
 */

#include "queue.h"
GCodeQueue queue;

#include "gcode.h"

#include "../lcd/ultralcd.h"
#include "../sd/cardreader.h"
#include "../module/planner.h"
#include "../module/temperature.h"
#include "../Marlin.h"

#if ENABLED(PRINTER_EVENT_LEDS)
  #include "../feature/leds/printer_event_leds.h"
#endif

/**
 * GCode line number handling. Hosts may opt to include line numbers when
 * sending commands to Marlin, and lines will be checked for sequentiality.
 * M110 N<int> sets the current line number.
 */
long gcode_N, GCodeQueue::last_N, GCodeQueue::stopped_N = 0;

/**
 * GCode Command Queue
 * A simple ring buffer of BUFSIZE command strings.
 *
 * Commands are copied into this buffer by the command injectors
 * (immediate, serial, sd card) and they are processed sequentially by
 * the main loop. The gcode.process_next_command method parses the next
 * command and hands off execution to individual handler functions.
 */
uint8_t GCodeQueue::length = 0,  // Count of commands in the queue
        GCodeQueue::index_r = 0, // Ring buffer read position
        GCodeQueue::index_w = 0; // Ring buffer write position

char GCodeQueue::buffer[BUFSIZE][MAX_CMD_SIZE];

/*
 * The port that the command was received on
 */
#if NUM_SERIAL > 1
  int16_t GCodeQueue::port[BUFSIZE];
#endif

/**
 * Serial command injection
 */

// Number of characters read in the current line of serial input
static int serial_count[NUM_SERIAL] = { 0 };

bool send_ok[BUFSIZE];

/**
 * Next Injected Command pointer. nullptr if no commands are being injected.
 * Used by Marlin internally to ensure that commands initiated from within
 * are enqueued ahead of any pending serial or sd card commands.
 */
static PGM_P injected_commands_P = nullptr;

GCodeQueue::GCodeQueue() {
  // Send "ok" after commands by default
  for (uint8_t i = 0; i < COUNT(send_ok); i++) send_ok[i] = true;
}

/**
 * Check whether there are any commands yet to be executed
 */
bool GCodeQueue::has_commands_queued() {
  return queue.length || injected_commands_P;
}

/**
 * Clear the Marlin command queue
 */
void GCodeQueue::clear() {
  index_r = index_w = length = 0;
}

/**
 * Once a new command is in the ring buffer, call this to commit it
 */
void GCodeQueue::_commit_command(bool say_ok
  #if NUM_SERIAL > 1
    , int16_t p/*=-1*/
  #endif
) {
  send_ok[index_w] = say_ok;
  #if NUM_SERIAL > 1
    port[index_w] = p;
  #endif
  if (++index_w >= BUFSIZE) index_w = 0;
  length++;
}

/**
 * Copy a command from RAM into the main command buffer.
 * Return true if the command was successfully added.
 * Return false for a full buffer, or if the 'command' is a comment.
 */
bool GCodeQueue::_enqueue(const char* cmd, bool say_ok/*=false*/
  #if NUM_SERIAL > 1
    , int16_t pn/*=-1*/
  #endif
) {
  if (*cmd == ';' || length >= BUFSIZE) return false;
  strcpy(buffer[index_w], cmd);
  _commit_command(say_ok
    #if NUM_SERIAL > 1
      , pn
    #endif
  );
  return true;
}

/**
 * Enqueue with Serial Echo
 * Return true if the command was consumed
 */
bool GCodeQueue::enqueue_one(const char* cmd) {

  //SERIAL_ECHOPGM("enqueue_one(\"");
  //SERIAL_ECHO(cmd);
  //SERIAL_ECHOPGM("\") \n");

  if (*cmd == 0 || *cmd == '\n' || *cmd == '\r') return true;

  if (_enqueue(cmd)) {
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_ENQUEUEING, cmd, "\"");
    return true;
  }
  return false;
}

/**
 * Process the next "immediate" command.
 * Return 'true' if any commands were processed,
 * or remain to process.
 */
bool GCodeQueue::process_injected_command() {
  if (injected_commands_P == nullptr) return false;

  char c;
  size_t i = 0;
  while ((c = pgm_read_byte(&injected_commands_P[i])) && c != '\n') i++;
  if (i) {
    char cmd[i + 1];
    memcpy_P(cmd, injected_commands_P, i);
    cmd[i] = '\0';

    parser.parse(cmd);
    PORT_REDIRECT(SERIAL_PORT);
    gcode.process_parsed_command();
  }
  injected_commands_P = c ? injected_commands_P + i + 1 : nullptr;
  return true;
}

/**
 * Enqueue one or many commands to run from program memory.
 * Do not inject a comment or use leading spaces!
 * Aborts the current queue, if any.
 * Note: process_injected_command() will be called to drain any commands afterwards
 */
void GCodeQueue::inject_P(PGM_P const pgcode) { injected_commands_P = pgcode; }

/**
 * Enqueue and return only when commands are actually enqueued.
 * Never call this from a G-code handler!
 */
void GCodeQueue::enqueue_one_now(const char* cmd) { while (!enqueue_one(cmd)) idle(); }

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
void GCodeQueue::ok_to_send() {
  #if NUM_SERIAL > 1
    const int16_t pn = port[index_r];
    if (pn < 0) return;
    PORT_REDIRECT(pn);
  #endif
  if (!send_ok[index_r]) return;
  SERIAL_ECHOPGM(MSG_OK);
  #if ENABLED(ADVANCED_OK)
    char* p = buffer[index_r];
    if (*p == 'N') {
      SERIAL_ECHO(' ');
      SERIAL_ECHO(*p++);
      while (NUMERIC_SIGNED(*p))
        SERIAL_ECHO(*p++);
    }
    SERIAL_ECHOPGM(" P"); SERIAL_ECHO(int(BLOCK_BUFFER_SIZE - planner.movesplanned() - 1));
    SERIAL_ECHOPGM(" B"); SERIAL_ECHO(BUFSIZE - length);
  #endif
  SERIAL_EOL();
}

/**
 * Send a "Resend: nnn" message to the host to
 * indicate that a command needs to be re-sent.
 */
void GCodeQueue::flush_and_request_resend() {
  #if NUM_SERIAL > 1
    const int16_t p = port[index_r];
    if (p < 0) return;
    PORT_REDIRECT(p);
  #endif
  SERIAL_FLUSH();
  SERIAL_ECHOPGM(MSG_RESEND);
  SERIAL_ECHOLN(last_N + 1);
  ok_to_send();
}

inline bool serial_data_available() {
  return false
    || MYSERIAL0.available()
    #if NUM_SERIAL > 1
      || MYSERIAL1.available()
    #endif
  ;
}

inline int read_serial(const uint8_t index) {
  switch (index) {
    case 0: return MYSERIAL0.read();
    #if NUM_SERIAL > 1
      case 1: return MYSERIAL1.read();
    #endif
    default: return -1;
  }
}

#if ENABLED(BINARY_FILE_TRANSFER)

  inline bool serial_data_available(const uint8_t index) {
    switch (index) {
      case 0: return MYSERIAL0.available();
      #if NUM_SERIAL > 1
        case 1: return MYSERIAL1.available();
      #endif
      default: return false;
    }
  }

  class BinaryStream {
  public:
    enum class StreamState : uint8_t {
      STREAM_RESET,
      PACKET_RESET,
      STREAM_HEADER,
      PACKET_HEADER,
      PACKET_DATA,
      PACKET_VALIDATE,
      PACKET_RESEND,
      PACKET_FLUSHRX,
      PACKET_TIMEOUT,
      STREAM_COMPLETE,
      STREAM_FAILED,
    };

    #pragma pack(push, 1)

      struct StreamHeader {
        uint16_t token;
        uint32_t filesize;
      };
      union {
        uint8_t stream_header_bytes[sizeof(StreamHeader)];
        StreamHeader stream_header;
      };

      struct Packet {
        struct Header {
          uint32_t id;
          uint16_t size, checksum;
        };
        union {
          uint8_t header_bytes[sizeof(Header)];
          Header header;
        };
        uint32_t bytes_received;
        uint16_t checksum;
        millis_t timeout;
      } packet{};

    #pragma pack(pop)

    void packet_reset() {
      packet.header.id = 0;
      packet.header.size = 0;
      packet.header.checksum = 0;
      packet.bytes_received = 0;
      packet.checksum = 0x53A2;
      packet.timeout = millis() + STREAM_MAX_WAIT;
    }

    void stream_reset() {
      packets_received = 0;
      bytes_received = 0;
      packet_retries = 0;
      buffer_next_index = 0;
      stream_header.token = 0;
      stream_header.filesize = 0;
    }

    uint32_t checksum(uint32_t seed, uint8_t value) {
      return ((seed ^ value) ^ (seed << 8)) & 0xFFFF;
    }

    // read the next byte from the data stream keeping track of
    // whether the stream times out from data starvation
    // takes the data variable by reference in order to return status
    bool stream_read(uint8_t& data) {
      if (ELAPSED(millis(), packet.timeout)) {
        stream_state = StreamState::PACKET_TIMEOUT;
        return false;
      }
      if (!serial_data_available(card.transfer_port_index)) return false;
      data = read_serial(card.transfer_port_index);
      packet.timeout = millis() + STREAM_MAX_WAIT;
      return true;
    }

    template<const size_t buffer_size>
    void receive(char (&buffer)[buffer_size]) {
      uint8_t data = 0;
      millis_t transfer_timeout = millis() + RX_TIMESLICE;

      #if ENABLED(SDSUPPORT)
        PORT_REDIRECT(card.transfer_port_index);
      #endif

      while (PENDING(millis(), transfer_timeout)) {
        switch (stream_state) {
          case StreamState::STREAM_RESET:
            stream_reset();
          case StreamState::PACKET_RESET:
            packet_reset();
            stream_state = StreamState::PACKET_HEADER;
            break;
          case StreamState::STREAM_HEADER: // The filename could also be in this packet, rather than handling it in the gcode
            for (size_t i = 0; i < sizeof(stream_header); ++i)
              stream_header_bytes[i] = buffer[i];

            if (stream_header.token == 0x1234) {
              stream_state = StreamState::PACKET_RESET;
              bytes_received = 0;
              time_stream_start = millis();
              // confirm active stream and the maximum block size supported
              SERIAL_ECHO_START();
              SERIAL_ECHOLNPAIR("Datastream initialized (", stream_header.filesize, " bytes expected)");
              SERIAL_ECHOLNPAIR("so", buffer_size);
            }
            else {
              SERIAL_ECHO_MSG("Datastream init error (invalid token)");
              stream_state = StreamState::STREAM_FAILED;
            }
            buffer_next_index = 0;
            break;
          case StreamState::PACKET_HEADER:
            if (!stream_read(data)) break;

            packet.header_bytes[packet.bytes_received++] = data;
            if (packet.bytes_received == sizeof(Packet::Header)) {
              if (packet.header.id == packets_received) {
                buffer_next_index = 0;
                packet.bytes_received = 0;
                stream_state = StreamState::PACKET_DATA;
              }
              else {
                SERIAL_ECHO_MSG("Datastream packet out of order");
                stream_state = StreamState::PACKET_FLUSHRX;
              }
            }
            break;
          case StreamState::PACKET_DATA:
            if (!stream_read(data)) break;

            if (buffer_next_index < buffer_size)
              buffer[buffer_next_index] = data;
            else {
              SERIAL_ECHO_MSG("Datastream packet data buffer overrun");
              stream_state = StreamState::STREAM_FAILED;
              break;
            }

            packet.checksum = checksum(packet.checksum, data);
            packet.bytes_received++;
            buffer_next_index++;

            if (packet.bytes_received == packet.header.size)
              stream_state = StreamState::PACKET_VALIDATE;

            break;
          case StreamState::PACKET_VALIDATE:
            if (packet.header.checksum == packet.checksum) {
              packet_retries = 0;
              packets_received++;
              bytes_received += packet.header.size;

              if (packet.header.id == 0)                   // id 0 is always the stream descriptor
                stream_state = StreamState::STREAM_HEADER; // defer packet confirmation to STREAM_HEADER state
              else {
                if (bytes_received < stream_header.filesize) {
                  stream_state = StreamState::PACKET_RESET;  // reset and receive next packet
                  SERIAL_ECHOLNPAIR("ok", packet.header.id); // transmit confirm packet received and valid token
                }
                else
                  stream_state = StreamState::STREAM_COMPLETE; // no more data required

                if (card.write(buffer, buffer_next_index) < 0) {
                  stream_state = StreamState::STREAM_FAILED;
                  SERIAL_ECHO_MSG("SDCard IO Error");
                  break;
                };
              }
            }
            else {
              SERIAL_ECHO_START();
              SERIAL_ECHOLNPAIR("Block(", packet.header.id, ") Corrupt");
              stream_state = StreamState::PACKET_FLUSHRX;
            }
            break;
          case StreamState::PACKET_RESEND:
            if (packet_retries < MAX_RETRIES) {
              packet_retries++;
              stream_state = StreamState::PACKET_RESET;
              SERIAL_ECHO_START();
              SERIAL_ECHOLNPAIR("Resend request ", int(packet_retries));
              SERIAL_ECHOLNPAIR("rs", packet.header.id); // transmit resend packet token
            }
            else {
              stream_state = StreamState::STREAM_FAILED;
            }
            break;
          case StreamState::PACKET_FLUSHRX:
            if (ELAPSED(millis(), packet.timeout)) {
              stream_state = StreamState::PACKET_RESEND;
              break;
            }
            if (!serial_data_available(card.transfer_port_index)) break;
            read_serial(card.transfer_port_index); // throw away data
            packet.timeout = millis() + STREAM_MAX_WAIT;
            break;
          case StreamState::PACKET_TIMEOUT:
            SERIAL_ECHO_START();
            SERIAL_ECHOLNPGM("Datastream timeout");
            stream_state = StreamState::PACKET_RESEND;
            break;
          case StreamState::STREAM_COMPLETE:
            stream_state = StreamState::STREAM_RESET;
            card.flag.binary_mode = false;
            SERIAL_ECHO_START();
            SERIAL_ECHO(card.filename);
            SERIAL_ECHOLNPAIR(" transfer completed @ ", ((bytes_received / (millis() - time_stream_start) * 1000) / 1024), "KiB/s");
            SERIAL_ECHOLNPGM("sc"); // transmit stream complete token
            card.closefile();
            return;
          case StreamState::STREAM_FAILED:
            stream_state = StreamState::STREAM_RESET;
            card.flag.binary_mode = false;
            card.closefile();
            card.removeFile(card.filename);
            SERIAL_ECHO_START();
            SERIAL_ECHOLNPGM("File transfer failed");
            SERIAL_ECHOLNPGM("sf"); // transmit stream failed token
            return;
        }
      }
    }

    static const uint16_t STREAM_MAX_WAIT = 500, RX_TIMESLICE = 20, MAX_RETRIES = 3;
    uint8_t  packet_retries;
    uint16_t buffer_next_index;
    uint32_t packets_received,  bytes_received;
    millis_t time_stream_start;
    StreamState stream_state = StreamState::STREAM_RESET;

  } binaryStream{};

#endif // BINARY_FILE_TRANSFER

void GCodeQueue::gcode_line_error(PGM_P const err, const int8_t port) {
  PORT_REDIRECT(port);
  SERIAL_ERROR_START();
  serialprintPGM(err);
  SERIAL_ECHOLN(last_N);
  while (read_serial(port) != -1);           // clear out the RX buffer
  flush_and_request_resend();
  serial_count[port] = 0;
}

FORCE_INLINE bool is_M29(const char * const cmd) {  // matches "M29" & "M29 ", but not "M290", etc
  const char * const m29 = strstr_P(cmd, PSTR("M29"));
  return m29 && !NUMERIC(m29[3]);
}

/**
 * Get all commands waiting on the serial port and queue them.
 * Exit when the buffer is full or when no more characters are
 * left on the serial port.
 */
void GCodeQueue::get_serial_commands() {
  static char serial_line_buffer[NUM_SERIAL][MAX_CMD_SIZE];
  static bool serial_comment_mode[NUM_SERIAL] = { false }
              #if ENABLED(PAREN_COMMENTS)
                , serial_comment_paren_mode[NUM_SERIAL] = { false }
              #endif
            ;

  #if ENABLED(BINARY_FILE_TRANSFER)
    if (card.flag.saving && card.flag.binary_mode) {
      /**
       * For binary stream file transfer, use serial_line_buffer as the working
       * receive buffer (which limits the packet size to MAX_CMD_SIZE).
       * The receive buffer also limits the packet size for reliable transmission.
       */
      binaryStream.receive(serial_line_buffer[card.transfer_port_index]);
      return;
    }
  #endif

  // If the command buffer is empty for too long,
  // send "wait" to indicate Marlin is still waiting.
  #if NO_TIMEOUTS > 0
    static millis_t last_command_time = 0;
    const millis_t ms = millis();
    if (length == 0 && !serial_data_available() && ELAPSED(ms, last_command_time + NO_TIMEOUTS)) {
      SERIAL_ECHOLNPGM(MSG_WAIT);
      last_command_time = ms;
    }
  #endif

  /**
   * Loop while serial characters are incoming and the queue is not full
   */
  while (length < BUFSIZE && serial_data_available()) {
    for (uint8_t i = 0; i < NUM_SERIAL; ++i) {
      int c;
      if ((c = read_serial(i)) < 0) continue;

      char serial_char = c;

      /**
       * If the character ends the line
       */
      if (serial_char == '\n' || serial_char == '\r') {

        // Start with comment mode off
        serial_comment_mode[i] = false;
        #if ENABLED(PAREN_COMMENTS)
          serial_comment_paren_mode[i] = false;
        #endif

        // Skip empty lines and comments
        if (!serial_count[i]) { thermalManager.manage_heater(); continue; }

        serial_line_buffer[i][serial_count[i]] = 0;       // Terminate string
        serial_count[i] = 0;                              // Reset buffer

        char* command = serial_line_buffer[i];

        while (*command == ' ') command++;                // Skip leading spaces
        char *npos = (*command == 'N') ? command : nullptr;  // Require the N parameter to start the line

        if (npos) {

          bool M110 = strstr_P(command, PSTR("M110")) != nullptr;

          if (M110) {
            char* n2pos = strchr(command + 4, 'N');
            if (n2pos) npos = n2pos;
          }

          gcode_N = strtol(npos + 1, nullptr, 10);

          if (gcode_N != last_N + 1 && !M110)
            return gcode_line_error(PSTR(MSG_ERR_LINE_NO), i);

          char *apos = strrchr(command, '*');
          if (apos) {
            uint8_t checksum = 0, count = uint8_t(apos - command);
            while (count) checksum ^= command[--count];
            if (strtol(apos + 1, nullptr, 10) != checksum)
              return gcode_line_error(PSTR(MSG_ERR_CHECKSUM_MISMATCH), i);
          }
          else
            return gcode_line_error(PSTR(MSG_ERR_NO_CHECKSUM), i);

          last_N = gcode_N;
        }
        #if ENABLED(SDSUPPORT)
          // Pronterface "M29" and "M29 " has no line number
          else if (card.flag.saving && !is_M29(command))
            return gcode_line_error(PSTR(MSG_ERR_NO_CHECKSUM), i);
        #endif

        // Movement commands alert when stopped
        if (IsStopped()) {
          char* gpos = strchr(command, 'G');
          if (gpos) {
            switch (strtol(gpos + 1, nullptr, 10)) {
              case 0:
              case 1:
              #if ENABLED(ARC_SUPPORT)
                case 2:
                case 3:
              #endif
              #if ENABLED(BEZIER_CURVE_SUPPORT)
                case 5:
              #endif
                SERIAL_ECHOLNPGM(MSG_ERR_STOPPED);
                LCD_MESSAGEPGM(MSG_STOPPED);
                break;
            }
          }
        }

        #if DISABLED(EMERGENCY_PARSER)
          // Process critical commands early
          if (strcmp(command, "M108") == 0) {
            wait_for_heatup = false;
            #if HAS_LCD_MENU
              wait_for_user = false;
            #endif
          }
          if (strcmp(command, "M112") == 0) kill();
          if (strcmp(command, "M410") == 0) quickstop_stepper();
        #endif

        #if defined(NO_TIMEOUTS) && NO_TIMEOUTS > 0
          last_command_time = ms;
        #endif

        // Add the command to the queue
        _enqueue(serial_line_buffer[i], true
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
        if ((c = read_serial(i)) >= 0 && !serial_comment_mode[i]
          #if ENABLED(PAREN_COMMENTS)
            && !serial_comment_paren_mode[i]
          #endif
        )
          serial_line_buffer[i][serial_count[i]++] = (char)c;
      }
      else { // it's not a newline, carriage return or escape char
        if (serial_char == ';') serial_comment_mode[i] = true;
        #if ENABLED(PAREN_COMMENTS)
          else if (serial_char == '(') serial_comment_paren_mode[i] = true;
          else if (serial_char == ')') serial_comment_paren_mode[i] = false;
        #endif
        else if (!serial_comment_mode[i]
          #if ENABLED(PAREN_COMMENTS)
            && ! serial_comment_paren_mode[i]
          #endif
        ) serial_line_buffer[i][serial_count[i]++] = serial_char;
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
  inline void GCodeQueue::get_sdcard_commands() {
    static bool stop_buffering = false,
                sd_comment_mode = false
                #if ENABLED(PAREN_COMMENTS)
                  , sd_comment_paren_mode = false
                #endif
              ;

    if (!IS_SD_PRINTING()) return;

    /**
     * '#' stops reading from SD to the buffer prematurely, so procedural
     * macro calls are possible. If it occurs, stop_buffering is triggered
     * and the buffer is run dry; this character _can_ occur in serial com
     * due to checksums, however, no checksums are used in SD printing.
     */

    if (length == 0) stop_buffering = false;

    uint16_t sd_count = 0;
    bool card_eof = card.eof();
    while (length < BUFSIZE && !card_eof && !stop_buffering) {
      const int16_t n = card.get();
      char sd_char = (char)n;
      card_eof = card.eof();
      if (card_eof || n == -1
          || sd_char == '\n' || sd_char == '\r'
          || ((sd_char == '#' || sd_char == ':') && !sd_comment_mode
            #if ENABLED(PAREN_COMMENTS)
              && !sd_comment_paren_mode
            #endif
          )
      ) {
        if (card_eof) {

          card.printingHasFinished();

          if (IS_SD_PRINTING())
            sd_count = 0; // If a sub-file was printing, continue from call point
          else {
            SERIAL_ECHOLNPGM(MSG_FILE_PRINTED);
            #if ENABLED(PRINTER_EVENT_LEDS)
              printerEventLEDs.onPrintCompleted();
              #if HAS_RESUME_CONTINUE
                inject_P(PSTR("M0 S"
                  #if HAS_LCD_MENU
                    "1800"
                  #else
                    "60"
                  #endif
                ));
              #endif
            #endif // PRINTER_EVENT_LEDS
          }
        }
        else if (n == -1)
          SERIAL_ERROR_MSG(MSG_SD_ERR_READ);

        if (sd_char == '#') stop_buffering = true;

        sd_comment_mode = false; // for new command
        #if ENABLED(PAREN_COMMENTS)
          sd_comment_paren_mode = false;
        #endif

        // Skip empty lines and comments
        if (!sd_count) { thermalManager.manage_heater(); continue; }

        buffer[index_w][sd_count] = '\0'; // terminate string
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
        #if ENABLED(PAREN_COMMENTS)
          else if (sd_char == '(') sd_comment_paren_mode = true;
          else if (sd_char == ')') sd_comment_paren_mode = false;
        #endif
        else if (!sd_comment_mode
          #if ENABLED(PAREN_COMMENTS)
            && ! sd_comment_paren_mode
          #endif
        ) buffer[index_w][sd_count++] = sd_char;
      }
    }
  }

#endif // SDSUPPORT

/**
 * Add to the circular command queue the next command from:
 *  - The command-injection queue (injected_commands_P)
 *  - The active serial input (usually USB)
 *  - The SD card file being actively printed
 */
void GCodeQueue::get_available_commands() {

  get_serial_commands();

  #if ENABLED(SDSUPPORT)
    get_sdcard_commands();
  #endif
}

/**
 * Get the next command in the queue, optionally log it to SD, then dispatch it
 */
void GCodeQueue::advance() {

  // Process immediate commands
  if (process_injected_command()) return;

  // Return if the G-code buffer is empty
  if (!length) return;

  #if ENABLED(SDSUPPORT)

    if (card.flag.saving) {
      char* command = buffer[index_r];
      if (is_M29(command)) {
        // M29 closes the file
        card.closefile();
        SERIAL_ECHOLNPGM(MSG_FILE_SAVED);

        #if !defined(__AVR__) || !defined(USBCON)
          #if ENABLED(SERIAL_STATS_DROPPED_RX)
            SERIAL_ECHOLNPAIR("Dropped bytes: ", MYSERIAL0.dropped());
          #endif

          #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
            SERIAL_ECHOLNPAIR("Max RX Queue Size: ", MYSERIAL0.rxMaxEnqueued());
          #endif
        #endif //  !defined(__AVR__) || !defined(USBCON)

        ok_to_send();
      }
      else {
        // Write the string from the read buffer to SD
        card.write_command(command);
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
  if (length) {
    --length;
    if (++index_r >= BUFSIZE) index_r = 0;
  }

}
