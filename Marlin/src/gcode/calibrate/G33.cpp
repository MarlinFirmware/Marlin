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
#include "../../module/probe.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../module/endstops.h"
#include "../../module/tool_change.h"
#include "../../lcd/ultralcd.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

/**
 * G33 - Delta '1-4-7-point' Auto-Calibration
 *       Calibrate height, endstops, delta radius, and tower angles.
 *
 * Parameters:
 *
 *   Pn  Number of probe points:
 *
 *      P0     No probe. Normalize only.
 *      P1     Probe center and set height only.
 *      P2     Probe center and towers. Set height, endstops and delta radius.
 *      P3     Probe all positions: center, towers and opposite towers. Set all.
 *      P4-P7  Probe all positions at different locations and average them.
 *
 *   T   Don't calibrate tower angle corrections
 *
 *   Cn.nn Calibration precision; when omitted calibrates to maximum precision
 *
 *   Fn  Force to run at least n iterations and takes the best result
 *
 *   A   Auto tune calibartion factors (set in Configuration.h)
 *
 *   Vn  Verbose level:
 *
 *      V0  Dry-run mode. Report settings and probe results. No calibration.
 *      V1  Report settings
 *      V2  Report settings and probe results
 *
 *   E   Engage the probe for each point
 */

static void print_signed_float(const char * const prefix, const float &f) {
  SERIAL_PROTOCOLPGM("  ");
  serialprintPGM(prefix);
  SERIAL_PROTOCOLCHAR(':');
  if (f >= 0) SERIAL_CHAR('+');
  SERIAL_PROTOCOL_F(f, 2);
}

static void print_G33_settings(const bool end_stops, const bool tower_angles) {
  SERIAL_PROTOCOLPAIR(".Height:", DELTA_HEIGHT + home_offset[Z_AXIS]);
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
  SERIAL_EOL();
}

static void print_G33_results(const float z_at_pt[13], const bool tower_points, const bool opposite_points) {
  SERIAL_PROTOCOLPGM(".    ");
  print_signed_float(PSTR("c"), z_at_pt[0]);
  if (tower_points) {
    print_signed_float(PSTR(" x"), z_at_pt[1]);
    print_signed_float(PSTR(" y"), z_at_pt[5]);
    print_signed_float(PSTR(" z"), z_at_pt[9]);
  }
  if (tower_points && opposite_points) {
    SERIAL_EOL();
    SERIAL_CHAR('.');
    SERIAL_PROTOCOL_SP(13);
  }
  if (opposite_points) {
    print_signed_float(PSTR("yz"), z_at_pt[7]);
    print_signed_float(PSTR("zx"), z_at_pt[11]);
    print_signed_float(PSTR("xy"), z_at_pt[3]);
  }
  SERIAL_EOL();
}

static void G33_cleanup(
  #if HOTENDS > 1
    const uint8_t old_tool_index
  #endif
) {
  #if ENABLED(DELTA_HOME_TO_SAFE_ZONE)
    do_blocking_move_to_z(delta_clip_start_height);
  #endif
  STOW_PROBE();
  clean_up_after_endstop_or_probe_move();
  #if HOTENDS > 1
    tool_change(old_tool_index, 0, true);
  #endif
}

