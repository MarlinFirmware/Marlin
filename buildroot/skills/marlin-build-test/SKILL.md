---
name: marlin-build-test
description: Use when you need to compile Marlin for a board, apply a CI test configuration, or run the build tests locally. Wraps buildroot/bin/mftest, mfenvs, run_tests, and the Makefile targets.
---

# Build and Test Marlin

Requires PlatformIO (`pio`) in your `PATH`. Run everything from the repo root.

## Build the configured board

1. Set `MOTHERBOARD` in `Marlin/Configuration.h` and set up your options.
2. List the PlatformIO environments for the board: `buildroot/bin/mfenvs`
3. Build the auto-detected environment: `buildroot/bin/mftest -a` (rebuild with `-r`, silent with `-s`).
4. Or build one environment directly: `pio run -e <env>`

## Apply a CI test config and build it

Tests live in `buildroot/tests/<env>/config-NN.ini` and are applied with the `opt_*` tools.

- Interactive menu: `buildroot/bin/mftest`
- Specific test: `buildroot/bin/mftest -t mega2560 -n 5 -y` (`-t` env name or shortcut, `-n` test index, `-y` build without asking)
- Start from default configs first: add `-d`
- Only emit the config changes without building: add `-i`
- Use the Docker/make method: add `-m`

Shortcuts for `-t`: `tree due esp lin lp8 lp9 m128 mega stm f1 f4 f7 s6 teensy t31 t35 t40`.

## Add tests for a new feature or option

A PR that adds a feature or option should add tests, so CI keeps it building and working.

**Build test** (`buildroot/tests/<env>/config-NN.ini`)

1. Pick the env directory that matches a board and platform that can build the feature. Use `buildroot/bin/mfenvs` or the existing directories under `buildroot/tests`.
2. Prefer adding the option to an existing `config-NN.ini` with a similar feature set. Add a new file only for a combination that no existing test covers. Use the next free number.
3. Each file begins with a header (`# Test: <env>/config-NN.ini`, then a one-line description of the main options) and a `[config:base]` section. Start from an example with `ini_use_config = example/...`, or leave it out for the defaults.
4. Set options as `name = value` or `name = on`. Test the feature enabled along with the options it depends on, and with the dependent sub-options.
5. Run just your test: `buildroot/bin/mftest -t <env> -n <index> -y` or `make tests-single-local TEST_TARGET=<env> ONLY_TEST=<index>`, where `<index>` is the test's position in file order (1-based) and normally matches `NN`.
6. Keep the number of tests small. Every test adds to CI time.

**Unit test** (when the logic can be checked without hardware)

Unit tests run natively on the `linux_native_test` environment. Good candidates are parsing, math, conversions, state machines and G-code handling. Anything that needs real pins, timers, or devices isn't a good candidate. See `test/README.md`.

1. Add `Marlin/tests/<area>/test_<name>.cpp`. Copy the license header and the `MARLIN_TEST(group, name)` macros from an existing test such as `Marlin/tests/feature/test_runout.cpp`.
2. Wrap the file's contents in `#if ENABLED(FEATURE)` so it compiles only for configs that enable it.
3. If no existing config in `test/*.ini` enables your feature, add one (`test/NNN-name.ini`). It must use `motherboard = BOARD_SIMULATED` and include only the options needed to pass the sanity checks.
4. Run: `make unit-test-all-local`, or one config with `make unit-test-single-local UNIT_TEST_CONFIG=<name>`. (There is a `-docker` variant of each.)

## Run the tests like CI does

- One environment: `make tests-single-local TEST_TARGET=mega2560`
- One test of it: `make tests-single-local TEST_TARGET=mega2560 ONLY_TEST=2`
- Everything (slow): `make tests-all-local`
- In Docker: `make tests-single-local-docker TEST_TARGET=mega2560` (see `make help`)
- Unit tests: `make unit-test-all-local`

The test runner restores the configs between tests and may `git reset --hard` (see `GIT_RESET_HARD` in `make help`), so commit or stash your work first.

## Restore configuration files

After testing, run `buildroot/bin/restore_configs`. It deletes generated files (`marlin_config.json`, `_Bootscreen.h`, `_Statusscreen.h`) and does a `git checkout` of the configuration files, discarding any uncommitted edits to them. `restore_configs -d` instead loads the default example configs.

## Choosing what to build

- Build at least one environment each for any HAL or board family you changed.
- A change to common code deserves an AVR build (tight flash) and a 32-bit build.
- For features, build with the feature both enabled and disabled.
- New feature or option: add a build test and, where it makes sense, a unit test (see above).

## Report results

State exactly which envs and tests you built. Do not claim a pass that you did not see.
