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
 * The class Servo uses the PWM class to implement it's functions
 *
 * The PWM1 module is only used to generate interrups at specified times. It
 * is NOT used to directly toggle pins. The ISR writes to the pin assigned to
 * that interrupt
 *
 * All PWMs use the same repetition rate - 20mS because that's the normal servo rate
 *
 */

/**
 * The data structures are setup to minimize the computation done by the ISR which
 * minimizes ISR execution time.  Execution times are 1.7 to 1.9 microseconds.
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
 */


#ifdef TARGET_LPC1768
#include <lpc17xx_pinsel.h>
//#include "../HAL.h"
//#include "../../../macros.h"
#include "serial.h"

typedef struct {       // holds all data needed to control the 6 PWM channels
    uint8_t sequence;  // 0: available slot, 1 - 6: PWM channel assigned to that slot
    uint8_t logical_pin;
    uint16_t PWM_mask;
    volatile uint32_t* set_register;
    volatile uint32_t* clr_register;
    uint32_t write_mask;
    uint32_t microseconds;
    uint32_t min;
    uint32_t max;
    bool    PWM_flag;     //
    uint8_t servo_index;  // 0 - MAX_SERVO -1 : servo index,  0xFF : PWM channel
    bool    active_flag;

} PWM_map;

#define MICRO_MAX 0xffffffff

#define PWM_MAP_INIT_ROW {0, 0xff, 0, 0, 0, 0, MICRO_MAX, 0, 0, 0, 0, 0}
#define PWM_MAP_INIT {PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                     };

PWM_map PWM1_map_A[6] = PWM_MAP_INIT;
PWM_map PWM1_map_B[6] = PWM_MAP_INIT;

PWM_map *active_table = PWM1_map_A;
PWM_map *work_table = PWM1_map_B;
PWM_map *ISR_table;


#define NUM_PWMS 6

volatile uint8_t PWM1_ISR_index = 0;

#define IR_BIT(p) (p >= 0 && p <= 3 ? p : p + 4 )
#define COPY_ACTIVE_TABLE    for (uint8_t i = 0; i < 6 ; i++) work_table[i] = active_table[i]
#define PIN_IS_INVERTED(p) 0  // place holder in case inverting PWM output is offered


/**
 *  Prescale register and MR0 register values
 *
 *               100MHz PCLK          50MHz PCLK          25MHz PCLK         12.5MHz PCLK
 *             -----------------   -----------------   -----------------   -----------------
 *  desired    prescale  MR0       prescale  MR0       prescale  MR0       prescale  MR0        resolution
 *  prescale   register  register  register  register  register  register  register  register   in degrees
 *  freq       value     value     value     value     value     value     value     value
 *
 *      8        11.5    159,999      5.25   159,999       2.13  159,999    0.5625   159,999    0.023
 *      4        24       79,999     11.5     79,999       5.25   79,999    2.125     79,999    0.045
 *      2        49       39,999     24       39,999      11.5    39,999    5.25      39,999    0.090
 *      1        99       19,999     49       19,999      24      19,999   11.5       19,999    0.180
 *    0.5       199        9,999     99        9,999      49       9,999   24          9,999    0.360
 *   0.25       399        4,999    199        4,999      99       4,999   49          4,999    0.720
 *  0.125       799        2,499    399        2,499     199       2,499   99          2,499    1.440
 *
 *  The desired prescale frequency comes from an input in the range of 544 - 2400 microseconds and the
 *  desire to just shift the input left or right as needed.
 *
 *  A resolution of 0.2 degrees seems reasonable so a prescale frequency output of 1MHz is being used.
 *  It also means we don't need to scale the input.
 *
 *  The PCLK is set to 25MHz because that's the slowest one that gives whole numbers for prescale and
 *  MR0 registers.
 *
 *  Final settings:
 *   PCLKSEL0: 0x0
 *   PWM1PR:   0x018 (24)
 *   PWM1MR0:  0x04E1F (19,999)
 *
 */

