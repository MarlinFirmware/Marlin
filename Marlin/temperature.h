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

#include "thermistortables.h"

#include "MarlinConfig.h"

#if ENABLED(AUTO_POWER_CONTROL)
  #include "power.h"
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  #include "stepper.h"
#endif

#ifndef SOFT_PWM_SCALE
  #define SOFT_PWM_SCALE 0
#endif

#define ENABLE_TEMPERATURE_INTERRUPT()  SBI(TIMSK0, OCIE0B)
#define DISABLE_TEMPERATURE_INTERRUPT() CBI(TIMSK0, OCIE0B)
#define TEMPERATURE_ISR_ENABLED()      TEST(TIMSK0, OCIE0B)

#define HOTEND_LOOP() for (int8_t e = 0; e < HOTENDS; e++)

#if HOTENDS == 1
  #define HOTEND_INDEX  0
#else
  #define HOTEND_INDEX  e
#endif

/**
 * States for ADC reading in the ISR
 */
enum ADCSensorState : char {
  StartSampling,
  #if HAS_TEMP_ADC_0
    PrepareTemp_0,
    MeasureTemp_0,
  #endif
  #if HAS_TEMP_ADC_1
    PrepareTemp_1,
    MeasureTemp_1,
  #endif
  #if HAS_TEMP_ADC_2
    PrepareTemp_2,
    MeasureTemp_2,
  #endif
  #if HAS_TEMP_ADC_3
    PrepareTemp_3,
    MeasureTemp_3,
  #endif
  #if HAS_TEMP_ADC_4
    PrepareTemp_4,
    MeasureTemp_4,
  #endif
  #if HAS_HEATED_BED
    PrepareTemp_BED,
    MeasureTemp_BED,
  #endif
  #if HAS_TEMP_CHAMBER
    PrepareTemp_CHAMBER,
    MeasureTemp_CHAMBER,
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    Prepare_FILWIDTH,
    Measure_FILWIDTH,
  #endif
  #if ENABLED(ADC_KEYPAD)
    Prepare_ADC_KEY,
    Measure_ADC_KEY,
  #endif
  SensorsReady, // Temperatures ready. Delay the next round of readings to let ADC pins settle.
  StartupDelay  // Startup, delay initial temp reading a tiny bit so the hardware can settle
};

// Minimum number of Temperature::ISR loops between sensor readings.
// Multiplied by 16 (OVERSAMPLENR) to obtain the total time to
// get all oversampled sensor readings
#define MIN_ADC_ISR_LOOPS 10

#define ACTUAL_ADC_SAMPLES MAX(int(MIN_ADC_ISR_LOOPS), int(SensorsReady))

#if HAS_PID_HEATING
  #define PID_K2 (1.0f-PID_K1)
  #define PID_dT ((OVERSAMPLENR * float(ACTUAL_ADC_SAMPLES)) / (F_CPU / 64.0f / 256.0f))

  // Apply the scale factors to the PID values
  #define scalePID_i(i)   ( (i) * float(PID_dT) )
  #define unscalePID_i(i) ( (i) / float(PID_dT) )
  #define scalePID_d(d)   ( (d) / float(PID_dT) )
  #define unscalePID_d(d) ( (d) * float(PID_dT) )
#endif

class Temperature {

  public:

    static float current_temperature[HOTENDS];
    static int16_t current_temperature_raw[HOTENDS],
                   target_temperature[HOTENDS];
    static uint8_t soft_pwm_amount[HOTENDS];

    #if ENABLED(AUTO_POWER_E_FANS)
      static int16_t autofan_speed[HOTENDS];
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      static uint8_t soft_pwm_amount_fan[FAN_COUNT],
                     soft_pwm_count_fan[FAN_COUNT];
    #endif

    #if ENABLED(PIDTEMP)

      #if ENABLED(PID_PARAMS_PER_HOTEND) && HOTENDS > 1

        static float Kp[HOTENDS], Ki[HOTENDS], Kd[HOTENDS];
        #if ENABLED(PID_EXTRUSION_SCALING)
          static float Kc[HOTENDS];
        #endif
        #define PID_PARAM(param, h) Temperature::param[h]

