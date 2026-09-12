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
#include "timer.h"
#include <string.h>  // memcpy, memset vb. için EKLENDİ
#include <stdint.h>  // uint32_t için (opsiyonel ama iyi)

typedef enum
{
    TIMER1, TIMER2, TIMER3, TIMER4, TIMER5, TIMER6, TIMER7, TIMER8,
    TIMER9, TIMER10, TIMER11, TIMER12, TIMER13, TIMER14, TIMER15,
    TIMER_MAX
} TIMER_Type;

static Timer_CallbackFunction_t Timer_CallbackFunction[TIMER_MAX] = { 0 };

/**
  * @brief  启动或关闭指定定时器的时钟
  * @param  TIMx:定时器地址
  * @param  NewState: ENABLE启动，DISABLE关闭
  * @retval 无
  */
void Timer_ClockCmd(tmr_type* TIMx, bool Enable)
{
    int index;
    typedef struct
    {
        tmr_type* tmr;
        crm_periph_clock_type crm_periph_clock;
    } crm_tmr_clock_map_t;

#   define CLOCK_MAP_DEF(n) {TMR##n, CRM_TMR##n##_PERIPH_CLOCK}

    static const crm_tmr_clock_map_t clock_map[] =
    {
        CLOCK_MAP_DEF(1),
        CLOCK_MAP_DEF(2),
        CLOCK_MAP_DEF(3),
        CLOCK_MAP_DEF(4),
        CLOCK_MAP_DEF(5),
        CLOCK_MAP_DEF(6),
        CLOCK_MAP_DEF(7),
        CLOCK_MAP_DEF(8),
        CLOCK_MAP_DEF(9),
        CLOCK_MAP_DEF(10),
        CLOCK_MAP_DEF(11),
        CLOCK_MAP_DEF(12),
        CLOCK_MAP_DEF(13),
        CLOCK_MAP_DEF(14)
    };

    for(index = 0; index < sizeof(clock_map) / sizeof(crm_tmr_clock_map_t); index++)
    {
        if(TIMx == clock_map[index].tmr)
        {
            crm_periph_clock_enable(clock_map[index].crm_periph_clock, Enable ? TRUE : FALSE);
        }
    }
}

static float Qsqrt(float number)
{
    uint32_t i;
    float x2;
    float y = number;  // Initialize immediately
    const float threehalfs = 1.5f;
    
    x2 = number * 0.5f;
    
    // Safe type-punning using memcpy
    memcpy(&i, &y, sizeof(i));
    i = 0x5f3759df - (i >> 1);
    memcpy(&y, &i, sizeof(y));
    
    // Two iterations of Newton-Raphson
    y = y * (threehalfs - (x2 * y * y));
    y = y * (threehalfs - (x2 * y * y));
    
    return 1.0f / y;
}

/**
  * @brief  频率因数分解，获取接近的值
  * @param  freq: 中断频率(Hz)
  * @param  clock: 定时器时钟
  * @param  *period: 重装值地址
  * @param  *prescaler: 时钟分频值地址
  * @retval 误差值(Hz)
  */
static bool Timer_FreqFactorization(
    uint32_t freq,
    uint32_t clock,
    uint16_t* factor1,
    uint16_t* factor2,
    int32_t* error
)
{
    uint32_t targetProdect;
    uint16_t fct1;
    uint16_t fct2;
    uint16_t fct1_save = 1;
    uint16_t fct2_save = 1;
    uint16_t fct_max;
    uint16_t max_error = 0xFFFF;

    if(freq == 0 || freq > clock)
    {
        return false;
    }

    /* 获取目标乘积 */
    targetProdect = clock / freq;

    /* 从targetProdect的平方根开始计算 */
    fct1 = Qsqrt(targetProdect);

    /* 计算因数最大值，减少遍历次数 */
    fct_max = (targetProdect < 0xFFFF) ? targetProdect : 0xFFFF;

    /* 遍历，使两因数的乘积足够接近prodect */
    for(; fct1 > 1; fct1--)
    {
        for(fct2 = fct1; fct2 < fct_max; fct2++)
        {
            /* 求误差 */
            int32_t newerr = fct1 * fct2 - targetProdect;

            if(newerr < 0)
            {
                newerr = -newerr;
            }

            if(newerr < max_error)
            {
                /* 保存最小误差 */
                max_error = (uint16_t)newerr;

                fct1_save = fct1;
                fct2_save = fct2;

                /* 最佳 */
                if(max_error == 0)
                {
                    break;
                }
            }
        }
    }

    *factor1 = fct1_save;
    *factor2 = fct2_save;
    *error = (freq - clock / (fct1_save * fct2_save));

    return true;
}

