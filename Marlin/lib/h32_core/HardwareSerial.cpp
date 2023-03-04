#include "HardwareSerial.h"
#include "libmaple.h"
#include "usart.h"
#include "drivers/gpio/gpio.h"

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
	// init IO pins
	PORT_SetFuncGPIO(this->tx_pin, Disable);
	PORT_SetFuncGPIO(this->rx_pin, Disable);
	usart_init(this->usart_device);
	usart_set_baud_rate(this->usart_device, baud);
	usart_enable(this->usart_device);
}

void HardwareSerial::end(void)
{
	usart_disable(this->usart_device);
}

int HardwareSerial::read(void)
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

int HardwareSerial::peek(void)
{
	return usart_peek(this->usart_device);
}

int HardwareSerial::availableForWrite(void)
{
	return this->usart_device->wb->capacity() - this->usart_device->wb->count();
}

size_t HardwareSerial::write(unsigned char ch)
{
	usart_putc(this->usart_device, ch);
	return 1;
}

void HardwareSerial::flush(void)
{
	while (!this->usart_device->wb->isEmpty())
		;
}
