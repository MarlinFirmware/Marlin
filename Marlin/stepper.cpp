/*
  stepper.c - stepper motor driver: executes motion plans using stepper motors
  Part of Grbl

  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The timer calculations of this module informed by the 'RepRap cartesian firmware' by Zack Smith
   and Philipp Tiefenbacher. */

#include "stepper.h"
#include "Configuration.h"
#include "Marlin.h"
#include "planner.h"
#include "pins.h"
#include "fastio.h"
#include "temperature.h"
#include "ultralcd.h"

#include "speed_lookuptable.h"

// if DEBUG_STEPS is enabled, M114 can be used to compare two methods of determining the X,Y,Z position of the printer.
// for debugging purposes only, should be disabled by default
#ifdef DEBUG_STEPS
volatile long count_position[NUM_AXIS] = { 0, 0, 0, 0};
volatile int count_direction[NUM_AXIS] = { 1, 1, 1, 1};
#endif


// intRes = intIn1 * intIn2 >> 16
// uses:
// r26 to store 0
// r27 to store the byte 1 of the 24 bit result
#define MultiU16X8toH16(intRes, charIn1, intIn2) \
asm volatile ( \
"clr r26 \n\t" \
"mul %A1, %B2 \n\t" \
"movw %A0, r0 \n\t" \
"mul %A1, %A2 \n\t" \
"add %A0, r1 \n\t" \
"adc %B0, r26 \n\t" \
"lsr r0 \n\t" \
"adc %A0, r26 \n\t" \
"adc %B0, r26 \n\t" \
"clr r1 \n\t" \
: \
"=&r" (intRes) \
: \
"d" (charIn1), \
"d" (intIn2) \
: \
"r26" \
)

// intRes = longIn1 * longIn2 >> 24
// uses:
// r26 to store 0
// r27 to store the byte 1 of the 48bit result
#define MultiU24X24toH16(intRes, longIn1, longIn2) \
asm volatile ( \
"clr r26 \n\t" \
"mul %A1, %B2 \n\t" \
"mov r27, r1 \n\t" \
"mul %B1, %C2 \n\t" \
"movw %A0, r0 \n\t" \
"mul %C1, %C2 \n\t" \
"add %B0, r0 \n\t" \
"mul %C1, %B2 \n\t" \
"add %A0, r0 \n\t" \
"adc %B0, r1 \n\t" \
"mul %A1, %C2 \n\t" \
"add r27, r0 \n\t" \
"adc %A0, r1 \n\t" \
"adc %B0, r26 \n\t" \
"mul %B1, %B2 \n\t" \
"add r27, r0 \n\t" \
"adc %A0, r1 \n\t" \
"adc %B0, r26 \n\t" \
"mul %C1, %A2 \n\t" \
"add r27, r0 \n\t" \
"adc %A0, r1 \n\t" \
"adc %B0, r26 \n\t" \
"mul %B1, %A2 \n\t" \
"add r27, r1 \n\t" \
"adc %A0, r26 \n\t" \
"adc %B0, r26 \n\t" \
"lsr r27 \n\t" \
"adc %A0, r26 \n\t" \
"adc %B0, r26 \n\t" \
"clr r1 \n\t" \
: \
"=&r" (intRes) \
: \
"d" (longIn1), \
"d" (longIn2) \
: \
"r26" , "r27" \
)

// Some useful constants

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  TIMSK1 |= (1<<OCIE1A)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() TIMSK1 &= ~(1<<OCIE1A)

static block_t *current_block;  // A pointer to the block currently being traced

// Variables used by The Stepper Driver Interrupt
static unsigned char out_bits;        // The next stepping-bits to be output
static long counter_x,       // Counter variables for the bresenham line tracer
            counter_y, 
            counter_z,       
            counter_e;
static unsigned long step_events_completed; // The number of step events executed in the current block
#ifdef ADVANCE
static long advance_rate, advance, final_advance = 0;
static short old_advance = 0;
static short e_steps;
#endif
static unsigned char busy = false; // TRUE when SIG_OUTPUT_COMPARE1A is being serviced. Used to avoid retriggering that handler.
static long acceleration_time, deceleration_time;
//static unsigned long accelerate_until, decelerate_after, acceleration_rate, initial_rate, final_rate, nominal_rate;
static unsigned short acc_step_rate; // needed for deccelaration start point
static char step_loops;


