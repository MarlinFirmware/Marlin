/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../Marlin.h"

#if ENABLED(CALIBRATION_GCODE)

#include "../gcode.h"

#if ENABLED(BACKLASH_GCODE)
  #include "../../feature/backlash.h"
#endif

#include "../../lcd/ultralcd.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/tool_change.h"
#include "../../module/endstops.h"
#include "../../feature/bedlevel/bedlevel.h"

/**
 * G425 backs away from the calibration object by various distances
 * depending on the confidence level:
 *
 *   UNKNOWN   - No real notion on where the calibration object is on the bed
 *   UNCERTAIN - Measurement may be uncertain due to backlash
 *   CERTAIN   - Measurement obtained with backlash compensation
 */

#ifndef CALIBRATION_MEASUREMENT_UNKNOWN
  #define CALIBRATION_MEASUREMENT_UNKNOWN   5.0 // mm
#endif
#ifndef CALIBRATION_MEASUREMENT_UNCERTAIN
  #define CALIBRATION_MEASUREMENT_UNCERTAIN 1.0 // mm
#endif
#ifndef CALIBRATION_MEASUREMENT_CERTAIN
  #define CALIBRATION_MEASUREMENT_CERTAIN   0.5 // mm
#endif

#define HAS_X_CENTER BOTH(CALIBRATION_MEASURE_LEFT, CALIBRATION_MEASURE_RIGHT)
#define HAS_Y_CENTER BOTH(CALIBRATION_MEASURE_FRONT, CALIBRATION_MEASURE_BACK)

enum side_t : uint8_t { TOP, RIGHT, FRONT, LEFT, BACK, NUM_SIDES };

static constexpr xyz_pos_t true_center CALIBRATION_OBJECT_CENTER;
static constexpr xyz_float_t dimensions CALIBRATION_OBJECT_DIMENSIONS;
static constexpr xy_float_t nod = { CALIBRATION_NOZZLE_OUTER_DIAMETER, CALIBRATION_NOZZLE_OUTER_DIAMETER };

struct measurements_t {
  xyz_pos_t obj_center = true_center; // Non-static must be assigned from xyz_pos_t

  float obj_side[NUM_SIDES], backlash[NUM_SIDES];
  xyz_float_t pos_error;

  xy_float_t nozzle_outer_dimension = nod;
};

#define TEMPORARY_SOFT_ENDSTOP_STATE(enable) REMEMBER(tes, soft_endstops_enabled, enable);

#if ENABLED(BACKLASH_GCODE)
  #define TEMPORARY_BACKLASH_CORRECTION(value) REMEMBER(tbst, backlash.correction, value)
#else
  #define TEMPORARY_BACKLASH_CORRECTION(value)
#endif

#if ENABLED(BACKLASH_GCODE) && defined(BACKLASH_SMOOTHING_MM)
  #define TEMPORARY_BACKLASH_SMOOTHING(value) REMEMBER(tbsm, backlash.smoothing_mm, value)
#else
  #define TEMPORARY_BACKLASH_SMOOTHING(value)
#endif

inline void calibration_move() {
  do_blocking_move_to(current_position, MMM_TO_MMS(CALIBRATION_FEEDRATE_TRAVEL));
}

/**
 * Move to the exact center above the calibration object
 *
 *   m                  in     - Measurement record
 *   uncertainty        in     - How far away from the object top to park
 */
inline void park_above_object(measurements_t &m, const float uncertainty) {
  // Move to safe distance above calibration object
  current_position.z = m.obj_center.z + dimensions.z / 2 + uncertainty;
  calibration_move();

  // Move to center of calibration object in XY
  current_position = xy_pos_t(m.obj_center);
  calibration_move();
}

#if HOTENDS > 1
  inline void set_nozzle(measurements_t &m, const uint8_t extruder) {
    if (extruder != active_extruder) {
      park_above_object(m, CALIBRATION_MEASUREMENT_UNKNOWN);
      tool_change(extruder);
    }
  }
