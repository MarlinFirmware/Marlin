#include "../../inc/MarlinConfig.h"

#if ENABLED(MOTION_STEPS_COUNTER)

#include "../gcode.h"
#include "../../feature/runout.h"

void GcodeSuite::M1111() {
  FilamentSensorEncoder::encoder_steps = 0;
}

#endif // MOTION_STEPS_COUNTER
