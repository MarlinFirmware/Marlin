/**
 * TMC26XStepper.h - - TMC26X Stepper library for Wiring/Arduino
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
 *
 */

#include "../../inc/MarlinConfig.h"

// ensure this library description is only included once
#ifndef _TMC26XSTEPPER_H_
#define _TMC26XSTEPPER_H_

//! return value for TMC26XStepper.getOverTemperature() if there is a overtemperature situation in the TMC chip
/*!
 * This warning indicates that the TCM chip is too warm.
 * It is still working but some parameters may be inferior.
 * You should do something against it.
 */
#define TMC26X_OVERTEMPERATURE_PREWARING 1
//! return value for TMC26XStepper.getOverTemperature() if there is a overtemperature shutdown in the TMC chip
/*!
 * This warning indicates that the TCM chip is too warm to operate and has shut down to prevent damage.
 * It will stop working until it cools down again.
 * If you encouter this situation you must do something against it. Like reducing the current or improving the PCB layout
 * and/or heat management.
 */
#define TMC26X_OVERTEMPERATURE_SHUTDOWN 2

//which values can be read out
/*!
 * Selects to readout the microstep position from the motor.
 *\sa readStatus()
 */
#define TMC26X_READOUT_POSITION 0
/*!
 * Selects to read out the StallGuard value of the motor.
 *\sa readStatus()
 */
#define TMC26X_READOUT_STALLGUARD 1
/*!
 * Selects to read out the current current setting (acc. to CoolStep) and the upper bits of the StallGuard value from the motor.
 *\sa readStatus(), setCurrent()
 */
#define TMC26X_READOUT_CURRENT 3

/*!
 * Define to set the minimum current for CoolStep operation to 1/2 of the selected CS minium.
 *\sa setCoolStepConfiguration()
 */
#define COOL_STEP_HALF_CS_LIMIT 0
/*!
 * Define to set the minimum current for CoolStep operation to 1/4 of the selected CS minium.
 *\sa setCoolStepConfiguration()
 */
#define COOL_STEP_QUARTDER_CS_LIMIT 1

/*!
 * \class TMC26XStepper
 * \brief Class representing a TMC26X stepper driver
 *
 * In order to use one fo those drivers in your Arduino code you have to create an object of that class:
 * \code
 * TMC26XStepper stepper = TMC26XStepper(200,1,2,3,500);
 * \endcode
 * see TMC26XStepper(int number_of_steps, int cs_pin, int dir_pin, int step_pin, unsigned int rms_current)
 *
 * Keep in mind that you need to start the driver with start() in order to get the TMC26X configured.
 *
 * The most important function is the move(). It checks if the motor has to do a step or not.
 * It is important that you call move() as often as possible in your Arduino loop() routine. I suggest
 * to use a very fast loop routine and always call it at the beginning or the end.
 *
 * In order to move you have to provide a movement speed with setSpeed(). The speed is a positive value setting
 * the rotations per minute.
 *
 * To really move the motor you have to call step() to tell the driver to move the motor the given number
 * of steps in the given direction. Positive values move the motor in one direction, negative values in the other direction.
 *
 * You can check with isMoving() if the mototr is still moving or stop it  apruptely with stop().
 */
class TMC26XStepper {
  public:
    /*!
     * \brief creates a new represenatation of a stepper motor connected to a TMC26X stepper driver
     *
     * This is the main constructor. If in doubt use this. You must provide all parameters as described below.
     *
     * \param number_of_steps the number of steps the motor has per rotation.
     * \param cs_pin The Arduino pin you have connected the Cient Select Pin (!CS) of the TMC26X for SPI
     * \param dir_pin the number of the Arduino pin the Direction input of the TMC26X is connected
     * \param step_pin the number of the Arduino pin the step pin of the TMC26X driver is connected.
     * \param rms_current the maximum current to privide to the motor in mA (!). A value of 200 would send up to 200mA to the motor
     * \param resistor the current sense resistor in milli Ohm, defaults to ,15 Ohm ( or 150 milli Ohm) as in the TMC260 Arduino Shield
     *
     * Keep in mind that you must also call TMC26XStepper.start() in order to configure the stepper driver for use.
     *
     * By default the Constant Off Time chopper is used, see TCM262Stepper.setConstantOffTimeChopper() for details.
     * This should work on most motors (YMMV). You may want to configure and use the Spread Cycle Chopper, see  setSpreadCycleChopper().
     *
     * By default a microstepping of 1/32th is used to provide a smooth motor run, while still giving a good progression per step.
     * You can select a different stepping with setMicrosteps() to aa different value.
     * \sa start(), setMicrosteps()
     */
    TMC26XStepper(int number_of_steps, int cs_pin, int dir_pin, int step_pin, unsigned int current, unsigned int resistor=100); //resistor=150