#endif

#if HAS_HOTEND_OFFSET

  inline void normalize_hotend_offsets() {
    for (uint8_t e = 1; e < HOTENDS; e++)
      hotend_offset[e] -= hotend_offset[0];
    hotend_offset[0].reset();
  }

#endif

inline bool read_calibration_pin() {
  #if HAS_CALIBRATION_PIN
    return (READ(CALIBRATION_PIN) != CALIBRATION_PIN_INVERTING);
  #elif ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    return (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING);
  #else
    return (READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING);
  #endif
}

/**
 * Move along axis in the specified dir until the probe value becomes stop_state,
 * then return the axis value.
 *
 *   axis         in - Axis along which the measurement will take place
 *   dir          in - Direction along that axis (-1 or 1)
 *   stop_state   in - Move until probe pin becomes this value
 *   fast         in - Fast vs. precise measurement
 */
float measuring_movement(const AxisEnum axis, const int dir, const bool stop_state, const bool fast) {
  const float step     = fast ? 0.25 : CALIBRATION_MEASUREMENT_RESOLUTION;
  const feedRate_t mms = fast ? MMM_TO_MMS(CALIBRATION_FEEDRATE_FAST) : MMM_TO_MMS(CALIBRATION_FEEDRATE_SLOW);
  const float limit    = fast ? 50 : 5;

  destination = current_position;
  for (float travel = 0; travel < limit; travel += step) {
    destination[axis] += dir * step;
    do_blocking_move_to(destination, mms);
    planner.synchronize();
    if (read_calibration_pin() == stop_state) break;
  }
  return destination[axis];
}

/**
 * Move along axis until the probe is triggered. Move toolhead to its starting
 * point and return the measured value.
 *
 *   axis               in     - Axis along which the measurement will take place
 *   dir                in     - Direction along that axis (-1 or 1)
 *   stop_state         in     - Move until probe pin becomes this value
 *   backlash_ptr       in/out - When not nullptr, measure and record axis backlash
 *   uncertainty        in     - If uncertainty is CALIBRATION_MEASUREMENT_UNKNOWN, do a fast probe.
 */
inline float measure(const AxisEnum axis, const int dir, const bool stop_state, float * const backlash_ptr, const float uncertainty) {
  const bool fast = uncertainty == CALIBRATION_MEASUREMENT_UNKNOWN;

  // Save position
  destination = current_position;
  const float start_pos = destination[axis];
  const float measured_pos = measuring_movement(axis, dir, stop_state, fast);
  // Measure backlash
  if (backlash_ptr && !fast) {
    const float release_pos = measuring_movement(axis, -dir, !stop_state, fast);
    *backlash_ptr = ABS(release_pos - measured_pos);
  }
  // Return to starting position
  destination[axis] = start_pos;
  do_blocking_move_to(destination, MMM_TO_MMS(CALIBRATION_FEEDRATE_TRAVEL));
  return measured_pos;
}

/**
 * Probe one side of the calibration object
 *
 *   m                  in/out - Measurement record, m.obj_center and m.obj_side will be updated.
 *   uncertainty        in     - How far away from the calibration object to begin probing
 *   side               in     - Side of probe where probe will occur
 *   probe_top_at_edge  in     - When probing sides, probe top of calibration object nearest edge
 *                               to find out height of edge
 */
