/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 * Copyright (c) 2012 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/*
 * Arduino-compatible digital I/O implementation.
 */

#include "io.h"
#include "../board/startup.h"

uint32 digitalRead(uint8 pin)
{
	if (pin >= BOARD_NR_GPIO_PINS)
	{
		return 0;
	}
	return PORT_GetBitMapp(pin);
}

void digitalWrite(uint8 pin, uint8 val)
{
	if (pin >= BOARD_NR_GPIO_PINS)
	{
		return;
	}
	if (val > 0)
	{
		PORT_SetBitsMapp(pin);
	}
	else
	{
		PORT_ResetBitsMapp(pin);
	}
}

void pwmWrite(uint8 pin, uint16 duty_cycle)
{
	DDL_ASSERT(pin < BOARD_NR_GPIO_PINS);
	if (pin >= BOARD_NR_GPIO_PINS)
	{
		return;
	}
	// timer_dev *dev = PIN_MAP[pin].timer_device;
	// uint8 cc_channel = PIN_MAP[pin].timer_channel;
	// DDL_ASSERT(dev && cc_channel);
	// timer_set_compare(dev, cc_channel, duty_cycle);
}

void analogWrite(uint8 pin, int duty_cycle8)
{
	pinMode(pin, PWM);
	pwmWrite(pin, duty_cycle8 * 257); // 257 maps 255 to 65535 (i.e 255*257 = 65535)
}

uint16 analogRead(uint8 pin)
{
	if (pin > BOARD_NR_GPIO_PINS)
		return (0);
	adc_dev *dev = PIN_MAP[pin].adc_device;
	if (dev == NULL)
	{
		return 0;
	}

	return adc_read(dev, PIN_MAP[pin].adc_channel);
}

void gpio_set_mode(uint8 pin, WiringPinMode mode)
{
	if (pin > BOARD_NR_GPIO_PINS)
		return;
	stc_port_init_t stcPortInit;
	MEM_ZERO_STRUCT(stcPortInit);
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
	default:
		return;
	}
	PORT_Init(PIN_MAP[pin].gpio_port, PIN_MAP[pin].gpio_pin, &stcPortInit);
}

WiringPinMode gpio_get_mode(uint8 PinNum)
{
	WiringPinMode crMode;
	crMode = INPUT_ANALOG;
	return (crMode);
}
