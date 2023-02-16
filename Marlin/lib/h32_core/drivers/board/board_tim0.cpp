#define _BSP_TIM0_C_

#include "startup.h"
#include "../HAL/HC32F46x/timers.h"
#include "../module/temperature.h"
#include "../module/stepper.h"
#include "../../HAL/HC32F46x/endstop_interrupts.h"

extern void HAL_STEP_TIMER_ISR(void);
extern void HAL_TEMP_TIMER_ISR(void);

/***************************************************/
extern void setup_systick(void)
{
	stc_clk_freq_t stcClkFreq;
	MEM_ZERO_STRUCT(stcClkFreq);
	CLK_GetClockFreq(&stcClkFreq);
	SysTick_Config(stcClkFreq.sysclkFreq / OS_TICKS_PER_SEC);
}
volatile uint32_t systick_uptime_millis = 0;
extern void SysTickIrq_Handler(void)
{
	++systick_uptime_millis;
}

extern uint32_t millis(void)
{
	return (systick_uptime_millis);
}
/***************************************************/
/***************************************************/

/*
static uint16_t systick_uptime_frequency=1000ul;
timer set compare(M4_TMR02, Tim0_ChannelB,1000);
*/
extern en_result_t timer_preset_compare(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh, const uint16_t compare, en_functional_state_t counterclr)
{
#if 0
	uint32_t u32Pclk1;
	stc_clk_freq_t stcClkTmp;
	en_result_t rt=Error;
	uint8_t err_cnt=0;
	//if(compare==HAL_TIMER_TYPE_MAX)return(ErrorInvalidParameter);
	/* Get pclk1 */    
	CLK_GetClockFreq(&stcClkTmp);	
	u32Pclk1 = stcClkTmp.pclk1Freq;// 84MHZ
	
	uint16_t PRESCALE = (enCh==Tim0_ChannelA)?TEMP_TIMER_PRESCALE:STEPPER_TIMER_PRESCALE;
	uint16_t cnt=(enCh==Tim0_ChannelA)?(u32Pclk1/(PRESCALE*compare)):compare;
	if(counterclr==Enable)
	{
		do{
			rt = TIMER0_WriteCntReg(pstcTim0Reg, enCh, 0);
			if(rt==Ok)break;
			if(++err_cnt>=5)
			{
				err_cnt = 0;
				goto exit;
			}
		}while(Ok != rt);
	}
	rt = TIMER0_WriteCmpReg(pstcTim0Reg, enCh,cnt);
	exit:
	//printf("HAL_timer_set_compare->[freq=%u->%s],rt=%u,err_cnt=%u\r\n",compare,(rt==Ok) ?"succ":"fail",rt,err_cnt);  
	return(rt);
#else
	TIMER0_WriteCmpReg(pstcTim0Reg, enCh, compare);
	return (Ok);
#endif
}

extern en_result_t timer_set_compare(const uint8_t timer_num, const uint16_t compare, en_functional_state_t setCounterZero)
{
	en_tim0_channel_t timer_channel;
	timer_channel = (timer_num == STEP_TIMER_NUM) ? Tim0_ChannelB : Tim0_ChannelA;
	return timer_preset_compare(TMR_UNIT, timer_channel, compare, setCounterZero);
}

extern uint16_t timer_get_count(const uint8_t timer_num)
{
	en_tim0_channel_t timer_channel;
	timer_channel = (timer_num == STEP_TIMER_NUM) ? Tim0_ChannelB : Tim0_ChannelA;
	return (TIMER0_GetCntReg(TMR_UNIT, timer_channel));
}

extern void timer_enable_irq(const uint8_t timer_num)
{
	en_tim0_channel_t timer_channel;
	timer_channel = (timer_num == STEP_TIMER_NUM) ? Tim0_ChannelB : Tim0_ChannelA;
	TIMER0_Cmd(TMR_UNIT, timer_channel, Enable);
}

extern void timer_disable_irq(const uint8_t timer_num)
{
	en_tim0_channel_t timer_channel;
	timer_channel = (timer_num == STEP_TIMER_NUM) ? Tim0_ChannelB : Tim0_ChannelA;
	TIMER0_Cmd(TMR_UNIT, timer_channel, Disable);
}

extern bool timer_irq_enabled(M4_TMR0_TypeDef *pstcTim0Reg, const uint8_t timer_num)
{
	switch (timer_num)
	{
	case STEP_TIMER_NUM:
		return bool(pstcTim0Reg->BCONR_f.CSTB);
	case TEMP_TIMER_NUM:
		return bool(pstcTim0Reg->BCONR_f.CSTA);
	}
	return false;
}

void stepTC_Handler(void)
{
	TIMER0_ClearFlag(M4_TMR02, Tim0_ChannelB);
	HAL_STEP_TIMER_ISR();
}

