#ifndef __HAX_H
#define __HAX_H

#define manage_inactivity manage_inactivity2

void manage_inactivity2();

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../core/enum.h"

//From "../HAL/HAL_LPC1768/include/Arduino.h"
// Program Memory
#define pgm_read_ptr(addr)        (*((void**)(addr)))
#define pgm_read_byte_near(addr)  (*((uint8_t*)(addr)))
#define pgm_read_float_near(addr) (*((float*)(addr)))
#define pgm_read_word_near(addr)  (*((uint16_t*)(addr)))
#define pgm_read_dword_near(addr) (*((uint32_t*)(addr)))
#define pgm_read_byte(addr)       pgm_read_byte_near(addr)
#define pgm_read_float(addr)      pgm_read_float_near(addr)
#define pgm_read_word(addr)       pgm_read_word_near(addr)
#define pgm_read_dword(addr)      pgm_read_dword_near(addr)
#define sq(v) ((v) * (v))
#define square(x) ((x)*(x))
#define constrain(value, arg_min, arg_max) ((value) < (arg_min) ? (arg_min) :((value) > (arg_max) ? (arg_max) : (value)))



//From "../Marlin.cpp"
#define idle()

#define BLOCK_BUFFER_SIZE 16
//#define FORCE_INLINE inline __attribute__((always_inline)) 
#define NUM_AXIS 4

#define min fmin
#define max fmax

#define CRITICAL_SECTION_START
#define CRITICAL_SECTION_END

/*enum AxisEnum : unsigned char {
  X_AXIS    = 0,
  A_AXIS    = 0,
  Y_AXIS    = 1,
  B_AXIS    = 1,
  Z_AXIS    = 2,
  C_AXIS    = 2,
  E_AXIS    = 3,
  X_HEAD    = 4,
  Y_HEAD    = 5,
  Z_HEAD    = 6,
  ALL_AXES  = 0xFE,
  NO_AXIS   = 0xFF
  };*/

#define enable_X() while(0) {}
#define enable_Y() while(0) {}
#define enable_Z() while(0) {}
#define enable_e()
#define enable_E0() while(0) {}
#define enable_e1()
#define enable_e2()

#define disable_x()
#define disable_y()
#define disable_z()
#define disable_e()
#define disable_e0()
#define disable_e1()
#define disable_e2()
#define disable_all_steppers()

#define MINIMUM_PLANNER_SPEED 0.05
// (mm/sec)

extern int fanSpeed;
extern int extrudemultiply;

#define F_CPU 16000000

#define manage_heater() 
#define lcd_update() 

#define byte uint8_t

// Fixes for missing stepper.h and stepper.cpp

#define STEPPER_ISR_ENABLED() false  // TODO: Check if this affects anything.
#define DISABLE_STEPPER_DRIVER_INTERRUPT()
#define ENABLE_STEPPER_DRIVER_INTERRUPT()
#define STEPPER_TIMER_RATE 1 // TODO: Check if this affects anything.
struct Stepper {
  static bool is_block_busy(const void* const block) {
    return false;
  }
  static void quick_stop() {}
  static void endstop_triggered(const AxisEnum axis) {}
  static int32_t triggered_position(const AxisEnum axis) {
    return 0; // TODO: Check if this affects anything.
  }
  static int32_t position(const AxisEnum axis) {
    return 0; // TODO: Check if this affects anything.
  }
  void wake_up() {}
  inline static void set_position(const AxisEnum a, const int32_t &v) {}
  inline static void set_position(const int32_t &a, const int32_t &b, const int32_t &c, const int32_t &e) {
  }
};

#define DRYRUN 1 // TODO: Check if this affects anything.
#define DEBUGGING(F) true  // TODO: Check if this affects anything.
#endif
