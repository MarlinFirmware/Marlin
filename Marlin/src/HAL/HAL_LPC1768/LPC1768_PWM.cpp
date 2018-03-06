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
 * The PWM1 module is used to directly control the Servo 0, 1 & 3 pins and D9 & D10 pins.  This keeps
 * the pulse width jitter to under a microsecond.
 *
 * For all other pins a timer is used to generate interrupts.  The ISR
 * routine does the actual setting/clearing of pins.  The upside is that any pin can
 * have a PWM channel assigned to it.  The downside is that there is more pulse width
 * jitter. The jitter depends on what else is happening in the system and what ISRs
 * pre-empt the PWM ISR.
 */

/**
 * The data structures are set up to minimize the computation done by the ISR which
 * minimizes ISR execution time.  Execution times are 5-14µs depending on how full the
 * ISR table is.  14uS is for a 20 element ISR table.
 *
 * Two tables are used.  One table contains the data used by the ISR to update/control
 * the PWM pins.  The other is used as an aid when updating the ISR table.
 *
 * See the end of this file for details on the hardware/firmware interaction
 */

/**
 * Directly controlled PWM pins (
 *   NA means not being used as a directly controlled PWM pin
 *
 *                   Re-ARM              MKS Sbase
 *  PWM1.1   P1_18   SERVO3_PIN           NA(no connection)
 *  PWM1.1   P2_00    NA(E0_STEP_PIN)     NA(X stepper)
 *  PWM1.2   P1_20   SERVO0_PIN           NA(no connection)
 *  PWM1.2   P2_01    NA(X_STEP_PIN)      NA(Y stepper)
 *  PWM1.3   P1_21   SERVO1_PIN           NA(no connection)
 *  PWM1.3   P2_02    NA(Y_STEP_PIN)      NA(Z stepper)
 *  PWM1.4   P1_23    NA(SDSS(SSEL0))    SERVO0_PIN
 *  PWM1.4   P2_03    NA(Z_STEP_PIN)      NA(E0 stepper)
 *  PWM1.5   P1_24    NA(X_MIN_PIN)       NA(X_MIN_pin)
 *  PWM1.5   P2_04   RAMPS_D9_PIN        FAN_PIN
 *  PWM1.6   P1_26    NA(Y_MIN_PIN)       NA(Y_MIN_pin)
 *  PWM1.6   P2_05   RAMPS_D10_PIN       HEATER_BED_PIN
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"
#include <lpc17xx_pinsel.h>
#include "LPC1768_PWM.h"
#include <Arduino.h>

#define NUM_ISR_PWMS 20


#define LPC_PORT_OFFSET         (0x0020)
#define LPC_PIN(pin)            (1UL << pin)
#define LPC_GPIO(port)          ((volatile LPC_GPIO_TypeDef *)(LPC_GPIO0_BASE + LPC_PORT_OFFSET * port))


typedef struct {            // holds all data needed to control/init one of the PWM channels
  bool                active_flag;    // THIS TABLE ENTRY IS ACTIVELY TOGGLING A PIN
  pin_t               pin;
  volatile uint32_t*  set_register;
  volatile uint32_t*  clr_register;
  uint32_t            write_mask;     // USED BY SET/CLEAR COMMANDS
  uint32_t            microseconds;   // value written to MR register
  uint32_t            min;            // lower value limit checked by WRITE routine before writing to the MR register
  uint32_t            max;            // upper value limit checked by WRITE routine before writing to the MR register
  uint8_t             servo_index;    // 0 - MAX_SERVO -1 : servo index,  0xFF : PWM channel
} PWM_map;

PWM_map PWM1_map_A[NUM_ISR_PWMS];  // compiler will initialize to all zeros
PWM_map PWM1_map_B[NUM_ISR_PWMS];  // compiler will initialize to all zeros

PWM_map *active_table = PWM1_map_A;
PWM_map *work_table = PWM1_map_B;
PWM_map *temp_table;

#define P1_18_PWM_channel  1  // servo 3
#define P1_20_PWM_channel  2  // servo 0
#define P1_21_PWM_channel  3  // servo 1
#define P1_23_PWM_channel  4  // servo 0 for MKS Sbase
#define P2_04_PWM_channel  5  // D9
#define P2_05_PWM_channel  6  // D10

typedef struct {
  uint32_t min;
  uint32_t max;
  bool     assigned;
} table_direct;

table_direct direct_table[6];  // compiler will initialize to all zeros

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
 *  The desired prescale frequency column comes from an input in the range of 544 - 2400 microseconds
 *  and the desire to just shift the input left or right as needed.
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

void LPC1768_PWM_init(void) {

  /////  directly controlled PWM pins (interrupts not used for these)

  #define SBIT_CNTEN      0  // PWM1 counter & pre-scaler enable/disable
  #define SBIT_CNTRST     1  // reset counters to known state
  #define SBIT_PWMEN      3  // 1 - PWM, 0 - timer
  #define SBIT_PWMMR0R    1
  #define PCPWM1          6
  #define PCLK_PWM1      12

  SBI(LPC_SC->PCONP, PCPWM1);                                             // Enable PWM1 controller (enabled on power up)
  LPC_SC->PCLKSEL0 &= ~(0x3 << PCLK_PWM1);
  LPC_SC->PCLKSEL0 |= (LPC_PWM1_PCLKSEL0 << PCLK_PWM1);

  uint32_t PR = (CLKPWR_GetPCLK(CLKPWR_PCLKSEL_PWM1) / 1000000) - 1;      // Prescalar to create 1 MHz output

  LPC_PWM1->MR0  = LPC_PWM1_MR0;                                          // TC resets every 19,999 + 1 cycles - sets PWM cycle(Ton+Toff) to 20 mS
  // MR0 must be set before TCR enables the PWM
  LPC_PWM1->TCR  = _BV(SBIT_CNTEN) | _BV(SBIT_CNTRST) | _BV(SBIT_PWMEN);  // Enable counters, reset counters, set mode to PWM
  CBI(LPC_PWM1->TCR, SBIT_CNTRST);                                        // Take counters out of reset
  LPC_PWM1->PR   = PR;
  LPC_PWM1->MCR  = _BV(SBIT_PWMMR0R) | _BV(0);                            // Reset TC if it matches MR0, disable all interrupts except for MR0
  LPC_PWM1->CTCR = 0;                                                     // Disable counter mode (enable PWM mode)
  LPC_PWM1->LER  = 0x07F;                                                 // Set the latch Enable Bits to load the new Match Values for MR0 - MR6
  LPC_PWM1->PCR  = 0;                                                     // Single edge mode for all channels, PWM1 control of outputs off

  ////  interrupt controlled PWM setup

  LPC_SC->PCONP |= 1 << 23;  // power on timer3
  HAL_PWM_TIMER->PR = PR;
  HAL_PWM_TIMER->MCR = 0x0B;              // Interrupt on MR0 & MR1, reset on MR0
  HAL_PWM_TIMER->MR0 = LPC_PWM1_MR0;
  HAL_PWM_TIMER->MR1 = 0;
  HAL_PWM_TIMER->TCR = _BV(0);       // enable
  NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);
  NVIC_SetPriority(HAL_PWM_TIMER_IRQn, NVIC_EncodePriority(0, 4, 0));
}


bool ISR_table_update = false;  // flag to tell the ISR that the tables need to be updated & swapped
uint8_t ISR_index = 0;          // index used by ISR to skip already actioned entries
#define COPY_ACTIVE_TABLE    for (uint8_t i = 0; i < NUM_ISR_PWMS ; i++) work_table[i] = active_table[i]
uint32_t first_MR1_value = LPC_PWM1_MR0 + 1;

void LPC1768_PWM_sort(void) {

  for (uint8_t i = NUM_ISR_PWMS; --i;) {  // (bubble) sort table by microseconds
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
}

bool LPC1768_PWM_attach_pin(pin_t pin, uint32_t min /* = 1 */, uint32_t max /* = (LPC_PWM1_MR0 - 1) */, uint8_t servo_index /* = 0xff */) {

  pin = GET_PIN_MAP_PIN(GET_PIN_MAP_INDEX(pin & 0xFF));  // Sometimes the upper byte is garbled

////  direct control PWM code
  switch(pin) {
    case P1_23:                                       // MKS Sbase Servo 0, PWM1 channel 4  (J3-8 PWM1.4)
      direct_table[P1_23_PWM_channel - 1].min = min;
      direct_table[P1_23_PWM_channel - 1].max = MIN(max, LPC_PWM1_MR0 - MR0_MARGIN);
      direct_table[P1_23_PWM_channel - 1].assigned = true;
      return true;
    case P1_20:                                       // Servo 0, PWM1 channel 2  (Pin 11  P1.20 PWM1.2)
      direct_table[P1_20_PWM_channel - 1].min = min;
      direct_table[P1_20_PWM_channel - 1].max = MIN(max, LPC_PWM1_MR0 - MR0_MARGIN);
      direct_table[P1_20_PWM_channel - 1].assigned = true;
      return true;
    case P1_21:                                       // Servo 1, PWM1 channel 3  (Pin 6  P1.21 PWM1.3)
      direct_table[P1_21_PWM_channel - 1].min = min;
      direct_table[P1_21_PWM_channel - 1].max = MIN(max, LPC_PWM1_MR0 - MR0_MARGIN);
      direct_table[P1_21_PWM_channel - 1].assigned = true;
      return true;
    case P1_18:                                       // Servo 3, PWM1 channel 1 (Pin 4  P1.18 PWM1.1)
      direct_table[P1_18_PWM_channel - 1].min = min;
      direct_table[P1_18_PWM_channel - 1].max = MIN(max, LPC_PWM1_MR0 - MR0_MARGIN);
      direct_table[P1_18_PWM_channel - 1].assigned = true;
      return true;
    case P2_04:                                       // D9 FET, PWM1 channel 5  (Pin 9  P2_04 PWM1.5)
      direct_table[P2_04_PWM_channel - 1].min = min;
      direct_table[P2_04_PWM_channel - 1].max = MIN(max, LPC_PWM1_MR0 - MR0_MARGIN);
      direct_table[P2_04_PWM_channel - 1].assigned = true;
      return true;
    case P2_05:                                       // D10 FET, PWM1 channel 6 (Pin 10  P2_05 PWM1.6)
      direct_table[P2_05_PWM_channel - 1].min = min;
      direct_table[P2_05_PWM_channel - 1].max = MIN(max, LPC_PWM1_MR0 - MR0_MARGIN);
      direct_table[P2_05_PWM_channel - 1].assigned = true;
      return true;
  }

////  interrupt controlled PWM code
  NVIC_DisableIRQ(HAL_PWM_TIMER_IRQn);    // make it safe to update the active table
                                 // OK to update the active table because the
                                 // ISR doesn't use any of the changed items

  if (ISR_table_update) //use work table if that's the newest
    temp_table = work_table;
  else
    temp_table = active_table;

  uint8_t slot = 0;
  for (uint8_t i = 0; i < NUM_ISR_PWMS; i++)         // see if already in table
    if (temp_table[i].pin == pin) {
      NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);  // re-enable PWM interrupts
      return 1;
    }

  for (uint8_t i = 1; (i < NUM_ISR_PWMS + 1) && !slot; i++)         // find empty slot
    if ( !(temp_table[i - 1].set_register)) { slot = i; break; }  // any item that can't be zero when active or just attached is OK

  if (!slot) {
    NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);  // re-enable PWM interrupts
    return 0;
  }

  slot--;  // turn it into array index

  temp_table[slot].pin          = pin;     // init slot
  temp_table[slot].set_register = &LPC_GPIO(LPC1768_PIN_PORT(pin))->FIOSET;
  temp_table[slot].clr_register = &LPC_GPIO(LPC1768_PIN_PORT(pin))->FIOCLR;
  temp_table[slot].write_mask   = LPC_PIN(LPC1768_PIN_PIN(pin));
  temp_table[slot].min          = min;
  temp_table[slot].max          = max;                // different max for ISR PWMs than for direct PWMs
  temp_table[slot].servo_index  = servo_index;
  temp_table[slot].active_flag  = false;

  NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);  // re-enable PWM interrupts

  return 1;
}