//         __________________________
//        /|                        |\     _________________         ^
//       / |                        | \   /|               |\        |
//      /  |                        |  \ / |               | \       s
//     /   |                        |   |  |               |  \      p
//    /    |                        |   |  |               |   \     e
//   +-----+------------------------+---+--+---------------+----+    e
//   |               BLOCK 1            |      BLOCK 2          |    d
//
//                           time ----->
// 
//  The trapezoid is the shape the speed curve over time. It starts at block->initial_rate, accelerates 
//  first block->accelerate_until step_events_completed, then keeps going at constant speed until 
//  step_events_completed reaches block->decelerate_after after which it decelerates until the trapezoid generator is reset.
//  The slope of acceleration is calculated with the leib ramp alghorithm.

void st_wake_up() {
  //  TCNT1 = 0;
  ENABLE_STEPPER_DRIVER_INTERRUPT();  
}

inline unsigned short calc_timer(unsigned short step_rate) {
  unsigned short timer;
  if(step_rate > MAX_STEP_FREQUENCY) step_rate = MAX_STEP_FREQUENCY;
  
  if(step_rate > 20000) { // If steprate > 20kHz >> step 4 times
    step_rate = step_rate >> 2;
    step_loops = 4;
  }
  else if(step_rate > 10000) { // If steprate > 10kHz >> step 2 times
    step_rate = step_rate >> 1;
    step_loops = 2;
  }
  else {
    step_loops = 1;
  } 
  
  if(step_rate < 32) step_rate = 32;
  step_rate -= 32; // Correct for minimal speed
  if(step_rate >= (8*256)){ // higher step rate 
    unsigned short table_address = (unsigned short)&speed_lookuptable_fast[(unsigned char)(step_rate>>8)][0];
    unsigned char tmp_step_rate = (step_rate & 0x00ff);
    unsigned short gain = (unsigned short)pgm_read_word_near(table_address+2);
    MultiU16X8toH16(timer, tmp_step_rate, gain);
    timer = (unsigned short)pgm_read_word_near(table_address) - timer;
  }
  else { // lower step rates
    unsigned short table_address = (unsigned short)&speed_lookuptable_slow[0][0];
    table_address += ((step_rate)>>1) & 0xfffc;
    timer = (unsigned short)pgm_read_word_near(table_address);
    timer -= (((unsigned short)pgm_read_word_near(table_address+2) * (unsigned char)(step_rate & 0x0007))>>3);
  }
  if(timer < 100) timer = 100;
  return timer;
}

// Initializes the trapezoid generator from the current block. Called whenever a new 
// block begins.
inline void trapezoid_generator_reset() {
#ifdef ADVANCE
  advance = current_block->initial_advance;
  final_advance = current_block->final_advance;
#endif
  deceleration_time = 0;
  // advance_rate = current_block->advance_rate;
  // step_rate to timer interval
  acc_step_rate = current_block->initial_rate;
  acceleration_time = calc_timer(acc_step_rate);
  OCR1A = acceleration_time;
}

