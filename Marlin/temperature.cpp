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
 * temperature.cpp - temperature control
 */

#include "Marlin.h"
#include "ultralcd.h"
#include "temperature.h"
#include "thermistortables.h"
#include "language.h"
#if ENABLED(BABYSTEPPING)
  #include "stepper.h"
#endif

#if ENABLED(USE_WATCHDOG)
  #include "watchdog.h"
#endif

#ifdef K1 // Defined in Configuration.h in the PID settings
  #define K2 (1.0-K1)
#endif

#if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
  static void* heater_ttbl_map[2] = {(void*)HEATER_0_TEMPTABLE, (void*)HEATER_1_TEMPTABLE };
  static uint8_t heater_ttbllen_map[2] = { HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN };
#else
  static void* heater_ttbl_map[HOTENDS] = ARRAY_BY_HOTENDS((void*)HEATER_0_TEMPTABLE, (void*)HEATER_1_TEMPTABLE, (void*)HEATER_2_TEMPTABLE, (void*)HEATER_3_TEMPTABLE);
  static uint8_t heater_ttbllen_map[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN, HEATER_2_TEMPTABLE_LEN, HEATER_3_TEMPTABLE_LEN);
#endif

Temperature thermalManager;

// public:

float Temperature::current_temperature[HOTENDS] = { 0.0 },
      Temperature::current_temperature_bed = 0.0;
int   Temperature::current_temperature_raw[HOTENDS] = { 0 },
      Temperature::target_temperature[HOTENDS] = { 0 },
      Temperature::current_temperature_bed_raw = 0,
      Temperature::target_temperature_bed = 0;

#if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
  float Temperature::redundant_temperature = 0.0;
#endif

uint8_t Temperature::soft_pwm_bed;

#if ENABLED(FAN_SOFT_PWM)
  uint8_t Temperature::fanSpeedSoftPwm[FAN_COUNT];
#endif

#if ENABLED(PIDTEMP)
  #if ENABLED(PID_PARAMS_PER_HOTEND) && HOTENDS > 1
    float Temperature::Kp[HOTENDS] = ARRAY_BY_HOTENDS1(DEFAULT_Kp),
          Temperature::Ki[HOTENDS] = ARRAY_BY_HOTENDS1((DEFAULT_Ki) * (PID_dT)),
          Temperature::Kd[HOTENDS] = ARRAY_BY_HOTENDS1((DEFAULT_Kd) / (PID_dT));
    #if ENABLED(PID_EXTRUSION_SCALING)
      float Temperature::Kc[HOTENDS] = ARRAY_BY_HOTENDS1(DEFAULT_Kc);
    #endif
  #else
    float Temperature::Kp = DEFAULT_Kp,
          Temperature::Ki = (DEFAULT_Ki) * (PID_dT),
          Temperature::Kd = (DEFAULT_Kd) / (PID_dT);
    #if ENABLED(PID_EXTRUSION_SCALING)
      float Temperature::Kc = DEFAULT_Kc;
    #endif
  #endif
#endif

#if ENABLED(PIDTEMPBED)
  float Temperature::bedKp = DEFAULT_bedKp,
        Temperature::bedKi = ((DEFAULT_bedKi) * PID_dT),
        Temperature::bedKd = ((DEFAULT_bedKd) / PID_dT);
#endif

#if ENABLED(BABYSTEPPING)
  volatile int Temperature::babystepsTodo[XYZ] = { 0 };
#endif

#if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0
  int Temperature::watch_target_temp[HOTENDS] = { 0 };
  millis_t Temperature::watch_heater_next_ms[HOTENDS] = { 0 };
#endif

#if ENABLED(THERMAL_PROTECTION_BED) && WATCH_BED_TEMP_PERIOD > 0
  int Temperature::watch_target_bed_temp = 0;
  millis_t Temperature::watch_bed_next_ms = 0;
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  bool Temperature::allow_cold_extrude = false;
  float Temperature::extrude_min_temp = EXTRUDE_MINTEMP;
#endif

// private:

#if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
  int Temperature::redundant_temperature_raw = 0;
  float Temperature::redundant_temperature = 0.0;
#endif

volatile bool Temperature::temp_meas_ready = false;

#if ENABLED(PIDTEMP)
  float Temperature::temp_iState[HOTENDS] = { 0 },
        Temperature::temp_dState[HOTENDS] = { 0 },
        Temperature::pTerm[HOTENDS],
        Temperature::iTerm[HOTENDS],
        Temperature::dTerm[HOTENDS];

  #if ENABLED(PID_EXTRUSION_SCALING)
    float Temperature::cTerm[HOTENDS];
    long Temperature::last_e_position;
    long Temperature::lpq[LPQ_MAX_LEN];
    int Temperature::lpq_ptr = 0;
  #endif

  float Temperature::pid_error[HOTENDS];
  bool Temperature::pid_reset[HOTENDS];
#endif

#if ENABLED(PIDTEMPBED)
  float Temperature::temp_iState_bed = { 0 },
        Temperature::temp_dState_bed = { 0 },
        Temperature::pTerm_bed,
        Temperature::iTerm_bed,
        Temperature::dTerm_bed,
        Temperature::pid_error_bed;
#else
  millis_t Temperature::next_bed_check_ms;
#endif

unsigned long Temperature::raw_temp_value[4] = { 0 };
unsigned long Temperature::raw_temp_bed_value = 0;

// Init min and max temp with extreme values to prevent false errors during startup
int Temperature::minttemp_raw[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_RAW_LO_TEMP , HEATER_1_RAW_LO_TEMP , HEATER_2_RAW_LO_TEMP, HEATER_3_RAW_LO_TEMP),
    Temperature::maxttemp_raw[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_RAW_HI_TEMP , HEATER_1_RAW_HI_TEMP , HEATER_2_RAW_HI_TEMP, HEATER_3_RAW_HI_TEMP),
    Temperature::minttemp[HOTENDS] = { 0 },
    Temperature::maxttemp[HOTENDS] = ARRAY_BY_HOTENDS1(16383);

#ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
  int Temperature::consecutive_low_temperature_error[HOTENDS] = { 0 };
#endif

#ifdef MILLISECONDS_PREHEAT_TIME
  unsigned long Temperature::preheat_end_time[HOTENDS] = { 0 };
#endif

#ifdef BED_MINTEMP
  int Temperature::bed_minttemp_raw = HEATER_BED_RAW_LO_TEMP;
#endif

#ifdef BED_MAXTEMP
  int Temperature::bed_maxttemp_raw = HEATER_BED_RAW_HI_TEMP;
#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  int Temperature::meas_shift_index;  // Index of a delayed sample in buffer
#endif

#if HAS_AUTO_FAN
  millis_t Temperature::next_auto_fan_check_ms = 0;
#endif

uint8_t Temperature::soft_pwm[HOTENDS];

#if ENABLED(FAN_SOFT_PWM)
  uint8_t Temperature::soft_pwm_fan[FAN_COUNT];
#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  int Temperature::current_raw_filwidth = 0;  //Holds measured filament diameter - one extruder only
#endif

