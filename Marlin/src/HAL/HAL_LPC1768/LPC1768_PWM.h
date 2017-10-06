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


#ifdef TARGET_LPC1768
#include <lpc17xx_pinsel.h>

#define NUM_PWMS 6

typedef struct {            // holds all data needed to control/init one of the PWM channels
    uint8_t  sequence;       // 0: available slot, 1 - 6: PWM channel assigned to that slot
    uint8_t  logical_pin;
    uint16_t PWM_mask;       // MASK TO CHECK/WRITE THE IR REGISTER
    volatile uint32_t* set_register;
    volatile uint32_t* clr_register;
    uint32_t write_mask;     // USED BY SET/CLEAR COMMANDS
    uint32_t microseconds;   // value written to MR register
    uint32_t min;            // lower value limit checked by WRITE routine before writing to the MR register
    uint32_t max;            // upper value limit checked by WRITE routine before writing to the MR register
    bool     PWM_flag;       // 0 - USED BY sERVO, 1 - USED BY ANALOGWRITE
    uint8_t  servo_index;    // 0 - MAX_SERVO -1 : servo index,  0xFF : PWM channel
    bool     active_flag;    // THIS TABLE ENTRY IS ACTIVELY TOGGLING A PIN
    uint8_t  assigned_MR;    // Which MR (1-6) is used by this logical channel
    uint32_t PCR_bit;        // PCR register bit to enable PWM1 control of this pin
    uint32_t PINSEL3_bits;   // PINSEL3 register bits to set pin mode to PWM1 control

} PWM_map;


#define MICRO_MAX 0xffffffff

#define PWM_MAP_INIT_ROW {0, 0xff, 0, 0, 0, 0, MICRO_MAX, 0, 0, 0, 0, 0, 0, 0, 0}
#define PWM_MAP_INIT {PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                      PWM_MAP_INIT_ROW,\
                     };

PWM_map PWM1_map_A[NUM_PWMS] = PWM_MAP_INIT;
PWM_map PWM1_map_B[NUM_PWMS] = PWM_MAP_INIT;

PWM_map *active_table = PWM1_map_A;
PWM_map *work_table = PWM1_map_B;
PWM_map *ISR_table;


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


bool PWM_table_swap = false;  // flag to tell the ISR that the tables have been swapped
bool PWM_MR0_wait = false;  // flag to ensure don't delay MR0 interrupt


bool LPC1768_PWM_attach_pin(uint8_t pin, uint32_t min = 1, uint32_t max = (LPC_PWM1_MR0 - MR0_MARGIN), uint8_t servo_index = 0xff) {
  while (PWM_table_swap) delay(5);  // don't do anything until the previous change has been implemented by the ISR
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
  PWM_MR0_wait = true;
  while (PWM_MR0_wait) delay(5);  //wait until MR0 interrupt has happend so don't delay it.

  NVIC_DisableIRQ(PWM1_IRQn);
  PWM_map *pointer_swap = active_table;
  active_table = work_table;
  work_table = pointer_swap;
  PWM_table_swap = true;  // tell the ISR that the tables have been swapped
  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts

  return 1;
}

#define pin_11_PWM_channel 2
#define pin_6_PWM_channel  3
#define pin_4_PWM_channel  1

// used to keep track of which Match Registers have been used and if they will be used by the
// PWM1 module to directly control the pin or will be used to generate an interrupt
typedef struct {                        // status of PWM1 channel
    uint8_t map_used;                   // 0 - this MR register not used/assigned
    uint8_t map_PWM_INT;                // 0 - available for interrupts, 1 - in use by PWM
    uint8_t map_PWM_PIN;                // logical pin number for this PwM1 controlled pin / port
    volatile uint32_t* MR_register;     // address of the MR register for this PWM1 channel
    uint32_t PCR_bit;                   // PCR register bit to enable PWM1 control of this pin
    uint32_t PINSEL3_bits;              // PINSEL3 register bits to set pin mode to PWM1 control
} MR_map;

MR_map map_MR[NUM_PWMS];

void LPC1768_PWM_update_map_MR(void) {
  map_MR[0] = {0, (uint8_t) (LPC_PWM1->PCR & _BV(8 + pin_4_PWM_channel)  ? 1 : 0),  4, &LPC_PWM1->MR1, 0, 0};
  map_MR[1] = {0, (uint8_t) (LPC_PWM1->PCR & _BV(8 + pin_11_PWM_channel) ? 1 : 0), 11, &LPC_PWM1->MR2, 0, 0};
  map_MR[2] = {0, (uint8_t) (LPC_PWM1->PCR & _BV(8 + pin_6_PWM_channel)  ? 1 : 0),  6, &LPC_PWM1->MR3, 0, 0};
  map_MR[3] = {0, 0,  0, &LPC_PWM1->MR4, 0, 0};
  map_MR[4] = {0, 0,  0, &LPC_PWM1->MR5, 0, 0};
  map_MR[5] = {0, 0,  0, &LPC_PWM1->MR6, 0, 0};
}


