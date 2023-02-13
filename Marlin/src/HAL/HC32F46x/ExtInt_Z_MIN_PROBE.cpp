#include <ExtInt_Z_MIN_PROBE.h>
#include "../inc/MarlinConfig.h"
#include "endstop_interrupts.h"
#include <stdint.h>
#include "hc32_ddl.h"

static inline en_exti_ch_t exti_out_ch(uint8 pin)
{
	uint8 gpio_bit_bum = 0;
	if (pin > BOARD_NR_GPIO_PINS)
		return (en_exti_ch_t)0;
	gpio_bit_bum = PIN_MAP[pin].gpio_bit;
	switch (gpio_bit_bum)
	{
	case 0:
		return ExtiCh00;
	case 1:
		return ExtiCh01;
	case 2:
		return ExtiCh02;
	case 3:
		return ExtiCh03;
	case 4:
		return ExtiCh04;
	case 5:
		return ExtiCh05;
	case 6:
		return ExtiCh06;
	case 7:
		return ExtiCh07;
	case 8:
		return ExtiCh08;
	case 9:
		return ExtiCh09;
	case 10:
		return ExtiCh10;
	case 11:
		return ExtiCh11;
	case 12:
		return ExtiCh12;
	case 13:
		return ExtiCh13;
	case 14:
		return ExtiCh14;
	case 15:
		return ExtiCh15;
	}
	// Can't happen
	DDL_ASSERT(0);
	return (en_exti_ch_t)0;
}

void ExtInt_Z_MIN_PROBE_Callback(void)
{
	en_exti_ch_t ExitCh = exti_out_ch(Z_MIN_PROBE_PIN);
	if (Set == EXINT_IrqFlgGet(ExitCh))
	{
		/* clear int request flag */
		EXINT_IrqFlgClr(ExitCh);
		endstop_ISR();
		// buzzer.tone(1000,100);
	}
}