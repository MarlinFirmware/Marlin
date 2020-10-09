/**
 * TMC26XStepper.cpp - - TMC26X Stepper library for Wiring/Arduino
 *
 * based on the stepper library by Tom Igoe, et. al.
 *
 * Copyright (c) 2011, Interactive Matter, Marcus Nowotny
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if defined(STM32GENERIC) && defined(STM32F7)

#include "../../../inc/MarlinConfigPre.h"

#if HAS_DRIVER(TMC2660)

#include <stdbool.h>
#include <SPI.h>
#include "TMC2660.h"

#include "../../../inc/MarlinConfig.h"
#include "../../../MarlinCore.h"
#include "../../../module/stepper/indirection.h"
#include "../../../module/printcounter.h"
#include "../../../libs/duration_t.h"
#include "../../../libs/hex_print.h"

//some default values used in initialization
#define DEFAULT_MICROSTEPPING_VALUE 32

//TMC26X register definitions
#define DRIVER_CONTROL_REGISTER 0x0ul
#define CHOPPER_CONFIG_REGISTER 0x80000ul
#define COOL_STEP_REGISTER  0xA0000ul
#define STALL_GUARD2_LOAD_MEASURE_REGISTER 0xC0000ul
#define DRIVER_CONFIG_REGISTER 0xE0000ul

#define REGISTER_BIT_PATTERN 0xFFFFFul

//definitions for the driver control register
#define MICROSTEPPING_PATTERN 0xFul
#define STEP_INTERPOLATION 0x200ul
#define DOUBLE_EDGE_STEP 0x100ul
#define VSENSE 0x40ul
#define READ_MICROSTEP_POSTION 0x0ul
#define READ_STALL_GUARD_READING 0x10ul
#define READ_STALL_GUARD_AND_COOL_STEP 0x20ul
#define READ_SELECTION_PATTERN 0x30ul

//definitions for the chopper config register
#define CHOPPER_MODE_STANDARD 0x0ul
#define CHOPPER_MODE_T_OFF_FAST_DECAY 0x4000ul
#define T_OFF_PATTERN 0xFul
#define RANDOM_TOFF_TIME 0x2000ul
#define BLANK_TIMING_PATTERN 0x18000ul
#define BLANK_TIMING_SHIFT 15
#define HYSTERESIS_DECREMENT_PATTERN 0x1800ul
#define HYSTERESIS_DECREMENT_SHIFT 11
#define HYSTERESIS_LOW_VALUE_PATTERN 0x780ul
#define HYSTERESIS_LOW_SHIFT 7
#define HYSTERESIS_START_VALUE_PATTERN 0x78ul
#define HYSTERESIS_START_VALUE_SHIFT 4
#define T_OFF_TIMING_PATERN 0xFul

//definitions for cool step register
#define MINIMUM_CURRENT_FOURTH 0x8000ul
#define CURRENT_DOWN_STEP_SPEED_PATTERN 0x6000ul
#define SE_MAX_PATTERN 0xF00ul
#define SE_CURRENT_STEP_WIDTH_PATTERN 0x60ul
#define SE_MIN_PATTERN 0xFul

//definitions for StallGuard2 current register
#define STALL_GUARD_FILTER_ENABLED 0x10000ul
#define STALL_GUARD_TRESHHOLD_VALUE_PATTERN 0x17F00ul
#define CURRENT_SCALING_PATTERN 0x1Ful
#define STALL_GUARD_CONFIG_PATTERN 0x17F00ul
#define STALL_GUARD_VALUE_PATTERN 0x7F00ul

//definitions for the input from the TMC2660
#define STATUS_STALL_GUARD_STATUS 0x1ul
#define STATUS_OVER_TEMPERATURE_SHUTDOWN 0x2ul
#define STATUS_OVER_TEMPERATURE_WARNING 0x4ul
#define STATUS_SHORT_TO_GROUND_A 0x8ul
#define STATUS_SHORT_TO_GROUND_B 0x10ul
#define STATUS_OPEN_LOAD_A 0x20ul
#define STATUS_OPEN_LOAD_B 0x40ul
#define STATUS_STAND_STILL 0x80ul
#define READOUT_VALUE_PATTERN 0xFFC00ul

#define CPU_32_BIT

//default values
#define INITIAL_MICROSTEPPING 0x3ul //32th microstepping

SPIClass SPI_6(SPI6, SPI6_MOSI_PIN, SPI6_MISO_PIN, SPI6_SCK_PIN);

#define STEPPER_SPI SPI_6

//debuging output

//#define TMC_DEBUG1

uint8_t current_scaling = 0;

/**
 * Constructor
 * number_of_steps - the steps per rotation
 * cs_pin - the SPI client select pin
 * dir_pin - the pin where the direction pin is connected
 * step_pin - the pin where the step pin is connected
 */