#if HAS_PID_HEATING

  void Temperature::PID_autotune(float temp, int hotend, int ncycles, bool set_result/*=false*/) {
    float input = 0.0;
    int cycles = 0;
    bool heating = true;

    millis_t temp_ms = millis(), t1 = temp_ms, t2 = temp_ms;
    long t_high = 0, t_low = 0;

    long bias, d;
    float Ku, Tu;
    float workKp = 0, workKi = 0, workKd = 0;
    float max = 0, min = 10000;

    #if HAS_AUTO_FAN
      next_auto_fan_check_ms = temp_ms + 2500UL;
    #endif

    if (hotend >=
        #if ENABLED(PIDTEMP)
          HOTENDS
        #else
          0
        #endif
      || hotend <
        #if ENABLED(PIDTEMPBED)
          -1
        #else
          0
        #endif
    ) {
      SERIAL_ECHOLN(MSG_PID_BAD_EXTRUDER_NUM);
      return;
    }

    SERIAL_ECHOLN(MSG_PID_AUTOTUNE_START);

    disable_all_heaters(); // switch off all heaters.

    #if HAS_PID_FOR_BOTH
      if (hotend < 0)
        soft_pwm_bed = bias = d = (MAX_BED_POWER) / 2;
      else
        soft_pwm[hotend] = bias = d = (PID_MAX) / 2;
    #elif ENABLED(PIDTEMP)
      soft_pwm[hotend] = bias = d = (PID_MAX) / 2;
    #else
      soft_pwm_bed = bias = d = (MAX_BED_POWER) / 2;
    #endif

    wait_for_heatup = true;

    // PID Tuning loop
    while (wait_for_heatup) {

      millis_t ms = millis();

      if (temp_meas_ready) { // temp sample ready
        updateTemperaturesFromRawValues();

        input =
          #if HAS_PID_FOR_BOTH
            hotend < 0 ? current_temperature_bed : current_temperature[hotend]
          #elif ENABLED(PIDTEMP)
            current_temperature[hotend]
          #else
            current_temperature_bed
          #endif
        ;

        NOLESS(max, input);
        NOMORE(min, input);

        #if HAS_AUTO_FAN
          if (ELAPSED(ms, next_auto_fan_check_ms)) {
            checkExtruderAutoFans();
            next_auto_fan_check_ms = ms + 2500UL;
          }
        #endif

        if (heating && input > temp) {
          if (ELAPSED(ms, t2 + 5000UL)) {
            heating = false;
            #if HAS_PID_FOR_BOTH
              if (hotend < 0)
                soft_pwm_bed = (bias - d) >> 1;
              else
                soft_pwm[hotend] = (bias - d) >> 1;
            #elif ENABLED(PIDTEMP)
              soft_pwm[hotend] = (bias - d) >> 1;
            #elif ENABLED(PIDTEMPBED)
              soft_pwm_bed = (bias - d) >> 1;
            #endif
            t1 = ms;
            t_high = t1 - t2;
            max = temp;
          }
        }

        if (!heating && input < temp) {
          if (ELAPSED(ms, t1 + 5000UL)) {
            heating = true;
            t2 = ms;
            t_low = t2 - t1;
            if (cycles > 0) {
              long max_pow =
                #if HAS_PID_FOR_BOTH
                  hotend < 0 ? MAX_BED_POWER : PID_MAX
                #elif ENABLED(PIDTEMP)
                  PID_MAX
                #else
                  MAX_BED_POWER
                #endif
              ;
              bias += (d * (t_high - t_low)) / (t_low + t_high);
              bias = constrain(bias, 20, max_pow - 20);
              d = (bias > max_pow / 2) ? max_pow - 1 - bias : bias;

              SERIAL_PROTOCOLPAIR(MSG_BIAS, bias);
              SERIAL_PROTOCOLPAIR(MSG_D, d);
              SERIAL_PROTOCOLPAIR(MSG_T_MIN, min);
              SERIAL_PROTOCOLPAIR(MSG_T_MAX, max);
              if (cycles > 2) {
                Ku = (4.0 * d) / (M_PI * (max - min) * 0.5);
                Tu = ((float)(t_low + t_high) * 0.001);
                SERIAL_PROTOCOLPAIR(MSG_KU, Ku);
                SERIAL_PROTOCOLPAIR(MSG_TU, Tu);
                workKp = 0.6 * Ku;
                workKi = 2 * workKp / Tu;
                workKd = workKp * Tu * 0.125;
                SERIAL_PROTOCOLLNPGM("\n" MSG_CLASSIC_PID);
                SERIAL_PROTOCOLPAIR(MSG_KP, workKp);
                SERIAL_PROTOCOLPAIR(MSG_KI, workKi);
                SERIAL_PROTOCOLLNPAIR(MSG_KD, workKd);
                /**
                workKp = 0.33*Ku;
                workKi = workKp/Tu;
                workKd = workKp*Tu/3;
                SERIAL_PROTOCOLLNPGM(" Some overshoot");
                SERIAL_PROTOCOLPAIR(" Kp: ", workKp);
                SERIAL_PROTOCOLPAIR(" Ki: ", workKi);
                SERIAL_PROTOCOLPAIR(" Kd: ", workKd);
                workKp = 0.2*Ku;
                workKi = 2*workKp/Tu;
                workKd = workKp*Tu/3;
                SERIAL_PROTOCOLLNPGM(" No overshoot");
                SERIAL_PROTOCOLPAIR(" Kp: ", workKp);
                SERIAL_PROTOCOLPAIR(" Ki: ", workKi);
                SERIAL_PROTOCOLPAIR(" Kd: ", workKd);
                */
              }
            }
            #if HAS_PID_FOR_BOTH
              if (hotend < 0)
                soft_pwm_bed = (bias + d) >> 1;
              else
                soft_pwm[hotend] = (bias + d) >> 1;
            #elif ENABLED(PIDTEMP)
              soft_pwm[hotend] = (bias + d) >> 1;
            #else
              soft_pwm_bed = (bias + d) >> 1;
            #endif
            cycles++;
            min = temp;
          }
        }
      }
      #define MAX_OVERSHOOT_PID_AUTOTUNE 20
      if (input > temp + MAX_OVERSHOOT_PID_AUTOTUNE) {
        SERIAL_PROTOCOLLNPGM(MSG_PID_TEMP_TOO_HIGH);
        return;
      }
      // Every 2 seconds...
      if (ELAPSED(ms, temp_ms + 2000UL)) {
        #if HAS_TEMP_HOTEND || HAS_TEMP_BED
          print_heaterstates();
          SERIAL_EOL;
        #endif

        temp_ms = ms;
      } // every 2 seconds
      // Over 2 minutes?
      if (((ms - t1) + (ms - t2)) > (10L * 60L * 1000L * 2L)) {
        SERIAL_PROTOCOLLNPGM(MSG_PID_TIMEOUT);
        return;
      }
      if (cycles > ncycles) {
        SERIAL_PROTOCOLLNPGM(MSG_PID_AUTOTUNE_FINISHED);

        #if HAS_PID_FOR_BOTH
          const char* estring = hotend < 0 ? "bed" : "";
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_", estring); SERIAL_PROTOCOLPAIR("Kp ", workKp); SERIAL_EOL;
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_", estring); SERIAL_PROTOCOLPAIR("Ki ", workKi); SERIAL_EOL;
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_", estring); SERIAL_PROTOCOLPAIR("Kd ", workKd); SERIAL_EOL;
        #elif ENABLED(PIDTEMP)
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_Kp ", workKp); SERIAL_EOL;
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_Ki ", workKi); SERIAL_EOL;
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_Kd ", workKd); SERIAL_EOL;
        #else
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_bedKp ", workKp); SERIAL_EOL;
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_bedKi ", workKi); SERIAL_EOL;
          SERIAL_PROTOCOLPAIR("#define  DEFAULT_bedKd ", workKd); SERIAL_EOL;
        #endif

        #define _SET_BED_PID() do { \
          bedKp = workKp; \
          bedKi = scalePID_i(workKi); \
          bedKd = scalePID_d(workKd); \
          updatePID(); } while(0)

        #define _SET_EXTRUDER_PID() do { \
          PID_PARAM(Kp, hotend) = workKp; \
          PID_PARAM(Ki, hotend) = scalePID_i(workKi); \
          PID_PARAM(Kd, hotend) = scalePID_d(workKd); \
          updatePID(); } while(0)

        // Use the result? (As with "M303 U1")
        if (set_result) {
          #if HAS_PID_FOR_BOTH
            if (hotend < 0)
              _SET_BED_PID();
            else
              _SET_EXTRUDER_PID();
          #elif ENABLED(PIDTEMP)
            _SET_EXTRUDER_PID();
          #else
            _SET_BED_PID();
          #endif
        }
        return;
      }
      lcd_update();
    }
    if (!wait_for_heatup) disable_all_heaters();
  }

#endif // HAS_PID_HEATING

/**
 * Class and Instance Methods
 */

Temperature::Temperature() { }

void Temperature::updatePID() {
  #if ENABLED(PIDTEMP)
    #if ENABLED(PID_EXTRUSION_SCALING)
      last_e_position = 0;
    #endif
  #endif
}

int Temperature::getHeaterPower(int heater) {
  return heater < 0 ? soft_pwm_bed : soft_pwm[heater];
}

#if HAS_AUTO_FAN

  void Temperature::checkExtruderAutoFans() {
    const int8_t fanPin[] = { EXTRUDER_0_AUTO_FAN_PIN, EXTRUDER_1_AUTO_FAN_PIN, EXTRUDER_2_AUTO_FAN_PIN, EXTRUDER_3_AUTO_FAN_PIN };
    const int fanBit[] = {
                    0,
      AUTO_1_IS_0 ? 0 :               1,
      AUTO_2_IS_0 ? 0 : AUTO_2_IS_1 ? 1 :               2,
      AUTO_3_IS_0 ? 0 : AUTO_3_IS_1 ? 1 : AUTO_3_IS_2 ? 2 : 3
    };
    uint8_t fanState = 0;
 
    HOTEND_LOOP() {
      if (current_temperature[e] > EXTRUDER_AUTO_FAN_TEMPERATURE)
        SBI(fanState, fanBit[e]);
    }
 
    uint8_t fanDone = 0;
    for (uint8_t f = 0; f < COUNT(fanPin); f++) {
      int8_t pin = fanPin[f];
      if (pin >= 0 && !TEST(fanDone, fanBit[f])) {
        uint8_t newFanSpeed = TEST(fanState, fanBit[f]) ? EXTRUDER_AUTO_FAN_SPEED : 0;
        // this idiom allows both digital and PWM fan outputs (see M42 handling).
        digitalWrite(pin, newFanSpeed);
        analogWrite(pin, newFanSpeed);
        SBI(fanDone, fanBit[f]);
      }
    }
  }

