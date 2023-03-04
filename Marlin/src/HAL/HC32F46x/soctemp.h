#pragma once
#include <stdio.h>
#include "Arduino.h"

#define SOC_WARN_TEMP 75
#define SOC_CRITICAL_TEMP 90
#define SOC_TEMP_CHECK_INTERVAL 1000

class SOCTemp
{
public:
    static void init();
    static bool criticalTemperatureReached();

private:
    static bool shouldCheck();
    static uint32_t lastCheckMillis;
};

#if SOC_CRITICAL_TEMP < SOC_WARN_TEMP
#error "SOC_CRITICAL_TEMP must be higher than SOC_WARN_TEMP"
#endif
