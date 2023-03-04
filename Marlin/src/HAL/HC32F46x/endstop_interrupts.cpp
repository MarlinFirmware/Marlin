#include "endstop_interrupts.h"
#include "../module/endstops.h"
#include "interrupts.h"

// Z_MIN_PROBE does not follow the same naming as all other pins...
#if HAS_Z_MIN_PROBE_PIN && !defined(HAS_Z_MIN_PROBE)
#define HAS_Z_MIN_PROBE
#endif

//
// IRQ handler
//
void endstopIRQHandler()
{
    bool flag = false;

// check all irq flags
#define CHECK(name) TERN_(HAS_##name, flag |= checkAndClearExtIRQFlag(name##_PIN))

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
#define SETUP(name) TERN_(HAS_##name, attachInterrupt(name##_PIN, endstopIRQHandler, CHANGE))

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
#if HAS_X_MAX && HAS_X_MIN && HAS_Y_MAX && HAS_Y_MIN && HAS_Z_MAX && HAS_Z_MIN && HAS_Z2_MAX && HAS_Z2_MIN && HAS_Z3_MAX && HAS_Z3_MIN && HAS_MIN_Z_PROBE
#error "too many endstop interrupts! HC32F46x only supports 10 endstop interrupts."
#endif
