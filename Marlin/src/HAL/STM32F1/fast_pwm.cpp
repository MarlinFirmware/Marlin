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
#ifdef __STM32F1__

#include "../../inc/MarlinConfigPre.h"

#if NEEDS_HARDWARE_PWM // Specific meta-flag for features that mandate PWM

#include <pwm.h>
#include "HAL.h"
#include "timers.h"

void set_pwm_frequency(const pin_t pin, int f_desired) {
    if(!PWM_PIN(pin)) return;   //don't proceed if no hardware timer

    timer_dev* timer = PIN_MAP[pin].timer_device;
    uint8_t chanel = PIN_MAP[pin].timer_channel;

    //protect used timers
    if(timer == get_timer_dev(STEP_TIMER_NUM)) return;
    if(timer == get_timer_dev(PULSE_TIMER_NUM)) return;
    if(timer == get_timer_dev(TEMP_TIMER_NUM)) return;
    
    if(!(timer->regs.bas->SR & TIMER_CR1_CEN)) { //if timer is disabled, enable
        timer_init(timer);
    }
    timer_set_mode(timer, chanel, TIMER_PWM);
    uint16_t preload = 255; //lock 255 pwm resolution for high frequencies
    int32_t prescaler = (HAL_TIMER_RATE/(preload + 1)/(f_desired)) - 1;
    if(prescaler > 65535) { //for low frequencies increase prescaler
        prescaler = 65535;
        preload = (HAL_TIMER_RATE/(prescaler+1)/(f_desired)) - 1;
    }
    if(prescaler < 0) return;   //too high frequency
    timer_set_reload(timer, preload);
    timer_set_prescaler(timer, prescaler);
}

void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size /*=255*/, const bool invert /*=false*/) {
    timer_dev* timer = PIN_MAP[pin].timer_device;
    uint16_t max_val = timer->regs.bas->ARR;
    pwmWrite(pin, invert ? v_size - max_val * v / v_size : max_val * v / v_size);
}

/* May be used for invoking HAL_timer_start

uint8_t get_timer_num_by_dev(timer_dev* dev){
    #if STM32_HAVE_TIMER(1)
    if(dev == &timer1) return 1;
    #endif
    #if STM32_HAVE_TIMER(2)
    if(dev == &timer2) return 2;
    #endif
    #if STM32_HAVE_TIMER(3)
    if(dev == &timer3) return 3;
    #endif
    #if STM32_HAVE_TIMER(4)
    if(dev == &timer4) return 4;
    #endif
    #if STM32_HAVE_TIMER(5)
    if(dev == &timer5) return 5;
    #endif
    #if STM32_HAVE_TIMER(6)
    if(dev == &timer6) return 6;
    #endif
    #if STM32_HAVE_TIMER(7)
    if(dev == &timer7) return 7;
    #endif
    #if STM32_HAVE_TIMER(8)
    if(dev == &timer8) return 8;
    #endif
    #if STM32_HAVE_TIMER(9)
    if(dev == &timer9) return 9;
    #endif
    #if STM32_HAVE_TIMER(10)
    if(dev == &timer10) return 10;
    #endif
    #if STM32_HAVE_TIMER(11)
    if(dev == &timer11) return 11;
    #endif
    #if STM32_HAVE_TIMER(12)
    if(dev == &timer12) return 12;
    #endif
    #if STM32_HAVE_TIMER(13)
    if(dev == &timer13) return 13;
    #endif
    #if STM32_HAVE_TIMER(14)
    if(dev == &timer14) return 14;
    #endif
}*/
#endif // NEEDS_HARDWARE_PWM
#endif // __STM32F1__