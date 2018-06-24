/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Based on servo.cpp - Interrupt driven Servo library for Arduino using 16 bit
 * timers- Version 2  Copyright (c) 2009 Michael Margolis.  All right reserved.
 */

/**
 * A servo is activated by creating an instance of the Servo class passing the desired pin to the attach() method.
 * The servos are pulsed in the background using the value most recently written using the write() method
 *
 * Note that analogWrite of PWM on pins associated with the timer are disabled when the first servo is attached.
 * Timers are seized as needed in groups of 12 servos - 24 servos use two timers, 48 servos will use four.
 *
 * The methods are:
 *
 * Servo - Class for manipulating servo motors connected to Arduino pins.
 *
 * attach(pin)           - Attach a servo motor to an i/o pin.
 * attach(pin, min, max) - Attach to a pin, setting min and max values in microseconds
 *                         Default min is 544, max is 2400
 *
 * write()               - Set the servo angle in degrees. (Invalid angles —over MIN_PULSE_WIDTH— are treated as µs.)
 * writeMicroseconds()   - Set the servo pulse width in microseconds.
 * move(pin, angle)      - Sequence of attach(pin), write(angle), safe_delay(servo_delay[servoIndex]).
 *                         With DEACTIVATE_SERVOS_AFTER_MOVE it detaches after servo_delay[servoIndex].
 * read()                - Get the last-written servo pulse width as an angle between 0 and 180.
 * readMicroseconds()    - Get the last-written servo pulse width in microseconds.
 * attached()            - Return true if a servo is attached.
 * detach()              - Stop an attached servo from pulsing its i/o pin.
 *
 */

/**
 * The only time that this library wants physical movement is when a WRITE
 * command is issued.  Before that all the attach & detach activity is solely
 * within the data base.
 *
 * The PWM output is inactive until the first WRITE.  After that it stays active
 * unless DEACTIVATE_SERVOS_AFTER_MOVE is enabled and a MOVE command was issued.
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

  #include "LPC1768_PWM.h"
  #include "LPC1768_Servo.h"
  #include "servo_private.h"

  ServoInfo_t servo_info[MAX_SERVOS];                  // static array of servo info structures
  uint8_t ServoCount = 0;                              // the total number of attached servos

  #define US_TO_PULSE_WIDTH(p) p
  #define PULSE_WIDTH_TO_US(p) p
  #define TRIM_DURATION 0
  #define SERVO_MIN() MIN_PULSE_WIDTH  // minimum value in uS for this servo
  #define SERVO_MAX() MAX_PULSE_WIDTH  // maximum value in uS for this servo

  Servo::Servo() {
    if (ServoCount < MAX_SERVOS) {
      this->servoIndex = ServoCount++;                    // assign a servo index to this instance
      servo_info[this->servoIndex].pulse_width = US_TO_PULSE_WIDTH(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
    }
    else
    this->servoIndex = INVALID_SERVO;  // too many servos
  }

  int8_t Servo::attach(const int pin) {
    return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  }

  int8_t Servo::attach(const int pin, const int min, const int max) {

    if (this->servoIndex >= MAX_SERVOS) return -1;

    if (pin > 0) servo_info[this->servoIndex].Pin.nbr = pin;  // only assign a pin value if the pin info is
                                                              // greater than zero. This way the init routine can
                                                              // assign the pin and the MOVE command only needs the value.

    this->min = MIN_PULSE_WIDTH; //resolution of min/max is 1 uS
    this->max = MAX_PULSE_WIDTH;

    servo_info[this->servoIndex].Pin.isActive = true;

    return this->servoIndex;
  }

  void Servo::detach() {
    servo_info[this->servoIndex].Pin.isActive = false;
  }

  void Servo::write(int value) {
    if (value < MIN_PULSE_WIDTH) { // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
      value = map(constrain(value, 0, 180), 0, 180, SERVO_MIN(), SERVO_MAX());
        // odd - this sets zero degrees to 544 and 180 degrees to 2400 microseconds but the literature says
        //          zero degrees should be 500 microseconds and 180 should be 2500
    }
    this->writeMicroseconds(value);
  }

  void Servo::writeMicroseconds(int value) {
    // calculate and store the values for the given channel
    byte channel = this->servoIndex;
    if (channel < MAX_SERVOS) {  // ensure channel is valid
      // ensure pulse width is valid
      value = constrain(value, SERVO_MIN(), SERVO_MAX()) - (TRIM_DURATION);
      value = US_TO_PULSE_WIDTH(value);  // convert to pulse_width after compensating for interrupt overhead - 12 Aug 2009

      servo_info[channel].pulse_width = value;
      LPC1768_PWM_attach_pin(servo_info[this->servoIndex].Pin.nbr, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, this->servoIndex);
      LPC1768_PWM_write(servo_info[this->servoIndex].Pin.nbr, value);

    }
  }

  // return the value as degrees
  int Servo::read() { return map(this->readMicroseconds() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180); }

  int Servo::readMicroseconds() {
    return (this->servoIndex == INVALID_SERVO) ? 0 : PULSE_WIDTH_TO_US(servo_info[this->servoIndex].pulse_width) + TRIM_DURATION;
  }

  bool Servo::attached() { return servo_info[this->servoIndex].Pin.isActive; }

  void Servo::move(const int value) {
    constexpr uint16_t servo_delay[] = SERVO_DELAY;
    static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");
    if (this->attach(0) >= 0) {    // notice the pin number is zero here
      this->write(value);
      safe_delay(servo_delay[this->servoIndex]);
      #if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
        this->detach();
        LPC1768_PWM_detach_pin(servo_info[this->servoIndex].Pin.nbr);  // shut down the PWM signal
        LPC1768_PWM_attach_pin(servo_info[this->servoIndex].Pin.nbr, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, this->servoIndex);  // make sure no one else steals the slot
      #endif
    }
  }

#endif // HAS_SERVOS
#endif // TARGET_LPC1768
