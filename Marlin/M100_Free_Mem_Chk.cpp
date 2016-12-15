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
 * M100 Free Memory Watcher
 *
 * This code watches the free memory block between the bottom of the heap and the top of the stack.
 * This memory block is initialized and watched via the M100 command.
 *
 * M100 I Initializes the free memory block and prints vitals statistics about the area
 * M100 F Identifies how much of the free memory block remains free and unused.  It also
 *    detects and reports any corruption within the free memory block that may have
 *    happened due to errant firmware.
 * M100 D Does a hex display of the free memory block along with a flag for any errant
 *    data that does not match the expected value.
 * M100 C x Corrupts x locations within the free memory block.   This is useful to check the
 *    correctness of the M100 F and M100 D commands.
 *
 * Initial version by Roxy-3DPrintBoard
 */
#define M100_FREE_MEMORY_DUMPER     // Comment out to remove Dump sub-command
#define M100_FREE_MEMORY_CORRUPTOR    // Comment out to remove Corrupt sub-command

#include "Marlin.h"

#if ENABLED(M100_FREE_MEMORY_WATCHER)
extern char* __brkval;
extern size_t  __heap_start, __heap_end, __flp;
extern char __bss_end;

//
// Utility functions used by M100 to get its work done.
//

char* top_of_stack();
void prt_hex_nibble(unsigned int);
void prt_hex_byte(unsigned int);
void prt_hex_word(unsigned int);
int how_many_E5s_are_here(char*);

