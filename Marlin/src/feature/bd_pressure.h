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
#pragma once

/**
 * feature/bd_pressure.h - bd_pressure strain gauge over I2C
 *
 * A BF350-1EB strain gauge on an STM32C0 with an ADS1220 ADC, which does two
 * unrelated jobs: nozzle-contact detection (a plain endstop output, needing no
 * firmware) and Pressure Advance calibration (all host-side).
 *
 * Not to be confused with BD_SENSOR, which is a different device by the same
 * author with a different protocol.
 */

#include "../inc/MarlinConfig.h"

// Register file. A flat byte array with an auto-incrementing pointer;
// there is no command set. Offsets are into the device's `Receive_D` struct.
enum BDPRegister : uint8_t {
  BDP_REG_VERSION   =  0,   // 16 bytes, R  - "pandapi3dv1\n", NUL-padded
  BDP_REG_MEASURE   = 16,   // 32 bytes, R  - latest result string, "R:res,lk,rk,Hk,Ha\n"
  BDP_REG_MODE      = 48,   //  1 byte,  W  - BDP_MODE_PROBE or BDP_MODE_PA
  BDP_REG_RAW_OUT   = 49,   //  1 byte,  W  - 1 = stream raw samples on the module's UART
  BDP_REG_THRESHOLD = 50,   //  1 byte,  W  - probe trigger threshold
  BDP_REG_RANGE     = 51,   //  1 byte,  R  - peak-to-peak of the last window
  BDP_REG_TARE      = 52,   //  1 byte,  W  - BDP_TARE_*
  BDP_REG_INVERT    = 53    //  1 byte,  W  - sign of the raw reading
};

// Mode values are ADC oversampling settings, not arbitrary ids.
enum BDPMode : uint8_t {
  BDP_MODE_PROBE = 2,       // CLOCK_OSR_512   - runs process_triggered()
  BDP_MODE_PA    = 7        // CLOCK_OSR_16384 - runs Pressure_advance()
};

enum BDPTare : uint8_t {
  BDP_TARE_AUTO     = 0,
  BDP_TARE_NOW      = 1,
  BDP_TARE_AUTO_OFF = 2
};

#define BDP_I2C_ADDRESS   0x04
#define BDP_VERSION_ID    "pandapi3dv1"
#define BDP_MEASURE_LEN     32

class BDPressure {
public:
  /**
   * @brief Bring up the I2C bus and check for the module.
   * @details Reports a failure on the console rather than failing silently,
   *          since two mistyped pins are otherwise indistinguishable from a
   *          working bus with nothing on it.
   * @return TRUE if the module answered with the expected version string.
   */
  static bool init();

  static bool is_online() { return online; }

  /**
   * @brief Report the module version, threshold and range. (M900 D)
   */
  static void report();

  #if ENABLED(BD_PRESSURE_PROBE)
    /**
     * @brief Put the module in probe mode and re-tare it before a probing move.
     * @details Mirrors Klipper's handle_homing_move_begin. The mode write is
     *          not redundant: a PA calibration leaves the module in PA mode,
     *          where the endstop output stops being driven at all.
     */
    static void probe_prep();
  #endif

  #if ENABLED(BD_PRESSURE_PA)
    /**
     * @brief Run an automatic Pressure Advance calibration. (M900 C)
     * @details Sweeps K from 0 in `step` increments, extruding one 80mm line
     *          per pass, and reads the module's curve metrics after each.
     *          Sets ADVANCE_K to the best value on success. Does not save.
     * @param step   PA increment per pass
     * @param passes Maximum number of passes
     */
    static void calibrate(const float step, const uint8_t passes);
  #endif

private:
  static bool online;
  static uint8_t mode;    // Last BDPMode written, to avoid needless ADC re-inits

  // Transport
  static bool write_reg(const uint8_t reg, const uint8_t * const data, const uint8_t len);
  static bool write_reg(const uint8_t reg, const uint8_t value) { return write_reg(reg, &value, 1); }
  static bool read_reg(const uint8_t reg, uint8_t * const data, const uint8_t len);
  static bool read_str(const uint8_t reg, char * const str, const uint8_t len);

  #if ENABLED(BD_PRESSURE_PA)
    // One pass of the sweep. Fields are the module's own curve metrics.
    struct pa_sample_t {
      float pa;         // The K value this pass was run at
      int16_t res;      // Combined residual - the figure of merit, lower is better
      int16_t lk, rk;   // Width of the leading / trailing pressure ramp, in samples
      int16_t hk;       // Peak height above baseline (x10)
      int16_t ha;       // Difference between the two shoulder heights
    };

    static bool read_sample(pa_sample_t &s);
  #endif
};

extern BDPressure bdp;