TMC26XStepper::TMC26XStepper(const int16_t in_steps, int16_t cs_pin, int16_t dir_pin, int16_t step_pin, uint16_t current, uint16_t resistor) {
  // We are not started yet
  started = false;

  // By default cool step is not enabled
  cool_step_enabled = false;

  // Save the pins for later use
  this->cs_pin = cs_pin;
  this->dir_pin = dir_pin;
  this->step_pin = step_pin;

  // Store the current sense resistor value for later use
  this->resistor = resistor;

  // Initizalize our status values
  this->steps_left = 0;
  this->direction = 0;

  // Initialize register values
  driver_control_register_value = DRIVER_CONTROL_REGISTER | INITIAL_MICROSTEPPING;
  chopper_config_register = CHOPPER_CONFIG_REGISTER;

  // Setting the default register values
  driver_control_register_value = DRIVER_CONTROL_REGISTER|INITIAL_MICROSTEPPING;
  microsteps = _BV(INITIAL_MICROSTEPPING);
  chopper_config_register = CHOPPER_CONFIG_REGISTER;
  cool_step_register_value = COOL_STEP_REGISTER;
  stallguard2_current_register_value = STALL_GUARD2_LOAD_MEASURE_REGISTER;
  driver_configuration_register_value = DRIVER_CONFIG_REGISTER | READ_STALL_GUARD_READING;

  // Set the current
  setCurrent(current);
  // Set to a conservative start value
  setConstantOffTimeChopper(7, 54, 13,12,1);
  // Set a nice microstepping value
  setMicrosteps(DEFAULT_MICROSTEPPING_VALUE);
  // Save the number of steps
  number_of_steps = in_steps;
}


/**
 * start & configure the stepper driver
 * just must be called.
 */
void TMC26XStepper::start() {

  #ifdef TMC_DEBUG1
    SERIAL_ECHOLNPGM("\n  TMC26X stepper library");
    SERIAL_ECHOPAIR("\n  CS pin: ", cs_pin);
    SERIAL_ECHOPAIR("\n  DIR pin: ", dir_pin);
    SERIAL_ECHOPAIR("\n  STEP pin: ", step_pin);
    SERIAL_PRINTF("\n  current scaling: %d", current_scaling);
    SERIAL_PRINTF("\n  Resistor: %d", resistor);
    //SERIAL_PRINTF("\n  current: %d", current);
    SERIAL_ECHOPAIR("\n  Microstepping: ", microsteps);
  #endif

  //set the pins as output & its initial value
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(cs_pin, OUTPUT);
  extDigitalWrite(step_pin, LOW);
  extDigitalWrite(dir_pin, LOW);
  extDigitalWrite(cs_pin, HIGH);

  STEPPER_SPI.begin();
  STEPPER_SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE3));

  //set the initial values
  send262(driver_control_register_value);
  send262(chopper_config_register);
  send262(cool_step_register_value);
  send262(stallguard2_current_register_value);
  send262(driver_configuration_register_value);

  //save that we are in running mode
  started = true;
}

/**
 * Mark the driver as unstarted to be able to start it again
 */
void TMC26XStepper::un_start() { started = false; }


/**
 * Sets the speed in revs per minute
 */
void TMC26XStepper::setSpeed(uint16_t whatSpeed) {
  this->speed = whatSpeed;
  this->step_delay = 60UL * sq(1000UL) / ((uint32_t)this->number_of_steps * (uint32_t)whatSpeed * (uint32_t)this->microsteps);
  #ifdef TMC_DEBUG0 // crashes
    SERIAL_ECHOPAIR("\nStep delay in micros: ", this->step_delay);
  #endif
  // Update the next step time
  this->next_step_time = this->last_step_time + this->step_delay;
}

uint16_t TMC26XStepper::getSpeed() { return this->speed; }

/**
 * Moves the motor steps_to_move steps.
 * Negative indicates the reverse direction.
 */
char TMC26XStepper::step(int16_t steps_to_move) {
  if (this->steps_left == 0) {
    this->steps_left = ABS(steps_to_move);  // how many steps to take

    // determine direction based on whether steps_to_move is + or -:
    if (steps_to_move > 0)
      this->direction = 1;
    else if (steps_to_move < 0)
      this->direction = 0;
    return 0;
  }
  return -1;
}

