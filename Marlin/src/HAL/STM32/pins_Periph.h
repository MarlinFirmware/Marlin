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
// Peripheral instances that can appear in the PinMap tables searched by M43, so
// it can name the function a pin is currently assigned to. Each instance macro
// is a pointer to the peripheral base, which is what PinMap::peripheral holds.
//
// Only timer, serial, SPI, I2C and ADC instances are listed. Those are the only
// PinMap tables every STM32duino variant defines, so they are the only ones that
// can be referenced without risking a link error on a part that lacks them.
//
// The CMSIS device header only defines the instances a part actually has, so
// every entry is guarded. Include this with _PNAME(N) defined.
//

// Timers
#ifdef TIM1
  _PNAME(TIM1)
#endif
#ifdef TIM2
  _PNAME(TIM2)
#endif
#ifdef TIM3
  _PNAME(TIM3)
#endif
#ifdef TIM4
  _PNAME(TIM4)
#endif
#ifdef TIM5
  _PNAME(TIM5)
#endif
#ifdef TIM6
  _PNAME(TIM6)
#endif
#ifdef TIM7
  _PNAME(TIM7)
#endif
#ifdef TIM8
  _PNAME(TIM8)
#endif
#ifdef TIM9
  _PNAME(TIM9)
#endif
#ifdef TIM10
  _PNAME(TIM10)
#endif
#ifdef TIM11
  _PNAME(TIM11)
#endif
#ifdef TIM12
  _PNAME(TIM12)
#endif
#ifdef TIM13
  _PNAME(TIM13)
#endif
#ifdef TIM14
  _PNAME(TIM14)
#endif
#ifdef TIM15
  _PNAME(TIM15)
#endif
#ifdef TIM16
  _PNAME(TIM16)
#endif
#ifdef TIM17
  _PNAME(TIM17)
#endif
#ifdef TIM20
  _PNAME(TIM20)
#endif
#ifdef TIM21
  _PNAME(TIM21)
#endif
#ifdef TIM22
  _PNAME(TIM22)
#endif
#ifdef HRTIM1
  _PNAME(HRTIM1)
#endif
#ifdef LPTIM1
  _PNAME(LPTIM1)
#endif
#ifdef LPTIM2
  _PNAME(LPTIM2)
#endif

// Serial
#ifdef USART1
  _PNAME(USART1)
#endif
#ifdef USART2
  _PNAME(USART2)
#endif
#ifdef USART3
  _PNAME(USART3)
#endif
#ifdef USART4
  _PNAME(USART4)
#endif
#ifdef USART5
  _PNAME(USART5)
#endif
#ifdef USART6
  _PNAME(USART6)
#endif
#ifdef USART7
  _PNAME(USART7)
#endif
#ifdef USART8
  _PNAME(USART8)
#endif
#ifdef USART9
  _PNAME(USART9)
#endif
#ifdef USART10
  _PNAME(USART10)
#endif
#ifdef UART4
  _PNAME(UART4)
#endif
#ifdef UART5
  _PNAME(UART5)
#endif
#ifdef UART7
  _PNAME(UART7)
#endif
#ifdef UART8
  _PNAME(UART8)
#endif
#ifdef UART9
  _PNAME(UART9)
#endif
#ifdef UART10
  _PNAME(UART10)
#endif
#ifdef LPUART1
  _PNAME(LPUART1)
#endif

// SPI / I2S
#ifdef SPI1
  _PNAME(SPI1)
#endif
#ifdef SPI2
  _PNAME(SPI2)
#endif
#ifdef SPI3
  _PNAME(SPI3)
#endif
#ifdef SPI4
  _PNAME(SPI4)
#endif
#ifdef SPI5
  _PNAME(SPI5)
#endif
#ifdef SPI6
  _PNAME(SPI6)
#endif

// I2C
#ifdef I2C1
  _PNAME(I2C1)
#endif
#ifdef I2C2
  _PNAME(I2C2)
#endif
#ifdef I2C3
  _PNAME(I2C3)
#endif
#ifdef I2C4
  _PNAME(I2C4)
#endif

// ADC
#ifdef ADC1
  _PNAME(ADC1)
#endif
#ifdef ADC2
  _PNAME(ADC2)
#endif
#ifdef ADC3
  _PNAME(ADC3)
#endif
#ifdef ADC4
  _PNAME(ADC4)
#endif
#ifdef ADC5
  _PNAME(ADC5)
#endif
#ifdef ADC
  _PNAME(ADC)
#endif
