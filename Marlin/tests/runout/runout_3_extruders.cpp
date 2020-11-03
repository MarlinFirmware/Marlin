#include "tests/marlin_tests.h"
#include <src/feature/runout.h>
#include "./runout_any_extruders.cpp"

MARLIN_TEST(test_get_runout_states, {
  FilamentSensorBase sensor;
  TEST_ASSERT_EQUAL(7, sensor.get_runout_states());
});

MARLIN_TEST(test_runout, {
  RUN_TEST(test_get_runout_states);
  RUN_TEST(test_compare_runout_states);
});
