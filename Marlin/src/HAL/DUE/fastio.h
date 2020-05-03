/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Fast I/O Routines for SAM3X8E
 * Use direct port manipulation to save scads of processor time.
 * Contributed by Triffid_Hunter and modified by Kliment, thinkyhead, Bob-the-Kuhn, et.al.
 */

/**
 * Description: Fast IO functions for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 * Note the code here was specifically crafted by disassembling what GCC produces
 * out of it, so GCC is able to optimize it out as much as possible to the least
 * amount of instructions. Be very carefull if you modify them, as "clean code"
 * leads to less efficient compiled code!!
 */

#include <pins_arduino.h>

#include "../../inc/MarlinConfigPre.h"

/**
 * Utility functions
 */

// Due has 12 PWMs assigned to logical pins 2-13.
// 6, 7, 8 & 9 come from the PWM controller. The others come from the timers.
#define PWM_PIN(P)              WITHIN(P, 2, 13)

#ifndef MASK
  #define MASK(PIN) (1 << PIN)
#endif

/**
 * Magic I/O routines
 *
 * Now you can simply SET_OUTPUT(STEP); WRITE(STEP, HIGH); WRITE(STEP, LOW);
 *
 * Why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
 */

// Read a pin
#define _READ(IO) bool(DIO ## IO ## _WPORT -> PIO_PDSR & MASK(DIO ## IO ## _PIN))

