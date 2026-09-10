/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(BD_PRESSURE)

#include "bd_pressure.h"

#include "../HAL/shared/Delay.h"
#include "../MarlinCore.h"

#include "../module/planner.h"

#if ENABLED(BD_PRESSURE_PA)
  #include "../gcode/gcode.h"
  #include "../module/motion.h"
  #include "../module/temperature.h"
#endif

BDPressure bdp;
bool BDPressure::online;  // = false
uint8_t BDPressure::mode; // = 0 (neither, so the first probe_prep always writes)

//#define DEBUG_OUT_BD_PRESSURE
#define DEBUG_OUT ENABLED(DEBUG_OUT_BD_PRESSURE)
#include "../core/debug_out.h"

// ============================================================================
// Software I2C
// ============================================================================
//
// Neither of the software I2C libraries Marlin already carries fits this
// device. Panda_SoftMasterI2C (used by BD_SENSOR) has a configurable bit
// period but its only write helper hard-codes register 0 and stops at a NUL,
// so it cannot set registers 48-53. SlowSoftWire has the full Wire API but a
// hard-coded 4us bit period (~125kHz) and a setClock() that does nothing.
//
// Both also drive the clock unconditionally, with no clock stretching. This
// module enables stretching (I2C_NOSTRETCH_DISABLE) and needs it: it services
// the bus one byte per interrupt while running the ADC and the PA maths, so a
// master that ignores stretching is relying on being slower than the slave's
// worst-case interrupt latency. That is the only reason the reference driver
// runs at 10kHz.
//
// So: a bit-banger with a configurable period that honours stretching.

#define BDP_SDA   BD_PRESSURE_I2C_SDA_PIN
#define BDP_SCL   BD_PRESSURE_I2C_SCL_PIN
#define BDP_DELAY DELAY_US(BD_PRESSURE_I2C_DELAY)

// Bit periods to wait for the slave to release a stretched clock.
#define BDP_STRETCH_TIMEOUT 2000

#define BDP_I2C_READ  1
#define BDP_I2C_WRITE 0

// Open-drain emulation. A line is released by making it an input with the
// internal pull-up, and asserted by driving it low.
static void bdp_sda_high() { SET_INPUT_PULLUP(BDP_SDA); }
static void bdp_sda_low()  { OUT_WRITE(BDP_SDA, LOW); }
static void bdp_scl_low()  { OUT_WRITE(BDP_SCL, LOW); }

/**
 * @brief Release SCL and wait for the slave to let it rise.
 * @return FALSE if the slave held the clock down past the timeout.
 */
static bool bdp_scl_high() {
  SET_INPUT_PULLUP(BDP_SCL);
  BDP_DELAY;
  uint16_t timeout = BDP_STRETCH_TIMEOUT;
  while (!READ(BDP_SCL)) {         // Clock stretching
    if (!--timeout) return false;
    BDP_DELAY;
  }
  return true;
}

static void bdp_stop() {
  bdp_sda_low();
  BDP_DELAY;
  bdp_scl_high();
  BDP_DELAY;
  bdp_sda_high();
  BDP_DELAY;
}

/**
 * @brief Write one byte and read back the slave's ACK.
 * @return TRUE if the slave acknowledged.
 */
static bool bdp_write_byte(const uint8_t value) {
  for (uint8_t mask = 0x80; mask; mask >>= 1) {
    if (value & mask) bdp_sda_high(); else bdp_sda_low();
    if (!bdp_scl_high()) return false;
    BDP_DELAY;
    bdp_scl_low();
    BDP_DELAY;
  }
  bdp_sda_high();                  // Release SDA for the ACK bit
  if (!bdp_scl_high()) return false;
  BDP_DELAY;
  const bool ack = !READ(BDP_SDA);
  bdp_scl_low();
  BDP_DELAY;
  return ack;
}

static uint8_t bdp_read_byte(const bool nack) {
  uint8_t b = 0;
  bdp_sda_high();
  for (uint8_t i = 0; i < 8; ++i) {
    b <<= 1;
    if (!bdp_scl_high()) return 0;
    BDP_DELAY;
    if (READ(BDP_SDA)) b |= 1;
    bdp_scl_low();
    BDP_DELAY;
  }
  if (nack) bdp_sda_high(); else bdp_sda_low();
  bdp_scl_high();
  BDP_DELAY;
  bdp_scl_low();
  BDP_DELAY;
  bdp_sda_high();
  return b;
}

