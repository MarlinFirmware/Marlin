/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfigPre.h"

#if ENABLED(MARLIN_TEST_BUILD)

#include "../module/endstops.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../module/settings.h"
#include "../module/stepper.h"
#include "../module/temperature.h"

// Individual tests are localized in each module.
// Each test produces its own report.

// Startup tests are run at the end of setup()
void runStartupTests() {
  // Call post-setup tests here to validate behaviors.

  // String with cutoff at 20 chars:
  // "F-string, 1234.50, 2"
  SString<20> str20;
  str20 = F("F-string, ");
  str20.append(1234.5f).append(',').append(' ')
       .append(2345.67).append(',').append(' ')
       .echoln();

  // Truncate to "F-string"
  str20.trunc(8).echoln();

  // 100 dashes, but chopped down to DEFAULT_MSTRING_SIZE (20)
  TSS(repchr_t('-', 100)).echoln();

  // Hello World!-123456------   <spaces!33
  // ^ eol! ... 1234.50*2345.602 = 2895645.67
  SString<100> str(F("Hello"));
  str.append(F(" World!"));
  str += '-';
  str += uint8_t(123);
  str += F("456");
  str += repchr_t('-', 6);
  str += Spaces(3);
  str += "< spaces!";
  str += int8_t(33);
  str.eol();
  str += "^ eol!";

  str.append("...", 1234.5f, '*', p_float_t(2345.602, 3), F(" = "), 1234.5 * 2345.602).echoln();

  // Print it again with SERIAL_ECHOLN
  auto print_char_ptr = [](char * const str) { SERIAL_ECHOLN(str); };
  print_char_ptr(str);

}

// Periodic tests are run from within loop()
void runPeriodicTests() {
  // Call periodic tests here to validate behaviors.
}

#endif // MARLIN_TEST_BUILD
