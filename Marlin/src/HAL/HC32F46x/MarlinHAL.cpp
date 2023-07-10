/**
 * HAL for HC32F46x, based heavily on the legacy implementation and STM32F1
 */
#ifdef TARGET_HC32F46x

#include "HAL.h"
#include <IWatchdog.h>
#include <OnChipTemperature.h>
#include <AsyncAnalogRead.h>
#include "../../inc/MarlinConfig.h"

extern "C" char *_sbrk(int incr);

pin_t MarlinHAL::last_adc_pin;

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
    WDT.begin(&wdtConf);
#endif
}

void MarlinHAL::watchdog_refresh()
{
#if ENABLED(USE_WATCHDOG)
    WDT.reload();
#endif
}

void MarlinHAL::init()
{
    NVIC_SetPriorityGrouping(0x3);

    // print clock frequencies to host serial
    SERIAL_LEAF_1.print("-- clocks dump -- \n");
    SERIAL_LEAF_1.print(SYSTEM_CLOCK_FREQUENCIES.system);
    SERIAL_LEAF_1.print(" ; ");
    SERIAL_LEAF_1.print(SYSTEM_CLOCK_FREQUENCIES.hclk);
    SERIAL_LEAF_1.print(" ; ");
    SERIAL_LEAF_1.print(SYSTEM_CLOCK_FREQUENCIES.pclk0);
    SERIAL_LEAF_1.print(" ; ");
    SERIAL_LEAF_1.print(SYSTEM_CLOCK_FREQUENCIES.pclk1);
    SERIAL_LEAF_1.print(" ; ");
    SERIAL_LEAF_1.print(SYSTEM_CLOCK_FREQUENCIES.pclk2);
    SERIAL_LEAF_1.print(" ; ");
    SERIAL_LEAF_1.print(SYSTEM_CLOCK_FREQUENCIES.pclk3);
    SERIAL_LEAF_1.print(" ; ");
    SERIAL_LEAF_1.print(SYSTEM_CLOCK_FREQUENCIES.pclk4);
    SERIAL_LEAF_1.print(" ; ");
    SERIAL_LEAF_1.print(SYSTEM_CLOCK_FREQUENCIES.exclk);
    SERIAL_LEAF_1.print(" ; F_CPU=");
    SERIAL_LEAF_1.print(F_CPU);
    SERIAL_LEAF_1.print("\n");

    // start OTS, min. 1s between reads
    ChipTemperature.begin();
    ChipTemperature.setMinimumReadDeltaMillis(1000);
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
    float temp;
    if (ChipTemperature.read(temp))
    {
        // print SoC temperature on every read
        // char tempStr[10];
        // dtostrf(temp, 8, 1, tempStr);
        // printf("SoC temperature is %sC\n", tempStr);

        // warn after reaching 60C
        if (temp > 60)
        {
            char tempStr[10];
            dtostrf(temp, 8, 1, tempStr);
            printf("SoC temperature %s is above 60C", tempStr);
        }

        // panic after reaching 80C
        if (temp > 80)
        {
            printf("SoC overheat! temperature is > 80C");
            MarlinHAL::reboot();
        }
    }
}

uint8_t MarlinHAL::get_reset_source()
{
    // query reset cause from RMU
    stc_rmu_rstcause_t rstCause;
    RMU_GetResetCause(&rstCause);

    // map reset cause code to those expected by Marlin
    // - reset causes are flags, so multiple can be set
    printf("-- Reset Cause -- \n");
    uint8_t cause = 0;
#define MAP_CAUSE(from, to)                 \
    if (rstCause.from == Set)               \
    {                                       \
        printf(" - " STRINGIFY(from) "\n"); \
        cause |= to;                        \
    }

    // power on
    MAP_CAUSE(enPowerOn, RST_POWER_ON) // power on reset

    // external
    MAP_CAUSE(enRstPin, RST_EXTERNAL) // reset pin
    MAP_CAUSE(enPvd1, RST_EXTERNAL)   // program voltage detection reset
    MAP_CAUSE(enPvd2, RST_EXTERNAL)   // "

    // brown out
    MAP_CAUSE(enBrownOut, RST_BROWN_OUT) // brown out reset

    // wdt
    MAP_CAUSE(enWdt, RST_WATCHDOG)  // Watchdog reset
    MAP_CAUSE(enSwdt, RST_WATCHDOG) // Special WDT reset

    // software
    MAP_CAUSE(enPowerDown, RST_SOFTWARE) // MCU power down (?)
    MAP_CAUSE(enSoftware, RST_SOFTWARE)  // software reset (e.g. NVIC_SystemReset())

    // misc.
    MAP_CAUSE(enMpuErr, RST_BACKUP)       // MPU error
    MAP_CAUSE(enRamParityErr, RST_BACKUP) // RAM parity error
    MAP_CAUSE(enRamEcc, RST_BACKUP)       // RAM ecc error
    MAP_CAUSE(enClkFreqErr, RST_BACKUP)   // clock frequency failure
    MAP_CAUSE(enXtalErr, RST_BACKUP)      // XTAL failure

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
    // just set pin mode to analog
    pinMode(pin, INPUT_ANALOG);
}

void MarlinHAL::adc_start(const pin_t pin)
{
    CORE_ASSERT(IS_GPIO_PIN(pin), "adc_start: invalid pin")
    MarlinHAL::last_adc_pin = pin;

    analogReadAsync(pin);
}

bool MarlinHAL::adc_ready()
{
    CORE_ASSERT(IS_GPIO_PIN(MarlinHAL::last_adc_pin), "adc_ready: invalid pin")

    return getAnalogReadComplete(MarlinHAL::last_adc_pin);
}

uint16_t MarlinHAL::adc_value()
{
    // read conversion result
    CORE_ASSERT(IS_GPIO_PIN(MarlinHAL::last_adc_pin), "adc_value: invalid pin")

    return getAnalogReadValue(MarlinHAL::last_adc_pin);
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