static bool bdp_start(const uint8_t addr_rw) {
  bdp_sda_high();
  if (!bdp_scl_high()) return false;
  BDP_DELAY;
  bdp_sda_low();                   // START: SDA falls while SCL is high
  BDP_DELAY;
  bdp_scl_low();
  BDP_DELAY;
  return bdp_write_byte(addr_rw);
}

// ============================================================================
// Register access
// ============================================================================
//
// Write: START | addr<<1|W | offset | data... | STOP
// Read:  START | addr<<1|W | offset | REP START | addr<<1|R | data... | STOP
//
// Reads need the repeated START - there is no separate "set pointer"
// transaction, since the device only resets its offset latch on ListenCplt.

bool BDPressure::write_reg(const uint8_t reg, const uint8_t * const data, const uint8_t len) {
  bool ok = bdp_start((BDP_I2C_ADDRESS << 1) | BDP_I2C_WRITE) && bdp_write_byte(reg);
  for (uint8_t i = 0; ok && i < len; ++i) ok = bdp_write_byte(data[i]);
  bdp_stop();
  if (!ok) DEBUG_ECHOLNPGM("BDP: write reg ", reg, " failed");
  return ok;
}

bool BDPressure::read_reg(const uint8_t reg, uint8_t * const data, const uint8_t len) {
  bool ok = bdp_start((BDP_I2C_ADDRESS << 1) | BDP_I2C_WRITE) && bdp_write_byte(reg);
  if (ok) ok = bdp_start((BDP_I2C_ADDRESS << 1) | BDP_I2C_READ);  // Repeated START
  if (ok) for (uint8_t i = 0; i < len; ++i) data[i] = bdp_read_byte(i == len - 1);
  bdp_stop();
  if (!ok) DEBUG_ECHOLNPGM("BDP: read reg ", reg, " failed");
  return ok;
}

/**
 * @brief Read a NUL-terminated string from the register file.
 * @param len Buffer size including the terminator.
 */
bool BDPressure::read_str(const uint8_t reg, char * const str, const uint8_t len) {
  str[0] = '\0';
  if (!read_reg(reg, (uint8_t*)str, len - 1)) return false;
  str[len - 1] = '\0';
  // The device NUL-pads, but a partly-written field may not be terminated.
  for (uint8_t i = 0; i < len; ++i) if (str[i] == '\n') { str[i] = '\0'; break; }
  return true;
}

// ============================================================================
// Setup and reporting
// ============================================================================

bool BDPressure::init() {
  online = false;

  bdp_sda_high();
  SET_INPUT_PULLUP(BDP_SCL);
  BDP_DELAY;

  // A line stuck low is a wiring fault, not a missing device. Say which.
  if (!READ(BDP_SDA) || !READ(BDP_SCL)) {
    SERIAL_ERROR_MSG("BD_PRESSURE: bus stuck low - check wiring/pull-ups");
    return false;
  }

  char ver[17];
  if (!read_str(BDP_REG_VERSION, ver, sizeof(ver)) || !strstr(ver, BDP_VERSION_ID)) {
    SERIAL_ERROR_START();
    SERIAL_ECHOLNPGM("BD_PRESSURE: no response on SDA:", BDP_SDA, " SCL:", BDP_SCL);
    return false;
  }

  online = true;
  SERIAL_ECHOLNPGM("BD_PRESSURE: ", ver);

  #if ENABLED(BD_PRESSURE_PROBE)
    write_reg(BDP_REG_MODE, BDP_MODE_PROBE);
    write_reg(BDP_REG_THRESHOLD, BD_PRESSURE_THRESHOLD);
    mode = BDP_MODE_PROBE;
  #endif

  return true;
}

void BDPressure::report() {
  if (!online && !init()) return;

  char ver[17];
  uint8_t thr = 0, range = 0;
  read_str(BDP_REG_VERSION, ver, sizeof(ver));
  read_reg(BDP_REG_THRESHOLD, &thr, 1);
  read_reg(BDP_REG_RANGE, &range, 1);

  SERIAL_ECHOLNPGM("BD_PRESSURE ", ver, " threshold:", thr, " range:", range);
}

