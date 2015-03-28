/*
  temperature.c - temperature control
  Part of Marlin
  
 Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 This firmware is a mashup between Sprinter and grbl.
  (https://github.com/kliment/Sprinter)
  (https://github.com/simen/grbl/tree)
 
 It has preliminary support for Matthew Roberts advance algorithm 
    http://reprap.org/pipermail/reprap-dev/2011-May/003323.html

 */


#include "Marlin.h"
#include "ultralcd.h"
#include "temperature.h"
#include "watchdog.h"
#include "language.h"

#include "Sd2PinMap.h"

//===========================================================================
//================================== macros =================================
//===========================================================================

#ifdef K1 // Defined in Configuration.h in the PID settings
  #define K2 (1.0-K1)
#endif

#if defined(PIDTEMPBED) || defined(PIDTEMP)
  #define PID_dT ((OVERSAMPLENR * 12.0)/(F_CPU / 64.0 / 256.0))
#endif

//===========================================================================
//============================= public variables ============================
//===========================================================================

int target_temperature[4] = { 0 };
int target_temperature_bed = 0;
int current_temperature_raw[4] = { 0 };
float current_temperature[4] = { 0.0 };
int current_temperature_bed_raw = 0;
float current_temperature_bed = 0.0;
#ifdef TEMP_SENSOR_1_AS_REDUNDANT
  int redundant_temperature_raw = 0;
  float redundant_temperature = 0.0;
#endif

#ifdef PIDTEMPBED
  float bedKp=DEFAULT_bedKp;
  float bedKi=(DEFAULT_bedKi*PID_dT);
  float bedKd=(DEFAULT_bedKd/PID_dT);
#endif //PIDTEMPBED
  
#ifdef FAN_SOFT_PWM
  unsigned char fanSpeedSoftPwm;
#endif

unsigned char soft_pwm_bed;
  
#ifdef BABYSTEPPING
  volatile int babystepsTodo[3] = { 0 };
#endif

#ifdef FILAMENT_SENSOR
  int current_raw_filwidth = 0;  //Holds measured filament diameter - one extruder only
#endif  
//===========================================================================
//=============================private variables============================
//===========================================================================
static volatile bool temp_meas_ready = false;

#ifdef PIDTEMP
  //static cannot be external:
  static float temp_iState[EXTRUDERS] = { 0 };
  static float temp_dState[EXTRUDERS] = { 0 };
  static float pTerm[EXTRUDERS];
  static float iTerm[EXTRUDERS];
  static float dTerm[EXTRUDERS];
  //int output;
  static float pid_error[EXTRUDERS];
  static float temp_iState_min[EXTRUDERS];
  static float temp_iState_max[EXTRUDERS];
  static bool pid_reset[EXTRUDERS];
#endif //PIDTEMP
#ifdef PIDTEMPBED
  //static cannot be external:
  static float temp_iState_bed = { 0 };
  static float temp_dState_bed = { 0 };
  static float pTerm_bed;
  static float iTerm_bed;
  static float dTerm_bed;
  //int output;
  static float pid_error_bed;
  static float temp_iState_min_bed;
  static float temp_iState_max_bed;
#else //PIDTEMPBED
  static unsigned long  previous_millis_bed_heater;
#endif //PIDTEMPBED
  static unsigned char soft_pwm[EXTRUDERS];

#ifdef FAN_SOFT_PWM
  static unsigned char soft_pwm_fan;
#endif
#if HAS_AUTO_FAN
  static unsigned long extruder_autofan_last_check;
#endif  

#ifdef PIDTEMP
  #ifdef PID_PARAMS_PER_EXTRUDER
    float Kp[EXTRUDERS] = ARRAY_BY_EXTRUDERS(DEFAULT_Kp, DEFAULT_Kp, DEFAULT_Kp, DEFAULT_Kp);
    float Ki[EXTRUDERS] = ARRAY_BY_EXTRUDERS(DEFAULT_Ki*PID_dT, DEFAULT_Ki*PID_dT, DEFAULT_Ki*PID_dT, DEFAULT_Ki*PID_dT);
    float Kd[EXTRUDERS] = ARRAY_BY_EXTRUDERS(DEFAULT_Kd / PID_dT, DEFAULT_Kd / PID_dT, DEFAULT_Kd / PID_dT, DEFAULT_Kd / PID_dT);
    #ifdef PID_ADD_EXTRUSION_RATE
      float Kc[EXTRUDERS] = ARRAY_BY_EXTRUDERS(DEFAULT_Kc, DEFAULT_Kc, DEFAULT_Kc, DEFAULT_Kc);
    #endif // PID_ADD_EXTRUSION_RATE
  #else //PID_PARAMS_PER_EXTRUDER
    float Kp = DEFAULT_Kp;
    float Ki = DEFAULT_Ki * PID_dT;
    float Kd = DEFAULT_Kd / PID_dT;
    #ifdef PID_ADD_EXTRUSION_RATE
      float Kc = DEFAULT_Kc;
    #endif // PID_ADD_EXTRUSION_RATE
  #endif // PID_PARAMS_PER_EXTRUDER
#endif //PIDTEMP

// Init min and max temp with extreme values to prevent false errors during startup
static int minttemp_raw[EXTRUDERS] = ARRAY_BY_EXTRUDERS( HEATER_0_RAW_LO_TEMP , HEATER_1_RAW_LO_TEMP , HEATER_2_RAW_LO_TEMP, HEATER_3_RAW_LO_TEMP);
static int maxttemp_raw[EXTRUDERS] = ARRAY_BY_EXTRUDERS( HEATER_0_RAW_HI_TEMP , HEATER_1_RAW_HI_TEMP , HEATER_2_RAW_HI_TEMP, HEATER_3_RAW_HI_TEMP);
static int minttemp[EXTRUDERS] = { 0 };
static int maxttemp[EXTRUDERS] = ARRAY_BY_EXTRUDERS( 16383, 16383, 16383, 16383 );
//static int bed_minttemp_raw = HEATER_BED_RAW_LO_TEMP; /* No bed mintemp error implemented?!? */
#ifdef BED_MAXTEMP
  static int bed_maxttemp_raw = HEATER_BED_RAW_HI_TEMP;
#endif

#ifdef TEMP_SENSOR_1_AS_REDUNDANT
  static void *heater_ttbl_map[2] = {(void *)HEATER_0_TEMPTABLE, (void *)HEATER_1_TEMPTABLE };
  static uint8_t heater_ttbllen_map[2] = { HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN };
#else
  static void *heater_ttbl_map[EXTRUDERS] = ARRAY_BY_EXTRUDERS( (void *)HEATER_0_TEMPTABLE, (void *)HEATER_1_TEMPTABLE, (void *)HEATER_2_TEMPTABLE, (void *)HEATER_3_TEMPTABLE );
  static uint8_t heater_ttbllen_map[EXTRUDERS] = ARRAY_BY_EXTRUDERS( HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN, HEATER_2_TEMPTABLE_LEN, HEATER_3_TEMPTABLE_LEN );
