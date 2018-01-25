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
 * LCD implementation for Malyan's LCD firmware, which is a separate
 * ESP8266 MCU running on Serial1 for the M200 board.
 * This module outputs a pseudo-gcode wrapped in curly braces
 * which the lcd implementation translates into actual GCODE commands
 *
 * Added to Marlin for Mini/Malyan malyanM200
 * Unknown commands as of Jan/2018: {H:}
 * Not currently implemented:
 * {E:} when sent by LCD. Meaning unknown.
 *
 * Notes for connecting to boards that are not Maylan:
 * The LCD is 3.3v, so if powering from a RAMPS 1.4 board or
 * other 5v/12v board, use a buck converter to power the LCD and
 * the 3.3v side of a logic level shifter. Aux1 on the RAMPS board
 * has serial1 and 12v, making it perfect for this.
 * Copyright (c) 2017 Jason Nelson (xC0000005)
 */

#include "../inc/MarlinConfig.h"
#include "../sd/cardreader.h"
#include "../sd/SdFatConfig.h"
#include "../module/temperature.h"
#include "../module/planner.h"
#include "../module/stepper.h"
#include "../module/motion.h"
#include "../module/probe.h"
#include "../libs/duration_t.h"
#include "../module/printcounter.h"
#include "../gcode/gcode.h"
#include "../gcode/queue.h"
#include "../module/configuration_store.h"

#include "../Marlin.h"

#if ENABLED(MALYAN_LCD)

  // On the malyan M200, this will be serial1. On a RAMPS board,
  // it might not be.
  #define LCD_SERIAL Serial1

  // This is based on longest sys command + a filename, plus some buffer
  // in case we encounter some data we don't recognize
  #define MAX_CURLY_COMMAND (32 + LONG_FILENAME_LENGTH) * 2

  // For processing the E:J command (which enables/disables steppers)
  bool steppers_enabled = false;

  // Whether or not we were connected at last status.
  bool last_usb_connected_status = false;

  // There is no evidence a line will ever be this long, but better safe than sory
  char inbound_buffer[MAX_CURLY_COMMAND];
  int inbound_count;

  int file_count;

  // for building LCD messages
  char message_buffer[MAX_CURLY_COMMAND];

  // Marlin function used to queue commands for exectuion
  extern bool enqueue_and_echo_command(const char* cmd, bool say_ok/*=false*/);

  // Everything written needs the high bit set.
  void write_to_lcd(const char* message) {
    // Note we can't re-use the message_buffer because it is probably being passed.
    char encoded_message[MAX_CURLY_COMMAND];
    int message_length = min(strlen(message), sizeof(encoded_message));

    for (int i = 0; i < message_length; i++)
      encoded_message[i] = message[i] | 0x80;

    LCD_SERIAL.Print::write(encoded_message, message_length);
  }

  /* Process a command that begins with C from the LCD.
  These are currently all temperature commands
  {C:T0190}
  Set temp for hotend to 190
  {C:P050}
  Set temp for bed to 50

  the command portion begins after the :
  */
  void process_lcd_c_command(const char* command) {
    switch(command[0]) {
      case 'T':
        // M104 S<temperature>
        sprintf(message_buffer, "M104 S%s", command + 1);
        enqueue_and_echo_command(message_buffer, false);
        break;
      case 'P':
        // M140 S<temperature>
        sprintf(message_buffer, "M140 S%s", command + 1);
        enqueue_and_echo_command(message_buffer, false);
        break;
      default:
        SERIAL_ECHOLNPAIR("UNKNOWN C COMMAND", command);
        return;
    }
  }

  /* Process a command that begins with B from the LCD.
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
    switch(command[0]) {
      case '0':
        elapsed = print_job_timer.duration();
        sprintf(elapsed_buffer, "%02u%02u%02u", uint16_t(elapsed.hour()), uint16_t(elapsed.minute()) % 60UL, elapsed.second());

        sprintf(message_buffer,
                "{T0:%03.0f/%03i}{T1:000/000}{TP:%03.0f/%03i}{TQ:%03i}{TT:%s}",
                thermalManager.degHotend(0),
                thermalManager.degTargetHotend(0),
                thermalManager.degBed(),
                thermalManager.degTargetBed(),
                card.percentDone(),
                elapsed_buffer);
        write_to_lcd(message_buffer);
        break;
      default:
        SERIAL_ECHOLNPAIR("UNKNOWN E/B COMMAND", command);
        return;
    }
  }

  /* Process a command that begins with j from the LCD.
  These are currently all movement commands
  the command portion begins after the :
  Move X Axis

  {J:E}{J:X-200}{J:E}
  {J:E}{J:X+200}{J:E}
  X, Y, Z, A (extruder)
  */
  void process_lcd_j_command(const char* command) {
    char axis = command[0];
    float value = 0;

    switch(axis) {
      case 'E':
        // enable steppers or disable
        // switch to relative
        enqueue_and_echo_command("G91", false);

        if (!steppers_enabled)
          enqueue_and_echo_command("M17", false);
        else
          enqueue_and_echo_command("M18", false);

        steppers_enabled = !steppers_enabled;
        break;
      case 'A':
        axis = 'E';
        //now fall through
      case 'Y':
      case 'Z':
      case 'X':
        // G0 <AXIS><distance>
        // The M200 class UI seems to send movement in .1mm values.
        value = atof(command+1);
        value = value / 10;

        sprintf(message_buffer, "G1 %c%03.1f", axis, value);
        enqueue_and_echo_command(message_buffer, false);
        break;
      default:
        SERIAL_ECHOLNPAIR("UNKNOWN J COMMAND", command);
        return;
    }
  }

