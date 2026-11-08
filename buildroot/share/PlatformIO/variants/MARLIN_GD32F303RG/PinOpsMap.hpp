#pragma once

#include "PinOps.hpp"

inline constexpr gpio::Output_Speed MIN_SPEED = gpio::Output_Speed::SPEED_2MHZ;
inline constexpr gpio::Output_Speed MEDIUM_SPEED = gpio::Output_Speed::SPEED_10MHZ;
inline constexpr gpio::Output_Speed HIGH_SPEED = gpio::Output_Speed::SPEED_50MHZ;
inline constexpr gpio::Output_Speed MAX_SPEED = gpio::Output_Speed::SPEED_MAX;


///////////////////////////// USART /////////////////////////////

inline constexpr USARTPinOps UART_TX_PinOps[] = {
    {usart::USART_Base::USART0_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_9,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,  gpio::Pin_Remap_Select::USART0_NO_REMAP,  0, 0)},
    {usart::USART_Base::USART1_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_2,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,  gpio::Pin_Remap_Select::USART1_NO_REMAP,  0, 0)},
    {usart::USART_Base::USART2_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_10,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,  gpio::Pin_Remap_Select::USART2_NO_REMAP,  0, 0)},
    {usart::USART_Base::UART3_BASE,     gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_10,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,  gpio::Pin_Remap_Select::NO_REMAP,         0, 0)},
    {usart::USART_Base::UART4_BASE,     gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_12,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,  gpio::Pin_Remap_Select::NO_REMAP,         0, 0)},
    {usart::USART_Base::INVALID,        gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr USARTPinOps UART_RX_PinOps[] = {
    {usart::USART_Base::USART0_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_10,     createPackedPinOps(gpio::Pin_Mode::INPUT_PULLUP,  MAX_SPEED,    gpio::Pin_Remap_Select::USART0_NO_REMAP,  0, 0)},
    {usart::USART_Base::USART1_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_3,      createPackedPinOps(gpio::Pin_Mode::INPUT_PULLUP,  MAX_SPEED,    gpio::Pin_Remap_Select::USART1_NO_REMAP,  0, 0)},
    {usart::USART_Base::USART2_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_11,     createPackedPinOps(gpio::Pin_Mode::INPUT_PULLUP,  MAX_SPEED,    gpio::Pin_Remap_Select::USART2_NO_REMAP,  0, 0)},
    {usart::USART_Base::UART3_BASE,     gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_11,     createPackedPinOps(gpio::Pin_Mode::INPUT_PULLUP,  MAX_SPEED,    gpio::Pin_Remap_Select::NO_REMAP,         0, 0)},
    {usart::USART_Base::UART4_BASE,     gpio::GPIO_Base::GPIOD_BASE,  gpio::Pin_Number::PIN_2,      createPackedPinOps(gpio::Pin_Mode::INPUT_PULLUP,  MAX_SPEED,    gpio::Pin_Remap_Select::NO_REMAP,         0, 0)},
    {usart::USART_Base::INVALID,        gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

// Unused for now
/*inline constexpr USARTPinOps UART_RTS_PinOps[] = {
    {usart::USART_Base::USART0_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_12,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,    gpio::Pin_Remap_Select::USART0_NO_REMAP,  0, 0)},
    {usart::USART_Base::USART1_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_1,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,    gpio::Pin_Remap_Select::USART1_NO_REMAP,  0, 0)},
    {usart::USART_Base::USART2_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_14,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,    gpio::Pin_Remap_Select::USART2_NO_REMAP,  0, 0)},
    {usart::USART_Base::INVALID,        gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

// Unused for now
inline constexpr USARTPinOps UART_CTS_PinOps[] = {
    {usart::USART_Base::USART0_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_11,     createPackedPinOps(gpio::Pin_Mode::INPUT_PULLUP,  MAX_SPEED,  gpio::Pin_Remap_Select::USART0_NO_REMAP,  0, 0)},
    {usart::USART_Base::USART1_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_0,      createPackedPinOps(gpio::Pin_Mode::INPUT_PULLUP,  MAX_SPEED,  gpio::Pin_Remap_Select::USART1_NO_REMAP,  0, 0)},
    {usart::USART_Base::USART2_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_13,     createPackedPinOps(gpio::Pin_Mode::INPUT_PULLUP,  MAX_SPEED,  gpio::Pin_Remap_Select::USART2_NO_REMAP,  0, 0)},
    {usart::USART_Base::INVALID,        gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};*/


///////////////////////////// TIMERS /////////////////////////////

inline constexpr TIMERPinOps TIMER_PinOps[] = {
    // TIMER0
    {timer::TIMER_Base::TIMER0_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_8,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER0_NO_REMAP,      0, 0)},
    {timer::TIMER_Base::TIMER0_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_9,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER0_NO_REMAP,      1, 0)},
    {timer::TIMER_Base::TIMER0_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_10,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER0_NO_REMAP,      2, 0)},
    {timer::TIMER_Base::TIMER0_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_11,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER0_NO_REMAP,      3, 0)},
    {timer::TIMER_Base::TIMER0_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_13,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER0_NO_REMAP,      0, 1)},
    {timer::TIMER_Base::TIMER0_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_14,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER0_NO_REMAP,      1, 1)},
    {timer::TIMER_Base::TIMER0_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_15,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER0_NO_REMAP,      2, 1)},
    {timer::TIMER_Base::TIMER0_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_1,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER0_PARTIAL_REMAP, 2, 1)},
    // TIMER1
    {timer::TIMER_Base::TIMER1_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_0,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER1_NO_REMAP,      0, 0)},
    {timer::TIMER_Base::TIMER1_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_1,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER1_NO_REMAP,      1, 0)},
    {timer::TIMER_Base::TIMER1_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_2,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER1_NO_REMAP,      2, 0)},
    {timer::TIMER_Base::TIMER1_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_15,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER1_PARTIAL_REMAP, 0, 0)},
    {timer::TIMER_Base::TIMER1_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_3,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER1_PARTIAL_REMAP, 1, 0)},
    {timer::TIMER_Base::TIMER1_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_10,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER1_PARTIAL_REMAP, 2, 0)},
    {timer::TIMER_Base::TIMER1_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_11,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER1_PARTIAL_REMAP, 3, 0)},
    // TIMER2
    {timer::TIMER_Base::TIMER2_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_6,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER2_NO_REMAP,      0, 0)},
    {timer::TIMER_Base::TIMER2_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_0,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER2_NO_REMAP,      2, 0)},
    {timer::TIMER_Base::TIMER2_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_4,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER2_PARTIAL_REMAP, 0, 0)},
    {timer::TIMER_Base::TIMER2_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_5,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER2_PARTIAL_REMAP, 1, 0)},
    {timer::TIMER_Base::TIMER2_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_6,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER2_FULL_REMAP,    0, 0)},
    {timer::TIMER_Base::TIMER2_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_7,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER2_FULL_REMAP,    1, 0)},
    {timer::TIMER_Base::TIMER2_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_9,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER2_FULL_REMAP,    3, 0)},
    // TIMER3
    {timer::TIMER_Base::TIMER3_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_6,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER3_NO_REMAP,      0, 0)},
    {timer::TIMER_Base::TIMER3_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_7,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER3_NO_REMAP,      1, 0)},
    {timer::TIMER_Base::TIMER3_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_8,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER3_NO_REMAP,      2, 0)},
    {timer::TIMER_Base::TIMER3_BASE,    gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_9,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::TIMER3_NO_REMAP,      3, 0)},
    // TIMER4
    {timer::TIMER_Base::TIMER4_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_3,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::NO_REMAP,             3, 0)},
    // TIMER7
    {timer::TIMER_Base::TIMER7_BASE,    gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_7,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::NO_REMAP,             0, 1)},
    {timer::TIMER_Base::TIMER7_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_8,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL,  MAX_SPEED,     gpio::Pin_Remap_Select::NO_REMAP,             2, 0)},
    {timer::TIMER_Base::INVALID,        gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};


