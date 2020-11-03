# Testing Marlin

To ensure that configuration and settings are correct, we need to maintain a
good set of tests, to avoid regressions and mistakes.

There are two types of test in Marlin:
* Tests about [configuration combinations](../buildroot/tests)
* Unit & integration tests

## Configuration combinations tests

To test various configuration combinations, in conjunction with different 
boards, there are various test suites, one for each board, were we compile 
Marlin, by changing options in `Configuration.h` and `Configuration_adv.h`.

In the [buildroot/tests](../buildroot/tests) you can find some examples, which 
utilise various commands, found in [buildroot/bin](../buildroot/bin), and add,
modify, or disable `#define`d options in either, or both, `Configuration.h` and
`Configuration_adv.h`:
* `opt_add`: Add an option in the form of `#define <option>`
  * It will add it to `Configuration.h`
  * Examples:
    * `opt_add EXTUI_EXAMPLE`: `#define EXTUI_EXAMPLE`
    * `opt_add Z2_MAX_PIN 2`: `#define Z2_MAX_PIN 2`
* `opt_disable`: Disable a toggle/option
  * It will look in either file
  * You can add multiple options in one command
  * Examples:
    * `opt_disable PSU_CONTROL`: `// #define PSU_CONTROL`
    * `opt_disable MIN_SOFTWARE_ENDSTOP_Z MAX_SOFTWARE_ENDSTOPS`: `// #define MIN_SOFTWARE_ENDSTOP_Z` and `// #define MAX_SOFTWARE_ENDSTOPS`
* `opt_enable`: Enable a commented toggle/option
  * It will look in either file
  * You can add multiple options in one command
  * Examples:
    * `opt_disable PIDTEMPBED`: `#define PIDTEMPBED`
    * `opt_disable Z_MULTI_ENDSTOPS USE_XMAX_PLUG`: `#define Z_MULTI_ENDSTOPS` and `#define USE_XMAX_PLUG`
* `opt_set` Set the value to a defined option
  * It will look in either file
  * Example:
    * `opt_set TEMP_SENSOR_1 1`: `#define TEMP_SENSOR_1 1`
* `pins_set`: Set the value for a pin in a pins file
  * You need to provide the board name inside the `Marlin/src/pins/` folder, and 
  also the sub-board inside `Marlin/src/pins/<board>/`, omitting the `pins_` and 
  `.h` 
  * You also need to provide the pin name and the value
  * Examples:
    * `pins_set ramps/RAMPS X_MAX_PIN -1`: `#define X_MAX_PIN -1` in `Marlin/src/pins/ramps/pins_RAMPS.h`
* `restore_configs`: Restore any changes made to configs
  * It will restore configuration header, bootscreen header, and pin header 
  files
  * Example:
    * `restore_configs`: restore all configs
