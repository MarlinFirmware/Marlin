/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * The class Servo uses the PWM class to implement its functions
 *
 * All PWMs use the same repetition rate - 20mS because that's the normal servo rate
 */

/**
 * This is a hybrid system.
 *
 * The PWM1 module is used to directly control the Servo 0, 1 & 3 pins.  This keeps
 * the pulse width jitter to under a microsecond.
 *
 * For all other pins the PWM1 module is used to generate interrupts.  The ISR
 * routine does the actual setting/clearing of pins.  The upside is that any pin can
 * have a PWM channel assigned to it.  The downside is that there is more pulse width
 * jitter. The jitter depends on what else is happening in the system and what ISRs
 * prempt the PWM ISR.  Writing to the SD card can add 20 microseconds to the pulse
 * width.
 */

/**
 * The data structures are setup to minimize the computation done by the ISR which
 * minimizes ISR execution time.  Execution times are 2.2 - 3.7 microseconds.
 *
 * Two tables are used.  active_table is used by the ISR.  Changes to the table are
 * are done by copying the active_table into the work_table, updating the work_table
 * and then swapping the two tables.  Swapping is done by manipulating pointers.
 *
 * Immediately after the swap the ISR uses the work_table until the start of the
 * next 20mS cycle. During this transition the "work_table" is actually the table
 * that was being used before the swap.  The "active_table" contains the data that
 * will start being used at the start of the next 20mS period.  This keeps the pins
 * well behaved during the transition.
 *
 * The ISR's priority is set to the maximum otherwise other ISRs can cause considerable
 * jitter in the PWM high time.
 *
 * See the end of this file for details on the hardware/firmware interaction
 */

#ifndef _LPC1768_PWM_H_
#define _LPC1768_PWM_H_

#include <pinmapping.h>
#include <lpc17xx_clkpwr.h>

#define LPC_PWM1_MR0 19999  // base repetition rate minus one count - 20mS
#define LPC_PWM1_PCLKSEL0 CLKPWR_PCLKSEL_CCLK_DIV_4 // select clock divider for prescaler - defaults to 4 on power up
#define MR0_MARGIN 200      // if channel value too close to MR0 the system locks up

void LPC1768_PWM_init(void);
bool LPC1768_PWM_attach_pin(pin_t pin, uint32_t min=1, uint32_t max=(LPC_PWM1_MR0 - (MR0_MARGIN)), uint8_t servo_index=0xFF);
bool LPC1768_PWM_write(pin_t pin, uint32_t value);
bool LPC1768_PWM_detach_pin(pin_t pin);
bool useable_hardware_PWM(pin_t pin);

#endif // _LPC1768_PWM_H_
