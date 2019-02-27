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

/**
 * temperature.cpp - temperature control
 */

#include "temperature.h"
#include "endstops.h"

#include "../Marlin.h"
#include "../lcd/ultralcd.h"
#include "planner.h"
#include "../core/language.h"
#include "../HAL/shared/Delay.h"

#define MAX6675_SEPARATE_SPI (ENABLED(HEATER_0_USES_MAX6675) || ENABLED(HEATER_1_USES_MAX6675)) && PIN_EXISTS(MAX6675_SCK) && PIN_EXISTS(MAX6675_DO)

#if MAX6675_SEPARATE_SPI
  #include "../libs/private_spi.h"
#endif

#if ENABLED(BABYSTEPPING) || ENABLED(PID_EXTRUSION_SCALING)
  #include "stepper.h"
#endif

#if ENABLED(BABYSTEPPING)
  #include "../module/motion.h"
  #if ENABLED(BABYSTEP_ALWAYS_AVAILABLE)
    #include "../gcode/gcode.h"
  #endif
#endif

#include "printcounter.h"

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  #include "../feature/filwidth.h"
#endif

#if ENABLED(EMERGENCY_PARSER)
  #include "../feature/emergency_parser.h"
#endif

#if ENABLED(PRINTER_EVENT_LEDS)
  #include "../feature/leds/printer_event_leds.h"
#endif

#if ENABLED(SINGLENOZZLE)
  #include "tool_change.h"
#endif

#if HOTEND_USES_THERMISTOR
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    static void* heater_ttbl_map[2] = { (void*)HEATER_0_TEMPTABLE, (void*)HEATER_1_TEMPTABLE };
    static constexpr uint8_t heater_ttbllen_map[2] = { HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN };
  #else
    static void* heater_ttbl_map[HOTENDS] = ARRAY_BY_HOTENDS((void*)HEATER_0_TEMPTABLE, (void*)HEATER_1_TEMPTABLE, (void*)HEATER_2_TEMPTABLE, (void*)HEATER_3_TEMPTABLE, (void*)HEATER_4_TEMPTABLE);
    static constexpr uint8_t heater_ttbllen_map[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN, HEATER_2_TEMPTABLE_LEN, HEATER_3_TEMPTABLE_LEN, HEATER_4_TEMPTABLE_LEN);
  #endif
#endif

Temperature thermalManager;

/**
 * Macros to include the heater id in temp errors. The compiler's dead-code
 * elimination should (hopefully) optimize out the unused strings.
 */
#if HAS_HEATED_BED
  #define TEMP_ERR_PSTR(MSG, E) \
    (E) == -1 ? PSTR(MSG ## _BED) : \
    (HOTENDS > 1 && (E) == 1) ? PSTR(MSG_E2 " " MSG) : \
    (HOTENDS > 2 && (E) == 2) ? PSTR(MSG_E3 " " MSG) : \
    (HOTENDS > 3 && (E) == 3) ? PSTR(MSG_E4 " " MSG) : \
    (HOTENDS > 4 && (E) == 4) ? PSTR(MSG_E5 " " MSG) : \
    (HOTENDS > 5 && (E) == 5) ? PSTR(MSG_E6 " " MSG) : \
    PSTR(MSG_E1 " " MSG)
#else
  #define TEMP_ERR_PSTR(MSG, E) \
    (HOTENDS > 1 && (E) == 1) ? PSTR(MSG_E2 " " MSG) : \
    (HOTENDS > 2 && (E) == 2) ? PSTR(MSG_E3 " " MSG) : \
    (HOTENDS > 3 && (E) == 3) ? PSTR(MSG_E4 " " MSG) : \
    (HOTENDS > 4 && (E) == 4) ? PSTR(MSG_E5 " " MSG) : \
    (HOTENDS > 5 && (E) == 5) ? PSTR(MSG_E6 " " MSG) : \
    PSTR(MSG_E1 " " MSG)
#endif

// public:

#if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
  bool Temperature::adaptive_fan_slowing = true;
#endif

float Temperature::current_temperature[HOTENDS]; // = { 0.0 };
int16_t Temperature::current_temperature_raw[HOTENDS], // = { 0 }
        Temperature::target_temperature[HOTENDS]; // = { 0 }

#if ENABLED(AUTO_POWER_E_FANS)
  uint8_t Temperature::autofan_speed[HOTENDS]; // = { 0 }
#endif

#if FAN_COUNT > 0

  uint8_t Temperature::fan_speed[FAN_COUNT]; // = { 0 }

  #if ENABLED(EXTRA_FAN_SPEED)
    uint8_t Temperature::old_fan_speed[FAN_COUNT], Temperature::new_fan_speed[FAN_COUNT];

    void Temperature::set_temp_fan_speed(const uint8_t fan, const uint16_t tmp_temp) {
      switch (tmp_temp) {
        case 1:
          set_fan_speed(fan, old_fan_speed[fan]);
          break;
        case 2:
          old_fan_speed[fan] = fan_speed[fan];
          set_fan_speed(fan, new_fan_speed[fan]);
          break;
        default:
          new_fan_speed[fan] = MIN(tmp_temp, 255U);
          break;
      }
    }

  #endif

  #if ENABLED(PROBING_FANS_OFF)
    bool Temperature::fans_paused; // = false;
    uint8_t Temperature::paused_fan_speed[FAN_COUNT]; // = { 0 }
  #endif

  #if ENABLED(ADAPTIVE_FAN_SLOWING)
    uint8_t Temperature::fan_speed_scaler[FAN_COUNT] = ARRAY_N(FAN_COUNT, 128, 128, 128, 128, 128, 128);
  #endif

  #if HAS_LCD_MENU

    uint8_t Temperature::lcd_tmpfan_speed[
      #if ENABLED(SINGLENOZZLE)
        MAX(EXTRUDERS, FAN_COUNT)
      #else
        FAN_COUNT
      #endif
    ]; // = { 0 }

  #endif

  void Temperature::set_fan_speed(uint8_t target, uint16_t speed) {

    NOMORE(speed, 255U);

    #if ENABLED(SINGLENOZZLE)
      if (target != active_extruder) {
        if (target < EXTRUDERS) singlenozzle_fan_speed[target] = speed;
        return;
      }
      target = 0; // Always use fan index 0 with SINGLENOZZLE
    #endif

    if (target >= FAN_COUNT) return;

    fan_speed[target] = speed;
    #if HAS_LCD_MENU
      lcd_tmpfan_speed[target] = speed;
    #endif
  }

  #if ENABLED(PROBING_FANS_OFF)

    void Temperature::set_fans_paused(const bool p) {
      if (p != fans_paused) {
        fans_paused = p;
        if (p)
          for (uint8_t x = 0; x < FAN_COUNT; x++) {
            paused_fan_speed[x] = fan_speed[x];
            fan_speed[x] = 0;
          }
        else
          for (uint8_t x = 0; x < FAN_COUNT; x++)
            fan_speed[x] = paused_fan_speed[x];
      }
    }

  #endif // PROBING_FANS_OFF

#endif // FAN_COUNT > 0

#if HAS_HEATED_BED
  float Temperature::current_temperature_bed = 0.0;
  int16_t Temperature::current_temperature_bed_raw = 0,
          Temperature::target_temperature_bed = 0;
  uint8_t Temperature::soft_pwm_amount_bed;
  #ifdef BED_MINTEMP
    int16_t Temperature::bed_minttemp_raw = HEATER_BED_RAW_LO_TEMP;
  #endif
  #ifdef BED_MAXTEMP
    int16_t Temperature::bed_maxttemp_raw = HEATER_BED_RAW_HI_TEMP;
  #endif
  #if WATCH_THE_BED
    uint16_t Temperature::watch_target_bed_temp = 0;
    millis_t Temperature::watch_bed_next_ms = 0;
  #endif
  #if ENABLED(PIDTEMPBED)
    PID_t Temperature::bed_pid; // Initialized by settings.load()
  #else
    millis_t Temperature::next_bed_check_ms;
  #endif
  uint16_t Temperature::raw_temp_bed_value = 0;
  #if HEATER_IDLE_HANDLER
    millis_t Temperature::bed_idle_timeout_ms = 0;
    bool Temperature::bed_idle_timeout_exceeded = false;
  #endif
#endif // HAS_HEATED_BED

#if HAS_TEMP_CHAMBER
  float Temperature::current_temperature_chamber = 0.0;
  int16_t Temperature::current_temperature_chamber_raw = 0;
  uint16_t Temperature::raw_temp_chamber_value = 0;
#endif

// Initialized by settings.load()
#if ENABLED(PIDTEMP)
  hotend_pid_t Temperature::pid[HOTENDS];
#endif

#if ENABLED(BABYSTEPPING)
  volatile int16_t Temperature::babystepsTodo[XYZ] = { 0 };
#endif

#if WATCH_HOTENDS
  uint16_t Temperature::watch_target_temp[HOTENDS] = { 0 };
  millis_t Temperature::watch_heater_next_ms[HOTENDS] = { 0 };
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  bool Temperature::allow_cold_extrude = false;
  int16_t Temperature::extrude_min_temp = EXTRUDE_MINTEMP;
#endif

// private:

#if EARLY_WATCHDOG
  bool Temperature::inited = false;
#endif

#if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
  uint16_t Temperature::redundant_temperature_raw = 0;
  float Temperature::redundant_temperature = 0.0;
#endif

volatile bool Temperature::temp_meas_ready = false;

#if ENABLED(PID_EXTRUSION_SCALING)
  long Temperature::last_e_position;
  long Temperature::lpq[LPQ_MAX_LEN];
  int Temperature::lpq_ptr = 0;
#endif

uint16_t Temperature::raw_temp_value[MAX_EXTRUDERS] = { 0 };

// Init min and max temp with extreme values to prevent false errors during startup
int16_t Temperature::minttemp_raw[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_RAW_LO_TEMP , HEATER_1_RAW_LO_TEMP , HEATER_2_RAW_LO_TEMP, HEATER_3_RAW_LO_TEMP, HEATER_4_RAW_LO_TEMP),
        Temperature::maxttemp_raw[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_RAW_HI_TEMP , HEATER_1_RAW_HI_TEMP , HEATER_2_RAW_HI_TEMP, HEATER_3_RAW_HI_TEMP, HEATER_4_RAW_HI_TEMP),
        Temperature::minttemp[HOTENDS] = { 0 },
        Temperature::maxttemp[HOTENDS] = ARRAY_BY_HOTENDS1(16383);

#ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
  uint8_t Temperature::consecutive_low_temperature_error[HOTENDS] = { 0 };
#endif

#ifdef MILLISECONDS_PREHEAT_TIME
  millis_t Temperature::preheat_end_time[HOTENDS] = { 0 };
#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  int8_t Temperature::meas_shift_index;  // Index of a delayed sample in buffer
#endif

#if HAS_AUTO_FAN
  millis_t Temperature::next_auto_fan_check_ms = 0;
#endif

uint8_t Temperature::soft_pwm_amount[HOTENDS];

#if ENABLED(FAN_SOFT_PWM)
  uint8_t Temperature::soft_pwm_amount_fan[FAN_COUNT],
          Temperature::soft_pwm_count_fan[FAN_COUNT];
#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  uint16_t Temperature::current_raw_filwidth = 0; // Measured filament diameter - one extruder only
#endif

#if ENABLED(PROBING_HEATERS_OFF)
  bool Temperature::paused;
#endif

#if HEATER_IDLE_HANDLER
  millis_t Temperature::heater_idle_timeout_ms[HOTENDS] = { 0 };
  bool Temperature::heater_idle_timeout_exceeded[HOTENDS] = { false };
#endif

// public:

#if HAS_ADC_BUTTONS
  uint32_t Temperature::current_ADCKey_raw = 0;
  uint8_t Temperature::ADCKey_count = 0;
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  int16_t Temperature::lpq_len; // Initialized in configuration_store
#endif

#if HAS_PID_HEATING

  inline void say_default_() { SERIAL_ECHOPGM("#define DEFAULT_"); }

  /**
   * PID Autotuning (M303)
   *
   * Alternately heat and cool the nozzle, observing its behavior to
   * determine the best PID values to achieve a stable temperature.
   * Needs sufficient heater power to make some overshoot at target
   * temperature to succeed.
   */
  void Temperature::PID_autotune(const float &target, const int8_t heater, const int8_t ncycles, const bool set_result/*=false*/) {
    float current = 0.0;
    int cycles = 0;
    bool heating = true;

    millis_t next_temp_ms = millis(), t1 = next_temp_ms, t2 = next_temp_ms;
    long t_high = 0, t_low = 0;

    long bias, d;
    PID_t tune_pid = { 0, 0, 0 };
    float max = 0, min = 10000;

    #if HAS_PID_FOR_BOTH
      #define GHV(B,H) (heater < 0 ? (B) : (H))
      #define SHV(S,B,H) do{ if (heater < 0) S##_bed = B; else S [heater] = H; }while(0)
      #define ONHEATINGSTART() (heater < 0 ? printerEventLEDs.onBedHeatingStart() : printerEventLEDs.onHotendHeatingStart())
      #define ONHEATING(S,C,T) do{ if (heater < 0) printerEventLEDs.onBedHeating(S,C,T); else printerEventLEDs.onHotendHeating(S,C,T); }while(0)
    #elif ENABLED(PIDTEMPBED)
      #define GHV(B,H) B
      #define SHV(S,B,H) (S##_bed = B)
      #define ONHEATINGSTART() printerEventLEDs.onBedHeatingStart()
      #define ONHEATING(S,C,T) printerEventLEDs.onBedHeating(S,C,T)
    #else
      #define GHV(B,H) H
      #define SHV(S,B,H) (S [heater] = H)
      #define ONHEATINGSTART() printerEventLEDs.onHotendHeatingStart()
      #define ONHEATING(S,C,T) printerEventLEDs.onHotendHeating(S,C,T)
    #endif

    #if WATCH_THE_BED || WATCH_HOTENDS
      #define HAS_TP_BED (ENABLED(THERMAL_PROTECTION_BED) && ENABLED(PIDTEMPBED))
      #if HAS_TP_BED && ENABLED(THERMAL_PROTECTION_HOTENDS) && ENABLED(PIDTEMP)
        #define GTV(B,H) (heater < 0 ? (B) : (H))
      #elif HAS_TP_BED
        #define GTV(B,H) (B)
      #else
        #define GTV(B,H) (H)
      #endif
      const uint16_t watch_temp_period = GTV(WATCH_BED_TEMP_PERIOD, WATCH_TEMP_PERIOD);
      const uint8_t watch_temp_increase = GTV(WATCH_BED_TEMP_INCREASE, WATCH_TEMP_INCREASE);
      const float watch_temp_target = target - float(watch_temp_increase + GTV(TEMP_BED_HYSTERESIS, TEMP_HYSTERESIS) + 1);
      millis_t temp_change_ms = next_temp_ms + watch_temp_period * 1000UL;
      float next_watch_temp = 0.0;
      bool heated = false;
    #endif

    #if HAS_AUTO_FAN
      next_auto_fan_check_ms = next_temp_ms + 2500UL;
    #endif

    if (target > GHV(BED_MAXTEMP, maxttemp[heater]) - 15) {
      SERIAL_ECHOLNPGM(MSG_PID_TEMP_TOO_HIGH);
      return;
    }

    SERIAL_ECHOLNPGM(MSG_PID_AUTOTUNE_START);

    disable_all_heaters();

    SHV(soft_pwm_amount, bias = d = (MAX_BED_POWER) >> 1, bias = d = (PID_MAX) >> 1);

    wait_for_heatup = true; // Can be interrupted with M108
    #if ENABLED(PRINTER_EVENT_LEDS)
      const float start_temp = GHV(current_temperature_bed, current_temperature[heater]);
      LEDColor color = ONHEATINGSTART();
    #endif

    #if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
      adaptive_fan_slowing = false;
    #endif

    // PID Tuning loop
    while (wait_for_heatup) {

      const millis_t ms = millis();

      if (temp_meas_ready) { // temp sample ready
        updateTemperaturesFromRawValues();

        // Get the current temperature and constrain it
        current = GHV(current_temperature_bed, current_temperature[heater]);
        NOLESS(max, current);
        NOMORE(min, current);

        #if ENABLED(PRINTER_EVENT_LEDS)
          ONHEATING(start_temp, current, target);
        #endif

        #if HAS_AUTO_FAN
          if (ELAPSED(ms, next_auto_fan_check_ms)) {
            checkExtruderAutoFans();
            next_auto_fan_check_ms = ms + 2500UL;
          }
        #endif

        if (heating && current > target) {
          if (ELAPSED(ms, t2 + 5000UL)) {
            heating = false;
            SHV(soft_pwm_amount, (bias - d) >> 1, (bias - d) >> 1);
            t1 = ms;
            t_high = t1 - t2;
            max = target;
          }
        }

        if (!heating && current < target) {
          if (ELAPSED(ms, t1 + 5000UL)) {
            heating = true;
            t2 = ms;
            t_low = t2 - t1;
            if (cycles > 0) {
              const long max_pow = GHV(MAX_BED_POWER, PID_MAX);
              bias += (d * (t_high - t_low)) / (t_low + t_high);
              bias = constrain(bias, 20, max_pow - 20);
              d = (bias > max_pow >> 1) ? max_pow - 1 - bias : bias;

              SERIAL_ECHOPAIR(MSG_BIAS, bias);
              SERIAL_ECHOPAIR(MSG_D, d);
              SERIAL_ECHOPAIR(MSG_T_MIN, min);
              SERIAL_ECHOPAIR(MSG_T_MAX, max);
              if (cycles > 2) {
                float Ku = (4.0f * d) / (float(M_PI) * (max - min) * 0.5f),
                      Tu = ((float)(t_low + t_high) * 0.001f);
                SERIAL_ECHOPAIR(MSG_KU, Ku);
                SERIAL_ECHOPAIR(MSG_TU, Tu);
                tune_pid.Kp = 0.6f * Ku;
                tune_pid.Ki = 2 * tune_pid.Kp / Tu;
                tune_pid.Kd = tune_pid.Kp * Tu * 0.125f;
                SERIAL_ECHOLNPGM("\n" MSG_CLASSIC_PID);
                SERIAL_ECHOPAIR(MSG_KP, tune_pid.Kp);
                SERIAL_ECHOPAIR(MSG_KI, tune_pid.Ki);
                SERIAL_ECHOLNPAIR(MSG_KD, tune_pid.Kd);
                /**
                tune_pid.Kp = 0.33*Ku;
                tune_pid.Ki = tune_pid.Kp/Tu;
                tune_pid.Kd = tune_pid.Kp*Tu/3;
                SERIAL_ECHOLNPGM(" Some overshoot");
                SERIAL_ECHOPAIR(" Kp: ", tune_pid.Kp);
                SERIAL_ECHOPAIR(" Ki: ", tune_pid.Ki);
                SERIAL_ECHOPAIR(" Kd: ", tune_pid.Kd);
                tune_pid.Kp = 0.2*Ku;
                tune_pid.Ki = 2*tune_pid.Kp/Tu;
                tune_pid.Kd = tune_pid.Kp*Tu/3;
                SERIAL_ECHOLNPGM(" No overshoot");
                SERIAL_ECHOPAIR(" Kp: ", tune_pid.Kp);
                SERIAL_ECHOPAIR(" Ki: ", tune_pid.Ki);
                SERIAL_ECHOPAIR(" Kd: ", tune_pid.Kd);
                */
              }
            }
            SHV(soft_pwm_amount, (bias + d) >> 1, (bias + d) >> 1);
            cycles++;
            min = target;
          }
        }
      }

      // Did the temperature overshoot very far?
      #ifndef MAX_OVERSHOOT_PID_AUTOTUNE
        #define MAX_OVERSHOOT_PID_AUTOTUNE 20
      #endif
      if (current > target + MAX_OVERSHOOT_PID_AUTOTUNE) {
        SERIAL_ECHOLNPGM(MSG_PID_TEMP_TOO_HIGH);
        break;
      }

      // Report heater states every 2 seconds
      if (ELAPSED(ms, next_temp_ms)) {
        #if HAS_TEMP_SENSOR
          print_heater_states(heater >= 0 ? heater : active_extruder);
          SERIAL_EOL();
        #endif
        next_temp_ms = ms + 2000UL;

        // Make sure heating is actually working
        #if WATCH_THE_BED || WATCH_HOTENDS
          if (
            #if WATCH_THE_BED && WATCH_HOTENDS
              true
            #elif WATCH_HOTENDS
              heater >= 0
            #else
              heater < 0
            #endif
          ) {
            if (!heated) {                                          // If not yet reached target...
              if (current > next_watch_temp) {                      // Over the watch temp?
                next_watch_temp = current + watch_temp_increase;    // - set the next temp to watch for
                temp_change_ms = ms + watch_temp_period * 1000UL;   // - move the expiration timer up
                if (current > watch_temp_target) heated = true;     // - Flag if target temperature reached
              }
              else if (ELAPSED(ms, temp_change_ms))                 // Watch timer expired
                _temp_error(heater, PSTR(MSG_T_HEATING_FAILED), TEMP_ERR_PSTR(MSG_HEATING_FAILED_LCD, heater));
            }
            else if (current < target - (MAX_OVERSHOOT_PID_AUTOTUNE)) // Heated, then temperature fell too far?
              _temp_error(heater, PSTR(MSG_T_THERMAL_RUNAWAY), TEMP_ERR_PSTR(MSG_THERMAL_RUNAWAY, heater));
          }
        #endif
      } // every 2 seconds

      // Timeout after MAX_CYCLE_TIME_PID_AUTOTUNE minutes since the last undershoot/overshoot cycle
      #ifndef MAX_CYCLE_TIME_PID_AUTOTUNE
        #define MAX_CYCLE_TIME_PID_AUTOTUNE 20L
      #endif
      if (((ms - t1) + (ms - t2)) > (MAX_CYCLE_TIME_PID_AUTOTUNE * 60L * 1000L)) {
        SERIAL_ECHOLNPGM(MSG_PID_TIMEOUT);
        break;
      }

      if (cycles > ncycles && cycles > 2) {
        SERIAL_ECHOLNPGM(MSG_PID_AUTOTUNE_FINISHED);

        #if HAS_PID_FOR_BOTH
          const char * const estring = GHV(PSTR("bed"), PSTR(""));
          say_default_(); serialprintPGM(estring); SERIAL_ECHOLNPAIR("Kp ", tune_pid.Kp);
          say_default_(); serialprintPGM(estring); SERIAL_ECHOLNPAIR("Ki ", tune_pid.Ki);
          say_default_(); serialprintPGM(estring); SERIAL_ECHOLNPAIR("Kd ", tune_pid.Kd);
        #elif ENABLED(PIDTEMP)
          say_default_(); SERIAL_ECHOLNPAIR("Kp ", tune_pid.Kp);
          say_default_(); SERIAL_ECHOLNPAIR("Ki ", tune_pid.Ki);
          say_default_(); SERIAL_ECHOLNPAIR("Kd ", tune_pid.Kd);
        #else
          say_default_(); SERIAL_ECHOLNPAIR("bedKp ", tune_pid.Kp);
          say_default_(); SERIAL_ECHOLNPAIR("bedKi ", tune_pid.Ki);
          say_default_(); SERIAL_ECHOLNPAIR("bedKd ", tune_pid.Kd);
        #endif

        #define _SET_BED_PID() do { \
          bed_pid.Kp = tune_pid.Kp; \
          bed_pid.Ki = scalePID_i(tune_pid.Ki); \
          bed_pid.Kd = scalePID_d(tune_pid.Kd); \
        }while(0)

        #define _SET_EXTRUDER_PID() do { \
          PID_PARAM(Kp, heater) = tune_pid.Kp; \
          PID_PARAM(Ki, heater) = scalePID_i(tune_pid.Ki); \
          PID_PARAM(Kd, heater) = scalePID_d(tune_pid.Kd); \
          updatePID(); }while(0)

        // Use the result? (As with "M303 U1")
        if (set_result) {
          #if HAS_PID_FOR_BOTH
            if (heater < 0) _SET_BED_PID(); else _SET_EXTRUDER_PID();
          #elif ENABLED(PIDTEMP)
            _SET_EXTRUDER_PID();
          #else
            _SET_BED_PID();
          #endif
        }

        #if ENABLED(PRINTER_EVENT_LEDS)
          printerEventLEDs.onPidTuningDone(color);
        #endif

        goto EXIT_M303;
      }
      ui.update();
    }

    disable_all_heaters();

    #if ENABLED(PRINTER_EVENT_LEDS)
      printerEventLEDs.onPidTuningDone(color);
    #endif

    EXIT_M303:
      #if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
        adaptive_fan_slowing = true;
      #endif
      return;
  }

