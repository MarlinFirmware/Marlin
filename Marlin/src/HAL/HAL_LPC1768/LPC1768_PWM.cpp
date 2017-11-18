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
 * For all other pins the PWM1 module is used to generate interrupts.  The ISR
 * routine does the actual setting/clearing of pins.  The upside is that any pin can
 * have a PWM channel assigned to it.  The downside is that there is more pulse width
 * jitter. The jitter depends on what else is happening in the system and what ISRs
 * pre-empt the PWM ISR.
 */

/**
 * The data structures are set up to minimize the computation done by the ISR which
 * minimizes ISR execution time.  Execution times are 2-4µs except when updating to
 * a new value when they are 19µs.
 *
 * Two tables are used.  One table contains the data used by the ISR to update/control
 * the PWM pins.  The other is used as an aid when rebuilding the ISR table.
 *
 * The LPC1768_PWM_attach_pin routine disables the ISR and then adds the new info to
 * ISR table.  It can update the table directly because none of its changes affect
 * what the ISR does.
 *
 * LPC1768_PWM_detach_pin routine disables the ISR, disables the pin immediately if
 * it's a directly controlled pin and updates the helper table.  It then flags the
 * ISR that the ISR table needs to be rebuilt.
 *
 * LPC1768_PWM_write routine disables the ISR and updates the helper table.  It then
 * flags the ISR that the ISR table needs to be rebuilt.
 *
 * The ISR's priority is set to less than the stepper ISR otherwise it could cause jitter
 * in the step pulses.
 *
 * See the end of this file for details on the hardware/firmware interaction
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

// #include <math.h>
// #include <stdio.h>
// #include <stdlib.h>

#include <lpc17xx_pinsel.h>
#include "LPC1768_PWM.h"
#include "arduino.h"

#define NUM_PWMS 6

typedef struct {            // holds all data needed to control/init one of the PWM channels
  uint8_t             sequence;       // 0: available slot, 1 - 6: PWM channel assigned to that slot
  pin_t               pin;
  uint16_t            PWM_mask;       // MASK TO CHECK/WRITE THE IR REGISTER
  volatile uint32_t*  set_register;
  volatile uint32_t*  clr_register;
  uint32_t            write_mask;     // USED BY SET/CLEAR COMMANDS
  uint32_t            microseconds;   // value written to MR register
  uint32_t            min;            // lower value limit checked by WRITE routine before writing to the MR register
  uint32_t            max;            // upper value limit checked by WRITE routine before writing to the MR register
  bool                PWM_flag;       // 0 - USED BY hardware PWM, 1 - USED BY ANALOGWRITE
  uint8_t             servo_index;    // 0 - MAX_SERVO -1 : servo index,  0xFF : PWM channel
  bool                active_flag;    // THIS TABLE ENTRY IS ACTIVELY TOGGLING A PIN
  uint32_t            PCR_bit;        // PCR register bit to enable PWM1 control of this pin
  volatile uint32_t*  PINSEL_reg;     // PINSEL register
  uint32_t            PINSEL_bits;    // PINSEL register bits to set pin mode to PWM1 control

} PWM_map;


#define MICRO_MAX 0xFFFFFFFF

#define PWM_MAP_INIT_ROW { 0, 0x7FFF, 0, 0, 0, 0, MICRO_MAX, 0, 0, 0, 0, 0, 0, 0, 0 }
#define PWM_MAP_INIT { PWM_MAP_INIT_ROW, PWM_MAP_INIT_ROW, PWM_MAP_INIT_ROW, \
                       PWM_MAP_INIT_ROW, PWM_MAP_INIT_ROW, PWM_MAP_INIT_ROW, \
                     };

PWM_map ISR_table[NUM_PWMS] = PWM_MAP_INIT;

#define IR_BIT(p) ((p) >= 0 && (p) <= 3 ? (p) : p + 4 )
#define PIN_IS_INVERTED(p) 0  // placeholder in case inverting PWM output is offered



#define P1_18_PWM_channel  1  // servo 3
#define P1_20_PWM_channel  2  // servo 0
#define P1_21_PWM_channel  3  // servo 1
#define P2_4_PWM_channel   5  // D9
#define P2_5_PWM_channel   6  // D10

// used to keep track of which Match Registers have been used and if they will be used by the
// PWM1 module to directly control the pin or will be used to generate an interrupt
typedef struct {                    // status of PWM1 channel
  uint8_t             map_used;     // 0 - this MR register not used/assigned
  uint8_t             map_PWM_INT;  // 0 - available for interrupts, 1 - in use by PWM
  pin_t               map_PWM_PIN;  // pin for this PwM1 controlled pin / port
  volatile uint32_t*  MR_register;  // address of the MR register for this PWM1 channel
  uint32_t            PCR_bit;      // PCR register bit to enable PWM1 control of this pin
                                    // 0 - don't switch to PWM1 direct control
  volatile uint32_t*  PINSEL_reg;   // PINSEL register
  uint32_t            PINSEL_bits;  // PINSEL register bits to set pin mode to PWM1 control
} MR_map;

MR_map map_MR[NUM_PWMS];

void LPC1768_PWM_update_map_MR(void) {
  map_MR[0] = { 0, (uint8_t) (LPC_PWM1->PCR & _BV(8 + P1_18_PWM_channel) ? 1 : 0), P1_18, &LPC_PWM1->MR1, 0, 0, 0 };
  map_MR[1] = { 0, (uint8_t) (LPC_PWM1->PCR & _BV(8 + P1_20_PWM_channel) ? 1 : 0), P1_20, &LPC_PWM1->MR2, 0, 0, 0 };
  map_MR[2] = { 0, (uint8_t) (LPC_PWM1->PCR & _BV(8 + P1_21_PWM_channel) ? 1 : 0), P1_21, &LPC_PWM1->MR3, 0, 0, 0 };
  map_MR[3] = { 0, 0, P_NC, &LPC_PWM1->MR4, 0, 0, 0 };
  map_MR[4] = { 0, (uint8_t) (LPC_PWM1->PCR & _BV(8 + P2_4_PWM_channel) ? 1 : 0), P2_4, &LPC_PWM1->MR5, 0, 0, 0 };
  map_MR[5] = { 0, (uint8_t) (LPC_PWM1->PCR & _BV(8 + P2_5_PWM_channel) ? 1 : 0), P2_5, &LPC_PWM1->MR6, 0, 0, 0 };
}

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

bool ISR_table_update = false;  // flag to tell the ISR that the tables need to be updated & swapped

void LPC1768_PWM_init(void) {
  #define SBIT_CNTEN      0  // PWM1 counter & pre-scaler enable/disable
  #define SBIT_CNTRST     1  // reset counters to known state
  #define SBIT_PWMEN      3  // 1 - PWM, 0 - timer
  #define SBIT_PWMMR0R    1
  #define PCPWM1          6
  #define PCLK_PWM1      12

  SBI(LPC_SC->PCONP, PCPWM1);                                             // Enable PWM1 controller (enabled on power up)
  LPC_SC->PCLKSEL0 &= ~(0x3 << PCLK_PWM1);
  LPC_SC->PCLKSEL0 |= (LPC_PWM1_PCLKSEL0 << PCLK_PWM1);

  LPC_PWM1->MR0  = LPC_PWM1_MR0;                                          // TC resets every 19,999 + 1 cycles - sets PWM cycle(Ton+Toff) to 20 mS
  // MR0 must be set before TCR enables the PWM
  LPC_PWM1->TCR  = _BV(SBIT_CNTEN) | _BV(SBIT_CNTRST) | _BV(SBIT_PWMEN);  // Enable counters, reset counters, set mode to PWM
  LPC_PWM1->TCR  &= ~(_BV(SBIT_CNTRST));                                  // Take counters out of reset
  LPC_PWM1->PR   =  LPC_PWM1_PR;
  LPC_PWM1->MCR  = _BV(SBIT_PWMMR0R) | _BV(0);                            // Reset TC if it matches MR0, disable all interrupts except for MR0
  LPC_PWM1->CTCR = 0;                                                     // Disable counter mode (enable PWM mode)
  LPC_PWM1->LER  = 0x07F;                                                 // Set the latch Enable Bits to load the new Match Values for MR0 - MR6
  LPC_PWM1->PCR  = 0;                                                     // Single edge mode for all channels, PWM1 control of outputs off

  NVIC_EnableIRQ(PWM1_IRQn);                                              // Enable interrupt handler
  NVIC_SetPriority(PWM1_IRQn, NVIC_EncodePriority(0, 10, 0));             // Normal priority for PWM module
  //NVIC_SetPriority(PWM1_IRQn, NVIC_EncodePriority(0, 0, 0));            // Minimizes jitter due to higher priority ISRs
}


bool LPC1768_PWM_attach_pin(pin_t pin, uint32_t min /* = 1 */, uint32_t max /* = (LPC_PWM1_MR0 - MR0_MARGIN) */, uint8_t servo_index /* = 0xff */) {

  pin = GET_PIN_MAP_PIN(GET_PIN_MAP_INDEX(pin & 0xFF));  // Sometimes the upper byte is garbled

  NVIC_DisableIRQ(PWM1_IRQn);    // make it safe to update the active table
                                 // OK to update the active table because the
                                 // ISR doesn't use any of the changed items
  uint8_t slot = 0;
  for (uint8_t i = 0; i < NUM_PWMS ; i++)         // see if already in table
    if (ISR_table[i].pin == pin) {
      NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts
      return 1;
    }

  for (uint8_t i = 1; (i < NUM_PWMS + 1) && !slot; i++)         // find empty slot
    if ( !(ISR_table[i - 1].set_register)) { slot = i; break; }  // any item that can't be zero when active or just attached is OK
  if (!slot) return 0;
  slot--;  // turn it into array index

  ISR_table[slot].pin          = pin;     // init slot
  ISR_table[slot].PWM_mask     = 0;  // real value set by PWM_write
  ISR_table[slot].set_register = PIN_IS_INVERTED(pin) ? &LPC_GPIO(LPC1768_PIN_PORT(pin))->FIOCLR : &LPC_GPIO(LPC1768_PIN_PORT(pin))->FIOSET;
  ISR_table[slot].clr_register = PIN_IS_INVERTED(pin) ? &LPC_GPIO(LPC1768_PIN_PORT(pin))->FIOSET : &LPC_GPIO(LPC1768_PIN_PORT(pin))->FIOCLR;
  ISR_table[slot].write_mask   = LPC_PIN(LPC1768_PIN_PIN(pin));
  ISR_table[slot].microseconds = MICRO_MAX;
  ISR_table[slot].min          = min;
  ISR_table[slot].max          = MIN(max, LPC_PWM1_MR0 - MR0_MARGIN);
  ISR_table[slot].servo_index  = servo_index;
  ISR_table[slot].active_flag  = false;

  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts

  return 1;
}