// "The Stepper Driver Interrupt" - This timer interrupt is the workhorse.  
// It pops blocks from the block_buffer and executes them by pulsing the stepper pins appropriately. 
ISR(TIMER1_COMPA_vect)
{        
  if(busy){ Serial.print(*(unsigned short *)OCR1A); Serial.println(" BUSY");
    return; 
  } // The busy-flag is used to avoid reentering this interrupt

  busy = true;
  sei(); // Re enable interrupts (normally disabled while inside an interrupt handler)

  // If there is no current block, attempt to pop one from the buffer
  if (current_block == NULL) {
    // Anything in the buffer?
    current_block = plan_get_current_block();
    if (current_block != NULL) {
      trapezoid_generator_reset();
      counter_x = -(current_block->step_event_count >> 1);
      counter_y = counter_x;
      counter_z = counter_x;
      counter_e = counter_x;
      step_events_completed = 0;
      #ifdef ADVANCE
      e_steps = 0;
      #endif
    } 
    else {
//      DISABLE_STEPPER_DRIVER_INTERRUPT();
    }    
  } 

  if (current_block != NULL) {
    // Set directions TO DO This should be done once during init of trapezoid. Endstops -> interrupt
    out_bits = current_block->direction_bits;

#ifdef ADVANCE
    // Calculate E early.
    counter_e += current_block->steps_e;
    if (counter_e > 0) {
      counter_e -= current_block->step_event_count;
      if ((out_bits & (1<<E_AXIS)) != 0) { // - direction
        CRITICAL_SECTION_START;
        e_steps--;
        CRITICAL_SECTION_END;
      }
      else {
        CRITICAL_SECTION_START;
        e_steps++;
        CRITICAL_SECTION_END;
      }
    }    
    // Do E steps + advance steps
    CRITICAL_SECTION_START;
    e_steps += ((advance >> 16) - old_advance);
    CRITICAL_SECTION_END;
    old_advance = advance >> 16;  
#endif //ADVANCE

    // Set direction en check limit switches
if ((out_bits & (1<<X_AXIS)) != 0) {   // -direction
      WRITE(X_DIR_PIN, INVERT_X_DIR);
      #ifdef DEBUG_STEPS
      count_direction[X_AXIS]=-1;
      #endif
#if X_MIN_PIN > -1
      if(READ(X_MIN_PIN) != ENDSTOPS_INVERTING) {
        step_events_completed = current_block->step_event_count;
      }
#endif
    }
    else { // +direction 
        WRITE(X_DIR_PIN,!INVERT_X_DIR);
        #ifdef DEBUG_STEPS
        count_direction[X_AXIS]=1;
        #endif
#if X_MAX_PIN > -1
        if((READ(X_MAX_PIN) != ENDSTOPS_INVERTING)  && (current_block->steps_x >0)){
          step_events_completed = current_block->step_event_count;
        }
#endif
    }

    if ((out_bits & (1<<Y_AXIS)) != 0) {   // -direction
      WRITE(Y_DIR_PIN,INVERT_Y_DIR);
      #ifdef DEBUG_STEPS
      count_direction[Y_AXIS]=-1;
      #endif
#if Y_MIN_PIN > -1
      if(READ(Y_MIN_PIN) != ENDSTOPS_INVERTING) {
        step_events_completed = current_block->step_event_count;
      }
#endif
    }
    else { // +direction
    WRITE(Y_DIR_PIN,!INVERT_Y_DIR);
      #ifdef DEBUG_STEPS
      count_direction[Y_AXIS]=1;
      #endif
#if Y_MAX_PIN > -1
    if((READ(Y_MAX_PIN) != ENDSTOPS_INVERTING) && (current_block->steps_y >0)){
        step_events_completed = current_block->step_event_count;
      }
#endif
    }

    if ((out_bits & (1<<Z_AXIS)) != 0) {   // -direction
      WRITE(Z_DIR_PIN,INVERT_Z_DIR);
      #ifdef DEBUG_STEPS
      count_direction[Z_AXIS]=-1;
      #endif
#if Z_MIN_PIN > -1
      if(READ(Z_MIN_PIN) != ENDSTOPS_INVERTING) {
        step_events_completed = current_block->step_event_count;
      }
#endif
    }
    else { // +direction
        WRITE(Z_DIR_PIN,!INVERT_Z_DIR);
        #ifdef DEBUG_STEPS
        count_direction[Z_AXIS]=1;
        #endif
#if Z_MAX_PIN > -1
        if((READ(Z_MAX_PIN) != ENDSTOPS_INVERTING)  && (current_block->steps_z >0)){
          step_events_completed = current_block->step_event_count;
        }
#endif
    }

#ifndef ADVANCE
    if ((out_bits & (1<<E_AXIS)) != 0)   // -direction
      WRITE(E_DIR_PIN,INVERT_E_DIR);
    else // +direction
      WRITE(E_DIR_PIN,!INVERT_E_DIR);
#endif //!ADVANCE

    for(char i=0; i < step_loops; i++) { // Take multiple steps per interrupt (For high speed moves) 
      counter_x += current_block->steps_x;
      if (counter_x > 0) {
        WRITE(X_STEP_PIN, HIGH);
        counter_x -= current_block->step_event_count;
        WRITE(X_STEP_PIN, LOW);
        #ifdef DEBUG_STEPS
        count_position[X_AXIS]+=count_direction[X_AXIS];   
        #endif
      }

      counter_y += current_block->steps_y;
      if (counter_y > 0) {
        WRITE(Y_STEP_PIN, HIGH);
        counter_y -= current_block->step_event_count;
        WRITE(Y_STEP_PIN, LOW);
        #ifdef DEBUG_STEPS
        count_position[Y_AXIS]+=count_direction[Y_AXIS];
        #endif
      }

      counter_z += current_block->steps_z;
      if (counter_z > 0) {
        WRITE(Z_STEP_PIN, HIGH);
        counter_z -= current_block->step_event_count;
        WRITE(Z_STEP_PIN, LOW);
        #ifdef DEBUG_STEPS
        count_position[Z_AXIS]+=count_direction[Z_AXIS];
        #endif
      }

#ifndef ADVANCE
      counter_e += current_block->steps_e;
      if (counter_e > 0) {
        WRITE(E_STEP_PIN, HIGH);
        counter_e -= current_block->step_event_count;
        WRITE(E_STEP_PIN, LOW);
      }
#endif //!ADVANCE
      step_events_completed += 1;  
      if(step_events_completed >= current_block->step_event_count) break;
    }
    // Calculare new timer value
    unsigned short timer;
    unsigned short step_rate;
    if (step_events_completed <= current_block->accelerate_until) {
      MultiU24X24toH16(acc_step_rate, acceleration_time, current_block->acceleration_rate);
      acc_step_rate += current_block->initial_rate;
      
      // upper limit
      if(acc_step_rate > current_block->nominal_rate)
        acc_step_rate = current_block->nominal_rate;

      // step_rate to timer interval
      timer = calc_timer(acc_step_rate);
#ifdef ADVANCE
      advance += advance_rate;
#endif
      acceleration_time += timer;
      OCR1A = timer;
    } 
    else if (step_events_completed > current_block->decelerate_after) {   
      MultiU24X24toH16(step_rate, deceleration_time, current_block->acceleration_rate);
      
      if(step_rate > acc_step_rate) { // Check step_rate stays positive
        step_rate = current_block->final_rate;
      }
      else {
        step_rate = acc_step_rate - step_rate; // Decelerate from aceleration end point.
      }

      // lower limit
      if(step_rate < current_block->final_rate)
        step_rate = current_block->final_rate;

      // step_rate to timer interval
      timer = calc_timer(step_rate);
#ifdef ADVANCE
      advance -= advance_rate;
      if(advance < final_advance)
        advance = final_advance;
#endif //ADVANCE
      deceleration_time += timer;
      OCR1A = timer;
    }       
    // If current block is finished, reset pointer 
    if (step_events_completed >= current_block->step_event_count) {
      current_block = NULL;
      plan_discard_current_block();
    }   
  } 
  cli(); // disable interrupts
  busy=false;
}

