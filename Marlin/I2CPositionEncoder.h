/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef I2CPOSENC_H
#define I2CPOSENC_H

#include "MarlinConfig.h"

#if ENABLED(I2C_POSITION_ENCODERS)

  #include "enum.h"
  #include "macros.h"
  #include "types.h"
  #include <Wire.h>

  //=========== Advanced / Less-Common Encoder Configuration Settings ==========

  #define I2CPE_EC_THRESH_PROPORTIONAL                    // if enabled adjusts the error correction threshold
                                                          // proportional to the current speed of the axis allows
                                                          // for very small error margin at low speeds without
                                                          // stuttering due to reading latency at high speeds

  #define I2CPE_DEBUG                                     // enable encoder-related debug serial echos

  #define I2CPE_REBOOT_TIME             5000              // time we wait for an encoder module to reboot
                                                          // after changing address.

  #define I2CPE_MAG_SIG_GOOD            0
  #define I2CPE_MAG_SIG_MID             1
  #define I2CPE_MAG_SIG_BAD             2
  #define I2CPE_MAG_SIG_NF              255

  #define I2CPE_REQ_REPORT              0
  #define I2CPE_RESET_COUNT             1
  #define I2CPE_SET_ADDR                2
  #define I2CPE_SET_REPORT_MODE         3
  #define I2CPE_CLEAR_EEPROM            4

  #define I2CPE_LED_PAR_MODE            10
  #define I2CPE_LED_PAR_BRT             11
  #define I2CPE_LED_PAR_RATE            14

  #define I2CPE_REPORT_DISTANCE         0
  #define I2CPE_REPORT_STRENGTH         1
  #define I2CPE_REPORT_VERSION          2

  // Default I2C addresses
  #define I2CPE_PRESET_ADDR_X           30
  #define I2CPE_PRESET_ADDR_Y           31
  #define I2CPE_PRESET_ADDR_Z           32
  #define I2CPE_PRESET_ADDR_E           33

  #define I2CPE_DEF_AXIS                X_AXIS
  #define I2CPE_DEF_ADDR                I2CPE_PRESET_ADDR_X

  // Error event counter; tracks how many times there is an error exceeding a certain threshold
  #define I2CPE_ERR_CNT_THRESH          3.00
  #define I2CPE_ERR_CNT_DEBOUNCE_MS     2000

  #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
    #define I2CPE_ERR_ARRAY_SIZE        32
  #endif

  // Error Correction Methods
  #define I2CPE_ECM_NONE                0
  #define I2CPE_ECM_MICROSTEP           1
  #define I2CPE_ECM_PLANNER             2
  #define I2CPE_ECM_STALLDETECT         3

  // Encoder types
  #define I2CPE_ENC_TYPE_ROTARY         0
  #define I2CPE_ENC_TYPE_LINEAR         1

  // Parser
  #define I2CPE_PARSE_ERR               1
  #define I2CPE_PARSE_OK                0

  #define LOOP_PE(VAR) LOOP_L_N(VAR, I2CPE_ENCODER_CNT)
  #define CHECK_IDX if (!WITHIN(idx, 0, I2CPE_ENCODER_CNT - 1)) return;

  extern const char axis_codes[XYZE];

  typedef union {
    volatile long   val = 0;
    uint8_t         bval[4];
  } i2cLong;

  class I2CPositionEncoder {
  private:
    AxisEnum        encoderAxis             = I2CPE_DEF_AXIS;

    uint8_t         i2cAddress              = I2CPE_DEF_ADDR,
                    ecMethod                = I2CPE_DEF_EC_METHOD,
                    type                    = I2CPE_DEF_TYPE,
                    H                       = I2CPE_MAG_SIG_NF;    // Magnetic field strength

    int             encoderTicksPerUnit     = I2CPE_DEF_ENC_TICKS_UNIT,
                    stepperTicks            = I2CPE_DEF_TICKS_REV;

    float           ecThreshold             = I2CPE_DEF_EC_THRESH;

    bool            homed                   = false,
                    trusted                 = false,
                    initialised             = false,
                    active                  = false,
                    invert                  = false,
                    ec                      = true;

    int             errorCount              = 0,
                    errorPrev               = 0;

    float           axisOffset              = 0;

    long            axisOffsetTicks         = 0,
                    zeroOffset              = 0,
                    lastPosition            = 0,
                    position;

    unsigned long   lastPositionTime        = 0,
                    lastErrorCountTime      = 0,
                    lastErrorTime;

    //double        positionMm; //calculate

    #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
      uint8_t       errIdx = 0;
      int           err[I2CPE_ERR_ARRAY_SIZE] = {0};
    #endif

  public:
    void init(uint8_t address, AxisEnum axis);
    void reset();

    void update();

    void set_homed();

    long get_raw_count();

    FORCE_INLINE double mm_from_count(long count) {
      if (type == I2CPE_ENC_TYPE_LINEAR) return count / encoderTicksPerUnit;
      else if (type == I2CPE_ENC_TYPE_ROTARY)
        return (count * stepperTicks) / (encoderTicksPerUnit * planner.axis_steps_per_mm[encoderAxis]);
      return -1;
    }

    FORCE_INLINE double get_position_mm() { return mm_from_count(get_position()); }
    FORCE_INLINE long get_position() { return get_raw_count() - zeroOffset - axisOffsetTicks; }

    long get_axis_error_steps(bool report);
    double get_axis_error_mm(bool report);

    void calibrate_steps_mm(int iter);

    bool passes_test(bool report);

    bool test_axis(void);

    FORCE_INLINE int get_error_count(void) { return errorCount; }
    FORCE_INLINE void set_error_count(int newCount) { errorCount = newCount; }

    FORCE_INLINE uint8_t get_address() { return i2cAddress; }
    FORCE_INLINE void set_address(uint8_t addr) { i2cAddress = addr; }

    FORCE_INLINE bool get_active(void) { return active; }
    FORCE_INLINE void set_active(bool a) { active = a; }

    FORCE_INLINE void set_inverted(bool i) { invert = i; }

    FORCE_INLINE AxisEnum get_axis() { return encoderAxis; }

    FORCE_INLINE bool get_ec_enabled() { return ec; }
    FORCE_INLINE void set_ec_enabled(bool enabled) { ec = enabled; }

    FORCE_INLINE uint8_t get_ec_method() { return ecMethod; }
    FORCE_INLINE void set_ec_method(byte method) { ecMethod = method; }

    FORCE_INLINE float get_ec_threshold() { return ecThreshold; }
    FORCE_INLINE void set_ec_threshold(float newThreshold) { ecThreshold = newThreshold; }

    FORCE_INLINE int get_encoder_ticks_mm() {
      if (type == I2CPE_ENC_TYPE_LINEAR) return encoderTicksPerUnit;
      else if (type == I2CPE_ENC_TYPE_ROTARY)
        return (int)((encoderTicksPerUnit / stepperTicks) * planner.axis_steps_per_mm[encoderAxis]);
      return 0;
    }

    FORCE_INLINE int get_ticks_unit() { return encoderTicksPerUnit; }
    FORCE_INLINE void set_ticks_unit(int ticks) { encoderTicksPerUnit = ticks; }

    FORCE_INLINE uint8_t get_type() { return type; }
    FORCE_INLINE void set_type(byte newType) { type = newType; }

    FORCE_INLINE int get_stepper_ticks() { return stepperTicks; }
    FORCE_INLINE void set_stepper_ticks(int ticks) { stepperTicks = ticks; }

    FORCE_INLINE float get_axis_offset() { return axisOffset; }
    FORCE_INLINE void set_axis_offset(float newOffset) {
      axisOffset = newOffset;
      axisOffsetTicks = (long)(axisOffset * get_encoder_ticks_mm());
    }

    FORCE_INLINE void set_current_position(float newPositionMm) {
      set_axis_offset(get_position_mm() - newPositionMm + axisOffset);
    }
  };

  class I2CPositionEncodersMgr {
  private:
    bool I2CPE_anyaxis;
    uint8_t I2CPE_addr;
    int8_t I2CPE_idx;

  public:
    void init(void);

    // consider only updating one endoder per call / tick if encoders become too time intensive
    void update(void) { LOOP_PE(i) encoders[i].update(); }

    void homed(AxisEnum axis) {
      LOOP_PE(i)
        if (encoders[i].get_axis() == axis) encoders[i].set_homed();
    }

    void report_position(uint8_t idx, bool units, bool noOffset);

    void report_status(uint8_t idx) {
      CHECK_IDX
      SERIAL_ECHOPAIR("Encoder ",idx);
      SERIAL_ECHOPGM(": ");
      encoders[idx].get_raw_count();
      encoders[idx].passes_test(true);
    }

    void report_error(uint8_t idx) {
      CHECK_IDX
      encoders[idx].get_axis_error_steps(true);
    }

    void test_axis(uint8_t idx) {
      CHECK_IDX
      encoders[idx].test_axis();
    }

    void calibrate_steps_mm(uint8_t idx, int iterations) {
      CHECK_IDX
      encoders[idx].calibrate_steps_mm(iterations);
    }

    void change_module_address(uint8_t oldaddr, uint8_t newaddr);
    void report_module_firmware(uint8_t address);

    void report_error_count(uint8_t idx, AxisEnum axis) {
      CHECK_IDX
      SERIAL_ECHOPAIR("Error count on ", axis_codes[axis]);
      SERIAL_ECHOLNPAIR(" axis is ", encoders[idx].get_error_count());
    }

    void reset_error_count(uint8_t idx, AxisEnum axis) {
      CHECK_IDX
      encoders[idx].set_error_count(0);
      SERIAL_ECHOPAIR("Error count on ", axis_codes[axis]);
      SERIAL_ECHOLNPGM(" axis has been reset.");
    }

    void enable_ec(uint8_t idx, bool enabled, AxisEnum axis) {
      CHECK_IDX
      encoders[idx].set_ec_enabled(enabled);
      SERIAL_ECHOPAIR("Error correction on ", axis_codes[axis]);
      SERIAL_ECHOPGM(" axis is ");
      serialprintPGM(encoders[idx].get_ec_enabled() ? PSTR("en") : PSTR("dis"));
      SERIAL_ECHOLNPGM("abled.");
    }

    void set_ec_threshold(uint8_t idx, float newThreshold, AxisEnum axis) {
      CHECK_IDX
      encoders[idx].set_ec_threshold(newThreshold);
      SERIAL_ECHOPAIR("Error correct threshold for ", axis_codes[axis]);
      SERIAL_ECHOPAIR_F(" axis set to ", newThreshold);
      SERIAL_ECHOLNPGM("mm.");
    }

    void get_ec_threshold(uint8_t idx, AxisEnum axis) {
      CHECK_IDX
      float threshold = encoders[idx].get_ec_threshold();
      SERIAL_ECHOPAIR("Error correct threshold for ", axis_codes[axis]);
      SERIAL_ECHOPAIR_F(" axis is ", threshold);
      SERIAL_ECHOLNPGM("mm.");
    }

    int8_t idx_from_axis(AxisEnum axis) {
      LOOP_PE(i)
        if (encoders[i].get_axis() == axis) return i;

      return -1;
    }

    int8_t idx_from_addr(uint8_t addr) {
      LOOP_PE(i)
        if (encoders[i].get_address() == addr) return i;

      return -1;
    }

    int8_t parse();

    void M860();
    void M861();
    void M862();
    void M863();
    void M864();
    void M865();
    void M866();
    void M867();
    void M868();
    void M869();

    I2CPositionEncoder encoders[I2CPE_ENCODER_CNT];
  };

  extern I2CPositionEncodersMgr I2CPEM;

  FORCE_INLINE void gcode_M860() { I2CPEM.M860(); }
  FORCE_INLINE void gcode_M861() { I2CPEM.M861(); }
  FORCE_INLINE void gcode_M862() { I2CPEM.M862(); }
  FORCE_INLINE void gcode_M863() { I2CPEM.M863(); }
  FORCE_INLINE void gcode_M864() { I2CPEM.M864(); }
  FORCE_INLINE void gcode_M865() { I2CPEM.M865(); }
  FORCE_INLINE void gcode_M866() { I2CPEM.M866(); }
  FORCE_INLINE void gcode_M867() { I2CPEM.M867(); }
  FORCE_INLINE void gcode_M868() { I2CPEM.M868(); }
  FORCE_INLINE void gcode_M869() { I2CPEM.M869(); }

#endif //I2C_POSITION_ENCODERS
#endif //I2CPOSENC_H


