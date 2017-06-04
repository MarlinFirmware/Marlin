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

  #define I2CPE_DEBUG_ECHOS                               // enable encoder-related debug serial echos

  #define I2CPE_REBOOT_TIME             5000              // time we wait for an encoder module to reboot
                                                          // after changing address.

  #define I2CPE_MAG_SIG_GOOD            0
  #define I2CPE_MAG_SIG_MID             1
  #define I2CPE_MAG_SIG_BAD             2

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

  // Error event counter. Tracks how many times there is an error surpassing a certain threshold
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

  #define LOOP_PE(VAR) LOOP_L_N(VAR, I2CPE_ENCODER_CNT)

  extern const char axis_codes[XYZE];

  typedef union {
    volatile long   val = 0;
    uint8_t         bval[4];
  } i2cLong;

  class I2CEncoder {
  private:
    AxisEnum        encoderAxis             = I2CPE_DEF_AXIS;

    float           errorCorrectThreshold   = I2CPE_DEF_EC_THRESH;

    uint8_t         i2cAddress              = I2CPE_DEF_ADDR,
                    errorCorrectMethod      = I2CPE_DEF_EC_METHOD,
                    encoderType             = I2CPE_DEF_TYPE,
                    magneticStrength        = I2CPE_MAG_SIG_BAD;

    int             encoderTicksPerUnit     = I2CPE_DEF_ENC_TICKS_UNIT,
                    stepperTicks            = I2CPE_DEF_TICKS_REV;


    bool            homed = false,
                    trusted = false,
                    initialised = false,
                    active = false,
                    invertDirection = false,
                    errorCorrect = true;

    int             errorCount = 0,
                    errorPrev = 0;

    float           axisOffset = 0;

    long            axisOffsetTicks = 0,
                    zeroOffset = 0,
                    lastPosition = 0,
                    position;

    unsigned long   lastPositionTime = 0,
                    lastErrorCountTime = 0,
                    lastErrorTime;

    //double        positionMm; //calculate

    #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
      uint8_t       errArrayIndex = 0;
      int           errorArray[I2CPE_ERR_ARRAY_SIZE] = {0};
    #endif

  public:
    void init(AxisEnum axis, byte address, bool invert);
    void reset();

    void update();

    void set_homed();

    long get_raw_count();
    double mm_from_count(long count);
    FORCE_INLINE double get_position_mm() { return mm_from_count(get_position()); }
    FORCE_INLINE long get_position() { return get_raw_count() - zeroOffset - axisOffsetTicks; }

    long get_axis_error_steps(bool report);
    double get_axis_error_mm(bool report);

    void calibrate_steps_mm(int iter);

    bool passes_test(bool report,bool &moduleDetected);
    bool passes_test(bool report);
    bool test_axis(void);

    FORCE_INLINE uint8_t get_magnetic_strength(void) { return magneticStrength; }

    FORCE_INLINE int get_error_count(void) { return errorCount; }
    FORCE_INLINE void set_error_count(int newCount) { errorCount = newCount; }

    FORCE_INLINE uint8_t get_address() { return i2cAddress; }

    FORCE_INLINE void set_active(bool a) { active = a; }
    FORCE_INLINE bool get_active(void) { return active; }

    FORCE_INLINE void set_inverted(bool i) { invertDirection = i; }

    FORCE_INLINE AxisEnum get_axis() { return encoderAxis; }

    FORCE_INLINE bool get_error_correct_enabled() { return errorCorrect; }
    FORCE_INLINE void set_error_correct_enabled(bool enabled) { errorCorrect = enabled; }

    FORCE_INLINE uint8_t get_error_correct_method() { return errorCorrectMethod; }
    FORCE_INLINE void set_error_correct_method(byte method) { errorCorrectMethod = method; }

    FORCE_INLINE float get_error_correct_threshold() { return errorCorrectThreshold; }
    FORCE_INLINE void set_error_correct_threshold(float newThreshold) { errorCorrectThreshold = newThreshold; }

    FORCE_INLINE int get_encoder_ticks_mm() {
      switch(get_encoder_type()) {
        default:
        case I2CPE_ENC_TYPE_LINEAR: return get_encoder_ticks_unit();
        case I2CPE_ENC_TYPE_ROTARY: return (int)((get_encoder_ticks_unit() / get_stepper_ticks()) * planner.axis_steps_per_mm[encoderAxis]);
      }
    }

    FORCE_INLINE int get_encoder_ticks_unit() { return encoderTicksPerUnit; }
    FORCE_INLINE void set_encoder_ticks_unit(int ticks) { encoderTicksPerUnit = ticks; }

    FORCE_INLINE uint8_t get_encoder_type() { return encoderType; }
    FORCE_INLINE void set_encoder_type(byte type) { encoderType = type; }

    FORCE_INLINE int get_stepper_ticks() { return stepperTicks; }
    FORCE_INLINE void set_stepper_ticks(int ticks) { stepperTicks = ticks; }

    FORCE_INLINE float get_axis_offset() { return axisOffset; }
    FORCE_INLINE void set_axis_offset(float newOffset) {
      axisOffset = newOffset;
      axisOffsetTicks = (long)(axisOffset * get_encoder_ticks_mm());
    }

    FORCE_INLINE void set_current_position(float newPositionMm) {
      set_axis_offset(get_position_mm() - newPositionMm + get_axis_offset());
    }
  };


  class EncoderManager {
  public:
    EncoderManager() { Wire.begin(); }  // We use no address so we will join the BUS as the master
    void init(void);

    void update(void) { LOOP_PE(i) encoders[i].update(); }  //consider only updating one endoder per call / tick if encoders become too time intensive

    void homed(AxisEnum axis) {
      LOOP_PE(i)
        if(encoders[i].get_axis() == axis) encoders[i].set_homed();
    }

    void report_position(AxisEnum axis, bool units, bool noOffset);

    void report_status(AxisEnum axis) {
      LOOP_PE(i)
        if(encoders[i].get_axis() == axis) {
          encoders[i].passes_test(true);
          return;
        }
    }

    void report_error(AxisEnum axis) {
      LOOP_PE(i) {
        if(encoders[i].get_axis() == axis && encoders[i].get_active()) {
          encoders[i].get_axis_error_steps(true);
          return;
        }
      }
    }

    void test_axis(AxisEnum axis) {
      LOOP_PE(i)
        if(encoders[i].get_axis() == axis) {
          encoders[i].test_axis();
          return;
        }
    }

    void calibrate_steps_mm(AxisEnum axis, int iterations) {
      bool responded = false;

      LOOP_PE(i) {
        if(encoders[i].get_axis() == axis) {
          encoders[i].calibrate_steps_mm(iterations);
          responded = true;
          break;
        }

        if (!responded) {
          SERIAL_ECHOLNPGM("No encoder configured for given axis!");
          responded = true;
        }
      }
    }

    void change_module_address(int oldaddr, int newaddr);
    void check_module_firmware(int address);

    void report_error_count(AxisEnum axis) {
      LOOP_PE(i) {
        if(encoders[i].get_axis() == axis) {
          SERIAL_ECHOPGM("Error count on ");
          SERIAL_ECHO(axis_codes[axis]);
          SERIAL_ECHOPGM(" axis is ");
          SERIAL_ECHO(encoders[i].get_error_count());
          SERIAL_ECHOLNPGM(" events.");
          break;
        }
      }
    }

    void reset_error_count(AxisEnum axis) {
      LOOP_PE(i) {
        if(encoders[i].get_axis() == axis) {
          encoders[i].set_error_count(0);
          SERIAL_ECHOPGM("Error count on ");
          SERIAL_ECHO(axis_codes[axis]);
          SERIAL_ECHOLNPGM(" axis has been reset.");
          break;
        }
      }
    }

    void enable_ec(AxisEnum axis, bool enabled) {
      LOOP_PE(i)
        if(encoders[i].get_axis() == axis) {
          encoders[i].set_error_correct_enabled(enabled);
          SERIAL_ECHOPGM("Error correction on ");
          SERIAL_ECHO(axis_codes[axis]);
          SERIAL_ECHOPGM(" axis is ");
          if(encoders[i].get_error_correct_enabled()) {
            SERIAL_ECHOLNPGM("enabled.");
          } else {
            SERIAL_ECHOLNPGM("disabled.");
          }
          break;
        }
    }

    void set_ec_threshold(AxisEnum axis, float newThreshold) {
      LOOP_PE(i)
        if(encoders[i].get_axis() == axis)
          encoders[i].set_error_correct_threshold(newThreshold);
    }

    void get_ec_threshold(AxisEnum axis) {
      float threshold = -999;

      LOOP_PE(i)
        if(encoders[i].get_axis() == axis) {
          threshold = encoders[i].get_error_correct_threshold();
          break;
        }

      if(threshold != -999) {
        SERIAL_ECHOPGM("Error correct threshold on ");
        SERIAL_ECHO(axis_codes[axis]);
        SERIAL_ECHOPGM(" axis is ");
        SERIAL_ECHO(threshold);
        SERIAL_ECHOLNPGM("mm.");
      }
    }

    int get_encoder_index_from_axis(AxisEnum axis) {
      int index = -1;
      LOOP_PE(i)
        if(encoders[i].get_axis() == axis) {
          index = i;
          break;
        }

      return index;
    }

    int get_encoder_index_from_address(uint8_t addr) {
      int index = -1;
      LOOP_PE(i)
        if(encoders[i].get_address() == addr) {
          index = i;
          break;
        }

      return index;
    }

    I2CEncoder encoders[I2CPE_ENCODER_CNT];
  };



#endif //I2C_POSITION_ENCODERS

#endif //I2CPOSENC_H


