#include "../test/marlin_tests.h"

#if ENABLED(FILAMENT_RUNOUT_SENSOR)

#include <src/feature/runout.h>

MARLIN_TEST(runout, poll_runout_states)
{
  FilamentSensorBase sensor;
  // Expected default value is one bit set for each extruder
  uint8_t expected = static_cast<uint8_t>(~(~0u << NUM_RUNOUT_SENSORS));
  TEST_ASSERT_EQUAL(expected, sensor.poll_runout_states());
}
#endif
