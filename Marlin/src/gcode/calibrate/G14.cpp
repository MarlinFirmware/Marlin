#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/endstops.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../feature/tmc_util.h"

void GcodeSuite::G14() {
  if (!parser.seenval('A')/*|| !parser.seenval('B')*/) {
      SERIAL_ECHOLNPGM("G14: Missing parameters A (X-angle) and B (Y-angle)");
      return;
  }

  //const float rad_x = radians(parser.floatval('A'));
  const float rad_y = radians(parser.floatval('A'));

  // Compute height offsets based on tilt
  float z2_offset  = 0 - ((sin(rad_y) * Y_BED_SIZE) / 2);
  float z_offset = (sin(rad_y) * Y_BED_SIZE) / 2;
  //float z3_offset = (sin(rad_y) * Y_BED_SIZE) / 2;

  //z2_offset = z2_offset + ((sin(rad_x) * X_BED_SIZE) / 2);
  //z3_offset = z3_offset - ((sin(rad_x) * X_BED_SIZE) / 2);

  // Apply movement to the bed
  wv_pos[0] = z_offset;
  wv_pos[1] = z2_offset;
  planner.synchronize();  // Ensure previous moves are completed
  current_position[Z_AXIS] += wv_pos[0] - current_position[Z_AXIS];
  current_position[I_AXIS] += wv_pos[1] - current_position[I_AXIS];
  //current_position[J_AXIS] += z3_offset;
  
  stepper.set_samostatny_pohyb(true);
  planner.buffer_line(current_position, feedrate_mm_s, active_extruder);
  planner.synchronize();
  stepper.set_samostatny_pohyb(false);

}  