static float probe_G33_points(float z_at_pt[13], const int8_t probe_points, const bool towers_set, const bool stow_after_each) {
  const bool _0p_calibration      = probe_points == 0,
             _1p_calibration      = probe_points == 1,
             _4p_calibration      = probe_points == 2,
             _4p_opposite_points  = _4p_calibration && !towers_set,
             _7p_calibration      = probe_points >= 3 || probe_points == 0,
             _7p_half_circle      = probe_points == 3,
             _7p_double_circle    = probe_points == 5,
             _7p_triple_circle    = probe_points == 6,
             _7p_quadruple_circle = probe_points == 7,
             _7p_intermed_points  = probe_points >= 4,
             _7p_multi_circle     = probe_points >= 5;
  #if DISABLED(PROBE_MANUALLY)
    const float dx = (X_PROBE_OFFSET_FROM_EXTRUDER),
                dy = (Y_PROBE_OFFSET_FROM_EXTRUDER);
  #endif

  for (int8_t i=0; i<13; i++) z_at_pt[i] = 0.0;
  if (!_0p_calibration) {
    if (!_7p_half_circle && !_7p_triple_circle) { // probe the center
      #if ENABLED(PROBE_MANUALLY)
        z_at_pt[0] += lcd_probe_pt(0, 0);
      #else
        z_at_pt[0] += probe_pt(dx, dy, stow_after_each, 1, false);
      #endif
    }
    if (_7p_calibration) { // probe extra center points
      for (int8_t axis = _7p_multi_circle ? 11 : 9; axis > 0; axis -= _7p_multi_circle ? 2 : 4) {
        const float a = RADIANS(180 + 30 * axis), r = delta_calibration_radius * 0.1;
        #if ENABLED(PROBE_MANUALLY)
          z_at_pt[0] += lcd_probe_pt(cos(a) * r, sin(a) * r);
        #else
          z_at_pt[0] += probe_pt(cos(a) * r + dx, sin(a) * r + dy, stow_after_each, 1);
        #endif
      }
      z_at_pt[0] /= float(_7p_double_circle ? 7 : probe_points);
    }
    if (!_1p_calibration) {  // probe the radius
      bool zig_zag = true;
      const uint8_t start = _4p_opposite_points ? 3 : 1,
                    step = _4p_calibration ? 4 : _7p_half_circle ? 2 : 1;
      for (uint8_t axis = start; axis < 13; axis += step) {
        const float zigadd = (zig_zag ? 0.5 : 0.0),
                    offset_circles = _7p_quadruple_circle ? zigadd + 1.0 :
                                     _7p_triple_circle    ? zigadd + 0.5 :
                                     _7p_double_circle    ? zigadd : 0;
        for (float circles = -offset_circles ; circles <= offset_circles; circles++) {
          const float a = RADIANS(180 + 30 * axis),
                      r = delta_calibration_radius * (1 + circles * (zig_zag ? 0.1 : -0.1));
          #if ENABLED(PROBE_MANUALLY)
            z_at_pt[axis] += lcd_probe_pt(cos(a) * r, sin(a) * r);
          #else
            z_at_pt[axis] += probe_pt(cos(a) * r + dx, sin(a) * r + dy, stow_after_each, 1);
          #endif
        }
        zig_zag = !zig_zag;
        z_at_pt[axis] /= (2 * offset_circles + 1);
      }
    }
    if (_7p_intermed_points) // average intermediates to tower and opposites
      for (uint8_t axis = 1; axis < 13; axis += 2)
        z_at_pt[axis] = (z_at_pt[axis] + (z_at_pt[axis + 1] + z_at_pt[(axis + 10) % 12 + 1]) / 2.0) / 2.0;

    float S1 = z_at_pt[0],
          S2 = sq(z_at_pt[0]);
    int16_t N = 1;
    if (!_1p_calibration) // std dev from zero plane
      for (uint8_t axis = (_4p_opposite_points ? 3 : 1); axis < 13; axis += (_4p_calibration ? 4 : 2)) {
        S1 += z_at_pt[axis];
        S2 += sq(z_at_pt[axis]);
        N++;
      }
    return round(SQRT(S2 / N) * 1000.0) / 1000.0 + 0.00001;
  }
  else return 0.00001;
}