// Write to a pin
#define _WRITE(IO,V) do { \
  volatile Pio* port = (DIO ##  IO ## _WPORT); \
  const uint32_t mask = MASK(DIO ## IO ## _PIN); \
  if (V) port->PIO_SODR = mask; \
  else port->PIO_CODR = mask; \
}while(0)

// Toggle a pin
#define _TOGGLE(IO) _WRITE(IO, !READ(IO))

#if MB(PRINTRBOARD_G2)

  #include "fastio/G2_pins.h"

  // Set pin as input
  #define _SET_INPUT(IO) do{ \
    pmc_enable_periph_clk(G2_g_APinDescription[IO].ulPeripheralId); \
    PIO_Configure((DIO ## IO ## _WPORT), PIO_INPUT, MASK(DIO ## IO ## _PIN), 0); \
  }while(0)

  // Set pin as output
  #define _SET_OUTPUT(IO) do{ \
    uint32_t mask = MASK(G2_g_APinDescription[IO].ulPeripheralId); \
    if ((PMC->PMC_PCSR0 & mask) != (mask)) PMC->PMC_PCER0 = mask; \
    volatile Pio* port = (DIO ## IO ## _WPORT); \
    mask = MASK(DIO ## IO ## _PIN); \
    if (_READ(IO)) port->PIO_SODR = mask; \
    else port->PIO_CODR = mask; \
    port->PIO_IDR = mask; \
    const uint32_t pin_config = G2_g_APinDescription[IO].ulPinConfiguration; \
    if (pin_config & PIO_PULLUP) port->PIO_PUER = mask; \
    else port->PIO_PUDR = mask; \
    if (pin_config & PIO_OPENDRAIN) port->PIO_MDER = mask; \
    else port->PIO_MDDR = mask; \
    port->PIO_PER = mask; \
    port->PIO_OER = mask; \
    g_pinStatus[IO] = (g_pinStatus[IO] & 0xF0) | PIN_STATUS_DIGITAL_OUTPUT; \
  }while(0)

 /**
  *  Set pin as output with comments
  *  #define _SET_OUTPUT(IO) do{ \
  *    uint32_t mask = MASK(G2_g_APinDescription[IO].ulPeripheralId); \
  *    if ((PMC->PMC_PCSR0 & mask ) != (mask))  PMC->PMC_PCER0 = mask; \  // enable PIO clock if not already enabled
  *
  *    volatile Pio* port = (DIO ##  IO ## _WPORT); \
  *    const uint32_t mask = MASK(DIO ## IO ## _PIN); \
  *    if (_READ(IO)) port->PIO_SODR = mask; \ // set output to match input BEFORE setting direction or will glitch the output
  *    else port->PIO_CODR = mask; \
  *
  *    port->PIO_IDR = mask; \ // disable interrupt
  *
  *    uint32_t pin_config = G2_g_APinDescription[IO].ulPinConfiguration; \
  *    if (pin_config & PIO_PULLUP) pPio->PIO_PUER = mask; \  // enable pullup if necessary
  *    else  pPio->PIO_PUDR = mask; \
  *
  *    if (pin_config & PIO_OPENDRAIN) port->PIO_MDER = mask; \ // Enable multi-drive if necessary
  *    else  port->PIO_MDDR = mask; \
  *
  *    port->PIO_PER = mask; \
  *    port->PIO_OER = mask; \  // set to output
  *
  *    g_pinStatus[IO] = (g_pinStatus[IO] & 0xF0) | PIN_STATUS_DIGITAL_OUTPUT; \
  *  }while(0)
  */

#else

    // Set pin as input
  #define _SET_INPUT(IO) do{ \
    pmc_enable_periph_clk(g_APinDescription[IO].ulPeripheralId); \
    PIO_Configure(digitalPinToPort(IO), PIO_INPUT, digitalPinToBitMask(IO), 0); \
  }while(0)

  // Set pin as output
  #define _SET_OUTPUT(IO) do{ \
    pmc_enable_periph_clk(g_APinDescription[IO].ulPeripheralId); \
    PIO_Configure(digitalPinToPort(IO), _READ(IO) ? PIO_OUTPUT_1 : PIO_OUTPUT_0, digitalPinToBitMask(IO), g_APinDescription[IO].ulPinConfiguration); \
    g_pinStatus[IO] = (g_pinStatus[IO] & 0xF0) | PIN_STATUS_DIGITAL_OUTPUT; \
  }while(0)
#endif

// Set pin as input with pullup mode
#define _PULLUP(IO,V)        pinMode(IO, (V) ? INPUT_PULLUP : INPUT)

// Read a pin (wrapper)
#define READ(IO)             _READ(IO)

// Write to a pin (wrapper)
#define WRITE(IO,V)          _WRITE(IO,V)

// Toggle a pin (wrapper)
#define TOGGLE(IO)           _TOGGLE(IO)

// Set pin as input (wrapper)
#define SET_INPUT(IO)        _SET_INPUT(IO)
// Set pin as input with pullup (wrapper)
#define SET_INPUT_PULLUP(IO) do{ _SET_INPUT(IO); _PULLUP(IO, HIGH); }while(0)
// Set pin as output (wrapper) -  reads the pin and sets the output to that value
#define SET_OUTPUT(IO)       _SET_OUTPUT(IO)
// Set pin as PWM
#define SET_PWM               SET_OUTPUT

// Check if pin is an input
#define IS_INPUT(IO)         ((digitalPinToPort(IO)->PIO_OSR & digitalPinToBitMask(IO)) == 0)
// Check if pin is an output
#define IS_OUTPUT(IO)        ((digitalPinToPort(IO)->PIO_OSR & digitalPinToBitMask(IO)) != 0)

// Shorthand
#define OUT_WRITE(IO,V)      do{ SET_OUTPUT(IO); WRITE(IO,V); }while(0)

// digitalRead/Write wrappers
#define extDigitalRead(IO)    digitalRead(IO)
#define extDigitalWrite(IO,V) digitalWrite(IO,V)

/**
 * Ports and functions
 * Added as necessary or if I feel like it- not a comprehensive list!
 */

// UART
#define RXD        DIO0
#define TXD        DIO1

// TWI (I2C)
#define SCL        DIO21
#define SDA        DIO20

/**
 * pins
 */

#define DIO0_PIN 8
#define DIO0_WPORT PIOA

#define DIO1_PIN 9
#define DIO1_WPORT PIOA

#define DIO2_PIN 25
#define DIO2_WPORT PIOB

#define DIO3_PIN 28
#define DIO3_WPORT PIOC

#define DIO4_PIN 26
#define DIO4_WPORT PIOC

#define DIO5_PIN 25
#define DIO5_WPORT PIOC

#define DIO6_PIN 24
#define DIO6_WPORT PIOC

#define DIO7_PIN 23
#define DIO7_WPORT PIOC

#define DIO8_PIN 22
#define DIO8_WPORT PIOC

#define DIO9_PIN 21
#define DIO9_WPORT PIOC

#define DIO10_PIN 29
#define DIO10_WPORT PIOC

#define DIO11_PIN 7
#define DIO11_WPORT PIOD

#define DIO12_PIN 8
#define DIO12_WPORT PIOD

#define DIO13_PIN 27
#define DIO13_WPORT PIOB

#define DIO14_PIN 4
#define DIO14_WPORT PIOD

#define DIO15_PIN 5
#define DIO15_WPORT PIOD

#define DIO16_PIN 13
#define DIO16_WPORT PIOA

#define DIO17_PIN 12
#define DIO17_WPORT PIOA

#define DIO18_PIN 11
#define DIO18_WPORT PIOA

#define DIO19_PIN 10
#define DIO19_WPORT PIOA

#define DIO20_PIN 12
#define DIO20_WPORT PIOB

#define DIO21_PIN 13
#define DIO21_WPORT PIOB

#define DIO22_PIN 26
#define DIO22_WPORT PIOB

#define DIO23_PIN 14
#define DIO23_WPORT PIOA

#define DIO24_PIN 15
#define DIO24_WPORT PIOA

#define DIO25_PIN 0
#define DIO25_WPORT PIOD

#define DIO26_PIN 1
#define DIO26_WPORT PIOD

#define DIO27_PIN 2
#define DIO27_WPORT PIOD

#define DIO28_PIN 3
#define DIO28_WPORT PIOD

#define DIO29_PIN 6
#define DIO29_WPORT PIOD

#define DIO30_PIN 9
#define DIO30_WPORT PIOD

#define DIO31_PIN 7
#define DIO31_WPORT PIOA

#define DIO32_PIN 10
#define DIO32_WPORT PIOD

#define DIO33_PIN 1
#define DIO33_WPORT PIOC

#if !MB(PRINTRBOARD_G2)    // normal DUE pin mapping

  #define DIO34_PIN 2
  #define DIO34_WPORT PIOC

  #define DIO35_PIN 3
  #define DIO35_WPORT PIOC

  #define DIO36_PIN 4
  #define DIO36_WPORT PIOC

  #define DIO37_PIN 5
  #define DIO37_WPORT PIOC

  #define DIO38_PIN 6
  #define DIO38_WPORT PIOC

  #define DIO39_PIN 7
  #define DIO39_WPORT PIOC

  #define DIO40_PIN 8
  #define DIO40_WPORT PIOC

  #define DIO41_PIN 9
  #define DIO41_WPORT PIOC

#endif // !PRINTRBOARD_G2

#define DIO42_PIN 19
#define DIO42_WPORT PIOA

#define DIO43_PIN 20
#define DIO43_WPORT PIOA

#define DIO44_PIN 19
#define DIO44_WPORT PIOC

#define DIO45_PIN 18
#define DIO45_WPORT PIOC

#define DIO46_PIN 17
#define DIO46_WPORT PIOC

#define DIO47_PIN 16
#define DIO47_WPORT PIOC

#define DIO48_PIN 15
#define DIO48_WPORT PIOC

#define DIO49_PIN 14
#define DIO49_WPORT PIOC

#define DIO50_PIN 13
#define DIO50_WPORT PIOC

#define DIO51_PIN 12
#define DIO51_WPORT PIOC

#define DIO52_PIN 21
#define DIO52_WPORT PIOB

#define DIO53_PIN 14
#define DIO53_WPORT PIOB

#define DIO54_PIN 16
#define DIO54_WPORT PIOA

#define DIO55_PIN 24
#define DIO55_WPORT PIOA

#define DIO56_PIN 23
#define DIO56_WPORT PIOA

#define DIO57_PIN 22
#define DIO57_WPORT PIOA

#define DIO58_PIN 6
#define DIO58_WPORT PIOA

#define DIO59_PIN 4
#define DIO59_WPORT PIOA

#define DIO60_PIN 3
#define DIO60_WPORT PIOA

#define DIO61_PIN 2
#define DIO61_WPORT PIOA

#define DIO62_PIN 17
#define DIO62_WPORT PIOB

#define DIO63_PIN 18
#define DIO63_WPORT PIOB

#define DIO64_PIN 19
#define DIO64_WPORT PIOB

#define DIO65_PIN 20
#define DIO65_WPORT PIOB

#define DIO66_PIN 15
#define DIO66_WPORT PIOB

#define DIO67_PIN 16
#define DIO67_WPORT PIOB

#define DIO68_PIN 1
#define DIO68_WPORT PIOA

#define DIO69_PIN 0
#define DIO69_WPORT PIOA

#define DIO70_PIN 17
#define DIO70_WPORT PIOA

#define DIO71_PIN 18
#define DIO71_WPORT PIOA

#define DIO72_PIN 30
#define DIO72_WPORT PIOC

#define DIO73_PIN 21
#define DIO73_WPORT PIOA

#define DIO74_PIN 25
#define DIO74_WPORT PIOA

#define DIO75_PIN 26
#define DIO75_WPORT PIOA

#define DIO76_PIN 27
#define DIO76_WPORT PIOA

#define DIO77_PIN 28
#define DIO77_WPORT PIOA

#define DIO78_PIN 23
#define DIO78_WPORT PIOB

#define DIO79_PIN 17
#define DIO79_WPORT PIOA

#define DIO80_PIN 12
#define DIO80_WPORT PIOB

#define DIO81_PIN 8
#define DIO81_WPORT PIOA

#define DIO82_PIN 11
#define DIO82_WPORT PIOA

#define DIO83_PIN 13
#define DIO83_WPORT PIOA

#define DIO84_PIN 4
#define DIO84_WPORT PIOD

#define DIO85_PIN 11
#define DIO85_WPORT PIOB

#define DIO86_PIN 21
#define DIO86_WPORT PIOB

#define DIO87_PIN 29
#define DIO87_WPORT PIOA

#define DIO88_PIN 15
#define DIO88_WPORT PIOB

#define DIO89_PIN 14
#define DIO89_WPORT PIOB

#define DIO90_PIN 1
#define DIO90_WPORT PIOA

#define DIO91_PIN 15
#define DIO91_WPORT PIOB

#if ARDUINO_SAM_ARCHIM

  #define DIO92_PIN 11
  #define DIO92_WPORT PIOC

  #define DIO93_PIN 2
  #define DIO93_WPORT PIOB

  #define DIO94_PIN 1
  #define DIO94_WPORT PIOB

  #define DIO95_PIN 0
  #define DIO95_WPORT PIOB

  #define DIO96_PIN 10
  #define DIO96_WPORT PIOC

  #define DIO97_PIN 24
  #define DIO97_WPORT PIOB

  #define DIO98_PIN 7
  #define DIO98_WPORT PIOB

  #define DIO99_PIN 6
  #define DIO99_WPORT PIOB

  #define DIO100_PIN 8
  #define DIO100_WPORT PIOB

  #define DIO101_PIN 5
  #define DIO101_WPORT PIOB

  #define DIO102_PIN 4
  #define DIO102_WPORT PIOB

  #define DIO103_PIN 3
  #define DIO103_WPORT PIOB

  #define DIO104_PIN 20
  #define DIO104_WPORT PIOC

  #define DIO105_PIN 22
  #define DIO105_WPORT PIOB

  #define DIO106_PIN 27
  #define DIO106_WPORT PIOC

  #define DIO107_PIN 10
  #define DIO107_WPORT PIOB

  #define DIO108_PIN 9
  #define DIO108_WPORT PIOB

#else // !ARDUINO_SAM_ARCHIM

  #define DIO92_PIN 5
  #define DIO92_WPORT PIOA

  #define DIO93_PIN 12
  #define DIO93_WPORT PIOB

  #define DIO94_PIN 22
  #define DIO94_WPORT PIOB

  #define DIO95_PIN 23
  #define DIO95_WPORT PIOB

  #define DIO96_PIN 24
  #define DIO96_WPORT PIOB

  #define DIO97_PIN 20
  #define DIO97_WPORT PIOC

  #define DIO98_PIN 27
  #define DIO98_WPORT PIOC

  #define DIO99_PIN 10
  #define DIO99_WPORT PIOC

  #define DIO100_PIN 11
  #define DIO100_WPORT PIOC

#endif // !ARDUINO_SAM_ARCHIM
