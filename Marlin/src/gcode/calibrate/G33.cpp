/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(DELTA_AUTO_CALIBRATION)

#include "../gcode.h"
#include "../../module/delta.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../module/endstops.h"
#include "../../lcd/ultralcd.h"

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

#if HOTENDS > 1
  #include "../../module/tool_change.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

constexpr uint8_t _7P_STEP = 1,              // 7-point step - to change number of calibration points
                  _4P_STEP = _7P_STEP * 2,   // 4-point step
                  NPP      = _7P_STEP * 6;   // number of calibration points on the radius
enum CalEnum : char {                        // the 7 main calibration points - add definitions if needed
  CEN      = 0,
  __A      = 1,
  _AB      = __A + _7P_STEP,
  __B      = _AB + _7P_STEP,
  _BC      = __B + _7P_STEP,
  __C      = _BC + _7P_STEP,
  _CA      = __C + _7P_STEP,
};

#define LOOP_CAL_PT(VAR, S, N) for (uint8_t VAR=S; VAR<=NPP; VAR+=N)
#define F_LOOP_CAL_PT(VAR, S, N) for (float VAR=S; VAR<NPP+0.9999; VAR+=N)
#define I_LOOP_CAL_PT(VAR, S, N) for (float VAR=S; VAR>CEN+0.9999; VAR-=N)
#define LOOP_CAL_ALL(VAR) LOOP_CAL_PT(VAR, CEN, 1)
#define LOOP_CAL_RAD(VAR) LOOP_CAL_PT(VAR, __A, _7P_STEP)
#define LOOP_CAL_ACT(VAR, _4P, _OP) LOOP_CAL_PT(VAR, _OP ? _AB : __A, _4P ? _4P_STEP : _7P_STEP)

#if HOTENDS > 1
  const uint8_t old_tool_index = active_extruder;
  #define AC_CLEANUP() ac_cleanup(old_tool_index)
#else
  #define AC_CLEANUP() ac_cleanup()
#endif

float lcd_probe_pt(const float &rx, const float &ry);

bool ac_home() {
  endstops.enable(true);
  if (!home_delta())
    return false;
  endstops.not_homing();
  return true;
}

void ac_setup(const bool reset_bed) {
  #if HOTENDS > 1
    tool_change(0, 0, true);
  #endif

  planner.synchronize();
  setup_for_endstop_or_probe_move();

  #if HAS_LEVELING
    if (reset_bed) reset_bed_level(); // After full calibration bed-level data is no longer valid
  #endif
}

void ac_cleanup(
  #if HOTENDS > 1
    const uint8_t old_tool_index
  #endif
) {
  #if ENABLED(DELTA_HOME_TO_SAFE_ZONE)
    do_blocking_move_to_z(delta_clip_start_height);
  #endif
  #if HAS_BED_PROBE
    STOW_PROBE();
  #endif
  clean_up_after_endstop_or_probe_move();
  #if HOTENDS > 1
    tool_change(old_tool_index, 0, true);
  #endif
}

void print_signed_float(const char * const prefix, const float &f) {
  SERIAL_PROTOCOLPGM("  ");
  serialprintPGM(prefix);
  SERIAL_PROTOCOLCHAR(':');
  if (f >= 0) SERIAL_CHAR('+');
  SERIAL_PROTOCOL_F(f, 2);
}

/**
 *  - Print the delta settings
 */