* `use_example_configs`: Use an example config from [MarlinFirmware/Configurations](https://github.com/MarlinFirmware/Configurations)
  * It needs internet connection to fetch the boards
  * You need to provide the directory inside the `config/examples` directory
  * Example:
    * `use_example_configs Alfawise/U20`: Replace the files in this repo with 
    the ones [in that folder](https://github.com/MarlinFirmware/Configurations/tree/import-2.0.x/config/examples/Alfawise/U20)

An example use of the above:
```shell script
restore_configs
# Set the board explicitly
opt_set MOTHERBOARD BOARD_BTT_GTR_V1_0
# Or use an example config that will have one set
use_example_configs FYSETC/S6
# Add a new option
opt_add Z2_MAX_ENDSTOP_INVERTING false
# Comment-out some options
opt_disable TFT_INTERFACE_FSMC TFT_COLOR_UI TOUCH_SCREEN TFT_RES_320x240
# Uncomment some options
opt_enable REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER SDSUPPORT \
           ADAPTIVE_FAN_SLOWING NO_FAN_SLOWING_IN_PID_TUNING \
           FILAMENT_WIDTH_SENSOR FILAMENT_LCD_DISPLAY PID_EXTRUSION_SCALING
# Override default values in configs
opt_set Y_DRIVER_TYPE TMC2209
opt_set Z_DRIVER_TYPE TMC2130
# Change some pins
pins_set ramps/RAMPS X_MAX_PIN -1
```

Then, make a call to build Marlin, while giving a descriptive title:
```shell script
exec_test $1 $2 "BigTreeTech GTR 8 Extruders with Auto-Fan and Mixed TMC Drivers" "$3"
```

Of course you can have multiple tests in a file:
```shell script
restore_configs
opt_set MOTHERBOARD BOARD_BTT_GTR_V1_0
...
exec_test $1 $2 "One set of tests" "$3"

restore_configs
opt_set MOTHERBOARD BOARD_BTT_GTR_V1_0
...
exec_test $1 $2 "A different set" "$3"

...
```

You need to follow the convention of naming the file `<environment>-tests`, 
where the environment must be already defined in [platformio.ini](../platformio.ini)

## Unit tests

As the above tests ensure that different combinations can work together, they 
rely on the pre-processor checks to catch any issues.

We must also write tests to check the behaviour of the code with various inputs.
To achieve that we are using [platformio unit tests](https://docs.platformio.org/en/latest/plus/unit-testing.html).

Because we must also set different settings via the configuration headers, we 
re-use the commands described in the previous section to define the 
configuration environment, and the write tests for those features.

This is the structure of these tests:
In `test` we add a directory with a single folder, eg 
`test_default_config/test_all.cpp`, and in there, we add a section that defines 
the configuration, minus any board definitions:
```cpp
/* START_CONFIGURATION

opt_set ...
...

END CONFIGURATION */
```

Bellow that we only add a `main` function, that calls the tests:
```cpp
/* START_CONFIGURATION
...
END CONFIGURATION */
#include "tests/marlin_tests.cpp"
// Include tests code files, eg:
// #include "tests/your_tests.cpp"

// You need to provide a `main` function, and this macro does it for you, with
// the additional convenience of running all collected tests
MAIN_FOR_MARLIN_TESTS();
```

The actual test code is inside `Marlin/tests`, and is rather grouped by feature,
test functionality, and capabilities (based on configuration). We group tests in 
folders per the above, eg `runout/runout_1_extruder.cpp`, and all tests marked
with `MARLIN_TEST` are automatically collected, and will be run in 
`MAIN_FOR_MARLIN_TESTS`:
```cpp
#include "tests/marlin_tests.h"
// Include Marlin code, or other test files

MARLIN_TEST(test_foo_works_under_condition, {
  ...
  TEST_ASSERT_EQUAL(1, marlin.calculation());
  ...
});
``` 

We should also group tests for common configurations, so that we can group tests
in different supersets of configurations, by creating test files under 
`Marlin/tests/common_for/<common-configuration-set>.cpp`.

## Advice about writing good unit tests
* Name tests appropriately: not `test_runout` but `test_runout_states_match_count`
* Don't make too many assertions in one test: split the test in multiple ones,
where each test builds up on assumptions "proven" by previous ones
* Put them into logical groups, and split files if they get too big
* Follow the pattern of Prepare - Run - Assert:
  * `int value = 0;`
  * `result = calculation(value);`
  * `TEST_ASSERT_EQUALS(5, result);`
* Use the `TEST_ASSERT_*` macros, as seen in [the documentation](https://docs.platformio.org/en/latest/plus/unit-testing.html#api)
* Modify the code so that it's easily testable:
  * Break down big functions (use `inline` to avoid calls)
  * Use functional programming when possible: immutable inputs -> output
    * It allows to reason about code more easily
    * It doesn't affect global variables, and therefore other tests
    * It's ok to pass mutable outputs, if you need multiple outputs
* Make it easy for future developers to understand quickly what was broken
* Use multiple variations and combinations of inputs, think about edge cases
* Don't only test happy paths: test that code fails predictably, and that if you 
give it invalid/malformed input the code can handle it
* When you touch some untested code, be a good boy-scout/girl-scout and add a 
couple of tests
