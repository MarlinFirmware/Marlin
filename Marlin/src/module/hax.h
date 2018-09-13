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
#include "../inc/MarlinConfig.h"

//From motion.h
#define MMS_SCALED(MM_S) ((MM_S)*feedrate_percentage*0.01f)
extern float current_position[XYZE];
#define SYNC_PLAN_POSITION_KINEMATIC()

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
#define STEPPER_TIMER_RATE     HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define HAL_TIMER_RATE         ((SystemCoreClock) / 4)  // frequency of timers peripherals
#define SystemCoreClock 110000000


//From "../Marlin.cpp"
extern bool idle2();
#define idle()

//#define FORCE_INLINE inline __attribute__((always_inline)) 
#define NUM_AXIS 4
#define MAX_EXTRUDERS EXTRUDERS

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

#define enable_X() do {} while(0)
#define enable_Y() do {} while(0)
#define enable_Z() do {} while(0)
#define enable_e() do {} while(0)
#define enable_E0() do {} while(0)
#define enable_E1() do {} while(0)
#define enable_E2() do {} while(0)
#define enable_E3() do {} while(0)
#define enable_E4() do {} while(0)
#define enable_e1() do {} while(0)
#define enable_e2() do {} while(0)

#define disable_x() do {} while(0)
#define disable_y() do {} while(0)
#define disable_z() do {} while(0)
#define disable_e() do {} while(0)
#define disable_e0() do {} while(0)
#define disable_e1() do {} while(0)
#define disable_e2() do {} while(0)
#define disable_all_steppers() do {} while(0)
#define disable_E0() do { } while (0)
#define disable_E1() do { } while (0)
#define disable_E2() do { } while (0)
#define disable_E3() do { } while (0)
#define disable_E4() do { } while (0)

#define MINIMUM_PLANNER_SPEED 0.05
// (mm/sec)

extern int fanSpeed;
extern int extrudemultiply;

#define F_CPU 16000000

#define manage_heater() do {} while(0)
#define lcd_update() do {} while(0)

#define byte uint8_t

// Fixes for missing stepper.h and stepper.cpp

#define STEPPER_ISR_ENABLED() false  // TODO: Check if this affects anything.
#define DISABLE_STEPPER_DRIVER_INTERRUPT() do {} while(0)
#define ENABLE_STEPPER_DRIVER_INTERRUPT() do {} while(0)
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

typedef struct ExtraData {
  double filepos; // file position in percentage.
  double extruder_position; // Extruded so far.
} ExtraData;

#define DRYRUN 1
#define DEBUGGING(F) false
#endif