bool LPC1768_PWM_detach_pin(pin_t pin) {

  pin = GET_PIN_MAP_PIN(GET_PIN_MAP_INDEX(pin & 0xFF));

////  direct control PWM code
  switch(pin) {
    case P1_23:                                       // MKS Sbase Servo 0, PWM1 channel 4  (J3-8 PWM1.4)
      if (!direct_table[P1_23_PWM_channel - 1].assigned) return false;
      CBI(LPC_PWM1->PCR, 8 + P1_23_PWM_channel);      // disable PWM1 module control of this pin
      LPC_PINCON->PINSEL3 &= ~(0x3 <<  14);           // return pin to general purpose I/O
      direct_table[P1_23_PWM_channel - 1].assigned = false;
      return true;
    case P1_20:                                       // Servo 0, PWM1 channel 2  (Pin 11  P1.20 PWM1.2)
      if (!direct_table[P1_20_PWM_channel - 1].assigned) return false;
      CBI(LPC_PWM1->PCR, 8 + P1_20_PWM_channel);      // disable PWM1 module control of this pin
      LPC_PINCON->PINSEL3 &= ~(0x3 <<  8);            // return pin to general purpose I/O
      direct_table[P1_20_PWM_channel - 1].assigned = false;
      return true;
    case P1_21:                                       // Servo 1, PWM1 channel 3  (Pin 6  P1.21 PWM1.3)
      if (!direct_table[P1_21_PWM_channel - 1].assigned) return false;
      CBI(LPC_PWM1->PCR, 8 + P1_21_PWM_channel);      // disable PWM1 module control of this pin
      LPC_PINCON->PINSEL3 &= ~(0x3 << 10);            // return pin to general purpose I/O
      direct_table[P1_21_PWM_channel - 1].assigned = false;
      return true;
    case P1_18:                                       // Servo 3, PWM1 channel 1 (Pin 4  P1.18 PWM1.1)
      if (!direct_table[P1_18_PWM_channel - 1].assigned) return false;
      CBI(LPC_PWM1->PCR, 8 + P1_18_PWM_channel);      // disable PWM1 module control of this pin
      LPC_PINCON->PINSEL3 &= ~(0x3 <<  4);            // return pin to general purpose I/O
      direct_table[P1_18_PWM_channel - 1].assigned = false;
      return true;
    case P2_04:                                       // D9 FET, PWM1 channel 5  (Pin 9  P2_04 PWM1.5)
      if (!direct_table[P2_04_PWM_channel - 1].assigned) return false;
      CBI(LPC_PWM1->PCR, 8 + P2_04_PWM_channel);      // disable PWM1 module control of this pin
      LPC_PINCON->PINSEL4 &= ~(0x3 << 10);            // return pin to general purpose I/O
      direct_table[P2_04_PWM_channel - 1].assigned = false;
      return true;
    case P2_05:                                       // D10 FET, PWM1 channel 6 (Pin 10  P2_05 PWM1.6)
      if (!direct_table[P2_05_PWM_channel - 1].assigned) return false;
      CBI(LPC_PWM1->PCR, 8 + P2_05_PWM_channel);      // disable PWM1 module control of this pin
      LPC_PINCON->PINSEL4 &= ~(0x3 <<  4);            // return pin to general purpose I/O
      direct_table[P2_05_PWM_channel - 1].assigned = false;
      return true;
  }

////  interrupt controlled PWM code
  NVIC_DisableIRQ(HAL_PWM_TIMER_IRQn);

  if (ISR_table_update) {
    ISR_table_update = false;    // don't update yet - have another update to do
    NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);  // re-enable PWM interrupts
  }
  else {
    NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);  // re-enable PWM interrupts
    COPY_ACTIVE_TABLE;  // copy active table into work table
  }

  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < NUM_ISR_PWMS; i++) {        // find slot
    if (work_table[i].pin == pin) {
      slot = i;
      break;
    }
  }
  if (slot == 0xFF)    // return error if pin not found
    return false;

  work_table[slot] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  LPC1768_PWM_sort();    // sort table by microseconds
  ISR_table_update = true;
  return true;
}

