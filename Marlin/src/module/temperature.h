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
#pragma once

/**
 * temperature.h - temperature controller
 */

#include "thermistor/thermistors.h"

#include "../inc/MarlinConfig.h"

#if ENABLED(AUTO_POWER_CONTROL)
  #include "../feature/power.h"
#endif

#if ENABLED(AUTO_REPORT_TEMPERATURES)
  #include "../libs/autoreport.h"
#endif

#ifndef SOFT_PWM_SCALE
  #define SOFT_PWM_SCALE 0
#endif

#define HOTEND_INDEX TERN(HAS_MULTI_HOTEND, e, 0)
#define E_NAME TERN_(HAS_MULTI_HOTEND, e)

// Element identifiers. Positive values are hotends. Negative values are other heaters or coolers.
typedef enum : int8_t {
  H_NONE = -6,
  H_COOLER, H_PROBE, H_REDUNDANT, H_CHAMBER, H_BED,
  H_E0, H_E1, H_E2, H_E3, H_E4, H_E5, H_E6, H_E7
} heater_id_t;

// PID storage
typedef struct { float Kp, Ki, Kd;     } PID_t;
typedef struct { float Kp, Ki, Kd, Kc; } PIDC_t;
typedef struct { float Kp, Ki, Kd, Kf; } PIDF_t;
typedef struct { float Kp, Ki, Kd, Kc, Kf; } PIDCF_t;

typedef
  #if BOTH(PID_EXTRUSION_SCALING, PID_FAN_SCALING)
    PIDCF_t
  #elif ENABLED(PID_EXTRUSION_SCALING)
    PIDC_t
  #elif ENABLED(PID_FAN_SCALING)
    PIDF_t
  #else
    PID_t
  #endif
hotend_pid_t;

#if ENABLED(PID_EXTRUSION_SCALING)
  typedef IF<(LPQ_MAX_LEN > 255), uint16_t, uint8_t>::type lpq_ptr_t;
#endif

#define PID_PARAM(F,H) _PID_##F(TERN(PID_PARAMS_PER_HOTEND, H, 0 & H)) // Always use 'H' to suppress warning
#define _PID_Kp(H) TERN(PIDTEMP, Temperature::temp_hotend[H].pid.Kp, NAN)
#define _PID_Ki(H) TERN(PIDTEMP, Temperature::temp_hotend[H].pid.Ki, NAN)
#define _PID_Kd(H) TERN(PIDTEMP, Temperature::temp_hotend[H].pid.Kd, NAN)
#if ENABLED(PIDTEMP)
  #define _PID_Kc(H) TERN(PID_EXTRUSION_SCALING, Temperature::temp_hotend[H].pid.Kc, 1)
  #define _PID_Kf(H) TERN(PID_FAN_SCALING,       Temperature::temp_hotend[H].pid.Kf, 0)
#else
  #define _PID_Kc(H) 1
  #define _PID_Kf(H) 0
#endif

/**
 * States for ADC reading in the ISR
 */
enum ADCSensorState : char {
  StartSampling,
  #if HAS_TEMP_ADC_0
    PrepareTemp_0, MeasureTemp_0,
  #endif
  #if HAS_TEMP_ADC_BED
    PrepareTemp_BED, MeasureTemp_BED,
  #endif
  #if HAS_TEMP_ADC_CHAMBER
    PrepareTemp_CHAMBER, MeasureTemp_CHAMBER,
  #endif
  #if HAS_TEMP_ADC_COOLER
    PrepareTemp_COOLER, MeasureTemp_COOLER,
  #endif
  #if HAS_TEMP_ADC_PROBE
    PrepareTemp_PROBE, MeasureTemp_PROBE,
  #endif
  #if HAS_TEMP_ADC_REDUNDANT
    PrepareTemp_REDUNDANT, MeasureTemp_REDUNDANT,
  #endif
  #if HAS_TEMP_ADC_1
    PrepareTemp_1, MeasureTemp_1,
  #endif
  #if HAS_TEMP_ADC_2
    PrepareTemp_2, MeasureTemp_2,
  #endif
  #if HAS_TEMP_ADC_3
    PrepareTemp_3, MeasureTemp_3,
  #endif
  #if HAS_TEMP_ADC_4
    PrepareTemp_4, MeasureTemp_4,
  #endif
  #if HAS_TEMP_ADC_5
    PrepareTemp_5, MeasureTemp_5,
  #endif
  #if HAS_TEMP_ADC_6
    PrepareTemp_6, MeasureTemp_6,
  #endif
  #if HAS_TEMP_ADC_7
    PrepareTemp_7, MeasureTemp_7,
  #endif
  #if HAS_JOY_ADC_X
    PrepareJoy_X, MeasureJoy_X,
  #endif
  #if HAS_JOY_ADC_Y
    PrepareJoy_Y, MeasureJoy_Y,
  #endif
  #if HAS_JOY_ADC_Z
    PrepareJoy_Z, MeasureJoy_Z,
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    Prepare_FILWIDTH, Measure_FILWIDTH,
  #endif
  #if ENABLED(POWER_MONITOR_CURRENT)
    Prepare_POWER_MONITOR_CURRENT,
    Measure_POWER_MONITOR_CURRENT,
  #endif
  #if ENABLED(POWER_MONITOR_VOLTAGE)
    Prepare_POWER_MONITOR_VOLTAGE,
    Measure_POWER_MONITOR_VOLTAGE,
  #endif
  #if HAS_ADC_BUTTONS
    Prepare_ADC_KEY, Measure_ADC_KEY,
  #endif
  SensorsReady, // Temperatures ready. Delay the next round of readings to let ADC pins settle.
  StartupDelay  // Startup, delay initial temp reading a tiny bit so the hardware can settle
};

