
#include "../../inc/MarlinConfig.h"
#include "HAL.h"
#include "timers.h"

static hal_timer_t compare_hal = 0x0;

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency)
{
  switch (timer_num)
  {
  case STEP_TIMER_NUM:
    setup_time2B(frequency);
    break;
  case TEMP_TIMER_NUM:
    setup_time2A(frequency);
    break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num)
{
  switch (timer_num)
  {
  case STEP_TIMER_NUM:
    ENABLE_STEPPER_DRIVER_INTERRUPT();
    break;
  case TEMP_TIMER_NUM:
    ENABLE_TEMPERATURE_INTERRUPT();
    break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num)
{
  switch (timer_num)
  {
  case STEP_TIMER_NUM:
    DISABLE_STEPPER_DRIVER_INTERRUPT();
    break;
  case TEMP_TIMER_NUM:
    DISABLE_TEMPERATURE_INTERRUPT();
    break;
  }
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num)
{
  return timer_irq_enabled(TMR_UNIT, timer_num);
}

void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare)
{
  if (compare_hal != compare)
  {
    compare_hal = compare;
    en_functional_state_t state_t = (timer_num == STEP_TIMER_NUM) ? Disable : Enable;
    timer_set_compare(timer_num, compare, state_t);
  }
}
