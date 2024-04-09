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
We are currently using [PlatformIO unit tests](https://docs.platformio.org/en/latest/plus/unit-testing.html).

Due to Marlin's reliance on conditional compilation, it is not possible to test all Marlin features from a single test binary.
The following technique is used to unit test a variety of configurations:

1. The sub-folders beneath this folder each contain a `config.ini` file, which describe unique configurations which unit tests will execute against. These use the the same syntax as the default `config.ini` file.
2. Tests are defined as CPP files in the `Marlin/tests` folder. They use typical Marlin techniques such as `#if ENABLED(feature)` to register tests or alter test behavior according to the configuration.
3. The PlatformIO environment `linux_native_test` is defined to use an extra script to collect the tests from this folder and insert them into PlatformIO as test targets.
4. Tests are built and executed by the `Makefile` commands `tests-code-all-local` or `tests-code-all-local-docker`.
