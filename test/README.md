# Testing Marlin

Marlin included two types of automated tests:
- [Build Tests](../buildroot/tests) to catch syntax and code build errors.
- Unit Tests (this folder) to catch implementation errors.

This document focuses on Unit tests.

## Unit tests

Unit testing allows for functional testing of Marlin logic on a local machine. This strategy is available to all developers, and will be able to be used on generic GitHub workers to automate testing. While PlatformIO does support the execution of unit tests on target controllers, that is not yet implemented and not really practical. This would require dedicated testing labs, and would be less broadly usable than testing directly on the development or build machine.

Unit tests verify the behavior of small discrete sections of Marlin code. By thoroughly unit testing important parts of Marlin code, we effectively provide "guard rails" which will prevent major regressions in behavior. As long as all submissions go through the Pull Request process and execute automated checks, it is possible to catch most major issues prior to completion of a PR.

## What unit tests can and can't do

Unit tests can be used to validate the logic of single functions or whole features, as long as that function or feature doesn't depend on real hardware. So, for example, we can test whether a G-code command is parsed correctly and produces all the expected state changes, but we can't test whether a G-code triggered an endstop or the filament runout sensor without adding a new layer to simulate pins.

Generally speaking, the types of errors caught by unit tests are most often caught in the initial process of writing the tests, and thereafter they shore up the codebase against regressions, especially in core classes and types, which can be very useful for refactoring.

### Unit test FAQ

#### Q: Isn't writing unit tests a lot of work?
A: Yes, and it can be especially difficult with existing code that wasn't designed for unit testing. Some common sense should be used to decide where to employ unit testing, and at what level to perform it. While unit testing takes effort, it pays dividends in preventing regressions, and helping to pinpoint the source of failures when they do occur.

#### Q: Will this make refactoring harder?
A: Yes and No. Of course if you refactor code that unit tests use directly, it will have to be reworked as well. It actually can make refactoring more efficient, by providing assurance that the mechanism still works as intended.

#### Q: How can I debug one of these failing unit tests?
A: That's a great question, without a known immediate answer. It is likely possible to debug them interactively through PlatformIO, but that can at times take some creativity to configure. Unit tests are generally extremely small, so even without interactive debugging it can get you fairly close to the cause of the problem.

### Unit test architecture
We are currently using [PlatformIO unit tests](https://docs.platformio.org/en/latest/plus/unit-testing.html).

Since Marlin only compiles code required by the configuration, a separate test binary must be generated for any configuration change. The following process is used to unit test a variety of configurations:

1. This folder contains a set of INI configuration files (See `config.ini`), each containing a distinct set of configuration options for unit testing. All applicable unit tests will be run for each of these configurations.
2. The `Marlin/tests` folder contains the CPP code for all Unit Tests. Marlin macros (`ENABLED(feature)`, `TERN(FEATURE, A, B)`, etc.) are used to determine which tests should be registered and to alter test behavior.
3. The `linux_native_test` PlatformIO environment specifies a script to collect all the tests from this folder and add them to PlatformIO's list of test targets.
4. Tests are built and executed by the `Makefile` commands `unit-test-all-local` or `unit-test-all-local-docker`.
