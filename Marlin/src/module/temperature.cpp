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
#include "printcounter.h"

#if EITHER(HAS_COOLER, LASER_COOLANT_FLOW_METER)
  #include "../feature/cooler.h"
  #include "../feature/spindle_laser.h"
#endif

#if ENABLED(EMERGENCY_PARSER)
  #include "motion.h"
#endif

#if ENABLED(DWIN_CREALITY_LCD)
  #include "../lcd/e3v2/creality/dwin.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../feature/host_actions.h"
#endif

// MAX TC related macros
#define TEMP_SENSOR_IS_MAX(n, M) (ENABLED(TEMP_SENSOR_##n##_IS_MAX##M) || (ENABLED(TEMP_SENSOR_REDUNDANT_IS_MAX##M) && REDUNDANT_TEMP_MATCH(SOURCE, E##n)))
#define TEMP_SENSOR_IS_ANY_MAX_TC(n) (ENABLED(TEMP_SENSOR_##n##_IS_MAX_TC) || (ENABLED(TEMP_SENSOR_REDUNDANT_IS_MAX_TC) && REDUNDANT_TEMP_MATCH(SOURCE, E##n)))

// LIB_MAX6675 can be added to the build_flags in platformio.ini to use a user-defined library
// If LIB_MAX6675 is not on the build_flags then raw SPI reads will be used.
#if HAS_MAX6675 && USE_LIB_MAX6675
  #include <max6675.h>
  #define HAS_MAX6675_LIBRARY 1
#endif

// LIB_MAX31855 can be added to the build_flags in platformio.ini to use a user-defined library.
// If LIB_MAX31855 is not on the build_flags then raw SPI reads will be used.
#if HAS_MAX31855 && USE_ADAFRUIT_MAX31855
  #include <Adafruit_MAX31855.h>
  #define HAS_MAX31855_LIBRARY 1
  typedef Adafruit_MAX31855 MAX31855;
#endif

#if HAS_MAX31865
  #if USE_ADAFRUIT_MAX31865
    #include <Adafruit_MAX31865.h>
    typedef Adafruit_MAX31865 MAX31865;
  #else
    #include "../libs/MAX31865.h"
  #endif
#endif

#if HAS_MAX6675_LIBRARY || HAS_MAX31855_LIBRARY || HAS_MAX31865
  #define HAS_MAXTC_LIBRARIES 1
#endif

// If we have a MAX TC with SCK and MISO pins defined, it's either on a separate/dedicated Hardware
// SPI bus, or some pins for Software SPI. Alternate Hardware SPI buses are not supported yet, so
// your SPI options are:
//
//  1. Only CS pin(s) defined:                  Hardware SPI on the default bus (usually the SD card SPI).
//  2. CS, MISO, and SCK pins defined:          Software SPI on a separate bus, as defined by MISO, SCK.
//  3. CS, MISO, and SCK pins w/ FORCE_HW_SPI:  Hardware SPI on the default bus, ignoring MISO, SCK.
//
#if TEMP_SENSOR_IS_ANY_MAX_TC(0) && TEMP_SENSOR_0_HAS_SPI_PINS && DISABLED(TEMP_SENSOR_FORCE_HW_SPI)
    #define TEMP_SENSOR_0_USES_SW_SPI 1
#endif
#if TEMP_SENSOR_IS_ANY_MAX_TC(1) && TEMP_SENSOR_1_HAS_SPI_PINS && DISABLED(TEMP_SENSOR_FORCE_HW_SPI)
    #define TEMP_SENSOR_1_USES_SW_SPI 1
#endif

#if (TEMP_SENSOR_0_USES_SW_SPI || TEMP_SENSOR_1_USES_SW_SPI) && !HAS_MAXTC_LIBRARIES
  #include "../libs/private_spi.h"
  #define HAS_MAXTC_SW_SPI 1

  // Define pins for SPI-based sensors
  #if TEMP_SENSOR_0_USES_SW_SPI
    #define SW_SPI_SCK_PIN    TEMP_0_SCK_PIN
    #define SW_SPI_MISO_PIN   TEMP_0_MISO_PIN
    #if PIN_EXISTS(TEMP_0_MOSI)
      #define SW_SPI_MOSI_PIN TEMP_0_MOSI_PIN
    #endif
  #else
    #define SW_SPI_SCK_PIN    TEMP_1_SCK_PIN
    #define SW_SPI_MISO_PIN   TEMP_1_MISO_PIN
    #if PIN_EXISTS(TEMP_1_MOSI)
      #define SW_SPI_MOSI_PIN TEMP_1_MOSI_PIN
    #endif
  #endif
  #ifndef SW_SPI_MOSI_PIN
    #define SW_SPI_MOSI_PIN   SD_MOSI_PIN
  #endif
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  #include "stepper.h"
#endif

#if ENABLED(BABYSTEPPING) && DISABLED(INTEGRATED_BABYSTEPPING)
  #include "../feature/babystep.h"
#endif

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
  #include "servo.h"
#endif

#if ANY(TEMP_SENSOR_0_IS_THERMISTOR, TEMP_SENSOR_1_IS_THERMISTOR, TEMP_SENSOR_2_IS_THERMISTOR, TEMP_SENSOR_3_IS_THERMISTOR, \
        TEMP_SENSOR_4_IS_THERMISTOR, TEMP_SENSOR_5_IS_THERMISTOR, TEMP_SENSOR_6_IS_THERMISTOR, TEMP_SENSOR_7_IS_THERMISTOR )
  #define HAS_HOTEND_THERMISTOR 1
#endif

#if HAS_HOTEND_THERMISTOR
  #define NEXT_TEMPTABLE(N) ,TEMPTABLE_##N
  #define NEXT_TEMPTABLE_LEN(N) ,TEMPTABLE_##N##_LEN
  static const temp_entry_t* heater_ttbl_map[HOTENDS] = ARRAY_BY_HOTENDS(TEMPTABLE_0 REPEAT_S(1, HOTENDS, NEXT_TEMPTABLE));
  static constexpr uint8_t heater_ttbllen_map[HOTENDS] = ARRAY_BY_HOTENDS(TEMPTABLE_0_LEN REPEAT_S(1, HOTENDS, NEXT_TEMPTABLE_LEN));
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
#if HAS_COOLER
  #define _COOLER_PSTR(h) (h) == H_COOLER ? GET_TEXT(MSG_COOLER) :
#else
  #define _COOLER_PSTR(h)
#endif
#define _E_PSTR(h,N) ((HOTENDS) > N && (h) == N) ? PSTR(LCD_STR_E##N) :
#define HEATER_PSTR(h) _BED_PSTR(h) _CHAMBER_PSTR(h) _COOLER_PSTR(h) _E_PSTR(h,1) _E_PSTR(h,2) _E_PSTR(h,3) _E_PSTR(h,4) _E_PSTR(h,5) PSTR(LCD_STR_E0)

