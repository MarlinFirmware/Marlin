#include "../../inc/MarlinConfig.h"

#include "../gcode.h"

#include "../../module/endstops.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../feature/tmc_util.h"

void GcodeSuite::G14() {
  if (!parser.seenval('A') || !parser.seenval('B')) {
      SERIAL_ECHOLNPGM("G14: Missing parameters A (X-angle) and B (Y-angle)");
      return;
  }

  // Read angles from command
  const float angle_x = parser.floatval('A');  // A parameter (rotation around X)
  const float angle_y = parser.floatval('B');  // B parameter (rotation around Y)

  // Printer-specific dimensions
  // constexpr float bed_length = 200.0;   // Distance between Z and Z2/Z3 in Y direction
  // constexpr float bed_width  = 200.0;   // Distance between Z2 and Z3 in X direction
  constexpr float center_x   = 110.0;   // Center point X
  constexpr float center_y   = 110.0;   // Center point Y
  constexpr float max_adjust = 20.0;    // Max allowed height adjustment

  // Convert degrees to radians
  const float rad_x = radians(angle_x);
  const float rad_y = radians(angle_y);

  // Compute height offsets based on tilt
  float z2_offset = tan(rad_x) * (center_x - 0) + tan(rad_y) * (center_y - 220);  // Z2 (Left Front)
  float z3_offset = tan(rad_x) * (center_x - 220) + tan(rad_y) * (center_y - 220); // Z3 (Right Front)
  float z_offset  = tan(rad_x) * (center_x - 110) + tan(rad_y) * (center_y - 0);   // Z (Back Middle)

  // Adjustments relative to the center (X=100, Y=100) staying at the same Z
  float z_center_offset = (z2_offset + z3_offset) / 2.0;
  z_offset  -= z_center_offset;
  z2_offset -= z_center_offset;
  z3_offset -= z_center_offset;

  // Limit max adjustments to prevent damage
  z_offset  = constrain(z_offset,  -max_adjust, max_adjust);
  z2_offset = constrain(z2_offset, -max_adjust, max_adjust);
  z3_offset = constrain(z3_offset, -max_adjust, max_adjust);

  // Apply movement to the bed
  planner.synchronize();  // Ensure previous moves are completed
  current_position[Z_AXIS] += z_offset;
  current_position[I_AXIS] += z2_offset;
  current_position[J_AXIS] += z3_offset;
  
  stepper.set_samostatny_pohyb(true);
  planner.buffer_line(current_position, feedrate_mm_s, active_extruder);
  planner.synchronize();
  stepper.set_samostatny_pohyb(false);

}  