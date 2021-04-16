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
enum heater_id_t : int8_t {
  INDEX_NONE = -6,
  H_COOLER, H_PROBE, H_REDUNDANT, H_CHAMBER, H_BED,
  H_E0, H_E1, H_E2, H_E3, H_E4, H_E5, H_E6, H_E7
};

/**
 * States for ADC reading in the ISR
 */
enum ADCSensorState : char {
  StartSampling,
  HeatersSampling,
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


// PID storage
// Used for serialization in settings.cpp only, made so it can be stored as-is via memcpy
struct PIDVec { float Kp, Ki, Kd; };
struct PIDVecL : public PIDVec { float Kc, Kf; };

struct NoPID {
  virtual float getKp() const { return 0; } virtual void setKp(float) {}
  virtual float getKi() const { return 0; } virtual void setKi(float) {}
  virtual float getKd() const { return 0; } virtual void setKd(float) {}
  virtual float getKc() const { return 0; } virtual void setKc(float) {}
  virtual float getKf() const { return 0; } virtual void setKf(float) {}
  NO_INLINE void unscaleTo(PIDVec & o)  const { o.Kp = getKp(); o.Ki = unscalePID_i(getKi()); o.Kd = unscalePID_d(getKd()); }
  NO_INLINE void unscaleTo(PIDVecL & o) const { unscaleTo((PIDVec&)o); o.Kc = getKc(); o.Kf = getKf(); }
  NO_INLINE void scaleFrom(const PIDVec & o)  { setKp(o.Kp); setKi(scalePID_i(o.Ki)); setKd(scalePID_d(o.Kd)); }
  NO_INLINE void scaleFrom(const PIDVecL & o) { scaleFrom((PIDVec&)o); setKc(o.Kc); setKf(o.Kf); }
//  virtual ~NoPID() {}
};
#define ACCESSOR(X) float get##X() const { return X; } void set##X(float v) { X = v; }
struct PID_t    : public NoPID   { float Kp = 0, Ki = 0, Kd = 0;  ACCESSOR(Kp); ACCESSOR(Ki); ACCESSOR(Kd); };
struct PIDC_t   : public PID_t   { float Kc = 1.0f;               ACCESSOR(Kc); };
struct PIDF_t   : public PID_t   { float Kf = 0;                  ACCESSOR(Kf); };
struct PIDCF_t  : public PIDF_t  { float Kc = 1.0f;               ACCESSOR(Kc); };
#undef ACCESSOR

#if ENABLED(PID_EXTRUSION_SCALING)
  typedef IF<(LPQ_MAX_LEN > 255), uint16_t, uint8_t>::type lpq_ptr_t;
#endif

#if BOTH(HAS_LCD_MENU, G26_MESH_VALIDATION)
  #define G26_CLICK_CAN_CANCEL 1
#endif

// A temperature sensor
struct temp_info_t {
  uint16_t acc;
  int16_t raw;
  celsius_t celsius;
  inline void reset() { acc = 0; }
  inline void sample(const uint16_t s) { acc += s; }
  inline void update() { raw = acc; }
};

// A PWM heater with temperature sensor
struct heater_info_t : public temp_info_t {
  celsius_t target;
  uint8_t soft_pwm_amount;
};

#define LIST_BY_HOTENDS(V...) LIST_N(HOTENDS, V)

#if ANY(WATCH_HOTENDS, WATCH_BED, WATCH_CHAMBER, WATCH_COOLER)
  #define HAS_WATCH_HEATER 1
#endif

// Unlike heater_id_t which is public, this is used internally as an index in the heater's array.
enum heater_pos_t : uint8_t {
  // Must match the declaration in temperature.cpp, since it's used as an index in the array
  #if HAS_HOTEND
    #define DeclareHEPos(N) HotEndPos##N
    LIST_BY_HOTENDS(DeclareHEPos(0), DeclareHEPos(1), DeclareHEPos(2), DeclareHEPos(3), DeclareHEPos(4), DeclareHEPos(5), DeclareHEPos(6), DeclareHEPos(7)),
    #undef DeclareHEPos
  #endif
  #if HAS_TEMP_PROBE
    TempProbePos,
  #endif
  #if HAS_HEATED_BED
    HeatedBedPos,
  #endif
  #if HAS_HEATED_CHAMBER
    HeatedChamberPos,
  #endif
  #if EITHER(HAS_COOLER, HAS_TEMP_COOLER)
    CoolerPos,
  #endif

