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

/**
 * temperature.cpp - temperature control
 */

#include "temperature.h"
#include "endstops.h"

#include "../MarlinCore.h"
#include "../lcd/ultralcd.h"
#include "planner.h"
#include "../core/language.h"
#include "../HAL/shared/Delay.h"
#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extensible_ui/ui_api.h"
#endif

#if ENABLED(MAX6675_IS_MAX31865)
  #include "Adafruit_MAX31865.h"
  #ifndef MAX31865_CS_PIN
    #define MAX31865_CS_PIN     MAX6675_SS_PIN  // HW:49   SW:65    for example
  #endif
  #ifndef MAX31865_MOSI_PIN
    #define MAX31865_MOSI_PIN   MOSI_PIN        //            63
  #endif
  #ifndef MAX31865_MISO_PIN
    #define MAX31865_MISO_PIN   MAX6675_DO_PIN  //            42
  #endif
  #ifndef MAX31865_SCK_PIN
    #define MAX31865_SCK_PIN    MAX6675_SCK_PIN //            40
  #endif
  Adafruit_MAX31865 max31865 = Adafruit_MAX31865(MAX31865_CS_PIN
    #if MAX31865_CS_PIN != MAX6675_SS_PIN
      , MAX31865_MOSI_PIN           // For software SPI also set MOSI/MISO/SCK
      , MAX31865_MISO_PIN
      , MAX31865_SCK_PIN
    #endif
  );
#endif

#define MAX6675_SEPARATE_SPI (EITHER(HEATER_0_USES_MAX6675, HEATER_1_USES_MAX6675) && PINS_EXIST(MAX6675_SCK, MAX6675_DO))

#if MAX6675_SEPARATE_SPI
  #include "../libs/private_spi.h"
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  #include "stepper.h"
#endif

#if ENABLED(BABYSTEPPING) && DISABLED(INTEGRATED_BABYSTEPPING)
  #include "../feature/babystep.h"
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

#if ENABLED(JOYSTICK)
  #include "../feature/joystick.h"
#endif

#if ENABLED(SINGLENOZZLE)
  #include "tool_change.h"
#endif

#if USE_BEEPER
  #include "../libs/buzzer.h"
#endif

#if HOTEND_USES_THERMISTOR
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    static void* heater_ttbl_map[2] = { (void*)HEATER_0_TEMPTABLE, (void*)HEATER_1_TEMPTABLE };
    static constexpr uint8_t heater_ttbllen_map[2] = { HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN };
  #else
    static void* heater_ttbl_map[HOTENDS] = ARRAY_BY_HOTENDS((void*)HEATER_0_TEMPTABLE, (void*)HEATER_1_TEMPTABLE, (void*)HEATER_2_TEMPTABLE, (void*)HEATER_3_TEMPTABLE, (void*)HEATER_4_TEMPTABLE, (void*)HEATER_5_TEMPTABLE);
    static constexpr uint8_t heater_ttbllen_map[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN, HEATER_2_TEMPTABLE_LEN, HEATER_3_TEMPTABLE_LEN, HEATER_4_TEMPTABLE_LEN, HEATER_5_TEMPTABLE_LEN);
  #endif
#endif

Temperature thermalManager;

const char str_t_thermal_runaway[] PROGMEM = STR_T_THERMAL_RUNAWAY,
           str_t_heating_failed[] PROGMEM = STR_T_HEATING_FAILED;

/**
 * Macros to include the heater id in temp errors. The compiler's dead-code
 * elimination should (hopefully) optimize out the unused strings.
 */

#if HAS_HEATED_BED
  #define _BED_PSTR(h) (h) == H_BED ? GET_TEXT(MSG_BED) :
#else
  #define _BED_PSTR(h)
#endif
#if HAS_HEATED_CHAMBER
  #define _CHAMBER_PSTR(h) (h) == H_CHAMBER ? GET_TEXT(MSG_CHAMBER) :
#else
  #define _CHAMBER_PSTR(h)
#endif
#define _E_PSTR(h,N) ((HOTENDS) > N && (h) == N) ? PSTR(LCD_STR_E##N) :
#define HEATER_PSTR(h) _BED_PSTR(h) _CHAMBER_PSTR(h) _E_PSTR(h,1) _E_PSTR(h,2) _E_PSTR(h,3) _E_PSTR(h,4) _E_PSTR(h,5) PSTR(LCD_STR_E0)

// public:

#if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
  bool Temperature::adaptive_fan_slowing = true;
#endif

#if HOTENDS
  hotend_info_t Temperature::temp_hotend[HOTEND_TEMPS]; // = { 0 }
#endif

#if ENABLED(AUTO_POWER_E_FANS)
  uint8_t Temperature::autofan_speed[HOTENDS]; // = { 0 }
#endif

#if ENABLED(AUTO_POWER_CHAMBER_FAN)
  uint8_t Temperature::chamberfan_speed; // = 0
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
          new_fan_speed[fan] = _MIN(tmp_temp, 255U);
          break;
      }
    }

  #endif

  #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)
    bool Temperature::fans_paused; // = false;
    uint8_t Temperature::saved_fan_speed[FAN_COUNT]; // = { 0 }
  #endif

  #if ENABLED(ADAPTIVE_FAN_SLOWING)
    uint8_t Temperature::fan_speed_scaler[FAN_COUNT] = ARRAY_N(FAN_COUNT, 128, 128, 128, 128, 128, 128);
  #endif

  /**
   * Set the print fan speed for a target extruder
   */
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
  }

  #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)

    void Temperature::set_fans_paused(const bool p) {
      if (p != fans_paused) {
        fans_paused = p;
        if (p)
          FANS_LOOP(i) { saved_fan_speed[i] = fan_speed[i]; fan_speed[i] = 0; }
        else
          FANS_LOOP(i) fan_speed[i] = saved_fan_speed[i];
      }
    }

  #endif

#endif // FAN_COUNT > 0

#if WATCH_HOTENDS
  hotend_watch_t Temperature::watch_hotend[HOTENDS]; // = { { 0 } }
#endif
#if HEATER_IDLE_HANDLER
  hotend_idle_t Temperature::hotend_idle[HOTENDS]; // = { { 0 } }
#endif

#if HAS_HEATED_BED
  bed_info_t Temperature::temp_bed; // = { 0 }
  // Init min and max temp with extreme values to prevent false errors during startup
  #ifdef BED_MINTEMP
    int16_t Temperature::mintemp_raw_BED = HEATER_BED_RAW_LO_TEMP;
  #endif
  #ifdef BED_MAXTEMP
    int16_t Temperature::maxtemp_raw_BED = HEATER_BED_RAW_HI_TEMP;
  #endif
  #if WATCH_BED
    bed_watch_t Temperature::watch_bed; // = { 0 }
  #endif
  #if DISABLED(PIDTEMPBED)
    millis_t Temperature::next_bed_check_ms;
  #endif
  #if HEATER_IDLE_HANDLER
    hotend_idle_t Temperature::bed_idle; // = { 0 }
  #endif
#endif // HAS_HEATED_BED

#if HAS_TEMP_CHAMBER
  chamber_info_t Temperature::temp_chamber; // = { 0 }
  #if HAS_HEATED_CHAMBER
    #ifdef CHAMBER_MINTEMP
      int16_t Temperature::mintemp_raw_CHAMBER = HEATER_CHAMBER_RAW_LO_TEMP;
    #endif
    #ifdef CHAMBER_MAXTEMP
      int16_t Temperature::maxtemp_raw_CHAMBER = HEATER_CHAMBER_RAW_HI_TEMP;
    #endif
    #if WATCH_CHAMBER
      chamber_watch_t Temperature::watch_chamber{0};
    #endif
    millis_t Temperature::next_chamber_check_ms;
  #endif // HAS_HEATED_CHAMBER
#endif // HAS_TEMP_CHAMBER

#if HAS_TEMP_PROBE
  probe_info_t Temperature::temp_probe; // = { 0 }
#endif

// Initialized by settings.load()
#if ENABLED(PIDTEMP)
  //hotend_pid_t Temperature::pid[HOTENDS];
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

volatile bool Temperature::raw_temps_ready = false;

#if ENABLED(PID_EXTRUSION_SCALING)
  int32_t Temperature::last_e_position, Temperature::lpq[LPQ_MAX_LEN];
  lpq_ptr_t Temperature::lpq_ptr = 0;
#endif

#define TEMPDIR(N) ((HEATER_##N##_RAW_LO_TEMP) < (HEATER_##N##_RAW_HI_TEMP) ? 1 : -1)