#endif

static float analog2temp(int raw, uint8_t e);
static float analog2tempBed(int raw);
static void updateTemperaturesFromRawValues();

#ifdef WATCH_TEMP_PERIOD
  int watch_start_temp[EXTRUDERS] = { 0 };
  unsigned long watchmillis[EXTRUDERS] = { 0 };
#endif //WATCH_TEMP_PERIOD

#ifndef SOFT_PWM_SCALE
  #define SOFT_PWM_SCALE 0
#endif

#ifdef FILAMENT_SENSOR
  static int meas_shift_index;  //used to point to a delayed sample in buffer for filament width sensor
#endif

#ifdef HEATER_0_USES_MAX6675
  static int read_max6675();
#endif

//===========================================================================
//=============================   functions      ============================
//===========================================================================

void PID_autotune(float temp, int extruder, int ncycles)
{
  float input = 0.0;
  int cycles = 0;
  bool heating = true;

  unsigned long temp_millis = millis(), t1 = temp_millis, t2 = temp_millis;
  long t_high = 0, t_low = 0;

  long bias, d;
  float Ku, Tu;
  float Kp, Ki, Kd;
  float max = 0, min = 10000;

  #if HAS_AUTO_FAN
        unsigned long extruder_autofan_last_check = temp_millis;
  #endif

  if (extruder >= EXTRUDERS
    #if !HAS_TEMP_BED
       || extruder < 0
    #endif
  ) {
    SERIAL_ECHOLN(MSG_PID_BAD_EXTRUDER_NUM);
    return;
  }
  
  SERIAL_ECHOLN(MSG_PID_AUTOTUNE_START);

  disable_heater(); // switch off all heaters.

  if (extruder < 0)
    soft_pwm_bed = bias = d = MAX_BED_POWER / 2;
  else
    soft_pwm[extruder] = bias = d = PID_MAX / 2;

  // PID Tuning loop
  for(;;) {

    unsigned long ms = millis();

    if (temp_meas_ready) { // temp sample ready
      updateTemperaturesFromRawValues();

      input = (extruder<0)?current_temperature_bed:current_temperature[extruder];

      max = max(max, input);
      min = min(min, input);

      #if HAS_AUTO_FAN
        if (ms > extruder_autofan_last_check + 2500) {
          checkExtruderAutoFans();
          extruder_autofan_last_check = ms;
        }
      #endif

      if (heating == true && input > temp) {
        if (ms - t2 > 5000) {
          heating = false;
          if (extruder < 0)
            soft_pwm_bed = (bias - d) >> 1;
          else
            soft_pwm[extruder] = (bias - d) >> 1;
          t1 = ms;
          t_high = t1 - t2;
          max = temp;
        }
      }
      if (heating == false && input < temp) {
        if (ms - t1 > 5000) {
          heating = true;
          t2 = ms;
          t_low = t2 - t1;
          if (cycles > 0) {
            long max_pow = extruder < 0 ? MAX_BED_POWER : PID_MAX;
            bias += (d*(t_high - t_low))/(t_low + t_high);
            bias = constrain(bias, 20, max_pow - 20);
            d = (bias > max_pow / 2) ? max_pow - 1 - bias : bias;

            SERIAL_PROTOCOLPGM(MSG_BIAS); SERIAL_PROTOCOL(bias);
            SERIAL_PROTOCOLPGM(MSG_D);    SERIAL_PROTOCOL(d);
            SERIAL_PROTOCOLPGM(MSG_T_MIN);  SERIAL_PROTOCOL(min);
            SERIAL_PROTOCOLPGM(MSG_T_MAX);  SERIAL_PROTOCOLLN(max);
            if (cycles > 2) {
              Ku = (4.0 * d) / (3.14159265 * (max - min) / 2.0);
              Tu = ((float)(t_low + t_high) / 1000.0);
              SERIAL_PROTOCOLPGM(MSG_KU); SERIAL_PROTOCOL(Ku);
              SERIAL_PROTOCOLPGM(MSG_TU); SERIAL_PROTOCOLLN(Tu);
              Kp = 0.6 * Ku;
              Ki = 2 * Kp / Tu;
              Kd = Kp * Tu / 8;
              SERIAL_PROTOCOLLNPGM(MSG_CLASSIC_PID);
              SERIAL_PROTOCOLPGM(MSG_KP); SERIAL_PROTOCOLLN(Kp);
              SERIAL_PROTOCOLPGM(MSG_KI); SERIAL_PROTOCOLLN(Ki);
              SERIAL_PROTOCOLPGM(MSG_KD); SERIAL_PROTOCOLLN(Kd);
              /*
              Kp = 0.33*Ku;
              Ki = Kp/Tu;
              Kd = Kp*Tu/3;
              SERIAL_PROTOCOLLNPGM(" Some overshoot ");
              SERIAL_PROTOCOLPGM(" Kp: "); SERIAL_PROTOCOLLN(Kp);
              SERIAL_PROTOCOLPGM(" Ki: "); SERIAL_PROTOCOLLN(Ki);
              SERIAL_PROTOCOLPGM(" Kd: "); SERIAL_PROTOCOLLN(Kd);
              Kp = 0.2*Ku;
              Ki = 2*Kp/Tu;
              Kd = Kp*Tu/3;
              SERIAL_PROTOCOLLNPGM(" No overshoot ");
              SERIAL_PROTOCOLPGM(" Kp: "); SERIAL_PROTOCOLLN(Kp);
              SERIAL_PROTOCOLPGM(" Ki: "); SERIAL_PROTOCOLLN(Ki);
              SERIAL_PROTOCOLPGM(" Kd: "); SERIAL_PROTOCOLLN(Kd);
              */
            }
          }
          if (extruder < 0)
            soft_pwm_bed = (bias + d) >> 1;
          else
            soft_pwm[extruder] = (bias + d) >> 1;
          cycles++;
          min = temp;
        }
      } 
    }
    if (input > temp + 20) {
      SERIAL_PROTOCOLLNPGM(MSG_PID_TEMP_TOO_HIGH);
      return;
    }
    // Every 2 seconds...
    if (ms > temp_millis + 2000) {
      int p;
      if (extruder < 0) {
        p = soft_pwm_bed;
        SERIAL_PROTOCOLPGM(MSG_OK_B);
      }
      else {
        p = soft_pwm[extruder];
        SERIAL_PROTOCOLPGM(MSG_OK_T);
      }

      SERIAL_PROTOCOL(input);
      SERIAL_PROTOCOLPGM(MSG_AT);
      SERIAL_PROTOCOLLN(p);

      temp_millis = ms;
    } // every 2 seconds
    // Over 2 minutes?
    if (((ms - t1) + (ms - t2)) > (10L*60L*1000L*2L)) {
      SERIAL_PROTOCOLLNPGM(MSG_PID_TIMEOUT);
      return;
    }
    if (cycles > ncycles) {
      SERIAL_PROTOCOLLNPGM(MSG_PID_AUTOTUNE_FINISHED);
      return;
    }
    lcd_update();
  }
}

