#include "endstop_interrupts.h"
#include "../../module/endstops.h"
#include <Arduino.h>

#define ENDSTOP_IRQ_PRIORITY DDL_IRQ_PRIORITY_06

//
// IRQ handler
//
void endstopIRQHandler()
{
  bool flag = false;

// check all irq flags
#define CHECK(name) TERN_(USE_##name, flag |= checkIRQFlag(name##_PIN, /*clear*/ true))

  CHECK(X_MAX);
  CHECK(X_MIN);

  CHECK(Y_MAX);
  CHECK(Y_MIN);

  CHECK(Z_MAX);
  CHECK(Z_MIN);

  CHECK(Z2_MAX);
  CHECK(Z2_MIN);

  CHECK(Z3_MAX);
  CHECK(Z3_MIN);

  CHECK(Z_MIN_PROBE);

  // update endstops
  if (flag)
  {
    endstops.update();
  }
}

//
// HAL functions
//
void setup_endstop_interrupts()
{
#define SETUP(name) TERN_(USE_##name, attachInterrupt(name##_PIN, endstopIRQHandler, CHANGE); setInterruptPriority(name##_PIN, ENDSTOP_IRQ_PRIORITY))

  SETUP(X_MAX);
  SETUP(X_MIN);

  SETUP(Y_MAX);
  SETUP(Y_MIN);

  SETUP(Z_MAX);
  SETUP(Z_MIN);

  SETUP(Z2_MAX);
  SETUP(Z2_MIN);

  SETUP(Z3_MAX);
  SETUP(Z3_MIN);

  SETUP(Z_MIN_PROBE);
}

// ensure max. 10 irqs are registered
// if you encounter this error, you'll have to disable some endstops
#define ENDSTOPS_INTERRUPTS_COUNT COUNT_ENABLED(USE_X_MAX, USE_X_MIN, USE_Y_MAX, USE_Y_MIN, USE_Z_MAX, USE_Z_MIN, USE_Z2_MAX, USE_Z2_MIN, USE_Z3_MAX, USE_Z3_MIN, USE_Z_MIN_PROBE)
#if ENDSTOPS_INTERRUPTS_COUNT > 10
  #error "too many endstop interrupts! HC32F46x only supports 10 endstop interrupts."
#endif
#if ENDSTOPS_INTERRUPTS_COUNT == 0
  #error "no endstop interrupts are enabled! If you wish to ignore this error, please comment out this line."
#endif
