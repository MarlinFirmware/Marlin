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

  // if enabled adjusts the error correction threshold proportional to the current speed of the axis
  // allows for very small error margin at low speeds without stuttering due to reading latency at high speeds
  #define I2CPE_ERROR_THRESHOLD_PROPORTIONAL_SPEED
  #define I2CPE_STEPRATE 1

  //enable encoder-related debug serial echos
  #define I2CPE_DEBUG_ECHOS

  //time we wait for an encoder module to reboot after changing address.
  #define I2CPE_REBOOT_TIME 5000

  //I2C defines / enums etc
  #define I2CPE_MAG_SIG_GOOD 0
  #define I2CPE_MAG_SIG_MID 1
  #define I2CPE_MAG_SIG_BAD 2

  #define I2CPE_REQ_REPORT        0
  #define I2CPE_RESET_COUNT       1
  #define I2CPE_SET_ADDR          2
  #define I2CPE_SET_REPORT_MODE   3
  #define I2CPE_CLEAR_EEPROM      4

  #define I2CPE_LED_PAR_MODE  10
  #define I2CPE_LED_PAR_BRT   11
  #define I2CPE_LED_PAR_RATE  14

  #define I2CPE_REPORT_MODE_DISTANCE 0
  #define I2CPE_REPORT_MODE_STRENGTH 1
  #define I2CPE_REPORT_MODE_VERSION  2

  //default I2C addresses
  #define I2CPE_PRESET_ADDR_X 30
  #define I2CPE_PRESET_ADDR_Y 31
  #define I2CPE_PRESET_ADDR_Z 32
  #define I2CPE_PRESET_ADDR_E 33

  #define I2CPE_DEF_AXIS X_AXIS
  #define I2CPE_DEF_ADDR I2CPE_PRESET_ADDR_X

  //Error event counter. Tracks how many times there is an error surpassing a certain threshold
  #define I2CPE_ERR_CNT_TRIGGER_THRESHOLD     3.00
  #define I2CPE_ERR_CNT_DEBOUNCE_MS           2000

  #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
    #define I2CPE_ERR_ARRAY_SIZE 32
  #endif

  //Error Correction Methods
  #define I2CPE_ECM_NONE            0
  #define I2CPE_ECM_MICROSTEP       1
  #define I2CPE_ECM_PLANNER         2
  #define I2CPE_ECM_STALLDETECT     3

  #define I2CPE_ENC_TYPE_ROTARY  0
  #define I2CPE_ENC_TYPE_LINEAR  1

  extern const char axis_codes[XYZE];

  typedef union{
    volatile long val = 0;
    uint8_t bval[4];
  } i2cLong;

  class I2CEncoder {
  private:
    AxisEnum        encoderAxis             = I2CPE_DEF_AXIS;

    float           errorCorrectThreshold   = DEFAULT_AXIS_ERROR_THRESHOLD;

    uint8_t         i2cAddress              = I2CPE_DEF_ADDR,
                    errorCorrectMethod      = DEFAULT_EC_METHOD,
                    encoderType             = DEFAULT_ENCODER_TYPE,
                    magneticStrength        = I2CPE_MAG_SIG_BAD;

    int             encoderTicksPerUnit     = DEFAULT_ENCODER_TICKS_PER_MM,
                    stepperTicks            = DEFAULT_STEPPER_TICKS_REVOLUTION;


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

    //double positionMm; //calculate

    #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
      uint8_t       errArrayIndex = 0;
      int           errorArray[I2CPE_ERR_ARRAY_SIZE] = {0};
    #endif

  public:
    void update(void);
    void set_homed(void);
    void init(AxisEnum axis, byte address, bool invert);
    double get_axis_error_mm(bool report);
    long get_axis_error_steps(bool report);
    double get_position_mm(void);
    double mm_from_count(long count);
    long get_position(void);
    long get_raw_count(void);
    void set_zeroed(void);
    bool passes_test(bool report,bool &moduleDetected);
    bool passes_test(bool report);
    uint8_t get_magnetic_strength(void);
    bool test_axis(void);
    void calibrate_steps_mm(int iter);

    void set_current_position(float newPositionMm);

    int get_error_count(void);
    void set_error_count(int newCount);

    FORCE_INLINE uint8_t get_address() { return i2cAddress; }

    FORCE_INLINE void set_active(bool a) { active = a; }
    FORCE_INLINE bool get_active(void) { return active; }

    FORCE_INLINE void set_inverted(bool i) { invertDirection = i; }

    FORCE_INLINE AxisEnum get_axis() { return encoderAxis; }

    bool get_error_correct_enabled();
    void set_error_correct_enabled(bool enabled);

    uint8_t get_error_correct_method();
    void set_error_correct_method(byte method);

    float get_error_correct_threshold();
    void set_error_correct_threshold(float newThreshold);

    int get_encoder_ticks_mm();

    int get_encoder_ticks_unit();
    void set_encoder_ticks_unit(int ticks);

    uint8_t get_encoder_type();
    void set_encoder_type(byte type);

    int get_stepper_ticks();
    void set_stepper_ticks(int ticks);

    float get_axis_offset();
    void set_axis_offset(float newOffset);
  };


  class EncoderManager {
  public:
    EncoderManager() { Wire.begin(); }  // We use no address so we will join the BUS as the master
    void init(void);

    void update(void) { LOOP_NA(i) encoderArray[i].update(); }  //consider only updating one endoder per call / tick if encoders become too time intensive

    void homed(AxisEnum axis) {
      LOOP_NA(i)
        if(encoderArray[i].get_axis() == axis) encoderArray[i].set_homed();
    }

    void report_position(AxisEnum axis, bool units, bool noOffset);
    void report_status(AxisEnum axis);
    void report_error(AxisEnum axis) {
      LOOP_NA(i) {
        if(encoderArray[i].get_axis() == axis && encoderArray[i].get_active()) {
          encoderArray[i].get_axis_error_steps(true);
          return;
        }
      }
    }

    void test_axis(AxisEnum axis) {
      LOOP_NA(i)
        if(encoderArray[i].get_axis() == axis) {
          encoderArray[i].test_axis();
          return;
        }
    }

    void test_axis(void) { LOOP_NA(i) test_axis((AxisEnum)i); }

    void calibrate_steps_mm(AxisEnum axis, int iterations) {
      bool responded = false;

      LOOP_NA(i) {
        if(encoderArray[i].get_axis() == axis) {
          encoderArray[i].calibrate_steps_mm(iterations);
          responded = true;
          break;
        }

        if (!responded) {
          SERIAL_ECHOLNPGM("No encoder configured for given axis!");
          responded = true;
        }
      }
    }

    void calibrate_steps_mm(int iterations) {
      LOOP_NA(i)
        if(encoderArray[i].get_active())
          encoderArray[i].calibrate_steps_mm(iterations);
    }
    void change_module_address(int oldaddr, int newaddr);
    void check_module_firmware(int address);
    void report_error_count(AxisEnum axis);
    void report_error_count(void);
    void reset_error_count(AxisEnum axis);
    void reset_error_count(void);
    void enable_error_correction(AxisEnum axis, bool enabled);
    void set_error_correct_threshold(AxisEnum axis, float newThreshold);
    void get_error_correct_threshold(AxisEnum axis);

    int get_encoder_index_from_axis(AxisEnum axis);

    I2CEncoder encoderArray[NUM_AXIS];
  };



#endif //I2C_POSITION_ENCODERS

#endif //I2CPOSENC_H


