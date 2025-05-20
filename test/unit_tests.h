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
#pragma once

#include <list>
#include <string>
#include <unity.h>

// Include MarlinConfig so configurations are available to all tests
#include "src/inc/MarlinConfig.h"

/**
 * Class that allows us to dynamically collect tests
 */
class MarlinTest {
public:
    MarlinTest(const std::string name, const void(*test)(), const char *_file, const int line);
    /**
     * Run the test via Unity
     */
    void run();

    /**
     * The name, a pointer to the function, and the line number. These are
     * passed to the Unity test framework.
     */
    const std::string name;
    const void(*test)();
    const std::string file;
    const int line;
};

/**
 * Internal macros used by MARLIN_TEST
 */
#define _MARLIN_TEST_CLASS_NAME(SUITE, NAME) MarlinTestClass_##SUITE##_##NAME
#define _MARLIN_TEST_INSTANCE_NAME(SUITE, NAME) MarlinTestClass_##SUITE##_##NAME##_instance

/**
 * Macro to define a test. This will create a class with the test body and
 * register it with the global list of tests.
 *
 * Usage:
 * MARLIN_TEST(test_suite_name, test_name) {
 *  // Test body
 * }
 */
#define MARLIN_TEST(SUITE, NAME) \
    class _MARLIN_TEST_CLASS_NAME(SUITE, NAME) : public MarlinTest { \
    public: \
        _MARLIN_TEST_CLASS_NAME(SUITE, NAME)() : MarlinTest(#SUITE "___" #NAME, (const void(*)())&TestBody, __FILE__, __LINE__) {} \
        static void TestBody(); \
    }; \
    const _MARLIN_TEST_CLASS_NAME(SUITE, NAME) _MARLIN_TEST_INSTANCE_NAME(SUITE, NAME); \
    void _MARLIN_TEST_CLASS_NAME(SUITE, NAME)::TestBody()