#define LPC_PWM1_MR0 19999  // base repetition rate minus one count - 20mS
#define LPC_PWM1_PR 24      // prescaler value - prescaler divide by 24 + 1  -  1 MHz output
#define LPC_PWM1_PCLKSEL0 0x00   // select clock source for prescaler - defaults to 25MHz on power up
                                 // 0: 25MHz, 1: 100MHz, 2: 50MHz, 3: 12.5MHZ to PWM1 prescaler
#define MR0_MARGIN 200       // if channel value too close to MR0 the system locks up


void LPC1768_PWM_init(void) {
  #define SBIT_CNTEN     0  // PWM1 counter & pre-scaler enable/disable
  #define SBIT_CNTRST    1  // reset counters to known state
  #define SBIT_PWMEN     3  // 1 - PWM, 0 - timer
  #define SBIT_PWMMR0R   1
  #define PCPWM1         6
  #define PCLK_PWM1      12

  LPC_SC->PCONP |= (1 << PCPWM1);      // enable PWM1 controller (enabled on power up)
  LPC_SC->PCLKSEL0 &= ~(0x3 << PCLK_PWM1);
  LPC_SC->PCLKSEL0 |= (LPC_PWM1_PCLKSEL0 << PCLK_PWM1);
  LPC_PWM1->MR0 = LPC_PWM1_MR0;                // TC resets every 19,999 + 1 cycles - sets PWM cycle(Ton+Toff) to 20 mS
                                        // MR0 must be set before TCR enables the PWM
  LPC_PWM1->TCR = _BV(SBIT_CNTEN) | _BV(SBIT_CNTRST)| _BV(SBIT_PWMEN);;  // enable counters, reset counters, set mode to PWM
  LPC_PWM1->TCR &= ~(_BV(SBIT_CNTRST));  // take counters out of reset
  LPC_PWM1->PR  =  LPC_PWM1_PR;
  LPC_PWM1->MCR = (_BV(SBIT_PWMMR0R) | _BV(0));     // Reset TC if it matches MR0, disable all interrupts except for MR0
  LPC_PWM1->CTCR = 0;                   // disable counter mode (enable PWM mode)

  LPC_PWM1->LER = 0x07F;  // Set the latch Enable Bits to load the new Match Values for MR0 - MR6
  // Set all PWMs to single edge
  LPC_PWM1->PCR = 0;    // single edge mode for all channels, PWM1 control of outputs off

  NVIC_EnableIRQ(PWM1_IRQn);     // Enable interrupt handler
  //      NVIC_SetPriority(PWM1_IRQn, NVIC_EncodePriority(0, 10, 0));  // normal priority for PWM module
  NVIC_SetPriority(PWM1_IRQn, NVIC_EncodePriority(0, 0, 0));  // minimizes jitter due to higher priority ISRs
}


bool PWM_table_swap;  // flag to tell the ISR that the tables have been swapped



bool LPC1768_PWM_attach_pin(uint8_t pin, uint32_t min = 1, uint32_t max = (LPC_PWM1_MR0 - MR0_MARGIN), uint8_t servo_index = 0xff) {
  COPY_ACTIVE_TABLE;  // copy active table into work table
  uint8_t slot = 0;
  for (uint8_t i = 0; i < NUM_PWMS ; i++)         // see if already in table
    if (work_table[i].logical_pin == pin) return 1;

  for (uint8_t i = 1; (i < NUM_PWMS + 1) && !slot; i++)         // find empty slot
    if ( !(work_table[i - 1].set_register)) slot = i;  // any item that can't be zero when active or just attached is OK
  if (!slot) return 0;
  slot--;  // turn it into array index

  work_table[slot].logical_pin = pin;  // init slot
  work_table[slot].PWM_mask = 0;  // real value set by PWM_write
  work_table[slot].set_register = PIN_IS_INVERTED(pin) ? &LPC_GPIO(pin_map[pin].port)->FIOCLR : &LPC_GPIO(pin_map[pin].port)->FIOSET;
  work_table[slot].clr_register = PIN_IS_INVERTED(pin) ? &LPC_GPIO(pin_map[pin].port)->FIOSET : &LPC_GPIO(pin_map[pin].port)->FIOCLR;
  work_table[slot].write_mask = LPC_PIN(pin_map[pin].pin);
  work_table[slot].microseconds = MICRO_MAX;
  work_table[slot].min = min;
  work_table[slot].max = MIN(max, LPC_PWM1_MR0 - MR0_MARGIN);
  work_table[slot].servo_index = servo_index;
  work_table[slot].active_flag = false;

  //swap tables
  NVIC_DisableIRQ(PWM1_IRQn);
  PWM_map *pointer_swap = active_table;
  active_table = work_table;
  work_table = pointer_swap;
  PWM_table_swap = true;  // tell the ISR that the tables have been swapped
  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts

  return 1;
}



