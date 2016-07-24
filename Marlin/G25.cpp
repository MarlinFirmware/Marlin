
/*
 * Marlin Firmware -- G25
 */

#include "Marlin.h"
#include "Configuration.h"

#ifdef  DELTA
#ifdef  HAS_BED_PROBE
#ifdef  DELTA_PHYSICAL_BED_LEVELING_FEATURE

//#include "planner.h"
//#include "configuration_store.h"
//#include "parser.h"
//#include "probe_management/probe_management.h"
//#include "kinematics/coefficients.h"
//#include "motion/motion.h"
//#include "host_interface/host_io.h"

// G25 3-Point Supported Bed Adjustment
//
// This function greatly helps in physically leveling a bed supported at its perimeter by 3 points.
// There are several modes of operation.  The function defaults to C-ircular if no other probing option
// is specified.  Suggested usage is to use the C option ignoring the outer circle numbers.  Then use
// the L option to get the DELTA_RADIUS closer and then fine tune the C option's outer circle numbers.
//
// C -	Circular Probing Operation.  This is primarily useful for measuring the height at the 3 bed support
// 	points at uniform distances from the center of the bed.  It will probe 3 points
// 	along the perimeter of a circle.  The operation will default to 3 circles with the outer most circle at
//	DELTA_PROBEABLE_RADIUS if no radius is specified.   The positioning of the 3 bed support points is
//	assumed to be halfway between each tower unless an angular offset is given using the T option.  A number
//	can follow the C command and will specify the number of circles to probe.  If no number is provided the
//	command will default to 3 circles with the outer most circle at the defaulted or specified radius.
//
//	The outer circle numbers are useful because they most closely measure the height of the bed
//	at the mounting points.  However, those numbers should not be as trusted as the inner numbers because
//	Delta printers have positioning inaccuracies that compound as they move further from center.   The
//	inner circles are more useful for accurate leveling.
//
// L -	Radial lines from the far side of the bed through the origin to the Towers.  This is useful to see the
//	slope of the bed straight away from the towers.  But it is especially valuable when trying to get the
//	DELTA_RADIUS number set accurately.  The command assumes the back tower is straight back.
//
// D -	Displace Probe.  Aligns lines of sampled points to the 1st nozzle instead of to the probe offset.
// 	This will displace the Z-Probe from the radial lines such that the nozzle tracks the radial line.  This
// 	will result in the diagonal rods being straight out from the tower that is having a radial line probed to
// 	it.
//
// O -	Set the origin on the bed if not at the bed's center. [CURRENTLY NOT IMPLEMENTED.  WAITING FOR FEEDBACK]
//
// T -	Set the angular offset for bed mount points and radial lines to the towers.  If the bed support points
// 	are not positioned halfway between the towers you will need to consider running the C and L commands
// 	seperately with different offsets to get the desired numbers.  T represents the Theta of the back tower.
// 	Theta defaults to 90 degrees using an X / Y grid on the printer's bed.
//
// R -	Set the radius for the probing operations.  DELTA_PROBEABLE_RADIUS is used as the maximum radius if
// 	no radius is specified.
//
// Example commands:	G25 C 			Probe 3 bed mounting points at DELTA_PROBEABLE_RADIUS
//			G25 C 4 T 30 R 80	Probe 3 bed mounting points at a radius of 80mm and
//						angular displacement of 30 degrees with 4 circles
//			G26 C 2  R 65		Probe 3 bed mounting points at a radius of 65mm with 2 circles
//			G25 L 			Probe 3 radial lines going to each tower using a radius of
//						DELTA_PROBEABLE_RADIUS with 5 points per line
//			G25 L 7			Probe 3 radial lines going to each tower using a radius of
//						DELTA_PROBEABLE_RADIUS with 7 points per line
//			G25 L 7  R 65		Probe 3 radial lines going to each tower using a radius of
//						65mm and 7 points per line
//
//
//
//

