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

#include "common.h"

#if HOTENDS > 1
  #include "../control/tool_change.h"
#endif

/**
 * G33 - Delta '1-4-7-point' Auto-Calibration
 *       Calibrate height, endstops, delta radius, and tower angles.
 *
 * Parameters:
 *
 *   Pn  Number of probe points:
 *
 *      P1     Probe center and set height only.
 *      P2     Probe center and towers. Set height, endstops, and delta radius.
 *      P3     Probe all positions: center, towers and opposite towers. Set all.
 *      P4-P7  Probe all positions at different locations and average them.
 *
 *   T0  Don't calibrate tower angle corrections
 *
 *   Cn.nn Calibration precision; when omitted calibrates to maximum precision
 *
 *   Fn  Force to run at least n iterations and takes the best result
 *
 *   Vn  Verbose level:
 *
 *      V0  Dry-run mode. Report settings and probe results. No calibration.
 *      V1  Report settings
 *      V2  Report settings and probe results
 *
 *   E   Engage the probe for each point
 */

void print_signed_float(const char * const prefix, const float &f) {
  SERIAL_PROTOCOLPGM("  ");
  serialprintPGM(prefix);
  SERIAL_PROTOCOLCHAR(':');
  if (f >= 0) SERIAL_CHAR('+');
  SERIAL_PROTOCOL_F(f, 2);
}

inline void print_G33_settings(const bool end_stops, const bool tower_angles){ // TODO echo these to LCD ???
  SERIAL_PROTOCOLPAIR(".Height:", DELTA_HEIGHT + home_offset[Z_AXIS]);
  if (end_stops) {
    print_signed_float(PSTR("  Ex"), endstop_adj[A_AXIS]);
    print_signed_float(PSTR("Ey"), endstop_adj[B_AXIS]);
    print_signed_float(PSTR("Ez"), endstop_adj[C_AXIS]);
    SERIAL_PROTOCOLPAIR("    Radius:", delta_radius);
  }
  SERIAL_EOL();
  if (tower_angles) {
    SERIAL_PROTOCOLPGM(".Tower angle :  ");
    print_signed_float(PSTR("Tx"), delta_tower_angle_trim[A_AXIS]);
    print_signed_float(PSTR("Ty"), delta_tower_angle_trim[B_AXIS]);
    SERIAL_PROTOCOLLNPGM("  Tz:+0.00");
  }
}

