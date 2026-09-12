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
#ifndef __EXTI_H
#define __EXTI_H

#include "mcu_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHANGE  EXINT_TRIGGER_BOTH_EDGE
#define FALLING EXINT_TRIGGER_FALLING_EDGE
#define RISING  EXINT_TRIGGER_RISING_EDGE

typedef void(*EXTI_CallbackFunction_t)(void);

void EXTIx_Init(
    uint8_t Pin,
    EXTI_CallbackFunction_t Function,
    exint_polarity_config_type line_polarity,
    uint8_t PreemptionPriority,
    uint8_t SubPriority
);
void attachInterrupt(
    uint8_t Pin,
    EXTI_CallbackFunction_t Function,
    exint_polarity_config_type polarity_config
);
void detachInterrupt(uint8_t Pin);

#ifdef __cplusplus
}
#endif

#endif
