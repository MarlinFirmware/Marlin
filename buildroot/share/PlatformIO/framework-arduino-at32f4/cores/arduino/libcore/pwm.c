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
#include "pwm.h"
#include "timer.h"
#include "Arduino.h"

/**
  * @brief  定时器输出捕获初始化
  * @param  TIMx: 定时器地址
  * @param  arr: 自动重装值
  * @param  psc: 时钟预分频数
  * @param  TimerChannel: 定时器通道
  * @retval 无
  */
static void TIMx_OCxInit(tmr_type* TIMx, uint32_t arr, uint16_t psc, uint8_t TimerChannel)
{
    tmr_output_config_type tmr_output_struct;

    Timer_ClockCmd(TIMx, true);

    tmr_base_init(TIMx, arr, psc);
    tmr_cnt_dir_set(TIMx, TMR_COUNT_UP);

    tmr_output_default_para_init(&tmr_output_struct);
    tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_B;
    tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
    tmr_output_struct.oc_idle_state = TRUE;
    tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_output_struct.occ_idle_state = FALSE;
    tmr_output_struct.oc_output_state = TRUE;
    tmr_output_struct.occ_output_state = TRUE;

    switch(TimerChannel)
    {
    case 1:
        tmr_output_channel_config(TIMx, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
        break;
    case 2:
        tmr_output_channel_config(TIMx, TMR_SELECT_CHANNEL_2, &tmr_output_struct);
        break;
    case 3:
        tmr_output_channel_config(TIMx, TMR_SELECT_CHANNEL_3, &tmr_output_struct);
        break;
    case 4:
        tmr_output_channel_config(TIMx, TMR_SELECT_CHANNEL_4, &tmr_output_struct);
        break;
    default:
        return;
    }

    tmr_output_enable(TIMx, TRUE);
    tmr_counter_enable(TIMx, TRUE);
}

/**
  * @brief  PWM输出初始化
  * @param  Pin:引脚编号
  * @param  Resolution: PWM分辨率
  * @param  Frequency: PWM频率
  * @retval 引脚对应的定时器通道
  */
uint8_t PWM_Init(uint8_t Pin, uint32_t Resolution, uint32_t Frequency)
{
    uint32_t arr, psc;

    if(!IS_PWM_PIN(Pin))
    {
        return 0;
    }

    if(Resolution == 0 || Frequency == 0 || (Resolution * Frequency) > F_CPU)
    {
        return 0;
    }

    pinMode(Pin, OUTPUT_AF_PP);

    arr = Resolution;
    psc = Timer_GetClockMax(PIN_MAP[Pin].TIMx) / Resolution / Frequency;

    Timer_SetEnable(PIN_MAP[Pin].TIMx, false);
    TIMx_OCxInit(PIN_MAP[Pin].TIMx, arr - 1, psc - 1, PIN_MAP[Pin].TimerChannel);
    return PIN_MAP[Pin].TimerChannel;
}

/**
  * @brief  输出PWM信号
  * @param  Pin: 引脚编号
  * @param  Value: PWM输出值
  * @retval PWM占空比值
  */
void PWM_Write(uint8_t Pin, uint32_t Value)
{
    Timer_SetCompare(PIN_MAP[Pin].TIMx, PIN_MAP[Pin].TimerChannel, Value);
}
