/*
   Contributors:
   Copyright (c) 2014 Bob Cousins bobcousins42@googlemail.com
*/

/*
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
*/

// **************************************************************************
//
// Description:          *** HAL for Arduino Due ***
//
// **************************************************************************

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "HAL.h"

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

void cli(void)
{
	//todo port for Due
}

void sei(void)
{
	//todo port for Due
}

void _delay_ms (int delay_ms)
{
	//todo: port for Due
	delay (delay_ms);
}

int freeMemory()
{
	//todo: port for Due
	return 0;
}

//

void eeprom_write_byte(unsigned char *pos, unsigned char value) {}

unsigned char eeprom_read_byte(unsigned char *pos)
{
	return '\0';
}

//

void HAL_startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency)
{
	pmc_set_writeprotect(false);
	pmc_enable_periph_clk((uint32_t)irq);
	TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1);

	uint32_t rc = VARIANT_MCK/2/frequency; // divide by 2 because we selected TIMER_CLOCK1 above

	TC_SetRC(tc, channel, rc);

	TC_Start(tc, channel);

	// enable interrupt on RC compare
	tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;

	NVIC_EnableIRQ(irq);
}

void HAL_set_step_timer (uint32_t count)
{
	TC_SetRC (TC1, 0, count);
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
//! @brief
//! @param[in]
//! @param[out]
//! @return
// --------------------------------------------------------------------------
