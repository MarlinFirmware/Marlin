
#include "io.h"
#include "../board/startup.h"

void pinMode(uint8 pin, WiringPinMode mode)
{
	stc_port_init_t stcPortInit;
	MEM_ZERO_STRUCT(stcPortInit);
	if (pin >= BOARD_NR_GPIO_PINS)
	{
		return;
	}
	switch (mode)
	{
	case OUTPUT:
		stcPortInit.enPinMode = Pin_Mode_Out;
		stcPortInit.enExInt = Disable;
		stcPortInit.enPullUp = Disable;
		break;
	case INPUT:
	case INPUT_FLOATING:
		stcPortInit.enPinMode = Pin_Mode_In;
		stcPortInit.enExInt = Disable;
		stcPortInit.enPullUp = Disable;
		break;
	case INPUT_ANALOG:
		stcPortInit.enPinMode = Pin_Mode_Ana;
		stcPortInit.enExInt = Disable;
		stcPortInit.enPullUp = Disable;
		break;
	case INPUT_PULLUP:
		stcPortInit.enPinMode = Pin_Mode_In;
		stcPortInit.enExInt = Disable;
		stcPortInit.enPullUp = Enable;
		break;
	case INPUT_PULLDOWN:
		stcPortInit.enPinMode = Pin_Mode_In;
		stcPortInit.enExInt = Disable;
		stcPortInit.enPullUp = Disable;
		break;
	default:
		return;
	}
	PORT_InitMapp(pin, &stcPortInit);
}
