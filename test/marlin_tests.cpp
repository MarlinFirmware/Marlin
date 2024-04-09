/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// This provides the main() function which is used for all compiled test binaries.
// It collects all the tests defined in the code and runs them through Unity.

#include "marlin_tests.h"

static std::list<MarlinTest*> allMarlinTests;

MarlinTest::MarlinTest(const std::string _name, const void(*_test)(), const int _line)
: name(_name), test(_test), line(_line) {
  allMarlinTests.push_back(this);
}

void MarlinTest::run() {
  UnityDefaultTestRun((UnityTestFunction)test, name.c_str(), line);
}

void runAllMarlinTests() {
  for (const auto registration : allMarlinTests) {
    registration->run();
  }
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  runAllMarlinTests();
  UNITY_END();
  return 0;
}