  HeatersCount
};

// A generic common class for heaters
struct Heater {
  // The heater information
  heater_info_t info;
  // The position of this heater in the heater's array
  heater_pos_t  heaterPos;
  // The heater id
  heater_id_t   id;
  // The heater pin
  pin_t         pin;
  // Inverting pin
  bool          inverting;

  // HEATER SPECIFIC PARAMETERS BELOW
  // This heater defined hysteresis and window (set in the child class)
  const celsius_t hysteresis, window;
  // The minimum temperature (if applicable)
  const celsius_t minTemp;
  // The maximum temperature (if applicable)
  const celsius_t maxTemp;
  // The residency time for this heater (in milliseconds)
  const uint16_t  residencyTime;
  // The minimum cooling slope in degree per period
  const celsius_t minCoolingSlope;
  // The minimum cooling period (in milliseconds)
  const uint16_t  minCoolingPeriod;
  // Max PID power
  const uint8_t   maxPower;
  // The PID used
  NoPID &         pid;

  bool temp_conditions(millis_t start, millis_t now, bool wants_to_cool) {
    if (residencyTime) return !start || PENDING(now, start + residencyTime);
    return wants_to_cool ? is_cooling() : is_heating();
  }


  /** Get the current temperature */
  celsius_t deg() { return info.celsius; }
  /** Get the target temperature */
  celsius_t degTarget() { return info.target; }
  /** Get the maximum target temperature minus the (expected) overshoot */
  celsius_t maxTarget() const { return maxTemp - celsius_t(HOTEND_OVERSHOOT); }
  /** Get the raw sensor value */
  int16_t  raw() const { return info.raw; }

  #if ENABLED(SHOW_TEMP_ADC_VALUES)
    int16_t raw() { return info.raw; }
  #endif
  /** Set the target temperature */
  void set_target(const celsius_t target) { info.target = target; }
  /** Check if the heater is heating */
  bool is_heating() const { return info.celsius < info.target; }
  /** Check if the heater is cooling */
  bool is_cooling() const { return info.celsius > info.target; }
  /** Write the heater pin */
  void write(bool value) const { if (pin >= 0) digitalWrite(pin, value ^ inverting); }

  // Generic method here cuts the binary size to avoid reproducing similar code
  /** Convert from analog value to celsius */
  celsius_t analog_to_celsius(const int raw);
  /** Wait until the heater has reached the target temperature */
  bool wait_for_temperature(const uint8_t extruder, const bool no_wait_for_cooling = true
          #if G26_CLICK_CAN_CANCEL
            , const bool click_to_cancel = false
          #endif
          );
  /** Get the current PID output power to apply */
  float get_pid_output(const float minPower, const float maxPower, const float bangMax = 0);


  #if ENABLED(PRINTER_EVENT_LEDS)
    LEDColor  pe_heating_start();
    void      pe_heating(float start, float current, float target);
    void      pe_heating_done();
  #endif

  #if HAS_WATCH_HEATER
    celsius_t target;
    millis_t  next_ms;

    celsius_t increase;
    int       period_ms;

    bool elapsed(const millis_t& ms = millis()) const { return next_ms && ELAPSED(ms, next_ms); }
    void restart(const celsius_t curr, const celsius_t tgt) {
      if (tgt != celsius_t(0)) {
        const celsius_t newtarget = curr + increase;
        if (newtarget < tgt - hysteresis - 1) {
          target = newtarget;
          next_ms = millis() + period_ms;
          return;
        }
      }
      next_ms = 0;
    }
    void start_watching() { restart(deg(), degTarget()); }
  #else
    void start_watching() {}
  #endif

  #if HEATER_IDLE_HANDLER
    millis_t timeout_ms;
    bool timed_out;
    void update(const millis_t &ms) { if (!timed_out && timeout_ms && ELAPSED(ms, timeout_ms)) timed_out = true; }
    void start(const millis_t &ms) { timeout_ms = millis() + ms; timed_out = false; }
    void reset() { timeout_ms = 0; timed_out = false; }
    void expire() { start(0); }
  #endif