/**
  * @brief  将定时中断时间转换为重装值和时钟分频值
  * @param  time: 中断时间(微秒)
  * @param  clock: 定时器时钟
  * @param  *period: 重装值地址
  * @param  *prescaler: 时钟分频值地址
  * @retval 无
  */
static void Timer_TimeFactorization(
    uint32_t time,
    uint32_t clock,
    uint16_t* factor1,
    uint16_t* factor2
)
{
    const uint32_t cyclesPerMicros = clock / 1000000U;
    const uint32_t prodect = time * cyclesPerMicros;
    uint16_t fct1, fct2;

    if(prodect < cyclesPerMicros * 30)
    {
        fct1 = 10;
        fct2 = prodect / 10;
    }
    else if(prodect < 65535 * 1000)
    {
        fct1 = prodect / 1000;
        fct2 = prodect / fct1;
    }
    else
    {
        fct1 = prodect / 20000;
        fct2 = prodect / fct1;
    }
    *factor1 = fct1;
    *factor2 = fct2;
}

/**
  * @brief  定时器使能
  * @param  TIMx: 定时器地址
  * @param  Enable: 使能
  * @retval 无
  */
void Timer_SetEnable(tmr_type* TIMx, bool Enable)
{
    tmr_counter_enable(TIMx, Enable ? TRUE : FALSE);
}

/**
  * @brief  定时中断配置
  * @param  TIMx:定时器地址
  * @param  Time: 中断时间(微秒)
  * @param  Function: 定时中断回调函数
  * @retval 无
  */
void Timer_SetInterrupt(tmr_type* TIMx, uint32_t Time, Timer_CallbackFunction_t Function)
{
    uint16_t period = 0;
    uint16_t prescaler = 0;
    uint32_t clock = Timer_GetClockMax(TIMx);

    if(Time == 0)
    {
        return;
    }

    /*将定时中断时间转换为重装值和时钟分频值*/
    Timer_TimeFactorization(
        Time,
        clock,
        &period,
        &prescaler
    );

    /*定时中断配置*/
    Timer_SetInterruptBase(
        TIMx,
        period,
        prescaler,
        Function,
        TIMER_PREEMPTIONPRIORITY_DEFAULT,
        TIMER_SUBPRIORITY_DEFAULT
    );
}

/**
  * @brief  更新定时中断频率
  * @param  TIMx:定时器地址
  * @param  Freq:中断频率
  * @retval true: 设置成功
  */
bool Timer_SetInterruptFreqUpdate(tmr_type* TIMx, uint32_t Freq)
{
    uint16_t period, prescaler;
    uint32_t clock = Timer_GetClockMax(TIMx);
    int32_t error;

    if(Freq == 0)
        return false;

    bool success = Timer_FreqFactorization(
                       Freq,
                       clock,
                       &period,
                       &prescaler,
                       &error
                   );

    if(!success)
    {
        return false;
    }

    tmr_base_init(TIMx, period - 1, prescaler - 1);
    return true;
}

/**
  * @brief  获取定时器最大频率
  * @param  TIMx:定时器地址
  * @retval 最大频率
  */
uint32_t Timer_GetClockMax(tmr_type* TIMx)
{
    static crm_clocks_freq_type crm_clocks_freq_struct = {0};
    if(!crm_clocks_freq_struct.sclk_freq)
    {
        crm_clocks_freq_get(&crm_clocks_freq_struct);
    }

    return crm_clocks_freq_struct.apb1_freq * 2;
}

/**
  * @brief  获取定时器中断频率
  * @param  TIMx:定时器地址
  * @retval 中断频率
  */
uint32_t Timer_GetClockOut(tmr_type* TIMx)
{
    uint32_t clock = Timer_GetClockMax(TIMx);

    return (clock / ((TIMx->pr + 1) * (TIMx->div + 1)));
}