// Minimum number of Temperature::ISR loops between sensor readings.
// Multiplied by 16 (OVERSAMPLENR) to obtain the total time to
// get all oversampled sensor readings
#define MIN_ADC_ISR_LOOPS 10

#define ACTUAL_ADC_SAMPLES _MAX(int(MIN_ADC_ISR_LOOPS), int(SensorsReady))

#if HAS_PID_HEATING
  #define PID_K2 (1-float(PID_K1))
  #define PID_dT ((OVERSAMPLENR * float(ACTUAL_ADC_SAMPLES)) / TEMP_TIMER_FREQUENCY)

  // Apply the scale factors to the PID values
  #define scalePID_i(i)   ( float(i) * PID_dT )
  #define unscalePID_i(i) ( float(i) / PID_dT )
  #define scalePID_d(d)   ( float(d) / PID_dT )
  #define unscalePID_d(d) ( float(d) * PID_dT )
#endif

#if ENABLED(G26_MESH_VALIDATION) && EITHER(HAS_LCD_MENU, EXTENSIBLE_UI)
  #define G26_CLICK_CAN_CANCEL 1
#endif

// A temperature sensor
typedef struct TempInfo {
  uint16_t acc;
  int16_t raw;
  celsius_float_t celsius;
  inline void reset() { acc = 0; }
  inline void sample(const uint16_t s) { acc += s; }
  inline void update() { raw = acc; }
} temp_info_t;

#if HAS_TEMP_REDUNDANT
  // A redundant temperature sensor
  typedef struct RedundantTempInfo : public TempInfo {
    temp_info_t* target;
  } redundant_temp_info_t;
#endif

// A PWM heater with temperature sensor
typedef struct HeaterInfo : public TempInfo {
  celsius_t target;
  uint8_t soft_pwm_amount;
} heater_info_t;

// A heater with PID stabilization
template<typename T>
struct PIDHeaterInfo : public HeaterInfo {
  T pid;  // Initialized by settings.load()
};

#if ENABLED(PIDTEMP)
  typedef struct PIDHeaterInfo<hotend_pid_t> hotend_info_t;
#else
  typedef heater_info_t hotend_info_t;
#endif
#if HAS_HEATED_BED
  #if ENABLED(PIDTEMPBED)
    typedef struct PIDHeaterInfo<PID_t> bed_info_t;
  #else
    typedef heater_info_t bed_info_t;
  #endif
#endif
#if HAS_TEMP_PROBE
  typedef temp_info_t probe_info_t;
#endif
#if HAS_HEATED_CHAMBER
  #if ENABLED(PIDTEMPCHAMBER)
    typedef struct PIDHeaterInfo<PID_t> chamber_info_t;
  #else
    typedef heater_info_t chamber_info_t;
  #endif
#elif HAS_TEMP_CHAMBER
  typedef temp_info_t chamber_info_t;
#endif
#if EITHER(HAS_COOLER, HAS_TEMP_COOLER)
  typedef heater_info_t cooler_info_t;
#endif

// Heater watch handling
template <int INCREASE, int HYSTERESIS, millis_t PERIOD>
struct HeaterWatch {
  celsius_t target;
  millis_t next_ms;
  inline bool elapsed(const millis_t &ms) { return next_ms && ELAPSED(ms, next_ms); }
  inline bool elapsed() { return elapsed(millis()); }

  inline bool check(const celsius_t curr) { return curr >= target; }

  inline void restart(const celsius_t curr, const celsius_t tgt) {
    if (tgt) {
      const celsius_t newtarget = curr + INCREASE;
      if (newtarget < tgt - HYSTERESIS - 1) {
        target = newtarget;
        next_ms = millis() + SEC_TO_MS(PERIOD);
        return;
      }
    }
    next_ms = 0;
  }
};

#if WATCH_HOTENDS
  typedef struct HeaterWatch<WATCH_TEMP_INCREASE, TEMP_HYSTERESIS, WATCH_TEMP_PERIOD> hotend_watch_t;
#endif
#if WATCH_BED
  typedef struct HeaterWatch<WATCH_BED_TEMP_INCREASE, TEMP_BED_HYSTERESIS, WATCH_BED_TEMP_PERIOD> bed_watch_t;