static void print_calibration_settings(const bool end_stops, const bool tower_angles) {
  SERIAL_PROTOCOLPAIR(".Height:", delta_height);
  if (end_stops) {
    print_signed_float(PSTR("Ex"), delta_endstop_adj[A_AXIS]);
    print_signed_float(PSTR("Ey"), delta_endstop_adj[B_AXIS]);
    print_signed_float(PSTR("Ez"), delta_endstop_adj[C_AXIS]);
  }
  if (end_stops && tower_angles) {
    SERIAL_PROTOCOLPAIR("  Radius:", delta_radius);
    SERIAL_EOL();
    SERIAL_CHAR('.');
    SERIAL_PROTOCOL_SP(13);
  }
  if (tower_angles) {
    print_signed_float(PSTR("Tx"), delta_tower_angle_trim[A_AXIS]);
    print_signed_float(PSTR("Ty"), delta_tower_angle_trim[B_AXIS]);
    print_signed_float(PSTR("Tz"), delta_tower_angle_trim[C_AXIS]);
  }
  if ((!end_stops && tower_angles) || (end_stops && !tower_angles)) { // XOR
    SERIAL_PROTOCOLPAIR("  Radius:", delta_radius);
  }
  #if HAS_BED_PROBE
    if (!end_stops && !tower_angles) {
      SERIAL_PROTOCOL_SP(30);
      print_signed_float(PSTR("Offset"), zprobe_zoffset);
    }
  #endif
  SERIAL_EOL();
}

/**
 *  - Print the probe results
 */
static void print_calibration_results(const float z_pt[NPP + 1], const bool tower_points, const bool opposite_points) {
  SERIAL_PROTOCOLPGM(".    ");
  print_signed_float(PSTR("c"), z_pt[CEN]);
  if (tower_points) {
    print_signed_float(PSTR(" x"), z_pt[__A]);
    print_signed_float(PSTR(" y"), z_pt[__B]);
    print_signed_float(PSTR(" z"), z_pt[__C]);
  }
  if (tower_points && opposite_points) {
    SERIAL_EOL();
    SERIAL_CHAR('.');
    SERIAL_PROTOCOL_SP(13);
  }
  if (opposite_points) {
    print_signed_float(PSTR("yz"), z_pt[_BC]);
    print_signed_float(PSTR("zx"), z_pt[_CA]);
    print_signed_float(PSTR("xy"), z_pt[_AB]);
  }
  SERIAL_EOL();
}

/**
 *  - Calculate the standard deviation from the zero plane
 */
static float std_dev_points(float z_pt[NPP + 1], const bool _0p_cal, const bool _1p_cal, const bool _4p_cal, const bool _4p_opp) {
  if (!_0p_cal) {
    float S2 = sq(z_pt[CEN]);
    int16_t N = 1;
    if (!_1p_cal) { // std dev from zero plane
      LOOP_CAL_ACT(rad, _4p_cal, _4p_opp) {
        S2 += sq(z_pt[rad]);
        N++;
      }
      return round(SQRT(S2 / N) * 1000.0) / 1000.0 + 0.00001;
    }
  }
  return 0.00001;
}

/**
 *  - Probe a point
 */
static float calibration_probe(const float &nx, const float &ny, const bool stow, const bool set_up) {
  #if HAS_BED_PROBE
    return probe_pt(nx, ny, set_up ? PROBE_PT_BIG_RAISE : stow ? PROBE_PT_STOW : PROBE_PT_RAISE, 0, false);
  #else
    UNUSED(stow);
    UNUSED(set_up);
    return lcd_probe_pt(nx, ny);
  #endif
}

#if HAS_BED_PROBE
  static float probe_z_shift(const float center) {
    STOW_PROBE();
    endstops.enable_z_probe(false);
    float z_shift = lcd_probe_pt(0, 0) - center;
    endstops.enable_z_probe(true);
    return z_shift;
  }
#endif

/**
 *  - Probe a grid
 */