bool LPC1768_PWM_detach_pin(pin_t pin) {

  pin = GET_PIN_MAP_PIN(GET_PIN_MAP_INDEX(pin & 0xFF));

  NVIC_EnableIRQ(PWM1_IRQn);   // ?? fixes compiler problem??  ISR won't start
                               // unless put in an extra "enable"
  NVIC_DisableIRQ(PWM1_IRQn);

  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // find slot
    if (ISR_table[i].pin == pin) { slot = i; break; }
  if (slot == 0xFF) return false;    // return error if pin not found

  LPC1768_PWM_update_map_MR();

  // OK to make these changes before the MR0 interrupt
  switch(pin) {
    case P1_20:                        // Servo 0, PWM1 channel 2  (Pin 11  P1.20 PWM1.2)
      LPC_PWM1->PCR &= ~(_BV(8 + P1_20_PWM_channel));                 // disable PWM1 module control of this pin
      map_MR[P1_20_PWM_channel - 1].PCR_bit = 0;
      LPC_PINCON->PINSEL3 &= ~(0x3 <<  8);    // return pin to general purpose I/O
      map_MR[P1_20_PWM_channel - 1].PINSEL_bits = 0;
      map_MR[P1_20_PWM_channel - 1].map_PWM_INT = 0;               // 0 - available for interrupts, 1 - in use by PWM
      break;
    case P1_21:                        // Servo 1, PWM1 channel 3  (Pin 6  P1.21 PWM1.3)
      LPC_PWM1->PCR &= ~(_BV(8 + P1_21_PWM_channel));                  // disable PWM1 module control of this pin
      map_MR[P1_21_PWM_channel - 1].PCR_bit = 0;
      LPC_PINCON->PINSEL3 &= ~(0x3 << 10);  // return pin to general purpose I/O
      map_MR[P1_21_PWM_channel - 1].PINSEL_bits = 0;
      map_MR[P1_21_PWM_channel - 1].map_PWM_INT = 0;                // 0 - available for interrupts, 1 - in use by PWM
      break;
    case P1_18:                        // Servo 3, PWM1 channel 1 (Pin 4  P1.18 PWM1.1)
      LPC_PWM1->PCR &= ~(_BV(8 + P1_18_PWM_channel));                  // disable PWM1 module control of this pin
      map_MR[P1_18_PWM_channel - 1].PCR_bit =  0;
      LPC_PINCON->PINSEL3 &= ~(0x3 <<  4);  // return pin to general purpose I/O
      map_MR[P1_18_PWM_channel - 1].PINSEL_bits =  0;
      map_MR[P1_18_PWM_channel - 1].map_PWM_INT = 0;                // 0 - available for interrupts, 1 - in use by PWM
      break;
    case P2_4:                        // D9 FET, PWM1 channel 5  (Pin 9  P2_4 PWM1.5)
      LPC_PWM1->PCR &= ~(_BV(8 + P2_4_PWM_channel));                  // disable PWM1 module control of this pin
      map_MR[P2_4_PWM_channel - 1].PCR_bit = 0;
      LPC_PINCON->PINSEL4 &= ~(0x3 << 10);  // return pin to general purpose I/O
      map_MR[P2_4_PWM_channel - 1].PINSEL_bits = 0;
      map_MR[P2_4_PWM_channel - 1].map_PWM_INT = 0;                // 0 - available for interrupts, 1 - in use by PWM
      break;
    case P2_5:                        // D10 FET, PWM1 channel 6 (Pin 10  P2_5 PWM1.6)
      LPC_PWM1->PCR &= ~(_BV(8 + P2_5_PWM_channel));                  // disable PWM1 module control of this pin
      map_MR[P2_5_PWM_channel - 1].PCR_bit =  0;
      LPC_PINCON->PINSEL4 &= ~(0x3 <<  4);  // return pin to general purpose I/O
      map_MR[P2_5_PWM_channel - 1].PINSEL_bits =  0;
      map_MR[P2_5_PWM_channel - 1].map_PWM_INT = 0;                // 0 - available for interrupts, 1 - in use by PWM
      break;
    default:
      break;
  }

  ISR_table[slot] = PWM_MAP_INIT_ROW;

  ISR_table_update = true;
  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts

  return 1;
}