void updatePID() {
  #ifdef PIDTEMP
    for (int e = 0; e < EXTRUDERS; e++) {
      temp_iState_max[e] = PID_INTEGRAL_DRIVE_MAX / PID_PARAM(Ki,e);
    }
  #endif
  #ifdef PIDTEMPBED
    temp_iState_max_bed = PID_INTEGRAL_DRIVE_MAX / bedKi;
  #endif
}

int getHeaterPower(int heater) {
  return heater < 0 ? soft_pwm_bed : soft_pwm[heater];
}

#if HAS_AUTO_FAN

void setExtruderAutoFanState(int pin, bool state)
{
  unsigned char newFanSpeed = (state != 0) ? EXTRUDER_AUTO_FAN_SPEED : 0;
  // this idiom allows both digital and PWM fan outputs (see M42 handling).
  pinMode(pin, OUTPUT);
  digitalWrite(pin, newFanSpeed);
  analogWrite(pin, newFanSpeed);
}

void checkExtruderAutoFans()
{
  uint8_t fanState = 0;

  // which fan pins need to be turned on?      
  #if HAS_AUTO_FAN_0
    if (current_temperature[0] > EXTRUDER_AUTO_FAN_TEMPERATURE) 
      fanState |= 1;
  #endif
  #if HAS_AUTO_FAN_1
    if (current_temperature[1] > EXTRUDER_AUTO_FAN_TEMPERATURE) 
    {
      if (EXTRUDER_1_AUTO_FAN_PIN == EXTRUDER_0_AUTO_FAN_PIN)
        fanState |= 1;
      else
        fanState |= 2;
    }
  #endif
  #if HAS_AUTO_FAN_2
    if (current_temperature[2] > EXTRUDER_AUTO_FAN_TEMPERATURE) 
    {
      if (EXTRUDER_2_AUTO_FAN_PIN == EXTRUDER_0_AUTO_FAN_PIN) 
        fanState |= 1;
      else if (EXTRUDER_2_AUTO_FAN_PIN == EXTRUDER_1_AUTO_FAN_PIN) 
        fanState |= 2;
      else
        fanState |= 4;
    }
  #endif
  #if HAS_AUTO_FAN_3
    if (current_temperature[3] > EXTRUDER_AUTO_FAN_TEMPERATURE) 
    {
      if (EXTRUDER_3_AUTO_FAN_PIN == EXTRUDER_0_AUTO_FAN_PIN) 
        fanState |= 1;
      else if (EXTRUDER_3_AUTO_FAN_PIN == EXTRUDER_1_AUTO_FAN_PIN) 
        fanState |= 2;
      else if (EXTRUDER_3_AUTO_FAN_PIN == EXTRUDER_2_AUTO_FAN_PIN) 
        fanState |= 4;
      else
        fanState |= 8;
    }
  #endif
  
  // update extruder auto fan states
  #if HAS_AUTO_FAN_0
    setExtruderAutoFanState(EXTRUDER_0_AUTO_FAN_PIN, (fanState & 1) != 0);
  #endif 
  #if HAS_AUTO_FAN_1
    if (EXTRUDER_1_AUTO_FAN_PIN != EXTRUDER_0_AUTO_FAN_PIN)
      setExtruderAutoFanState(EXTRUDER_1_AUTO_FAN_PIN, (fanState & 2) != 0);
  #endif 
  #if HAS_AUTO_FAN_2
    if (EXTRUDER_2_AUTO_FAN_PIN != EXTRUDER_0_AUTO_FAN_PIN
        && EXTRUDER_2_AUTO_FAN_PIN != EXTRUDER_1_AUTO_FAN_PIN)
      setExtruderAutoFanState(EXTRUDER_2_AUTO_FAN_PIN, (fanState & 4) != 0);
  #endif
  #if HAS_AUTO_FAN_3
    if (EXTRUDER_3_AUTO_FAN_PIN != EXTRUDER_0_AUTO_FAN_PIN
        && EXTRUDER_3_AUTO_FAN_PIN != EXTRUDER_1_AUTO_FAN_PIN
        && EXTRUDER_3_AUTO_FAN_PIN != EXTRUDER_2_AUTO_FAN_PIN)
      setExtruderAutoFanState(EXTRUDER_3_AUTO_FAN_PIN, (fanState & 8) != 0);
  #endif
}

#endif // any extruder auto fan pins set

//
// Temperature Error Handlers
//
inline void _temp_error(int e, const char *msg1, const char *msg2) {
  if (!IsStopped()) {
    SERIAL_ERROR_START;
    if (e >= 0) SERIAL_ERRORLN((int)e);
    serialprintPGM(msg1);
    MYSERIAL.write('\n');
    #ifdef ULTRA_LCD
      lcd_setalertstatuspgm(msg2);
    #endif
  }
  #ifndef BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE
    Stop();
  #endif
}

void max_temp_error(uint8_t e) {
  disable_heater();
  _temp_error(e, PSTR(MSG_MAXTEMP_EXTRUDER_OFF), PSTR(MSG_ERR_MAXTEMP));
}
void min_temp_error(uint8_t e) {
  disable_heater();
  _temp_error(e, PSTR(MSG_MINTEMP_EXTRUDER_OFF), PSTR(MSG_ERR_MINTEMP));
}
void bed_max_temp_error(void) {
  #if HAS_HEATER_BED
    WRITE_HEATER_BED(0);
  #endif
  _temp_error(-1, PSTR(MSG_MAXTEMP_BED_OFF), PSTR(MSG_ERR_MAXTEMP_BED));
}