    /*!
     * \brief configures and starts the TMC26X stepper driver. Before you called this function the stepper driver is in nonfunctional mode.
     *
     * This routine configures the TMC26X stepper driver for the given values via SPI.
     * Most member functions are non functional if the driver has not been started.
     * Therefore it is best to call this in your Arduino setup() function.
     */
    void start();

    /*!
     * \brief resets the stepper in unconfigured mode.
     *
     * This routine enables you to call start again. It does not change anything
     * in the internal stepper configuration or the desired configuration.
     * It just marks the stepper as not yet startet. You do not have to reconfigure
     * the stepper to start it again, but it is not reset to any factory settings
     * this has to be configured back by yourself.
     * (Hint: Normally you do not need this function)
     */
    void un_start();


    /*!
     * \brief Sets the rotation speed in revolutions per minute.
     * \param whatSpeed the desired speed in rotations per minute.
     */
    void setSpeed(unsigned int whatSpeed);

    /*!
     * \brief reads out the currently selected speed in revolutions per minute.
     * \sa setSpeed()
     */
    unsigned int getSpeed(void);

    /*!
     * \brief Set the number of microsteps in 2^i values (rounded) up to 256
     *
     * This method set's the number of microsteps per step in 2^i interval.
     * This means you can select 1, 2, 4, 16, 32, 64, 128 or 256 as valid microsteps.
     * If you give any other value it will be rounded to the next smaller number (3 would give a microstepping of 2).
     * You can always check the current microstepping with getMicrosteps().
     */
    void setMicrosteps(int number_of_steps);

    /*!
     * \brief returns the effective current number of microsteps selected.
     *
     * This function always returns the effective number of microsteps.
     * This can be a bit different than the micro steps set in setMicrosteps() since it is rounded to 2^i.
     *
     * \sa setMicrosteps()
     */
    int getMicrosteps(void);

    /*!
     * \brief Initiate a movement for the given number of steps. Positive numbers move in one, negative numbers in the other direction.
     *
     * \param number_of_steps The number of steps to move the motor.
     * \return 0 if the motor was not moving and moves now. -1 if the motor is moving and the new steps could not be set.
     *
     * If the previous movement is not finished yet the function will return -1 and not change the steps to move the motor.
     * If the motor does not move it return 0
     *
     * The direction of the movement is indicated by the sign of the steps parameter. It is not determinable if positive values are right
     * or left This depends on the internal construction of the motor and how you connected it to the stepper driver.
     *
     * You can always verify with isMoving() or even use stop() to stop the motor before giving it new step directions.
     * \sa isMoving(), getStepsLeft(), stop()
     */
    char step(int number_of_steps);

    /*!
     * \brief Central movement method, must be called as often as possible in the lopp function and is very fast.
     *
     * This routine checks if the motor still has to move, if the waiting delay has passed to send a new step command to the motor
     * and manages the number of steps yet to move to fulfill the current move command.
     *
     * This function is implemented to be as fast as possible to call it as often as possible in your loop routine.
     * The more regurlarly you call this function the better. In both senses of 'regularly': Calling it as often as
     * possible is not a bad idea and if you even manage that the intervals you call this function are not too irregular helps too.
     *
     * You can call this routine even if you know that the motor is not miving. It introduces just a very small penalty in your code.
     * You must not call isMoving() to determine if you need to call this function, since taht is done internally already and only
     * slows down you code.
     *
     * How often you call this function directly influences your top miving speed for the motor. It may be a good idea to call this
     * from an timer overflow interrupt to ensure proper calling.
     * \sa step()
     */
    char move(void);