#endif // HAS_AUTO_FAN

//
// Temperature Error Handlers
//
void Temperature::_temp_error(int e, const char* serial_msg, const char* lcd_msg) {
  static bool killed = false;
  if (IsRunning()) {
    SERIAL_ERROR_START;
    serialprintPGM(serial_msg);
    SERIAL_ERRORPGM(MSG_STOPPED_HEATER);
    if (e >= 0) SERIAL_ERRORLN((int)e); else SERIAL_ERRORLNPGM(MSG_HEATER_BED);
  }
  #if DISABLED(BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE)
    if (!killed) {
      Running = false;
      killed = true;
      kill(lcd_msg);
    }
    else
      disable_all_heaters(); // paranoia
  #endif
}

void Temperature::max_temp_error(int8_t e) {
  #if HAS_TEMP_BED
    _temp_error(e, PSTR(MSG_T_MAXTEMP), e >= 0 ? PSTR(MSG_ERR_MAXTEMP) : PSTR(MSG_ERR_MAXTEMP_BED));
  #else
    _temp_error(HOTEND_INDEX, PSTR(MSG_T_MAXTEMP), PSTR(MSG_ERR_MAXTEMP));
    #if HOTENDS == 1
      UNUSED(e);
    #endif
  #endif
}
void Temperature::min_temp_error(int8_t e) {
  #if HAS_TEMP_BED
    _temp_error(e, PSTR(MSG_T_MINTEMP), e >= 0 ? PSTR(MSG_ERR_MINTEMP) : PSTR(MSG_ERR_MINTEMP_BED));
  #else
    _temp_error(HOTEND_INDEX, PSTR(MSG_T_MINTEMP), PSTR(MSG_ERR_MINTEMP));
    #if HOTENDS == 1
      UNUSED(e);
    #endif
  #endif
}

float Temperature::get_pid_output(int e) {
  #if HOTENDS == 1
    UNUSED(e);
    #define _HOTEND_TEST     true
  #else
    #define _HOTEND_TEST     e == active_extruder
  #endif
  float pid_output;
  #if ENABLED(PIDTEMP)
    #if DISABLED(PID_OPENLOOP)
      pid_error[HOTEND_INDEX] = target_temperature[HOTEND_INDEX] - current_temperature[HOTEND_INDEX];
      dTerm[HOTEND_INDEX] = K2 * PID_PARAM(Kd, HOTEND_INDEX) * (current_temperature[HOTEND_INDEX] - temp_dState[HOTEND_INDEX]) + K1 * dTerm[HOTEND_INDEX];
      temp_dState[HOTEND_INDEX] = current_temperature[HOTEND_INDEX];
      if (pid_error[HOTEND_INDEX] > PID_FUNCTIONAL_RANGE) {
        pid_output = BANG_MAX;
        pid_reset[HOTEND_INDEX] = true;
      }
      else if (pid_error[HOTEND_INDEX] < -(PID_FUNCTIONAL_RANGE) || target_temperature[HOTEND_INDEX] == 0) {
        pid_output = 0;
        pid_reset[HOTEND_INDEX] = true;
      }
      else {
        if (pid_reset[HOTEND_INDEX]) {
          temp_iState[HOTEND_INDEX] = 0.0;
          pid_reset[HOTEND_INDEX] = false;
        }
        pTerm[HOTEND_INDEX] = PID_PARAM(Kp, HOTEND_INDEX) * pid_error[HOTEND_INDEX];
        temp_iState[HOTEND_INDEX] += pid_error[HOTEND_INDEX];
        iTerm[HOTEND_INDEX] = PID_PARAM(Ki, HOTEND_INDEX) * temp_iState[HOTEND_INDEX];

        pid_output = pTerm[HOTEND_INDEX] + iTerm[HOTEND_INDEX] - dTerm[HOTEND_INDEX];

        #if ENABLED(PID_EXTRUSION_SCALING)
          cTerm[HOTEND_INDEX] = 0;
          if (_HOTEND_TEST) {
            long e_position = stepper.position(E_AXIS);
            if (e_position > last_e_position) {
              lpq[lpq_ptr] = e_position - last_e_position;
              last_e_position = e_position;
            }
            else {
              lpq[lpq_ptr] = 0;
            }
            if (++lpq_ptr >= lpq_len) lpq_ptr = 0;
            cTerm[HOTEND_INDEX] = (lpq[lpq_ptr] * planner.steps_to_mm[E_AXIS]) * PID_PARAM(Kc, HOTEND_INDEX);
            pid_output += cTerm[HOTEND_INDEX];
          }
        #endif // PID_EXTRUSION_SCALING

        if (pid_output > PID_MAX) {
          if (pid_error[HOTEND_INDEX] > 0) temp_iState[HOTEND_INDEX] -= pid_error[HOTEND_INDEX]; // conditional un-integration
          pid_output = PID_MAX;
        }
        else if (pid_output < 0) {
          if (pid_error[HOTEND_INDEX] < 0) temp_iState[HOTEND_INDEX] -= pid_error[HOTEND_INDEX]; // conditional un-integration
          pid_output = 0;
        }
      }
    #else
      pid_output = constrain(target_temperature[HOTEND_INDEX], 0, PID_MAX);
    #endif //PID_OPENLOOP

    #if ENABLED(PID_DEBUG)
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR(MSG_PID_DEBUG, HOTEND_INDEX);
      SERIAL_ECHOPAIR(MSG_PID_DEBUG_INPUT, current_temperature[HOTEND_INDEX]);
      SERIAL_ECHOPAIR(MSG_PID_DEBUG_OUTPUT, pid_output);
      SERIAL_ECHOPAIR(MSG_PID_DEBUG_PTERM, pTerm[HOTEND_INDEX]);
      SERIAL_ECHOPAIR(MSG_PID_DEBUG_ITERM, iTerm[HOTEND_INDEX]);
      SERIAL_ECHOPAIR(MSG_PID_DEBUG_DTERM, dTerm[HOTEND_INDEX]);
      #if ENABLED(PID_EXTRUSION_SCALING)
        SERIAL_ECHOPAIR(MSG_PID_DEBUG_CTERM, cTerm[HOTEND_INDEX]);
      #endif
      SERIAL_EOL;
    #endif //PID_DEBUG

  #else /* PID off */
    pid_output = (current_temperature[HOTEND_INDEX] < target_temperature[HOTEND_INDEX]) ? PID_MAX : 0;
  #endif

  return pid_output;
}

#if ENABLED(PIDTEMPBED)
  float Temperature::get_pid_output_bed() {
    float pid_output;
    #if DISABLED(PID_OPENLOOP)
      pid_error_bed = target_temperature_bed - current_temperature_bed;
      pTerm_bed = bedKp * pid_error_bed;
      temp_iState_bed += pid_error_bed;
      iTerm_bed = bedKi * temp_iState_bed;

      dTerm_bed = K2 * bedKd * (current_temperature_bed - temp_dState_bed) + K1 * dTerm_bed;
      temp_dState_bed = current_temperature_bed;

      pid_output = pTerm_bed + iTerm_bed - dTerm_bed;
      if (pid_output > MAX_BED_POWER) {
        if (pid_error_bed > 0) temp_iState_bed -= pid_error_bed; // conditional un-integration
        pid_output = MAX_BED_POWER;
      }
      else if (pid_output < 0) {
        if (pid_error_bed < 0) temp_iState_bed -= pid_error_bed; // conditional un-integration
        pid_output = 0;
      }
    #else
      pid_output = constrain(target_temperature_bed, 0, MAX_BED_POWER);
    #endif // PID_OPENLOOP

    #if ENABLED(PID_BED_DEBUG)
      SERIAL_ECHO_START;
      SERIAL_ECHOPGM(" PID_BED_DEBUG ");
      SERIAL_ECHOPGM(": Input ");
      SERIAL_ECHO(current_temperature_bed);
      SERIAL_ECHOPGM(" Output ");
      SERIAL_ECHO(pid_output);
      SERIAL_ECHOPGM(" pTerm ");
      SERIAL_ECHO(pTerm_bed);
      SERIAL_ECHOPGM(" iTerm ");
      SERIAL_ECHO(iTerm_bed);
      SERIAL_ECHOPGM(" dTerm ");
      SERIAL_ECHOLN(dTerm_bed);
    #endif //PID_BED_DEBUG

    return pid_output;
  }
#endif //PIDTEMPBED

/**
 * Manage heating activities for extruder hot-ends and a heated bed
 *  - Acquire updated temperature readings
 *    - Also resets the watchdog timer
 *  - Invoke thermal runaway protection
 *  - Manage extruder auto-fan
 *  - Apply filament width to the extrusion rate (may move)
 *  - Update the heated bed PID output value
 */