void G33_cleanup(
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

void gcode_G33() {

  const int8_t probe_points = parser.intval('P', DELTA_CALIBRATION_DEFAULT_POINTS);
  if (!WITHIN(probe_points, 1, 7)) {
    SERIAL_PROTOCOLLNPGM("?(P)oints is implausible (1-7).");
    return;
  }

  const int8_t verbose_level = parser.byteval('V', 1);
  if (!WITHIN(verbose_level, 0, 2)) {
    SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-2).");
    return;
  }

  const float calibration_precision = parser.floatval('C');
  if (calibration_precision < 0) {
    SERIAL_PROTOCOLLNPGM("?(C)alibration precision is implausible (>0).");
    return;
  }

  const int8_t force_iterations = parser.intval('F', 0);
  if (!WITHIN(force_iterations, 0, 30)) {
    SERIAL_PROTOCOLLNPGM("?(F)orce iteration is implausible (0-30).");
    return;
  }

  const bool towers_set           = parser.boolval('T', true),
             stow_after_each      = parser.boolval('E'),
             _1p_calibration      = probe_points == 1,
             _4p_calibration      = probe_points == 2,
             _4p_towers_points    = _4p_calibration && towers_set,
             _4p_opposite_points  = _4p_calibration && !towers_set,
             _7p_calibration      = probe_points >= 3,
             _7p_half_circle      = probe_points == 3,
             _7p_double_circle    = probe_points == 5,
             _7p_triple_circle    = probe_points == 6,
             _7p_quadruple_circle = probe_points == 7,
             _7p_multi_circle     = _7p_double_circle || _7p_triple_circle || _7p_quadruple_circle,
             _7p_intermed_points  = _7p_calibration && !_7p_half_circle;
  const static char save_message[] PROGMEM = "Save with M500 and/or copy to Configuration.h";
  const float dx = (X_PROBE_OFFSET_FROM_EXTRUDER),
              dy = (Y_PROBE_OFFSET_FROM_EXTRUDER);
  int8_t iterations = 0;
  float test_precision,
        zero_std_dev = (verbose_level ? 999.0 : 0.0), // 0.0 in dry-run mode : forced end
        zero_std_dev_old = zero_std_dev,
        zero_std_dev_min = zero_std_dev,
        e_old[XYZ] = {
          endstop_adj[A_AXIS],
          endstop_adj[B_AXIS],
          endstop_adj[C_AXIS]
        },
        dr_old = delta_radius,
        zh_old = home_offset[Z_AXIS],
        alpha_old = delta_tower_angle_trim[A_AXIS],
        beta_old = delta_tower_angle_trim[B_AXIS];

  if (!_1p_calibration) {  // test if the outer radius is reachable
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
  SERIAL_PROTOCOLLNPGM("G33 Auto Calibrate");

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
  if (!home_delta())
    return;
  endstops.not_homing();

  // print settings

  const char *checkingac = PSTR("Checking... AC"); // TODO: Make translatable string
  serialprintPGM(checkingac);
  if (verbose_level == 0) SERIAL_PROTOCOLPGM(" (DRY-RUN)");
  SERIAL_EOL();
  lcd_setstatusPGM(checkingac);

  print_G33_settings(!_1p_calibration, _7p_calibration && towers_set);

  #if DISABLED(PROBE_MANUALLY)
    const float measured_z = probe_pt(dx, dy, stow_after_each, 1, false); // 1st probe to set height
    if (isnan(measured_z)) return G33_CLEANUP();
    home_offset[Z_AXIS] -= measured_z;
  #endif

  do {

    float z_at_pt[13] = { 0.0 };

    test_precision = zero_std_dev_old != 999.0 ? (zero_std_dev + zero_std_dev_old) / 2 : zero_std_dev;

    iterations++;

    // Probe the points

    if (!_7p_half_circle && !_7p_triple_circle) { // probe the center
      #if ENABLED(PROBE_MANUALLY)
        z_at_pt[0] += lcd_probe_pt(0, 0);
      #else
        z_at_pt[0] += probe_pt(dx, dy, stow_after_each, 1, false);
        if (isnan(z_at_pt[0])) return G33_CLEANUP();
      #endif
    }
    if (_7p_calibration) { // probe extra center points
      for (int8_t axis = _7p_multi_circle ? 11 : 9; axis > 0; axis -= _7p_multi_circle ? 2 : 4) {
        const float a = RADIANS(180 + 30 * axis), r = delta_calibration_radius * 0.1;
        #if ENABLED(PROBE_MANUALLY)
          z_at_pt[0] += lcd_probe_pt(cos(a) * r, sin(a) * r);
        #else
          z_at_pt[0] += probe_pt(cos(a) * r + dx, sin(a) * r + dy, stow_after_each, 1);
          if (isnan(z_at_pt[0])) return G33_CLEANUP();
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
            if (isnan(z_at_pt[axis])) return G33_CLEANUP();
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
    zero_std_dev_old = zero_std_dev;
    zero_std_dev = round(SQRT(S2 / N) * 1000.0) / 1000.0 + 0.00001;

    // Solve matrices

    if ((zero_std_dev < test_precision && zero_std_dev > calibration_precision) || iterations <= force_iterations) {
      if (zero_std_dev < zero_std_dev_min) {
        COPY(e_old, endstop_adj);
        dr_old = delta_radius;
        zh_old = home_offset[Z_AXIS];
        alpha_old = delta_tower_angle_trim[A_AXIS];
        beta_old = delta_tower_angle_trim[B_AXIS];
      }

      float e_delta[XYZ] = { 0.0 }, r_delta = 0.0, t_alpha = 0.0, t_beta = 0.0;
      const float r_diff = delta_radius - delta_calibration_radius,
                  h_factor = 1.00 + r_diff * 0.001,                          //1.02 for r_diff = 20mm
                  r_factor = -(1.75 + 0.005 * r_diff + 0.001 * sq(r_diff)),  //2.25 for r_diff = 20mm
                  a_factor = 100.0 / delta_calibration_radius;               //1.25 for cal_rd = 80mm

      #define ZP(N,I) ((N) * z_at_pt[I])
      #define Z1000(I) ZP(1.00, I)
      #define Z1050(I) ZP(h_factor, I)
      #define Z0700(I) ZP(h_factor * 2.0 / 3.00, I)
      #define Z0350(I) ZP(h_factor / 3.00, I)
      #define Z0175(I) ZP(h_factor / 6.00, I)
      #define Z2250(I) ZP(r_factor, I)
      #define Z0750(I) ZP(r_factor / 3.00, I)
      #define Z0375(I) ZP(r_factor / 6.00, I)
      #define Z0444(I) ZP(a_factor * 4.0 / 9.0, I)
      #define Z0888(I) ZP(a_factor * 8.0 / 9.0, I)

      #if ENABLED(PROBE_MANUALLY)
        test_precision = 0.00; // forced end
      #endif

      switch (probe_points) {
        case 1:
          test_precision = 0.00; // forced end
          LOOP_XYZ(i) e_delta[i] = Z1000(0);
          break;

        case 2:
          if (towers_set) {
            e_delta[X_AXIS] = Z1050(0) + Z0700(1) - Z0350(5) - Z0350(9);
            e_delta[Y_AXIS] = Z1050(0) - Z0350(1) + Z0700(5) - Z0350(9);
            e_delta[Z_AXIS] = Z1050(0) - Z0350(1) - Z0350(5) + Z0700(9);
            r_delta         = Z2250(0) - Z0750(1) - Z0750(5) - Z0750(9);
          }
          else {
            e_delta[X_AXIS] = Z1050(0) - Z0700(7) + Z0350(11) + Z0350(3);
            e_delta[Y_AXIS] = Z1050(0) + Z0350(7) - Z0700(11) + Z0350(3);
            e_delta[Z_AXIS] = Z1050(0) + Z0350(7) + Z0350(11) - Z0700(3);
            r_delta         = Z2250(0) - Z0750(7) - Z0750(11) - Z0750(3);
          }
          break;

        default:
          e_delta[X_AXIS] = Z1050(0) + Z0350(1) - Z0175(5) - Z0175(9) - Z0350(7) + Z0175(11) + Z0175(3);
          e_delta[Y_AXIS] = Z1050(0) - Z0175(1) + Z0350(5) - Z0175(9) + Z0175(7) - Z0350(11) + Z0175(3);
          e_delta[Z_AXIS] = Z1050(0) - Z0175(1) - Z0175(5) + Z0350(9) + Z0175(7) + Z0175(11) - Z0350(3);
          r_delta         = Z2250(0) - Z0375(1) - Z0375(5) - Z0375(9) - Z0375(7) - Z0375(11) - Z0375(3);

          if (towers_set) {
            t_alpha = Z0444(1) - Z0888(5) + Z0444(9) + Z0444(7) - Z0888(11) + Z0444(3);
            t_beta  = Z0888(1) - Z0444(5) - Z0444(9) + Z0888(7) - Z0444(11) - Z0444(3);
          }
          break;
      }

      LOOP_XYZ(axis) endstop_adj[axis] += e_delta[axis];
      delta_radius += r_delta;
      delta_tower_angle_trim[A_AXIS] += t_alpha;
      delta_tower_angle_trim[B_AXIS] += t_beta;

      // adjust delta_height and endstops by the max amount
      const float z_temp = MAX3(endstop_adj[A_AXIS], endstop_adj[B_AXIS], endstop_adj[C_AXIS]);
      home_offset[Z_AXIS] -= z_temp;
      LOOP_XYZ(i) endstop_adj[i] -= z_temp;

      recalc_delta_settings(delta_radius, delta_diagonal_rod);
    }
    else if (zero_std_dev >= test_precision) {   // step one back
      COPY(endstop_adj, e_old);
      delta_radius = dr_old;
      home_offset[Z_AXIS] = zh_old;
      delta_tower_angle_trim[A_AXIS] = alpha_old;
      delta_tower_angle_trim[B_AXIS] = beta_old;

      recalc_delta_settings(delta_radius, delta_diagonal_rod);
    }
    NOMORE(zero_std_dev_min, zero_std_dev);

    // print report

    if (verbose_level != 1) {
      SERIAL_PROTOCOLPGM(".    ");
      print_signed_float(PSTR("c"), z_at_pt[0]);
      if (_4p_towers_points || _7p_calibration) {
        print_signed_float(PSTR("   x"), z_at_pt[1]);
        print_signed_float(PSTR(" y"), z_at_pt[5]);
        print_signed_float(PSTR(" z"), z_at_pt[9]);
      }
      if (!_4p_opposite_points) SERIAL_EOL();
      if ((_4p_opposite_points) || _7p_calibration) {
        if (_7p_calibration) {
          SERIAL_CHAR('.');
          SERIAL_PROTOCOL_SP(13);
        }
        print_signed_float(PSTR("  yz"), z_at_pt[7]);
        print_signed_float(PSTR("zx"), z_at_pt[11]);
        print_signed_float(PSTR("xy"), z_at_pt[3]);
        SERIAL_EOL();
      }
    }
    if (verbose_level != 0) {                                    // !dry run
      if ((zero_std_dev >= test_precision || zero_std_dev <= calibration_precision) && iterations > force_iterations) {  // end iterations
        SERIAL_PROTOCOLPGM("Calibration OK");
        SERIAL_PROTOCOL_SP(36);
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
        print_G33_settings(!_1p_calibration, _7p_calibration && towers_set);
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
        SERIAL_PROTOCOL_SP(36);
        SERIAL_PROTOCOLPGM("std dev:");
        SERIAL_PROTOCOL_F(zero_std_dev, 3);
        SERIAL_EOL();
        lcd_setstatus(mess);
        print_G33_settings(!_1p_calibration, _7p_calibration && towers_set);
      }
    }
    else {                                                       // dry run
      const char *enddryrun = PSTR("End DRY-RUN");
      serialprintPGM(enddryrun);
      SERIAL_PROTOCOL_SP(39);
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
    home_delta();
    endstops.not_homing();

  }
  while ((zero_std_dev < test_precision && zero_std_dev > calibration_precision && iterations < 31) || iterations <= force_iterations);

  G33_CLEANUP();
}