// value is 0-20,000 microseconds (0% to 100% duty cycle)
// servo routine provides values in the 544 - 2400 range
bool LPC1768_PWM_write(pin_t pin, uint32_t value) {

  pin = GET_PIN_MAP_PIN(GET_PIN_MAP_INDEX(pin & 0xFF));

////  direct control PWM code
  switch(pin) {
    case P1_23:                                                           // MKS Sbase Servo 0, PWM1 channel 4  (J3-8 PWM1.4)
      if (!direct_table[P1_23_PWM_channel - 1].assigned) return false;
      LPC_PWM1->PCR |=  _BV(8 + P1_23_PWM_channel); // enable PWM1 module control of this pin
      LPC_PINCON->PINSEL3 = 0x2 <<  14;             // must set pin function AFTER setting PCR
      // load the new time value
      LPC_PWM1->MR4 = MAX(MIN(value, direct_table[P1_23_PWM_channel - 1].max), direct_table[P1_23_PWM_channel - 1].min);
      LPC_PWM1->LER = 0x1 << P1_23_PWM_channel; // Set the latch Enable Bit to load the new Match Value on the next MR0
      return true;
    case P1_20:                                                           // Servo 0, PWM1 channel 2 (Pin 11  P1.20 PWM1.2)
      if (!direct_table[P1_20_PWM_channel - 1].assigned) return false;
      LPC_PWM1->PCR |=  _BV(8 + P1_20_PWM_channel); // enable PWM1 module control of this pin
      LPC_PINCON->PINSEL3 |= 0x2 <<  8;             // must set pin function AFTER setting PCR
      // load the new time value
      LPC_PWM1->MR2 = MAX(MIN(value, direct_table[P1_20_PWM_channel - 1].max), direct_table[P1_20_PWM_channel - 1].min);
      LPC_PWM1->LER = 0x1 << P1_20_PWM_channel; // Set the latch Enable Bit to load the new Match Value on the next MR0
      return true;
    case P1_21:                                                           // Servo 1, PWM1 channel 3 (Pin 6  P1.21 PWM1.3)
      if (!direct_table[P1_21_PWM_channel - 1].assigned) return false;
      LPC_PWM1->PCR |=  _BV(8 + P1_21_PWM_channel); // enable PWM1 module control of this pin
      LPC_PINCON->PINSEL3 |= 0x2 << 10;              // must set pin function AFTER setting PCR
      // load the new time value
      LPC_PWM1->MR3 = MAX(MIN(value, direct_table[P1_21_PWM_channel - 1].max), direct_table[P1_21_PWM_channel - 1].min);
      LPC_PWM1->LER = 0x1 << P1_21_PWM_channel; // Set the latch Enable Bit to load the new Match Value on the next MR0
      return true;
    case P1_18:                                                           // Servo 3, PWM1 channel 1 (Pin 4  P1.18 PWM1.1)
      if (!direct_table[P1_18_PWM_channel - 1].assigned) return false;
      LPC_PWM1->PCR |=  _BV(8 + P1_18_PWM_channel); // enable PWM1 module control of this pin
      LPC_PINCON->PINSEL3 |= 0x2 <<  4;             // must set pin function AFTER setting PCR
      // load the new time value
      LPC_PWM1->MR1 = MAX(MIN(value, direct_table[P1_18_PWM_channel - 1].max), direct_table[P1_18_PWM_channel - 1].min);
      LPC_PWM1->LER = 0x1 << P1_18_PWM_channel; // Set the latch Enable Bit to load the new Match Value on the next MR0
      return true;
    case P2_04:                                                           // D9 FET, PWM1 channel 5 (Pin 9  P2_04 PWM1.5)
      if (!direct_table[P2_04_PWM_channel - 1].assigned) return false;
      LPC_PWM1->PCR |=  _BV(8 + P2_04_PWM_channel); // enable PWM1 module control of this pin
      LPC_PINCON->PINSEL4 |= 0x1 <<  8;             // must set pin function AFTER setting PCR
      // load the new time value
      LPC_PWM1->MR5 = MAX(MIN(value, direct_table[P2_04_PWM_channel - 1].max), direct_table[P2_04_PWM_channel - 1].min);
      LPC_PWM1->LER = 0x1 << P2_04_PWM_channel; // Set the latch Enable Bit to load the new Match Value on the next MR0
      return true;
    case P2_05:                                                           // D10 FET, PWM1 channel 6 (Pin 10  P2_05 PWM1.6)
      if (!direct_table[P2_05_PWM_channel - 1].assigned) return false;
      LPC_PWM1->PCR |=  _BV(8 + P2_05_PWM_channel); // enable PWM1 module control of this pin
      LPC_PINCON->PINSEL4 |= 0x1 << 10;             // must set pin function AFTER setting PCR
      // load the new time value
      LPC_PWM1->MR6 = MAX(MIN(value, direct_table[P2_05_PWM_channel - 1].max), direct_table[P2_05_PWM_channel - 1].min);
      LPC_PWM1->LER = 0x1 << P2_05_PWM_channel; // Set the latch Enable Bit to load the new Match Value on the next MR0
      return true;
  }

////  interrupt controlled PWM code
  NVIC_DisableIRQ(HAL_PWM_TIMER_IRQn);
  if (!ISR_table_update)   // use the most up to date table
    COPY_ACTIVE_TABLE;  // copy active table into work table

  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < NUM_ISR_PWMS; i++)         // find slot
    if (work_table[i].pin == pin) { slot = i; break; }
  if (slot == 0xFF) {   // return error if pin not found
    NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);
    return false;
  }

  work_table[slot].microseconds = MAX(MIN(value, work_table[slot].max), work_table[slot].min);;
  work_table[slot].active_flag  = true;

  LPC1768_PWM_sort();    // sort table by microseconds
  ISR_table_update = true;

  NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);  // re-enable PWM interrupts
  return 1;
}