inline void probe_side(measurements_t &m, const float uncertainty, const side_t side, const bool probe_top_at_edge=false) {
  const xyz_float_t dimensions = CALIBRATION_OBJECT_DIMENSIONS;
  AxisEnum axis;
  float dir;

  park_above_object(m, uncertainty);

  switch (side) {
    case TOP: {
      const float measurement = measure(Z_AXIS, -1, true, &m.backlash[TOP], uncertainty);
      m.obj_center.z = measurement - dimensions.z / 2;
      m.obj_side[TOP] = measurement;
      return;
    }
    case RIGHT: axis = X_AXIS; dir = -1; break;
    case FRONT: axis = Y_AXIS; dir =  1; break;
    case LEFT:  axis = X_AXIS; dir =  1; break;
    case BACK:  axis = Y_AXIS; dir = -1; break;
    default: return;
  }

  if (probe_top_at_edge) {
    // Probe top nearest the side we are probing
    current_position[axis] = m.obj_center[axis] + (-dir) * (dimensions[axis] / 2 - m.nozzle_outer_dimension[axis]);
    calibration_move();
    m.obj_side[TOP] = measure(Z_AXIS, -1, true, &m.backlash[TOP], uncertainty);
    m.obj_center.z = m.obj_side[TOP] - dimensions.z / 2;
  }

  // Move to safe distance to the side of the calibration object
  current_position[axis] = m.obj_center[axis] + (-dir) * (dimensions[axis] / 2 + m.nozzle_outer_dimension[axis] / 2 + uncertainty);
  calibration_move();

  // Plunge below the side of the calibration object and measure
  current_position.z = m.obj_side[TOP] - CALIBRATION_NOZZLE_TIP_HEIGHT * 0.7;
  calibration_move();
  const float measurement = measure(axis, dir, true, &m.backlash[side], uncertainty);
  m.obj_center[axis] = measurement + dir * (dimensions[axis] / 2 + m.nozzle_outer_dimension[axis] / 2);
  m.obj_side[side] = measurement;
}

/**
 * Probe all sides of the calibration calibration object
 *
 *   m                  in/out - Measurement record: center, backlash and error values be updated.
 *   uncertainty        in     - How far away from the calibration object to begin probing
 */
inline void probe_sides(measurements_t &m, const float uncertainty) {
  #ifdef CALIBRATION_MEASURE_AT_TOP_EDGES
    constexpr bool probe_top_at_edge = true;
  #else
    // Probing at the exact center only works if the center is flat. Probing on a washer
    // or bolt will require probing the top near the side edges, away from the center.
    constexpr bool probe_top_at_edge = false;
    probe_side(m, uncertainty, TOP);
  #endif

  #ifdef CALIBRATION_MEASURE_RIGHT
    probe_side(m, uncertainty, RIGHT, probe_top_at_edge);
  #endif

  #ifdef CALIBRATION_MEASURE_FRONT
    probe_side(m, uncertainty, FRONT, probe_top_at_edge);
  #endif

  #ifdef CALIBRATION_MEASURE_LEFT
    probe_side(m, uncertainty, LEFT,  probe_top_at_edge);
  #endif
  #ifdef CALIBRATION_MEASURE_BACK
    probe_side(m, uncertainty, BACK,  probe_top_at_edge);
  #endif

  // Compute the measured center of the calibration object.
  #if HAS_X_CENTER
    m.obj_center.x = (m.obj_side[LEFT] + m.obj_side[RIGHT]) / 2;
  #endif
  #if HAS_Y_CENTER
    m.obj_center.y = (m.obj_side[FRONT] + m.obj_side[BACK]) / 2;
  #endif

  // Compute the outside diameter of the nozzle at the height
  // at which it makes contact with the calibration object
  #if HAS_X_CENTER
    m.nozzle_outer_dimension.x = m.obj_side[RIGHT] - m.obj_side[LEFT] - dimensions.x;
  #endif
  #if HAS_Y_CENTER
    m.nozzle_outer_dimension.y = m.obj_side[BACK]  - m.obj_side[FRONT] - dimensions.y;
  #endif

  park_above_object(m, uncertainty);

  // The difference between the known and the measured location
  // of the calibration object is the positional error
  m.pos_error.x = (0
    #if HAS_X_CENTER
      + true_center.x - m.obj_center.x
    #endif
  );
  m.pos_error.y = (0
    #if HAS_Y_CENTER
      + true_center.y - m.obj_center.y
    #endif
  );
  m.pos_error.z = true_center.z - m.obj_center.z;
}