#endif // HAS_PID_HEATING

/**
 * Class and Instance Methods
 */

Temperature::Temperature() { }

int Temperature::getHeaterPower(const int heater) {
  return (
    #if HAS_HEATED_BED
      heater < 0 ? soft_pwm_amount_bed :
    #endif
    soft_pwm_amount[heater]
  );
}

#if HAS_AUTO_FAN

  void Temperature::checkExtruderAutoFans() {
    static const pin_t fanPin[] PROGMEM = { E0_AUTO_FAN_PIN, E1_AUTO_FAN_PIN, E2_AUTO_FAN_PIN, E3_AUTO_FAN_PIN, E4_AUTO_FAN_PIN, E5_AUTO_FAN_PIN, CHAMBER_AUTO_FAN_PIN };
    static const uint8_t fanBit[] PROGMEM = {
                    0,
      AUTO_1_IS_0 ? 0 :               1,
      AUTO_2_IS_0 ? 0 : AUTO_2_IS_1 ? 1 :               2,
      AUTO_3_IS_0 ? 0 : AUTO_3_IS_1 ? 1 : AUTO_3_IS_2 ? 2 :               3,
      AUTO_4_IS_0 ? 0 : AUTO_4_IS_1 ? 1 : AUTO_4_IS_2 ? 2 : AUTO_4_IS_3 ? 3 :                   4,
      AUTO_5_IS_0 ? 0 : AUTO_5_IS_1 ? 1 : AUTO_5_IS_2 ? 2 : AUTO_5_IS_3 ? 3 : AUTO_5_IS_4 ? 4 : 5,
      AUTO_CHAMBER_IS_0 ? 0 : AUTO_CHAMBER_IS_1 ? 1 : AUTO_CHAMBER_IS_2 ? 2 : AUTO_CHAMBER_IS_3 ? 3 : AUTO_CHAMBER_IS_4 ? 4 : 5
    };
    uint8_t fanState = 0;

    HOTEND_LOOP()
      if (current_temperature[e] > EXTRUDER_AUTO_FAN_TEMPERATURE)
        SBI(fanState, pgm_read_byte(&fanBit[e]));

    #if HAS_TEMP_CHAMBER
      if (current_temperature_chamber > EXTRUDER_AUTO_FAN_TEMPERATURE)
        SBI(fanState, pgm_read_byte(&fanBit[5]));
    #endif

    uint8_t fanDone = 0;
    for (uint8_t f = 0; f < COUNT(fanPin); f++) {
      const pin_t pin =
        #ifdef ARDUINO
          pgm_read_byte(&fanPin[f])
        #else
          fanPin[f]
        #endif
      ;
      const uint8_t bit = pgm_read_byte(&fanBit[f]);
      if (pin >= 0 && !TEST(fanDone, bit)) {
        uint8_t newFanSpeed = TEST(fanState, bit) ? EXTRUDER_AUTO_FAN_SPEED : 0;
        #if ENABLED(AUTO_POWER_E_FANS)
          autofan_speed[f] = newFanSpeed;
        #endif
        // this idiom allows both digital and PWM fan outputs (see M42 handling).
        digitalWrite(pin, newFanSpeed);
        analogWrite(pin, newFanSpeed);
        SBI(fanDone, bit);
      }
    }
  }

