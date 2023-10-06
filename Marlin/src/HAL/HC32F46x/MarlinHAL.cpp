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

void HAL_wdt_callback()
{
    panic("WDT timeout");
    NVIC_SystemReset();
}

inline void HAL_clock_frequencies_dump()
{
    // 1. dump all clock frequencies
    update_system_clock_frequencies();
    SERIAL_ECHOPGM("-- clocks dump -- \nSYS=");
    SERIAL_ECHO(SYSTEM_CLOCK_FREQUENCIES.system);
    SERIAL_ECHOPGM("\nHCLK=");
    SERIAL_ECHO(SYSTEM_CLOCK_FREQUENCIES.hclk);
    SERIAL_ECHOPGM("\nPCLK0=");
    SERIAL_ECHO(SYSTEM_CLOCK_FREQUENCIES.pclk0);
    SERIAL_ECHOPGM("\nPCLK1=");
    SERIAL_ECHO(SYSTEM_CLOCK_FREQUENCIES.pclk1);
    SERIAL_ECHOPGM("\nPCLK2=");
    SERIAL_ECHO(SYSTEM_CLOCK_FREQUENCIES.pclk2);
    SERIAL_ECHOPGM("\nPCLK3=");
    SERIAL_ECHO(SYSTEM_CLOCK_FREQUENCIES.pclk3);
    SERIAL_ECHOPGM("\nPCLK4=");
    SERIAL_ECHO(SYSTEM_CLOCK_FREQUENCIES.pclk4);
    SERIAL_ECHOPGM("\nEXCLK=");
    SERIAL_ECHO(SYSTEM_CLOCK_FREQUENCIES.exclk);
    SERIAL_ECHOPGM("\nF_CPU=");
    SERIAL_ECHO(F_CPU);

    // 2. dump current system clock source
    en_clk_sys_source_t clkSrc = CLK_GetSysClkSource();
    SERIAL_ECHOPGM("\nSYSCLK=");
    switch (clkSrc)
    {
    case ClkSysSrcHRC:
        SERIAL_ECHOPGM("HRC");
        break;
    case ClkSysSrcMRC:
        SERIAL_ECHOPGM("MRC");
        break;
    case ClkSysSrcLRC:
        SERIAL_ECHOPGM("LRC");
        break;
    case ClkSysSrcXTAL:
        SERIAL_ECHOPGM("XTAL");
        break;
    case ClkSysSrcXTAL32:
        SERIAL_ECHOPGM("XTAL32");
        break;
    case CLKSysSrcMPLL:
        SERIAL_ECHOPGM("MPLL");

        // 3. if MPLL is used, dump MPLL settings:
        // (derived from CLK_SetPllSource and CLK_MpllConfig)
        // source
        switch (M4_SYSREG->CMU_PLLCFGR_f.PLLSRC)
        {
        case ClkPllSrcXTAL:
            SERIAL_ECHOPGM(",XTAL");
            break;
        case ClkPllSrcHRC:
            SERIAL_ECHOPGM(",HRC");
            break;
        default:
            break;
        }

        // PLL multiplies and dividers
        SERIAL_ECHOPGM("\nP=");
        SERIAL_ECHO(M4_SYSREG->CMU_PLLCFGR_f.MPLLP + 1ul);
        SERIAL_ECHOPGM("\nQ=");
        SERIAL_ECHO(M4_SYSREG->CMU_PLLCFGR_f.MPLLQ + 1ul);
        SERIAL_ECHOPGM("\nR=");
        SERIAL_ECHO(M4_SYSREG->CMU_PLLCFGR_f.MPLLR + 1ul);
        SERIAL_ECHOPGM("\nN=");
        SERIAL_ECHO(M4_SYSREG->CMU_PLLCFGR_f.MPLLN + 1ul);
        SERIAL_ECHOPGM("\nM=");
        SERIAL_ECHO(M4_SYSREG->CMU_PLLCFGR_f.MPLLM + 1ul);
        break;
    default:
        break;
    }

    // done
    SERIAL_ECHOPGM("\n--\n");
}

//
// MarlinHAL class implementation
//

pin_t MarlinHAL::last_adc_pin;

MarlinHAL::MarlinHAL() {}

void MarlinHAL::watchdog_init()
{
#if ENABLED(USE_WATCHDOG)
    // 5s timeout, panic on timeout
    WDT.begin(5000, &HAL_wdt_callback);
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
    HAL_clock_frequencies_dump();

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
    // #define HAL_ALWAYS_PRINT_SOC_TEMP
    float temp;
    if (ChipTemperature.read(temp))
    {
#ifdef HAL_ALWAYS_PRINT_SOC_TEMP
        // print SoC temperature on every read
        char tempStr[10];
        dtostrf(temp, 8, 1, tempStr);
        printf("SoC temperature is %sC\n", tempStr);
#endif

        // warn after reaching 60C
        if (temp > 60)
        {
#ifndef HAL_ALWAYS_PRINT_SOC_TEMP
            char tempStr[10];
            dtostrf(temp, 8, 1, tempStr);
#endif
            printf("SoC temperature %s is above 60C\n", tempStr);
        }

        // panic after reaching 80C
        if (temp > 80)
        {
            printf("SoC overheat! temperature is > 80C\n");
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

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t value, const uint16_t scale, const bool invert)
{
    // invert value if requested
    uint16_t val = invert ? scale - value : value;

    // analogWrite the value, core handles the rest
    // pin mode should be set by Marlin by calling SET_PWM() before calling this function
    analogWriteScaled(pin, val, scale);
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired)
{
    // TODO stub
    printf("set_pwm_frequency is not implemented yet\n");
}

void flashFirmware(const int16_t) { MarlinHAL::reboot(); }

#endif // TARGET_HC32F46x