    /*!
     * \brief checks if the motor still has to move to fulfill the last movement command.
     * \return 0 if the motor stops, -1 if the motor is moving.
     *
     * This method can be used to determine if the motor is ready for new movements.
     *\sa step(), move()
     */
    char isMoving(void);

    /*!
     * \brief Get the number of steps left in the current movement.
     * \return The number of steps left in the movement. This number is always positive.
     */
    unsigned int getStepsLeft(void);

    /*!
     * \brief Stops the motor regardless if it moves or not.
     * \return -1 if the motor was moving and is really stoped or 0 if it was not moving at all.
     *
     * This method directly and apruptely stops the motor and may be used as an emergency stop.
     */
    char stop(void);

    /*!
     * \brief Sets and configure the classical Constant Off Timer Chopper
     * \param constant_off_time The off time setting controls the minimum chopper frequency. For most applications an off time within the range of 5μs to 20μs will fit. Setting this parameter to zero completely disables all driver transistors and the motor can free-wheel. 0: chopper off, 1:15: off time setting (1 will work with minimum blank time of 24 clocks)
     * \param blank_time Selects the comparator blank time. This time needs to safely cover the switching event and the duration of the ringing on the sense resistor. For most low current drivers, a setting of 1 or 2 is good. For high current applications with large MOSFETs, a setting of 2 or 3 will be required. 0 (min setting) … (3) amx setting
     * \param fast_decay_time_setting Fast decay time setting. Controls the portion of fast decay for each chopper cycle. 0: slow decay only, 1…15: duration of fast decay phase
     * \param sine_wave_offset Sine wave offset. Controls the sine wave offset. A positive offset corrects for zero crossing error. -3…-1: negative offset, 0: no offset,1…12: positive offset
     * \param use_curreent_comparator Selects usage of the current comparator for termination of the fast decay cycle. If current comparator is enabled, it terminates the fast decay cycle in case the current reaches a higher negative value than the actual positive value. (0 disable, -1 enable).
     *
     * The classic constant off time chopper uses a fixed portion of fast decay following each on phase.
     * While the duration of the on time is determined by the chopper comparator, the fast decay time needs
     * to be set by the user in a way, that the current decay is enough for the driver to be able to follow
     * the falling slope of the sine wave, and on the other hand it should not be too long, in order to minimize
     * motor current ripple and power dissipation. This best can be tuned using an oscilloscope or
     * trying out motor smoothness at different velocities. A good starting value is a fast decay time setting
     * similar to the slow decay time setting.
     * After tuning of the fast decay time, the offset should be determined, in order to have a smooth zero transition.
     * This is necessary, because the fast decay phase leads to the absolute value of the motor current being lower
     * than the target current (see figure 17). If the zero offset is too low, the motor stands still for a short
     * moment during current zero crossing, if it is set too high, it makes a larger microstep.
     * Typically, a positive offset setting is required for optimum operation.
     *
     * \sa setSpreadCycleChoper() for other alternatives.
     * \sa setRandomOffTime() for spreading the noise over a wider spectrum
     */
    void setConstantOffTimeChopper(char constant_off_time, char blank_time, char fast_decay_time_setting, char sine_wave_offset, unsigned char use_current_comparator);

