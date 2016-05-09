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
 * temperature.h - temperature controller
 */

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "Marlin.h"
#include "planner.h"

#if ENABLED(PID_ADD_EXTRUSION_RATE)
  #include "stepper.h"
#endif

#ifndef SOFT_PWM_SCALE
  #define SOFT_PWM_SCALE 0
#endif

class Temperature {

  public:

    int current_temperature_raw[EXTRUDERS] = { 0 };
    float current_temperature[EXTRUDERS] = { 0.0 };
    int target_temperature[EXTRUDERS] = { 0 };

    int current_temperature_bed_raw = 0;
    float current_temperature_bed = 0.0;
    int target_temperature_bed = 0;

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      float redundant_temperature = 0.0;
    #endif

    unsigned char soft_pwm_bed;

    #if ENABLED(FAN_SOFT_PWM)
      unsigned char fanSpeedSoftPwm[FAN_COUNT];
    #endif

    #if ENABLED(PIDTEMP) || ENABLED(PIDTEMPBED)
      #define PID_dT ((OVERSAMPLENR * 12.0)/(F_CPU / 64.0 / 256.0))
    #endif

    #if ENABLED(PIDTEMP)

      #if ENABLED(PID_PARAMS_PER_EXTRUDER)

        static float Kp[EXTRUDERS], Ki[EXTRUDERS], Kd[EXTRUDERS];
        #if ENABLED(PID_ADD_EXTRUSION_RATE)
          float Kc[EXTRUDERS];
        #endif
        #define PID_PARAM(param, e) Temperature::param[e]

      #else

        static float Kp, Ki, Kd;
        #if ENABLED(PID_ADD_EXTRUSION_RATE)
          static float Kc;
        #endif
        #define PID_PARAM(param, e) Temperature::param

      #endif // PID_PARAMS_PER_EXTRUDER

      // Apply the scale factors to the PID values
      #define scalePID_i(i)   ( (i) * PID_dT )
      #define unscalePID_i(i) ( (i) / PID_dT )
      #define scalePID_d(d)   ( (d) / PID_dT )
      #define unscalePID_d(d) ( (d) * PID_dT )

    #endif

    #if ENABLED(PIDTEMPBED)
      float bedKp = DEFAULT_bedKp,
            bedKi = ((DEFAULT_bedKi) * PID_dT),
            bedKd = ((DEFAULT_bedKd) / PID_dT);
    #endif

    #if ENABLED(BABYSTEPPING)
      volatile int babystepsTodo[3] = { 0 };
    #endif

    #if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0
      int watch_target_temp[EXTRUDERS] = { 0 };
      millis_t watch_heater_next_ms[EXTRUDERS] = { 0 };
    #endif

    #if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_BED_TEMP_PERIOD > 0
      int watch_target_bed_temp = 0;
      millis_t watch_bed_next_ms = 0;
    #endif

    #if ENABLED(PREVENT_DANGEROUS_EXTRUDE)
      float extrude_min_temp = EXTRUDE_MINTEMP;
      FORCE_INLINE bool tooColdToExtrude(uint8_t e) { return degHotend(e) < extrude_min_temp; }
    #else
      FORCE_INLINE bool tooColdToExtrude(uint8_t e) { UNUSED(e); return false; }
    #endif

  private:

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      int redundant_temperature_raw = 0;
      float redundant_temperature = 0.0;
    #endif

    volatile bool temp_meas_ready = false;

    #if ENABLED(PIDTEMP)
      float temp_iState[EXTRUDERS] = { 0 };
      float temp_dState[EXTRUDERS] = { 0 };
      float pTerm[EXTRUDERS];
      float iTerm[EXTRUDERS];
      float dTerm[EXTRUDERS];

      #if ENABLED(PID_ADD_EXTRUSION_RATE)
        float cTerm[EXTRUDERS];
        long last_position[EXTRUDERS];
        long lpq[LPQ_MAX_LEN];
        int lpq_ptr = 0;
      #endif

