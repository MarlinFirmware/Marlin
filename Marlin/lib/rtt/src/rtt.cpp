

#include "rtt.h"
#include "Arduino.h"
#include "SEGGER_RTT.h"


RttLog::RttLog()
{
    //nothing to do.
}

// RttLog::~RttLog()
// {
//     //nothing to do.
// }


size_t RttLog::write(uint8_t c)
{
    return SEGGER_RTT_PutChar(0, c);
    // return 1;
}

static int a;
void RttLog::flush(void)
{
    a++;
    //nothing.
}

int RttLog::available(void)
{
  return 0;
}

int RttLog::peek(void)
{

}

int RttLog::read(void)
{

}

RttLog rtt;