#endif
#if WATCH_CHAMBER
  typedef struct HeaterWatch<WATCH_CHAMBER_TEMP_INCREASE, TEMP_CHAMBER_HYSTERESIS, WATCH_CHAMBER_TEMP_PERIOD> chamber_watch_t;
#endif
#if WATCH_COOLER
  typedef struct HeaterWatch<WATCH_COOLER_TEMP_INCREASE, TEMP_COOLER_HYSTERESIS, WATCH_COOLER_TEMP_PERIOD> cooler_watch_t;
#endif

// Temperature sensor read value ranges
typedef struct { int16_t raw_min, raw_max; } raw_range_t;
typedef struct { celsius_t mintemp, maxtemp; } celsius_range_t;
typedef struct { int16_t raw_min, raw_max; celsius_t mintemp, maxtemp; } temp_range_t;

#define THERMISTOR_ABS_ZERO_C           -273.15f  // bbbbrrrrr cold !
#define THERMISTOR_RESISTANCE_NOMINAL_C 25.0f     // mmmmm comfortable

#if HAS_USER_THERMISTORS

  enum CustomThermistorIndex : uint8_t {
    #if TEMP_SENSOR_0_IS_CUSTOM
      CTI_HOTEND_0,
    #endif
    #if TEMP_SENSOR_1_IS_CUSTOM
      CTI_HOTEND_1,
    #endif
    #if TEMP_SENSOR_2_IS_CUSTOM
      CTI_HOTEND_2,
    #endif
    #if TEMP_SENSOR_3_IS_CUSTOM
      CTI_HOTEND_3,
    #endif
    #if TEMP_SENSOR_4_IS_CUSTOM
      CTI_HOTEND_4,
    #endif
    #if TEMP_SENSOR_5_IS_CUSTOM
      CTI_HOTEND_5,
    #endif
    #if TEMP_SENSOR_BED_IS_CUSTOM
      CTI_BED,
    #endif
    #if TEMP_SENSOR_PROBE_IS_CUSTOM
      CTI_PROBE,
    #endif
    #if TEMP_SENSOR_CHAMBER_IS_CUSTOM
      CTI_CHAMBER,
    #endif
    #if TEMP_SENSOR_COOLER_IS_CUSTOM
      CTI_COOLER,
    #endif
    #if TEMP_SENSOR_REDUNDANT_IS_CUSTOM
      CTI_REDUNDANT,
    #endif
    USER_THERMISTORS
  };

  // User-defined thermistor
  typedef struct {
    bool pre_calc;     // true if pre-calculations update needed
    float sh_c_coeff,  // Steinhart-Hart C coefficient .. defaults to '0.0'
          sh_alpha,
          series_res,
          res_25, res_25_recip,
          res_25_log,
          beta, beta_recip;
  } user_thermistor_t;

#endif

class Temperature {

  public:

    #if HAS_HOTEND
      static hotend_info_t temp_hotend[HOTENDS];
      static const celsius_t hotend_maxtemp[HOTENDS];
      static inline celsius_t hotend_max_target(const uint8_t e) { return hotend_maxtemp[e] - (HOTEND_OVERSHOOT); }
    #endif
    #if HAS_HEATED_BED
      static bed_info_t temp_bed;
    #endif
    #if HAS_TEMP_PROBE
      static probe_info_t temp_probe;
    #endif
    #if HAS_TEMP_CHAMBER
      static chamber_info_t temp_chamber;
    #endif
    #if HAS_TEMP_COOLER
      static cooler_info_t temp_cooler;
    #endif
    #if HAS_TEMP_REDUNDANT
      static redundant_temp_info_t temp_redundant;
    #endif

    #if ENABLED(AUTO_POWER_E_FANS)
      static uint8_t autofan_speed[HOTENDS];
    #endif
    #if ENABLED(AUTO_POWER_CHAMBER_FAN)
      static uint8_t chamberfan_speed;
    #endif
    #if ENABLED(AUTO_POWER_COOLER_FAN)
      static uint8_t coolerfan_speed;
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      static uint8_t soft_pwm_amount_fan[FAN_COUNT],
                     soft_pwm_count_fan[FAN_COUNT];
    #endif

    #if ENABLED(PREVENT_COLD_EXTRUSION)
      static bool allow_cold_extrude;
      static celsius_t extrude_min_temp;
      static inline bool tooCold(const celsius_t temp) { return allow_cold_extrude ? false : temp < extrude_min_temp - (TEMP_WINDOW); }
      static inline bool tooColdToExtrude(const uint8_t E_NAME)       { return tooCold(wholeDegHotend(HOTEND_INDEX)); }
      static inline bool targetTooColdToExtrude(const uint8_t E_NAME) { return tooCold(degTargetHotend(HOTEND_INDEX)); }
    #else
      static inline bool tooColdToExtrude(const uint8_t) { return false; }
      static inline bool targetTooColdToExtrude(const uint8_t) { return false; }
    #endif