//
// Initialize MAX TC objects/SPI
//
#if HAS_MAX_TC

  #if HAS_MAXTC_SW_SPI
    // Initialize SoftSPI for non-lib Software SPI; Libraries take care of it themselves.
    template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin>
      SoftSPI<MisoPin, MosiPin, SckPin> SPIclass<MisoPin, MosiPin, SckPin>::softSPI;
    SPIclass<SW_SPI_MISO_PIN, SW_SPI_MOSI_PIN, SW_SPI_SCK_PIN> max_tc_spi;

  #endif

  #define MAXTC_INIT(n, M) \
      MAX##M max##M##_##n = MAX##M( \
        TEMP_##n##_CS_PIN \
        OPTARG(_MAX31865_##n##_SW, TEMP_##n##_MOSI_PIN) \
        OPTARG(TEMP_SENSOR_##n##_USES_SW_SPI, TEMP_##n##_MISO_PIN, TEMP_##n##_SCK_PIN) \
        OPTARG(LARGE_PINMAP, HIGH) \
      )

  #if HAS_MAX6675_LIBRARY
    #if TEMP_SENSOR_IS_MAX(0, 6675)
      MAXTC_INIT(0, 6675);
    #endif
    #if TEMP_SENSOR_IS_MAX(1, 6675)
      MAXTC_INIT(1, 6675);
    #endif
  #endif

  #if HAS_MAX31855_LIBRARY
    #if TEMP_SENSOR_IS_MAX(0, 31855)
      MAXTC_INIT(0, 31855);
    #endif
    #if TEMP_SENSOR_IS_MAX(1, 31855)
      MAXTC_INIT(1, 31855);
    #endif
  #endif

  // MAX31865 always uses a library, unlike '55 & 6675
  #if HAS_MAX31865
    #define _MAX31865_0_SW TEMP_SENSOR_0_USES_SW_SPI
    #define _MAX31865_1_SW TEMP_SENSOR_1_USES_SW_SPI

    #if TEMP_SENSOR_IS_MAX(0, 31865)
      MAXTC_INIT(0, 31865);
    #endif
    #if TEMP_SENSOR_IS_MAX(1, 31865)
      MAXTC_INIT(1, 31865);
    #endif

    #undef _MAX31865_0_SW
    #undef _MAX31865_1_SW
  #endif

  #undef MAXTC_INIT

#endif

/**
 * public:
 */

#if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
  bool Temperature::adaptive_fan_slowing = true;
#endif

#if HAS_HOTEND
  hotend_info_t Temperature::temp_hotend[HOTENDS];
  #define _HMT(N) HEATER_##N##_MAXTEMP,
  const celsius_t Temperature::hotend_maxtemp[HOTENDS] = ARRAY_BY_HOTENDS(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP, HEATER_5_MAXTEMP, HEATER_6_MAXTEMP, HEATER_7_MAXTEMP);
#endif

#if HAS_TEMP_REDUNDANT
  redundant_info_t Temperature::temp_redundant;
#endif

#if ENABLED(AUTO_POWER_E_FANS)
  uint8_t Temperature::autofan_speed[HOTENDS]; // = { 0 }
#endif

#if ENABLED(AUTO_POWER_CHAMBER_FAN)
  uint8_t Temperature::chamberfan_speed; // = 0
#endif

#if ENABLED(AUTO_POWER_COOLER_FAN)
  uint8_t Temperature::coolerfan_speed; // = 0
#endif

// Init fans according to whether they're native PWM or Software PWM
#ifdef BOARD_OPENDRAIN_MOSFETS
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

// HAS_FAN does not include CONTROLLER_FAN
#if HAS_FAN

  uint8_t Temperature::fan_speed[FAN_COUNT]; // = { 0 }

  #if ENABLED(EXTRA_FAN_SPEED)

    Temperature::extra_fan_t Temperature::extra_fan_speed[FAN_COUNT];

    /**
     * Handle the M106 P<fan> T<speed> command:
     *  T1       = Restore fan speed saved on the last T2
     *  T2       = Save the fan speed, then set to the last T<3-255> value
     *  T<3-255> = Set the "extra fan speed"
     */
    void Temperature::set_temp_fan_speed(const uint8_t fan, const uint16_t command_or_speed) {
      switch (command_or_speed) {
        case 1:
          set_fan_speed(fan, extra_fan_speed[fan].saved);
          break;
        case 2:
          extra_fan_speed[fan].saved = fan_speed[fan];
          set_fan_speed(fan, extra_fan_speed[fan].speed);
          break;
        default:
          extra_fan_speed[fan].speed = _MIN(command_or_speed, 255U);
          break;
      }
    }

  #endif

  #if EITHER(PROBING_FANS_OFF, ADVANCED_PAUSE_FANS_PAUSE)
    bool Temperature::fans_paused; // = false;
    uint8_t Temperature::saved_fan_speed[FAN_COUNT]; // = { 0 }
  #endif

  #if ENABLED(ADAPTIVE_FAN_SLOWING)
    uint8_t Temperature::fan_speed_scaler[FAN_COUNT] = ARRAY_N_1(FAN_COUNT, 128);
  #endif

  /**
   * Set the print fan speed for a target extruder
   */
  void Temperature::set_fan_speed(uint8_t fan, uint16_t speed) {

    NOMORE(speed, 255U);

    #if ENABLED(SINGLENOZZLE_STANDBY_FAN)
      if (fan != active_extruder) {
        if (fan < EXTRUDERS) singlenozzle_fan_speed[fan] = speed;
        return;
      }
    #endif

    TERN_(SINGLENOZZLE, if (fan < EXTRUDERS) fan = 0); // Always fan 0 for SINGLENOZZLE E fan

    if (fan >= FAN_COUNT) return;

    fan_speed[fan] = speed;
    #if REDUNDANT_PART_COOLING_FAN
      if (fan == 0) fan_speed[REDUNDANT_PART_COOLING_FAN] = speed;
    #endif

    TERN_(REPORT_FAN_CHANGE, report_fan_speed(fan));
  }

  #if ENABLED(REPORT_FAN_CHANGE)
    /**
     * Report print fan speed for a target extruder
     */
    void Temperature::report_fan_speed(const uint8_t fan) {
      if (fan >= FAN_COUNT) return;
      PORT_REDIRECT(SerialMask::All);
      SERIAL_ECHOLNPAIR("M106 P", fan, " S", fan_speed[fan]);
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
  int16_t Temperature::mintemp_raw_BED = TEMP_SENSOR_BED_RAW_LO_TEMP,
          Temperature::maxtemp_raw_BED = TEMP_SENSOR_BED_RAW_HI_TEMP;
  TERN_(WATCH_BED, bed_watch_t Temperature::watch_bed); // = { 0 }
  IF_DISABLED(PIDTEMPBED, millis_t Temperature::next_bed_check_ms);
#endif

#if HAS_TEMP_CHAMBER
  chamber_info_t Temperature::temp_chamber; // = { 0 }
  #if HAS_HEATED_CHAMBER
    millis_t next_cool_check_ms_2 = 0;
    celsius_float_t old_temp = 9999;
    int16_t Temperature::mintemp_raw_CHAMBER = TEMP_SENSOR_CHAMBER_RAW_LO_TEMP,
            Temperature::maxtemp_raw_CHAMBER = TEMP_SENSOR_CHAMBER_RAW_HI_TEMP;
    TERN_(WATCH_CHAMBER, chamber_watch_t Temperature::watch_chamber{0});
    IF_DISABLED(PIDTEMPCHAMBER, millis_t Temperature::next_chamber_check_ms);
  #endif
#endif

#if HAS_TEMP_COOLER
  cooler_info_t Temperature::temp_cooler; // = { 0 }
  #if HAS_COOLER
    bool flag_cooler_state;
    //bool flag_cooler_excess = false;
    celsius_float_t previous_temp = 9999;
    int16_t Temperature::mintemp_raw_COOLER = TEMP_SENSOR_COOLER_RAW_LO_TEMP,
            Temperature::maxtemp_raw_COOLER = TEMP_SENSOR_COOLER_RAW_HI_TEMP;
    #if WATCH_COOLER
      cooler_watch_t Temperature::watch_cooler{0};
    #endif
    millis_t Temperature::next_cooler_check_ms, Temperature::cooler_fan_flush_ms;
  #endif
#endif

#if HAS_TEMP_PROBE
  probe_info_t Temperature::temp_probe; // = { 0 }
#endif

#if HAS_TEMP_BOARD
  board_info_t Temperature::temp_board; // = { 0 }
  #if ENABLED(THERMAL_PROTECTION_BOARD)
    int16_t Temperature::mintemp_raw_BOARD = TEMP_SENSOR_BOARD_RAW_LO_TEMP,
            Temperature::maxtemp_raw_BOARD = TEMP_SENSOR_BOARD_RAW_HI_TEMP;
  #endif
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  bool Temperature::allow_cold_extrude = false;
  celsius_t Temperature::extrude_min_temp = EXTRUDE_MINTEMP;
#endif

#if HAS_ADC_BUTTONS
  uint32_t Temperature::current_ADCKey_raw = HAL_ADC_RANGE;
  uint16_t Temperature::ADCKey_count = 0;
#endif

#if ENABLED(PID_EXTRUSION_SCALING)
  int16_t Temperature::lpq_len; // Initialized in settings.cpp
#endif

/**
 * private:
 */

volatile bool Temperature::raw_temps_ready = false;

#if ENABLED(PID_EXTRUSION_SCALING)
  int32_t Temperature::last_e_position, Temperature::lpq[LPQ_MAX_LEN];
  lpq_ptr_t Temperature::lpq_ptr = 0;
#endif

#define TEMPDIR(N) ((TEMP_SENSOR_##N##_RAW_LO_TEMP) < (TEMP_SENSOR_##N##_RAW_HI_TEMP) ? 1 : -1)

#if HAS_HOTEND
  // Init mintemp and maxtemp with extreme values to prevent false errors during startup
  constexpr temp_range_t sensor_heater_0 { TEMP_SENSOR_0_RAW_LO_TEMP, TEMP_SENSOR_0_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_1 { TEMP_SENSOR_1_RAW_LO_TEMP, TEMP_SENSOR_1_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_2 { TEMP_SENSOR_2_RAW_LO_TEMP, TEMP_SENSOR_2_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_3 { TEMP_SENSOR_3_RAW_LO_TEMP, TEMP_SENSOR_3_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_4 { TEMP_SENSOR_4_RAW_LO_TEMP, TEMP_SENSOR_4_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_5 { TEMP_SENSOR_5_RAW_LO_TEMP, TEMP_SENSOR_5_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_6 { TEMP_SENSOR_6_RAW_LO_TEMP, TEMP_SENSOR_6_RAW_HI_TEMP, 0, 16383 },
                         sensor_heater_7 { TEMP_SENSOR_7_RAW_LO_TEMP, TEMP_SENSOR_7_RAW_HI_TEMP, 0, 16383 };

  temp_range_t Temperature::temp_range[HOTENDS] = ARRAY_BY_HOTENDS(sensor_heater_0, sensor_heater_1, sensor_heater_2, sensor_heater_3, sensor_heater_4, sensor_heater_5, sensor_heater_6, sensor_heater_7);
#endif

#if MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED > 1
  uint8_t Temperature::consecutive_low_temperature_error[HOTENDS] = { 0 };
#endif

#if MILLISECONDS_PREHEAT_TIME > 0
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
  celsius_t Temperature::singlenozzle_temp[EXTRUDERS];
#endif
#if ENABLED(SINGLENOZZLE_STANDBY_FAN)
  uint8_t Temperature::singlenozzle_fan_speed[EXTRUDERS];
#endif

#if ENABLED(PROBING_HEATERS_OFF)
  bool Temperature::paused_for_probing;
#endif

/**
 * public:
 * Class and Instance Methods
 */

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
  void Temperature::PID_autotune(const celsius_t target, const heater_id_t heater_id, const int8_t ncycles, const bool set_result/*=false*/) {
    celsius_float_t current_temp = 0.0;
    int cycles = 0;
    bool heating = true;

    millis_t next_temp_ms = millis(), t1 = next_temp_ms, t2 = next_temp_ms;
    long t_high = 0, t_low = 0;

    PID_t tune_pid = { 0, 0, 0 };
    celsius_float_t maxT = 0, minT = 10000;

    const bool isbed = (heater_id == H_BED);
    const bool ischamber = (heater_id == H_CHAMBER);

    #if ENABLED(PIDTEMPCHAMBER)
      #define C_TERN(T,A,B) ((T) ? (A) : (B))
    #else
      #define C_TERN(T,A,B) (B)
    #endif
    #if ENABLED(PIDTEMPBED)
      #define B_TERN(T,A,B) ((T) ? (A) : (B))
    #else
      #define B_TERN(T,A,B) (B)
    #endif
    #define GHV(C,B,H) C_TERN(ischamber, C, B_TERN(isbed, B, H))
    #define SHV(V) C_TERN(ischamber, temp_chamber.soft_pwm_amount = V, B_TERN(isbed, temp_bed.soft_pwm_amount = V, temp_hotend[heater_id].soft_pwm_amount = V))
    #define ONHEATINGSTART() C_TERN(ischamber, printerEventLEDs.onChamberHeatingStart(), B_TERN(isbed, printerEventLEDs.onBedHeatingStart(), printerEventLEDs.onHotendHeatingStart()))
    #define ONHEATING(S,C,T) C_TERN(ischamber, printerEventLEDs.onChamberHeating(S,C,T), B_TERN(isbed, printerEventLEDs.onBedHeating(S,C,T), printerEventLEDs.onHotendHeating(S,C,T)))

    #define WATCH_PID BOTH(WATCH_CHAMBER, PIDTEMPCHAMBER) || BOTH(WATCH_BED, PIDTEMPBED) || BOTH(WATCH_HOTENDS, PIDTEMP)

    #if WATCH_PID
      #if BOTH(THERMAL_PROTECTION_CHAMBER, PIDTEMPCHAMBER)
        #define C_GTV(T,A,B) ((T) ? (A) : (B))
      #else
        #define C_GTV(T,A,B) (B)
      #endif
      #if BOTH(THERMAL_PROTECTION_BED, PIDTEMPBED)
        #define B_GTV(T,A,B) ((T) ? (A) : (B))
      #else
        #define B_GTV(T,A,B) (B)
      #endif
      #define GTV(C,B,H) C_GTV(ischamber, C, B_GTV(isbed, B, H))
      const uint16_t watch_temp_period = GTV(WATCH_CHAMBER_TEMP_PERIOD, WATCH_BED_TEMP_PERIOD, WATCH_TEMP_PERIOD);
      const uint8_t watch_temp_increase = GTV(WATCH_CHAMBER_TEMP_INCREASE, WATCH_BED_TEMP_INCREASE, WATCH_TEMP_INCREASE);
      const celsius_float_t watch_temp_target = celsius_float_t(target - (watch_temp_increase + GTV(TEMP_CHAMBER_HYSTERESIS, TEMP_BED_HYSTERESIS, TEMP_HYSTERESIS) + 1));
      millis_t temp_change_ms = next_temp_ms + SEC_TO_MS(watch_temp_period);
      celsius_float_t next_watch_temp = 0.0;
      bool heated = false;
    #endif

    TERN_(HAS_AUTO_FAN, next_auto_fan_check_ms = next_temp_ms + 2500UL);

    TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_STARTED));

    if (target > GHV(CHAMBER_MAX_TARGET, BED_MAX_TARGET, temp_range[heater_id].maxtemp - (HOTEND_OVERSHOOT))) {
      SERIAL_ECHOLNPGM(STR_PID_TEMP_TOO_HIGH);
      TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_TEMP_TOO_HIGH));
      return;
    }

    SERIAL_ECHOLNPGM(STR_PID_AUTOTUNE_START);

    disable_all_heaters();
    TERN_(AUTO_POWER_CONTROL, powerManager.power_on());

    long bias = GHV(MAX_CHAMBER_POWER, MAX_BED_POWER, PID_MAX) >> 1, d = bias;
    SHV(bias);

    #if ENABLED(PRINTER_EVENT_LEDS)
      const celsius_float_t start_temp = GHV(degChamber(), degBed(), degHotend(heater_id));
      LEDColor color = ONHEATINGSTART();
    #endif

    TERN_(NO_FAN_SLOWING_IN_PID_TUNING, adaptive_fan_slowing = false);

    // PID Tuning loop
    wait_for_heatup = true; // Can be interrupted with M108
    while (wait_for_heatup) {

      const millis_t ms = millis();

      if (updateTemperaturesIfReady()) { // temp sample ready

        // Get the current temperature and constrain it
        current_temp = GHV(degChamber(), degBed(), degHotend(heater_id));
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

        if (heating && current_temp > target && ELAPSED(ms, t2 + 5000UL)) {
          heating = false;
          SHV((bias - d) >> 1);
          t1 = ms;
          t_high = t1 - t2;
          maxT = target;
        }

        if (!heating && current_temp < target && ELAPSED(ms, t1 + 5000UL)) {
          heating = true;
          t2 = ms;
          t_low = t2 - t1;
          if (cycles > 0) {
            const long max_pow = GHV(MAX_CHAMBER_POWER, MAX_BED_POWER, PID_MAX);
            bias += (d * (t_high - t_low)) / (t_low + t_high);
            LIMIT(bias, 20, max_pow - 20);
            d = (bias > max_pow >> 1) ? max_pow - 1 - bias : bias;

            SERIAL_ECHOPAIR(STR_BIAS, bias, STR_D_COLON, d, STR_T_MIN, minT, STR_T_MAX, maxT);
            if (cycles > 2) {
              const float Ku = (4.0f * d) / (float(M_PI) * (maxT - minT) * 0.5f),
                          Tu = float(t_low + t_high) * 0.001f,
                          pf = ischamber ? 0.2f : (isbed ? 0.2f : 0.6f),
                          df = ischamber ? 1.0f / 3.0f : (isbed ? 1.0f / 3.0f : 1.0f / 8.0f);

              tune_pid.Kp = Ku * pf;
              tune_pid.Ki = tune_pid.Kp * 2.0f / Tu;
              tune_pid.Kd = tune_pid.Kp * Tu * df;

              SERIAL_ECHOLNPAIR(STR_KU, Ku, STR_TU, Tu);
              if (ischamber || isbed)
                SERIAL_ECHOLNPGM(" No overshoot");
              else
                SERIAL_ECHOLNPGM(STR_CLASSIC_PID);
              SERIAL_ECHOLNPAIR(STR_KP, tune_pid.Kp, STR_KI, tune_pid.Ki, STR_KD, tune_pid.Kd);
            }
          }
          SHV((bias + d) >> 1);
          cycles++;
          minT = target;
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
          print_heater_states(ischamber ? active_extruder : (isbed ? active_extruder : heater_id));
          SERIAL_EOL();
        #endif
        next_temp_ms = ms + 2000UL;

        // Make sure heating is actually working
        #if WATCH_PID
          if (BOTH(WATCH_BED, WATCH_HOTENDS) || isbed == DISABLED(WATCH_HOTENDS) || ischamber == DISABLED(WATCH_HOTENDS)) {
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

        #if EITHER(PIDTEMPBED, PIDTEMPCHAMBER)
          PGM_P const estring = GHV(PSTR("chamber"), PSTR("bed"), NUL_STR);
          say_default_(); SERIAL_ECHOPGM_P(estring); SERIAL_ECHOLNPAIR("Kp ", tune_pid.Kp);
          say_default_(); SERIAL_ECHOPGM_P(estring); SERIAL_ECHOLNPAIR("Ki ", tune_pid.Ki);
          say_default_(); SERIAL_ECHOPGM_P(estring); SERIAL_ECHOLNPAIR("Kd ", tune_pid.Kd);
        #else
          say_default_(); SERIAL_ECHOLNPAIR("Kp ", tune_pid.Kp);
          say_default_(); SERIAL_ECHOLNPAIR("Ki ", tune_pid.Ki);
          say_default_(); SERIAL_ECHOLNPAIR("Kd ", tune_pid.Kd);
        #endif

        auto _set_hotend_pid = [](const uint8_t e, const PID_t &in_pid) {
          #if ENABLED(PIDTEMP)
            PID_PARAM(Kp, e) = in_pid.Kp;
            PID_PARAM(Ki, e) = scalePID_i(in_pid.Ki);
            PID_PARAM(Kd, e) = scalePID_d(in_pid.Kd);
            updatePID();
          #else
            UNUSED(e); UNUSED(in_pid);
          #endif
        };

        #if ENABLED(PIDTEMPBED)
          auto _set_bed_pid = [](const PID_t &in_pid) {
            temp_bed.pid.Kp = in_pid.Kp;
            temp_bed.pid.Ki = scalePID_i(in_pid.Ki);
            temp_bed.pid.Kd = scalePID_d(in_pid.Kd);
          };
        #endif

        #if ENABLED(PIDTEMPCHAMBER)
          auto _set_chamber_pid = [](const PID_t &in_pid) {
            temp_chamber.pid.Kp = in_pid.Kp;
            temp_chamber.pid.Ki = scalePID_i(in_pid.Ki);
            temp_chamber.pid.Kd = scalePID_d(in_pid.Kd);
          };
        #endif

        // Use the result? (As with "M303 U1")
        if (set_result)
          GHV(_set_chamber_pid(tune_pid), _set_bed_pid(tune_pid), _set_hotend_pid(heater_id, tune_pid));

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

int16_t Temperature::getHeaterPower(const heater_id_t heater_id) {
  switch (heater_id) {
    #if HAS_HEATED_BED
      case H_BED: return temp_bed.soft_pwm_amount;
    #endif
    #if HAS_HEATED_CHAMBER
      case H_CHAMBER: return temp_chamber.soft_pwm_amount;
    #endif
    #if HAS_COOLER
      case H_COOLER: return temp_cooler.soft_pwm_amount;
    #endif
    default:
      return TERN0(HAS_HOTEND, temp_hotend[heater_id].soft_pwm_amount);
  }
}

#define _EFANOVERLAP(A,B) _FANOVERLAP(E##A,B)

#if HAS_AUTO_FAN
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

    #if HAS_AUTO_COOLER_FAN
      if (temp_cooler.celsius >= COOLER_AUTO_FAN_TEMPERATURE)
        SBI(fanState, pgm_read_byte(&fanBit[COOLER_FAN_INDEX]));
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
    SERIAL_ECHOPGM_P(serial_msg);
    SERIAL_ECHOPGM(STR_STOPPED_HEATER);

    heater_id_t real_heater_id = heater_id;

    #if HAS_TEMP_REDUNDANT
      if (heater_id == H_REDUNDANT) {
        SERIAL_ECHOPGM(STR_REDUNDANT); // print redundant and cascade to print target, too.
        real_heater_id = (heater_id_t)HEATER_ID(TEMP_SENSOR_REDUNDANT_TARGET);
      }
    #endif

    switch (real_heater_id) {
      OPTCODE(HAS_TEMP_COOLER,  case H_COOLER:  SERIAL_ECHOPGM(STR_COOLER);         break)
      OPTCODE(HAS_TEMP_PROBE,   case H_PROBE:   SERIAL_ECHOPGM(STR_PROBE);          break)
      OPTCODE(HAS_TEMP_BOARD,   case H_BOARD:   SERIAL_ECHOPGM(STR_MOTHERBOARD);    break)
      OPTCODE(HAS_TEMP_CHAMBER, case H_CHAMBER: SERIAL_ECHOPGM(STR_HEATER_CHAMBER); break)
      OPTCODE(HAS_TEMP_BED,     case H_BED:     SERIAL_ECHOPGM(STR_HEATER_BED);     break)
      default:
        if (real_heater_id >= 0)
          SERIAL_ECHOLNPAIR("E", real_heater_id);
    }
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

#if ANY(PID_DEBUG, PID_BED_DEBUG, PID_CHAMBER_DEBUG)
  bool Temperature::pid_debug_flag; // = 0
#endif

#if HAS_HOTEND

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
          SERIAL_ECHO_MSG(STR_PID_DEBUG, ee, STR_PID_DEBUG_INPUT, temp_hotend[ee].celsius, STR_PID_DEBUG_OUTPUT, pid_output
            #if DISABLED(PID_OPENLOOP)
              , STR_PID_DEBUG_PTERM, work_pid[ee].Kp
              , STR_PID_DEBUG_ITERM, work_pid[ee].Ki
              , STR_PID_DEBUG_DTERM, work_pid[ee].Kd
              #if ENABLED(PID_EXTRUSION_SCALING)
                , STR_PID_DEBUG_CTERM, work_pid[ee].Kc
              #endif
            #endif
          );
        }
      #endif

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
      if (pid_debug_flag) {
        SERIAL_ECHO_MSG(
          " PID_BED_DEBUG : Input ", temp_bed.celsius, " Output ", pid_output
          #if DISABLED(PID_OPENLOOP)
            , STR_PID_DEBUG_PTERM, work_pid.Kp
            , STR_PID_DEBUG_ITERM, work_pid.Ki
            , STR_PID_DEBUG_DTERM, work_pid.Kd
          #endif
        );
      }
    #endif

    return pid_output;
  }

#endif // PIDTEMPBED

#if ENABLED(PIDTEMPCHAMBER)

  float Temperature::get_pid_output_chamber() {

    #if DISABLED(PID_OPENLOOP)

      static PID_t work_pid{0};
      static float temp_iState = 0, temp_dState = 0;
      static bool pid_reset = true;
      float pid_output = 0;
      const float max_power_over_i_gain = float(MAX_CHAMBER_POWER) / temp_chamber.pid.Ki - float(MIN_CHAMBER_POWER),
                  pid_error = temp_chamber.target - temp_chamber.celsius;

      if (!temp_chamber.target || pid_error < -(PID_FUNCTIONAL_RANGE)) {
        pid_output = 0;
        pid_reset = true;
      }
      else if (pid_error > PID_FUNCTIONAL_RANGE) {
        pid_output = MAX_CHAMBER_POWER;
        pid_reset = true;
      }
      else {
        if (pid_reset) {
          temp_iState = 0.0;
          work_pid.Kd = 0.0;
          pid_reset = false;
        }

        temp_iState = constrain(temp_iState + pid_error, 0, max_power_over_i_gain);

        work_pid.Kp = temp_chamber.pid.Kp * pid_error;
        work_pid.Ki = temp_chamber.pid.Ki * temp_iState;
        work_pid.Kd = work_pid.Kd + PID_K2 * (temp_chamber.pid.Kd * (temp_dState - temp_chamber.celsius) - work_pid.Kd);

        temp_dState = temp_chamber.celsius;

        pid_output = constrain(work_pid.Kp + work_pid.Ki + work_pid.Kd + float(MIN_CHAMBER_POWER), 0, MAX_CHAMBER_POWER);
      }

    #else // PID_OPENLOOP

      const float pid_output = constrain(temp_chamber.target, 0, MAX_CHAMBER_POWER);

    #endif // PID_OPENLOOP

    #if ENABLED(PID_CHAMBER_DEBUG)
    {
      SERIAL_ECHO_MSG(
        " PID_CHAMBER_DEBUG : Input ", temp_chamber.celsius, " Output ", pid_output
        #if DISABLED(PID_OPENLOOP)
          , STR_PID_DEBUG_PTERM, work_pid.Kp
          , STR_PID_DEBUG_ITERM, work_pid.Ki
          , STR_PID_DEBUG_DTERM, work_pid.Kd
        #endif
      );
    }
    #endif

    return pid_output;
  }

#endif // PIDTEMPCHAMBER

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
  if (marlin_state == MF_INITIALIZING) return watchdog_refresh(); // If Marlin isn't started, at least reset the watchdog!

  static bool no_reentry = false;  // Prevent recursion
  if (no_reentry) return;
  REMEMBER(mh, no_reentry, true);

  #if ENABLED(EMERGENCY_PARSER)
    if (emergency_parser.killed_by_M112) kill(M112_KILL_STR, nullptr, true);

    if (emergency_parser.quickstop_by_M410) {
      emergency_parser.quickstop_by_M410 = false; // quickstop_stepper may call idle so clear this now!
      quickstop_stepper();
    }
  #endif

  if (!updateTemperaturesIfReady()) return; // Will also reset the watchdog if temperatures are ready

  #if DISABLED(IGNORE_THERMOCOUPLE_ERRORS)
    #if TEMP_SENSOR_0_IS_MAX_TC
      if (degHotend(0) > _MIN(HEATER_0_MAXTEMP, TEMP_SENSOR_0_MAX_TC_TMAX - 1.0)) max_temp_error(H_E0);
      if (degHotend(0) < _MAX(HEATER_0_MINTEMP, TEMP_SENSOR_0_MAX_TC_TMIN + .01)) min_temp_error(H_E0);
    #endif
    #if TEMP_SENSOR_1_IS_MAX_TC
      if (degHotend(1) > _MIN(HEATER_1_MAXTEMP, TEMP_SENSOR_1_MAX_TC_TMAX - 1.0)) max_temp_error(H_E1);
      if (degHotend(1) < _MAX(HEATER_1_MINTEMP, TEMP_SENSOR_1_MAX_TC_TMIN + .01)) min_temp_error(H_E1);
    #endif
    #if TEMP_SENSOR_REDUNDANT_IS_MAX_TC
      if (degRedundant() > TEMP_SENSOR_REDUNDANT_MAX_TC_TMAX - 1.0) max_temp_error(H_REDUNDANT);
      if (degRedundant() < TEMP_SENSOR_REDUNDANT_MAX_TC_TMIN + .01) min_temp_error(H_REDUNDANT);
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
        if (watch_hotend[e].elapsed(ms)) {          // Enabled and time to check?
          if (watch_hotend[e].check(degHotend(e)))  // Increased enough?
            start_watching_hotend(e);               // If temp reached, turn off elapsed check
          else {
            TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
            _temp_error((heater_id_t)e, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
          }
        }
      #endif

    } // HOTEND_LOOP

  #endif // HAS_HOTEND

  #if HAS_TEMP_REDUNDANT
    // Make sure measured temperatures are close together
    if (ABS(degRedundantTarget() - degRedundant()) > TEMP_SENSOR_REDUNDANT_MAX_DIFF)
      _temp_error((heater_id_t)HEATER_ID(TEMP_SENSOR_REDUNDANT_TARGET), PSTR(STR_REDUNDANCY), GET_TEXT(MSG_ERR_REDUNDANT_TEMP));
  #endif

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
      if (watch_bed.elapsed(ms)) {              // Time to check the bed?
        if (watch_bed.check(degBed()))          // Increased enough?
          start_watching_bed();                 // If temp reached, turn off elapsed check
        else {
          TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
          _temp_error(H_BED, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
        }
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
          && TERN1(PAUSE_CHANGE_REQD, paused_for_probing == last_pause_state)
        ) break;
        next_bed_check_ms = ms + BED_CHECK_INTERVAL;
        TERN_(PAUSE_CHANGE_REQD, last_pause_state = paused_for_probing);
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
      if (watch_chamber.elapsed(ms)) {          // Time to check the chamber?
        if (watch_chamber.check(degChamber()))  // Increased enough? Error below.
          start_watching_chamber();             // If temp reached, turn off elapsed check.
        else
          _temp_error(H_CHAMBER, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
      }
    #endif

    #if EITHER(CHAMBER_FAN, CHAMBER_VENT) || DISABLED(PIDTEMPCHAMBER)
      static bool flag_chamber_excess_heat; // = false;
    #endif

    #if EITHER(CHAMBER_FAN, CHAMBER_VENT)
      static bool flag_chamber_off; // = false

      if (temp_chamber.target > CHAMBER_MINTEMP) {
        flag_chamber_off = false;

        #if ENABLED(CHAMBER_FAN)
          int16_t fan_chamber_pwm;
          #if CHAMBER_FAN_MODE == 0
            fan_chamber_pwm = CHAMBER_FAN_BASE;
          #elif CHAMBER_FAN_MODE == 1
            fan_chamber_pwm = (temp_chamber.celsius > temp_chamber.target) ? (CHAMBER_FAN_BASE) + (CHAMBER_FAN_FACTOR) * (temp_chamber.celsius - temp_chamber.target) : 0;
          #elif CHAMBER_FAN_MODE == 2
            fan_chamber_pwm = (CHAMBER_FAN_BASE) + (CHAMBER_FAN_FACTOR) * ABS(temp_chamber.celsius - temp_chamber.target);
            if (temp_chamber.soft_pwm_amount)
              fan_chamber_pwm += (CHAMBER_FAN_FACTOR) * 2;
          #elif CHAMBER_FAN_MODE == 3
            fan_chamber_pwm = CHAMBER_FAN_BASE + _MAX((CHAMBER_FAN_FACTOR) * (temp_chamber.celsius - temp_chamber.target), 0);
          #endif
          NOMORE(fan_chamber_pwm, 225);
          set_fan_speed(CHAMBER_FAN_INDEX, fan_chamber_pwm); // TODO: instead of fan 2, set to chamber fan
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
              if (temp_chamber.celsius - old_temp > MIN_COOLING_SLOPE_DEG_CHAMBER_VENT)
                flag_chamber_excess_heat = true; // the bed is heating the chamber too much
              next_cool_check_ms_2 = ms + SEC_TO_MS(MIN_COOLING_SLOPE_TIME_CHAMBER_VENT);
              old_temp = temp_chamber.celsius;
            }
          }
          else {
            next_cool_check_ms_2 = 0;
            old_temp = 9999;
          }
          if (flag_chamber_excess_heat && (temp_chamber.target - temp_chamber.celsius >= LOW_EXCESS_HEAT_LIMIT))
            flag_chamber_excess_heat = false;
        #endif
      }
      else if (!flag_chamber_off) {
        #if ENABLED(CHAMBER_FAN)
          flag_chamber_off = true;
          set_fan_speed(CHAMBER_FAN_INDEX, 0);
        #endif
        #if ENABLED(CHAMBER_VENT)
          flag_chamber_excess_heat = false;
          MOVE_SERVO(CHAMBER_VENT_SERVO_NR, 90);
        #endif
      }
    #endif

    #if ENABLED(PIDTEMPCHAMBER)
      // PIDTEMPCHAMBER doesn't support a CHAMBER_VENT yet.
      temp_chamber.soft_pwm_amount = WITHIN(temp_chamber.celsius, CHAMBER_MINTEMP, CHAMBER_MAXTEMP) ? (int)get_pid_output_chamber() >> 1 : 0;
    #else
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
     }
     #if ENABLED(THERMAL_PROTECTION_CHAMBER)
       tr_state_machine[RUNAWAY_IND_CHAMBER].run(temp_chamber.celsius, temp_chamber.target, H_CHAMBER, THERMAL_PROTECTION_CHAMBER_PERIOD, THERMAL_PROTECTION_CHAMBER_HYSTERESIS);
     #endif
   #endif

  #endif // HAS_HEATED_CHAMBER

  #if HAS_COOLER

    #ifndef COOLER_CHECK_INTERVAL
      #define COOLER_CHECK_INTERVAL 2000UL
    #endif

    #if ENABLED(THERMAL_PROTECTION_COOLER)
      if (degCooler() > COOLER_MAXTEMP) max_temp_error(H_COOLER);
    #endif

    #if WATCH_COOLER
      // Make sure temperature is decreasing
      if (watch_cooler.elapsed(ms)) {             // Time to check the cooler?
        if (degCooler() > watch_cooler.target)    // Failed to decrease enough?
          _temp_error(H_COOLER, GET_TEXT(MSG_COOLING_FAILED), GET_TEXT(MSG_COOLING_FAILED));
        else
          start_watching_cooler();                 // Start again if the target is still far off
      }
    #endif

    static bool flag_cooler_state; // = false

    if (cooler.enabled) {
      flag_cooler_state = true; // used to allow M106 fan control when cooler is disabled
      if (temp_cooler.target == 0) temp_cooler.target = COOLER_MIN_TARGET;
      if (ELAPSED(ms, next_cooler_check_ms)) {
        next_cooler_check_ms = ms + COOLER_CHECK_INTERVAL;
        if (temp_cooler.celsius > temp_cooler.target) {
          temp_cooler.soft_pwm_amount = temp_cooler.celsius > temp_cooler.target ? MAX_COOLER_POWER : 0;
          flag_cooler_state = temp_cooler.soft_pwm_amount > 0 ? true : false; // used to allow M106 fan control when cooler is disabled
          #if ENABLED(COOLER_FAN)
            int16_t fan_cooler_pwm = (COOLER_FAN_BASE) + (COOLER_FAN_FACTOR) * ABS(temp_cooler.celsius - temp_cooler.target);
            NOMORE(fan_cooler_pwm, 255);
            set_fan_speed(COOLER_FAN_INDEX, fan_cooler_pwm); // Set cooler fan pwm
            cooler_fan_flush_ms = ms + 5000;
          #endif
        }
        else {
          temp_cooler.soft_pwm_amount = 0;
          #if ENABLED(COOLER_FAN)
            set_fan_speed(COOLER_FAN_INDEX, temp_cooler.celsius > temp_cooler.target - 2 ? COOLER_FAN_BASE : 0);
          #endif
          WRITE_HEATER_COOLER(LOW);
        }
      }
    }
    else {
      temp_cooler.soft_pwm_amount = 0;
      if (flag_cooler_state) {
        flag_cooler_state = false;
        thermalManager.set_fan_speed(COOLER_FAN_INDEX, 0);
      }
      WRITE_HEATER_COOLER(LOW);
    }

    #if ENABLED(THERMAL_PROTECTION_COOLER)
      tr_state_machine[RUNAWAY_IND_COOLER].run(temp_cooler.celsius, temp_cooler.target, H_COOLER, THERMAL_PROTECTION_COOLER_PERIOD, THERMAL_PROTECTION_COOLER_HYSTERESIS);
    #endif

  #endif // HAS_COOLER

  #if ENABLED(LASER_COOLANT_FLOW_METER)
    cooler.flowmeter_task(ms);
    #if ENABLED(FLOWMETER_SAFETY)
      if (cutter.enabled() && cooler.check_flow_too_low()) {
        cutter.disable();
        TERN_(HAS_DISPLAY, ui.flow_fault());
      }
    #endif
  #endif

  UNUSED(ms);
}

#define TEMP_AD595(RAW)  ((RAW) * 5.0 * 100.0 / float(HAL_ADC_RANGE) / (OVERSAMPLENR) * (TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET)
#define TEMP_AD8495(RAW) ((RAW) * 6.6 * 100.0 / float(HAL_ADC_RANGE) / (OVERSAMPLENR) * (TEMP_SENSOR_AD8495_GAIN) + TEMP_SENSOR_AD8495_OFFSET)

/**
 * Bisect search for the range of the 'raw' value, then interpolate
 * proportionally between the under and over values.
 */
#define SCAN_THERMISTOR_TABLE(TBL,LEN) do{                                \
  uint8_t l = 0, r = LEN, m;                                              \
  for (;;) {                                                              \
    m = (l + r) >> 1;                                                     \
    if (!m) return celsius_t(pgm_read_word(&TBL[0].celsius));             \
    if (m == l || m == r) return celsius_t(pgm_read_word(&TBL[LEN-1].celsius)); \
    int16_t v00 = pgm_read_word(&TBL[m-1].value),                         \
            v10 = pgm_read_word(&TBL[m-0].value);                         \
         if (raw < v00) r = m;                                            \
    else if (raw > v10) l = m;                                            \
    else {                                                                \
      const celsius_t v01 = celsius_t(pgm_read_word(&TBL[m-1].celsius)),  \
                      v11 = celsius_t(pgm_read_word(&TBL[m-0].celsius));  \
      return v01 + (raw - v00) * float(v11 - v01) / float(v10 - v00);     \
    }                                                                     \
  }                                                                       \
}while(0)

#if HAS_USER_THERMISTORS

  user_thermistor_t Temperature::user_thermistor[USER_THERMISTORS]; // Initialized by settings.load()

  void Temperature::reset_user_thermistors() {
    user_thermistor_t default_user_thermistor[USER_THERMISTORS] = {
      #if TEMP_SENSOR_0_IS_CUSTOM
        { true, 0, 0, HOTEND0_PULLUP_RESISTOR_OHMS, HOTEND0_RESISTANCE_25C_OHMS, 0, 0, HOTEND0_BETA, 0 },
      #endif
      #if TEMP_SENSOR_1_IS_CUSTOM
        { true, 0, 0, HOTEND1_PULLUP_RESISTOR_OHMS, HOTEND1_RESISTANCE_25C_OHMS, 0, 0, HOTEND1_BETA, 0 },
      #endif
      #if TEMP_SENSOR_2_IS_CUSTOM
        { true, 0, 0, HOTEND2_PULLUP_RESISTOR_OHMS, HOTEND2_RESISTANCE_25C_OHMS, 0, 0, HOTEND2_BETA, 0 },
      #endif
      #if TEMP_SENSOR_3_IS_CUSTOM
        { true, 0, 0, HOTEND3_PULLUP_RESISTOR_OHMS, HOTEND3_RESISTANCE_25C_OHMS, 0, 0, HOTEND3_BETA, 0 },
      #endif
      #if TEMP_SENSOR_4_IS_CUSTOM
        { true, 0, 0, HOTEND4_PULLUP_RESISTOR_OHMS, HOTEND4_RESISTANCE_25C_OHMS, 0, 0, HOTEND4_BETA, 0 },
      #endif
      #if TEMP_SENSOR_5_IS_CUSTOM
        { true, 0, 0, HOTEND5_PULLUP_RESISTOR_OHMS, HOTEND5_RESISTANCE_25C_OHMS, 0, 0, HOTEND5_BETA, 0 },
      #endif
      #if TEMP_SENSOR_6_IS_CUSTOM
        { true, 0, 0, HOTEND6_PULLUP_RESISTOR_OHMS, HOTEND6_RESISTANCE_25C_OHMS, 0, 0, HOTEND6_BETA, 0 },
      #endif
      #if TEMP_SENSOR_7_IS_CUSTOM
        { true, 0, 0, HOTEND7_PULLUP_RESISTOR_OHMS, HOTEND7_RESISTANCE_25C_OHMS, 0, 0, HOTEND7_BETA, 0 },
      #endif
      #if TEMP_SENSOR_BED_IS_CUSTOM
        { true, 0, 0, BED_PULLUP_RESISTOR_OHMS, BED_RESISTANCE_25C_OHMS, 0, 0, BED_BETA, 0 },
      #endif
      #if TEMP_SENSOR_CHAMBER_IS_CUSTOM
        { true, 0, 0, CHAMBER_PULLUP_RESISTOR_OHMS, CHAMBER_RESISTANCE_25C_OHMS, 0, 0, CHAMBER_BETA, 0 },
      #endif
      #if TEMP_SENSOR_COOLER_IS_CUSTOM
        { true, 0, 0, COOLER_PULLUP_RESISTOR_OHMS, COOLER_RESISTANCE_25C_OHMS, 0, 0, COOLER_BETA, 0 },
      #endif
      #if TEMP_SENSOR_PROBE_IS_CUSTOM
        { true, 0, 0, PROBE_PULLUP_RESISTOR_OHMS, PROBE_RESISTANCE_25C_OHMS, 0, 0, PROBE_BETA, 0 },
      #endif
      #if TEMP_SENSOR_BOARD_IS_CUSTOM
        { true, 0, 0, BOARD_PULLUP_RESISTOR_OHMS, BOARD_RESISTANCE_25C_OHMS, 0, 0, BOARD_BETA, 0 },
      #endif
      #if TEMP_SENSOR_REDUNDANT_IS_CUSTOM
        { true, 0, 0, REDUNDANT_PULLUP_RESISTOR_OHMS, REDUNDANT_RESISTANCE_25C_OHMS, 0, 0, REDUNDANT_BETA, 0 },
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
    SERIAL_ECHOPGM_P(
      TERN_(TEMP_SENSOR_0_IS_CUSTOM, t_index == CTI_HOTEND_0 ? PSTR("HOTEND 0") :)
      TERN_(TEMP_SENSOR_1_IS_CUSTOM, t_index == CTI_HOTEND_1 ? PSTR("HOTEND 1") :)
      TERN_(TEMP_SENSOR_2_IS_CUSTOM, t_index == CTI_HOTEND_2 ? PSTR("HOTEND 2") :)
      TERN_(TEMP_SENSOR_3_IS_CUSTOM, t_index == CTI_HOTEND_3 ? PSTR("HOTEND 3") :)
      TERN_(TEMP_SENSOR_4_IS_CUSTOM, t_index == CTI_HOTEND_4 ? PSTR("HOTEND 4") :)
      TERN_(TEMP_SENSOR_5_IS_CUSTOM, t_index == CTI_HOTEND_5 ? PSTR("HOTEND 5") :)
      TERN_(TEMP_SENSOR_6_IS_CUSTOM, t_index == CTI_HOTEND_6 ? PSTR("HOTEND 6") :)
      TERN_(TEMP_SENSOR_7_IS_CUSTOM, t_index == CTI_HOTEND_7 ? PSTR("HOTEND 7") :)
      TERN_(TEMP_SENSOR_BED_IS_CUSTOM, t_index == CTI_BED ? PSTR("BED") :)
      TERN_(TEMP_SENSOR_CHAMBER_IS_CUSTOM, t_index == CTI_CHAMBER ? PSTR("CHAMBER") :)
      TERN_(TEMP_SENSOR_COOLER_IS_CUSTOM, t_index == CTI_COOLER ? PSTR("COOLER") :)
      TERN_(TEMP_SENSOR_PROBE_IS_CUSTOM, t_index == CTI_PROBE ? PSTR("PROBE") :)
      TERN_(TEMP_SENSOR_BOARD_IS_CUSTOM, t_index == CTI_BOARD ? PSTR("BOARD") :)
      TERN_(TEMP_SENSOR_REDUNDANT_IS_CUSTOM, t_index == CTI_REDUNDANT ? PSTR("REDUNDANT") :)
      nullptr
    );
    SERIAL_EOL();
  }

  celsius_float_t Temperature::user_thermistor_to_deg_c(const uint8_t t_index, const int16_t raw) {

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

    // Return degrees C (up to 999, as the LCD only displays 3 digits)
    return _MIN(value + THERMISTOR_ABS_ZERO_C, 999);
  }
#endif

#if HAS_HOTEND
  // Derived from RepRap FiveD extruder::getTemperature()
  // For hot end temperature measurement.
  celsius_float_t Temperature::analog_to_celsius_hotend(const int16_t raw, const uint8_t e) {
      if (e >= HOTENDS) {
        SERIAL_ERROR_START();
        SERIAL_ECHO(e);
        SERIAL_ECHOLNPGM(STR_INVALID_EXTRUDER_NUM);
        kill();
        return 0;
      }

    switch (e) {
      case 0:
        #if TEMP_SENSOR_0_IS_CUSTOM
          return user_thermistor_to_deg_c(CTI_HOTEND_0, raw);
        #elif TEMP_SENSOR_0_IS_MAX_TC
          #if TEMP_SENSOR_0_IS_MAX31865
            return TERN(LIB_INTERNAL_MAX31865,
              max31865_0.temperature((uint16_t)raw),
              max31865_0.temperature(MAX31865_SENSOR_OHMS_0, MAX31865_CALIBRATION_OHMS_0)
            );
          #else
            return raw * 0.25;
          #endif
        #elif TEMP_SENSOR_0_IS_AD595
          return TEMP_AD595(raw);
        #elif TEMP_SENSOR_0_IS_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 1:
        #if TEMP_SENSOR_1_IS_CUSTOM
          return user_thermistor_to_deg_c(CTI_HOTEND_1, raw);
        #elif TEMP_SENSOR_1_IS_MAX_TC
          #if TEMP_SENSOR_0_IS_MAX31865
            return TERN(LIB_INTERNAL_MAX31865,
              max31865_1.temperature((uint16_t)raw),
              max31865_1.temperature(MAX31865_SENSOR_OHMS_1, MAX31865_CALIBRATION_OHMS_1)
            );
          #else
            return raw * 0.25;
          #endif
        #elif TEMP_SENSOR_1_IS_AD595
          return TEMP_AD595(raw);
        #elif TEMP_SENSOR_1_IS_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 2:
        #if TEMP_SENSOR_2_IS_CUSTOM
          return user_thermistor_to_deg_c(CTI_HOTEND_2, raw);
        #elif TEMP_SENSOR_2_IS_AD595
          return TEMP_AD595(raw);
        #elif TEMP_SENSOR_2_IS_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 3:
        #if TEMP_SENSOR_3_IS_CUSTOM
          return user_thermistor_to_deg_c(CTI_HOTEND_3, raw);
        #elif TEMP_SENSOR_3_IS_AD595
          return TEMP_AD595(raw);
        #elif TEMP_SENSOR_3_IS_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 4:
        #if TEMP_SENSOR_4_IS_CUSTOM
          return user_thermistor_to_deg_c(CTI_HOTEND_4, raw);
        #elif TEMP_SENSOR_4_IS_AD595
          return TEMP_AD595(raw);
        #elif TEMP_SENSOR_4_IS_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 5:
        #if TEMP_SENSOR_5_IS_CUSTOM
          return user_thermistor_to_deg_c(CTI_HOTEND_5, raw);
        #elif TEMP_SENSOR_5_IS_AD595
          return TEMP_AD595(raw);
        #elif TEMP_SENSOR_5_IS_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 6:
        #if TEMP_SENSOR_6_IS_CUSTOM
          return user_thermistor_to_deg_c(CTI_HOTEND_6, raw);
        #elif TEMP_SENSOR_6_IS_AD595
          return TEMP_AD595(raw);
        #elif TEMP_SENSOR_6_IS_AD8495
          return TEMP_AD8495(raw);
        #else
          break;
        #endif
      case 7:
        #if TEMP_SENSOR_7_IS_CUSTOM
          return user_thermistor_to_deg_c(CTI_HOTEND_7, raw);
        #elif TEMP_SENSOR_7_IS_AD595
          return TEMP_AD595(raw);
        #elif TEMP_SENSOR_7_IS_AD8495
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
  // For bed temperature measurement.
  celsius_float_t Temperature::analog_to_celsius_bed(const int16_t raw) {
    #if TEMP_SENSOR_BED_IS_CUSTOM
      return user_thermistor_to_deg_c(CTI_BED, raw);
    #elif TEMP_SENSOR_BED_IS_THERMISTOR
      SCAN_THERMISTOR_TABLE(TEMPTABLE_BED, TEMPTABLE_BED_LEN);
    #elif TEMP_SENSOR_BED_IS_AD595
      return TEMP_AD595(raw);
    #elif TEMP_SENSOR_BED_IS_AD8495
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_HEATED_BED

#if HAS_TEMP_CHAMBER
  // For chamber temperature measurement.
  celsius_float_t Temperature::analog_to_celsius_chamber(const int16_t raw) {
    #if TEMP_SENSOR_CHAMBER_IS_CUSTOM
      return user_thermistor_to_deg_c(CTI_CHAMBER, raw);
    #elif TEMP_SENSOR_CHAMBER_IS_THERMISTOR
      SCAN_THERMISTOR_TABLE(TEMPTABLE_CHAMBER, TEMPTABLE_CHAMBER_LEN);
    #elif TEMP_SENSOR_CHAMBER_IS_AD595
      return TEMP_AD595(raw);
    #elif TEMP_SENSOR_CHAMBER_IS_AD8495
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_TEMP_CHAMBER

#if HAS_TEMP_COOLER
  // For cooler temperature measurement.
  celsius_float_t Temperature::analog_to_celsius_cooler(const int16_t raw) {
    #if TEMP_SENSOR_COOLER_IS_CUSTOM
      return user_thermistor_to_deg_c(CTI_COOLER, raw);
    #elif TEMP_SENSOR_COOLER_IS_THERMISTOR
      SCAN_THERMISTOR_TABLE(TEMPTABLE_COOLER, TEMPTABLE_COOLER_LEN);
    #elif TEMP_SENSOR_COOLER_IS_AD595
      return TEMP_AD595(raw);
    #elif TEMP_SENSOR_COOLER_IS_AD8495
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_TEMP_COOLER

#if HAS_TEMP_PROBE
  // For probe temperature measurement.
  celsius_float_t Temperature::analog_to_celsius_probe(const int16_t raw) {
    #if TEMP_SENSOR_PROBE_IS_CUSTOM
      return user_thermistor_to_deg_c(CTI_PROBE, raw);
    #elif TEMP_SENSOR_PROBE_IS_THERMISTOR
      SCAN_THERMISTOR_TABLE(TEMPTABLE_PROBE, TEMPTABLE_PROBE_LEN);
    #elif TEMP_SENSOR_PROBE_IS_AD595
      return TEMP_AD595(raw);
    #elif TEMP_SENSOR_PROBE_IS_AD8495
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_TEMP_PROBE

#if HAS_TEMP_BOARD
  // For motherboard temperature measurement.
  celsius_float_t Temperature::analog_to_celsius_board(const int16_t raw) {
    #if TEMP_SENSOR_BOARD_IS_CUSTOM
      return user_thermistor_to_deg_c(CTI_BOARD, raw);
    #elif TEMP_SENSOR_BOARD_IS_THERMISTOR
      SCAN_THERMISTOR_TABLE(TEMPTABLE_BOARD, TEMPTABLE_BOARD_LEN);
    #elif TEMP_SENSOR_BOARD_IS_AD595
      return TEMP_AD595(raw);
    #elif TEMP_SENSOR_BOARD_IS_AD8495
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_TEMP_BOARD

#if HAS_TEMP_REDUNDANT
  // For redundant temperature measurement.
  celsius_float_t Temperature::analog_to_celsius_redundant(const int16_t raw) {
    #if TEMP_SENSOR_REDUNDANT_IS_CUSTOM
      return user_thermistor_to_deg_c(CTI_REDUNDANT, raw);
    #elif TEMP_SENSOR_REDUNDANT_IS_MAX_TC && REDUNDANT_TEMP_MATCH(SOURCE, E0)
      return TERN(TEMP_SENSOR_REDUNDANT_IS_MAX31865, max31865_0.temperature((uint16_t)raw), raw * 0.25);
    #elif TEMP_SENSOR_REDUNDANT_IS_MAX_TC && REDUNDANT_TEMP_MATCH(SOURCE, E1)
      return TERN(TEMP_SENSOR_REDUNDANT_IS_MAX31865, max31865_1.temperature((uint16_t)raw), raw * 0.25);
    #elif TEMP_SENSOR_REDUNDANT_IS_THERMISTOR
      SCAN_THERMISTOR_TABLE(TEMPTABLE_REDUNDANT, TEMPTABLE_REDUNDANT_LEN);
    #elif TEMP_SENSOR_REDUNDANT_IS_AD595
      return TEMP_AD595(raw);
    #elif TEMP_SENSOR_REDUNDANT_IS_AD8495
      return TEMP_AD8495(raw);
    #else
      UNUSED(raw);
      return 0;
    #endif
  }
#endif // HAS_TEMP_REDUNDANT

/**
 * Convert the raw sensor readings into actual Celsius temperatures and
 * validate raw temperatures. Bad readings generate min/maxtemp errors.
 *
 * The raw values are generated entirely in interrupt context, and this
 * method is called from normal context once 'raw_temps_ready' has been
 * set by update_raw_temperatures().
 *
 * The watchdog is dependent on this method. If 'raw_temps_ready' stops
 * being set by the interrupt so that this method is not called for over
 * 4 seconds then something has gone afoul and the machine will be reset.
 */
void Temperature::updateTemperaturesFromRawValues() {

  watchdog_refresh(); // Reset because raw_temps_ready was set by the interrupt

  TERN_(TEMP_SENSOR_0_IS_MAX_TC, temp_hotend[0].raw = READ_MAX_TC(0));
  TERN_(TEMP_SENSOR_1_IS_MAX_TC, temp_hotend[1].raw = READ_MAX_TC(1));
  TERN_(TEMP_SENSOR_REDUNDANT_IS_MAX_TC, temp_redundant.raw = READ_MAX_TC(HEATER_ID(TEMP_SENSOR_REDUNDANT_SOURCE)));

  #if HAS_HOTEND
    HOTEND_LOOP() temp_hotend[e].celsius = analog_to_celsius_hotend(temp_hotend[e].raw, e);
  #endif

  TERN_(HAS_HEATED_BED,     temp_bed.celsius       = analog_to_celsius_bed(temp_bed.raw));
  TERN_(HAS_TEMP_CHAMBER,   temp_chamber.celsius   = analog_to_celsius_chamber(temp_chamber.raw));
  TERN_(HAS_TEMP_COOLER,    temp_cooler.celsius    = analog_to_celsius_cooler(temp_cooler.raw));
  TERN_(HAS_TEMP_PROBE,     temp_probe.celsius     = analog_to_celsius_probe(temp_probe.raw));
  TERN_(HAS_TEMP_BOARD,     temp_board.celsius     = analog_to_celsius_board(temp_board.raw));
  TERN_(HAS_TEMP_REDUNDANT, temp_redundant.celsius = analog_to_celsius_redundant(temp_redundant.raw));

  TERN_(FILAMENT_WIDTH_SENSOR, filwidth.update_measured_mm());
  TERN_(HAS_POWER_MONITOR,     power_monitor.capture_values());

  #if HAS_HOTEND
    static constexpr int8_t temp_dir[] = {
      #if TEMP_SENSOR_IS_ANY_MAX_TC(0)
        0
      #else
        TEMPDIR(0)
      #endif
      #if HAS_MULTI_HOTEND
        #if TEMP_SENSOR_IS_ANY_MAX_TC(1)
          , 0
        #else
          , TEMPDIR(1)
        #endif
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
        if (rawtemp > temp_range[e].raw_max * tdir) max_temp_error((heater_id_t)e);

        const bool heater_on = temp_hotend[e].target > 0;
        if (heater_on && rawtemp < temp_range[e].raw_min * tdir && !is_preheating(e)) {
          #if MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED > 1
            if (++consecutive_low_temperature_error[e] >= MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED)
          #endif
              min_temp_error((heater_id_t)e);
        }
        #if MAX_CONSECUTIVE_LOW_TEMPERATURE_ERROR_ALLOWED > 1
          else
            consecutive_low_temperature_error[e] = 0;
        #endif
      }
    }

  #endif // HAS_HOTEND

  #define TP_CMP(S,A,B) (TEMPDIR(S) < 0 ? ((A)<(B)) : ((A)>(B)))
  #if ENABLED(THERMAL_PROTECTION_BED)
    if (TP_CMP(BED, temp_bed.raw, maxtemp_raw_BED)) max_temp_error(H_BED);
    if (temp_bed.target > 0 && TP_CMP(BED, mintemp_raw_BED, temp_bed.raw)) min_temp_error(H_BED);
  #endif

  #if BOTH(HAS_HEATED_CHAMBER, THERMAL_PROTECTION_CHAMBER)
    if (TP_CMP(CHAMBER, temp_chamber.raw, maxtemp_raw_CHAMBER)) max_temp_error(H_CHAMBER);
    if (temp_chamber.target > 0 && TP_CMP(CHAMBER, mintemp_raw_CHAMBER, temp_chamber.raw)) min_temp_error(H_CHAMBER);
  #endif

  #if BOTH(HAS_COOLER, THERMAL_PROTECTION_COOLER)
    if (cutter.unitPower > 0 && TP_CMP(COOLER, temp_cooler.raw, maxtemp_raw_COOLER)) max_temp_error(H_COOLER);
    if (TP_CMP(COOLER, mintemp_raw_COOLER, temp_cooler.raw)) min_temp_error(H_COOLER);
  #endif

  #if BOTH(HAS_TEMP_BOARD, THERMAL_PROTECTION_BOARD)
    if (TP_CMP(BOARD, temp_board.raw, maxtemp_raw_BOARD)) max_temp_error(H_BOARD);
    if (TP_CMP(BOARD, mintemp_raw_BOARD, temp_board.raw)) min_temp_error(H_BOARD);
  #endif
  #undef TP_CMP

} // Temperature::updateTemperaturesFromRawValues

/**
 * Initialize the temperature manager
 *
 * The manager is implemented by periodic calls to manage_heater()
 *
 *  - Init (and disable) SPI thermocouples like MAX6675 and MAX31865
 *  - Disable RUMBA JTAG to accommodate a thermocouple extension
 *  - Read-enable thermistors with a read-enable pin
 *  - Init HEATER and COOLER pins for OUTPUT in OFF state
 *  - Init the FAN pins as PWM or OUTPUT
 *  - Init the SPI interface for SPI thermocouples
 *  - Init ADC according to the HAL
 *  - Set thermistor pins to analog inputs according to the HAL
 *  - Start the Temperature ISR timer
 *  - Init the AUTO FAN pins as PWM or OUTPUT
 *  - Wait 250ms for temperatures to settle
 *  - Init temp_range[], used for catching min/maxtemp
 */
void Temperature::init() {

  TERN_(PROBING_HEATERS_OFF, paused_for_probing = false);

  #if BOTH(PIDTEMP, PID_EXTRUSION_SCALING)
    last_e_position = 0;
  #endif

  // Init (and disable) SPI thermocouples
  #if TEMP_SENSOR_IS_ANY_MAX_TC(0) && PIN_EXISTS(TEMP_0_CS)
    OUT_WRITE(TEMP_0_CS_PIN, HIGH);
  #endif
  #if TEMP_SENSOR_IS_ANY_MAX_TC(1) && PIN_EXISTS(TEMP_1_CS)
    OUT_WRITE(TEMP_1_CS_PIN, HIGH);
  #endif

  // Setup objects for library-based polling of MAX TCs
  #if HAS_MAXTC_LIBRARIES
    #define _MAX31865_WIRES(n) MAX31865_##n##WIRE
    #define MAX31865_WIRES(n) _MAX31865_WIRES(n)

    #if TEMP_SENSOR_IS_MAX(0, 6675) && HAS_MAX6675_LIBRARY
      max6675_0.begin();
    #elif TEMP_SENSOR_IS_MAX(0, 31855) && HAS_MAX31855_LIBRARY
      max31855_0.begin();
    #elif TEMP_SENSOR_IS_MAX(0, 31865)
      max31865_0.begin(
        MAX31865_WIRES(MAX31865_SENSOR_WIRES_0) // MAX31865_2WIRE, MAX31865_3WIRE, MAX31865_4WIRE
        OPTARG(LIB_INTERNAL_MAX31865, MAX31865_SENSOR_OHMS_0, MAX31865_CALIBRATION_OHMS_0)
      );
      #if defined(LIB_INTERNAL_MAX31865) && ENABLED(MAX31865_50HZ_FILTER)
        max31865_0.enable50HzFilter(1);
      #endif
    #endif

    #if TEMP_SENSOR_IS_MAX(1, 6675) && HAS_MAX6675_LIBRARY
      max6675_1.begin();
    #elif TEMP_SENSOR_IS_MAX(1, 31855) && HAS_MAX31855_LIBRARY
      max31855_1.begin();
    #elif TEMP_SENSOR_IS_MAX(1, 31865)
      max31865_1.begin(
        MAX31865_WIRES(MAX31865_SENSOR_WIRES_1) // MAX31865_2WIRE, MAX31865_3WIRE, MAX31865_4WIRE
        OPTARG(LIB_INTERNAL_MAX31865, MAX31865_SENSOR_OHMS_1, MAX31865_CALIBRATION_OHMS_1)
      );
      #if defined(LIB_INTERNAL_MAX31865) && ENABLED(MAX31865_50HZ_FILTER)
        max31865_1.enable50HzFilter(1);
      #endif
    #endif
    #undef MAX31865_WIRES
    #undef _MAX31865_WIRES
  #endif

  #if MB(RUMBA)
    // Disable RUMBA JTAG in case the thermocouple extension is plugged on top of JTAG connector
    #define _AD(N) (TEMP_SENSOR_##N##_IS_AD595 || TEMP_SENSOR_##N##_IS_AD8495)
    #if _AD(0) || _AD(1) || _AD(2) || _AD(BED) || _AD(CHAMBER) || _AD(REDUNDANT)
      MCUCR = _BV(JTD);
      MCUCR = _BV(JTD);
    #endif
  #endif

  // Thermistor activation by MCU pin
  #if PIN_EXISTS(TEMP_0_TR_ENABLE)
    OUT_WRITE(TEMP_0_TR_ENABLE_PIN,
      #if TEMP_SENSOR_IS_ANY_MAX_TC(0)
        1
      #else
        0
      #endif
    );
  #endif
  #if PIN_EXISTS(TEMP_1_TR_ENABLE)
    OUT_WRITE(TEMP_1_TR_ENABLE_PIN,
      #if TEMP_SENSOR_IS_ANY_MAX_TC(1)
        1
      #else
        0
      #endif
    );
  #endif

  #if HAS_HEATER_0
    #ifdef BOARD_OPENDRAIN_MOSFETS
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
    #ifdef BOARD_OPENDRAIN_MOSFETS
      OUT_WRITE_OD(HEATER_BED_PIN, HEATER_BED_INVERTING);
    #else
      OUT_WRITE(HEATER_BED_PIN, HEATER_BED_INVERTING);
    #endif
  #endif

  #if HAS_HEATED_CHAMBER
    OUT_WRITE(HEATER_CHAMBER_PIN, HEATER_CHAMBER_INVERTING);
  #endif

  #if HAS_COOLER
    OUT_WRITE(COOLER_PIN, COOLER_INVERTING);
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

  TERN_(HAS_MAXTC_SW_SPI, max_tc_spi.init());

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
  #if HAS_TEMP_ADC_COOLER
    HAL_ANALOG_SELECT(TEMP_COOLER_PIN);
  #endif
  #if HAS_TEMP_ADC_PROBE
    HAL_ANALOG_SELECT(TEMP_PROBE_PIN);
  #endif
  #if HAS_TEMP_ADC_BOARD
    HAL_ANALOG_SELECT(TEMP_BOARD_PIN);
  #endif
  #if HAS_TEMP_ADC_REDUNDANT
    HAL_ANALOG_SELECT(TEMP_REDUNDANT_PIN);
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

  #if HAS_HOTEND
    #define _TEMP_MIN_E(NR) do{ \
      const celsius_t tmin = _MAX(HEATER_##NR##_MINTEMP, TERN(TEMP_SENSOR_##NR##_IS_CUSTOM, 0, (int)pgm_read_word(&TEMPTABLE_##NR [TEMP_SENSOR_##NR##_MINTEMP_IND].celsius))); \
      temp_range[NR].mintemp = tmin; \
      while (analog_to_celsius_hotend(temp_range[NR].raw_min, NR) < tmin) \
        temp_range[NR].raw_min += TEMPDIR(NR) * (OVERSAMPLENR); \
    }while(0)
    #define _TEMP_MAX_E(NR) do{ \
      const celsius_t tmax = _MIN(HEATER_##NR##_MAXTEMP, TERN(TEMP_SENSOR_##NR##_IS_CUSTOM, 2000, (int)pgm_read_word(&TEMPTABLE_##NR [TEMP_SENSOR_##NR##_MAXTEMP_IND].celsius) - 1)); \
      temp_range[NR].maxtemp = tmax; \
      while (analog_to_celsius_hotend(temp_range[NR].raw_max, NR) > tmax) \
        temp_range[NR].raw_max -= TEMPDIR(NR) * (OVERSAMPLENR); \
    }while(0)

    #define _MINMAX_TEST(N,M) (HOTENDS > N && TEMP_SENSOR_##N > 0 && TEMP_SENSOR_##N != 998 && TEMP_SENSOR_##N != 999 && defined(HEATER_##N##_##M##TEMP))

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

  // TODO: combine these into the macros above
  #if HAS_HEATED_BED
    while (analog_to_celsius_bed(mintemp_raw_BED) < BED_MINTEMP) mintemp_raw_BED += TEMPDIR(BED) * (OVERSAMPLENR);
    while (analog_to_celsius_bed(maxtemp_raw_BED) > BED_MAXTEMP) maxtemp_raw_BED -= TEMPDIR(BED) * (OVERSAMPLENR);
  #endif

  #if HAS_HEATED_CHAMBER
    while (analog_to_celsius_chamber(mintemp_raw_CHAMBER) < CHAMBER_MINTEMP) mintemp_raw_CHAMBER += TEMPDIR(CHAMBER) * (OVERSAMPLENR);
    while (analog_to_celsius_chamber(maxtemp_raw_CHAMBER) > CHAMBER_MAXTEMP) maxtemp_raw_CHAMBER -= TEMPDIR(CHAMBER) * (OVERSAMPLENR);
  #endif

  #if HAS_COOLER
    while (analog_to_celsius_cooler(mintemp_raw_COOLER) > COOLER_MINTEMP) mintemp_raw_COOLER += TEMPDIR(COOLER) * (OVERSAMPLENR);
    while (analog_to_celsius_cooler(maxtemp_raw_COOLER) < COOLER_MAXTEMP) maxtemp_raw_COOLER -= TEMPDIR(COOLER) * (OVERSAMPLENR);
  #endif

  #if BOTH(HAS_TEMP_BOARD, THERMAL_PROTECTION_BOARD)
    while (analog_to_celsius_board(mintemp_raw_BOARD) < BOARD_MINTEMP) mintemp_raw_BOARD += TEMPDIR(BOARD) * (OVERSAMPLENR);
    while (analog_to_celsius_board(maxtemp_raw_BOARD) > BOARD_MAXTEMP) maxtemp_raw_BOARD -= TEMPDIR(BOARD) * (OVERSAMPLENR);
  #endif

  #if HAS_TEMP_REDUNDANT
    temp_redundant.target = &(
      #if REDUNDANT_TEMP_MATCH(TARGET, COOLER) && HAS_TEMP_COOLER
        temp_cooler
      #elif REDUNDANT_TEMP_MATCH(TARGET, PROBE) && HAS_TEMP_PROBE
        temp_probe
      #elif REDUNDANT_TEMP_MATCH(TARGET, BOARD) && HAS_TEMP_BOARD
        temp_board
      #elif REDUNDANT_TEMP_MATCH(TARGET, CHAMBER) && HAS_TEMP_CHAMBER
        temp_chamber
      #elif REDUNDANT_TEMP_MATCH(TARGET, BED) && HAS_TEMP_BED
        temp_bed
      #else
        temp_hotend[HEATER_ID(TEMP_SENSOR_REDUNDANT_TARGET)]
      #endif
    );
  #endif
}

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
  void Temperature::tr_state_machine_t::run(const_celsius_float_t current, const_celsius_float_t target, const heater_id_t heater_id, const uint16_t period_seconds, const celsius_t hysteresis_degc) {

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
    */

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

  // Disable autotemp, unpause and reset everything
  TERN_(AUTOTEMP, planner.autotemp_enabled = false);
  TERN_(PROBING_HEATERS_OFF, pause_heaters(false));

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

  #if HAS_COOLER
    setTargetCooler(0);
    temp_cooler.soft_pwm_amount = 0;
    WRITE_HEATER_COOLER(LOW);
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

#endif // PRINTJOB_TIMER_AUTOSTART

#if ENABLED(PROBING_HEATERS_OFF)

  void Temperature::pause_heaters(const bool p) {
    if (p != paused_for_probing) {
      paused_for_probing = p;
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

#if EITHER(SINGLENOZZLE_STANDBY_TEMP, SINGLENOZZLE_STANDBY_FAN)

  void Temperature::singlenozzle_change(const uint8_t old_tool, const uint8_t new_tool) {
    #if ENABLED(SINGLENOZZLE_STANDBY_FAN)
      singlenozzle_fan_speed[old_tool] = fan_speed[0];
      fan_speed[0] = singlenozzle_fan_speed[new_tool];
    #endif
    #if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
      singlenozzle_temp[old_tool] = temp_hotend[0].target;
      if (singlenozzle_temp[new_tool] && singlenozzle_temp[new_tool] != singlenozzle_temp[old_tool]) {
        setTargetHotend(singlenozzle_temp[new_tool], 0);
        TERN_(AUTOTEMP, planner.autotemp_update());
        set_heating_message(0);
        (void)wait_for_hotend(0, false);  // Wait for heating or cooling
      }
    #endif
  }

#endif // SINGLENOZZLE_STANDBY_TEMP || SINGLENOZZLE_STANDBY_FAN

#if HAS_MAX_TC

  #ifndef THERMOCOUPLE_MAX_ERRORS
    #define THERMOCOUPLE_MAX_ERRORS 15
  #endif

  /**
   * @brief Read MAX Thermocouple temperature.
   *
   * Reads the thermocouple board via HW or SW SPI, using a library (LIB_USR_x) or raw SPI reads.
   * Doesn't strictly return a temperature; returns an "ADC Value" (i.e. raw register content).
   *
   * @param  hindex  the hotend we're referencing (if MULTI_MAX_TC)
   * @return         integer representing the board's buffer, to be converted later if needed
   */
  int16_t Temperature::read_max_tc(TERN_(HAS_MULTI_MAX_TC, const uint8_t hindex/*=0*/)) {
    #define MAXTC_HEAT_INTERVAL 250UL

    #if HAS_MAX31855
      #define MAX_TC_ERROR_MASK    7        // D2-0: SCV, SCG, OC
      #define MAX_TC_DISCARD_BITS 18        // Data D31-18; sign bit D31
      #define MAX_TC_SPEED_BITS    3        // ~1MHz
    #elif HAS_MAX31865
      #define MAX_TC_ERROR_MASK    1        // D0 Bit on fault only
      #define MAX_TC_DISCARD_BITS  1        // Data is in D15-D1
      #define MAX_TC_SPEED_BITS    3        // ~1MHz
    #else // MAX6675
      #define MAX_TC_ERROR_MASK    3        // D2 only; 1 = open circuit
      #define MAX_TC_DISCARD_BITS  3        // Data D15-D1
      #define MAX_TC_SPEED_BITS    2        // ~2MHz
    #endif

    #if HAS_MULTI_MAX_TC
      // Needed to return the correct temp when this is called between readings
      static int16_t max_tc_temp_previous[MAX_TC_COUNT] = { 0 };
      #define THERMO_TEMP(I) max_tc_temp_previous[I]
      #define THERMO_SEL(A,B) (hindex ? (B) : (A))
      #define MAXTC_CS_WRITE(V) do{ switch (hindex) { case 1: WRITE(TEMP_1_CS_PIN, V); break; default: WRITE(TEMP_0_CS_PIN, V); } }while(0)
    #else
      // When we have only 1 max tc, THERMO_SEL will pick the appropriate sensor
      // variable, and MAXTC_*() macros will be hardcoded to the correct CS pin.
      constexpr uint8_t hindex = 0;
      #define THERMO_TEMP(I) max_tc_temp
      #if TEMP_SENSOR_IS_ANY_MAX_TC(0)
        #define THERMO_SEL(A,B) A
        #define MAXTC_CS_WRITE(V)  WRITE(TEMP_0_CS_PIN, V)
      #else
        #define THERMO_SEL(A,B) B
        #define MAXTC_CS_WRITE(V)  WRITE(TEMP_1_CS_PIN, V)
      #endif
    #endif

    static TERN(HAS_MAX31855, uint32_t, uint16_t) max_tc_temp = THERMO_SEL(
      TEMP_SENSOR_0_MAX_TC_TMAX,
      TEMP_SENSOR_1_MAX_TC_TMAX
    );

    static uint8_t max_tc_errors[MAX_TC_COUNT] = { 0 };
    static millis_t next_max_tc_ms[MAX_TC_COUNT] = { 0 };

    // Return last-read value between readings
    millis_t ms = millis();
    if (PENDING(ms, next_max_tc_ms[hindex]))
      return (int16_t)THERMO_TEMP(hindex);

    next_max_tc_ms[hindex] = ms + MAXTC_HEAT_INTERVAL;

    #if !HAS_MAXTC_LIBRARIES
      max_tc_temp = 0;

      #if !HAS_MAXTC_SW_SPI
        // Initialize SPI using the default Hardware SPI bus.
        // FIXME: spiBegin, spiRec and spiInit doesn't work when soft spi is used.
        spiBegin();
        spiInit(MAX_TC_SPEED_BITS);
      #endif

      MAXTC_CS_WRITE(LOW);  // enable MAXTC
      DELAY_NS(100);        // Ensure 100ns delay

      // Read a big-endian temperature value without using a library
      for (uint8_t i = sizeof(max_tc_temp); i--;) {
        max_tc_temp |= TERN(HAS_MAXTC_SW_SPI, max_tc_spi.receive(), spiRec());
        if (i > 0) max_tc_temp <<= 8; // shift left if not the last byte
      }

      MAXTC_CS_WRITE(HIGH);  // disable MAXTC
    #else
      #if HAS_MAX6675_LIBRARY
        MAX6675 &max6675ref = THERMO_SEL(max6675_0, max6675_1);
        max_tc_temp = max6675ref.readRaw16();
      #endif

      #if HAS_MAX31855_LIBRARY
        MAX31855 &max855ref = THERMO_SEL(max31855_0, max31855_1);
        max_tc_temp = max855ref.readRaw32();
      #endif

      #if HAS_MAX31865
        MAX31865 &max865ref = THERMO_SEL(max31865_0, max31865_1);
        max_tc_temp = TERN(LIB_INTERNAL_MAX31865, max865ref.readRaw(), max865ref.readRTD_with_Fault());
      #endif
    #endif

    // Handle an error. If there have been more than THERMOCOUPLE_MAX_ERRORS, send an error over serial.
    // Either way, return the TMAX for the thermocouple to trigger a max_temp_error()
    if (max_tc_temp & MAX_TC_ERROR_MASK) {
      max_tc_errors[hindex]++;

      if (max_tc_errors[hindex] > THERMOCOUPLE_MAX_ERRORS) {
        SERIAL_ERROR_START();
        SERIAL_ECHOPGM("Temp measurement error! ");
        #if HAS_MAX31855
          SERIAL_ECHOPAIR("MAX31855 Fault: (", max_tc_temp & 0x7, ") >> ");
          if (max_tc_temp & 0x1)
            SERIAL_ECHOLNPGM("Open Circuit");
          else if (max_tc_temp & 0x2)
            SERIAL_ECHOLNPGM("Short to GND");
          else if (max_tc_temp & 0x4)
            SERIAL_ECHOLNPGM("Short to VCC");
        #elif HAS_MAX31865
          const uint8_t fault_31865 = max865ref.readFault();
          max865ref.clearFault();
          if (fault_31865) {
            SERIAL_EOL();
            SERIAL_ECHOLNPAIR("\nMAX31865 Fault: (", fault_31865, ")  >>");
            if (fault_31865 & MAX31865_FAULT_HIGHTHRESH)
              SERIAL_ECHOLNPGM("RTD High Threshold");
            if (fault_31865 & MAX31865_FAULT_LOWTHRESH)
              SERIAL_ECHOLNPGM("RTD Low Threshold");
            if (fault_31865 & MAX31865_FAULT_REFINLOW)
              SERIAL_ECHOLNPGM("REFIN- > 0.85 x V bias");
            if (fault_31865 & MAX31865_FAULT_REFINHIGH)
              SERIAL_ECHOLNPGM("REFIN- < 0.85 x V bias (FORCE- open)");
            if (fault_31865 & MAX31865_FAULT_RTDINLOW)
              SERIAL_ECHOLNPGM("REFIN- < 0.85 x V bias (FORCE- open)");
            if (fault_31865 & MAX31865_FAULT_OVUV)
              SERIAL_ECHOLNPGM("Under/Over voltage");
          }
        #else // MAX6675
          SERIAL_ECHOLNPGM("MAX6675 Fault: Open Circuit");
        #endif

        // Set thermocouple above max temperature (TMAX)
        max_tc_temp = THERMO_SEL(TEMP_SENSOR_0_MAX_TC_TMAX, TEMP_SENSOR_1_MAX_TC_TMAX) << (MAX_TC_DISCARD_BITS + 1);
      }
    }
    else {
      max_tc_errors[hindex] = 0; // No error bit, reset error count
    }

    max_tc_temp >>= MAX_TC_DISCARD_BITS;

    #if HAS_MAX31855
      // Support negative temperature for MAX38155
      if (max_tc_temp & 0x00002000) max_tc_temp |= 0xFFFFC000;
    #endif

    THERMO_TEMP(hindex) = max_tc_temp;

    return (int16_t)max_tc_temp;
  }

#endif // HAS_MAX_TC

/**
 * Update raw temperatures
 *
 * Called by ISR => readings_ready when new temperatures have been set by updateTemperaturesFromRawValues.
 * Applies all the accumulators to the current raw temperatures.
 */
void Temperature::update_raw_temperatures() {

  // TODO: can this be collapsed into a HOTEND_LOOP()?
  #if HAS_TEMP_ADC_0 && !TEMP_SENSOR_0_IS_MAX_TC
    temp_hotend[0].update();
  #endif

  #if HAS_TEMP_ADC_1 && !TEMP_SENSOR_1_IS_MAX_TC
    temp_hotend[1].update();
  #endif

  #if HAS_TEMP_ADC_REDUNDANT && !TEMP_SENSOR_REDUNDANT_IS_MAX_TC
    temp_redundant.update();
  #endif

  TERN_(HAS_TEMP_ADC_2,       temp_hotend[2].update());
  TERN_(HAS_TEMP_ADC_3,       temp_hotend[3].update());
  TERN_(HAS_TEMP_ADC_4,       temp_hotend[4].update());
  TERN_(HAS_TEMP_ADC_5,       temp_hotend[5].update());
  TERN_(HAS_TEMP_ADC_6,       temp_hotend[6].update());
  TERN_(HAS_TEMP_ADC_7,       temp_hotend[7].update());
  TERN_(HAS_TEMP_ADC_BED,     temp_bed.update());
  TERN_(HAS_TEMP_ADC_CHAMBER, temp_chamber.update());
  TERN_(HAS_TEMP_ADC_PROBE,   temp_probe.update());
  TERN_(HAS_TEMP_ADC_BOARD,   temp_board.update());
  TERN_(HAS_TEMP_ADC_COOLER,  temp_cooler.update());

  TERN_(HAS_JOY_ADC_X, joystick.x.update());
  TERN_(HAS_JOY_ADC_Y, joystick.y.update());
  TERN_(HAS_JOY_ADC_Z, joystick.z.update());
}

/**
 * Called by the Temperature ISR when all the ADCs have been processed.
 * Reset all the ADC accumulators for another round of updates.
 */
void Temperature::readings_ready() {

  // Update raw values only if they're not already set.
  if (!raw_temps_ready) {
    update_raw_temperatures();
    raw_temps_ready = true;
  }

  // Filament Sensor - can be read any time since IIR filtering is used
  TERN_(FILAMENT_WIDTH_SENSOR, filwidth.reading_ready());

  #if HAS_HOTEND
    HOTEND_LOOP() temp_hotend[e].reset();
  #endif

  TERN_(HAS_HEATED_BED,     temp_bed.reset());
  TERN_(HAS_TEMP_CHAMBER,   temp_chamber.reset());
  TERN_(HAS_TEMP_PROBE,     temp_probe.reset());
  TERN_(HAS_TEMP_COOLER,    temp_cooler.reset());
  TERN_(HAS_TEMP_BOARD,     temp_board.reset());
  TERN_(HAS_TEMP_REDUNDANT, temp_redundant.reset());

  TERN_(HAS_JOY_ADC_X, joystick.x.reset());
  TERN_(HAS_JOY_ADC_Y, joystick.y.reset());
  TERN_(HAS_JOY_ADC_Z, joystick.z.reset());
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
 *  - Call planner.isr to count down its "ignore" time
 */
HAL_TEMP_TIMER_ISR() {
  HAL_timer_isr_prologue(TEMP_TIMER_NUM);

  Temperature::isr();

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
void Temperature::isr() {

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

  #if HAS_COOLER
    static SoftPWM soft_pwm_cooler;
  #endif

  #define WRITE_FAN(n, v) WRITE(FAN##n##_PIN, (v) ^ FAN_INVERTING)

  #if DISABLED(SLOW_PWM_HEATERS)

    #if ANY(HAS_HOTEND, HAS_HEATED_BED, HAS_HEATED_CHAMBER, HAS_COOLER, FAN_SOFT_PWM)
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
        _PWM_MOD(BED, soft_pwm_bed, temp_bed);
      #endif

      #if HAS_HEATED_CHAMBER
        _PWM_MOD(CHAMBER, soft_pwm_chamber, temp_chamber);
      #endif

      #if HAS_COOLER
        _PWM_MOD(COOLER, soft_pwm_cooler, temp_cooler);
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

      #if HAS_COOLER
        _PWM_LOW(COOLER, soft_pwm_cooler);
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

      #if HAS_COOLER
        _SLOW_PWM(COOLER, soft_pwm_cooler, temp_cooler);
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

    #if HAS_COOLER
      _PWM_OFF(COOLER, soft_pwm_cooler, temp_cooler);
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
      TERN_(HAS_COOLER, soft_pwm_cooler.dec());
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

    #if HAS_TEMP_ADC_COOLER
      case PrepareTemp_COOLER: HAL_START_ADC(TEMP_COOLER_PIN); break;
      case MeasureTemp_COOLER: ACCUMULATE_ADC(temp_cooler); break;
    #endif

    #if HAS_TEMP_ADC_PROBE
      case PrepareTemp_PROBE: HAL_START_ADC(TEMP_PROBE_PIN); break;
      case MeasureTemp_PROBE: ACCUMULATE_ADC(temp_probe); break;
    #endif

    #if HAS_TEMP_ADC_BOARD
      case PrepareTemp_BOARD: HAL_START_ADC(TEMP_BOARD_PIN); break;
      case MeasureTemp_BOARD: ACCUMULATE_ADC(temp_board); break;
    #endif

    #if HAS_TEMP_ADC_REDUNDANT
      case PrepareTemp_REDUNDANT: HAL_START_ADC(TEMP_REDUNDANT_PIN); break;
      case MeasureTemp_REDUNDANT: ACCUMULATE_ADC(temp_redundant); break;
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

  // Periodically call the planner timer service routine
  planner.isr();
}

#if HAS_TEMP_SENSOR

  #include "../gcode/gcode.h"

  /**
   * Print a single heater state in the form:
   *        Bed: " B:nnn.nn /nnn.nn"
   *    Chamber: " C:nnn.nn /nnn.nn"
   *      Probe: " P:nnn.nn /nnn.nn"
   *     Cooler: " L:nnn.nn /nnn.nn"
   *  Redundant: " R:nnn.nn /nnn.nn"
   *   Extruder: " T0:nnn.nn /nnn.nn"
   *   With ADC: " T0:nnn.nn /nnn.nn (nnn.nn)"
   */
  static void print_heater_state(const heater_id_t e, const_celsius_float_t c, const_celsius_float_t t
    OPTARG(SHOW_TEMP_ADC_VALUES, const float r)
  ) {
    char k;
    switch (e) {
      default:
        #if HAS_TEMP_HOTEND
          k = 'T'; break;
        #endif
      #if HAS_TEMP_BED
        case H_BED: k = 'B'; break;
      #endif
      #if HAS_TEMP_CHAMBER
        case H_CHAMBER: k = 'C'; break;
      #endif
      #if HAS_TEMP_PROBE
        case H_PROBE: k = 'P'; break;
      #endif
      #if HAS_TEMP_COOLER
        case H_COOLER: k = 'L'; break;
      #endif
      #if HAS_TEMP_BOARD
        case H_BOARD: k = 'M'; break;
      #endif
      #if HAS_TEMP_REDUNDANT
        case H_REDUNDANT: k = 'R'; break;
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
      // Temperature MAX SPI boards do not have an OVERSAMPLENR defined
      SERIAL_ECHOPAIR(" (", TERN(HAS_MAXTC_LIBRARIES, k == 'T', false) ? r : r * RECIPROCAL(OVERSAMPLENR));
      SERIAL_CHAR(')');
    #endif
    delay(2);
  }

  void Temperature::print_heater_states(const uint8_t target_extruder
    OPTARG(HAS_TEMP_REDUNDANT, const bool include_r/*=false*/)
  ) {
    #if HAS_TEMP_HOTEND
      print_heater_state(H_E0, degHotend(target_extruder), degTargetHotend(target_extruder) OPTARG(SHOW_TEMP_ADC_VALUES, rawHotendTemp(target_extruder)));
    #endif
    #if HAS_HEATED_BED
      print_heater_state(H_BED, degBed(), degTargetBed() OPTARG(SHOW_TEMP_ADC_VALUES, rawBedTemp()));
    #endif
    #if HAS_TEMP_CHAMBER
      print_heater_state(H_CHAMBER, degChamber(), TERN0(HAS_HEATED_CHAMBER, degTargetChamber()) OPTARG(SHOW_TEMP_ADC_VALUES, rawChamberTemp()));
    #endif
    #if HAS_TEMP_COOLER
      print_heater_state(H_COOLER, degCooler(), TERN0(HAS_COOLER, degTargetCooler()) OPTARG(SHOW_TEMP_ADC_VALUES, rawCoolerTemp()));
    #endif
    #if HAS_TEMP_PROBE
      print_heater_state(H_PROBE, degProbe(), 0 OPTARG(SHOW_TEMP_ADC_VALUES, rawProbeTemp()));
    #endif
    #if HAS_TEMP_BOARD
      print_heater_state(H_BOARD, degBoard(), 0 OPTARG(SHOW_TEMP_ADC_VALUES, rawBoardTemp()));
    #endif
    #if HAS_TEMP_REDUNDANT
      if (include_r) print_heater_state(H_REDUNDANT, degRedundant(), degRedundantTarget() OPTARG(SHOW_TEMP_ADC_VALUES, rawRedundantTemp()));
    #endif
    #if HAS_MULTI_HOTEND
      HOTEND_LOOP() print_heater_state((heater_id_t)e, degHotend(e), degTargetHotend(e) OPTARG(SHOW_TEMP_ADC_VALUES, rawHotendTemp(e)));
    #endif
    SERIAL_ECHOPAIR(" @:", getHeaterPower((heater_id_t)target_extruder));
    #if HAS_HEATED_BED
      SERIAL_ECHOPAIR(" B@:", getHeaterPower(H_BED));
    #endif
    #if HAS_HEATED_CHAMBER
      SERIAL_ECHOPAIR(" C@:", getHeaterPower(H_CHAMBER));
    #endif
    #if HAS_COOLER
      SERIAL_ECHOPAIR(" C@:", getHeaterPower(H_COOLER));
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
    AutoReporter<Temperature::AutoReportTemp> Temperature::auto_reporter;
    void Temperature::AutoReportTemp::report() { print_heater_states(active_extruder); SERIAL_EOL(); }
  #endif

  #if HAS_HOTEND && HAS_STATUS_MESSAGE
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
      OPTARG(G26_CLICK_CAN_CANCEL, const bool click_to_cancel/*=false*/)
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
        const celsius_float_t start_temp = degHotend(target_extruder);
        printerEventLEDs.onHotendHeatingStart();
      #endif

      bool wants_to_cool = false;
      celsius_float_t target_temp = -1.0, old_temp = 9999.0;
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

        const celsius_float_t temp = degHotend(target_extruder);

        #if ENABLED(PRINTER_EVENT_LEDS)
          // Gradually change LED strip from violet to red as nozzle heats up
          if (!wants_to_cool) printerEventLEDs.onHotendHeating(start_temp, temp, target_temp);
        #endif

        #if TEMP_RESIDENCY_TIME > 0

          const celsius_float_t temp_diff = ABS(target_temp - temp);

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
            TERN_(HAS_LCD_MENU, ui.quick_feedback());
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

    #if ENABLED(WAIT_FOR_HOTEND)
      void Temperature::wait_for_hotend_heating(const uint8_t target_extruder) {
        if (isHeatingHotend(target_extruder)) {
          SERIAL_ECHOLNPGM("Wait for hotend heating...");
          LCD_MESSAGEPGM(MSG_HEATING);
          wait_for_hotend(target_extruder);
          ui.reset_status();
        }
      }
    #endif

  #endif // HAS_TEMP_HOTEND

  #if HAS_HEATED_BED

    #ifndef MIN_COOLING_SLOPE_DEG_BED
      #define MIN_COOLING_SLOPE_DEG_BED 1.00
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_BED
      #define MIN_COOLING_SLOPE_TIME_BED 60
    #endif

    bool Temperature::wait_for_bed(const bool no_wait_for_cooling/*=true*/
      OPTARG(G26_CLICK_CAN_CANCEL, const bool click_to_cancel/*=false*/)
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
        const celsius_float_t start_temp = degBed();
        printerEventLEDs.onBedHeatingStart();
      #endif

      bool wants_to_cool = false;
      celsius_float_t target_temp = -1, old_temp = 9999;
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

        const celsius_float_t temp = degBed();

        #if ENABLED(PRINTER_EVENT_LEDS)
          // Gradually change LED strip from blue to violet as bed heats up
          if (!wants_to_cool) printerEventLEDs.onBedHeating(start_temp, temp, target_temp);
        #endif

        #if TEMP_BED_RESIDENCY_TIME > 0

          const celsius_float_t temp_diff = ABS(target_temp - temp);

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
            TERN_(HAS_LCD_MENU, ui.quick_feedback());
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

    bool Temperature::wait_for_probe(const celsius_t target_temp, bool no_wait_for_cooling/*=true*/) {

      const bool wants_to_cool = isProbeAboveTemp(target_temp),
                 will_wait = !(wants_to_cool && no_wait_for_cooling);
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

  #if HAS_COOLER

    #ifndef MIN_COOLING_SLOPE_DEG_COOLER
      #define MIN_COOLING_SLOPE_DEG_COOLER 1.50
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_COOLER
      #define MIN_COOLING_SLOPE_TIME_COOLER 120
    #endif

    bool Temperature::wait_for_cooler(const bool no_wait_for_cooling/*=true*/) {

      #if TEMP_COOLER_RESIDENCY_TIME > 0
        millis_t residency_start_ms = 0;
        bool first_loop = true;
        // Loop until the temperature has stabilized
        #define TEMP_COOLER_CONDITIONS (!residency_start_ms || PENDING(now, residency_start_ms + SEC_TO_MS(TEMP_COOLER_RESIDENCY_TIME)))
      #else
        // Loop until the temperature is very close target
        #define TEMP_COOLER_CONDITIONS (wants_to_cool ? isLaserHeating() : isLaserCooling())
      #endif

      #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
        KEEPALIVE_STATE(NOT_BUSY);
      #endif

      bool wants_to_cool = false;
      float target_temp = -1, previous_temp = 9999;
      millis_t now, next_temp_ms = 0, next_cooling_check_ms = 0;
      wait_for_heatup = true;
      do {
        // Target temperature might be changed during the loop
        if (target_temp != degTargetCooler()) {
          wants_to_cool = isLaserHeating();
          target_temp = degTargetCooler();

          // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
          if (no_wait_for_cooling && wants_to_cool) break;
        }

        now = millis();
        if (ELAPSED(now, next_temp_ms)) { // Print Temp Reading every 1 second while heating up.
          next_temp_ms = now + 1000UL;
          print_heater_states(active_extruder);
          #if TEMP_COOLER_RESIDENCY_TIME > 0
            SERIAL_ECHOPGM(" W:");
            if (residency_start_ms)
              SERIAL_ECHO(long((SEC_TO_MS(TEMP_COOLER_RESIDENCY_TIME) - (now - residency_start_ms)) / 1000UL));
            else
              SERIAL_CHAR('?');
          #endif
          SERIAL_EOL();
        }

        idle();
        gcode.reset_stepper_timeout(); // Keep steppers powered

        const celsius_float_t current_temp = degCooler();

        #if TEMP_COOLER_RESIDENCY_TIME > 0

          const celsius_float_t temp_diff = ABS(target_temp - temp);

          if (!residency_start_ms) {
            // Start the TEMP_COOLER_RESIDENCY_TIME timer when we reach target temp for the first time.
            if (temp_diff < TEMP_COOLER_WINDOW)
              residency_start_ms = now + (first_loop ? SEC_TO_MS(TEMP_COOLER_RESIDENCY_TIME) / 3 : 0);
          }
          else if (temp_diff > TEMP_COOLER_HYSTERESIS) {
            // Restart the timer whenever the temperature falls outside the hysteresis.
            residency_start_ms = now;
          }

          first_loop = false;
        #endif // TEMP_COOLER_RESIDENCY_TIME > 0

        if (wants_to_cool) {
          // Break after MIN_COOLING_SLOPE_TIME_CHAMBER seconds
          // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG_CHAMBER
          if (!next_cooling_check_ms || ELAPSED(now, next_cooling_check_ms)) {
            if (previous_temp - current_temp < float(MIN_COOLING_SLOPE_DEG_COOLER)) break;
            next_cooling_check_ms = now + SEC_TO_MS(MIN_COOLING_SLOPE_TIME_COOLER);
            previous_temp = current_temp;
          }
        }

      } while (wait_for_heatup && TEMP_COOLER_CONDITIONS);

      // Prevent a wait-forever situation if R is misused i.e. M191 R0
      if (wait_for_heatup) {
        wait_for_heatup = false;
        ui.reset_status();
        return true;
      }

      return false;
    }

  #endif // HAS_COOLER

#endif // HAS_TEMP_SENSOR
