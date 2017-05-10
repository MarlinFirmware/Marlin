/*
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

#if defined(I2C_ENCODERS_ENABLED)

#ifndef I2CENC_H
#define I2CENC_H

#include "macros.h"

//=========== Advanced / Less-Common Encoder Configuration Settings ==========//
//    (perhaps could go along with main configuration in configuration.h)     //

//if enabled adjusts the error correction threshold proportional to the current speed of the axis
//allows for very small error margin at low speeds without stuttering due to reading latency at high speeds
#define ERROR_THRESHOLD_PROPORTIONAL_SPEED
#define STEPRATE 1

//enable encoder-related debug serial echos
#define ENCODER_DEBUG_ECHOS

//time we wait for an encoder module to reboot after changing address.
#define REBOOT_TIME 5000

//I2C defines / enums etc
#define I2C_MAG_SIG_GOOD 0
#define I2C_MAG_SIG_MID 1
#define I2C_MAG_SIG_BAD 2

#define I2C_REQ_REPORT        0
#define I2C_RESET_COUNT       1
#define I2C_SET_ADDR          2
#define I2C_SET_REPORT_MODE   3
#define I2C_CLEAR_EEPROM      4

#define I2C_ENC_LED_PAR_MODE  10
#define I2C_ENC_LED_PAR_BRT   11
#define I2C_ENC_LED_PAR_RATE  14

#define I2C_ENC_REPORT_MODE_DISTANCE 0
#define I2C_ENC_REPORT_MODE_STRENGTH 1
#define I2C_ENC_REPORT_MODE_VERSION  2

//default I2C addresses
#define I2C_ENCODER_PRESET_ADDR_X 30
#define I2C_ENCODER_PRESET_ADDR_Y 31
#define I2C_ENCODER_PRESET_ADDR_Z 32
#define I2C_ENCODER_PRESET_ADDR_E 33

#define I2C_ENCODER_DEF_AXIS X_AXIS
#define I2C_ENCODER_DEF_ADDR I2C_ENCODER_PRESET_ADDR_X

//Error event counter. Tracks how many times there is an error surpassing a certain threshold
#define ERROR_COUNTER_TRIGGER_THRESHOLD     3.00
#define ERROR_COUNTER_DEBOUNCE_MS           2000

//Use a rolling average to identify persistant errors that indicate skips vs vibration and noise
#define ERROR_ROLLING_AVERAGE
#if ENABLED(ERROR_ROLLING_AVERAGE)
  #define ERROR_ARRAY_SIZE 32
#endif

//Error Correction Methods
#define ECM_NONE            0
#define ECM_MICROSTEP       1
#define ECM_PLANNER         2
#define ECM_STALLDETECT     3


#define ENC_TYPE_ROTARY  0
#define ENC_TYPE_LINEAR  1


typedef union{
  volatile long val = 0;
  byte bval[4];
}i2cLong;

void gcode_M860();
void gcode_M861();
void gcode_M862();
void gcode_M863();
//void gcode_M864();
//void gcode_M865();
void gcode_M866();
void gcode_M867();
void gcode_M868();
//void gcode_M869();

class I2cEncoder {
    private:

        byte i2cAddress             = I2C_ENCODER_DEF_ADDR;
        AxisEnum encoderAxis        = I2C_ENCODER_DEF_AXIS;
        float errorCorrectThreshold = DEFAULT_AXIS_ERROR_THRESHOLD;
        byte errorCorrectMethod     = DEFAULT_ERROR_CORRECT_METHOD;
        byte encoderType            = DEFAULT_ENCODER_TYPE;
        int encoderTicksPerUnit     = DEFAULT_ENCODER_TICKS_PER_MM;
        int stepperTicks            = DEFAULT_STEPPER_TICKS_REVOLUTION;

//        float axisOffset = 0;
        long axisOffsetTicks = 0;
        long zeroOffset = 0;
        bool homed = false;
        bool trusted = false;
        bool initialised = false;
        bool active = false;
        long position;
//        double positionMm; //calculate
        unsigned long lastErrorTime;
        bool invertDirection = false; 
        long lastPosition = 0;
        unsigned long lastPositionTime = 0;
        bool errorCorrect = true;   
        int errorCount = 0;
        unsigned long lastErrorCountTime = 0;
        int errorPrev = 0;
        byte magneticStrength = I2C_MAG_SIG_BAD;
        #if ENABLED(ERROR_ROLLING_AVERAGE)
          int errorArray[ERROR_ARRAY_SIZE] = {0};
          uint8_t errArrayIndex = 0;
        #endif
        
    public:
        void init(AxisEnum axis, byte address);
        void update();
        void set_homed();
//        double get_axis_error_mm(bool report);
        long get_axis_error_steps(bool report);
        double get_position_mm();
        double mm_from_count(long count);
        long get_position();
        long get_raw_count();
//        void set_led_param(byte, byte, byte);
        void set_zeroed();
        bool passes_test(bool report,bool &moduleDetected);
        bool passes_test(bool report);
        byte get_magnetic_strength();
        bool test_axis();
        void calibrate_steps_mm(int iterations);

        void set_current_position(float newPositionMm) ;

        int get_error_count();
        void set_error_count(int newCount);
        
        void set_address(byte address);
        byte get_address();

        void set_active(bool);
        bool get_active();

        void set_inverted(bool);
        bool get_inverted();

        AxisEnum get_axis();
        void set_axis(AxisEnum axis);

        bool get_error_correct_enabled();
        void set_error_correct_enabled(bool enabled);

        byte get_error_correct_method();
        void set_error_correct_method(byte method);

        float get_error_correct_threshold();
        void set_error_correct_threshold(float newThreshold);

        int get_encoder_ticks_mm();

        int get_encoder_ticks_unit();
        void set_encoder_ticks_unit(int ticks);

        byte get_encoder_type();
        void set_encoder_type(byte type);

        int get_stepper_ticks();
        void set_stepper_ticks(int ticks);

        float get_axis_offset();
        void set_axis_offset(float newOffset);
};

class EncoderManager {
    private:
        

    public:
        EncoderManager();
        void init();
        void update();
        void homed(AxisEnum axis);
        void report_position(AxisEnum axis, bool units, bool noOffset);
        void report_status(AxisEnum axis);
//        void report_error(AxisEnum axis);
        void test_axis(AxisEnum axis);
        void test_axis();
        void calibrate_steps_mm(AxisEnum axis, int iterations);
        void calibrate_steps_mm(int iterations);
//        void change_module_address(int oldAddress, int newAddress);
//        void check_module_firmware(int address);
        void report_error_count(AxisEnum axis);
        void report_error_count();
        void reset_error_count(AxisEnum axis);
        void reset_error_count();
        void enable_error_correction(AxisEnum axis, bool enabled);
        void set_error_correct_threshold(AxisEnum axis, float newThreshold);
        void get_error_correct_threshold(AxisEnum axis);

        int get_encoder_index_from_axis(AxisEnum axis);

        I2cEncoder encoderArray[NUM_AXIS];

};

static inline int8_t sgn(int val) {
 if (val < 0) return -1;
 if (val==0) return 0;
 return 1;
}

static inline int8_t sgn(long val) {
 if (val < 0) return -1;
 if (val==0) return 0;
 return 1;
}

static inline int8_t sgn(double val) {
    if (val < 0) return -1;
    return 1;
}

#endif //I2CENC_H
#endif //I2C_ENCODERS_ENABLED


