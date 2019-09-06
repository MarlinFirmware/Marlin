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

/**
 * joystick.cpp - joystick input / jogging
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(JOYSTICK)

#include "joystick.h"

#include "../inc/MarlinConfig.h"  // for pins
#include "../module/planner.h"
#include "../module/temperature.h"

Joystick joystick;

#if HAS_JOY_ADC_X
  temp_info_t Joystick::x; // = { 0 }
#endif
#if HAS_JOY_ADC_Y
  temp_info_t Joystick::y; // = { 0 }
#endif
#if HAS_JOY_ADC_Z
  temp_info_t Joystick::z; // = { 0 }
#endif

#if ENABLED(JOYSTICK_DEBUG)
  void Joystick::report() {
    SERIAL_ECHOPGM("Joystick");
    #if HAS_JOY_ADC_X
      SERIAL_ECHOPAIR(" X", x.raw);
    #endif
    #if HAS_JOY_ADC_Y
      SERIAL_ECHOPAIR(" Y", y.raw);
    #endif
    #if HAS_JOY_ADC_Z
      SERIAL_ECHOPAIR(" Z", z.raw);
    #endif
    #if HAS_JOY_ADC_EN
      SERIAL_ECHO_TERNARY(READ(JOY_EN_PIN), " EN=", "HIGH (dis", "LOW (en", "abled)");
    #endif
    SERIAL_EOL();
  }
#endif

void Joystick::calculate(float norm_jog[XYZ]) {
  // Do nothing if enable pin (active-low) is not LOW
  #if HAS_JOY_ADC_EN
    if (READ(JOY_EN_PIN)) return;
  #endif

  auto _normalize_joy = [](float &adc, const int16_t raw, const int16_t (&joy_limits)[4]) {
    if (WITHIN(raw, joy_limits[0], joy_limits[3])) {
      // within limits, check deadzone
      if (raw > joy_limits[2])
        adc = (raw - joy_limits[2]) / float(joy_limits[3] - joy_limits[2]);
      else if (raw < joy_limits[1])
        adc = (raw - joy_limits[1]) / float(joy_limits[1] - joy_limits[0]);  // negative value
    }
  };

  #if HAS_JOY_ADC_X
    static constexpr int16_t joy_x_limits[4] = JOY_X_LIMITS;
    _normalize_joy(norm_jog[X_AXIS], x.raw, joy_x_limits);
  #endif
  #if HAS_JOY_ADC_Y
    static constexpr int16_t joy_y_limits[4] = JOY_Y_LIMITS;
    _normalize_joy(norm_jog[Y_AXIS], y.raw, joy_y_limits);
  #endif
  #if HAS_JOY_ADC_Z
    static constexpr int16_t joy_z_limits[4] = JOY_Z_LIMITS;
    _normalize_joy(norm_jog[Z_AXIS], z.raw, joy_z_limits);
  #endif
}

#if ENABLED(POLL_JOG)

  void Joystick::inject_jog_moves() {
    // Recursion barrier
    static bool injecting_now; // = false;
    if (injecting_now) return;

    static constexpr int QUEUE_DEPTH = 5;                                 // Insert up to this many movements
    static constexpr float target_lag = 0.25f,                            // Aim for 1/4 second lag
                           seg_time = target_lag / QUEUE_DEPTH;           // 0.05 seconds, short segments inserted every 1/20th of a second
    static constexpr millis_t timer_limit_ms = millis_t(seg_time * 500);  // 25 ms minimum delay between insertions

    // The planner can merge/collapse small moves, so the movement queue is unreliable to control the lag
    static millis_t next_run = 0;
    if (PENDING(millis(), next_run)) return;
    next_run = millis() + timer_limit_ms;

    // Only inject a command if the planner has fewer than 5 moves and there are no unparsed commands
    if (planner.movesplanned() >= QUEUE_DEPTH || queue.has_commands_queued())
      return;

    // Normalized jog values are 0 for no movement and -1 or +1 for as max feedrate (nonlinear relationship)
    // Jog are initialized to zero and handling input can update values but doesn't have to
    // You could use a two-axis joystick and a one-axis keypad and they might work together
    float norm_jog[XYZ] = { 0 };

    // Use ADC values and defined limits. The active zone is normalized: -1..0 (dead) 0..1
    joystick.calculate(norm_jog);

    // Other non-joystick poll-based jogging could be implemented here
    // with "jogging" encapsulated as a more general class.

    // Jogging value maps continuously (quadratic relationship) to feedrate
    float move_dist[XYZ] = { 0 }, hypot2 = 0;
    LOOP_XYZ(i) if (norm_jog[i]) {
      move_dist[i] = seg_time * sq(norm_jog[i]) * planner.settings.max_feedrate_mm_s[i];
      // Very small movements disappear when printed as decimal with 4 digits of precision
      NOLESS(move_dist[i], 0.0002f);
      if (norm_jog[i] < 0) move_dist[i] *= -1;  // preserve sign
      hypot2 += sq(move_dist[i]);
    }

    if (!UNEAR_ZERO(hypot2)) {
      LOOP_XYZ(i) current_position[i] += move_dist[i];
      const float length = sqrt(hypot2);
      injecting_now = true;
      planner.buffer_line(current_position, length / seg_time, active_extruder, length);
      injecting_now = false;
    }
  }

#endif // POLL_JOG

#endif // JOYSTICK
