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
 * Clock / power management for Atmel SAM3U (AT91SAM3U4E)
 *
 * The 4pi carries a 12MHz crystal. PLLA multiplies it to 96MHz, which drives
 * the Master Clock (MCK) with no prescaling — the part's maximum rated speed.
 *
 *   MCK = (BOARD_MAINOSC / DIVA) * (MULA + 1) / PRES
 *       = (12MHz / 1) * (7 + 1) / 1
 *       = 96MHz
 *
 * At 96MHz with VDDCORE at 1.80V the embedded flash needs 4 read cycles, i.e.
 * EEFC_FMR.FWS = 3 (datasheet Table 42-55). Note the original 4pi/Sprinter
 * firmware programmed only 2 wait states here while still running at 96MHz,
 * which is out of spec; this HAL uses the documented value.
 */

#include <sam3u4e.h>

#define BOARD_MAINOSC   12000000UL      // 4pi main crystal
#define SAM3U_PLLA_MULA 7               // Multiplier - 1
#define SAM3U_PLLA_DIVA 1               // Divider
#define SAM3U_MCK       (BOARD_MAINOSC / (SAM3U_PLLA_DIVA) * ((SAM3U_PLLA_MULA) + 1))

#if SAM3U_MCK != F_CPU
  #error "F_CPU does not match the SAM3U PLLA configuration."
#endif

// Bring the CPU up to F_CPU. Called from Reset_Handler via SystemInit().
#ifdef __cplusplus
  extern "C"
#endif
void sam3u_clock_init();

/**
 * Enable the peripheral clock for a peripheral ID (ID_PIOA, ID_TC0, ...).
 * SAM3U tops out at 30 peripheral IDs, so only the PCER0/PCDR0 bank exists.
 */
static inline void pmc_enable_periph_clk(const uint32_t id) {
  if (id < ID_PERIPH_COUNT && !(PMC->PMC_PCSR0 & (1UL << id))) PMC->PMC_PCER0 = 1UL << id;
}

static inline void pmc_disable_periph_clk(const uint32_t id) {
  if (id < ID_PERIPH_COUNT) PMC->PMC_PCDR0 = 1UL << id;
}