    /*!
     * \brief Sets and configures with spread cycle chopper.
     * \param constant_off_time The off time setting controls the minimum chopper frequency. For most applications an off time within the range of 5μs to 20μs will fit. Setting this parameter to zero completely disables all driver transistors and the motor can free-wheel. 0: chopper off, 1:15: off time setting (1 will work with minimum blank time of 24 clocks)
     * \param blank_time Selects the comparator blank time. This time needs to safely cover the switching event and the duration of the ringing on the sense resistor. For most low current drivers, a setting of 1 or 2 is good. For high current applications with large MOSFETs, a setting of 2 or 3 will be required. 0 (min setting) … (3) amx setting
     * \param hysteresis_start Hysteresis start setting. Please remark, that this value is an offset to the hysteresis end value. 1 … 8
     * \param hysteresis_end Hysteresis end setting. Sets the hysteresis end value after a number of decrements. Decrement interval time is controlled by hysteresis_decrement. The sum hysteresis_start + hysteresis_end must be <16. At a current setting CS of max. 30 (amplitude reduced to 240), the sum is not limited.
     * \param hysteresis_decrement Hysteresis decrement setting. This setting determines the slope of the hysteresis during on time and during fast decay time. 0 (fast decrement) … 3 (slow decrement).
     *
     * The spreadCycle chopper scheme (pat.fil.) is a precise and simple to use chopper principle, which automatically determines
     * the optimum fast decay portion for the motor. Anyhow, a number of settings can be made in order to optimally fit the driver
     * to the motor.
     * Each chopper cycle is comprised of an on-phase, a slow decay phase, a fast decay phase and a second slow decay phase.
     * The slow decay phases limit the maximum chopper frequency and are important for low motor and driver power dissipation.
     * The hysteresis start setting limits the chopper frequency by forcing the driver to introduce a minimum amount of
     * current ripple into the motor coils. The motor inductivity determines the ability to follow a changing motor current.
     * The duration of the on- and fast decay phase needs to cover at least the blank time, because the current comparator is
     * disabled during this time.
     *
     * \sa setRandomOffTime() for spreading the noise over a wider spectrum
     */
    void setSpreadCycleChopper(char constant_off_time, char blank_time, char hysteresis_start, char hysteresis_end, char hysteresis_decrement);

    /*!
     * \brief Use random off time for noise reduction (0 for off, -1 for on).
     * \param value 0 for off, -1 for on
     *
     * In a constant off time chopper scheme both coil choppers run freely, i.e. are not synchronized.
     * The frequency of each chopper mainly depends on the coil current and the position dependant motor coil inductivity,
     * thus it depends on the microstep position. With some motors a slightly audible beat can occur between the chopper
     * frequencies, especially when they are near to each other. This typically occurs at a few microstep positions within
     * each quarter wave.
     * This effect normally is not audible when compared to mechanical noise generated by ball bearings,
     * etc. Further factors which can cause a similar effect are a poor layout of sense resistor GND connection.
     * In order to minimize the effect of a beat between both chopper frequencies, an internal random generator is provided.
     * It modulates the slow decay time setting when switched on. The random off time feature further spreads the chopper spectrum,
     * reducing electromagnetic emission on single frequencies.
     */
    void setRandomOffTime(char value);

    /*!
     * \brief set the maximum motor current in mA (1000 is 1 Amp)
     * Keep in mind this is the maximum peak Current. The RMS current will be 1/sqrt(2) smaller. The actual current can also be smaller
     * by employing CoolStep.
     * \param current the maximum motor current in mA
     * \sa getCurrent(), getCurrentCurrent()
     */
    void setCurrent(unsigned int current);

    /*!
     * \brief readout the motor maximum current in mA (1000 is an Amp)
     * This is the maximum current. to get the current current - which may be affected by CoolStep us getCurrentCurrent()
     * \return the maximum motor current in milli amps
     * \sa getCurrentCurrent()
     */
    unsigned int getCurrent(void);

    /*!
     * \brief set the StallGuard threshold in order to get sensible StallGuard readings.
     * \param stall_guard_threshold -64 … 63 the StallGuard threshold
     * \param stall_guard_filter_enabled 0 if the filter is disabled, -1 if it is enabled
     *
     * The StallGuard threshold is used to optimize the StallGuard reading to sensible values. It should be at 0 at
     * the maximum allowable load on the otor (but not before). = is a good starting point (and the default)
     * If you get Stall Gaurd readings of 0 without any load or with too little laod increase the value.
     * If you get readings of 1023 even with load decrease the setting.
     *
     * If you switch on the filter the StallGuard reading is only updated each 4th full step to reduce the noise in the
     * reading.
     *
     * \sa getCurrentStallGuardReading() to read out the current value.
     */
    void setStallGuardThreshold(char stall_guard_threshold, char stall_guard_filter_enabled);