///////////////////////////// ADC /////////////////////////////

inline constexpr ADCPinOps ADC_PinOps[] = {
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_0,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 0, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_1,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 1, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_2,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 2, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_3,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 3, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_4,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 4, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_5,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 5, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_6,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 6, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_7,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 7, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_0,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 8, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_1,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 9, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_0,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 10, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_1,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 11, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_2,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 12, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_3,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 13, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_4,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 14, 0)},
    {adc::ADC_Base::ADC0_BASE,  gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_5,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 15, 0)},
    {adc::ADC_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};


///////////////////////////// DAC /////////////////////////////

inline constexpr DACPinOps DAC_PinOps[] = {
    {dac::Internal_Device::DAC0_BASE,   gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_4,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 1, 0)},
    {dac::Internal_Device::DAC0_BASE,   gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_5,      createPackedPinOps(gpio::Pin_Mode::ANALOG, MAX_SPEED,   gpio::Pin_Remap_Select::NO_REMAP, 2, 0)},
    {dac::Internal_Device::INVALID,     gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};


///////////////////////////// I2C /////////////////////////////

inline constexpr uint32_t ALT_OD_I2C0_NO_REMAP_NO_CH =  createPackedPinOps(gpio::Pin_Mode::ALT_OPENDRAIN, MAX_SPEED, gpio::Pin_Remap_Select::I2C0_NO_REMAP, 0, 0);
inline constexpr uint32_t ALT_OD_I2C0_REMAP_NO_CH =     createPackedPinOps(gpio::Pin_Mode::ALT_OPENDRAIN, MAX_SPEED, gpio::Pin_Remap_Select::I2C0_REMAP, 0, 0);
inline constexpr uint32_t ALT_OD_NO_REMAP_NO_CH =       createPackedPinOps(gpio::Pin_Mode::ALT_OPENDRAIN, MAX_SPEED, gpio::Pin_Remap_Select::NO_REMAP, 0, 0);