char TMC26XStepper::move() {
  // decrement the number of steps, moving one step each time:
  if (this->steps_left > 0) {
    uint32_t time = micros();
    // move only if the appropriate delay has passed:

    // rem if (time >= this->next_step_time) {

    if (ABS(time - this->last_step_time) > this->step_delay) {
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1)
        extDigitalWrite(step_pin, HIGH);
      else {
        extDigitalWrite(dir_pin, HIGH);
        extDigitalWrite(step_pin, HIGH);
      }
      // get the timeStamp of when you stepped:
      this->last_step_time = time;
      this->next_step_time = time + this->step_delay;
      // decrement the steps left:
      steps_left--;
      //disable the step & dir pins
      extDigitalWrite(step_pin, LOW);
      extDigitalWrite(dir_pin, LOW);
    }
    return -1;
  }
  return 0;
}

char TMC26XStepper::isMoving() { return this->steps_left > 0; }

uint16_t TMC26XStepper::getStepsLeft() { return this->steps_left; }

char TMC26XStepper::stop() {
  //note to self if the motor is currently moving
  char state = isMoving();
  //stop the motor
  this->steps_left = 0;
  this->direction = 0;
  //return if it was moving
  return state;
}

void TMC26XStepper::setCurrent(uint16_t current) {
  uint8_t current_scaling = 0;
  //calculate the current scaling from the max current setting (in mA)
  float mASetting = (float)current,
         resistor_value = (float)this->resistor;
  // remove vsense flag
  this->driver_configuration_register_value &= ~(VSENSE);
  // Derived from I = (cs + 1) / 32 * (Vsense / Rsense)
  //   leading to cs = 32 * R * I / V (with V = 0,31V oder 0,165V and I = 1000 * current)
  // with Rsense = 0,15
  // for vsense = 0,310V (VSENSE not set)
  // or vsense = 0,165V (VSENSE set)
  current_scaling = (byte)((resistor_value * mASetting * 32.0 / (0.31 * sq(1000.0))) - 0.5); //theoretically - 1.0 for better rounding it is 0.5

  // Check if the current scalingis too low
  if (current_scaling < 16) {
    // Set the csense bit to get a use half the sense voltage (to support lower motor currents)
    this->driver_configuration_register_value |= VSENSE;
    // and recalculate the current setting
    current_scaling = (byte)((resistor_value * mASetting * 32.0 / (0.165 * sq(1000.0))) - 0.5); //theoretically - 1.0 for better rounding it is 0.5
    #ifdef TMC_DEBUG0 // crashes
        SERIAL_ECHOPAIR("\nCS (Vsense=1): ",current_scaling);
    #endif
  }
  #ifdef TMC_DEBUG0 // crashes
    else
      SERIAL_ECHOPAIR("\nCS: ", current_scaling);
  #endif

  // do some sanity checks
  NOMORE(current_scaling, 31);

  // delete the old value
  stallguard2_current_register_value &= ~(CURRENT_SCALING_PATTERN);
  // set the new current scaling
  stallguard2_current_register_value |= current_scaling;
  // if started we directly send it to the motor
  if (started) {
    send262(driver_configuration_register_value);
    send262(stallguard2_current_register_value);
  }
}

uint16_t TMC26XStepper::getCurrent() {
  // Calculate the current according to the datasheet to be on the safe side.
  // This is not the fastest but the most accurate and illustrative way.
  float result = (float)(stallguard2_current_register_value & CURRENT_SCALING_PATTERN),
         resistor_value = (float)this->resistor,
         voltage = (driver_configuration_register_value & VSENSE) ? 0.165 : 0.31;
  result = (result + 1.0) / 32.0 * voltage / resistor_value * sq(1000.0);
  return (uint16_t)result;
}

void TMC26XStepper::setStallGuardThreshold(char stallguard_threshold, char stallguard_filter_enabled) {
  // We just have 5 bits
  LIMIT(stallguard_threshold, -64, 63);

  // Add trim down to 7 bits
  stallguard_threshold &= 0x7F;
  // Delete old StallGuard settings
  stallguard2_current_register_value &= ~(STALL_GUARD_CONFIG_PATTERN);
  if (stallguard_filter_enabled)
    stallguard2_current_register_value |= STALL_GUARD_FILTER_ENABLED;

  // Set the new StallGuard threshold
  stallguard2_current_register_value |= (((uint32_t)stallguard_threshold << 8) & STALL_GUARD_CONFIG_PATTERN);
  // If started we directly send it to the motor
  if (started) send262(stallguard2_current_register_value);
}