void gcode_M100() {
  static bool m100_not_initialized = true;
  char* sp, *ptr;
  int i, j, n;
  //
  // M100 D dumps the free memory block from __brkval to the stack pointer.
  // malloc() eats memory from the start of the block and the stack grows
  // up from the bottom of the block.    Solid 0xE5's indicate nothing has
  // used that memory yet.   There should not be anything but 0xE5's within
  // the block of 0xE5's.  If there is, that would indicate memory corruption
  // probably caused by bad pointers.  Any unexpected values will be flagged in
  // the right hand column to help spotting them.
  //
  #if ENABLED(M100_FREE_MEMORY_DUMPER) // Disable to remove Dump sub-command
    if (code_seen('D')) {
      ptr = __brkval ? __brkval : &__bss_end;
      //
      // We want to start and end the dump on a nice 16 byte boundry even though
      // the values we are using are not 16 byte aligned.
      //
      SERIAL_ECHOPGM("\nbss_end : ");
      prt_hex_word((unsigned int) ptr);
      ptr = (char*)((unsigned long) ptr & 0xfff0);
      sp = top_of_stack();
      SERIAL_ECHOPGM("\nStack Pointer : ");
      prt_hex_word((unsigned int) sp);
      SERIAL_EOL;
      sp = (char*)((unsigned long) sp | 0x000f);
      n = sp - ptr;
      //
      // This is the main loop of the Dump command.
      //
      while (ptr < sp) {
        prt_hex_word((unsigned int) ptr); // Print the address
        SERIAL_CHAR(':');
        for (i = 0; i < 16; i++) {      // and 16 data bytes
          prt_hex_byte(*(ptr + i));
          SERIAL_CHAR(' ');
        }
        SERIAL_CHAR('|');         // now show where non 0xE5's are
        for (i = 0; i < 16; i++) {
          if (*(ptr + i) == (char)0xe5)
            SERIAL_CHAR(' ');
          else
            SERIAL_CHAR('?');
        }
        SERIAL_EOL;
        ptr += 16;
      }
      return;
    }
  #endif
  //
  // M100 F   requests the code to return the number of free bytes in the memory pool along with
  // other vital statistics that define the memory pool.
  //
  if (code_seen('F')) {
    #if 0
      int max_addr = (int)  __brkval ? __brkval : &__bss_end;
      int max_cnt = 0;
    #endif
    int block_cnt = 0;
    ptr =  __brkval ? __brkval : &__bss_end;
    sp = top_of_stack();
    n = sp - ptr;
    // Scan through the range looking for the biggest block of 0xE5's we can find
    for (i = 0; i < n; i++) {
      if (*(ptr + i) == (char)0xe5) {
        j = how_many_E5s_are_here(ptr + i);
        if (j > 8) {
          SERIAL_ECHOPAIR("Found ", j);
          SERIAL_ECHOPGM(" bytes free at 0x");
          prt_hex_word((int) ptr + i);
          SERIAL_EOL;
          i += j;
          block_cnt++;
        }
        #if 0
          if (j > max_cnt) {      // We don't do anything with this information yet
            max_cnt  = j;     // but we do know where the biggest free memory block is.
            max_addr = (int) ptr + i;
          }
        #endif
      }
    }
    if (block_cnt > 1)
      SERIAL_ECHOLNPGM("\nMemory Corruption detected in free memory area.");
    return;
  }
  //
  // M100 C x  Corrupts x locations in the free memory pool and reports the locations of the corruption.
  // This is useful to check the correctness of the M100 D and the M100 F commands.
  //
  #if ENABLED(M100_FREE_MEMORY_CORRUPTOR)
    if (code_seen('C')) {
      int x = code_value_int(); // x gets the # of locations to corrupt within the memory pool
      SERIAL_ECHOLNPGM("Corrupting free memory block.\n");
      ptr = __brkval ? __brkval : &__bss_end;
      SERIAL_ECHOPAIR("\nbss_end : ", ptr);
      ptr += 8;
      sp = top_of_stack();
      SERIAL_ECHOPAIR("\nStack Pointer : ", sp);
      SERIAL_ECHOLNPGM("\n");
      n = sp - ptr - 64;    // -64 just to keep us from finding interrupt activity that
      // has altered the stack.
      j = n / (x + 1);
      for (i = 1; i <= x; i++) {
        *(ptr + (i * j)) = i;
        SERIAL_ECHOPGM("\nCorrupting address: 0x");
        prt_hex_word((unsigned int)(ptr + (i * j)));
      }
      SERIAL_ECHOLNPGM("\n");
      return;
    }
  #endif
  //
  // M100 I    Initializes the free memory pool so it can be watched and prints vital
  // statistics that define the free memory pool.
  //
  if (m100_not_initialized || code_seen('I')) {            // If no sub-command is specified, the first time
    SERIAL_ECHOLNPGM("Initializing free memory block.\n"); // this happens, it will Initialize.
    ptr = __brkval ? __brkval : &__bss_end;                // Repeated M100 with no sub-command will not destroy the
    SERIAL_ECHOPAIR("\nbss_end : ", ptr);                  // state of the initialized free memory pool.
    ptr += 8;
    sp = top_of_stack();
    SERIAL_ECHOPAIR("\nStack Pointer : ", sp);
    SERIAL_ECHOLNPGM("\n");
    n = sp - ptr - 64;    // -64 just to keep us from finding interrupt activity that
    // has altered the stack.
    SERIAL_ECHO(n);
    SERIAL_ECHOLNPGM(" bytes of memory initialized.\n");
    for (i = 0; i < n; i++)
      *(ptr + i) = (char)0xe5;
    for (i = 0; i < n; i++) {
      if (*(ptr + i) != (char)0xe5) {
        SERIAL_ECHOPAIR("? address : ", ptr + i);
        SERIAL_ECHOPAIR("=", *(ptr + i));
        SERIAL_ECHOLNPGM("\n");
      }
    }
    m100_not_initialized = false;
    return;
  }
  return;
}

// top_of_stack() returns the location of a variable on its stack frame.  The value returned is above
// the stack once the function returns to the caller.

char* top_of_stack() {
  char x;
  return &x + 1; // x is pulled on return;
}

//
// 3 support routines to print hex numbers.  We can print a nibble, byte and word
//

void prt_hex_nibble(unsigned int n) {
  if (n <= 9)
    SERIAL_ECHO(n);
  else
    SERIAL_ECHO((char)('A' + n - 10));
}

void prt_hex_byte(unsigned int b) {
  prt_hex_nibble((b & 0xf0) >> 4);
  prt_hex_nibble(b & 0x0f);
}

void prt_hex_word(unsigned int w) {
  prt_hex_byte((w & 0xff00) >> 8);
  prt_hex_byte(w & 0x0ff);
}

// how_many_E5s_are_here() is a utility function to easily find out how many 0xE5's are
// at the specified location.  Having this logic as a function simplifies the search code.
//
int how_many_E5s_are_here(char* p) {
  int n;
  for (n = 0; n < 32000; n++) {
    if (*(p + n) != (char)0xe5)
      return n - 1;
  }
  return -1;
}

#endif

