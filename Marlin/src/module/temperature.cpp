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

/**
 * temperature.cpp - temperature control
 */

// Useful when debugging thermocouples
//#define IGNORE_THERMOCOUPLE_ERRORS

#include "../MarlinCore.h"
#include "../HAL/shared/Delay.h"
#include "../lcd/marlinui.h"

#include "temperature.h"
#include "endstops.h"
#include "planner.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "motion.h"
#endif

#if ENABLED(DWIN_CREALITY_LCD)
  #include "../lcd/dwin/e3v2/dwin.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

#if MAX6675_0_IS_MAX31865 || MAX6675_1_IS_MAX31865
  #include <Adafruit_MAX31865.h>
  #if MAX6675_0_IS_MAX31865 && !defined(MAX31865_CS_PIN) && PIN_EXISTS(MAX6675_SS)
    #define MAX31865_CS_PIN   MAX6675_SS_PIN
  #endif
  #if MAX6675_1_IS_MAX31865 && !defined(MAX31865_CS2_PIN) && PIN_EXISTS(MAX6675_SS2)
    #define MAX31865_CS2_PIN  MAX6675_SS2_PIN
  #endif
  #ifndef MAX31865_MOSI_PIN
    #define MAX31865_MOSI_PIN SD_MOSI_PIN
  #endif
  #ifndef MAX31865_MISO_PIN
    #define MAX31865_MISO_PIN MAX6675_DO_PIN
  #endif
  #ifndef MAX31865_SCK_PIN
    #define MAX31865_SCK_PIN  MAX6675_SCK_PIN
  #endif
  #if MAX6675_0_IS_MAX31865 && PIN_EXISTS(MAX31865_CS)
    #define HAS_MAX31865 1
    Adafruit_MAX31865 max31865_0 = Adafruit_MAX31865(MAX31865_CS_PIN
      #if MAX31865_CS_PIN != MAX6675_SS_PIN
        , MAX31865_MOSI_PIN, MAX31865_MISO_PIN, MAX31865_SCK_PIN // For software SPI also set MOSI/MISO/SCK
      #endif
    );
  #endif
  #if MAX6675_1_IS_MAX31865 && PIN_EXISTS(MAX31865_CS2)
    #define HAS_MAX31865 1
    Adafruit_MAX31865 max31865_1 = Adafruit_MAX31865(MAX31865_CS2_PIN
      #if MAX31865_CS2_PIN != MAX6675_SS2_PIN
        , MAX31865_MOSI_PIN, MAX31865_MISO_PIN, MAX31865_SCK_PIN // For software SPI also set MOSI/MISO/SCK
      #endif
    );
  #endif
#endif

#if EITHER(HEATER_0_USES_MAX6675, HEATER_1_USES_MAX6675) && PINS_EXIST(MAX6675_SCK, MAX6675_DO)
  #define MAX6675_SEPARATE_SPI 1
#endif

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

#if HAS_POWER_MONITOR
  #include "../feature/power_monitor.h"
#endif

#if ENABLED(EMERGENCY_PARSER)
  #include "../feature/e_parser.h"
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

#if HAS_SERVOS
  #include "./servo.h"
#endif

#if ANY(HEATER_0_USES_THERMISTOR, HEATER_1_USES_THERMISTOR, HEATER_2_USES_THERMISTOR, HEATER_3_USES_THERMISTOR, \
        HEATER_4_USES_THERMISTOR, HEATER_5_USES_THERMISTOR, HEATER_6_USES_THERMISTOR, HEATER_7_USES_THERMISTOR )
  #define HAS_HOTEND_THERMISTOR 1
#endif

#if HAS_HOTEND_THERMISTOR
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    static const temp_entry_t* heater_ttbl_map[2] = { HEATER_0_TEMPTABLE, HEATER_1_TEMPTABLE };
    static constexpr uint8_t heater_ttbllen_map[2] = { HEATER_0_TEMPTABLE_LEN, HEATER_1_TEMPTABLE_LEN };
  #else
    #define NEXT_TEMPTABLE(N) ,HEATER_##N##_TEMPTABLE
    #define NEXT_TEMPTABLE_LEN(N) ,HEATER_##N##_TEMPTABLE_LEN
    static const temp_entry_t* heater_ttbl_map[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_TEMPTABLE REPEAT_S(1, HOTENDS, NEXT_TEMPTABLE));
    static constexpr uint8_t heater_ttbllen_map[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_TEMPTABLE_LEN REPEAT_S(1, HOTENDS, NEXT_TEMPTABLE_LEN));
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

#if HAS_HOTEND
  hotend_info_t Temperature::temp_hotend[HOTEND_TEMPS]; // = { 0 }
  const uint16_t Temperature::heater_maxtemp[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP, HEATER_5_MAXTEMP, HEATER_6_MAXTEMP, HEATER_7_MAXTEMP);
#endif

#if ENABLED(AUTO_POWER_E_FANS)
  uint8_t Temperature::autofan_speed[HOTENDS]; // = { 0 }
#endif

#if ENABLED(AUTO_POWER_CHAMBER_FAN)
  uint8_t Temperature::chamberfan_speed; // = 0
#endif

#if HAS_FAN

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
    uint8_t Temperature::fan_speed_scaler[FAN_COUNT] = ARRAY_N(FAN_COUNT, 128, 128, 128, 128, 128, 128, 128, 128);
  #endif

  /**
   * Set the print fan speed for a target extruder
   */
  void Temperature::set_fan_speed(uint8_t target, uint16_t speed) {

    NOMORE(speed, 255U);

    #if ENABLED(SINGLENOZZLE_STANDBY_FAN)
      if (target != active_extruder) {
        if (target < EXTRUDERS) singlenozzle_fan_speed[target] = speed;
        return;
      }
    #endif

    TERN_(SINGLENOZZLE, target = 0); // Always use fan index 0 with SINGLENOZZLE

    if (target >= FAN_COUNT) return;

    fan_speed[target] = speed;

    TERN_(REPORT_FAN_CHANGE, report_fan_speed(target));
  }

  #if ENABLED(REPORT_FAN_CHANGE)
    /**
     * Report print fan speed for a target extruder
     */
    void Temperature::report_fan_speed(const uint8_t target) {
      if (target >= FAN_COUNT) return;
      PORT_REDIRECT(SERIAL_ALL);
      SERIAL_ECHOLNPAIR("M106 P", target, " S", fan_speed[target]);
    }
  #endif

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

#endif // HAS_FAN

#if WATCH_HOTENDS
  hotend_watch_t Temperature::watch_hotend[HOTENDS]; // = { { 0 } }
#endif
#if HEATER_IDLE_HANDLER
  Temperature::heater_idle_t Temperature::heater_idle[NR_HEATER_IDLE]; // = { { 0 } }
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
  TERN_(WATCH_BED, bed_watch_t Temperature::watch_bed); // = { 0 }
  IF_DISABLED(PIDTEMPBED, millis_t Temperature::next_bed_check_ms);
#endif // HAS_HEATED_BED

#if HAS_TEMP_CHAMBER
  chamber_info_t Temperature::temp_chamber; // = { 0 }
  #if HAS_HEATED_CHAMBER
    int16_t fan_chamber_pwm;
    bool flag_chamber_off;
    bool flag_chamber_excess_heat = false;
    millis_t next_cool_check_ms_2 = 0;
    float old_temp = 9999;
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

#if HAS_HOTEND
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

#if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
  uint16_t Temperature::singlenozzle_temp[EXTRUDERS];
  #if HAS_FAN
    uint8_t Temperature::singlenozzle_fan_speed[EXTRUDERS];
  #endif
#endif

#if ENABLED(PROBING_HEATERS_OFF)
  bool Temperature::paused;
#endif

// public:

#if HAS_ADC_BUTTONS
  uint32_t Temperature::current_ADCKey_raw = HAL_ADC_RANGE;
  uint16_t Temperature::ADCKey_count = 0;
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  int16_t Temperature::lpq_len; // Initialized in settings.cpp
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
  void Temperature::PID_autotune(const float &target, const heater_id_t heater_id, const int8_t ncycles, const bool set_result/*=false*/) {
    float current_temp = 0.0;
    int cycles = 0;
    bool heating = true;

    millis_t next_temp_ms = millis(), t1 = next_temp_ms, t2 = next_temp_ms;
    long t_high = 0, t_low = 0;

    long bias, d;
    PID_t tune_pid = { 0, 0, 0 };
    float maxT = 0, minT = 10000;

    const bool isbed = (heater_id == H_BED);

    #if HAS_PID_FOR_BOTH
      #define GHV(B,H) (isbed ? (B) : (H))
      #define SHV(B,H) do{ if (isbed) temp_bed.soft_pwm_amount = B; else temp_hotend[heater_id].soft_pwm_amount = H; }while(0)
      #define ONHEATINGSTART() (isbed ? printerEventLEDs.onBedHeatingStart() : printerEventLEDs.onHotendHeatingStart())
      #define ONHEATING(S,C,T) (isbed ? printerEventLEDs.onBedHeating(S,C,T) : printerEventLEDs.onHotendHeating(S,C,T))
    #elif ENABLED(PIDTEMPBED)
      #define GHV(B,H) B
      #define SHV(B,H) (temp_bed.soft_pwm_amount = B)
      #define ONHEATINGSTART() printerEventLEDs.onBedHeatingStart()
      #define ONHEATING(S,C,T) printerEventLEDs.onBedHeating(S,C,T)
    #else
      #define GHV(B,H) H
      #define SHV(B,H) (temp_hotend[heater_id].soft_pwm_amount = H)
      #define ONHEATINGSTART() printerEventLEDs.onHotendHeatingStart()
      #define ONHEATING(S,C,T) printerEventLEDs.onHotendHeating(S,C,T)
    #endif
    #define WATCH_PID BOTH(WATCH_BED, PIDTEMPBED) || BOTH(WATCH_HOTENDS, PIDTEMP)

    #if WATCH_PID
      #if ALL(THERMAL_PROTECTION_HOTENDS, PIDTEMP, THERMAL_PROTECTION_BED, PIDTEMPBED)
        #define GTV(B,H) (isbed ? (B) : (H))
      #elif BOTH(THERMAL_PROTECTION_HOTENDS, PIDTEMP)
        #define GTV(B,H) (H)
      #else
        #define GTV(B,H) (B)
      #endif
      const uint16_t watch_temp_period = GTV(WATCH_BED_TEMP_PERIOD, WATCH_TEMP_PERIOD);
      const uint8_t watch_temp_increase = GTV(WATCH_BED_TEMP_INCREASE, WATCH_TEMP_INCREASE);
      const float watch_temp_target = target - float(watch_temp_increase + GTV(TEMP_BED_HYSTERESIS, TEMP_HYSTERESIS) + 1);
      millis_t temp_change_ms = next_temp_ms + SEC_TO_MS(watch_temp_period);
      float next_watch_temp = 0.0;
      bool heated = false;
    #endif

    TERN_(HAS_AUTO_FAN, next_auto_fan_check_ms = next_temp_ms + 2500UL);

    if (target > GHV(BED_MAX_TARGET, temp_range[heater_id].maxtemp - HOTEND_OVERSHOOT)) {
      SERIAL_ECHOLNPGM(STR_PID_TEMP_TOO_HIGH);
      TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_TEMP_TOO_HIGH));
      return;
    }

    SERIAL_ECHOLNPGM(STR_PID_AUTOTUNE_START);

    disable_all_heaters();
    TERN_(AUTO_POWER_CONTROL, powerManager.power_on());

    SHV(bias = d = (MAX_BED_POWER) >> 1, bias = d = (PID_MAX) >> 1);

    #if ENABLED(PRINTER_EVENT_LEDS)
      const float start_temp = GHV(temp_bed.celsius, temp_hotend[heater_id].celsius);
      LEDColor color = ONHEATINGSTART();
    #endif

    TERN_(NO_FAN_SLOWING_IN_PID_TUNING, adaptive_fan_slowing = false);

    // PID Tuning loop
    wait_for_heatup = true; // Can be interrupted with M108
    while (wait_for_heatup) {

      const millis_t ms = millis();

      if (raw_temps_ready) { // temp sample ready
        updateTemperaturesFromRawValues();

        // Get the current temperature and constrain it
        current_temp = GHV(temp_bed.celsius, temp_hotend[heater_id].celsius);
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
        TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_TEMP_TOO_HIGH));
        break;
      }

      // Report heater states every 2 seconds
      if (ELAPSED(ms, next_temp_ms)) {
        #if HAS_TEMP_SENSOR
          print_heater_states(isbed ? active_extruder : heater_id);
          SERIAL_EOL();
        #endif
        next_temp_ms = ms + 2000UL;

        // Make sure heating is actually working
        #if WATCH_PID
          if (BOTH(WATCH_BED, WATCH_HOTENDS) || isbed == DISABLED(WATCH_HOTENDS)) {
            if (!heated) {                                            // If not yet reached target...
              if (current_temp > next_watch_temp) {                   // Over the watch temp?
                next_watch_temp = current_temp + watch_temp_increase; // - set the next temp to watch for
                temp_change_ms = ms + SEC_TO_MS(watch_temp_period);     // - move the expiration timer up
                if (current_temp > watch_temp_target) heated = true;  // - Flag if target temperature reached
              }
              else if (ELAPSED(ms, temp_change_ms))                   // Watch timer expired
                _temp_error(heater_id, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
            }
            else if (current_temp < target - (MAX_OVERSHOOT_PID_AUTOTUNE)) // Heated, then temperature fell too far?
              _temp_error(heater_id, str_t_thermal_runaway, GET_TEXT(MSG_THERMAL_RUNAWAY));
          }
        #endif
      } // every 2 seconds

      // Timeout after MAX_CYCLE_TIME_PID_AUTOTUNE minutes since the last undershoot/overshoot cycle
      #ifndef MAX_CYCLE_TIME_PID_AUTOTUNE
        #define MAX_CYCLE_TIME_PID_AUTOTUNE 20L
      #endif
      if ((ms - _MIN(t1, t2)) > (MAX_CYCLE_TIME_PID_AUTOTUNE * 60L * 1000L)) {
        TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
        TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_TUNING_TIMEOUT));
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
          PID_PARAM(Kp, heater_id) = tune_pid.Kp; \
          PID_PARAM(Ki, heater_id) = scalePID_i(tune_pid.Ki); \
          PID_PARAM(Kd, heater_id) = scalePID_d(tune_pid.Kd); \
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

        TERN_(PRINTER_EVENT_LEDS, printerEventLEDs.onPidTuningDone(color));

        TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_DONE));

        goto EXIT_M303;
      }

      // Run HAL idle tasks
      TERN_(HAL_IDLETASK, HAL_idletask());

      // Run UI update
      TERN(DWIN_CREALITY_LCD, DWIN_Update(), ui.update());
    }
    wait_for_heatup = false;

    disable_all_heaters();

    TERN_(PRINTER_EVENT_LEDS, printerEventLEDs.onPidTuningDone(color));

    TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_DONE));

    EXIT_M303:
      TERN_(NO_FAN_SLOWING_IN_PID_TUNING, adaptive_fan_slowing = true);
      return;
  }

#endif // HAS_PID_HEATING

/**
 * Class and Instance Methods
 */

int16_t Temperature::getHeaterPower(const heater_id_t heater_id) {
  switch (heater_id) {
    #if HAS_HEATED_BED
      case H_BED: return temp_bed.soft_pwm_amount;
    #endif
    #if HAS_HEATED_CHAMBER
      case H_CHAMBER: return temp_chamber.soft_pwm_amount;
    #endif
    default:
      return TERN0(HAS_HOTEND, temp_hotend[heater_id].soft_pwm_amount);
  }
}

#define _EFANOVERLAP(A,B) _FANOVERLAP(E##A,B)

