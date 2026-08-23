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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Pin mapping for Atmel SAM3U (AT91SAM3U4E)
 *
 * The SAM3U has three 32-bit PIO controllers (PIOA, PIOB, PIOC) giving 96
 * possible I/O lines. There is no Arduino core for this part, so Marlin
 * addresses pins by their native port/bit position rather than through a
 * board-specific "digital pin" table:
 *
 *     pin number = (port index * 32) + bit    PA0..PA31 =  0..31
 *                                             PB0..PB31 = 32..63
 *                                             PC0..PC31 = 64..95
 *
 * Pin files therefore read exactly like the schematic (`PA20`, `PC28`, ...),
 * and the port/bit arithmetic below folds away entirely for constant pins.
 *
 * Note the CMSIS headers claim the `PIO_PAxx` namespace, not the bare `PAxx`
 * names used here, so the two can coexist.
 */

#include <stdint.h>

// The CMSIS device header supplies the Pio struct and the PIOA/PIOB/PIOC bases
#include <sam3u4e.h>

// ------------------------
// Port/bit decomposition
// ------------------------

#define SAM3U_PORT(P)  (uint32_t(P) >> 5)          // 0=PIOA, 1=PIOB, 2=PIOC
#define SAM3U_BIT(P)   (uint32_t(P) & 31)          // Bit position within the port
#define SAM3U_MASK(P)  (uint32_t(1) << SAM3U_BIT(P))

// Resolve a pin to its PIO controller. Constant-folds for constant pins.
#define SAM3U_PIO(P)   ((P) < 32 ? PIOA : (P) < 64 ? PIOB : PIOC)

// Peripheral ID of the PIO controller owning the pin (for pmc_enable_periph_clk)
#define SAM3U_PIO_ID(P) ((P) < 32 ? ID_PIOA : (P) < 64 ? ID_PIOB : ID_PIOC)

#define NUM_DIGITAL_PINS 96
#define VALID_PIN(P)     ((P) >= 0 && (P) < NUM_DIGITAL_PINS)

// 96 pins fit comfortably in a signed byte, leaving -1 free for "no pin"
typedef int8_t pin_t;

// ------------------------
// Pin names
// ------------------------

// PIOA
#define PA0   0
#define PA1   1
#define PA2   2
#define PA3   3
#define PA4   4
#define PA5   5
#define PA6   6
#define PA7   7
#define PA8   8
#define PA9   9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15
#define PA16 16
#define PA17 17
#define PA18 18
#define PA19 19
#define PA20 20
#define PA21 21
#define PA22 22
#define PA23 23
#define PA24 24
#define PA25 25
#define PA26 26
#define PA27 27
#define PA28 28
#define PA29 29
#define PA30 30
#define PA31 31

// PIOB
#define PB0  32
#define PB1  33
#define PB2  34
#define PB3  35
#define PB4  36
#define PB5  37
#define PB6  38
#define PB7  39
#define PB8  40
#define PB9  41
#define PB10 42
#define PB11 43
#define PB12 44
#define PB13 45
#define PB14 46
#define PB15 47
#define PB16 48
#define PB17 49
#define PB18 50
#define PB19 51
#define PB20 52
#define PB21 53
#define PB22 54
#define PB23 55
#define PB24 56
#define PB25 57
#define PB26 58
#define PB27 59
#define PB28 60
#define PB29 61
#define PB30 62
#define PB31 63

// PIOC
#define PC0  64
#define PC1  65
#define PC2  66
#define PC3  67
#define PC4  68
#define PC5  69
#define PC6  70
#define PC7  71
#define PC8  72
#define PC9  73
#define PC10 74
#define PC11 75
#define PC12 76
#define PC13 77
#define PC14 78
#define PC15 79
#define PC16 80
#define PC17 81
#define PC18 82
#define PC19 83
#define PC20 84
#define PC21 85
#define PC22 86
#define PC23 87
#define PC24 88
#define PC25 89
#define PC26 90
#define PC27 91
#define PC28 92
#define PC29 93
#define PC30 94
#define PC31 95