#if ENABLED(CALIBRATION_REPORTING)
  inline void report_measured_faces(const measurements_t &m) {
    SERIAL_ECHOLNPGM("Sides:");
    SERIAL_ECHOLNPAIR("  Top: ", m.obj_side[TOP]);
    #if ENABLED(CALIBRATION_MEASURE_LEFT)
      SERIAL_ECHOLNPAIR("  Left: ", m.obj_side[LEFT]);
    #endif
    #if ENABLED(CALIBRATION_MEASURE_RIGHT)
      SERIAL_ECHOLNPAIR("  Right: ", m.obj_side[RIGHT]);
    #endif
    #if ENABLED(CALIBRATION_MEASURE_FRONT)
      SERIAL_ECHOLNPAIR("  Front: ", m.obj_side[FRONT]);
    #endif
    #if ENABLED(CALIBRATION_MEASURE_BACK)
      SERIAL_ECHOLNPAIR("  Back: ", m.obj_side[BACK]);
    #endif
    SERIAL_EOL();
  }

  inline void report_measured_center(const measurements_t &m) {
    SERIAL_ECHOLNPGM("Center:");
    #if HAS_X_CENTER
      SERIAL_ECHOLNPAIR(" X", m.obj_center.x);
    #endif
    #if HAS_Y_CENTER
      SERIAL_ECHOLNPAIR(" Y", m.obj_center.y);
    #endif
    SERIAL_ECHOLNPAIR(" Z", m.obj_center.z);
    SERIAL_EOL();
  }

  inline void report_measured_backlash(const measurements_t &m) {
    SERIAL_ECHOLNPGM("Backlash:");
    #if ENABLED(CALIBRATION_MEASURE_LEFT)
      SERIAL_ECHOLNPAIR("  Left: ", m.backlash[LEFT]);
    #endif
    #if ENABLED(CALIBRATION_MEASURE_RIGHT)
      SERIAL_ECHOLNPAIR("  Right: ", m.backlash[RIGHT]);
    #endif
    #if ENABLED(CALIBRATION_MEASURE_FRONT)
      SERIAL_ECHOLNPAIR("  Front: ", m.backlash[FRONT]);
    #endif
    #if ENABLED(CALIBRATION_MEASURE_BACK)
      SERIAL_ECHOLNPAIR("  Back: ", m.backlash[BACK]);
    #endif
    SERIAL_ECHOLNPAIR("  Top: ", m.backlash[TOP]);
    SERIAL_EOL();
  }

  inline void report_measured_positional_error(const measurements_t &m) {
    SERIAL_CHAR('T');
    SERIAL_ECHO(int(active_extruder));
    SERIAL_ECHOLNPGM(" Positional Error:");
    #if HAS_X_CENTER
      SERIAL_ECHOLNPAIR(" X", m.pos_error.x);
    #endif
    #if HAS_Y_CENTER
      SERIAL_ECHOLNPAIR(" Y", m.pos_error.y);
    #endif
    SERIAL_ECHOLNPAIR(" Z", m.pos_error.z);
    SERIAL_EOL();
  }

  inline void report_measured_nozzle_dimensions(const measurements_t &m) {
    SERIAL_ECHOLNPGM("Nozzle Tip Outer Dimensions:");
    #if HAS_X_CENTER || HAS_Y_CENTER
      #if HAS_X_CENTER
        SERIAL_ECHOLNPAIR(" X", m.nozzle_outer_dimension.x);
      #endif
      #if HAS_Y_CENTER
        SERIAL_ECHOLNPAIR(" Y", m.nozzle_outer_dimension.y);
      #endif
    #else
      UNUSED(m);
    #endif
    SERIAL_EOL();
  }

  #if HAS_HOTEND_OFFSET
    //
    // This function requires normalize_hotend_offsets() to be called
    //
    inline void report_hotend_offsets() {
      for (uint8_t e = 1; e < HOTENDS; e++)
        SERIAL_ECHOLNPAIR("T", int(e), " Hotend Offset X", hotend_offset[e].x, " Y", hotend_offset[e].y, " Z", hotend_offset[e].z);
    }
  #endif