/**
  * @brief  更新定时中断时间
  * @param  TIMx:定时器地址
  * @param  Time: 中断时间(微秒)
  * @retval 无
  */
void Timer_SetInterruptTimeUpdate(TIM_TypeDef* TIMx, uint32_t Time)
{
    uint16_t period, prescaler;
    uint32_t clock = Timer_GetClockMax(TIMx);

    Timer_TimeFactorization(
        Time,
        clock,
        &period,
        &prescaler
    );

    tmr_base_init(TIMx, period - 1, prescaler - 1);
}

/**
  * @brief  定时中断基本配置
  * @param  TIMx:定时器地址
  * @param  Period:重装值
  * @param  Prescaler:时钟分频值
  * @param  Function: 定时中断回调函数
  * @param  PreemptionPriority: 抢占优先级
  * @param  SubPriority: 子优先级
  * @retval 无
  */
void Timer_SetInterruptBase(
    tmr_type* TIMx,
    uint16_t Period,
    uint16_t Prescaler,
    Timer_CallbackFunction_t Function,
    uint8_t PreemptionPriority,
    uint8_t SubPriority
)
{
    IRQn_Type TMRx_IRQn = (IRQn_Type)0;
    TIMER_Type TIMERx = TIMER1;

#define TMRx_IRQ_DEF(n,x_IRQn)\
do{\
    if(TIMx == TIM##n)\
    {\
        TIMERx = TIMER##n;\
        TMRx_IRQn = x_IRQn;\
        goto match;\
    }\
}\
while(0)

    /*如果编译器提示：identifier "xxx_IRQn" is undefined
     *把未定义的注释掉即可
     */
    TMRx_IRQ_DEF(1, TMR1_OVF_TMR10_IRQn);
    TMRx_IRQ_DEF(2, TMR2_GLOBAL_IRQn);
    TMRx_IRQ_DEF(3, TMR3_GLOBAL_IRQn);
    TMRx_IRQ_DEF(4, TMR4_GLOBAL_IRQn);
    TMRx_IRQ_DEF(5, TMR5_GLOBAL_IRQn);
    TMRx_IRQ_DEF(6, TMR6_GLOBAL_IRQn);
    TMRx_IRQ_DEF(7, TMR7_GLOBAL_IRQn);
    TMRx_IRQ_DEF(8, TMR8_OVF_TMR13_IRQn);
    TMRx_IRQ_DEF(9, TMR1_BRK_TMR9_IRQn);
    TMRx_IRQ_DEF(10, TMR1_OVF_TMR10_IRQn);
    TMRx_IRQ_DEF(11, TMR1_TRG_HALL_TMR11_IRQn);
    TMRx_IRQ_DEF(12, TMR8_BRK_TMR12_IRQn);
    TMRx_IRQ_DEF(13, TMR8_OVF_TMR13_IRQn);
    TMRx_IRQ_DEF(14, TMR8_TRG_HALL_TMR14_IRQn);

match:

    if(TMRx_IRQn == 0)
    {
        return;
    }

    Timer_CallbackFunction[TIMERx] = Function;

    tmr_reset(TIMx);
    Timer_ClockCmd(TIMx, true);

    tmr_repetition_counter_set(TIMx, 0);
    tmr_cnt_dir_set(TIMx, TMR_COUNT_UP);
    tmr_base_init(TIMx, Period - 1, Prescaler - 1);

    nvic_irq_enable(TMRx_IRQn, PreemptionPriority, SubPriority);

    tmr_flag_clear(TIMx, TMR_OVF_FLAG);
    tmr_interrupt_enable(TIMx, TMR_OVF_INT, TRUE);
}

/**
  * @brief  设置输出比较值
  * @param  TIMx: 定时器地址
  * @param  TimerChannel: 定时器通道
  * @param  Compare:输出比较值
  * @retval 无
  */
void Timer_SetCompare(TIM_TypeDef* TIMx, uint8_t TimerChannel, uint32_t Compare)
{
    switch(TimerChannel)
    {
    case 1:
        tmr_channel_value_set(TIMx, TMR_SELECT_CHANNEL_1, Compare);
        break;
    case 2:
        tmr_channel_value_set(TIMx, TMR_SELECT_CHANNEL_2, Compare);
        break;
    case 3:
        tmr_channel_value_set(TIMx, TMR_SELECT_CHANNEL_3, Compare);
        break;
    case 4:
        tmr_channel_value_set(TIMx, TMR_SELECT_CHANNEL_4, Compare);
        break;
    default:
        break;
    }
}

/**
  * @brief  获取捕获值
  * @param  TIMx: 定时器地址
  * @param  TimerChannel: 定时器通道
  * @retval 捕获值
  */
uint16_t Timer_GetCompare(TIM_TypeDef* TIMx, uint8_t TimerChannel)
{
    uint16_t retval = 0;
    switch(TimerChannel)
    {
    case 1:
        retval = tmr_channel_value_get(TIMx, TMR_SELECT_CHANNEL_1);
        break;
    case 2:
        retval = tmr_channel_value_get(TIMx, TMR_SELECT_CHANNEL_2);
        break;
    case 3:
        retval = tmr_channel_value_get(TIMx, TMR_SELECT_CHANNEL_3);
        break;
    case 4:
        retval = tmr_channel_value_get(TIMx, TMR_SELECT_CHANNEL_4);
        break;
    default:
        break;
    }

    return retval;
}

/**
  * @brief  更新定时器时钟预分频数
  * @param  TIMx: 定时器地址
  * @param  rescaler: 时钟预分频数
  * @retval 无
  */
void Timer_SetPrescaler(tmr_type* TIMx, uint16_t rescaler)
{
    TIMx->div = rescaler;
}

/**
  * @brief  更新定时器自动重装值
  * @param  TIMx: 定时器地址
  * @param  Reload: 自动重装值
  * @retval 无
  */
void Timer_SetReload(tmr_type* TIMx, uint16_t Reload)
{
    TIMx->pr = Reload;
}

void Timer_SetCounter(tmr_type* TIMx, uint32_t Counter)
{
    TIMx->cval = Counter;
}

/**
  * @brief  应用定时器更改
  * @param  TIMx: 定时器地址
  * @retval 无
  */
void Timer_GenerateUpdate(tmr_type* TIMx)
{
    TIMx->swevt_bit.ovfswtr = TRUE;
}

#define TMRx_IRQHANDLER(n) \
do{\
    if (tmr_flag_get(TMR##n, TMR_OVF_FLAG) != RESET)\
    {\
        if(Timer_CallbackFunction[TIMER##n])\
        {\
            Timer_CallbackFunction[TIMER##n]();\
        }\
        tmr_flag_clear(TMR##n, TMR_OVF_FLAG);\
    }\
}while(0)

/**
  * @brief  定时中断入口，定时器1、10
  * @param  无
  * @retval 无
  */
void TMR1_OV_TMR10_IRQHandler(void)
{
    TMRx_IRQHANDLER(1);
    TMRx_IRQHANDLER(10);
}

/**
  * @brief  定时中断入口，定时器2
  * @param  无
  * @retval 无
  */
void TMR2_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(2);
}

/**
  * @brief  定时中断入口，定时器3
  * @param  无
  * @retval 无
  */
void TMR3_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(3);
}

/**
  * @brief  定时中断入口，定时器4
  * @param  无
  * @retval 无
  */
void TMR4_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(4);
}

/**
  * @brief  定时中断入口，定时器5
  * @param  无
  * @retval 无
  */
void TMR5_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(5);
}

/**
  * @brief  定时中断入口，定时器6
  * @param  无
  * @retval 无
  */
void TMR6_GLOBAL_IRQHandler(void)
{
#ifdef TMR6
    TMRx_IRQHANDLER(6);
#endif
}

/**
  * @brief  定时中断入口，定时器7
  * @param  无
  * @retval 无
  */
void TMR7_GLOBAL_IRQHandler(void)
{
#ifdef TMR7
    TMRx_IRQHANDLER(7);
#endif
}

/**
  * @brief  定时中断入口，定时器8、13
  * @param  无
  * @retval 无
  */
void TMR8_OV_TMR13_IRQHandler(void)
{
    TMRx_IRQHANDLER(8);
#ifdef TMR13
    TMRx_IRQHANDLER(13);
#endif
}

/**
  * @brief  定时中断入口，定时器15
  * @param  无
  * @retval 无
  */
void TMR15_OV_IRQHandler(void)
{
#ifdef TMR15
    TMRx_IRQHANDLER(15);
#endif
}
