# Testing Marlin

Marlin has two primary types of tests:
- [Configuration Compilation Tests](../buildroot/tests)
- Unit tests (this folder)

This document focuses on Unit tests.

## Unit tests

Unit testing allows for functional testing of Marlin logic on a local machine. This strategy is available to all developers, and will be able to be used on generic GitHub workers to automate testing. While PlatformIO does support the execution of unit tests on target controllers, there has been no effort to implement that at this time. This would require dedicated testing labs, and would be less broadly usable than testing directly on the development or build machine.

Unit tests verify the behavior of small discrete sections of Marlin code. By thoroughly unit testing important parts of Marlin code, we effectively provide "guard rails" which will prevent major regressions in behavior. As long as all submissions go through the Pull Request process and execute automated checks, it is possible to catch most major issues prior to completion of a PR.

### Unit test FAQ

#### Q: Isn't writing unit tests a lot of work?
A: Yes, and it can be especially difficult with existing code that wasn't designed for unit testing. Some common sense should be used to decide where to employ unit testing, and at what level to perform it. While unit testing takes effort, it pays dividends in preventing regressions, and helping to pinpoint the source of failures when they do occur.

#### Q: Will this make refactoring harder?
A: Yes and No. Of course if you refactor code that unit tests use directly, it will have to be reworked as well. It actually can make refactoring more efficient, by providing assurance that the mechanism still works as intended.

#### Q: How can I debug one of these failing unit tests?
A: That's a great question, without a known immediate answer. It is likely possible to debug them interactively through PlatformIO, but that can at times take some creativity to configure. Unit tests are generally extremely small, so even without interactive debugging it can get you fairly close to the cause of the problem.

### Unit test architecture
We are currently using [platformio unit tests](https://docs.platformio.org/en/latest/plus/unit-testing.html).

Due to Marlin's reliance on conditional compilation, it is not possible to test all Marlin features from a single test binary.
To enable unit testing on a variety of configurations, the following technique is employed:

1. Each test beneath this folder defines configuration changes that should be made on top of the default Marlin coniguration. This uses the same tools and syntax as the previously described Configuration Compilation tests.
2. Tests are added in the Marlin/tests folder. They use typical Marlin techniques such as `#if ENABLED(feature)` to determine whether the configuration is suitable to register a test, or to alter test behavior.
3. The PlatformIO `linux_native_test` environment utilizes an extra script which collects the tests from this folder, and inserts them into PlatformIO as test targets.
4. The Makefile commands `tests-code-all-local` and `tests-code-all-local-docker` are used to build and execute the tests.
