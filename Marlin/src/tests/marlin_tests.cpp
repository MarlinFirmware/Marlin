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
}

// Periodic tests are run from within loop()
void runPeriodicTests() {
  // Call periodic tests here to validate behaviors.
}

#endif // MARLIN_TEST_BUILD