uint32_t LPC1768_PWM_interrupt_mask = 1;

void LPC1768_PWM_update(void) {
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

  LPC1768_PWM_interrupt_mask = 0;                          // set match registers to new values, build IRQ mask
  for (uint8_t i = 0; i < NUM_PWMS; i++) {
    if (work_table[i].active_flag == true) {
      work_table[i].sequence = i + 1;

      // first see if there is a PWM1 controlled pin for this entry
      bool found = false;
      for (uint8_t j = 0; (j < NUM_PWMS) && !found; j++) {
        if ( (map_MR[j].map_PWM_PIN == work_table[i].logical_pin) && map_MR[j].map_PWM_INT ) {
          *map_MR[j].MR_register = work_table[i].microseconds;  // found one of the PWM pins
          work_table[i].PWM_mask = 0;
          work_table[i].PCR_bit = map_MR[j].PCR_bit;            // PCR register bit to enable PWM1 control of this pin
          work_table[i].PINSEL3_bits = map_MR[j].PINSEL3_bits;  // PINSEL3 register bits to set pin mode to PWM1 control} MR_map;
          map_MR[j].map_used = 2;
          work_table[i].assigned_MR = j +1;                    // only used to help in debugging
          found = true;
        }
      }

      // didn't find a PWM1 pin so get an interrupt
      for (uint8_t k = 0; (k < NUM_PWMS) && !found; k++) {
        if ( !(map_MR[k].map_PWM_INT || map_MR[k].map_used)) {
          *map_MR[k].MR_register = work_table[i].microseconds;  // found one for an interrupt pin
          map_MR[k].map_used = 1;
          LPC1768_PWM_interrupt_mask |= _BV(3 * (k + 1));  // set bit in the MCR to enable this MR to generate an interrupt
          work_table[i].PWM_mask = _BV(IR_BIT(k + 1));  // bit in the IR that will go active when this MR generates an interrupt
          work_table[i].assigned_MR = k +1;                // only used to help in debugging
          found = true;
        }
      }
    }
    else
      work_table[i].sequence = 0;
  }
  LPC1768_PWM_interrupt_mask |= (uint32_t) _BV(0);  // add in MR0 interrupt

   // swap tables

  PWM_MR0_wait = true;
  while (PWM_MR0_wait) delay(5);  //wait until MR0 interrupt has happend so don't delay it.

  NVIC_DisableIRQ(PWM1_IRQn);
  LPC_PWM1->LER = 0x07E;  // Set the latch Enable Bits to load the new Match Values for MR1 - MR6
  PWM_map *pointer_swap = active_table;
  active_table = work_table;
  work_table = pointer_swap;
  PWM_table_swap = true;  // tell the ISR that the tables have been swapped
  NVIC_EnableIRQ(PWM1_IRQn);  // re-enable PWM interrupts
}


bool LPC1768_PWM_write(uint8_t pin, uint32_t value) {
  while (PWM_table_swap) delay(5);  // don't do anything until the previous change has been implemented by the ISR
  COPY_ACTIVE_TABLE;  // copy active table into work table
  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // find slot
    if (work_table[i].logical_pin == pin) slot = i;
  if (slot == 0xFF) return false;    // return error if pin not found

  LPC1768_PWM_update_map_MR();

  switch(pin) {
    case 11:                        // Servo 0, PWM1 channel 2 (Pin 11  P1.20 PWM1.2)
      map_MR[pin_11_PWM_channel - 1].PCR_bit = _BV(8 + pin_11_PWM_channel);  // enable PWM1 module control of this pin
      map_MR[pin_11_PWM_channel - 1].map_PWM_INT = 1;               // 0 - available for interrupts, 1 - in use by PWM
      map_MR[pin_11_PWM_channel - 1].PINSEL3_bits = 0x2 <<  8;      // ISR must do this AFTER setting PCR
      break;
    case  6:                        // Servo 1, PWM1 channel 3 (Pin 6  P1.21 PWM1.3)
      map_MR[pin_6_PWM_channel - 1].PCR_bit = _BV(8 + pin_6_PWM_channel);                  // enable PWM1 module control of this pin
      map_MR[pin_6_PWM_channel - 1].map_PWM_INT = 1;                // 0 - available for interrupts, 1 - in use by PWM
      map_MR[pin_6_PWM_channel - 1].PINSEL3_bits = 0x2 << 10;       // ISR must do this AFTER setting PCR
      break;
    case  4:                        // Servo 3, PWM1 channel 1 (Pin 4  P1.18 PWM1.1)
      map_MR[pin_4_PWM_channel - 1].PCR_bit = _BV(8 + pin_4_PWM_channel);                  // enable PWM1 module control of this pin
      map_MR[pin_4_PWM_channel - 1].map_PWM_INT = 1;                // 0 - available for interrupts, 1 - in use by PWM
      map_MR[pin_4_PWM_channel - 1].PINSEL3_bits =  0x2 <<  4;       // ISR must do this AFTER setting PCR
      break;
    default:                                                        // ISR pins
      pinMode(pin, OUTPUT);  // set pin to output but don't write anything in case it's already in use
      break;
  }

  work_table[slot].microseconds = MAX(MIN(value, work_table[slot].max), work_table[slot].min);
  work_table[slot].active_flag = true;

  LPC1768_PWM_update();

  return 1;
}


