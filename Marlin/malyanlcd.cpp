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
 * malyanlcd.cpp
 *
 * LCD implementation for Malyan's LCD, a separate ESP8266 MCU running
 * on Serial1 for the M200 board. This module outputs a pseudo-gcode
 * wrapped in curly braces which the LCD implementation translates into
 * actual G-code commands.
 *
 * Added to Marlin for Mini/Malyan M200
 * Unknown commands as of Jan 2018: {H:}
 * Not currently implemented:
 * {E:} when sent by LCD. Meaning unknown.
 *
 * Notes for connecting to boards that are not Malyan:
 * The LCD is 3.3v, so if powering from a RAMPS 1.4 board or
 * other 5v/12v board, use a buck converter to power the LCD and
 * the 3.3v side of a logic level shifter. Aux1 on the RAMPS board
 * has Serial1 and 12v, making it perfect for this.
 * Copyright (c) 2017 Jason Nelson (xC0000005)
 */

#include "MarlinConfig.h"

#if ENABLED(MALYAN_LCD)

#include "cardreader.h"
#include "SdFatConfig.h"
#include "temperature.h"
#include "planner.h"
#include "stepper.h"
#include "duration_t.h"
#include "printcounter.h"
#include "parser.h"
#include "configuration_store.h"

#include "Marlin.h"

// On the Malyan M200, this will be Serial1. On a RAMPS board,
// it might not be.
#define LCD_SERIAL Serial1

// This is based on longest sys command + a filename, plus some buffer
// in case we encounter some data we don't recognize
// There is no evidence a line will ever be this long, but better safe than sorry
#define MAX_CURLY_COMMAND (32 + LONG_FILENAME_LENGTH) * 2

// Track incoming command bytes from the LCD
int inbound_count;

// Everything written needs the high bit set.
void write_to_lcd_P(const char * const message) {
  char encoded_message[MAX_CURLY_COMMAND];
  uint8_t message_length = min(strlen_P(message), sizeof(encoded_message));

  for (uint8_t i = 0; i < message_length; i++)
    encoded_message[i] = pgm_read_byte(&message[i]) | 0x80;

  LCD_SERIAL.Print::write(encoded_message, message_length);
}

void write_to_lcd(const char * const message) {
  char encoded_message[MAX_CURLY_COMMAND];
  const uint8_t message_length = min(strlen(message), sizeof(encoded_message));

  for (uint8_t i = 0; i < message_length; i++)
    encoded_message[i] = message[i] | 0x80;

  LCD_SERIAL.Print::write(encoded_message, message_length);
}

/**
 * Process an LCD 'C' command.
 * These are currently all temperature commands
 * {C:T0190}
 * Set temp for hotend to 190
 * {C:P050}
 * Set temp for bed to 50
 *
 * the command portion begins after the :
 */
void process_lcd_c_command(const char* command) {
  switch (command[0]) {
    case 'T': {
      // M104 S<temperature>
      char cmd[20];
      sprintf_P(cmd, PSTR("M104 S%s"), command + 1);
      enqueue_and_echo_command_now(cmd, false);
    } break;

    case 'P': {
      // M140 S<temperature>
      char cmd[20];
      sprintf_P(cmd, PSTR("M140 S%s"), command + 1);
      enqueue_and_echo_command_now(cmd, false);
    } break;

    default:
      SERIAL_ECHOLNPAIR("UNKNOWN C COMMAND", command);
      return;
  }
}

/**
 * Process an LCD 'B' command.
 * {B:0} results in: {T0:008/195}{T1:000/000}{TP:000/000}{TQ:000C}{TT:000000}
 * T0/T1 are hot end temperatures, TP is bed, TQ is percent, and TT is probably
 * time remaining (HH:MM:SS). The UI can't handle displaying a second hotend,
 * but the stock firmware always sends it, and it's always zero.
 */
void process_lcd_eb_command(const char* command) {
  char elapsed_buffer[10];
  duration_t elapsed;
  bool has_days;
  uint8_t len;
  switch (command[0]) {
    case '0': {
      elapsed = print_job_timer.duration();
      sprintf_P(elapsed_buffer, PSTR("%02u%02u%02u"), uint16_t(elapsed.hour()), uint16_t(elapsed.minute()) % 60UL, elapsed.second());

      char message_buffer[MAX_CURLY_COMMAND];
      sprintf_P(message_buffer,
              PSTR("{T0:%03.0f/%03i}{T1:000/000}{TP:%03.0f/%03i}{TQ:%03i}{TT:%s}"),
              thermalManager.degHotend(0),
              thermalManager.degTargetHotend(0),
              thermalManager.degBed(),
              thermalManager.degTargetBed(),
              card.percentDone(),
              elapsed_buffer);
      write_to_lcd(message_buffer);
    } break;

    default:
      SERIAL_ECHOLNPAIR("UNKNOWN E/B COMMAND", command);
      return;
  }
}

