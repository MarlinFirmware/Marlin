/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "MarlinConfig.h"

#if ENABLED(I2C_POSITION_ENCODERS)

  #include "Marlin.h"
  #include "temperature.h"
  #include "stepper.h"
  #include "I2CEncoder.h"

  #include <Wire.h>

  void I2CEncoder::init(AxisEnum axis, uint8_t address, bool invert) {
    encoderAxis = axis;
    i2cAddress = address;
    invertDirection = invert;

    initialised = true;

    SERIAL_ECHOPGM("Encoder on ");
    SERIAL_ECHO(axis_codes[encoderAxis]);
    SERIAL_ECHOPGM(" axis, address = ");
    SERIAL_ECHOLN((int) address);
    position = get_position();
  }

  void I2CEncoder::update() {

    //check encoder is set up and active
    if(initialised && homed && active) {

      bool moduleDetected;

      position = get_position();
      //we don't want to stop things just because the encoder missed a message,
      //so we only care about responses that indicate bad magnetic strength
      bool signalGood = passes_test(false,moduleDetected);

      //check encoder data is good
      if(signalGood) {

        //if data is historically good, proceed
        if(trusted) {

          //get latest position
          lastPosition = position;
          unsigned long positionTime = millis();

          //only do error correction if setup and enabled
          if(get_error_correct_method() != I2CPE_ECM_NONE && get_error_correct_enabled()) {

            #if defined(I2CPE_ERROR_THRESHOLD_PROPORTIONAL_SPEED)
              unsigned long distance = abs(position - lastPosition);
              unsigned long deltaTime = positionTime - lastPositionTime;
              unsigned long speed = distance / deltaTime;
              float threshold = constrain((speed / 50),1,50) * get_error_correct_threshold();
            #else
              float threshold = get_error_correct_threshold();
            #endif

            //check error
            #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
              ((errArrayIndex >= I2CPE_ERR_ARRAY_SIZE - 1) ? (errArrayIndex = 0) : errArrayIndex++);
              errorArray[errArrayIndex] = get_axis_error_steps(false);
              double  sum = 0;
              double diffSum = 0;
              for (int i=0;i<I2CPE_ERR_ARRAY_SIZE;i++) {
                sum += errorArray[i];
                if (i > 0) diffSum += abs(errorArray[i-1]-errorArray[i]);
              }
              long error = (long)(sum/(I2CPE_ERR_ARRAY_SIZE + 1)); //calculate average for error

            #else
              long error = get_axis_error_steps(false);
            #endif

            //SERIAL_ECHOPGM("Axis err*r steps: ");
            //SERIAL_ECHOLN(error);

            #if defined(AXIS_ERROR_THRESHOLD_ABORT)
              if(abs(error) > AXIS_ERROR_THRESHOLD_ABORT * planner.axis_steps_per_mm[encoderAxis]) {
                //kill("Significant Error");
                SERIAL_ECHOPGM("Axis error greater than set threshold, aborting!");
                SERIAL_ECHOLN(error);
                delay(5000);
              }
            #endif

            #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
              if(errArrayIndex == 0) {
                // in order to correct for "error" but avoid correcting for noise and non skips
                // it must be > threshold and have a difference average of < 10 and be < 2000 steps
                if(abs(error) > threshold * planner.axis_steps_per_mm[encoderAxis] && diffSum < 10*(I2CPE_ERR_ARRAY_SIZE-1) && abs(error) < 2000) { //Check for persistent error (skip)
                  SERIAL_ECHO(axis_codes[encoderAxis]);
                  SERIAL_ECHOPGM(" diffSum: ");
                  SERIAL_ECHO(diffSum/(I2CPE_ERR_ARRAY_SIZE-1));
                  SERIAL_ECHOPGM(" err detected: ");
                  SERIAL_ECHO(error / planner.axis_steps_per_mm[encoderAxis]);
                  SERIAL_ECHOLNPGM("mm");
                  SERIAL_ECHOLNPGM("Correcting");
                  thermalManager.babystepsTodo[encoderAxis] = -lround(error);
                }
              }
            #else
              if(abs(error) > threshold * planner.axis_steps_per_mm[encoderAxis]) {
                //SERIAL_ECHOLN(error);
                //SERIAL_ECHOLN(position);
                thermalManager.babystepsTodo[encoderAxis] = -lround(error/2);
              }
            #endif

            if(abs(error) > (I2CPE_ERR_CNT_TRIGGER_THRESHOLD * planner.axis_steps_per_mm[encoderAxis]) && millis() - lastErrorCountTime > I2CPE_ERR_CNT_DEBOUNCE_MS) {
              SERIAL_ECHOPGM("Large error on ");
              SERIAL_ECHO(axis_codes[encoderAxis]);
              SERIAL_ECHOPGM(" axis. error: ");
              SERIAL_ECHO((int)error);
              SERIAL_ECHOPGM("; diffSum: ");
              SERIAL_ECHOLN(diffSum);
              errorCount++;
              lastErrorCountTime = millis();
            }
          }

          lastPositionTime = positionTime;
        }/* else { //commented out because it introduces error and can cause bad print quality. See NOTE.

  //      NOTE: This code is intended to manage situations where the encoder has reported bad magnetic strength.
  //      This indicates that the magnetic strip was too far away from the sensor to reliably track position. When this
  //      happens, this code resets the offset based on where the printer thinks it is. This has been shown to introduce
  //      errors in actual position which result in drifting prints and poor print quality. Perhaps a better method would
  //      be to disable correction on the axis with a problem, report it to the user via the status leds on the encoder
  //      module and prompt the user to re-home the axis at which point the encoder would be re-enabled

          //if the magnetic strength has been good for a certain time, start trusting the module again
          if(millis() - lastErrorTime > STABLE_TIME_UNTIL_TRUSTED) {
            trusted = true;

            SERIAL_ECHOPGM("Untrusted encoder module on ");
            SERIAL_ECHO(axis_codes[encoderAxis]);
            SERIAL_ECHOLNPGM(" axis has been fault-free for set duration, reinstating error correction.");

            //the encoder likely lost its place when the error occured, so we'll reset and use the printer's
            //idea of where it the axis is to re-initialise
            double position = stepper.get_axis_position_mm(encoderAxis);
            long positionInTicks = position * get_encoder_ticks_unit();

            //shift position from previous to current position
            zeroOffset -= (positionInTicks - get_position());

            #if defined(I2CPE_DEBUG_ECHOS)
              SERIAL_ECHOPGM("Current position is ");
              SERIAL_ECHOLN(position);

              SERIAL_ECHOPGM("Position in encoder ticks is ");
              SERIAL_ECHOLN(positionInTicks);

              SERIAL_ECHOPGM("New zero-offset of ");
              SERIAL_ECHOLN(zeroOffset);

              SERIAL_ECHOPGM("New position reads as ");
              SERIAL_ECHO(get_position());
              SERIAL_ECHOPGM("(");
              SERIAL_ECHO(mm_from_count(get_position()));
              SERIAL_ECHOLNPGM(")");
            #endif
          }
        }*/
      } else {
        lastErrorTime = millis();
  /*
        if(trusted) { //commented out as part of the above NOTE
          trusted = false;
          SERIAL_ECHOPGM("Fault detected on ");
          SERIAL_ECHO(axis_codes[encoderAxis]);
          SERIAL_ECHOLNPGM(" axis encoder. Disengaging error correction until module is trusted again.");
        }*/
      }
    }
  }

  void I2CEncoder::set_homed() {
    if(active) {
      //reset module's offset to zero (so current position is homed / zero)
      set_zeroed();
      delay(10);
      this->zeroOffset = get_raw_count();
      this->homed = true;
      this->trusted = true;
      #if defined(I2CPE_DEBUG_ECHOS)
        SERIAL_ECHO(axis_codes[encoderAxis]);
        SERIAL_ECHOPGM(" axis encoder homed, offset of ");
        SERIAL_ECHO(zeroOffset);
        SERIAL_ECHOLNPGM(" ticks.");
      #endif
      }
  }

  bool I2CEncoder::passes_test(bool report) {
    bool encoderPresent;
    return (passes_test(report,encoderPresent) && encoderPresent);
  }

  bool I2CEncoder::passes_test(bool report, bool &moduleDetected) {
   byte magStrength = get_magnetic_strength();

    moduleDetected = true;

    if(magStrength == I2CPE_MAG_SIG_BAD) {
      if(report) {
        SERIAL_ECHOPGM("Warning, ");
        SERIAL_ECHO(axis_codes[encoderAxis]);
        SERIAL_ECHOLNPGM(" axis magnetic strip not detected!");
      }
      return false;
    } else if (magStrength == I2CPE_MAG_SIG_GOOD || magStrength == I2CPE_MAG_SIG_MID) {
      if(report) {
        SERIAL_ECHO(axis_codes[encoderAxis]);
        SERIAL_ECHOLNPGM(" axis encoder passes test.");
      }
      return true;
    } else {
      if(report) {
        SERIAL_ECHOPGM("Warning, ");
        SERIAL_ECHO(axis_codes[encoderAxis]);
        SERIAL_ECHOLNPGM(" axis encoder not detected!");
      }
      moduleDetected = false;
      return true;
    }
  }

  double I2CEncoder::get_axis_error_mm(bool report) {
    double target, actual, error;

    target = stepper.get_axis_position_mm(encoderAxis);
    actual = mm_from_count(position);
    error = actual - target;

    if (abs(error) > 10000) {
      error = 0;
    }

    if(report) {
      SERIAL_ECHO(axis_codes[encoderAxis]);
      SERIAL_ECHOPGM(" Target: ");
      SERIAL_ECHOLN(target);
      SERIAL_ECHO(axis_codes[encoderAxis]);
      SERIAL_ECHOPGM(" Actual: ");
      SERIAL_ECHOLN(actual);
      SERIAL_ECHO(axis_codes[encoderAxis]);
      SERIAL_ECHOPGM(" Error : ");
      SERIAL_ECHOLN(error);
    }

    return error;
  }

  long I2CEncoder::get_axis_error_steps(bool report) {
    long encoderTicks = position;
    //long stepperTicks = stepper.position(encoderAxis);
    long encoderCountInStepperTicksScaled;

    float stepperTicksPerUnit;
    bool suppressOutput = false;

    if(get_encoder_type() == I2CPE_ENC_TYPE_ROTARY) {
      // In a rotary encoder we're concerned with ticks / revolution
      stepperTicksPerUnit = get_stepper_ticks();

    } else {

      // In a linear encoder we're concerned with ticks / mm
      stepperTicksPerUnit = planner.axis_steps_per_mm[encoderAxis];
    }

    //convert both 'ticks' into same units / base
    encoderCountInStepperTicksScaled = lround((stepperTicksPerUnit * encoderTicks) / get_encoder_ticks_unit());

    long target = stepper.position(encoderAxis);
    long error = (encoderCountInStepperTicksScaled - target);

    //suppress discontinuities (might be caused by bad I2C readings...?)
    if(abs(error - errorPrev) > 100) {
      suppressOutput = true;
    }

    if(report) {
      SERIAL_ECHO(axis_codes[encoderAxis]);
      SERIAL_ECHOPGM(" Target: ");
      SERIAL_ECHOLN(target);
      SERIAL_ECHO(axis_codes[encoderAxis]);
      SERIAL_ECHOPGM(" Actual: ");
      SERIAL_ECHOLN(encoderCountInStepperTicksScaled);
      SERIAL_ECHO(axis_codes[encoderAxis]);
      SERIAL_ECHOPGM(" Error : ");
      SERIAL_ECHOLN(error);
      if(suppressOutput) {
        SERIAL_ECHOLNPGM("Discontinuity detected, suppressing error.");
      }
    }

    errorPrev = error;

    return (suppressOutput ? 0 : error);
  }

  double I2CEncoder::get_position_mm() {
    return mm_from_count(get_position());
  }

  double I2CEncoder::mm_from_count(long count) {
    if(get_encoder_type() == I2CPE_ENC_TYPE_LINEAR) {
      return (double) count / get_encoder_ticks_unit();
    } else if (get_encoder_type() == I2CPE_ENC_TYPE_ROTARY) {
      return (double) (count * get_stepper_ticks()) / (get_encoder_ticks_unit() * planner.axis_steps_per_mm[encoderAxis]);
    }

    return -1;
  }

  long I2CEncoder::get_position() {
    return get_raw_count() - zeroOffset - axisOffsetTicks;
  }

  long I2CEncoder::get_raw_count() {
    uint8_t index = 0;
    i2cLong encoderCount;

    Wire.requestFrom((int)i2cAddress,3);

    encoderCount.val = 0x00;

    while (Wire.available())
      encoderCount.bval[index++] = Wire.read();

    //extract the magnetic strength
    magneticStrength = (B00000011 & (encoderCount.bval[2] >> 6));

    //extract sign bit; sign = (encoderCount.bval[2] & B00100000);
    //set all upper bits to the sign value to overwrite magneticStrength
    encoderCount.val = (encoderCount.bval[2] & B00100000) ? (encoderCount.val | 0xFFC00000) : (encoderCount.val & 0x003FFFFF);

    return (invertDirection ? -1 * encoderCount.val : encoderCount.val);
  }

  uint8_t I2CEncoder::get_magnetic_strength() {
      /* //Prevous method before magnetic strength was packed into the position words
      //Set module to report magnetic strength
      Wire.beginTransmission((int)i2cAddress);
      Wire.write(I2CPE_SET_REPORT_MODE);
      Wire.write(I2CPE_REPORT_MODE_STRENGTH);
      Wire.endTransmission();

      //Read value
      Wire.requestFrom((int)i2cAddress,1);

      uint8_t reading = 99;

      reading = Wire.read();

      //Set module back to normal (distance) mode
      Wire.beginTransmission((int)i2cAddress);
      Wire.write(I2CPE_SET_REPORT_MODE);
      Wire.write(I2CPE_REPORT_MODE_DISTANCE);
      Wire.endTransmission();

      return reading;
      */
      return magneticStrength;
    }

  bool I2CEncoder::test_axis() {
    //only works on XYZ cartesian machines for the time being
    if(!(encoderAxis == X_AXIS || encoderAxis == Y_AXIS || encoderAxis == Z_AXIS)) return false;

    int feedrate;
    float startPosition, endPosition;
    float startCoord[NUM_AXIS] = {0};
    float endCoord[NUM_AXIS] = {0};

    startPosition = soft_endstop_min[encoderAxis] + 10;
    endPosition = soft_endstop_max[encoderAxis] - 10;

    feedrate = (int)MMM_TO_MMS((encoderAxis == Z_AXIS)?HOMING_FEEDRATE_Z:HOMING_FEEDRATE_XY);

    errorCorrect = false;

    LOOP_NA(i) {
      startCoord[i] = stepper.get_axis_position_mm((AxisEnum)i);
      endCoord[i] = stepper.get_axis_position_mm((AxisEnum)i);
    }

    startCoord[encoderAxis] = startPosition;
    endCoord[encoderAxis] = endPosition;

    stepper.synchronize();

    planner.buffer_line(startCoord[X_AXIS],startCoord[Y_AXIS],startCoord[Z_AXIS], stepper.get_axis_position_mm(E_AXIS), feedrate, 0);
    stepper.synchronize();

    //if the module isn't currently trusted, wait until it is (or until it should be if things are working)
    if(!trusted) {
      long startWaitingTime = millis();
      while(!trusted && millis() - startWaitingTime < STABLE_TIME_UNTIL_TRUSTED)
        safe_delay(500);
    }

    if(trusted) {
      //if trusted, commence test
      planner.buffer_line(endCoord[X_AXIS],endCoord[Y_AXIS],endCoord[Z_AXIS], stepper.get_axis_position_mm(E_AXIS), feedrate, 0);
      stepper.synchronize();

      return trusted;
    }

    return false;
  }


  void I2CEncoder::calibrate_steps_mm(int iter) {
    float oldStepsMm, newStepsMm, startDistance, endDistance, travelDistance, travelledDistance, total = 0;
    long startCount, stopCount;

    float startCoord[NUM_AXIS] = {0};
    float endCoord[NUM_AXIS] = {0};

    double feedrate;

    if(get_encoder_type() != I2CPE_ENC_TYPE_LINEAR) {
      SERIAL_ECHOLNPGM("Steps per mm calibration is only available using linear encoders.");
    } else {

      if(encoderAxis == X_AXIS || encoderAxis == Y_AXIS || encoderAxis == Z_AXIS) {

        feedrate = MMM_TO_MMS((encoderAxis == Z_AXIS) ? HOMING_FEEDRATE_Z : HOMING_FEEDRATE_XY);

        errorCorrect = false;

        startDistance = 20;
        endDistance = soft_endstop_max[encoderAxis] - 20;
        travelDistance = endDistance - startDistance;

        LOOP_NA(i) {
          startCoord[i] = stepper.get_axis_position_mm((AxisEnum) i);
          endCoord[i] = stepper.get_axis_position_mm((AxisEnum) i);
        }

        startCoord[encoderAxis] = startDistance;
        endCoord[encoderAxis] = endDistance;


        for(int i = 0; i < iter; i++) {
          stepper.synchronize();

          planner.buffer_line(startCoord[X_AXIS],startCoord[Y_AXIS],startCoord[Z_AXIS],
                              stepper.get_axis_position_mm(E_AXIS), feedrate, 0);
          stepper.synchronize();

          delay(250);
          startCount = get_position();

          //do_blocking_move_to(endCoord[X_AXIS],endCoord[Y_AXIS],endCoord[Z_AXIS]);

          planner.buffer_line(endCoord[X_AXIS],endCoord[Y_AXIS],endCoord[Z_AXIS],
                              stepper.get_axis_position_mm(E_AXIS), feedrate, 0);
          stepper.synchronize();

          //Read encoder distance

          delay(250);
          stopCount = get_position();

          travelledDistance = mm_from_count(abs(stopCount - startCount));

          SERIAL_ECHOPGM("Attempted to travel: ");
          SERIAL_ECHO(travelDistance);
          SERIAL_ECHOLNPGM("mm.");

          SERIAL_ECHOPGM("Actually travelled:  ");
          SERIAL_ECHO(travelledDistance);
          SERIAL_ECHOLNPGM("mm.");

          //Calculate new axis steps per unit
          oldStepsMm = planner.axis_steps_per_mm[encoderAxis];
          newStepsMm = (oldStepsMm * travelDistance) / travelledDistance;

          SERIAL_ECHOPGM("Old steps per mm: ");
          SERIAL_ECHOLN(oldStepsMm);

          SERIAL_ECHOPGM("New steps per mm: ");
          SERIAL_ECHOLN(newStepsMm);

          //Save new value
          planner.axis_steps_per_mm[encoderAxis] = newStepsMm;

          if(iter > 1) {
            total += newStepsMm;

            //swap start and end points so next loop runs from current position
            float tempCoord = startCoord[encoderAxis];
            startCoord[encoderAxis] = endCoord[encoderAxis];
            endCoord[encoderAxis] = tempCoord;
          }
        }

        if(iter > 1) {
          total = total / (float) iter;
          SERIAL_ECHOPGM("Average steps per mm: ");
          SERIAL_ECHOLN(total);
        }

        errorCorrect = true;

        SERIAL_ECHOLNPGM("Calculated steps per mm has been set. Please save to EEPROM (M500) if you wish to keep these values.");
      } else {
        SERIAL_ECHOLNPGM("Automatic steps / mm calibration not supported for this axis.");
      }
    }
  }

  void I2CEncoder::set_zeroed() {
    Wire.beginTransmission(i2cAddress);
    Wire.write(I2CPE_RESET_COUNT);
    Wire.endTransmission();
    #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
      ZERO(errorArray);
  //    SERIAL_ECHOLNPGM(" homed");
    #endif
  }

  int I2CEncoder::get_error_count() {
    return errorCount;
  }

  void I2CEncoder::set_error_count(int newCount) {
    errorCount = newCount;
  }

  bool I2CEncoder::get_error_correct_enabled() {
    return errorCorrect;
  }

  void I2CEncoder::set_error_correct_enabled(bool enabled) {
    errorCorrect = enabled;
  }

  uint8_t I2CEncoder::get_error_correct_method() {
    return errorCorrectMethod;
  }

  void I2CEncoder::set_error_correct_method(uint8_t method) {
    errorCorrectMethod = method;
  }

  float I2CEncoder::get_error_correct_threshold() {
    return errorCorrectThreshold;
  }

  void I2CEncoder::set_error_correct_threshold(float newThreshold) {
    errorCorrectThreshold = newThreshold;
  }

  int I2CEncoder::get_encoder_ticks_mm() {
    switch(get_encoder_type()) {
      default:
      case I2CPE_ENC_TYPE_LINEAR: return get_encoder_ticks_unit();
      case I2CPE_ENC_TYPE_ROTARY: return (int)((get_encoder_ticks_unit() / get_stepper_ticks()) * planner.axis_steps_per_mm[encoderAxis]);
    }
  }

  int I2CEncoder::get_encoder_ticks_unit() {
    return encoderTicksPerUnit;
  }

  void I2CEncoder::set_encoder_ticks_unit(int ticks) {
    encoderTicksPerUnit = ticks;
  }

  uint8_t I2CEncoder::get_encoder_type() {
    return encoderType;
  }

  void I2CEncoder::set_encoder_type(uint8_t type) {
    encoderType = type;
  }

  int I2CEncoder::get_stepper_ticks() {
    return stepperTicks;
  }

  void I2CEncoder::set_stepper_ticks(int ticks) {
    stepperTicks = ticks;
  }

  float I2CEncoder::get_axis_offset() {
    return axisOffset;
  }

  void I2CEncoder::set_current_position(float newPositionMm) {
    set_axis_offset(get_position_mm() - newPositionMm + get_axis_offset());
  }

  void I2CEncoder::set_axis_offset(float newOffset) {
    axisOffset = newOffset;
    axisOffsetTicks = axisOffset * get_encoder_ticks_mm();
  }


  void EncoderManager::init() {

    //there's probably a better way to do all this, maybe something using a macro...?

    #if defined(I2C_ENCODER_1_ADDR) && defined(I2C_ENCODER_1_AXIS)
      encoderArray[I2C_ENCODER_1_AXIS].init(I2C_ENCODER_1_AXIS, I2C_ENCODER_1_ADDR, I2C_ENCODER_1_INVERT);
      encoderArray[I2C_ENCODER_1_AXIS].set_active(encoderArray[I2C_ENCODER_1_AXIS].passes_test(true));
      #if defined (I2C_ENCODER_1_EC_METHOD)
        encoderArray[I2C_ENCODER_1_AXIS].set_error_correct_method(I2C_ENCODER_1_EC_METHOD);
      #endif
      #if defined (I2C_ENCODER_1_EC_THRESHOLD)
        encoderArray[I2C_ENCODER_1_AXIS].set_error_correct_threshold(I2C_ENCODER_1_EC_THRESHOLD);
      #endif
      #if defined (I2C_ENCODER_1_TICKS_PER_UNIT)
        encoderArray[I2C_ENCODER_1_AXIS].set_encoder_ticks_unit(I2C_ENCODER_1_TICKS_PER_UNIT);
      #endif
      #if defined (I2C_ENCODER_1_ENCODER_TYPE)
        encoderArray[I2C_ENCODER_1_AXIS].set_encoder_type(I2C_ENCODER_1_ENCODER_TYPE);
      #endif
      #if defined (I2C_ENCODER_1_STEPPER_TICKS_REVOLUTION)
        encoderArray[I2C_ENCODER_1_AXIS].set_stepper_ticks(I2C_ENCODER_1_STEPPER_TICKS_REVOLUTION);
      #endif
      //#if (I2C_ENCODER_1_AXIS == E_AXIS)
      //  encoderArray[index].set_homed();
      //#endif
    #endif

    #if defined(I2C_ENCODER_2_ADDR) && defined(I2C_ENCODER_2_AXIS)
      encoderArray[I2C_ENCODER_2_AXIS].init(I2C_ENCODER_2_AXIS,I2C_ENCODER_2_ADDR,I2C_ENCODER_2_INVERT);

      encoderArray[I2C_ENCODER_2_AXIS].set_active(encoderArray[I2C_ENCODER_2_AXIS].passes_test(true));
      #if defined (I2C_ENCODER_2_EC_METHOD)
        encoderArray[I2C_ENCODER_2_AXIS].set_error_correct_method(I2C_ENCODER_2_EC_METHOD);
      #endif
      #if defined (I2C_ENCODER_2_EC_THRESHOLD)
        encoderArray[I2C_ENCODER_2_AXIS].set_error_correct_threshold(I2C_ENCODER_2_EC_THRESHOLD);
      #endif
      #if defined (I2C_ENCODER_2_TICKS_PER_UNIT)
        encoderArray[I2C_ENCODER_2_AXIS].set_encoder_ticks_unit(I2C_ENCODER_2_TICKS_PER_UNIT);
      #endif
      #if defined (I2C_ENCODER_2_ENCODER_TYPE)
        encoderArray[I2C_ENCODER_2_AXIS].set_encoder_type(I2C_ENCODER_2_ENCODER_TYPE);
      #endif
      #if defined (I2C_ENCODER_2_STEPPER_TICKS_REVOLUTION)
        encoderArray[I2C_ENCODER_2_AXIS].set_stepper_ticks(I2C_ENCODER_2_STEPPER_TICKS_REVOLUTION);
      #endif
  //    #if (I2C_ENCODER_2_AXIS == E_AXIS)
  //      encoderArray[I2C_ENCODER_2_AXIS].set_homed();
  //    #endif
    #endif

    #if defined(I2C_ENCODER_3_ADDR) && defined(I2C_ENCODER_3_AXIS)
      encoderArray[I2C_ENCODER_3_AXIS].init(I2C_ENCODER_3_AXIS,I2C_ENCODER_3_ADDR, I2C_ENCODER_3_INVERT);
      encoderArray[I2C_ENCODER_3_AXIS].set_active(encoderArray[I2C_ENCODER_3_AXIS].passes_test(true));
      #if defined (I2C_ENCODER_3_EC_METHOD)
        encoderArray[I2C_ENCODER_3_AXIS].set_error_correct_method(I2C_ENCODER_3_EC_METHOD);
      #endif
      #if defined (I2C_ENCODER_3_EC_THRESHOLD)
        encoderArray[I2C_ENCODER_3_AXIS].set_error_correct_threshold(I2C_ENCODER_3_EC_THRESHOLD);
      #endif
      #if defined (I2C_ENCODER_3_TICKS_PER_UNIT)
        encoderArray[I2C_ENCODER_3_AXIS].set_encoder_ticks_unit(I2C_ENCODER_3_TICKS_PER_UNIT);
      #endif
      #if defined (I2C_ENCODER_3_ENCODER_TYPE)
        encoderArray[I2C_ENCODER_3_AXIS].set_encoder_type(I2C_ENCODER_3_ENCODER_TYPE);
      #endif
      #if defined (I2C_ENCODER_3_STEPPER_TICKS_REVOLUTION)
        encoderArray[I2C_ENCODER_3_AXIS].set_stepper_ticks(I2C_ENCODER_3_STEPPER_TICKS_REVOLUTION);
      #endif
  //    #if (I2C_ENCODER_3_AXIS == E_AXIS)
  //      encoderArray[I2C_ENCODER_3_AXIS].set_homed();
  //    #endif
    #endif

    #if defined(I2C_ENCODER_4_ADDR) && defined(I2C_ENCODER_4_AXIS)
      encoderArray[I2C_ENCODER_4_AXIS].init(I2C_ENCODER_4_AXIS,I2C_ENCODER_4_ADDR,I2C_ENCODER_4_INVERT);
      encoderArray[I2C_ENCODER_4_AXIS].set_active(encoderArray[I2C_ENCODER_4_AXIS].passes_test(true));
      #if defined (I2C_ENCODER_4_EC_METHOD)
        encoderArray[I2C_ENCODER_4_AXIS].set_error_correct_method(I2C_ENCODER_4_EC_METHOD);
      #endif
      #if defined (I2C_ENCODER_4_EC_THRESHOLD)
        encoderArray[I2C_ENCODER_4_AXIS].set_error_correct_threshold(I2C_ENCODER_4_EC_THRESHOLD);
      #endif
      #if defined (I2C_ENCODER_4_TICKS_PER_UNIT)
        encoderArray[I2C_ENCODER_4_AXIS].set_encoder_ticks_unit(I2C_ENCODER_4_TICKS_PER_UNIT);
      #endif
      #if defined (I2C_ENCODER_4_ENCODER_TYPE)
        encoderArray[I2C_ENCODER_4_AXIS].set_encoder_type(I2C_ENCODER_4_ENCODER_TYPE);
      #endif
      #if defined (I2C_ENCODER_4_STEPPER_TICKS_REVOLUTION)
        encoderArray[I2C_ENCODER_4_AXIS].set_stepper_ticks(I2C_ENCODER_4_STEPPER_TICKS_REVOLUTION);
      #endif
  //    #if (I2C_ENCODER_4_AXIS == E_AXIS)
  //      encoderArray[I2C_ENCODER_4_AXIS].set_homed();
  //    #endif
    #endif
  /*
    #if defined(I2C_ENCODER_5_ADDR) && defined(I2C_ENCODER_5_AXIS)
      encoderArray[I2C_ENCODER_5_AXIS].init(I2C_ENCODER_5_AXIS,I2C_ENCODER_5_ADDR,I2C_ENCODER_5_INVERT);
      encoderArray[I2C_ENCODER_5_AXIS].set_active(encoderArray[index].passes_test(true));
      #if defined (I2C_ENCODER_5_EC_METHOD)
        encoderArray[I2C_ENCODER_5_AXIS].set_error_correct_method(I2C_ENCODER_5_EC_METHOD);
      #endif
      #if defined (I2C_ENCODER_5_EC_THRESHOLD)
        encoderArray[I2C_ENCODER_5_AXIS].set_error_correct_threshold(I2C_ENCODER_5_EC_THRESHOLD);
      #endif
      #if defined (I2C_ENCODER_5_TICKS_PER_UNIT)
        encoderArray[I2C_ENCODER_5_AXIS].set_encoder_ticks_unit(I2C_ENCODER_5_TICKS_PER_UNIT);
      #endif
      #if defined (I2C_ENCODER_5_ENCODER_TYPE)
        encoderArray[I2C_ENCODER_5_AXIS].set_encoder_type(I2C_ENCODER_5_ENCODER_TYPE);
      #endif
      #if defined (I2C_ENCODER_5_STEPPER_TICKS_REVOLUTION)
        encoderArray[I2C_ENCODER_5_AXIS].set_stepper_ticks(I2C_ENCODER_5_STEPPER_TICKS_REVOLUTION);
      #endif
      #if (I2C_ENCODER_5_AXIS == E_AXIS)
        encoderArray[I2C_ENCODER_5_AXIS].set_homed();
      #endif
    #endif
  */
  }

  void EncoderManager::report_position(AxisEnum axis, bool units, bool noOffset) {
    LOOP_NA(i) {
      if(encoderArray[i].get_axis() == axis && encoderArray[i].get_active()) {
        if(units)
          SERIAL_ECHOLN(noOffset ? encoderArray[i].mm_from_count(encoderArray[i].get_raw_count()) : encoderArray[i].get_position_mm());
        else {
          if(noOffset) {
            long raw_count = encoderArray[i].get_raw_count();
            SERIAL_ECHO(axis_codes[encoderArray[i].get_axis()]);
            SERIAL_ECHOPGM(" ");

            for (uint8_t j = 31; j > 0; j--)
              SERIAL_ECHO((bool)(0x00000001 & (raw_count >> j)));

            SERIAL_ECHO((bool)(0x00000001 & (raw_count)));
            SERIAL_ECHOPGM(" ");
            SERIAL_ECHOLN(raw_count);
          } else
            SERIAL_ECHOLN(encoderArray[i].get_position());
        }
        break;
      }
    }
  }



  void EncoderManager::report_status(AxisEnum axis) {
    bool responded = false;

    LOOP_NA(i) {
      if(encoderArray[i].get_axis() == axis) {
        encoderArray[i].passes_test(true);
        responded = true;
        break;
      //} else {
        //SERIAL_ECHOPGM("Encoder not operational");
      }
    }

    if (!responded) SERIAL_ECHOLNPGM("No encoder configured for given axis!");
  }

  void EncoderManager::change_module_address(int oldaddr, int newaddr) {
    uint8_t error;

    //first check 'new' address is not in use
    Wire.beginTransmission(newaddr);
    error = Wire.endTransmission();

    if(error == 0) {
      SERIAL_ECHOLNPGM("Warning! There is already a device with that address on the I2C bus!");
    } else {

      //now check that we can find the module on the oldaddr address
      Wire.beginTransmission(oldaddr);
      error = Wire.endTransmission();

      if(error == 0) {

        SERIAL_ECHOPGM("Module found at ");
        SERIAL_ECHO(oldaddr);
        SERIAL_ECHOLNPGM(", changing address...");

        //change the modules address
        Wire.beginTransmission(oldaddr);
        Wire.write(I2CPE_SET_ADDR);
        Wire.write(newaddr);
        Wire.endTransmission();

        SERIAL_ECHOLNPGM("Address changed, waiting for confirmation...");

        //Wait for the module to reset (can probably be improved by polling address with a timeout)
        long startWaiting = millis();
        while(millis() - startWaiting < I2CPE_REBOOT_TIME) {
          idle();
          delay(500);
        }

        //look for the module at the new address
        Wire.beginTransmission(newaddr);
        error = Wire.endTransmission();

        if(error == 0) {
          SERIAL_ECHOLNPGM("Confirmed! Address change successful.");

          // now, if this module is supposed to be used, find which encoder instance it corresponds to and enable it
          // (it will likely have failed initialisation on power-up, before the address change)
          LOOP_NA(i) {
            if(encoderArray[i].get_address() == newaddr) {
              if(encoderArray[i].get_active() == false) {
                SERIAL_ECHO(axis_codes[encoderArray[i].get_axis()]);
                SERIAL_ECHOLNPGM(" axis encoder was not detected on printer startup. Trying again now address is correct...");
                encoderArray[i].set_active(encoderArray[i].passes_test(true));
              }
              break;
            }
          }
        } else {
          SERIAL_ECHOLNPGM("Failed. Please check encoder module.");
        }

      } else {
        SERIAL_ECHOLNPGM("No module detected!");
      }
    }
  }


  void EncoderManager::check_module_firmware(int address) {

    //first check there is a module
    Wire.beginTransmission(address);
    int error = Wire.endTransmission();

    if(error != 0) {
      SERIAL_ECHOLNPGM("Warning! No module detected at given address!");
    } else {

      SERIAL_ECHOPGM("Requesting version info from module at address ");
      SERIAL_ECHOLN(address);

      Wire.beginTransmission(address);

      Wire.write(I2CPE_SET_REPORT_MODE);
      Wire.write(I2CPE_REPORT_MODE_VERSION);
      Wire.endTransmission();

      //Read value
      Wire.requestFrom((int)address,32);

      uint8_t temp[32] = {0};
      int tempIndex = 0;

      while(Wire.available() > 0) {
        temp[tempIndex] = Wire.read();
        tempIndex++;
      }

      for(int i = 0; i < tempIndex+1; i++) {
        if((char)temp[i] > 0) {
          SERIAL_ECHO((char)temp[i]);
        }
      }

      //Set module back to normal (distance) mode
      Wire.beginTransmission((int)address);
      Wire.write(I2CPE_SET_REPORT_MODE);
      Wire.write(I2CPE_REPORT_MODE_DISTANCE);
      Wire.endTransmission();

    }
  }

  void EncoderManager::report_error_count(AxisEnum axis) {

    LOOP_NA(i) {
      if(encoderArray[i].get_axis() == axis) {
        SERIAL_ECHOPGM("Error count on ");
        SERIAL_ECHO(axis_codes[axis]);
        SERIAL_ECHOPGM(" axis is ");
        SERIAL_ECHO(encoderArray[i].get_error_count());
        SERIAL_ECHOLNPGM(" events.");
        break;
      }
    }
  }

  void EncoderManager::report_error_count() {
    LOOP_NA(i)
      report_error_count((AxisEnum)i);
  }

  void EncoderManager::reset_error_count(AxisEnum axis) {
    LOOP_NA(i) {
      if(encoderArray[i].get_axis() == axis) {
        encoderArray[i].set_error_count(0);
        SERIAL_ECHOPGM("Error count on ");
        SERIAL_ECHO(axis_codes[axis]);
        SERIAL_ECHOLNPGM(" axis has been reset.");
        break;
      }
    }
  }

  void EncoderManager::reset_error_count() {
    LOOP_NA(i)
      reset_error_count((AxisEnum)i);
  }

  void EncoderManager::enable_error_correction(AxisEnum axis, bool enable) {
    LOOP_NA(i) {
      if(encoderArray[i].get_axis() == axis) {
        encoderArray[i].set_error_correct_enabled(enable);
        SERIAL_ECHOPGM("Error correction on ");
        SERIAL_ECHO(axis_codes[axis]);
        SERIAL_ECHOPGM(" axis is ");
        if(encoderArray[i].get_error_correct_enabled()) {
          SERIAL_ECHOLNPGM("enabled.");
        } else {
          SERIAL_ECHOLNPGM("disabled.");
        }
        break;
      }
    }
  }

  void EncoderManager::set_error_correct_threshold(AxisEnum axis, float newThreshold) {
    LOOP_NA(i)
      if(encoderArray[i].get_axis() == axis)
        encoderArray[i].set_error_correct_threshold(newThreshold);
  }

  void EncoderManager::get_error_correct_threshold(AxisEnum axis) {
    float threshold = -999;

    LOOP_NA(i) {
      if(encoderArray[i].get_axis() == axis) {
        threshold = encoderArray[i].get_error_correct_threshold();
        break;
      }
    }

    if(threshold != -999) {
      SERIAL_ECHOPGM("Error correct threshold on ");
      SERIAL_ECHO(axis_codes[axis]);
      SERIAL_ECHOPGM(" axis is ");
      SERIAL_ECHO(threshold);
      SERIAL_ECHOLNPGM("mm.");
    }
  }

  int EncoderManager::get_encoder_index_from_axis(AxisEnum axis) {
    int index = -1;
    LOOP_NA(i) {
      if(encoderArray[i].get_axis() == axis) {
        index = i;
        break;
      }
    }
    return index;
  }

#endif