char TMC26XStepper::getStallGuardThreshold() {
  uint32_t stallguard_threshold = stallguard2_current_register_value & STALL_GUARD_VALUE_PATTERN;
  //shift it down to bit 0
  stallguard_threshold >>= 8;
  //convert the value to an int16_t to correctly handle the negative numbers
  char result = stallguard_threshold;
  //check if it is negative and fill it up with leading 1 for proper negative number representation
  //rem if (result & _BV(6)) {

  if (TEST(result, 6)) result |= 0xC0;
  return result;
}

char TMC26XStepper::getStallGuardFilter() {
  if (stallguard2_current_register_value & STALL_GUARD_FILTER_ENABLED)
    return -1;
  return 0;
}

/**
 * Set the number of microsteps per step.
 * 0,2,4,8,16,32,64,128,256 is supported
 * any value in between will be mapped to the next smaller value
 * 0 and 1 set the motor in full step mode
 */
void TMC26XStepper::setMicrosteps(const int16_t in_steps) {
  uint16_t setting_pattern;

       if (in_steps >= 256) setting_pattern = 0;
  else if (in_steps >= 128) setting_pattern = 1;
  else if (in_steps >=  64) setting_pattern = 2;
  else if (in_steps >=  32) setting_pattern = 3;
  else if (in_steps >=  16) setting_pattern = 4;
  else if (in_steps >=   8) setting_pattern = 5;
  else if (in_steps >=   4) setting_pattern = 6;
  else if (in_steps >=   2) setting_pattern = 7;
  else if (in_steps <=   1) setting_pattern = 8; // 1 and 0 lead to full step

  microsteps = _BV(8 - setting_pattern);

  #ifdef TMC_DEBUG0 // crashes
    SERIAL_ECHOPAIR("\n Microstepping: ", microsteps);
  #endif

  // Delete the old value
  this->driver_control_register_value &= 0x000FFFF0UL;

  // Set the new value
  this->driver_control_register_value |= setting_pattern;

  // If started we directly send it to the motor
  if (started) send262(driver_control_register_value);

  // Recalculate the stepping delay by simply setting the speed again
  this->setSpeed(this->speed);
}

/**
 * returns the effective number of microsteps at the moment
 */
int16_t TMC26XStepper::getMicrosteps() { return microsteps; }

/**
 * constant_off_time: The off time setting controls the minimum chopper frequency.
 * For most applications an off time within the range of 5μs to 20μs will fit.
 *    2...15: off time setting
 *
 * blank_time: Selects the comparator blank time. This time needs to safely cover the switching event and the
 * duration of the ringing on the sense resistor. For
 *    0: min. setting 3: max. setting
 *
 * fast_decay_time_setting: Fast decay time setting. With CHM=1, these bits control the portion of fast decay for each chopper cycle.
 *    0: slow decay only
 *    1...15: duration of fast decay phase
 *
 * sine_wave_offset: Sine wave offset. With CHM=1, these bits control the sine wave offset.
 * A positive offset corrects for zero crossing error.
 *    -3..-1: negative offset 0: no offset 1...12: positive offset
 *
 * use_current_comparator: Selects usage of the current comparator for termination of the fast decay cycle.
 * If current comparator is enabled, it terminates the fast decay cycle in case the current
 * reaches a higher negative value than the actual positive value.
 *    1: enable comparator termination of fast decay cycle
 *    0: end by time only
 */
void TMC26XStepper::setConstantOffTimeChopper(char constant_off_time, char blank_time, char fast_decay_time_setting, char sine_wave_offset, uint8_t use_current_comparator) {
  // Perform some sanity checks
  LIMIT(constant_off_time, 2, 15);

  // Save the constant off time
  this->constant_off_time = constant_off_time;

  // Calculate the value acc to the clock cycles
  const char blank_value = blank_time >= 54 ? 3 :
                           blank_time >= 36 ? 2 :
                           blank_time >= 24 ? 1 : 0;

  LIMIT(fast_decay_time_setting, 0, 15);
  LIMIT(sine_wave_offset, -3, 12);

  // Shift the sine_wave_offset
  sine_wave_offset += 3;

  // Calculate the register setting
  // First of all delete all the values for this
  chopper_config_register &= ~(_BV(12) | BLANK_TIMING_PATTERN | HYSTERESIS_DECREMENT_PATTERN | HYSTERESIS_LOW_VALUE_PATTERN | HYSTERESIS_START_VALUE_PATTERN | T_OFF_TIMING_PATERN);
  // Set the constant off pattern
  chopper_config_register |= CHOPPER_MODE_T_OFF_FAST_DECAY;
  // Set the blank timing value
  chopper_config_register |= ((uint32_t)blank_value) << BLANK_TIMING_SHIFT;
  // Setting the constant off time
  chopper_config_register |= constant_off_time;
  // Set the fast decay time
  // Set msb
  chopper_config_register |= (((uint32_t)(fast_decay_time_setting & 0x8)) << HYSTERESIS_DECREMENT_SHIFT);
  // Other bits
  chopper_config_register |= (((uint32_t)(fast_decay_time_setting & 0x7)) << HYSTERESIS_START_VALUE_SHIFT);
  // Set the sine wave offset
  chopper_config_register |= (uint32_t)sine_wave_offset << HYSTERESIS_LOW_SHIFT;
  // Using the current comparator?
  if (!use_current_comparator)
    chopper_config_register |= _BV(12);

  // If started we directly send it to the motor
  if (started) {
    // rem send262(driver_control_register_value);
    send262(chopper_config_register);
  }
}