/**
 * Process an LCD 'J' command.
 * These are currently all movement commands.
 * The command portion begins after the :
 * Move X Axis
 *
 * {J:E}{J:X-200}{J:E}
 * {J:E}{J:X+200}{J:E}
 * X, Y, Z, A (extruder)
 */
void process_lcd_j_command(const char* command) {
  static bool steppers_enabled = false;
  char axis = command[0];

  switch (axis) {
    case 'E':
      // enable or disable steppers
      // switch to relative
      enqueue_and_echo_command_now("G91");
      enqueue_and_echo_command_now(steppers_enabled ? "M18" : "M17");
      steppers_enabled = !steppers_enabled;
      break;
    case 'A':
      axis = 'E';
      // fallthru
    case 'Y':
    case 'Z':
    case 'X': {
      // G0 <AXIS><distance>
      // The M200 class UI seems to send movement in .1mm values.
      char cmd[20];
      sprintf_P(cmd, PSTR("G1 %c%03.1f"), axis, atof(command + 1) / 10.0);
      enqueue_and_echo_command_now(cmd);
    } break;
    default:
      SERIAL_ECHOLNPAIR("UNKNOWN J COMMAND", command);
      return;
  }
}

/**
 * Process an LCD 'P' command, related to homing and printing.
 * Cancel:
 * {P:X}
 *
 * Home all axes:
 * {P:H}
 *
 * Print a file:
 * {P:000}
 * The File number is specified as a three digit value.
 * Printer responds with:
 * {PRINTFILE:Mini_SNES_Bottom.gcode}
 * {SYS:BUILD}echo:Now fresh file: Mini_SNES_Bottom.gcode
 * File opened: Mini_SNES_Bottom.gcode Size: 5805813
 * File selected
 * {SYS:BUILD}
 * T:-2526.8 E:0
 * T:-2533.0 E:0
 * T:-2537.4 E:0
 * Note only the curly brace stuff matters.
 */
void process_lcd_p_command(const char* command) {

  switch (command[0]) {
    case 'X':
      // cancel print
      write_to_lcd_P(PSTR("{SYS:CANCELING}"));
      card.stopSDPrint(
        #if SD_RESORT
          true
        #endif
      );
      clear_command_queue();
      quickstop_stepper();
      print_job_timer.stop();
      thermalManager.disable_all_heaters();
      #if FAN_COUNT > 0
        for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
      #endif
      wait_for_heatup = false;
      write_to_lcd_P(PSTR("{SYS:STARTED}"));
      break;
    case 'H':
      // Home all axis
      enqueue_and_echo_command_now("G28");
      break;
    default: {
      // Print file 000 - a three digit number indicating which
      // file to print in the SD card. If it's a directory,
      // then switch to the directory.

      // Find the name of the file to print.
      // It's needed to echo the PRINTFILE option.
      // The {S:L} command should've ensured the SD card was mounted.
      card.getfilename(atoi(command));

      // There may be a difference in how V1 and V2 LCDs handle subdirectory
      // prints. Investigate more. This matches the V1 motion controller actions
      // but the V2 LCD switches to "print" mode on {SYS:DIR} response.
      if (card.filenameIsDir) {
        card.chdir(card.filename);
        write_to_lcd_P(PSTR("{SYS:DIR}"));
      }
      else {
        char message_buffer[MAX_CURLY_COMMAND];
        sprintf_P(message_buffer, PSTR("{PRINTFILE:%s}"), card.filename);
        write_to_lcd(message_buffer);
        write_to_lcd_P(PSTR("{SYS:BUILD}"));
        card.openAndPrintFile(card.filename);
      }
    } break; // default
  } // switch
}

/**
 * Handle an lcd 'S' command
 * {S:I} - Temperature request
 * {T0:999/000}{T1:000/000}{TP:004/000}
 *
 * {S:L} - File Listing request
 * Printer Response:
 * {FILE:buttons.gcode}
 * {FILE:update.bin}
 * {FILE:nupdate.bin}
 * {FILE:fcupdate.flg}
 * {SYS:OK}
 */