    static inline bool hotEnoughToExtrude(const uint8_t e) { return !tooColdToExtrude(e); }
    static inline bool targetHotEnoughToExtrude(const uint8_t e) { return !targetTooColdToExtrude(e); }

    #if EITHER(SINGLENOZZLE_STANDBY_TEMP, SINGLENOZZLE_STANDBY_FAN)
      #if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
        static celsius_t singlenozzle_temp[EXTRUDERS];
      #endif
      #if ENABLED(SINGLENOZZLE_STANDBY_FAN)
        static uint8_t singlenozzle_fan_speed[EXTRUDERS];
      #endif
      static void singlenozzle_change(const uint8_t old_tool, const uint8_t new_tool);
    #endif

    #if HEATER_IDLE_HANDLER

      // Heater idle handling. Marlin creates one per hotend and one for the heated bed.
      typedef struct {
        millis_t timeout_ms;
        bool timed_out;
        inline void update(const millis_t &ms) { if (!timed_out && timeout_ms && ELAPSED(ms, timeout_ms)) timed_out = true; }
        inline void start(const millis_t &ms) { timeout_ms = millis() + ms; timed_out = false; }
        inline void reset() { timeout_ms = 0; timed_out = false; }
        inline void expire() { start(0); }
      } heater_idle_t;

      // Indices and size for the heater_idle array
      enum IdleIndex : int8_t {
        _II = -1

        #define _IDLE_INDEX_E(N) ,IDLE_INDEX_E##N
        REPEAT(HOTENDS, _IDLE_INDEX_E)
        #undef _IDLE_INDEX_E

        OPTARG(HAS_HEATED_BED, IDLE_INDEX_BED)

        , NR_HEATER_IDLE
      };

      // Convert the given heater_id_t to idle array index
      static inline IdleIndex idle_index_for_id(const int8_t heater_id) {
        TERN_(HAS_HEATED_BED, if (heater_id == H_BED) return IDLE_INDEX_BED);
        return (IdleIndex)_MAX(heater_id, 0);
      }

      static heater_idle_t heater_idle[NR_HEATER_IDLE];

    #endif // HEATER_IDLE_TIMER

    #if HAS_ADC_BUTTONS
      static uint32_t current_ADCKey_raw;
      static uint16_t ADCKey_count;
    #endif

    #if ENABLED(PID_EXTRUSION_SCALING)
      static int16_t lpq_len;
    #endif

  private:

    #if ENABLED(WATCH_HOTENDS)
      static hotend_watch_t watch_hotend[HOTENDS];
    #endif

    #if ENABLED(PID_EXTRUSION_SCALING)
      static int32_t last_e_position, lpq[LPQ_MAX_LEN];
      static lpq_ptr_t lpq_ptr;
    #endif

    #if HAS_HOTEND
      static temp_range_t temp_range[HOTENDS];
    #endif

    #if HAS_HEATED_BED
      #if ENABLED(WATCH_BED)
        static bed_watch_t watch_bed;
      #endif
      IF_DISABLED(PIDTEMPBED, static millis_t next_bed_check_ms);
      static int16_t mintemp_raw_BED, maxtemp_raw_BED;
    #endif

    #if HAS_HEATED_CHAMBER
      #if ENABLED(WATCH_CHAMBER)
        static chamber_watch_t watch_chamber;
      #endif
      TERN(PIDTEMPCHAMBER,,static millis_t next_chamber_check_ms);
      static int16_t mintemp_raw_CHAMBER, maxtemp_raw_CHAMBER;
    #endif

    #if HAS_COOLER
      #if ENABLED(WATCH_COOLER)
        static cooler_watch_t watch_cooler;
      #endif
      static millis_t next_cooler_check_ms, cooler_fan_flush_ms;
      static int16_t mintemp_raw_COOLER, maxtemp_raw_COOLER;
    #endif

    #if MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED > 1
      static uint8_t consecutive_low_temperature_error[HOTENDS];
    #endif

    #if MILLISECONDS_PREHEAT_TIME > 0
      static millis_t preheat_end_time[HOTENDS];
    #endif

    #if HAS_AUTO_FAN
      static millis_t next_auto_fan_check_ms;
    #endif

    #if ENABLED(PROBING_HEATERS_OFF)
      static bool paused_for_probing;
    #endif

  public:
    /**
     * Instance Methods
     */

    void init();

    /**
     * Static (class) methods
     */