bool LPC1768_PWM_write(pin_t pin, uint32_t value) {

  pin = GET_PIN_MAP_PIN(GET_PIN_MAP_INDEX(pin & 0xFF));

  NVIC_DisableIRQ(PWM1_IRQn);

  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // find slot
    if (ISR_table[i].pin == pin) { slot = i; break; }
  if (slot == 0xFF) return false;    // return error if pin not found

  LPC1768_PWM_update_map_MR();

  switch(pin) {
    case P1_20:                        // Servo 0, PWM1 channel 2 (Pin 11  P1.20 PWM1.2)
      map_MR[P1_20_PWM_channel - 1].PCR_bit = _BV(8 + P1_20_PWM_channel);  // enable PWM1 module control of this pin
      map_MR[P1_20_PWM_channel - 1].PINSEL_reg = &LPC_PINCON->PINSEL3;
      map_MR[P1_20_PWM_channel - 1].PINSEL_bits = 0x2 <<  8;      // ISR must do this AFTER setting PCR
      break;
    case P1_21:                        // Servo 1, PWM1 channel 3 (Pin 6  P1.21 PWM1.3)
      map_MR[P1_21_PWM_channel - 1].PCR_bit = _BV(8 + P1_21_PWM_channel);                  // enable PWM1 module control of this pin
      map_MR[P1_21_PWM_channel - 1].PINSEL_reg = &LPC_PINCON->PINSEL3;
      map_MR[P1_21_PWM_channel - 1].PINSEL_bits = 0x2 << 10;       // ISR must do this AFTER setting PCR
      break;
    case P1_18:                        // Servo 3, PWM1 channel 1 (Pin 4  P1.18 PWM1.1)
      map_MR[P1_18_PWM_channel - 1].PCR_bit = _BV(8 + P1_18_PWM_channel);                  // enable PWM1 module control of this pin
      map_MR[P1_18_PWM_channel - 1].PINSEL_reg = &LPC_PINCON->PINSEL3;
      map_MR[P1_18_PWM_channel - 1].PINSEL_bits =  0x2 <<  4;       // ISR must do this AFTER setting PCR
      break;
    case P2_4:                        // D9 FET, PWM1 channel 5 (Pin 9  P2_4 PWM1.5)
      map_MR[P2_4_PWM_channel - 1].PCR_bit = _BV(8 + P2_4_PWM_channel);                  // enable PWM1 module control of this pin
      map_MR[P2_4_PWM_channel - 1].PINSEL_reg = &LPC_PINCON->PINSEL4;
      map_MR[P2_4_PWM_channel - 1].PINSEL_bits = 0x1 << 8;       // ISR must do this AFTER setting PCR
      break;
    case P2_5:                        // D10 FET, PWM1 channel 6 (Pin 10  P2_5 PWM1.6)
      map_MR[P2_5_PWM_channel - 1].PCR_bit = _BV(8 + P2_5_PWM_channel);                  // enable PWM1 module control of this pin
      map_MR[P2_5_PWM_channel - 1].PINSEL_reg = &LPC_PINCON->PINSEL4;
      map_MR[P2_5_PWM_channel - 1].PINSEL_bits =  0x1 <<  10;       // ISR must do this AFTER setting PCR
      break;
    default:  // ISR pins
      pinMode(pin, OUTPUT);  // set pin to output
      break;
  }

  ISR_table[slot].microseconds = MAX(MIN(value, ISR_table[slot].max), ISR_table[slot].min);
  ISR_table[slot].active_flag = 1;

  ISR_table_update = true;

  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts

  return 1;
}