/**
 * constant_off_time: The off time setting controls the minimum chopper frequency.
 * For most applications an off time within the range of 5μs to 20μs will fit.
 *    2...15: off time setting
 *
 * blank_time: Selects the comparator blank time. This time needs to safely cover the switching event and the
 * duration of the ringing on the sense resistor. For
 *    0: min. setting 3: max. setting
 *
 * hysteresis_start: Hysteresis start setting. Please remark, that this value is an offset to the hysteresis end value HEND.
 *    1...8
 *
 * hysteresis_end: Hysteresis end setting. Sets the hysteresis end value after a number of decrements. Decrement interval time is controlled by HDEC.
 * The sum HSTRT+HEND must be <16. At a current setting CS of max. 30 (amplitude reduced to 240), the sum is not limited.
 *    -3..-1: negative HEND 0: zero HEND 1...12: positive HEND
 *
 * hysteresis_decrement: Hysteresis decrement setting. This setting determines the slope of the hysteresis during on time and during fast decay time.
 *    0: fast decrement 3: very slow decrement
 */

void TMC26XStepper::setSpreadCycleChopper(char constant_off_time, char blank_time, char hysteresis_start, char hysteresis_end, char hysteresis_decrement) {
  // Perform some sanity checks
  LIMIT(constant_off_time, 2, 15);

  // Save the constant off time
  this->constant_off_time = constant_off_time;

  // Calculate the value acc to the clock cycles
  const char blank_value = blank_time >= 54 ? 3 :
                           blank_time >= 36 ? 2 :
                           blank_time >= 24 ? 1 : 0;

  LIMIT(hysteresis_start, 1, 8);
  hysteresis_start--;

  LIMIT(hysteresis_start, -3, 12);

  // Shift the hysteresis_end
  hysteresis_end += 3;

  LIMIT(hysteresis_decrement, 0, 3);

  //first of all delete all the values for this
  chopper_config_register &= ~(CHOPPER_MODE_T_OFF_FAST_DECAY | BLANK_TIMING_PATTERN | HYSTERESIS_DECREMENT_PATTERN | HYSTERESIS_LOW_VALUE_PATTERN | HYSTERESIS_START_VALUE_PATTERN | T_OFF_TIMING_PATERN);

  //set the blank timing value
  chopper_config_register |= ((uint32_t)blank_value) << BLANK_TIMING_SHIFT;
  //setting the constant off time
  chopper_config_register |= constant_off_time;
  //set the hysteresis_start
  chopper_config_register |= ((uint32_t)hysteresis_start) << HYSTERESIS_START_VALUE_SHIFT;
  //set the hysteresis end
  chopper_config_register |= ((uint32_t)hysteresis_end) << HYSTERESIS_LOW_SHIFT;
  //set the hystereis decrement
  chopper_config_register |= ((uint32_t)blank_value) << BLANK_TIMING_SHIFT;
  //if started we directly send it to the motor
  if (started) {
    //rem send262(driver_control_register_value);
    send262(chopper_config_register);
  }
}

/**
 * In a constant off time chopper scheme both coil choppers run freely, i.e. are not synchronized.
 * The frequency of each chopper mainly depends on the coil current and the position dependant motor coil inductivity, thus it depends on the microstep position.
 * With some motors a slightly audible beat can occur between the chopper frequencies, especially when they are near to each other. This typically occurs at a
 * few microstep positions within each quarter wave. This effect normally is not audible when compared to mechanical noise generated by ball bearings, etc.
 * Further factors which can cause a similar effect are a poor layout of sense resistor GND connection.
 * Hint: A common factor, which can cause motor noise, is a bad PCB layout causing coupling of both sense resistor voltages
 * (please refer to sense resistor layout hint in chapter 8.1).
 * In order to minimize the effect of a beat between both chopper frequencies, an internal random generator is provided.
 * It modulates the slow decay time setting when switched on by the RNDTF bit. The RNDTF feature further spreads the chopper spectrum,
 * reducing electromagnetic emission on single frequencies.
 */