    #if HAS_USER_THERMISTORS
      static user_thermistor_t user_thermistor[USER_THERMISTORS];
      static void log_user_thermistor(const uint8_t t_index, const bool eprom=false);
      static void reset_user_thermistors();
      static celsius_float_t user_thermistor_to_deg_c(const uint8_t t_index, const int16_t raw);
      static inline bool set_pull_up_res(int8_t t_index, float value) {
        //if (!WITHIN(t_index, 0, USER_THERMISTORS - 1)) return false;
        if (!WITHIN(value, 1, 1000000)) return false;
        user_thermistor[t_index].series_res = value;
        return true;
      }
      static inline bool set_res25(int8_t t_index, float value) {
        if (!WITHIN(value, 1, 10000000)) return false;
        user_thermistor[t_index].res_25 = value;
        user_thermistor[t_index].pre_calc = true;
        return true;
      }
      static inline bool set_beta(int8_t t_index, float value) {
        if (!WITHIN(value, 1, 1000000)) return false;
        user_thermistor[t_index].beta = value;
        user_thermistor[t_index].pre_calc = true;
        return true;
      }
      static inline bool set_sh_coeff(int8_t t_index, float value) {
        if (!WITHIN(value, -0.01f, 0.01f)) return false;
        user_thermistor[t_index].sh_c_coeff = value;
        user_thermistor[t_index].pre_calc = true;
        return true;
      }
    #endif

    #if HAS_HOTEND
      static celsius_float_t analog_to_celsius_hotend(const int16_t raw, const uint8_t e);
    #endif
    #if HAS_HEATED_BED
      static celsius_float_t analog_to_celsius_bed(const int16_t raw);
    #endif
    #if HAS_TEMP_PROBE
      static celsius_float_t analog_to_celsius_probe(const int16_t raw);
    #endif
    #if HAS_TEMP_CHAMBER
      static celsius_float_t analog_to_celsius_chamber(const int16_t raw);
    #endif
    #if HAS_TEMP_COOLER
      static celsius_float_t analog_to_celsius_cooler(const int16_t raw);
    #endif
    #if HAS_TEMP_REDUNDANT
      static celsius_float_t analog_to_celsius_redundant(const int16_t raw);
    #endif

    #if HAS_FAN

      static uint8_t fan_speed[FAN_COUNT];
      #define FANS_LOOP(I) LOOP_L_N(I, FAN_COUNT)

      static void set_fan_speed(const uint8_t fan, const uint16_t speed);

      #if ENABLED(REPORT_FAN_CHANGE)
        static void report_fan_speed(const uint8_t fan);
      #endif

      #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)
        static bool fans_paused;
        static uint8_t saved_fan_speed[FAN_COUNT];
      #endif

      #if ENABLED(ADAPTIVE_FAN_SLOWING)
        static uint8_t fan_speed_scaler[FAN_COUNT];
      #endif

      static inline uint8_t scaledFanSpeed(const uint8_t fan, const uint8_t fs) {
        UNUSED(fan); // Potentially unused!
        return (fs * uint16_t(TERN(ADAPTIVE_FAN_SLOWING, fan_speed_scaler[fan], 128))) >> 7;
      }

      static inline uint8_t scaledFanSpeed(const uint8_t fan) {
        return scaledFanSpeed(fan, fan_speed[fan]);
      }

      static constexpr inline uint8_t pwmToPercent(const uint8_t speed) { return ui8_to_percent(speed); }
      static inline uint8_t fanSpeedPercent(const uint8_t fan)          { return ui8_to_percent(fan_speed[fan]); }
      static inline uint8_t scaledFanSpeedPercent(const uint8_t fan)    { return ui8_to_percent(scaledFanSpeed(fan)); }

      #if ENABLED(EXTRA_FAN_SPEED)
        typedef struct { uint8_t saved, speed; } extra_fan_t;
        static extra_fan_t extra_fan_speed[FAN_COUNT];
        static void set_temp_fan_speed(const uint8_t fan, const uint16_t command_or_speed);
      #endif

      #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)
        void set_fans_paused(const bool p);
      #endif

    #endif // HAS_FAN

    static inline void zero_fan_speeds() {
      #if HAS_FAN
        FANS_LOOP(i) set_fan_speed(i, 0);
      #endif
    }

    /**
     * Called from the Temperature ISR
     */
    static void isr();
    static void readings_ready();

    /**
     * Call periodically to manage heaters
     */
    static void manage_heater() _O2; // Added _O2 to work around a compiler error

    /**
     * Preheating hotends
     */
    #if MILLISECONDS_PREHEAT_TIME > 0
      static inline bool is_preheating(const uint8_t E_NAME) {
        return preheat_end_time[HOTEND_INDEX] && PENDING(millis(), preheat_end_time[HOTEND_INDEX]);
      }
      static inline void start_preheat_time(const uint8_t E_NAME) {
        preheat_end_time[HOTEND_INDEX] = millis() + MILLISECONDS_PREHEAT_TIME;
      }
      static inline void reset_preheat_time(const uint8_t E_NAME) {
        preheat_end_time[HOTEND_INDEX] = 0;
      }
    #else
      #define is_preheating(n) (false)
    #endif

    //high level conversion routines, for use outside of temperature.cpp
    //inline so that there is no performance decrease.
    //deg=degreeCelsius

    static inline celsius_float_t degHotend(const uint8_t E_NAME) {
      return TERN0(HAS_HOTEND, temp_hotend[HOTEND_INDEX].celsius);
    }

