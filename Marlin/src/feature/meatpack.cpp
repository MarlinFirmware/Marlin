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
 * MeatPack G-Code Compression
 *
 * Algorithm & Implementation: Scott Mudge - mail@scottmudge.com
 * Date: Dec. 2020
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(MEATPACK)

#include "meatpack.h"

MeatPack meatpack;

//#define MP_DEBUG
#define DEBUG_OUT ENABLED(MP_DEBUG)
#include "../core/debug_out.h"

// Utility definitions
#define MeatPack_CommandByte      0b11111111
#define MeatPack_NextPackedFirst  0b00000001
#define MeatPack_NextPackedSecond 0b00000010

#define MeatPack_SpaceCharIdx       11U
#define MeatPack_SpaceCharReplace   'E'

/*
  Character Frequencies from ~30 MB of comment-stripped gcode:
   '1' -> 4451136
   '0' -> 4253577
   ' ' -> 3053297
   '.' -> 3035310
   '2' -> 1523296
   '8' -> 1366812
   '4' -> 1353273
   '9' -> 1352147
   '3' -> 1262929
   '5' -> 1189871
   '6' -> 1127900
   '7' -> 1112908
  '\n' -> 1087683
   'G' -> 1075806
   'X' ->  975742
   'E' ->  965275
   'Y' ->  965274
   'F' ->   99416
   '-' ->   90242
   'Z' ->   34109
   'M' ->   11879
   'S' ->    9910
   If spaces are omitted, we add 'E'
*/

// Note:
// I've tried both a switch/case method and a lookup table. The disassembly is exactly the same after compilation, byte-to-byte.
// Thus, performance is identical.
#define USE_LOOKUP_TABLE

#ifdef USE_LOOKUP_TABLE
  // The 15 most-common characters used in G-code, ~90-95% of all G-code uses these characters
  // NOT storing this with PROGMEM, given how frequently this table will be accessed.
  uint8_t MeatPackLookupTbl[16] = {
    '0', // 0000
    '1', // 0001
    '2', // 0010
    '3', // 0011
    '4', // 0100
    '5', // 0101
    '6', // 0110
    '7', // 0111
    '8', // 1000
    '9', // 1001
    '.', // 1010
    ' ', // 1011
    '\n',// 1100
    'G', // 1101
    'X', // 1110
    '\0' // never used, 0b1111 is used to indicate next 8-bits is a full character
  };

#else

  inline uint8_t get_char(register const uint8_t in) {
    switch (in) {
      case 0b0000 ... 0b1001: return '0' + in;
      case 0b1010: return '.';
      case 0b1011: return ' ';
      case 0b1100: return '\n';
      case 0b1101: return 'G';
      case 0b1110: return 'X';
    }
    return 0;
  }

#endif

// State variables
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool MeatPack::cmd_is_next = false;       // A command is pending
uint8_t MeatPack::config = MPConfig_None; // Configuration state
uint8_t MeatPack::char_buf = 0;           // Buffers a character if dealing with out-of-sequence pairs
uint8_t MeatPack::cmd_count = 0;          // Counts how many command bytes are received (need 2)
uint8_t MeatPack::full_char_count = 0;    // Counts how many full-width characters are to be received
uint8_t MeatPack::char_out_buf[2];        // Output buffer for caching up to 2 characters
uint8_t MeatPack::char_out_count = 0;     // Stores number of characters to be read out.

// DEBUGGING
TERN_(MP_DEBUG, uint32_t chars_decoded = 0);

void MeatPack::reset_state() {
  SERIAL_ECHOLNPGM("MP Reset");

  cmd_is_next = false;
  config = MPConfig_None;
  char_buf = 0;
  cmd_count = 0;
  full_char_count = 0;
  char_out_count = 0;

  TERN_(MP_DEBUG, chars_decoded = 0);
}

// Storing
// packed = ((low & 0xF) << 4) | (high & 0xF);

// Unpacking
// low = (packed >> 4) & 0xF;
// high = (packed & 0xF);

uint8_t MeatPack::unpack_chars(const uint8_t pk, uint8_t* __restrict const chars_out) {
  register uint8_t out = 0;

  #ifdef USE_LOOKUP_TABLE

    // If lower nybble is 1111, the higher nybble is unused, and next char is full.
    if ((pk & MeatPack_FirstNotPacked) == MeatPack_FirstNotPacked) out |= MeatPack_NextPackedFirst;
    else chars_out[0] = MeatPackLookupTbl[pk & 0xF]; // Assign lower char

    // Check if upper nybble is 1111... if so, we don't need the second char.
    if ((pk & MeatPack_SecondNotPacked) == MeatPack_SecondNotPacked) out |= MeatPack_NextPackedSecond;
    else chars_out[1] = MeatPackLookupTbl[(pk >> 4) & 0xF]; // Assign upper char

  #else
    // If lower nybble is 1111, the higher nybble is unused, and next char is full.
    if ((pk & MeatPack_FirstNotPacked) == MeatPack_FirstNotPacked) out |= MeatPack_NextPackedFirst;
    else chars_out[0] = get_char(pk & 0xF); // Assign lower char

    // Check if upper nybble is 1111... if so, we don't need the second char.
    if ((pk & MeatPack_SecondNotPacked) == MeatPack_SecondNotPacked) out |= MeatPack_NextPackedSecond;
    else chars_out[1] = get_char((pk >> 4) & 0xF); // Assign upper char

  #endif

  return out;
}