void TMC26XStepper::setRandomOffTime(char value) {
  if (value)
    chopper_config_register |= RANDOM_TOFF_TIME;
  else
    chopper_config_register &= ~(RANDOM_TOFF_TIME);
  //if started we directly send it to the motor
  if (started) {
    //rem send262(driver_control_register_value);
    send262(chopper_config_register);
  }
}

void TMC26XStepper::setCoolStepConfiguration(
  uint16_t lower_SG_threshold,
  uint16_t SG_hysteresis,
  uint8_t current_decrement_step_size,
  uint8_t current_increment_step_size,
  uint8_t lower_current_limit
) {
  // Sanitize the input values
  NOMORE(lower_SG_threshold, 480);
  // Divide by 32
  lower_SG_threshold >>= 5;
  NOMORE(SG_hysteresis, 480);
  // Divide by 32
  SG_hysteresis >>= 5;
  NOMORE(current_decrement_step_size, 3);
  NOMORE(current_increment_step_size, 3);
  NOMORE(lower_current_limit, 1);

  // Store the lower level in order to enable/disable the cool step
  this->cool_step_lower_threshold=lower_SG_threshold;
  // If cool step is not enabled we delete the lower value to keep it disabled
  if (!this->cool_step_enabled) lower_SG_threshold = 0;
  // The good news is that we can start with a complete new cool step register value
  // And simply set the values in the register
  cool_step_register_value = ((uint32_t)lower_SG_threshold)
                          | (((uint32_t)SG_hysteresis) << 8)
                          | (((uint32_t)current_decrement_step_size) << 5)
                          | (((uint32_t)current_increment_step_size) << 13)
                          | (((uint32_t)lower_current_limit) << 15)
                          | COOL_STEP_REGISTER; // Register signature

  if (started) send262(cool_step_register_value);
}

void TMC26XStepper::setCoolStepEnabled(boolean enabled) {
  // Simply delete the lower limit to disable the cool step
  cool_step_register_value &= ~SE_MIN_PATTERN;
  // And set it to the proper value if cool step is to be enabled
  if (enabled)
    cool_step_register_value |= this->cool_step_lower_threshold;
  // And save the enabled status
  this->cool_step_enabled = enabled;
  // Save the register value
  if (started) send262(cool_step_register_value);
}

boolean TMC26XStepper::isCoolStepEnabled() { return this->cool_step_enabled; }

uint16_t TMC26XStepper::getCoolStepLowerSgThreshold() {
  // We return our internally stored value - in order to provide the correct setting even if cool step is not enabled
  return this->cool_step_lower_threshold<<5;
}

uint16_t TMC26XStepper::getCoolStepUpperSgThreshold() {
  return uint8_t((cool_step_register_value & SE_MAX_PATTERN) >> 8) << 5;
}

uint8_t TMC26XStepper::getCoolStepCurrentIncrementSize() {
  return uint8_t((cool_step_register_value & CURRENT_DOWN_STEP_SPEED_PATTERN) >> 13);
}

uint8_t TMC26XStepper::getCoolStepNumberOfSGReadings() {
  return uint8_t((cool_step_register_value & SE_CURRENT_STEP_WIDTH_PATTERN) >> 5);
}

uint8_t TMC26XStepper::getCoolStepLowerCurrentLimit() {
  return uint8_t((cool_step_register_value & MINIMUM_CURRENT_FOURTH) >> 15);
}

void TMC26XStepper::setEnabled(boolean enabled) {
  //delete the t_off in the chopper config to get sure
  chopper_config_register &= ~(T_OFF_PATTERN);
  if (enabled) {
    //and set the t_off time
    chopper_config_register |= this->constant_off_time;
  }
  //if not enabled we don't have to do anything since we already delete t_off from the register
  if (started) send262(chopper_config_register);
}

boolean TMC26XStepper::isEnabled() { return !!(chopper_config_register & T_OFF_PATTERN); }

/**
 * reads a value from the TMC26X status register. The value is not obtained directly but can then
 * be read by the various status routines.
 */
