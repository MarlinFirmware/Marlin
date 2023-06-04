#include "timers.h"
#include <core_debug.h>

/**
 * HAL only uses Timer0 Unit 2 for Temperature and Step interrupts
 */
Timer0 timer0(&TIMER02_config);

void HAL_timer_start(const timer_channel_t timer_num, const uint32_t frequency)
{
  switch(timer_num)
  {
    case TEMP_TIMER_NUM:
      CORE_DEBUG_PRINTF("HAL_timer_start: temp timer, f=%ld\n", long(frequency));
      timer0.start(TEMP_TIMER_NUM, frequency, TEMP_TIMER_PRESCALE);
      timer0.setCallback(TEMP_TIMER_NUM, &Temp_Handler);
      timer0.setCallbackPriority(TEMP_TIMER_NUM, TEMP_TIMER_PRIORITY);
      break;
    case STEP_TIMER_NUM:
      CORE_DEBUG_PRINTF("HAL_timer_start: step timer, f=%ld\n", long(frequency));
      timer0.start(STEP_TIMER_NUM, frequency, STEPPER_TIMER_PRESCALE);
      timer0.setCallback(STEP_TIMER_NUM, &Step_Handler);
      timer0.setCallbackPriority(STEP_TIMER_NUM, STEP_TIMER_PRIORITY);
      break;
  }
}

void HAL_timer_enable_interrupt(const timer_channel_t timer_num)
{
  timer0.resume(timer_num);
}

void HAL_timer_disable_interrupt(const timer_channel_t timer_num)
{
  timer0.pause(timer_num);
}

bool HAL_timer_interrupt_enabled(const timer_channel_t timer_num)
{
  return !timer0.isPaused(timer_num);
}

void HAL_timer_set_compare(const timer_channel_t timer_num, const hal_timer_t compare)
{
  timer0.setCompareValue(timer_num, compare);
}

hal_timer_t HAL_timer_get_count(const timer_channel_t timer_num)
{
  return timer0.getCount(timer_num);
}