#ifdef ADVANCE

unsigned char old_OCR0A;
// Timer interrupt for E. e_steps is set in the main routine;
// Timer 0 is shared with millies
ISR(TIMER0_COMPA_vect)
{
  // Critical section needed because Timer 1 interrupt has higher priority. 
  // The pin set functions are placed on trategic position to comply with the stepper driver timing.
  WRITE(E_STEP_PIN, LOW);
  // Set E direction (Depends on E direction + advance)
  if (e_steps < 0) {
    WRITE(E_DIR_PIN,INVERT_E_DIR);    
    e_steps++;
    WRITE(E_STEP_PIN, HIGH);
  } 
  if (e_steps > 0) {
    WRITE(E_DIR_PIN,!INVERT_E_DIR);
    e_steps--;
    WRITE(E_STEP_PIN, HIGH);
  }
  old_OCR0A += 25; // 10kHz interrupt
  OCR0A = old_OCR0A;
}
#endif // ADVANCE

void st_init()
{
    //Initialize Dir Pins
#if X_DIR_PIN > -1
  SET_OUTPUT(X_DIR_PIN);
#endif
#if Y_DIR_PIN > -1 
  SET_OUTPUT(Y_DIR_PIN);
#endif
#if Z_DIR_PIN > -1 
  SET_OUTPUT(Z_DIR_PIN);
#endif
#if E_DIR_PIN > -1 
  SET_OUTPUT(E_DIR_PIN);
#endif

  //Initialize Enable Pins - steppers default to disabled.

#if (X_ENABLE_PIN > -1)
  SET_OUTPUT(X_ENABLE_PIN);
  if(!X_ENABLE_ON) WRITE(X_ENABLE_PIN,HIGH);
#endif
#if (Y_ENABLE_PIN > -1)
  SET_OUTPUT(Y_ENABLE_PIN);
  if(!Y_ENABLE_ON) WRITE(Y_ENABLE_PIN,HIGH);
#endif
#if (Z_ENABLE_PIN > -1)
  SET_OUTPUT(Z_ENABLE_PIN);
  if(!Z_ENABLE_ON) WRITE(Z_ENABLE_PIN,HIGH);
#endif
#if (E_ENABLE_PIN > -1)
  SET_OUTPUT(E_ENABLE_PIN);
  if(!E_ENABLE_ON) WRITE(E_ENABLE_PIN,HIGH);
#endif

  //endstops and pullups
#ifdef ENDSTOPPULLUPS
#if X_MIN_PIN > -1
  SET_INPUT(X_MIN_PIN); 
  WRITE(X_MIN_PIN,HIGH);
#endif
#if X_MAX_PIN > -1
  SET_INPUT(X_MAX_PIN); 
  WRITE(X_MAX_PIN,HIGH);
#endif
#if Y_MIN_PIN > -1
  SET_INPUT(Y_MIN_PIN); 
  WRITE(Y_MIN_PIN,HIGH);
#endif
#if Y_MAX_PIN > -1
  SET_INPUT(Y_MAX_PIN); 
  WRITE(Y_MAX_PIN,HIGH);
#endif
#if Z_MIN_PIN > -1
  SET_INPUT(Z_MIN_PIN); 
  WRITE(Z_MIN_PIN,HIGH);
#endif
#if Z_MAX_PIN > -1
  SET_INPUT(Z_MAX_PIN); 
  WRITE(Z_MAX_PIN,HIGH);
#endif
#else //ENDSTOPPULLUPS
#if X_MIN_PIN > -1
  SET_INPUT(X_MIN_PIN); 
#endif
#if X_MAX_PIN > -1
  SET_INPUT(X_MAX_PIN); 
#endif
#if Y_MIN_PIN > -1
  SET_INPUT(Y_MIN_PIN); 
#endif
#if Y_MAX_PIN > -1
  SET_INPUT(Y_MAX_PIN); 
#endif
#if Z_MIN_PIN > -1
  SET_INPUT(Z_MIN_PIN); 
#endif
#if Z_MAX_PIN > -1
  SET_INPUT(Z_MAX_PIN); 
#endif
#endif //ENDSTOPPULLUPS
 

  //Initialize Step Pins
#if (X_STEP_PIN > -1) 
  SET_OUTPUT(X_STEP_PIN);
#endif  
#if (Y_STEP_PIN > -1) 
  SET_OUTPUT(Y_STEP_PIN);
#endif  
#if (Z_STEP_PIN > -1) 
  SET_OUTPUT(Z_STEP_PIN);
#endif  
#if (E_STEP_PIN > -1) 
  SET_OUTPUT(E_STEP_PIN);
#endif  

  // waveform generation = 0100 = CTC
  TCCR1B &= ~(1<<WGM13);
  TCCR1B |=  (1<<WGM12);
  TCCR1A &= ~(1<<WGM11); 
  TCCR1A &= ~(1<<WGM10);

  // output mode = 00 (disconnected)
  TCCR1A &= ~(3<<COM1A0); 
  TCCR1A &= ~(3<<COM1B0); 
  TCCR1B = (TCCR1B & ~(0x07<<CS10)) | (2<<CS10); // 2MHz timer

  OCR1A = 0x4000;
  DISABLE_STEPPER_DRIVER_INTERRUPT();  

#ifdef ADVANCE
  e_steps = 0;
  TIMSK0 |= (1<<OCIE0A);
#endif //ADVANCE
  sei();
}

// Block until all buffered steps are executed
void st_synchronize()
{
  while(plan_get_current_block()) {
    manage_heater();
    manage_inactivity(1);
    LCD_STATUS;
  }   
}
