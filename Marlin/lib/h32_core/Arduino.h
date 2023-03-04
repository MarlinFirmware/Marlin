#pragma once

#include "wirish.h"
#include "interrupts.h"
#include "drivers/systick/systick.h"
#include "drivers/gpio/gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern uint32_t CPU_FREQ;

#ifndef F_CPU
#define F_CPU CPU_FREQ
#endif

    extern void setup();
    extern void loop();

#ifdef __cplusplus
}
#endif