static bool probe_calibration_points(float z_pt[NPP + 1], const int8_t probe_points, const bool towers_set, const bool stow_after_each, const bool set_up) {
  const bool _0p_calibration      = probe_points == 0,
             _1p_calibration      = probe_points == 1 || probe_points == -1,
             _4p_calibration      = probe_points == 2,
             _4p_opposite_points  = _4p_calibration && !towers_set,
             _7p_calibration      = probe_points >= 3,
             _7p_no_intermediates = probe_points == 3,
             _7p_1_intermediates  = probe_points == 4,
             _7p_2_intermediates  = probe_points == 5,
             _7p_4_intermediates  = probe_points == 6,
             _7p_6_intermediates  = probe_points == 7,
             _7p_8_intermediates  = probe_points == 8,
             _7p_11_intermediates = probe_points == 9,
             _7p_14_intermediates = probe_points == 10,
             _7p_intermed_points  = probe_points >= 4,
             _7p_6_center         = probe_points >= 5 && probe_points <= 7,
             _7p_9_center         = probe_points >= 8;

  LOOP_CAL_ALL(rad) z_pt[rad] = 0.0;

  if (!_0p_calibration) {

    if (!_7p_no_intermediates && !_7p_4_intermediates && !_7p_11_intermediates) { // probe the center
      z_pt[CEN] += calibration_probe(0, 0, stow_after_each, set_up);
      if (isnan(z_pt[CEN])) return false;
    }

    if (_7p_calibration) { // probe extra center points
      const float start  = _7p_9_center ? _CA + _7P_STEP / 3.0 : _7p_6_center ? _CA : __C,
                  steps  = _7p_9_center ? _4P_STEP / 3.0 : _7p_6_center ? _7P_STEP : _4P_STEP;
      I_LOOP_CAL_PT(rad, start, steps) {
        const float a = RADIANS(210 + (360 / NPP) *  (rad - 1)),
                    r = delta_calibration_radius * 0.1;
        z_pt[CEN] += calibration_probe(cos(a) * r, sin(a) * r, stow_after_each, set_up);
        if (isnan(z_pt[CEN])) return false;
     }
      z_pt[CEN] /= float(_7p_2_intermediates ? 7 : probe_points);
    }

    if (!_1p_calibration) {  // probe the radius
      const CalEnum start  = _4p_opposite_points ? _AB : __A;
      const float   steps  = _7p_14_intermediates ? _7P_STEP / 15.0 : // 15r * 6 + 10c = 100
                             _7p_11_intermediates ? _7P_STEP / 12.0 : // 12r * 6 +  9c = 81
                             _7p_8_intermediates  ? _7P_STEP /  9.0 : //  9r * 6 + 10c = 64
                             _7p_6_intermediates  ? _7P_STEP /  7.0 : //  7r * 6 +  7c = 49
                             _7p_4_intermediates  ? _7P_STEP /  5.0 : //  5r * 6 +  6c = 36
                             _7p_2_intermediates  ? _7P_STEP /  3.0 : //  3r * 6 +  7c = 25
                             _7p_1_intermediates  ? _7P_STEP /  2.0 : //  2r * 6 +  4c = 16
                             _7p_no_intermediates ? _7P_STEP :        //  1r * 6 +  3c = 9
                             _4P_STEP;                                // .5r * 6 +  1c = 4
      bool zig_zag = true;
      F_LOOP_CAL_PT(rad, start, _7p_9_center ? steps * 3 : steps) {
        const int8_t offset = _7p_9_center ? 2 : 0;
        for (int8_t circle = 0; circle <= offset; circle++) {
          const float a = RADIANS(210 + (360 / NPP) *  (rad - 1)),
                      r = delta_calibration_radius * (1 - 0.1 * (zig_zag ? offset - circle : circle)),
                      interpol = fmod(rad, 1);
          const float z_temp = calibration_probe(cos(a) * r, sin(a) * r, stow_after_each, set_up);
          if (isnan(z_temp)) return false;
          // split probe point to neighbouring calibration points
          z_pt[uint8_t(round(rad - interpol + NPP - 1)) % NPP + 1] += z_temp * sq(cos(RADIANS(interpol * 90)));
          z_pt[uint8_t(round(rad - interpol))           % NPP + 1] += z_temp * sq(sin(RADIANS(interpol * 90)));
        }
        zig_zag = !zig_zag;
      }
      if (_7p_intermed_points)
        LOOP_CAL_RAD(rad)
          z_pt[rad] /= _7P_STEP / steps;

      do_blocking_move_to_xy(0.0, 0.0);
    }
  }
  return true;
}

/**
 * kinematics routines and auto tune matrix scaling parameters:
 * see https://github.com/LVD-AC/Marlin-AC/tree/1.1.x-AC/documentation for
 *  - formulae for approximative forward kinematics in the end-stop displacement matrix
 *  - definition of the matrix scaling parameters
 */