bool LPC1768_PWM_write(uint8_t pin, uint32_t value) {
  COPY_ACTIVE_TABLE;  // copy active table into work table
  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // find slot
    if (work_table[i].logical_pin == pin) slot = i;
  if (slot == 0xFF) return false;    // return error if pin not found
  digitalWrite(pin, 0);  // set pin to output & set it low
  work_table[slot].microseconds = MAX(MIN(value, work_table[slot].max), work_table[slot].min);
  work_table[slot].active_flag = true;

  for (uint8_t i = NUM_PWMS; --i;) {  // (bubble) sort table by microseconds
    bool didSwap = false;
    PWM_map temp;
    for (uint16_t j = 0; j < i; ++j) {
      if (work_table[j].microseconds > work_table[j + 1].microseconds) {
        temp = work_table[j + 1];
        work_table[j + 1] = work_table[j];
        work_table[j] = temp;
        didSwap = true;
      }
    }
    if (!didSwap) break;
  }

  for (uint8_t i = 0; i < NUM_PWMS; i++)                             // set the index & PWM_mask
    if (work_table[i].active_flag == true) {
      work_table[i].sequence = i + 1;
      work_table[i].PWM_mask = _BV(IR_BIT(i + 1));
    }
    else work_table[i].sequence = 0;

  uint32_t interrupt_mask = 0;                          // set match registers to new values, build IRQ mask
  if (work_table[0].active_flag == true) {
    LPC_PWM1->MR1 = work_table[0].microseconds;
    interrupt_mask |= _BV(3);
  }
  if (work_table[1].active_flag == true) {
    LPC_PWM1->MR2 = work_table[1].microseconds;
    interrupt_mask |= _BV(6);
  }
  if (work_table[2].active_flag == true) {
    LPC_PWM1->MR3 = work_table[2].microseconds;
    interrupt_mask |= _BV(9);
  }
  if (work_table[3].active_flag == true) {
    LPC_PWM1->MR4 = work_table[3].microseconds;
    interrupt_mask |= _BV(12);
  }
  if (work_table[4].active_flag == true) {
    LPC_PWM1->MR5 = work_table[4].microseconds;
    interrupt_mask |= _BV(15);
  }
  if (work_table[5].active_flag == true) {
    LPC_PWM1->MR6 = work_table[5].microseconds;
    interrupt_mask |= _BV(18);
  }
  interrupt_mask |= _BV(0);  // add in MR0 interrupt

  // swap tables
  NVIC_DisableIRQ(PWM1_IRQn);
  LPC_PWM1->LER = 0x07E;  // Set the latch Enable Bits to load the new Match Values for MR1 - MR6
  PWM_map *pointer_swap = active_table;
  active_table = work_table;
  work_table = pointer_swap;
  PWM_table_swap = true;  // tell the ISR that the tables have been swapped
  LPC_PWM1->MCR = interrupt_mask;          // enable new PWM individual channel interrupts
  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts

  return 1;
}



