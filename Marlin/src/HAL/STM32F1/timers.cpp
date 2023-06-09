/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
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

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

// ------------------------
// Local defines
// ------------------------

// ------------------------
// Public functions
// ------------------------

/**
 * Timer_clock1: Prescaler   2 ->  36    MHz
 * Timer_clock2: Prescaler   8 ->   9    MHz
 * Timer_clock3: Prescaler  32 ->   2.25 MHz
 * Timer_clock4: Prescaler 128 -> 562.5  kHz
 */

/**
 * TODO: Calculate Timer prescale value, so we get the 32bit to adjust
 */

void HAL_timer_set_interrupt_priority(uint_fast8_t timer_num, uint_fast8_t priority) {
  nvic_irq_num irq_num;
  switch (timer_num) {
    case 1: irq_num = NVIC_TIMER1_CC; break;
    case 2: irq_num = NVIC_TIMER2; break;
    case 3: irq_num = NVIC_TIMER3; break;
    case 4: irq_num = NVIC_TIMER4; break;
    case 5: irq_num = NVIC_TIMER5; break;
    #ifdef STM32_HIGH_DENSITY
      // 6 & 7 are basic timers, avoid them
      case 8: irq_num = NVIC_TIMER8_CC; break;
    #endif
    default:
      /**
       * This should never happen. Add a Sanitycheck for timer number.
       * Should be a general timer since basic timers have no CC channels.
       */
      return;
  }

  nvic_irq_set_priority(irq_num, priority);
}

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  /**
   * Give the Stepper ISR a higher priority (lower number)
   * so it automatically preempts the Temperature ISR.
   */

  switch (timer_num) {
    case MF_TIMER_STEP:
      timer_pause(STEP_TIMER_DEV);
      timer_set_mode(STEP_TIMER_DEV, STEP_TIMER_CHAN, TIMER_OUTPUT_COMPARE); // counter
      timer_set_count(STEP_TIMER_DEV, 0);
      timer_set_prescaler(STEP_TIMER_DEV, (uint16_t)(STEPPER_TIMER_PRESCALE - 1));
      timer_set_reload(STEP_TIMER_DEV, 0xFFFF);
      timer_oc_set_mode(STEP_TIMER_DEV, STEP_TIMER_CHAN, TIMER_OC_MODE_FROZEN, TIMER_OC_NO_PRELOAD); // no output pin change
      timer_set_compare(STEP_TIMER_DEV, STEP_TIMER_CHAN, _MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (STEPPER_TIMER_RATE) / frequency));
      timer_no_ARR_preload_ARPE(STEP_TIMER_DEV); // Need to be sure no preload on ARR register
      timer_attach_interrupt(STEP_TIMER_DEV, STEP_TIMER_CHAN, stepTC_Handler);
      HAL_timer_set_interrupt_priority(MF_TIMER_STEP, STEP_TIMER_IRQ_PRIO);
      timer_generate_update(STEP_TIMER_DEV);
      timer_resume(STEP_TIMER_DEV);
      break;
    case MF_TIMER_TEMP:
      timer_pause(TEMP_TIMER_DEV);
      timer_set_mode(TEMP_TIMER_DEV, TEMP_TIMER_CHAN, TIMER_OUTPUT_COMPARE);
      timer_set_count(TEMP_TIMER_DEV, 0);
      timer_set_prescaler(TEMP_TIMER_DEV, (uint16_t)(TEMP_TIMER_PRESCALE - 1));
      timer_set_reload(TEMP_TIMER_DEV, 0xFFFF);
      timer_set_compare(TEMP_TIMER_DEV, TEMP_TIMER_CHAN, _MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (F_CPU) / (TEMP_TIMER_PRESCALE) / frequency));
      timer_attach_interrupt(TEMP_TIMER_DEV, TEMP_TIMER_CHAN, tempTC_Handler);
      HAL_timer_set_interrupt_priority(MF_TIMER_TEMP, TEMP_TIMER_IRQ_PRIO);
      timer_generate_update(TEMP_TIMER_DEV);
      timer_resume(TEMP_TIMER_DEV);
      break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: ENABLE_STEPPER_DRIVER_INTERRUPT(); break;
    case MF_TIMER_TEMP: ENABLE_TEMPERATURE_INTERRUPT(); break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: DISABLE_STEPPER_DRIVER_INTERRUPT(); break;
    case MF_TIMER_TEMP: DISABLE_TEMPERATURE_INTERRUPT(); break;
  }
}

static inline bool HAL_timer_irq_enabled(const timer_dev * const dev, const uint8_t interrupt) {
  return bool(*bb_perip(&(dev->regs).gen->DIER, interrupt));
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: return HAL_timer_irq_enabled(STEP_TIMER_DEV, STEP_TIMER_CHAN);
    case MF_TIMER_TEMP: return HAL_timer_irq_enabled(TEMP_TIMER_DEV, TEMP_TIMER_CHAN);
  }
  return false;
}