void TMC26XStepper::readStatus(char read_value) {
  uint32_t old_driver_configuration_register_value = driver_configuration_register_value;
  //reset the readout configuration
  driver_configuration_register_value &= ~(READ_SELECTION_PATTERN);
  //this now equals TMC26X_READOUT_POSITION - so we just have to check the other two options
  if (read_value == TMC26X_READOUT_STALLGUARD)
    driver_configuration_register_value |= READ_STALL_GUARD_READING;
  else if (read_value == TMC26X_READOUT_CURRENT)
    driver_configuration_register_value |= READ_STALL_GUARD_AND_COOL_STEP;

  //all other cases are ignored to prevent funny values
  //check if the readout is configured for the value we are interested in
  if (driver_configuration_register_value != old_driver_configuration_register_value) {
    //because then we need to write the value twice - one time for configuring, second time to get the value, see below
    send262(driver_configuration_register_value);
  }
  //write the configuration to get the last status
  send262(driver_configuration_register_value);
}

int16_t TMC26XStepper::getMotorPosition() {
  //we read it out even if we are not started yet - perhaps it is useful information for somebody
  readStatus(TMC26X_READOUT_POSITION);
  return getReadoutValue();
}

//reads the StallGuard setting from last status
//returns -1 if StallGuard information is not present
int16_t TMC26XStepper::getCurrentStallGuardReading() {
  //if we don't yet started there cannot be a StallGuard value
  if (!started) return -1;
  //not time optimal, but solution optiomal:
  //first read out the StallGuard value
  readStatus(TMC26X_READOUT_STALLGUARD);
  return getReadoutValue();
}

uint8_t TMC26XStepper::getCurrentCSReading() {
  //if we don't yet started there cannot be a StallGuard value
  if (!started) return 0;
  //not time optimal, but solution optiomal:
  //first read out the StallGuard value
  readStatus(TMC26X_READOUT_CURRENT);
  return (getReadoutValue() & 0x1F);
}

uint16_t TMC26XStepper::getCurrentCurrent() {
    float result = (float)getCurrentCSReading(),
           resistor_value = (float)this->resistor,
           voltage = (driver_configuration_register_value & VSENSE)? 0.165 : 0.31;
    result = (result + 1.0) / 32.0 * voltage / resistor_value * sq(1000.0);
    return (uint16_t)result;
}

/**
 * Return true if the StallGuard threshold has been reached
 */
boolean TMC26XStepper::isStallGuardOverThreshold() {
  if (!this->started) return false;
  return (driver_status_result & STATUS_STALL_GUARD_STATUS);
}

/**
 * returns if there is any over temperature condition:
 * OVER_TEMPERATURE_PREWARING if pre warning level has been reached
 * OVER_TEMPERATURE_SHUTDOWN if the temperature is so hot that the driver is shut down
 * Any of those levels are not too good.
 */
char TMC26XStepper::getOverTemperature() {
  if (!this->started) return 0;

  if (driver_status_result & STATUS_OVER_TEMPERATURE_SHUTDOWN)
    return TMC26X_OVERTEMPERATURE_SHUTDOWN;

  if (driver_status_result & STATUS_OVER_TEMPERATURE_WARNING)
    return TMC26X_OVERTEMPERATURE_PREWARING;

  return 0;
}

// Is motor channel A shorted to ground
boolean TMC26XStepper::isShortToGroundA() {
  if (!this->started) return false;
  return (driver_status_result & STATUS_SHORT_TO_GROUND_A);
}

// Is motor channel B shorted to ground
boolean TMC26XStepper::isShortToGroundB() {
  if (!this->started) return false;
  return (driver_status_result & STATUS_SHORT_TO_GROUND_B);
}

// Is motor channel A connected
boolean TMC26XStepper::isOpenLoadA() {
  if (!this->started) return false;
  return (driver_status_result & STATUS_OPEN_LOAD_A);
}

// Is motor channel B connected
boolean TMC26XStepper::isOpenLoadB() {
  if (!this->started) return false;
  return (driver_status_result & STATUS_OPEN_LOAD_B);
}

// Is chopper inactive since 2^20 clock cycles - defaults to ~0,08s
boolean TMC26XStepper::isStandStill() {
  if (!this->started) return false;
  return (driver_status_result & STATUS_STAND_STILL);
}

//is chopper inactive since 2^20 clock cycles - defaults to ~0,08s
boolean TMC26XStepper::isStallGuardReached() {
  if (!this->started) return false;
  return (driver_status_result & STATUS_STALL_GUARD_STATUS);
}

//reads the StallGuard setting from last status
//returns -1 if StallGuard information is not present
int16_t TMC26XStepper::getReadoutValue() {
  return (int)(driver_status_result >> 10);
}

