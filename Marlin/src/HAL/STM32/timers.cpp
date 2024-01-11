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
#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"

// ------------------------
// Local defines
// ------------------------

// Default timer priorities. Override by specifying alternate priorities in the board pins file.
// The TONE timer is not present here, as it currently cannot be set programmatically. It is set
// by defining TIM_IRQ_PRIO in the variant.h or platformio.ini file, which adjusts the default
// priority for STM32 HardwareTimer objects.
#define SWSERIAL_TIMER_IRQ_PRIO_DEFAULT  1 // Requires tight bit timing to communicate reliably with TMC drivers
#define SERVO_TIMER_IRQ_PRIO_DEFAULT     1 // Requires tight PWM timing to control a BLTouch reliably
#define STEP_TIMER_IRQ_PRIO_DEFAULT      2
#define TEMP_TIMER_IRQ_PRIO_DEFAULT     14 // Low priority avoids interference with other hardware and timers

#ifndef STEP_TIMER_IRQ_PRIO
  #define STEP_TIMER_IRQ_PRIO STEP_TIMER_IRQ_PRIO_DEFAULT
#endif
#ifndef TEMP_TIMER_IRQ_PRIO
  #define TEMP_TIMER_IRQ_PRIO TEMP_TIMER_IRQ_PRIO_DEFAULT
#endif
#if HAS_TMC_SW_SERIAL
  #include <SoftwareSerial.h>
  #ifndef SWSERIAL_TIMER_IRQ_PRIO
    #define SWSERIAL_TIMER_IRQ_PRIO SWSERIAL_TIMER_IRQ_PRIO_DEFAULT
  #endif
#endif
#if HAS_SERVOS
  #include "Servo.h"
  #ifndef SERVO_TIMER_IRQ_PRIO
    #define SERVO_TIMER_IRQ_PRIO SERVO_TIMER_IRQ_PRIO_DEFAULT
  #endif
#endif
#if ENABLED(SPEAKER)
  // Ensure the default timer priority is somewhere between the STEP and TEMP priorities.
  // The STM32 framework defaults to interrupt 14 for all timers. This should be increased so that
  // timing-sensitive operations such as speaker output are not impacted by the long-running
  // temperature ISR. This must be defined in the platformio.ini file or the board's variant.h,
  // so that it will be consumed by framework code.
  #if !(TIM_IRQ_PRIO > STEP_TIMER_IRQ_PRIO && TIM_IRQ_PRIO < TEMP_TIMER_IRQ_PRIO)
    #error "Default timer interrupt priority is unspecified or set to a value which may degrade performance."
  #endif
#endif

#if defined(STM32F0xx) || defined(STM32G0xx)
  #define MCU_STEP_TIMER 16
  #define MCU_TEMP_TIMER 17
#elif defined(STM32F1xx)
  #define MCU_STEP_TIMER  4
  #define MCU_TEMP_TIMER  2
#elif defined(STM32F401xC) || defined(STM32F401xE)
  #define MCU_STEP_TIMER  9           // STM32F401 has no TIM6, TIM7, or TIM8
  #define MCU_TEMP_TIMER 10
#elif defined(STM32F4xx) || defined(STM32F7xx) || defined(STM32H7xx)
  #define MCU_STEP_TIMER  6
  #define MCU_TEMP_TIMER 14           // TIM7 is consumed by Software Serial if used.
#endif

#ifndef HAL_TIMER_RATE
  #define HAL_TIMER_RATE GetStepperTimerClkFreq()
#endif

#ifndef STEP_TIMER
  #define STEP_TIMER MCU_STEP_TIMER
#endif
#ifndef TEMP_TIMER
  #define TEMP_TIMER MCU_TEMP_TIMER
#endif

#define __TIMER_DEV(X) TIM##X
#define _TIMER_DEV(X) __TIMER_DEV(X)
#define STEP_TIMER_DEV _TIMER_DEV(STEP_TIMER)
#define TEMP_TIMER_DEV _TIMER_DEV(TEMP_TIMER)

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

#define NUM_HARDWARE_TIMERS 2

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

HardwareTimer *timer_instance[NUM_HARDWARE_TIMERS] = { nullptr };

// ------------------------
// Public functions
// ------------------------

uint32_t GetStepperTimerClkFreq() {
  // Timer input clocks vary between devices, and in some cases between timers on the same device.
  // Retrieve at runtime to ensure device compatibility. Cache result to avoid repeated overhead.
  static uint32_t clkfreq = timer_instance[MF_TIMER_STEP]->getTimerClkFreq();
  return clkfreq;
}

