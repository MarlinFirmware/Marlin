/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  #define E_UNUSED() UNUSED(e)
#else
  #define HOTEND_INDEX  e
  #define E_UNUSED()
#endif

// PID storage
typedef struct { float Kp, Ki, Kd;     } PID_t;
typedef struct { float Kp, Ki, Kd, Kc; } PIDC_t;
#if ENABLED(PID_EXTRUSION_SCALING)
  typedef PIDC_t hotend_pid_t;
  #if LPQ_MAX_LEN > 255
    typedef uint16_t lpq_ptr_t;
  #else
    typedef uint8_t lpq_ptr_t;
  #endif
#else
  typedef PID_t hotend_pid_t;
#endif

#define DUMMY_PID_VALUE 3000.0f

#if ENABLED(PIDTEMP)
  #define _PID_Kp(H) Temperature::temp_hotend[H].pid.Kp
  #define _PID_Ki(H) Temperature::temp_hotend[H].pid.Ki
  #define _PID_Kd(H) Temperature::temp_hotend[H].pid.Kd
  #if ENABLED(PID_EXTRUSION_SCALING)
    #define _PID_Kc(H) Temperature::temp_hotend[H].pid.Kc
  #else
    #define _PID_Kc(H) 1
  #endif
#else
  #define _PID_Kp(H) DUMMY_PID_VALUE
  #define _PID_Ki(H) DUMMY_PID_VALUE
  #define _PID_Kd(H) DUMMY_PID_VALUE
  #define _PID_Kc(H) 1
#endif

#define PID_PARAM(F,H) _PID_##F(H)

/**
 * States for ADC reading in the ISR
 */
enum ADCSensorState : char {
  StartSampling,
  #if HAS_TEMP_ADC_0
    PrepareTemp_0,
    MeasureTemp_0,
  #endif
  #if HAS_HEATED_BED
    PrepareTemp_BED,
    MeasureTemp_BED,
  #endif
  #if HAS_TEMP_CHAMBER
    PrepareTemp_CHAMBER,
    MeasureTemp_CHAMBER,
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
  #if HAS_TEMP_ADC_5
    PrepareTemp_5,
    MeasureTemp_5,
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    Prepare_FILWIDTH,
    Measure_FILWIDTH,
  #endif
  #if HAS_ADC_BUTTONS
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
  #define PID_K2 (1-float(PID_K1))
  #define PID_dT ((OVERSAMPLENR * float(ACTUAL_ADC_SAMPLES)) / TEMP_TIMER_FREQUENCY)