    /*!
     * \brief reads out the StallGuard threshold
     * \return a number between -64 and 63.
     */
    char getStallGuardThreshold(void);

    /*!
     * \brief returns the current setting of the StallGuard filter
     * \return 0 if not set, -1 if set
     */
    char getStallGuardFilter(void);

    /*!
     * \brief This method configures the CoolStep smart energy operation. You must have a proper StallGuard configuration for the motor situation (current, voltage, speed) in rder to use this feature.
     * \param lower_SG_threshold Sets the lower threshold for stallGuard2TM reading. Below this value, the motor current becomes increased. Allowed values are 0...480
     * \param SG_hysteresis Sets the distance between the lower and the upper threshold for stallGuard2TM reading. Above the upper threshold (which is lower_SG_threshold+SG_hysteresis+1) the motor current becomes decreased. Allowed values are 0...480
     * \param current_decrement_step_size Sets the current decrement steps. If the StallGuard value is above the threshold the current gets decremented by this step size. 0...32
     * \param current_increment_step_size Sets the current increment step. The current becomes incremented for each measured stallGuard2TM value below the lower threshold. 0...8
     * \param lower_current_limit Sets the lower motor current limit for coolStepTM operation by scaling the CS value. Values can be COOL_STEP_HALF_CS_LIMIT, COOL_STEP_QUARTER_CS_LIMIT
     * The CoolStep smart energy operation automatically adjust the current sent into the motor according to the current load,
     * read out by the StallGuard in order to provide the optimum torque with the minimal current consumption.
     * You configure the CoolStep current regulator by defining upper and lower bounds of StallGuard readouts. If the readout is above the
     * limit the current gets increased, below the limit the current gets decreased.
     * You can specify the upper an lower threshold of the StallGuard readout in order to adjust the current. You can also set the number of
     * StallGuard readings neccessary above or below the limit to get a more stable current adjustement.
     * The current adjustement itself is configured by the number of steps the current gests in- or decreased and the absolut minimum current
     * (1/2 or 1/4th otf the configured current).
     * \sa COOL_STEP_HALF_CS_LIMIT, COOL_STEP_QUARTER_CS_LIMIT
     */
    void setCoolStepConfiguration(unsigned int lower_SG_threshold, unsigned int SG_hysteresis, unsigned char current_decrement_step_size,
                                  unsigned char current_increment_step_size, unsigned char lower_current_limit);

    /*!
     * \brief enables or disables the CoolStep smart energy operation feature. It must be configured before enabling it.
     * \param enabled true if CoolStep should be enabled, false if not.
     * \sa setCoolStepConfiguration()
     */
    void setCoolStepEnabled(boolean enabled);


    /*!
     * \brief check if the CoolStep feature is enabled
     * \sa setCoolStepEnabled()
     */
    boolean isCoolStepEnabled();

    /*!
     * \brief returns the lower StallGuard threshold for the CoolStep operation
     * \sa setCoolStepConfiguration()
     */
    unsigned int getCoolStepLowerSgThreshold();

    /*!
     * \brief returns the upper StallGuard threshold for the CoolStep operation
     * \sa setCoolStepConfiguration()
     */
    unsigned int getCoolStepUpperSgThreshold();

    /*!
     * \brief returns the number of StallGuard readings befor CoolStep adjusts the motor current.
     * \sa setCoolStepConfiguration()
     */
    unsigned char getCoolStepNumberOfSGReadings();

    /*!
     * \brief returns the increment steps for the current for the CoolStep operation
     * \sa setCoolStepConfiguration()
     */
    unsigned char getCoolStepCurrentIncrementSize();

    /*!
     * \brief returns the absolut minium current for the CoolStep operation
     * \sa setCoolStepConfiguration()
     * \sa COOL_STEP_HALF_CS_LIMIT, COOL_STEP_QUARTER_CS_LIMIT
     */
    unsigned char getCoolStepLowerCurrentLimit();

