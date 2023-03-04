#pragma once
#include "libmaple_types.h"
#include "Print.h"
#include "Stream.h"

#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 64
typedef uint8_t tx_buffer_index_t;
typedef uint8_t rx_buffer_index_t;

struct usart_dev;

#define DEFINE_HWSERIAL(name, n)                 \
    HardwareSerial name(USART##n,                \
                        BOARD_USART##n##_TX_PIN, \
                        BOARD_USART##n##_RX_PIN)

class HardwareSerial : public Stream
{

public:
    HardwareSerial(struct usart_dev *usart_device,
                   uint8 tx_pin,
                   uint8 rx_pin);

    void init(usart_dev *usart_device,
              uint8_t tx_pin,
              uint8_t rx_pin);
    void begin(uint32 baud);
    void begin(uint32 baud, uint8_t config);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    int availableForWrite(void);
    virtual void flush(void);
    size_t write(unsigned char ch);

    int txPin(void) { return this->tx_pin; }
    int rxPin(void) { return this->rx_pin; }

    operator bool() { return true; }

    // Escape hatch into libmaple
    struct usart_dev *c_dev(void) { return this->usart_device; }

private:
    struct usart_dev *usart_device;
    uint8 tx_pin;
    uint8 rx_pin;

protected:
public:
    size_t putStr(const char *str);
    size_t putStr(const void *buf, uint32 len);

    size_t print(const String &);
    size_t print(char);
    size_t print(const char[]);
    size_t print(uint8, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(long long, int = DEC);
    size_t print(unsigned long long, int = DEC);
    size_t print(double, int = 2);
    size_t print(const __FlashStringHelper *);
    size_t println(void);
    size_t println(const String &s);
    size_t println(char);
    size_t println(const char[]);
    size_t println(uint8, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(long long, int = DEC);
    size_t println(unsigned long long, int = DEC);
    size_t println(double, int = 2);
    size_t println(const __FlashStringHelper *);
#ifdef SUPPORTS_PRINTF
    // Roger Clark. Work in progress to add printf support
    int printf(const char *format, ...);
#endif

    int getWriteError()
    {
        return write_error;
    }
    void clearWriteError() { setWriteError(0); }

protected:
    void setWriteError(int err = 1) { write_error = err; }

private:
    int write_error;
    size_t printNumber(unsigned long long, uint8);
    size_t printFloat(double, uint8);
};
