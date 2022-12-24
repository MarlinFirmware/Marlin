/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/* Here is the expected behavior of a system producing a CPU exception with this hook installed:
   1. Before the system is crashed
     1.1 Upon validation (not done yet in this code, but we could be using DEBUG flags here to allow/disallow hooking)
     1.2 Install the hook by overwriting the vector table exception handler with the hooked function
   2. Upon system crash (for example, by a dereference of a NULL pointer or anything else)
     2.1 The CPU triggers its exception and jump into the vector table for the exception type
     2.2 Instead of finding the default handler, it finds the updated pointer to our hook
     2.3 The CPU jumps into our hook function (likely a naked function to keep all information about crash point intact)
     2.4 The hook (naked) function saves the important registers (stack pointer, program counter, current mode) and jumps to a common exception handler (in C)
     2.5 The common exception handler dumps the registers on the serial link and perform a backtrace around the crashing point
     2.6 Once the backtrace is performed the last resort function is called (platform specific).
         On some platform with a LCD screen, this might display the crash information as a QR code or as text for the
         user to capture by taking a picture
     2.7 The CPU is reset and/or halted by triggering a debug breakpoint if a debugger is attached */

// Hook into CPU exception interrupt table to call the backtracing code upon an exception
// Most platform will simply do nothing here, but those who can will install/overwrite the default exception handler
// with a more performant exception handler
void hook_cpu_exceptions();

// Some platform might deal without a hard fault handler, in that case, return 0 in your platform here or skip implementing it
void * __attribute__((weak)) hook_get_hardfault_vector_address(unsigned base_address);
// Some platform might deal without a memory management fault handler, in that case, return 0 in your platform here or skip implementing it
void * __attribute__((weak)) hook_get_memfault_vector_address(unsigned base_address);
// Some platform might deal without a bus fault handler, in that case, return 0 in your platform here or skip implementing it
void * __attribute__((weak)) hook_get_busfault_vector_address(unsigned base_address);
// Some platform might deal without a usage fault handler, in that case, return 0 in your platform here or skip implementing it
void * __attribute__((weak)) hook_get_usagefault_vector_address(unsigned base_address);

// Last resort function that can be called after the exception handler was called.
void __attribute__((weak)) hook_last_resort_func();