#if DISABLED(PROBE_MANUALLY)
  static void G33_auto_tune() {
    float z_at_pt[13] = { 0.0 }, z_at_pt_base[13] = { 0.0 }, z_temp,
          h_fac = 0.0, r_fac = 0.0, a_fac = 0.0, norm = 0.8;

    #define ZP(N,I) ((N) * z_at_pt[I])
    #define Z06(I)  ZP(6, I)
    #define Z03(I)  ZP(3, I)
    #define Z02(I)  ZP(2, I)
    #define Z01(I)  ZP(1, I)
    #define Z32(I)  ZP(3/2, I)

    SERIAL_PROTOCOLPGM("AUTO TUNE baseline");
    SERIAL_EOL();
    probe_G33_points(z_at_pt_base, 3, true, false);
    print_G33_results(z_at_pt_base, true, true);

    LOOP_XYZ(axis) {
      delta_endstop_adj[axis] -= 1.0;
      endstops.enable(true);
      if (!home_delta())
        return;
      endstops.not_homing();
      SERIAL_PROTOCOLPGM("Tuning E");
      SERIAL_PROTOCOL((char)tolower(axis_codes[axis]));
      SERIAL_EOL();
      probe_G33_points(z_at_pt, 3, true, false);
      for (int8_t i=0; i<13; i++) z_at_pt[i] -= z_at_pt_base[i];
      print_G33_results(z_at_pt, true, true);
      delta_endstop_adj[axis] += 1.0;
      switch (axis) {
        case A_AXIS :
        h_fac += 4.0 / (Z03(0) +Z01(1)                         +Z32(11) +Z32(3)); //displacement by X-tower end-stop
        break;
        case B_AXIS :
        h_fac += 4.0 / (Z03(0)         +Z01(5)         +Z32(7)          +Z32(3)); //displacement by Y-tower end-stop
        break;
        case C_AXIS :
        h_fac += 4.0 / (Z03(0)                 +Z01(9) +Z32(7) +Z32(11)        ); //displacement by Z-tower end-stop
        break;
      }
    }
    h_fac /= 3.0;
    h_fac *= norm; // normalize to 1.02 for Kossel mini

    for (int8_t zig_zag = -1; zig_zag < 2; zig_zag += 2) {
      delta_radius += 1.0 * zig_zag;
      recalc_delta_settings(delta_radius, delta_diagonal_rod, delta_tower_angle_trim);
      endstops.enable(true);
      if (!home_delta())
        return;
      endstops.not_homing();
      SERIAL_PROTOCOLPGM("Tuning R");
      SERIAL_PROTOCOL(zig_zag == -1 ? "-" : "+");
      SERIAL_EOL();
      probe_G33_points(z_at_pt, 3, true, false);
      for (int8_t i=0; i<13; i++) z_at_pt[i] -= z_at_pt_base[i];
      print_G33_results(z_at_pt, true, true);
      delta_radius -= 1.0 * zig_zag;
      recalc_delta_settings(delta_radius, delta_diagonal_rod, delta_tower_angle_trim);
      r_fac -= zig_zag * 6.0 / (Z03(1) +Z03(5) +Z03(9) +Z03(7) +Z03(11) +Z03(3)); //displacement by delta radius          
    }
    r_fac /= 2.0;
    r_fac *= 3*norm; // normalize to 2.25 for Kossel mini

    LOOP_XYZ(axis) {
      delta_tower_angle_trim[axis] += 1.0;
      delta_endstop_adj[(axis+1) % 3] -= 1.0/4.5;
      delta_endstop_adj[(axis+2) % 3] += 1.0/4.5;
      z_temp = MAX3(delta_endstop_adj[A_AXIS], delta_endstop_adj[B_AXIS], delta_endstop_adj[C_AXIS]);
      home_offset[Z_AXIS] -= z_temp;
      LOOP_XYZ(axis) delta_endstop_adj[axis] -= z_temp;
      recalc_delta_settings(delta_radius, delta_diagonal_rod, delta_tower_angle_trim);
      endstops.enable(true);
      if (!home_delta())
        return;
      endstops.not_homing();
      SERIAL_PROTOCOLPGM("Tuning T");
      SERIAL_PROTOCOL((char)tolower(axis_codes[axis]));
      SERIAL_EOL();
      probe_G33_points(z_at_pt, 3, true, false);
      for (int8_t i=0; i<13; i++) z_at_pt[i] -= z_at_pt_base[i];
      print_G33_results(z_at_pt, true, true);
      delta_tower_angle_trim[axis] -= 1.0;
      delta_endstop_adj[(axis+1) % 3] += 1.0/4.5;
      delta_endstop_adj[(axis+2) % 3] -= 1.0/4.5;
      z_temp = MAX3(delta_endstop_adj[A_AXIS], delta_endstop_adj[B_AXIS], delta_endstop_adj[C_AXIS]);
      home_offset[Z_AXIS] -= z_temp;
      LOOP_XYZ(axis) delta_endstop_adj[axis] -= z_temp;
      recalc_delta_settings(delta_radius, delta_diagonal_rod, delta_tower_angle_trim);
      switch (axis) {
        case A_AXIS :
        a_fac += 4.0 / (       Z06(5) -Z06(9)         +Z06(11) -Z06(3)); //displacement by alfa tower angle
        break;
        case B_AXIS :
        a_fac += 4.0 / (-Z06(1)       +Z06(9) -Z06(7)          +Z06(3)); //displacement by beta tower angle
        break;
        case C_AXIS :
        a_fac += 4.0 / (Z06(1) -Z06(5)        +Z06(7) -Z06(11)        ); //displacement by gamma tower angle
        break;
      }
    }
    a_fac /= 3.0;
    a_fac *= norm; // normalize to 0.83 for Kossel mini

    endstops.enable(true);
    if (!home_delta())
      return;
    endstops.not_homing();
    print_signed_float(PSTR( "H_FACTOR: "), h_fac);
    print_signed_float(PSTR(" R_FACTOR: "), r_fac);
    print_signed_float(PSTR(" A_FACTOR: "), a_fac);
    SERIAL_EOL();
    SERIAL_PROTOCOLPGM("Copy these values to Configuration.h");
    SERIAL_EOL();
  }