#endif // HAS_AUTO_FAN

//
// Temperature Error Handlers
//
void Temperature::_temp_error(const int8_t heater, PGM_P const serial_msg, PGM_P const lcd_msg) {
  static bool killed = false;
  if (IsRunning()) {
    SERIAL_ERROR_START();
    serialprintPGM(serial_msg);
    SERIAL_ECHOPGM(MSG_STOPPED_HEATER);
    if (heater >= 0) SERIAL_ECHOLN((int)heater); else SERIAL_ECHOLNPGM(MSG_HEATER_BED);
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

void Temperature::max_temp_error(const int8_t heater) {
  _temp_error(heater, PSTR(MSG_T_MAXTEMP), TEMP_ERR_PSTR(MSG_ERR_MAXTEMP, heater));
}

void Temperature::min_temp_error(const int8_t heater) {
  _temp_error(heater, PSTR(MSG_T_MINTEMP), TEMP_ERR_PSTR(MSG_ERR_MINTEMP, heater));
}

float Temperature::get_pid_output(const int8_t e) {
  #if HOTENDS == 1
    UNUSED(e);
    #define _HOTEND_TEST true
  #else
    #define _HOTEND_TEST (e == active_extruder)
  #endif
  float pid_output;
  #if ENABLED(PIDTEMP)
    #if DISABLED(PID_OPENLOOP)
      static hotend_pid_t work_pid[HOTENDS];
      static float temp_iState[HOTENDS] = { 0 },
                   temp_dState[HOTENDS] = { 0 };
      static bool pid_reset[HOTENDS] = { false };
      float pid_error = target_temperature[HOTEND_INDEX] - current_temperature[HOTEND_INDEX];
      work_pid[HOTEND_INDEX].Kd = PID_K2 * PID_PARAM(Kd, HOTEND_INDEX) * (current_temperature[HOTEND_INDEX] - temp_dState[HOTEND_INDEX]) + float(PID_K1) * work_pid[HOTEND_INDEX].Kd;
      temp_dState[HOTEND_INDEX] = current_temperature[HOTEND_INDEX];
      #if HEATER_IDLE_HANDLER
        if (heater_idle_timeout_exceeded[HOTEND_INDEX]) {
          pid_output = 0;
          pid_reset[HOTEND_INDEX] = true;
        }
        else
      #endif
          if (pid_error > PID_FUNCTIONAL_RANGE) {
            pid_output = BANG_MAX;
            pid_reset[HOTEND_INDEX] = true;
          }
          else if (pid_error < -(PID_FUNCTIONAL_RANGE) || target_temperature[HOTEND_INDEX] == 0
            #if HEATER_IDLE_HANDLER
              || heater_idle_timeout_exceeded[HOTEND_INDEX]
            #endif
          ) {
            pid_output = 0;
            pid_reset[HOTEND_INDEX] = true;
          }
          else {
            if (pid_reset[HOTEND_INDEX]) {
              temp_iState[HOTEND_INDEX] = 0.0;
              pid_reset[HOTEND_INDEX] = false;
            }
            temp_iState[HOTEND_INDEX] += pid_error;
            work_pid[HOTEND_INDEX].Kp = PID_PARAM(Kp, HOTEND_INDEX) * pid_error;
            work_pid[HOTEND_INDEX].Ki = PID_PARAM(Ki, HOTEND_INDEX) * temp_iState[HOTEND_INDEX];

            pid_output = work_pid[HOTEND_INDEX].Kp + work_pid[HOTEND_INDEX].Ki - work_pid[HOTEND_INDEX].Kd;

            #if ENABLED(PID_EXTRUSION_SCALING)
              work_pid[HOTEND_INDEX].Kc = 0;
              if (_HOTEND_TEST) {
                const long e_position = stepper.position(E_AXIS);
                if (e_position > last_e_position) {
                  lpq[lpq_ptr] = e_position - last_e_position;
                  last_e_position = e_position;
                }
                else
                  lpq[lpq_ptr] = 0;

                if (++lpq_ptr >= lpq_len) lpq_ptr = 0;
                work_pid[HOTEND_INDEX].Kc = (lpq[lpq_ptr] * planner.steps_to_mm[E_AXIS]) * PID_PARAM(Kc, HOTEND_INDEX);
                pid_output += work_pid[HOTEND_INDEX].Kc;
              }
            #endif // PID_EXTRUSION_SCALING

            if (pid_output > PID_MAX) {
              if (pid_error > 0) temp_iState[HOTEND_INDEX] -= pid_error; // conditional un-integration
              pid_output = PID_MAX;
            }
            else if (pid_output < 0) {
              if (pid_error < 0) temp_iState[HOTEND_INDEX] -= pid_error; // conditional un-integration
              pid_output = 0;
            }
          }

    #else // PID_OPENLOOP

      const float pid_output = constrain(target_temperature[HOTEND_INDEX], 0, PID_MAX);

    #endif // PID_OPENLOOP

    #if ENABLED(PID_DEBUG)
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR(MSG_PID_DEBUG, HOTEND_INDEX);
      SERIAL_ECHOPAIR(MSG_PID_DEBUG_INPUT, current_temperature[HOTEND_INDEX]);
      SERIAL_ECHOPAIR(MSG_PID_DEBUG_OUTPUT, pid_output);
      #if DISABLED(PID_OPENLOOP)
        SERIAL_ECHOPAIR(MSG_PID_DEBUG_PTERM, work_pid[HOTEND_INDEX].Kp);
        SERIAL_ECHOPAIR(MSG_PID_DEBUG_ITERM, work_pid[HOTEND_INDEX].Ki);
        SERIAL_ECHOPAIR(MSG_PID_DEBUG_DTERM, work_pid[HOTEND_INDEX].Kd);
        #if ENABLED(PID_EXTRUSION_SCALING)
          SERIAL_ECHOPAIR(MSG_PID_DEBUG_CTERM, work_pid[HOTEND_INDEX].Kc);
        #endif
      #endif
      SERIAL_EOL();
    #endif // PID_DEBUG

  #else /* PID off */
    #if HEATER_IDLE_HANDLER
      if (heater_idle_timeout_exceeded[HOTEND_INDEX])
        pid_output = 0;
      else
    #endif
    pid_output = (current_temperature[HOTEND_INDEX] < target_temperature[HOTEND_INDEX]) ? BANG_MAX : 0;
  #endif

  return pid_output;
}

#if ENABLED(PIDTEMPBED)

  float Temperature::get_pid_output_bed() {

    #if DISABLED(PID_OPENLOOP)

      static PID_t work_pid = { 0 };
      static float temp_iState = 0, temp_dState = 0;

      float pid_error = target_temperature_bed - current_temperature_bed;
      temp_iState += pid_error;
      work_pid.Kp = bed_pid.Kp * pid_error;
      work_pid.Ki = bed_pid.Ki * temp_iState;
      work_pid.Kd = PID_K2 * bed_pid.Kd * (current_temperature_bed - temp_dState) + PID_K1 * work_pid.Kd;

      temp_dState = current_temperature_bed;

      float pid_output = work_pid.Kp + work_pid.Ki - work_pid.Kd;
      if (pid_output > MAX_BED_POWER) {
        if (pid_error > 0) temp_iState -= pid_error; // conditional un-integration
        pid_output = MAX_BED_POWER;
      }
      else if (pid_output < 0) {
        if (pid_error < 0) temp_iState -= pid_error; // conditional un-integration
        pid_output = 0;
      }

    #else // PID_OPENLOOP

      const float pid_output = constrain(target_temperature_bed, 0, MAX_BED_POWER);

    #endif // PID_OPENLOOP

    #if ENABLED(PID_BED_DEBUG)
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR(" PID_BED_DEBUG : Input ", current_temperature_bed);
      SERIAL_ECHOPAIR(" Output ", pid_output);
      #if DISABLED(PID_OPENLOOP)
        SERIAL_ECHOPAIR(MSG_PID_DEBUG_PTERM, work_pid.Kp);
        SERIAL_ECHOPAIR(MSG_PID_DEBUG_ITERM, work_pid.Ki);
        SERIAL_ECHOLNPAIR(MSG_PID_DEBUG_DTERM, work_pid.Kd);
      #endif
    #endif

    return pid_output;
  }

#endif // PIDTEMPBED

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

  #if EARLY_WATCHDOG
    // If thermal manager is still not running, make sure to at least reset the watchdog!
    if (!inited) {
      watchdog_reset();
      return;
    }
  #endif

  #if ENABLED(PROBING_HEATERS_OFF) && ENABLED(BED_LIMIT_SWITCHING)
    static bool last_pause_state;
  #endif

  #if ENABLED(EMERGENCY_PARSER)
    if (emergency_parser.killed_by_M112) kill();
  #endif

  if (!temp_meas_ready) return;

  updateTemperaturesFromRawValues(); // also resets the watchdog

  #if ENABLED(HEATER_0_USES_MAX6675)
    if (current_temperature[0] > MIN(HEATER_0_MAXTEMP, HEATER_0_MAX6675_TMAX - 1.0)) max_temp_error(0);
    if (current_temperature[0] < MAX(HEATER_0_MINTEMP, HEATER_0_MAX6675_TMIN + .01)) min_temp_error(0);
  #endif

  #if ENABLED(HEATER_1_USES_MAX6675)
    if (current_temperature[1] > MIN(HEATER_1_MAXTEMP, HEATER_1_MAX6675_TMAX - 1.0)) max_temp_error(1);
    if (current_temperature[1] < MAX(HEATER_1_MINTEMP, HEATER_1_MAX6675_TMIN + .01)) min_temp_error(1);
  #endif

  #if WATCH_HOTENDS || WATCH_THE_BED || DISABLED(PIDTEMPBED) || HAS_AUTO_FAN || HEATER_IDLE_HANDLER
    millis_t ms = millis();
  #endif

  HOTEND_LOOP() {

    #if HEATER_IDLE_HANDLER
      if (!heater_idle_timeout_exceeded[e] && heater_idle_timeout_ms[e] && ELAPSED(ms, heater_idle_timeout_ms[e]))
        heater_idle_timeout_exceeded[e] = true;
    #endif

    #if ENABLED(THERMAL_PROTECTION_HOTENDS)
      // Check for thermal runaway
      thermal_runaway_protection(&thermal_runaway_state_machine[e], &thermal_runaway_timer[e], current_temperature[e], target_temperature[e], e, THERMAL_PROTECTION_PERIOD, THERMAL_PROTECTION_HYSTERESIS);
    #endif

    soft_pwm_amount[e] = (current_temperature[e] > minttemp[e] || is_preheating(e)) && current_temperature[e] < maxttemp[e] ? (int)get_pid_output(e) >> 1 : 0;

    #if WATCH_HOTENDS
      // Make sure temperature is increasing
      if (watch_heater_next_ms[e] && ELAPSED(ms, watch_heater_next_ms[e])) { // Time to check this extruder?
        if (degHotend(e) < watch_target_temp[e])                             // Failed to increase enough?
          _temp_error(e, PSTR(MSG_T_HEATING_FAILED), TEMP_ERR_PSTR(MSG_HEATING_FAILED_LCD, e));
        else                                                                 // Start again if the target is still far off
          start_watching_heater(e);
      }
    #endif

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      // Make sure measured temperatures are close together
      if (ABS(current_temperature[0] - redundant_temperature) > MAX_REDUNDANT_TEMP_SENSOR_DIFF)
        _temp_error(0, PSTR(MSG_REDUNDANCY), PSTR(MSG_ERR_REDUNDANT_TEMP));
    #endif

  } // HOTEND_LOOP

  #if HAS_AUTO_FAN
    if (ELAPSED(ms, next_auto_fan_check_ms)) { // only need to check fan state very infrequently
      checkExtruderAutoFans();
      next_auto_fan_check_ms = ms + 2500UL;
    }
  #endif

  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    /**
     * Filament Width Sensor dynamically sets the volumetric multiplier
     * based on a delayed measurement of the filament diameter.
     */
    if (filament_sensor) {
      meas_shift_index = filwidth_delay_index[0] - meas_delay_cm;
      if (meas_shift_index < 0) meas_shift_index += MAX_MEASUREMENT_DELAY + 1;  //loop around buffer if needed
      meas_shift_index = constrain(meas_shift_index, 0, MAX_MEASUREMENT_DELAY);
      planner.calculate_volumetric_for_width_sensor(measurement_delay[meas_shift_index]);
    }
  #endif // FILAMENT_WIDTH_SENSOR

  #if HAS_HEATED_BED

    #if WATCH_THE_BED
      // Make sure temperature is increasing
      if (watch_bed_next_ms && ELAPSED(ms, watch_bed_next_ms)) {        // Time to check the bed?
        if (degBed() < watch_target_bed_temp)                           // Failed to increase enough?
          _temp_error(-1, PSTR(MSG_T_HEATING_FAILED), TEMP_ERR_PSTR(MSG_HEATING_FAILED_LCD, -1));
        else                                                            // Start again if the target is still far off
          start_watching_bed();
      }
    #endif // WATCH_THE_BED

    #if DISABLED(PIDTEMPBED)
      if (PENDING(ms, next_bed_check_ms)
        #if ENABLED(PROBING_HEATERS_OFF) && ENABLED(BED_LIMIT_SWITCHING)
          && paused == last_pause_state
        #endif
      ) return;
      next_bed_check_ms = ms + BED_CHECK_INTERVAL;
      #if ENABLED(PROBING_HEATERS_OFF) && ENABLED(BED_LIMIT_SWITCHING)
        last_pause_state = paused;
      #endif
    #endif

    #if HEATER_IDLE_HANDLER
      if (!bed_idle_timeout_exceeded && bed_idle_timeout_ms && ELAPSED(ms, bed_idle_timeout_ms))
        bed_idle_timeout_exceeded = true;
    #endif

    #if HAS_THERMALLY_PROTECTED_BED
      thermal_runaway_protection(&thermal_runaway_bed_state_machine, &thermal_runaway_bed_timer, current_temperature_bed, target_temperature_bed, -1, THERMAL_PROTECTION_BED_PERIOD, THERMAL_PROTECTION_BED_HYSTERESIS);
    #endif

    #if HEATER_IDLE_HANDLER
      if (bed_idle_timeout_exceeded) {
        soft_pwm_amount_bed = 0;
        #if DISABLED(PIDTEMPBED)
          WRITE_HEATER_BED(LOW);
        #endif
      }
      else
    #endif
    {
      #if ENABLED(PIDTEMPBED)
        soft_pwm_amount_bed = WITHIN(current_temperature_bed, BED_MINTEMP, BED_MAXTEMP) ? (int)get_pid_output_bed() >> 1 : 0;
      #else
        // Check if temperature is within the correct band
        if (WITHIN(current_temperature_bed, BED_MINTEMP, BED_MAXTEMP)) {
          #if ENABLED(BED_LIMIT_SWITCHING)
            if (current_temperature_bed >= target_temperature_bed + BED_HYSTERESIS)
              soft_pwm_amount_bed = 0;
            else if (current_temperature_bed <= target_temperature_bed - (BED_HYSTERESIS))
              soft_pwm_amount_bed = MAX_BED_POWER >> 1;
          #else // !PIDTEMPBED && !BED_LIMIT_SWITCHING
            soft_pwm_amount_bed = current_temperature_bed < target_temperature_bed ? MAX_BED_POWER >> 1 : 0;
          #endif
        }
        else {
          soft_pwm_amount_bed = 0;
          WRITE_HEATER_BED(LOW);
        }
      #endif
    }
  #endif // HAS_HEATED_BED
}

#define TEMP_AD595(RAW)  ((RAW) * 5.0 * 100.0 / 1024.0 / (OVERSAMPLENR) * (TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET)
#define TEMP_AD8495(RAW) ((RAW) * 6.6 * 100.0 / 1024.0 / (OVERSAMPLENR) * (TEMP_SENSOR_AD8495_GAIN) + TEMP_SENSOR_AD8495_OFFSET)

/**
 * Bisect search for the range of the 'raw' value, then interpolate
 * proportionally between the under and over values.
 */
#define SCAN_THERMISTOR_TABLE(TBL,LEN) do{                             \
  uint8_t l = 0, r = LEN, m;                                           \
  for (;;) {                                                           \
    m = (l + r) >> 1;                                                  \
    if (m == l || m == r) return (short)pgm_read_word(&TBL[LEN-1][1]); \
    short v00 = pgm_read_word(&TBL[m-1][0]),                           \
          v10 = pgm_read_word(&TBL[m-0][0]);                           \
         if (raw < v00) r = m;                                         \
    else if (raw > v10) l = m;                                         \
    else {                                                             \
      const short v01 = (short)pgm_read_word(&TBL[m-1][1]),            \
                  v11 = (short)pgm_read_word(&TBL[m-0][1]);            \
      return v01 + (raw - v00) * float(v11 - v01) / float(v10 - v00);  \
    }                                                                  \
  }                                                                    \
}while(0)