  Heater(heater_pos_t pos, heater_id_t id, pin_t pin, bool inverting, NoPID & pid, const celsius_t hysteresis, const celsius_t window, const celsius_t minCoolingSlope, const uint16_t minCoolingPeriod, const celsius_t minTemp = minCValue, const celsius_t maxTemp = maxCValue, int residencyTime = 0, uint8_t maxPower = 255
  #if HAS_WATCH_HEATER
    , const celsius_t increase = 0, int period = 0
  #endif
  ) :
    heaterPos(pos), id(id), pin(pin), inverting(inverting), hysteresis(hysteresis), window(window), minTemp(minTemp), maxTemp(maxTemp), residencyTime(residencyTime), minCoolingSlope(minCoolingSlope), minCoolingPeriod(minCoolingPeriod), maxPower(maxPower), pid(pid)
  #if HAS_WATCH_HEATER
    , target(0), next_ms(0), increase(increase), period_ms(SEC_TO_MS(period))
  #endif
  #if HEATER_IDLE_HANDLER
    , timeout_ms(0), timed_out(false)
  #endif
  {}

  // Required for HOTEND_LOOP macro
  explicit operator bool() const { return true; }
};

template <unsigned Pin, bool Inverting, unsigned Hysteresis, unsigned Window, unsigned MinCoolingSlope, unsigned MinCoolingPeriod, unsigned ResidencyTime, unsigned MaxPower, typename PIDType
#if HAS_WATCH_HEATER
  , unsigned Increase, unsigned Period
#endif
>
struct HeaterImpl : public Heater
{
  PIDType _pid;
  // Here we use the special celsius_t constructor that's not converting to fixed point, since the value will arrive already converted from
  // the TEMP_ macros below
  HeaterImpl(heater_pos_t pos, heater_id_t id, const celsius_t minTemp, const celsius_t maxTemp) : Heater(pos, id, Pin, Inverting, _pid, celsius_t(Hysteresis, true), celsius_t(Window, true), celsius_t(MinCoolingSlope, true), minCoolingPeriod, minTemp, maxTemp, ResidencyTime, MaxPower
  #if HAS_WATCH_HEATER
    , celsius_t(Increase, true), Period
  #endif
  ) {}
};
// Used for TEMP_PROBE only
struct HeaterNoPID : public Heater { NoPID _pid; HeaterNoPID(heater_pos_t pos, heater_id_t id) : Heater(pos, id, -1, false, _pid, 0, 0, 0, 0) {} };

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
    #if COOLER_USER_THERMISTOR
      CTI_COOLER,
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

struct Temperature {
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
      FORCE_INLINE static bool tooCold(const celsius_t temp) { return allow_cold_extrude ? false : temp < extrude_min_temp - celsius_t(TEMP_WINDOW); } // Allow for floating point temp window here
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

    #if ENABLED(SINGLENOZZLE_STANDBY_FAN)
      static celsius_t singlenozzle_temp[EXTRUDERS];
      #if HAS_FAN
        static uint8_t singlenozzle_fan_speed[EXTRUDERS];
      #endif
      static void singlenozzle_change(const uint8_t old_tool, const uint8_t new_tool);
    #endif

  private:

    #if ENABLED(EARLY_WATCHDOG)
      static bool inited;   // If temperature controller is running
    #endif

    static volatile bool raw_temps_ready;

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      static uint16_t redundant_temperature_raw;
      static celsius_t redundant_temperature;
    #endif

    #if ENABLED(PID_EXTRUSION_SCALING)
      static int32_t last_e_position, lpq[LPQ_MAX_LEN];
      static lpq_ptr_t lpq_ptr;
    #endif

    #if ENABLED(HAS_HOTEND)
      static temp_range_t temp_range[HOTENDS];
    #endif

    #if HAS_HEATED_BED
      IF_DISABLED(PIDTEMPBED, static millis_t next_bed_check_ms);
      static int16_t mintemp_raw_BED, maxtemp_raw_BED;
    #endif

    #if HAS_HEATED_CHAMBER
      TERN(PIDTEMPCHAMBER,,static millis_t next_chamber_check_ms);
      static int16_t mintemp_raw_CHAMBER, maxtemp_raw_CHAMBER;
    #endif

    #if HAS_COOLER
      static millis_t next_cooler_check_ms, cooler_fan_flush_ms;
      static int16_t mintemp_raw_COOLER, maxtemp_raw_COOLER;
    #endif

    #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
      static uint8_t consecutive_low_temperature_error[HOTENDS];
    #endif

    #ifdef MILLISECONDS_PREHEAT_TIME
      static millis_t preheat_end_time[HOTENDS];
    #endif