    static inline celsius_t wholeDegHotend(const uint8_t E_NAME) {
      return TERN0(HAS_HOTEND, static_cast<celsius_t>(temp_hotend[HOTEND_INDEX].celsius + 0.5f));
    }

    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      static inline int16_t rawHotendTemp(const uint8_t E_NAME) {
        return TERN0(HAS_HOTEND, temp_hotend[HOTEND_INDEX].raw);
      }
    #endif

    static inline celsius_t degTargetHotend(const uint8_t E_NAME) {
      return TERN0(HAS_HOTEND, temp_hotend[HOTEND_INDEX].target);
    }

    #if HAS_HOTEND

      static void setTargetHotend(const celsius_t celsius, const uint8_t E_NAME) {
        const uint8_t ee = HOTEND_INDEX;
        #if MILLISECONDS_PREHEAT_TIME > 0
          if (celsius == 0)
            reset_preheat_time(ee);
          else if (temp_hotend[ee].target == 0)
            start_preheat_time(ee);
        #endif
        TERN_(AUTO_POWER_CONTROL, if (celsius) powerManager.power_on());
        temp_hotend[ee].target = _MIN(celsius, hotend_max_target(ee));
        start_watching_hotend(ee);
      }

      static inline bool isHeatingHotend(const uint8_t E_NAME) {
        return temp_hotend[HOTEND_INDEX].target > temp_hotend[HOTEND_INDEX].celsius;
      }

      static inline bool isCoolingHotend(const uint8_t E_NAME) {
        return temp_hotend[HOTEND_INDEX].target < temp_hotend[HOTEND_INDEX].celsius;
      }

      #if HAS_TEMP_HOTEND
        static bool wait_for_hotend(const uint8_t target_extruder, const bool no_wait_for_cooling=true
          OPTARG(G26_CLICK_CAN_CANCEL, const bool click_to_cancel=false)
        );

        #if ENABLED(WAIT_FOR_HOTEND)
          static void wait_for_hotend_heating(const uint8_t target_extruder);
        #endif
      #endif

      static inline bool still_heating(const uint8_t e) {
        return degTargetHotend(e) > TEMP_HYSTERESIS && ABS(wholeDegHotend(e) - degTargetHotend(e)) > TEMP_HYSTERESIS;
      }

      static inline bool degHotendNear(const uint8_t e, const celsius_t temp) {
        return ABS(wholeDegHotend(e) - temp) < (TEMP_HYSTERESIS);
      }

      // Start watching a Hotend to make sure it's really heating up
      static inline void start_watching_hotend(const uint8_t E_NAME) {
        UNUSED(HOTEND_INDEX);
        #if WATCH_HOTENDS
          watch_hotend[HOTEND_INDEX].restart(degHotend(HOTEND_INDEX), degTargetHotend(HOTEND_INDEX));
        #endif
      }

    #endif // HAS_HOTEND

    #if HAS_HEATED_BED

      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        static inline int16_t rawBedTemp()    { return temp_bed.raw; }
      #endif
      static inline celsius_float_t degBed()  { return temp_bed.celsius; }
      static inline celsius_t wholeDegBed()   { return static_cast<celsius_t>(degBed() + 0.5f); }
      static inline celsius_t degTargetBed()  { return temp_bed.target; }
      static inline bool isHeatingBed()       { return temp_bed.target > temp_bed.celsius; }
      static inline bool isCoolingBed()       { return temp_bed.target < temp_bed.celsius; }

      // Start watching the Bed to make sure it's really heating up
      static inline void start_watching_bed() { TERN_(WATCH_BED, watch_bed.restart(degBed(), degTargetBed())); }

      static void setTargetBed(const celsius_t celsius) {
        TERN_(AUTO_POWER_CONTROL, if (celsius) powerManager.power_on());
        temp_bed.target = _MIN(celsius, BED_MAX_TARGET);
        start_watching_bed();
      }

      static bool wait_for_bed(const bool no_wait_for_cooling=true
        OPTARG(G26_CLICK_CAN_CANCEL, const bool click_to_cancel=false)
      );

      static void wait_for_bed_heating();

      static inline bool degBedNear(const celsius_t temp) {
        return ABS(wholeDegBed() - temp) < (TEMP_BED_HYSTERESIS);
      }

    #endif // HAS_HEATED_BED

    #if HAS_TEMP_PROBE
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        static inline int16_t rawProbeTemp()    { return temp_probe.raw; }
      #endif
      static inline celsius_float_t degProbe()  { return temp_probe.celsius; }
      static inline celsius_t wholeDegProbe()   { return static_cast<celsius_t>(degProbe() + 0.5f); }
      static inline bool isProbeBelowTemp(const celsius_t target_temp) { return wholeDegProbe() < target_temp; }
      static inline bool isProbeAboveTemp(const celsius_t target_temp) { return wholeDegProbe() > target_temp; }
      static bool wait_for_probe(const celsius_t target_temp, bool no_wait_for_cooling=true);
    #endif

