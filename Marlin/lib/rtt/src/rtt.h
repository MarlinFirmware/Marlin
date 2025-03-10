
#pragma once

#include <inttypes.h>

#include "Stream.h"
// #include "uart.h"
#include "SEGGER_RTT.h"

class RttLog : public Stream {
  protected:
    // Has any byte been written to the rtt since begin()
    bool _written;

    // Don't put any members after these buffers, since only the first
    // 32 bytes of this struct can be accessed quickly using the ldd
    // instruction.
    // unsigned char _rx_buffer[256];
    // unsigned char _tx_buffer[256];

  public:
    RttLog();

    void begin(unsigned long baud)
    {
      begin(baud);
    }
    void begin(unsigned long, uint8_t);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
      return write((uint8_t)n);
    }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool()
    {
      return true;
    }
};

extern RttLog rtt;