void gcode_G25() {
  float r, x_nozzle, y_nozzle, x_probe, y_probe;
  float radius = DELTA_PROBEABLE_RADIUS, theta = 90.0;
  // float Back_tower_angle=0.0;  Unused
  int i, tower, n = 5, displace_nozzle_flag = 0, do_circle_report = 0, do_tower_line_report = 0;
  int n_circles = 3;
  /*
  	SERIAL_PROTOCOLPGM("Delta Printable Radius ");
  	SERIAL_PROTOCOLLN(DELTA_PRINTABLE_RADIUS);
  	SERIAL_PROTOCOLPGM("Delta Probable Radius ");
  	SERIAL_PROTOCOLLN(DELTA_PROBEABLE_RADIUS);
  */
  if (code_seen('D'))
    displace_nozzle_flag++;
  if (code_seen('C'))	{
    do_circle_report++;
    if (code_has_value()) {
      n_circles = code_value_int();
      SERIAL_PROTOCOLPGM("Setting ");
      SERIAL_PROTOCOL(n_circles);
      SERIAL_PROTOCOLPGM(" circles.\n");
      if (n_circles < 1 || n_circles > 5) {
        SERIAL_PROTOCOLPGM("?Number of circles specified not plausable.\n");
        return;
      }
    }
  }
  if (code_seen('L'))	{
    do_tower_line_report++;
    if (code_has_value()) {
      n = code_value_int();
      if (n < 3 || n > 20) {
        SERIAL_PROTOCOLPGM("?Number of points specified not plausable.\n");
        return;
      }
      SERIAL_PROTOCOLPGM("Setting ");
      SERIAL_PROTOCOL(n);
      SERIAL_PROTOCOLPGM(" points along lines.\n");
    }
  }
  if (code_seen('R'))	{
    radius = code_value_float();
    if (radius < 5.0 || radius > 300.0) {
      SERIAL_PROTOCOLPGM("?Specified radius not plausable.\n");
      return;
    }
  }
  if (code_seen('T'))	{
    theta = code_value_float();
    if (theta <= -360.0 || theta >= 360.0) {
      SERIAL_PROTOCOLPGM("?Specified angle not plausable.\n");
      return;
    }
  }
  if (do_tower_line_report == 0) 		// If no report is specified we default to the circle report
    do_circle_report++;
  if (do_circle_report) { 		// Probe at the 3 bed support points
    DEPLOY_PROBE();
    do_blocking_move_to_z( Z_RAISE_PROBE_DEPLOY_STOW );
    SERIAL_PROTOCOLPGM("Doing ");
    SERIAL_PROTOCOL(n_circles);
    SERIAL_PROTOCOLPGM(" circles.\n");
    SERIAL_PROTOCOLPGM("Origin at: ");
    SERIAL_PROTOCOLPGM("Outer Circle of Points:\n");
    for (i = 0; i < n_circles; i++) {
      r =  radius * (n_circles - i) / ((float) n_circles);
      SERIAL_PROTOCOLPGM("Next Circle at: ");
      SERIAL_PROTOCOLLN(r);
      for (tower = 0; tower < 3; tower++) {
        x_probe = r * cos((theta + 30.0 + (tower * 120.0)) / 57.295779513082320876798154814105); // if only we had this much precision!  :)
        y_probe = r * sin((theta + 30.0 + (tower * 120.0)) / 57.295779513082320876798154814105);
        if (displace_nozzle_flag) {				// if the user specified the O-ffset flag, we are
          x_probe += X_PROBE_OFFSET_FROM_EXTRUDER;	// going to make the nozzle follow the radial line
          y_probe += Y_PROBE_OFFSET_FROM_EXTRUDER;	// to the tower instead of having the probe do it.
        }
        x_nozzle = x_probe - X_PROBE_OFFSET_FROM_EXTRUDER;	// Calculate the nozzle's position
        y_nozzle = y_probe - Y_PROBE_OFFSET_FROM_EXTRUDER;
        //	Check that both the nozzle and probe are going to be within the specified radius.
        //	We add a little bit of extra space to insure any rounding errors will allow the check
        //	check to succeed.
        if (sqrt(x_probe * x_probe + y_probe * y_probe) > DELTA_PROBEABLE_RADIUS + .001) {
          SERIAL_PROTOCOLPGM("Probe too far from center at: X=");
          SERIAL_PROTOCOL(x_probe);
          SERIAL_PROTOCOL(",Y=");
          SERIAL_PROTOCOLLN(y_probe);
          continue;
        }
        if (sqrt(x_nozzle * x_nozzle + y_nozzle * y_nozzle) > DELTA_PRINTABLE_RADIUS + .001) {
          SERIAL_PROTOCOLPGM("Nozzle too far from center at: X=");
          SERIAL_PROTOCOL(x_nozzle);
          SERIAL_PROTOCOL(",Y=");
          SERIAL_PROTOCOLLN(y_nozzle);
          continue;
        }
        probe_pt(x_probe, y_probe, false, 4);
      }
    }
    clean_up_after_endstop_or_probe_move();
    STOW_PROBE();
    return;
  }
  if (do_tower_line_report) {
    DEPLOY_PROBE();
    do_blocking_move_to_z( Z_RAISE_PROBE_DEPLOY_STOW );
    for (tower = 0; tower < 3; tower++) {
      switch (tower) {
      case 0: SERIAL_PROTOCOLPGM("Radial Line To Back Tower:\n");
        break;
      case 1: SERIAL_PROTOCOLPGM("Radial Line To Left Tower:\n");
        //					theta += 120.0;
        break;
      case 2: SERIAL_PROTOCOLPGM("Radial Line To Right Tower:\n");
        //					theta -= 240.0;
        break;
      }
      for (i = 0; i < n; i++) {
        r = -radius + (radius * 2.0 * ((float) i) / (float) n);
        x_probe = r * cos((theta + (tower * 120.0)) / 57.295779513082320876798154814105);
        y_probe = r * sin((theta + (tower * 120.0)) / 57.295779513082320876798154814105);	// if only we had this much precision!  :)
        if (displace_nozzle_flag) {			// if the user specified the Nozzle offset flag, we are
          x_probe += X_PROBE_OFFSET_FROM_EXTRUDER;	// going to make the nozzle follow the radial line
          y_probe += Y_PROBE_OFFSET_FROM_EXTRUDER;	// to the tower instead of having the probe do it.
        }
        x_nozzle = x_probe - X_PROBE_OFFSET_FROM_EXTRUDER;
        y_nozzle = y_probe - Y_PROBE_OFFSET_FROM_EXTRUDER;
        if (x_nozzle * x_nozzle + y_nozzle * y_nozzle > DELTA_PRINTABLE_RADIUS * DELTA_PRINTABLE_RADIUS) {
          SERIAL_ECHOPAIR("Nozzle out of printable area at: ", x_nozzle);
          SERIAL_ECHOPAIR(", ", y_nozzle);
          SERIAL_EOL;
          continue;
        }
        if (x_probe * x_probe + y_probe * y_probe > DELTA_PROBEABLE_RADIUS * DELTA_PROBEABLE_RADIUS) {
          SERIAL_ECHOPAIR("Probe out of probable area at: ", x_probe);
          SERIAL_ECHOPAIR(", ", y_probe);
          SERIAL_EOL;
          continue;
        }
        probe_pt(x_probe, y_probe, false, 4);
      }
    }
    do_blocking_move_to_z( Z_RAISE_PROBE_DEPLOY_STOW );
    STOW_PROBE();
    return;
  }
  return;
}

#endif
#endif
#endif