void Temperature::manage_heater() {

  if (!temp_meas_ready) return;

  updateTemperaturesFromRawValues(); // also resets the watchdog

  #if ENABLED(HEATER_0_USES_MAX6675)
    if (current_temperature[0] > min(HEATER_0_MAXTEMP, 1023)) max_temp_error(0);
    if (current_temperature[0] < max(HEATER_0_MINTEMP, 0.01)) min_temp_error(0);
  #endif

  #if (ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0) || (ENABLED(THERMAL_PROTECTION_BED) && WATCH_BED_TEMP_PERIOD > 0) || DISABLED(PIDTEMPBED) || HAS_AUTO_FAN
    millis_t ms = millis();
  #endif

  // Loop through all hotends
  HOTEND_LOOP() {

    #if ENABLED(THERMAL_PROTECTION_HOTENDS)
      thermal_runaway_protection(&thermal_runaway_state_machine[e], &thermal_runaway_timer[e], current_temperature[e], target_temperature[e], e, THERMAL_PROTECTION_PERIOD, THERMAL_PROTECTION_HYSTERESIS);
    #endif

    float pid_output = get_pid_output(e);

    // Check if temperature is within the correct range
    soft_pwm[e] = (current_temperature[e] > minttemp[e] || is_preheating(e)) && current_temperature[e] < maxttemp[e] ? (int)pid_output >> 1 : 0;

    // Check if the temperature is failing to increase
    #if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0

      // Is it time to check this extruder's heater?
      if (watch_heater_next_ms[e] && ELAPSED(ms, watch_heater_next_ms[e])) {
        // Has it failed to increase enough?
        if (degHotend(e) < watch_target_temp[e]) {
          // Stop!
          _temp_error(e, PSTR(MSG_T_HEATING_FAILED), PSTR(MSG_HEATING_FAILED_LCD));
        }
        else {
          // Start again if the target is still far off
          start_watching_heater(e);
        }
      }

    #endif // THERMAL_PROTECTION_HOTENDS

    // Check if the temperature is failing to increase
    #if ENABLED(THERMAL_PROTECTION_BED) && WATCH_BED_TEMP_PERIOD > 0

      // Is it time to check the bed?
      if (watch_bed_next_ms && ELAPSED(ms, watch_bed_next_ms)) {
        // Has it failed to increase enough?
        if (degBed() < watch_target_bed_temp) {
          // Stop!
          _temp_error(-1, PSTR(MSG_T_HEATING_FAILED), PSTR(MSG_HEATING_FAILED_LCD));
        }
        else {
          // Start again if the target is still far off
          start_watching_bed();
        }
      }

    #endif // THERMAL_PROTECTION_HOTENDS

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      if (fabs(current_temperature[0] - redundant_temperature) > MAX_REDUNDANT_TEMP_SENSOR_DIFF) {
        _temp_error(0, PSTR(MSG_REDUNDANCY), PSTR(MSG_ERR_REDUNDANT_TEMP));
      }
    #endif

  } // Hotends Loop

  #if HAS_AUTO_FAN
    if (ELAPSED(ms, next_auto_fan_check_ms)) { // only need to check fan state very infrequently
      checkExtruderAutoFans();
      next_auto_fan_check_ms = ms + 2500UL;
    }
  #endif

  // Control the extruder rate based on the width sensor
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    if (filament_sensor) {
      meas_shift_index = filwidth_delay_index[0] - meas_delay_cm;
      if (meas_shift_index < 0) meas_shift_index += MAX_MEASUREMENT_DELAY + 1;  //loop around buffer if needed

      // Get the delayed info and add 100 to reconstitute to a percent of
      // the nominal filament diameter then square it to get an area
      meas_shift_index = constrain(meas_shift_index, 0, MAX_MEASUREMENT_DELAY);
      float vm = pow((measurement_delay[meas_shift_index] + 100.0) * 0.01, 2);
      NOLESS(vm, 0.01);
      volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM] = vm;
    }
  #endif //FILAMENT_WIDTH_SENSOR

  #if DISABLED(PIDTEMPBED)
    if (PENDING(ms, next_bed_check_ms)) return;
    next_bed_check_ms = ms + BED_CHECK_INTERVAL;
  #endif

  #if TEMP_SENSOR_BED != 0

    #if HAS_THERMALLY_PROTECTED_BED
      thermal_runaway_protection(&thermal_runaway_bed_state_machine, &thermal_runaway_bed_timer, current_temperature_bed, target_temperature_bed, -1, THERMAL_PROTECTION_BED_PERIOD, THERMAL_PROTECTION_BED_HYSTERESIS);
    #endif

    #if ENABLED(PIDTEMPBED)
      float pid_output = get_pid_output_bed();

      soft_pwm_bed = current_temperature_bed > BED_MINTEMP && current_temperature_bed < BED_MAXTEMP ? (int)pid_output >> 1 : 0;

    #elif ENABLED(BED_LIMIT_SWITCHING)
      // Check if temperature is within the correct band
      if (current_temperature_bed > BED_MINTEMP && current_temperature_bed < BED_MAXTEMP) {
        if (current_temperature_bed >= target_temperature_bed + BED_HYSTERESIS)
          soft_pwm_bed = 0;
        else if (current_temperature_bed <= target_temperature_bed - (BED_HYSTERESIS))
          soft_pwm_bed = MAX_BED_POWER >> 1;
      }
      else {
        soft_pwm_bed = 0;
        WRITE_HEATER_BED(LOW);
      }
    #else // !PIDTEMPBED && !BED_LIMIT_SWITCHING
      // Check if temperature is within the correct range
      if (current_temperature_bed > BED_MINTEMP && current_temperature_bed < BED_MAXTEMP) {
        soft_pwm_bed = current_temperature_bed < target_temperature_bed ? MAX_BED_POWER >> 1 : 0;
      }
      else {
        soft_pwm_bed = 0;
        WRITE_HEATER_BED(LOW);
      }
    #endif
  #endif //TEMP_SENSOR_BED != 0
}

#define PGM_RD_W(x)   (short)pgm_read_word(&x)

