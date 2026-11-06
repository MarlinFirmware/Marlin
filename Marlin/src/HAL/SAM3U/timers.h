/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * HAL Timers for Atmel SAM3U (AT91SAM3U4E)
 *
 * IMPORTANT - this is the main way the SAM3U differs from its SAM3X8E cousin
 * on the Arduino Due. The SAM3U has a *single* Timer/Counter block with three
 * channels, and those channels are **16-bit** (datasheet section 35.6.2),
 * where the SAM3X has three blocks of three 32-bit channels.
 *
 * So:
 *   - hal_timer_t is uint16_t, exactly as on AVR, and Marlin's stepper ISR
 *     re-enters when a step interval exceeds the counter range.
 *   - All three channels are spoken for: stepper, temperature, tone. There is
 *     no spare channel, which is why servos and PWM are handled elsewhere.
 *
 * Clock choice. TIMER_CLOCK3 divides MCK by 32, giving 96MHz / 32 = 3MHz.
 * That mirrors AVR's proven 2MHz setup: the 0.333us resolution is plenty for
 * the step rates a 96MHz Cortex-M3 can service, and the resulting floor of
 * STEPPER_TIMER_RATE / 65535 = ~46 steps/s is far below any usable feedrate.
 * A faster tick (MCK/8 = 12MHz) would raise that floor to ~183 steps/s, which
 * slow Z and extruder moves really can reach.
 */

#include <stdint.h>
#include <sam3u4e.h>

// ------------------------
// Defines
// ------------------------

#define FORCE_INLINE __attribute__((always_inline)) inline

typedef uint16_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX hal_timer_t(UINT16_MAX)

#define HAL_TIMER_PRESCALER    32                             // TC_CMR_TCCLKS_TIMER_CLOCK3
#define HAL_TIMER_RATE         ((F_CPU) / (HAL_TIMER_PRESCALER))  // 3MHz

// Timer indices into timer_config[] - also the TC0 channel numbers
#ifndef MF_TIMER_STEP
  #define MF_TIMER_STEP         0  // TC0 channel 0
#endif
#ifndef MF_TIMER_PULSE
  #define MF_TIMER_PULSE        MF_TIMER_STEP
#endif
#ifndef MF_TIMER_TEMP
  #define MF_TIMER_TEMP         1  // TC0 channel 1
#endif
#ifndef MF_TIMER_TONE
  #define MF_TIMER_TONE         2  // TC0 channel 2
#endif

#define TEMP_TIMER_FREQUENCY   1000 // (Hz) Temperature ISR frequency

#define STEPPER_TIMER_RATE          HAL_TIMER_RATE                              // (Hz) Frequency of Stepper Timer ISR
#define STEPPER_TIMER_TICKS_PER_US  ((STEPPER_TIMER_RATE) / 1000000UL)          // (MHz) Stepper Timer ticks per us
#define STEPPER_TIMER_PRESCALE      (CYCLES_PER_MICROSECOND / STEPPER_TIMER_TICKS_PER_US)

#define PULSE_TIMER_RATE            STEPPER_TIMER_RATE
#define PULSE_TIMER_PRESCALE        STEPPER_TIMER_PRESCALE

#define ENABLE_STEPPER_DRIVER_INTERRUPT()   HAL_timer_enable_interrupt(MF_TIMER_STEP)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt(MF_TIMER_STEP)
#define STEPPER_ISR_ENABLED()               HAL_timer_interrupt_enabled(MF_TIMER_STEP)

#define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt(MF_TIMER_TEMP)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(MF_TIMER_TEMP)

#ifndef HAL_STEP_TIMER_ISR
  #define HAL_STEP_TIMER_ISR() extern "C" void TC0_Handler()
#endif
#ifndef HAL_TEMP_TIMER_ISR
  #define HAL_TEMP_TIMER_ISR() extern "C" void TC1_Handler()
#endif
#ifndef HAL_TONE_TIMER_ISR
  #define HAL_TONE_TIMER_ISR() extern "C" void TC2_Handler()
#endif

// ------------------------
// Types
// ------------------------

typedef struct {
  uint8_t     channel;    // TC0 channel, 0..2
  IRQn_Type   IRQ_Id;
  uint8_t     priority;
} tTimerConfig;

// ------------------------
// Public Variables
// ------------------------

extern const tTimerConfig timer_config[];

// ------------------------
// Public functions
// ------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  TC0->TC_CHANNEL[timer_config[timer_num].channel].TC_RC = compare;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  return hal_timer_t(TC0->TC_CHANNEL[timer_config[timer_num].channel].TC_RC);
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  return hal_timer_t(TC0->TC_CHANNEL[timer_config[timer_num].channel].TC_CV);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

FORCE_INLINE static void HAL_timer_isr_prologue(const uint8_t timer_num) {
  // Reading the status register clears the RC-compare interrupt flag
  (void)TC0->TC_CHANNEL[timer_config[timer_num].channel].TC_SR;
}

FORCE_INLINE static void HAL_timer_isr_epilogue(const uint8_t) {}