// frequency is in Hertz
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  if (!HAL_timer_initialized(timer_num)) {
    switch (timer_num) {
      case MF_TIMER_STEP: // STEPPER TIMER - use a 32bit timer if possible
        timer_instance[timer_num] = new HardwareTimer(STEP_TIMER_DEV);
        /* Set the prescaler to the final desired value.
         * This will change the effective ISR callback frequency but when
         * HAL_timer_start(timer_num=0) is called in the core for the first time
         * the real frequency isn't important as long as, after boot, the ISR
         * gets called with the correct prescaler and count register. So here
         * we set the prescaler to the correct, final value and ignore the frequency
         * asked. We will call back the ISR in 1 second to start at full speed.
         *
         * The proper fix, however, would be a correct initialization OR a
         * HAL_timer_change(const uint8_t timer_num, const uint32_t frequency)
         * which changes the prescaler when an IRQ frequency change is needed
         * (for example when steppers are turned on)
         */

        timer_instance[timer_num]->setPrescaleFactor(STEPPER_TIMER_PRESCALE); //the -1 is done internally
        timer_instance[timer_num]->setOverflow(_MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (HAL_TIMER_RATE) / (STEPPER_TIMER_PRESCALE) /* /frequency */), TICK_FORMAT);
        break;
      case MF_TIMER_TEMP: // TEMP TIMER - any available 16bit timer
        timer_instance[timer_num] = new HardwareTimer(TEMP_TIMER_DEV);
        // The prescale factor is computed automatically for HERTZ_FORMAT
        timer_instance[timer_num]->setOverflow(frequency, HERTZ_FORMAT);
        break;
    }

    // Disable preload. Leaving it default-enabled can cause the timer to stop if it happens
    // to exit the ISR after the start time for the next interrupt has already passed.
    timer_instance[timer_num]->setPreloadEnable(false);

    HAL_timer_enable_interrupt(timer_num);

    // Start the timer.
    timer_instance[timer_num]->resume(); // First call to resume() MUST follow the attachInterrupt()

    // This is fixed in Arduino_Core_STM32 1.8.
    // These calls can be removed and replaced with
    // timer_instance[timer_num]->setInterruptPriority
    switch (timer_num) {
      case MF_TIMER_STEP:
        timer_instance[timer_num]->setInterruptPriority(STEP_TIMER_IRQ_PRIO, 0);
        break;
      case MF_TIMER_TEMP:
        timer_instance[timer_num]->setInterruptPriority(TEMP_TIMER_IRQ_PRIO, 0);
        break;
    }
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  if (HAL_timer_initialized(timer_num) && !timer_instance[timer_num]->hasInterrupt()) {
    switch (timer_num) {
      case MF_TIMER_STEP:
        timer_instance[timer_num]->attachInterrupt(Step_Handler);
        break;
      case MF_TIMER_TEMP:
        timer_instance[timer_num]->attachInterrupt(Temp_Handler);
        break;
    }
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  if (HAL_timer_initialized(timer_num)) timer_instance[timer_num]->detachInterrupt();
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  return HAL_timer_initialized(timer_num) && timer_instance[timer_num]->hasInterrupt();
}

void SetTimerInterruptPriorities() {
  TERN_(HAS_TMC_SW_SERIAL, SoftwareSerial::setInterruptPriority(SWSERIAL_TIMER_IRQ_PRIO, 0));
  TERN_(HAS_SERVOS, libServo::setInterruptPriority(SERVO_TIMER_IRQ_PRIO, 0));
}

// ------------------------
// Detect timer conflicts
// ------------------------