#if HOTENDS
  // Init mintemp and maxtemp with extreme values to prevent false errors during startup
  constexpr temp_range_t sensor_heater_0 { HEATER_0_RAW_LO_TEMP, HEATER_0_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_1 { HEATER_1_RAW_LO_TEMP, HEATER_1_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_2 { HEATER_2_RAW_LO_TEMP, HEATER_2_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_3 { HEATER_3_RAW_LO_TEMP, HEATER_3_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_4 { HEATER_4_RAW_LO_TEMP, HEATER_4_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_5 { HEATER_5_RAW_LO_TEMP, HEATER_5_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_6 { HEATER_6_RAW_LO_TEMP, HEATER_6_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_7 { HEATER_7_RAW_LO_TEMP, HEATER_7_RAW_HI_TEMP, 0, 16383 };

  temp_range_t Temperature::temp_range[HOTENDS] = ARRAY_BY_HOTENDS(sensor_heater_0, sensor_heater_1, sensor_heater_2, sensor_heater_3, sensor_heater_4, sensor_heater_5, sensor_heater_6, sensor_heater_7);
#endif

#ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
  uint8_t Temperature::consecutive_low_temperature_error[HOTENDS] = { 0 };
#endif

#ifdef MILLISECONDS_PREHEAT_TIME
  millis_t Temperature::preheat_end_time[HOTENDS] = { 0 };
#endif

#if HAS_AUTO_FAN
  millis_t Temperature::next_auto_fan_check_ms = 0;
#endif

#if ENABLED(FAN_SOFT_PWM)
  uint8_t Temperature::soft_pwm_amount_fan[FAN_COUNT],
          Temperature::soft_pwm_count_fan[FAN_COUNT];
#endif

#if ENABLED(PROBING_HEATERS_OFF)
  bool Temperature::paused;
#endif

// public:

#if HAS_ADC_BUTTONS
  uint32_t Temperature::current_ADCKey_raw = HAL_ADC_RANGE;
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
  void Temperature::PID_autotune(const float &target, const heater_ind_t heater, const int8_t ncycles, const bool set_result/*=false*/) {
    float current_temp = 0.0;
    int cycles = 0;
    bool heating = true;

    millis_t next_temp_ms = millis(), t1 = next_temp_ms, t2 = next_temp_ms;
    long t_high = 0, t_low = 0;

    long bias, d;
    PID_t tune_pid = { 0, 0, 0 };
    float maxT = 0, minT = 10000;

    const bool isbed = (heater == H_BED);

    #if HAS_PID_FOR_BOTH
      #define GHV(B,H) (isbed ? (B) : (H))
      #define SHV(B,H) do{ if (isbed) temp_bed.soft_pwm_amount = B; else temp_hotend[heater].soft_pwm_amount = H; }while(0)
      #define ONHEATINGSTART() (isbed ? printerEventLEDs.onBedHeatingStart() : printerEventLEDs.onHotendHeatingStart())
      #define ONHEATING(S,C,T) (isbed ? printerEventLEDs.onBedHeating(S,C,T) : printerEventLEDs.onHotendHeating(S,C,T))
    #elif ENABLED(PIDTEMPBED)
      #define GHV(B,H) B
      #define SHV(B,H) (temp_bed.soft_pwm_amount = B)
      #define ONHEATINGSTART() printerEventLEDs.onBedHeatingStart()
      #define ONHEATING(S,C,T) printerEventLEDs.onBedHeating(S,C,T)
    #else
      #define GHV(B,H) H
      #define SHV(B,H) (temp_hotend[heater].soft_pwm_amount = H)
      #define ONHEATINGSTART() printerEventLEDs.onHotendHeatingStart()
      #define ONHEATING(S,C,T) printerEventLEDs.onHotendHeating(S,C,T)
    #endif

    #if WATCH_BED || WATCH_HOTENDS
      #define HAS_TP_BED BOTH(THERMAL_PROTECTION_BED, PIDTEMPBED)
      #if HAS_TP_BED && BOTH(THERMAL_PROTECTION_HOTENDS, PIDTEMP)
        #define GTV(B,H) (isbed ? (B) : (H))
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

    if (target > GHV(BED_MAXTEMP - 10, temp_range[heater].maxtemp - 15)) {
      SERIAL_ECHOLNPGM(STR_PID_TEMP_TOO_HIGH);
      #if ENABLED(EXTENSIBLE_UI)
        ExtUI::OnPidTuning(ExtUI::result_t::PID_TEMP_TOO_HIGH);
      #endif
      return;
    }

    SERIAL_ECHOLNPGM(STR_PID_AUTOTUNE_START);

    disable_all_heaters();

    SHV(bias = d = (MAX_BED_POWER) >> 1, bias = d = (PID_MAX) >> 1);

    wait_for_heatup = true; // Can be interrupted with M108
    #if ENABLED(PRINTER_EVENT_LEDS)
      const float start_temp = GHV(temp_bed.celsius, temp_hotend[heater].celsius);
      LEDColor color = ONHEATINGSTART();
    #endif

    #if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
      adaptive_fan_slowing = false;
    #endif

    // PID Tuning loop
    while (wait_for_heatup) {

      const millis_t ms = millis();

      if (raw_temps_ready) { // temp sample ready
        updateTemperaturesFromRawValues();

        // Get the current temperature and constrain it
        current_temp = GHV(temp_bed.celsius, temp_hotend[heater].celsius);
        NOLESS(maxT, current_temp);
        NOMORE(minT, current_temp);

        #if ENABLED(PRINTER_EVENT_LEDS)
          ONHEATING(start_temp, current_temp, target);
        #endif

        #if HAS_AUTO_FAN
          if (ELAPSED(ms, next_auto_fan_check_ms)) {
            checkExtruderAutoFans();
            next_auto_fan_check_ms = ms + 2500UL;
          }
        #endif

        if (heating && current_temp > target) {
          if (ELAPSED(ms, t2 + 5000UL)) {
            heating = false;
            SHV((bias - d) >> 1, (bias - d) >> 1);
            t1 = ms;
            t_high = t1 - t2;
            maxT = target;
          }
        }

        if (!heating && current_temp < target) {
          if (ELAPSED(ms, t1 + 5000UL)) {
            heating = true;
            t2 = ms;
            t_low = t2 - t1;
            if (cycles > 0) {
              const long max_pow = GHV(MAX_BED_POWER, PID_MAX);
              bias += (d * (t_high - t_low)) / (t_low + t_high);
              LIMIT(bias, 20, max_pow - 20);
              d = (bias > max_pow >> 1) ? max_pow - 1 - bias : bias;

              SERIAL_ECHOPAIR(STR_BIAS, bias, STR_D_COLON, d, STR_T_MIN, minT, STR_T_MAX, maxT);
              if (cycles > 2) {
                const float Ku = (4.0f * d) / (float(M_PI) * (maxT - minT) * 0.5f),
                            Tu = float(t_low + t_high) * 0.001f,
                            pf = isbed ? 0.2f : 0.6f,
                            df = isbed ? 1.0f / 3.0f : 1.0f / 8.0f;

                SERIAL_ECHOPAIR(STR_KU, Ku, STR_TU, Tu);
                if (isbed) { // Do not remove this otherwise PID autotune won't work right for the bed!
                  tune_pid.Kp = Ku * 0.2f;
                  tune_pid.Ki = 2 * tune_pid.Kp / Tu;
                  tune_pid.Kd = tune_pid.Kp * Tu / 3;
                  SERIAL_ECHOLNPGM("\n" " No overshoot"); // Works far better for the bed. Classic and some have bad ringing.
                  SERIAL_ECHOLNPAIR(STR_KP, tune_pid.Kp, STR_KI, tune_pid.Ki, STR_KD, tune_pid.Kd);
                }
                else {
                  tune_pid.Kp = Ku * pf;
                  tune_pid.Kd = tune_pid.Kp * Tu * df;
                  tune_pid.Ki = 2 * tune_pid.Kp / Tu;
                  SERIAL_ECHOLNPGM("\n" STR_CLASSIC_PID);
                  SERIAL_ECHOLNPAIR(STR_KP, tune_pid.Kp, STR_KI, tune_pid.Ki, STR_KD, tune_pid.Kd);
                }

                /**
                tune_pid.Kp = 0.33 * Ku;
                tune_pid.Ki = tune_pid.Kp / Tu;
                tune_pid.Kd = tune_pid.Kp * Tu / 3;
                SERIAL_ECHOLNPGM(" Some overshoot");
                SERIAL_ECHOLNPAIR(" Kp: ", tune_pid.Kp, " Ki: ", tune_pid.Ki, " Kd: ", tune_pid.Kd, " No overshoot");
                tune_pid.Kp = 0.2 * Ku;
                tune_pid.Ki = 2 * tune_pid.Kp / Tu;
                tune_pid.Kd = tune_pid.Kp * Tu / 3;
                SERIAL_ECHOPAIR(" Kp: ", tune_pid.Kp, " Ki: ", tune_pid.Ki, " Kd: ", tune_pid.Kd);
                */
              }
            }
            SHV((bias + d) >> 1, (bias + d) >> 1);
            cycles++;
            minT = target;
          }
        }
      }

      // Did the temperature overshoot very far?
      #ifndef MAX_OVERSHOOT_PID_AUTOTUNE
        #define MAX_OVERSHOOT_PID_AUTOTUNE 30
      #endif
      if (current_temp > target + MAX_OVERSHOOT_PID_AUTOTUNE) {
        SERIAL_ECHOLNPGM(STR_PID_TEMP_TOO_HIGH);
        #if ENABLED(EXTENSIBLE_UI)
          ExtUI::OnPidTuning(ExtUI::result_t::PID_TEMP_TOO_HIGH);
        #endif
        break;
      }

      // Report heater states every 2 seconds
      if (ELAPSED(ms, next_temp_ms)) {
        #if HAS_TEMP_SENSOR
          print_heater_states(isbed ? active_extruder : heater);
          SERIAL_EOL();
        #endif
        next_temp_ms = ms + 2000UL;

        // Make sure heating is actually working
        #if WATCH_BED || WATCH_HOTENDS
          if (
            #if WATCH_BED && WATCH_HOTENDS
              true
            #elif WATCH_HOTENDS
              !isbed
            #else
              isbed
            #endif
          ) {
            if (!heated) {                                          // If not yet reached target...
              if (current_temp > next_watch_temp) {                      // Over the watch temp?
                next_watch_temp = current_temp + watch_temp_increase;    // - set the next temp to watch for
                temp_change_ms = ms + watch_temp_period * 1000UL;   // - move the expiration timer up
                if (current_temp > watch_temp_target) heated = true;     // - Flag if target temperature reached
              }
              else if (ELAPSED(ms, temp_change_ms))                 // Watch timer expired
                _temp_error(heater, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
            }
            else if (current_temp < target - (MAX_OVERSHOOT_PID_AUTOTUNE)) // Heated, then temperature fell too far?
              _temp_error(heater, str_t_thermal_runaway, GET_TEXT(MSG_THERMAL_RUNAWAY));
          }
        #endif
      } // every 2 seconds

      // Timeout after MAX_CYCLE_TIME_PID_AUTOTUNE minutes since the last undershoot/overshoot cycle
      #ifndef MAX_CYCLE_TIME_PID_AUTOTUNE
        #define MAX_CYCLE_TIME_PID_AUTOTUNE 20L
      #endif
      if (((ms - t1) + (ms - t2)) > (MAX_CYCLE_TIME_PID_AUTOTUNE * 60L * 1000L)) {
        #if ENABLED(EXTENSIBLE_UI)
          ExtUI::OnPidTuning(ExtUI::result_t::PID_TUNING_TIMEOUT);
        #endif
        SERIAL_ECHOLNPGM(STR_PID_TIMEOUT);
        break;
      }

      if (cycles > ncycles && cycles > 2) {
        SERIAL_ECHOLNPGM(STR_PID_AUTOTUNE_FINISHED);

        #if HAS_PID_FOR_BOTH
          const char * const estring = GHV(PSTR("bed"), NUL_STR);
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
          temp_bed.pid.Kp = tune_pid.Kp; \
          temp_bed.pid.Ki = scalePID_i(tune_pid.Ki); \
          temp_bed.pid.Kd = scalePID_d(tune_pid.Kd); \
        }while(0)

        #define _SET_EXTRUDER_PID() do { \
          PID_PARAM(Kp, heater) = tune_pid.Kp; \
          PID_PARAM(Ki, heater) = scalePID_i(tune_pid.Ki); \
          PID_PARAM(Kd, heater) = scalePID_d(tune_pid.Kd); \
          updatePID(); }while(0)

        // Use the result? (As with "M303 U1")
        if (set_result) {
          #if HAS_PID_FOR_BOTH
            if (isbed) _SET_BED_PID(); else _SET_EXTRUDER_PID();
          #elif ENABLED(PIDTEMP)
            _SET_EXTRUDER_PID();
          #else
            _SET_BED_PID();
          #endif
        }

        #if ENABLED(PRINTER_EVENT_LEDS)
          printerEventLEDs.onPidTuningDone(color);
        #endif
        #if ENABLED(EXTENSIBLE_UI)
          ExtUI::OnPidTuning(ExtUI::result_t::PID_DONE);
        #endif

        goto EXIT_M303;
      }
      ui.update();
    }

    disable_all_heaters();

    #if ENABLED(PRINTER_EVENT_LEDS)
      printerEventLEDs.onPidTuningDone(color);
    #endif
    #if ENABLED(EXTENSIBLE_UI)
      ExtUI::OnPidTuning(ExtUI::result_t::PID_DONE);
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

int16_t Temperature::getHeaterPower(const heater_ind_t heater_id) {
  switch (heater_id) {
    #if HAS_HEATED_BED
      case H_BED: return temp_bed.soft_pwm_amount;
    #endif
    #if HAS_HEATED_CHAMBER
      case H_CHAMBER: return temp_chamber.soft_pwm_amount;
    #endif
    default:
      return (0
        #if HOTENDS
          + temp_hotend[heater_id].soft_pwm_amount
        #endif
      );
  }
}

#define _EFANOVERLAP(A,B) _FANOVERLAP(E##A,B)

#if HAS_AUTO_FAN

  #define CHAMBER_FAN_INDEX HOTENDS

  void Temperature::checkExtruderAutoFans() {
    #define _EFAN(B,A) _EFANOVERLAP(A,B) ? B :
    static const uint8_t fanBit[] PROGMEM = {
      0
      #if HOTENDS > 1
        #define _NEXT_FAN(N) , REPEAT2(N,_EFAN,N) N
        RREPEAT_S(1, HOTENDS, _NEXT_FAN)
      #endif
      #if HAS_AUTO_CHAMBER_FAN
        #define _CFAN(B) _FANOVERLAP(CHAMBER,B) ? B :
        , REPEAT(HOTENDS,_CFAN) (HOTENDS)
      #endif
    };

    uint8_t fanState = 0;
    HOTEND_LOOP()
      if (temp_hotend[e].celsius >= EXTRUDER_AUTO_FAN_TEMPERATURE)
        SBI(fanState, pgm_read_byte(&fanBit[e]));

    #if HAS_AUTO_CHAMBER_FAN
      if (temp_chamber.celsius >= CHAMBER_AUTO_FAN_TEMPERATURE)
        SBI(fanState, pgm_read_byte(&fanBit[CHAMBER_FAN_INDEX]));
    #endif

    #define _UPDATE_AUTO_FAN(P,D,A) do{                  \
      if (PWM_PIN(P##_AUTO_FAN_PIN) && A < 255)          \
        analogWrite(pin_t(P##_AUTO_FAN_PIN), D ? A : 0); \
      else                                               \
        WRITE(P##_AUTO_FAN_PIN, D);                      \
    }while(0)

    uint8_t fanDone = 0;
    for (uint8_t f = 0; f < COUNT(fanBit); f++) {
      const uint8_t realFan = pgm_read_byte(&fanBit[f]);
      if (TEST(fanDone, realFan)) continue;
      const bool fan_on = TEST(fanState, realFan);
      switch (f) {
        #if ENABLED(AUTO_POWER_CHAMBER_FAN)
          case CHAMBER_FAN_INDEX:
            chamberfan_speed = fan_on ? CHAMBER_AUTO_FAN_SPEED : 0;
            break;
        #endif
        default:
          #if ENABLED(AUTO_POWER_E_FANS)
            autofan_speed[realFan] = fan_on ? EXTRUDER_AUTO_FAN_SPEED : 0;
          #endif
          break;
      }

      switch (f) {
        #if HAS_AUTO_FAN_0
          case 0: _UPDATE_AUTO_FAN(E0, fan_on, EXTRUDER_AUTO_FAN_SPEED); break;
        #endif
        #if HAS_AUTO_FAN_1
          case 1: _UPDATE_AUTO_FAN(E1, fan_on, EXTRUDER_AUTO_FAN_SPEED); break;
        #endif
        #if HAS_AUTO_FAN_2
          case 2: _UPDATE_AUTO_FAN(E2, fan_on, EXTRUDER_AUTO_FAN_SPEED); break;
        #endif
        #if HAS_AUTO_FAN_3
          case 3: _UPDATE_AUTO_FAN(E3, fan_on, EXTRUDER_AUTO_FAN_SPEED); break;
        #endif
        #if HAS_AUTO_FAN_4
          case 4: _UPDATE_AUTO_FAN(E4, fan_on, EXTRUDER_AUTO_FAN_SPEED); break;
        #endif
        #if HAS_AUTO_FAN_5
          case 5: _UPDATE_AUTO_FAN(E5, fan_on, EXTRUDER_AUTO_FAN_SPEED); break;
        #endif
        #if HAS_AUTO_FAN_6
          case 6: _UPDATE_AUTO_FAN(E6, fan_on, EXTRUDER_AUTO_FAN_SPEED); break;
        #endif
        #if HAS_AUTO_FAN_7
          case 7: _UPDATE_AUTO_FAN(E7, fan_on, EXTRUDER_AUTO_FAN_SPEED); break;
        #endif
        #if HAS_AUTO_CHAMBER_FAN && !AUTO_CHAMBER_IS_E
          case CHAMBER_FAN_INDEX: _UPDATE_AUTO_FAN(CHAMBER, fan_on, CHAMBER_AUTO_FAN_SPEED); break;
        #endif
      }
      SBI(fanDone, realFan);
    }
  }

#endif // HAS_AUTO_FAN

//
// Temperature Error Handlers
//

inline void loud_kill(PGM_P const lcd_msg, const heater_ind_t heater) {
  Running = false;
  #if USE_BEEPER
    for (uint8_t i = 20; i--;) {
      WRITE(BEEPER_PIN, HIGH); delay(25);
      WRITE(BEEPER_PIN, LOW); delay(80);
    }
    WRITE(BEEPER_PIN, HIGH);
  #endif
  kill(lcd_msg, HEATER_PSTR(heater));
}

void Temperature::_temp_error(const heater_ind_t heater, PGM_P const serial_msg, PGM_P const lcd_msg) {

  static uint8_t killed = 0;

  if (IsRunning()
    #if BOGUS_TEMPERATURE_GRACE_PERIOD
      && killed == 2
    #endif
  ) {
    SERIAL_ERROR_START();
    serialprintPGM(serial_msg);
    SERIAL_ECHOPGM(STR_STOPPED_HEATER);
    if (heater >= 0) SERIAL_ECHO((int)heater);
    #if HAS_HEATED_CHAMBER
      else if (heater == H_CHAMBER) SERIAL_ECHOPGM(STR_HEATER_CHAMBER);
    #endif
    else SERIAL_ECHOPGM(STR_HEATER_BED);
    SERIAL_EOL();
  }

  disable_all_heaters(); // always disable (even for bogus temp)

  #if BOGUS_TEMPERATURE_GRACE_PERIOD
    const millis_t ms = millis();
    static millis_t expire_ms;
    switch (killed) {
      case 0:
        expire_ms = ms + BOGUS_TEMPERATURE_GRACE_PERIOD;
        ++killed;
        break;
      case 1:
        if (ELAPSED(ms, expire_ms)) ++killed;
        break;
      case 2:
        loud_kill(lcd_msg, heater);
        ++killed;
        break;
    }
  #elif defined(BOGUS_TEMPERATURE_GRACE_PERIOD)
    UNUSED(killed);
  #else
    if (!killed) { killed = 1; loud_kill(lcd_msg, heater); }
  #endif
}

void Temperature::max_temp_error(const heater_ind_t heater) {
  _temp_error(heater, PSTR(STR_T_MAXTEMP), GET_TEXT(MSG_ERR_MAXTEMP));
}

void Temperature::min_temp_error(const heater_ind_t heater) {
  _temp_error(heater, PSTR(STR_T_MINTEMP), GET_TEXT(MSG_ERR_MINTEMP));
}

#if HOTENDS

  float Temperature::get_pid_output_hotend(const uint8_t E_NAME) {
    const uint8_t ee = HOTEND_INDEX;
    #if ENABLED(PIDTEMP)
      #if DISABLED(PID_OPENLOOP)
        static hotend_pid_t work_pid[HOTENDS];
        static float temp_iState[HOTENDS] = { 0 },
                     temp_dState[HOTENDS] = { 0 };
        static bool pid_reset[HOTENDS] = { false };
        const float pid_error = temp_hotend[ee].target - temp_hotend[ee].celsius;

        float pid_output;

        if (temp_hotend[ee].target == 0
          || pid_error < -(PID_FUNCTIONAL_RANGE)
          #if HEATER_IDLE_HANDLER
            || hotend_idle[ee].timed_out
          #endif
        ) {
          pid_output = 0;
          pid_reset[ee] = true;
        }
        else if (pid_error > PID_FUNCTIONAL_RANGE) {
          pid_output = BANG_MAX;
          pid_reset[ee] = true;
        }
        else {
          if (pid_reset[ee]) {
            temp_iState[ee] = 0.0;
            work_pid[ee].Kd = 0.0;
            pid_reset[ee] = false;
          }

          work_pid[ee].Kd = work_pid[ee].Kd + PID_K2 * (PID_PARAM(Kd, ee) * (temp_dState[ee] - temp_hotend[ee].celsius) - work_pid[ee].Kd);
          const float max_power_over_i_gain = float(PID_MAX) / PID_PARAM(Ki, ee) - float(MIN_POWER);
          temp_iState[ee] = constrain(temp_iState[ee] + pid_error, 0, max_power_over_i_gain);
          work_pid[ee].Kp = PID_PARAM(Kp, ee) * pid_error;
          work_pid[ee].Ki = PID_PARAM(Ki, ee) * temp_iState[ee];

          pid_output = work_pid[ee].Kp + work_pid[ee].Ki + work_pid[ee].Kd + float(MIN_POWER);

          #if ENABLED(PID_EXTRUSION_SCALING)
            #if HOTENDS == 1
              constexpr bool this_hotend = true;
            #else
              const bool this_hotend = (ee == active_extruder);
            #endif
            work_pid[ee].Kc = 0;
            if (this_hotend) {
              const long e_position = stepper.position(E_AXIS);
              if (e_position > last_e_position) {
                lpq[lpq_ptr] = e_position - last_e_position;
                last_e_position = e_position;
              }
              else
                lpq[lpq_ptr] = 0;

              if (++lpq_ptr >= lpq_len) lpq_ptr = 0;
              work_pid[ee].Kc = (lpq[lpq_ptr] * planner.steps_to_mm[E_AXIS]) * PID_PARAM(Kc, ee);
              pid_output += work_pid[ee].Kc;
            }
          #endif // PID_EXTRUSION_SCALING
          #if ENABLED(PID_FAN_SCALING)
            if (thermalManager.fan_speed[active_extruder] > PID_FAN_SCALING_MIN_SPEED) {
              work_pid[ee].Kf = PID_PARAM(Kf, ee) + (PID_FAN_SCALING_LIN_FACTOR) * thermalManager.fan_speed[active_extruder];
              pid_output += work_pid[ee].Kf;
            }
            //pid_output -= work_pid[ee].Ki;
            //pid_output += work_pid[ee].Ki * work_pid[ee].Kf
          #endif // PID_FAN_SCALING
          LIMIT(pid_output, 0, PID_MAX);
        }
        temp_dState[ee] = temp_hotend[ee].celsius;

      #else // PID_OPENLOOP

        const float pid_output = constrain(temp_hotend[ee].target, 0, PID_MAX);

      #endif // PID_OPENLOOP

      #if ENABLED(PID_DEBUG)
        if (ee == active_extruder) {
          SERIAL_ECHO_START();
          SERIAL_ECHOPAIR(
            STR_PID_DEBUG, ee,
            STR_PID_DEBUG_INPUT, temp_hotend[ee].celsius,
            STR_PID_DEBUG_OUTPUT, pid_output
          );
          #if DISABLED(PID_OPENLOOP)
          {
            SERIAL_ECHOPAIR(
              STR_PID_DEBUG_PTERM, work_pid[ee].Kp,
              STR_PID_DEBUG_ITERM, work_pid[ee].Ki,
              STR_PID_DEBUG_DTERM, work_pid[ee].Kd
              #if ENABLED(PID_EXTRUSION_SCALING)
                , STR_PID_DEBUG_CTERM, work_pid[ee].Kc
              #endif
            );
          }
          #endif
          SERIAL_EOL();
        }
      #endif // PID_DEBUG

    #else // No PID enabled

      #if HEATER_IDLE_HANDLER
        const bool is_idling = hotend_idle[ee].timed_out;
      #else
        constexpr bool is_idling = false;
      #endif
      const float pid_output = (!is_idling && temp_hotend[ee].celsius < temp_hotend[ee].target) ? BANG_MAX : 0;

    #endif

    return pid_output;
  }

#endif // HOTENDS

#if ENABLED(PIDTEMPBED)

  float Temperature::get_pid_output_bed() {

    #if DISABLED(PID_OPENLOOP)

      static PID_t work_pid{0};
      static float temp_iState = 0, temp_dState = 0;
      static bool pid_reset = true;
      float pid_output = 0;
      const float max_power_over_i_gain = float(MAX_BED_POWER) / temp_bed.pid.Ki - float(MIN_BED_POWER),
                  pid_error = temp_bed.target - temp_bed.celsius;

      if (!temp_bed.target || pid_error < -(PID_FUNCTIONAL_RANGE)) {
        pid_output = 0;
        pid_reset = true;
      }
      else if (pid_error > PID_FUNCTIONAL_RANGE) {
        pid_output = MAX_BED_POWER;
        pid_reset = true;
      }
      else {
        if (pid_reset) {
          temp_iState = 0.0;
          work_pid.Kd = 0.0;
          pid_reset = false;
        }

        temp_iState = constrain(temp_iState + pid_error, 0, max_power_over_i_gain);

        work_pid.Kp = temp_bed.pid.Kp * pid_error;
        work_pid.Ki = temp_bed.pid.Ki * temp_iState;
        work_pid.Kd = work_pid.Kd + PID_K2 * (temp_bed.pid.Kd * (temp_dState - temp_bed.celsius) - work_pid.Kd);

        temp_dState = temp_bed.celsius;

        pid_output = constrain(work_pid.Kp + work_pid.Ki + work_pid.Kd + float(MIN_BED_POWER), 0, MAX_BED_POWER);
      }

    #else // PID_OPENLOOP

      const float pid_output = constrain(temp_bed.target, 0, MAX_BED_POWER);

    #endif // PID_OPENLOOP

    #if ENABLED(PID_BED_DEBUG)
    {
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPAIR(
        " PID_BED_DEBUG : Input ", temp_bed.celsius, " Output ", pid_output,
        #if DISABLED(PID_OPENLOOP)
          STR_PID_DEBUG_PTERM, work_pid.Kp,
          STR_PID_DEBUG_ITERM, work_pid.Ki,
          STR_PID_DEBUG_DTERM, work_pid.Kd,
        #endif
      );
    }
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
    if (!inited) return watchdog_refresh();
  #endif

  #if BOTH(PROBING_HEATERS_OFF, BED_LIMIT_SWITCHING)
    static bool last_pause_state;
  #endif

  #if ENABLED(EMERGENCY_PARSER)
    if (emergency_parser.killed_by_M112) kill(M112_KILL_STR, nullptr, true);
  #endif

  if (!raw_temps_ready) return;

  updateTemperaturesFromRawValues(); // also resets the watchdog

  #if ENABLED(HEATER_0_USES_MAX6675)
    if (temp_hotend[0].celsius > _MIN(HEATER_0_MAXTEMP, HEATER_0_MAX6675_TMAX - 1.0)) max_temp_error(H_E0);
    if (temp_hotend[0].celsius < _MAX(HEATER_0_MINTEMP, HEATER_0_MAX6675_TMIN + .01)) min_temp_error(H_E0);
  #endif

  #if ENABLED(HEATER_1_USES_MAX6675)
    if (temp_hotend[1].celsius > _MIN(HEATER_1_MAXTEMP, HEATER_1_MAX6675_TMAX - 1.0)) max_temp_error(H_E1);
    if (temp_hotend[1].celsius < _MAX(HEATER_1_MINTEMP, HEATER_1_MAX6675_TMIN + .01)) min_temp_error(H_E1);
  #endif

  millis_t ms = millis();

  #if HOTENDS

    HOTEND_LOOP() {
      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        if (degHotend(e) > temp_range[e].maxtemp)
          _temp_error((heater_ind_t)e, str_t_thermal_runaway, GET_TEXT(MSG_THERMAL_RUNAWAY));
      #endif

      #if HEATER_IDLE_HANDLER
        hotend_idle[e].update(ms);
      #endif

      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        // Check for thermal runaway
        thermal_runaway_protection(tr_state_machine[e], temp_hotend[e].celsius, temp_hotend[e].target, (heater_ind_t)e, THERMAL_PROTECTION_PERIOD, THERMAL_PROTECTION_HYSTERESIS);
      #endif

      temp_hotend[e].soft_pwm_amount = (temp_hotend[e].celsius > temp_range[e].mintemp || is_preheating(e)) && temp_hotend[e].celsius < temp_range[e].maxtemp ? (int)get_pid_output_hotend(e) >> 1 : 0;

      #if WATCH_HOTENDS
        // Make sure temperature is increasing
        if (watch_hotend[e].next_ms && ELAPSED(ms, watch_hotend[e].next_ms)) {  // Time to check this extruder?
          if (degHotend(e) < watch_hotend[e].target)                            // Failed to increase enough?
            _temp_error((heater_ind_t)e, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
          else                                                                  // Start again if the target is still far off
            start_watching_hotend(e);
        }
      #endif

      #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
        // Make sure measured temperatures are close together
        if (ABS(temp_hotend[0].celsius - redundant_temperature) > MAX_REDUNDANT_TEMP_SENSOR_DIFF)
          _temp_error(H_E0, PSTR(STR_REDUNDANCY), GET_TEXT(MSG_ERR_REDUNDANT_TEMP));
      #endif

    } // HOTEND_LOOP

  #endif // HOTENDS

  #if HAS_AUTO_FAN
    if (ELAPSED(ms, next_auto_fan_check_ms)) { // only need to check fan state very infrequently
      checkExtruderAutoFans();
      next_auto_fan_check_ms = ms + 2500UL;
    }
  #endif

  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    /**
     * Dynamically set the volumetric multiplier based
     * on the delayed Filament Width measurement.
     */
    filwidth.update_volumetric();
  #endif

  #if HAS_HEATED_BED

    #if ENABLED(THERMAL_PROTECTION_BED)
      if (degBed() > BED_MAXTEMP)
        _temp_error(H_BED, str_t_thermal_runaway, GET_TEXT(MSG_THERMAL_RUNAWAY));
    #endif

    #if WATCH_BED
      // Make sure temperature is increasing
      if (watch_bed.elapsed(ms)) {        // Time to check the bed?
        if (degBed() < watch_bed.target)                                // Failed to increase enough?
          _temp_error(H_BED, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
        else                                                            // Start again if the target is still far off
          start_watching_bed();
      }
    #endif // WATCH_BED

    do {

      #if DISABLED(PIDTEMPBED)
        if (PENDING(ms, next_bed_check_ms)
          #if BOTH(PROBING_HEATERS_OFF, BED_LIMIT_SWITCHING)
            && paused == last_pause_state
          #endif
        ) break;
        next_bed_check_ms = ms + BED_CHECK_INTERVAL;
        #if BOTH(PROBING_HEATERS_OFF, BED_LIMIT_SWITCHING)
          last_pause_state = paused;
        #endif
      #endif

      #if HEATER_IDLE_HANDLER
        bed_idle.update(ms);
      #endif

      #if HAS_THERMALLY_PROTECTED_BED
        thermal_runaway_protection(tr_state_machine_bed, temp_bed.celsius, temp_bed.target, H_BED, THERMAL_PROTECTION_BED_PERIOD, THERMAL_PROTECTION_BED_HYSTERESIS);
      #endif

      #if HEATER_IDLE_HANDLER
        if (bed_idle.timed_out) {
          temp_bed.soft_pwm_amount = 0;
          #if DISABLED(PIDTEMPBED)
            WRITE_HEATER_BED(LOW);
          #endif
        }
        else
      #endif
      {
        #if ENABLED(PIDTEMPBED)
          temp_bed.soft_pwm_amount = WITHIN(temp_bed.celsius, BED_MINTEMP, BED_MAXTEMP) ? (int)get_pid_output_bed() >> 1 : 0;
        #else
          // Check if temperature is within the correct band
          if (WITHIN(temp_bed.celsius, BED_MINTEMP, BED_MAXTEMP)) {
            #if ENABLED(BED_LIMIT_SWITCHING)
              if (temp_bed.celsius >= temp_bed.target + BED_HYSTERESIS)
                temp_bed.soft_pwm_amount = 0;
              else if (temp_bed.celsius <= temp_bed.target - (BED_HYSTERESIS))
                temp_bed.soft_pwm_amount = MAX_BED_POWER >> 1;
            #else // !PIDTEMPBED && !BED_LIMIT_SWITCHING
              temp_bed.soft_pwm_amount = temp_bed.celsius < temp_bed.target ? MAX_BED_POWER >> 1 : 0;
            #endif
          }
          else {
            temp_bed.soft_pwm_amount = 0;
            WRITE_HEATER_BED(LOW);
          }
        #endif
      }

    } while (false);

  #endif // HAS_HEATED_BED

  #if HAS_HEATED_CHAMBER

    #ifndef CHAMBER_CHECK_INTERVAL
      #define CHAMBER_CHECK_INTERVAL 1000UL
    #endif

    #if ENABLED(THERMAL_PROTECTION_CHAMBER)
      if (degChamber() > CHAMBER_MAXTEMP)
        _temp_error(H_CHAMBER, str_t_thermal_runaway, GET_TEXT(MSG_THERMAL_RUNAWAY));
    #endif

    #if WATCH_CHAMBER
      // Make sure temperature is increasing
      if (watch_chamber.elapsed(ms)) {              // Time to check the chamber?
        if (degChamber() < watch_chamber.target)    // Failed to increase enough?
          _temp_error(H_CHAMBER, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
        else
          start_watching_chamber();                 // Start again if the target is still far off
      }
    #endif

    if (ELAPSED(ms, next_chamber_check_ms)) {
      next_chamber_check_ms = ms + CHAMBER_CHECK_INTERVAL;

      if (WITHIN(temp_chamber.celsius, CHAMBER_MINTEMP, CHAMBER_MAXTEMP)) {
        #if ENABLED(CHAMBER_LIMIT_SWITCHING)
          if (temp_chamber.celsius >= temp_chamber.target + TEMP_CHAMBER_HYSTERESIS)
            temp_chamber.soft_pwm_amount = 0;
          else if (temp_chamber.celsius <= temp_chamber.target - (TEMP_CHAMBER_HYSTERESIS))
            temp_chamber.soft_pwm_amount = MAX_CHAMBER_POWER >> 1;
        #else
          temp_chamber.soft_pwm_amount = temp_chamber.celsius < temp_chamber.target ? MAX_CHAMBER_POWER >> 1 : 0;
        #endif
      }
      else {
        temp_chamber.soft_pwm_amount = 0;
        WRITE_HEATER_CHAMBER(LOW);
      }

      #if ENABLED(THERMAL_PROTECTION_CHAMBER)
        thermal_runaway_protection(tr_state_machine_chamber, temp_chamber.celsius, temp_chamber.target, H_CHAMBER, THERMAL_PROTECTION_CHAMBER_PERIOD, THERMAL_PROTECTION_CHAMBER_HYSTERESIS);
      #endif
    }

    // TODO: Implement true PID pwm
    //temp_bed.soft_pwm_amount = WITHIN(temp_chamber.celsius, CHAMBER_MINTEMP, CHAMBER_MAXTEMP) ? (int)get_pid_output_chamber() >> 1 : 0;

  #endif // HAS_HEATED_CHAMBER

  UNUSED(ms);
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
    if (!m) return short(pgm_read_word(&TBL[0][1]));                   \
    if (m == l || m == r) return short(pgm_read_word(&TBL[LEN-1][1])); \
    short v00 = pgm_read_word(&TBL[m-1][0]),                           \
          v10 = pgm_read_word(&TBL[m-0][0]);                           \
         if (raw < v00) r = m;                                         \
    else if (raw > v10) l = m;                                         \
    else {                                                             \
      const short v01 = short(pgm_read_word(&TBL[m-1][1])),            \
                  v11 = short(pgm_read_word(&TBL[m-0][1]));            \
      return v01 + (raw - v00) * float(v11 - v01) / float(v10 - v00);  \
    }                                                                  \
  }                                                                    \
}while(0)

#if HAS_USER_THERMISTORS

  user_thermistor_t Temperature::user_thermistor[USER_THERMISTORS]; // Initialized by settings.load()

  void Temperature::reset_user_thermistors() {
    user_thermistor_t user_thermistor[USER_THERMISTORS] = {
      #if ENABLED(HEATER_0_USER_THERMISTOR)
        { true, 0, 0, HOTEND0_PULLUP_RESISTOR_OHMS, HOTEND0_RESISTANCE_25C_OHMS, 0, 0, HOTEND0_BETA, 0 },
      #endif
      #if ENABLED(HEATER_1_USER_THERMISTOR)
        { true, 0, 0, HOTEND1_PULLUP_RESISTOR_OHMS, HOTEND1_RESISTANCE_25C_OHMS, 0, 0, HOTEND1_BETA, 0 },
      #endif
      #if ENABLED(HEATER_2_USER_THERMISTOR)
        { true, 0, 0, HOTEND2_PULLUP_RESISTOR_OHMS, HOTEND2_RESISTANCE_25C_OHMS, 0, 0, HOTEND2_BETA, 0 },
      #endif
      #if ENABLED(HEATER_3_USER_THERMISTOR)
        { true, 0, 0, HOTEND3_PULLUP_RESISTOR_OHMS, HOTEND3_RESISTANCE_25C_OHMS, 0, 0, HOTEND3_BETA, 0 },
      #endif
      #if ENABLED(HEATER_4_USER_THERMISTOR)
        { true, 0, 0, HOTEND4_PULLUP_RESISTOR_OHMS, HOTEND4_RESISTANCE_25C_OHMS, 0, 0, HOTEND4_BETA, 0 },
      #endif
      #if ENABLED(HEATER_5_USER_THERMISTOR)
        { true, 0, 0, HOTEND5_PULLUP_RESISTOR_OHMS, HOTEND5_RESISTANCE_25C_OHMS, 0, 0, HOTEND5_BETA, 0 },
      #endif
      #if ENABLED(HEATER_6_USER_THERMISTOR)
        { true, 0, 0, HOTEND6_PULLUP_RESISTOR_OHMS, HOTEND6_RESISTANCE_25C_OHMS, 0, 0, HOTEND6_BETA, 0 },
      #endif
      #if ENABLED(HEATER_7_USER_THERMISTOR)
        { true, 0, 0, HOTEND7_PULLUP_RESISTOR_OHMS, HOTEND7_RESISTANCE_25C_OHMS, 0, 0, HOTEND7_BETA, 0 },
      #endif
      #if ENABLED(HEATER_BED_USER_THERMISTOR)
        { true, 0, 0, BED_PULLUP_RESISTOR_OHMS, BED_RESISTANCE_25C_OHMS, 0, 0, BED_BETA, 0 },
      #endif
      #if ENABLED(HEATER_CHAMBER_USER_THERMISTOR)
        { true, 0, 0, CHAMBER_PULLUP_RESISTOR_OHMS, CHAMBER_RESISTANCE_25C_OHMS, 0, 0, CHAMBER_BETA, 0 }
      #endif
    };
    COPY(thermalManager.user_thermistor, user_thermistor);
  }

  void Temperature::log_user_thermistor(const uint8_t t_index, const bool eprom/*=false*/) {

    if (eprom)
      SERIAL_ECHOPGM("  M305 ");
    else
      SERIAL_ECHO_START();
    SERIAL_CHAR('P');
    SERIAL_CHAR('0' + t_index);

    const user_thermistor_t &t = user_thermistor[t_index];

    SERIAL_ECHOPAIR_F(" R", t.series_res, 1);
    SERIAL_ECHOPAIR_F_P(SP_T_STR, t.res_25, 1);
    SERIAL_ECHOPAIR_F(" B", t.beta, 1);
    SERIAL_ECHOPAIR_F(" C", t.sh_c_coeff, 9);
    SERIAL_ECHOPGM(" ; ");
    serialprintPGM(
      #if ENABLED(HEATER_0_USER_THERMISTOR)
        t_index == CTI_HOTEND_0 ? PSTR("HOTEND 0") :
      #endif
      #if ENABLED(HEATER_1_USER_THERMISTOR)
        t_index == CTI_HOTEND_1 ? PSTR("HOTEND 1") :
      #endif
      #if ENABLED(HEATER_2_USER_THERMISTOR)
        t_index == CTI_HOTEND_2 ? PSTR("HOTEND 2") :
      #endif
      #if ENABLED(HEATER_3_USER_THERMISTOR)
        t_index == CTI_HOTEND_3 ? PSTR("HOTEND 3") :
      #endif
      #if ENABLED(HEATER_4_USER_THERMISTOR)
        t_index == CTI_HOTEND_4 ? PSTR("HOTEND 4") :
      #endif
      #if ENABLED(HEATER_5_USER_THERMISTOR)
        t_index == CTI_HOTEND_5 ? PSTR("HOTEND 5") :
      #endif
      #if ENABLED(HEATER_6_USER_THERMISTOR)
        t_index == CTI_HOTEND_6 ? PSTR("HOTEND 6") :
      #endif
      #if ENABLED(HEATER_7_USER_THERMISTOR)
        t_index == CTI_HOTEND_7 ? PSTR("HOTEND 7") :
      #endif
      #if ENABLED(HEATER_BED_USER_THERMISTOR)
        t_index == CTI_BED ? PSTR("BED") :
      #endif
      #if ENABLED(HEATER_CHAMBER_USER_THERMISTOR)
        t_index == CTI_CHAMBER ? PSTR("CHAMBER") :
      #endif
      nullptr
    );
    SERIAL_EOL();
  }

  float Temperature::user_thermistor_to_deg_c(const uint8_t t_index, const int raw) {
    //#if (MOTHERBOARD == BOARD_RAMPS_14_EFB)
    //  static uint32_t clocks_total = 0;
    //  static uint32_t calls = 0;
    //  uint32_t tcnt5 = TCNT5;
    //#endif

    if (!WITHIN(t_index, 0, COUNT(user_thermistor) - 1)) return 25;

    user_thermistor_t &t = user_thermistor[t_index];
    if (t.pre_calc) { // pre-calculate some variables
      t.pre_calc     = false;
      t.res_25_recip = 1.0f / t.res_25;
      t.res_25_log   = logf(t.res_25);
      t.beta_recip   = 1.0f / t.beta;
      t.sh_alpha     = RECIPROCAL(THERMISTOR_RESISTANCE_NOMINAL_C - (THERMISTOR_ABS_ZERO_C))
                        - (t.beta_recip * t.res_25_log) - (t.sh_c_coeff * cu(t.res_25_log));
    }

    // maximum adc value .. take into account the over sampling
    const int adc_max = MAX_RAW_THERMISTOR_VALUE,
              adc_raw = constrain(raw, 1, adc_max - 1); // constrain to prevent divide-by-zero

    const float adc_inverse = (adc_max - adc_raw) - 0.5f,
                resistance = t.series_res * (adc_raw + 0.5f) / adc_inverse,
                log_resistance = logf(resistance);

    float value = t.sh_alpha;
    value += log_resistance * t.beta_recip;
    if (t.sh_c_coeff != 0)
      value += t.sh_c_coeff * cu(log_resistance);
    value = 1.0f / value;

    //#if (MOTHERBOARD == BOARD_RAMPS_14_EFB)
    //  int32_t clocks = TCNT5 - tcnt5;
    //  if (clocks >= 0) {
    //    clocks_total += clocks;
    //    calls++;
    //  }
    //#endif

    // Return degrees C (up to 999, as the LCD only displays 3 digits)
    return _MIN(value + THERMISTOR_ABS_ZERO_C, 999);
  }
#endif

#if HOTENDS
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
        SERIAL_ECHOLNPGM(STR_INVALID_EXTRUDER_NUM);
        kill();
        return 0;
      }

    switch (e) {
      case 0:
        #if ENABLED(HEATER_0_USER_THERMISTOR)
          return user_thermistor_to_deg_c(CTI_HOTEND_0, raw);
        #elif ENABLED(HEATER_0_USES_MAX6675)
          return (
            #if ENABLED(MAX6675_IS_MAX31865)
              max31865.temperature(100, 400)  // 100 ohms = PT100 resistance. 400 ohms = calibration resistor
            #else
              raw * 0.25
            #endif
          );
        #elif ENABLED(HEATER_0_USES_AD595)
          return TEMP_AD595(raw);
        #elif ENABLED(HEATER_0_USES_AD8495)
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 1:
        #if ENABLED(HEATER_1_USER_THERMISTOR)
          return user_thermistor_to_deg_c(CTI_HOTEND_1, raw);
        #elif ENABLED(HEATER_1_USES_MAX6675)
          return raw * 0.25;
        #elif ENABLED(HEATER_1_USES_AD595)
          return TEMP_AD595(raw);
        #elif ENABLED(HEATER_1_USES_AD8495)
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 2:
        #if ENABLED(HEATER_2_USER_THERMISTOR)
          return user_thermistor_to_deg_c(CTI_HOTEND_2, raw);
        #elif ENABLED(HEATER_2_USES_AD595)
          return TEMP_AD595(raw);
        #elif ENABLED(HEATER_2_USES_AD8495)
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 3:
        #if ENABLED(HEATER_3_USER_THERMISTOR)
          return user_thermistor_to_deg_c(CTI_HOTEND_3, raw);
        #elif ENABLED(HEATER_3_USES_AD595)
          return TEMP_AD595(raw);
        #elif ENABLED(HEATER_3_USES_AD8495)
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 4:
        #if ENABLED(HEATER_4_USER_THERMISTOR)
          return user_thermistor_to_deg_c(CTI_HOTEND_4, raw);
        #elif ENABLED(HEATER_4_USES_AD595)
          return TEMP_AD595(raw);
        #elif ENABLED(HEATER_4_USES_AD8495)
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 5:
        #if ENABLED(HEATER_5_USER_THERMISTOR)
          return user_thermistor_to_deg_c(CTI_HOTEND_5, raw);
        #elif ENABLED(HEATER_5_USES_AD595)
          return TEMP_AD595(raw);
        #elif ENABLED(HEATER_5_USES_AD8495)
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 6:
        #if ENABLED(HEATER_6_USER_THERMISTOR)
          return user_thermistor_to_deg_c(CTI_HOTEND_6, raw);
        #elif ENABLED(HEATER_6_USES_AD595)
          return TEMP_AD595(raw);
        #elif ENABLED(HEATER_6_USES_AD8495)
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 7:
        #if ENABLED(HEATER_7_USER_THERMISTOR)
          return user_thermistor_to_deg_c(CTI_HOTEND_7, raw);
        #elif ENABLED(HEATER_7_USES_AD595)
          return TEMP_AD595(raw);
        #elif ENABLED(HEATER_7_USES_AD8495)
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
#endif // HOTENDS

#if HAS_HEATED_BED
  // Derived from RepRap FiveD extruder::getTemperature()
  // For bed temperature measurement.
  float Temperature::analog_to_celsius_bed(const int raw) {
    #if ENABLED(HEATER_BED_USER_THERMISTOR)
      return user_thermistor_to_deg_c(CTI_BED, raw);
    #elif ENABLED(HEATER_BED_USES_THERMISTOR)
      SCAN_THERMISTOR_TABLE(BED_TEMPTABLE, BED_TEMPTABLE_LEN);
    #elif ENABLED(HEATER_BED_USES_AD595)
      return TEMP_AD595(raw);
    #elif ENABLED(HEATER_BED_USES_AD8495)
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_HEATED_BED

#if HAS_TEMP_CHAMBER
  // Derived from RepRap FiveD extruder::getTemperature()
  // For chamber temperature measurement.
  float Temperature::analog_to_celsius_chamber(const int raw) {
    #if ENABLED(HEATER_CHAMBER_USER_THERMISTOR)
      return user_thermistor_to_deg_c(CTI_CHAMBER, raw);
    #elif ENABLED(HEATER_CHAMBER_USES_THERMISTOR)
      SCAN_THERMISTOR_TABLE(CHAMBER_TEMPTABLE, CHAMBER_TEMPTABLE_LEN);
    #elif ENABLED(HEATER_CHAMBER_USES_AD595)
      return TEMP_AD595(raw);
    #elif ENABLED(HEATER_CHAMBER_USES_AD8495)
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_TEMP_CHAMBER

#if HAS_TEMP_PROBE
  // Derived from RepRap FiveD extruder::getTemperature()
  // For probe temperature measurement.
  float Temperature::analog_to_celsius_probe(const int raw) {
    #if ENABLED(PROBE_USER_THERMISTOR)
      return user_thermistor_to_deg_c(CTI_PROBE, raw);
    #elif ENABLED(PROBE_USES_THERMISTOR)
      SCAN_THERMISTOR_TABLE(PROBE_TEMPTABLE, PROBE_TEMPTABLE_LEN);
    #elif ENABLED(PROBE_USES_AD595)
      return TEMP_AD595(raw);
    #elif ENABLED(PROBE_USES_AD8495)
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_TEMP_PROBE

/**
 * Get the raw values into the actual temperatures.
 * The raw values are created in interrupt context,
 * and this function is called from normal context
 * as it would block the stepper routine.
 */
void Temperature::updateTemperaturesFromRawValues() {
  #if ENABLED(HEATER_0_USES_MAX6675)
    temp_hotend[0].raw = READ_MAX6675(0);
  #endif
  #if ENABLED(HEATER_1_USES_MAX6675)
    temp_hotend[1].raw = READ_MAX6675(1);
  #endif
  #if HOTENDS
    HOTEND_LOOP() temp_hotend[e].celsius = analog_to_celsius_hotend(temp_hotend[e].raw, e);
  #endif
  #if HAS_HEATED_BED
    temp_bed.celsius = analog_to_celsius_bed(temp_bed.raw);
  #endif
  #if HAS_TEMP_CHAMBER
    temp_chamber.celsius = analog_to_celsius_chamber(temp_chamber.raw);
  #endif
  #if HAS_TEMP_PROBE
    temp_probe.celsius = analog_to_celsius_probe(temp_probe.raw);
  #endif
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    redundant_temperature = analog_to_celsius_hotend(redundant_temperature_raw, 1);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    filwidth.update_measured_mm();
  #endif

  // Reset the watchdog on good temperature measurement
  watchdog_refresh();

  raw_temps_ready = false;
}

#if MAX6675_SEPARATE_SPI
  SPIclass<MAX6675_DO_PIN, MOSI_PIN, MAX6675_SCK_PIN> max6675_spi;
#endif

// Init fans according to whether they're native PWM or Software PWM
#ifdef ALFAWISE_UX0
  #define _INIT_SOFT_FAN(P) OUT_WRITE_OD(P, FAN_INVERTING ? LOW : HIGH)
#else
  #define _INIT_SOFT_FAN(P) OUT_WRITE(P, FAN_INVERTING ? LOW : HIGH)
#endif
#if ENABLED(FAN_SOFT_PWM)
  #define _INIT_FAN_PIN(P) _INIT_SOFT_FAN(P)
#else
  #define _INIT_FAN_PIN(P) do{ if (PWM_PIN(P)) SET_PWM(P); else _INIT_SOFT_FAN(P); }while(0)
#endif
#if ENABLED(FAST_PWM_FAN)
  #define SET_FAST_PWM_FREQ(P) set_pwm_frequency(P, FAST_PWM_FAN_FREQUENCY)
#else
  #define SET_FAST_PWM_FREQ(P) NOOP
#endif
#define INIT_FAN_PIN(P) do{ _INIT_FAN_PIN(P); SET_FAST_PWM_FREQ(P); }while(0)
#if EXTRUDER_AUTO_FAN_SPEED != 255
  #define INIT_E_AUTO_FAN_PIN(P) do{ if (P == FAN1_PIN || P == FAN2_PIN) { SET_PWM(P); SET_FAST_PWM_FREQ(FAST_PWM_FAN_FREQUENCY); } else SET_OUTPUT(P); }while(0)
#else
  #define INIT_E_AUTO_FAN_PIN(P) SET_OUTPUT(P)
#endif
#if CHAMBER_AUTO_FAN_SPEED != 255
  #define INIT_CHAMBER_AUTO_FAN_PIN(P) do{ if (P == FAN1_PIN || P == FAN2_PIN) { SET_PWM(P); SET_FAST_PWM_FREQ(FAST_PWM_FAN_FREQUENCY); } else SET_OUTPUT(P); }while(0)
#else
  #define INIT_CHAMBER_AUTO_FAN_PIN(P) SET_OUTPUT(P)
#endif

/**
 * Initialize the temperature manager
 * The manager is implemented by periodic calls to manage_heater()
 */
void Temperature::init() {

  #if ENABLED(MAX6675_IS_MAX31865)
    max31865.begin(MAX31865_2WIRE); // MAX31865_2WIRE, MAX31865_3WIRE, MAX31865_4WIRE
  #endif

  #if EARLY_WATCHDOG
    // Flag that the thermalManager should be running
    if (inited) return;
    inited = true;
  #endif

  #if MB(RUMBA)
    #define _AD(N) ANY(HEATER_##N##_USES_AD595, HEATER_##N##_USES_AD8495)
    #if  _AD(0) || _AD(1) || _AD(2) /* RUMBA has 3 E plugs // || _AD(3) || _AD(4) || _AD(5) || _AD(6) || _AD(7) */ \
      || _AD(BED) || _AD(CHAMBER)
      // Disable RUMBA JTAG in case the thermocouple extension is plugged on top of JTAG connector
      MCUCR = _BV(JTD);
      MCUCR = _BV(JTD);
    #endif
  #endif

  #if BOTH(PIDTEMP, PID_EXTRUSION_SCALING)
    last_e_position = 0;
  #endif

  #if HAS_HEATER_0
    #ifdef ALFAWISE_UX0
      OUT_WRITE_OD(HEATER_0_PIN, HEATER_0_INVERTING);
    #else
      OUT_WRITE(HEATER_0_PIN, HEATER_0_INVERTING);
    #endif
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
  #if HAS_HEATER_5
    OUT_WRITE(HEATER_5_PIN, HEATER_5_INVERTING);
  #endif
  #if HAS_HEATER_6
    OUT_WRITE(HEATER_6_PIN, HEATER_6_INVERTING);
  #endif
  #if HAS_HEATER_7
    OUT_WRITE(HEATER_7_PIN, HEATER_7_INVERTING);
  #endif

  #if HAS_HEATED_BED
    #ifdef ALFAWISE_UX0
      OUT_WRITE_OD(HEATER_BED_PIN, HEATER_BED_INVERTING);
    #else
      OUT_WRITE(HEATER_BED_PIN, HEATER_BED_INVERTING);
    #endif
  #endif

  #if HAS_HEATED_CHAMBER
    OUT_WRITE(HEATER_CHAMBER_PIN, HEATER_CHAMBER_INVERTING);
  #endif

  #if HAS_FAN0
    INIT_FAN_PIN(FAN_PIN);
  #endif
  #if HAS_FAN1
    INIT_FAN_PIN(FAN1_PIN);
  #endif
  #if HAS_FAN2
    INIT_FAN_PIN(FAN2_PIN);
  #endif
  #if HAS_FAN3
    INIT_FAN_PIN(FAN3_PIN);
  #endif
  #if HAS_FAN4
    INIT_FAN_PIN(FAN4_PIN);
  #endif
  #if HAS_FAN5
    INIT_FAN_PIN(FAN5_PIN);
  #endif
  #if HAS_FAN6
    INIT_FAN_PIN(FAN6_PIN);
  #endif
  #if HAS_FAN7
    INIT_FAN_PIN(FAN7_PIN);
  #endif
  #if ENABLED(USE_CONTROLLER_FAN)
    INIT_FAN_PIN(CONTROLLER_FAN_PIN);
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
  #if HAS_TEMP_ADC_6
    HAL_ANALOG_SELECT(TEMP_6_PIN);
  #endif
  #if HAS_TEMP_ADC_7
    HAL_ANALOG_SELECT(TEMP_7_PIN);
  #endif
  #if HAS_JOY_ADC_X
    HAL_ANALOG_SELECT(JOY_X_PIN);
  #endif
  #if HAS_JOY_ADC_Y
    HAL_ANALOG_SELECT(JOY_Y_PIN);
  #endif
  #if HAS_JOY_ADC_Z
    HAL_ANALOG_SELECT(JOY_Z_PIN);
  #endif
  #if HAS_JOY_ADC_EN
    SET_INPUT_PULLUP(JOY_EN_PIN);
  #endif
  #if HAS_HEATED_BED
    HAL_ANALOG_SELECT(TEMP_BED_PIN);
  #endif
  #if HAS_TEMP_CHAMBER
    HAL_ANALOG_SELECT(TEMP_CHAMBER_PIN);
  #endif
  #if HAS_TEMP_PROBE
    HAL_ANALOG_SELECT(TEMP_PROBE_PIN);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    HAL_ANALOG_SELECT(FILWIDTH_PIN);
  #endif
  #if HAS_ADC_BUTTONS
    HAL_ANALOG_SELECT(ADC_KEYPAD_PIN);
  #endif

  HAL_timer_start(TEMP_TIMER_NUM, TEMP_TIMER_FREQUENCY);
  ENABLE_TEMPERATURE_INTERRUPT();

  #if HAS_AUTO_FAN_0
    INIT_E_AUTO_FAN_PIN(E0_AUTO_FAN_PIN);
  #endif
  #if HAS_AUTO_FAN_1 && !_EFANOVERLAP(1,0)
    INIT_E_AUTO_FAN_PIN(E1_AUTO_FAN_PIN);
  #endif
  #if HAS_AUTO_FAN_2 && !(_EFANOVERLAP(2,0) || _EFANOVERLAP(2,1))
    INIT_E_AUTO_FAN_PIN(E2_AUTO_FAN_PIN);
  #endif
  #if HAS_AUTO_FAN_3 && !(_EFANOVERLAP(3,0) || _EFANOVERLAP(3,1) || _EFANOVERLAP(3,2))
    INIT_E_AUTO_FAN_PIN(E3_AUTO_FAN_PIN);
  #endif
  #if HAS_AUTO_FAN_4 && !(_EFANOVERLAP(4,0) || _EFANOVERLAP(4,1) || _EFANOVERLAP(4,2) || _EFANOVERLAP(4,3))
    INIT_E_AUTO_FAN_PIN(E4_AUTO_FAN_PIN);
  #endif
  #if HAS_AUTO_FAN_5 && !(_EFANOVERLAP(5,0) || _EFANOVERLAP(5,1) || _EFANOVERLAP(5,2) || _EFANOVERLAP(5,3) || _EFANOVERLAP(5,4))
    INIT_E_AUTO_FAN_PIN(E5_AUTO_FAN_PIN);
  #endif
  #if HAS_AUTO_FAN_6 && !(_EFANOVERLAP(6,0) || _EFANOVERLAP(6,1) || _EFANOVERLAP(6,2) || _EFANOVERLAP(6,3) || _EFANOVERLAP(6,4) || _EFANOVERLAP(6,5))
    INIT_E_AUTO_FAN_PIN(E6_AUTO_FAN_PIN);
  #endif
  #if HAS_AUTO_FAN_7 && !(_EFANOVERLAP(7,0) || _EFANOVERLAP(7,1) || _EFANOVERLAP(7,2) || _EFANOVERLAP(7,3) || _EFANOVERLAP(7,4) || _EFANOVERLAP(7,5) || _EFANOVERLAP(7,6))
    INIT_E_AUTO_FAN_PIN(E7_AUTO_FAN_PIN);
  #endif
  #if HAS_AUTO_CHAMBER_FAN && !AUTO_CHAMBER_IS_E
    INIT_CHAMBER_AUTO_FAN_PIN(CHAMBER_AUTO_FAN_PIN);
  #endif

  // Wait for temperature measurement to settle
  delay(250);

  #if HOTENDS

    #define _TEMP_MIN_E(NR) do{ \
      temp_range[NR].mintemp = HEATER_ ##NR## _MINTEMP; \
      while (analog_to_celsius_hotend(temp_range[NR].raw_min, NR) < HEATER_ ##NR## _MINTEMP) \
        temp_range[NR].raw_min += TEMPDIR(NR) * (OVERSAMPLENR); \
    }while(0)
    #define _TEMP_MAX_E(NR) do{ \
      temp_range[NR].maxtemp = HEATER_ ##NR## _MAXTEMP; \
      while (analog_to_celsius_hotend(temp_range[NR].raw_max, NR) > HEATER_ ##NR## _MAXTEMP) \
        temp_range[NR].raw_max -= TEMPDIR(NR) * (OVERSAMPLENR); \
    }while(0)

    #ifdef HEATER_0_MINTEMP
      _TEMP_MIN_E(0);
    #endif
    #ifdef HEATER_0_MAXTEMP
      _TEMP_MAX_E(0);
    #endif
    #if HOTENDS > 1
      #ifdef HEATER_1_MINTEMP
        _TEMP_MIN_E(1);
      #endif
      #ifdef HEATER_1_MAXTEMP
        _TEMP_MAX_E(1);
      #endif
      #if HOTENDS > 2
        #ifdef HEATER_2_MINTEMP
          _TEMP_MIN_E(2);
        #endif
        #ifdef HEATER_2_MAXTEMP
          _TEMP_MAX_E(2);
        #endif
        #if HOTENDS > 3
          #ifdef HEATER_3_MINTEMP
            _TEMP_MIN_E(3);
          #endif
          #ifdef HEATER_3_MAXTEMP
            _TEMP_MAX_E(3);
          #endif
          #if HOTENDS > 4
            #ifdef HEATER_4_MINTEMP
              _TEMP_MIN_E(4);
            #endif
            #ifdef HEATER_4_MAXTEMP
              _TEMP_MAX_E(4);
            #endif
            #if HOTENDS > 5
              #ifdef HEATER_5_MINTEMP
                _TEMP_MIN_E(5);
              #endif
              #ifdef HEATER_5_MAXTEMP
                _TEMP_MAX_E(5);
              #endif
              #if HOTENDS > 6
                #ifdef HEATER_6_MINTEMP
                  _TEMP_MIN_E(6);
                #endif
                #ifdef HEATER_6_MAXTEMP
                  _TEMP_MAX_E(6);
                #endif
                #if HOTENDS > 7
                  #ifdef HEATER_7_MINTEMP
                    _TEMP_MIN_E(7);
                  #endif
                  #ifdef HEATER_7_MAXTEMP
                    _TEMP_MAX_E(7);
                  #endif
                #endif // HOTENDS > 7
              #endif // HOTENDS > 6
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1

  #endif // HOTENDS

  #if HAS_HEATED_BED
    #ifdef BED_MINTEMP
      while (analog_to_celsius_bed(mintemp_raw_BED) < BED_MINTEMP) mintemp_raw_BED += TEMPDIR(BED) * (OVERSAMPLENR);
    #endif
    #ifdef BED_MAXTEMP
      while (analog_to_celsius_bed(maxtemp_raw_BED) > BED_MAXTEMP) maxtemp_raw_BED -= TEMPDIR(BED) * (OVERSAMPLENR);
    #endif
  #endif // HAS_HEATED_BED

  #if HAS_HEATED_CHAMBER
    #ifdef CHAMBER_MINTEMP
      while (analog_to_celsius_chamber(mintemp_raw_CHAMBER) < CHAMBER_MINTEMP) mintemp_raw_CHAMBER += TEMPDIR(CHAMBER) * (OVERSAMPLENR);
    #endif
    #ifdef CHAMBER_MAXTEMP
      while (analog_to_celsius_chamber(maxtemp_raw_CHAMBER) > CHAMBER_MAXTEMP) maxtemp_raw_CHAMBER -= TEMPDIR(CHAMBER) * (OVERSAMPLENR);
    #endif
  #endif

  #if ENABLED(PROBING_HEATERS_OFF)
    paused = false;
  #endif
}

#if WATCH_HOTENDS
  /**
   * Start Heating Sanity Check for hotends that are below
   * their target temperature by a configurable margin.
   * This is called when the temperature is set. (M104, M109)
   */
  void Temperature::start_watching_hotend(const uint8_t E_NAME) {
    const uint8_t ee = HOTEND_INDEX;
    watch_hotend[ee].restart(degHotend(ee), degTargetHotend(ee));
  }
#endif

#if WATCH_BED
  /**
   * Start Heating Sanity Check for hotends that are below
   * their target temperature by a configurable margin.
   * This is called when the temperature is set. (M140, M190)
   */
  void Temperature::start_watching_bed() {
    watch_bed.restart(degBed(), degTargetBed());
  }
#endif

#if WATCH_CHAMBER
  /**
   * Start Heating Sanity Check for chamber that is below
   * its target temperature by a configurable margin.
   * This is called when the temperature is set. (M141, M191)
   */
  void Temperature::start_watching_chamber() {
    watch_chamber.restart(degChamber(), degTargetChamber());
  }
#endif

#if HAS_THERMAL_PROTECTION

  #if ENABLED(THERMAL_PROTECTION_HOTENDS)
    Temperature::tr_state_machine_t Temperature::tr_state_machine[HOTENDS]; // = { { TRInactive, 0 } };
  #endif
  #if HAS_THERMALLY_PROTECTED_BED
    Temperature::tr_state_machine_t Temperature::tr_state_machine_bed; // = { TRInactive, 0 };
  #endif
  #if ENABLED(THERMAL_PROTECTION_CHAMBER)
    Temperature::tr_state_machine_t Temperature::tr_state_machine_chamber; // = { TRInactive, 0 };
  #endif

  void Temperature::thermal_runaway_protection(Temperature::tr_state_machine_t &sm, const float &current, const float &target, const heater_ind_t heater_id, const uint16_t period_seconds, const uint16_t hysteresis_degc) {

    static float tr_target_temperature[HOTENDS + 1] = { 0.0 };

    /**
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("Thermal Thermal Runaway Running. Heater ID: ");
      if (heater_id == H_CHAMBER) SERIAL_ECHOPGM("chamber");
      if (heater_id < 0) SERIAL_ECHOPGM("bed"); else SERIAL_ECHO(heater_id);
      SERIAL_ECHOPAIR(" ;  State:", sm.state, " ;  Timer:", sm.timer, " ;  Temperature:", current, " ;  Target Temp:", target);
      if (heater_id >= 0)
        SERIAL_ECHOPAIR(" ;  Idle Timeout:", hotend_idle[heater_id].timed_out);
      else
        SERIAL_ECHOPAIR(" ;  Idle Timeout:", bed_idle.timed_out);
      SERIAL_EOL();
    //*/

    const int heater_index = heater_id >= 0 ? heater_id : HOTENDS;

    #if HEATER_IDLE_HANDLER
      // If the heater idle timeout expires, restart
      if ((heater_id >= 0 && hotend_idle[heater_id].timed_out)
        #if HAS_HEATED_BED
          || (heater_id < 0 && bed_idle.timed_out)
        #endif
      ) {
        sm.state = TRInactive;
        tr_target_temperature[heater_index] = 0;
      }
      else
    #endif
    {
      // If the target temperature changes, restart
      if (tr_target_temperature[heater_index] != target) {
        tr_target_temperature[heater_index] = target;
        sm.state = target > 0 ? TRFirstHeating : TRInactive;
      }
    }

    switch (sm.state) {
      // Inactive state waits for a target temperature to be set
      case TRInactive: break;

      // When first heating, wait for the temperature to be reached then go to Stable state
      case TRFirstHeating:
        if (current < tr_target_temperature[heater_index]) break;
        sm.state = TRStable;

      // While the temperature is stable watch for a bad temperature
      case TRStable:

        #if ENABLED(ADAPTIVE_FAN_SLOWING)
          if (adaptive_fan_slowing && heater_id >= 0) {
            const int fan_index = _MIN(heater_id, FAN_COUNT - 1);
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
          sm.timer = millis() + period_seconds * 1000UL;
          break;
        }
        else if (PENDING(millis(), sm.timer)) break;
        sm.state = TRRunaway;

      case TRRunaway:
        _temp_error(heater_id, str_t_thermal_runaway, GET_TEXT(MSG_THERMAL_RUNAWAY));
    }
  }

#endif // HAS_THERMAL_PROTECTION

void Temperature::disable_all_heaters() {

  #if ENABLED(AUTOTEMP)
    planner.autotemp_enabled = false;
  #endif

  #if HOTENDS
    HOTEND_LOOP() setTargetHotend(0, e);
  #endif

  #if HAS_HEATED_BED
    setTargetBed(0);
  #endif

  #if HAS_HEATED_CHAMBER
    setTargetChamber(0);
  #endif

  // Unpause and reset everything
  #if ENABLED(PROBING_HEATERS_OFF)
    pause(false);
  #endif

  #define DISABLE_HEATER(N) {           \
    setTargetHotend(0, N);              \
    temp_hotend[N].soft_pwm_amount = 0; \
    WRITE_HEATER_##N(LOW);              \
  }

  #if HAS_TEMP_HOTEND
    REPEAT(HOTENDS, DISABLE_HEATER);
  #endif

  #if HAS_HEATED_BED
    temp_bed.target = 0;
    temp_bed.soft_pwm_amount = 0;
    WRITE_HEATER_BED(LOW);
  #endif

  #if HAS_HEATED_CHAMBER
    temp_chamber.target = 0;
    temp_chamber.soft_pwm_amount = 0;
    WRITE_HEATER_CHAMBER(LOW);
  #endif
}

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)

  bool Temperature::over_autostart_threshold() {
    #if HOTENDS
      HOTEND_LOOP() if (degTargetHotend(e) > (EXTRUDE_MINTEMP) / 2) return true;
    #endif
    #if HAS_HEATED_BED
      if (degTargetBed() > BED_MINTEMP) return true;
    #endif
    #if HAS_HEATED_CHAMBER
      if (degTargetChamber() > CHAMBER_MINTEMP) return true;
    #endif
    return false;
  }

  void Temperature::check_timer_autostart(const bool can_start, const bool can_stop) {
    if (over_autostart_threshold()) {
      if (can_start) startOrResumeJob();
    }
    else if (can_stop) {
      print_job_timer.stop();
      ui.reset_status();
    }
  }

#endif


#if ENABLED(PROBING_HEATERS_OFF)

  void Temperature::pause(const bool p) {
    if (p != paused) {
      paused = p;
      if (p) {
        HOTEND_LOOP() hotend_idle[e].expire(); // timeout immediately
        #if HAS_HEATED_BED
          bed_idle.expire(); // timeout immediately
        #endif
      }
      else {
        HOTEND_LOOP() reset_hotend_idle_timer(e);
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

    #if ENABLED(MAX6675_IS_MAX31865)
      max6675_temp = int(max31865.temperature(100, 400)); // 100 ohms = PT100 resistance. 400 ohms = calibration resistor
    #endif

    //
    // TODO: spiBegin, spiRec and spiInit doesn't work when soft spi is used.
    //
    #if !MAX6675_SEPARATE_SPI
      spiBegin();
      spiInit(MAX6675_SPEED_BITS);
    #endif

    #if COUNT_6675 > 1
      #define WRITE_MAX6675(V) do{ switch (hindex) { case 1: WRITE(MAX6675_SS2_PIN, V); break; default: WRITE(MAX6675_SS_PIN, V); } }while(0)
      #define SET_OUTPUT_MAX6675() do{ switch (hindex) { case 1: SET_OUTPUT(MAX6675_SS2_PIN); break; default: SET_OUTPUT(MAX6675_SS_PIN); } }while(0)
    #elif ENABLED(HEATER_1_USES_MAX6675)
      #define WRITE_MAX6675(V) WRITE(MAX6675_SS2_PIN, V)
      #define SET_OUTPUT_MAX6675() SET_OUTPUT(MAX6675_SS2_PIN)
    #else
      #define WRITE_MAX6675(V) WRITE(MAX6675_SS_PIN, V)
      #define SET_OUTPUT_MAX6675() SET_OUTPUT(MAX6675_SS_PIN)
    #endif

    SET_OUTPUT_MAX6675();
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
 * Update raw temperatures
 */
void Temperature::update_raw_temperatures() {

  #if HAS_TEMP_ADC_0 && DISABLED(HEATER_0_USES_MAX6675)
    temp_hotend[0].update();
  #endif

  #if HAS_TEMP_ADC_1
    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      redundant_temperature_raw = temp_hotend[1].acc;
    #elif DISABLED(HEATER_1_USES_MAX6675)
      temp_hotend[1].update();
    #endif
  #endif

  #if HAS_TEMP_ADC_2
    temp_hotend[2].update();
  #endif
  #if HAS_TEMP_ADC_3
    temp_hotend[3].update();
  #endif
  #if HAS_TEMP_ADC_4
    temp_hotend[4].update();
  #endif
  #if HAS_TEMP_ADC_5
    temp_hotend[5].update();
  #endif
  #if HAS_TEMP_ADC_6
    temp_hotend[6].update();
  #endif
  #if HAS_TEMP_ADC_7
    temp_hotend[7].update();
  #endif

  #if HAS_HEATED_BED
    temp_bed.update();
  #endif

  #if HAS_TEMP_CHAMBER
    temp_chamber.update();
  #endif

  #if HAS_TEMP_PROBE
    temp_probe.update();
  #endif

  #if HAS_JOY_ADC_X
    joystick.x.update();
  #endif
  #if HAS_JOY_ADC_Y
    joystick.y.update();
  #endif
  #if HAS_JOY_ADC_Z
    joystick.z.update();
  #endif

  raw_temps_ready = true;
}

void Temperature::readings_ready() {

  // Update the raw values if they've been read. Else we could be updating them during reading.
  if (!raw_temps_ready) update_raw_temperatures();

  // Filament Sensor - can be read any time since IIR filtering is used
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    filwidth.reading_ready();
  #endif

  #if HOTENDS
    HOTEND_LOOP() temp_hotend[e].reset();
    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      temp_hotend[1].reset();
    #endif
  #endif

  #if HAS_HEATED_BED
    temp_bed.reset();
  #endif

  #if HAS_TEMP_CHAMBER
    temp_chamber.reset();
  #endif

  #if HAS_TEMP_PROBE
    temp_probe.reset();
  #endif

  #if HAS_JOY_ADC_X
    joystick.x.reset();
  #endif
  #if HAS_JOY_ADC_Y
    joystick.y.reset();
  #endif
  #if HAS_JOY_ADC_Z
    joystick.z.reset();
  #endif

  #if HOTENDS

    static constexpr int8_t temp_dir[] = {
      #if ENABLED(HEATER_0_USES_MAX6675)
        0
      #else
        TEMPDIR(0)
      #endif
      #if HOTENDS > 1
        #define _TEMPDIR(N) , TEMPDIR(N)
        #if ENABLED(HEATER_1_USES_MAX6675)
          , 0
        #else
          _TEMPDIR(1)
        #endif
        #if HOTENDS > 2
          REPEAT_S(2, HOTENDS, _TEMPDIR)
        #endif // HOTENDS > 2
      #endif // HOTENDS > 1
    };

    for (uint8_t e = 0; e < COUNT(temp_dir); e++) {
      const int8_t tdir = temp_dir[e];
      if (tdir) {
        const int16_t rawtemp = temp_hotend[e].raw * tdir; // normal direction, +rawtemp, else -rawtemp
        const bool heater_on = (temp_hotend[e].target > 0
          #if ENABLED(PIDTEMP)
            || temp_hotend[e].soft_pwm_amount > 0
          #endif
        );
        if (rawtemp > temp_range[e].raw_max * tdir) max_temp_error((heater_ind_t)e);
        if (heater_on && rawtemp < temp_range[e].raw_min * tdir && !is_preheating(e)) {
          #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
            if (++consecutive_low_temperature_error[e] >= MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED)
          #endif
              min_temp_error((heater_ind_t)e);
        }
        #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
          else
            consecutive_low_temperature_error[e] = 0;
        #endif
      }
    }

  #endif // HOTENDS

  #if HAS_HEATED_BED
    #if TEMPDIR(BED) < 0
      #define BEDCMP(A,B) ((A)<=(B))
    #else
      #define BEDCMP(A,B) ((A)>=(B))
    #endif
    const bool bed_on = (temp_bed.target > 0)
      #if ENABLED(PIDTEMPBED)
        || (temp_bed.soft_pwm_amount > 0)
      #endif
    ;
    if (BEDCMP(temp_bed.raw, maxtemp_raw_BED)) max_temp_error(H_BED);
    if (bed_on && BEDCMP(mintemp_raw_BED, temp_bed.raw)) min_temp_error(H_BED);
  #endif

  #if HAS_HEATED_CHAMBER
    #if TEMPDIR(CHAMBER) < 0
      #define CHAMBERCMP(A,B) ((A)<=(B))
    #else
      #define CHAMBERCMP(A,B) ((A)>=(B))
    #endif
    const bool chamber_on = (temp_chamber.target > 0);
    if (CHAMBERCMP(temp_chamber.raw, maxtemp_raw_CHAMBER)) max_temp_error(H_CHAMBER);
    if (chamber_on && CHAMBERCMP(mintemp_raw_CHAMBER, temp_chamber.raw)) min_temp_error(H_CHAMBER);
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
HAL_TEMP_TIMER_ISR() {
  HAL_timer_isr_prologue(TEMP_TIMER_NUM);

  Temperature::tick();

  HAL_timer_isr_epilogue(TEMP_TIMER_NUM);
}

#if ENABLED(SLOW_PWM_HEATERS) && !defined(MIN_STATE_TIME)
  #define MIN_STATE_TIME 16 // MIN_STATE_TIME * 65.5 = time in milliseconds
#endif

class SoftPWM {
public:
  uint8_t count;
  inline bool add(const uint8_t mask, const uint8_t amount) {
    count = (count & mask) + amount; return (count > mask);
  }
  #if ENABLED(SLOW_PWM_HEATERS)
    bool state_heater;
    uint8_t state_timer_heater;
    inline void dec() { if (state_timer_heater > 0) state_timer_heater--; }
    inline bool ready(const bool v) {
      const bool rdy = !state_timer_heater;
      if (rdy && state_heater != v) {
        state_heater = v;
        state_timer_heater = MIN_STATE_TIME;
      }
      return rdy;
    }
  #endif
};

/**
 * Handle various ~1KHz tasks associated with temperature
 *  - Heater PWM (~1KHz with scaler)
 *  - LCD Button polling (~500Hz)
 *  - Start / Read one ADC sensor
 *  - Advance Babysteps
 *  - Endstop polling
 *  - Planner clean buffer
 */
void Temperature::tick() {

  static int8_t temp_count = -1;
  static ADCSensorState adc_sensor_state = StartupDelay;
  static uint8_t pwm_count = _BV(SOFT_PWM_SCALE);

  // avoid multiple loads of pwm_count
  uint8_t pwm_count_tmp = pwm_count;

  #if HAS_ADC_BUTTONS
    static unsigned int raw_ADCKey_value = 0;
    static bool ADCKey_pressed = false;
  #endif

  #if HOTENDS
    static SoftPWM soft_pwm_hotend[HOTENDS];
  #endif

  #if HAS_HEATED_BED
    static SoftPWM soft_pwm_bed;
  #endif

  #if HAS_HEATED_CHAMBER
    static SoftPWM soft_pwm_chamber;
  #endif

  #if DISABLED(SLOW_PWM_HEATERS)

    #if HOTENDS || HAS_HEATED_BED || HAS_HEATED_CHAMBER
      constexpr uint8_t pwm_mask =
        #if ENABLED(SOFT_PWM_DITHER)
          _BV(SOFT_PWM_SCALE) - 1
        #else
          0
        #endif
      ;
      #define _PWM_MOD(N,S,T) do{                           \
        const bool on = S.add(pwm_mask, T.soft_pwm_amount); \
        WRITE_HEATER_##N(on);                               \
      }while(0)
    #endif

    /**
     * Standard heater PWM modulation
     */
    if (pwm_count_tmp >= 127) {
      pwm_count_tmp -= 127;

      #if HOTENDS
        #define _PWM_MOD_E(N) _PWM_MOD(N,soft_pwm_hotend[N],temp_hotend[N]);
        REPEAT(HOTENDS, _PWM_MOD_E);
      #endif

      #if HAS_HEATED_BED
        _PWM_MOD(BED,soft_pwm_bed,temp_bed);
      #endif

      #if HAS_HEATED_CHAMBER
        _PWM_MOD(CHAMBER,soft_pwm_chamber,temp_chamber);
      #endif

      #if ENABLED(FAN_SOFT_PWM)
        #define _FAN_PWM(N) do{                                     \
          uint8_t &spcf = soft_pwm_count_fan[N];                    \
          spcf = (spcf & pwm_mask) + (soft_pwm_amount_fan[N] >> 1); \
          WRITE_FAN(N, spcf > pwm_mask ? HIGH : LOW);               \
        }while(0)
        #if HAS_FAN0
          _FAN_PWM(0);
        #endif
        #if HAS_FAN1
          _FAN_PWM(1);
        #endif
        #if HAS_FAN2
          _FAN_PWM(2);
        #endif
        #if HAS_FAN3
          _FAN_PWM(3);
        #endif
        #if HAS_FAN4
          _FAN_PWM(4);
        #endif
        #if HAS_FAN5
          _FAN_PWM(5);
        #endif
        #if HAS_FAN6
          _FAN_PWM(6);
        #endif
        #if HAS_FAN7
          _FAN_PWM(7);
        #endif
      #endif
    }
    else {
      #define _PWM_LOW(N,S) do{ if (S.count <= pwm_count_tmp) WRITE_HEATER_##N(LOW); }while(0)
      #if HOTENDS
        #define _PWM_LOW_E(N) _PWM_LOW(N, soft_pwm_hotend[N]);
        REPEAT(HOTENDS, _PWM_LOW_E);
      #endif

      #if HAS_HEATED_BED
        _PWM_LOW(BED, soft_pwm_bed);
      #endif

      #if HAS_HEATED_CHAMBER
        _PWM_LOW(CHAMBER, soft_pwm_chamber);
      #endif

      #if ENABLED(FAN_SOFT_PWM)
        #if HAS_FAN0
          if (soft_pwm_count_fan[0] <= pwm_count_tmp) WRITE_FAN(0, LOW);
        #endif
        #if HAS_FAN1
          if (soft_pwm_count_fan[1] <= pwm_count_tmp) WRITE_FAN(1, LOW);
        #endif
        #if HAS_FAN2
          if (soft_pwm_count_fan[2] <= pwm_count_tmp) WRITE_FAN(2, LOW);
        #endif
        #if HAS_FAN3
          if (soft_pwm_count_fan[3] <= pwm_count_tmp) WRITE_FAN(3, LOW);
        #endif
        #if HAS_FAN4
          if (soft_pwm_count_fan[4] <= pwm_count_tmp) WRITE_FAN(4, LOW);
        #endif
        #if HAS_FAN5
          if (soft_pwm_count_fan[5] <= pwm_count_tmp) WRITE_FAN(5, LOW);
        #endif
        #if HAS_FAN6
          if (soft_pwm_count_fan[6] <= pwm_count_tmp) WRITE_FAN(6, LOW);
        #endif
        #if HAS_FAN7
          if (soft_pwm_count_fan[7] <= pwm_count_tmp) WRITE_FAN(7, LOW);
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
    #define _SLOW_SET(NR,PWM,V) do{ if (PWM.ready(V)) WRITE_HEATER_##NR(V); }while(0)
    #define _SLOW_PWM(NR,PWM,SRC) do{ PWM.count = SRC.soft_pwm_amount; _SLOW_SET(NR,PWM,(PWM.count > 0)); }while(0)
    #define _PWM_OFF(NR,PWM) do{ if (PWM.count < slow_pwm_count) _SLOW_SET(NR,PWM,0); }while(0)

    static uint8_t slow_pwm_count = 0;

    if (slow_pwm_count == 0) {

      #if HOTENDS
        #define _SLOW_PWM_E(N) _SLOW_PWM(N, soft_pwm_hotend[N], temp_hotend[N]);
        REPEAT(HOTENDS, _SLOW_PWM_E);
      #endif

      #if HAS_HEATED_BED
        _SLOW_PWM(BED, soft_pwm_bed, temp_bed);
      #endif

    } // slow_pwm_count == 0

    #if HOTENDS
      #define _PWM_OFF_E(N) _PWM_OFF(N, soft_pwm_hotend[N]);
      REPEAT(HOTENDS, _PWM_OFF_E);
    #endif

    #if HAS_HEATED_BED
      _PWM_OFF(BED, soft_pwm_bed);
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      if (pwm_count_tmp >= 127) {
        pwm_count_tmp = 0;
        #define _PWM_FAN(N) do{                                 \
          soft_pwm_count_fan[N] = soft_pwm_amount_fan[N] >> 1;  \
          WRITE_FAN(N, soft_pwm_count_fan[N] > 0 ? HIGH : LOW); \
        }while(0)
        #if HAS_FAN0
          _PWM_FAN(0);
        #endif
        #if HAS_FAN1
          _PWM_FAN(1);
        #endif
        #if HAS_FAN2
          _PWM_FAN(2);
        #endif
        #if HAS_FAN3
          _FAN_PWM(3);
        #endif
        #if HAS_FAN4
          _FAN_PWM(4);
        #endif
        #if HAS_FAN5
          _FAN_PWM(5);
        #endif
        #if HAS_FAN6
          _FAN_PWM(6);
        #endif
        #if HAS_FAN7
          _FAN_PWM(7);
        #endif
      }
      #if HAS_FAN0
        if (soft_pwm_count_fan[0] <= pwm_count_tmp) WRITE_FAN(0, LOW);
      #endif
      #if HAS_FAN1
        if (soft_pwm_count_fan[1] <= pwm_count_tmp) WRITE_FAN(1, LOW);
      #endif
      #if HAS_FAN2
        if (soft_pwm_count_fan[2] <= pwm_count_tmp) WRITE_FAN(2, LOW);
      #endif
      #if HAS_FAN3
        if (soft_pwm_count_fan[3] <= pwm_count_tmp) WRITE_FAN(3, LOW);
      #endif
      #if HAS_FAN4
        if (soft_pwm_count_fan[4] <= pwm_count_tmp) WRITE_FAN(4, LOW);
      #endif
      #if HAS_FAN5
        if (soft_pwm_count_fan[5] <= pwm_count_tmp) WRITE_FAN(5, LOW);
      #endif
      #if HAS_FAN6
        if (soft_pwm_count_fan[6] <= pwm_count_tmp) WRITE_FAN(6, LOW);
      #endif
      #if HAS_FAN7
        if (soft_pwm_count_fan[7] <= pwm_count_tmp) WRITE_FAN(7, LOW);
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

      #if HOTENDS
        HOTEND_LOOP() soft_pwm_hotend[e].dec();
      #endif
      #if HAS_HEATED_BED
        soft_pwm_bed.dec();
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
  #define ACCUMULATE_ADC(obj) do{ \
    if (!HAL_ADC_READY()) next_sensor_state = adc_sensor_state; \
    else obj.sample(HAL_READ_ADC()); \
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
      case PrepareTemp_0: HAL_START_ADC(TEMP_0_PIN); break;
      case MeasureTemp_0: ACCUMULATE_ADC(temp_hotend[0]); break;
    #endif

    #if HAS_HEATED_BED
      case PrepareTemp_BED: HAL_START_ADC(TEMP_BED_PIN); break;
      case MeasureTemp_BED: ACCUMULATE_ADC(temp_bed); break;
    #endif

    #if HAS_TEMP_CHAMBER
      case PrepareTemp_CHAMBER: HAL_START_ADC(TEMP_CHAMBER_PIN); break;
      case MeasureTemp_CHAMBER: ACCUMULATE_ADC(temp_chamber); break;
    #endif

    #if HAS_TEMP_PROBE
      case PrepareTemp_PROBE: HAL_START_ADC(TEMP_PROBE_PIN); break;
      case MeasureTemp_PROBE: ACCUMULATE_ADC(temp_probe); break;
    #endif

    #if HAS_TEMP_ADC_1
      case PrepareTemp_1: HAL_START_ADC(TEMP_1_PIN); break;
      case MeasureTemp_1: ACCUMULATE_ADC(temp_hotend[1]); break;
    #endif

    #if HAS_TEMP_ADC_2
      case PrepareTemp_2: HAL_START_ADC(TEMP_2_PIN); break;
      case MeasureTemp_2: ACCUMULATE_ADC(temp_hotend[2]); break;
    #endif

    #if HAS_TEMP_ADC_3
      case PrepareTemp_3: HAL_START_ADC(TEMP_3_PIN); break;
      case MeasureTemp_3: ACCUMULATE_ADC(temp_hotend[3]); break;
    #endif

    #if HAS_TEMP_ADC_4
      case PrepareTemp_4: HAL_START_ADC(TEMP_4_PIN); break;
      case MeasureTemp_4: ACCUMULATE_ADC(temp_hotend[4]); break;
    #endif

    #if HAS_TEMP_ADC_5
      case PrepareTemp_5: HAL_START_ADC(TEMP_5_PIN); break;
      case MeasureTemp_5: ACCUMULATE_ADC(temp_hotend[5]); break;
    #endif

    #if HAS_TEMP_ADC_6
      case PrepareTemp_6: HAL_START_ADC(TEMP_6_PIN); break;
      case MeasureTemp_6: ACCUMULATE_ADC(temp_hotend[6]); break;
    #endif

    #if HAS_TEMP_ADC_7
      case PrepareTemp_7: HAL_START_ADC(TEMP_7_PIN); break;
      case MeasureTemp_7: ACCUMULATE_ADC(temp_hotend[7]); break;
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      case Prepare_FILWIDTH: HAL_START_ADC(FILWIDTH_PIN); break;
      case Measure_FILWIDTH:
        if (!HAL_ADC_READY())
          next_sensor_state = adc_sensor_state; // redo this state
        else
          filwidth.accumulate(HAL_READ_ADC());
      break;
    #endif

    #if HAS_JOY_ADC_X
      case PrepareJoy_X: HAL_START_ADC(JOY_X_PIN); break;
      case MeasureJoy_X: ACCUMULATE_ADC(joystick.x); break;
    #endif

    #if HAS_JOY_ADC_Y
      case PrepareJoy_Y: HAL_START_ADC(JOY_Y_PIN); break;
      case MeasureJoy_Y: ACCUMULATE_ADC(joystick.y); break;
    #endif

    #if HAS_JOY_ADC_Z
      case PrepareJoy_Z: HAL_START_ADC(JOY_Z_PIN); break;
      case MeasureJoy_Z: ACCUMULATE_ADC(joystick.z); break;
    #endif

    #if HAS_ADC_BUTTONS
      #ifndef ADC_BUTTON_DEBOUNCE_DELAY
        #define ADC_BUTTON_DEBOUNCE_DELAY 16
      #endif
      case Prepare_ADC_KEY: HAL_START_ADC(ADC_KEYPAD_PIN); break;
      case Measure_ADC_KEY:
        if (!HAL_ADC_READY())
          next_sensor_state = adc_sensor_state; // redo this state
        else if (ADCKey_count < ADC_BUTTON_DEBOUNCE_DELAY) {
          raw_ADCKey_value = HAL_READ_ADC();
          if (raw_ADCKey_value <= 900UL * HAL_ADC_RANGE / 1024UL) {
            NOMORE(current_ADCKey_raw, raw_ADCKey_value);
            ADCKey_count++;
          }
          else { //ADC Key release
            if (ADCKey_count > 0) ADCKey_count++; else ADCKey_pressed = false;
            if (ADCKey_pressed) {
              ADCKey_count = 0;
              current_ADCKey_raw = HAL_ADC_RANGE;
            }
          }
        }
        if (ADCKey_count == ADC_BUTTON_DEBOUNCE_DELAY) ADCKey_pressed = true;
        break;
    #endif // HAS_ADC_BUTTONS

    case StartupDelay: break;

  } // switch(adc_sensor_state)

  // Go to the next state
  adc_sensor_state = next_sensor_state;

  //
  // Additional ~1KHz Tasks
  //

  #if ENABLED(BABYSTEPPING) && DISABLED(INTEGRATED_BABYSTEPPING)
    babystep.task();
  #endif

  // Poll endstops state, if required
  endstops.poll();

  // Periodically call the planner timer
  planner.tick();
}

#if HAS_TEMP_SENSOR

  #include "../gcode/gcode.h"

  static void print_heater_state(const float &c, const float &t
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      , const float r
    #endif
    , const heater_ind_t e=INDEX_NONE
  ) {
    char k;
    switch (e) {
      #if HAS_TEMP_CHAMBER
        case H_CHAMBER: k = 'C'; break;
      #endif
      #if HAS_TEMP_PROBE
        case H_PROBE: k = 'P'; break;
      #endif
      #if HAS_TEMP_HOTEND
        default: k = 'T'; break;
        #if HAS_HEATED_BED
          case H_BED: k = 'B'; break;
        #endif
        #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
          case H_REDUNDANT: k = 'R'; break;
        #endif
      #elif HAS_HEATED_BED
        default: k = 'B'; break;
      #endif
    }
    SERIAL_CHAR(' ');
    SERIAL_CHAR(k);
    #if HOTENDS > 1
      if (e >= 0) SERIAL_CHAR('0' + e);
    #endif
    SERIAL_CHAR(':');
    SERIAL_ECHO(c);
    SERIAL_ECHOPAIR(" /" , t);
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      SERIAL_ECHOPAIR(" (", r * RECIPROCAL(OVERSAMPLENR));
      SERIAL_CHAR(')');
    #endif
    delay(2);
  }

  void Temperature::print_heater_states(const uint8_t target_extruder
    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      , const bool include_r/*=false*/
    #endif
  ) {
    #if HAS_TEMP_HOTEND
      print_heater_state(degHotend(target_extruder), degTargetHotend(target_extruder)
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawHotendTemp(target_extruder)
        #endif
      );
      #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
        if (include_r) print_heater_state(redundant_temperature, degTargetHotend(target_extruder)
          #if ENABLED(SHOW_TEMP_ADC_VALUES)
            , redundant_temperature_raw
          #endif
          , H_REDUNDANT
        );
      #endif
    #endif
    #if HAS_HEATED_BED
      print_heater_state(degBed(), degTargetBed()
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawBedTemp()
        #endif
        , H_BED
      );
    #endif
    #if HAS_TEMP_CHAMBER
      print_heater_state(degChamber()
        #if HAS_HEATED_CHAMBER
          , degTargetChamber()
        #else
          , 0
        #endif
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawChamberTemp()
        #endif
        , H_CHAMBER
      );
    #endif // HAS_TEMP_CHAMBER
    #if HAS_TEMP_PROBE
      print_heater_state(degProbe(), 0
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawProbeTemp()
        #endif
        , H_PROBE
      );
    #endif // HAS_TEMP_PROBE
    #if HOTENDS > 1
      HOTEND_LOOP() print_heater_state(degHotend(e), degTargetHotend(e)
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawHotendTemp(e)
        #endif
        , (heater_ind_t)e
      );
    #endif
    SERIAL_ECHOPAIR(" @:", getHeaterPower((heater_ind_t)target_extruder));
    #if HAS_HEATED_BED
      SERIAL_ECHOPAIR(" B@:", getHeaterPower(H_BED));
    #endif
    #if HAS_HEATED_CHAMBER
      SERIAL_ECHOPAIR(" C@:", getHeaterPower(H_CHAMBER));
    #endif
    #if HOTENDS > 1
      HOTEND_LOOP() {
        SERIAL_ECHOPAIR(" @", e);
        SERIAL_CHAR(':');
        SERIAL_ECHO(getHeaterPower((heater_ind_t)e));
      }
    #endif
  }

  #if ENABLED(AUTO_REPORT_TEMPERATURES)

    uint8_t Temperature::auto_report_temp_interval;
    millis_t Temperature::next_temp_report_ms;

    void Temperature::auto_report_temperatures() {
      if (auto_report_temp_interval && ELAPSED(millis(), next_temp_report_ms)) {
        next_temp_report_ms = millis() + 1000UL * auto_report_temp_interval;
        PORT_REDIRECT(SERIAL_BOTH);
        print_heater_states(active_extruder);
        SERIAL_EOL();
      }
    }

  #endif // AUTO_REPORT_TEMPERATURES

  #if HOTENDS && HAS_DISPLAY
    void Temperature::set_heating_message(const uint8_t e) {
      const bool heating = isHeatingHotend(e);
      ui.status_printf_P(0,
        #if HOTENDS > 1
          PSTR("E%c " S_FMT), '1' + e
        #else
          PSTR("E " S_FMT)
        #endif
        , heating ? GET_TEXT(MSG_HEATING) : GET_TEXT(MSG_COOLING)
      );
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
        bool first_loop = true;
        // Loop until the temperature has stabilized
        #define TEMP_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + (TEMP_RESIDENCY_TIME) * 1000UL))
      #else
        // Loop until the temperature is very close target
        #define TEMP_CONDITIONS (wants_to_cool ? isCoolingHotend(target_extruder) : isHeatingHotend(target_extruder))
      #endif

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
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
            if (temp_diff < TEMP_WINDOW) {
              residency_start_ms = now;
              if (first_loop) residency_start_ms += (TEMP_RESIDENCY_TIME) * 1000UL;
            }
          }
          else if (temp_diff > TEMP_HYSTERESIS) {
            // Restart the timer whenever the temperature falls outside the hysteresis.
            residency_start_ms = now;
          }

          first_loop = false;

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
        bool first_loop = true;
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
            if (temp_diff < TEMP_BED_WINDOW) {
              residency_start_ms = now;
              if (first_loop) residency_start_ms += (TEMP_BED_RESIDENCY_TIME) * 1000UL;
            }
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

        #if TEMP_BED_RESIDENCY_TIME > 0
          first_loop = false;
        #endif

      } while (wait_for_heatup && TEMP_BED_CONDITIONS);

      if (wait_for_heatup) ui.reset_status();

      return wait_for_heatup;
    }

  #endif // HAS_HEATED_BED

  #if HAS_HEATED_CHAMBER

    #ifndef MIN_COOLING_SLOPE_DEG_CHAMBER
      #define MIN_COOLING_SLOPE_DEG_CHAMBER 1.50
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_CHAMBER
      #define MIN_COOLING_SLOPE_TIME_CHAMBER 60
    #endif

    bool Temperature::wait_for_chamber(const bool no_wait_for_cooling/*=true*/) {
      #if TEMP_CHAMBER_RESIDENCY_TIME > 0
        millis_t residency_start_ms = 0;
        bool first_loop = true;
        // Loop until the temperature has stabilized
        #define TEMP_CHAMBER_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + (TEMP_CHAMBER_RESIDENCY_TIME) * 1000UL))
      #else
        // Loop until the temperature is very close target
        #define TEMP_CHAMBER_CONDITIONS (wants_to_cool ? isCoolingChamber() : isHeatingChamber())
      #endif

      float target_temp = -1, old_temp = 9999;
      bool wants_to_cool = false;
      wait_for_heatup = true;
      millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        KEEPALIVE_STATE(NOT_BUSY);
      #endif

      do {
        // Target temperature might be changed during the loop
        if (target_temp != degTargetChamber()) {
          wants_to_cool = isCoolingChamber();
          target_temp = degTargetChamber();

          // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
          if (no_wait_for_cooling && wants_to_cool) break;
        }

        now = millis();
        if (ELAPSED(now, next_temp_ms)) { //Print Temp Reading every 1 second while heating up.
          next_temp_ms = now + 1000UL;
          print_heater_states(active_extruder);
          #if TEMP_CHAMBER_RESIDENCY_TIME > 0
            SERIAL_ECHOPGM(" W:");
            if (residency_start_ms)
              SERIAL_ECHO(long((((TEMP_CHAMBER_RESIDENCY_TIME) * 1000UL) - (now - residency_start_ms)) / 1000UL));
            else
              SERIAL_CHAR('?');
          #endif
          SERIAL_EOL();
        }

        idle();
        gcode.reset_stepper_timeout(); // Keep steppers powered

        const float temp = degChamber();

        #if TEMP_CHAMBER_RESIDENCY_TIME > 0

          const float temp_diff = ABS(target_temp - temp);

          if (!residency_start_ms) {
            // Start the TEMP_CHAMBER_RESIDENCY_TIME timer when we reach target temp for the first time.
            if (temp_diff < TEMP_CHAMBER_WINDOW) {
              residency_start_ms = now;
              if (first_loop) residency_start_ms += (TEMP_CHAMBER_RESIDENCY_TIME) * 1000UL;
            }
          }
          else if (temp_diff > TEMP_CHAMBER_HYSTERESIS) {
            // Restart the timer whenever the temperature falls outside the hysteresis.
            residency_start_ms = now;
          }

          first_loop = false;
        #endif // TEMP_CHAMBER_RESIDENCY_TIME > 0

        // Prevent a wait-forever situation if R is misused i.e. M191 R0
        if (wants_to_cool) {
          // Break after MIN_COOLING_SLOPE_TIME_CHAMBER seconds
          // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG_CHAMBER
          if (!next_cool_check_ms || ELAPSED(now, next_cool_check_ms)) {
            if (old_temp - temp < float(MIN_COOLING_SLOPE_DEG_CHAMBER)) break;
            next_cool_check_ms = now + 1000UL * MIN_COOLING_SLOPE_TIME_CHAMBER;
            old_temp = temp;
          }
        }
      } while (wait_for_heatup && TEMP_CHAMBER_CONDITIONS);

      if (wait_for_heatup) ui.reset_status();

      return wait_for_heatup;
    }

  #endif // HAS_HEATED_CHAMBER

#endif // HAS_TEMP_SENSOR