uint32_t LPC1768_PWM_interrupt_mask = 1;


void LPC1768_PWM_update(void) {     // only called by the ISR
  LPC1768_PWM_interrupt_mask = 0;                          // set match registers to new values, build IRQ mask
  // first setup directly controlled PWM pin slots

  bool found;
  for (uint8_t i = 0; i < NUM_PWMS; i++) {
    ISR_table[i].PCR_bit = 0;     // clear entries
    ISR_table[i].PINSEL_reg = 0;
    ISR_table[i].PINSEL_bits = 0;
    ISR_table[i].PWM_flag = 1;    // mark slot as interrupt mode until find differently

    if (ISR_table[i].active_flag) {
      ISR_table[i].sequence = i + 1;

      // first see if there is a PWM1 controlled pin for this entry
      found = false;
      for (uint8_t j = 0; (j < NUM_PWMS) && !found; j++) {
        if ( (map_MR[j].map_PWM_PIN == ISR_table[i].pin)) {
          map_MR[j].map_PWM_INT = 1;                            // flag that it's already setup for direct control
          ISR_table[i].PWM_mask = 0;
          ISR_table[i].PCR_bit = map_MR[j].PCR_bit;            // PCR register bit to enable PWM1 control of this pin
          ISR_table[i].PINSEL_reg = map_MR[j].PINSEL_reg;  // PINSEL register address to set pin mode to PWM1 control} MR_map;
          ISR_table[i].PINSEL_bits = map_MR[j].PINSEL_bits;  // PINSEL register bits to set pin mode to PWM1 control} MR_map;
          map_MR[j].map_used = 2;
          ISR_table[i].PWM_flag = 0;
          *map_MR[j].MR_register = ISR_table[i].microseconds;
          found = true;
        }
      }
    }
    else
    ISR_table[i].sequence = 0;
  }

  // next fill in interrupt slots
  for (uint8_t i = 0; i < NUM_PWMS; i++) {

    if (ISR_table[i].active_flag && ISR_table[i].PWM_flag) {

      // setup interrupt slot
      found = false;
      for (uint8_t k = 0; (k < NUM_PWMS) && !found; k++) {
        if ( !(map_MR[k].map_PWM_INT || map_MR[k].map_used)) {
          *map_MR[k].MR_register = ISR_table[i].microseconds;  // found one for an interrupt pin
          map_MR[k].map_used = 1;
          LPC1768_PWM_interrupt_mask |= _BV(3 * (k + 1));  // set bit in the MCR to enable this MR to generate an interrupt
          ISR_table[i].set_register = PIN_IS_INVERTED(ISR_table[i].pin) ? &LPC_GPIO(LPC1768_PIN_PORT(ISR_table[i].pin))->FIOCLR : &LPC_GPIO(LPC1768_PIN_PORT(ISR_table[i].pin))->FIOSET;
          ISR_table[i].clr_register = PIN_IS_INVERTED(ISR_table[i].pin) ? &LPC_GPIO(LPC1768_PIN_PORT(ISR_table[i].pin))->FIOSET : &LPC_GPIO(LPC1768_PIN_PORT(ISR_table[i].pin))->FIOCLR;
          ISR_table[i].write_mask = LPC_PIN(LPC1768_PIN_PIN(ISR_table[i].pin));
          ISR_table[i].PWM_mask = _BV(IR_BIT(k + 1));  // bit in the IR that will go active when this MR generates an interrupt
          ISR_table[i].PWM_flag = 1;
          found = true;
        }
      }
    }
  }

  LPC1768_PWM_interrupt_mask |= (uint32_t) _BV(0);  // add in MR0 interrupt

  LPC_PWM1->LER = 0x07E;  // Set the latch Enable Bits to load the new Match Values for MR1 - MR6
}


