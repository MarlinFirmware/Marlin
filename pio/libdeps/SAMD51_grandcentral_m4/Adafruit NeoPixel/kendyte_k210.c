// This is a mash-up of the Due show() code + insights from Michael Miller's
// ESP8266 work for the NeoPixelBus library: github.com/Makuna/NeoPixelBus
// Needs to be a separate .c file to enforce ICACHE_RAM_ATTR execution.
#if defined(K210) 
#define KENDRYTE_K210 1
#endif

#if defined(KENDRYTE_K210)

#include <Arduino.h>
#include "sysctl.h"

void  k210Show(
    uint8_t pin, uint8_t *pixels, uint32_t numBytes, boolean is800KHz)
{

#define CYCLES_800_T0H (sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 2500000) // 0.4us
#define CYCLES_800_T1H (sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 1250000) // 0.8us
#define CYCLES_800 (sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 800000)      // 1.25us per bit
#define CYCLES_400_T0H (sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 2000000) // 0.5uS
#define CYCLES_400_T1H (sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 833333)  // 1.2us
#define CYCLES_400 (sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 400000)      // 2.5us per bit

    uint8_t *p, *end, pix, mask;
    uint32_t t, time0, time1, period, c, startTime;

    p = pixels;
    end = p + numBytes;
    pix = *p++;
    mask = 0x80;
    startTime = 0;

#ifdef NEO_KHZ400
    if (is800KHz)
    {
#endif
        time0 = CYCLES_800_T0H;
        time1 = CYCLES_800_T1H;
        period = CYCLES_800;
#ifdef NEO_KHZ400
    }
    else
    { // 400 KHz bitstream
        time0 = CYCLES_400_T0H;
        time1 = CYCLES_400_T1H;
        period = CYCLES_400;
    }
#endif

    for (t = time0;; t = time0)
    {
        if (pix & mask)
            t = time1; // Bit high duration
        while (((c = read_cycle()) - startTime) < period)
            ; // Wait for bit start
        digitalWrite(pin, HIGH);
        startTime = c; // Save start time
        while (((c = read_cycle()) - startTime) < t)
            ; // Wait high duration
        digitalWrite(pin, LOW);

        if (!(mask >>= 1))
        { // Next bit/byte
            if (p >= end)
                break;
            pix = *p++;
            mask = 0x80;
        }
    }
    while ((read_cycle() - startTime) < period)
        ; // Wait for last bit
}

#endif // KENDRYTE_K210