inline constexpr I2CPinOps I2C_SDA_PinOps[] = {
    {i2c::I2C_Base::I2C0_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_7,      ALT_OD_I2C0_NO_REMAP_NO_CH},
    {i2c::I2C_Base::I2C0_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_9,      ALT_OD_I2C0_REMAP_NO_CH},
    {i2c::I2C_Base::I2C1_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_11,     ALT_OD_NO_REMAP_NO_CH},
    {i2c::I2C_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr I2CPinOps I2C_SCL_PinOps[] = {
    {i2c::I2C_Base::I2C0_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_6,      ALT_OD_I2C0_NO_REMAP_NO_CH},
    {i2c::I2C_Base::I2C0_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_8,      ALT_OD_I2C0_REMAP_NO_CH},
    {i2c::I2C_Base::I2C1_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_10,     ALT_OD_NO_REMAP_NO_CH},
    {i2c::I2C_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};


///////////////////////////// SPI /////////////////////////////

inline constexpr uint32_t ALT_PP_SPI0_NO_REMAP_NO_CH =  createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL, MAX_SPEED,     gpio::Pin_Remap_Select::SPI0_NO_REMAP, 0, 0);
inline constexpr uint32_t ALT_PP_SPI2_NO_REMAP_NO_CH =  createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL, MAX_SPEED,     gpio::Pin_Remap_Select::SPI2_NO_REMAP, 0, 0);
inline constexpr uint32_t ALT_PP_NO_REMAP_NO_CH =       createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL, MAX_SPEED,     gpio::Pin_Remap_Select::NO_REMAP, 0, 0);
inline constexpr uint32_t ALT_PP_NO_REMAP_NO_CH_MED =   createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL, MEDIUM_SPEED,  gpio::Pin_Remap_Select::NO_REMAP, 0, 0);