int16_t TMC26XStepper::getResistor() { return this->resistor; }

boolean TMC26XStepper::isCurrentScalingHalfed() {
  return !!(this->driver_configuration_register_value & VSENSE);
}
/**
 * version() returns the version of the library:
 */
int16_t TMC26XStepper::version() { return 1; }

void TMC26XStepper::debugLastStatus() {
  #ifdef TMC_DEBUG1
    if (this->started) {
      if (this->getOverTemperature()&TMC26X_OVERTEMPERATURE_PREWARING)
        SERIAL_ECHOLNPGM("\n  WARNING: Overtemperature Prewarning!");
      else if (this->getOverTemperature()&TMC26X_OVERTEMPERATURE_SHUTDOWN)
        SERIAL_ECHOLNPGM("\n  ERROR: Overtemperature Shutdown!");

      if (this->isShortToGroundA())
        SERIAL_ECHOLNPGM("\n  ERROR: SHORT to ground on channel A!");

      if (this->isShortToGroundB())
        SERIAL_ECHOLNPGM("\n  ERROR: SHORT to ground on channel B!");

      if (this->isOpenLoadA())
        SERIAL_ECHOLNPGM("\n  ERROR: Channel A seems to be unconnected!");

      if (this->isOpenLoadB())
        SERIAL_ECHOLNPGM("\n  ERROR: Channel B seems to be unconnected!");

      if (this->isStallGuardReached())
        SERIAL_ECHOLNPGM("\n  INFO: Stall Guard level reached!");

      if (this->isStandStill())
        SERIAL_ECHOLNPGM("\n  INFO: Motor is standing still.");

      uint32_t readout_config = driver_configuration_register_value & READ_SELECTION_PATTERN;
      const int16_t value = getReadoutValue();
      if (readout_config == READ_MICROSTEP_POSTION) {
        SERIAL_ECHOPAIR("\n  Microstep position phase A: ", value);
      }
      else if (readout_config == READ_STALL_GUARD_READING) {
        SERIAL_ECHOPAIR("\n  Stall Guard value:", value);
      }
      else if (readout_config == READ_STALL_GUARD_AND_COOL_STEP) {
        SERIAL_ECHOPAIR("\n  Approx Stall Guard: ", value & 0xF);
        SERIAL_ECHOPAIR("\n  Current level", value & 0x1F0);
      }
    }
  #endif
}

/**
 * send register settings to the stepper driver via SPI
 * returns the current status
 */
inline void TMC26XStepper::send262(uint32_t datagram) {
  uint32_t i_datagram;

  //preserver the previous spi mode
  //uint8_t oldMode =  SPCR & SPI_MODE_MASK;

  //if the mode is not correct set it to mode 3
  //if (oldMode != SPI_MODE3) {
  //  SPI.setDataMode(SPI_MODE3);
  //}

  //select the TMC driver
  extDigitalWrite(cs_pin, LOW);

  //ensure that only valid bist are set (0-19)
  //datagram &=REGISTER_BIT_PATTERN;

  #ifdef TMC_DEBUG1
    //SERIAL_PRINTF("Sending ");
    //SERIAL_PRINTF("Sending ", datagram,HEX);
    //SERIAL_ECHOPAIR("\n\nSending \n", print_hex_long(datagram));
    SERIAL_PRINTF("\n\nSending %x", datagram);
  #endif

  //write/read the values
  i_datagram = STEPPER_SPI.transfer((datagram >> 16) & 0xFF);
  i_datagram <<= 8;
  i_datagram |= STEPPER_SPI.transfer((datagram >>  8) & 0xFF);
  i_datagram <<= 8;
  i_datagram |= STEPPER_SPI.transfer((datagram) & 0xFF);
  i_datagram >>= 4;

  #ifdef TMC_DEBUG1
    //SERIAL_PRINTF("Received ");
    //SERIAL_PRINTF("Received ", i_datagram,HEX);
    //SERIAL_ECHOPAIR("\n\nReceived \n", i_datagram);
    SERIAL_PRINTF("\n\nReceived %x", i_datagram);
    debugLastStatus();
  #endif

  //deselect the TMC chip
  extDigitalWrite(cs_pin, HIGH);

  //restore the previous SPI mode if neccessary
  //if the mode is not correct set it to mode 3
  //if (oldMode != SPI_MODE3) {
  //  SPI.setDataMode(oldMode);
  //}

  //store the datagram as status result
  driver_status_result = i_datagram;
}

#endif // HAS_DRIVER(TMC2660)

#endif // STM32GENERIC && STM32F7