#endif // CALIBRATION_REPORTING

/**
 * Probe around the calibration object to measure backlash
 *
 *   m              in/out - Measurement record, updated with new readings
 *   uncertainty    in     - How far away from the object to begin probing
 */
inline void calibrate_backlash(measurements_t &m, const float uncertainty) {
  // Backlash compensation should be off while measuring backlash

  {
    // New scope for TEMPORARY_BACKLASH_CORRECTION
    TEMPORARY_BACKLASH_CORRECTION(all_off);
    TEMPORARY_BACKLASH_SMOOTHING(0.0f);

    probe_sides(m, uncertainty);

    #if ENABLED(BACKLASH_GCODE)
      #if HAS_X_CENTER
        backlash.distance_mm.x = (m.backlash[LEFT] + m.backlash[RIGHT]) / 2;
      #elif ENABLED(CALIBRATION_MEASURE_LEFT)
        backlash.distance_mm.x = m.backlash[LEFT];
      #elif ENABLED(CALIBRATION_MEASURE_RIGHT)
        backlash.distance_mm.x = m.backlash[RIGHT];
      #endif

      #if HAS_Y_CENTER
        backlash.distance_mm.y = (m.backlash[FRONT] + m.backlash[BACK]) / 2;
      #elif ENABLED(CALIBRATION_MEASURE_FRONT)
        backlash.distance_mm.y = m.backlash[FRONT];
      #elif ENABLED(CALIBRATION_MEASURE_BACK)
        backlash.distance_mm.y = m.backlash[BACK];
      #endif

      backlash.distance_mm.z = m.backlash[TOP];
    #endif
  }

  #if ENABLED(BACKLASH_GCODE)
    // Turn on backlash compensation and move in all
    // directions to take up any backlash
    {
      // New scope for TEMPORARY_BACKLASH_CORRECTION
      TEMPORARY_BACKLASH_CORRECTION(all_on);
      TEMPORARY_BACKLASH_SMOOTHING(0.0f);
      const xyz_float_t move = { 3, 3, 3 };
      current_position += move; calibration_move();
      current_position -= move; calibration_move();
    }
  #endif
}

inline void update_measurements(measurements_t &m, const AxisEnum axis) {
  current_position[axis] += m.pos_error[axis];
  m.obj_center[axis] = true_center[axis];
  m.pos_error[axis] = 0;
}

/**
 * Probe around the calibration object. Adjust the position and toolhead offset
 * using the deviation from the known position of the calibration object.
 *
 *   m              in/out - Measurement record, updated with new readings
 *   uncertainty    in     - How far away from the object to begin probing
 *   extruder       in     - What extruder to probe
 *
 * Prerequisites:
 *    - Call calibrate_backlash() beforehand for best accuracy
 */
inline void calibrate_toolhead(measurements_t &m, const float uncertainty, const uint8_t extruder) {
  TEMPORARY_BACKLASH_CORRECTION(all_on);
  TEMPORARY_BACKLASH_SMOOTHING(0.0f);

  #if HOTENDS > 1
    set_nozzle(m, extruder);
  #else
    UNUSED(extruder);
  #endif

  probe_sides(m, uncertainty);

  // Adjust the hotend offset
  #if HAS_HOTEND_OFFSET
    #if HAS_X_CENTER
      hotend_offset[extruder].x += m.pos_error.x;
    #endif
    #if HAS_Y_CENTER
      hotend_offset[extruder].y += m.pos_error.y;
    #endif
    hotend_offset[extruder].z += m.pos_error.z;
    normalize_hotend_offsets();
  #endif

  // Correct for positional error, so the object
  // is at the known actual spot
  planner.synchronize();
  #if HAS_X_CENTER
    update_measurements(m, X_AXIS);
  #endif
  #if HAS_Y_CENTER
    update_measurements(m, Y_AXIS);
  #endif
  update_measurements(m, Z_AXIS);

  sync_plan_position();
}