bool useable_hardware_PWM(pin_t pin) {

  pin = GET_PIN_MAP_PIN(GET_PIN_MAP_INDEX(pin & 0xFF));

  NVIC_DisableIRQ(PWM1_IRQn);

  bool return_flag = false;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // see if it's already setup
    if (ISR_table[i].pin == pin && ISR_table[i].sequence) return_flag = true;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // see if there is an empty slot
    if (!ISR_table[i].sequence) return_flag = true;
  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts
  return return_flag;
}

////////////////////////////////////////////////////////////////////////////////

#define HAL_PWM_LPC1768_ISR  extern "C" void PWM1_IRQHandler(void)

// Both loops could be terminated when the last active channel is found but that would
// result in variations ISR run time which results in variations in pulse width

/**
 * Changes to PINSEL, PCR and MCR are only done during the MR0 interrupt otherwise
 * the wrong pin may be toggled or even have the system hang.
 */


HAL_PWM_LPC1768_ISR {

  if (LPC_PWM1->IR & 0x1) {                       // MR0 interrupt
    if (ISR_table_update) {                       // new values have been loaded so set everything
      LPC1768_PWM_update();                       // update & swap table
      LPC_PWM1->MCR = LPC1768_PWM_interrupt_mask; // enable new PWM individual channel interrupts
    }
    for (uint8_t i = 0; i < NUM_PWMS; i++) {
      if (ISR_table[i].active_flag && !((ISR_table[i].pin == P1_20) ||
                                        (ISR_table[i].pin == P1_21) ||
                                        (ISR_table[i].pin == P1_18) ||
                                        (ISR_table[i].pin == P2_4)  ||
                                        (ISR_table[i].pin == P2_5))
      ) {
        *ISR_table[i].set_register = ISR_table[i].write_mask;       // set pins for all enabled interrupt channels active
      }
      if (ISR_table_update && ISR_table[i].PCR_bit) {
        LPC_PWM1->PCR |= ISR_table[i].PCR_bit;                  // enable PWM1 module control of this pin
        *ISR_table[i].PINSEL_reg |= ISR_table[i].PINSEL_bits;   // set pin mode to PWM1 control - must be done after PCR
      }
    }
    ISR_table_update = false;
    LPC_PWM1->IR = 0x01;                                             // clear the MR0 interrupt flag bit
  }
  else {
    for (uint8_t i = 0; i < NUM_PWMS; i++)
      if (ISR_table[i].active_flag && (LPC_PWM1->IR & ISR_table[i].PWM_mask)) {
        LPC_PWM1->IR = ISR_table[i].PWM_mask;       // clear the interrupt flag bits for expected interrupts
        *ISR_table[i].clr_register = ISR_table[i].write_mask;   // set channel to inactive
      }
  }

  LPC_PWM1->IR = 0x70F;  // guarantees all interrupt flags are cleared which, if there is an unexpected
                         // PWM interrupt, will keep the ISR from hanging which will crash the controller
}

