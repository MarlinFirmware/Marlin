#ifndef USBserial_h_
#define USBserial_h_

#include <inttypes.h>

#include "Stream.h"

class usb_serial_class : public Stream
{
public:
	void begin(long);
	void end();
	virtual int available();
	virtual int read();
	virtual int peek();
	virtual void flush();
#if ARDUINO >= 100
	virtual size_t write(uint8_t c)		{ return write(&c, 1); }
	virtual size_t write(const uint8_t *buffer, uint16_t size);
	using Print::write;
#else
	virtual void write(uint8_t c)		{ write(&c, 1); }
	virtual void write(const uint8_t *buffer, uint16_t size);
	virtual void write(const char *s)	{ write((const uint8_t *)s, strlen(s)); }
#endif
	void send_now(void);
	uint32_t baud(void);
	uint8_t stopbits(void);
	uint8_t paritytype(void);
	uint8_t numbits(void);
	uint8_t dtr(void);
	uint8_t rts(void);
	operator bool();
#if ARDUINO >= 100
	size_t readBytes(char *buffer, size_t length);
#endif
private:
	int16_t peek_buf;
};

extern usb_serial_class Serial;

#endif

