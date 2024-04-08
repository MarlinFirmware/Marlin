#pragma once

#include <list>
#include <string>
#include <unity.h>

// Include MarlinConfig so configurations are available to all tests
#include "src/inc/MarlinConfig.h"

/*
 * Class that allows us to dynamically collect tests
 */
class MarlinTest {
public:
    MarlinTest(const std::string name, const void(*test)(), const int line);
    /*
     * Run the test via Unity
     */
    void run();

    /*
     * The name, a pointer to the function, and the line number. These are
     * passed to the Unity test framework.
     */
    const std::string name;
    const void(*test)();
    const int line;
};

/*
 * Internal macros used by MARLIN_TEST
 */
#define _MARLIN_TEST_CLASS_NAME(SUITE, NAME) MarlinTestClass_##SUITE##_##NAME
#define _MARLIN_TEST_INSTANCE_NAME(SUITE, NAME) MarlinTestClass_##SUITE##_##NAME##_instance

/*
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
        _MARLIN_TEST_CLASS_NAME(SUITE, NAME)() : MarlinTest(#NAME, (const void(*)())&TestBody, __LINE__) {} \
        static void TestBody(); \
    }; \
    const _MARLIN_TEST_CLASS_NAME(SUITE, NAME) _MARLIN_TEST_INSTANCE_NAME(SUITE, NAME); \
    void _MARLIN_TEST_CLASS_NAME(SUITE, NAME)::TestBody()
