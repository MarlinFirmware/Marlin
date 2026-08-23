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
 * Program entry point for Atmel SAM3U (AT91SAM3U4E)
 *
 * The CMSIS startup code for the SAM3U runs the .data/.bss setup and the C++
 * static constructors and then branches straight to main() - unlike most
 * vendor startups it never calls SystemInit(). So the very first thing main()
 * does is bring the clock tree up to F_CPU; everything before this point ran
 * on the 4MHz internal RC, which is harmless but slow.
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"
#include "HAL.h"

extern void setup();
extern void loop();

int main() {
  sam3u_clock_init();     // PLLA up to 96MHz, flash wait states, NRST enable
  sam3u_systick_init();   // 1kHz time base for millis()/micros()

  setup();
  for (;;) loop();

  return 0;
}

#endif // __SAM3U4E__