/**
 * Probe around the calibration object for all toolheads, adjusting the coordinate
 * system for the first nozzle and the nozzle offset for subsequent nozzles.
 *
 *   m              in/out - Measurement record, updated with new readings
 *   uncertainty    in     - How far away from the object to begin probing
 */
inline void calibrate_all_toolheads(measurements_t &m, const float uncertainty) {
  TEMPORARY_BACKLASH_CORRECTION(all_on);
  TEMPORARY_BACKLASH_SMOOTHING(0.0f);

  HOTEND_LOOP() calibrate_toolhead(m, uncertainty, e);

  #if HAS_HOTEND_OFFSET
    normalize_hotend_offsets();
  #endif

  #if HOTENDS > 1
    set_nozzle(m, 0);
  #endif
}

/**
 * Perform a full auto-calibration routine:
 *
 *   1) For each nozzle, touch top and sides of object to determine object position and
 *      nozzle offsets. Do a fast but rough search over a wider area.
 *   2) With the first nozzle, touch top and sides of object to determine backlash values
 *      for all axis (if BACKLASH_GCODE is enabled)
 *   3) For each nozzle, touch top and sides of object slowly to determine precise
 *      position of object. Adjust coordinate system and nozzle offsets so probed object
 *      location corresponds to known object location with a high degree of precision.
 */
inline void calibrate_all() {
  measurements_t m;

  #if HAS_HOTEND_OFFSET
    reset_hotend_offsets();
  #endif

  TEMPORARY_BACKLASH_CORRECTION(all_on);
  TEMPORARY_BACKLASH_SMOOTHING(0.0f);

  // Do a fast and rough calibration of the toolheads
  calibrate_all_toolheads(m, CALIBRATION_MEASUREMENT_UNKNOWN);

  #if ENABLED(BACKLASH_GCODE)
    calibrate_backlash(m, CALIBRATION_MEASUREMENT_UNCERTAIN);
  #endif

  // Cycle the toolheads so the servos settle into their "natural" positions
  #if HOTENDS > 1
    HOTEND_LOOP() set_nozzle(m, e);
  #endif

  // Do a slow and precise calibration of the toolheads
  calibrate_all_toolheads(m, CALIBRATION_MEASUREMENT_UNCERTAIN);

  current_position.x = X_CENTER;
  calibration_move();         // Park nozzle away from calibration object
}

/**
 * G425: Perform calibration with calibration object.
 *
 *   B           - Perform calibration of backlash only.
 *   T<extruder> - Perform calibration of toolhead only.
 *   V           - Probe object and print position, error, backlash and hotend offset.
 *   U           - Uncertainty, how far to start probe away from the object (mm)
 *
 *   no args     - Perform entire calibration sequence (backlash + position on all toolheads)
 */
void GcodeSuite::G425() {
  TEMPORARY_SOFT_ENDSTOP_STATE(false);
  TEMPORARY_BED_LEVELING_STATE(false);

  if (axis_unhomed_error()) return;

  measurements_t m;

  float uncertainty = parser.seenval('U') ? parser.value_float() : CALIBRATION_MEASUREMENT_UNCERTAIN;

  if (parser.seen('B'))
    calibrate_backlash(m, uncertainty);
  else if (parser.seen('T'))
    calibrate_toolhead(m, uncertainty, parser.has_value() ? parser.value_int() : active_extruder);
  #if ENABLED(CALIBRATION_REPORTING)
    else if (parser.seen('V')) {
      probe_sides(m, uncertainty);
      SERIAL_EOL();
      report_measured_faces(m);
      report_measured_center(m);
      report_measured_backlash(m);
      report_measured_nozzle_dimensions(m);
      report_measured_positional_error(m);
      #if HAS_HOTEND_OFFSET
        normalize_hotend_offsets();
        report_hotend_offsets();
      #endif
    }
  #endif
  else
    calibrate_all();
}

#endif // CALIBRATION_GCODE