bool LPC1768_PWM_detach_pin(uint8_t pin) {
  COPY_ACTIVE_TABLE;  // copy active table into work table
  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // find slot
    if (work_table[i].logical_pin == pin) slot = i;
  if (slot == 0xFF) return false;    // return error if pin not found
  pinMode(pin, INPUT_PULLUP);  // set pin to input with pullup
  work_table[slot] = PWM_MAP_INIT_ROW;

  for (uint8_t i = NUM_PWMS; --i;) {  // (bubble) sort table by microseconds
    bool didSwap = false;
    PWM_map temp;
    for (uint16_t j = 0; j < i; ++j) {
      if (work_table[j].microseconds > work_table[j + 1].microseconds) {
        temp = work_table[j + 1];
        work_table[j + 1] = work_table[j];
        work_table[j] = temp;
        didSwap = true;
      }
    }
    if (!didSwap) break;
  }

  for (uint8_t i = 0; i < NUM_PWMS; i++)                             // set the index & PWM_mask
    if (work_table[i].active_flag == true) {
      work_table[i].sequence = i + 1;
      work_table[i].PWM_mask = _BV(IR_BIT(i + 1));
    }
    else work_table[i].sequence = 0;

  uint32_t interrupt_mask = 0;                        // set match registers to new values, build IRQ mask
  if (work_table[0].active_flag == true) {
    LPC_PWM1->MR1 = work_table[0].microseconds;
    interrupt_mask |= _BV(3);
  }
  if (work_table[1].active_flag == true) {
    LPC_PWM1->MR2 = work_table[1].microseconds;
    interrupt_mask |= _BV(6);
  }
  if (work_table[2].active_flag == true) {
    LPC_PWM1->MR3 = work_table[2].microseconds;
    interrupt_mask |= _BV(9);
  }
  if (work_table[3].active_flag == true) {
    LPC_PWM1->MR4 = work_table[3].microseconds;
    interrupt_mask |= _BV(12);
  }
  if (work_table[4].active_flag == true) {
    LPC_PWM1->MR5 = work_table[4].microseconds;
    interrupt_mask |= _BV(15);
  }
  if (work_table[5].active_flag == true) {
    LPC_PWM1->MR6 = work_table[5].microseconds;
    interrupt_mask |= _BV(18);
  }

  interrupt_mask |= _BV(0);  // add in MR0 interrupt

  // swap tables
  NVIC_DisableIRQ(PWM1_IRQn);
  LPC_PWM1->LER = 0x07E;  // Set the latch Enable Bits to load the new Match Values for MR1 - MR6
  PWM_map *pointer_swap = active_table;
  active_table = work_table;
  work_table = pointer_swap;
  PWM_table_swap = true;  // tell the ISR that the tables have been swapped
  LPC_PWM1->MCR = interrupt_mask;          // enable remaining PWM individual channel interrupts
  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts

  return 1;
}

////////////////////////////////////////////////////////////////////////////////

#define HAL_PWM_LPC1768_ISR  extern "C" void PWM1_IRQHandler(void)

HAL_PWM_LPC1768_ISR {
  if (PWM_table_swap) ISR_table = work_table;   // use old table if a swap was just done
  else ISR_table = active_table;

  if (LPC_PWM1->IR & 0x1) {                                      // MR0 interrupt
    PWM_table_swap = false;                                         // MR0 means new values could have been
    ISR_table = active_table;                                       // loaded so set everything to normal operation
    for (uint8_t i = 0; (i < NUM_PWMS) && ISR_table[i].active_flag ; i++)
      *ISR_table[i].set_register = ISR_table[i].write_mask;     // set all enabled channels active
    LPC_PWM1->IR = 0x01;                                             // clear the MR0 interrupt flag bit
    PWM1_ISR_index = 0;
  }
  else {
    if (ISR_table[PWM1_ISR_index].active_flag && (LPC_PWM1->IR & ISR_table[PWM1_ISR_index].PWM_mask)) {
      LPC_PWM1->IR = ISR_table[PWM1_ISR_index].PWM_mask;       // clear the interrupt flag bit
      *ISR_table[PWM1_ISR_index].clr_register = ISR_table[PWM1_ISR_index].write_mask;   // set channel to inactive
    }
    PWM1_ISR_index++;                                           // should be the index for the next interrupt
  }

return;
}

#endif
