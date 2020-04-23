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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

#ifndef SOFT_PWM_SCALE
  #define SOFT_PWM_SCALE 0
#endif

#if HOTENDS <= 1
  #define HOTEND_INDEX  0
  #define E_NAME
#else
  #define HOTEND_INDEX  e
  #define E_NAME e
#endif

// Identifiers for other heaters
typedef enum : int8_t {
  INDEX_NONE = -5,
  H_PROBE, H_REDUNDANT, H_CHAMBER, H_BED,
  H_E0, H_E1, H_E2, H_E3, H_E4, H_E5, H_E6, H_E7
} heater_ind_t;

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

#if ENABLED(PIDTEMP)
  #define _PID_Kp(H) Temperature::temp_hotend[H].pid.Kp
  #define _PID_Ki(H) Temperature::temp_hotend[H].pid.Ki
  #define _PID_Kd(H) Temperature::temp_hotend[H].pid.Kd
  #if ENABLED(PID_EXTRUSION_SCALING)
    #define _PID_Kc(H) Temperature::temp_hotend[H].pid.Kc
  #else
    #define _PID_Kc(H) 1
  #endif

  #if ENABLED(PID_FAN_SCALING)
    #define _PID_Kf(H) Temperature::temp_hotend[H].pid.Kf
  #else
    #define _PID_Kf(H) 0
  #endif
#else
  #define _PID_Kp(H) NAN
  #define _PID_Ki(H) NAN
  #define _PID_Kd(H) NAN
  #define _PID_Kc(H) 1
#endif

#define PID_PARAM(F,H) _PID_##F(H)

/**
 * States for ADC reading in the ISR
 */