#endif

void GcodeSuite::G33() {

  const int8_t probe_points = parser.intval('P', DELTA_CALIBRATION_DEFAULT_POINTS);
  if (!WITHIN(probe_points, 0, 7)) {
    SERIAL_PROTOCOLLNPGM("?(P)oints is implausible (0-7).");
    return;
  }

  const int8_t verbose_level = parser.byteval('V', 1);
  if (!WITHIN(verbose_level, 0, 2)) {
    SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-2).");
    return;
  }

  const float calibration_precision = parser.floatval('C');
  if (calibration_precision < 0) {
    SERIAL_PROTOCOLLNPGM("?(C)alibration precision is implausible (>=0).");
    return;
  }

  const int8_t force_iterations = parser.intval('F', 0);
  if (!WITHIN(force_iterations, 0, 30)) {
    SERIAL_PROTOCOLLNPGM("?(F)orce iteration is implausible (0-30).");
    return;
  }

  const bool towers_set           = !parser.boolval('T'),
             auto_tune            = parser.boolval('A'),
             stow_after_each      = parser.boolval('E'),
             _0p_calibration      = probe_points == 0,
             _1p_calibration      = probe_points == 1,
             _4p_calibration      = probe_points == 2,
             _tower_results       = (_4p_calibration && towers_set) 
                                    || probe_points >= 3 || probe_points == 0,
             _opposite_results    = (_4p_calibration && !towers_set) 
                                    || probe_points >= 3 || probe_points == 0,
             _endstop_results     = probe_points != 1,
             _angle_results       = (probe_points >= 3 || probe_points == 0) && towers_set,
             _7p_double_circle    = probe_points == 5,
             _7p_triple_circle    = probe_points == 6,
             _7p_quadruple_circle = probe_points == 7;
  const static char save_message[] PROGMEM = "Save with M500 and/or copy to Configuration.h";
  int8_t iterations = 0;
  float test_precision,
        zero_std_dev = (verbose_level ? 999.0 : 0.0), // 0.0 in dry-run mode : forced end
        zero_std_dev_min = zero_std_dev,
        e_old[ABC] = {
          delta_endstop_adj[A_AXIS],
          delta_endstop_adj[B_AXIS],
          delta_endstop_adj[C_AXIS]
        },
        dr_old = delta_radius,
        zh_old = home_offset[Z_AXIS],
        ta_old[ABC] = {
          delta_tower_angle_trim[A_AXIS],
          delta_tower_angle_trim[B_AXIS],
          delta_tower_angle_trim[C_AXIS]
        };

  SERIAL_PROTOCOLLNPGM("G33 Auto Calibrate");

  if (!_1p_calibration && !_0p_calibration) {  // test if the outer radius is reachable
    const float circles = (_7p_quadruple_circle ? 1.5 :
                           _7p_triple_circle    ? 1.0 :
                           _7p_double_circle    ? 0.5 : 0),
                r = (1 + circles * 0.1) * delta_calibration_radius;
    for (uint8_t axis = 1; axis < 13; ++axis) {
      const float a = RADIANS(180 + 30 * axis);
      if (!position_is_reachable_xy(cos(a) * r, sin(a) * r)) {
        SERIAL_PROTOCOLLNPGM("?(M665 B)ed radius is implausible.");
        return;
      }
    }
  }

  stepper.synchronize();
  #if HAS_LEVELING
    reset_bed_level(); // After calibration bed-level data is no longer valid
  #endif

  #if HOTENDS > 1
    const uint8_t old_tool_index = active_extruder;
    tool_change(0, 0, true);
    #define G33_CLEANUP() G33_cleanup(old_tool_index)
  #else
    #define G33_CLEANUP() G33_cleanup()
  #endif

  setup_for_endstop_or_probe_move();
  endstops.enable(true);
  if (!_0p_calibration) {
    if (!home_delta())
      return;
    endstops.not_homing();
  }

  if (auto_tune) {
    #if ENABLED(PROBE_MANUALLY)
      SERIAL_PROTOCOLLNPGM("A probe is needed for auto-tune");
    #else
      G33_auto_tune();
    #endif
    G33_CLEANUP();
    return;
  }

  // print settings

  const char *checkingac = PSTR("Checking... AC"); // TODO: Make translatable string
  serialprintPGM(checkingac);
  if (verbose_level == 0) SERIAL_PROTOCOLPGM(" (DRY-RUN)");
  SERIAL_EOL();
  lcd_setstatusPGM(checkingac);

  print_G33_settings(_endstop_results, _angle_results);

  do {

    float z_at_pt[13] = { 0.0 };

    test_precision = zero_std_dev;

    iterations++;

    // Probe the points

    zero_std_dev = probe_G33_points(z_at_pt, probe_points, towers_set, stow_after_each);

    // Solve matrices

    if ((zero_std_dev < test_precision || iterations <= force_iterations) && zero_std_dev > calibration_precision) {
      if (zero_std_dev < zero_std_dev_min) {
        COPY(e_old, delta_endstop_adj);
        dr_old = delta_radius;
        zh_old = home_offset[Z_AXIS];
        COPY(ta_old, delta_tower_angle_trim);
      }

      float e_delta[ABC] = { 0.0 }, r_delta = 0.0, t_delta[ABC] = { 0.0 };
      const float r_diff = delta_radius - delta_calibration_radius,
                  h_factor = 1 / 6.0 *
                  #ifdef H_FACTOR
                    (H_FACTOR),                                       //set in Configuration.h
                  #else
                    (1.00 + r_diff * 0.001),                          //1.02 for r_diff = 20mm
                  #endif
                  r_factor = 1 / 6.0 *
                  #ifdef R_FACTOR
                    -(R_FACTOR),                                      //set in Configuration.h
                  #else
                    -(1.75 + 0.005 * r_diff + 0.001 * sq(r_diff)),    //2.25 for r_diff = 20mm
                  #endif
                  a_factor = 1 / 6.0 *
                  #ifdef A_FACTOR
                    (A_FACTOR);                                       //set in Configuration.h
                  #else
                    (66.66 / delta_calibration_radius);               //0.83 for cal_rd = 80mm
                  #endif

      #define ZP(N,I) ((N) * z_at_pt[I])
      #define Z6(I) ZP(6, I)
      #define Z4(I) ZP(4, I)
      #define Z2(I) ZP(2, I)
      #define Z1(I) ZP(1, I)

      #if ENABLED(PROBE_MANUALLY)
        test_precision = 0.00; // forced end
      #endif

      switch (probe_points) {
        case 0:
          test_precision = 0.00; // forced end
          break;

        case 1:
          test_precision = 0.00; // forced end
          LOOP_XYZ(axis) e_delta[axis] = Z1(0);
          break;

        case 2:
          if (towers_set) {
            e_delta[A_AXIS] = (Z6(0) + Z4(1) - Z2(5) - Z2(9)) * h_factor;
            e_delta[B_AXIS] = (Z6(0) - Z2(1) + Z4(5) - Z2(9)) * h_factor;
            e_delta[C_AXIS] = (Z6(0) - Z2(1) - Z2(5) + Z4(9)) * h_factor;
            r_delta         = (Z6(0) - Z2(1) - Z2(5) - Z2(9)) * r_factor;
          }
          else {
            e_delta[A_AXIS] = (Z6(0) - Z4(7) + Z2(11) + Z2(3)) * h_factor;
            e_delta[B_AXIS] = (Z6(0) + Z2(7) - Z4(11) + Z2(3)) * h_factor;
            e_delta[C_AXIS] = (Z6(0) + Z2(7) + Z2(11) - Z4(3)) * h_factor;
            r_delta         = (Z6(0) - Z2(7) - Z2(11) - Z2(3)) * r_factor;
          }
          break;

        default:
          e_delta[A_AXIS] = (Z6(0) + Z2(1) - Z1(5) - Z1(9) - Z2(7) + Z1(11) + Z1(3)) * h_factor;
          e_delta[B_AXIS] = (Z6(0) - Z1(1) + Z2(5) - Z1(9) + Z1(7) - Z2(11) + Z1(3)) * h_factor;
          e_delta[C_AXIS] = (Z6(0) - Z1(1) - Z1(5) + Z2(9) + Z1(7) + Z1(11) - Z2(3)) * h_factor;
          r_delta         = (Z6(0) - Z1(1) - Z1(5) - Z1(9) - Z1(7) - Z1(11) - Z1(3)) * r_factor;

          if (towers_set) {
            t_delta[A_AXIS] = (       - Z4(5) + Z4(9)         - Z4(11) + Z4(3)) * a_factor;
            t_delta[B_AXIS] = ( Z4(1)         - Z4(9) + Z4(7)          - Z4(3)) * a_factor;
            t_delta[C_AXIS] = (-Z4(1) + Z4(5)         - Z4(7) + Z4(11)        ) * a_factor;
            e_delta[A_AXIS] += (t_delta[B_AXIS] - t_delta[C_AXIS]) / 4.5;
            e_delta[B_AXIS] += (t_delta[C_AXIS] - t_delta[A_AXIS]) / 4.5;
            e_delta[C_AXIS] += (t_delta[A_AXIS] - t_delta[B_AXIS]) / 4.5;
          }
          break;
      }

      LOOP_XYZ(axis) delta_endstop_adj[axis] += e_delta[axis];
      delta_radius += r_delta;
      LOOP_XYZ(axis) delta_tower_angle_trim[axis] += t_delta[axis];
    }
    else if (zero_std_dev >= test_precision) {   // step one back
      COPY(delta_endstop_adj, e_old);
      delta_radius = dr_old;
      home_offset[Z_AXIS] = zh_old;
      COPY(delta_tower_angle_trim, ta_old);
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
      home_offset[Z_AXIS] -= z_temp;
      LOOP_XYZ(axis) delta_endstop_adj[axis] -= z_temp;
    }
    recalc_delta_settings(delta_radius, delta_diagonal_rod, delta_tower_angle_trim);
    NOMORE(zero_std_dev_min, zero_std_dev);

    // print report

    if (verbose_level != 1)
      print_G33_results(z_at_pt, _tower_results, _opposite_results);

    if (verbose_level != 0) {                                    // !dry run
      if ((zero_std_dev >= test_precision && iterations > force_iterations) || zero_std_dev <= calibration_precision) {  // end iterations
        SERIAL_PROTOCOLPGM("Calibration OK");
        SERIAL_PROTOCOL_SP(32);
        #if DISABLED(PROBE_MANUALLY)
          if (zero_std_dev >= test_precision && !_1p_calibration)
            SERIAL_PROTOCOLPGM("rolling back.");
          else
        #endif
          {
            SERIAL_PROTOCOLPGM("std dev:");
            SERIAL_PROTOCOL_F(zero_std_dev_min, 3);
          }
        SERIAL_EOL();
        char mess[21];
        sprintf_P(mess, PSTR("Calibration sd:"));
        if (zero_std_dev_min < 1)
          sprintf_P(&mess[15], PSTR("0.%03i"), (int)round(zero_std_dev_min * 1000.0));
        else
          sprintf_P(&mess[15], PSTR("%03i.x"), (int)round(zero_std_dev_min));
        lcd_setstatus(mess);
        print_G33_settings(_endstop_results, _angle_results);
        serialprintPGM(save_message);
        SERIAL_EOL();
      }
      else {                                                     // !end iterations
        char mess[15];
        if (iterations < 31)
          sprintf_P(mess, PSTR("Iteration : %02i"), (int)iterations);
        else
          sprintf_P(mess, PSTR("No convergence"));
        SERIAL_PROTOCOL(mess);
        SERIAL_PROTOCOL_SP(32);
        SERIAL_PROTOCOLPGM("std dev:");
        SERIAL_PROTOCOL_F(zero_std_dev, 3);
        SERIAL_EOL();
        lcd_setstatus(mess);
        print_G33_settings(_endstop_results, _angle_results);
      }
    }
    else {                                                       // dry run
      const char *enddryrun = PSTR("End DRY-RUN");
      serialprintPGM(enddryrun);
      SERIAL_PROTOCOL_SP(35);
      SERIAL_PROTOCOLPGM("std dev:");
      SERIAL_PROTOCOL_F(zero_std_dev, 3);
      SERIAL_EOL();

      char mess[21];
      sprintf_P(mess, enddryrun);
      sprintf_P(&mess[11], PSTR(" sd:"));
      if (zero_std_dev < 1)
        sprintf_P(&mess[15], PSTR("0.%03i"), (int)round(zero_std_dev * 1000.0));
      else
        sprintf_P(&mess[15], PSTR("%03i.x"), (int)round(zero_std_dev));
      lcd_setstatus(mess);
    }

    endstops.enable(true);
    if (!home_delta())
      return;
    endstops.not_homing();

  }
  while (((zero_std_dev < test_precision && iterations < 31) || iterations <= force_iterations) && zero_std_dev > calibration_precision);

  G33_CLEANUP();
}

#endif // DELTA_AUTO_CALIBRATION