    #if ENABLED(HAS_AUTO_FAN)
      static millis_t next_auto_fan_check_ms;
    #endif

    #if ENABLED(PROBING_HEATERS_OFF)
      static bool paused;
    #endif

  public:
    #if HAS_ADC_BUTTONS
      static uint32_t current_ADCKey_raw;
      static uint16_t ADCKey_count;
    #endif

    #if ENABLED(PID_EXTRUSION_SCALING)
      static int16_t lpq_len;
    #endif

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
      static celsius_t user_thermistor_to_deg_c(const uint8_t t_index, const int raw);
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
      static celsius_t analog_to_celsius_hotend(const int raw, const uint8_t e){ return get_heater(HotEndPos0 + e).analog_to_celsius(raw); }
    #endif
    #if HAS_TEMP_PROBE
      static celsius_t analog_to_celsius_probe(const int raw)                  { return get_heater(TempProbePos).analog_to_celsius(raw); }
    #endif
    #if HAS_HEATED_BED
      static celsius_t analog_to_celsius_bed(const int raw)                    { return get_heater(HeatedBedPos).analog_to_celsius(raw); }
    #endif
    #if HAS_TEMP_CHAMBER
      static celsius_t analog_to_celsius_chamber(const int raw)                { return get_heater(HeatedChamberPos).analog_to_celsius(raw); }
    #endif
    #if HAS_TEMP_COOLER
      static celsius_t analog_to_celsius_cooler(const int raw)                 { return get_heater(CoolerPos).analog_to_celsius(raw); }
    #endif

    #if HAS_FAN

      static uint8_t fan_speed[FAN_COUNT];
      #define FANS_LOOP(I) LOOP_L_N(I, FAN_COUNT)

      static void set_fan_speed(const uint8_t target, const uint16_t speed);

      #if ENABLED(REPORT_FAN_CHANGE)
        static void report_fan_speed(const uint8_t target);
      #endif

      #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)
        static bool fans_paused;
        static uint8_t saved_fan_speed[FAN_COUNT];
      #endif

      static constexpr inline uint8_t fanPercent(const uint8_t speed) { return ui8_to_percent(speed); }

      #if ENABLED(ADAPTIVE_FAN_SLOWING)
        static uint8_t fan_speed_scaler[FAN_COUNT];
      #endif

      static inline uint8_t scaledFanSpeed(const uint8_t target, const uint8_t fs) {
        UNUSED(target); // Potentially unused!
        return (fs * uint16_t(TERN(ADAPTIVE_FAN_SLOWING, fan_speed_scaler[target], 128))) >> 7;
      }

      static inline uint8_t scaledFanSpeed(const uint8_t target) {
        return scaledFanSpeed(target, fan_speed[target]);
      }

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

    // high level conversion routines, for use outside of temperature.cpp
    // inline so that there is no performance decrease.
    static Heater & get_heater(const int pos);

    FORCE_INLINE static celsius_t degHotend(const uint8_t E_NAME) { return TERN0(HAS_HOTEND, get_heater(HotEndPos0 + HOTEND_INDEX).deg()); }

    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      FORCE_INLINE static int16_t rawHotendTemp(const uint8_t E_NAME) { return TERN0(HAS_HOTEND, get_heater(HotEndPos0 + HOTEND_INDEX).raw()); }
    #endif

    FORCE_INLINE static celsius_t degTargetHotend(const uint8_t E_NAME)  { return TERN0(HAS_HOTEND, get_heater(HotEndPos0 + HOTEND_INDEX).degTarget()); }

    FORCE_INLINE static void start_watching_hotend(const uint8_t E_NAME) { TERN_(HAS_HOTEND, get_heater(HotEndPos0 + HOTEND_INDEX).start_watching()); }

    #if HAS_HOTEND

      static void setTargetHotend(const celsius_t celsius, const uint8_t E_NAME);

      FORCE_INLINE static bool isHeatingHotend(const uint8_t E_NAME) { return TERN0(HAS_HOTEND, get_heater(HotEndPos0 + HOTEND_INDEX).is_heating()); }

      FORCE_INLINE static bool isCoolingHotend(const uint8_t E_NAME) { return TERN0(HAS_HOTEND, get_heater(HotEndPos0 + HOTEND_INDEX).is_cooling()); }

      #if HAS_TEMP_HOTEND
        static bool wait_for_hotend(const uint8_t target_extruder, const bool no_wait_for_cooling=true
          #if G26_CLICK_CAN_CANCEL
            , const bool click_to_cancel=false
          #endif
        );

