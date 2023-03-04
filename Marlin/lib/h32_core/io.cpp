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

#include "io.h"
#include "drivers/gpio/gpio.h"

uint32 digitalRead(uint8 pin)
{
	if (pin >= BOARD_NR_GPIO_PINS)
	{
		return 0;
	}

	return PORT_GetBitGPIO(pin);
}

void digitalWrite(uint8 pin, uint8 val)
{
	if (pin >= BOARD_NR_GPIO_PINS)
	{
		return;
	}

	switch (val)
	{
	case HIGH:
		PORT_SetBitsGPIO(pin);
		break;
	case LOW:
	default:
		PORT_ResetBitsGPIO(pin);
		break;
	}
}

void pwmWrite(uint8 pin, uint16 duty_cycle)
{
	// TODO stub
}

void analogWrite(uint8 pin, int duty_cycle)
{
	// TODO stub
}

uint16 analogRead(uint8 pin)
{
	if (pin > BOARD_NR_GPIO_PINS)
	{
		return 0;
	}

	// get adc device
	adc_dev *dev = PIN_MAP[pin].adc_device;
	if (dev == NULL)
	{
		return 0;
	}

	// read from adc
	return adc_read(dev, PIN_MAP[pin].adc_channel);
}

void pinMode(uint8 pin, uint8_t mode)
{
	gpio_set_mode(pin, mode);
}

void gpio_set_mode(uint8 pin, uint8_t mode)
{
	if (pin > BOARD_NR_GPIO_PINS)
	{
		return;
	}

	// build pin configuration
	stc_port_init_t pinConf;
	MEM_ZERO_STRUCT(pinConf);
	switch (mode)
	{
	case OUTPUT:
		pinConf.enPinMode = Pin_Mode_Out;
		pinConf.enExInt = Disable;
		pinConf.enPullUp = Disable;
		break;
	case INPUT:
		// case INPUT_FLOATING:
		pinConf.enPinMode = Pin_Mode_In;
		pinConf.enExInt = Disable;
		pinConf.enPullUp = Disable;
		break;
	case INPUT_ANALOG:
		pinConf.enPinMode = Pin_Mode_Ana;
		pinConf.enExInt = Disable;
		pinConf.enPullUp = Disable;
		break;
	case INPUT_PULLUP:
		pinConf.enPinMode = Pin_Mode_In;
		pinConf.enExInt = Disable;
		pinConf.enPullUp = Enable;
		break;
	default:
		return;
	}

	PORT_InitGPIO(pin, &pinConf);
}

uint8_t gpio_get_mode(uint8 pin)
{
	if (pin > BOARD_NR_GPIO_PINS)
	{
		return INPUT_FLOATING;
	}

	// get pin configuration
	stc_port_init_t pinConf;
	PORT_GetConfigGPIO(pin, &pinConf);
	switch (pinConf.enPinMode)
	{
	case Pin_Mode_Out:
		return OUTPUT;
	case Pin_Mode_In:
		return (pinConf.enPullUp == Enable) ? INPUT_PULLUP : INPUT;
	case Pin_Mode_Ana:
		return INPUT_ANALOG;
	default:
		return INPUT_FLOATING;
	}
}
