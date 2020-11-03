#pragma once

#include <list>
#include <string>
#include <unity.h>

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
 * Holds all the tests defined in code
 */
std::list<MarlinTest*> allMarlinTests;

/*
 * Run all the tests through Unity
 */
void runAllMarlinTests();

/*
 * Macro to define a test. It should be used like:
 * MARLIN_TEST(test_feature_x, {
 *   // Test code here
 *   int value = 0;
 *   ...
 *   TEST_ASSERT_EQUAL(5, ...);
 * })
 */
#define MARLIN_TEST(NAME, BODY) void NAME() {BODY}; \
const MarlinTest MarlinTestFor_##NAME = MarlinTest(#NAME, (const void(*)())&NAME, __LINE__);

/*
 * Helper macro, to add a main function for Unity tests, if we only want to run
 * tests that are auto-collected.
 */
#define MAIN_FOR_MARLIN_TESTS() int main(int argc, char **argv) { \
  UNITY_BEGIN(); \
  runAllMarlinTests(); \
  UNITY_END(); \
  return 0; \
}