        #if ENABLED(WAIT_FOR_HOTEND)
          static void wait_for_hotend_heating(const uint8_t target_extruder);
        #endif
      #endif

      FORCE_INLINE static bool still_heating(const uint8_t e) {
        return degTargetHotend(e) > get_heater(HotEndPos0 + e).hysteresis && ABS(degHotend(e) - degTargetHotend(e)) > get_heater(HotEndPos0 + e).hysteresis;
      }

      FORCE_INLINE static bool degHotendNear(const uint8_t e, const_float_t temp) { return ABS(degHotend(e) - temp) < get_heater(HotEndPos0 + e).hysteresis; }

    #endif // HAS_HOTEND

    #if HAS_TEMP_PROBE
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawProbeTemp()    { return get_heater(TempProbePos).raw; }
      #endif
      FORCE_INLINE static float degProbe()            { return get_heater(TempProbePos).deg(); }
      FORCE_INLINE static bool isProbeBelowTemp(const_float_t target_temp) { return degProbe() < target_temp; }
      FORCE_INLINE static bool isProbeAboveTemp(const_float_t target_temp) { return degProbe() > target_temp; }
      static bool wait_for_probe(const_float_t target_temp, bool no_wait_for_cooling=true);
    #endif

    FORCE_INLINE static void start_watching_probe() { TERN_(HAS_TEMP_PROBE, get_heater(TempProbePos).start_watching()); }

    #if HAS_HEATED_BED

      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawBedTemp()    { return get_heater(HeatedBedPos).raw(); }
      #endif
      FORCE_INLINE static celsius_t degBed()        { return get_heater(HeatedBedPos).deg(); }
      FORCE_INLINE static celsius_t degTargetBed()  { return get_heater(HeatedBedPos).degTarget(); }
      FORCE_INLINE static bool isHeatingBed()       { return get_heater(HeatedBedPos).is_heating(); }
      FORCE_INLINE static bool isCoolingBed()       { return get_heater(HeatedBedPos).is_cooling(); }


      static void setTargetBed(const celsius_t celsius) {
        TERN_(AUTO_POWER_CONTROL, if (celsius) powerManager.power_on());
        get_heater(HeatedBedPos).set_target(_MIN(celsius, celsius_t(BED_MAX_TARGET)));
        start_watching_bed();
      }

      static bool wait_for_bed(const bool no_wait_for_cooling=true
        #if G26_CLICK_CAN_CANCEL
          , const bool click_to_cancel=false
        #endif
      );

      static void wait_for_bed_heating();

      FORCE_INLINE static bool degBedNear(const_float_t temp) {
        return ABS(degBed() - temp) < get_heater(HeatedBedPos).hysteresis;
      }

    #endif // HAS_HEATED_BED
    FORCE_INLINE static void start_watching_bed() { TERN_(HAS_HEATED_BED, get_heater(HeatedBedPos).start_watching()); }

    #if HAS_TEMP_CHAMBER
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawChamberTemp()          { return get_heater(HeatedChamberPos).raw(); }
      #endif
      FORCE_INLINE static celsius_t degChamber()              { return get_heater(HeatedChamberPos).deg(); }
      #if HAS_HEATED_CHAMBER
        FORCE_INLINE static celsius_t degTargetChamber()  { return get_heater(HeatedChamberPos).degTarget(); }
        FORCE_INLINE static bool isHeatingChamber()       { return get_heater(HeatedChamberPos).is_heating(); }
        FORCE_INLINE static bool isCoolingChamber()       { return get_heater(HeatedChamberPos).is_cooling(); }
        static bool wait_for_chamber(const bool no_wait_for_cooling=true);
      #endif
    #endif

    FORCE_INLINE static void start_watching_chamber() { TERN_(HAS_TEMP_CHAMBER, get_heater(HeatedChamberPos).start_watching()); }


    #if HAS_HEATED_CHAMBER
      static void setTargetChamber(const celsius_t celsius) {
        get_heater(HeatedChamberPos).set_target(_MIN(celsius, CHAMBER_MAX_TARGET));
        start_watching_chamber();
      }
    #endif