bool useable_hardware_PWM(pin_t pin) {

  pin = GET_PIN_MAP_PIN(GET_PIN_MAP_INDEX(pin & 0xFF));

  NVIC_DisableIRQ(HAL_PWM_TIMER_IRQn);

  bool return_flag = false;
  for (uint8_t i = 0; i < NUM_ISR_PWMS; i++)         // see if it's already setup
    if (active_table[i].pin == pin) return_flag = true;
  for (uint8_t i = 0; i < NUM_ISR_PWMS; i++)         // see if there is an empty slot
    if (!active_table[i].set_register) return_flag = true;
  NVIC_EnableIRQ(HAL_PWM_TIMER_IRQn);  // re-enable PWM interrupts
  return return_flag;
}


////////////////////////////////////////////////////////////////////////////////


#define PWM_LPC1768_ISR_SAFETY_FACTOR 5  // amount of time needed to guarantee MR1 count will be above TC
volatile bool in_PWM_isr = false;

HAL_PWM_TIMER_ISR {
  bool first_active_entry = true;
  uint32_t next_MR1_val;

  if (in_PWM_isr) goto exit_PWM_ISR;  // prevent re-entering this ISR
  in_PWM_isr = true;

  if (HAL_PWM_TIMER->IR & 0x01) {  // MR0 interrupt
    next_MR1_val = first_MR1_value;               // only used if have a blank ISR table
    if (ISR_table_update) {                       // new values have been loaded so swap tables
      temp_table = active_table;
      active_table = work_table;
      work_table = temp_table;
      ISR_table_update = false;
    }
  }
  HAL_PWM_TIMER->IR = 0x3F;  // clear all interrupts

  for (uint8_t i = 0; i < NUM_ISR_PWMS; i++) {
    if (active_table[i].active_flag) {
      if (first_active_entry) {
        first_active_entry = false;
        next_MR1_val = active_table[i].microseconds;
      }
      if (HAL_PWM_TIMER->TC < active_table[i].microseconds) {
        *active_table[i].set_register = active_table[i].write_mask;   // set pin high
      }
      else {
        *active_table[i].clr_register = active_table[i].write_mask;   // set pin low
        next_MR1_val = (i == NUM_ISR_PWMS -1)
          ? LPC_PWM1_MR0 + 1                  // done with table, wait for MR0
          : active_table[i + 1].microseconds; // set next MR1 interrupt?
      }
    }
  }
  if (first_active_entry) next_MR1_val = LPC_PWM1_MR0 + 1;  // empty table so disable MR1 interrupt
  HAL_PWM_TIMER->MR1 = MAX(next_MR1_val, HAL_PWM_TIMER->TC + PWM_LPC1768_ISR_SAFETY_FACTOR); // set next
  in_PWM_isr = false;

  exit_PWM_ISR:
  return;
}
#endif


