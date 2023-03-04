#include "soctemp.h"
#include "../ots/ots.h"

#define _MK_STRING(M) #M
#define MK_STRING(M) _MK_STRING(M)
#define SOC_WARN_TEMP_STR MK_STRING(SOC_WARN_TEMP)

uint32_t SOCTemp::lastCheckMillis = 0;

void SOCTemp::init()
{
    H32OTS::init();
}

bool SOCTemp::criticalTemperatureReached()
{
    if (shouldCheck())
    {
        float temperature = -1;
        if (H32OTS::read(temperature))
        {
            // print warning using printf
            if (temperature >= SOC_WARN_TEMP)
            {
                char buf[10];
                dtostrf(temperature, 8, 1, buf);
                printf("SoC temperature %s C is above warning treshold " SOC_WARN_TEMP_STR " C\n", buf);
            }

            // check critical temperature
            if (temperature >= SOC_CRITICAL_TEMP)
            {
                return true;
            }
        }
    }

    return false;
}

bool SOCTemp::shouldCheck()
{
    uint32_t now = millis();
    if (now > (lastCheckMillis + SOC_TEMP_CHECK_INTERVAL))
    {
        lastCheckMillis = now;
        return true;
    }

    return false;
}