enum ADCSensorState : char {
  StartSampling,
  #if HAS_TEMP_ADC_0
    PrepareTemp_0, MeasureTemp_0,
  #endif
  #if HAS_HEATED_BED
    PrepareTemp_BED, MeasureTemp_BED,
  #endif
  #if HAS_TEMP_CHAMBER
    PrepareTemp_CHAMBER, MeasureTemp_CHAMBER,
  #endif
  #if HAS_TEMP_PROBE
    PrepareTemp_PROBE, MeasureTemp_PROBE,
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

#if BOTH(HAS_LCD_MENU, G26_MESH_VALIDATION)
  #define G26_CLICK_CAN_CANCEL 1
#endif

// A temperature sensor
typedef struct TempInfo {
  uint16_t acc;
  int16_t raw;
  float celsius;
  inline void reset() { acc = 0; }
  inline void sample(const uint16_t s) { acc += s; }
  inline void update() { raw = acc; }
} temp_info_t;

// A PWM heater with temperature sensor
typedef struct HeaterInfo : public TempInfo {
  int16_t target;
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
  typedef heater_info_t chamber_info_t;
#elif HAS_TEMP_CHAMBER
  typedef temp_info_t chamber_info_t;
#endif

// Heater idle handling
typedef struct {
  millis_t timeout_ms;
  bool timed_out;
  inline void update(const millis_t &ms) { if (!timed_out && timeout_ms && ELAPSED(ms, timeout_ms)) timed_out = true; }
  inline void start(const millis_t &ms) { timeout_ms = millis() + ms; timed_out = false; }
  inline void reset() { timeout_ms = 0; timed_out = false; }
  inline void expire() { start(0); }
} hotend_idle_t;

// Heater watch handling
template <int INCREASE, int HYSTERESIS, millis_t PERIOD>
struct HeaterWatch {
  uint16_t target;
  millis_t next_ms;
  inline bool elapsed(const millis_t &ms) { return next_ms && ELAPSED(ms, next_ms); }
  inline bool elapsed() { return elapsed(millis()); }

  inline void restart(const int16_t curr, const int16_t tgt) {
    if (tgt) {
      const int16_t newtarget = curr + INCREASE;
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

// Temperature sensor read value ranges
typedef struct { int16_t raw_min, raw_max; } raw_range_t;
typedef struct { int16_t mintemp, maxtemp; } celsius_range_t;
typedef struct { int16_t raw_min, raw_max, mintemp, maxtemp; } temp_range_t;

#define THERMISTOR_ABS_ZERO_C           -273.15f  // bbbbrrrrr cold !
#define THERMISTOR_RESISTANCE_NOMINAL_C 25.0f     // mmmmm comfortable

#if HAS_USER_THERMISTORS

  enum CustomThermistorIndex : uint8_t {
    #if ENABLED(HEATER_0_USER_THERMISTOR)
      CTI_HOTEND_0,
    #endif
    #if ENABLED(HEATER_1_USER_THERMISTOR)
      CTI_HOTEND_1,
    #endif
    #if ENABLED(HEATER_2_USER_THERMISTOR)
      CTI_HOTEND_2,
    #endif
    #if ENABLED(HEATER_3_USER_THERMISTOR)
      CTI_HOTEND_3,
    #endif
    #if ENABLED(HEATER_4_USER_THERMISTOR)
      CTI_HOTEND_4,
    #endif
    #if ENABLED(HEATER_5_USER_THERMISTOR)
      CTI_HOTEND_5,
    #endif
    #if ENABLED(HEATER_BED_USER_THERMISTOR)
      CTI_BED,
    #endif
    #if ENABLED(HEATER_PROBE_USER_THERMISTOR)
      CTI_PROBE,
    #endif
    #if ENABLED(HEATER_CHAMBER_USER_THERMISTOR)
      CTI_CHAMBER,
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
      #define HOTEND_TEMPS (HOTENDS + ENABLED(TEMP_SENSOR_1_AS_REDUNDANT))
      static hotend_info_t temp_hotend[HOTEND_TEMPS];
    #endif
    TERN_(HAS_HEATED_BED, static bed_info_t temp_bed);
    TERN_(HAS_TEMP_PROBE, static probe_info_t temp_probe);
    TERN_(HAS_TEMP_CHAMBER, static chamber_info_t temp_chamber);

    TERN_(AUTO_POWER_E_FANS, static uint8_t autofan_speed[HOTENDS]);
    TERN_(AUTO_POWER_CHAMBER_FAN, static uint8_t chamberfan_speed);

    #if ENABLED(FAN_SOFT_PWM)
      static uint8_t soft_pwm_amount_fan[FAN_COUNT],
                     soft_pwm_count_fan[FAN_COUNT];
    #endif

    #if ENABLED(PREVENT_COLD_EXTRUSION)
      static bool allow_cold_extrude;
      static int16_t extrude_min_temp;
      FORCE_INLINE static bool tooCold(const int16_t temp) { return allow_cold_extrude ? false : temp < extrude_min_temp; }
      FORCE_INLINE static bool tooColdToExtrude(const uint8_t E_NAME) {
        return tooCold(degHotend(HOTEND_INDEX));
      }
      FORCE_INLINE static bool targetTooColdToExtrude(const uint8_t E_NAME) {
        return tooCold(degTargetHotend(HOTEND_INDEX));
      }
    #else
      FORCE_INLINE static bool tooColdToExtrude(const uint8_t) { return false; }
      FORCE_INLINE static bool targetTooColdToExtrude(const uint8_t) { return false; }
    #endif

    FORCE_INLINE static bool hotEnoughToExtrude(const uint8_t e) { return !tooColdToExtrude(e); }
    FORCE_INLINE static bool targetHotEnoughToExtrude(const uint8_t e) { return !targetTooColdToExtrude(e); }

    #if HEATER_IDLE_HANDLER
      static hotend_idle_t hotend_idle[HOTENDS];
      TERN_(HAS_HEATED_BED, static hotend_idle_t bed_idle);
      TERN_(HAS_HEATED_CHAMBER, static hotend_idle_t chamber_idle);
    #endif

  private:

    TERN_(EARLY_WATCHDOG, static bool inited);   // If temperature controller is running

    static volatile bool raw_temps_ready;

    TERN_(WATCH_HOTENDS, static hotend_watch_t watch_hotend[HOTENDS]);

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      static uint16_t redundant_temperature_raw;
      static float redundant_temperature;
    #endif

    #if ENABLED(PID_EXTRUSION_SCALING)
      static int32_t last_e_position, lpq[LPQ_MAX_LEN];
      static lpq_ptr_t lpq_ptr;
    #endif

    TERN_(HAS_HOTEND, static temp_range_t temp_range[HOTENDS]);

    #if HAS_HEATED_BED
      TERN_(WATCH_BED, static bed_watch_t watch_bed);
      TERN(PIDTEMPBED,,static millis_t next_bed_check_ms);
      #ifdef BED_MINTEMP
        static int16_t mintemp_raw_BED;
      #endif
      #ifdef BED_MAXTEMP
        static int16_t maxtemp_raw_BED;
      #endif
    #endif

    #if HAS_HEATED_CHAMBER
      TERN_(WATCH_CHAMBER, static chamber_watch_t watch_chamber);
      static millis_t next_chamber_check_ms;
      #ifdef CHAMBER_MINTEMP
        static int16_t mintemp_raw_CHAMBER;
      #endif
      #ifdef CHAMBER_MAXTEMP
        static int16_t maxtemp_raw_CHAMBER;
      #endif
    #endif

    #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
      static uint8_t consecutive_low_temperature_error[HOTENDS];
    #endif

    #ifdef MILLISECONDS_PREHEAT_TIME
      static millis_t preheat_end_time[HOTENDS];
    #endif

    TERN_(HAS_AUTO_FAN, static millis_t next_auto_fan_check_ms);

    TERN_(PROBING_HEATERS_OFF, static bool paused);

  public:
    #if HAS_ADC_BUTTONS
      static uint32_t current_ADCKey_raw;
      static uint8_t ADCKey_count;
    #endif

    TERN_(PID_EXTRUSION_SCALING, static int16_t lpq_len);

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
      static float user_thermistor_to_deg_c(const uint8_t t_index, const int raw);
      static bool set_pull_up_res(int8_t t_index, float value) {
        //if (!WITHIN(t_index, 0, USER_THERMISTORS - 1)) return false;
        if (!WITHIN(value, 1, 1000000)) return false;
        user_thermistor[t_index].series_res = value;
        return true;
      }
      static bool set_res25(int8_t t_index, float value) {
        if (!WITHIN(value, 1, 10000000)) return false;
        user_thermistor[t_index].res_25 = value;
        user_thermistor[t_index].pre_calc = true;
        return true;
      }
      static bool set_beta(int8_t t_index, float value) {
        if (!WITHIN(value, 1, 1000000)) return false;
        user_thermistor[t_index].beta = value;
        user_thermistor[t_index].pre_calc = true;
        return true;
      }
      static bool set_sh_coeff(int8_t t_index, float value) {
        if (!WITHIN(value, -0.01f, 0.01f)) return false;
        user_thermistor[t_index].sh_c_coeff = value;
        user_thermistor[t_index].pre_calc = true;
        return true;
      }
    #endif

    #if HAS_HOTEND
      static float analog_to_celsius_hotend(const int raw, const uint8_t e);
    #endif

    #if HAS_HEATED_BED
      static float analog_to_celsius_bed(const int raw);
    #endif
    #if HAS_TEMP_PROBE
      static float analog_to_celsius_probe(const int raw);
    #endif
    #if HAS_TEMP_CHAMBER
      static float analog_to_celsius_chamber(const int raw);
    #endif

    #if FAN_COUNT > 0

      static uint8_t fan_speed[FAN_COUNT];
      #define FANS_LOOP(I) LOOP_L_N(I, FAN_COUNT)

      static void set_fan_speed(const uint8_t target, const uint16_t speed);

      #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)
        static bool fans_paused;
        static uint8_t saved_fan_speed[FAN_COUNT];
      #endif

      static constexpr inline uint8_t fanPercent(const uint8_t speed) { return ui8_to_percent(speed); }

      TERN_(ADAPTIVE_FAN_SLOWING, static uint8_t fan_speed_scaler[FAN_COUNT]);

      static inline uint8_t scaledFanSpeed(const uint8_t target, const uint8_t fs) {
        UNUSED(target); // Potentially unused!
        return (fs * uint16_t(
          #if ENABLED(ADAPTIVE_FAN_SLOWING)
            fan_speed_scaler[target]
          #else
            128
          #endif
        )) >> 7;
      }

      static inline uint8_t scaledFanSpeed(const uint8_t target) {
        return scaledFanSpeed(target, fan_speed[target]);
      }

      #if ENABLED(EXTRA_FAN_SPEED)
        static uint8_t old_fan_speed[FAN_COUNT], new_fan_speed[FAN_COUNT];
        static void set_temp_fan_speed(const uint8_t fan, const uint16_t tmp_temp);
      #endif

      #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)
        void set_fans_paused(const bool p);
      #endif

    #endif // FAN_COUNT > 0

    static inline void zero_fan_speeds() {
      #if FAN_COUNT > 0
        FANS_LOOP(i) set_fan_speed(i, 0);
      #endif
    }

    /**
     * Called from the Temperature ISR
     */
    static void readings_ready();
    static void tick();

    /**
     * Call periodically to manage heaters
     */
    static void manage_heater() _O2; // Added _O2 to work around a compiler error

    /**
     * Preheating hotends
     */
    #ifdef MILLISECONDS_PREHEAT_TIME
      static bool is_preheating(const uint8_t E_NAME) {
        return preheat_end_time[HOTEND_INDEX] && PENDING(millis(), preheat_end_time[HOTEND_INDEX]);
      }
      static void start_preheat_time(const uint8_t E_NAME) {
        preheat_end_time[HOTEND_INDEX] = millis() + MILLISECONDS_PREHEAT_TIME;
      }
      static void reset_preheat_time(const uint8_t E_NAME) {
        preheat_end_time[HOTEND_INDEX] = 0;
      }
    #else
      #define is_preheating(n) (false)
    #endif

    //high level conversion routines, for use outside of temperature.cpp
    //inline so that there is no performance decrease.
    //deg=degreeCelsius

    FORCE_INLINE static float degHotend(const uint8_t E_NAME) {
      return TERN0(HAS_HOTEND, temp_hotend[HOTEND_INDEX].celsius);
    }

    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      FORCE_INLINE static int16_t rawHotendTemp(const uint8_t E_NAME) {
        return TERN0(HAS_HOTEND, temp_hotend[HOTEND_INDEX].raw);
      }
    #endif

    FORCE_INLINE static int16_t degTargetHotend(const uint8_t E_NAME) {
      return TERN0(HAS_HOTEND, temp_hotend[HOTEND_INDEX].target);
    }

    #if WATCH_HOTENDS
      static void start_watching_hotend(const uint8_t e=0);
    #else
      static inline void start_watching_hotend(const uint8_t=0) {}
    #endif

    #if HAS_HOTEND

      static void setTargetHotend(const int16_t celsius, const uint8_t E_NAME) {
        const uint8_t ee = HOTEND_INDEX;
        #ifdef MILLISECONDS_PREHEAT_TIME
          if (celsius == 0)
            reset_preheat_time(ee);
          else if (temp_hotend[ee].target == 0)
            start_preheat_time(ee);
        #endif
        TERN_(AUTO_POWER_CONTROL, powerManager.power_on());
        temp_hotend[ee].target = _MIN(celsius, temp_range[ee].maxtemp - 15);
        start_watching_hotend(ee);
      }

      FORCE_INLINE static bool isHeatingHotend(const uint8_t E_NAME) {
        return temp_hotend[HOTEND_INDEX].target > temp_hotend[HOTEND_INDEX].celsius;
      }

      FORCE_INLINE static bool isCoolingHotend(const uint8_t E_NAME) {
        return temp_hotend[HOTEND_INDEX].target < temp_hotend[HOTEND_INDEX].celsius;
      }

      #if HAS_TEMP_HOTEND
        static bool wait_for_hotend(const uint8_t target_extruder, const bool no_wait_for_cooling=true
          #if G26_CLICK_CAN_CANCEL
            , const bool click_to_cancel=false
          #endif
        );
      #endif

      FORCE_INLINE static bool still_heating(const uint8_t e) {
        return degTargetHotend(e) > TEMP_HYSTERESIS && ABS(degHotend(e) - degTargetHotend(e)) > TEMP_HYSTERESIS;
      }

    #endif // HOTENDS

    #if HAS_HEATED_BED

      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawBedTemp()  { return temp_bed.raw; }
      #endif
      FORCE_INLINE static float degBed()          { return temp_bed.celsius; }
      FORCE_INLINE static int16_t degTargetBed()  { return temp_bed.target; }
      FORCE_INLINE static bool isHeatingBed()     { return temp_bed.target > temp_bed.celsius; }
      FORCE_INLINE static bool isCoolingBed()     { return temp_bed.target < temp_bed.celsius; }

      #if WATCH_BED
        static void start_watching_bed();
      #else
        static inline void start_watching_bed() {}
      #endif

      static void setTargetBed(const int16_t celsius) {
        TERN_(AUTO_POWER_CONTROL, powerManager.power_on());
        temp_bed.target =
          #ifdef BED_MAXTEMP
            _MIN(celsius, BED_MAXTEMP - 10)
          #else
            celsius
          #endif
        ;
        start_watching_bed();
      }

      static bool wait_for_bed(const bool no_wait_for_cooling=true
        #if G26_CLICK_CAN_CANCEL
          , const bool click_to_cancel=false
        #endif
      );

      static void wait_for_bed_heating();

    #endif // HAS_HEATED_BED

    #if HAS_TEMP_PROBE
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawProbeTemp()    { return temp_probe.raw; }
      #endif
      FORCE_INLINE static float degProbe()            { return temp_probe.celsius; }
    #endif

    #if WATCH_PROBE
      static void start_watching_probe();
    #else
      static inline void start_watching_probe() {}
    #endif

    #if HAS_TEMP_CHAMBER
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawChamberTemp()    { return temp_chamber.raw; }
      #endif
      FORCE_INLINE static float degChamber()            { return temp_chamber.celsius; }
      #if HAS_HEATED_CHAMBER
        FORCE_INLINE static int16_t degTargetChamber()  { return temp_chamber.target; }
        FORCE_INLINE static bool isHeatingChamber()     { return temp_chamber.target > temp_chamber.celsius; }
        FORCE_INLINE static bool isCoolingChamber()     { return temp_chamber.target < temp_chamber.celsius; }

        static bool wait_for_chamber(const bool no_wait_for_cooling=true);
      #endif
    #endif // HAS_TEMP_CHAMBER

    #if WATCH_CHAMBER
      static void start_watching_chamber();
    #else
      static inline void start_watching_chamber() {}
    #endif

    #if HAS_HEATED_CHAMBER
      static void setTargetChamber(const int16_t celsius) {
        temp_chamber.target =
          #ifdef CHAMBER_MAXTEMP
            _MIN(celsius, CHAMBER_MAXTEMP - 10)
          #else
            celsius
          #endif
        ;
        start_watching_chamber();
      }
    #endif // HAS_HEATED_CHAMBER

    /**
     * The software PWM power for a heater
     */
    static int16_t getHeaterPower(const heater_ind_t heater);

    /**
     * Switch off all heaters, set all target temperatures to 0
     */
    static void disable_all_heaters();

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Methods to check if heaters are enabled, indicating an active job
       */
      static bool over_autostart_threshold();
      static void check_timer_autostart(const bool can_start, const bool can_stop);
    #endif

    /**
     * Perform auto-tuning for hotend or bed in response to M303
     */
    #if HAS_PID_HEATING
      static void PID_autotune(const float &target, const heater_ind_t hotend, const int8_t ncycles, const bool set_result=false);

      #if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
        static bool adaptive_fan_slowing;
      #elif ENABLED(ADAPTIVE_FAN_SLOWING)
        static constexpr bool adaptive_fan_slowing = true;
      #endif

      /**
       * Update the temp manager when PID values change
       */
      #if ENABLED(PIDTEMP)
        FORCE_INLINE static void updatePID() {
          TERN_(PID_EXTRUSION_SCALING, last_e_position = 0);
        }
      #endif

    #endif

    #if ENABLED(PROBING_HEATERS_OFF)
      static void pause(const bool p);
      FORCE_INLINE static bool is_paused() { return paused; }
    #endif

    #if HEATER_IDLE_HANDLER

      static void reset_hotend_idle_timer(const uint8_t E_NAME) {
        hotend_idle[HOTEND_INDEX].reset();
        start_watching_hotend(HOTEND_INDEX);
      }

      #if HAS_HEATED_BED
        static void reset_bed_idle_timer() {
          bed_idle.reset();
          start_watching_bed();
        }
      #endif

    #endif // HEATER_IDLE_HANDLER

    #if HAS_TEMP_SENSOR
      static void print_heater_states(const uint8_t target_extruder
        #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
          , const bool include_r=false
        #endif
      );
      #if ENABLED(AUTO_REPORT_TEMPERATURES)
        static uint8_t auto_report_temp_interval;
        static millis_t next_temp_report_ms;
        static void auto_report_temperatures();
        static inline void set_auto_report_interval(uint8_t v) {
          NOMORE(v, 60);
          auto_report_temp_interval = v;
          next_temp_report_ms = millis() + 1000UL * v;
        }
      #endif
    #endif

    TERN_(HAS_DISPLAY, static void set_heating_message(const uint8_t e));

  private:
    static void update_raw_temperatures();
    static void updateTemperaturesFromRawValues();

    #define HAS_MAX6675 EITHER(HEATER_0_USES_MAX6675, HEATER_1_USES_MAX6675)
    #if HAS_MAX6675
      #if BOTH(HEATER_0_USES_MAX6675, HEATER_1_USES_MAX6675)
        #define COUNT_6675 2
      #else
        #define COUNT_6675 1
      #endif
      #if COUNT_6675 > 1
        #define READ_MAX6675(N) read_max6675(N)
      #else
        #define READ_MAX6675(N) read_max6675()
      #endif
      static int read_max6675(
        #if COUNT_6675 > 1
          const uint8_t hindex=0
        #endif
      );
    #endif

    static void checkExtruderAutoFans();

    static float get_pid_output_hotend(const uint8_t e);

    TERN_(PIDTEMPBED, static float get_pid_output_bed());

    TERN_(HAS_HEATED_CHAMBER, static float get_pid_output_chamber());

    static void _temp_error(const heater_ind_t e, PGM_P const serial_msg, PGM_P const lcd_msg);
    static void min_temp_error(const heater_ind_t e);
    static void max_temp_error(const heater_ind_t e);

    #define HAS_THERMAL_PROTECTION (EITHER(THERMAL_PROTECTION_HOTENDS, THERMAL_PROTECTION_CHAMBER) || HAS_THERMALLY_PROTECTED_BED)

    #if HAS_THERMAL_PROTECTION

      enum TRState : char { TRInactive, TRFirstHeating, TRStable, TRRunaway };

      typedef struct {
        millis_t timer = 0;
        TRState state = TRInactive;
      } tr_state_machine_t;

      TERN_(THERMAL_PROTECTION_HOTENDS, static tr_state_machine_t tr_state_machine[HOTENDS]);
      TERN_(HAS_THERMALLY_PROTECTED_BED, static tr_state_machine_t tr_state_machine_bed);
      TERN_(THERMAL_PROTECTION_CHAMBER, static tr_state_machine_t tr_state_machine_chamber);

      static void thermal_runaway_protection(tr_state_machine_t &state, const float &current, const float &target, const heater_ind_t heater_id, const uint16_t period_seconds, const uint16_t hysteresis_degc);

    #endif // HAS_THERMAL_PROTECTION
};

extern Temperature thermalManager;
