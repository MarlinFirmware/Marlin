#include "timers.h"
#include "hc32f46x_clk.h"
#include "hc32f46x_pwc.h"
#include "hc32f46x_timer0.h"
#include "hc32f46x_interrupts.h"

#define TMR_UNIT M4_TMR02

//
// Util
//

inline void setupTimer(const en_tim0_channel_t channel, const uint32_t frequency, const uint32_t prescaler)
{
  // get pclk1 frequency
  stc_clk_freq_t clkInfo;
  CLK_GetClockFreq(&clkInfo);
  uint32_t pclk1Freq = clkInfo.pclk1Freq;

  // enable Timer0 peripheral
  PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM02, Enable);

  // configure timer channel
  stc_tim0_base_init_t timerConf;
  MEM_ZERO_STRUCT(timerConf);
  timerConf.Tim0_CounterMode = Tim0_Sync;
  timerConf.Tim0_SyncClockSource = Tim0_Pclk1;
  timerConf.Tim0_ClockDivision = Tim0_ClkDiv16;
  timerConf.Tim0_CmpValue = (uint16_t)((pclk1Freq / prescaler) / frequency);

  TIMER0_BaseInit(TMR_UNIT, channel, &timerConf);
}

inline void setupTimerIRQ(const en_tim0_channel_t channel, const IRQn irq, const en_int_src_t source, const uint32_t priority, const func_ptr_t handler)
{
  // enable timer interrupt
  TIMER0_IntCmd(TMR_UNIT, channel, Enable);

  stc_irq_regi_conf_t irqConf;
  MEM_ZERO_STRUCT(irqConf);

  // register timer interrupt on choosen irq
  irqConf.enIRQn = irq;
  irqConf.enIntSrc = source;

  // set irq handler
  irqConf.pfnCallback = handler;

  // register the irq
  enIrqRegistration(&irqConf);

  // clear pending irq, set priority and enable
  NVIC_ClearPendingIRQ(irqConf.enIRQn);
  NVIC_SetPriority(irqConf.enIRQn, priority);
  NVIC_EnableIRQ(irqConf.enIRQn);
}

//
// HAL
//

void HAL_timer_start(const timer_channel_t timer_num, const uint32_t frequency)
{
  switch (timer_num)
  {
  case STEP_TIMER_NUM:
    setupTimer(STEP_TIMER_NUM, frequency, STEPPER_TIMER_PRESCALE);
    setupTimerIRQ(STEP_TIMER_NUM, IRQn::Int000_IRQn, en_int_src_t::INT_TMR02_GCMB, DDL_IRQ_PRIORITY_01, &HAL_STEP_TIMER_ISR);
    break;
  case TEMP_TIMER_NUM:
    setupTimer(TEMP_TIMER_NUM, frequency, TEMP_TIMER_PRESCALE);
    setupTimerIRQ(TEMP_TIMER_NUM, IRQn::Int001_IRQn, en_int_src_t::INT_TMR02_GCMA, DDL_IRQ_PRIORITY_02, &HAL_TEMP_TIMER_ISR);
    break;
  }
}

void HAL_timer_enable_interrupt(const timer_channel_t timer_num)
{
  switch (timer_num)
  {
  case STEP_TIMER_NUM:
    TIMER0_Cmd(TMR_UNIT, STEP_TIMER_NUM, Enable);
    break;
  case TEMP_TIMER_NUM:
    TIMER0_Cmd(TMR_UNIT, TEMP_TIMER_NUM, Enable);
    break;
  }
}

void HAL_timer_disable_interrupt(const timer_channel_t timer_num)
{
  switch (timer_num)
  {
  case STEP_TIMER_NUM:
    TIMER0_Cmd(TMR_UNIT, STEP_TIMER_NUM, Disable);
    break;
  case TEMP_TIMER_NUM:
    TIMER0_Cmd(TMR_UNIT, TEMP_TIMER_NUM, Disable);
    break;
  }
}

bool HAL_timer_interrupt_enabled(const timer_channel_t timer_num)
{
  switch (timer_num)
  {
  case STEP_TIMER_NUM:
    return bool(TMR_UNIT->BCONR_f.CSTB);
  case TEMP_TIMER_NUM:
    return bool(TMR_UNIT->BCONR_f.CSTA);
  default:
    return false;
  }
}

hal_timer_t lastCompare = 0x0;

void HAL_timer_set_compare(const timer_channel_t timer_num, const hal_timer_t compare)
{
  if (lastCompare != compare)
  {
    lastCompare = compare;
    TIMER0_WriteCmpReg(TMR_UNIT, (en_tim0_channel_t)timer_num, compare);
  }
}

hal_timer_t HAL_timer_get_count(const timer_channel_t timer_num)
{
  return TIMER0_GetCntReg(TMR_UNIT, (en_tim0_channel_t)timer_num);
}

void HAL_timer_isr_prologue(const timer_channel_t timer_num)
{
  TIMER0_ClearFlag(M4_TMR02, (en_tim0_channel_t)timer_num);
}

void HAL_timer_isr_epilogue(const timer_channel_t timer_num) {}
