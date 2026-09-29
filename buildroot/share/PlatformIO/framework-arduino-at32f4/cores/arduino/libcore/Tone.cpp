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
#include "Tone.h"
#include "Arduino.h"

#define TONE_FREQ_MAX 500000U

static tmr_type* Tone_Timer = NULL;
static bool Tone_IsContinuousModeEnable = false;
static uint8_t Tone_Pin = NOT_A_PIN;
static uint32_t Tone_ToggleCounter = 0;

/**
  * @brief  tone中断入口，被定时中断调用
  * @param  无
  * @retval 无
  */
static void Tone_TimerHandler()
{
    if(!Tone_IsContinuousModeEnable)
    {
        if(Tone_ToggleCounter == 0)
        {
            noTone(Tone_Pin);
            return;
        }

        Tone_ToggleCounter--;
    }

    togglePin(Tone_Pin);
}

/**
  * @brief  改变tone所使用的定时器
  * @param  TIMx: 定时器地址
  * @retval 无
  */
void toneSetTimer(tmr_type* TIMx)
{
    Timer_SetInterruptBase(
        TIMx,
        0xFF,
        0xFF,
        Tone_TimerHandler,
        TONE_PREEMPTIONPRIORITY_DEFAULT,
        TONE_SUBPRIORITY_DEFAULT
    );
    Tone_Timer = TIMx;
}

/**
  * @brief  在Pin上生成指定频率 (50%占空比的方波)
  * @param  pin: 产生音调的 Pin
  * @param  freq: 频率(Hz)
  * @param  duration: 音调的持续时间 (以毫秒为单位)
  * @retval 无
  */
void tone(uint8_t pin, uint32_t freq, uint32_t duration)
{
    noTone(pin);

    if(duration == 0 || freq == 0 || freq > TONE_FREQ_MAX)
    {
        return;
    }

    Tone_Pin = pin;
    Tone_IsContinuousModeEnable = (duration == TONE_DURATION_INFINITE) ? true : false;

    if(!Tone_IsContinuousModeEnable)
    {
        Tone_ToggleCounter = freq * duration / 1000 * 2;

        if(Tone_ToggleCounter == 0)
        {
            return;
        }

        Tone_ToggleCounter--;
    }

    if(Tone_Timer == NULL)
    {
        toneSetTimer(TONE_TIMER_DEFAULT);
    }

    Timer_SetInterruptTimeUpdate(Tone_Timer, TONE_FREQ_MAX / freq);
    Timer_SetEnable(Tone_Timer, true);
}

/**
  * @brief  关闭声音
  * @param  Pin: 产生音调的引脚编号
  * @retval 无
  */
void noTone(uint8_t pin)
{
    if(Tone_Timer)
    {
        Timer_SetEnable(Tone_Timer, false);
    }

    digitalWrite(pin, LOW);
    Tone_IsContinuousModeEnable = false;
    Tone_Pin = NOT_A_PIN;
    Tone_ToggleCounter = 0;
}
