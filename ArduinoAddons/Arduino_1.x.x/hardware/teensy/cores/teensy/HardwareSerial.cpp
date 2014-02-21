/* UART (hardware serial) for Teensy & Teensy++
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "core_pins.h"
#include "HardwareSerial.h"

#define RX_BUFFER_SIZE 64
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t rx_buffer_head = 0;
static volatile uint8_t rx_buffer_tail = 0;

#define TX_BUFFER_SIZE 40
static volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t tx_buffer_head = 0;
static volatile uint8_t tx_buffer_tail = 0;
static volatile uint8_t transmitting = 0;
static volatile uint8_t tx_enable_pin = 255;

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::_begin(uint16_t baud_count, uint8_t txen_pin)
{
	tx_enable_pin = txen_pin;
	if (txen_pin < 255) {
		pinMode(txen_pin, OUTPUT);
		digitalWrite(txen_pin, LOW);
	}
	if ((baud_count & 1) && baud_count <= 4096) {
		UCSR1A = (1<<U2X1);
		UBRR1 = baud_count - 1;
	} else {
		UCSR1A = 0;
		UBRR1 = (baud_count >> 1) - 1;
	}
	if (!(UCSR1B & (1<<TXEN1))) {
		rx_buffer_head = 0;
		rx_buffer_tail = 0;
		tx_buffer_head = 0;
		tx_buffer_tail = 0;
		transmitting = 0;
		UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
		UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
	}
}

void HardwareSerial::end(void) 
{
	while (transmitting) ;	// wait for buffered data to send
	UCSR1B = 0;
	rx_buffer_head = 0;
	rx_buffer_tail = 0;
}

int HardwareSerial::available(void)
{
	uint8_t head, tail;

	head = rx_buffer_head;
	tail = rx_buffer_tail;
	if (head >= tail) return head - tail;
	return RX_BUFFER_SIZE + head - tail;
}

int HardwareSerial::peek(void)
{
	uint8_t head, tail;

	head = rx_buffer_head;
	tail = rx_buffer_tail;
	if (head == tail) return -1;
	return rx_buffer[tail];
}

int HardwareSerial::read(void)
{
	uint8_t c, i;

	if (rx_buffer_head == rx_buffer_tail) return -1;
	i = rx_buffer_tail + 1;
	if (i >= RX_BUFFER_SIZE) i = 0;
	c = rx_buffer[i];
	rx_buffer_tail = i;
	return c;
}

void HardwareSerial::flush()
{
#if ARDUINO >= 100
	while (transmitting) ;	// wait for buffered data to send
#else
	rx_buffer_head = rx_buffer_tail;
#endif
}

void HardwareSerial::clear()
{
	rx_buffer_head = rx_buffer_tail;
}

#if ARDUINO >= 100
size_t HardwareSerial::write(uint8_t c)
#else
void HardwareSerial::write(uint8_t c)
#endif
{
	uint8_t i;

	if (!(UCSR1B & (1<<TXEN1))) {
#if ARDUINO >= 100
		setWriteError();
		return 0;
#else
		return;
#endif
	}
	if (tx_enable_pin < 255 && !transmitting) {
		digitalWrite(tx_enable_pin, HIGH);
	}
	i = tx_buffer_head + 1;
	if (i >= TX_BUFFER_SIZE) i = 0;
	while (tx_buffer_tail == i) ; // wait until space in buffer
	tx_buffer[i] = c;
	transmitting = 1;
	tx_buffer_head = i;
	UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);
#if ARDUINO >= 100
	return 1;
#endif
}

ISR(USART1_RX_vect)
{
	uint8_t c, i;

	c = UDR1;
	i = rx_buffer_head + 1;
	if (i >= RX_BUFFER_SIZE) i = 0;
	if (i != rx_buffer_tail) {
		rx_buffer[i] = c;
		rx_buffer_head = i;
	}
}

ISR(USART1_UDRE_vect)
{
	uint8_t i;

	if (tx_buffer_head == tx_buffer_tail) {
		// buffer is empty, disable transmit interrupt
		UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
	} else {
		i = tx_buffer_tail + 1;
		if (i >= TX_BUFFER_SIZE) i = 0;
		UDR1 = tx_buffer[i];
		tx_buffer_tail = i;
	}
}

ISR(USART1_TX_vect)
{
	transmitting = 0;
	if (tx_enable_pin < 255) {
		digitalWrite(tx_enable_pin, LOW);
	}
}

// Preinstantiate Objects //////////////////////////////////////////////////////

HardwareSerial Serial1;