timer_dev* HAL_get_timer_dev(int number) {
  switch (number) {
    #if STM32_HAVE_TIMER(1)
      case 1: return &timer1;
    #endif
    #if STM32_HAVE_TIMER(2)
      case 2: return &timer2;
    #endif
    #if STM32_HAVE_TIMER(3)
      case 3: return &timer3;
    #endif
    #if STM32_HAVE_TIMER(4)
      case 4: return &timer4;
    #endif
    #if STM32_HAVE_TIMER(5)
      case 5: return &timer5;
    #endif
    #if STM32_HAVE_TIMER(6)
      case 6: return &timer6;
    #endif
    #if STM32_HAVE_TIMER(7)
      case 7: return &timer7;
    #endif
    #if STM32_HAVE_TIMER(8)
      case 8: return &timer8;
    #endif
    #if STM32_HAVE_TIMER(9)
      case 9: return &timer9;
    #endif
    #if STM32_HAVE_TIMER(10)
      case 10: return &timer10;
    #endif
    #if STM32_HAVE_TIMER(11)
      case 11: return &timer11;
    #endif
    #if STM32_HAVE_TIMER(12)
      case 12: return &timer12;
    #endif
    #if STM32_HAVE_TIMER(13)
      case 13: return &timer13;
    #endif
    #if STM32_HAVE_TIMER(14)
      case 14: return &timer14;
    #endif
    default: return nullptr;
  }
}

#if HAS_LASER_E3S1PRO

  static unsigned int sw = 0, laser_h = 0, laser_l = 0;

  void laser_timer_soft_pwm_start(const uint8_t pwm) {
    //SERIAL_ECHOLNPGM("laser_timer_soft_pwm_start():", pwm);

    LIMIT(pwm, 1, 255);

    if (pwm <= 0x00) { //
      timer_pause(LASER_TIMER_DEV);
      WRITE(LASER_SOFT_PWM_PIN, 0); //WRITE(PC0, 0);
    }
    else if (pwm >= 0xFE) {
      timer_pause(LASER_TIMER_DEV);
      WRITE(LASER_SOFT_PWM_PIN, 1); //WRITE(PC0, 0);
    }
    else {
      timer_pause(LASER_TIMER_DEV);
      sw = 0;
      laser_l = 256 - pwm;
      laser_h = pwm;
      timer_resume(LASER_TIMER_DEV);
    }
  }

  void laser_timer_soft_pwm_stop() {
    //SERIAL_ECHOLNPGM("laser_timer_soft_pwm_stop()");
    laser_timer_soft_pwm_start(1);
  }

  void laser_timer_soft_pwm_close() {
    timer_pause(LASER_TIMER_DEV);
  	WRITE(LASER_SOFT_PWM_PIN, LOW); // WRITE(PC0, 0)
  }

  void laser_timer_handler() {
    //SERIAL_ECHOLNPGM("laser_timer_handler");

    switch (sw) {
      case 0:
        timer_set_reload(LASER_TIMER_DEV, laser_l); // 0xFFFF
        WRITE(LASER_SOFT_PWM_PIN, HIGH);            // WRITE(PC0, 1)
        sw = 1;
        break;
      case 1:
        timer_set_reload(LASER_TIMER_DEV, laser_h); // 0xFFFF
        WRITE(LASER_SOFT_PWM_PIN, LOW);             // WRITE(PC0, 0)
        sw = 0;
        break;
    }
  }

  void laser_timer_soft_pwm_init(const uint32_t frequency) {
    timer_pause(LASER_TIMER_DEV);
    //timer_set_mode(LASER_TIMER_DEV, TEMP_TIMER_CHAN, TIMER_OUTPUT_COMPARE);
    timer_set_count(LASER_TIMER_DEV, 0);
    timer_set_prescaler(LASER_TIMER_DEV, (uint16_t)(LASER_TIMER_PRESCALE(frequency) - 1));
    timer_set_reload(LASER_TIMER_DEV, LASER_TIMER_PWM_MAX);//0xFFFF);
    //timer_set_compare(LASER_TIMER_DEV, TEMP_TIMER_CHAN, _MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (F_CPU) / (LASER_TIMER_PRESCALE) / frequency));
    timer_attach_interrupt(LASER_TIMER_DEV, TEMP_TIMER_CHAN, laser_timer_handler);
    timer_set_interrupt_priority(LASER_TIMER_NUM, LASER_TIMER_IRQ_PRIO);
    timer_generate_update(LASER_TIMER_DEV);
    // timer_resume(LASER_TIMER_DEV);
  }

#endif // HAS_LASER_E3S1PRO

#endif // __STM32F1__