static void reverse_kinematics_probe_points(float z_pt[NPP + 1], float mm_at_pt_axis[NPP + 1][ABC]) {
  float pos[XYZ] = { 0.0 };

  LOOP_CAL_ALL(rad) {
    const float a = RADIANS(210 + (360 / NPP) *  (rad - 1)),
                r = (rad == CEN ? 0.0 : delta_calibration_radius);
    pos[X_AXIS] = cos(a) * r;
    pos[Y_AXIS] = sin(a) * r;
    pos[Z_AXIS] = z_pt[rad];
    inverse_kinematics(pos);
    LOOP_XYZ(axis) mm_at_pt_axis[rad][axis] = delta[axis];
  }
}

static void forward_kinematics_probe_points(float mm_at_pt_axis[NPP + 1][ABC], float z_pt[NPP + 1]) {
  const float r_quot = delta_calibration_radius / delta_radius;

  #define ZPP(N,I,A) ((1 / 3.0 + r_quot * (N) / 3.0 ) * mm_at_pt_axis[I][A])
  #define Z00(I, A) ZPP( 0, I, A)
  #define Zp1(I, A) ZPP(+1, I, A)
  #define Zm1(I, A) ZPP(-1, I, A)
  #define Zp2(I, A) ZPP(+2, I, A)
  #define Zm2(I, A) ZPP(-2, I, A)

  z_pt[CEN] = Z00(CEN, A_AXIS) + Z00(CEN, B_AXIS) + Z00(CEN, C_AXIS);
  z_pt[__A] = Zp2(__A, A_AXIS) + Zm1(__A, B_AXIS) + Zm1(__A, C_AXIS);
  z_pt[__B] = Zm1(__B, A_AXIS) + Zp2(__B, B_AXIS) + Zm1(__B, C_AXIS);
  z_pt[__C] = Zm1(__C, A_AXIS) + Zm1(__C, B_AXIS) + Zp2(__C, C_AXIS);
  z_pt[_BC] = Zm2(_BC, A_AXIS) + Zp1(_BC, B_AXIS) + Zp1(_BC, C_AXIS);
  z_pt[_CA] = Zp1(_CA, A_AXIS) + Zm2(_CA, B_AXIS) + Zp1(_CA, C_AXIS);
  z_pt[_AB] = Zp1(_AB, A_AXIS) + Zp1(_AB, B_AXIS) + Zm2(_AB, C_AXIS);
}

static void calc_kinematics_diff_probe_points(float z_pt[NPP + 1], float delta_e[ABC], float delta_r, float delta_t[ABC]) {
  const float z_center = z_pt[CEN];
  float diff_mm_at_pt_axis[NPP + 1][ABC],
        new_mm_at_pt_axis[NPP + 1][ABC];

  reverse_kinematics_probe_points(z_pt, diff_mm_at_pt_axis);

  delta_radius += delta_r;
  LOOP_XYZ(axis) delta_tower_angle_trim[axis] += delta_t[axis];
  recalc_delta_settings();
  reverse_kinematics_probe_points(z_pt, new_mm_at_pt_axis);

  LOOP_XYZ(axis) LOOP_CAL_ALL(rad) diff_mm_at_pt_axis[rad][axis] -= new_mm_at_pt_axis[rad][axis] + delta_e[axis];
  forward_kinematics_probe_points(diff_mm_at_pt_axis, z_pt);

  LOOP_CAL_RAD(rad) z_pt[rad] -= z_pt[CEN] - z_center;
  z_pt[CEN] = z_center;

  delta_radius -= delta_r;
  LOOP_XYZ(axis) delta_tower_angle_trim[axis] -= delta_t[axis];
  recalc_delta_settings();
}

static float auto_tune_h() {
  const float r_quot = delta_calibration_radius / delta_radius;
  float h_fac = 0.0;

  h_fac = r_quot / (2.0 / 3.0);
  h_fac = 1.0 / h_fac; // (2/3)/CR
  return h_fac;
}