#if HAS_AUTO_FAN

  #define CHAMBER_FAN_INDEX HOTENDS

  void Temperature::checkExtruderAutoFans() {
    #define _EFAN(B,A) _EFANOVERLAP(A,B) ? B :
    static const uint8_t fanBit[] PROGMEM = {
      0
      #if HAS_MULTI_HOTEND
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
    LOOP_L_N(f, COUNT(fanBit)) {
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

inline void loud_kill(PGM_P const lcd_msg, const heater_id_t heater_id) {
  marlin_state = MF_KILLED;
  #if USE_BEEPER
    thermalManager.disable_all_heaters();
    for (uint8_t i = 20; i--;) {
      WRITE(BEEPER_PIN, HIGH);
      delay(25);
      watchdog_refresh();
      WRITE(BEEPER_PIN, LOW);
      delay(40);
      watchdog_refresh();
      delay(40);
      watchdog_refresh();
    }
    WRITE(BEEPER_PIN, HIGH);
  #endif
  kill(lcd_msg, HEATER_PSTR(heater_id));
}

void Temperature::_temp_error(const heater_id_t heater_id, PGM_P const serial_msg, PGM_P const lcd_msg) {

  static uint8_t killed = 0;

  if (IsRunning() && TERN1(BOGUS_TEMPERATURE_GRACE_PERIOD, killed == 2)) {
    SERIAL_ERROR_START();
    serialprintPGM(serial_msg);
    SERIAL_ECHOPGM(STR_STOPPED_HEATER);
    if (heater_id >= 0)
      SERIAL_ECHO((int)heater_id);
    else if (TERN0(HAS_HEATED_CHAMBER, heater_id == H_CHAMBER))
      SERIAL_ECHOPGM(STR_HEATER_CHAMBER);
    else
      SERIAL_ECHOPGM(STR_HEATER_BED);
    SERIAL_EOL();
  }

  disable_all_heaters(); // always disable (even for bogus temp)
  watchdog_refresh();

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
        loud_kill(lcd_msg, heater_id);
        ++killed;
        break;
    }
  #elif defined(BOGUS_TEMPERATURE_GRACE_PERIOD)
    UNUSED(killed);
  #else
    if (!killed) { killed = 1; loud_kill(lcd_msg, heater_id); }
  #endif
}

void Temperature::max_temp_error(const heater_id_t heater_id) {
  #if ENABLED(DWIN_CREALITY_LCD) && (HAS_HOTEND || HAS_HEATED_BED)
    DWIN_Popup_Temperature(1);
  #endif
  _temp_error(heater_id, PSTR(STR_T_MAXTEMP), GET_TEXT(MSG_ERR_MAXTEMP));
}

void Temperature::min_temp_error(const heater_id_t heater_id) {
  #if ENABLED(DWIN_CREALITY_LCD) && (HAS_HOTEND || HAS_HEATED_BED)
    DWIN_Popup_Temperature(0);
  #endif
  _temp_error(heater_id, PSTR(STR_T_MINTEMP), GET_TEXT(MSG_ERR_MINTEMP));
}

#if HAS_HOTEND
  #if ENABLED(PID_DEBUG)
    extern bool pid_debug_flag;
  #endif

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
          || TERN0(HEATER_IDLE_HANDLER, heater_idle[ee].timed_out)
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
            if (fan_speed[active_extruder] > PID_FAN_SCALING_MIN_SPEED) {
              work_pid[ee].Kf = PID_PARAM(Kf, ee) + (PID_FAN_SCALING_LIN_FACTOR) * fan_speed[active_extruder];
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
        if (ee == active_extruder && pid_debug_flag) {
          SERIAL_ECHO_START();
          SERIAL_ECHOPAIR(STR_PID_DEBUG, ee, STR_PID_DEBUG_INPUT, temp_hotend[ee].celsius, STR_PID_DEBUG_OUTPUT, pid_output);
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

      const bool is_idling = TERN0(HEATER_IDLE_HANDLER, heater_idle[ee].timed_out);
      const float pid_output = (!is_idling && temp_hotend[ee].celsius < temp_hotend[ee].target) ? BANG_MAX : 0;

    #endif

    return pid_output;
  }

#endif // HAS_HOTEND

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

  #if ENABLED(EMERGENCY_PARSER)
    if (emergency_parser.killed_by_M112) kill(M112_KILL_STR, nullptr, true);

    if (emergency_parser.quickstop_by_M410) {
      emergency_parser.quickstop_by_M410 = false; // quickstop_stepper may call idle so clear this now!
      quickstop_stepper();
    }
  #endif

  if (!raw_temps_ready) return;

  updateTemperaturesFromRawValues(); // also resets the watchdog

  #if DISABLED(IGNORE_THERMOCOUPLE_ERRORS)
    #if HEATER_0_USES_MAX6675
      if (temp_hotend[0].celsius > _MIN(HEATER_0_MAXTEMP, HEATER_0_MAX6675_TMAX - 1.0)) max_temp_error(H_E0);
      if (temp_hotend[0].celsius < _MAX(HEATER_0_MINTEMP, HEATER_0_MAX6675_TMIN + .01)) min_temp_error(H_E0);
    #endif
    #if HEATER_1_USES_MAX6675
      if (temp_hotend[1].celsius > _MIN(HEATER_1_MAXTEMP, HEATER_1_MAX6675_TMAX - 1.0)) max_temp_error(H_E1);
      if (temp_hotend[1].celsius < _MAX(HEATER_1_MINTEMP, HEATER_1_MAX6675_TMIN + .01)) min_temp_error(H_E1);
    #endif
  #endif

  millis_t ms = millis();

  #if HAS_HOTEND

    HOTEND_LOOP() {
      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        if (degHotend(e) > temp_range[e].maxtemp) max_temp_error((heater_id_t)e);
      #endif

      TERN_(HEATER_IDLE_HANDLER, heater_idle[e].update(ms));

      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        // Check for thermal runaway
        tr_state_machine[e].run(temp_hotend[e].celsius, temp_hotend[e].target, (heater_id_t)e, THERMAL_PROTECTION_PERIOD, THERMAL_PROTECTION_HYSTERESIS);
      #endif

      temp_hotend[e].soft_pwm_amount = (temp_hotend[e].celsius > temp_range[e].mintemp || is_preheating(e)) && temp_hotend[e].celsius < temp_range[e].maxtemp ? (int)get_pid_output_hotend(e) >> 1 : 0;

      #if WATCH_HOTENDS
        // Make sure temperature is increasing
        if (watch_hotend[e].next_ms && ELAPSED(ms, watch_hotend[e].next_ms)) {  // Time to check this extruder?
          if (degHotend(e) < watch_hotend[e].target) {                          // Failed to increase enough?
            TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
            _temp_error((heater_id_t)e, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
          }
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

  #endif // HAS_HOTEND

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
      if (degBed() > BED_MAXTEMP) max_temp_error(H_BED);
    #endif

    #if WATCH_BED
      // Make sure temperature is increasing
      if (watch_bed.elapsed(ms)) {        // Time to check the bed?
        if (degBed() < watch_bed.target) {                              // Failed to increase enough?
          TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
          _temp_error(H_BED, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
        }
        else                                                            // Start again if the target is still far off
          start_watching_bed();
      }
    #endif // WATCH_BED

    #if BOTH(PROBING_HEATERS_OFF, BED_LIMIT_SWITCHING)
      #define PAUSE_CHANGE_REQD 1
    #endif

    #if PAUSE_CHANGE_REQD
      static bool last_pause_state;
    #endif

    do {

      #if DISABLED(PIDTEMPBED)
        if (PENDING(ms, next_bed_check_ms)
          && TERN1(PAUSE_CHANGE_REQD, paused == last_pause_state)
        ) break;
        next_bed_check_ms = ms + BED_CHECK_INTERVAL;
        TERN_(PAUSE_CHANGE_REQD, last_pause_state = paused);
      #endif

      TERN_(HEATER_IDLE_HANDLER, heater_idle[IDLE_INDEX_BED].update(ms));

      #if HAS_THERMALLY_PROTECTED_BED
        tr_state_machine[RUNAWAY_IND_BED].run(temp_bed.celsius, temp_bed.target, H_BED, THERMAL_PROTECTION_BED_PERIOD, THERMAL_PROTECTION_BED_HYSTERESIS);
      #endif

      #if HEATER_IDLE_HANDLER
        if (heater_idle[IDLE_INDEX_BED].timed_out) {
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
      if (degChamber() > CHAMBER_MAXTEMP) max_temp_error(H_CHAMBER);
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

    #if EITHER(CHAMBER_FAN, CHAMBER_VENT)
      if (temp_chamber.target > CHAMBER_MINTEMP) {
        flag_chamber_off = false;

        #if ENABLED(CHAMBER_FAN)
          #if CHAMBER_FAN_MODE == 0
            fan_chamber_pwm = CHAMBER_FAN_BASE;
          #elif CHAMBER_FAN_MODE == 1
            fan_chamber_pwm = (temp_chamber.celsius > temp_chamber.target) ? (CHAMBER_FAN_BASE) + (CHAMBER_FAN_FACTOR) * (temp_chamber.celsius - temp_chamber.target) : 0;
          #elif CHAMBER_FAN_MODE == 2
            fan_chamber_pwm = (CHAMBER_FAN_BASE) + (CHAMBER_FAN_FACTOR) * ABS(temp_chamber.celsius - temp_chamber.target);
            if (temp_chamber.soft_pwm_amount)
              fan_chamber_pwm += (CHAMBER_FAN_FACTOR) * 2;
          #endif
          NOMORE(fan_chamber_pwm, 225);
          set_fan_speed(2, fan_chamber_pwm); // TODO: instead of fan 2, set to chamber fan
        #endif

        #if ENABLED(CHAMBER_VENT)
          #ifndef MIN_COOLING_SLOPE_TIME_CHAMBER_VENT
            #define MIN_COOLING_SLOPE_TIME_CHAMBER_VENT 20
          #endif
          #ifndef MIN_COOLING_SLOPE_DEG_CHAMBER_VENT
            #define MIN_COOLING_SLOPE_DEG_CHAMBER_VENT 1.5
          #endif
          if (!flag_chamber_excess_heat && temp_chamber.celsius - temp_chamber.target >= HIGH_EXCESS_HEAT_LIMIT) {
            // Open vent after MIN_COOLING_SLOPE_TIME_CHAMBER_VENT seconds if the
            // temperature didn't drop at least MIN_COOLING_SLOPE_DEG_CHAMBER_VENT
            if (next_cool_check_ms_2 == 0 || ELAPSED(ms, next_cool_check_ms_2)) {
              if (old_temp - temp_chamber.celsius < float(MIN_COOLING_SLOPE_DEG_CHAMBER_VENT)) flag_chamber_excess_heat = true; //the bed is heating the chamber too much
              next_cool_check_ms_2 = ms + SEC_TO_MS(MIN_COOLING_SLOPE_TIME_CHAMBER_VENT);
              old_temp = temp_chamber.celsius;
            }
          }
          else {
            next_cool_check_ms_2 = 0;
            old_temp = 9999;
          }
          if (flag_chamber_excess_heat && (temp_chamber.celsius - temp_chamber.target <= -LOW_EXCESS_HEAT_LIMIT) ) {
            flag_chamber_excess_heat = false;
          }
        #endif
      }
      else if (!flag_chamber_off) {
        #if ENABLED(CHAMBER_FAN)
          flag_chamber_off = true;
          set_fan_speed(2, 0);
        #endif
        #if ENABLED(CHAMBER_VENT)
          flag_chamber_excess_heat = false;
          MOVE_SERVO(CHAMBER_VENT_SERVO_NR, 90);
        #endif
      }
    #endif

    if (ELAPSED(ms, next_chamber_check_ms)) {
      next_chamber_check_ms = ms + CHAMBER_CHECK_INTERVAL;

      if (WITHIN(temp_chamber.celsius, CHAMBER_MINTEMP, CHAMBER_MAXTEMP)) {
        if (flag_chamber_excess_heat) {
          temp_chamber.soft_pwm_amount = 0;
          #if ENABLED(CHAMBER_VENT)
            if (!flag_chamber_off) MOVE_SERVO(CHAMBER_VENT_SERVO_NR, temp_chamber.celsius <= temp_chamber.target ? 0 : 90);
          #endif
        }
        else {
          #if ENABLED(CHAMBER_LIMIT_SWITCHING)
            if (temp_chamber.celsius >= temp_chamber.target + TEMP_CHAMBER_HYSTERESIS)
              temp_chamber.soft_pwm_amount = 0;
            else if (temp_chamber.celsius <= temp_chamber.target - (TEMP_CHAMBER_HYSTERESIS))
              temp_chamber.soft_pwm_amount = (MAX_CHAMBER_POWER) >> 1;
          #else
            temp_chamber.soft_pwm_amount = temp_chamber.celsius < temp_chamber.target ? (MAX_CHAMBER_POWER) >> 1 : 0;
          #endif
          #if ENABLED(CHAMBER_VENT)
            if (!flag_chamber_off) MOVE_SERVO(CHAMBER_VENT_SERVO_NR, 0);
          #endif
        }
      }
      else {
        temp_chamber.soft_pwm_amount = 0;
        WRITE_HEATER_CHAMBER(LOW);
      }

      #if ENABLED(THERMAL_PROTECTION_CHAMBER)
        tr_state_machine[RUNAWAY_IND_CHAMBER].run(temp_chamber.celsius, temp_chamber.target, H_CHAMBER, THERMAL_PROTECTION_CHAMBER_PERIOD, THERMAL_PROTECTION_CHAMBER_HYSTERESIS);
      #endif
    }

    // TODO: Implement true PID pwm
    //temp_bed.soft_pwm_amount = WITHIN(temp_chamber.celsius, CHAMBER_MINTEMP, CHAMBER_MAXTEMP) ? (int)get_pid_output_chamber() >> 1 : 0;

  #endif // HAS_HEATED_CHAMBER

  UNUSED(ms);
}

#define TEMP_AD595(RAW)  ((RAW) * 5.0 * 100.0 / float(HAL_ADC_RANGE) / (OVERSAMPLENR) * (TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET)
#define TEMP_AD8495(RAW) ((RAW) * 6.6 * 100.0 / float(HAL_ADC_RANGE) / (OVERSAMPLENR) * (TEMP_SENSOR_AD8495_GAIN) + TEMP_SENSOR_AD8495_OFFSET)

/**
 * Bisect search for the range of the 'raw' value, then interpolate
 * proportionally between the under and over values.
 */
#define SCAN_THERMISTOR_TABLE(TBL,LEN) do{                            \
  uint8_t l = 0, r = LEN, m;                                          \
  for (;;) {                                                          \
    m = (l + r) >> 1;                                                 \
    if (!m) return int16_t(pgm_read_word(&TBL[0].celsius));           \
    if (m == l || m == r) return int16_t(pgm_read_word(&TBL[LEN-1].celsius)); \
    int16_t v00 = pgm_read_word(&TBL[m-1].value),                     \
          v10 = pgm_read_word(&TBL[m-0].value);                       \
         if (raw < v00) r = m;                                        \
    else if (raw > v10) l = m;                                        \
    else {                                                            \
      const int16_t v01 = int16_t(pgm_read_word(&TBL[m-1].celsius)),  \
                  v11 = int16_t(pgm_read_word(&TBL[m-0].celsius));    \
      return v01 + (raw - v00) * float(v11 - v01) / float(v10 - v00); \
    }                                                                 \
  }                                                                   \
}while(0)

#if HAS_USER_THERMISTORS

  user_thermistor_t Temperature::user_thermistor[USER_THERMISTORS]; // Initialized by settings.load()

  void Temperature::reset_user_thermistors() {
    user_thermistor_t default_user_thermistor[USER_THERMISTORS] = {
      #if HEATER_0_USER_THERMISTOR
        { true, 0, 0, HOTEND0_PULLUP_RESISTOR_OHMS, HOTEND0_RESISTANCE_25C_OHMS, 0, 0, HOTEND0_BETA, 0 },
      #endif
      #if HEATER_1_USER_THERMISTOR
        { true, 0, 0, HOTEND1_PULLUP_RESISTOR_OHMS, HOTEND1_RESISTANCE_25C_OHMS, 0, 0, HOTEND1_BETA, 0 },
      #endif
      #if HEATER_2_USER_THERMISTOR
        { true, 0, 0, HOTEND2_PULLUP_RESISTOR_OHMS, HOTEND2_RESISTANCE_25C_OHMS, 0, 0, HOTEND2_BETA, 0 },
      #endif
      #if HEATER_3_USER_THERMISTOR
        { true, 0, 0, HOTEND3_PULLUP_RESISTOR_OHMS, HOTEND3_RESISTANCE_25C_OHMS, 0, 0, HOTEND3_BETA, 0 },
      #endif
      #if HEATER_4_USER_THERMISTOR
        { true, 0, 0, HOTEND4_PULLUP_RESISTOR_OHMS, HOTEND4_RESISTANCE_25C_OHMS, 0, 0, HOTEND4_BETA, 0 },
      #endif
      #if HEATER_5_USER_THERMISTOR
        { true, 0, 0, HOTEND5_PULLUP_RESISTOR_OHMS, HOTEND5_RESISTANCE_25C_OHMS, 0, 0, HOTEND5_BETA, 0 },
      #endif
      #if HEATER_6_USER_THERMISTOR
        { true, 0, 0, HOTEND6_PULLUP_RESISTOR_OHMS, HOTEND6_RESISTANCE_25C_OHMS, 0, 0, HOTEND6_BETA, 0 },
      #endif
      #if HEATER_7_USER_THERMISTOR
        { true, 0, 0, HOTEND7_PULLUP_RESISTOR_OHMS, HOTEND7_RESISTANCE_25C_OHMS, 0, 0, HOTEND7_BETA, 0 },
      #endif
      #if HEATER_BED_USER_THERMISTOR
        { true, 0, 0, BED_PULLUP_RESISTOR_OHMS, BED_RESISTANCE_25C_OHMS, 0, 0, BED_BETA, 0 },
      #endif
      #if HEATER_CHAMBER_USER_THERMISTOR
        { true, 0, 0, CHAMBER_PULLUP_RESISTOR_OHMS, CHAMBER_RESISTANCE_25C_OHMS, 0, 0, CHAMBER_BETA, 0 }
      #endif
    };
    COPY(user_thermistor, default_user_thermistor);
  }

  void Temperature::log_user_thermistor(const uint8_t t_index, const bool eprom/*=false*/) {

    if (eprom)
      SERIAL_ECHOPGM("  M305 ");
    else
      SERIAL_ECHO_START();
    SERIAL_CHAR('P', '0' + t_index);

    const user_thermistor_t &t = user_thermistor[t_index];

    SERIAL_ECHOPAIR_F(" R", t.series_res, 1);
    SERIAL_ECHOPAIR_F_P(SP_T_STR, t.res_25, 1);
    SERIAL_ECHOPAIR_F_P(SP_B_STR, t.beta, 1);
    SERIAL_ECHOPAIR_F_P(SP_C_STR, t.sh_c_coeff, 9);
    SERIAL_ECHOPGM(" ; ");
    serialprintPGM(
      TERN_(HEATER_0_USER_THERMISTOR, t_index == CTI_HOTEND_0 ? PSTR("HOTEND 0") :)
      TERN_(HEATER_1_USER_THERMISTOR, t_index == CTI_HOTEND_1 ? PSTR("HOTEND 1") :)
      TERN_(HEATER_2_USER_THERMISTOR, t_index == CTI_HOTEND_2 ? PSTR("HOTEND 2") :)
      TERN_(HEATER_3_USER_THERMISTOR, t_index == CTI_HOTEND_3 ? PSTR("HOTEND 3") :)
      TERN_(HEATER_4_USER_THERMISTOR, t_index == CTI_HOTEND_4 ? PSTR("HOTEND 4") :)
      TERN_(HEATER_5_USER_THERMISTOR, t_index == CTI_HOTEND_5 ? PSTR("HOTEND 5") :)
      TERN_(HEATER_6_USER_THERMISTOR, t_index == CTI_HOTEND_6 ? PSTR("HOTEND 6") :)
      TERN_(HEATER_7_USER_THERMISTOR, t_index == CTI_HOTEND_7 ? PSTR("HOTEND 7") :)
      TERN_(HEATER_BED_USER_THERMISTOR, t_index == CTI_BED ? PSTR("BED") :)
      TERN_(HEATER_CHAMBER_USER_THERMISTOR, t_index == CTI_CHAMBER ? PSTR("CHAMBER") :)
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

#if HAS_HOTEND
  // Derived from RepRap FiveD extruder::getTemperature()
  // For hot end temperature measurement.
  float Temperature::analog_to_celsius_hotend(const int raw, const uint8_t e) {
      if (e > HOTENDS - DISABLED(TEMP_SENSOR_1_AS_REDUNDANT)) {
        SERIAL_ERROR_START();
        SERIAL_ECHO((int)e);
        SERIAL_ECHOLNPGM(STR_INVALID_EXTRUDER_NUM);
        kill();
        return 0;
      }

    switch (e) {
      case 0:
        #if HEATER_0_USER_THERMISTOR
          return user_thermistor_to_deg_c(CTI_HOTEND_0, raw);
        #elif HEATER_0_USES_MAX6675
          return TERN(MAX6675_0_IS_MAX31865, max31865_0.temperature(MAX31865_SENSOR_OHMS_0, MAX31865_CALIBRATION_OHMS_0), raw * 0.25);
        #elif HEATER_0_USES_AD595
          return TEMP_AD595(raw);
        #elif HEATER_0_USES_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 1:
        #if HEATER_1_USER_THERMISTOR
          return user_thermistor_to_deg_c(CTI_HOTEND_1, raw);
        #elif HEATER_1_USES_MAX6675
          return TERN(MAX6675_1_IS_MAX31865, max31865_1.temperature(MAX31865_SENSOR_OHMS_1, MAX31865_CALIBRATION_OHMS_1), raw * 0.25);
        #elif HEATER_1_USES_AD595
          return TEMP_AD595(raw);
        #elif HEATER_1_USES_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 2:
        #if HEATER_2_USER_THERMISTOR
          return user_thermistor_to_deg_c(CTI_HOTEND_2, raw);
        #elif HEATER_2_USES_AD595
          return TEMP_AD595(raw);
        #elif HEATER_2_USES_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 3:
        #if HEATER_3_USER_THERMISTOR
          return user_thermistor_to_deg_c(CTI_HOTEND_3, raw);
        #elif HEATER_3_USES_AD595
          return TEMP_AD595(raw);
        #elif HEATER_3_USES_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 4:
        #if HEATER_4_USER_THERMISTOR
          return user_thermistor_to_deg_c(CTI_HOTEND_4, raw);
        #elif HEATER_4_USES_AD595
          return TEMP_AD595(raw);
        #elif HEATER_4_USES_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 5:
        #if HEATER_5_USER_THERMISTOR
          return user_thermistor_to_deg_c(CTI_HOTEND_5, raw);
        #elif HEATER_5_USES_AD595
          return TEMP_AD595(raw);
        #elif HEATER_5_USES_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 6:
        #if HEATER_6_USER_THERMISTOR
          return user_thermistor_to_deg_c(CTI_HOTEND_6, raw);
        #elif HEATER_6_USES_AD595
          return TEMP_AD595(raw);
        #elif HEATER_6_USES_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 7:
        #if HEATER_7_USER_THERMISTOR
          return user_thermistor_to_deg_c(CTI_HOTEND_7, raw);
        #elif HEATER_7_USES_AD595
          return TEMP_AD595(raw);
        #elif HEATER_7_USES_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      default: break;
    }

    #if HAS_HOTEND_THERMISTOR
      // Thermistor with conversion table?
      const temp_entry_t(*tt)[] = (temp_entry_t(*)[])(heater_ttbl_map[e]);
      SCAN_THERMISTOR_TABLE((*tt), heater_ttbllen_map[e]);
    #endif

    return 0;
  }
#endif // HAS_HOTEND

#if HAS_HEATED_BED
  // Derived from RepRap FiveD extruder::getTemperature()
  // For bed temperature measurement.
  float Temperature::analog_to_celsius_bed(const int raw) {
    #if HEATER_BED_USER_THERMISTOR
      return user_thermistor_to_deg_c(CTI_BED, raw);
    #elif HEATER_BED_USES_THERMISTOR
      SCAN_THERMISTOR_TABLE(BED_TEMPTABLE, BED_TEMPTABLE_LEN);
    #elif HEATER_BED_USES_AD595
      return TEMP_AD595(raw);
    #elif HEATER_BED_USES_AD8495
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
    #if HEATER_CHAMBER_USER_THERMISTOR
      return user_thermistor_to_deg_c(CTI_CHAMBER, raw);
    #elif HEATER_CHAMBER_USES_THERMISTOR
      SCAN_THERMISTOR_TABLE(CHAMBER_TEMPTABLE, CHAMBER_TEMPTABLE_LEN);
    #elif HEATER_CHAMBER_USES_AD595
      return TEMP_AD595(raw);
    #elif HEATER_CHAMBER_USES_AD8495
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
    #if HEATER_PROBE_USER_THERMISTOR
      return user_thermistor_to_deg_c(CTI_PROBE, raw);
    #elif HEATER_PROBE_USES_THERMISTOR
      SCAN_THERMISTOR_TABLE(PROBE_TEMPTABLE, PROBE_TEMPTABLE_LEN);
    #elif HEATER_PROBE_USES_AD595
      return TEMP_AD595(raw);
    #elif HEATER_PROBE_USES_AD8495
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
  TERN_(HEATER_0_USES_MAX6675, temp_hotend[0].raw = READ_MAX6675(0));
  TERN_(HEATER_1_USES_MAX6675, temp_hotend[1].raw = READ_MAX6675(1));
  #if HAS_HOTEND
    HOTEND_LOOP() temp_hotend[e].celsius = analog_to_celsius_hotend(temp_hotend[e].raw, e);
  #endif
  TERN_(HAS_HEATED_BED, temp_bed.celsius = analog_to_celsius_bed(temp_bed.raw));
  TERN_(HAS_TEMP_CHAMBER, temp_chamber.celsius = analog_to_celsius_chamber(temp_chamber.raw));
  TERN_(HAS_TEMP_PROBE, temp_probe.celsius = analog_to_celsius_probe(temp_probe.raw));
  TERN_(TEMP_SENSOR_1_AS_REDUNDANT, redundant_temperature = analog_to_celsius_hotend(redundant_temperature_raw, 1));
  TERN_(FILAMENT_WIDTH_SENSOR, filwidth.update_measured_mm());
  TERN_(HAS_POWER_MONITOR, power_monitor.capture_values());

  // Reset the watchdog on good temperature measurement
  watchdog_refresh();

  raw_temps_ready = false;
}

#if MAX6675_SEPARATE_SPI
  template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin> SoftSPI<MisoPin, MosiPin, SckPin> SPIclass<MisoPin, MosiPin, SckPin>::softSPI;
  SPIclass<MAX6675_DO_PIN, SD_MOSI_PIN, MAX6675_SCK_PIN> max6675_spi;
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
  #define INIT_E_AUTO_FAN_PIN(P) do{ if (P == FAN1_PIN || P == FAN2_PIN) { SET_PWM(P); SET_FAST_PWM_FREQ(P); } else SET_OUTPUT(P); }while(0)
#else
  #define INIT_E_AUTO_FAN_PIN(P) SET_OUTPUT(P)
#endif
#if CHAMBER_AUTO_FAN_SPEED != 255
  #define INIT_CHAMBER_AUTO_FAN_PIN(P) do{ if (P == FAN1_PIN || P == FAN2_PIN) { SET_PWM(P); SET_FAST_PWM_FREQ(P); } else SET_OUTPUT(P); }while(0)
#else
  #define INIT_CHAMBER_AUTO_FAN_PIN(P) SET_OUTPUT(P)
#endif

/**
 * Initialize the temperature manager
 * The manager is implemented by periodic calls to manage_heater()
 */
void Temperature::init() {

  TERN_(MAX6675_0_IS_MAX31865, max31865_0.begin(MAX31865_2WIRE)); // MAX31865_2WIRE, MAX31865_3WIRE, MAX31865_4WIRE
  TERN_(MAX6675_1_IS_MAX31865, max31865_1.begin(MAX31865_2WIRE));

  #if EARLY_WATCHDOG
    // Flag that the thermalManager should be running
    if (inited) return;
    inited = true;
  #endif

  #if MB(RUMBA)
    // Disable RUMBA JTAG in case the thermocouple extension is plugged on top of JTAG connector
    #define _AD(N) (HEATER_##N##_USES_AD595 || HEATER_##N##_USES_AD8495)
    #if _AD(0) || _AD(1) || _AD(2) || _AD(BED) || _AD(CHAMBER)
      MCUCR = _BV(JTD);
      MCUCR = _BV(JTD);
    #endif
  #endif

  // Thermistor activation by MCU pin
  #if PIN_EXISTS(TEMP_0_TR_ENABLE_PIN)
    OUT_WRITE(TEMP_0_TR_ENABLE_PIN, ENABLED(HEATER_0_USES_MAX6675));
  #endif
  #if PIN_EXISTS(TEMP_1_TR_ENABLE_PIN)
    OUT_WRITE(TEMP_1_TR_ENABLE_PIN, ENABLED(HEATER_1_USES_MAX6675));
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

  TERN_(MAX6675_SEPARATE_SPI, max6675_spi.init());

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
  #if HAS_TEMP_ADC_BED
    HAL_ANALOG_SELECT(TEMP_BED_PIN);
  #endif
  #if HAS_TEMP_ADC_CHAMBER
    HAL_ANALOG_SELECT(TEMP_CHAMBER_PIN);
  #endif
  #if HAS_TEMP_ADC_PROBE
    HAL_ANALOG_SELECT(TEMP_PROBE_PIN);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    HAL_ANALOG_SELECT(FILWIDTH_PIN);
  #endif
  #if HAS_ADC_BUTTONS
    HAL_ANALOG_SELECT(ADC_KEYPAD_PIN);
  #endif
  #if ENABLED(POWER_MONITOR_CURRENT)
    HAL_ANALOG_SELECT(POWER_MONITOR_CURRENT_PIN);
  #endif
  #if ENABLED(POWER_MONITOR_VOLTAGE)
    HAL_ANALOG_SELECT(POWER_MONITOR_VOLTAGE_PIN);
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

  #if HAS_HOTEND

    #define _TEMP_MIN_E(NR) do{ \
      const int16_t tmin = _MAX(HEATER_ ##NR## _MINTEMP, TERN(HEATER_##NR##_USER_THERMISTOR, 0, (int16_t)pgm_read_word(&HEATER_ ##NR## _TEMPTABLE[HEATER_ ##NR## _SENSOR_MINTEMP_IND].celsius))); \
      temp_range[NR].mintemp = tmin; \
      while (analog_to_celsius_hotend(temp_range[NR].raw_min, NR) < tmin) \
        temp_range[NR].raw_min += TEMPDIR(NR) * (OVERSAMPLENR); \
    }while(0)
    #define _TEMP_MAX_E(NR) do{ \
      const int16_t tmax = _MIN(HEATER_ ##NR## _MAXTEMP, TERN(HEATER_##NR##_USER_THERMISTOR, 2000, (int16_t)pgm_read_word(&HEATER_ ##NR## _TEMPTABLE[HEATER_ ##NR## _SENSOR_MAXTEMP_IND].celsius) - 1)); \
      temp_range[NR].maxtemp = tmax; \
      while (analog_to_celsius_hotend(temp_range[NR].raw_max, NR) > tmax) \
        temp_range[NR].raw_max -= TEMPDIR(NR) * (OVERSAMPLENR); \
    }while(0)

    #define _MINMAX_TEST(N,M) (HOTENDS > N && THERMISTOR_HEATER_##N && THERMISTOR_HEATER_##N != 998 && THERMISTOR_HEATER_##N != 999 && defined(HEATER_##N##_##M##TEMP))

    #if _MINMAX_TEST(0, MIN)
      _TEMP_MIN_E(0);
    #endif
    #if _MINMAX_TEST(0, MAX)
      _TEMP_MAX_E(0);
    #endif
    #if _MINMAX_TEST(1, MIN)
      _TEMP_MIN_E(1);
    #endif
    #if _MINMAX_TEST(1, MAX)
      _TEMP_MAX_E(1);
    #endif
    #if _MINMAX_TEST(2, MIN)
      _TEMP_MIN_E(2);
    #endif
    #if _MINMAX_TEST(2, MAX)
      _TEMP_MAX_E(2);
    #endif
    #if _MINMAX_TEST(3, MIN)
      _TEMP_MIN_E(3);
    #endif
    #if _MINMAX_TEST(3, MAX)
      _TEMP_MAX_E(3);
    #endif
    #if _MINMAX_TEST(4, MIN)
      _TEMP_MIN_E(4);
    #endif
    #if _MINMAX_TEST(4, MAX)
      _TEMP_MAX_E(4);
    #endif
    #if _MINMAX_TEST(5, MIN)
      _TEMP_MIN_E(5);
    #endif
    #if _MINMAX_TEST(5, MAX)
      _TEMP_MAX_E(5);
    #endif
    #if _MINMAX_TEST(6, MIN)
      _TEMP_MIN_E(6);
    #endif
    #if _MINMAX_TEST(6, MAX)
      _TEMP_MAX_E(6);
    #endif
    #if _MINMAX_TEST(7, MIN)
      _TEMP_MIN_E(7);
    #endif
    #if _MINMAX_TEST(7, MAX)
      _TEMP_MAX_E(7);
    #endif

  #endif // HAS_HOTEND

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

  TERN_(PROBING_HEATERS_OFF, paused = false);
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

  Temperature::tr_state_machine_t Temperature::tr_state_machine[NR_HEATER_RUNAWAY]; // = { { TRInactive, 0 } };

  /**
   * @brief Thermal Runaway state machine for a single heater
   * @param current          current measured temperature
   * @param target           current target temperature
   * @param heater_id        extruder index
   * @param period_seconds   missed temperature allowed time
   * @param hysteresis_degc  allowed distance from target
   *
   * TODO: Embed the last 3 parameters during init, if not less optimal
   */
  void Temperature::tr_state_machine_t::run(const float &current, const float &target, const heater_id_t heater_id, const uint16_t period_seconds, const uint16_t hysteresis_degc) {

    #if HEATER_IDLE_HANDLER
      // Convert the given heater_id_t to an idle array index
      const IdleIndex idle_index = idle_index_for_id(heater_id);
    #endif

    /**
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("Thermal Runaway Running. Heater ID: ");
      switch (heater_id) {
        case H_BED:     SERIAL_ECHOPGM("bed"); break;
        case H_CHAMBER: SERIAL_ECHOPGM("chamber"); break;
        default:        SERIAL_ECHO(heater_id);
      }
      SERIAL_ECHOLNPAIR(
        " ; sizeof(running_temp):", sizeof(running_temp),
        " ;  State:", state, " ;  Timer:", timer, " ;  Temperature:", current, " ;  Target Temp:", target
        #if HEATER_IDLE_HANDLER
          , " ;  Idle Timeout:", heater_idle[idle_index].timed_out
        #endif
      );
    //*/

    #if HEATER_IDLE_HANDLER
      // If the heater idle timeout expires, restart
      if (heater_idle[idle_index].timed_out) {
        state = TRInactive;
        running_temp = 0;
      }
      else
    #endif
    {
      // If the target temperature changes, restart
      if (running_temp != target) {
        running_temp = target;
        state = target > 0 ? TRFirstHeating : TRInactive;
      }
    }

    switch (state) {
      // Inactive state waits for a target temperature to be set
      case TRInactive: break;

      // When first heating, wait for the temperature to be reached then go to Stable state
      case TRFirstHeating:
        if (current < running_temp) break;
        state = TRStable;

      // While the temperature is stable watch for a bad temperature
      case TRStable:

        #if ENABLED(ADAPTIVE_FAN_SLOWING)
          if (adaptive_fan_slowing && heater_id >= 0) {
            const int fan_index = _MIN(heater_id, FAN_COUNT - 1);
            if (fan_speed[fan_index] == 0 || current >= running_temp - (hysteresis_degc * 0.25f))
              fan_speed_scaler[fan_index] = 128;
            else if (current >= running_temp - (hysteresis_degc * 0.3335f))
              fan_speed_scaler[fan_index] = 96;
            else if (current >= running_temp - (hysteresis_degc * 0.5f))
              fan_speed_scaler[fan_index] = 64;
            else if (current >= running_temp - (hysteresis_degc * 0.8f))
              fan_speed_scaler[fan_index] = 32;
            else
              fan_speed_scaler[fan_index] = 0;
          }
        #endif

        if (current >= running_temp - hysteresis_degc) {
          timer = millis() + SEC_TO_MS(period_seconds);
          break;
        }
        else if (PENDING(millis(), timer)) break;
        state = TRRunaway;

      case TRRunaway:
        TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
        _temp_error(heater_id, str_t_thermal_runaway, GET_TEXT(MSG_THERMAL_RUNAWAY));
    }
  }

#endif // HAS_THERMAL_PROTECTION

void Temperature::disable_all_heaters() {

  TERN_(AUTOTEMP, planner.autotemp_enabled = false);

  // Unpause and reset everything
  TERN_(PROBING_HEATERS_OFF, pause(false));

  #if HAS_HOTEND
    HOTEND_LOOP() {
      setTargetHotend(0, e);
      temp_hotend[e].soft_pwm_amount = 0;
    }
  #endif

  #if HAS_TEMP_HOTEND
    #define DISABLE_HEATER(N) WRITE_HEATER_##N(LOW);
    REPEAT(HOTENDS, DISABLE_HEATER);
  #endif

  #if HAS_HEATED_BED
    setTargetBed(0);
    temp_bed.soft_pwm_amount = 0;
    WRITE_HEATER_BED(LOW);
  #endif

  #if HAS_HEATED_CHAMBER
    setTargetChamber(0);
    temp_chamber.soft_pwm_amount = 0;
    WRITE_HEATER_CHAMBER(LOW);
  #endif
}

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)

  bool Temperature::auto_job_over_threshold() {
    #if HAS_HOTEND
      HOTEND_LOOP() if (degTargetHotend(e) > (EXTRUDE_MINTEMP) / 2) return true;
    #endif
    return TERN0(HAS_HEATED_BED, degTargetBed() > BED_MINTEMP)
        || TERN0(HAS_HEATED_CHAMBER, degTargetChamber() > CHAMBER_MINTEMP);
  }

  void Temperature::auto_job_check_timer(const bool can_start, const bool can_stop) {
    if (auto_job_over_threshold()) {
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
        HOTEND_LOOP() heater_idle[e].expire();    // Timeout immediately
        TERN_(HAS_HEATED_BED, heater_idle[IDLE_INDEX_BED].expire()); // Timeout immediately
      }
      else {
        HOTEND_LOOP() reset_hotend_idle_timer(e);
        TERN_(HAS_HEATED_BED, reset_bed_idle_timer());
      }
    }
  }

#endif // PROBING_HEATERS_OFF

#if ENABLED(SINGLENOZZLE_STANDBY_TEMP)

  void Temperature::singlenozzle_change(const uint8_t old_tool, const uint8_t new_tool) {
    #if HAS_FAN
      singlenozzle_fan_speed[old_tool] = fan_speed[0];
      fan_speed[0] = singlenozzle_fan_speed[new_tool];
    #endif
    singlenozzle_temp[old_tool] = temp_hotend[0].target;
    if (singlenozzle_temp[new_tool] && singlenozzle_temp[new_tool] != singlenozzle_temp[old_tool]) {
      setTargetHotend(singlenozzle_temp[new_tool], 0);
      TERN_(AUTOTEMP, planner.autotemp_update());
      TERN_(HAS_DISPLAY, set_heating_message(0));
      (void)wait_for_hotend(0, false);  // Wait for heating or cooling
    }
  }

#endif

#if HAS_MAX6675

  #ifndef THERMOCOUPLE_MAX_ERRORS
    #define THERMOCOUPLE_MAX_ERRORS 15
  #endif

  int Temperature::read_max6675(TERN_(HAS_MULTI_6675, const uint8_t hindex/*=0*/)) {
    #define MAX6675_HEAT_INTERVAL 250UL

    #if MAX6675_0_IS_MAX31855 || MAX6675_1_IS_MAX31855
      static uint32_t max6675_temp = 2000;
      #define MAX6675_ERROR_MASK    7
      #define MAX6675_DISCARD_BITS 18
      #define MAX6675_SPEED_BITS    3       // (_BV(SPR1)) // clock ÷ 64
    #elif HAS_MAX31865
      static uint16_t max6675_temp = 2000;  // From datasheet 16 bits D15-D0
      #define MAX6675_ERROR_MASK    1       // D0 Bit not used
      #define MAX6675_DISCARD_BITS  1       // Data is in D15-D1
      #define MAX6675_SPEED_BITS    3       //  (_BV(SPR1)) // clock ÷ 64
    #else
      static uint16_t max6675_temp = 2000;
      #define MAX6675_ERROR_MASK    4
      #define MAX6675_DISCARD_BITS  3
      #define MAX6675_SPEED_BITS    2       // (_BV(SPR0)) // clock ÷ 16
    #endif

    #if HAS_MULTI_6675
      // Needed to return the correct temp when this is called between readings
      static uint16_t max6675_temp_previous[COUNT_6675] = { 0 };
      #define MAX6675_TEMP(I) max6675_temp_previous[I]
      #define MAX6675_SEL(A,B) (hindex ? (B) : (A))
      #define MAX6675_WRITE(V)     do{ switch (hindex) { case 1:      WRITE(MAX6675_SS2_PIN, V); break; default:      WRITE(MAX6675_SS_PIN, V); } }while(0)
      #define MAX6675_SET_OUTPUT() do{ switch (hindex) { case 1: SET_OUTPUT(MAX6675_SS2_PIN);    break; default: SET_OUTPUT(MAX6675_SS_PIN);    } }while(0)
    #else
      constexpr uint8_t hindex = 0;
      #define MAX6675_TEMP(I) max6675_temp
      #if MAX6675_1_IS_MAX31865
        #define MAX6675_SEL(A,B) B
      #else
        #define MAX6675_SEL(A,B) A
      #endif
      #if HEATER_0_USES_MAX6675
        #define MAX6675_WRITE(V)          WRITE(MAX6675_SS_PIN, V)
        #define MAX6675_SET_OUTPUT() SET_OUTPUT(MAX6675_SS_PIN)
      #else
        #define MAX6675_WRITE(V)          WRITE(MAX6675_SS2_PIN, V)
        #define MAX6675_SET_OUTPUT() SET_OUTPUT(MAX6675_SS2_PIN)
      #endif
    #endif

    static uint8_t max6675_errors[COUNT_6675] = { 0 };

    // Return last-read value between readings
    static millis_t next_max6675_ms[COUNT_6675] = { 0 };
    millis_t ms = millis();
    if (PENDING(ms, next_max6675_ms[hindex])) return int(MAX6675_TEMP(hindex));
    next_max6675_ms[hindex] = ms + MAX6675_HEAT_INTERVAL;

    #if HAS_MAX31865
      Adafruit_MAX31865 &maxref = MAX6675_SEL(max31865_0, max31865_1);
      const uint16_t max31865_ohms = (uint32_t(maxref.readRTD()) * MAX6675_SEL(MAX31865_CALIBRATION_OHMS_0, MAX31865_CALIBRATION_OHMS_1)) >> 16;
    #endif

    //
    // TODO: spiBegin, spiRec and spiInit doesn't work when soft spi is used.
    //
    #if !MAX6675_SEPARATE_SPI
      spiBegin();
      spiInit(MAX6675_SPEED_BITS);
    #endif

    MAX6675_WRITE(LOW);  // enable TT_MAX6675
    DELAY_NS(100);       // Ensure 100ns delay

    // Read a big-endian temperature value
    max6675_temp = 0;
    for (uint8_t i = sizeof(max6675_temp); i--;) {
      max6675_temp |= TERN(MAX6675_SEPARATE_SPI, max6675_spi.receive(), spiRec());
      if (i > 0) max6675_temp <<= 8; // shift left if not the last byte
    }

    MAX6675_WRITE(HIGH); // disable TT_MAX6675

    const uint8_t fault_31865 = TERN1(HAS_MAX31865, maxref.readFault());

    if (DISABLED(IGNORE_THERMOCOUPLE_ERRORS) && (max6675_temp & MAX6675_ERROR_MASK) && fault_31865) {
      max6675_errors[hindex]++;
      if (max6675_errors[hindex] > THERMOCOUPLE_MAX_ERRORS) {
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
        #elif HAS_MAX31865
          if (fault_31865) {
            maxref.clearFault();
            SERIAL_ECHOPAIR("MAX31865 Fault :(", fault_31865, ")  >>");
            if (fault_31865 & MAX31865_FAULT_HIGHTHRESH)
              SERIAL_ECHOLNPGM("RTD High Threshold");
            else if (fault_31865 & MAX31865_FAULT_LOWTHRESH)
              SERIAL_ECHOLNPGM("RTD Low Threshold");
            else if (fault_31865 & MAX31865_FAULT_REFINLOW)
              SERIAL_ECHOLNPGM("REFIN- > 0.85 x Bias");
            else if (fault_31865 & MAX31865_FAULT_REFINHIGH)
              SERIAL_ECHOLNPGM("REFIN- < 0.85 x Bias - FORCE- open");
            else if (fault_31865 & MAX31865_FAULT_RTDINLOW)
              SERIAL_ECHOLNPGM("REFIN- < 0.85 x Bias - FORCE- open");
            else if (fault_31865 & MAX31865_FAULT_OVUV)
              SERIAL_ECHOLNPGM("Under/Over voltage");
          }
        #else
          SERIAL_ECHOLNPGM("MAX6675");
        #endif

        // Thermocouple open
        max6675_temp = 4 * MAX6675_SEL(HEATER_0_MAX6675_TMAX, HEATER_1_MAX6675_TMAX);
      }
      else
        max6675_temp >>= MAX6675_DISCARD_BITS;
    }
    else {
      max6675_temp >>= MAX6675_DISCARD_BITS;
      max6675_errors[hindex] = 0;
    }

    #if MAX6675_0_IS_MAX31855 || MAX6675_1_IS_MAX31855
      if (max6675_temp & 0x00002000) max6675_temp |= 0xFFFFC000; // Support negative temperature
    #endif

    // Return the RTD resistance for MAX31865 for display in SHOW_TEMP_ADC_VALUES
    TERN_(HAS_MAX31865, max6675_temp = max31865_ohms);

    MAX6675_TEMP(hindex) = max6675_temp;

    return int(max6675_temp);
  }

#endif // HAS_MAX6675

/**
 * Update raw temperatures
 */
void Temperature::update_raw_temperatures() {

  #if HAS_TEMP_ADC_0 && !HEATER_0_USES_MAX6675
    temp_hotend[0].update();
  #endif

  #if HAS_TEMP_ADC_1
    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      redundant_temperature_raw = temp_hotend[1].acc;
    #elif !HEATER_1_USES_MAX6675
      temp_hotend[1].update();
    #endif
  #endif

  TERN_(HAS_TEMP_ADC_2, temp_hotend[2].update());
  TERN_(HAS_TEMP_ADC_3, temp_hotend[3].update());
  TERN_(HAS_TEMP_ADC_4, temp_hotend[4].update());
  TERN_(HAS_TEMP_ADC_5, temp_hotend[5].update());
  TERN_(HAS_TEMP_ADC_6, temp_hotend[6].update());
  TERN_(HAS_TEMP_ADC_7, temp_hotend[7].update());
  TERN_(HAS_TEMP_ADC_BED, temp_bed.update());
  TERN_(HAS_TEMP_ADC_CHAMBER, temp_chamber.update());
  TERN_(HAS_TEMP_ADC_PROBE, temp_probe.update());

  TERN_(HAS_JOY_ADC_X, joystick.x.update());
  TERN_(HAS_JOY_ADC_Y, joystick.y.update());
  TERN_(HAS_JOY_ADC_Z, joystick.z.update());

  raw_temps_ready = true;
}

void Temperature::readings_ready() {

  // Update the raw values if they've been read. Else we could be updating them during reading.
  if (!raw_temps_ready) update_raw_temperatures();

  // Filament Sensor - can be read any time since IIR filtering is used
  TERN_(FILAMENT_WIDTH_SENSOR, filwidth.reading_ready());

  #if HAS_HOTEND
    HOTEND_LOOP() temp_hotend[e].reset();
    TERN_(TEMP_SENSOR_1_AS_REDUNDANT, temp_hotend[1].reset());
  #endif

  TERN_(HAS_HEATED_BED, temp_bed.reset());
  TERN_(HAS_TEMP_CHAMBER, temp_chamber.reset());
  TERN_(HAS_TEMP_PROBE, temp_probe.reset());

  TERN_(HAS_JOY_ADC_X, joystick.x.reset());
  TERN_(HAS_JOY_ADC_Y, joystick.y.reset());
  TERN_(HAS_JOY_ADC_Z, joystick.z.reset());

  #if HAS_HOTEND

    static constexpr int8_t temp_dir[] = {
      TERN(HEATER_0_USES_MAX6675, 0, TEMPDIR(0))
      #if HAS_MULTI_HOTEND
        , TERN(HEATER_1_USES_MAX6675, 0, TEMPDIR(1))
        #if HOTENDS > 2
          #define _TEMPDIR(N) , TEMPDIR(N)
          REPEAT_S(2, HOTENDS, _TEMPDIR)
        #endif
      #endif
    };

    LOOP_L_N(e, COUNT(temp_dir)) {
      const int8_t tdir = temp_dir[e];
      if (tdir) {
        const int16_t rawtemp = temp_hotend[e].raw * tdir; // normal direction, +rawtemp, else -rawtemp
        const bool heater_on = (temp_hotend[e].target > 0
          || TERN0(PIDTEMP, temp_hotend[e].soft_pwm_amount) > 0
        );
        if (rawtemp > temp_range[e].raw_max * tdir) max_temp_error((heater_id_t)e);
        if (heater_on && rawtemp < temp_range[e].raw_min * tdir && !is_preheating(e)) {
          #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
            if (++consecutive_low_temperature_error[e] >= MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED)
          #endif
              min_temp_error((heater_id_t)e);
        }
        #ifdef MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED
          else
            consecutive_low_temperature_error[e] = 0;
        #endif
      }
    }

  #endif // HAS_HOTEND

  #if ENABLED(THERMAL_PROTECTION_BED)
    #if TEMPDIR(BED) < 0
      #define BEDCMP(A,B) ((A)<(B))
    #else
      #define BEDCMP(A,B) ((A)>(B))
    #endif
    const bool bed_on = (temp_bed.target > 0) || TERN0(PIDTEMPBED, temp_bed.soft_pwm_amount > 0);
    if (BEDCMP(temp_bed.raw, maxtemp_raw_BED)) max_temp_error(H_BED);
    if (bed_on && BEDCMP(mintemp_raw_BED, temp_bed.raw)) min_temp_error(H_BED);
  #endif

  #if BOTH(HAS_HEATED_CHAMBER, THERMAL_PROTECTION_CHAMBER)
    #if TEMPDIR(CHAMBER) < 0
      #define CHAMBERCMP(A,B) ((A)<(B))
    #else
      #define CHAMBERCMP(A,B) ((A)>(B))
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

  #if HAS_HOTEND
    static SoftPWM soft_pwm_hotend[HOTENDS];
  #endif

  #if HAS_HEATED_BED
    static SoftPWM soft_pwm_bed;
  #endif

  #if HAS_HEATED_CHAMBER
    static SoftPWM soft_pwm_chamber;
  #endif

  #define WRITE_FAN(n, v) WRITE(FAN##n##_PIN, (v) ^ FAN_INVERTING)

  #if DISABLED(SLOW_PWM_HEATERS)

    #if ANY(HAS_HOTEND, HAS_HEATED_BED, HAS_HEATED_CHAMBER, FAN_SOFT_PWM)
      constexpr uint8_t pwm_mask = TERN0(SOFT_PWM_DITHER, _BV(SOFT_PWM_SCALE) - 1);
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

      #if HAS_HOTEND
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
      #if HAS_HOTEND
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

      #if HAS_HOTEND
        #define _SLOW_PWM_E(N) _SLOW_PWM(N, soft_pwm_hotend[N], temp_hotend[N]);
        REPEAT(HOTENDS, _SLOW_PWM_E);
      #endif

      #if HAS_HEATED_BED
        _SLOW_PWM(BED, soft_pwm_bed, temp_bed);
      #endif

      #if HAS_HEATED_CHAMBER
        _SLOW_PWM(CHAMBER, soft_pwm_chamber, temp_chamber);
      #endif

    } // slow_pwm_count == 0

    #if HAS_HOTEND
      #define _PWM_OFF_E(N) _PWM_OFF(N, soft_pwm_hotend[N]);
      REPEAT(HOTENDS, _PWM_OFF_E);
    #endif

    #if HAS_HEATED_BED
      _PWM_OFF(BED, soft_pwm_bed);
    #endif

    #if HAS_HEATED_CHAMBER
      _PWM_OFF(CHAMBER, soft_pwm_chamber);
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

      #if HAS_HOTEND
        HOTEND_LOOP() soft_pwm_hotend[e].dec();
      #endif
      TERN_(HAS_HEATED_BED, soft_pwm_bed.dec());
      TERN_(HAS_HEATED_CHAMBER, soft_pwm_chamber.dec());
    }

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

    #if HAS_TEMP_ADC_BED
      case PrepareTemp_BED: HAL_START_ADC(TEMP_BED_PIN); break;
      case MeasureTemp_BED: ACCUMULATE_ADC(temp_bed); break;
    #endif

    #if HAS_TEMP_ADC_CHAMBER
      case PrepareTemp_CHAMBER: HAL_START_ADC(TEMP_CHAMBER_PIN); break;
      case MeasureTemp_CHAMBER: ACCUMULATE_ADC(temp_chamber); break;
    #endif

    #if HAS_TEMP_ADC_PROBE
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
        if (!HAL_ADC_READY()) next_sensor_state = adc_sensor_state; // Redo this state
        else filwidth.accumulate(HAL_READ_ADC());
      break;
    #endif

    #if ENABLED(POWER_MONITOR_CURRENT)
      case Prepare_POWER_MONITOR_CURRENT:
        HAL_START_ADC(POWER_MONITOR_CURRENT_PIN);
        break;
      case Measure_POWER_MONITOR_CURRENT:
        if (!HAL_ADC_READY()) next_sensor_state = adc_sensor_state; // Redo this state
        else power_monitor.add_current_sample(HAL_READ_ADC());
        break;
    #endif

    #if ENABLED(POWER_MONITOR_VOLTAGE)
      case Prepare_POWER_MONITOR_VOLTAGE:
        HAL_START_ADC(POWER_MONITOR_VOLTAGE_PIN);
        break;
      case Measure_POWER_MONITOR_VOLTAGE:
        if (!HAL_ADC_READY()) next_sensor_state = adc_sensor_state; // Redo this state
        else power_monitor.add_voltage_sample(HAL_READ_ADC());
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
    , const heater_id_t e=INDEX_NONE
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
    SERIAL_CHAR(' ', k);
    #if HAS_MULTI_HOTEND
      if (e >= 0) SERIAL_CHAR('0' + e);
    #endif
    #ifdef SERIAL_FLOAT_PRECISION
      #define SFP _MIN(SERIAL_FLOAT_PRECISION, 2)
    #else
      #define SFP 2
    #endif
    SERIAL_CHAR(':');
    SERIAL_PRINT(c, SFP);
    SERIAL_ECHOPGM(" /");
    SERIAL_PRINT(t, SFP);
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
    #if HAS_MULTI_HOTEND
      HOTEND_LOOP() print_heater_state(degHotend(e), degTargetHotend(e)
        #if ENABLED(SHOW_TEMP_ADC_VALUES)
          , rawHotendTemp(e)
        #endif
        , (heater_id_t)e
      );
    #endif
    SERIAL_ECHOPAIR(" @:", getHeaterPower((heater_id_t)target_extruder));
    #if HAS_HEATED_BED
      SERIAL_ECHOPAIR(" B@:", getHeaterPower(H_BED));
    #endif
    #if HAS_HEATED_CHAMBER
      SERIAL_ECHOPAIR(" C@:", getHeaterPower(H_CHAMBER));
    #endif
    #if HAS_MULTI_HOTEND
      HOTEND_LOOP() {
        SERIAL_ECHOPAIR(" @", e);
        SERIAL_CHAR(':');
        SERIAL_ECHO(getHeaterPower((heater_id_t)e));
      }
    #endif
  }

  #if ENABLED(AUTO_REPORT_TEMPERATURES)
    Temperature::AutoReportTemp Temperature::auto_reporter;
    void Temperature::AutoReportTemp::auto_report() {
      print_heater_states(active_extruder);
      SERIAL_EOL();
    }
  #endif

  #if HAS_HOTEND && HAS_DISPLAY
    void Temperature::set_heating_message(const uint8_t e) {
      const bool heating = isHeatingHotend(e);
      ui.status_printf_P(0,
        #if HAS_MULTI_HOTEND
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

      #if ENABLED(AUTOTEMP)
        REMEMBER(1, planner.autotemp_enabled, false);
      #endif

      #if TEMP_RESIDENCY_TIME > 0
        millis_t residency_start_ms = 0;
        bool first_loop = true;
        // Loop until the temperature has stabilized
        #define TEMP_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + SEC_TO_MS(TEMP_RESIDENCY_TIME)))
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

      bool wants_to_cool = false;
      float target_temp = -1.0, old_temp = 9999.0;
      millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;
      wait_for_heatup = true;
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
              SERIAL_ECHO(long((SEC_TO_MS(TEMP_RESIDENCY_TIME) - (now - residency_start_ms)) / 1000UL));
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
            if (temp_diff < TEMP_WINDOW)
              residency_start_ms = now + (first_loop ? SEC_TO_MS(TEMP_RESIDENCY_TIME) / 3 : 0);
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
            next_cool_check_ms = now + SEC_TO_MS(MIN_COOLING_SLOPE_TIME);
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
        wait_for_heatup = false;
        #if ENABLED(DWIN_CREALITY_LCD)
          HMI_flag.heat_flag = 0;
          duration_t elapsed = print_job_timer.duration();  // print timer
          dwin_heat_time = elapsed.value;
        #else
          ui.reset_status();
        #endif
        TERN_(PRINTER_EVENT_LEDS, printerEventLEDs.onHeatingDone());
        return true;
      }

      return false;
    }

  #endif // HAS_TEMP_HOTEND

  #if HAS_HEATED_BED

    #ifndef MIN_COOLING_SLOPE_DEG_BED
      #define MIN_COOLING_SLOPE_DEG_BED 1.00
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
        #define TEMP_BED_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + SEC_TO_MS(TEMP_BED_RESIDENCY_TIME)))
      #else
        // Loop until the temperature is very close target
        #define TEMP_BED_CONDITIONS (wants_to_cool ? isCoolingBed() : isHeatingBed())
      #endif

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        KEEPALIVE_STATE(NOT_BUSY);
      #endif

      #if ENABLED(PRINTER_EVENT_LEDS)
        const float start_temp = degBed();
        printerEventLEDs.onBedHeatingStart();
      #endif

      bool wants_to_cool = false;
      float target_temp = -1, old_temp = 9999;
      millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;
      wait_for_heatup = true;
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
              SERIAL_ECHO(long((SEC_TO_MS(TEMP_BED_RESIDENCY_TIME) - (now - residency_start_ms)) / 1000UL));
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
            if (temp_diff < TEMP_BED_WINDOW)
              residency_start_ms = now + (first_loop ? SEC_TO_MS(TEMP_BED_RESIDENCY_TIME) / 3 : 0);
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
            next_cool_check_ms = now + SEC_TO_MS(MIN_COOLING_SLOPE_TIME_BED);
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

      if (wait_for_heatup) {
        wait_for_heatup = false;
        ui.reset_status();
        return true;
      }

      return false;
    }

    void Temperature::wait_for_bed_heating() {
      if (isHeatingBed()) {
        SERIAL_ECHOLNPGM("Wait for bed heating...");
        LCD_MESSAGEPGM(MSG_BED_HEATING);
        wait_for_bed();
        ui.reset_status();
      }
    }

  #endif // HAS_HEATED_BED

  #if HAS_TEMP_PROBE

    #ifndef MIN_DELTA_SLOPE_DEG_PROBE
      #define MIN_DELTA_SLOPE_DEG_PROBE 1.0
    #endif
    #ifndef MIN_DELTA_SLOPE_TIME_PROBE
      #define MIN_DELTA_SLOPE_TIME_PROBE 600
    #endif

    bool Temperature::wait_for_probe(const float target_temp, bool no_wait_for_cooling/*=true*/) {

      const bool wants_to_cool = isProbeAboveTemp(target_temp);
      const bool will_wait = !(wants_to_cool && no_wait_for_cooling);
      if (will_wait)
        SERIAL_ECHOLNPAIR("Waiting for probe to ", (wants_to_cool ? PSTR("cool down") : PSTR("heat up")), " to ", target_temp, " degrees.");

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        KEEPALIVE_STATE(NOT_BUSY);
      #endif

      float old_temp = 9999;
      millis_t next_temp_ms = 0, next_delta_check_ms = 0;
      wait_for_heatup = true;
      while (will_wait && wait_for_heatup) {

        // Print Temp Reading every 10 seconds while heating up.
        millis_t now = millis();
        if (!next_temp_ms || ELAPSED(now, next_temp_ms)) {
          next_temp_ms = now + 10000UL;
          print_heater_states(active_extruder);
          SERIAL_EOL();
        }

        idle();
        gcode.reset_stepper_timeout(); // Keep steppers powered

        // Break after MIN_DELTA_SLOPE_TIME_PROBE seconds if the temperature
        // did not drop at least MIN_DELTA_SLOPE_DEG_PROBE. This avoids waiting
        // forever as the probe is not actively heated.
        if (!next_delta_check_ms || ELAPSED(now, next_delta_check_ms)) {
          const float temp = degProbe(),
                      delta_temp = old_temp > temp ? old_temp - temp : temp - old_temp;
          if (delta_temp < float(MIN_DELTA_SLOPE_DEG_PROBE)) {
            SERIAL_ECHOLNPGM("Timed out waiting for probe temperature.");
            break;
          }
          next_delta_check_ms = now + SEC_TO_MS(MIN_DELTA_SLOPE_TIME_PROBE);
          old_temp = temp;
        }

        // Loop until the temperature is very close target
        if (!(wants_to_cool ? isProbeAboveTemp(target_temp) : isProbeBelowTemp(target_temp))) {
            SERIAL_ECHOLN(wants_to_cool ? PSTR("Cooldown") : PSTR("Heatup"));
            SERIAL_ECHOLNPGM(" complete, target probe temperature reached.");
            break;
        }
      }

      if (wait_for_heatup) {
        wait_for_heatup = false;
        ui.reset_status();
        return true;
      }
      else if (will_wait)
        SERIAL_ECHOLNPGM("Canceled wait for probe temperature.");

      return false;
    }

  #endif // HAS_TEMP_PROBE

  #if HAS_HEATED_CHAMBER

    #ifndef MIN_COOLING_SLOPE_DEG_CHAMBER
      #define MIN_COOLING_SLOPE_DEG_CHAMBER 1.50
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_CHAMBER
      #define MIN_COOLING_SLOPE_TIME_CHAMBER 120
    #endif

    bool Temperature::wait_for_chamber(const bool no_wait_for_cooling/*=true*/) {
      #if TEMP_CHAMBER_RESIDENCY_TIME > 0
        millis_t residency_start_ms = 0;
        bool first_loop = true;
        // Loop until the temperature has stabilized
        #define TEMP_CHAMBER_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + SEC_TO_MS(TEMP_CHAMBER_RESIDENCY_TIME)))
      #else
        // Loop until the temperature is very close target
        #define TEMP_CHAMBER_CONDITIONS (wants_to_cool ? isCoolingChamber() : isHeatingChamber())
      #endif

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        KEEPALIVE_STATE(NOT_BUSY);
      #endif

      bool wants_to_cool = false;
      float target_temp = -1, old_temp = 9999;
      millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;
      wait_for_heatup = true;
      do {
        // Target temperature might be changed during the loop
        if (target_temp != degTargetChamber()) {
          wants_to_cool = isCoolingChamber();
          target_temp = degTargetChamber();

          // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
          if (no_wait_for_cooling && wants_to_cool) break;
        }

        now = millis();
        if (ELAPSED(now, next_temp_ms)) { // Print Temp Reading every 1 second while heating up.
          next_temp_ms = now + 1000UL;
          print_heater_states(active_extruder);
          #if TEMP_CHAMBER_RESIDENCY_TIME > 0
            SERIAL_ECHOPGM(" W:");
            if (residency_start_ms)
              SERIAL_ECHO(long((SEC_TO_MS(TEMP_CHAMBER_RESIDENCY_TIME) - (now - residency_start_ms)) / 1000UL));
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
            if (temp_diff < TEMP_CHAMBER_WINDOW)
              residency_start_ms = now + (first_loop ? SEC_TO_MS(TEMP_CHAMBER_RESIDENCY_TIME) / 3 : 0);
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
            next_cool_check_ms = now + SEC_TO_MS(MIN_COOLING_SLOPE_TIME_CHAMBER);
            old_temp = temp;
          }
        }
      } while (wait_for_heatup && TEMP_CHAMBER_CONDITIONS);

      if (wait_for_heatup) {
        wait_for_heatup = false;
        ui.reset_status();
        return true;
      }

      return false;
    }

  #endif // HAS_HEATED_CHAMBER

#endif // HAS_TEMP_SENSOR