    #if HAS_TEMP_CHAMBER
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        static inline int16_t rawChamberTemp()      { return temp_chamber.raw; }
      #endif
      static inline celsius_float_t degChamber()    { return temp_chamber.celsius; }
      static inline celsius_t wholeDegChamber()     { return static_cast<celsius_t>(degChamber() + 0.5f); }
      #if HAS_HEATED_CHAMBER
        static inline celsius_t degTargetChamber()  { return temp_chamber.target; }
        static inline bool isHeatingChamber()       { return temp_chamber.target > temp_chamber.celsius; }
        static inline bool isCoolingChamber()       { return temp_chamber.target < temp_chamber.celsius; }
        static bool wait_for_chamber(const bool no_wait_for_cooling=true);
      #endif
    #endif

    #if HAS_HEATED_CHAMBER
      static void setTargetChamber(const celsius_t celsius) {
        temp_chamber.target = _MIN(celsius, CHAMBER_MAX_TARGET);
        start_watching_chamber();
      }
      // Start watching the Chamber to make sure it's really heating up
      static inline void start_watching_chamber() { TERN_(WATCH_CHAMBER, watch_chamber.restart(degChamber(), degTargetChamber())); }
    #endif

    #if HAS_TEMP_COOLER
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        static inline int16_t rawCoolerTemp()     { return temp_cooler.raw; }
      #endif
      static inline celsius_float_t degCooler()   { return temp_cooler.celsius; }
      static inline celsius_t wholeDegCooler()    { return static_cast<celsius_t>(temp_cooler.celsius + 0.5f); }
      #if HAS_COOLER
        static inline celsius_t degTargetCooler() { return temp_cooler.target; }
        static inline bool isLaserHeating()       { return temp_cooler.target > temp_cooler.celsius; }
        static inline bool isLaserCooling()       { return temp_cooler.target < temp_cooler.celsius; }
        static bool wait_for_cooler(const bool no_wait_for_cooling=true);
      #endif
    #endif

    #if HAS_TEMP_REDUNDANT
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        static inline int16_t rawRedundantTemp()         { return temp_redundant.raw; }
        static inline int16_t rawRedundanTargetTemp()    { return (*temp_redundant.target).raw; }
      #endif
      static inline celsius_float_t degRedundant()       { return temp_redundant.celsius; }
      static inline celsius_float_t degRedundantTarget() { return (*temp_redundant.target).celsius; }
      static inline celsius_t wholeDegRedundant()        { return static_cast<celsius_t>(temp_redundant.celsius + 0.5f); }
      static inline celsius_t wholeDegRedundantTarget()  { return static_cast<celsius_t>((*temp_redundant.target).celsius + 0.5f); }
    #endif

    #if HAS_COOLER
      static inline void setTargetCooler(const celsius_t celsius) {
        temp_cooler.target = constrain(celsius, COOLER_MIN_TARGET, COOLER_MAX_TARGET);
        start_watching_cooler();
      }
      // Start watching the Cooler to make sure it's really cooling down
      static inline void start_watching_cooler() { TERN_(WATCH_COOLER, watch_cooler.restart(degCooler(), degTargetCooler())); }
    #endif

    /**
     * The software PWM power for a heater
     */
    static int16_t getHeaterPower(const heater_id_t heater_id);

    /**
     * Switch off all heaters, set all target temperatures to 0
     */
    static void disable_all_heaters();

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Methods to check if heaters are enabled, indicating an active job
       */
      static bool auto_job_over_threshold();
      static void auto_job_check_timer(const bool can_start, const bool can_stop);
    #endif

    /**
     * Perform auto-tuning for hotend or bed in response to M303
     */
    #if HAS_PID_HEATING

      #if ANY(PID_DEBUG, PID_BED_DEBUG, PID_CHAMBER_DEBUG)
        static bool pid_debug_flag;
      #endif

      static void PID_autotune(const celsius_t target, const heater_id_t heater_id, const int8_t ncycles, const bool set_result=false);

      #if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
        static bool adaptive_fan_slowing;
      #elif ENABLED(ADAPTIVE_FAN_SLOWING)
        static constexpr bool adaptive_fan_slowing = true;
      #endif

      /**
       * Update the temp manager when PID values change
       */
      #if ENABLED(PIDTEMP)
        static inline void updatePID() {
          TERN_(PID_EXTRUSION_SCALING, last_e_position = 0);
        }
      #endif

    #endif

    #if ENABLED(PROBING_HEATERS_OFF)
      static void pause_heaters(const bool p);
    #endif

    #if HEATER_IDLE_HANDLER

      static inline void reset_hotend_idle_timer(const uint8_t E_NAME) {
        heater_idle[HOTEND_INDEX].reset();
        start_watching_hotend(HOTEND_INDEX);
      }

