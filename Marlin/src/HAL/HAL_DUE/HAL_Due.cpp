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
 * Description: HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */

#if defined(ARDUINO_ARCH_SAM)

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../HAL.h"

#include <Wire.h>

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

uint8_t MCUSR;

uint16_t HAL_adc_result;

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

// disable interrupts
void cli(void)
{
	noInterrupts();
}

// enable interrupts
void sei(void)
{
	interrupts();
}

void _delay_ms (int delay_ms)
{
	//todo: port for Due?
	delay (delay_ms);
}

extern "C" {
  extern unsigned int _ebss; // end of bss section
}

// return free memory between end of heap (or end bss) and whatever is current
int freeMemory()
{
  int free_memory;
  int heap_end = (int)_sbrk(0);

  if(heap_end == 0)
    free_memory = ((int)&free_memory) - ((int)&_ebss);
  else
    free_memory = ((int)&free_memory) - heap_end;

  return free_memory;

}

// --------------------------------------------------------------------------
// Timers
// --------------------------------------------------------------------------

typedef struct
{
    Tc          *pTimerRegs;
    uint16_t    channel;
    IRQn_Type   IRQ_Id;
  } tTimerConfig ;

#define  NUM_HARDWARE_TIMERS 9

static const tTimerConfig TimerConfig [NUM_HARDWARE_TIMERS] =
  {
    { TC0, 0, TC0_IRQn},
    { TC0, 1, TC1_IRQn},
    { TC0, 2, TC2_IRQn},
    { TC1, 0, TC3_IRQn},
    { TC1, 1, TC4_IRQn},
    { TC1, 2, TC5_IRQn},
    { TC2, 0, TC6_IRQn},
    { TC2, 1, TC7_IRQn},
    { TC2, 2, TC8_IRQn},
  };


void HAL_timer_start (uint8_t timer_num, uint32_t frequency)
{
	Tc *tc = TimerConfig [timer_num].pTimerRegs;
	IRQn_Type irq = TimerConfig [timer_num].IRQ_Id;
	uint32_t channel = TimerConfig [timer_num].channel;

	pmc_set_writeprotect(false);
	pmc_enable_periph_clk((uint32_t)irq);
	TC_Configure (tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3);

	uint32_t rc = VARIANT_MCK/32/frequency;

	TC_SetRC(tc, channel, rc);

	TC_Start(tc, channel);

	// enable interrupt on RC compare
	tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;

	NVIC_EnableIRQ(irq);
}

void HAL_timer_enable_interrupt (uint8_t timer_num)
{
	const tTimerConfig *pConfig = &TimerConfig [timer_num];

	pConfig->pTimerRegs->TC_CHANNEL [pConfig->channel].TC_IER = TC_IER_CPCS;
}

void HAL_timer_disable_interrupt (uint8_t timer_num)
{
	const tTimerConfig *pConfig = &TimerConfig [timer_num];

	pConfig->pTimerRegs->TC_CHANNEL [pConfig->channel].TC_IDR = TC_IER_CPCS;
}

void HAL_timer_set_count (uint8_t timer_num, uint32_t count)
{
	const tTimerConfig *pConfig = &TimerConfig [timer_num];

	TC_SetRC (pConfig->pTimerRegs, pConfig->channel, count);
}

void HAL_timer_isr_prologue (uint8_t timer_num)
{
	const tTimerConfig *pConfig = &TimerConfig [timer_num];

	TC_GetStatus (pConfig->pTimerRegs, pConfig->channel);
}

// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------

void HAL_adc_start_conversion (uint8_t adc_pin)
{
	HAL_adc_result = analogRead (adc_pin);
}

uint16_t HAL_adc_get_result(void)
{
	// nop
	return HAL_adc_result;
}

#endif // ARDUINO_ARCH_SAM

