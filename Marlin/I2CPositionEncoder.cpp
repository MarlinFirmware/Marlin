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

//todo:  add support for multiple encoders on a single axis
//todo:    add z axis auto-leveling
//todo:  consolidate some of the related M codes?
//todo:  add endstop-replacement mode?
//todo:  try faster I2C speed; tweak TWI_FREQ (400000L, or faster?); or just TWBR = ((CPU_FREQ / 400000L) - 16) / 2;
//todo:    consider Marlin-optimized Wire library; i.e. MarlinWire, like MarlinSerial


#include "MarlinConfig.h"

#if ENABLED(I2C_POSITION_ENCODERS)

  #include "Marlin.h"
  #include "temperature.h"
  #include "stepper.h"
  #include "I2CPositionEncoder.h"
  #include "gcode.h"

  #include <Wire.h>

  void I2CPositionEncoder::init(uint8_t address, AxisEnum axis) {
    encoderAxis = axis;
    i2cAddress = address;

    initialised++;

    SERIAL_ECHOPAIR("Seetting up encoder on ", axis_codes[encoderAxis]);
    SERIAL_ECHOLNPAIR(" axis, addr = ", address);

    position = get_position();
  }

  void I2CPositionEncoder::update() {
    if (!initialised || !homed || !active) return; //check encoder is set up and active

    position = get_position();

    //we don't want to stop things just because the encoder missed a message,
    //so we only care about responses that indicate bad magnetic strength

    if (!passes_test(false)) { //check encoder data is good
      lastErrorTime = millis();
      /*
      if (trusted) { //commented out as part of the note below
        trusted = false;
        SERIAL_ECHOPGM("Fault detected on ");
        SERIAL_ECHO(axis_codes[encoderAxis]);
        SERIAL_ECHOLNPGM(" axis encoder. Disengaging error correction until module is trusted again.");
      }
      */
      return;
    }

    if (!trusted) {
      /**
       * This is commented out because it introduces error and can cause bad print quality.
       *
       * This code is intended to manage situations where the encoder has reported bad magnetic strength.
       * This indicates that the magnetic strip was too far away from the sensor to reliably track position.
       * When this happens, this code resets the offset based on where the printer thinks it is. This has been
       * shown to introduce errors in actual position which result in drifting prints and poor print quality.
       * Perhaps a better method would be to disable correction on the axis with a problem, report it to the
       * user via the status leds on the encoder module and prompt the user to re-home the axis at which point
       * the encoder would be re-enabled.
       */

      /*
        // If the magnetic strength has been good for a certain time, start trusting the module again

        if (millis() - lastErrorTime > I2CPE_TIME_TRUSTED) {
          trusted = true;

          SERIAL_ECHOPGM("Untrusted encoder module on ");
          SERIAL_ECHO(axis_codes[encoderAxis]);
          SERIAL_ECHOLNPGM(" axis has been fault-free for set duration, reinstating error correction.");

          //the encoder likely lost its place when the error occured, so we'll reset and use the printer's
          //idea of where it the axis is to re-initialise
          double position = stepper.get_axis_position_mm(encoderAxis);
          long positionInTicks = position * get_ticks_unit();

          //shift position from previous to current position
          zeroOffset -= (positionInTicks - get_position());

          #if defined(I2CPE_DEBUG)
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
      */
      return;
    }

    lastPosition = position;
    unsigned long positionTime = millis();

    //only do error correction if setup and enabled
    if (ec && ecMethod != I2CPE_ECM_NONE) {

      #if defined(I2CPE_EC_THRESH_PROPORTIONAL)
        unsigned long distance = abs(position - lastPosition);
        unsigned long deltaTime = positionTime - lastPositionTime;
        unsigned long speed = distance / deltaTime;
        float threshold = constrain((speed / 50), 1, 50) * ecThreshold;
      #else
        float threshold = get_error_correct_threshold();
      #endif

      //check error
      #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
        double sum = 0, diffSum = 0;

        errIdx = (errIdx >= I2CPE_ERR_ARRAY_SIZE - 1) ? 0 : errIdx + 1;
        err[errIdx] = get_axis_error_steps(false);

        LOOP_L_N(i, I2CPE_ERR_ARRAY_SIZE) {
          sum += err[i];
          if (i) diffSum += abs(err[i-1] - err[i]);
        }

        long error = (long)(sum/(I2CPE_ERR_ARRAY_SIZE + 1)); //calculate average for error

      #else
        long error = get_axis_error_steps(false);
      #endif

      //SERIAL_ECHOPGM("Axis err*r steps: ");
      //SERIAL_ECHOLN(error);

      #if defined(I2CPE_ERR_THRESH_ABORT)
        if (abs(error) > I2CPE_ERR_THRESH_ABORT * planner.axis_steps_per_mm[encoderAxis]) {
          //kill("Significant Error");
          SERIAL_ECHOPGM("Axis error greater than set threshold, aborting!");
          SERIAL_ECHOLN(error);
          safe_delay(5000);
        }
      #endif

      #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
        if (errIdx == 0) {
          // in order to correct for "error" but avoid correcting for noise and non skips
          // it must be > threshold and have a difference average of < 10 and be < 2000 steps
          if (abs(error) > threshold * planner.axis_steps_per_mm[encoderAxis] &&
              diffSum < 10*(I2CPE_ERR_ARRAY_SIZE-1) && abs(error) < 2000) { //Check for persistent error (skip)
            SERIAL_ECHO(axis_codes[encoderAxis]);
            SERIAL_ECHOPAIR(" diffSum: ", diffSum/(I2CPE_ERR_ARRAY_SIZE-1));
            SERIAL_ECHOPAIR(" - err detected: ", error / planner.axis_steps_per_mm[encoderAxis]);
            SERIAL_ECHOLNPGM("mm; correcting!");
            thermalManager.babystepsTodo[encoderAxis] = -lround(error);
          }
        }
      #else
        if (abs(error) > threshold * planner.axis_steps_per_mm[encoderAxis]) {
          //SERIAL_ECHOLN(error);
          //SERIAL_ECHOLN(position);
          thermalManager.babystepsTodo[encoderAxis] = -lround(error/2);
        }
      #endif

      if (abs(error) > (I2CPE_ERR_CNT_THRESH * planner.axis_steps_per_mm[encoderAxis]) && millis() - lastErrorCountTime > I2CPE_ERR_CNT_DEBOUNCE_MS) {
        SERIAL_ECHOPAIR("Large error on ", axis_codes[encoderAxis]);
        SERIAL_ECHOPAIR(" axis. error: ", (int)error);
        SERIAL_ECHOLNPAIR("; diffSum: ", diffSum);
        errorCount++;
        lastErrorCountTime = millis();
      }
    }

    lastPositionTime = positionTime;
  }

  void I2CPositionEncoder::set_homed() {
    if (active) {
      reset();  // Reset module's offset to zero (so current position is homed / zero)
      delay(10);

      zeroOffset = get_raw_count();
      homed++;
      trusted++;

      #if defined(I2CPE_DEBUG)
        SERIAL_ECHO(axis_codes[encoderAxis]);
        SERIAL_ECHOPAIR(" axis encoder homed, offset of ", zeroOffset);
        SERIAL_ECHOLNPGM(" ticks.");
      #endif
    }
  }

  bool I2CPositionEncoder::passes_test(bool report) {
    if (H == I2CPE_MAG_SIG_GOOD) {
      if (report) {
        SERIAL_ECHO(axis_codes[encoderAxis]);
        SERIAL_ECHOLNPGM(" axis encoder passes test; field strength good.");
      }
      return true;
    } else if (H == I2CPE_MAG_SIG_MID) {
      if (report) {
        SERIAL_ECHOPAIR("Warning, ", axis_codes[encoderAxis]);
        SERIAL_ECHOLNPGM(" axis encoder passes test; field strength fair.");
      }
      return true;
    } else if (H == I2CPE_MAG_SIG_BAD) {
      if (report) {
        SERIAL_ECHOPAIR("Warning, ", axis_codes[encoderAxis]);
        SERIAL_ECHOLNPGM(" axis magnetic strip not detected!");
      }
      return false;
    }

    if (report) {
      SERIAL_ECHOPAIR("Warning, ", axis_codes[encoderAxis]);
      SERIAL_ECHOLNPGM(" axis encoder not detected!");
    }
    return false;
  }

  double I2CPositionEncoder::get_axis_error_mm(bool report) {
    double target, actual, error;

    target = stepper.get_axis_position_mm(encoderAxis);
    actual = mm_from_count(position);
    error = actual - target;

    if (abs(error) > 10000) error = 0; // ?

    if (report) {
      SERIAL_ECHO(axis_codes[encoderAxis]);
      SERIAL_ECHOPAIR(" axis target: ", target);
      SERIAL_ECHOPAIR(", actual: ", actual);
      SERIAL_ECHOLNPAIR(", error : ",error);
    }

    return error;
  }

  long I2CPositionEncoder::get_axis_error_steps(bool report) {
    if (!active) {
      if (report) {
        SERIAL_ECHO(axis_codes[encoderAxis]);
        SERIAL_ECHOLNPGM(" axis encoder not active!");
      }
      return 0;
    }

    float stepperTicksPerUnit;
    long encoderTicks = position, encoderCountInStepperTicksScaled;
    //long stepperTicks = stepper.position(encoderAxis);

    // With a rotary encoder we're concerned with ticks/rev; whereas with a linear we're concerned with ticks/mm
    stepperTicksPerUnit = (type == I2CPE_ENC_TYPE_ROTARY) ? stepperTicks : planner.axis_steps_per_mm[encoderAxis];

    //convert both 'ticks' into same units / base
    encoderCountInStepperTicksScaled = lround((stepperTicksPerUnit * encoderTicks) / encoderTicksPerUnit);

    long target = stepper.position(encoderAxis),
         error = (encoderCountInStepperTicksScaled - target);

    //suppress discontinuities (might be caused by bad I2C readings...?)
    bool suppressOutput = (abs(error - errorPrev) > 100);

    if (report) {
      SERIAL_ECHO(axis_codes[encoderAxis]);
      SERIAL_ECHOPAIR(" axis target: ", target);
      SERIAL_ECHOPAIR(", actual: ", encoderCountInStepperTicksScaled);
      SERIAL_ECHOLNPAIR(", error : ", error);

      if (suppressOutput) SERIAL_ECHOLNPGM("Discontinuity detected, suppressing error.");
    }

    errorPrev = error;

    return (suppressOutput ? 0 : error);
  }

  long I2CPositionEncoder::get_raw_count() {
    uint8_t index = 0;
    i2cLong encoderCount;

    encoderCount.val = 0x00;

    if (Wire.requestFrom((int)i2cAddress, 3) != 3) {
      //houston, we have a problem...
      H = I2CPE_MAG_SIG_NF;
      return 0;
    }

    while (Wire.available())
      encoderCount.bval[index++] = (uint8_t)Wire.read();

    //extract the magnetic strength
    H = (B00000011 & (encoderCount.bval[2] >> 6));

    //extract sign bit; sign = (encoderCount.bval[2] & B00100000);
    //set all upper bits to the sign value to overwrite H
    encoderCount.val = (encoderCount.bval[2] & B00100000) ? (encoderCount.val | 0xFFC00000) : (encoderCount.val & 0x003FFFFF);

    if (invert) encoderCount.val *= -1;

    return encoderCount.val;
  }

  bool I2CPositionEncoder::test_axis() {
    //only works on XYZ cartesian machines for the time being
    if (!(encoderAxis == X_AXIS || encoderAxis == Y_AXIS || encoderAxis == Z_AXIS)) return false;

    int feedrate;
    float startPosition, endPosition;
    float startCoord[NUM_AXIS] = {0}, endCoord[NUM_AXIS] = {0};

    startPosition = soft_endstop_min[encoderAxis] + 10;
    endPosition = soft_endstop_max[encoderAxis] - 10;

    feedrate = (int)MMM_TO_MMS((encoderAxis == Z_AXIS) ? HOMING_FEEDRATE_Z : HOMING_FEEDRATE_XY);

    ec = false;

    LOOP_NA(i) {
      startCoord[i] = stepper.get_axis_position_mm((AxisEnum)i);
      endCoord[i] = stepper.get_axis_position_mm((AxisEnum)i);
    }

    startCoord[encoderAxis] = startPosition;
    endCoord[encoderAxis] = endPosition;

    stepper.synchronize();

    planner.buffer_line(startCoord[X_AXIS],startCoord[Y_AXIS],startCoord[Z_AXIS],
                        stepper.get_axis_position_mm(E_AXIS), feedrate, 0);
    stepper.synchronize();

    // if the module isn't currently trusted, wait until it is (or until it should be if things are working)
    if (!trusted) {
      long startWaitingTime = millis();
      while (!trusted && millis() - startWaitingTime < I2CPE_TIME_TRUSTED)
        safe_delay(500);
    }

    if (trusted) { // if trusted, commence test
      planner.buffer_line(endCoord[X_AXIS], endCoord[Y_AXIS], endCoord[Z_AXIS],
                          stepper.get_axis_position_mm(E_AXIS), feedrate, 0);
      stepper.synchronize();
    }

    return trusted;
  }

  void I2CPositionEncoder::calibrate_steps_mm(int iter) {
    if (type != I2CPE_ENC_TYPE_LINEAR) {
      SERIAL_ECHOLNPGM("Steps per mm calibration is only available using linear encoders.");
      return;
    }

    if (!(encoderAxis == X_AXIS || encoderAxis == Y_AXIS || encoderAxis == Z_AXIS)) {
      SERIAL_ECHOLNPGM("Automatic steps / mm calibration not supported for this axis.");
      return;
    }

    float oldStepsMm, newStepsMm,
          startDistance, endDistance,
          travelDistance, travelledDistance, total = 0,
          startCoord[NUM_AXIS] = {0}, endCoord[NUM_AXIS] = {0};

    double feedrate;

    long startCount, stopCount;

    feedrate = MMM_TO_MMS((encoderAxis == Z_AXIS) ? HOMING_FEEDRATE_Z : HOMING_FEEDRATE_XY);

    bool oldec = ec;
    ec = false;

    startDistance = 20;
    endDistance = soft_endstop_max[encoderAxis] - 20;
    travelDistance = endDistance - startDistance;

    LOOP_NA(i) {
      startCoord[i] = stepper.get_axis_position_mm((AxisEnum)i);
      endCoord[i] = stepper.get_axis_position_mm((AxisEnum)i);
    }

    startCoord[encoderAxis] = startDistance;
    endCoord[encoderAxis] = endDistance;

    LOOP_L_N(i, iter) {
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

      SERIAL_ECHOPAIR("Attempted to travel: ", travelDistance);
      SERIAL_ECHOLNPGM("mm.");

      SERIAL_ECHOPAIR("Actually travelled:  ", travelledDistance);
      SERIAL_ECHOLNPGM("mm.");

      //Calculate new axis steps per unit
      oldStepsMm = planner.axis_steps_per_mm[encoderAxis];
      newStepsMm = (oldStepsMm * travelDistance) / travelledDistance;

      SERIAL_ECHOLNPAIR("Old steps per mm: ", oldStepsMm);
      SERIAL_ECHOLNPAIR("New steps per mm: ", newStepsMm);

      //Save new value
      planner.axis_steps_per_mm[encoderAxis] = newStepsMm;

      if (iter > 1) {
        total += newStepsMm;

        // swap start and end points so next loop runs from current position
        float tempCoord = startCoord[encoderAxis];
        startCoord[encoderAxis] = endCoord[encoderAxis];
        endCoord[encoderAxis] = tempCoord;
      }
    }

    if (iter > 1) {
      total /= (float)iter;
      SERIAL_ECHOLNPAIR("Average steps per mm: ", total);
    }

    ec = oldec;

    SERIAL_ECHOLNPGM("Calculated steps per mm has been set. Please save to EEPROM (M500) if you wish to keep these values.");
  }

  void I2CPositionEncoder::reset() {
    Wire.beginTransmission(i2cAddress);
    Wire.write(I2CPE_RESET_COUNT);
    Wire.endTransmission();

    #if ENABLED(I2CPE_ERR_ROLLING_AVERAGE)
      ZERO(err);
    #endif
  }

  void I2CPositionEncodersMgr::init() {
    Wire.begin();

    #if I2CPE_ENCODER_CNT > 0
      uint8_t i = 0;

      encoders[i].init(I2CPE_ENC_1_ADDR, I2CPE_ENC_1_AXIS);

      #if defined(I2CPE_ENC_1_TYPE)
        encoders[i].set_type(I2CPE_ENC_1_TYPE);
      #endif
      #if defined(I2CPE_ENC_1_TICKS_UNIT)
        encoders[i].set_ticks_unit(I2CPE_ENC_1_TICKS_UNIT);
      #endif
      #if defined(I2CPE_ENC_1_TICKS_REV)
        encoders[i].set_stepper_ticks(I2CPE_ENC_1_TICKS_REV);
      #endif
      #if defined(I2CPE_ENC_1_INVERT)
        encoders[i].set_inverted(I2CPE_ENC_1_INVERT);
      #endif
      #if defined(I2CPE_ENC_1_EC_METHOD)
        encoders[i].set_ec_method(I2CPE_ENC_1_EC_METHOD);
      #endif
      #if defined(I2CPE_ENC_1_EC_THRESH)
        encoders[i].set_ec_threshold(I2CPE_ENC_1_EC_THRESH);
      #endif

      encoders[i].set_active(encoders[i].passes_test(true));

      #if (I2CPE_ENC_1_AXIS == E_AXIS)
        encoders[i].set_homed();
      #endif
    #endif

    #if I2CPE_ENCODER_CNT > 1
      i++;

      encoders[i].init(I2CPE_ENC_2_ADDR, I2CPE_ENC_2_AXIS);

      #if defined(I2CPE_ENC_2_TYPE)
        encoders[i].set_type(I2CPE_ENC_2_TYPE);
      #endif
      #if defined(I2CPE_ENC_2_TICKS_UNIT)
        encoders[i].set_ticks_unit(I2CPE_ENC_2_TICKS_UNIT);
      #endif
      #if defined(I2CPE_ENC_2_TICKS_REV)
        encoders[i].set_stepper_ticks(I2CPE_ENC_2_TICKS_REV);
      #endif
      #if defined(I2CPE_ENC_2_INVERT)
        encoders[i].set_inverted(I2CPE_ENC_2_INVERT);
      #endif
      #if defined(I2CPE_ENC_2_EC_METHOD)
        encoders[i].set_ec_method(I2CPE_ENC_2_EC_METHOD);
      #endif
      #if defined(I2CPE_ENC_2_EC_THRESH)
        encoders[i].set_ec_threshold(I2CPE_ENC_2_EC_THRESH);
      #endif

      encoders[i].set_active(encoders[i].passes_test(true));

      #if (I2CPE_ENC_2_AXIS == E_AXIS)
        encoders[i].set_homed();
      #endif
    #endif

    #if I2CPE_ENCODER_CNT > 2
      i++;

      encoders[i].init(I2CPE_ENC_3_ADDR, I2CPE_ENC_3_AXIS);

      #if defined(I2CPE_ENC_3_TYPE)
        encoders[i].set_type(I2CPE_ENC_3_TYPE);
      #endif
      #if defined(I2CPE_ENC_3_TICKS_UNIT)
        encoders[i].set_ticks_unit(I2CPE_ENC_3_TICKS_UNIT);
      #endif
      #if defined(I2CPE_ENC_3_TICKS_REV)
        encoders[i].set_stepper_ticks(I2CPE_ENC_3_TICKS_REV);
      #endif
      #if defined(I2CPE_ENC_3_INVERT)
        encoders[i].set_inverted(I2CPE_ENC_3_INVERT);
      #endif
      #if defined(I2CPE_ENC_3_EC_METHOD)
        encoders[i].set_ec_method(I2CPE_ENC_3_EC_METHOD);
      #endif
      #if defined(I2CPE_ENC_3_EC_THRESH)
        encoders[i].set_ec_threshold(I2CPE_ENC_3_EC_THRESH);
      #endif

    encoders[i].set_active(encoders[i].passes_test(true));

      #if (I2CPE_ENC_3_AXIS == E_AXIS)
        encoders[i].set_homed();
      #endif
    #endif

    #if I2CPE_ENCODER_CNT > 3
      i++;

      encoders[i].init(I2CPE_ENC_4_ADDR, I2CPE_ENC_4_AXIS);

      #if defined(I2CPE_ENC_4_TYPE)
        encoders[i].set_type(I2CPE_ENC_4_TYPE);
      #endif
      #if defined(I2CPE_ENC_4_TICKS_UNIT)
        encoders[i].set_ticks_unit(I2CPE_ENC_4_TICKS_UNIT);
      #endif
      #if defined(I2CPE_ENC_4_TICKS_REV)
        encoders[i].set_stepper_ticks(I2CPE_ENC_4_TICKS_REV);
      #endif
      #if defined(I2CPE_ENC_4_INVERT)
        encoders[i].set_inverted(I2CPE_ENC_4_INVERT);
      #endif
      #if defined(I2CPE_ENC_4_EC_METHOD)
        encoders[i].set_ec_method(I2CPE_ENC_4_EC_METHOD);
      #endif
      #if defined(I2CPE_ENC_4_EC_THRESH)
        encoders[i].set_ec_threshold(I2CPE_ENC_4_EC_THRESH);
      #endif

      encoders[i].set_active(encoders[i].passes_test(true));

      #if (I2CPE_ENC_4_AXIS == E_AXIS)
        encoders[i].set_homed();
      #endif
    #endif

    #if I2CPE_ENCODER_CNT > 4
      i++;

      encoders[i].init(I2CPE_ENC_5_ADDR, I2CPE_ENC_5_AXIS);

      #if defined(I2CPE_ENC_5_TYPE)
        encoders[i].set_type(I2CPE_ENC_5_TYPE);
      #endif
      #if defined(I2CPE_ENC_5_TICKS_UNIT)
        encoders[i].set_ticks_unit(I2CPE_ENC_5_TICKS_UNIT);
      #endif
      #if defined(I2CPE_ENC_5_TICKS_REV)
        encoders[i].set_stepper_ticks(I2CPE_ENC_5_TICKS_REV);
      #endif
      #if defined(I2CPE_ENC_5_INVERT)
        encoders[i].set_inverted(I2CPE_ENC_5_INVERT);
      #endif
      #if defined(I2CPE_ENC_5_EC_METHOD)
        encoders[i].set_ec_method(I2CPE_ENC_5_EC_METHOD);
      #endif
      #if defined(I2CPE_ENC_5_EC_THRESH)
        encoders[i].set_ec_threshold(I2CPE_ENC_5_EC_THRESH);
      #endif

      encoders[i].set_active(encoders[i].passes_test(true));

      #if (I2CPE_ENC_5_AXIS == E_AXIS)
        encoders[i].set_homed();
      #endif
    #endif

  }

  void I2CPositionEncodersMgr::report_position(uint8_t idx, bool units, bool noOffset) {
    CHECK_IDX

    if (units) {
      SERIAL_ECHOLN(noOffset ? encoders[idx].mm_from_count(encoders[idx].get_raw_count()) : encoders[idx].get_position_mm());
    } else {
      if (noOffset) {
        long raw_count = encoders[idx].get_raw_count();
        SERIAL_ECHO(axis_codes[encoders[idx].get_axis()]);
        SERIAL_ECHOPGM(" ");

        for (uint8_t j = 31; j > 0; j--)
          SERIAL_ECHO((bool)(0x00000001 & (raw_count >> j)));

        SERIAL_ECHO((bool)(0x00000001 & (raw_count)));
        SERIAL_ECHOLNPAIR(" ", raw_count);
      } else
        SERIAL_ECHOLN(encoders[idx].get_position());
    }
  }

  void I2CPositionEncodersMgr::change_module_address(uint8_t oldaddr, uint8_t newaddr) {
    // First check 'new' address is not in use
    Wire.beginTransmission(newaddr);
    if (!Wire.endTransmission()) {
      SERIAL_ECHOPAIR("?There is already a device with that address on the I2C bus! (", newaddr);
      SERIAL_ECHOLNPGM(")");
      return;
    }

    // Now check that we can find the module on the oldaddr address
    Wire.beginTransmission(oldaddr);
    if (Wire.endTransmission()) {
      SERIAL_ECHOPAIR("?No module detected at this address! (", oldaddr);
      SERIAL_ECHOLNPGM(")");
      return;
    }

    SERIAL_ECHOPAIR("Module found at ", oldaddr);
    SERIAL_ECHOLNPAIR(", changing address to ", newaddr);

    // Change the modules address
    Wire.beginTransmission(oldaddr);
    Wire.write(I2CPE_SET_ADDR);
    Wire.write(newaddr);
    Wire.endTransmission();

    SERIAL_ECHOLNPGM("Address changed, resetting and waiting for confirmation..");

    // Wait for the module to reset (can probably be improved by polling address with a timeout).
    safe_delay(I2CPE_REBOOT_TIME);

    // Look for the module at the new address.
    Wire.beginTransmission(newaddr);
    if (Wire.endTransmission()) {
      SERIAL_ECHOLNPGM("Address change failed! Check encoder module.");
      return;
    }

    SERIAL_ECHOLNPGM("Address change successful!");

    // Now, if this module is configured, find which encoder instance it's supposed to correspond to
    // and enable it (it will likely have failed initialisation on power-up, before the address change).
    int8_t idx = idx_from_addr(newaddr);
    if (idx >= 0 && !encoders[idx].get_active()) {
      SERIAL_ECHO(axis_codes[encoders[idx].get_axis()]);
      SERIAL_ECHOLNPGM(" axis encoder was not detected on printer startup. Trying again.");
      encoders[idx].set_active(encoders[idx].passes_test(true));
    }
  }

  void I2CPositionEncodersMgr::report_module_firmware(uint8_t address) {
    // First check there is a module
    Wire.beginTransmission(address);
    if (Wire.endTransmission()) {
      SERIAL_ECHOPAIR("?No module detected at this address! (", address);
      SERIAL_ECHOLNPGM(")");
      return;
    }

    SERIAL_ECHOPAIR("Requesting version info from module at address ", address);
    SERIAL_ECHOPGM(":\n");

    Wire.beginTransmission(address);
    Wire.write(I2CPE_SET_REPORT_MODE);
    Wire.write(I2CPE_REPORT_VERSION);
    Wire.endTransmission();

    // Read value
    if (Wire.requestFrom((int)address, 32)) {
      char c;
      while (Wire.available() > 0 && (c = (char)Wire.read()) > 0)
        SERIAL_ECHO(c);
      SERIAL_EOL();
    }

    // Set module back to normal (distance) mode
    Wire.beginTransmission((int)address);
    Wire.write(I2CPE_SET_REPORT_MODE);
    Wire.write(I2CPE_REPORT_DISTANCE);
    Wire.endTransmission();
  }

  int8_t I2CPositionEncodersMgr::parse() {
    I2CPE_addr = 0;

    if (parser.seen('A')) {
      if (!parser.has_value()) {
        SERIAL_PROTOCOLLNPGM("?A seen, but no address specified! [30-200]");
        return I2CPE_PARSE_ERR;
      };

      I2CPE_addr = parser.value_byte();

      if (!WITHIN(I2CPE_addr, 30, 200)) { // reserve the first 30 and last 55
        SERIAL_PROTOCOLLNPGM("?Address out of range. [30-200]");
        return I2CPE_PARSE_ERR;
      }

      I2CPE_idx = idx_from_addr(I2CPE_addr);

      if (!WITHIN(I2CPE_idx, 0, I2CPE_ENCODER_CNT - 1)) {
        SERIAL_PROTOCOLLNPGM("?No device with this address!");
        return I2CPE_PARSE_ERR;
      }
    } else if (parser.seenval('I')) {
      if (!parser.has_value()) {
        SERIAL_PROTOCOLLNPAIR("?I seen, but no index specified! [0-", I2CPE_ENCODER_CNT - 1);
        SERIAL_ECHOLNPGM("]");
        return I2CPE_PARSE_ERR;
      };

      I2CPE_idx = parser.value_byte();

      if (!WITHIN(I2CPE_idx, 0, I2CPE_ENCODER_CNT - 1)) {
        SERIAL_PROTOCOLLNPAIR("?Index out of range. [0-", I2CPE_ENCODER_CNT - 1);
        SERIAL_ECHOLNPGM("]");
        return I2CPE_PARSE_ERR;
      }

      I2CPE_addr = encoders[I2CPE_idx].get_address();
    } else {
      I2CPE_idx = -1;
    }

    I2CPE_anyaxis = parser.seen_axis();

    return I2CPE_PARSE_OK;
  };

  /**
   * M860:  Report the position(s) of position encoder module(s).
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1]
   *   O        Include homed zero-offset in returned position.
   *   U        Units in mm or raw step count.
   *
   *   If A or I not specified:
   *    X       Report on X axis encoder, if present.
   *    Y       Report on Y axis encoder, if present.
   *    Z       Report on Z axis encoder, if present.
   *    E       Report on E axis encoder, if present.
   *
   */
  void I2CPositionEncodersMgr::M860() {
    if (parse()) return;

    bool hasU = parser.seen('U'), hasO = parser.seen('O');

    if (I2CPE_idx < 0) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0))
          report_position((uint8_t)idx, hasU, hasO);
      }
    } else report_position((uint8_t)I2CPE_idx, hasU, hasO);
  }

  /**
   * M861:  Report the status of position encoder modules.
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1]
   *
   *   If A or I not specified:
   *    X       Report on X axis encoder, if present.
   *    Y       Report on Y axis encoder, if present.
   *    Z       Report on Z axis encoder, if present.
   *    E       Report on E axis encoder, if present.
   *
   */
  void I2CPositionEncodersMgr::M861() {
    if (parse()) return;

    if (I2CPE_idx < 0) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0))
          report_status((uint8_t)idx);
      }
    } else report_status((uint8_t)I2CPE_idx);
  }

  /**
   * M862:  Perform an axis continuity test for position encoder
   *        modules.
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1]
   *
   *   If A or I not specified:
   *    X       Report on X axis encoder, if present.
   *    Y       Report on Y axis encoder, if present.
   *    Z       Report on Z axis encoder, if present.
   *    E       Report on E axis encoder, if present.
   *
   */
  void I2CPositionEncodersMgr::M862() {
    if (parse()) return;

    if (I2CPE_idx < 0) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0))
          test_axis((uint8_t)idx);
      }
    } else test_axis((uint8_t)I2CPE_idx);
  }

  /**
   * M863:  Perform steps-per-mm calibration for
   *        position encoder modules.
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1]
   *   P        Number of rePeats/iterations.
   *
   *   If A or I not specified:
   *    X       Report on X axis encoder, if present.
   *    Y       Report on Y axis encoder, if present.
   *    Z       Report on Z axis encoder, if present.
   *    E       Report on E axis encoder, if present.
   *
   */
  void I2CPositionEncodersMgr::M863() {
    if (parse()) return;

    int iterations = parser.seenval('P') ? constrain(parser.value_byte(), 1, 10) : 1;

    if (I2CPE_idx < 0) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0))
          calibrate_steps_mm((uint8_t)idx, iterations);
      }
    } else calibrate_steps_mm((uint8_t)I2CPE_idx, iterations);
  }

  /**
   * M864:  Change position encoder module I2C address.
   *
   *   A<addr>  Module current/old I2C address.  If not present,
   *            assumes default address (030).  [30, 200].
   *   N<addr>  Module new I2C address. [30, 200].
   *
   *   If N not specified:
   *    X       Use I2CPE_PRESET_ADDR_X (030).
   *    Y       Use I2CPE_PRESET_ADDR_Y (031).
   *    Z       Use I2CPE_PRESET_ADDR_Z (032).
   *    E       Use I2CPE_PRESET_ADDR_E (033).
   */
  void I2CPositionEncodersMgr::M864() {
    uint8_t newAddress;

    if (parse()) return;

    if (!I2CPE_addr) I2CPE_addr = I2CPE_PRESET_ADDR_X;

    if (parser.seen('N')) {
      if (!parser.has_value()) {
        SERIAL_PROTOCOLLNPGM("?N seen, but no address specified! [30-200]");
        return;
      };

      newAddress = parser.value_byte();

      if (!WITHIN(newAddress, 30, 200)) {
        SERIAL_PROTOCOLLNPGM("?New address out of range. [30-200]");
        return;
      }
    } else if (!I2CPE_anyaxis) {
      SERIAL_PROTOCOLLNPGM("?You must specify N or [XYZE].");
      return;
    } else {
      if (parser.seen('X')) newAddress = I2CPE_PRESET_ADDR_X;
      else if (parser.seen('Y')) newAddress = I2CPE_PRESET_ADDR_Y;
      else if (parser.seen('Z')) newAddress = I2CPE_PRESET_ADDR_Z;
      else if (parser.seen('E')) newAddress = I2CPE_PRESET_ADDR_E;
      else return;
    }

    SERIAL_ECHOPAIR("Changing module at address ", I2CPE_addr);
    SERIAL_ECHOLNPAIR(" to address ", newAddress);

    change_module_address(I2CPE_addr, newAddress);
  }

  /**
   * M865:  Check position encoder module firmware version.
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1].
   *
   *   If A or I not specified:
   *    X       Check X axis encoder, if present.
   *    Y       Check Y axis encoder, if present.
   *    Z       Check Z axis encoder, if present.
   *    E       Check E axis encoder, if present.
   */
  void I2CPositionEncodersMgr::M865() {
    if (parse()) return;

    if (!I2CPE_addr) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0))
          report_module_firmware(encoders[idx].get_address());
      }
    } else report_module_firmware(I2CPE_addr);
  }

  /**
   * M866:  Report or reset position encoder module error
   *        count.
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1].
   *   R        Reset error counter.
   *
   *   If A or I not specified:
   *    X       Act on X axis encoder, if present.
   *    Y       Act on Y axis encoder, if present.
   *    Z       Act on Z axis encoder, if present.
   *    E       Act on E axis encoder, if present.
   */
  void I2CPositionEncodersMgr::M866() {
    if (parse()) return;

    bool hasR = parser.seen('R');

    if (I2CPE_idx < 0) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0)) {
          if (hasR) reset_error_count((uint8_t)idx, AxisEnum(i));
          else report_error_count((uint8_t)idx, AxisEnum(i));
        }
      }
    } else {
      if (hasR) reset_error_count((uint8_t)I2CPE_idx, encoders[I2CPE_idx].get_axis());
      else report_error_count((uint8_t)I2CPE_idx, encoders[I2CPE_idx].get_axis());
    }
  }

  /**
   * M867:  Enable/disable or toggle error correction for position encoder modules.
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1].
   *   S<1|0>   Enable/disable error correction. 1 enables, 0 disables.  If not
   *            supplied, toggle.
   *
   *   If A or I not specified:
   *    X       Act on X axis encoder, if present.
   *    Y       Act on Y axis encoder, if present.
   *    Z       Act on Z axis encoder, if present.
   *    E       Act on E axis encoder, if present.
   */
  void I2CPositionEncodersMgr::M867() {
    if (parse()) return;

    int8_t onoff = parser.seenval('S') ? parser.value_int() : -1;

    if (I2CPE_idx < 0) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0)) {
          if (onoff == -1) enable_ec((uint8_t)idx, !encoders[idx].get_ec_enabled(), AxisEnum(i));
          else enable_ec((uint8_t)idx, (bool)onoff, AxisEnum(i));
        }
      }
    } else {
      if (onoff == -1) enable_ec((uint8_t)I2CPE_idx, !encoders[I2CPE_idx].get_ec_enabled(), encoders[I2CPE_idx].get_axis());
      else enable_ec((uint8_t)I2CPE_idx, (bool)onoff, encoders[I2CPE_idx].get_axis());
    }
  }

  /**
   * M868:  Report or set position encoder module error correction
   *        threshold.
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1].
   *   T        New error correction threshold.
   *
   *   If A not specified:
   *    X       Act on X axis encoder, if present.
   *    Y       Act on Y axis encoder, if present.
   *    Z       Act on Z axis encoder, if present.
   *    E       Act on E axis encoder, if present.
   */
  void I2CPositionEncodersMgr::M868() {
    if (parse()) return;

    float newThreshold = parser.seenval('T') ? parser.value_float() : -9999;

    if (I2CPE_idx < 0) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0)) {
          if (newThreshold != -9999) set_ec_threshold((uint8_t)idx, newThreshold, encoders[idx].get_axis());
          else get_ec_threshold((uint8_t)idx, encoders[idx].get_axis());
        }
      }
    } else {
      if (newThreshold != -9999) set_ec_threshold((uint8_t)I2CPE_idx, newThreshold, encoders[I2CPE_idx].get_axis());
      else get_ec_threshold((uint8_t)I2CPE_idx, encoders[I2CPE_idx].get_axis());
    }
  }

  /**
   * M869:  Report position encoder module error.
   *
   *   A<addr>  Module I2C address.  [30, 200].
   *   I<index> Module index.  [0, I2CPE_ENCODER_CNT - 1].
   *
   *   If A not specified:
   *    X       Act on X axis encoder, if present.
   *    Y       Act on Y axis encoder, if present.
   *    Z       Act on Z axis encoder, if present.
   *    E       Act on E axis encoder, if present.
   */
  void I2CPositionEncodersMgr::M869() {
    if (parse()) return;

    if (I2CPE_idx < 0) {
      int8_t idx;
      LOOP_XYZE(i) {
        if ((!I2CPE_anyaxis || parser.seen(axis_codes[i])) && ((idx = idx_from_axis(AxisEnum(i))) >= 0))
          report_error((uint8_t)idx);
      }
    } else report_error((uint8_t)I2CPE_idx);
  }

#endif