      #else

        static float Kp, Ki, Kd;
        #if ENABLED(PID_EXTRUSION_SCALING)
          static float Kc;
        #endif
        #define PID_PARAM(param, h) Temperature::param

      #endif // PID_PARAMS_PER_HOTEND

    #endif

    #if HAS_HEATED_BED
      static float current_temperature_bed;
      static int16_t current_temperature_bed_raw, target_temperature_bed;
      static uint8_t soft_pwm_amount_bed;
      #if ENABLED(PIDTEMPBED)
        static float bedKp, bedKi, bedKd;
      #endif
    #endif

    #if ENABLED(BABYSTEPPING)
      static volatile int babystepsTodo[3];
    #endif

    #if ENABLED(PREVENT_COLD_EXTRUSION)
      static bool allow_cold_extrude;
      static int16_t extrude_min_temp;
      FORCE_INLINE static bool tooCold(const int16_t temp) { return allow_cold_extrude ? false : temp < extrude_min_temp; }
      FORCE_INLINE static bool tooColdToExtrude(const uint8_t e) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        return tooCold(degHotend(HOTEND_INDEX));
      }
      FORCE_INLINE static bool targetTooColdToExtrude(const uint8_t e) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        return tooCold(degTargetHotend(HOTEND_INDEX));
      }
    #else
      FORCE_INLINE static bool tooColdToExtrude(const uint8_t e) { UNUSED(e); return false; }
      FORCE_INLINE static bool targetTooColdToExtrude(const uint8_t e) { UNUSED(e); return false; }
    #endif

    FORCE_INLINE static bool hotEnoughToExtrude(const uint8_t e) { return !tooColdToExtrude(e); }
    FORCE_INLINE static bool targetHotEnoughToExtrude(const uint8_t e) { return !targetTooColdToExtrude(e); }

  private:

    static volatile bool temp_meas_ready;
    static uint16_t raw_temp_value[MAX_EXTRUDERS];

    #if WATCH_HOTENDS
      static uint16_t watch_target_temp[HOTENDS];
      static millis_t watch_heater_next_ms[HOTENDS];
    #endif

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      static uint16_t redundant_temperature_raw;
      static float redundant_temperature;
    #endif

    #if ENABLED(PIDTEMP)
      static float temp_iState[HOTENDS],
                   temp_dState[HOTENDS],
                   pTerm[HOTENDS],
                   iTerm[HOTENDS],
                   dTerm[HOTENDS];

      #if ENABLED(PID_EXTRUSION_SCALING)
        static float cTerm[HOTENDS];
        static long last_e_position;
        static long lpq[LPQ_MAX_LEN];
        static int lpq_ptr;
      #endif

      static float pid_error[HOTENDS];
      static bool pid_reset[HOTENDS];
    #endif

    // Init min and max temp with extreme values to prevent false errors during startup
    static int16_t minttemp_raw[HOTENDS],
                   maxttemp_raw[HOTENDS],
                   minttemp[HOTENDS],
                   maxttemp[HOTENDS];

    #if HAS_HEATED_BED
      static uint16_t raw_temp_bed_value;
      #if WATCH_THE_BED
        static uint16_t watch_target_bed_temp;
        static millis_t watch_bed_next_ms;
      #endif
      #if ENABLED(PIDTEMPBED)
        static float temp_iState_bed,
                     temp_dState_bed,
                     pTerm_bed,
                     iTerm_bed,
                     dTerm_bed,
                     pid_error_bed;
      #else
        static millis_t next_bed_check_ms;
      #endif
      #if HEATER_IDLE_HANDLER
        static millis_t bed_idle_timeout_ms;
        static bool bed_idle_timeout_exceeded;
      #endif
      #ifdef BED_MINTEMP
        static int16_t bed_minttemp_raw;
      #endif
      #ifdef BED_MAXTEMP
        static int16_t bed_maxttemp_raw;
      #endif
    #endif

    #if HAS_TEMP_CHAMBER
      static uint16_t raw_temp_chamber_value;
      static float current_temperature_chamber;
      static int16_t current_temperature_chamber_raw;
    #endif

    #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
      static uint8_t consecutive_low_temperature_error[HOTENDS];
    #endif

    #ifdef MILLISECONDS_PREHEAT_TIME
      static millis_t preheat_end_time[HOTENDS];
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      static int8_t meas_shift_index;  // Index of a delayed sample in buffer
    #endif

    #if HAS_AUTO_FAN
      static millis_t next_auto_fan_check_ms;
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      static uint16_t current_raw_filwidth; // Measured filament diameter - one extruder only
    #endif

    #if ENABLED(PROBING_HEATERS_OFF)
      static bool paused;
    #endif

    #if HEATER_IDLE_HANDLER
      static millis_t heater_idle_timeout_ms[HOTENDS];
      static bool heater_idle_timeout_exceeded[HOTENDS];
    #endif

  public:
    #if ENABLED(ADC_KEYPAD)
      static uint32_t current_ADCKey_raw;
      static uint8_t ADCKey_count;
    #endif

    #if ENABLED(PID_EXTRUSION_SCALING)
      static int16_t lpq_len;
    #endif

    /**
     * Instance Methods
     */

    Temperature();

    void init();

    /**
     * Static (class) methods
     */
    static float analog2temp(const int raw, const uint8_t e);

    #if HAS_HEATED_BED
      static float analog2tempBed(const int raw);
    #endif
    #if HAS_TEMP_CHAMBER
      static float analog2tempChamber(const int raw);
    #endif

    /**
     * Called from the Temperature ISR
     */
    static void readings_ready();
    static void isr();

    /**
     * Call periodically to manage heaters
     */
    static void manage_heater() _O2; // Added _O2 to work around a compiler error

    /**
     * Preheating hotends
     */
    #ifdef MILLISECONDS_PREHEAT_TIME
      static bool is_preheating(const uint8_t e) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        return preheat_end_time[HOTEND_INDEX] && PENDING(millis(), preheat_end_time[HOTEND_INDEX]);
      }
      static void start_preheat_time(const uint8_t e) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        preheat_end_time[HOTEND_INDEX] = millis() + MILLISECONDS_PREHEAT_TIME;
      }
      static void reset_preheat_time(const uint8_t e) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        preheat_end_time[HOTEND_INDEX] = 0;
      }
    #else
      #define is_preheating(n) (false)
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      static float analog2widthFil();         // Convert raw Filament Width to millimeters
      static int8_t widthFil_to_size_ratio(); // Convert Filament Width (mm) to an extrusion ratio
    #endif


    //high level conversion routines, for use outside of temperature.cpp
    //inline so that there is no performance decrease.
    //deg=degreeCelsius

    FORCE_INLINE static float degHotend(const uint8_t e) {
      #if HOTENDS == 1
        UNUSED(e);
      #endif
      return current_temperature[HOTEND_INDEX];
    }

    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      FORCE_INLINE static int16_t rawHotendTemp(const uint8_t e) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        return current_temperature_raw[HOTEND_INDEX];
      }
    #endif

    FORCE_INLINE static int16_t degTargetHotend(const uint8_t e) {
      #if HOTENDS == 1
        UNUSED(e);
      #endif
      return target_temperature[HOTEND_INDEX];
    }

    #if WATCH_HOTENDS
      static void start_watching_heater(const uint8_t e = 0);
    #endif

    static void setTargetHotend(const int16_t celsius, const uint8_t e) {
      #if HOTENDS == 1
        UNUSED(e);
      #endif
      #ifdef MILLISECONDS_PREHEAT_TIME
        if (celsius == 0)
          reset_preheat_time(HOTEND_INDEX);
        else if (target_temperature[HOTEND_INDEX] == 0)
          start_preheat_time(HOTEND_INDEX);
      #endif
      #if ENABLED(AUTO_POWER_CONTROL)
        powerManager.power_on();
      #endif
      target_temperature[HOTEND_INDEX] = MIN(celsius, maxttemp[HOTEND_INDEX] - 15);
      #if WATCH_HOTENDS
        start_watching_heater(HOTEND_INDEX);
      #endif
    }

    FORCE_INLINE static bool isHeatingHotend(const uint8_t e) {
      #if HOTENDS == 1
        UNUSED(e);
      #endif
      return target_temperature[HOTEND_INDEX] > current_temperature[HOTEND_INDEX];
    }

    FORCE_INLINE static bool isCoolingHotend(const uint8_t e) {
      #if HOTENDS == 1
        UNUSED(e);
      #endif
      return target_temperature[HOTEND_INDEX] < current_temperature[HOTEND_INDEX];
    }

    #if HAS_HEATED_BED
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawBedTemp()  { return current_temperature_bed_raw; }
      #endif
      FORCE_INLINE static float degBed()          { return current_temperature_bed; }
      FORCE_INLINE static int16_t degTargetBed()  { return target_temperature_bed; }
      FORCE_INLINE static bool isHeatingBed()     { return target_temperature_bed > current_temperature_bed; }
      FORCE_INLINE static bool isCoolingBed()     { return target_temperature_bed < current_temperature_bed; }

      static void setTargetBed(const int16_t celsius) {
        #if ENABLED(AUTO_POWER_CONTROL)
          powerManager.power_on();
        #endif
        target_temperature_bed =
          #ifdef BED_MAXTEMP
            MIN(celsius, BED_MAXTEMP - 15)
          #else
            celsius
          #endif
        ;
        #if WATCH_THE_BED
          start_watching_bed();
        #endif
      }

      #if WATCH_THE_BED
        static void start_watching_bed();
      #endif
    #endif

    #if HAS_TEMP_CHAMBER
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawChamberTemp() { return current_temperature_chamber_raw; }
      #endif
      FORCE_INLINE static float degChamber() { return current_temperature_chamber; }
    #endif

    FORCE_INLINE static bool wait_for_heating(const uint8_t e) {
      return degTargetHotend(e) > TEMP_HYSTERESIS && ABS(degHotend(e) - degTargetHotend(e)) > TEMP_HYSTERESIS;
    }

    /**
     * The software PWM power for a heater
     */
    static int getHeaterPower(const int heater);

    /**
     * Switch off all heaters, set all target temperatures to 0
     */
    static void disable_all_heaters();

    /**
     * Perform auto-tuning for hotend or bed in response to M303
     */
    #if HAS_PID_HEATING
      static void PID_autotune(const float &target, const int8_t hotend, const int8_t ncycles, const bool set_result=false);

      /**
       * Update the temp manager when PID values change
       */
      #if ENABLED(PIDTEMP)
        FORCE_INLINE static void updatePID() {
          #if ENABLED(PID_EXTRUSION_SCALING)
            last_e_position = 0;
          #endif
        }
      #endif

    #endif

    #if ENABLED(BABYSTEPPING)

      static void babystep_axis(const AxisEnum axis, const int16_t distance) {
        if (TEST(axis_known_position, axis)) {
          #if IS_CORE
            #if ENABLED(BABYSTEP_XY)
              switch (axis) {
                case CORE_AXIS_1: // X on CoreXY and CoreXZ, Y on CoreYZ
                  babystepsTodo[CORE_AXIS_1] += distance * 2;
                  babystepsTodo[CORE_AXIS_2] += distance * 2;
                  break;
                case CORE_AXIS_2: // Y on CoreXY, Z on CoreXZ and CoreYZ
                  babystepsTodo[CORE_AXIS_1] += CORESIGN(distance * 2);
                  babystepsTodo[CORE_AXIS_2] -= CORESIGN(distance * 2);
                  break;
                case NORMAL_AXIS: // Z on CoreXY, Y on CoreXZ, X on CoreYZ
                  babystepsTodo[NORMAL_AXIS] += distance;
                  break;
              }
            #elif CORE_IS_XZ || CORE_IS_YZ
              // Only Z stepping needs to be handled here
              babystepsTodo[CORE_AXIS_1] += CORESIGN(distance * 2);
              babystepsTodo[CORE_AXIS_2] -= CORESIGN(distance * 2);
            #else
              babystepsTodo[Z_AXIS] += distance;
            #endif
          #else
            babystepsTodo[axis] += distance;
          #endif
        }
      }

    #endif // BABYSTEPPING

    #if ENABLED(PROBING_HEATERS_OFF)
      static void pause(const bool p);
      FORCE_INLINE static bool is_paused() { return paused; }
    #endif

    #if HEATER_IDLE_HANDLER

      static void start_heater_idle_timer(const uint8_t e, const millis_t timeout_ms) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        heater_idle_timeout_ms[HOTEND_INDEX] = millis() + timeout_ms;
        heater_idle_timeout_exceeded[HOTEND_INDEX] = false;
      }

      static void reset_heater_idle_timer(const uint8_t e) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        heater_idle_timeout_ms[HOTEND_INDEX] = 0;
        heater_idle_timeout_exceeded[HOTEND_INDEX] = false;
        #if WATCH_HOTENDS
          start_watching_heater(HOTEND_INDEX);
        #endif
      }

      FORCE_INLINE static bool is_heater_idle(const uint8_t e) {
        #if HOTENDS == 1
          UNUSED(e);
        #endif
        return heater_idle_timeout_exceeded[HOTEND_INDEX];
      }

      #if HAS_HEATED_BED
        static void start_bed_idle_timer(const millis_t timeout_ms) {
          bed_idle_timeout_ms = millis() + timeout_ms;
          bed_idle_timeout_exceeded = false;
        }

        static void reset_bed_idle_timer() {
          bed_idle_timeout_ms = 0;
          bed_idle_timeout_exceeded = false;
          #if WATCH_THE_BED
            start_watching_bed();
          #endif
        }

        FORCE_INLINE static bool is_bed_idle() { return bed_idle_timeout_exceeded; }
      #endif

    #endif // HEATER_IDLE_HANDLER

    #if HAS_TEMP_SENSOR
      static void print_heaterstates();
      #if ENABLED(AUTO_REPORT_TEMPERATURES)
        static uint8_t auto_report_temp_interval;
        static millis_t next_temp_report_ms;
        static void auto_report_temperatures(void);
        FORCE_INLINE void set_auto_report_interval(uint8_t v) {
          NOMORE(v, 60);
          auto_report_temp_interval = v;
          next_temp_report_ms = millis() + 1000UL * v;
        }
      #endif
    #endif

  private:

    #if ENABLED(FAST_PWM_FAN)
      static void setPwmFrequency(const pin_t pin, int val);
    #endif

    static void set_current_temp_raw();

    static void updateTemperaturesFromRawValues();

    #if ENABLED(HEATER_0_USES_MAX6675)
      static int read_max6675();
    #endif

    static void checkExtruderAutoFans();

    static float get_pid_output(const int8_t e);

    #if ENABLED(PIDTEMPBED)
      static float get_pid_output_bed();
    #endif

    static void _temp_error(const int8_t e, const char * const serial_msg, const char * const lcd_msg);
    static void min_temp_error(const int8_t e);
    static void max_temp_error(const int8_t e);

    #if ENABLED(THERMAL_PROTECTION_HOTENDS) || HAS_THERMALLY_PROTECTED_BED

      enum TRState : char { TRInactive, TRFirstHeating, TRStable, TRRunaway };

      static void thermal_runaway_protection(TRState * const state, millis_t * const timer, const float &current, const float &target, const int8_t heater_id, const uint16_t period_seconds, const uint16_t hysteresis_degc);

      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        static TRState thermal_runaway_state_machine[HOTENDS];
        static millis_t thermal_runaway_timer[HOTENDS];
      #endif

      #if HAS_THERMALLY_PROTECTED_BED
        static TRState thermal_runaway_bed_state_machine;
        static millis_t thermal_runaway_bed_timer;
      #endif

    #endif // THERMAL_PROTECTION

};

extern Temperature thermalManager;

#endif // TEMPERATURE_H