void Z_limit_query(void)
{
	static bool pe11test = false;
	if (pe11test != PORT_GetBitMapp(Z_MIN_PIN))
	{
		pe11test = PORT_GetBitMapp(Z_MIN_PIN);
		endstop_ISR();
	}
}

void tempTC_Handler(void)
{
	TIMER0_ClearFlag(M4_TMR02, Tim0_ChannelA);
	// Z_limit_query();
	HAL_TEMP_TIMER_ISR();
}
/***************************************************/
extern void __O0 setup_time2A(const uint32_t frequency)
{
	stc_tim0_base_init_t stcTimerCfg;
	stc_irq_regi_conf_t stcIrqRegiConf;
	uint32_t u32Pclk1;
	stc_clk_freq_t stcClkTmp;

	MEM_ZERO_STRUCT(stcTimerCfg);
	MEM_ZERO_STRUCT(stcIrqRegiConf);

	/*****************************************************************************/
	/* Get pclk1 */
	CLK_GetClockFreq(&stcClkTmp);
	u32Pclk1 = stcClkTmp.pclk1Freq; // 84MHZ

	/* Timer0 peripheral enable */
	PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM02, Enable);

	/*config register for channel A */
	stcTimerCfg.Tim0_CounterMode = Tim0_Sync;
	stcTimerCfg.Tim0_SyncClockSource = Tim0_Pclk1;
	stcTimerCfg.Tim0_ClockDivision = Tim0_ClkDiv16;
	stcTimerCfg.Tim0_CmpValue = (uint16_t)(((u32Pclk1 / TEMP_TIMER_PRESCALE) / frequency) - 1);
	TIMER0_BaseInit(TMR_UNIT, Tim0_ChannelA, &stcTimerCfg);
	/* Enable channel A interrupt */
	TIMER0_IntCmd(TMR_UNIT, Tim0_ChannelA, Enable);
	/* Register TMR_INI_GCMA Int to Vect.No.002 */
	stcIrqRegiConf.enIRQn = Int001_IRQn;
	/* Select I2C Error or Event interrupt function */
	stcIrqRegiConf.enIntSrc = TMR_INI_GCMA;
	/* Callback function */
	stcIrqRegiConf.pfnCallback = &tempTC_Handler;
	/* Registration IRQ */
	enIrqRegistration(&stcIrqRegiConf);
	/* Clear Pending */
	NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
	/* Set priority */
	NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_02);
	/* Enable NVIC */
	NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
	/*start timer0*/
	// TIMER0_Cmd(TMR_UNIT,Tim0_ChannelA,Enable);
}

extern void __O0 setup_time2B(const uint32_t frequency)
{
	stc_tim0_base_init_t stcTimerCfg;
	stc_irq_regi_conf_t stcIrqRegiConf;
	uint32_t u32Pclk1;
	stc_clk_freq_t stcClkTmp;

	MEM_ZERO_STRUCT(stcTimerCfg);
	MEM_ZERO_STRUCT(stcIrqRegiConf);

	/*****************************************************************************/
	/* Get pclk1 */
	CLK_GetClockFreq(&stcClkTmp);
	u32Pclk1 = stcClkTmp.pclk1Freq; // 84MHZ

	/* Timer0 peripheral enable */
	PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM02, Enable);

	/*config register for channel B */
	stcTimerCfg.Tim0_CounterMode = Tim0_Sync;
	stcTimerCfg.Tim0_SyncClockSource = Tim0_Pclk1;
	stcTimerCfg.Tim0_ClockDivision = Tim0_ClkDiv16;
	stcTimerCfg.Tim0_CmpValue = (uint16_t)((u32Pclk1 / STEPPER_TIMER_PRESCALE) / frequency);
	TIMER0_BaseInit(TMR_UNIT, Tim0_ChannelB, &stcTimerCfg);
	/* Enable channel B interrupt */
	TIMER0_IntCmd(TMR_UNIT, Tim0_ChannelB, Enable);
	/* Register TMR_INI_GCMB Int to Vect.No.002 */
	stcIrqRegiConf.enIRQn = Int000_IRQn;
	/* Select I2C Error or Event interrupt function */
	stcIrqRegiConf.enIntSrc = TMR_INI_GCMB;
	/* Callback function */
	stcIrqRegiConf.pfnCallback = &stepTC_Handler;
	/* Registration IRQ */
	enIrqRegistration(&stcIrqRegiConf);
	/* Clear Pending */
	NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
	/* Set priority */
	NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_01);
	/* Enable NVIC */
	NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
	/*start timer0*/
	// TIMER0_Cmd(TMR_UNIT,Tim0_ChannelB,Enable);
}

extern void setup_time(void)
{
	// setup_time2A(FREQ_1000HZ);
	// setup_time2B(FREQ_122HZ);
}

#undef _BSP_TIM0_C_

/*************************end of file***************************************/
