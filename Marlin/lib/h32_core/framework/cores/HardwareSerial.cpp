
#include "../board/startup.h"
#include "../pins/hc32f46x/pins_Aquila_X2.h"
#include "HardwareSerial.h"
#include "libmaple.h"
#include "usart.h"

HardwareSerial::HardwareSerial(usart_dev *usart_device,
							   uint8 tx_pin,
							   uint8 rx_pin)
{
	this->usart_device = usart_device;
	this->tx_pin = tx_pin;
	this->rx_pin = rx_pin;
}

void HardwareSerial::init(usart_dev *usart_device,
						  uint8_t tx_pin,
						  uint8_t rx_pin)
{
	this->usart_device = usart_device;
	this->tx_pin = tx_pin;
	this->rx_pin = rx_pin;
}

void HardwareSerial::begin(uint32 baud)
{
	begin(baud, 0);
}

void HardwareSerial::begin(uint32 baud, uint8_t config)
{
	if (baud > this->usart_device->max_baud)
	{
		return;
	}
	/* Initialize USART IO */
	PORT_SetFuncMapp(this->tx_pin, Disable);
	PORT_SetFuncMapp(this->rx_pin, Disable);
	usart_init(this->usart_device);
	usart_set_baud_rate(this->usart_device, baud);
	usart_enable(this->usart_device);
}

void HardwareSerial::end(void)
{
	usart_disable(this->usart_device);
}

int __O0 HardwareSerial::read(void)
{
	if (usart_data_available(usart_device) > 0)
	{
		return usart_getc(usart_device);
	}
	else
	{
		return -1;
	}
}

int HardwareSerial::available(void)
{
	return usart_data_available(this->usart_device);
}

/* Roger Clark. Added function missing from LibMaple code */

int __O0 HardwareSerial::peek(void)
{
	return usart_peek(this->usart_device);
}

int __O0 HardwareSerial::availableForWrite(void)
{
	return this->usart_device->wb->size - rb_full_count(this->usart_device->wb);
}

size_t __O0 HardwareSerial::write(unsigned char ch)
{
	usart_putc(this->usart_device, ch);
	return 1;
}

/* edogaldo: Waits for the transmission of outgoing serial data to complete (Arduino 1.0 api specs) */
void HardwareSerial::flush(void)
{
	while (!rb_is_empty(this->usart_device->wb))
		; // wait for TX buffer empty
}