#endif

/////////////////////////////////////////////////////////////////
/////////////////  HARDWARE FIRMWARE INTERACTION ////////////////
/////////////////////////////////////////////////////////////////

/**
 *  Almost all changes to the hardware registers must be coordinated with the Match Register 0 (MR0)
 *  interrupt.  The only exception is detaching pins.  It doesn't matter when they go
 *  tristate.
 *
 *  The LPC1768_PWM_init routine kicks off the MR0 interrupt.  This interrupt is never disabled.  It
 *  can be delayed by higher priority interrupts.  Actions on directly controlled pins are not delayed
 *  by other interrupts
 *
 *  The ISR_table_update flag is set when the ISR table needs to be rebuilt.  It is
 *  cleared by the ISR during the MR0 interrupt after it rebuilds the ISR table.
 *
 *  The sequence of events during a write to a PWM channel is:
 *    1) Attach routine puts the pin number in the ISR table but doesn't mark it active.
 *    2) Write routine marks the pin as active, updates the helper table and flags the ISR that the
 *       ISR table needs to be rebuilt.
 *    3) On the MR0 interrupt the ISR:
 *         a. Rebuilds the ISR table if needed.
 *                 MR1-MR6 are updated at this time.  The updates aren't put into use until the first
 *                 MR0 after the LER register has been written.  The LER register is written during the
 *                 table rebuild process.  The result is new timing takes 20-40 mS to be implemented.
 *         b. Sets the interrupt controlled pin(s) to their active state
 *         c. Writes to the PCR register to enable the directly controlled pins
 *         d. Sets the PINSEL register to the function/mode for the directly controlled pins
 *
 *    4) For each interrupt controlled pin there is another ISR call.  During this call the pin is set
 *       to its inactive state.  The call is initiated when a MR1-MR6 reg times out.
 */
