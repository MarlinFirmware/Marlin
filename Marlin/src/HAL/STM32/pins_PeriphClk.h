/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

//
// RCC clock-enable bit for each peripheral that can appear in a PinMap table.
//
// STM32F1 has no alternate-function multiplexer, so several peripherals can claim
// the same pin in the default mapping and the GPIO registers cannot tell them
// apart - PA9 is both USART1_TX and TIM1_CH2. Whether a peripheral is clocked is
// what separates them. Include this with _PCLK1/_PCLK2 defined for the APB1 and
// APB2 buses.
//
// APB2
#if defined(TIM1) && defined(RCC_APB2ENR_TIM1EN)
  _PCLK2(TIM1, TIM1EN)
#endif
#if defined(TIM8) && defined(RCC_APB2ENR_TIM8EN)
  _PCLK2(TIM8, TIM8EN)
#endif
#if defined(TIM9) && defined(RCC_APB2ENR_TIM9EN)
  _PCLK2(TIM9, TIM9EN)
#endif
#if defined(TIM10) && defined(RCC_APB2ENR_TIM10EN)
  _PCLK2(TIM10, TIM10EN)
#endif
#if defined(TIM11) && defined(RCC_APB2ENR_TIM11EN)
  _PCLK2(TIM11, TIM11EN)
#endif
#if defined(TIM15) && defined(RCC_APB2ENR_TIM15EN)
  _PCLK2(TIM15, TIM15EN)
#endif
#if defined(TIM16) && defined(RCC_APB2ENR_TIM16EN)
  _PCLK2(TIM16, TIM16EN)
#endif
#if defined(TIM17) && defined(RCC_APB2ENR_TIM17EN)
  _PCLK2(TIM17, TIM17EN)
#endif
#if defined(USART1) && defined(RCC_APB2ENR_USART1EN)
  _PCLK2(USART1, USART1EN)
#endif
#if defined(SPI1) && defined(RCC_APB2ENR_SPI1EN)
  _PCLK2(SPI1, SPI1EN)
#endif
#if defined(ADC1) && defined(RCC_APB2ENR_ADC1EN)
  _PCLK2(ADC1, ADC1EN)
#endif
#if defined(ADC2) && defined(RCC_APB2ENR_ADC2EN)
  _PCLK2(ADC2, ADC2EN)
#endif
#if defined(ADC3) && defined(RCC_APB2ENR_ADC3EN)
  _PCLK2(ADC3, ADC3EN)
#endif

// APB1
#if defined(TIM2) && defined(RCC_APB1ENR_TIM2EN)
  _PCLK1(TIM2, TIM2EN)
#endif
#if defined(TIM3) && defined(RCC_APB1ENR_TIM3EN)
  _PCLK1(TIM3, TIM3EN)
#endif
#if defined(TIM4) && defined(RCC_APB1ENR_TIM4EN)
  _PCLK1(TIM4, TIM4EN)
#endif
#if defined(TIM5) && defined(RCC_APB1ENR_TIM5EN)
  _PCLK1(TIM5, TIM5EN)
#endif
#if defined(TIM6) && defined(RCC_APB1ENR_TIM6EN)
  _PCLK1(TIM6, TIM6EN)
#endif
#if defined(TIM7) && defined(RCC_APB1ENR_TIM7EN)
  _PCLK1(TIM7, TIM7EN)
#endif
#if defined(TIM12) && defined(RCC_APB1ENR_TIM12EN)
  _PCLK1(TIM12, TIM12EN)
#endif
#if defined(TIM13) && defined(RCC_APB1ENR_TIM13EN)
  _PCLK1(TIM13, TIM13EN)
#endif
#if defined(TIM14) && defined(RCC_APB1ENR_TIM14EN)
  _PCLK1(TIM14, TIM14EN)
#endif
#if defined(USART2) && defined(RCC_APB1ENR_USART2EN)
  _PCLK1(USART2, USART2EN)
#endif
#if defined(USART3) && defined(RCC_APB1ENR_USART3EN)
  _PCLK1(USART3, USART3EN)
#endif
#if defined(UART4) && defined(RCC_APB1ENR_UART4EN)
  _PCLK1(UART4, UART4EN)
#endif
#if defined(UART5) && defined(RCC_APB1ENR_UART5EN)
  _PCLK1(UART5, UART5EN)
#endif
#if defined(SPI2) && defined(RCC_APB1ENR_SPI2EN)
  _PCLK1(SPI2, SPI2EN)
#endif
#if defined(SPI3) && defined(RCC_APB1ENR_SPI3EN)
  _PCLK1(SPI3, SPI3EN)
#endif
#if defined(I2C1) && defined(RCC_APB1ENR_I2C1EN)
  _PCLK1(I2C1, I2C1EN)
#endif
#if defined(I2C2) && defined(RCC_APB1ENR_I2C2EN)
  _PCLK1(I2C2, I2C2EN)
#endif
#if defined(DAC) && defined(RCC_APB1ENR_DACEN)
  _PCLK1(DAC, DACEN)
#endif

