/* **************************************************************************

 Marlin 3D Printer Firmware
 Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/


/**
 * Teensy3.5 __MK64FX512__
 * Teensy3.6 __MK66FX1M0__
 */

#if defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "../HAL.h"
#include "HAL_timers_Teensy.h"


void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case 0:
      FTM0_MODE = FTM_MODE_WPDIS | FTM_MODE_FTMEN;
      FTM0_SC = 0x00; // Set this to zero before changing the modulus
      FTM0_CNT = 0x0000; // Reset the count to zero
      FTM0_MOD = 0xFFFF; // max modulus = 65535
      FTM0_C0V = FTM0_TIMER_RATE / frequency; // Initial FTM Channel 0 compare value
      FTM0_SC = (FTM_SC_CLKS(0b1) & FTM_SC_CLKS_MASK) | (FTM_SC_PS(FTM0_TIMER_PRESCALE_BITS) & FTM_SC_PS_MASK); // Bus clock 60MHz divided by prescaler 8
      FTM0_C0SC = FTM_CSC_CHIE | FTM_CSC_MSA | FTM_CSC_ELSA;
      break;
    case 1:
      FTM1_MODE = FTM_MODE_WPDIS | FTM_MODE_FTMEN; // Disable write protection, Enable FTM1
      FTM1_SC = 0x00; // Set this to zero before changing the modulus
      FTM1_CNT = 0x0000; // Reset the count to zero
      FTM1_MOD = 0xFFFF; // max modulus = 65535
      FTM1_C0V = FTM1_TIMER_RATE / frequency; // Initial FTM Channel 0 compare value 65535
      FTM1_SC = (FTM_SC_CLKS(0b1) & FTM_SC_CLKS_MASK) | (FTM_SC_PS(FTM1_TIMER_PRESCALE_BITS) & FTM_SC_PS_MASK); // Bus clock 60MHz divided by prescaler 4
      FTM1_C0SC = FTM_CSC_CHIE | FTM_CSC_MSA | FTM_CSC_ELSA;
      break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch(timer_num) {
    case 0: NVIC_ENABLE_IRQ(IRQ_FTM0); break;
    case 1: NVIC_ENABLE_IRQ(IRQ_FTM1); break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_DISABLE_IRQ(IRQ_FTM0); break;
    case 1: NVIC_DISABLE_IRQ(IRQ_FTM1); break;
  }
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return NVIC_IS_ENABLED(IRQ_FTM0);
    case 1: return NVIC_IS_ENABLED(IRQ_FTM1);
  }
  return false;
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch(timer_num) {
    case 0:
      FTM0_CNT = 0x0000;
      FTM0_SC &= ~FTM_SC_TOF; // Clear FTM Overflow flag
      FTM0_C0SC &= ~FTM_CSC_CHF; // Clear FTM Channel Compare flag
      break;
    case 1:
      FTM1_CNT = 0x0000;
      FTM1_SC &= ~FTM_SC_TOF; // Clear FTM Overflow flag
      FTM1_C0SC &= ~FTM_CSC_CHF; // Clear FTM Channel Compare flag
      break;
  }
}

#endif // Teensy3.5 or Teensy3.6