// This list serves two purposes. Firstly, it facilitates build-time mapping between
// variant-defined timer names (such as TIM1) and timer numbers. It also replicates
// the order of timers used in the framework's SoftwareSerial.cpp. The first timer in
// this list will be automatically used by SoftwareSerial if it is not already defined
// in the board's variant or compiler options.
static constexpr struct {uintptr_t base_address; int timer_number;} stm32_timer_map[] = {
  #ifdef TIM18_BASE
    { uintptr_t(TIM18), 18 },
  #endif
  #ifdef TIM7_BASE
    { uintptr_t(TIM7),   7 },
  #endif
  #ifdef TIM6_BASE
    { uintptr_t(TIM6),   6 },
  #endif
  #ifdef TIM22_BASE
    { uintptr_t(TIM22), 22 },
  #endif
  #ifdef TIM21_BASE
    { uintptr_t(TIM21), 21 },
  #endif
  #ifdef TIM17_BASE
    { uintptr_t(TIM17), 17 },
  #endif
  #ifdef TIM16_BASE
    { uintptr_t(TIM16), 16 },
  #endif
  #ifdef TIM15_BASE
    { uintptr_t(TIM15), 15 },
  #endif
  #ifdef TIM14_BASE
    { uintptr_t(TIM14), 14 },
  #endif
  #ifdef TIM13_BASE
    { uintptr_t(TIM13), 13 },
  #endif
  #ifdef TIM11_BASE
    { uintptr_t(TIM11), 11 },
  #endif
  #ifdef TIM10_BASE
    { uintptr_t(TIM10), 10 },
  #endif
  #ifdef TIM12_BASE
    { uintptr_t(TIM12), 12 },
  #endif
  #ifdef TIM19_BASE
    { uintptr_t(TIM19), 19 },
  #endif
  #ifdef TIM9_BASE
    { uintptr_t(TIM9),   9 },
  #endif
  #ifdef TIM5_BASE
    { uintptr_t(TIM5),   5 },
  #endif
  #ifdef TIM4_BASE
    { uintptr_t(TIM4),   4 },
  #endif
  #ifdef TIM3_BASE
    { uintptr_t(TIM3),   3 },
  #endif
  #ifdef TIM2_BASE
    { uintptr_t(TIM2),   2 },
  #endif
  #ifdef TIM20_BASE
    { uintptr_t(TIM20), 20 },
  #endif
  #ifdef TIM8_BASE
    { uintptr_t(TIM8),   8 },
  #endif
  #ifdef TIM1_BASE
    { uintptr_t(TIM1),   1 }
  #endif
};

// Convert from a timer base address to its integer timer number.
static constexpr int get_timer_num_from_base_address(uintptr_t base_address) {
  for (const auto &timer : stm32_timer_map)
    if (timer.base_address == base_address) return timer.timer_number;
  return 0;
}

// The platform's SoftwareSerial.cpp will use the first timer from stm32_timer_map.
#if HAS_TMC_SW_SERIAL && !defined(TIMER_SERIAL)
  #define  TIMER_SERIAL (stm32_timer_map[0].base_address)
#endif

// constexpr doesn't like using the base address pointers that timers evaluate to.
// We can get away with casting them to uintptr_t, if we do so inside an array.
// GCC will not currently do it directly to a uintptr_t.
TERN_(HAS_TMC_SW_SERIAL, static constexpr uintptr_t timer_serial[] = {uintptr_t(TIMER_SERIAL)});
TERN_(SPEAKER,           static constexpr uintptr_t timer_tone[]   = {uintptr_t(TIMER_TONE)});
TERN_(HAS_SERVOS,        static constexpr uintptr_t timer_servo[]  = {uintptr_t(TIMER_SERVO)});

enum TimerPurpose { TP_SERIAL, TP_TONE, TP_SERVO, TP_STEP, TP_TEMP };

// List of timers, to enable checking for conflicts.
// Includes the purpose of each timer to ease debugging when evaluating at build-time.
// This cannot yet account for timers used for PWM output, such as for fans.
static constexpr struct { TimerPurpose p; int t; } timers_in_use[] = {
  #if HAS_TMC_SW_SERIAL
    { TP_SERIAL, get_timer_num_from_base_address(timer_serial[0]) }, // Set in variant.h, or as a define in platformio.h if not present in variant.h
  #endif
  #if ENABLED(SPEAKER)
    { TP_TONE, get_timer_num_from_base_address(timer_tone[0]) },     // Set in variant.h, or as a define in platformio.h if not present in variant.h
  #endif
  #if HAS_SERVOS
    { TP_SERVO, get_timer_num_from_base_address(timer_servo[0]) },   // Set in variant.h, or as a define in platformio.h if not present in variant.h
  #endif
  { TP_STEP, STEP_TIMER },
  { TP_TEMP, TEMP_TIMER },
};

static constexpr bool verify_no_timer_conflicts() {
  for (uint8_t i = 0; i < COUNT(timers_in_use); ++i)
    for (uint8_t j = i + 1; j < COUNT(timers_in_use); ++j)
      if (timers_in_use[i].t == timers_in_use[j].t) return false;
  return true;
}

// If this assertion fails at compile time, review the timers_in_use array.
// If default_envs is defined properly in platformio.ini, VS Code can evaluate the array
// when hovering over it, making it easy to identify the conflicting timers.
static_assert(verify_no_timer_conflicts(), "One or more timer conflict detected. Examine \"timers_in_use\" to help identify conflict.");

// Support for Creality CV Laser module
// Code from CrealityOfficial laser support repository:
// https://github.com/CrealityOfficial/Ender-3S1/tree/ender-3s1-lasermodel
// 107011 激光模式

