#include "./marlin_tests.h"

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
