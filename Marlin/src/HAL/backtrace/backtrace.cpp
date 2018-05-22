/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "backtrace.h"

#if defined(__arm__) || defined(__thumb__)

#include "unwinder.h"
#include "unwmemaccess.h"

#include "../../core/serial.h"
#include <stdarg.h>

// Dump a backtrace entry
static bool UnwReportOut(void* ctx, const UnwReport* bte) {
  int *p = (int*)ctx;

  (*p)++;

  SERIAL_CHAR('#'); SERIAL_PRINT(*p,DEC); SERIAL_ECHOPGM(" : ");
  SERIAL_ECHOPGM(bte->name ? bte->name : "unknown"); SERIAL_ECHOPGM("@0x"); SERIAL_PRINT(bte->function, HEX);
  SERIAL_CHAR('+'); SERIAL_PRINT(bte->address - bte->function,DEC);
  SERIAL_ECHOPGM(" PC:"); SERIAL_PRINT(bte->address,HEX); SERIAL_CHAR('\n');
  return true;
}

#ifdef UNW_DEBUG
  void UnwPrintf(const char* format, ...) {
    char dest[256];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);
    TX(&dest[0]);
  }
#endif

/* Table of function pointers for passing to the unwinder */
static const UnwindCallbacks UnwCallbacks = {
  UnwReportOut,
  UnwReadW,
  UnwReadH,
  UnwReadB
  #ifdef UNW_DEBUG
   , UnwPrintf
  #endif
};

void backtrace(void) {

  UnwindFrame btf;
  uint32_t sp = 0, lr = 0, pc = 0;

  // Capture the values of the registers to perform the traceback
  __asm__ __volatile__ (
    " mov %[lrv],lr\n"
    " mov %[spv],sp\n"
    " mov %[pcv],pc\n"
    : [spv]"+r"( sp ),
      [lrv]"+r"( lr ),
      [pcv]"+r"( pc )
    ::
  );

  // Fill the traceback structure
  btf.sp = sp;
  btf.fp = btf.sp;
  btf.lr = lr;
  btf.pc = pc | 1; // Force Thumb, as CORTEX only support it

  // Perform a backtrace
  SERIAL_ERROR_START();
  SERIAL_ERRORLNPGM("Backtrace:");
  int ctr = 0;
  UnwindStart(&btf, &UnwCallbacks, &ctr);
}

#else // !__arm__ && !__thumb__

void backtrace(void) {}

#endif