  // Apply the scale factors to the PID values
  #define scalePID_i(i)   ( float(i) * PID_dT )
  #define unscalePID_i(i) ( float(i) / PID_dT )
  #define scalePID_d(d)   ( float(d) / PID_dT )
  #define unscalePID_d(d) ( float(d) * PID_dT )
#endif

#define G26_CLICK_CAN_CANCEL (HAS_LCD_MENU && ENABLED(G26_MESH_VALIDATION))

enum TempIndex : uint8_t {
  #if HOTENDS > 0
    TEMP_E0,
    #if HOTENDS > 1
      TEMP_E1,
      #if HOTENDS > 2
        TEMP_E2,
        #if HOTENDS > 3
          TEMP_E3,
          #if HOTENDS > 4
            TEMP_E4,
            #if HOTENDS > 5
              TEMP_E5,
            #endif
          #endif
        #endif
      #endif
    #endif
  #endif
  #if HAS_HEATED_BED
    TEMP_BED,
  #endif
  #if HAS_HEATED_CHAMBER
    TEMP_CHAMBER,
  #endif
  tempCOUNT
};

// A temperature sensor
typedef struct TempInfo {
  uint16_t acc;
  int16_t raw;
  float current;
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
#if HAS_TEMP_CHAMBER
  #if HAS_HEATED_CHAMBER
    #if ENABLED(PIDTEMPCHAMBER)
      typedef struct PIDHeaterInfo<PID_t> chamber_info_t;
    #else
      typedef heater_info_t chamber_info_t;
    #endif
  #else
    typedef temp_info_t chamber_info_t;
  #endif
#endif

// Heater idle handling
typedef struct {
  millis_t timeout_ms;
  bool timed_out;
  inline void update(const millis_t &ms) { if (!timed_out && timeout_ms && ELAPSED(ms, timeout_ms)) timed_out = true; }
  inline void start(const millis_t &ms) { timeout_ms = millis() + ms; timed_out = false; }
  inline void reset() { timeout_ms = 0; timed_out = false; }
  inline void expire() { start(0); }
} heater_idle_t;

// Heater watch handling
typedef struct {
  uint16_t target;
  millis_t next_ms;
  inline bool elapsed(const millis_t &ms) { return next_ms && ELAPSED(ms, next_ms); }
  inline bool elapsed() { return elapsed(millis()); }
} heater_watch_t;

// Temperature sensor read value ranges
typedef struct { int16_t raw_min, raw_max; } raw_range_t;
typedef struct { int16_t mintemp, maxtemp; } celsius_range_t;
typedef struct { int16_t raw_min, raw_max, mintemp, maxtemp; } temp_range_t;

class Temperature {

  public:

    static volatile bool in_temp_isr;

    static hotend_info_t temp_hotend[HOTENDS];

    #if HAS_HEATED_BED
      static bed_info_t temp_bed;
    #endif

    #if HAS_TEMP_CHAMBER
      static chamber_info_t temp_chamber;
    #endif

    #if ENABLED(AUTO_POWER_E_FANS)
      static uint8_t autofan_speed[HOTENDS];
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      static uint8_t soft_pwm_amount_fan[FAN_COUNT],
                     soft_pwm_count_fan[FAN_COUNT];
    #endif

    #if ENABLED(BABYSTEPPING)
      static volatile int16_t babystepsTodo[3];
    #endif

    #if ENABLED(PREVENT_COLD_EXTRUSION)
      static bool allow_cold_extrude;
      static int16_t extrude_min_temp;
      FORCE_INLINE static bool tooCold(const int16_t temp) { return allow_cold_extrude ? false : temp < extrude_min_temp; }
      FORCE_INLINE static bool tooColdToExtrude(const uint8_t e) {
        E_UNUSED();
        return tooCold(degHotend(HOTEND_INDEX));
      }
      FORCE_INLINE static bool targetTooColdToExtrude(const uint8_t e) {
        E_UNUSED();
        return tooCold(degTargetHotend(HOTEND_INDEX));
      }
    #else
      FORCE_INLINE static bool tooColdToExtrude(const uint8_t e) { UNUSED(e); return false; }
      FORCE_INLINE static bool targetTooColdToExtrude(const uint8_t e) { UNUSED(e); return false; }
    #endif

    FORCE_INLINE static bool hotEnoughToExtrude(const uint8_t e) { return !tooColdToExtrude(e); }
    FORCE_INLINE static bool targetHotEnoughToExtrude(const uint8_t e) { return !targetTooColdToExtrude(e); }

    #if HEATER_IDLE_HANDLER
      static heater_idle_t hotend_idle[HOTENDS];
      #if HAS_HEATED_BED
        static heater_idle_t bed_idle;
      #endif
      #if HAS_HEATED_CHAMBER
        static heater_idle_t chamber_idle;
      #endif
    #endif

  private:

    #if EARLY_WATCHDOG
      static bool inited;   // If temperature controller is running
    #endif

    static volatile bool temp_meas_ready;

    #if WATCH_HOTENDS
      static heater_watch_t watch_hotend[HOTENDS];
    #endif

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      static uint16_t redundant_temperature_raw;
      static float redundant_temperature;
    #endif

    #if ENABLED(PID_EXTRUSION_SCALING)
      static int32_t last_e_position, lpq[LPQ_MAX_LEN];
      static lpq_ptr_t lpq_ptr;
    #endif

    static temp_range_t temp_range[HOTENDS];

    #if HAS_HEATED_BED
      #if WATCH_BED
        static heater_watch_t watch_bed;
      #endif
      #if DISABLED(PIDTEMPBED)
        static millis_t next_bed_check_ms;
      #endif
      #ifdef BED_MINTEMP
        static int16_t mintemp_raw_BED;
      #endif
      #ifdef BED_MAXTEMP
        static int16_t maxtemp_raw_BED;
      #endif
    #endif

    #if HAS_HEATED_CHAMBER
      #if WATCH_CHAMBER
        static heater_watch_t watch_chamber;
      #endif
      #if DISABLED(PIDTEMPCHAMBER)
        static millis_t next_chamber_check_ms;
      #endif
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

  public:
    #if HAS_ADC_BUTTONS
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
    static float analog_to_celsius_hotend(const int raw, const uint8_t e);

    #if HAS_HEATED_BED
      static float analog_to_celsius_bed(const int raw);
    #endif
    #if HAS_TEMP_CHAMBER
      static float analog_to_celsius_chamber(const int raw);
    #endif

    #if FAN_COUNT > 0

      static uint8_t fan_speed[FAN_COUNT];
      #define FANS_LOOP(I) LOOP_L_N(I, FAN_COUNT)

      static void set_fan_speed(const uint8_t target, const uint16_t speed);

      #if ENABLED(PROBING_FANS_OFF)
        static bool fans_paused;
        static uint8_t paused_fan_speed[FAN_COUNT];
      #endif

      static constexpr inline uint8_t fanPercent(const uint8_t speed) { return (int(speed) * 100 + 127) / 255; }

      #if ENABLED(ADAPTIVE_FAN_SLOWING)
        static uint8_t fan_speed_scaler[FAN_COUNT];
      #else
        static constexpr uint8_t fan_speed_scaler[FAN_COUNT] = ARRAY_N(FAN_COUNT, 128, 128, 128, 128, 128, 128);
      #endif

      static inline uint8_t lcd_fanSpeedActual(const uint8_t target) {
        return (fan_speed[target] * uint16_t(fan_speed_scaler[target])) >> 7;
      }

      #if ENABLED(EXTRA_FAN_SPEED)
        static uint8_t old_fan_speed[FAN_COUNT], new_fan_speed[FAN_COUNT];
        static void set_temp_fan_speed(const uint8_t fan, const uint16_t tmp_temp);
      #endif

      #if HAS_LCD_MENU

        static uint8_t lcd_tmpfan_speed[
          #if ENABLED(SINGLENOZZLE)
            MAX(EXTRUDERS, FAN_COUNT)
          #else
            FAN_COUNT
          #endif
        ];

        static inline void lcd_setFanSpeed(const uint8_t target) { set_fan_speed(target, lcd_tmpfan_speed[target]); }

        #if HAS_FAN0
          FORCE_INLINE static void lcd_setFanSpeed0() { lcd_setFanSpeed(0); }
        #endif
        #if HAS_FAN1 || (ENABLED(SINGLENOZZLE) && EXTRUDERS > 1)
          FORCE_INLINE static void lcd_setFanSpeed1() { lcd_setFanSpeed(1); }
        #endif
        #if HAS_FAN2 || (ENABLED(SINGLENOZZLE) && EXTRUDERS > 2)
          FORCE_INLINE static void lcd_setFanSpeed2() { lcd_setFanSpeed(2); }
        #endif

      #endif // HAS_LCD_MENU

      #if ENABLED(PROBING_FANS_OFF)
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
        E_UNUSED();
        return preheat_end_time[HOTEND_INDEX] && PENDING(millis(), preheat_end_time[HOTEND_INDEX]);
      }
      static void start_preheat_time(const uint8_t e) {
        E_UNUSED();
        preheat_end_time[HOTEND_INDEX] = millis() + MILLISECONDS_PREHEAT_TIME;
      }
      static void reset_preheat_time(const uint8_t e) {
        E_UNUSED();
        preheat_end_time[HOTEND_INDEX] = 0;
      }
    #else
      #define is_preheating(n) (false)
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      static float analog_to_mm_fil_width();         // Convert raw Filament Width to millimeters
      static int8_t widthFil_to_size_ratio(); // Convert Filament Width (mm) to an extrusion ratio
    #endif


    //high level conversion routines, for use outside of temperature.cpp
    //inline so that there is no performance decrease.
    //deg=degreeCelsius

    FORCE_INLINE static float degHotend(const uint8_t e) {
      E_UNUSED();
      return temp_hotend[HOTEND_INDEX].current;
    }

    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      FORCE_INLINE static int16_t rawHotendTemp(const uint8_t e) {
        E_UNUSED();
        return temp_hotend[HOTEND_INDEX].raw;
      }
    #endif

    FORCE_INLINE static int16_t degTargetHotend(const uint8_t e) {
      E_UNUSED();
      return temp_hotend[HOTEND_INDEX].target;
    }

    #if WATCH_HOTENDS
      static void start_watching_heater(const uint8_t e=0);
    #else
      static inline void start_watching_heater(const uint8_t e=0) { UNUSED(e); }
    #endif

    #if HAS_LCD_MENU
      static inline void start_watching_E0() { start_watching_heater(0); }
      static inline void start_watching_E1() { start_watching_heater(1); }
      static inline void start_watching_E2() { start_watching_heater(2); }
      static inline void start_watching_E3() { start_watching_heater(3); }
      static inline void start_watching_E4() { start_watching_heater(4); }
      static inline void start_watching_E5() { start_watching_heater(5); }
    #endif

    static void setTargetHotend(const int16_t celsius, const uint8_t e) {
      E_UNUSED();
      #ifdef MILLISECONDS_PREHEAT_TIME
        if (celsius == 0)
          reset_preheat_time(HOTEND_INDEX);
        else if (temp_hotend[HOTEND_INDEX].target == 0)
          start_preheat_time(HOTEND_INDEX);
      #endif
      #if ENABLED(AUTO_POWER_CONTROL)
        powerManager.power_on();
      #endif
      temp_hotend[HOTEND_INDEX].target = MIN(celsius, temp_range[HOTEND_INDEX].maxtemp - 15);
      start_watching_heater(HOTEND_INDEX);
    }

    #if WATCH_CHAMBER
      static void start_watching_chamber();
    #else
      static inline void start_watching_chamber() {}
    #endif

    #if HAS_TEMP_CHAMBER
      static void setTargetChamber(const int16_t celsius) {
        #if HAS_HEATED_CHAMBER
          temp_chamber.target =
            #ifdef CHAMBER_MAXTEMP
              min(celsius, CHAMBER_MAXTEMP)
            #else
              celsius
            #endif
          ;
          start_watching_chamber();
        #endif // HAS_HEATED_CHAMBER
      }
    #endif // HAS_TEMP_CHAMBER

    FORCE_INLINE static bool isHeatingHotend(const uint8_t e) {
      E_UNUSED();
      return temp_hotend[HOTEND_INDEX].target > temp_hotend[HOTEND_INDEX].current;
    }

    FORCE_INLINE static bool isCoolingHotend(const uint8_t e) {
      E_UNUSED();
      return temp_hotend[HOTEND_INDEX].target < temp_hotend[HOTEND_INDEX].current;
    }

    #if HAS_TEMP_HOTEND
      static bool wait_for_hotend(const uint8_t target_extruder, const bool no_wait_for_cooling=true
        #if G26_CLICK_CAN_CANCEL
          , const bool click_to_cancel=false
        #endif
      );
    #endif

    #if HAS_HEATED_BED

      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawBedTemp()  { return temp_bed.raw; }
      #endif
      FORCE_INLINE static float degBed()          { return temp_bed.current; }
      FORCE_INLINE static int16_t degTargetBed()  { return temp_bed.target; }
      FORCE_INLINE static bool isHeatingBed()     { return temp_bed.target > temp_bed.current; }
      FORCE_INLINE static bool isCoolingBed()     { return temp_bed.target < temp_bed.current; }

      #if WATCH_BED
        static void start_watching_bed();
      #else
        static inline void start_watching_bed() {}
      #endif

      static void setTargetBed(const int16_t celsius) {
        #if ENABLED(AUTO_POWER_CONTROL)
          powerManager.power_on();
        #endif
        temp_bed.target =
          #ifdef BED_MAXTEMP
            MIN(celsius, BED_MAXTEMP - 10)
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

    #endif // HAS_HEATED_BED

    #if HAS_TEMP_CHAMBER
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawChamberTemp() { return temp_chamber.raw; }
      #endif
      FORCE_INLINE static float degChamber() { return temp_chamber.current; }
      #if HAS_HEATED_CHAMBER
        FORCE_INLINE static bool isHeatingChamber()     { return temp_chamber.target > temp_chamber.current; }
        FORCE_INLINE static bool isCoolingChamber()     { return temp_chamber.target < temp_chamber.current; }
        FORCE_INLINE static int16_t degTargetChamber() {return temp_chamber.target; }
      #endif
    #endif // HAS_TEMP_CHAMBER

    FORCE_INLINE static bool still_heating(const uint8_t e) {
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

      #if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
        static bool adaptive_fan_slowing;
      #elif ENABLED(ADAPTIVE_FAN_SLOWING)
        constexpr static bool adaptive_fan_slowing = true;
      #endif

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
      static void babystep_axis(const AxisEnum axis, const int16_t distance);
    #endif

    #if ENABLED(PROBING_HEATERS_OFF)
      static void pause(const bool p);
      FORCE_INLINE static bool is_paused() { return paused; }
    #endif

    #if HEATER_IDLE_HANDLER

      static void reset_heater_idle_timer(const uint8_t e) {
        E_UNUSED();
        hotend_idle[HOTEND_INDEX].reset();
        start_watching_heater(HOTEND_INDEX);
      }

      #if HAS_HEATED_BED
        static void reset_bed_idle_timer() {
          bed_idle.reset();
          start_watching_bed();
        }
      #endif

    #endif // HEATER_IDLE_HANDLER

    #if HAS_TEMP_SENSOR
      static void print_heater_states(const uint8_t target_extruder);
      #if ENABLED(AUTO_REPORT_TEMPERATURES)
        static uint8_t auto_report_temp_interval;
        static millis_t next_temp_report_ms;
        static void auto_report_temperatures(void);
        static inline void set_auto_report_interval(uint8_t v) {
          NOMORE(v, 60);
          auto_report_temp_interval = v;
          next_temp_report_ms = millis() + 1000UL * v;
        }
      #endif
    #endif

    #if EITHER(ULTRA_LCD, EXTENSIBLE_UI)
      static void set_heating_message(const uint8_t e);
    #endif

  private:
    static void set_current_temp_raw();
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

    static float get_pid_output(const int8_t e);

    #if ENABLED(PIDTEMPBED)
      static float get_pid_output_bed();
    #endif

    #if HAS_HEATED_CHAMBER
      static float get_pid_output_chamber();
    #endif

    static void _temp_error(const int8_t e, PGM_P const serial_msg, PGM_P const lcd_msg);
    static void min_temp_error(const int8_t e);
    static void max_temp_error(const int8_t e);
    #if HAS_TEMP_CHAMBER
      static void chamber_temp_error(const bool max);
    #endif

    #if ENABLED(THERMAL_PROTECTION_HOTENDS) || HAS_THERMALLY_PROTECTED_BED || ENABLED(THERMAL_PROTECTION_CHAMBER)

      enum TRState : char { TRInactive, TRFirstHeating, TRStable, TRRunaway };

      typedef struct {
        millis_t timer = 0;
        TRState state = TRInactive;
      } tr_state_machine_t;

      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        static tr_state_machine_t tr_state_machine[HOTENDS];
      #endif
      #if HAS_THERMALLY_PROTECTED_BED
        static tr_state_machine_t tr_state_machine_bed;
      #endif
      #if ENABLED(THERMAL_PROTECTION_CHAMBER)
        static tr_state_machine_t tr_state_machine_chamber;
      #endif

      static void thermal_runaway_protection(tr_state_machine_t &state, const float &current, const float &target, const int8_t heater_id, const uint16_t period_seconds, const uint16_t hysteresis_degc);

    #endif // THERMAL_PROTECTION
};

extern Temperature thermalManager;