float get_pid_output(int e) {
  float pid_output;
  #ifdef PIDTEMP
    #ifndef PID_OPENLOOP
      pid_error[e] = target_temperature[e] - current_temperature[e];
      if (pid_error[e] > PID_FUNCTIONAL_RANGE) {
        pid_output = BANG_MAX;
        pid_reset[e] = true;
      }
      else if (pid_error[e] < -PID_FUNCTIONAL_RANGE || target_temperature[e] == 0) {
        pid_output = 0;
        pid_reset[e] = true;
      }
      else {
        if (pid_reset[e]) {
          temp_iState[e] = 0.0;
          pid_reset[e] = false;
        }
        pTerm[e] = PID_PARAM(Kp,e) * pid_error[e];
        temp_iState[e] += pid_error[e];
        temp_iState[e] = constrain(temp_iState[e], temp_iState_min[e], temp_iState_max[e]);
        iTerm[e] = PID_PARAM(Ki,e) * temp_iState[e];

        dTerm[e] = K2 * PID_PARAM(Kd,e) * (current_temperature[e] - temp_dState[e]) + K1 * dTerm[e];
        pid_output = pTerm[e] + iTerm[e] - dTerm[e];
        if (pid_output > PID_MAX) {
          if (pid_error[e] > 0) temp_iState[e] -= pid_error[e]; // conditional un-integration
          pid_output = PID_MAX;
        }
        else if (pid_output < 0) {
          if (pid_error[e] < 0) temp_iState[e] -= pid_error[e]; // conditional un-integration
          pid_output = 0;
        }
      }
      temp_dState[e] = current_temperature[e];
    #else
      pid_output = constrain(target_temperature[e], 0, PID_MAX);
    #endif //PID_OPENLOOP

    #ifdef PID_DEBUG
      SERIAL_ECHO_START;
      SERIAL_ECHO(MSG_PID_DEBUG);
      SERIAL_ECHO(e);
      SERIAL_ECHO(MSG_PID_DEBUG_INPUT);
      SERIAL_ECHO(current_temperature[e]);
      SERIAL_ECHO(MSG_PID_DEBUG_OUTPUT);
      SERIAL_ECHO(pid_output);
      SERIAL_ECHO(MSG_PID_DEBUG_PTERM);
      SERIAL_ECHO(pTerm[e]);
      SERIAL_ECHO(MSG_PID_DEBUG_ITERM);
      SERIAL_ECHO(iTerm[e]);
      SERIAL_ECHO(MSG_PID_DEBUG_DTERM);
      SERIAL_ECHOLN(dTerm[e]);
    #endif //PID_DEBUG

  #else /* PID off */
    pid_output = (current_temperature[e] < target_temperature[e]) ? PID_MAX : 0;
  #endif

  return pid_output;
}

#ifdef PIDTEMPBED
  float get_pid_output_bed() {
    float pid_output;
    #ifndef PID_OPENLOOP
      pid_error_bed = target_temperature_bed - current_temperature_bed;
      pTerm_bed = bedKp * pid_error_bed;
      temp_iState_bed += pid_error_bed;
      temp_iState_bed = constrain(temp_iState_bed, temp_iState_min_bed, temp_iState_max_bed);
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

    #ifdef PID_BED_DEBUG
      SERIAL_ECHO_START;
      SERIAL_ECHO(" PID_BED_DEBUG ");
      SERIAL_ECHO(": Input ");
      SERIAL_ECHO(current_temperature_bed);
      SERIAL_ECHO(" Output ");
      SERIAL_ECHO(pid_output);
      SERIAL_ECHO(" pTerm ");
      SERIAL_ECHO(pTerm_bed);
      SERIAL_ECHO(" iTerm ");
      SERIAL_ECHO(iTerm_bed);
      SERIAL_ECHO(" dTerm ");
      SERIAL_ECHOLN(dTerm_bed);
    #endif //PID_BED_DEBUG

    return pid_output;
  }
#endif

void manage_heater() {

  if (!temp_meas_ready) return;

  updateTemperaturesFromRawValues();

  #ifdef HEATER_0_USES_MAX6675
    float ct = current_temperature[0];
    if (ct > min(HEATER_0_MAXTEMP, 1023)) max_temp_error(0);
    if (ct < max(HEATER_0_MINTEMP, 0.01)) min_temp_error(0);
  #endif //HEATER_0_USES_MAX6675

  #if defined(WATCH_TEMP_PERIOD) || !defined(PIDTEMPBED) || HAS_AUTO_FAN
    unsigned long ms = millis();
  #endif

  // Loop through all extruders
  for (int e = 0; e < EXTRUDERS; e++) {

    #if defined (THERMAL_RUNAWAY_PROTECTION_PERIOD) && THERMAL_RUNAWAY_PROTECTION_PERIOD > 0
      thermal_runaway_protection(&thermal_runaway_state_machine[e], &thermal_runaway_timer[e], current_temperature[e], target_temperature[e], e, THERMAL_RUNAWAY_PROTECTION_PERIOD, THERMAL_RUNAWAY_PROTECTION_HYSTERESIS);
    #endif

    float pid_output = get_pid_output(e);

    // Check if temperature is within the correct range
    soft_pwm[e] = current_temperature[e] > minttemp[e] && current_temperature[e] < maxttemp[e] ? (int)pid_output >> 1 : 0;

    #ifdef WATCH_TEMP_PERIOD
      if (watchmillis[e] && ms > watchmillis[e] + WATCH_TEMP_PERIOD) {
        if (degHotend(e) < watch_start_temp[e] + WATCH_TEMP_INCREASE) {
          setTargetHotend(0, e);
          LCD_MESSAGEPGM(MSG_HEATING_FAILED_LCD); // translatable
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM(MSG_HEATING_FAILED);
        }
        else {
          watchmillis[e] = 0;
        }
      }
    #endif //WATCH_TEMP_PERIOD

    #ifdef TEMP_SENSOR_1_AS_REDUNDANT
      if (fabs(current_temperature[0] - redundant_temperature) > MAX_REDUNDANT_TEMP_SENSOR_DIFF) {
        disable_heater();
        _temp_error(0, PSTR(MSG_EXTRUDER_SWITCHED_OFF), PSTR(MSG_ERR_REDUNDANT_TEMP));
      }
    #endif //TEMP_SENSOR_1_AS_REDUNDANT

  } // Extruders Loop

  #if HAS_AUTO_FAN
    if (ms > extruder_autofan_last_check + 2500) { // only need to check fan state very infrequently
      checkExtruderAutoFans();
      extruder_autofan_last_check = ms;
    }
  #endif       
  
  #ifndef PIDTEMPBED
    if (ms < previous_millis_bed_heater + BED_CHECK_INTERVAL) return;
    previous_millis_bed_heater = ms;
  #endif //PIDTEMPBED

  #if TEMP_SENSOR_BED != 0
  
    #if defined(THERMAL_RUNAWAY_PROTECTION_BED_PERIOD) && THERMAL_RUNAWAY_PROTECTION_BED_PERIOD > 0
      thermal_runaway_protection(&thermal_runaway_bed_state_machine, &thermal_runaway_bed_timer, current_temperature_bed, target_temperature_bed, 9, THERMAL_RUNAWAY_PROTECTION_BED_PERIOD, THERMAL_RUNAWAY_PROTECTION_BED_HYSTERESIS);
    #endif

    #ifdef PIDTEMPBED
      float pid_output = get_pid_output_bed();

      soft_pwm_bed = current_temperature_bed > BED_MINTEMP && current_temperature_bed < BED_MAXTEMP ? (int)pid_output >> 1 : 0;

    #elif !defined(BED_LIMIT_SWITCHING)
      // Check if temperature is within the correct range
      if (current_temperature_bed > BED_MINTEMP && current_temperature_bed < BED_MAXTEMP) {
        soft_pwm_bed = current_temperature_bed < target_temperature_bed ? MAX_BED_POWER >> 1 : 0;
      }
      else {
        soft_pwm_bed = 0;
        WRITE_HEATER_BED(LOW);
      }
    #else //#ifdef BED_LIMIT_SWITCHING
      // Check if temperature is within the correct band
      if (current_temperature_bed > BED_MINTEMP && current_temperature_bed < BED_MAXTEMP) {
        if (current_temperature_bed >= target_temperature_bed + BED_HYSTERESIS)
          soft_pwm_bed = 0;
        else if (current_temperature_bed <= target_temperature_bed - BED_HYSTERESIS)
          soft_pwm_bed = MAX_BED_POWER >> 1;
      }
      else {
        soft_pwm_bed = 0;
        WRITE_HEATER_BED(LOW);
      }
    #endif
  #endif //TEMP_SENSOR_BED != 0
  
  // Control the extruder rate based on the width sensor
  #ifdef FILAMENT_SENSOR
    if (filament_sensor) {
      meas_shift_index = delay_index1 - meas_delay_cm;
      if (meas_shift_index < 0) meas_shift_index += MAX_MEASUREMENT_DELAY + 1;  //loop around buffer if needed
      
      // Get the delayed info and add 100 to reconstitute to a percent of
      // the nominal filament diameter then square it to get an area
      meas_shift_index = constrain(meas_shift_index, 0, MAX_MEASUREMENT_DELAY);
      float vm = pow((measurement_delay[meas_shift_index] + 100.0) / 100.0, 2);
      if (vm < 0.01) vm = 0.01;
      volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM] = vm;
    }
  #endif //FILAMENT_SENSOR
}