static float auto_tune_r() {
  const float diff = 0.01;
  float r_fac = 0.0,
        z_pt[NPP + 1] = { 0.0 },
        delta_e[ABC] = {0.0},
        delta_r = {0.0},
        delta_t[ABC] = {0.0};

  delta_r = diff;
  calc_kinematics_diff_probe_points(z_pt, delta_e, delta_r, delta_t);
  r_fac = -(z_pt[__A] + z_pt[__B] + z_pt[__C] + z_pt[_BC] + z_pt[_CA] + z_pt[_AB]) / 6.0;
  r_fac = diff / r_fac / 3.0; // 1/(3*delta_Z)
  return r_fac;
}

static float auto_tune_a() {
  const float diff = 0.01;
  float a_fac = 0.0,
        z_pt[NPP + 1] = { 0.0 },
        delta_e[ABC] = {0.0},
        delta_r = {0.0},
        delta_t[ABC] = {0.0};

  LOOP_XYZ(axis) {
    LOOP_XYZ(axis_2) delta_t[axis_2] = 0.0;
    delta_t[axis] = diff;
    calc_kinematics_diff_probe_points(z_pt, delta_e, delta_r, delta_t);
    a_fac += z_pt[uint8_t((axis * _4P_STEP) - _7P_STEP + NPP) % NPP + 1] / 6.0;
    a_fac -= z_pt[uint8_t((axis * _4P_STEP) + 1 + _7P_STEP)] / 6.0;
  }
  a_fac = diff / a_fac / 3.0; // 1/(3*delta_Z)
  return a_fac;
}

/**
 * G33 - Delta '1-4-7-point' Auto-Calibration
 *       Calibrate height, z_offset, endstops, delta radius, and tower angles.
 *
 * Parameters:
 *
 *   S   Setup mode; disables probe protection
 *
 *   Pn  Number of probe points:
 *      P-1      Checks the z_offset with a center probe and paper test.
 *      P0       Normalizes calibration.
 *      P1       Calibrates height only with center probe.
 *      P2       Probe center and towers. Calibrate height, endstops and delta radius.
 *      P3       Probe all positions: center, towers and opposite towers. Calibrate all.
 *      P4-P10   Probe all positions at different intermediate locations and average them.
 *
 *   T   Don't calibrate tower angle corrections
 *
 *   Cn.nn  Calibration precision; when omitted calibrates to maximum precision
 *
 *   Fn  Force to run at least n iterations and take the best result
 *
 *   Vn  Verbose level:
 *      V0  Dry-run mode. Report settings and probe results. No calibration.
 *      V1  Report start and end settings only
 *      V2  Report settings at each iteration
 *      V3  Report settings and probe results
 *
 *   E   Engage the probe for each point
 */
