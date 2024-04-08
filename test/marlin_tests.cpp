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