void MeatPack::handle_output_char(const uint8_t c) {
  char_out_buf[char_out_count++] = c;

  #if ENABLED(MP_DEBUG)
    if (chars_decoded < 64) {
      ++chars_decoded;
      DEBUG_ECHOPGM("Rec Byte: ");
      MYSERIAL.print("0x");
      MYSERIAL.print((uint8_t)c, HEX);
      DEBUG_EOL();
    }
  #endif
}

void MeatPack::handle_rx_char_inner(const uint8_t c) {

  // Packing enabled, handle character and re-arrange them appropriately.
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (config & MPConfig_Active) {
    if (full_char_count) {
      handle_output_char(c);
      if (char_buf) {
        handle_output_char(char_buf);
        char_buf = 0;
      }
      --full_char_count;
    }
    else {
      uint8_t buf[2] = { 0, 0 };
      register const uint8_t res = unpack_chars(c, buf);

      if (res & MeatPack_NextPackedFirst) {
        ++full_char_count;
        if (res & MeatPack_NextPackedSecond) ++full_char_count;
        else char_buf = buf[1];
      }
      else {
        handle_output_char(buf[0]);
        if (res & MeatPack_NextPackedSecond) ++full_char_count;
        else handle_output_char(buf[1]);
      }
    }
  }
  else // Packing not enabled, just copy character to output
    handle_output_char(c);
}

void MeatPack::echo_config_state() {
  SERIAL_ECHOPGM("[MP] ");

  // Echo current state
  serialprint_onoff(config & MPConfig_Active);

  SERIAL_CHAR(' ');

  if (config & MPConfig_NoSpaces)
    SERIAL_ECHOPGM("NSP"); // [N]o [SP]aces
  else
    SERIAL_ECHOPGM("ESP"); // [E]nabled [SP]aces

  SERIAL_EOL();

  // Validate config vars
  MeatPackLookupTbl[MeatPack_SpaceCharIdx] = (config & MPConfig_NoSpaces) ? MeatPack_SpaceCharReplace : ' ';
}

void MeatPack::handle_cmd(const MeatPack_Command c) {
  switch (c) {
    case MPCommand_EnablePacking:   config |= MPConfig_Active;      DEBUG_ECHOLNPGM("[MPDBG] ENABL REC"); break;
    case MPCommand_DisablePacking:  config &= ~(MPConfig_Active);   DEBUG_ECHOLNPGM("[MPDBG] DISBL REC"); break;
    case MPCommand_TogglePacking:   config ^= MPConfig_Active;      DEBUG_ECHOLNPGM("[MPDBG] TGL REC");   break;
    case MPCommand_ResetAll:        reset_state();                  DEBUG_ECHOLNPGM("[MPDBG] RESET REC"); break;
    case MPCommand_EnableNoSpaces:  config |= MPConfig_NoSpaces;    DEBUG_ECHOLNPGM("[MPDBG] ENABL NSP"); break;
    case MPCommand_DisableNoSpaces: config &= ~(MPConfig_NoSpaces); DEBUG_ECHOLNPGM("[MPDBG] DISBL NSP"); break;
    default:                                                        DEBUG_ECHOLNPGM("[MPDBG] UNK CMD REC");
    case MPCommand_QueryConfig: break;
  }
  echo_config_state();
}

void MeatPack::handle_rx_char(const uint8_t c) {
  // Check for commit complete
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (c == uint8_t(MeatPack_CommandByte)) {
    if (!cmd_count)
      ++cmd_count; // cmd_count = 1
    else {
      cmd_count = 0;
      cmd_is_next = true;
    }
    return;
  }

  if (cmd_is_next) {
    cmd_is_next = false;
    handle_cmd((MeatPack_Command)c);
    return;
  }

  if (cmd_count) {
    cmd_count = 0;
    handle_rx_char_inner(uint8_t(MeatPack_CommandByte));
  }

  handle_rx_char_inner(c);
}

uint8_t MeatPack::get_result_char(char* const __restrict out) {
  uint8_t res = 0;
  if (char_out_count) {
    res = char_out_count;
    char_out_count = 0;
    for (register uint8_t i = 0; i < res; ++i)
      out[i] = (char)char_out_buf[i];
  }
  return res;
}

void MeatPack::trigger_cmd(const MeatPack_Command cmd) { handle_cmd(cmd); }

#endif // MEATPACK