      float pid_error[EXTRUDERS];
      float temp_iState_min[EXTRUDERS];
      float temp_iState_max[EXTRUDERS];
      bool pid_reset[EXTRUDERS];
    #endif

    #if ENABLED(PIDTEMPBED)
      float temp_iState_bed = { 0 };
      float temp_dState_bed = { 0 };
      float pTerm_bed;
      float iTerm_bed;
      float dTerm_bed;
      float pid_error_bed;
      float temp_iState_min_bed;
      float temp_iState_max_bed;
    #else
      millis_t next_bed_check_ms;
    #endif

    unsigned long raw_temp_value[4] = { 0 };
    unsigned long raw_temp_bed_value = 0;

    // Init min and max temp with extreme values to prevent false errors during startup
    int minttemp_raw[EXTRUDERS] = ARRAY_BY_EXTRUDERS(HEATER_0_RAW_LO_TEMP , HEATER_1_RAW_LO_TEMP , HEATER_2_RAW_LO_TEMP, HEATER_3_RAW_LO_TEMP);
    int maxttemp_raw[EXTRUDERS] = ARRAY_BY_EXTRUDERS(HEATER_0_RAW_HI_TEMP , HEATER_1_RAW_HI_TEMP , HEATER_2_RAW_HI_TEMP, HEATER_3_RAW_HI_TEMP);
    int minttemp[EXTRUDERS] = { 0 };
    int maxttemp[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(16383);

    #ifdef BED_MINTEMP
      int bed_minttemp_raw = HEATER_BED_RAW_LO_TEMP;
    #endif

    #ifdef BED_MAXTEMP
      int bed_maxttemp_raw = HEATER_BED_RAW_HI_TEMP;
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      int meas_shift_index;  // Index of a delayed sample in buffer
    #endif

    #if HAS_AUTO_FAN
      millis_t next_auto_fan_check_ms;
    #endif

    unsigned char soft_pwm[EXTRUDERS];

    #if ENABLED(FAN_SOFT_PWM)
      unsigned char soft_pwm_fan[FAN_COUNT];
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      int current_raw_filwidth = 0;  //Holds measured filament diameter - one extruder only
    #endif

  public:

    /**
     * Static (class) methods
     */
    static float analog2temp(int raw, uint8_t e);
    static float analog2tempBed(int raw);

    /**
     * Instance Methods
     */

    Temperature();

    void init();

    /**
     * Called from the Temperature ISR
     */
    void isr();

    /**
     * Call periodically to manage heaters
     */
    void manage_heater();

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      float analog2widthFil(); // Convert raw Filament Width to millimeters
      int widthFil_to_size_ratio(); // Convert raw Filament Width to an extrusion ratio
    #endif


    //high level conversion routines, for use outside of temperature.cpp
    //inline so that there is no performance decrease.
    //deg=degreeCelsius

    FORCE_INLINE float degHotend(uint8_t extruder) { return current_temperature[extruder]; }
    FORCE_INLINE float degBed() { return current_temperature_bed; }

    #if ENABLED(SHOW_TEMP_ADC_VALUES)
    FORCE_INLINE float rawHotendTemp(uint8_t extruder) { return current_temperature_raw[extruder]; }
    FORCE_INLINE float rawBedTemp() { return current_temperature_bed_raw; }
    #endif

    FORCE_INLINE float degTargetHotend(uint8_t extruder) { return target_temperature[extruder]; }
    FORCE_INLINE float degTargetBed() { return target_temperature_bed; }

    #if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0
      void start_watching_heater(int e = 0);
    #endif

    #if ENABLED(THERMAL_PROTECTION_BED) && WATCH_BED_TEMP_PERIOD > 0
      void start_watching_bed();
    #endif

    FORCE_INLINE void setTargetHotend(const float& celsius, uint8_t extruder) {
      target_temperature[extruder] = celsius;
      #if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0
        start_watching_heater(extruder);
      #endif
    }

    FORCE_INLINE void setTargetBed(const float& celsius) {
      target_temperature_bed = celsius;
      #if ENABLED(THERMAL_PROTECTION_BED) && WATCH_BED_TEMP_PERIOD > 0
        start_watching_bed();
      #endif
    }

    FORCE_INLINE bool isHeatingHotend(uint8_t extruder) { return target_temperature[extruder] > current_temperature[extruder]; }
    FORCE_INLINE bool isHeatingBed() { return target_temperature_bed > current_temperature_bed; }

    FORCE_INLINE bool isCoolingHotend(uint8_t extruder) { return target_temperature[extruder] < current_temperature[extruder]; }
    FORCE_INLINE bool isCoolingBed() { return target_temperature_bed < current_temperature_bed; }

    /**
     * The software PWM power for a heater
     */
    int getHeaterPower(int heater);

    /**
     * Switch off all heaters, set all target temperatures to 0
     */
    void disable_all_heaters();

    /**
     * Perform auto-tuning for hotend or bed in response to M303
     */
    #if HAS_PID_HEATING
      void PID_autotune(float temp, int extruder, int ncycles, bool set_result=false);
    #endif

    /**
     * Update the temp manager when PID values change
     */
    void updatePID();

    FORCE_INLINE void autotempShutdown() {
      #if ENABLED(AUTOTEMP)
        if (planner.autotemp_enabled) {
          planner.autotemp_enabled = false;
          if (degTargetHotend(active_extruder) > planner.autotemp_min)
            setTargetHotend(0, active_extruder);
        }
      #endif
    }

    #if ENABLED(BABYSTEPPING)

      FORCE_INLINE void babystep_axis(AxisEnum axis, int distance) {
        #if ENABLED(COREXY) || ENABLED(COREXZ)
          #if ENABLED(BABYSTEP_XY)
            switch (axis) {
              case X_AXIS: // X on CoreXY and CoreXZ
                babystepsTodo[A_AXIS] += distance * 2;
                babystepsTodo[CORE_AXIS_2] += distance * 2;
                break;
              case CORE_AXIS_2: // Y on CoreXY, Z on CoreXZ
                babystepsTodo[A_AXIS] += distance * 2;
                babystepsTodo[CORE_AXIS_2] -= distance * 2;
                break;
              case CORE_AXIS_3: // Z on CoreXY, Y on CoreXZ
                babystepsTodo[CORE_AXIS_3] += distance;
                break;
            }
          #elif ENABLED(COREXZ)
            babystepsTodo[A_AXIS] += distance * 2;
            babystepsTodo[C_AXIS] -= distance * 2;
          #else
            babystepsTodo[Z_AXIS] += distance;
          #endif
        #else
          babystepsTodo[axis] += distance;
        #endif
      }

    #endif // BABYSTEPPING

  private:

    void set_current_temp_raw();

    void updateTemperaturesFromRawValues();

    #if ENABLED(HEATER_0_USES_MAX6675)
      int read_max6675();
    #endif

    void checkExtruderAutoFans();

    float get_pid_output(int e);

    #if ENABLED(PIDTEMPBED)
      float get_pid_output_bed();
    #endif

    void _temp_error(int e, const char* serial_msg, const char* lcd_msg);
    void min_temp_error(uint8_t e);
    void max_temp_error(uint8_t e);

    #if ENABLED(THERMAL_PROTECTION_HOTENDS) || HAS_THERMALLY_PROTECTED_BED

      typedef enum TRState { TRInactive, TRFirstHeating, TRStable, TRRunaway } TRstate;

      void thermal_runaway_protection(TRState* state, millis_t* timer, float temperature, float target_temperature, int heater_id, int period_seconds, int hysteresis_degc);

      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        TRState thermal_runaway_state_machine[EXTRUDERS] = { TRInactive };
        millis_t thermal_runaway_timer[EXTRUDERS] = { 0 };
      #endif

      #if HAS_THERMALLY_PROTECTED_BED
        TRState thermal_runaway_bed_state_machine = TRInactive;
        millis_t thermal_runaway_bed_timer;
      #endif

    #endif // THERMAL_PROTECTION

};

extern Temperature thermalManager;

#endif // TEMPERATURE_H
