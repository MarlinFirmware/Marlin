/**
 * HAL for HC32F46x, based heavily on the legacy implementation and STM32F1
 */
#ifdef TARGET_HC32F46x

#include "HAL.h"
#include "hc32f46x_wdt.h"
#include "../../inc/MarlinConfig.h"
#include "soctemp.h"

extern "C" char *_sbrk(int incr);

enum TEMP_PINS
{
    TEMP_BED = 14,
    TEMP_0 = 15,
};

uint16_t MarlinHAL::adc_result;

MarlinHAL::MarlinHAL() {}

void MarlinHAL::watchdog_init()
{
#if ENABLED(USE_WATCHDOG)
    stc_wdt_init_t wdtConf = {
        .enCountCycle = WdtCountCycle65536,
        .enClkDiv = WdtPclk3Div8192,
        .enRefreshRange = WdtRefresh100Pct,
        .enSleepModeCountEn = Disable,
        .enRequestType = WdtTriggerResetRequest};
    WDT_Init(&wdtConf);
    WDT_RefreshCounter();
#endif
}

void MarlinHAL::watchdog_refresh()
{
#if ENABLED(USE_WATCHDOG)
    WDT_RefreshCounter();
#endif
}

void MarlinHAL::init()
{
    NVIC_SetPriorityGrouping(0x3);
    SOCTemp::init();
}

void MarlinHAL::init_board() {}

void MarlinHAL::reboot()
{
    NVIC_SystemReset();
}

bool MarlinHAL::isr_state()
{
    return !__get_PRIMASK();
}

void MarlinHAL::isr_on()
{
    __enable_irq();
}

void MarlinHAL::isr_off()
{
    __disable_irq();
}

void MarlinHAL::delay_ms(const int ms)
{
    delay(ms);
}

void MarlinHAL::idletask()
{
    MarlinHAL::watchdog_refresh();

    // monitor SOC temperature
    if(SOCTemp::criticalTemperatureReached())
    {
        printf("SoC reached critical temperature, rebooting\n");
        MarlinHAL::reboot();
    }
}

uint8_t MarlinHAL::get_reset_source()
{
    // query reset cause
    stc_rmu_rstcause_t rstCause;
    MEM_ZERO_STRUCT(rstCause);
    RMU_GetResetCause(&rstCause);

    // map reset causes to those expected by Marlin
    uint8_t cause = 0;
#define MAP_CAUSE(from, to)                                \
    if (rstCause.from == Set)                              \
    {                                                      \
        printf("GetResetCause " STRINGIFY(from) " set\n"); \
        cause |= to;                                       \
    }

    // external
    MAP_CAUSE(enRstPin, RST_EXTERNAL)
    MAP_CAUSE(enPvd1, RST_EXTERNAL)
    MAP_CAUSE(enPvd2, RST_EXTERNAL)

    // brown out
    MAP_CAUSE(enBrownOut, RST_BROWN_OUT)

    // wdt
    MAP_CAUSE(enWdt, RST_WATCHDOG)
    MAP_CAUSE(enSwdt, RST_WATCHDOG)

    // software
    MAP_CAUSE(enPowerDown, RST_SOFTWARE)
    MAP_CAUSE(enSoftware, RST_SOFTWARE)

    // other
    MAP_CAUSE(enMpuErr, RST_BACKUP)
    MAP_CAUSE(enRamParityErr, RST_BACKUP)
    MAP_CAUSE(enRamEcc, RST_BACKUP)
    MAP_CAUSE(enClkFreqErr, RST_BACKUP)
    MAP_CAUSE(enXtalErr, RST_BACKUP)

    // power on
    MAP_CAUSE(enPowerOn, RST_POWER_ON)
    return cause;
}

void MarlinHAL::clear_reset_source()
{
    RMU_ClrResetFlag();
}

int MarlinHAL::freeMemory()
{
    volatile char top;
    return &top - _sbrk(0);
}

void MarlinHAL::adc_init() {}

void MarlinHAL::adc_enable(const pin_t pin)
{
    pinMode(pin, INPUT_ANALOG);
}

void MarlinHAL::adc_start(const pin_t pin)
{
    TEMP_PINS pin_index;
    switch (pin)
    {
    default:
    case TEMP_0_PIN:
        pin_index = TEMP_0;
        break;
    case TEMP_BED_PIN:
        pin_index = TEMP_BED;
        break;
    }

    MarlinHAL::adc_result = adc_read(ADC1, (uint8_t)pin_index);
}

bool MarlinHAL::adc_ready()
{
    return true;
}

uint16_t MarlinHAL::adc_value()
{
    return MarlinHAL::adc_result;
}

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t a, const bool b)
{
    // TODO stub
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired)
{
    // TODO stub
}

void flashFirmware(const int16_t) { MarlinHAL::reboot(); }

#endif // TARGET_HC32F46x