    #if HAS_TEMP_COOLER
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        FORCE_INLINE static int16_t rawCoolerTemp()     { return get_heater(CoolerPos).raw(); }
      #endif
      FORCE_INLINE static float degCooler()             { return get_heater(CoolerPos).deg(); }
      #if HAS_COOLER
        FORCE_INLINE static celsius_t degTargetCooler() { return get_heater(CoolerPos).degTarget(); }
        FORCE_INLINE static bool isLaserHeating()       { return get_heater(CoolerPos).is_heating(); }
        FORCE_INLINE static bool isLaserCooling()       { return get_heater(CoolerPos).is_cooling(); }
        static bool wait_for_cooler(const bool no_wait_for_cooling=true);
      #endif
    #endif

    FORCE_INLINE static void start_watching_cooler() { TERN_(HAS_TEMP_COOLER, get_heater(CoolerPos).start_watching()); }

    #if HAS_COOLER
      static void setTargetCooler(const celsius_t celsius) {
        get_heater(CoolerPos).set_target(constrain(celsius, COOLER_MIN_TARGET, COOLER_MAX_TARGET));
        start_watching_cooler();
      }
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
        get_heater(HotEndPos0 + HOTEND_INDEX).reset();
        start_watching_hotend(HOTEND_INDEX);
      }

      #if HAS_HEATED_BED
        static void reset_bed_idle_timer() {
          get_heater(HeatedBedPos).reset();
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
        struct AutoReportTemp { static void report(); };
        static AutoReporter<AutoReportTemp> auto_reporter;
      #endif
    #endif

    #if HAS_STATUS_MESSAGE
      static void set_heating_message(const uint8_t e);
    #endif

    #if HAS_LCD_MENU && HAS_TEMPERATURE
      static void lcd_preheat(const uint8_t e, const int8_t indh, const int8_t indb);
    #endif

  private:
    static void update_raw_temperatures();
    static void updateTemperaturesFromRawValues();

    #if HAS_MAX_TC
      #define MAX_TC_COUNT 1 + BOTH(TEMP_SENSOR_0_IS_MAX_TC, TEMP_SENSOR_1_IS_MAX_TC)
      #if MAX_TC_COUNT > 1
        #define HAS_MULTI_MAX_TC 1
        #define READ_MAX_TC(N) read_max_tc(N)
      #else
        #define READ_MAX_TC(N) read_max_tc()
      #endif
      static int read_max_tc(TERN_(HAS_MULTI_MAX_TC, const uint8_t hindex=0));
    #endif

    static void checkExtruderAutoFans();

    #if ENABLED(HAS_HOTEND)
      static float get_pid_output_hotend(const uint8_t E_NAME);
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
      #define _ENUM_FOR_E(N) RUNAWAY_IND_E##N,
      enum RunawayIndex : uint8_t {
        #if ENABLED(THERMAL_PROTECTION_HOTENDS)
          REPEAT(HOTENDS, _ENUM_FOR_E)
        #endif
        #if ENABLED(HAS_THERMALLY_PROTECTED_BED)
          RUNAWAY_IND_BED,
        #endif
        #if ENABLED(THERMAL_PROTECTION_CHAMBER)
          RUNAWAY_IND_CHAMBER,
        #endif
        #if ENABLED(THERMAL_PROTECTION_COOLER)
          RUNAWAY_IND_COOLER,
        #endif
        NR_HEATER_RUNAWAY
      };
      #undef _ENUM_FOR_E

      // Convert the given heater_id_t to runaway state array index
      static inline RunawayIndex runaway_index_for_id(const int8_t heater_id) {
        #if HAS_THERMALLY_PROTECTED_CHAMBER
          if (heater_id == H_CHAMBER) return RUNAWAY_IND_CHAMBER;
        #endif
        #if HAS_THERMALLY_PROTECTED_CHAMBER
          if (heater_id == H_COOLER) return RUNAWAY_IND_COOLER;
        #endif
        #if HAS_THERMALLY_PROTECTED_BED
          if (heater_id == H_BED) return RUNAWAY_IND_BED;
        #endif
        return (RunawayIndex)_MAX(heater_id, 0);
      }

      enum TRState : char { TRInactive, TRFirstHeating, TRStable, TRRunaway };

      struct tr_state_machine_t {
        millis_t timer = 0;
        TRState state = TRInactive;
        celsius_t running_temp;
        void run(Heater & heater, int type);
      };

      static tr_state_machine_t tr_state_machine[NR_HEATER_RUNAWAY];

    #endif // HAS_THERMAL_PROTECTION
};

extern Temperature thermalManager;
