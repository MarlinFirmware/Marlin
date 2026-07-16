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
#include "delay.h"

#ifndef SYSTICK_TICK_FREQ
#  define SYSTICK_TICK_FREQ     1000 // Hz
#endif

#define SYSTICK_TICK_INTERVAL   (1000 / SYSTICK_TICK_FREQ)
#define SYSTICK_LOAD_VALUE      (system_core_clock / SYSTICK_TICK_FREQ)
#define SYSTICK_MILLIS          (SystemTickCount * SYSTICK_TICK_INTERVAL)

static volatile uint32_t SystemTickCount = 0;

/**
  * @brief  系统滴答定时器初始化，定时1ms
  * @param  无
  * @retval 无
  */
void Delay_Init(void)
{
    system_core_clock_update();
    SysTick_Config(SYSTICK_LOAD_VALUE);
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIORITY);
}

/**
  * @brief  系统滴答定时器中断入口
  * @param  无
  * @retval 无
  */
void SysTick_Handler(void)
{
    SystemTickCount++;
}

/**
  * @brief  获取单片机自上电以来经过的毫秒数
  * @param  无
  * @retval 当前系统时钟毫秒数
  */
uint32_t millis(void)
{
    return SYSTICK_MILLIS;
}

/**
  * @brief  获取单片机自上电以来经过的微秒数
  * @param  无
  * @retval 当前系统时钟微秒数
  */
uint32_t micros(void)
{
    return (SYSTICK_MILLIS * 1000 + (SYSTICK_LOAD_VALUE - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}

/**
  * @brief  毫秒级延时
  * @param  ms: 要延时的毫秒数
  * @retval 无
  */
void delay_ms(uint32_t ms)
{
    uint32_t tickstart = SystemTickCount;
    uint32_t wait = ms / SYSTICK_TICK_INTERVAL;

    while((SystemTickCount - tickstart) < wait)
    {
    }
}

/**
  * @brief  微秒级延时
  * @param  us: 要延时的微秒数
  * @retval 无
  */
void delay_us(uint32_t us)
{
    uint32_t total = 0;
    uint32_t target = CYCLES_PER_MICROSECOND * us;
    int last = SysTick->VAL;
    int now = last;
    int diff = 0;
start:
    now = SysTick->VAL;
    diff = last - now;
    if(diff > 0)
    {
        total += diff;
    }
    else
    {
        total += diff + SYSTICK_LOAD_VALUE;
    }
    if(total > target)
    {
        return;
    }
    last = now;
    goto start;
}
