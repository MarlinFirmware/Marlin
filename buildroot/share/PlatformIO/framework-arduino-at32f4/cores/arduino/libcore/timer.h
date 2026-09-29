/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __TIMER_H
#define __TIMER_H

#include <stdbool.h>
#include "mcu_type.h"

#ifdef __cplusplus
extern "C" {
#endif
   
typedef void(*Timer_CallbackFunction_t)(void);

void     Timer_SetEnable(tmr_type* TIMx, bool Enable);
void     Timer_SetInterrupt(tmr_type* TIMx, uint32_t time, Timer_CallbackFunction_t Function);
void     Timer_SetInterruptTimeUpdate(tmr_type* TIMx, uint32_t Time);
bool     Timer_SetInterruptFreqUpdate(tmr_type* TIMx, uint32_t Freq);
void     Timer_SetCounter(tmr_type* TIMx, uint32_t Counter);
void     Timer_SetInterruptBase(
    tmr_type* TIMx, 
    uint16_t Period, uint16_t Prescaler, 
    Timer_CallbackFunction_t Function, 
    uint8_t PreemptionPriority, uint8_t SubPriority
);
void     Timer_SetCompare(tmr_type* TIMx, uint8_t TimerChannel, uint32_t Compare);
void     Timer_SetPrescaler(tmr_type* TIMx, uint16_t Prescaler);
void     Timer_SetReload(tmr_type* TIMx, uint16_t Reload);
void     Timer_ClockCmd(tmr_type* TIMx, bool Enable);
uint32_t Timer_GetClockMax(tmr_type* TIMx);
uint32_t Timer_GetClockOut(tmr_type* TIMx);
uint16_t Timer_GetCompare(tmr_type* TIMx, uint8_t TimerChannel);
void     Timer_GenerateUpdate(tmr_type* TIMx);

#ifdef __cplusplus
}
#endif

#endif
