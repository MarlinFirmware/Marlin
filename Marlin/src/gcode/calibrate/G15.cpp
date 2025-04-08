#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/endstops.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../feature/tmc_util.h"

void GcodeSuite::G15() {
  if (parser.seenval('A')){
    stepper.set_separate_multi_axis(true);
    stepper.set_z1_lock(true);
  }
  if (parser.seenval('B')){
    stepper.set_z1_lock(false);
    stepper.set_separate_multi_axis(false);
  }
  if (parser.seenval('C')){
    stepper.set_separate_multi_axis(true);
    stepper.set_z2_lock(true);
  }
  if (parser.seenval('D')){
    stepper.set_z2_lock(false);
    stepper.set_separate_multi_axis(false);
  }
}  