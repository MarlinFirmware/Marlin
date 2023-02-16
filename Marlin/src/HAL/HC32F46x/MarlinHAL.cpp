/**
 * HAL for HC32F46x, based heavily on the legacy implementation and STM32F1
 */
#ifdef TARGET_HC32F46x

#include "HAL.h"
#include "../../inc/MarlinConfig.h"
#include "../cores/iwdg.h"

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
    //  increase WDT timeout, see https://github.com/alexqzd/Marlin-H32/commit/d8483fdff9d582e6773b002a1730052718787c3a
    stc_wdt_init_t wdt_config;
    wdt_config.enCountCycle = WdtCountCycle65536;      ///< Count cycle
    wdt_config.enClkDiv = WdtPclk3Div8192;             ///< Count clock division
    wdt_config.enRefreshRange = WdtRefresh100Pct;      ///< Allow refresh percent range
    wdt_config.enSleepModeCountEn = Disable;           ///< Enable/disable count in the sleep mode
    wdt_config.enRequsetType = WdtTriggerResetRequest; ///< Refresh error or count underflow trigger event type
    WDT_Init(&wdt_config);
#endif
}

void MarlinHAL::watchdog_refresh()
{
#if ENABLED(USE_WATCHDOG)
    iwdg_feed();
#endif
}

void MarlinHAL::init()
{
    NVIC_SetPriorityGrouping(0x3);
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
}

uint8_t MarlinHAL::get_reset_source()
{
    return /* RST_POWER_ON */ 1;
}

void MarlinHAL::clear_reset_source() {}

int MarlinHAL::freeMemory()
{
    // TODO HC32F46x: untested, taken from STM32F1 HAL
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