void process_lcd_s_command(const char* command) {
  switch (command[0]) {
    case 'I': {
      // temperature information
      char message_buffer[MAX_CURLY_COMMAND];
      sprintf_P(message_buffer, PSTR("{T0:%03.0f/%03i}{T1:000/000}{TP:%03.0f/%03i}"),
        thermalManager.degHotend(0), thermalManager.degTargetHotend(0),
        thermalManager.degBed(), thermalManager.degTargetBed()
      );
      write_to_lcd(message_buffer);
    } break;

    case 'H':
      // Home all axis
      enqueue_and_echo_command("G28", false);
      break;

    case 'L': {
      if (!card.cardOK) card.initsd();

      // A more efficient way to do this would be to
      // implement a callback in the ls_SerialPrint code, but
      // that requires changes to the core cardreader class that
      // would not benefit the majority of users. Since one can't
      // select a file for printing during a print, there's
      // little reason not to do it this way.
      char message_buffer[MAX_CURLY_COMMAND];
      uint16_t file_count = card.get_num_Files();
      for (uint16_t i = 0; i < file_count; i++) {
        card.getfilename(i);
        sprintf_P(message_buffer, card.filenameIsDir ? PSTR("{DIR:%s}") : PSTR("{FILE:%s}"), card.filename);
        write_to_lcd(message_buffer);
      }

      write_to_lcd_P(PSTR("{SYS:OK}"));
    } break;

    default:
      SERIAL_ECHOLNPAIR("UNKNOWN S COMMAND", command);
      return;
  }
}

/**
 * Receive a curly brace command and translate to G-code.
 * Currently {E:0} is not handled. Its function is unknown,
 * but it occurs during the temp window after a sys build.
 */
void process_lcd_command(const char* command) {
  const char *current = command;

  current++; // skip the leading {. The trailing one is already gone.
  byte command_code = *current++;
  if (*current != ':') {
    SERIAL_ECHOLNPAIR("UNKNOWN COMMAND FORMAT", command);
    return;
  }

  current++; // skip the :

  switch (command_code) {
    case 'S':
      process_lcd_s_command(current);
      break;
    case 'J':
      process_lcd_j_command(current);
      break;
    case 'P':
      process_lcd_p_command(current);
      break;
    case 'C':
      process_lcd_c_command(current);
      break;
    case 'B':
    case 'E':
      process_lcd_eb_command(current);
      break;
    default:
      SERIAL_ECHOLNPAIR("UNKNOWN COMMAND", command);
      return;
  }
}

/**
 * UC means connected.
 * UD means disconnected
 * The stock firmware considers USB initialied as "connected."
 */
void update_usb_status(const bool forceUpdate) {
  static bool last_usb_connected_status = false;
  // This is mildly different than stock, which
  // appears to use the usb discovery status.
  // This is more logical.
  if (last_usb_connected_status != Serial || forceUpdate) {
    last_usb_connected_status =  Serial;
    write_to_lcd_P(last_usb_connected_status ? PSTR("{R:UC}\r\n") : PSTR("{R:UD}\r\n"));
  }
}

/**
 * - from printer on startup:
 * {SYS:STARTED}{VER:29}{SYS:STARTED}{R:UD}
 * The optimize attribute fixes a register Compile
 * error for amtel.
 */
void lcd_update() _O2 {
  static char inbound_buffer[MAX_CURLY_COMMAND];

  // First report USB status.
  update_usb_status(false);

  // now drain commands...
  while (LCD_SERIAL.available()) {
    const byte b = (byte)LCD_SERIAL.read() & 0x7F;
    inbound_buffer[inbound_count++] = b;
    if (b == '}' || inbound_count == sizeof(inbound_buffer) - 1) {
      inbound_buffer[inbound_count - 1] = '\0';
      process_lcd_command(inbound_buffer);
      inbound_count = 0;
      inbound_buffer[0] = 0;
    }
  }

  // If there's a print in progress, we need to emit the status as
  // {TQ:<PERCENT>}
  if (card.sdprinting) {
    // We also need to send: T:-2538.0 E:0
    // I have no idea what this means.
    char message_buffer[10];
    sprintf_P(message_buffer, PSTR("{TQ:%03i}"), card.percentDone());
    write_to_lcd(message_buffer);
  }
}

/**
 * The Malyan LCD actually runs as a separate MCU on Serial 1.
 * This code's job is to siphon the weird curly-brace commands from
 * it and translate into gcode, which then gets injected into
 * the command queue where possible.
 */
void lcd_init() {
  inbound_count = 0;
  LCD_SERIAL.begin(500000);

  // Signal init
  write_to_lcd_P(PSTR("{SYS:STARTED}\r\n"));

  // send a version that says "unsupported"
  write_to_lcd_P(PSTR("{VER:99}\r\n"));

  // No idea why it does this twice.
  write_to_lcd_P(PSTR("{SYS:STARTED}\r\n"));
  update_usb_status(true);
}

/**
 * Set an alert.
 */
void lcd_setalertstatusPGM(const char* message) {
  char message_buffer[MAX_CURLY_COMMAND];
  sprintf_P(message_buffer, PSTR("{E:%s}"), message);
  write_to_lcd(message_buffer);
}

#endif // MALYAN_LCD
