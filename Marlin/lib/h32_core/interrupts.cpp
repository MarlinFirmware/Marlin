#include "interrupts.h"
#include "drivers/gpio/gpio.h"
#include "hdsc/common/hc32_ddl.h"
#include "io.h"

//
// Util
//
inline en_exti_lvl_t mapToTriggerMode(uint32_t mode)
{
    switch (mode)
    {
    case RISING:
        return ExIntRisingEdge;
    case FALLING:
        return ExIntFallingEdge;
    case CHANGE:
        return ExIntBothEdge;
    case LOW:
        return ExIntLowLevel;
    }

    DDL_ASSERT(0);
    return ExIntFallingEdge;
}

inline en_exti_ch_t mapToExternalInterruptChannel(uint32_t pin)
{
    // check range
    if (pin > BOARD_NR_GPIO_PINS)
    {
        return ExtiCh00;
    }

    // map to exti channel
    uint8_t ch = PIN_MAP[pin].gpio_bit;
    if (ch < 0 || ch > 15)
    {
        // invalid channel, only 00 - 15 are valid
        DDL_ASSERT(0);
        return ExtiCh00;
    }

    // exti channel maps 1-to-1
    return (en_exti_ch_t)ch;
}

inline en_int_src_t mapToInterruptSource(uint32_t pin)
{
    // check range
    if (pin > BOARD_NR_GPIO_PINS)
    {
        return INT_PORT_EIRQ0;
    }

    // map to interrupt source
    uint8_t ch = PIN_MAP[pin].gpio_bit;
    if (ch < 0 || ch > 15)
    {
        // invalid source, only 00 - 15 are valid
        DDL_ASSERT(0);
        return INT_PORT_EIRQ0;
    }

    // interrupt source maps 1-to-1
    return (en_int_src_t)ch;
}

inline IRQn_Type mapToIQRVector(uint8_t n)
{
    // map 0-10 to IRQ 10-20
    uint8_t vec = FIRST_IRQn + n;
    DDL_ASSERT(vec >= FIRST_IRQn && vec <= LAST_IRQn);
    return (IRQn_Type)vec;
}

void _attachInterrupt(uint32_t pin, isr_handler_t handler, uint8_t irqNum, uint32_t mode)
{
    // check inputs
    if (pin >= BOARD_NR_GPIO_PINS || !handler)
    {
        return;
    }

    // initialize external interrupt channel
    stc_exint_config_t extiConf = {
        .enExitCh = mapToExternalInterruptChannel(pin),
        .enFilterEn = Enable,
        .enFltClk = Pclk3Div8,
        .enExtiLvl = mapToTriggerMode(mode)};
    EXINT_Init(&extiConf);

    // configure port for external interrupt
    stc_port_init_t portConf;
    MEM_ZERO_STRUCT(portConf);
    portConf.enExInt = Enable;
    PORT_InitGPIO(pin, &portConf);

    // register IRQ
    stc_irq_regi_conf_t irqReg = {
        .enIntSrc = mapToInterruptSource(pin),
        .enIRQn = mapToIQRVector(irqNum),
        .pfnCallback = handler};
    enIrqRegistration(&irqReg);

    // clear pending, set priority and enable
    NVIC_ClearPendingIRQ(irqReg.enIRQn);
    NVIC_SetPriority(irqReg.enIRQn, DDL_IRQ_PRIORITY_06);
    NVIC_EnableIRQ(irqReg.enIRQn);
}

void _detachInterrupt(uint32_t pin, uint8_t irqNum)
{
    // check inputs
    if (pin >= BOARD_NR_GPIO_PINS)
    {
        return;
    }

    // configure port to disable external interrupt
    stc_port_init_t portConf;
    MEM_ZERO_STRUCT(portConf);
    portConf.enExInt = Disable;
    PORT_InitGPIO(pin, &portConf);

    // clear pending and disable IRQ
    IRQn_Type irqVec = mapToIQRVector(irqNum);
    NVIC_ClearPendingIRQ(irqVec);
    NVIC_EnableIRQ(irqVec);
}

//
// IRQn auto-assignment
//
#define IRQn_MAPPING_NONE 0xff

/**
 * maps IRQn to pin number using that IRQn
 * [IRQn] = <pin_nr>|IRQn_MAPPING_NONE
 */
static uint32_t pinToIRQnMapping[AVAILABLE_IRQn_COUNT];

/**
 * get the next IRQn that is available for use
 */
inline bool getNextFreeIRQn(uint8_t &irqn)
{
    // find the first free IRQn
    for (int i = 0; i < AVAILABLE_IRQn_COUNT; i++)
    {
        if (pinToIRQnMapping[i] == IRQn_MAPPING_NONE)
        {
            irqn = i;
            return true;
        }
    }

    // if we got here, no more IRQns are available...
    return false;
}

/**
 * get the IRQn that is assigned to a pin's interrupt.
 */
inline bool getIRQnForPin(uint32_t pin, uint8_t &irqn)
{
    // linear search the pin in the mapping
    for (int i = 0; i < AVAILABLE_IRQn_COUNT; i++)
    {
        if (pinToIRQnMapping[i] == pin)
        {
            irqn = i;
            return true;
        }
    }

    // if we got here, no IRQn is assigned to the pin...
    return false;
}

/**
 * assign a IRQn to a pin
 */
inline void assignIRQn(uint32_t pin, uint8_t irqn)
{
    DDL_ASSERT(pinToIRQnMapping[irqn] == IRQn_MAPPING_NONE);
    pinToIRQnMapping[irqn] = pin;
}

/**
 * clear the assignment of a IRQn to a pin
 */
inline void clearIRQnAssignment(uint32_t pin, uint8_t irqn)
{
    DDL_ASSERT(pinToIRQnMapping[irqn] == pin);
    pinToIRQnMapping[irqn] = IRQn_MAPPING_NONE;
}

//
// Public API
//
void attachInterrupt(uint32_t pin, isr_handler_t callback, uint32_t mode)
{
    // auto-assign a irqn
    uint8_t irqn;
    if (!getNextFreeIRQn(irqn))
    {
        DDL_ASSERT(0);
        return;
    }

    // set the assignment
    assignIRQn(pin, irqn);

    // set the interrupt
    _attachInterrupt(pin, callback, irqn, mode);
}

void detachInterrupt(uint32_t pin)
{
    // get IRQn for the pin
    uint8_t irqn;
    if (!getIRQnForPin(pin, irqn))
    {
        // DDL_ASSERT(0);
        return;
    }

    // remove the interrupt
    _detachInterrupt(pin, irqn);

    // clear irqn assignment
    clearIRQnAssignment(pin, irqn);
}

bool checkAndClearExtIRQFlag(uint32_t pin)
{
    en_exti_ch_t ch = mapToExternalInterruptChannel(pin);
    if (EXINT_IrqFlgGet(ch) == Set)
    {
        EXINT_IrqFlgClr(ch);
        return true;
    }

    return false;
}

void interrupts_init()
{
    // set all IRQn assignments to NONE
    for (int i = 0; i < AVAILABLE_IRQn_COUNT; i++)
    {
        pinToIRQnMapping[i] = IRQn_MAPPING_NONE;
    }
}