// Derived from RepRap FiveD extruder::getTemperature()
// For hot end temperature measurement.
float Temperature::analog_to_celsius_hotend(const int raw, const uint8_t e) {
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    if (e > HOTENDS)
  #else
    if (e >= HOTENDS)
  #endif
    {
      SERIAL_ERROR_START();
      SERIAL_ECHO((int)e);
      SERIAL_ECHOLNPGM(MSG_INVALID_EXTRUDER_NUM);
      kill();
      return 0.0;
    }

  switch (e) {
    case 0:
      #if ENABLED(HEATER_0_USES_MAX6675)
        return raw * 0.25;
      #elif ENABLED(HEATER_0_USES_AD595)
        return TEMP_AD595(raw);
      #elif ENABLED(HEATER_0_USES_AD8495)
        return TEMP_AD8495(raw);
      #else
        break;
      #endif
    case 1:
      #if ENABLED(HEATER_1_USES_MAX6675)
        return raw * 0.25;
      #elif ENABLED(HEATER_1_USES_AD595)
        return TEMP_AD595(raw);
      #elif ENABLED(HEATER_1_USES_AD8495)
        return TEMP_AD8495(raw);
      #else
        break;
      #endif
    case 2:
      #if ENABLED(HEATER_2_USES_AD595)
        return TEMP_AD595(raw);
      #elif ENABLED(HEATER_2_USES_AD8495)
        return TEMP_AD8495(raw);
      #else
        break;
      #endif
    case 3:
      #if ENABLED(HEATER_3_USES_AD595)
        return TEMP_AD595(raw);
      #elif ENABLED(HEATER_3_USES_AD8495)
        return TEMP_AD8495(raw);
      #else
        break;
      #endif
    case 4:
      #if ENABLED(HEATER_4_USES_AD595)
        return TEMP_AD595(raw);
      #elif ENABLED(HEATER_4_USES_AD8495)
        return TEMP_AD8495(raw);
      #else
        break;
      #endif
    default: break;
  }

  #if HOTEND_USES_THERMISTOR
    // Thermistor with conversion table?
    const short(*tt)[][2] = (short(*)[][2])(heater_ttbl_map[e]);
    SCAN_THERMISTOR_TABLE((*tt), heater_ttbllen_map[e]);
  #endif

  return 0;
}

#if HAS_HEATED_BED
  // Derived from RepRap FiveD extruder::getTemperature()
  // For bed temperature measurement.
  float Temperature::analog_to_celsius_bed(const int raw) {
    #if ENABLED(HEATER_BED_USES_THERMISTOR)
      SCAN_THERMISTOR_TABLE(BEDTEMPTABLE, BEDTEMPTABLE_LEN);
    #elif ENABLED(HEATER_BED_USES_AD595)
      return TEMP_AD595(raw);
    #elif ENABLED(HEATER_BED_USES_AD8495)
      return TEMP_AD8495(raw);
    #else
      return 0;
    #endif
  }
#endif // HAS_HEATED_BED

#if HAS_TEMP_CHAMBER
  // Derived from RepRap FiveD extruder::getTemperature()
  // For chamber temperature measurement.
  float Temperature::analog_to_celsiusChamber(const int raw) {
    #if ENABLED(HEATER_CHAMBER_USES_THERMISTOR)
      SCAN_THERMISTOR_TABLE(CHAMBERTEMPTABLE, CHAMBERTEMPTABLE_LEN);
    #elif ENABLED(HEATER_CHAMBER_USES_AD595)
      return TEMP_AD595(raw);
    #elif ENABLED(HEATER_CHAMBER_USES_AD8495)
      return TEMP_AD8495(raw);
    #else
      return 0;
    #endif
  }
#endif // HAS_TEMP_CHAMBER

/**
 * Get the raw values into the actual temperatures.
 * The raw values are created in interrupt context,
 * and this function is called from normal context
 * as it would block the stepper routine.
 */
void Temperature::updateTemperaturesFromRawValues() {
  #if ENABLED(HEATER_0_USES_MAX6675)
    current_temperature_raw[0] = READ_MAX6675(0);
  #endif
  #if ENABLED(HEATER_1_USES_MAX6675)
    current_temperature_raw[1] = READ_MAX6675(1);
  #endif
  HOTEND_LOOP() current_temperature[e] = analog_to_celsius_hotend(current_temperature_raw[e], e);
  #if HAS_HEATED_BED
    current_temperature_bed = analog_to_celsius_bed(current_temperature_bed_raw);
  #endif
  #if HAS_TEMP_CHAMBER
    current_temperature_chamber = analog_to_celsiusChamber(current_temperature_chamber_raw);
  #endif
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    redundant_temperature = analog_to_celsius_hotend(redundant_temperature_raw, 1);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    filament_width_meas = analog_to_mm_fil_width();
  #endif

  #if ENABLED(USE_WATCHDOG)
    // Reset the watchdog after we know we have a temperature measurement.
    watchdog_reset();
  #endif

  temp_meas_ready = false;
}


#if ENABLED(FILAMENT_WIDTH_SENSOR)

  // Convert raw Filament Width to millimeters
  float Temperature::analog_to_mm_fil_width() {
    return current_raw_filwidth * 5.0f * (1.0f / 16383.0f);
  }

  /**
   * Convert Filament Width (mm) to a simple ratio
   * and reduce to an 8 bit value.
   *
   * A nominal width of 1.75 and measured width of 1.73
   * gives (100 * 1.75 / 1.73) for a ratio of 101 and
   * a return value of 1.
   */
  int8_t Temperature::widthFil_to_size_ratio() {
    if (ABS(filament_width_nominal - filament_width_meas) <= FILWIDTH_ERROR_MARGIN)
      return int(100.0f * filament_width_nominal / filament_width_meas) - 100;
    return 0;
  }

#endif

#if MAX6675_SEPARATE_SPI
  SPIclass<MAX6675_DO_PIN, MOSI_PIN, MAX6675_SCK_PIN> max6675_spi;
#endif

/**
 * Initialize the temperature manager
 * The manager is implemented by periodic calls to manage_heater()
 */
