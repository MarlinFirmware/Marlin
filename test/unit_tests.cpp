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

/**
 * Provide the main() function used for all compiled unit test binaries.
 * It collects all the tests defined in the code and runs them through Unity.
 */

#include "unit_tests.h"

static std::list<MarlinTest*> all_marlin_tests;

MarlinTest::MarlinTest(const std::string _name, const void(*_test)(), const char *_file, const int _line)
: name(_name), test(_test), file(_file), line(_line) {
  all_marlin_tests.push_back(this);
}

void MarlinTest::run() {
  Unity.TestFile = file.c_str();
  UnityDefaultTestRun((UnityTestFunction)test, name.c_str(), line);
}

void run_all_marlin_tests() {
  for (const auto registration : all_marlin_tests) {
    registration->run();
  }
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  run_all_marlin_tests();
  UNITY_END();
  return 0;
}
