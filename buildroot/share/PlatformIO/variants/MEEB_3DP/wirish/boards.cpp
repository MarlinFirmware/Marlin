/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 * Copyright (c) 2011, 2012 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file wirish/boards.cpp
 * @brief init() and board routines.
 *
 * This file is mostly interesting for the init() function, which
 * configures Flash, the core clocks, and a variety of other available
 * peripherals on the board so the rest of Wirish doesn't have to turn
 * things on before using them.
 *
 * Prior to returning, init() calls boardInit(), which allows boards
 * to perform any initialization they need to. This file includes a
 * weak no-op definition of boardInit(), so boards that don't need any
 * special initialization don't have to define their own.
 *
 * How init() works is chip-specific. See the boards_setup.cpp files
 * under e.g. wirish/stm32f1/, wirish/stmf32f2 for the details, but be
 * advised: their contents are unstable, and can/will change without
 * notice.
 */

#include <boards.h>
#include <libmaple/libmaple_types.h>
#include <libmaple/flash.h>
#include <libmaple/nvic.h>
#include <libmaple/systick.h>
#include "boards_private.h"

static void setup_flash(void);
static void setup_clocks(void);
static void setup_nvic(void);
static void setup_adcs(void);
static void setup_timers(void);

/*
 * Exported functions
 */

void init(void) {
    setup_flash();
    setup_clocks();
    setup_nvic();
    systick_init(SYSTICK_RELOAD_VAL);
    wirish::priv::board_setup_gpio();
    setup_adcs();
    setup_timers();
    wirish::priv::board_setup_usb();
    wirish::priv::series_init();
    boardInit();
}

/* Provide a default no-op boardInit(). */
__weak void boardInit(void) {
}

/* You could farm this out to the files in boards/ if e.g. it takes
 * too long to test on boards with lots of pins. */
bool boardUsesPin(uint8 pin) {
    for (int i = 0; i < BOARD_NR_USED_PINS; i++) {
        if (pin == boardUsedPins[i]) {
            return true;
        }
    }
    return false;
}

/*
 * Auxiliary routines
 */

static void setup_flash(void) {
    // Turn on as many Flash "go faster" features as
    // possible. flash_enable_features() just ignores any flags it
    // can't support.
    flash_enable_features(FLASH_PREFETCH | FLASH_ICACHE | FLASH_DCACHE);
    // Configure the wait states, assuming we're operating at "close
    // enough" to 3.3V.
    flash_set_latency(FLASH_SAFE_WAIT_STATES);
}

static void setup_clocks(void) {
    // Turn on HSI. We'll switch to and run off of this while we're
    // setting up the main PLL.
    rcc_turn_on_clk(RCC_CLK_HSI);

    // Turn off and reset the clock subsystems we'll be using, as well
    // as the clock security subsystem (CSS). Note that resetting CFGR
    // to its default value of 0 implies a switch to HSI for SYSCLK.
    RCC_BASE->CFGR = 0x00000000;
    rcc_disable_css();
    rcc_turn_off_clk(RCC_CLK_PLL);
    rcc_turn_off_clk(RCC_CLK_HSE);
    wirish::priv::board_reset_pll();
    // Clear clock readiness interrupt flags and turn off clock
    // readiness interrupts.
    RCC_BASE->CIR = 0x00000000;
#if !USE_HSI_CLOCK
    // Enable HSE, and wait until it's ready.
    rcc_turn_on_clk(RCC_CLK_HSE);
    while (!rcc_is_clk_ready(RCC_CLK_HSE))
        ;
#endif
    // Configure AHBx, APBx, etc. prescalers and the main PLL.
    wirish::priv::board_setup_clock_prescalers();
    rcc_configure_pll(&wirish::priv::w_board_pll_cfg);

    // Enable the PLL, and wait until it's ready.
    rcc_turn_on_clk(RCC_CLK_PLL);
    while(!rcc_is_clk_ready(RCC_CLK_PLL))
        ;

    // Finally, switch to the now-ready PLL as the main clock source.
    rcc_switch_sysclk(RCC_CLKSRC_PLL);
}

/*
 * These addresses are where usercode starts when a bootloader is
 * present. If no bootloader is present, the user NVIC usually starts
 * at the Flash base address, 0x08000000.
 */
#if defined(BOOTLOADER_maple)
	#define USER_ADDR_ROM 0x08002000
#else
	#define USER_ADDR_ROM 0x08000000
#endif
#define USER_ADDR_RAM 0x20000C00
extern char __text_start__;

static void setup_nvic(void) {

nvic_init((uint32)VECT_TAB_ADDR, 0);

/* Roger Clark. We now control nvic vector table in boards.txt using the build.vect paramater
#ifdef VECT_TAB_FLASH
    nvic_init(USER_ADDR_ROM, 0);
#elif defined VECT_TAB_RAM
    nvic_init(USER_ADDR_RAM, 0);
#elif defined VECT_TAB_BASE
    nvic_init((uint32)0x08000000, 0);
#elif defined VECT_TAB_ADDR
    // A numerically supplied value
    nvic_init((uint32)VECT_TAB_ADDR, 0);
#else
    // Use the __text_start__ value from the linker script; this
    // should be the start of the vector table.
    nvic_init((uint32)&__text_start__, 0);
#endif

*/
}

static void adc_default_config(adc_dev *dev) {
    adc_enable_single_swstart(dev);
    adc_set_sample_rate(dev, wirish::priv::w_adc_smp);
}

static void setup_adcs(void) {
    adc_set_prescaler(wirish::priv::w_adc_pre);
    adc_foreach(adc_default_config);
}

static void timer_default_config(timer_dev *dev) {
    timer_adv_reg_map *regs = (dev->regs).adv;
    const uint16 full_overflow = 0xFFFF;
    const uint16 half_duty = 0x8FFF;

    timer_init(dev);
    timer_pause(dev);

    regs->CR1 = TIMER_CR1_ARPE;
    regs->PSC = 1;
    regs->SR = 0;
    regs->DIER = 0;
    regs->EGR = TIMER_EGR_UG;
    switch (dev->type) {
    case TIMER_ADVANCED:
        regs->BDTR = TIMER_BDTR_MOE | TIMER_BDTR_LOCK_OFF;
        // fall-through
    case TIMER_GENERAL:
        timer_set_reload(dev, full_overflow);
        for (uint8 channel = 1; channel <= 4; channel++) {
            if (timer_has_cc_channel(dev, channel)) {
                timer_set_compare(dev, channel, half_duty);
                timer_oc_set_mode(dev, channel, TIMER_OC_MODE_PWM_1,
                                  TIMER_OC_PE);
            }
        }
        // fall-through
    case TIMER_BASIC:
        break;
    }

    timer_generate_update(dev);
    timer_resume(dev);
}

static void setup_timers(void) {
    timer_foreach(timer_default_config);
}
