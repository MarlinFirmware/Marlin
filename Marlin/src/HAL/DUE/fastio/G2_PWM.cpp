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

/**
 * The PWM module is only used to generate interrupts at specified times. It
 * is NOT used to directly toggle pins. The ISR writes to the pin assigned to
 * that interrupt.
 *
 * All PWMs use the same repetition rate.  The G2 needs about 10kHz min in order to
 * not have obvious ripple on the Vref signals.
 *
 * The data structures are setup to minimize the computation done by the ISR which
 * minimizes ISR execution time.  Execution times are 0.8 to 1.1 microseconds.
 *
 * FIve PWM interrupt sources are used.  Channel 0 sets the base period.  All Vref
 * signals are set active when this counter overflows and resets to zero.  The compare
 * values in channels 1-4 are set to give the desired duty cycle for that Vref pin.
 * When counter 0 matches the compare value then that channel generates an interrupt.
 * The ISR checks the source of the interrupt and sets the corresponding pin inactive.
 *
 * Some jitter in the Vref signal is OK so the interrupt priority is left at its default value.
 */

#include "../../../inc/MarlinConfig.h"

#if MB(PRINTRBOARD_G2)

#include "G2_PWM.h"

#if PIN_EXISTS(MOTOR_CURRENT_PWM_X)
  #define G2_PWM_X 1
#else
  #define G2_PWM_X 0
#endif
#if PIN_EXISTS(MOTOR_CURRENT_PWM_Y)
  #define G2_PWM_Y 1
#else
  #define G2_PWM_Y 0
#endif
#if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
  #define G2_PWM_Z 1
#else
  #define G2_PWM_Z 0
#endif
#if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
  #define G2_PWM_E 1
#else
  #define G2_PWM_E 0
#endif
#define G2_MASK_X(V) (G2_PWM_X * (V))
#define G2_MASK_Y(V) (G2_PWM_Y * (V))
#define G2_MASK_Z(V) (G2_PWM_Z * (V))
#define G2_MASK_E(V) (G2_PWM_E * (V))

volatile uint32_t *SODR_A = &PIOA->PIO_SODR,
                  *SODR_B = &PIOB->PIO_SODR,
                  *CODR_A = &PIOA->PIO_CODR,
                  *CODR_B = &PIOB->PIO_CODR;

PWM_map ISR_table[NUM_PWMS] = PWM_MAP_INIT;

