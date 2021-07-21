/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669
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

// Internal engine
//#include "TestVector.hpp"  // Done in the makefile

// Tests start here
//#include "Strings.tests.cpp"  // Done in the makefile

int main(int argc, char ** argv) {
  int failed = 0;
  for (auto test : Test::testVectors) {
    if (!test.launchTest()) {
      failed = 1;
      fprintf(stderr, " %-30s [FAIL]\n", test.getTestName().c_str());
    }
    else
      fprintf(stdout, " %-30s [ OK ]\n", test.getTestName().c_str());
  }
  return failed;
}