#if ENABLED(BD_PRESSURE_PROBE)

  void BDPressure::probe_prep() {
    if (!online) return;

    // Only write the mode when it actually changes. Writing status_clk makes
    // the module re-initialize its ADS1220, which then needs time to settle -
    // pointless before every probe point. A PA calibration is what leaves it in
    // PA mode, where Pressure_advance() runs instead of process_triggered() and
    // the endstop output stops being driven at all.
    if (mode != BDP_MODE_PROBE) {
      write_reg(BDP_REG_MODE, BDP_MODE_PROBE);
      write_reg(BDP_REG_THRESHOLD, BD_PRESSURE_THRESHOLD);
      mode = BDP_MODE_PROBE;
      safe_delay(BD_PRESSURE_PROBE_SETTLE_MS);
    }

    // Tare with the machine at rest, as Klipper's PA_RESET does with its M400.
    // Taring mid-move captures the moving load as the baseline.
    planner.synchronize();
    write_reg(BDP_REG_TARE, BDP_TARE_NOW);

    // Let the strain gauge settle before probing. Users report this matters
    // for repeatability; the reference macro leans on repeated resets instead.
    safe_delay(BD_PRESSURE_PROBE_SETTLE_MS);
  }

#endif // BD_PRESSURE_PROBE

// ============================================================================
// Pressure Advance calibration
// ============================================================================

#if ENABLED(BD_PRESSURE_PA)

/**
 * @brief Read and parse one "R:res,lk,rk,Hk,Ha" result.
 * @details The device never clears this register between passes, so a read
 *          that lands before the analysis finishes returns the previous
 *          pass's numbers. The caller writes a NUL here first and polls, so
 *          an empty string means "not ready yet" rather than "no data".
 * @return TRUE if a complete result was parsed.
 */
bool BDPressure::read_sample(pa_sample_t &s) {
  char buf[BDP_MEASURE_LEN + 1];
  if (!read_str(BDP_REG_MEASURE, buf, sizeof(buf))) return false;

  char * const p = strstr(buf, "R:");
  if (!p) return false;

  int res, lk, rk, hk, ha;
  if (sscanf(p + 2, "%d,%d,%d,%d,%d", &res, &lk, &rk, &hk, &ha) != 5) return false;

  s.res = res; s.lk = lk; s.rk = rk; s.hk = hk; s.ha = ha;
  return true;
}