inline constexpr SPIPinOps SPI_MOSI_PinOps[] = {
    {spi::SPI_Base::SPI0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_7,      ALT_PP_SPI0_NO_REMAP_NO_CH},
    {spi::SPI_Base::SPI1_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_15,     ALT_PP_NO_REMAP_NO_CH},
    {spi::SPI_Base::SPI2_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_5,      ALT_PP_SPI2_NO_REMAP_NO_CH},
    {spi::SPI_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr SPIPinOps SPI_MISO_PinOps[] = {
    {spi::SPI_Base::SPI0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_6,      ALT_PP_SPI0_NO_REMAP_NO_CH},
    {spi::SPI_Base::SPI1_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_14,     ALT_PP_NO_REMAP_NO_CH},
    {spi::SPI_Base::SPI2_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_4,      ALT_PP_SPI2_NO_REMAP_NO_CH},
    {spi::SPI_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr SPIPinOps SPI_SCLK_PinOps[] = {
    {spi::SPI_Base::SPI0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_5,      ALT_PP_SPI0_NO_REMAP_NO_CH},
    {spi::SPI_Base::SPI1_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_13,     ALT_PP_NO_REMAP_NO_CH},
    {spi::SPI_Base::SPI2_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_3,      ALT_PP_SPI2_NO_REMAP_NO_CH},
    {spi::SPI_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr SPIPinOps SPI_SSEL_PinOps[] = {
    {spi::SPI_Base::SPI0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_4,      ALT_PP_SPI0_NO_REMAP_NO_CH},
    {spi::SPI_Base::SPI1_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_12,     ALT_PP_NO_REMAP_NO_CH},
    {spi::SPI_Base::SPI2_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_15,     ALT_PP_SPI2_NO_REMAP_NO_CH},
    {spi::SPI_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};


///////////////////////////// CAN /////////////////////////////

//inline constexpr CANPinOps CAN_RD_PinOps[] = {
//  {can::CAN_Base::CAN0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_11,     createPackedPinOps(gpio::Pin_Mode::INPUT_FLOATING, MAX_SPEED,    gpio::Pin_Remap_Select::CAN_NO_REMAP     0, 0)},
//  {can::CAN_Base::CAN0_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_8,      createPackedPinOps(gpio::Pin_Mode::INPUT_FLOATING, MAX_SPEED,    gpio::Pin_Remap_Select::CAN_FULL_REMAP,  0, 0)},
//  {can::CAN_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
//};

//inline constexpr CANPinOps CAN_TD_PinOps[] = {
//  {can::CAN_Base::CAN0_BASE,  gpio::GPIO_Base::GPIOA_BASE,  gpio::Pin_Number::PIN_12,     createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL, MAX_SPEED,  gpio::Pin_Remap_Select::CAN_NO_REMAP,      0, 0)},
//  {can::CAN_Base::CAN0_BASE,  gpio::GPIO_Base::GPIOB_BASE,  gpio::Pin_Number::PIN_9,      createPackedPinOps(gpio::Pin_Mode::ALT_PUSHPULL, MAX_SPEED,  gpio::Pin_Remap_Select::CAN_FULL_REMAP,    0, 0)},
//  {can::CAN_Base::INVALID,    gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
//};


///////////////////////////// SDIO /////////////////////////////

inline constexpr SDIOPinOps SD_DATA0_PinOps[] = {
    {sdio::SDIO_Base::SDIO_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_8,      ALT_PP_NO_REMAP_NO_CH_MED},
    {sdio::SDIO_Base::INVALID,      gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr SDIOPinOps SD_DATA1_PinOps[] = {
    {sdio::SDIO_Base::SDIO_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_9,      ALT_PP_NO_REMAP_NO_CH_MED},
    {sdio::SDIO_Base::INVALID,      gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr SDIOPinOps SD_DATA2_PinOps[] = {
    {sdio::SDIO_Base::SDIO_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_10,     ALT_PP_NO_REMAP_NO_CH_MED},
    {sdio::SDIO_Base::INVALID,      gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr SDIOPinOps SD_DATA3_PinOps[] = {
    {sdio::SDIO_Base::SDIO_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_11,     ALT_PP_NO_REMAP_NO_CH_MED},
    {sdio::SDIO_Base::INVALID,      gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr SDIOPinOps SD_CK_PinOps[] = {
    {sdio::SDIO_Base::SDIO_BASE,    gpio::GPIO_Base::GPIOC_BASE,  gpio::Pin_Number::PIN_12,     ALT_PP_NO_REMAP_NO_CH_MED},
    {sdio::SDIO_Base::INVALID,      gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};

inline constexpr SDIOPinOps SD_CMD_PinOps[] = {
    {sdio::SDIO_Base::SDIO_BASE,    gpio::GPIO_Base::GPIOD_BASE,  gpio::Pin_Number::PIN_2,      ALT_PP_NO_REMAP_NO_CH_MED},
    {sdio::SDIO_Base::INVALID,      gpio::GPIO_Base::INVALID,     gpio::Pin_Number::INVALID,    0}
};