bool LPC1768_PWM_detach_pin(uint8_t pin) {
  while (PWM_table_swap) delay(5);  // don't do anything until the previous change has been implemented by the ISR
  COPY_ACTIVE_TABLE;  // copy active table into work table
  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // find slot
    if (work_table[i].logical_pin == pin) slot = i;
  if (slot == 0xFF) return false;    // return error if pin not found

  LPC1768_PWM_update_map_MR();

  // OK to make these changes before the MR0 interrupt
  switch(pin) {
    case 11:                        // Servo 0, PWM1 channel 2  (Pin 11  P1.20 PWM1.2)
      LPC_PWM1->PCR &= ~(_BV(8 + pin_11_PWM_channel));                 // disable PWM1 module control of this pin
      map_MR[pin_11_PWM_channel - 1].PCR_bit = 0;
      LPC_PINCON->PINSEL3 &= ~(0x3 <<  8);    // return pin to general purpose I/O
      map_MR[pin_11_PWM_channel - 1].PINSEL3_bits = 0;
      map_MR[pin_11_PWM_channel - 1].map_PWM_INT = 0;               // 0 - available for interrupts, 1 - in use by PWM
      break;
    case  6:                        // Servo 1, PWM1 channel 3  (Pin 6  P1.21 PWM1.3)
      LPC_PWM1->PCR &= ~(_BV(8 + pin_6_PWM_channel));                  // disable PWM1 module control of this pin
      map_MR[pin_6_PWM_channel - 1].PCR_bit = 0;
      LPC_PINCON->PINSEL3 &= ~(0x3 << 10);  // return pin to general purpose I/O
      map_MR[pin_6_PWM_channel - 1].PINSEL3_bits = 0;
      map_MR[pin_6_PWM_channel - 1].map_PWM_INT = 0;                // 0 - available for interrupts, 1 - in use by PWM
      break;
    case  4:                        // Servo 3, PWM1 channel 1 (Pin 4  P1.18 PWM1.1)
      LPC_PWM1->PCR &= ~(_BV(8 + pin_4_PWM_channel));                  // disable PWM1 module control of this pin
      map_MR[pin_4_PWM_channel - 1].PCR_bit =  0;
      LPC_PINCON->PINSEL3 &= ~(0x3 <<  4);  // return pin to general purpose I/O
      map_MR[pin_4_PWM_channel - 1].PINSEL3_bits =  0;
      map_MR[pin_4_PWM_channel - 1].map_PWM_INT = 0;                // 0 - available for interrupts, 1 - in use by PWM
      break;
  }

  pinMode(pin, INPUT);

  work_table[slot] = PWM_MAP_INIT_ROW;

  LPC1768_PWM_update();

  return 1;
}


bool useable_hardware_PWM(uint8_t pin) {
  COPY_ACTIVE_TABLE;  // copy active table into work table
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // see if it's already setup
    if (work_table[i].logical_pin == pin && work_table[i].sequence) return true;
  for (uint8_t i = 0; i < NUM_PWMS; i++)         // see if there is an empty slot
    if (!work_table[i].sequence) return true;
  return false;    // only get here if neither the above are true
}

////////////////////////////////////////////////////////////////////////////////

#define HAL_PWM_LPC1768_ISR  extern "C" void PWM1_IRQHandler(void)


// Both loops could be terminated when the last active channel is found but that would
// result in variations ISR run time which results in variations in pulse width

