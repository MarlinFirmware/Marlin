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
 * feature/spindle_laser.cpp
 */

#include "../inc/MarlinConfig.h"

#if HAS_CUTTER

#include "spindle_laser.h"

#if ENABLED(SPINDLE_SERVO)
  #include "../module/servo.h"
#endif

#if ENABLED(I2C_AMMETER)
  #include "../feature/ammeter.h"
#endif

SpindleLaser cutter;
uint8_t SpindleLaser::ocr_power = 0;                                  // Startup disable
uint8_t SpindleLaser::dir = SPINDLE_DIR_CW;                           // Startup clockwise
SpindleLaserEvent SpindleLaser::state = SpindleLaserEvent::OFF;       // Startup OFF
#if ENABLED(LASER_FEATURE)
  cutter_test_pulse_t SpindleLaser::testPulse = 50;                   // Test fire Pulse time ms value.
#endif
bool SpindleLaser::isReady;                                           // Ready to apply power setting from the UI to OCR
cutter_power_t SpindleLaser::menuPower,                               // Power set via LCD menu in PWM, PERCENT, or RPM
               SpindleLaser::unitPower;                               // LCD status power in PWM, PERCENT, or RPM

#if ENABLED(MARLIN_DEV_MODE)
  cutter_frequency_t SpindleLaser::frequency;                         // PWM frequency setting; range: 2K - 50K
#endif
#define SPINDLE_LASER_PWM_OFF TERN(SPINDLE_LASER_PWM_INVERT, 255, 0)

/**
 * Init the cutter to a safe OFF state
 */
void SpindleLaser::init() {
  #if ENABLED(SPINDLE_SERVO)
    MOVE_SERVO(SPINDLE_SERVO_NR, SPINDLE_SERVO_MIN);
  #else
    OUT_WRITE(SPINDLE_LASER_ENA_PIN, !SPINDLE_LASER_ACTIVE_STATE);    // Init spindle to off
  #endif
  #if ENABLED(SPINDLE_CHANGE_DIR)
    //dir_set()
    OUT_WRITE(SPINDLE_DIR_PIN, SPINDLE_INVERT_DIR ? 255 : 0);         // Init rotation to clockwise (M3)
  #endif
  #if ENABLED(SPINDLE_LASER_PWM)
    SET_PWM(SPINDLE_LASER_PWM_PIN);
    analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_PWM_OFF); // Set to lowest speed
  #endif
  #if ENABLED(HAL_CAN_SET_PWM_FREQ) && defined(SPINDLE_LASER_FREQUENCY)
    set_pwm_frequency(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_FREQUENCY);
    TERN_(MARLIN_DEV_MODE, frequency = SPINDLE_LASER_FREQUENCY);
  #endif
  #if ENABLED(AIR_EVACUATION)
    OUT_WRITE(AIR_EVACUATION_PIN, !AIR_EVACUATION_ACTIVE);            // Init Vacuum/Blower OFF
  #endif
  #if ENABLED(AIR_ASSIST)
    OUT_WRITE(AIR_ASSIST_PIN, !AIR_ASSIST_ACTIVE);                    // Init Air Assist OFF
  #endif
  #if ENABLED(I2C_AMMETER)
    ammeter.init();                                                   // Init I2C Ammeter
  #endif
}

#if ENABLED(SPINDLE_LASER_PWM)
  /**
   * Set the cutter PWM directly to the given ocr value
   *
   * @param ocr Power value
   */
  void SpindleLaser::ocr_set(const uint8_t ocr) {
    #if NEEDS_HARDWARE_PWM && SPINDLE_LASER_FREQUENCY
      set_pwm_frequency(pin_t(SPINDLE_LASER_PWM_PIN), TERN(MARLIN_DEV_MODE, frequency, SPINDLE_LASER_FREQUENCY));
      set_pwm_duty(pin_t(SPINDLE_LASER_PWM_PIN), ocr ^ SPINDLE_LASER_PWM_OFF);
    #else
      analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), ocr ^ SPINDLE_LASER_PWM_OFF);
    #endif
  }

#endif // SPINDLE_LASER_PWM

/**
 * Set state for pin spindle/laser
 * @param enable true = on; false = off
 */
void SpindleLaser::ena_pin_set(const bool enable) {
  WRITE(SPINDLE_LASER_ENA_PIN, enable ? SPINDLE_LASER_ACTIVE_STATE : !SPINDLE_LASER_ACTIVE_STATE);
}

#if ENABLED(SPINDLE_CHANGE_DIR)
  /**
   * Set direction pin for spindle
   */
  void SpindleLaser::dir_pin_set() {
    // Forward (M3) HIGH when not inverted
    const uint8_t dir_state = (dir == TERN(SPINDLE_INVERT_DIR, SPINDLE_DIR_CCW, SPINDLE_DIR_CW)) ? HIGH : LOW;
    WRITE(SPINDLE_DIR_PIN, dir_state);
  }
#endif

/**
 * Detection event
 *
 * @param opwr        New power value
 * @param odir        New direction spindle
 * @param oena_pin_on New ena_pin state
 */
SpindleLaserEvent SpindleLaser::get_event(const uint8_t opwr, const uint8_t odir, const bool oena_pin_on) {
  if ((opwr == 0) && (!oena_pin_on))
    return state == SpindleLaserEvent::OFF ? SpindleLaserEvent::OFF : SpindleLaserEvent::TO_OFF;
  if (state == SpindleLaserEvent::OFF)
    return odir == SPINDLE_DIR_CW ? SpindleLaserEvent::TO_ON_CW : SpindleLaserEvent::TO_ON_CCW;
  if (odir != dir)
    return odir == SPINDLE_DIR_CW ? SpindleLaserEvent::TO_ON_ON_CW : SpindleLaserEvent::TO_ON_ON_CCW;
  // We don't change state
  // return dir == SPINDLE_DIR_CW ? SpindleLaserEvent::ON_CW : SpindleLaserEvent::ON_CCW;
  return state;
}