#define PGM_RD_W(x)   (short)pgm_read_word(&x)
// Derived from RepRap FiveD extruder::getTemperature()
// For hot end temperature measurement.
static float analog2temp(int raw, uint8_t e) {
#ifdef TEMP_SENSOR_1_AS_REDUNDANT
  if (e > EXTRUDERS)
#else
  if (e >= EXTRUDERS)
#endif
  {
      SERIAL_ERROR_START;
      SERIAL_ERROR((int)e);
      SERIAL_ERRORLNPGM(MSG_INVALID_EXTRUDER_NUM);
      kill();
      return 0.0;
  } 
  #ifdef HEATER_0_USES_MAX6675
    if (e == 0)
    {
      return 0.25 * raw;
    }
  #endif

  if(heater_ttbl_map[e] != NULL)
  {
    float celsius = 0;
    uint8_t i;
    short (*tt)[][2] = (short (*)[][2])(heater_ttbl_map[e]);

    for (i=1; i<heater_ttbllen_map[e]; i++)
    {
      if (PGM_RD_W((*tt)[i][0]) > raw)
      {
        celsius = PGM_RD_W((*tt)[i-1][1]) + 
          (raw - PGM_RD_W((*tt)[i-1][0])) * 
          (float)(PGM_RD_W((*tt)[i][1]) - PGM_RD_W((*tt)[i-1][1])) /
          (float)(PGM_RD_W((*tt)[i][0]) - PGM_RD_W((*tt)[i-1][0]));
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == heater_ttbllen_map[e]) celsius = PGM_RD_W((*tt)[i-1][1]);

    return celsius;
  }
  return ((raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR) * TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET;
}

// Derived from RepRap FiveD extruder::getTemperature()
// For bed temperature measurement.
static float analog2tempBed(int raw) {
  #ifdef BED_USES_THERMISTOR
    float celsius = 0;
    byte i;

    for (i=1; i<BEDTEMPTABLE_LEN; i++)
    {
      if (PGM_RD_W(BEDTEMPTABLE[i][0]) > raw)
      {
        celsius  = PGM_RD_W(BEDTEMPTABLE[i-1][1]) + 
          (raw - PGM_RD_W(BEDTEMPTABLE[i-1][0])) * 
          (float)(PGM_RD_W(BEDTEMPTABLE[i][1]) - PGM_RD_W(BEDTEMPTABLE[i-1][1])) /
          (float)(PGM_RD_W(BEDTEMPTABLE[i][0]) - PGM_RD_W(BEDTEMPTABLE[i-1][0]));
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == BEDTEMPTABLE_LEN) celsius = PGM_RD_W(BEDTEMPTABLE[i-1][1]);

    return celsius;
  #elif defined BED_USES_AD595
    return ((raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR) * TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET;
  #else
    return 0;
  #endif
}

/* Called to get the raw values into the the actual temperatures. The raw values are created in interrupt context,
    and this function is called from normal context as it is too slow to run in interrupts and will block the stepper routine otherwise */
static void updateTemperaturesFromRawValues() {
  #ifdef HEATER_0_USES_MAX6675
    current_temperature_raw[0] = read_max6675();
  #endif
  for(uint8_t e = 0; e < EXTRUDERS; e++) {
    current_temperature[e] = analog2temp(current_temperature_raw[e], e);
  }
  current_temperature_bed = analog2tempBed(current_temperature_bed_raw);
  #ifdef TEMP_SENSOR_1_AS_REDUNDANT
    redundant_temperature = analog2temp(redundant_temperature_raw, 1);
  #endif
  #if HAS_FILAMENT_SENSOR
    filament_width_meas = analog2widthFil();
  #endif
  //Reset the watchdog after we know we have a temperature measurement.
  watchdog_reset();

  CRITICAL_SECTION_START;
  temp_meas_ready = false;
  CRITICAL_SECTION_END;
}


#ifdef FILAMENT_SENSOR

  // Convert raw Filament Width to millimeters
  float analog2widthFil() {
    return current_raw_filwidth / 16383.0 * 5.0;
    //return current_raw_filwidth;
  }

  // Convert raw Filament Width to a ratio
  int widthFil_to_size_ratio() {
    float temp = filament_width_meas;
    if (temp < MEASURED_LOWER_LIMIT) temp = filament_width_nominal;  //assume sensor cut out
    else if (temp > MEASURED_UPPER_LIMIT) temp = MEASURED_UPPER_LIMIT;
    return filament_width_nominal / temp * 100;
  } 

#endif





void tp_init()
{
  #if MB(RUMBA) && ((TEMP_SENSOR_0==-1)||(TEMP_SENSOR_1==-1)||(TEMP_SENSOR_2==-1)||(TEMP_SENSOR_BED==-1))
    //disable RUMBA JTAG in case the thermocouple extension is plugged on top of JTAG connector
    MCUCR=BIT(JTD);
    MCUCR=BIT(JTD);
  #endif
  
  // Finish init of mult extruder arrays 
  for (int e = 0; e < EXTRUDERS; e++) {
    // populate with the first value 
    maxttemp[e] = maxttemp[0];
    #ifdef PIDTEMP
      temp_iState_min[e] = 0.0;
      temp_iState_max[e] = PID_INTEGRAL_DRIVE_MAX / PID_PARAM(Ki,e);
    #endif //PIDTEMP
    #ifdef PIDTEMPBED
      temp_iState_min_bed = 0.0;
      temp_iState_max_bed = PID_INTEGRAL_DRIVE_MAX / bedKi;
    #endif //PIDTEMPBED
  }

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
  #if HAS_FAN
    SET_OUTPUT(FAN_PIN);
    #ifdef FAST_PWM_FAN
      setPwmFrequency(FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
    #ifdef FAN_SOFT_PWM
      soft_pwm_fan = fanSpeedSoftPwm / 2;
    #endif
  #endif

  #ifdef HEATER_0_USES_MAX6675

    #ifndef SDSUPPORT
      OUT_WRITE(SCK_PIN, LOW);
      OUT_WRITE(MOSI_PIN, HIGH);
      OUT_WRITE(MISO_PIN, HIGH);
    #else
      pinMode(SS_PIN, OUTPUT);
      digitalWrite(SS_PIN, HIGH);
    #endif
    
    OUT_WRITE(MAX6675_SS,HIGH);

  #endif //HEATER_0_USES_MAX6675

  #ifdef DIDR2
    #define ANALOG_SELECT(pin) do{ if (pin < 8) DIDR0 |= BIT(pin); else DIDR2 |= BIT(pin - 8); }while(0)
  #else
    #define ANALOG_SELECT(pin) do{ DIDR0 |= BIT(pin); }while(0)
  #endif

  // Set analog inputs
  ADCSRA = BIT(ADEN) | BIT(ADSC) | BIT(ADIF) | 0x07;
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
  #if HAS_FILAMENT_SENSOR
    ANALOG_SELECT(FILWIDTH_PIN);
  #endif
  
  // Use timer0 for temperature measurement
  // Interleave temperature interrupt with millies interrupt
  OCR0B = 128;
  TIMSK0 |= BIT(OCIE0B);  
  
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
  #if EXTRUDERS > 1
    #ifdef HEATER_1_MINTEMP
      TEMP_MIN_ROUTINE(1);
    #endif
    #ifdef HEATER_1_MAXTEMP
      TEMP_MAX_ROUTINE(1);
    #endif
    #if EXTRUDERS > 2
      #ifdef HEATER_2_MINTEMP
        TEMP_MIN_ROUTINE(2);
      #endif
      #ifdef HEATER_2_MAXTEMP
        TEMP_MAX_ROUTINE(2);
      #endif
      #if EXTRUDERS > 3
        #ifdef HEATER_3_MINTEMP
          TEMP_MIN_ROUTINE(3);
        #endif
        #ifdef HEATER_3_MAXTEMP
          TEMP_MAX_ROUTINE(3);
        #endif
      #endif // EXTRUDERS > 3
    #endif // EXTRUDERS > 2
  #endif // EXTRUDERS > 1

  #ifdef BED_MINTEMP
    /* No bed MINTEMP error implemented?!? */ /*
    while(analog2tempBed(bed_minttemp_raw) < BED_MINTEMP) {
      #if HEATER_BED_RAW_LO_TEMP < HEATER_BED_RAW_HI_TEMP
        bed_minttemp_raw += OVERSAMPLENR;
      #else
        bed_minttemp_raw -= OVERSAMPLENR;
      #endif
    }
    */
  #endif //BED_MINTEMP
  #ifdef BED_MAXTEMP
    while(analog2tempBed(bed_maxttemp_raw) > BED_MAXTEMP) {
      #if HEATER_BED_RAW_LO_TEMP < HEATER_BED_RAW_HI_TEMP
        bed_maxttemp_raw -= OVERSAMPLENR;
      #else
        bed_maxttemp_raw += OVERSAMPLENR;
      #endif
    }
  #endif //BED_MAXTEMP
}

void setWatch() {
  #ifdef WATCH_TEMP_PERIOD
    unsigned long ms = millis();
    for (int e = 0; e < EXTRUDERS; e++) {
      if (degHotend(e) < degTargetHotend(e) - (WATCH_TEMP_INCREASE * 2)) {
        watch_start_temp[e] = degHotend(e);
        watchmillis[e] = ms;
      } 
    }
  #endif 
}

#if defined(THERMAL_RUNAWAY_PROTECTION_PERIOD) && THERMAL_RUNAWAY_PROTECTION_PERIOD > 0
void thermal_runaway_protection(int *state, unsigned long *timer, float temperature, float target_temperature, int heater_id, int period_seconds, int hysteresis_degc)
{
/*
      SERIAL_ECHO_START;
      SERIAL_ECHO("Thermal Thermal Runaway Running. Heater ID:");
      SERIAL_ECHO(heater_id);
      SERIAL_ECHO(" ;  State:");
      SERIAL_ECHO(*state);
      SERIAL_ECHO(" ;  Timer:");
      SERIAL_ECHO(*timer);
      SERIAL_ECHO(" ;  Temperature:");
      SERIAL_ECHO(temperature);
      SERIAL_ECHO(" ;  Target Temp:");
      SERIAL_ECHO(target_temperature);
      SERIAL_ECHOLN("");    
*/
  if ((target_temperature == 0) || thermal_runaway)
  {
    *state = 0;
    *timer = 0;
    return;
  }
  switch (*state)
  {
    case 0: // "Heater Inactive" state
      if (target_temperature > 0) *state = 1;
      break;
    case 1: // "First Heating" state
      if (temperature >= target_temperature) *state = 2;
      break;
    case 2: // "Temperature Stable" state
    {
      unsigned long ms = millis();
      if (temperature >= (target_temperature - hysteresis_degc))
      {
        *timer = ms;
      } 
      else if ( (ms - *timer) > ((unsigned long) period_seconds) * 1000)
      {
        SERIAL_ERROR_START;
        SERIAL_ERRORLNPGM(MSG_THERMAL_RUNAWAY_STOP);
        SERIAL_ERRORLN((int)heater_id);
        LCD_ALERTMESSAGEPGM(MSG_THERMAL_RUNAWAY); // translatable
        thermal_runaway = true;
        while(1)
        {
          disable_heater();
          disable_x();
          disable_y();
          disable_z();
          disable_e0();
          disable_e1();
          disable_e2();
          disable_e3();
          manage_heater();
          lcd_update();
        }
      }
    } break;
  }
}
#endif //THERMAL_RUNAWAY_PROTECTION_PERIOD


void disable_heater() {
  for (int i=0; i<EXTRUDERS; i++) setTargetHotend(0, i);
  setTargetBed(0);

  #define DISABLE_HEATER(NR) { \
    target_temperature[NR] = 0; \
    soft_pwm[NR] = 0; \
    WRITE_HEATER_ ## NR (LOW); \
  }

  #if HAS_TEMP_0
    target_temperature[0] = 0;
    soft_pwm[0] = 0;
    WRITE_HEATER_0P(LOW); // Should HEATERS_PARALLEL apply here? Then change to DISABLE_HEATER(0)
  #endif

  #if EXTRUDERS > 1 && HAS_TEMP_1
    DISABLE_HEATER(1);
  #endif

  #if EXTRUDERS > 2 && HAS_TEMP_2
    DISABLE_HEATER(2);
  #endif

  #if EXTRUDERS > 3 && HAS_TEMP_3
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

#ifdef HEATER_0_USES_MAX6675
  #define MAX6675_HEAT_INTERVAL 250
  long max6675_previous_millis = MAX6675_HEAT_INTERVAL;
  int max6675_temp = 2000;

  static int read_max6675() {

    unsigned long ms = millis();
    if (ms < max6675_previous_millis + MAX6675_HEAT_INTERVAL)
      return max6675_temp;
    
    max6675_previous_millis = ms;
    max6675_temp = 0;

    #ifdef PRR
      PRR &= ~BIT(PRSPI);
    #elif defined(PRR0)
      PRR0 &= ~BIT(PRSPI);
    #endif

    SPCR = BIT(MSTR) | BIT(SPE) | BIT(SPR0);

    // enable TT_MAX6675
    WRITE(MAX6675_SS, 0);

    // ensure 100ns delay - a bit extra is fine
    asm("nop");//50ns on 20Mhz, 62.5ns on 16Mhz
    asm("nop");//50ns on 20Mhz, 62.5ns on 16Mhz

    // read MSB
    SPDR = 0;
    for (;(SPSR & BIT(SPIF)) == 0;);
    max6675_temp = SPDR;
    max6675_temp <<= 8;

    // read LSB
    SPDR = 0;
    for (;(SPSR & BIT(SPIF)) == 0;);
    max6675_temp |= SPDR;

    // disable TT_MAX6675
    WRITE(MAX6675_SS, 1);

    if (max6675_temp & 4) {
      // thermocouple open
      max6675_temp = 4000;
    }
    else {
      max6675_temp = max6675_temp >> 3;
    }

    return max6675_temp;
  }

#endif //HEATER_0_USES_MAX6675

/**
 * Stages in the ISR loop
 */
enum TempState {
  PrepareTemp_0,
  MeasureTemp_0,
  PrepareTemp_BED,
  MeasureTemp_BED,
  PrepareTemp_1,
  MeasureTemp_1,
  PrepareTemp_2,
  MeasureTemp_2,
  PrepareTemp_3,
  MeasureTemp_3,
  Prepare_FILWIDTH,
  Measure_FILWIDTH,
  StartupDelay // Startup, delay initial temp reading a tiny bit so the hardware can settle
};

static unsigned long raw_temp_value[4] = { 0 };
static unsigned long raw_temp_bed_value = 0;

static void set_current_temp_raw() {
  #if HAS_TEMP_0 && !defined(HEATER_0_USES_MAX6675)
    current_temperature_raw[0] = raw_temp_value[0];
  #endif
  #if HAS_TEMP_1
    #ifdef TEMP_SENSOR_1_AS_REDUNDANT
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

//
// Timer 0 is shared with millies
//
ISR(TIMER0_COMPB_vect) {
  //these variables are only accesible from the ISR, but static, so they don't lose their value
  static unsigned char temp_count = 0;
  static TempState temp_state = StartupDelay;
  static unsigned char pwm_count = BIT(SOFT_PWM_SCALE);

  // Static members for each heater
  #ifdef SLOW_PWM_HEATERS
    static unsigned char slow_pwm_count = 0;
    #define ISR_STATICS(n) \
      static unsigned char soft_pwm_ ## n; \
      static unsigned char state_heater_ ## n = 0; \
      static unsigned char state_timer_heater_ ## n = 0
  #else
    #define ISR_STATICS(n) static unsigned char soft_pwm_ ## n
  #endif 

  // Statics per heater
  ISR_STATICS(0);
  #if (EXTRUDERS > 1) || defined(HEATERS_PARALLEL)
    ISR_STATICS(1);
    #if EXTRUDERS > 2
      ISR_STATICS(2);
      #if EXTRUDERS > 3
        ISR_STATICS(3);
      #endif
    #endif
  #endif
  #if HAS_HEATER_BED
    ISR_STATICS(BED);
  #endif

  #if HAS_FILAMENT_SENSOR
    static unsigned long raw_filwidth_value = 0;
  #endif
  
  #ifndef SLOW_PWM_HEATERS
    /**
     * standard PWM modulation
     */
    if (pwm_count == 0) {
      soft_pwm_0 = soft_pwm[0];
      if (soft_pwm_0 > 0) {
        WRITE_HEATER_0(1);
      }
      else WRITE_HEATER_0P(0); // If HEATERS_PARALLEL should apply, change to WRITE_HEATER_0

      #if EXTRUDERS > 1
        soft_pwm_1 = soft_pwm[1];
        WRITE_HEATER_1(soft_pwm_1 > 0 ? 1 : 0);
        #if EXTRUDERS > 2
          soft_pwm_2 = soft_pwm[2];
          WRITE_HEATER_2(soft_pwm_2 > 0 ? 1 : 0);
          #if EXTRUDERS > 3
            soft_pwm_3 = soft_pwm[3];
            WRITE_HEATER_3(soft_pwm_3 > 0 ? 1 : 0);
          #endif
        #endif
      #endif

      #if HAS_HEATER_BED
        soft_pwm_BED = soft_pwm_bed;
        WRITE_HEATER_BED(soft_pwm_BED > 0 ? 1 : 0);
      #endif
      #ifdef FAN_SOFT_PWM
        soft_pwm_fan = fanSpeedSoftPwm / 2;
        WRITE_FAN(soft_pwm_fan > 0 ? 1 : 0);
      #endif
    }

    if (soft_pwm_0 < pwm_count) { WRITE_HEATER_0(0); }
    #if EXTRUDERS > 1
      if (soft_pwm_1 < pwm_count) WRITE_HEATER_1(0);
      #if EXTRUDERS > 2
        if (soft_pwm_2 < pwm_count) WRITE_HEATER_2(0);
        #if EXTRUDERS > 3
          if (soft_pwm_3 < pwm_count) WRITE_HEATER_3(0);
        #endif
      #endif
    #endif

    #if HAS_HEATER_BED
      if (soft_pwm_BED < pwm_count) WRITE_HEATER_BED(0);
    #endif

    #ifdef FAN_SOFT_PWM
      if (soft_pwm_fan < pwm_count) WRITE_FAN(0);
    #endif
    
    pwm_count += BIT(SOFT_PWM_SCALE);
    pwm_count &= 0x7f;
  
  #else // SLOW_PWM_HEATERS
    /*
     * SLOW PWM HEATERS
     *
     * for heaters drived by relay
     */
    #ifndef MIN_STATE_TIME
      #define MIN_STATE_TIME 16 // MIN_STATE_TIME * 65.5 = time in milliseconds
    #endif

    // Macros for Slow PWM timer logic - HEATERS_PARALLEL applies
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
      #if EXTRUDERS > 1
        SLOW_PWM_ROUTINE(1); // EXTRUDER 1
        #if EXTRUDERS > 2
          SLOW_PWM_ROUTINE(2); // EXTRUDER 2
          #if EXTRUDERS > 3
            SLOW_PWM_ROUTINE(3); // EXTRUDER 3
          #endif
        #endif
      #endif
      #if HAS_HEATER_BED
        _SLOW_PWM_ROUTINE(BED, soft_pwm_bed); // BED
      #endif

    } // slow_pwm_count == 0

    PWM_OFF_ROUTINE(0); // EXTRUDER 0
    #if EXTRUDERS > 1
      PWM_OFF_ROUTINE(1); // EXTRUDER 1
      #if EXTRUDERS > 2
        PWM_OFF_ROUTINE(2); // EXTRUDER 2
        #if EXTRUDERS > 3
          PWM_OFF_ROUTINE(3); // EXTRUDER 3
        #endif
      #endif
    #endif
    #if HAS_HEATER_BED
      PWM_OFF_ROUTINE(BED); // BED
    #endif

    #ifdef FAN_SOFT_PWM
      if (pwm_count == 0) {
        soft_pwm_fan = fanSpeedSoftPwm / 2;
        WRITE_FAN(soft_pwm_fan > 0 ? 1 : 0);
      }
      if (soft_pwm_fan < pwm_count) WRITE_FAN(0);
    #endif //FAN_SOFT_PWM

    pwm_count += BIT(SOFT_PWM_SCALE);
    pwm_count &= 0x7f;

    // increment slow_pwm_count only every 64 pwm_count circa 65.5ms
    if ((pwm_count % 64) == 0) {
      slow_pwm_count++;
      slow_pwm_count &= 0x7f;
    
      // EXTRUDER 0
      if (state_timer_heater_0 > 0) state_timer_heater_0--; 
      #if EXTRUDERS > 1    // EXTRUDER 1
        if (state_timer_heater_1 > 0) state_timer_heater_1--;
        #if EXTRUDERS > 2    // EXTRUDER 2
          if (state_timer_heater_2 > 0) state_timer_heater_2--;
          #if EXTRUDERS > 3    // EXTRUDER 3
            if (state_timer_heater_3 > 0) state_timer_heater_3--;
          #endif
        #endif
      #endif
      #if HAS_HEATER_BED
        if (state_timer_heater_BED > 0) state_timer_heater_BED--;
      #endif
    } // (pwm_count % 64) == 0
  
  #endif // SLOW_PWM_HEATERS

  #define SET_ADMUX_ADCSRA(pin) ADMUX = BIT(REFS0) | (pin & 0x07); ADCSRA |= BIT(ADSC)
  #ifdef MUX5
    #define START_ADC(pin) if (pin > 7) ADCSRB = BIT(MUX5); else ADCSRB = 0; SET_ADMUX_ADCSRA(pin)
  #else
    #define START_ADC(pin) ADCSRB = 0; SET_ADMUX_ADCSRA(pin)
  #endif

  switch(temp_state) {
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
      #if HAS_FILAMENT_SENSOR
        START_ADC(FILWIDTH_PIN);
      #endif
      lcd_buttons_update();
      temp_state = Measure_FILWIDTH;
      break;
    case Measure_FILWIDTH:
      #if HAS_FILAMENT_SENSOR
        // raw_filwidth_value += ADC;  //remove to use an IIR filter approach
        if (ADC > 102) { //check that ADC is reading a voltage > 0.5 volts, otherwise don't take in the data.
          raw_filwidth_value -= (raw_filwidth_value>>7);  //multiply raw_filwidth_value by 127/128
          raw_filwidth_value += ((unsigned long)ADC<<7);  //add new ADC reading
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
    #if HAS_FILAMENT_SENSOR
      current_raw_filwidth = raw_filwidth_value >> 10;  // Divide to get to 0-16384 range since we used 1/128 IIR filter approach
    #endif

    temp_count = 0;
    for (int i = 0; i < 4; i++) raw_temp_value[i] = 0;
    raw_temp_bed_value = 0;

    #ifndef HEATER_0_USES_MAX6675
      #if HEATER_0_RAW_LO_TEMP > HEATER_0_RAW_HI_TEMP
        #define GE0 <=
      #else
        #define GE0 >=
      #endif
      if (current_temperature_raw[0] GE0 maxttemp_raw[0]) max_temp_error(0);
      if (minttemp_raw[0] GE0 current_temperature_raw[0]) min_temp_error(0);
    #endif

    #if EXTRUDERS > 1
      #if HEATER_1_RAW_LO_TEMP > HEATER_1_RAW_HI_TEMP
        #define GE1 <=
      #else
        #define GE1 >=
      #endif
      if (current_temperature_raw[1] GE1 maxttemp_raw[1]) max_temp_error(1);
      if (minttemp_raw[1] GE0 current_temperature_raw[1]) min_temp_error(1);

      #if EXTRUDERS > 2
        #if HEATER_2_RAW_LO_TEMP > HEATER_2_RAW_HI_TEMP
          #define GE2 <=
        #else
          #define GE2 >=
        #endif
        if (current_temperature_raw[2] GE2 maxttemp_raw[2]) max_temp_error(2);
        if (minttemp_raw[2] GE0 current_temperature_raw[2]) min_temp_error(2);

        #if EXTRUDERS > 3
          #if HEATER_3_RAW_LO_TEMP > HEATER_3_RAW_HI_TEMP
            #define GE3 <=
          #else
            #define GE3 >=
          #endif
          if (current_temperature_raw[3] GE3 maxttemp_raw[3]) max_temp_error(3);
          if (minttemp_raw[3] GE0 current_temperature_raw[3]) min_temp_error(3);

        #endif // EXTRUDERS > 3
      #endif // EXTRUDERS > 2
    #endif // EXTRUDERS > 1

    #if defined(BED_MAXTEMP) && (TEMP_SENSOR_BED != 0)
      #if HEATER_BED_RAW_LO_TEMP > HEATER_BED_RAW_HI_TEMP
        #define GEBED <=
      #else
        #define GEBED >=
      #endif
      if (current_temperature_bed_raw GEBED bed_maxttemp_raw) {
        target_temperature_bed = 0;
        bed_max_temp_error();
      }
    #endif

  } // temp_count >= OVERSAMPLENR

  #ifdef BABYSTEPPING
    for (uint8_t axis=X_AXIS; axis<=Z_AXIS; axis++) {
      int curTodo=babystepsTodo[axis]; //get rid of volatile for performance
     
      if (curTodo > 0) {
        babystep(axis,/*fwd*/true);
        babystepsTodo[axis]--; //less to do next time
      }
      else if(curTodo < 0) {
        babystep(axis,/*fwd*/false);
        babystepsTodo[axis]++; //less to do next time
      }
    }
  #endif //BABYSTEPPING
}

#ifdef PIDTEMP
  // Apply the scale factors to the PID values
  float scalePID_i(float i)   { return i * PID_dT; }
  float unscalePID_i(float i) { return i / PID_dT; }
  float scalePID_d(float d)   { return d / PID_dT; }
  float unscalePID_d(float d) { return d * PID_dT; }
#endif //PIDTEMP
