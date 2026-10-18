---
name: marlin-unit-tests
description: Use when writing or running Marlin's native unit tests (make unit-test-*), or when a native unit-test build fails locally, especially on macOS.
---

# Marlin Native Unit Tests

Background: `test/README.md`. For adding a test to a feature PR, see also the unit-test section of skill marlin-build-test.

## Layout

- Test sources: `Marlin/tests/<area>/test_<name>.cpp` (Unity, `MARLIN_TEST(group, name)`).
- Test configurations: `test/NNN-name.ini`. Each one builds a separate test binary with a different Marlin configuration. Suites are discovered from this folder by `buildroot/share/PlatformIO/scripts/collect-code-tests.py`.
- The PlatformIO environment is `linux_native_test` in `ini/native.ini`. It is built on the Linux native HAL, with `-Werror`.

## Run

    make unit-test-all-local                              # all configs
    make unit-test-single-local UNIT_TEST_CONFIG=default  # one config (test/001-default.ini)
    make unit-test-all-local-docker                       # same, in a container

## Write one

1. Copy the license header and macros from an existing test, such as `Marlin/tests/feature/test_runout.cpp`.
2. Wrap the file in `#if ENABLED(FEATURE)` so it compiles only for configs that enable the feature.
3. If no `test/*.ini` enables the feature, add one with `motherboard = BOARD_SIMULATED` and only the options required to pass the sanity checks.
4. Test logic, not hardware: parsing, math, conversions, state machines. Anything needing real pins or timers isn't a good candidate.

## macOS

On macOS `/usr/bin/gcc` is Clang, which cannot build Marlin. Install GCC with MacPorts or Homebrew, then:

    buildroot/bin/mac_gcc macports      # or: homebrew   (apple resets to the system compiler)
    which gcc                           # should now be the real GCC

If the native build fails on macOS, first check the compiler diagnostics and which `gcc` you got. Don't assume the failure is a bug in the test wiring.

## Pitfalls

- Test files are compiled with `-Werror`. An unused variable or sign-compare warning fails the run.
- Don't work around a compiler warning with duplicate includes or duplicate keys in the INI.
- Run the test configs one at a time. They share the `.pio` build directory.
