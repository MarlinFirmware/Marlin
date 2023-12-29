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

  SERIAL_ECHOLNPGM("\n<< Startup Tests >>");

  SERIAL_ECHOLNPGM("\n<< SERIAL_ECHO>>");

  SERIAL_ECHOLN("C String ", F("F String "), C('X'), C(' '), 123, C(','), 123.4, C(','), -123, C(','), -123.4, C(','), int8_t(-123), C(','), uint8_t(123), C(','), int16_t(-123), C(','), uint16_t(123), C(','), int32_t(-123), C(','), uint32_t(123));
  for (uint8_t i = 0; i <= 9; ++i) SERIAL_CHAR('0' + char(i));
  SERIAL_EOL();

  SERIAL_ECHOLNPGM("\n<< ELAPSED / PENDING >>");

  constexpr millis_t erly = 0x0000FFFF, late = 0x7FFFFF00,
                     erly2 = erly + MIN_TO_MS(1), late2 = late + MIN_TO_MS(1), huge = erly + 0x7FFFFFF0;
  SERIAL_ECHOLN(F("PENDING("), int32_t(erly), C(','), int32_t(erly2), F(") is "), PENDING(erly, erly2) ? F("OK") :  F("BAD"));
  SERIAL_ECHOLN(F("PENDING("), int32_t(late), C(','), int32_t(late2), F(") is "), PENDING(late, late2) ? F("OK") :  F("BAD"));
  SERIAL_ECHOLN(F("PENDING("), int32_t(erly), C(','), int32_t(huge), F(") is "), PENDING(erly, huge) ? F("OK") :  F("BAD"));

  MTimeout<millis_t> timeout(100);
  timeout.prime();
  timeout.idle();

  MDelay delay4000(4000);
  delay4000.dofunc([] {
    SERIAL_ECHOLNPGM("millis() = ", millis());
    safe_delay(500);
  });

  SERIAL_ECHOLNPGM("\n<< SString class >>");

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