// Derived from RepRap FiveD extruder::getTemperature()
// For hot end temperature measurement.
float Temperature::analog2temp(int raw, uint8_t e) {
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    if (e > HOTENDS)
  #else
    if (e >= HOTENDS)
  #endif
    {
      SERIAL_ERROR_START;
      SERIAL_ERROR((int)e);
      SERIAL_ERRORLNPGM(MSG_INVALID_EXTRUDER_NUM);
      kill(PSTR(MSG_KILLED));
      return 0.0;
    }

  #if ENABLED(HEATER_0_USES_MAX6675)
    if (e == 0) return 0.25 * raw;
  #endif

  if (heater_ttbl_map[e] != NULL) {
    float celsius = 0;
    uint8_t i;
    short(*tt)[][2] = (short(*)[][2])(heater_ttbl_map[e]);

    for (i = 1; i < heater_ttbllen_map[e]; i++) {
      if (PGM_RD_W((*tt)[i][0]) > raw) {
        celsius = PGM_RD_W((*tt)[i - 1][1]) +
                  (raw - PGM_RD_W((*tt)[i - 1][0])) *
                  (float)(PGM_RD_W((*tt)[i][1]) - PGM_RD_W((*tt)[i - 1][1])) /
                  (float)(PGM_RD_W((*tt)[i][0]) - PGM_RD_W((*tt)[i - 1][0]));
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == heater_ttbllen_map[e]) celsius = PGM_RD_W((*tt)[i - 1][1]);

    return celsius;
  }
  return ((raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR) * (TEMP_SENSOR_AD595_GAIN)) + TEMP_SENSOR_AD595_OFFSET;
}

// Derived from RepRap FiveD extruder::getTemperature()
// For bed temperature measurement.
float Temperature::analog2tempBed(int raw) {
  #if ENABLED(BED_USES_THERMISTOR)
    float celsius = 0;
    byte i;

    for (i = 1; i < BEDTEMPTABLE_LEN; i++) {
      if (PGM_RD_W(BEDTEMPTABLE[i][0]) > raw) {
        celsius  = PGM_RD_W(BEDTEMPTABLE[i - 1][1]) +
                   (raw - PGM_RD_W(BEDTEMPTABLE[i - 1][0])) *
                   (float)(PGM_RD_W(BEDTEMPTABLE[i][1]) - PGM_RD_W(BEDTEMPTABLE[i - 1][1])) /
                   (float)(PGM_RD_W(BEDTEMPTABLE[i][0]) - PGM_RD_W(BEDTEMPTABLE[i - 1][0]));
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == BEDTEMPTABLE_LEN) celsius = PGM_RD_W(BEDTEMPTABLE[i - 1][1]);

    return celsius;

  #elif defined(BED_USES_AD595)

    return ((raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR) * (TEMP_SENSOR_AD595_GAIN)) + TEMP_SENSOR_AD595_OFFSET;

  #else

    UNUSED(raw);
    return 0;

  #endif
}

/**
 * Get the raw values into the actual temperatures.
 * The raw values are created in interrupt context,
 * and this function is called from normal context
 * as it would block the stepper routine.
 */
void Temperature::updateTemperaturesFromRawValues() {
  #if ENABLED(HEATER_0_USES_MAX6675)
    current_temperature_raw[0] = read_max6675();
  #endif
  HOTEND_LOOP() {
    current_temperature[e] = Temperature::analog2temp(current_temperature_raw[e], e);
  }
  current_temperature_bed = Temperature::analog2tempBed(current_temperature_bed_raw);
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    redundant_temperature = Temperature::analog2temp(redundant_temperature_raw, 1);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    filament_width_meas = analog2widthFil();
  #endif

  #if ENABLED(USE_WATCHDOG)
    // Reset the watchdog after we know we have a temperature measurement.
    watchdog_reset();
  #endif

  CRITICAL_SECTION_START;
  temp_meas_ready = false;
  CRITICAL_SECTION_END;
}


#if ENABLED(FILAMENT_WIDTH_SENSOR)

  // Convert raw Filament Width to millimeters
  float Temperature::analog2widthFil() {
    return current_raw_filwidth / 16383.0 * 5.0;
    //return current_raw_filwidth;
  }

  // Convert raw Filament Width to a ratio
  int Temperature::widthFil_to_size_ratio() {
    float temp = filament_width_meas;
    if (temp < MEASURED_LOWER_LIMIT) temp = filament_width_nominal;  //assume sensor cut out
    else NOMORE(temp, MEASURED_UPPER_LIMIT);
    return filament_width_nominal / temp * 100;
  }

#endif


/**
 * Initialize the temperature manager
 * The manager is implemented by periodic calls to manage_heater()
 */
void Temperature::init() {

  #if MB(RUMBA) && ((TEMP_SENSOR_0==-1)||(TEMP_SENSOR_1==-1)||(TEMP_SENSOR_2==-1)||(TEMP_SENSOR_BED==-1))
    //disable RUMBA JTAG in case the thermocouple extension is plugged on top of JTAG connector
    MCUCR = _BV(JTD);
    MCUCR = _BV(JTD);
  #endif

  // Finish init of mult hotend arrays
  HOTEND_LOOP() {
    // populate with the first value
    maxttemp[e] = maxttemp[0];
    #if ENABLED(PIDTEMP)
      #if ENABLED(PID_EXTRUSION_SCALING)
        last_e_position = 0;
      #endif
    #endif //PIDTEMP
  }

  #if ENABLED(PIDTEMP) && ENABLED(PID_EXTRUSION_SCALING)
    last_e_position = 0;
  #endif

  #if HAS_HEATER_0
    SET_OUTPUT(HEATER_0_PIN);
  #endif
  #if HAS_HEATER_1
    SET_OUTPUT(HEATER_1_PIN);
  #endif
  #if HAS_HEATER_2
    SET_OUTPUT(HEATER_2_PIN);
  #endif
  #if HAS_HEATER_3
    SET_OUTPUT(HEATER_3_PIN);
  #endif
  #if HAS_HEATER_BED
    SET_OUTPUT(HEATER_BED_PIN);
  #endif

  #if HAS_FAN0
    SET_OUTPUT(FAN_PIN);
    #if ENABLED(FAST_PWM_FAN)
      setPwmFrequency(FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
    #if ENABLED(FAN_SOFT_PWM)
      soft_pwm_fan[0] = fanSpeedSoftPwm[0] / 2;
    #endif
  #endif

  #if HAS_FAN1
    SET_OUTPUT(FAN1_PIN);
    #if ENABLED(FAST_PWM_FAN)
      setPwmFrequency(FAN1_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
    #if ENABLED(FAN_SOFT_PWM)
      soft_pwm_fan[1] = fanSpeedSoftPwm[1] / 2;
    #endif
  #endif

  #if HAS_FAN2
    SET_OUTPUT(FAN2_PIN);
    #if ENABLED(FAST_PWM_FAN)
      setPwmFrequency(FAN2_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
    #if ENABLED(FAN_SOFT_PWM)
      soft_pwm_fan[2] = fanSpeedSoftPwm[2] / 2;
    #endif
  #endif

  #if ENABLED(HEATER_0_USES_MAX6675)

    #if DISABLED(SDSUPPORT)
      OUT_WRITE(SCK_PIN, LOW);
      OUT_WRITE(MOSI_PIN, HIGH);
      SET_INPUT(MISO_PIN);
      WRITE(MISO_PIN,1);
    #else
      OUT_WRITE(SS_PIN, HIGH);
    #endif

    OUT_WRITE(MAX6675_SS, HIGH);

  #endif //HEATER_0_USES_MAX6675

  #ifdef DIDR2
    #define ANALOG_SELECT(pin) do{ if (pin < 8) SBI(DIDR0, pin); else SBI(DIDR2, pin - 8); }while(0)
  #else
    #define ANALOG_SELECT(pin) do{ SBI(DIDR0, pin); }while(0)
  #endif

  // Set analog inputs
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | 0x07;
  DIDR0 = 0;
  #ifdef DIDR2
    DIDR2 = 0;
  #endif
  #if HAS_TEMP_0
    ANALOG_SELECT(TEMP_0_PIN);
  #endif
  #if HAS_TEMP_1
    ANALOG_SELECT(TEMP_1_PIN);
  #endif
  #if HAS_TEMP_2
    ANALOG_SELECT(TEMP_2_PIN);
  #endif
  #if HAS_TEMP_3
    ANALOG_SELECT(TEMP_3_PIN);
  #endif
  #if HAS_TEMP_BED
    ANALOG_SELECT(TEMP_BED_PIN);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    ANALOG_SELECT(FILWIDTH_PIN);
  #endif

  #if HAS_AUTO_FAN_0
    #if EXTRUDER_0_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(EXTRUDER_0_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(EXTRUDER_0_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      pinMode(EXTRUDER_0_AUTO_FAN_PIN, OUTPUT);
    #endif
  #endif
  #if HAS_AUTO_FAN_1 && !AUTO_1_IS_0
    #if EXTRUDER_1_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(EXTRUDER_1_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(EXTRUDER_1_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      pinMode(EXTRUDER_1_AUTO_FAN_PIN, OUTPUT);
    #endif
  #endif
  #if HAS_AUTO_FAN_2 && !AUTO_2_IS_0 && !AUTO_2_IS_1
    #if EXTRUDER_2_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(EXTRUDER_2_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(EXTRUDER_2_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      pinMode(EXTRUDER_2_AUTO_FAN_PIN, OUTPUT);
    #endif
  #endif
  #if HAS_AUTO_FAN_3 && !AUTO_3_IS_0 && !AUTO_3_IS_1 && !AUTO_3_IS_2
    #if EXTRUDER_3_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(EXTRUDER_3_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(EXTRUDER_3_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      pinMode(EXTRUDER_3_AUTO_FAN_PIN, OUTPUT);
    #endif
  #endif

  // Use timer0 for temperature measurement
  // Interleave temperature interrupt with millies interrupt
  OCR0B = 128;
  SBI(TIMSK0, OCIE0B);

  // Wait for temperature measurement to settle
  delay(250);

  #define TEMP_MIN_ROUTINE(NR) \
    minttemp[NR] = HEATER_ ## NR ## _MINTEMP; \
    while(analog2temp(minttemp_raw[NR], NR) < HEATER_ ## NR ## _MINTEMP) { \
      if (HEATER_ ## NR ## _RAW_LO_TEMP < HEATER_ ## NR ## _RAW_HI_TEMP) \
        minttemp_raw[NR] += OVERSAMPLENR; \
      else \
        minttemp_raw[NR] -= OVERSAMPLENR; \
    }
  #define TEMP_MAX_ROUTINE(NR) \
    maxttemp[NR] = HEATER_ ## NR ## _MAXTEMP; \
    while(analog2temp(maxttemp_raw[NR], NR) > HEATER_ ## NR ## _MAXTEMP) { \
      if (HEATER_ ## NR ## _RAW_LO_TEMP < HEATER_ ## NR ## _RAW_HI_TEMP) \
        maxttemp_raw[NR] -= OVERSAMPLENR; \
      else \
        maxttemp_raw[NR] += OVERSAMPLENR; \
    }

  #ifdef HEATER_0_MINTEMP
    TEMP_MIN_ROUTINE(0);
  #endif
  #ifdef HEATER_0_MAXTEMP
    TEMP_MAX_ROUTINE(0);
  #endif
  #if HOTENDS > 1
    #ifdef HEATER_1_MINTEMP
      TEMP_MIN_ROUTINE(1);
    #endif
    #ifdef HEATER_1_MAXTEMP
      TEMP_MAX_ROUTINE(1);
    #endif
    #if HOTENDS > 2
      #ifdef HEATER_2_MINTEMP
        TEMP_MIN_ROUTINE(2);
      #endif
      #ifdef HEATER_2_MAXTEMP
        TEMP_MAX_ROUTINE(2);
      #endif
      #if HOTENDS > 3
        #ifdef HEATER_3_MINTEMP
          TEMP_MIN_ROUTINE(3);
        #endif
        #ifdef HEATER_3_MAXTEMP
          TEMP_MAX_ROUTINE(3);
        #endif
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1

  #ifdef BED_MINTEMP
    while(analog2tempBed(bed_minttemp_raw) < BED_MINTEMP) {
      #if HEATER_BED_RAW_LO_TEMP < HEATER_BED_RAW_HI_TEMP
        bed_minttemp_raw += OVERSAMPLENR;
      #else
        bed_minttemp_raw -= OVERSAMPLENR;
      #endif
    }
  #endif //BED_MINTEMP
  #ifdef BED_MAXTEMP
    while (analog2tempBed(bed_maxttemp_raw) > BED_MAXTEMP) {
      #if HEATER_BED_RAW_LO_TEMP < HEATER_BED_RAW_HI_TEMP
        bed_maxttemp_raw -= OVERSAMPLENR;
      #else
        bed_maxttemp_raw += OVERSAMPLENR;
      #endif
    }
  #endif //BED_MAXTEMP
}

#if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0
  /**
   * Start Heating Sanity Check for hotends that are below
   * their target temperature by a configurable margin.
   * This is called when the temperature is set. (M104, M109)
   */
  void Temperature::start_watching_heater(uint8_t e) {
    #if HOTENDS == 1
      UNUSED(e);
    #endif
    if (degHotend(HOTEND_INDEX) < degTargetHotend(HOTEND_INDEX) - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1)) {
      watch_target_temp[HOTEND_INDEX] = degHotend(HOTEND_INDEX) + WATCH_TEMP_INCREASE;
      watch_heater_next_ms[HOTEND_INDEX] = millis() + (WATCH_TEMP_PERIOD) * 1000UL;
    }
    else
      watch_heater_next_ms[HOTEND_INDEX] = 0;
  }
#endif

#if ENABLED(THERMAL_PROTECTION_BED) && WATCH_BED_TEMP_PERIOD > 0
  /**
   * Start Heating Sanity Check for hotends that are below
   * their target temperature by a configurable margin.
   * This is called when the temperature is set. (M140, M190)
   */
  void Temperature::start_watching_bed() {
    if (degBed() < degTargetBed() - (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1)) {
      watch_target_bed_temp = degBed() + WATCH_BED_TEMP_INCREASE;
      watch_bed_next_ms = millis() + (WATCH_BED_TEMP_PERIOD) * 1000UL;
    }
    else
      watch_bed_next_ms = 0;
  }
#endif

#if ENABLED(THERMAL_PROTECTION_HOTENDS) || HAS_THERMALLY_PROTECTED_BED

  #if ENABLED(THERMAL_PROTECTION_HOTENDS)
    Temperature::TRState Temperature::thermal_runaway_state_machine[HOTENDS] = { TRInactive };
    millis_t Temperature::thermal_runaway_timer[HOTENDS] = { 0 };
  #endif

  #if HAS_THERMALLY_PROTECTED_BED
    Temperature::TRState Temperature::thermal_runaway_bed_state_machine = TRInactive;
    millis_t Temperature::thermal_runaway_bed_timer;
  #endif

  void Temperature::thermal_runaway_protection(Temperature::TRState* state, millis_t* timer, float temperature, float target_temperature, int heater_id, int period_seconds, int hysteresis_degc) {

    static float tr_target_temperature[HOTENDS + 1] = { 0.0 };

    /**
        SERIAL_ECHO_START;
        SERIAL_ECHOPGM("Thermal Thermal Runaway Running. Heater ID: ");
        if (heater_id < 0) SERIAL_ECHOPGM("bed"); else SERIAL_ECHO(heater_id);
        SERIAL_ECHOPAIR(" ;  State:", *state);
        SERIAL_ECHOPAIR(" ;  Timer:", *timer);
        SERIAL_ECHOPAIR(" ;  Temperature:", temperature);
        SERIAL_ECHOPAIR(" ;  Target Temp:", target_temperature);
        SERIAL_EOL;
    */

    int heater_index = heater_id >= 0 ? heater_id : HOTENDS;

    // If the target temperature changes, restart
    if (tr_target_temperature[heater_index] != target_temperature) {
      tr_target_temperature[heater_index] = target_temperature;
      *state = target_temperature > 0 ? TRFirstHeating : TRInactive;
    }

    switch (*state) {
      // Inactive state waits for a target temperature to be set
      case TRInactive: break;
      // When first heating, wait for the temperature to be reached then go to Stable state
      case TRFirstHeating:
        if (temperature < tr_target_temperature[heater_index]) break;
        *state = TRStable;
      // While the temperature is stable watch for a bad temperature
      case TRStable:
        if (temperature >= tr_target_temperature[heater_index] - hysteresis_degc) {
          *timer = millis() + period_seconds * 1000UL;
          break;
        }
        else if (PENDING(millis(), *timer)) break;
        *state = TRRunaway;
      case TRRunaway:
        _temp_error(heater_id, PSTR(MSG_T_THERMAL_RUNAWAY), PSTR(MSG_THERMAL_RUNAWAY));
    }
  }

#endif // THERMAL_PROTECTION_HOTENDS || THERMAL_PROTECTION_BED

void Temperature::disable_all_heaters() {
  HOTEND_LOOP() setTargetHotend(0, e);
  setTargetBed(0);

  // If all heaters go down then for sure our print job has stopped
  print_job_timer.stop();

  #define DISABLE_HEATER(NR) { \
    setTargetHotend(0, NR); \
    soft_pwm[NR] = 0; \
    WRITE_HEATER_ ## NR (LOW); \
  }

  #if HAS_TEMP_HOTEND
    DISABLE_HEATER(0);
  #endif

  #if HOTENDS > 1 && HAS_TEMP_1
    DISABLE_HEATER(1);
  #endif

  #if HOTENDS > 2 && HAS_TEMP_2
    DISABLE_HEATER(2);
  #endif

  #if HOTENDS > 3 && HAS_TEMP_3
    DISABLE_HEATER(3);
  #endif

  #if HAS_TEMP_BED
    target_temperature_bed = 0;
    soft_pwm_bed = 0;
    #if HAS_HEATER_BED
      WRITE_HEATER_BED(LOW);
    #endif
  #endif
}

#if ENABLED(HEATER_0_USES_MAX6675)

  #define MAX6675_HEAT_INTERVAL 250u

  #if ENABLED(MAX6675_IS_MAX31855)
    uint32_t max6675_temp = 2000;
    #define MAX6675_ERROR_MASK 7
    #define MAX6675_DISCARD_BITS 18
    #define MAX6675_SPEED_BITS (_BV(SPR1)) // clock ÷ 64
  #else
    uint16_t max6675_temp = 2000;
    #define MAX6675_ERROR_MASK 4
    #define MAX6675_DISCARD_BITS 3
    #define MAX6675_SPEED_BITS (_BV(SPR0)) // clock ÷ 16
  #endif

  int Temperature::read_max6675() {

    static millis_t next_max6675_ms = 0;

    millis_t ms = millis();

    if (PENDING(ms, next_max6675_ms)) return (int)max6675_temp;

    next_max6675_ms = ms + MAX6675_HEAT_INTERVAL;

    CBI(
      #ifdef PRR
        PRR
      #elif defined(PRR0)
        PRR0
      #endif
        , PRSPI);
    SPCR = _BV(MSTR) | _BV(SPE) | MAX6675_SPEED_BITS;

    WRITE(MAX6675_SS, 0); // enable TT_MAX6675

    // ensure 100ns delay - a bit extra is fine
    asm("nop");//50ns on 20Mhz, 62.5ns on 16Mhz
    asm("nop");//50ns on 20Mhz, 62.5ns on 16Mhz

    // Read a big-endian temperature value
    max6675_temp = 0;
    for (uint8_t i = sizeof(max6675_temp); i--;) {
      SPDR = 0;
      for (;!TEST(SPSR, SPIF););
      max6675_temp |= SPDR;
      if (i > 0) max6675_temp <<= 8; // shift left if not the last byte
    }

    WRITE(MAX6675_SS, 1); // disable TT_MAX6675

    if (max6675_temp & MAX6675_ERROR_MASK)
      max6675_temp = 4000; // thermocouple open
    else
      max6675_temp >>= MAX6675_DISCARD_BITS;

    return (int)max6675_temp;
  }

#endif //HEATER_0_USES_MAX6675

/**
 * Get raw temperatures
 */
void Temperature::set_current_temp_raw() {
  #if HAS_TEMP_0 && DISABLED(HEATER_0_USES_MAX6675)
    current_temperature_raw[0] = raw_temp_value[0];
  #endif
  #if HAS_TEMP_1
    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      redundant_temperature_raw = raw_temp_value[1];
    #else
      current_temperature_raw[1] = raw_temp_value[1];
    #endif
    #if HAS_TEMP_2
      current_temperature_raw[2] = raw_temp_value[2];
      #if HAS_TEMP_3
        current_temperature_raw[3] = raw_temp_value[3];
      #endif
    #endif
  #endif
  current_temperature_bed_raw = raw_temp_bed_value;
  temp_meas_ready = true;
}

/**
 * Timer 0 is shared with millies so don't change the prescaler.
 *
 * This ISR uses the compare method so it runs at the base
 * frequency (16 MHz / 256 = 62500 Hz), but at the TCNT0 set
 * in OCR0B above (128 or halfway between OVFs).
 *
 *  - Manage PWM to all the heaters and fan
 *  - Update the raw temperature values
 *  - Check new temperature values for MIN/MAX errors
 *  - Step the babysteps value for each axis towards 0
 */
ISR(TIMER0_COMPB_vect) { Temperature::isr(); }

void Temperature::isr() {

  static uint8_t temp_count = 0;
  static TempState temp_state = StartupDelay;
  static uint8_t pwm_count = _BV(SOFT_PWM_SCALE);

  // Static members for each heater
  #if ENABLED(SLOW_PWM_HEATERS)
    static uint8_t slow_pwm_count = 0;
    #define ISR_STATICS(n) \
      static uint8_t soft_pwm_ ## n; \
      static uint8_t state_heater_ ## n = 0; \
      static uint8_t state_timer_heater_ ## n = 0
  #else
    #define ISR_STATICS(n) static uint8_t soft_pwm_ ## n
  #endif

  // Statics per heater
  ISR_STATICS(0);
  #if HOTENDS > 1
    ISR_STATICS(1);
    #if HOTENDS > 2
      ISR_STATICS(2);
      #if HOTENDS > 3
        ISR_STATICS(3);
      #endif
    #endif
  #endif
  #if HAS_HEATER_BED
    ISR_STATICS(BED);
  #endif

  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    static unsigned long raw_filwidth_value = 0;
  #endif

  #if DISABLED(SLOW_PWM_HEATERS)
    /**
     * Standard PWM modulation
     */
    if (pwm_count == 0) {
      soft_pwm_0 = soft_pwm[0];
      WRITE_HEATER_0(soft_pwm_0 > 0 ? 1 : 0);
      #if HOTENDS > 1
        soft_pwm_1 = soft_pwm[1];
        WRITE_HEATER_1(soft_pwm_1 > 0 ? 1 : 0);
        #if HOTENDS > 2
          soft_pwm_2 = soft_pwm[2];
          WRITE_HEATER_2(soft_pwm_2 > 0 ? 1 : 0);
          #if HOTENDS > 3
            soft_pwm_3 = soft_pwm[3];
            WRITE_HEATER_3(soft_pwm_3 > 0 ? 1 : 0);
          #endif
        #endif
      #endif

      #if HAS_HEATER_BED
        soft_pwm_BED = soft_pwm_bed;
        WRITE_HEATER_BED(soft_pwm_BED > 0 ? 1 : 0);
      #endif

      #if ENABLED(FAN_SOFT_PWM)
        #if HAS_FAN0
          soft_pwm_fan[0] = fanSpeedSoftPwm[0] / 2;
          WRITE_FAN(soft_pwm_fan[0] > 0 ? 1 : 0);
        #endif
        #if HAS_FAN1
          soft_pwm_fan[1] = fanSpeedSoftPwm[1] / 2;
          WRITE_FAN1(soft_pwm_fan[1] > 0 ? 1 : 0);
        #endif
        #if HAS_FAN2
          soft_pwm_fan[2] = fanSpeedSoftPwm[2] / 2;
          WRITE_FAN2(soft_pwm_fan[2] > 0 ? 1 : 0);
        #endif
      #endif
    }

    if (soft_pwm_0 < pwm_count) WRITE_HEATER_0(0);
    #if HOTENDS > 1
      if (soft_pwm_1 < pwm_count) WRITE_HEATER_1(0);
      #if HOTENDS > 2
        if (soft_pwm_2 < pwm_count) WRITE_HEATER_2(0);
        #if HOTENDS > 3
          if (soft_pwm_3 < pwm_count) WRITE_HEATER_3(0);
        #endif
      #endif
    #endif

    #if HAS_HEATER_BED
      if (soft_pwm_BED < pwm_count) WRITE_HEATER_BED(0);
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      #if HAS_FAN0
        if (soft_pwm_fan[0] < pwm_count) WRITE_FAN(0);
      #endif
      #if HAS_FAN1
        if (soft_pwm_fan[1] < pwm_count) WRITE_FAN1(0);
      #endif
      #if HAS_FAN2
        if (soft_pwm_fan[2] < pwm_count) WRITE_FAN2(0);
      #endif
    #endif

    // 488.28 Hz (or 1:976.56, 2:1953.12, 3:3906.25, 4:7812.5, 5:7812.5 6:15625, 6:15625 7:31250)
    pwm_count += _BV(SOFT_PWM_SCALE);
    pwm_count &= 0x7f;

  #else // SLOW_PWM_HEATERS

    /**
     * SLOW PWM HEATERS
     *
     * For relay-driven heaters
     */
    #ifndef MIN_STATE_TIME
      #define MIN_STATE_TIME 16 // MIN_STATE_TIME * 65.5 = time in milliseconds
    #endif

    // Macros for Slow PWM timer logic
    #define _SLOW_PWM_ROUTINE(NR, src) \
      soft_pwm_ ## NR = src; \
      if (soft_pwm_ ## NR > 0) { \
        if (state_timer_heater_ ## NR == 0) { \
          if (state_heater_ ## NR == 0) state_timer_heater_ ## NR = MIN_STATE_TIME; \
          state_heater_ ## NR = 1; \
          WRITE_HEATER_ ## NR(1); \
        } \
      } \
      else { \
        if (state_timer_heater_ ## NR == 0) { \
          if (state_heater_ ## NR == 1) state_timer_heater_ ## NR = MIN_STATE_TIME; \
          state_heater_ ## NR = 0; \
          WRITE_HEATER_ ## NR(0); \
        } \
      }
    #define SLOW_PWM_ROUTINE(n) _SLOW_PWM_ROUTINE(n, soft_pwm[n])

    #define PWM_OFF_ROUTINE(NR) \
      if (soft_pwm_ ## NR < slow_pwm_count) { \
        if (state_timer_heater_ ## NR == 0) { \
          if (state_heater_ ## NR == 1) state_timer_heater_ ## NR = MIN_STATE_TIME; \
          state_heater_ ## NR = 0; \
          WRITE_HEATER_ ## NR (0); \
        } \
      }

    if (slow_pwm_count == 0) {

      SLOW_PWM_ROUTINE(0); // EXTRUDER 0
      #if HOTENDS > 1
        SLOW_PWM_ROUTINE(1); // EXTRUDER 1
        #if HOTENDS > 2
          SLOW_PWM_ROUTINE(2); // EXTRUDER 2
          #if HOTENDS > 3
            SLOW_PWM_ROUTINE(3); // EXTRUDER 3
          #endif
        #endif
      #endif
      #if HAS_HEATER_BED
        _SLOW_PWM_ROUTINE(BED, soft_pwm_bed); // BED
      #endif

    } // slow_pwm_count == 0

    PWM_OFF_ROUTINE(0); // EXTRUDER 0
    #if HOTENDS > 1
      PWM_OFF_ROUTINE(1); // EXTRUDER 1
      #if HOTENDS > 2
        PWM_OFF_ROUTINE(2); // EXTRUDER 2
        #if HOTENDS > 3
          PWM_OFF_ROUTINE(3); // EXTRUDER 3
        #endif
      #endif
    #endif
    #if HAS_HEATER_BED
      PWM_OFF_ROUTINE(BED); // BED
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      if (pwm_count == 0) {
        #if HAS_FAN0
          soft_pwm_fan[0] = fanSpeedSoftPwm[0] / 2;
          WRITE_FAN(soft_pwm_fan[0] > 0 ? 1 : 0);
        #endif
        #if HAS_FAN1
          soft_pwm_fan[1] = fanSpeedSoftPwm[1] / 2;
          WRITE_FAN1(soft_pwm_fan[1] > 0 ? 1 : 0);
        #endif
        #if HAS_FAN2
          soft_pwm_fan[2] = fanSpeedSoftPwm[2] / 2;
          WRITE_FAN2(soft_pwm_fan[2] > 0 ? 1 : 0);
        #endif
      }
      #if HAS_FAN0
        if (soft_pwm_fan[0] < pwm_count) WRITE_FAN(0);
      #endif
      #if HAS_FAN1
        if (soft_pwm_fan[1] < pwm_count) WRITE_FAN1(0);
      #endif
      #if HAS_FAN2
        if (soft_pwm_fan[2] < pwm_count) WRITE_FAN2(0);
      #endif
    #endif //FAN_SOFT_PWM

    pwm_count += _BV(SOFT_PWM_SCALE);
    pwm_count &= 0x7f;

    // increment slow_pwm_count only every 64 pwm_count circa 65.5ms
    if ((pwm_count % 64) == 0) {
      slow_pwm_count++;
      slow_pwm_count &= 0x7f;

      // EXTRUDER 0
      if (state_timer_heater_0 > 0) state_timer_heater_0--;
      #if HOTENDS > 1    // EXTRUDER 1
        if (state_timer_heater_1 > 0) state_timer_heater_1--;
        #if HOTENDS > 2    // EXTRUDER 2
          if (state_timer_heater_2 > 0) state_timer_heater_2--;
          #if HOTENDS > 3    // EXTRUDER 3
            if (state_timer_heater_3 > 0) state_timer_heater_3--;
          #endif
        #endif
      #endif
      #if HAS_HEATER_BED
        if (state_timer_heater_BED > 0) state_timer_heater_BED--;
      #endif
    } // (pwm_count % 64) == 0

  #endif // SLOW_PWM_HEATERS

  #define SET_ADMUX_ADCSRA(pin) ADMUX = _BV(REFS0) | (pin & 0x07); SBI(ADCSRA, ADSC)
  #ifdef MUX5
    #define START_ADC(pin) if (pin > 7) ADCSRB = _BV(MUX5); else ADCSRB = 0; SET_ADMUX_ADCSRA(pin)
  #else
    #define START_ADC(pin) ADCSRB = 0; SET_ADMUX_ADCSRA(pin)
  #endif

  // Prepare or measure a sensor, each one every 12th frame
  switch (temp_state) {
    case PrepareTemp_0:
      #if HAS_TEMP_0
        START_ADC(TEMP_0_PIN);
      #endif
      lcd_buttons_update();
      temp_state = MeasureTemp_0;
      break;
    case MeasureTemp_0:
      #if HAS_TEMP_0
        raw_temp_value[0] += ADC;
      #endif
      temp_state = PrepareTemp_BED;
      break;

    case PrepareTemp_BED:
      #if HAS_TEMP_BED
        START_ADC(TEMP_BED_PIN);
      #endif
      lcd_buttons_update();
      temp_state = MeasureTemp_BED;
      break;
    case MeasureTemp_BED:
      #if HAS_TEMP_BED
        raw_temp_bed_value += ADC;
      #endif
      temp_state = PrepareTemp_1;
      break;

    case PrepareTemp_1:
      #if HAS_TEMP_1
        START_ADC(TEMP_1_PIN);
      #endif
      lcd_buttons_update();
      temp_state = MeasureTemp_1;
      break;
    case MeasureTemp_1:
      #if HAS_TEMP_1
        raw_temp_value[1] += ADC;
      #endif
      temp_state = PrepareTemp_2;
      break;

    case PrepareTemp_2:
      #if HAS_TEMP_2
        START_ADC(TEMP_2_PIN);
      #endif
      lcd_buttons_update();
      temp_state = MeasureTemp_2;
      break;
    case MeasureTemp_2:
      #if HAS_TEMP_2
        raw_temp_value[2] += ADC;
      #endif
      temp_state = PrepareTemp_3;
      break;

    case PrepareTemp_3:
      #if HAS_TEMP_3
        START_ADC(TEMP_3_PIN);
      #endif
      lcd_buttons_update();
      temp_state = MeasureTemp_3;
      break;
    case MeasureTemp_3:
      #if HAS_TEMP_3
        raw_temp_value[3] += ADC;
      #endif
      temp_state = Prepare_FILWIDTH;
      break;

    case Prepare_FILWIDTH:
      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        START_ADC(FILWIDTH_PIN);
      #endif
      lcd_buttons_update();
      temp_state = Measure_FILWIDTH;
      break;
    case Measure_FILWIDTH:
      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        // raw_filwidth_value += ADC;  //remove to use an IIR filter approach
        if (ADC > 102) { //check that ADC is reading a voltage > 0.5 volts, otherwise don't take in the data.
          raw_filwidth_value -= (raw_filwidth_value >> 7); //multiply raw_filwidth_value by 127/128
          raw_filwidth_value += ((unsigned long)ADC << 7); //add new ADC reading
        }
      #endif
      temp_state = PrepareTemp_0;
      temp_count++;
      break;

    case StartupDelay:
      temp_state = PrepareTemp_0;
      break;

    // default:
    //   SERIAL_ERROR_START;
    //   SERIAL_ERRORLNPGM("Temp measurement error!");
    //   break;
  } // switch(temp_state)

  if (temp_count >= OVERSAMPLENR) { // 10 * 16 * 1/(16000000/64/256)  = 164ms.
    // Update the raw values if they've been read. Else we could be updating them during reading.
    if (!temp_meas_ready) set_current_temp_raw();

    // Filament Sensor - can be read any time since IIR filtering is used
    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      current_raw_filwidth = raw_filwidth_value >> 10;  // Divide to get to 0-16384 range since we used 1/128 IIR filter approach
    #endif

    temp_count = 0;
    for (int i = 0; i < 4; i++) raw_temp_value[i] = 0;
    raw_temp_bed_value = 0;

    #if HAS_TEMP_0 && DISABLED(HEATER_0_USES_MAX6675)
      #if HEATER_0_RAW_LO_TEMP > HEATER_0_RAW_HI_TEMP
        #define GE0 <=
      #else
        #define GE0 >=
      #endif
      if (current_temperature_raw[0] GE0 maxttemp_raw[0]) max_temp_error(0);
      if (minttemp_raw[0] GE0 current_temperature_raw[0] && !is_preheating(0) && target_temperature[0] > 0.0f) {
        #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
          if (++consecutive_low_temperature_error[0] >= MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED)
        #endif
            min_temp_error(0);
      }
      #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
        else
          consecutive_low_temperature_error[0] = 0;
      #endif
    #endif

    #if HAS_TEMP_1 && HOTENDS > 1
      #if HEATER_1_RAW_LO_TEMP > HEATER_1_RAW_HI_TEMP
        #define GE1 <=
      #else
        #define GE1 >=
      #endif
      if (current_temperature_raw[1] GE1 maxttemp_raw[1]) max_temp_error(1);
      if (minttemp_raw[1] GE1 current_temperature_raw[1] && !is_preheating(1) && target_temperature[1] > 0.0f) {
        #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
          if (++consecutive_low_temperature_error[1] >= MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED)
        #endif
          min_temp_error(1);
      }
      #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
        else
          consecutive_low_temperature_error[1] = 0;
      #endif
    #endif // TEMP_SENSOR_1

    #if HAS_TEMP_2 && HOTENDS > 2
      #if HEATER_2_RAW_LO_TEMP > HEATER_2_RAW_HI_TEMP
        #define GE2 <=
      #else
        #define GE2 >=
      #endif
      if (current_temperature_raw[2] GE2 maxttemp_raw[2]) max_temp_error(2);
      if (minttemp_raw[2] GE2 current_temperature_raw[2] && !is_preheating(2) && target_temperature[2] > 0.0f) {
        #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
          if (++consecutive_low_temperature_error[2] >= MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED)
        #endif
          min_temp_error(2);
      }
      #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
        else
          consecutive_low_temperature_error[2] = 0;
      #endif
    #endif // TEMP_SENSOR_2

    #if HAS_TEMP_3 && HOTENDS > 3
      #if HEATER_3_RAW_LO_TEMP > HEATER_3_RAW_HI_TEMP
        #define GE3 <=
      #else
        #define GE3 >=
      #endif
      if (current_temperature_raw[3] GE3 maxttemp_raw[3]) max_temp_error(3);
      if (minttemp_raw[3] GE3 current_temperature_raw[3] && !is_preheating(3) && target_temperature[3] > 0.0f) {
        #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
          if (++consecutive_low_temperature_error[3] >= MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED)
        #endif
          min_temp_error(3);
      }
      #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
        else
          consecutive_low_temperature_error[3] = 0;
      #endif
    #endif // TEMP_SENSOR_3

    #if HAS_TEMP_BED
      #if HEATER_BED_RAW_LO_TEMP > HEATER_BED_RAW_HI_TEMP
        #define GEBED <=
      #else
        #define GEBED >=
      #endif
      if (current_temperature_bed_raw GEBED bed_maxttemp_raw) max_temp_error(-1);
      if (bed_minttemp_raw GEBED current_temperature_bed_raw && target_temperature_bed > 0.0f) min_temp_error(-1);
    #endif

  } // temp_count >= OVERSAMPLENR

  #if ENABLED(BABYSTEPPING)
    for (uint8_t axis = X_AXIS; axis <= Z_AXIS; axis++) {
      int curTodo = babystepsTodo[axis]; //get rid of volatile for performance

      if (curTodo > 0) {
        stepper.babystep(axis,/*fwd*/true);
        babystepsTodo[axis]--; //fewer to do next time
      }
      else if (curTodo < 0) {
        stepper.babystep(axis,/*fwd*/false);
        babystepsTodo[axis]++; //fewer to do next time
      }
    }
  #endif //BABYSTEPPING
}