#if ENABLED(CV_LASER_MODULE)

  #define LASER_TIMER_NUM	                          3
  #define LASER_TIMER_DEV	                          _TIMER_DEV(LASER_TIMER_NUM)
  #define LASER_TIMER_PRESCALE(timer_clk, freq)     ((timer_clk) / ((freq) * (LASER_TIMER_PWM_MAX + 1)))
  #define LASER_TIMER_IRQ_PRIO                      1

  typedef enum {
    LASER_PWM_STATE_L = 0,
    LASER_PWM_STATE_H
  } laser_pwm_state_t;

  static HardwareTimer *timer_laser = nullptr;
  static uint8_t laser_h = 0, laser_l = 0;
  static laser_pwm_state_t laser_pwm_state;

  FORCE_INLINE static void laser_timer_set_compare(const hal_timer_t overflow) {
    if (timer_laser) {
      // 格式为TICK_FORMAT时，设置的是重载寄存器ARR，
      // 且setOverflow(overflow) {ARR = overflow - 1;}，所以形参需设置为overflow + 1
      timer_laser->setOverflow(overflow + 1, TICK_FORMAT);
      // 如果溢出值小于计数值。专业固件是最好的，将触发中断
      if (overflow < timer_laser->getCount())
        timer_laser->refresh();
    }
  }

  static void laser_timer_handler(void) {
    // SERIAL_ECHO_MSG("laser_timer_handler");
    switch(laser_pwm_state) {
      case LASER_PWM_STATE_L:
        laser_timer_set_compare(laser_h);
        WRITE(LASER_SOFT_PWM_PIN, 1);
        laser_pwm_state = LASER_PWM_STATE_H;
        break;
      case LASER_PWM_STATE_H:
        laser_timer_set_compare(laser_l);
        WRITE(LASER_SOFT_PWM_PIN, 0);
        laser_pwm_state = LASER_PWM_STATE_L;
        break;
    }
  }

  void laser_timer_soft_pwm_start(uint8_t pwm) {
    // SERIAL_ECHOLNPAIR("laser_timer_soft_pwm_start():", pwm);
    if (timer_laser == nullptr) return;
    if (pwm > LASER_TIMER_PWM_MAX) pwm = LASER_TIMER_PWM_MAX;
    if (pwm == 0x00) {
      laser_timer_soft_pwm_stop();
    }
    else if (pwm == 0xFF) {
      timer_laser->pause();
      OUT_WRITE(LASER_SOFT_PWM_PIN, 1);
    }
    else {
      timer_laser->pause();
      laser_pwm_state = LASER_PWM_STATE_H;
      WRITE(LASER_SOFT_PWM_PIN, 1);
      laser_l = LASER_TIMER_PWM_MAX - pwm;
      laser_h = pwm;
      laser_timer_set_compare(laser_h);
      timer_laser->resume();
      // 立即进入中断 -> 设置为0无法触发中断。
      // laser_timer_set_compare(0);
    }
  }

  // 相对停止， 以最弱的激光输出
  void laser_timer_soft_pwm_stop(void) {
    // SERIAL_ECHO_MSG("laser_timer_soft_pwm_stop()");
    laser_timer_soft_pwm_start(1);
  }

  void laser_timer_soft_pwm_close() {
    // SERIAL_ECHO_MSG("laser_timer_soft_pwm_close()");
    if(timer_laser == nullptr) return;
    timer_laser->pause();
    WRITE(LASER_SOFT_PWM_PIN, 0);
  }

  // 初始化 frequency: 频率
  void laser_timer_soft_pwm_init() {
    const uint32_t frequency = LASER_TIMER_FREQUENCY;
    if(timer_laser == nullptr) {
      // 创建硬件定时器。
      timer_laser = new HardwareTimer(LASER_TIMER_DEV);

      // 根据频率自动设置分频因子
      // timer_laser->setOverflow(frequency * LASER_TIMER_PWM_MAX, HERTZ_FORMAT);
      // 设置分频因子
      uint32_t prescale = LASER_TIMER_PRESCALE(timer_laser->getTimerClkFreq(), frequency);
      timer_laser->setPrescaleFactor(prescale);
      timer_laser->setOverflow(_MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (timer_laser->getTimerClkFreq()) / (prescale) /* /frequency */), TICK_FORMAT);

      // 禁用预加载。专业固件是最好的
      timer_laser->setPreloadEnable(false);

      // 设置中断处理函数
      if(!timer_laser->hasInterrupt()) timer_laser->attachInterrupt(laser_timer_handler);

      // 停止定时器
      timer_laser->pause();

      // 启动定时器
      // timer_laser->resume(); // First call to resume() MUST follow the attachInterrupt()

      // 设置中断优先级
      timer_laser->setInterruptPriority(LASER_TIMER_IRQ_PRIO, 0);
    }
  }

#endif // CV_LASER_MODULE

#endif // HAL_STM32
