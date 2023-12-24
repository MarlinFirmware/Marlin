/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef __AVR__

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(HAL_AVR_DIRTY_INIT)

#include "registers.h"

// Since the compiler could be creating multiple copies of function code-graphs for each header inline-inclusion,
// we want to off-load the function definitions that define static memory into this solitary compilation unit.
// This way the ROM is NOT bloated (who knows if the compiler is optimizing same-content constant objects into one?)

ATmegaPinFunctions _ATmega_getPinFunctions(int pin) {
  if (pin < 0) return {};

  ATmegaPinInfo info = _ATmega_getPinInfo((unsigned int)pin);

  #ifdef __AVR_TRM01__
    if (info.port == eATmegaPort::PORT_A) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD0 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_B) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC0A, eATmegaPinFunc::TOC1C, eATmegaPinFunc::PCI7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1B, eATmegaPinFunc::PCI6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1A, eATmegaPinFunc::PCI5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC2A, eATmegaPinFunc::PCI4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::PCI3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::PCI2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::PCI1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_CS, eATmegaPinFunc::PCI0 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_C) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD15 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD14 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD13 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD12 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD11 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD10 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD9 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD8 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_D) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER0_CLKI };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_CLKI };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART1_CLK };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_ICP };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT3, eATmegaPinFunc::USART1_TXD };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT2, eATmegaPinFunc::USART1_RXD };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT1, eATmegaPinFunc::TWI_SDA };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT0, eATmegaPinFunc::TWI_CLK };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_E) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT7, eATmegaPinFunc::TIMER3_ICP, eATmegaPinFunc::CLKO };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT6, eATmegaPinFunc::TIMER3_CLKI };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT5, eATmegaPinFunc::TOC3C };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT4, eATmegaPinFunc::TOC3B };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::AIN1, eATmegaPinFunc::TOC3A };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::AIN0, eATmegaPinFunc::USART0_CLK };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PDO, eATmegaPinFunc::USART0_TXD };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PDI, eATmegaPinFunc::USART0_RXD, eATmegaPinFunc::PCI8 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_F) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC0 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_G) {
      if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC0B };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOSC1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3 ) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOSC2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_ALE };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_RD };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_WR };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_H) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER4_CLKI };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC2B };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC4C };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC4B };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC4A };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART2_CLK };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART2_TXD };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART2_RXD };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_J) {
      if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI15 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI14 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI13 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI12 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART3_CLK, eATmegaPinFunc::PCI11 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART3_TXD, eATmegaPinFunc::PCI10 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART3_RXD, eATmegaPinFunc::PCI9 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_K) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC15, eATmegaPinFunc::PCI23 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC14, eATmegaPinFunc::PCI22 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC13, eATmegaPinFunc::PCI21 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC12, eATmegaPinFunc::PCI20 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC11, eATmegaPinFunc::PCI19 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC10, eATmegaPinFunc::PCI18 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC9, eATmegaPinFunc::PCI17 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC8, eATmegaPinFunc::PCI16 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_L) {
      if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC5C };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC5B };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC5A };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER5_CLKI };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER5_ICP };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER4_ICP };
        return { funcs, countof(funcs) };
      }
    }
  #elif defined(__AVR_TRM02__)
    if (info.port == eATmegaPort::PORT_A) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI7, eATmegaPinFunc::ADC7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI6, eATmegaPinFunc::ADC6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI5, eATmegaPinFunc::ADC5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI4, eATmegaPinFunc::ADC4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI3, eATmegaPinFunc::ADC3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI2, eATmegaPinFunc::ADC2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI1, eATmegaPinFunc::ADC1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI0, eATmegaPinFunc::ADC0 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_B) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::TOC3B, eATmegaPinFunc::PCI15 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::TOC3A, eATmegaPinFunc::PCI14 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::TIMER3_ICP, eATmegaPinFunc::PCI13 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_CS, eATmegaPinFunc::TOC0B, eATmegaPinFunc::PCI12 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::AIN1, eATmegaPinFunc::TOC0A, eATmegaPinFunc::PCI11 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::AIN0, eATmegaPinFunc::EINT2, eATmegaPinFunc::PCI10 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_ECI, eATmegaPinFunc::CLKO, eATmegaPinFunc::PCI9 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER0_ECI, eATmegaPinFunc::USART0_CLK, eATmegaPinFunc::PCI8 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_C) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOSC2, eATmegaPinFunc::PCI23 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOSC1, eATmegaPinFunc::PCI22 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI21 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI20 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI19 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI18 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI17, eATmegaPinFunc::TWI_SDA };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TWI_CLK, eATmegaPinFunc::PCI16 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_D) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC2A, eATmegaPinFunc::PCI31 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_ICP, eATmegaPinFunc::TOC2B, eATmegaPinFunc::PCI30 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1A, eATmegaPinFunc::PCI29 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1B, eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::PCI28 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT1, eATmegaPinFunc::USART1_TXD, eATmegaPinFunc::PCI27 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT0, eATmegaPinFunc::USART1_RXD, eATmegaPinFunc::PCI26 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART0_TXD, eATmegaPinFunc::PCI25 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART0_TXD, eATmegaPinFunc::PCI24, eATmegaPinFunc::TIMER3_ECI };
        return { funcs, countof(funcs) };
      }
    }
  #elif defined(__AVR_TRM03__)
    if (info.port == eATmegaPort::PORT_B) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::XTAL2, eATmegaPinFunc::TOSC2, eATmegaPinFunc::PCI7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::XTAL1, eATmegaPinFunc::TOSC1, eATmegaPinFunc::PCI6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::PCI5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::PCI4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::TOC2A, eATmegaPinFunc::PCI3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_CS, eATmegaPinFunc::TOC1B, eATmegaPinFunc::PCI2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1A, eATmegaPinFunc::PCI1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_ICP, eATmegaPinFunc::CLKO, eATmegaPinFunc::PCI0 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_C) {
      if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI14 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC5, eATmegaPinFunc::TWI_CLK, eATmegaPinFunc::PCI13 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC4, eATmegaPinFunc::TWI_SDA, eATmegaPinFunc::PCI12 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC3, eATmegaPinFunc::PCI11 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC2, eATmegaPinFunc::PCI10 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC1, eATmegaPinFunc::PCI9 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC0, eATmegaPinFunc::PCI8 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_D) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::AIN1, eATmegaPinFunc::PCI23 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::AIN0, eATmegaPinFunc::TOC0A, eATmegaPinFunc::PCI22 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_ECI, eATmegaPinFunc::TOC0B, eATmegaPinFunc::PCI21 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART_CLK, eATmegaPinFunc::TIMER0_ECI, eATmegaPinFunc::PCI20 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT1, eATmegaPinFunc::TOC2B, eATmegaPinFunc::PCI19 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT0, eATmegaPinFunc::PCI18 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART_TXD, eATmegaPinFunc::PCI17 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART_RXD, eATmegaPinFunc::PCI16 };
        return { funcs, countof(funcs) };
      }
    }
  #elif defined(__AVR_TRM04__)
    if (info.port == eATmegaPort::PORT_A) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD0 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_B) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC0A, eATmegaPinFunc::TOC1C, eATmegaPinFunc::PCI7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1B, eATmegaPinFunc::PCI6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1A, eATmegaPinFunc::PCI5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC2A, eATmegaPinFunc::PCI4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PDO, eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::PCI3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PDI, eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::PCI2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::PCI1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_CS, eATmegaPinFunc::PCI0 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_C) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD15, eATmegaPinFunc::TIMER3_ICP, eATmegaPinFunc::CLKO };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD14, eATmegaPinFunc::TOC3A };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD13, eATmegaPinFunc::TOC3B };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD12, eATmegaPinFunc::TOC3C };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD11, eATmegaPinFunc::TIMER3_CLKI };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD10 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD9 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_AD8 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_D) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER0_CLKI };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_CLKI };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART1_CLK };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_ICP };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT3, eATmegaPinFunc::USART1_TXD };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT2, eATmegaPinFunc::USART1_RXD };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT1, eATmegaPinFunc::TWI_SDA, eATmegaPinFunc::TOC2B };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT0, eATmegaPinFunc::TWI_CLK, eATmegaPinFunc::TOC0B };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_E) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT7, eATmegaPinFunc::AIN1, eATmegaPinFunc::UVCON };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT6, eATmegaPinFunc::AIN0 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT5, eATmegaPinFunc::TOSC2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT4, eATmegaPinFunc::TOSC2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::UID };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_ALE };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_RD };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EXTMEM_WR };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_F) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC0 };
        return { funcs, countof(funcs) };
      }
    }
  #elif defined(__AVR_TRM05__)
    if (info.port == eATmegaPort::PORT_A) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC7, eATmegaPinFunc::PCI7 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC6, eATmegaPinFunc::PCI6 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC5, eATmegaPinFunc::PCI5 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC4, eATmegaPinFunc::PCI4 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC3, eATmegaPinFunc::PCI3 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC2, eATmegaPinFunc::PCI2 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC1, eATmegaPinFunc::PCI1 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::ADC0, eATmegaPinFunc::PCI0 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_B) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_SCK, eATmegaPinFunc::PCI15 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_MISO, eATmegaPinFunc::PCI14 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_MOSI, eATmegaPinFunc::PCI13 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::SPI_CS, eATmegaPinFunc::TOC0B, eATmegaPinFunc::PCI12 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::AIN1, eATmegaPinFunc::TOC0A, eATmegaPinFunc::PCI11 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::AIN0, eATmegaPinFunc::EINT2, eATmegaPinFunc::PCI10 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_ECI, eATmegaPinFunc::CLKO, eATmegaPinFunc::PCI9 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER0_ECI, eATmegaPinFunc::USART0_CLK, eATmegaPinFunc::PCI8 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_C) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOSC2, eATmegaPinFunc::PCI23 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOSC1, eATmegaPinFunc::PCI22 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI21 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI20 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI19 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::PCI18 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TWI_SDA, eATmegaPinFunc::PCI17 };
        return { funcs, countof(funcs) };
      }
      else if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TWI_CLK, eATmegaPinFunc::PCI16 };
        return { funcs, countof(funcs) };
      }
    }
    else if (info.port == eATmegaPort::PORT_D) {
      if (info.pinidx == 7) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC2A, eATmegaPinFunc::PCI31 };
        return { funcs, countof(funcs) };
      }
      if (info.pinidx == 6) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TIMER1_ICP, eATmegaPinFunc::TOC2B, eATmegaPinFunc::PCI30 };
        return { funcs, countof(funcs) };
      }
      if (info.pinidx == 5) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1A, eATmegaPinFunc::PCI29 };
        return { funcs, countof(funcs) };
      }
      if (info.pinidx == 4) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::TOC1B, eATmegaPinFunc::USART1_CLK, eATmegaPinFunc::PCI28 };
        return { funcs, countof(funcs) };
      }
      if (info.pinidx == 3) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT1, eATmegaPinFunc::USART1_TXD, eATmegaPinFunc::PCI27 };
        return { funcs, countof(funcs) };
      }
      if (info.pinidx == 2) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::EINT0, eATmegaPinFunc::USART1_RXD, eATmegaPinFunc::PCI26 };
        return { funcs, countof(funcs) };
      }
      if (info.pinidx == 1) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART0_TXD, eATmegaPinFunc::PCI25 };
        return { funcs, countof(funcs) };
      }
      if (info.pinidx == 0) {
        static const eATmegaPinFunc funcs[] = { eATmegaPinFunc::USART0_RXD, eATmegaPinFunc::PCI24 };
        return { funcs, countof(funcs) };
      }
    }
  #endif

  return ATmegaPinFunctions();  // default and empty.
}

#endif // HAL_AVR_DIRTY_INIT
#endif // __AVR__