      #if HAS_HEATED_BED
        static inline void reset_bed_idle_timer() {
          heater_idle[IDLE_INDEX_BED].reset();
          start_watching_bed();
        }
      #endif

    #endif // HEATER_IDLE_HANDLER

    #if HAS_TEMP_SENSOR
      static void print_heater_states(const uint8_t target_extruder
        OPTARG(HAS_TEMP_REDUNDANT, const bool include_r=false)
      );
      #if ENABLED(AUTO_REPORT_TEMPERATURES)
        struct AutoReportTemp { static void report(); };
        static AutoReporter<AutoReportTemp> auto_reporter;
      #endif
    #endif

    #if HAS_HOTEND && HAS_STATUS_MESSAGE
      static void set_heating_message(const uint8_t e);
    #else
      static inline void set_heating_message(const uint8_t) {}
    #endif

    #if HAS_LCD_MENU && HAS_TEMPERATURE
      static void lcd_preheat(const uint8_t e, const int8_t indh, const int8_t indb);
    #endif

  private:

    // Reading raw temperatures and converting to Celsius when ready
    static volatile bool raw_temps_ready;
    static void update_raw_temperatures();
    static void updateTemperaturesFromRawValues();
    static inline bool updateTemperaturesIfReady() {
      if (!raw_temps_ready) return false;
      updateTemperaturesFromRawValues();
      raw_temps_ready = false;
      return true;
    }

    // MAX Thermocouples
    #if HAS_MAX_TC
      #define MAX_TC_COUNT COUNT_ENABLED(TEMP_SENSOR_0_IS_MAX_TC, TEMP_SENSOR_1_IS_MAX_TC, TEMP_SENSOR_REDUNDANT_IS_MAX_TC)
      #if MAX_TC_COUNT > 1
        #define HAS_MULTI_MAX_TC 1
        #define READ_MAX_TC(N) read_max_tc(N)
      #else
        #define READ_MAX_TC(N) read_max_tc()
      #endif
      static int16_t read_max_tc(TERN_(HAS_MULTI_MAX_TC, const uint8_t hindex=0));
    #endif

    static void checkExtruderAutoFans();

    #if HAS_HOTEND
      static float get_pid_output_hotend(const uint8_t e);
    #endif
    #if ENABLED(PIDTEMPBED)
      static float get_pid_output_bed();
    #endif
    #if ENABLED(PIDTEMPCHAMBER)
      static float get_pid_output_chamber();
    #endif

    static void _temp_error(const heater_id_t e, PGM_P const serial_msg, PGM_P const lcd_msg);
    static void min_temp_error(const heater_id_t e);
    static void max_temp_error(const heater_id_t e);

    #define HAS_THERMAL_PROTECTION ANY(THERMAL_PROTECTION_HOTENDS, THERMAL_PROTECTION_CHAMBER, HAS_THERMALLY_PROTECTED_BED, THERMAL_PROTECTION_COOLER)

    #if HAS_THERMAL_PROTECTION

      // Indices and size for the tr_state_machine array. One for each protected heater.
      enum RunawayIndex : int8_t {
        _RI = -1
        #if ENABLED(THERMAL_PROTECTION_HOTENDS)
          #define _RUNAWAY_IND_E(N) ,RUNAWAY_IND_E##N
          REPEAT(HOTENDS, _RUNAWAY_IND_E)
          #undef _RUNAWAY_IND_E
        #endif
        OPTARG(HAS_THERMALLY_PROTECTED_BED, RUNAWAY_IND_BED)
        OPTARG(THERMAL_PROTECTION_CHAMBER, RUNAWAY_IND_CHAMBER)
        OPTARG(THERMAL_PROTECTION_COOLER, RUNAWAY_IND_COOLER)
        , NR_HEATER_RUNAWAY
      };

      // Convert the given heater_id_t to runaway state array index
      static inline RunawayIndex runaway_index_for_id(const int8_t heater_id) {
        TERN_(HAS_THERMALLY_PROTECTED_CHAMBER, if (heater_id == H_CHAMBER) return RUNAWAY_IND_CHAMBER);
        TERN_(HAS_THERMALLY_PROTECTED_CHAMBER, if (heater_id == H_COOLER)  return RUNAWAY_IND_COOLER);
        TERN_(HAS_THERMALLY_PROTECTED_BED,     if (heater_id == H_BED)     return RUNAWAY_IND_BED);
        return (RunawayIndex)_MAX(heater_id, 0);
      }

      enum TRState : char { TRInactive, TRFirstHeating, TRStable, TRRunaway };

      typedef struct {
        millis_t timer = 0;
        TRState state = TRInactive;
        float running_temp;
        void run(const_celsius_float_t current, const_celsius_float_t target, const heater_id_t heater_id, const uint16_t period_seconds, const celsius_t hysteresis_degc);
      } tr_state_machine_t;

      static tr_state_machine_t tr_state_machine[NR_HEATER_RUNAWAY];

    #endif // HAS_THERMAL_PROTECTION
};

extern Temperature thermalManager;