    /*!
     * \brief Get the current microstep position for phase A
     * \return The current microstep position for phase A 0…255
     *
     * Keep in mind that this routine reads and writes a value via SPI - so this may take a bit time.
     */
    int getMotorPosition(void);

    /*!
     * \brief Reads the current StallGuard value.
     * \return The current StallGuard value, lesser values indicate higher load, 0 means stall detected.
     * Keep in mind that this routine reads and writes a value via SPI - so this may take a bit time.
     * \sa setStallGuardThreshold() for tuning the readout to sensible ranges.
     */
    int getCurrentStallGuardReading(void);

    /*!
     * \brief Reads the current current setting value as fraction of the maximum current
     * Returns values between 0 and 31, representing 1/32 to 32/32 (=1)
     * \sa setCoolStepConfiguration()
     */
    unsigned char getCurrentCSReading(void);


    /*!
     *\brief a convenience method to determine if the current scaling uses 0.31V or 0.165V as reference.
     *\return false if 0.13V is the reference voltage, true if 0.165V is used.
     */
    boolean isCurrentScalingHalfed();

    /*!
     * \brief Reads the current current setting value and recalculates the absolute current in mA (1A would be 1000).
     * This method calculates the currently used current setting (either by setting or by CoolStep) and reconstructs
     * the current in mA by usinge the VSENSE and resistor value. This method uses floating point math - so it
     * may not be the fastest.
     * \sa getCurrentCSReading(), getResistor(), isCurrentScalingHalfed(), getCurrent()
     */
    unsigned int getCurrentCurrent(void);

    /*!
     * \brief checks if there is a StallGuard warning in the last status
     * \return 0 if there was no warning, -1 if there was some warning.
     * Keep in mind that this method does not enforce a readout but uses the value of the last status readout.
     * You may want to use getMotorPosition() or getCurrentStallGuardReading() to enforce an updated status readout.
     *
     * \sa setStallGuardThreshold() for tuning the readout to sensible ranges.
     */
    boolean isStallGuardOverThreshold(void);

    /*!
     * \brief Return over temperature status of the last status readout
     * return 0 is everything is OK, TMC26X_OVERTEMPERATURE_PREWARING if status is reached, TMC26X_OVERTEMPERATURE_SHUTDOWN is the chip is shutdown, -1 if the status is unknown.
     * Keep in mind that this method does not enforce a readout but uses the value of the last status readout.
     * You may want to use getMotorPosition() or getCurrentStallGuardReading() to enforce an updated status readout.
     */
    char getOverTemperature(void);

    /*!
     * \brief Is motor channel A shorted to ground detected in the last status readout.
     * \return true is yes, false if not.
     * Keep in mind that this method does not enforce a readout but uses the value of the last status readout.
     * You may want to use getMotorPosition() or getCurrentStallGuardReading() to enforce an updated status readout.
     */

    boolean isShortToGroundA(void);

    /*!
     * \brief Is motor channel B shorted to ground detected in the last status readout.
     * \return true is yes, false if not.
     * Keep in mind that this method does not enforce a readout but uses the value of the last status readout.
     * You may want to use getMotorPosition() or getCurrentStallGuardReading() to enforce an updated status readout.
     */
    boolean isShortToGroundB(void);
    /*!
     * \brief iIs motor channel A connected according to the last statu readout.
     * \return true is yes, false if not.
     * Keep in mind that this method does not enforce a readout but uses the value of the last status readout.
     * You may want to use getMotorPosition() or getCurrentStallGuardReading() to enforce an updated status readout.
     */
    boolean isOpenLoadA(void);

    /*!
     * \brief iIs motor channel A connected according to the last statu readout.
     * \return true is yes, false if not.
     * Keep in mind that this method does not enforce a readout but uses the value of the last status readout.
     * You may want to use getMotorPosition() or getCurrentStallGuardReading() to enforce an updated status readout.
     */
    boolean isOpenLoadB(void);