/*
  processes LCD p commands, related to homing and printing.
  Cancel:
  {P:X}

  Home all axis:
  {P:H}

  Print a file:
  {P:000}
  The File number is specified as a three digit value.
  Printer responds with:
  {PRINTFILE:Mini_SNES_Bottom.gcode}
  {SYS:BUILD}echo:Now fresh file: Mini_SNES_Bottom.gcode
  File opened: Mini_SNES_Bottom.gcode Size: 5805813
  File selected
  {SYS:BUILD}
  T:-2526.8 E:0
  T:-2533.0 E:0
  T:-2537.4 E:0
  Note only the curly brace stuff matters.
*/
  void process_lcd_p_command(const char* command) {

    switch(command[0]) {
      case 'X':
        // cancel print
        write_to_lcd("{SYS:CANCELING}");
        clear_command_queue();
        quickstop_stepper();
        print_job_timer.stop();
        thermalManager.disable_all_heaters();
        #if FAN_COUNT > 0
          for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
        #endif
        wait_for_heatup = false;
        write_to_lcd("{SYS:STARTED}");
        break;
      case 'H':
        // Home all axis
        enqueue_and_echo_command("G28", false);
        break;
      default:
        // Print file 000 - a three digit number indicating which
        // file to print in the SD card. If it's a directory,
        // then switch to the directory.
        int fileIndex = atoi(command);

        // find the name of the file tor print.
        // It's needed to echo the PRINTFILE option.
        // The {S:L} command should have ensured the SD card was mounted
        card.getfilename(fileIndex);

        // There may be a difference in how V1 and V2 LCDs handle subdirectory
        // prints. Investigate more. This matches the V1 motion controller actions
        // but the V2 LCD switches to "print" mode on {SYS:DIR} response.
        if (card.filenameIsDir) {
          card.chdir(card.filename);
          write_to_lcd("{SYS:DIR}");
        }
        else {
          sprintf(message_buffer, "{PRINTFILE:%s}", card.filename);
          write_to_lcd(message_buffer);
          write_to_lcd("{SYS:BUILD}");
          card.openAndPrintFile(card.filename);
        }
        return;
    }
  }

  /*
  Handles lcd codes that begin with S
  {S:I} - DIsplay asks for temperature.
  {T0:999/000}{T1:000/000}{TP:004/000}

  Ask for list of files:
  {S:L}
  Printer Response:
  {FILE:buttons.gcode}
  {FILE:update.bin}
  {FILE:nupdate.bin}
  {FILE:fcupdate.flg}
  {SYS:OK}
  */
  void process_lcd_s_command(const char* command) {
    switch(command[0]) {
      case 'I':
        // temperature information
        sprintf(
                message_buffer,
                "{T0:%03.0f/%03i}{T1:000/000}{TP:%03.0f/%03i}",
                thermalManager.degHotend(0),
                thermalManager.degTargetHotend(0),
                thermalManager.degBed(),
                thermalManager.degTargetBed());
        write_to_lcd(message_buffer);
        break;
      case 'H':
        // Home all axis
        enqueue_and_echo_command("G28", false);
        break;
      case 'L':
        if (!card.cardOK) {
          card.initsd();
        }

        // A more efficient way to do this would be to
        // implement a callback in the ls_SerialPrint code, but
        // that requires changes to the core cardreader class that
        // would not benefit the majority of users. Since one can't
        // select a file for printing during a print, there's
        // little reason not to do it this way.
        file_count = card.get_num_Files();
        for (int i = 0; i < file_count; i++) {
          card.getfilename(i);
          sprintf(message_buffer, card.filenameIsDir ? "{DIR:%s}" : "{FILE:%s}", card.filename);
          write_to_lcd(message_buffer);
        }

        write_to_lcd("{SYS:OK}");
        break;
      default:
        SERIAL_ECHOLNPAIR("UNKNOWN S COMMAND", command);
        return;
    }
  }

  // This receives curly brace commands and translates to gcode
  // Currently unhandled is the {E:0} command. The function of This
  // is unknown, but it occurs during the temp window after a sys build.
  void process_lcd_command(const char* command) {
    const char *current = command;

    current++; // skip the leading {. The trailing one is already gone.
    byte command_code = *current++;
    if (*current != ':') {
      SERIAL_ECHOLNPAIR("UNKNOWN COMMAND FORMAT", command);
      return;
    }

    current++; // skip the :

    switch(command_code) {
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

  /*
   UC means connected.
   UD means disconnected
   The stock firmware considers USB initialied as "connected."
  */
  void update_usb_status(bool forceUpdate) {
    // This is mildly different than stock, which
    // appears to use the usb discovery status.
    // This is more logical.
    if (last_usb_connected_status != Serial || forceUpdate) {
      last_usb_connected_status =  Serial;

      if (last_usb_connected_status)
        write_to_lcd("{R:UC}\r\n");
      else
        write_to_lcd("{R:UD}\r\n");
    }
  }

  /*
  * - from printer on startup:
  * {SYS:STARTED}{VER:29}{SYS:STARTED}{R:UD}
  * The optimize attribute fixes a register Compile
  * error for amtel.
  */
  void lcd_update() __attribute__((__optimize__("O2")));
  void lcd_update() {
    // First report USB status.
    update_usb_status(false);

    // now drain commands...
    while(LCD_SERIAL.available()) {
      byte b = (byte)LCD_SERIAL.read();
      b = b & 0x7f;
      inbound_buffer[inbound_count++] = b;
      if (b == '}' || inbound_count == (sizeof(inbound_buffer) - 1)) {
        inbound_buffer[inbound_count-1] = 0;
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
      sprintf(message_buffer, "{TQ:%03i}", card.percentDone());
      write_to_lcd(message_buffer);
    }
  }

#ifdef BOOT_LOADER_EXTRACTION
  /* dumps the boot loader to sd. */
  void dump_bootlader() {
    card.initsd();
  	card.openFile((char *)"LOADER.BIN",false);
    watchdog_reset();
    card.write (((byte*)(0x08000000)), 0x2000);
  	card.closefile();
    card.release();
  }
#endif

  // The malyan LCD actually runs as a separate MCU on Serial 1.
  // This code's job is to siphon the weird curly-brace commands from
  // it and translate into gcode, which then gets injected into
  // the command queue where possible.
  void lcd_init() {
    #ifdef BOOT_LOADER_EXTRACTION
      dump_bootlader();
    #endif

    inbound_count = 0;
    LCD_SERIAL.begin(500000);

    // Signal init
    write_to_lcd("{SYS:STARTED}\r\n");

    // send a version that says "unsuported"
    write_to_lcd("{VER:66}\r\n");

    // No idea why it does this twice.
    write_to_lcd("{SYS:STARTED}\r\n");
    update_usb_status(true);
  }

  bool lcd_hasstatus() { return false; }
  void lcd_setstatus(const char* const message, const bool persist=false) { UNUSED(message); UNUSED(persist); }
  void lcd_setstatusPGM(const char* const message, const int8_t level=0) { UNUSED(message); UNUSED(level); }

  /*
  Set an alert.
  */
  void lcd_setalertstatusPGM(const char* message) {
    sprintf(message_buffer, "{E:%s}", message);
    write_to_lcd(message_buffer);
  }

  void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) { UNUSED(level); UNUSED(fmt); }
  void lcd_buttons_update() {}
  void lcd_reset_alert_level() {}
  bool lcd_detected() { return true; }
  void lcd_refresh() {}

#endif // Malyan LCD