void Temperature::init() {

  #if EARLY_WATCHDOG
    // Flag that the thermalManager should be running
    if (inited) return;
    inited = true;
  #endif

  #if MB(RUMBA) && ( \
       ENABLED(HEATER_0_USES_AD595)  || ENABLED(HEATER_1_USES_AD595)  || ENABLED(HEATER_2_USES_AD595)  || ENABLED(HEATER_3_USES_AD595)  || ENABLED(HEATER_4_USES_AD595)  || ENABLED(HEATER_BED_USES_AD595)  || ENABLED(HEATER_CHAMBER_USES_AD595) \
    || ENABLED(HEATER_0_USES_AD8495) || ENABLED(HEATER_1_USES_AD8495) || ENABLED(HEATER_2_USES_AD8495) || ENABLED(HEATER_3_USES_AD8495) || ENABLED(HEATER_4_USES_AD8495) || ENABLED(HEATER_BED_USES_AD8495) || ENABLED(HEATER_CHAMBER_USES_AD8495))
    // Disable RUMBA JTAG in case the thermocouple extension is plugged on top of JTAG connector
    MCUCR = _BV(JTD);
    MCUCR = _BV(JTD);
  #endif

  // Finish init of mult hotend arrays
  HOTEND_LOOP() maxttemp[e] = maxttemp[0];

  #if ENABLED(PIDTEMP) && ENABLED(PID_EXTRUSION_SCALING)
    last_e_position = 0;
  #endif

  #if HAS_HEATER_0
    OUT_WRITE(HEATER_0_PIN, HEATER_0_INVERTING);
  #endif
  #if HAS_HEATER_1
    OUT_WRITE(HEATER_1_PIN, HEATER_1_INVERTING);
  #endif
  #if HAS_HEATER_2
    OUT_WRITE(HEATER_2_PIN, HEATER_2_INVERTING);
  #endif
  #if HAS_HEATER_3
    OUT_WRITE(HEATER_3_PIN, HEATER_3_INVERTING);
  #endif
  #if HAS_HEATER_4
    OUT_WRITE(HEATER_4_PIN, HEATER_4_INVERTING);
  #endif
  #if HAS_HEATED_BED
    OUT_WRITE(HEATER_BED_PIN, HEATER_BED_INVERTING);
  #endif

  #if HAS_FAN0
    SET_OUTPUT(FAN_PIN);
    #if ENABLED(FAST_PWM_FAN)
      setPwmFrequency(FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
  #endif

  #if HAS_FAN1
    SET_OUTPUT(FAN1_PIN);
    #if ENABLED(FAST_PWM_FAN)
      setPwmFrequency(FAN1_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
  #endif

  #if HAS_FAN2
    SET_OUTPUT(FAN2_PIN);
    #if ENABLED(FAST_PWM_FAN)
      setPwmFrequency(FAN2_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
  #endif

  #if ENABLED(USE_CONTROLLER_FAN)
    SET_OUTPUT(CONTROLLER_FAN_PIN);
    #if ENABLED(FAST_PWM_FAN)
      setPwmFrequency(CONTROLLER_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
    #endif
  #endif

  #if MAX6675_SEPARATE_SPI

    OUT_WRITE(SCK_PIN, LOW);
    OUT_WRITE(MOSI_PIN, HIGH);
    SET_INPUT_PULLUP(MISO_PIN);

    max6675_spi.init();

    OUT_WRITE(SS_PIN, HIGH);
    OUT_WRITE(MAX6675_SS_PIN, HIGH);

  #endif

  #if ENABLED(HEATER_1_USES_MAX6675)
    OUT_WRITE(MAX6675_SS2_PIN, HIGH);
  #endif

  HAL_adc_init();

  #if HAS_TEMP_ADC_0
    HAL_ANALOG_SELECT(TEMP_0_PIN);
  #endif
  #if HAS_TEMP_ADC_1
    HAL_ANALOG_SELECT(TEMP_1_PIN);
  #endif
  #if HAS_TEMP_ADC_2
    HAL_ANALOG_SELECT(TEMP_2_PIN);
  #endif
  #if HAS_TEMP_ADC_3
    HAL_ANALOG_SELECT(TEMP_3_PIN);
  #endif
  #if HAS_TEMP_ADC_4
    HAL_ANALOG_SELECT(TEMP_4_PIN);
  #endif
  #if HAS_TEMP_ADC_5
    HAL_ANALOG_SELECT(TEMP_5_PIN);
  #endif
  #if HAS_HEATED_BED
    HAL_ANALOG_SELECT(TEMP_BED_PIN);
  #endif
  #if HAS_TEMP_CHAMBER
    HAL_ANALOG_SELECT(TEMP_CHAMBER_PIN);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    HAL_ANALOG_SELECT(FILWIDTH_PIN);
  #endif

  HAL_timer_start(TEMP_TIMER_NUM, TEMP_TIMER_FREQUENCY);
  ENABLE_TEMPERATURE_INTERRUPT();

  #if HAS_AUTO_FAN_0
    #if E0_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(E0_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(E0_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      SET_OUTPUT(E0_AUTO_FAN_PIN);
    #endif
  #endif
  #if HAS_AUTO_FAN_1 && !AUTO_1_IS_0
    #if E1_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(E1_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(E1_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      SET_OUTPUT(E1_AUTO_FAN_PIN);
    #endif
  #endif
  #if HAS_AUTO_FAN_2 && !(AUTO_2_IS_0 || AUTO_2_IS_1)
    #if E2_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(E2_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(E2_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      SET_OUTPUT(E2_AUTO_FAN_PIN);
    #endif
  #endif
  #if HAS_AUTO_FAN_3 && !(AUTO_3_IS_0 || AUTO_3_IS_1 || AUTO_3_IS_2)
    #if E3_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(E3_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(E3_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      SET_OUTPUT(E3_AUTO_FAN_PIN);
    #endif
  #endif
  #if HAS_AUTO_FAN_4 && !(AUTO_4_IS_0 || AUTO_4_IS_1 || AUTO_4_IS_2 || AUTO_4_IS_3)
    #if E4_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(E4_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(E4_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      SET_OUTPUT(E4_AUTO_FAN_PIN);
    #endif
  #endif
  #if HAS_AUTO_FAN_5 && !(AUTO_5_IS_0 || AUTO_5_IS_1 || AUTO_5_IS_2 || AUTO_5_IS_3 || AUTO_5_IS_4)
    #if E5_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(E5_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(E5_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      SET_OUTPUT(E5_AUTO_FAN_PIN);
    #endif
  #endif
  #if HAS_AUTO_CHAMBER_FAN && !(AUTO_CHAMBER_IS_0 || AUTO_CHAMBER_IS_1 || AUTO_CHAMBER_IS_2 || AUTO_CHAMBER_IS_3 || AUTO_CHAMBER_IS_4 || AUTO_CHAMBER_IS_5)
    #if CHAMBER_AUTO_FAN_PIN == FAN1_PIN
      SET_OUTPUT(CHAMBER_AUTO_FAN_PIN);
      #if ENABLED(FAST_PWM_FAN)
        setPwmFrequency(CHAMBER_AUTO_FAN_PIN, 1); // No prescaling. Pwm frequency = F_CPU/256/8
      #endif
    #else
      SET_OUTPUT(CHAMBER_AUTO_FAN_PIN);
    #endif
  #endif

  // Wait for temperature measurement to settle
  delay(250);

  #define TEMP_MIN_ROUTINE(NR) \
    minttemp[NR] = HEATER_ ##NR## _MINTEMP; \
    while (analog_to_celsius_hotend(minttemp_raw[NR], NR) < HEATER_ ##NR## _MINTEMP) { \
      if (HEATER_ ##NR## _RAW_LO_TEMP < HEATER_ ##NR## _RAW_HI_TEMP) \
        minttemp_raw[NR] += OVERSAMPLENR; \
      else \
        minttemp_raw[NR] -= OVERSAMPLENR; \
    }
  #define TEMP_MAX_ROUTINE(NR) \
    maxttemp[NR] = HEATER_ ##NR## _MAXTEMP; \
    while (analog_to_celsius_hotend(maxttemp_raw[NR], NR) > HEATER_ ##NR## _MAXTEMP) { \
      if (HEATER_ ##NR## _RAW_LO_TEMP < HEATER_ ##NR## _RAW_HI_TEMP) \
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
        #if HOTENDS > 4
          #ifdef HEATER_4_MINTEMP
            TEMP_MIN_ROUTINE(4);
          #endif
          #ifdef HEATER_4_MAXTEMP
            TEMP_MAX_ROUTINE(4);
          #endif
          #if HOTENDS > 5
            #ifdef HEATER_5_MINTEMP
              TEMP_MIN_ROUTINE(5);
            #endif
            #ifdef HEATER_5_MAXTEMP
              TEMP_MAX_ROUTINE(5);
            #endif
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1

  #if HAS_HEATED_BED
    #ifdef BED_MINTEMP
      while (analog_to_celsius_bed(bed_minttemp_raw) < BED_MINTEMP) {
        #if HEATER_BED_RAW_LO_TEMP < HEATER_BED_RAW_HI_TEMP
          bed_minttemp_raw += OVERSAMPLENR;
        #else
          bed_minttemp_raw -= OVERSAMPLENR;
        #endif
      }
    #endif // BED_MINTEMP
    #ifdef BED_MAXTEMP
      while (analog_to_celsius_bed(bed_maxttemp_raw) > BED_MAXTEMP) {
        #if HEATER_BED_RAW_LO_TEMP < HEATER_BED_RAW_HI_TEMP
          bed_maxttemp_raw -= OVERSAMPLENR;
        #else
          bed_maxttemp_raw += OVERSAMPLENR;
        #endif
      }
    #endif // BED_MAXTEMP
  #endif // HAS_HEATED_BED

  #if ENABLED(PROBING_HEATERS_OFF)
    paused = false;
  #endif
}

#if ENABLED(FAST_PWM_FAN)

  void Temperature::setPwmFrequency(const pin_t pin, int val) {
    #if defined(ARDUINO) && !defined(ARDUINO_ARCH_SAM)
      val &= 0x07;
      switch (digitalPinToTimer(pin)) {
        #ifdef TCCR0A
          #if !AVR_AT90USB1286_FAMILY
            case TIMER0A:
          #endif
          case TIMER0B:                           //_SET_CS(0, val);
                                                    break;
        #endif
        #ifdef TCCR1A
          case TIMER1A: case TIMER1B:             //_SET_CS(1, val);
                                                    break;
        #endif
        #if defined(TCCR2) || defined(TCCR2A)
          #ifdef TCCR2
            case TIMER2:
          #endif
          #ifdef TCCR2A
            case TIMER2A: case TIMER2B:
          #endif
                                                    _SET_CS(2, val); break;
        #endif
        #ifdef TCCR3A
          case TIMER3A: case TIMER3B: case TIMER3C: _SET_CS(3, val); break;
        #endif
        #ifdef TCCR4A
          case TIMER4A: case TIMER4B: case TIMER4C: _SET_CS(4, val); break;
        #endif
        #ifdef TCCR5A
          case TIMER5A: case TIMER5B: case TIMER5C: _SET_CS(5, val); break;
        #endif
      }
    #endif
  }

#endif // FAST_PWM_FAN

#if WATCH_HOTENDS
  /**
   * Start Heating Sanity Check for hotends that are below
   * their target temperature by a configurable margin.
   * This is called when the temperature is set. (M104, M109)
   */
  void Temperature::start_watching_heater(const uint8_t e) {
    #if HOTENDS == 1
      UNUSED(e);
    #endif
    if (degTargetHotend(HOTEND_INDEX) && degHotend(HOTEND_INDEX) < degTargetHotend(HOTEND_INDEX) - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1)) {
      watch_target_temp[HOTEND_INDEX] = degHotend(HOTEND_INDEX) + WATCH_TEMP_INCREASE;
      watch_heater_next_ms[HOTEND_INDEX] = millis() + (WATCH_TEMP_PERIOD) * 1000UL;
    }
    else
      watch_heater_next_ms[HOTEND_INDEX] = 0;
  }
#endif

#if WATCH_THE_BED
  /**
   * Start Heating Sanity Check for hotends that are below
   * their target temperature by a configurable margin.
   * This is called when the temperature is set. (M140, M190)
   */
  void Temperature::start_watching_bed() {
    if (degTargetBed() && degBed() < degTargetBed() - (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1)) {
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

  void Temperature::thermal_runaway_protection(Temperature::TRState * const state, millis_t * const timer, const float &current, const float &target, const int8_t heater_id, const uint16_t period_seconds, const uint16_t hysteresis_degc) {

    static float tr_target_temperature[HOTENDS + 1] = { 0.0 };

    /**
        SERIAL_ECHO_START();
        SERIAL_ECHOPGM("Thermal Thermal Runaway Running. Heater ID: ");
        if (heater_id < 0) SERIAL_ECHOPGM("bed"); else SERIAL_ECHO(heater_id);
        SERIAL_ECHOPAIR(" ;  State:", *state);
        SERIAL_ECHOPAIR(" ;  Timer:", *timer);
        SERIAL_ECHOPAIR(" ;  Temperature:", current);
        SERIAL_ECHOPAIR(" ;  Target Temp:", target);
        if (heater_id >= 0)
          SERIAL_ECHOPAIR(" ;  Idle Timeout:", heater_idle_timeout_exceeded[heater_id]);
        else
          SERIAL_ECHOPAIR(" ;  Idle Timeout:", bed_idle_timeout_exceeded);
        SERIAL_EOL();
    */

    const int heater_index = heater_id >= 0 ? heater_id : HOTENDS;

    #if HEATER_IDLE_HANDLER
      // If the heater idle timeout expires, restart
      if ((heater_id >= 0 && heater_idle_timeout_exceeded[heater_id])
        #if HAS_HEATED_BED
          || (heater_id < 0 && bed_idle_timeout_exceeded)
        #endif
      ) {
        *state = TRInactive;
        tr_target_temperature[heater_index] = 0;
      }
      else
    #endif
    {
      // If the target temperature changes, restart
      if (tr_target_temperature[heater_index] != target) {
        tr_target_temperature[heater_index] = target;
        *state = target > 0 ? TRFirstHeating : TRInactive;
      }
    }

    switch (*state) {
      // Inactive state waits for a target temperature to be set
      case TRInactive: break;

      // When first heating, wait for the temperature to be reached then go to Stable state
      case TRFirstHeating:
        if (current < tr_target_temperature[heater_index]) break;
        *state = TRStable;

      // While the temperature is stable watch for a bad temperature
      case TRStable:

        #if ENABLED(ADAPTIVE_FAN_SLOWING)
          if (adaptive_fan_slowing && heater_id >= 0) {
            const int fan_index = MIN(heater_id, FAN_COUNT - 1);
            if (fan_speed[fan_index] == 0 || current >= tr_target_temperature[heater_id] - (hysteresis_degc * 0.25f))
              fan_speed_scaler[fan_index] = 128;
            else if (current >= tr_target_temperature[heater_id] - (hysteresis_degc * 0.3335f))
              fan_speed_scaler[fan_index] = 96;
            else if (current >= tr_target_temperature[heater_id] - (hysteresis_degc * 0.5f))
              fan_speed_scaler[fan_index] = 64;
            else if (current >= tr_target_temperature[heater_id] - (hysteresis_degc * 0.8f))
              fan_speed_scaler[fan_index] = 32;
            else
              fan_speed_scaler[fan_index] = 0;
          }
        #endif

        if (current >= tr_target_temperature[heater_index] - hysteresis_degc) {
          *timer = millis() + period_seconds * 1000UL;
          break;
        }
        else if (PENDING(millis(), *timer)) break;
        *state = TRRunaway;

      case TRRunaway:
        _temp_error(heater_id, PSTR(MSG_T_THERMAL_RUNAWAY), TEMP_ERR_PSTR(MSG_THERMAL_RUNAWAY, heater_id));
    }
  }

#endif // THERMAL_PROTECTION_HOTENDS || THERMAL_PROTECTION_BED

void Temperature::disable_all_heaters() {

  #if ENABLED(AUTOTEMP)
    planner.autotemp_enabled = false;
  #endif

  HOTEND_LOOP() setTargetHotend(0, e);

  #if HAS_HEATED_BED
    setTargetBed(0);
  #endif

  // Unpause and reset everything
  #if ENABLED(PROBING_HEATERS_OFF)
    pause(false);
  #endif

  #define DISABLE_HEATER(NR) { \
    setTargetHotend(0, NR); \
    soft_pwm_amount[NR] = 0; \
    WRITE_HEATER_ ##NR (LOW); \
  }

  #if HAS_TEMP_HOTEND
    DISABLE_HEATER(0);
    #if HOTENDS > 1
      DISABLE_HEATER(1);
      #if HOTENDS > 2
        DISABLE_HEATER(2);
        #if HOTENDS > 3
          DISABLE_HEATER(3);
          #if HOTENDS > 4
            DISABLE_HEATER(4);
            #if HOTENDS > 5
              DISABLE_HEATER(5);
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1
  #endif

  #if HAS_HEATED_BED
    target_temperature_bed = 0;
    soft_pwm_amount_bed = 0;
    #if HAS_HEATED_BED
      WRITE_HEATER_BED(LOW);
    #endif
  #endif
}

#if ENABLED(PROBING_HEATERS_OFF)

  void Temperature::pause(const bool p) {
    if (p != paused) {
      paused = p;
      if (p) {
        HOTEND_LOOP() start_heater_idle_timer(e, 0); // timeout immediately
        #if HAS_HEATED_BED
          start_bed_idle_timer(0); // timeout immediately
        #endif
      }
      else {
        HOTEND_LOOP() reset_heater_idle_timer(e);
        #if HAS_HEATED_BED
          reset_bed_idle_timer();
        #endif
      }
    }
  }

#endif // PROBING_HEATERS_OFF

#if HAS_MAX6675

  int Temperature::read_max6675(
    #if COUNT_6675 > 1
      const uint8_t hindex
    #endif
  ) {
    #if COUNT_6675 == 1
      constexpr uint8_t hindex = 0;
    #else
      // Needed to return the correct temp when this is called too soon
      static uint16_t max6675_temp_previous[COUNT_6675] = { 0 };
    #endif

    #define MAX6675_HEAT_INTERVAL 250UL

    #if ENABLED(MAX6675_IS_MAX31855)
      static uint32_t max6675_temp = 2000;
      #define MAX6675_ERROR_MASK    7
      #define MAX6675_DISCARD_BITS 18
      #define MAX6675_SPEED_BITS    3  // (_BV(SPR1)) // clock ÷ 64
    #else
      static uint16_t max6675_temp = 2000;
      #define MAX6675_ERROR_MASK    4
      #define MAX6675_DISCARD_BITS  3
      #define MAX6675_SPEED_BITS    2  // (_BV(SPR0)) // clock ÷ 16
    #endif

    // Return last-read value between readings
    static millis_t next_max6675_ms[COUNT_6675] = { 0 };
    millis_t ms = millis();
    if (PENDING(ms, next_max6675_ms[hindex]))
      return int(
        #if COUNT_6675 == 1
          max6675_temp
        #else
          max6675_temp_previous[hindex] // Need to return the correct previous value
        #endif
      );

    next_max6675_ms[hindex] = ms + MAX6675_HEAT_INTERVAL;

    //
    // TODO: spiBegin, spiRec and spiInit doesn't work when soft spi is used.
    //
    #if MAX6675_SEPARATE_SPI
      spiBegin();
      spiInit(MAX6675_SPEED_BITS);
    #endif

    #if COUNT_6675 > 1
      #define WRITE_MAX6675(V) do{ switch (hindex) { case 1: WRITE(MAX6675_SS2_PIN, V); break; default: WRITE(MAX6675_SS_PIN, V); } }while(0)
    #elif ENABLED(HEATER_1_USES_MAX6675)
      #define WRITE_MAX6675(V) WRITE(MAX6675_SS2_PIN, V)
    #else
      #define WRITE_MAX6675(V) WRITE(MAX6675_SS_PIN, V)
    #endif

    WRITE_MAX6675(LOW);  // enable TT_MAX6675

    DELAY_NS(100);       // Ensure 100ns delay

    // Read a big-endian temperature value
    max6675_temp = 0;
    for (uint8_t i = sizeof(max6675_temp); i--;) {
      max6675_temp |= (
        #if MAX6675_SEPARATE_SPI
          max6675_spi.receive()
        #else
          spiRec()
        #endif
      );
      if (i > 0) max6675_temp <<= 8; // shift left if not the last byte
    }

    WRITE_MAX6675(HIGH); // disable TT_MAX6675

    if (max6675_temp & MAX6675_ERROR_MASK) {
      SERIAL_ERROR_START();
      SERIAL_ECHOPGM("Temp measurement error! ");
      #if MAX6675_ERROR_MASK == 7
        SERIAL_ECHOPGM("MAX31855 ");
        if (max6675_temp & 1)
          SERIAL_ECHOLNPGM("Open Circuit");
        else if (max6675_temp & 2)
          SERIAL_ECHOLNPGM("Short to GND");
        else if (max6675_temp & 4)
          SERIAL_ECHOLNPGM("Short to VCC");
      #else
        SERIAL_ECHOLNPGM("MAX6675");
      #endif

      // Thermocouple open
      max6675_temp = 4 * (
        #if COUNT_6675 > 1
          hindex ? HEATER_1_MAX6675_TMAX : HEATER_0_MAX6675_TMAX
        #elif ENABLED(HEATER_1_USES_MAX6675)
          HEATER_1_MAX6675_TMAX
        #else
          HEATER_0_MAX6675_TMAX
        #endif
      );
    }
    else
      max6675_temp >>= MAX6675_DISCARD_BITS;

    #if ENABLED(MAX6675_IS_MAX31855)
      if (max6675_temp & 0x00002000) max6675_temp |= 0xFFFFC000; // Support negative temperature
    #endif

    #if COUNT_6675 > 1
      max6675_temp_previous[hindex] = max6675_temp;
    #endif

    return int(max6675_temp);
  }

#endif // HAS_MAX6675

/**
 * Get raw temperatures
 */
void Temperature::set_current_temp_raw() {

  #if HAS_TEMP_ADC_0 && DISABLED(HEATER_0_USES_MAX6675)
    current_temperature_raw[0] = raw_temp_value[0];
  #endif

  #if HAS_TEMP_ADC_1

    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      redundant_temperature_raw = raw_temp_value[1];
    #elif DISABLED(HEATER_1_USES_MAX6675)
      current_temperature_raw[1] = raw_temp_value[1];
    #endif

    #if HAS_TEMP_ADC_2
      current_temperature_raw[2] = raw_temp_value[2];
      #if HAS_TEMP_ADC_3
        current_temperature_raw[3] = raw_temp_value[3];
        #if HAS_TEMP_ADC_4
          current_temperature_raw[4] = raw_temp_value[4];
          #if HAS_TEMP_ADC_5
            current_temperature_raw[5] = raw_temp_value[5];
          #endif // HAS_TEMP_ADC_5
        #endif // HAS_TEMP_ADC_4
      #endif // HAS_TEMP_ADC_3
    #endif // HAS_TEMP_ADC_2

  #endif // HAS_TEMP_ADC_1

  #if HAS_HEATED_BED
    current_temperature_bed_raw = raw_temp_bed_value;
  #endif
  #if HAS_TEMP_CHAMBER
    current_temperature_chamber_raw = raw_temp_chamber_value;
  #endif
  temp_meas_ready = true;
}

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  uint32_t raw_filwidth_value; // = 0
#endif

void Temperature::readings_ready() {
  // Update the raw values if they've been read. Else we could be updating them during reading.
  if (!temp_meas_ready) set_current_temp_raw();

  // Filament Sensor - can be read any time since IIR filtering is used
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    current_raw_filwidth = raw_filwidth_value >> 10;  // Divide to get to 0-16384 range since we used 1/128 IIR filter approach
  #endif

  ZERO(raw_temp_value);

  #if HAS_HEATED_BED
    raw_temp_bed_value = 0;
  #endif

  #if HAS_TEMP_CHAMBER
    raw_temp_chamber_value = 0;
  #endif

  #define TEMPDIR(N) ((HEATER_##N##_RAW_LO_TEMP) > (HEATER_##N##_RAW_HI_TEMP) ? -1 : 1)

  int constexpr temp_dir[] = {
    #if ENABLED(HEATER_0_USES_MAX6675)
       0
    #else
      TEMPDIR(0)
    #endif
    #if HOTENDS > 1
      , TEMPDIR(1)
      #if HOTENDS > 2
        , TEMPDIR(2)
        #if HOTENDS > 3
          , TEMPDIR(3)
          #if HOTENDS > 4
            , TEMPDIR(4)
            #if HOTENDS > 5
              , TEMPDIR(5)
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1
  };

  for (uint8_t e = 0; e < COUNT(temp_dir); e++) {
    const int16_t tdir = temp_dir[e], rawtemp = current_temperature_raw[e] * tdir;
    const bool heater_on = (target_temperature[e] > 0)
      #if ENABLED(PIDTEMP)
        || (soft_pwm_amount[e] > 0)
      #endif
    ;
    if (rawtemp > maxttemp_raw[e] * tdir) max_temp_error(e);
    if (rawtemp < minttemp_raw[e] * tdir && !is_preheating(e) && heater_on) {
      #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
        if (++consecutive_low_temperature_error[e] >= MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED)
      #endif
          min_temp_error(e);
    }
    #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
      else
        consecutive_low_temperature_error[e] = 0;
    #endif
  }

  #if HAS_HEATED_BED
    #if HEATER_BED_RAW_LO_TEMP > HEATER_BED_RAW_HI_TEMP
      #define BEDCMP(A,B) ((A)<=(B))
    #else
      #define BEDCMP(A,B) ((A)>=(B))
    #endif
    const bool bed_on = (target_temperature_bed > 0)
      #if ENABLED(PIDTEMPBED)
        || (soft_pwm_amount_bed > 0)
      #endif
    ;
    if (BEDCMP(current_temperature_bed_raw, bed_maxttemp_raw)) max_temp_error(-1);
    if (BEDCMP(bed_minttemp_raw, current_temperature_bed_raw) && bed_on) min_temp_error(-1);
  #endif
}

/**
 * Timer 0 is shared with millies so don't change the prescaler.
 *
 * On AVR this ISR uses the compare method so it runs at the base
 * frequency (16 MHz / 64 / 256 = 976.5625 Hz), but at the TCNT0 set
 * in OCR0B above (128 or halfway between OVFs).
 *
 *  - Manage PWM to all the heaters and fan
 *  - Prepare or Measure one of the raw ADC sensor values
 *  - Check new temperature values for MIN/MAX errors (kill on error)
 *  - Step the babysteps value for each axis towards 0
 *  - For PINS_DEBUGGING, monitor and report endstop pins
 *  - For ENDSTOP_INTERRUPTS_FEATURE check endstops if flagged
 *  - Call planner.tick to count down its "ignore" time
 */
HAL_TEMP_TIMER_ISR {
  HAL_timer_isr_prologue(TEMP_TIMER_NUM);

  Temperature::isr();

  HAL_timer_isr_epilogue(TEMP_TIMER_NUM);
}

void Temperature::isr() {

  static int8_t temp_count = -1;
  static ADCSensorState adc_sensor_state = StartupDelay;
  static uint8_t pwm_count = _BV(SOFT_PWM_SCALE);
  // avoid multiple loads of pwm_count
  uint8_t pwm_count_tmp = pwm_count;

  #if HAS_ADC_BUTTONS
    static unsigned int raw_ADCKey_value = 0;
  #endif

  // Static members for each heater
  #if ENABLED(SLOW_PWM_HEATERS)
    static uint8_t slow_pwm_count = 0;
    #define ISR_STATICS(n) \
      static uint8_t soft_pwm_count_ ## n, \
                     state_heater_ ## n = 0, \
                     state_timer_heater_ ## n = 0
  #else
    #define ISR_STATICS(n) static uint8_t soft_pwm_count_ ## n = 0
  #endif

  // Statics per heater
  ISR_STATICS(0);
  #if HOTENDS > 1
    ISR_STATICS(1);
    #if HOTENDS > 2
      ISR_STATICS(2);
      #if HOTENDS > 3
        ISR_STATICS(3);
        #if HOTENDS > 4
          ISR_STATICS(4);
          #if HOTENDS > 5
            ISR_STATICS(5);
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1
  #if HAS_HEATED_BED
    ISR_STATICS(BED);
  #endif

  #if DISABLED(SLOW_PWM_HEATERS)
    constexpr uint8_t pwm_mask =
      #if ENABLED(SOFT_PWM_DITHER)
        _BV(SOFT_PWM_SCALE) - 1
      #else
        0
      #endif
    ;

    /**
     * Standard heater PWM modulation
     */
    if (pwm_count_tmp >= 127) {
      pwm_count_tmp -= 127;
      soft_pwm_count_0 = (soft_pwm_count_0 & pwm_mask) + soft_pwm_amount[0];
      WRITE_HEATER_0(soft_pwm_count_0 > pwm_mask ? HIGH : LOW);
      #if HOTENDS > 1
        soft_pwm_count_1 = (soft_pwm_count_1 & pwm_mask) + soft_pwm_amount[1];
        WRITE_HEATER_1(soft_pwm_count_1 > pwm_mask ? HIGH : LOW);
        #if HOTENDS > 2
          soft_pwm_count_2 = (soft_pwm_count_2 & pwm_mask) + soft_pwm_amount[2];
          WRITE_HEATER_2(soft_pwm_count_2 > pwm_mask ? HIGH : LOW);
          #if HOTENDS > 3
            soft_pwm_count_3 = (soft_pwm_count_3 & pwm_mask) + soft_pwm_amount[3];
            WRITE_HEATER_3(soft_pwm_count_3 > pwm_mask ? HIGH : LOW);
            #if HOTENDS > 4
              soft_pwm_count_4 = (soft_pwm_count_4 & pwm_mask) + soft_pwm_amount[4];
              WRITE_HEATER_4(soft_pwm_count_4 > pwm_mask ? HIGH : LOW);
              #if HOTENDS > 5
                soft_pwm_count_5 = (soft_pwm_count_5 & pwm_mask) + soft_pwm_amount[5];
                WRITE_HEATER_5(soft_pwm_count_5 > pwm_mask ? HIGH : LOW);
              #endif // HOTENDS > 5
            #endif // HOTENDS > 4
          #endif // HOTENDS > 3
        #endif // HOTENDS > 2
      #endif // HOTENDS > 1

      #if HAS_HEATED_BED
        soft_pwm_count_BED = (soft_pwm_count_BED & pwm_mask) + soft_pwm_amount_bed;
        WRITE_HEATER_BED(soft_pwm_count_BED > pwm_mask ? HIGH : LOW);
      #endif

      #if ENABLED(FAN_SOFT_PWM)
        #if HAS_FAN0
          soft_pwm_count_fan[0] = (soft_pwm_count_fan[0] & pwm_mask) + (soft_pwm_amount_fan[0] >> 1);
          WRITE_FAN(soft_pwm_count_fan[0] > pwm_mask ? HIGH : LOW);
        #endif
        #if HAS_FAN1
          soft_pwm_count_fan[1] = (soft_pwm_count_fan[1] & pwm_mask) + (soft_pwm_amount_fan[1] >> 1);
          WRITE_FAN1(soft_pwm_count_fan[1] > pwm_mask ? HIGH : LOW);
        #endif
        #if HAS_FAN2
          soft_pwm_count_fan[2] = (soft_pwm_count_fan[2] & pwm_mask) + (soft_pwm_amount_fan[2] >> 1);
          WRITE_FAN2(soft_pwm_count_fan[2] > pwm_mask ? HIGH : LOW);
        #endif
      #endif
    }
    else {
      if (soft_pwm_count_0 <= pwm_count_tmp) WRITE_HEATER_0(LOW);
      #if HOTENDS > 1
        if (soft_pwm_count_1 <= pwm_count_tmp) WRITE_HEATER_1(LOW);
        #if HOTENDS > 2
          if (soft_pwm_count_2 <= pwm_count_tmp) WRITE_HEATER_2(LOW);
          #if HOTENDS > 3
            if (soft_pwm_count_3 <= pwm_count_tmp) WRITE_HEATER_3(LOW);
            #if HOTENDS > 4
              if (soft_pwm_count_4 <= pwm_count_tmp) WRITE_HEATER_4(LOW);
              #if HOTENDS > 5
                if (soft_pwm_count_5 <= pwm_count_tmp) WRITE_HEATER_5(LOW);
              #endif // HOTENDS > 5
            #endif // HOTENDS > 4
          #endif // HOTENDS > 3
        #endif // HOTENDS > 2
      #endif // HOTENDS > 1

      #if HAS_HEATED_BED
        if (soft_pwm_count_BED <= pwm_count_tmp) WRITE_HEATER_BED(LOW);
      #endif

      #if ENABLED(FAN_SOFT_PWM)
        #if HAS_FAN0
          if (soft_pwm_count_fan[0] <= pwm_count_tmp) WRITE_FAN(LOW);
        #endif
        #if HAS_FAN1
          if (soft_pwm_count_fan[1] <= pwm_count_tmp) WRITE_FAN1(LOW);
        #endif
        #if HAS_FAN2
          if (soft_pwm_count_fan[2] <= pwm_count_tmp) WRITE_FAN2(LOW);
        #endif
      #endif
    }

    // SOFT_PWM_SCALE to frequency:
    //
    // 0: 16000000/64/256/128 =   7.6294 Hz
    // 1:                / 64 =  15.2588 Hz
    // 2:                / 32 =  30.5176 Hz
    // 3:                / 16 =  61.0352 Hz
    // 4:                /  8 = 122.0703 Hz
    // 5:                /  4 = 244.1406 Hz
    pwm_count = pwm_count_tmp + _BV(SOFT_PWM_SCALE);

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
      soft_pwm_count_ ##NR = src; \
      if (soft_pwm_count_ ##NR > 0) { \
        if (state_timer_heater_ ##NR == 0) { \
          if (state_heater_ ##NR == 0) state_timer_heater_ ##NR = MIN_STATE_TIME; \
          state_heater_ ##NR = 1; \
          WRITE_HEATER_ ##NR(1); \
        } \
      } \
      else { \
        if (state_timer_heater_ ##NR == 0) { \
          if (state_heater_ ##NR == 1) state_timer_heater_ ##NR = MIN_STATE_TIME; \
          state_heater_ ##NR = 0; \
          WRITE_HEATER_ ##NR(0); \
        } \
      }
    #define SLOW_PWM_ROUTINE(n) _SLOW_PWM_ROUTINE(n, soft_pwm_amount[n])

    #define PWM_OFF_ROUTINE(NR) \
      if (soft_pwm_count_ ##NR < slow_pwm_count) { \
        if (state_timer_heater_ ##NR == 0) { \
          if (state_heater_ ##NR == 1) state_timer_heater_ ##NR = MIN_STATE_TIME; \
          state_heater_ ##NR = 0; \
          WRITE_HEATER_ ##NR (0); \
        } \
      }

    if (slow_pwm_count == 0) {

      SLOW_PWM_ROUTINE(0);
      #if HOTENDS > 1
        SLOW_PWM_ROUTINE(1);
        #if HOTENDS > 2
          SLOW_PWM_ROUTINE(2);
          #if HOTENDS > 3
            SLOW_PWM_ROUTINE(3);
            #if HOTENDS > 4
              SLOW_PWM_ROUTINE(4);
              #if HOTENDS > 5
                SLOW_PWM_ROUTINE(5);
              #endif // HOTENDS > 5
            #endif // HOTENDS > 4
          #endif // HOTENDS > 3
        #endif // HOTENDS > 2
      #endif // HOTENDS > 1
      #if HAS_HEATED_BED
        _SLOW_PWM_ROUTINE(BED, soft_pwm_amount_bed); // BED
      #endif

    } // slow_pwm_count == 0

    PWM_OFF_ROUTINE(0);
    #if HOTENDS > 1
      PWM_OFF_ROUTINE(1);
      #if HOTENDS > 2
        PWM_OFF_ROUTINE(2);
        #if HOTENDS > 3
          PWM_OFF_ROUTINE(3);
          #if HOTENDS > 4
            PWM_OFF_ROUTINE(4);
            #if HOTENDS > 5
              PWM_OFF_ROUTINE(5);
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      PWM_OFF_ROUTINE(BED); // BED
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      if (pwm_count_tmp >= 127) {
        pwm_count_tmp = 0;
        #if HAS_FAN0
          soft_pwm_count_fan[0] = soft_pwm_amount_fan[0] >> 1;
          WRITE_FAN(soft_pwm_count_fan[0] > 0 ? HIGH : LOW);
        #endif
        #if HAS_FAN1
          soft_pwm_count_fan[1] = soft_pwm_amount_fan[1] >> 1;
          WRITE_FAN1(soft_pwm_count_fan[1] > 0 ? HIGH : LOW);
        #endif
        #if HAS_FAN2
          soft_pwm_count_fan[2] = soft_pwm_amount_fan[2] >> 1;
          WRITE_FAN2(soft_pwm_count_fan[2] > 0 ? HIGH : LOW);
        #endif
      }
      #if HAS_FAN0
        if (soft_pwm_count_fan[0] <= pwm_count_tmp) WRITE_FAN(LOW);
      #endif
      #if HAS_FAN1
        if (soft_pwm_count_fan[1] <= pwm_count_tmp) WRITE_FAN1(LOW);
      #endif
      #if HAS_FAN2
        if (soft_pwm_count_fan[2] <= pwm_count_tmp) WRITE_FAN2(LOW);
      #endif
    #endif // FAN_SOFT_PWM

    // SOFT_PWM_SCALE to frequency:
    //
    // 0: 16000000/64/256/128 =   7.6294 Hz
    // 1:                / 64 =  15.2588 Hz
    // 2:                / 32 =  30.5176 Hz
    // 3:                / 16 =  61.0352 Hz
    // 4:                /  8 = 122.0703 Hz
    // 5:                /  4 = 244.1406 Hz
    pwm_count = pwm_count_tmp + _BV(SOFT_PWM_SCALE);

    // increment slow_pwm_count only every 64th pwm_count,
    // i.e. yielding a PWM frequency of 16/128 Hz (8s).
    if (((pwm_count >> SOFT_PWM_SCALE) & 0x3F) == 0) {
      slow_pwm_count++;
      slow_pwm_count &= 0x7F;

      if (state_timer_heater_0 > 0) state_timer_heater_0--;
      #if HOTENDS > 1
        if (state_timer_heater_1 > 0) state_timer_heater_1--;
        #if HOTENDS > 2
          if (state_timer_heater_2 > 0) state_timer_heater_2--;
          #if HOTENDS > 3
            if (state_timer_heater_3 > 0) state_timer_heater_3--;
            #if HOTENDS > 4
              if (state_timer_heater_4 > 0) state_timer_heater_4--;
              #if HOTENDS > 5
                if (state_timer_heater_5 > 0) state_timer_heater_5--;
              #endif // HOTENDS > 5
            #endif // HOTENDS > 4
          #endif // HOTENDS > 3
        #endif // HOTENDS > 2
      #endif // HOTENDS > 1
      #if HAS_HEATED_BED
        if (state_timer_heater_BED > 0) state_timer_heater_BED--;
      #endif
    } // ((pwm_count >> SOFT_PWM_SCALE) & 0x3F) == 0

  #endif // SLOW_PWM_HEATERS

  //
  // Update lcd buttons 488 times per second
  //
  static bool do_buttons;
  if ((do_buttons ^= true)) ui.update_buttons();

  /**
   * One sensor is sampled on every other call of the ISR.
   * Each sensor is read 16 (OVERSAMPLENR) times, taking the average.
   *
   * On each Prepare pass, ADC is started for a sensor pin.
   * On the next pass, the ADC value is read and accumulated.
   *
   * This gives each ADC 0.9765ms to charge up.
   */
  #define ACCUMULATE_ADC(var) do{ \
    if (!HAL_ADC_READY()) next_sensor_state = adc_sensor_state; \
    else var += HAL_READ_ADC(); \
  }while(0)

  ADCSensorState next_sensor_state = adc_sensor_state < SensorsReady ? (ADCSensorState)(int(adc_sensor_state) + 1) : StartSampling;

  switch (adc_sensor_state) {

    case SensorsReady: {
      // All sensors have been read. Stay in this state for a few
      // ISRs to save on calls to temp update/checking code below.
      constexpr int8_t extra_loops = MIN_ADC_ISR_LOOPS - (int8_t)SensorsReady;
      static uint8_t delay_count = 0;
      if (extra_loops > 0) {
        if (delay_count == 0) delay_count = extra_loops;  // Init this delay
        if (--delay_count)                                // While delaying...
          next_sensor_state = SensorsReady;               // retain this state (else, next state will be 0)
        break;
      }
      else {
        adc_sensor_state = StartSampling;                 // Fall-through to start sampling
        next_sensor_state = (ADCSensorState)(int(StartSampling) + 1);
      }
    }

    case StartSampling:                                   // Start of sampling loops. Do updates/checks.
      if (++temp_count >= OVERSAMPLENR) {                 // 10 * 16 * 1/(16000000/64/256)  = 164ms.
        temp_count = 0;
        readings_ready();
      }
      break;

    #if HAS_TEMP_ADC_0
      case PrepareTemp_0:
        HAL_START_ADC(TEMP_0_PIN);
        break;
      case MeasureTemp_0:
        ACCUMULATE_ADC(raw_temp_value[0]);
        break;
    #endif

    #if HAS_HEATED_BED
      case PrepareTemp_BED:
        HAL_START_ADC(TEMP_BED_PIN);
        break;
      case MeasureTemp_BED:
        ACCUMULATE_ADC(raw_temp_bed_value);
        break;
    #endif

    #if HAS_TEMP_CHAMBER
      case PrepareTemp_CHAMBER:
        HAL_START_ADC(TEMP_CHAMBER_PIN);
        break;
      case MeasureTemp_CHAMBER:
        ACCUMULATE_ADC(raw_temp_chamber_value);
        break;
    #endif

    #if HAS_TEMP_ADC_1
      case PrepareTemp_1:
        HAL_START_ADC(TEMP_1_PIN);
        break;
      case MeasureTemp_1:
        ACCUMULATE_ADC(raw_temp_value[1]);
        break;
    #endif

    #if HAS_TEMP_ADC_2
      case PrepareTemp_2:
        HAL_START_ADC(TEMP_2_PIN);
        break;
      case MeasureTemp_2:
        ACCUMULATE_ADC(raw_temp_value[2]);
        break;
    #endif

    #if HAS_TEMP_ADC_3
      case PrepareTemp_3:
        HAL_START_ADC(TEMP_3_PIN);
        break;
      case MeasureTemp_3:
        ACCUMULATE_ADC(raw_temp_value[3]);
        break;
    #endif

    #if HAS_TEMP_ADC_4
      case PrepareTemp_4:
        HAL_START_ADC(TEMP_4_PIN);
        break;
      case MeasureTemp_4:
        ACCUMULATE_ADC(raw_temp_value[4]);
        break;
    #endif

    #if HAS_TEMP_ADC_5
      case PrepareTemp_5:
        HAL_START_ADC(TEMP_5_PIN);
        break;
      case MeasureTemp_5:
        ACCUMULATE_ADC(raw_temp_value[5]);
        break;
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      case Prepare_FILWIDTH:
        HAL_START_ADC(FILWIDTH_PIN);
      break;
      case Measure_FILWIDTH:
        if (!HAL_ADC_READY())
          next_sensor_state = adc_sensor_state; // redo this state
        else if (HAL_READ_ADC() > 102) { // Make sure ADC is reading > 0.5 volts, otherwise don't read.
          raw_filwidth_value -= raw_filwidth_value >> 7; // Subtract 1/128th of the raw_filwidth_value
          raw_filwidth_value += uint32_t(HAL_READ_ADC()) << 7; // Add new ADC reading, scaled by 128
        }
      break;
    #endif

    #if HAS_ADC_BUTTONS
      case Prepare_ADC_KEY:
        HAL_START_ADC(ADC_KEYPAD_PIN);
        break;
      case Measure_ADC_KEY:
        if (!HAL_ADC_READY())
          next_sensor_state = adc_sensor_state; // redo this state
        else if (ADCKey_count < 16) {
          raw_ADCKey_value = HAL_READ_ADC();
          if (raw_ADCKey_value > 900) {
            //ADC Key release
            ADCKey_count = 0;
            current_ADCKey_raw = 0;
          }
          else {
            current_ADCKey_raw += raw_ADCKey_value;
            ADCKey_count++;
          }
        }
        break;
    #endif // ADC_KEYPAD

    case StartupDelay: break;

  } // switch(adc_sensor_state)

  // Go to the next state
  adc_sensor_state = next_sensor_state;

  //
  // Additional ~1KHz Tasks
  //

  #if ENABLED(BABYSTEPPING)
    #if ENABLED(BABYSTEP_XY) || ENABLED(I2C_POSITION_ENCODERS)
      LOOP_XYZ(axis) {
        const int16_t curTodo = babystepsTodo[axis]; // get rid of volatile for performance
        if (curTodo) {
          stepper.babystep((AxisEnum)axis, curTodo > 0);
          if (curTodo > 0) babystepsTodo[axis]--; else babystepsTodo[axis]++;
        }
      }
    #else
      const int16_t curTodo = babystepsTodo[Z_AXIS];
      if (curTodo) {
        stepper.babystep(Z_AXIS, curTodo > 0);
        if (curTodo > 0) babystepsTodo[Z_AXIS]--; else babystepsTodo[Z_AXIS]++;
      }
    #endif
  #endif

  // Poll endstops state, if required
  endstops.poll();

  // Periodically call the planner timer
  planner.tick();
}

#if ENABLED(BABYSTEPPING)

  #if ENABLED(BABYSTEP_ALWAYS_AVAILABLE)
    #define BSA_ENABLE(AXIS) do{ switch (AXIS) { case X_AXIS: enable_X(); break; case Y_AXIS: enable_Y(); break; case Z_AXIS: enable_Z(); } }while(0)
  #else
    #define BSA_ENABLE(AXIS) NOOP
  #endif

  #if ENABLED(BABYSTEP_WITHOUT_HOMING)
    #define CAN_BABYSTEP(AXIS) true
  #else
    #define CAN_BABYSTEP(AXIS) TEST(axis_known_position, AXIS)
  #endif

  extern uint8_t axis_known_position;

  void Temperature::babystep_axis(const AxisEnum axis, const int16_t distance) {
    if (!CAN_BABYSTEP(axis)) return;
    #if IS_CORE
      #if ENABLED(BABYSTEP_XY)
        switch (axis) {
          case CORE_AXIS_1: // X on CoreXY and CoreXZ, Y on CoreYZ
            BSA_ENABLE(CORE_AXIS_1);
            BSA_ENABLE(CORE_AXIS_2);
            babystepsTodo[CORE_AXIS_1] += distance * 2;
            babystepsTodo[CORE_AXIS_2] += distance * 2;
            break;
          case CORE_AXIS_2: // Y on CoreXY, Z on CoreXZ and CoreYZ
            BSA_ENABLE(CORE_AXIS_1);
            BSA_ENABLE(CORE_AXIS_2);
            babystepsTodo[CORE_AXIS_1] += CORESIGN(distance * 2);
            babystepsTodo[CORE_AXIS_2] -= CORESIGN(distance * 2);
            break;
          case NORMAL_AXIS: // Z on CoreXY, Y on CoreXZ, X on CoreYZ
          default:
            BSA_ENABLE(NORMAL_AXIS);
            babystepsTodo[NORMAL_AXIS] += distance;
            break;
        }
      #elif CORE_IS_XZ || CORE_IS_YZ
        // Only Z stepping needs to be handled here
        BSA_ENABLE(CORE_AXIS_1);
        BSA_ENABLE(CORE_AXIS_2);
        babystepsTodo[CORE_AXIS_1] += CORESIGN(distance * 2);
        babystepsTodo[CORE_AXIS_2] -= CORESIGN(distance * 2);
      #else
        BSA_ENABLE(Z_AXIS);
        babystepsTodo[Z_AXIS] += distance;
      #endif
    #else
      #if ENABLED(BABYSTEP_XY)
        BSA_ENABLE(axis);
      #else
        BSA_ENABLE(Z_AXIS);
      #endif
      babystepsTodo[axis] += distance;
    #endif
    #if ENABLED(BABYSTEP_ALWAYS_AVAILABLE)
      gcode.reset_stepper_timeout();
    #endif
  }

#endif // BABYSTEPPING

#if HAS_TEMP_SENSOR

  #include "../gcode/gcode.h"

  static void print_heater_state(const float &c, const float &t
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      , const float r
    #endif
    , const int8_t e=-3
  ) {
    #if !(HAS_HEATED_BED && HAS_TEMP_HOTEND && HAS_TEMP_CHAMBER) && HOTENDS <= 1
      UNUSED(e);
    #endif

    SERIAL_CHAR(' ');
    SERIAL_CHAR(
      #if HAS_TEMP_CHAMBER && HAS_HEATED_BED && HAS_TEMP_HOTEND
        e == -2 ? 'C' : e == -1 ? 'B' : 'T'
      #elif HAS_HEATED_BED && HAS_TEMP_HOTEND
        e == -1 ? 'B' : 'T'
      #elif HAS_TEMP_HOTEND
        'T'
      #else
        'B'
      #endif
    );
    #if HOTENDS > 1
      if (e >= 0) SERIAL_CHAR('0' + e);
    #endif
    SERIAL_CHAR(':');
    SERIAL_ECHO(c);
    SERIAL_ECHOPAIR(" /" , t);
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      SERIAL_ECHOPAIR(" (", r / OVERSAMPLENR);
      SERIAL_CHAR(')');
    #endif
    delay(2);
  }

  void Temperature::print_heater_states(const uint8_t target_extruder) {
    #if HAS_TEMP_HOTEND
      print_heater_state(degHotend(target_extruder), degTargetHotend(target_extruder)
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawHotendTemp(target_extruder)
        #endif
      );
    #endif
    #if HAS_HEATED_BED
      print_heater_state(degBed(), degTargetBed()
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawBedTemp()
        #endif
        , -1 // BED
      );
    #endif
    #if HAS_TEMP_CHAMBER
      print_heater_state(degChamber(), 0
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawChamberTemp()
        #endif
        , -2 // CHAMBER
      );
    #endif
    #if HOTENDS > 1
      HOTEND_LOOP() print_heater_state(degHotend(e), degTargetHotend(e)
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawHotendTemp(e)
        #endif
        , e
      );
    #endif
    SERIAL_ECHOPGM(" @:");
    SERIAL_ECHO(getHeaterPower(target_extruder));
    #if HAS_HEATED_BED
      SERIAL_ECHOPGM(" B@:");
      SERIAL_ECHO(getHeaterPower(-1));
    #endif
    #if HOTENDS > 1
      HOTEND_LOOP() {
        SERIAL_ECHOPAIR(" @", e);
        SERIAL_CHAR(':');
        SERIAL_ECHO(getHeaterPower(e));
      }
    #endif
  }

  #if ENABLED(AUTO_REPORT_TEMPERATURES)

    uint8_t Temperature::auto_report_temp_interval;
    millis_t Temperature::next_temp_report_ms;

    void Temperature::auto_report_temperatures() {
      if (auto_report_temp_interval && ELAPSED(millis(), next_temp_report_ms)) {
        next_temp_report_ms = millis() + 1000UL * auto_report_temp_interval;
        print_heater_states(active_extruder);
        SERIAL_EOL();
      }
    }

  #endif // AUTO_REPORT_TEMPERATURES

  #if ENABLED(ULTRA_LCD) || ENABLED(EXTENSIBLE_UI)
    void Temperature::set_heating_message(const uint8_t e) {
      const bool heating = isHeatingHotend(e);
      #if HOTENDS > 1
        ui.status_printf_P(0, heating ? PSTR("E%i " MSG_HEATING) : PSTR("E%i " MSG_COOLING), int(e + 1));
      #else
        ui.set_status_P(heating ? PSTR("E " MSG_HEATING) : PSTR("E " MSG_COOLING));
      #endif
    }
  #endif

  #if HAS_TEMP_HOTEND

    #ifndef MIN_COOLING_SLOPE_DEG
      #define MIN_COOLING_SLOPE_DEG 1.50
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME
      #define MIN_COOLING_SLOPE_TIME 60
    #endif

    bool Temperature::wait_for_hotend(const uint8_t target_extruder, const bool no_wait_for_cooling/*=true*/
      #if G26_CLICK_CAN_CANCEL
        , const bool click_to_cancel/*=false*/
      #endif
    ) {
      #if TEMP_RESIDENCY_TIME > 0
        millis_t residency_start_ms = 0;
        // Loop until the temperature has stabilized
        #define TEMP_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + (TEMP_RESIDENCY_TIME) * 1000UL))
      #else
        // Loop until the temperature is very close target
        #define TEMP_CONDITIONS (wants_to_cool ? isCoolingHotend(target_extruder) : isHeatingHotend(target_extruder))
      #endif

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        const GcodeSuite::MarlinBusyState old_busy_state = gcode.busy_state;
        KEEPALIVE_STATE(NOT_BUSY);
      #endif

      #if ENABLED(PRINTER_EVENT_LEDS)
        const float start_temp = degHotend(target_extruder);
        printerEventLEDs.onHotendHeatingStart();
      #endif

      float target_temp = -1.0, old_temp = 9999.0;
      bool wants_to_cool = false;
      wait_for_heatup = true;
      millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;
      do {
        // Target temperature might be changed during the loop
        if (target_temp != degTargetHotend(target_extruder)) {
          wants_to_cool = isCoolingHotend(target_extruder);
          target_temp = degTargetHotend(target_extruder);

          // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
          if (no_wait_for_cooling && wants_to_cool) break;
        }

        now = millis();
        if (ELAPSED(now, next_temp_ms)) { // Print temp & remaining time every 1s while waiting
          next_temp_ms = now + 1000UL;
          print_heater_states(target_extruder);
          #if TEMP_RESIDENCY_TIME > 0
            SERIAL_ECHOPGM(" W:");
            if (residency_start_ms)
              SERIAL_ECHO(long((((TEMP_RESIDENCY_TIME) * 1000UL) - (now - residency_start_ms)) / 1000UL));
            else
              SERIAL_CHAR('?');
          #endif
          SERIAL_EOL();
        }

        idle();
        gcode.reset_stepper_timeout(); // Keep steppers powered

        const float temp = degHotend(target_extruder);

        #if ENABLED(PRINTER_EVENT_LEDS)
          // Gradually change LED strip from violet to red as nozzle heats up
          if (!wants_to_cool) printerEventLEDs.onHotendHeating(start_temp, temp, target_temp);
        #endif

        #if TEMP_RESIDENCY_TIME > 0

          const float temp_diff = ABS(target_temp - temp);

          if (!residency_start_ms) {
            // Start the TEMP_RESIDENCY_TIME timer when we reach target temp for the first time.
            if (temp_diff < TEMP_WINDOW) residency_start_ms = now;
          }
          else if (temp_diff > TEMP_HYSTERESIS) {
            // Restart the timer whenever the temperature falls outside the hysteresis.
            residency_start_ms = now;
          }

        #endif

        // Prevent a wait-forever situation if R is misused i.e. M109 R0
        if (wants_to_cool) {
          // break after MIN_COOLING_SLOPE_TIME seconds
          // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG
          if (!next_cool_check_ms || ELAPSED(now, next_cool_check_ms)) {
            if (old_temp - temp < float(MIN_COOLING_SLOPE_DEG)) break;
            next_cool_check_ms = now + 1000UL * MIN_COOLING_SLOPE_TIME;
            old_temp = temp;
          }
        }

        #if G26_CLICK_CAN_CANCEL
          if (click_to_cancel && ui.use_click()) {
            wait_for_heatup = false;
            ui.quick_feedback();
          }
        #endif

      } while (wait_for_heatup && TEMP_CONDITIONS);

      if (wait_for_heatup) {
        ui.reset_status();
        #if ENABLED(PRINTER_EVENT_LEDS)
          printerEventLEDs.onHeatingDone();
        #endif
      }

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        gcode.busy_state = old_busy_state;
      #endif

      return wait_for_heatup;
    }

  #endif // HAS_TEMP_HOTEND

  #if HAS_HEATED_BED

    #ifndef MIN_COOLING_SLOPE_DEG_BED
      #define MIN_COOLING_SLOPE_DEG_BED 1.50
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_BED
      #define MIN_COOLING_SLOPE_TIME_BED 60
    #endif

    bool Temperature::wait_for_bed(const bool no_wait_for_cooling/*=true*/
      #if G26_CLICK_CAN_CANCEL
        , const bool click_to_cancel/*=false*/
      #endif
    ) {
      #if TEMP_BED_RESIDENCY_TIME > 0
        millis_t residency_start_ms = 0;
        // Loop until the temperature has stabilized
        #define TEMP_BED_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + (TEMP_BED_RESIDENCY_TIME) * 1000UL))
      #else
        // Loop until the temperature is very close target
        #define TEMP_BED_CONDITIONS (wants_to_cool ? isCoolingBed() : isHeatingBed())
      #endif

      float target_temp = -1, old_temp = 9999;
      bool wants_to_cool = false;
      wait_for_heatup = true;
      millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        const GcodeSuite::MarlinBusyState old_busy_state = gcode.busy_state;
        KEEPALIVE_STATE(NOT_BUSY);
      #endif

      #if ENABLED(PRINTER_EVENT_LEDS)
        const float start_temp = degBed();
        printerEventLEDs.onBedHeatingStart();
      #endif

      do {
        // Target temperature might be changed during the loop
        if (target_temp != degTargetBed()) {
          wants_to_cool = isCoolingBed();
          target_temp = degTargetBed();

          // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
          if (no_wait_for_cooling && wants_to_cool) break;
        }

        now = millis();
        if (ELAPSED(now, next_temp_ms)) { //Print Temp Reading every 1 second while heating up.
          next_temp_ms = now + 1000UL;
          print_heater_states(active_extruder);
          #if TEMP_BED_RESIDENCY_TIME > 0
            SERIAL_ECHOPGM(" W:");
            if (residency_start_ms)
              SERIAL_ECHO(long((((TEMP_BED_RESIDENCY_TIME) * 1000UL) - (now - residency_start_ms)) / 1000UL));
            else
              SERIAL_CHAR('?');
          #endif
          SERIAL_EOL();
        }

        idle();
        gcode.reset_stepper_timeout(); // Keep steppers powered

        const float temp = degBed();

        #if ENABLED(PRINTER_EVENT_LEDS)
          // Gradually change LED strip from blue to violet as bed heats up
          if (!wants_to_cool) printerEventLEDs.onBedHeating(start_temp, temp, target_temp);
        #endif

        #if TEMP_BED_RESIDENCY_TIME > 0

          const float temp_diff = ABS(target_temp - temp);

          if (!residency_start_ms) {
            // Start the TEMP_BED_RESIDENCY_TIME timer when we reach target temp for the first time.
            if (temp_diff < TEMP_BED_WINDOW) residency_start_ms = now;
          }
          else if (temp_diff > TEMP_BED_HYSTERESIS) {
            // Restart the timer whenever the temperature falls outside the hysteresis.
            residency_start_ms = now;
          }

        #endif // TEMP_BED_RESIDENCY_TIME > 0

        // Prevent a wait-forever situation if R is misused i.e. M190 R0
        if (wants_to_cool) {
          // Break after MIN_COOLING_SLOPE_TIME_BED seconds
          // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG_BED
          if (!next_cool_check_ms || ELAPSED(now, next_cool_check_ms)) {
            if (old_temp - temp < float(MIN_COOLING_SLOPE_DEG_BED)) break;
            next_cool_check_ms = now + 1000UL * MIN_COOLING_SLOPE_TIME_BED;
            old_temp = temp;
          }
        }

        #if G26_CLICK_CAN_CANCEL
          if (click_to_cancel && ui.use_click()) {
            wait_for_heatup = false;
            ui.quick_feedback();
          }
        #endif

      } while (wait_for_heatup && TEMP_BED_CONDITIONS);

      if (wait_for_heatup) ui.reset_status();

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        gcode.busy_state = old_busy_state;
      #endif

      return wait_for_heatup;
    }

  #endif // HAS_HEATED_BED

#endif // HAS_TEMP_SENSOR