/**
 * Changes to PINSEL3, PCR and MCR are only done during the MR0 interrupt otherwise
 * the wrong pin may be toggled or even have the system hang.
 */


HAL_PWM_LPC1768_ISR {
  if (PWM_table_swap) ISR_table = work_table;   // use old table if a swap was just done
  else ISR_table = active_table;

  if (LPC_PWM1->IR & 0x1) {                                      // MR0 interrupt
    ISR_table = active_table;                    // MR0 means new values could have been loaded so set everything
    if (PWM_table_swap) LPC_PWM1->MCR = LPC1768_PWM_interrupt_mask; // enable new PWM individual channel interrupts

    for (uint8_t i = 0; i < NUM_PWMS; i++) {
      if(ISR_table[i].active_flag && !((ISR_table[i].logical_pin ==  11) ||
                                       (ISR_table[i].logical_pin ==  4) ||
                                       (ISR_table[i].logical_pin ==  6)))
        *ISR_table[i].set_register = ISR_table[i].write_mask;       // set pins for all enabled interrupt channels active
      if (PWM_table_swap && ISR_table[i].PCR_bit) {
        LPC_PWM1->PCR |= ISR_table[i].PCR_bit;              // enable PWM1 module control of this pin
        LPC_PINCON->PINSEL3 |= ISR_table[i].PINSEL3_bits;   // set pin mode to PWM1 control - must be done after PCR
      }
    }
    PWM_table_swap = false;
    PWM_MR0_wait = false;
    LPC_PWM1->IR = 0x01;                                             // clear the MR0 interrupt flag bit
  }
  else {
    for (uint8_t i = 0; i < NUM_PWMS ; i++)
      if (ISR_table[i].active_flag && (LPC_PWM1->IR & ISR_table[i].PWM_mask) ){
        LPC_PWM1->IR = ISR_table[i].PWM_mask;       // clear the interrupt flag bits for expected interrupts
        *ISR_table[i].clr_register = ISR_table[i].write_mask;   // set channel to inactive
      }
  }

  LPC_PWM1->IR = 0x70F;  // guarantees all interrupt flags are cleared which, if there is an unexpected
                           // PWM interrupt, will keep the ISR from hanging which will crash the controller

return;
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
 *  The LPC1768_PWM_init routine kicks off the MR0 interrupt.  This interrupt is never disabled or
 *  delayed.
 *
 *  The PWM_table_swap flag is set when the firmware has swapped in an updated table.  It is
 *  cleared by the ISR during the MR0 interrupt as it completes the swap and accompanying updates.
 *  It serves two purposes:
 *    1) Tells the ISR that the tables have been swapped
 *    2) Keeps the firmware from starting a new update until the previous one has been completed.
 *
 *  The PWM_MR0_wait flag is set when the firmware is ready to swap in an updated table and cleared by
 *  the ISR during the MR0 interrupt.  It is used to avoid delaying the MR0 interrupt when swapping in
 *  an updated table.  This avoids glitches in pulse width and/or repetition rate.
 *
 *  The sequence of events during a write to a PWM channel is:
 *    1) Waits until PWM_table_swap flag is false before starting
 *    2) Copies the active table into the work table
 *    3) Updates the work table
 *         NOTES - MR1-MR6 are updated at this time.  The updates aren't put into use until the first
 *                 MR0 after the LER register has been written.  The LER register is written during the
 *                 table swap process.
 *               - The MCR mask is created at this time.  It is not used until the ISR writes the MCR
 *                 during the MR0 interrupt in the table swap process.
 *    4) Sets the PWM_MR0_wait flag
 *    5) ISR clears the PWM_MR0_wait flag during the next MR0 interrupt
 *    6) Once the PWM_MR0_wait flag is cleared then the firmware:
 *          disables the ISR interrupt
 *          swaps the pointers to the tables
 *          writes to the LER register
 *          sets the PWM_table_swap flag active
 *          re-enables the ISR
 *     7) On the next interrupt the ISR changes its pointer to the work table which is now the old,
 *        unmodified, active table.
 *     8) On the next MR0 interrupt the ISR:
 *          switches over to the active table
 *          clears the PWM_table_swap and PWM_MR0_wait flags
 *          updates the MCR register with the possibly new interrupt sources/assignments
 *          writes to the PCR register to enable the direct control of the Servo 0, 1 & 3 pins by the PWM1 module
 *          sets the PINSEL3 register to function/mode 0x2 for the Servo 0, 1 & 3 pins
 *             NOTE - PCR must be set before PINSEL
 *          sets the pins controlled by the ISR to their active states
 */