    /*!
     * \brief Is chopper inactive since 2^20 clock cycles - defaults to ~0,08s
     * \return true is yes, false if not.
     * Keep in mind that this method does not enforce a readout but uses the value of the last status readout.
     * You may want to use getMotorPosition() or getCurrentStallGuardReading() to enforce an updated status readout.
     */
    boolean isStandStill(void);

    /*!
     * \brief checks if there is a StallGuard warning in the last status
     * \return 0 if there was no warning, -1 if there was some warning.
     * Keep in mind that this method does not enforce a readout but uses the value of the last status readout.
     * You may want to use getMotorPosition() or getCurrentStallGuardReading() to enforce an updated status readout.
     *
     * \sa isStallGuardOverThreshold()
     * TODO why?
     *
     * \sa setStallGuardThreshold() for tuning the readout to sensible ranges.
     */
    boolean isStallGuardReached(void);

    /*!
     *\brief enables or disables the motor driver bridges. If disabled the motor can run freely. If enabled not.
     *\param enabled a boolean value true if the motor should be enabled, false otherwise.
     */
    void setEnabled(boolean enabled);

    /*!
     *\brief checks if the output bridges are enabled. If the bridges are not enabled the motor can run freely
     *\return true if the bridges and by that the motor driver are enabled, false if not.
     *\sa setEnabled()
     */
    boolean isEnabled();

    /*!
     * \brief Manually read out the status register
     * This function sends a byte to the motor driver in order to get the current readout. The parameter read_value
     * seletcs which value will get returned. If the read_vlaue changes in respect to the previous readout this method
     * automatically send two bytes to the motor: one to set the redout and one to get the actual readout. So this method
     * may take time to send and read one or two bits - depending on the previous readout.
     * \param read_value selects which value to read out (0..3). You can use the defines TMC26X_READOUT_POSITION, TMC_262_READOUT_STALLGUARD, or TMC_262_READOUT_CURRENT
     * \sa TMC26X_READOUT_POSITION, TMC_262_READOUT_STALLGUARD, TMC_262_READOUT_CURRENT
     */
    void readStatus(char read_value);

    /*!
     * \brief Returns the current sense resistor value in milliohm.
     * The default value of ,15 Ohm will return 150.
     */
    int getResistor();

    /*!
     * \brief Prints out all the information that can be found in the last status read out - it does not force a status readout.
     * The result is printed via Serial
     */
    void debugLastStatus(void);
    /*!
     * \brief library version
     * \return the version number as int.
     */
    int version(void);

  private:
    unsigned int steps_left;    // The steps the motor has to do to complete the movement
    int direction;              // Direction of rotation
    unsigned long step_delay;   // Delay between steps, in ms, based on speed
    int number_of_steps;        // Total number of steps this motor can take
    unsigned int speed;         // Store the current speed in order to change the speed after changing microstepping
    unsigned int resistor;      // Current sense resitor value in milliohm

    unsigned long last_step_time;   // Time stamp in ms of when the last step was taken
    unsigned long next_step_time;   // Time stamp in ms of when the last step was taken

    // Driver control register copies to easily set & modify the registers
    unsigned long driver_control_register_value;
    unsigned long chopper_config_register;
    unsigned long cool_step_register_value;
    unsigned long stall_guard2_current_register_value;
    unsigned long driver_configuration_register_value;
    // The driver status result
    unsigned long driver_status_result;

    // Helper routione to get the top 10 bit of the readout
    inline int getReadoutValue();

    // The pins for the stepper driver
    unsigned char cs_pin;
    unsigned char step_pin;
    unsigned char dir_pin;

    // Status values
    boolean started; // If the stepper has been started yet
    int microsteps; // The current number of micro steps
    char constant_off_time; // We need to remember this value in order to enable and disable the motor
    unsigned char cool_step_lower_threshold; //  we need to remember the threshold to enable and disable the CoolStep feature
    boolean cool_step_enabled; // We need to remember this to configure the coolstep if it si enabled

    // SPI sender
    inline void send262(unsigned long datagram);
};

#endif // _TMC26XSTEPPER_H_