void GcodeSuite::G33() {

  const bool set_up =
    #if HAS_BED_PROBE
      parser.seen('S');
    #else
      false;
    #endif

  const int8_t probe_points = set_up ? 2 : parser.intval('P', DELTA_CALIBRATION_DEFAULT_POINTS);
  if (!WITHIN(probe_points, -1, 10)) {
    SERIAL_PROTOCOLLNPGM("?(P)oints is implausible (-1 - 10).");
    return;
  }

  const bool towers_set = !parser.seen('T');

  const float calibration_precision = set_up ? Z_CLEARANCE_BETWEEN_PROBES / 5.0 : parser.floatval('C', 0.0);
  if (calibration_precision < 0) {
    SERIAL_PROTOCOLLNPGM("?(C)alibration precision is implausible (>=0).");
    return;
  }

  const int8_t force_iterations = parser.intval('F', 0);
  if (!WITHIN(force_iterations, 0, 30)) {
    SERIAL_PROTOCOLLNPGM("?(F)orce iteration is implausible (0 - 30).");
    return;
  }

  const int8_t verbose_level = parser.byteval('V', 1);
  if (!WITHIN(verbose_level, 0, 3)) {
    SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0 - 3).");
    return;
  }

  const bool stow_after_each = parser.seen('E');

  if (set_up) {
    delta_height = 999.99;
    delta_radius = DELTA_PRINTABLE_RADIUS;
    ZERO(delta_endstop_adj);
    ZERO(delta_tower_angle_trim);
    recalc_delta_settings();
  }

  const bool _0p_calibration      = probe_points == 0,
             _1p_calibration      = probe_points == 1 || probe_points == -1,
             _4p_calibration      = probe_points == 2,
             _4p_opposite_points  = _4p_calibration && !towers_set,
             _7p_9_center         = probe_points >= 8,
             _tower_results       = (_4p_calibration && towers_set) || probe_points >= 3,
             _opposite_results    = (_4p_calibration && !towers_set) || probe_points >= 3,
             _endstop_results     = probe_points != 1 && probe_points != -1 && probe_points != 0,
             _angle_results       = probe_points >= 3  && towers_set;
  static const char save_message[] PROGMEM = "Save with M500 and/or copy to Configuration.h";
  int8_t iterations = 0;
  float test_precision,
        zero_std_dev = (verbose_level ? 999.0 : 0.0), // 0.0 in dry-run mode : forced end
        zero_std_dev_min = zero_std_dev,
        zero_std_dev_old = zero_std_dev,
        h_factor,
        r_factor,
        a_factor,
        e_old[ABC] = {
          delta_endstop_adj[A_AXIS],
          delta_endstop_adj[B_AXIS],
          delta_endstop_adj[C_AXIS]
        },
        r_old = delta_radius,
        h_old = delta_height,
        a_old[ABC] = {
          delta_tower_angle_trim[A_AXIS],
          delta_tower_angle_trim[B_AXIS],
          delta_tower_angle_trim[C_AXIS]
        };

  SERIAL_PROTOCOLLNPGM("G33 Auto Calibrate");

  if (!_1p_calibration && !_0p_calibration) { // test if the outer radius is reachable
    LOOP_CAL_RAD(axis) {
      const float a = RADIANS(210 + (360 / NPP) *  (axis - 1)),
                  r = delta_calibration_radius;
      if (!position_is_reachable(cos(a) * r, sin(a) * r)) {
        SERIAL_PROTOCOLLNPGM("?(M665 B)ed radius is implausible.");
        return;
      }
    }
  }

  // Report settings
  const char* checkingac = PSTR("Checking... AC");
  serialprintPGM(checkingac);
  if (verbose_level == 0) SERIAL_PROTOCOLPGM(" (DRY-RUN)");
  if (set_up) SERIAL_PROTOCOLPGM("  (SET-UP)");
  SERIAL_EOL();
  lcd_setstatusPGM(checkingac);

  print_calibration_settings(_endstop_results, _angle_results);

  ac_setup(!_0p_calibration && !_1p_calibration);

  if (!_0p_calibration)
    if (!ac_home()) return;

  do { // start iterations

    float z_at_pt[NPP + 1] = { 0.0 };

    test_precision = zero_std_dev_old != 999.0 ? (zero_std_dev + zero_std_dev_old) / 2 : zero_std_dev;
    iterations++;

    // Probe the points
    zero_std_dev_old = zero_std_dev;
    if (!probe_calibration_points(z_at_pt, probe_points, towers_set, stow_after_each, set_up)) {
      SERIAL_PROTOCOLLNPGM("Correct delta settings with M665 and M666");
      return AC_CLEANUP();
    }
    zero_std_dev = std_dev_points(z_at_pt, _0p_calibration, _1p_calibration, _4p_calibration, _4p_opposite_points);

    // Solve matrices

    if ((zero_std_dev < test_precision || iterations <= force_iterations) && zero_std_dev > calibration_precision) {

      #if !HAS_BED_PROBE
        test_precision = 0.00; // forced end
      #endif

      if (zero_std_dev < zero_std_dev_min) {
        // set roll-back point
        COPY(e_old, delta_endstop_adj);
        r_old = delta_radius;
        h_old = delta_height;
        COPY(a_old, delta_tower_angle_trim);
      }

      float e_delta[ABC] = { 0.0 },
            r_delta = 0.0,
            t_delta[ABC] = { 0.0 };

      /**
       * convergence matrices:
       * see https://github.com/LVD-AC/Marlin-AC/tree/1.1.x-AC/documentation for
       *  - definition of the matrix scaling parameters
       *  - matrices for 4 and 7 point calibration
       */
      #define ZP(N,I) ((N) * z_at_pt[I] / 4.0) // 4.0 = divider to normalize to integers
      #define Z12(I) ZP(12, I)
      #define Z4(I) ZP(4, I)
      #define Z2(I) ZP(2, I)
      #define Z1(I) ZP(1, I)
      #define Z0(I) ZP(0, I)

      // calculate factors
      const float cr_old = delta_calibration_radius;
      if (_7p_9_center) delta_calibration_radius *= 0.9;
      h_factor = auto_tune_h();
      r_factor = auto_tune_r();
      a_factor = auto_tune_a();
      delta_calibration_radius = cr_old;

      switch (probe_points) {
        case -1:
          #if HAS_BED_PROBE
            zprobe_zoffset += probe_z_shift(z_at_pt[CEN]);
          #endif

        case 0:
          test_precision = 0.00; // forced end
          break;

        case 1:
          test_precision = 0.00; // forced end
          LOOP_XYZ(axis) e_delta[axis] = +Z4(CEN);
          break;

        case 2:
          if (towers_set) { // see 4 point calibration (towers) matrix
            e_delta[A_AXIS] = (+Z4(__A) -Z2(__B) -Z2(__C)) * h_factor  +Z4(CEN);
            e_delta[B_AXIS] = (-Z2(__A) +Z4(__B) -Z2(__C)) * h_factor  +Z4(CEN);
            e_delta[C_AXIS] = (-Z2(__A) -Z2(__B) +Z4(__C)) * h_factor  +Z4(CEN);
            r_delta         = (+Z4(__A) +Z4(__B) +Z4(__C) -Z12(CEN)) * r_factor;
          }
          else { // see 4 point calibration (opposites) matrix
            e_delta[A_AXIS] = (-Z4(_BC) +Z2(_CA) +Z2(_AB)) * h_factor  +Z4(CEN);
            e_delta[B_AXIS] = (+Z2(_BC) -Z4(_CA) +Z2(_AB)) * h_factor  +Z4(CEN);
            e_delta[C_AXIS] = (+Z2(_BC) +Z2(_CA) -Z4(_AB)) * h_factor  +Z4(CEN);
            r_delta         = (+Z4(_BC) +Z4(_CA) +Z4(_AB) -Z12(CEN)) * r_factor;
          }
          break;

        default: // see 7 point calibration (towers & opposites) matrix
          e_delta[A_AXIS] = (+Z2(__A) -Z1(__B) -Z1(__C) -Z2(_BC) +Z1(_CA) +Z1(_AB)) * h_factor  +Z4(CEN);
          e_delta[B_AXIS] = (-Z1(__A) +Z2(__B) -Z1(__C) +Z1(_BC) -Z2(_CA) +Z1(_AB)) * h_factor  +Z4(CEN);
          e_delta[C_AXIS] = (-Z1(__A) -Z1(__B) +Z2(__C) +Z1(_BC) +Z1(_CA) -Z2(_AB)) * h_factor  +Z4(CEN);
          r_delta         = (+Z2(__A) +Z2(__B) +Z2(__C) +Z2(_BC) +Z2(_CA) +Z2(_AB) -Z12(CEN)) * r_factor;

          if (towers_set) { // see 7 point tower angle calibration (towers & opposites) matrix
            t_delta[A_AXIS] = (+Z0(__A) -Z4(__B) +Z4(__C) +Z0(_BC) -Z4(_CA) +Z4(_AB) +Z0(CEN)) * a_factor;
            t_delta[B_AXIS] = (+Z4(__A) +Z0(__B) -Z4(__C) +Z4(_BC) +Z0(_CA) -Z4(_AB) +Z0(CEN)) * a_factor;
            t_delta[C_AXIS] = (-Z4(__A) +Z4(__B) +Z0(__C) -Z4(_BC) +Z4(_CA) +Z0(_AB) +Z0(CEN)) * a_factor;
          }
          break;
      }
      LOOP_XYZ(axis) delta_endstop_adj[axis] += e_delta[axis];
      delta_radius += r_delta;
      LOOP_XYZ(axis) delta_tower_angle_trim[axis] += t_delta[axis];
    }
    else if (zero_std_dev >= test_precision) {
      // roll back
      COPY(delta_endstop_adj, e_old);
      delta_radius = r_old;
      delta_height = h_old;
      COPY(delta_tower_angle_trim, a_old);
    }

    if (verbose_level != 0) {                                    // !dry run

      // normalise angles to least squares
      if (_angle_results) {
        float a_sum = 0.0;
        LOOP_XYZ(axis) a_sum += delta_tower_angle_trim[axis];
        LOOP_XYZ(axis) delta_tower_angle_trim[axis] -= a_sum / 3.0;
      }

      // adjust delta_height and endstops by the max amount
      const float z_temp = MAX3(delta_endstop_adj[A_AXIS], delta_endstop_adj[B_AXIS], delta_endstop_adj[C_AXIS]);
      delta_height -= z_temp;
      LOOP_XYZ(axis) delta_endstop_adj[axis] -= z_temp;
    }
    recalc_delta_settings();
    NOMORE(zero_std_dev_min, zero_std_dev);

    // print report

    if (verbose_level == 3)
      print_calibration_results(z_at_pt, _tower_results, _opposite_results);

    if (verbose_level != 0) { // !dry run
      if ((zero_std_dev >= test_precision && iterations > force_iterations) || zero_std_dev <= calibration_precision) { // end iterations
        SERIAL_PROTOCOLPGM("Calibration OK");
        SERIAL_PROTOCOL_SP(32);
        #if HAS_BED_PROBE
          if (zero_std_dev >= test_precision && !_1p_calibration && !_0p_calibration)
            SERIAL_PROTOCOLPGM("rolling back.");
          else
        #endif
          {
            SERIAL_PROTOCOLPGM("std dev:");
            SERIAL_PROTOCOL_F(zero_std_dev_min, 3);
          }
        SERIAL_EOL();
        char mess[21];
        strcpy_P(mess, PSTR("Calibration sd:"));
        if (zero_std_dev_min < 1)
          sprintf_P(&mess[15], PSTR("0.%03i"), (int)round(zero_std_dev_min * 1000.0));
        else
          sprintf_P(&mess[15], PSTR("%03i.x"), (int)round(zero_std_dev_min));
        lcd_setstatus(mess);
        print_calibration_settings(_endstop_results, _angle_results);
        serialprintPGM(save_message);
        SERIAL_EOL();
      }
      else { // !end iterations
        char mess[15];
        if (iterations < 31)
          sprintf_P(mess, PSTR("Iteration : %02i"), (int)iterations);
        else
          strcpy_P(mess, PSTR("No convergence"));
        SERIAL_PROTOCOL(mess);
        SERIAL_PROTOCOL_SP(32);
        SERIAL_PROTOCOLPGM("std dev:");
        SERIAL_PROTOCOL_F(zero_std_dev, 3);
        SERIAL_EOL();
        lcd_setstatus(mess);
        if (verbose_level > 1)
          print_calibration_settings(_endstop_results, _angle_results);
      }
    }
    else { // dry run
      const char *enddryrun = PSTR("End DRY-RUN");
      serialprintPGM(enddryrun);
      SERIAL_PROTOCOL_SP(35);
      SERIAL_PROTOCOLPGM("std dev:");
      SERIAL_PROTOCOL_F(zero_std_dev, 3);
      SERIAL_EOL();

      char mess[21];
      strcpy_P(mess, enddryrun);
      strcpy_P(&mess[11], PSTR(" sd:"));
      if (zero_std_dev < 1)
        sprintf_P(&mess[15], PSTR("0.%03i"), (int)round(zero_std_dev * 1000.0));
      else
        sprintf_P(&mess[15], PSTR("%03i.x"), (int)round(zero_std_dev));
      lcd_setstatus(mess);
    }
    if (!ac_home()) return;
  }
  while (((zero_std_dev < test_precision && iterations < 31) || iterations <= force_iterations) && zero_std_dev > calibration_precision);

  AC_CLEANUP();
}

#endif // DELTA_AUTO_CALIBRATION