void BDPressure::calibrate(const float step, const uint8_t passes) {
  if (!online && !init()) return;

  if (!motion.all_axes_trusted()) {
    SERIAL_ERROR_MSG("BD_PRESSURE: home the printer first");
    return;
  }

  // Line geometry. Distances are fixed - only the start point moves to suit
  // the machine. 20mm accel / 40mm cruise / 20mm decel; the two speed changes
  // are the events being measured.
  constexpr float l_acc = 20.0f, l_cruise = 40.0f, l_dec = 20.0f,
                  x_start = X_MIN_POS + BD_PRESSURE_PA_MARGIN,
                  y_start = Y_MIN_POS + BD_PRESSURE_PA_MARGIN,
                  x_mid1  = x_start + l_acc,
                  x_mid2  = x_mid1 + l_cruise,
                  x_end   = x_mid2 + l_dec;

  const float y_end = y_start + (passes - 1) * (BD_PRESSURE_PA_Y_STEP);

  if (x_end > X_MAX_POS) {
    SERIAL_ERROR_MSG("BD_PRESSURE: 80mm line does not fit in X");
    return;
  }
  if (y_end > Y_MAX_POS) {
    SERIAL_ERROR_MSG("BD_PRESSURE: too many passes to fit in Y");
    return;
  }

  // Extrusion amounts are for a 0.4mm nozzle at 100% flow and scale with the
  // square of the nozzle diameter.
  constexpr float e_scale = sq((BD_PRESSURE_PA_NOZZLE) / 0.4f) * (BD_PRESSURE_PA_FLOW),
                  e_purge  = 9.92204f * e_scale,
                  e_acc    = 0.92643f * e_scale,
                  e_cruise = 1.85285f * e_scale,
                  e_dec    = 0.92643f * e_scale;

  // Feedrates in mm/s.
  constexpr feedRate_t fr_low    = 51.0f * (BD_PRESSURE_PA_VOLUMETRIC) / 60.0f,
                       fr_high   = 537.0f * (BD_PRESSURE_PA_VOLUMETRIC) / 60.0f,
                       fr_travel = (BD_PRESSURE_PA_TRAVEL_SPEED);

  // Take the machine off the user's overrides for the duration - a flow or
  // feedrate percentage would scale the very thing being measured.
  const int16_t old_feedrate_percentage = motion.feedrate_percentage;
  const int16_t old_flow = planner.flow_percentage[motion.extruder];
  const float old_k = planner.get_advance_k();
  motion.feedrate_percentage = 100;
  planner.set_flow(motion.extruder, 100);

  auto cleanup = [&]() {
    motion.feedrate_percentage = old_feedrate_percentage;
    planner.set_flow(motion.extruder, old_flow);
  };

  auto extrude_to = [](const float x, const float y, const float e, const feedRate_t fr) {
    motion.destination = motion.position;
    motion.destination.x = x;
    motion.destination.y = y;
    motion.destination.e += e;
    motion.feedrate_mm_s = fr;
    motion.prepare_line_to_destination();
  };

  #if HAS_HOTEND
    thermalManager.setTargetHotend(BD_PRESSURE_PA_TEMP, motion.extruder);
    thermalManager.wait_for_hotend(motion.extruder);
  #endif

  write_reg(BDP_REG_MODE, BDP_MODE_PA);
  write_reg(BDP_REG_RAW_OUT, 0);
  mode = BDP_MODE_PA;

  // Prime the nozzle in place, at a controlled speed.
  extrude_to(motion.position.x, motion.position.y, e_purge, fr_low);
  planner.synchronize();
  safe_delay(4000);

  pa_sample_t samples[BD_PRESSURE_PA_PASSES];
  uint8_t count = 0;

  for (uint8_t i = 0; i < passes; ++i) {
    const float pa = i * step;
    planner.set_advance_k(pa);

    const float y = y_start + i * (BD_PRESSURE_PA_Y_STEP);

    motion.blocking_move(xy_pos_t({ x_start, y }), fr_travel);

    // Clear the result register so a stale read is distinguishable from a
    // fresh one. Nothing in the module's firmware reads this field.
    const uint8_t nul = 0;
    write_reg(BDP_REG_MEASURE, &nul, 1);

    extrude_to(x_mid1, y, e_acc,    fr_low);
    extrude_to(x_mid2, y, e_cruise, fr_high);
    extrude_to(x_end,  y, e_dec,    fr_low);
    planner.synchronize();

    // Poll for the new result rather than assuming it is ready.
    pa_sample_t s;
    bool got = false;
    for (uint8_t tries = 0; tries < 20; ++tries) {
      if (read_sample(s)) { got = true; break; }
      safe_delay(50);
      marlin.idle();
    }

    if (!got) {
      SERIAL_ERROR_MSG("BD_PRESSURE: no result for pass ", i);
      cleanup();
      planner.set_advance_k(old_k);
      return;
    }

    s.pa = pa;
    samples[count++] = s;

    SERIAL_ECHOLNPGM("BD_PRESSURE pass ", i, " K", p_float_t(pa, 4),
                     " res:", s.res, " lk:", s.lk, " rk:", s.rk,
                     " Hk:", s.hk, " Ha:", s.ha);

    // Stop once the pressure peak has flattened out - more PA is not doing
    // anything. Needs a decent run of samples before it means much.
    if (count >= 20) {
      bool flat = true;
      for (uint8_t j = count - 5; j < count; ++j)
        if (!(samples[j].hk < 2 || samples[j].ha < 5)) { flat = false; break; }
      if (flat) break;
    }

    marlin.idle();
  }

  cleanup();

  // Choosing the value. Throw away the obviously under-compensated start,
  // find where the curve goes flat, then take the best point in the flat
  // region. This is what a human does eyeballing a PA tower.
  constexpr uint8_t skip = 5;
  if (count <= skip) {
    SERIAL_ERROR_MSG("BD_PRESSURE: not enough samples");
    planner.set_advance_k(old_k);
    return;
  }

  int8_t flat_from = -1;
  for (int8_t i = count - 1; i >= skip; --i)
    if (samples[i].hk < 5) { flat_from = i; break; }

  if (flat_from < 0)
    for (int8_t i = count - 1; i >= skip; --i)
      if (samples[i].ha < 5) { flat_from = i; break; }

  if (flat_from < 0) {
    SERIAL_ERROR_MSG("BD_PRESSURE: no flat region found - PA not calibrated");
    planner.set_advance_k(old_k);
    return;
  }

  uint8_t best = flat_from;
  int32_t best_score = INT32_MAX;
  for (uint8_t i = flat_from; i < count; ++i) {
    const int32_t score = samples[i].res + ABS(samples[i].ha);
    if (score < best_score) { best_score = score; best = i; }
  }

  planner.set_advance_k(samples[best].pa);

  SERIAL_ECHOLNPGM("BD_PRESSURE: K", p_float_t(samples[best].pa, 4),
                   " (sample ", best, " of ", count, ")");
  SERIAL_ECHOLNPGM("Use M900 K", p_float_t(samples[best].pa, 4), " and M500 to keep it.");
}

#endif // BD_PRESSURE_PA

#endif // BD_PRESSURE