/////////////////////////////////////////////////////////////////
/////////////////  HARDWARE FIRMWARE INTERACTION ////////////////
/////////////////////////////////////////////////////////////////

/**
 *  There are two distinct systems used for PWMs:
 *    directly controlled pins
 *    ISR controlled pins.
 *
 *  The two systems are independent of each other.  The use the same counter frequency so there's no
 *  translation needed when setting the time values.  The init, attach, detach and write routines all
 *  start with the direct pin code which is followed by the ISR pin code.
 *
 *  The PMW1 module handles the directly controlled pins.  Each directly controlled pin is associated
 *  with a match register (MR1 - MR6).  When the associated MR equals the module's TIMER/COUNTER (TC)
 *  then the pins is set to low.  The MR0 register controls the repetition rate.  When the TC equals
 *  MR0 then the TC is reset and ALL directly controlled pins are set high.  The resulting pulse widths
 *  are almost immune to system loading and ISRs.  No PWM1 interrupts are used.
 *
 *  The ISR controlled pins use the TIMER/COUNTER, MR0 and MR1 registers from one timer.  MR0 controls
 *  period of the controls the repetition rate.  When the TC equals MR0 then the TC is reset and an
 *  interrupt is generated. When the TC equals MR1 then an interrupt is generated.
 *
 *  Each interrupt does the following:
 *    1) Swaps the tables if it's a MR0 interrupt and the swap flag is set.  It then clears the swap flag.
 *    2) Scans the entire ISR table (it's been sorted low to high time)
 *         a. If its the first active entry then it grabs the time as a tentative time for MR1
 *         b. If active and TC is less than the time then it sets the pin high
 *         c. If active and TC is more than the time it sets the pin high
 *         d. On every entry that sets a pin low it grabs the NEXT entry's time for use as the next MR1.
 *            This results in MR1 being set to the time in the first active entry that does NOT set a
 *            pin low.
 *         e. If it's setting the last entry's pin low then it sets MR1 to a value bigger than MR0
 *         f. If no value has been grabbed for the next MR1 then it's an empty table and MR1 is set to a
 *            value greater than MR0
 */
