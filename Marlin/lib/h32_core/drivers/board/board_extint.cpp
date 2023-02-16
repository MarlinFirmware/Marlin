#define _BOARD_EXTINT_C_

#include "startup.h"
#include "../../HAL/HC32F46x/endstop_interrupts.h"

void ExtInt05_Callback(void)
{
	if (Set == EXINT_IrqFlgGet(ExtiCh05))
	{
		/* clear int request flag */
		EXINT_IrqFlgClr(ExtiCh05);
		// printf("ExtInt05_Callback\r\n");
	}
}
void ExtInt06_Callback(void)
{
	if (Set == EXINT_IrqFlgGet(ExtiCh06))
	{
		/* clear int request flag */
		EXINT_IrqFlgClr(ExtiCh06);
		// printf("ExtInt06_Callback\r\n");
	}
}

static void __O0 set_extinterrupt(uint8_t PinNum, en_exti_ch_t ExitCh, en_exti_lvl_t edge,
								  en_int_src_t IntSrc, IRQn_Type IRQn, func_ptr_t Callback)
{
	stc_exint_config_t stcExtiConfig;
	stc_irq_regi_conf_t stcIrqRegiConf;
	stc_port_init_t stcPortInit;

	/* configuration structure initialization */
	MEM_ZERO_STRUCT(stcExtiConfig);
	MEM_ZERO_STRUCT(stcIrqRegiConf);
	MEM_ZERO_STRUCT(stcPortInit);
	/**************************************************************************/
	/* External Int Ch.3                                                      */
	/**************************************************************************/
	stcExtiConfig.enExitCh = ExitCh;

	/* Filter setting */
	stcExtiConfig.enFilterEn = Enable;
	stcExtiConfig.enFltClk = Pclk3Div8;
	/* Both edge */
	stcExtiConfig.enExtiLvl = edge;
	EXINT_Init(&stcExtiConfig);

	/* Set PD03 as External Int Ch.3 input */
	MEM_ZERO_STRUCT(stcPortInit);
	stcPortInit.enExInt = Enable;
	PORT_InitMapp(PinNum, &stcPortInit);

	/* Select External Int Ch.3 */
	stcIrqRegiConf.enIntSrc = IntSrc;

	/* Register External Int to Vect.No.000 */
	stcIrqRegiConf.enIRQn = IRQn;

	/* Callback function */
	stcIrqRegiConf.pfnCallback = Callback;

	/* Registration IRQ */
	enIrqRegistration(&stcIrqRegiConf);

	/* Clear pending */
	NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);

	/* Set priority */
	NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_06);

	/* Enable NVIC */
	NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

static inline en_exti_lvl_t exti_out_mode(ExtIntTriggerMode mode)
{
	switch (mode)
	{
	case RISING:
		return ExIntRisingEdge;
	case FALLING:
		return ExIntFallingEdge;
	case CHANGE:
		return ExIntBothEdge;
	}
	// Can't happen
	DDL_ASSERT(0);
	return (en_exti_lvl_t)0;
}

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

static inline en_int_src_t exti_out_int_port(uint8 pin)
{
	uint8 gpio_bit_bum = 0;
	if (pin > BOARD_NR_GPIO_PINS)
		return (en_int_src_t)0;
	gpio_bit_bum = PIN_MAP[pin].gpio_bit;
	switch (gpio_bit_bum)
	{
	case 0:
		return INT_PORT_EIRQ0;
	case 1:
		return INT_PORT_EIRQ1;
	case 2:
		return INT_PORT_EIRQ2;
	case 3:
		return INT_PORT_EIRQ3;
	case 4:
		return INT_PORT_EIRQ4;
	case 5:
		return INT_PORT_EIRQ5;
	case 6:
		return INT_PORT_EIRQ6;
	case 7:
		return INT_PORT_EIRQ7;
	case 8:
		return INT_PORT_EIRQ8;
	case 9:
		return INT_PORT_EIRQ9;
	case 10:
		return INT_PORT_EIRQ10;
	case 11:
		return INT_PORT_EIRQ11;
	case 12:
		return INT_PORT_EIRQ12;
	case 13:
		return INT_PORT_EIRQ13;
	case 14:
		return INT_PORT_EIRQ14;
	case 15:
		return INT_PORT_EIRQ15;
	}
	// Can't happen
	DDL_ASSERT(0);
	return (en_int_src_t)0;
}

/* number:0->10 -->Int010_IRQn->Int020_IRQn*/
static inline IRQn_Type exti_out_IRQn_Type(uint8 number)
{
	return (IRQn_Type)(Int010_IRQn + number);
}

extern void __O0 attachInterrupt(uint8 pin, voidFuncPtr handler, uint8 irqNum, ExtIntTriggerMode mode)
{
	if (pin >= BOARD_NR_GPIO_PINS || !handler)
	{
		return;
	}
	en_exti_lvl_t edge = exti_out_mode(mode);
	en_exti_ch_t ExitCh = exti_out_ch(pin);
	en_int_src_t IntSrc = exti_out_int_port(pin);
	IRQn_Type IRQn = exti_out_IRQn_Type(irqNum);
	set_extinterrupt(pin, ExitCh, edge, IntSrc, IRQn, handler);
}

extern void setup_Extinterrupt(void)
{
	// attachInterrupt(EXTINT1, ExtInt05_Callback, 9, FALLING);
	// attachInterrupt(EXTINT2, ExtInt06_Callback, 10, FALLING);
}

extern void ExtInt_X_MIN_Callback(void)
{
	en_exti_ch_t ExitCh = exti_out_ch(X_MIN_PIN);
	if (Set == EXINT_IrqFlgGet(ExitCh))
	{
		/* clear int request flag */
		EXINT_IrqFlgClr(ExitCh);
		endstop_ISR();
	}
}

extern void ExtInt_Y_MIN_Callback(void)
{
	en_exti_ch_t ExitCh = exti_out_ch(Y_MIN_PIN);
	if (Set == EXINT_IrqFlgGet(ExitCh))
	{
		/* clear int request flag */
		EXINT_IrqFlgClr(ExitCh);
		endstop_ISR();
	}
}

extern void ExtInt_Z_MIN_Callback(void)
{
	en_exti_ch_t ExitCh = exti_out_ch(Z_MIN_PIN);
	if (Set == EXINT_IrqFlgGet(ExitCh))
	{
		/* clear int request flag */
		EXINT_IrqFlgClr(ExitCh);
		endstop_ISR();
	}
}

#undef _BOARD_EXTINT_C_
