#pragma once

#ifndef OTS_USE_XTAL
#define OTS_USE_XTAL 1
#endif

#ifndef OTS_READ_TIMEOUT
#define OTS_READ_TIMEOUT 10
#endif 

class H32OTS
{
public:
    static void init();
    static bool read(float &temperature);
};