void Stepper::digipot_init() {

  #if PIN_EXISTS(MOTOR_CURRENT_PWM_X)
    OUT_WRITE(MOTOR_CURRENT_PWM_X_PIN, 0);  // init pins
  #endif
  #if PIN_EXISTS(MOTOR_CURRENT_PWM_Y)
    OUT_WRITE(MOTOR_CURRENT_PWM_Y_PIN, 0);
  #endif
  #if G2_PWM_Z
    OUT_WRITE(MOTOR_CURRENT_PWM_Z_PIN, 0);
  #endif
  #if G2_PWM_E
    OUT_WRITE(MOTOR_CURRENT_PWM_E_PIN, 0);
  #endif

  #define WPKEY          (0x50574D << 8) // “PWM” in ASCII
  #define WPCMD_DIS_SW   0  // command to disable Write Protect SW
  #define WPRG_ALL       (PWM_WPCR_WPRG0 | PWM_WPCR_WPRG1 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3 | PWM_WPCR_WPRG4 | PWM_WPCR_WPRG5)  // all Write Protect Groups

  #define PWM_CLOCK_F    F_CPU / 1000000UL   // set clock to 1MHz

  PMC->PMC_PCER1 = PMC_PCER1_PID36;                       // enable PWM controller clock (disabled on power up)

  PWM->PWM_WPCR = WPKEY | WPRG_ALL | WPCMD_DIS_SW;        // enable setting of all PWM registers
  PWM->PWM_CLK = PWM_CLOCK_F;                             // enable CLK_A and set it to 1MHz, leave CLK_B disabled
  PWM->PWM_CH_NUM[0].PWM_CMR = 0b1011;                    // set channel 0 to Clock A input & to left aligned
  if (G2_PWM_X) PWM->PWM_CH_NUM[1].PWM_CMR = 0b1011;      // set channel 1 to Clock A input & to left aligned
  if (G2_PWM_Y) PWM->PWM_CH_NUM[2].PWM_CMR = 0b1011;      // set channel 2 to Clock A input & to left aligned
  if (G2_PWM_Z) PWM->PWM_CH_NUM[3].PWM_CMR = 0b1011;      // set channel 3 to Clock A input & to left aligned
  if (G2_PWM_E) PWM->PWM_CH_NUM[4].PWM_CMR = 0b1011;      // set channel 4 to Clock A input & to left aligned

  PWM->PWM_CH_NUM[0].PWM_CPRD = PWM_PERIOD_US;            // set channel 0 Period

  PWM->PWM_IER2 = PWM_IER1_CHID0;                         // generate interrupt when counter0 overflows
  PWM->PWM_IER2 = PWM_IER2_CMPM0
    | G2_MASK_X(PWM_IER2_CMPM1)
    | G2_MASK_Y(PWM_IER2_CMPM2)
    | G2_MASK_Z(PWM_IER2_CMPM3)
    | G2_MASK_E(PWM_IER2_CMPM4)
  ; // generate interrupt on compare event

  if (G2_PWM_X) PWM->PWM_CMP[1].PWM_CMPV = 0x010000000LL | G2_VREF_COUNT(G2_VREF(motor_current_setting[0])); // interrupt when counter0 == CMPV - used to set Motor 1 PWM inactive
  if (G2_PWM_Y) PWM->PWM_CMP[2].PWM_CMPV = 0x010000000LL | G2_VREF_COUNT(G2_VREF(motor_current_setting[0])); // interrupt when counter0 == CMPV - used to set Motor 2 PWM inactive
  if (G2_PWM_Z) PWM->PWM_CMP[3].PWM_CMPV = 0x010000000LL | G2_VREF_COUNT(G2_VREF(motor_current_setting[1])); // interrupt when counter0 == CMPV - used to set Motor 3 PWM inactive
  if (G2_PWM_E) PWM->PWM_CMP[4].PWM_CMPV = 0x010000000LL | G2_VREF_COUNT(G2_VREF(motor_current_setting[2])); // interrupt when counter0 == CMPV - used to set Motor 4 PWM inactive

  if (G2_PWM_X) PWM->PWM_CMP[1].PWM_CMPM = 0x0001;  // enable compare event
  if (G2_PWM_Y) PWM->PWM_CMP[2].PWM_CMPM = 0x0001;  // enable compare event
  if (G2_PWM_Z) PWM->PWM_CMP[3].PWM_CMPM = 0x0001;  // enable compare event
  if (G2_PWM_E) PWM->PWM_CMP[4].PWM_CMPM = 0x0001;  // enable compare event

  PWM->PWM_SCM = PWM_SCM_UPDM_MODE0 | PWM_SCM_SYNC0
    | G2_MASK_X(PWM_SCM_SYNC1)
    | G2_MASK_Y(PWM_SCM_SYNC2)
    | G2_MASK_Z(PWM_SCM_SYNC3)
    | G2_MASK_E(PWM_SCM_SYNC4)
  ; // sync 1-4 with 0, use mode 0 for updates

  PWM->PWM_ENA = PWM_ENA_CHID0
    | G2_MASK_X(PWM_ENA_CHID1)
    | G2_MASK_Y(PWM_ENA_CHID2)
    | G2_MASK_Z(PWM_ENA_CHID3)
    | G2_MASK_E(PWM_ENA_CHID4)
  ; // enable channels used by G2

  PWM->PWM_IER1 = PWM_IER1_CHID0
    | G2_MASK_X(PWM_IER1_CHID1)
    | G2_MASK_Y(PWM_IER1_CHID2)
    | G2_MASK_Z(PWM_IER1_CHID3)
    | G2_MASK_E(PWM_IER1_CHID4)
  ; // enable interrupts for channels used by G2

  NVIC_EnableIRQ(PWM_IRQn);     // Enable interrupt handler
  NVIC_SetPriority(PWM_IRQn, NVIC_EncodePriority(0, 10, 0));  // normal priority for PWM module (can stand some jitter on the Vref signals)
}