/**
 * Change hardware state
 * set isReady is hack - his state and ena_pin_on is equal
 */
void SpindleLaser::_change_hw(const bool ena_pin_on) {
  #if ENABLED(SPINDLE_LASER_PWM)
    ocr_set(ocr_power);
    isReady = ena_pin_on;
  #endif

  #if ENABLED(SPINDLE_SERVO)
    MOVE_SERVO(SPINDLE_SERVO_NR, ocr_power);
  #else
    ena_pin_set(ena_pin_on);
    isReady = ena_pin_on;
  #endif
}

/**
 * Set cutter ocr_power value for PWM, Servo, and on/off pin.
 *
 * @param opwr       Power value. Range 0 to MAX. When 0 disable spindle/laser.
 * @param odir       Direction spindle (default SPINDLE_DIR_CURRENT)
 * @param ena_pin_on Enable/disable ENA_PIN. Work only in separate mode PWM.
 *                   If separate mode is disable - ENA_PIN is managed through opwr value
 *                   if true - enable pin; false - disable pin (default true)
 */
void SpindleLaser::ocr_set_power(const uint8_t opwr, const uint8_t odir, const bool ena_pin_on) {
  SpindleLaserEvent event;
  uint8_t dir_value = TERN(
    SPINDLE_CHANGE_DIR, (odir == SPINDLE_DIR_CURRENT) ? dir : odir, SPINDLE_DIR_CW
  );

  #if ENABLED(SPINDLE_CHANGE_DIR_STOP)
    uint8_t ocr_power_tmp;  // For save ocr_power (uint16_t)
  #endif

  #if BOTH(SPINDLE_LASER_PWM, SPINDLE_LASER_PWM_SEPARATE_PIN)
    bool ena_pin_on_value = ena_pin_on;
  #else
    bool ena_pin_on_value = opwr > 0 ? true : false;
  #endif

  event = get_event(opwr, dir_value, ena_pin_on_value);

  switch (event) {
    case SpindleLaserEvent::ON_CW:
    case SpindleLaserEvent::ON_CCW:
      if (opwr == ocr_power) break;
      ocr_power = opwr;
      _change_hw(ena_pin_on_value);
      break;

    case SpindleLaserEvent::TO_ON_CCW:
    case SpindleLaserEvent::TO_ON_CW:
      ocr_power = opwr;
      dir = dir_value;
      dir_pin_set();
      _change_hw(ena_pin_on_value);
      power_delay(true);
      break;

    case SpindleLaserEvent::OFF:
      dir = dir_value;
      break;

    case SpindleLaserEvent::TO_OFF:
      ocr_power = opwr;
      dir = dir_value;
      dir_pin_set();
      _change_hw(false);
      power_delay(false);
      break;

    case SpindleLaserEvent::TO_ON_ON_CW:
    case SpindleLaserEvent::TO_ON_ON_CCW:
      ocr_power = opwr;
      dir = dir_value;

      #if ENABLED(SPINDLE_CHANGE_DIR_STOP)
        //Stop
        ocr_power_tmp = ocr_power;
        ocr_power = 0;
        _change_hw(false);
        power_delay(false);
        //Start
        ocr_power = ocr_power_tmp;
        dir_pin_set();
        _change_hw(ena_pin_on_value);
        power_delay(true);
      #else
        dir_pin_set();
      #endif
      break;
  }
  state = static_cast<SpindleLaserEvent>(event & 0b01111);
}

/**
 * Return value ocr_power
 */
uint8_t SpindleLaser::get_ocr_power(){ return ocr_power; }

#if ENABLED(SPINDLE_CHANGE_DIR)
  /**
   * Set the spindle direction and apply immediately
   * ! deprecate and need delete
   * @param reverse If True is SPINDLE_DIR_CCW, false is SPINDLE_DIR_CW
   */
  void SpindleLaser::set_reverse(const bool reverse) {
    ocr_set_power(ocr_power, reverse ? SPINDLE_DIR_CCW : SPINDLE_DIR_CW);
  }
#endif

#if ENABLED(AIR_EVACUATION)
  // Enable / disable Cutter Vacuum or Laser Blower motor
  void SpindleLaser::air_evac_enable()  { WRITE(AIR_EVACUATION_PIN,  AIR_EVACUATION_ACTIVE); } // Turn ON
  void SpindleLaser::air_evac_disable() { WRITE(AIR_EVACUATION_PIN, !AIR_EVACUATION_ACTIVE); } // Turn OFF
  void SpindleLaser::air_evac_toggle()  { TOGGLE(AIR_EVACUATION_PIN); } // Toggle state
#endif

#if ENABLED(AIR_ASSIST)
  // Enable / disable air assist
  void SpindleLaser::air_assist_enable()  { WRITE(AIR_ASSIST_PIN,  AIR_ASSIST_PIN); } // Turn ON
  void SpindleLaser::air_assist_disable() { WRITE(AIR_ASSIST_PIN, !AIR_ASSIST_PIN); } // Turn OFF
  void SpindleLaser::air_assist_toggle()  { TOGGLE(AIR_ASSIST_PIN); } // Toggle state
#endif

#endif // HAS_CUTTER