void Stepper::set_digipot_current(const uint8_t driver, const int16_t current) {

  if (!(PWM->PWM_CH_NUM[0].PWM_CPRD == PWM_PERIOD_US)) digipot_init();  // Init PWM system if needed

  switch (driver) {
    case 0:
      if (G2_PWM_X) PWM->PWM_CMP[1].PWM_CMPVUPD = 0x010000000LL | G2_VREF_COUNT(G2_VREF(current));    // update X & Y
      if (G2_PWM_Y) PWM->PWM_CMP[2].PWM_CMPVUPD = 0x010000000LL | G2_VREF_COUNT(G2_VREF(current));
      if (G2_PWM_X) PWM->PWM_CMP[1].PWM_CMPMUPD = 0x0001;  // enable compare event
      if (G2_PWM_Y) PWM->PWM_CMP[2].PWM_CMPMUPD = 0x0001;  // enable compare event
      if (G2_PWM_X || G2_PWM_Y) PWM->PWM_SCUC = PWM_SCUC_UPDULOCK; // tell the PWM controller to update the values on the next cycle
      break;
    case 1:
      if (G2_PWM_Z) {
        PWM->PWM_CMP[3].PWM_CMPVUPD = 0x010000000LL | G2_VREF_COUNT(G2_VREF(current));    // update Z
        PWM->PWM_CMP[3].PWM_CMPMUPD = 0x0001;  // enable compare event
        PWM->PWM_SCUC = PWM_SCUC_UPDULOCK; // tell the PWM controller to update the values on the next cycle
      }
      break;
    default:
      if (G2_PWM_E) {
        PWM->PWM_CMP[4].PWM_CMPVUPD = 0x010000000LL | G2_VREF_COUNT(G2_VREF(current));    // update E
        PWM->PWM_CMP[4].PWM_CMPMUPD = 0x0001;  // enable compare event
        PWM->PWM_SCUC = PWM_SCUC_UPDULOCK; // tell the PWM controller to update the values on the next cycle
      }
      break;
  }
}

volatile uint32_t PWM_ISR1_STATUS, PWM_ISR2_STATUS;

void PWM_Handler() {
  PWM_ISR1_STATUS = PWM->PWM_ISR1;
  PWM_ISR2_STATUS = PWM->PWM_ISR2;
  if (PWM_ISR1_STATUS & PWM_IER1_CHID0) {                                                         // CHAN_0 interrupt
    if (G2_PWM_X) *ISR_table[0].set_register = ISR_table[0].write_mask;                           // set X to active
    if (G2_PWM_Y) *ISR_table[1].set_register = ISR_table[1].write_mask;                           // set Y to active
    if (G2_PWM_Z) *ISR_table[2].set_register = ISR_table[2].write_mask;                           // set Z to active
    if (G2_PWM_E) *ISR_table[3].set_register = ISR_table[3].write_mask;                           // set E to active
  }
  else {
    if (G2_PWM_X && (PWM_ISR2_STATUS & PWM_IER2_CMPM1)) *ISR_table[0].clr_register = ISR_table[0].write_mask; // set X to inactive
    if (G2_PWM_Y && (PWM_ISR2_STATUS & PWM_IER2_CMPM2)) *ISR_table[1].clr_register = ISR_table[1].write_mask; // set Y to inactive
    if (G2_PWM_Z && (PWM_ISR2_STATUS & PWM_IER2_CMPM3)) *ISR_table[2].clr_register = ISR_table[2].write_mask; // set Z to inactive
    if (G2_PWM_E && (PWM_ISR2_STATUS & PWM_IER2_CMPM4)) *ISR_table[3].clr_register = ISR_table[3].write_mask; // set E to inactive
  }
  return;
}

#endif // PRINTRBOARD_G2
