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

#if EITHER(HAS_COOLER, LASER_COOLANT_FLOW_METER)
  #include "../feature/cooler.h"
  #include "../feature/spindle_laser.h"
#endif

#if ENABLED(EMERGENCY_PARSER)
  #include "motion.h"
#endif

#if ENABLED(DWIN_CREALITY_LCD)
  #include "../lcd/dwin/e3v2/dwin.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../feature/host_actions.h"
#endif

#if HAS_TEMP_SENSOR
  #include "../gcode/gcode.h"
#endif

// LIB_MAX31855 can be added to the build_flags in platformio.ini to use a user-defined library
#if LIB_USR_MAX31855
  #include <Adafruit_MAX31855.h>
  #if PIN_EXISTS(MAX31855_MISO) && PIN_EXISTS(MAX31855_SCK)
    #define MAX31855_USES_SW_SPI 1
  #endif
  #if TEMP_SENSOR_0_IS_MAX31855 && PIN_EXISTS(MAX31855_CS)
    #define HAS_MAX31855_TEMP 1
    Adafruit_MAX31855 max31855_0 = Adafruit_MAX31855(MAX31855_CS_PIN
      #if MAX31855_USES_SW_SPI
        , MAX31855_MISO_PIN, MAX31855_SCK_PIN  // For software SPI also set MISO/SCK
      #endif
      #if ENABLED(LARGE_PINMAP)
        , HIGH
      #endif
    );
  #endif
  #if TEMP_SENSOR_1_IS_MAX31855 && PIN_EXISTS(MAX31855_CS2)
    #define HAS_MAX31855_TEMP 1
    Adafruit_MAX31855 max31855_1 = Adafruit_MAX31855(MAX31855_CS2_PIN
      #if MAX31855_USES_SW_SPI
        , MAX31855_MISO_PIN, MAX31855_SCK_PIN  // For software SPI also set MISO/SCK
      #endif
      #if ENABLED(LARGE_PINMAP)
        , HIGH
      #endif
    );
  #endif
#endif

// LIB_MAX31865 can be added to the build_flags in platformio.ini to use a user-defined library.
// If LIB_MAX31865 is not on the build_flags then the Adafruit MAX31865 V1.1.0 library is used.
#if HAS_MAX31865
  #include <Adafruit_MAX31865.h>
  #ifndef MAX31865_MOSI_PIN
    #define MAX31865_MOSI_PIN SD_MOSI_PIN
  #endif
  #if PIN_EXISTS(MAX31865_MISO) && PIN_EXISTS(MAX31865_SCK)
    #define MAX31865_USES_SW_SPI 1
  #endif
  #if TEMP_SENSOR_0_IS_MAX31865 && PIN_EXISTS(MAX31865_CS)
    #define HAS_MAX31865_TEMP 1
      Adafruit_MAX31865 max31865_0 = Adafruit_MAX31865(MAX31865_CS_PIN
        #if MAX31865_USES_SW_SPI && PIN_EXISTS(MAX31865_MOSI)
          , MAX31865_MOSI_PIN, MAX31865_MISO_PIN, MAX31865_SCK_PIN  // For software SPI also set MOSI/MISO/SCK
        #endif
        #if ENABLED(LARGE_PINMAP)
          , HIGH
        #endif
      );
  #endif
  #if TEMP_SENSOR_1_IS_MAX31865 && PIN_EXISTS(MAX31865_CS2)
    #define HAS_MAX31865_TEMP 1
    Adafruit_MAX31865 max31865_1 = Adafruit_MAX31865(MAX31865_CS2_PIN
      #if MAX31865_USES_SW_SPI && PIN_EXISTS(MAX31865_MOSI)
        , MAX31865_MOSI_PIN, MAX31865_MISO_PIN, MAX31865_SCK_PIN  // For software SPI also set MOSI/MISO/SCK
      #endif
      #if ENABLED(LARGE_PINMAP)
        , HIGH
      #endif
    );
  #endif
#endif

// LIB_MAX6675 can be added to the build_flags in platformio.ini to use a user-defined library
#if LIB_USR_MAX6675
  #include <max6675.h>
  #if PIN_EXISTS(MAX6675_MISO) && PIN_EXISTS(MAX6675_SCK)
    #define MAX6675_USES_SW_SPI 1
  #endif
  #if TEMP_SENSOR_0_IS_MAX6675 && PIN_EXISTS(MAX6675_CS)
    #define HAS_MAX6675_TEMP 1
    MAX6675 max6675_0 = MAX6675(MAX6675_CS_PIN
      #if MAX6675_USES_SW_SPI
        , MAX6675_MISO_PIN, MAX6675_SCK_PIN   // For software SPI also set MISO/SCK
      #endif
      #if ENABLED(LARGE_PINMAP)
        , HIGH
      #endif
    );
  #endif
  #if TEMP_SENSOR_1_IS_MAX6675 && PIN_EXISTS(MAX6675_CS2)
    #define HAS_MAX6675_TEMP 1
    MAX6675 max6675_1 = MAX6675(MAX6675_CS2_PIN
      #if MAX6675_USES_SW_SPI
        , MAX6675_MISO_PIN, MAX6675_SCK_PIN   // For software SPI also set MISO/SCK
      #endif
      #if ENABLED(LARGE_PINMAP)
        , HIGH
      #endif
    );
  #endif
#endif

#if !HAS_MAX6675_TEMP && !HAS_MAX31855_TEMP && !HAS_MAX31865_TEMP
  #define NO_THERMO_TEMPS 1
#endif

#if (TEMP_SENSOR_0_IS_MAX_TC || TEMP_SENSOR_1_IS_MAX_TC) && PINS_EXIST(MAX6675_SCK, MAX6675_DO) && NO_THERMO_TEMPS
  #define THERMO_SEPARATE_SPI 1
#endif

#if THERMO_SEPARATE_SPI
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
  #include "servo.h"
#endif

#if ANY(TEMP_SENSOR_0_IS_THERMISTOR, TEMP_SENSOR_1_IS_THERMISTOR, TEMP_SENSOR_2_IS_THERMISTOR, TEMP_SENSOR_3_IS_THERMISTOR, \
        TEMP_SENSOR_4_IS_THERMISTOR, TEMP_SENSOR_5_IS_THERMISTOR, TEMP_SENSOR_6_IS_THERMISTOR, TEMP_SENSOR_7_IS_THERMISTOR )
  #define HAS_HOTEND_THERMISTOR 1
#endif

#if HAS_HOTEND_THERMISTOR
  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    static const temp_entry_t* heater_ttbl_map[2] = { TEMPTABLE_0, TEMPTABLE_1 };
    static constexpr uint8_t heater_ttbllen_map[2] = { TEMPTABLE_0_LEN, TEMPTABLE_1_LEN };
  #else
    #define NEXT_TEMPTABLE(N) ,TEMPTABLE_##N
    #define NEXT_TEMPTABLE_LEN(N) ,TEMPTABLE_##N##_LEN
    static const temp_entry_t* heater_ttbl_map[HOTENDS] = ARRAY_BY_HOTENDS(TEMPTABLE_0 REPEAT_S(1, HOTENDS, NEXT_TEMPTABLE));
    static constexpr uint8_t heater_ttbllen_map[HOTENDS] = ARRAY_BY_HOTENDS(TEMPTABLE_0_LEN REPEAT_S(1, HOTENDS, NEXT_TEMPTABLE_LEN));
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
#if HAS_COOLER
  #define _COOLER_PSTR(h) (h) == H_COOLER ? GET_TEXT(MSG_COOLER) :
#else
  #define _COOLER_PSTR(h)
#endif
#define _E_PSTR(h,N) ((HOTENDS) > N && (h) == N) ? PSTR(LCD_STR_E##N) :
#define HEATER_PSTR(h) _BED_PSTR(h) _CHAMBER_PSTR(h) _COOLER_PSTR(h) _E_PSTR(h,1) _E_PSTR(h,2) _E_PSTR(h,3) _E_PSTR(h,4) _E_PSTR(h,5) PSTR(LCD_STR_E0)

// public:

#if ENABLED(NO_FAN_SLOWING_IN_PID_TUNING)
  bool Temperature::adaptive_fan_slowing = true;
#endif

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

#if WATCH_HOTENDS
  // Can it be terned with a comma ?
  #define WatchHE , celsius_t(WATCH_TEMP_INCREASE).raw(), WATCH_TEMP_PERIOD
#else
  #define WatchHE
#endif

// The order of the declarations below must follow the heater_pos_t order
static Heater heaters[] = {
  #if HAS_HOTEND
    #ifndef MIN_COOLING_SLOPE_DEG
      #define MIN_COOLING_SLOPE_DEG 1.50
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME
      #define MIN_COOLING_SLOPE_TIME 60
    #endif


    #define DeclareHotEnd(N) \
      HeaterImpl<HEATER_## N ##_PIN, HEATER_## N ##_INVERTING, celsius_t(TEMP_HYSTERESIS).raw(), celsius_t(TEMP_WINDOW).raw(), celsius_t(MIN_COOLING_SLOPE_DEG).raw(), SEC_TO_MS(MIN_COOLING_SLOPE_TIME), SEC_TO_MS(TEMP_RESIDENCY_TIME), PID_MAX, TERN(PIDTEMP, hotend_pid_t, NoPID) WatchHE > (HotEndPos## N, H_E## N, HEATER_## N ##_MINTEMP, HEATER_## N ##_MAXTEMP)
    // Parenthesis is required to avoid breaking on the comma when expanding the DeclareHotEnd macro
    LIST_BY_HOTENDS((DeclareHotEnd(0)), (DeclareHotEnd(1)), (DeclareHotEnd(2)), (DeclareHotEnd(3)), (DeclareHotEnd(4)), (DeclareHotEnd(5)), (DeclareHotEnd(6)), (DeclareHotEnd(7))),

    #undef DeclareHotEnd
  #endif

  #if HAS_TEMP_PROBE
    #ifndef MIN_DELTA_SLOPE_DEG_PROBE
      #define MIN_DELTA_SLOPE_DEG_PROBE 1.0
    #endif
    #ifndef MIN_DELTA_SLOPE_TIME_PROBE
      #define MIN_DELTA_SLOPE_TIME_PROBE 600
    #endif

    // Init min and max temp with extreme values to prevent false errors during startup
    HeaterNoPID(TempProbePos, H_PROBE),
  #endif

  #if HAS_HEATED_BED
    #ifndef MIN_COOLING_SLOPE_DEG_BED
      #define MIN_COOLING_SLOPE_DEG_BED 1.00
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_BED
      #define MIN_COOLING_SLOPE_TIME_BED 60
    #endif

    // Init min and max temp with extreme values to prevent false errors during startup
    HeaterImpl<HEATER_BED_PIN, HEATER_BED_INVERTING, celsius_t(TEMP_BED_HYSTERESIS).raw(), celsius_t(TEMP_BED_WINDOW).raw(), celsius_t(MIN_COOLING_SLOPE_DEG_BED).raw(), SEC_TO_MS(MIN_COOLING_SLOPE_TIME_BED), SEC_TO_MS(TEMP_BED_RESIDENCY_TIME), MAX_BED_POWER, TERN(PIDTEMPBED, PID_t, NoPID)
    #if WATCH_BED
      , celsius_t(WATCH_BED_TEMP_INCREASE).raw(), WATCH_BED_TEMP_PERIOD
    #endif
    > (HeatedBedPos, H_BED, BED_MINTEMP, BED_MAXTEMP),
  #endif

  #if HAS_HEATED_CHAMBER
    #ifndef MIN_COOLING_SLOPE_DEG_CHAMBER
      #define MIN_COOLING_SLOPE_DEG_CHAMBER 1.50
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_CHAMBER
      #define MIN_COOLING_SLOPE_TIME_CHAMBER 120
    #endif

    // Init min and max temp with extreme values to prevent false errors during startup
    HeaterImpl<HEATER_CHAMBER_PIN, HEATER_CHAMBER_INVERTING,  celsius_t(TEMP_CHAMBER_HYSTERESIS).raw(), celsius_t(TEMP_CHAMBER_WINDOW).raw(), celsius_t(MIN_COOLING_SLOPE_DEG_CHAMBER).raw(), SEC_TO_MS(MIN_COOLING_SLOPE_TIME_CHAMBER), SEC_TO_MS(TEMP_CHAMBER_RESIDENCY_TIME), MAX_CHAMBER_POWER, TERN(PIDTEMPCHAMBER, PID_t, NoPID)
    #if WATCH_CHAMBER
      , celsius_t(WATCH_CHAMBER_TEMP_INCREASE).raw(), WATCH_CHAMBER_TEMP_PERIOD
    #endif
    > (HeatedChamberPos, H_CHAMBER, CHAMBER_MINTEMP, CHAMBER_MAXTEMP),
  #endif

  #if EITHER(HAS_COOLER, HAS_TEMP_COOLER)
    #ifndef MIN_COOLING_SLOPE_DEG_COOLER
      #define MIN_COOLING_SLOPE_DEG_COOLER 1.50
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_COOLER
      #define MIN_COOLING_SLOPE_TIME_COOLER 120
    #endif

    HeaterImpl<HEATER_COOLER_PIN, HEATER_COOLER_INVERTING, celsius_t(TEMP_COOLER_HYSTERESIS).raw(), celsius_t(TEMP_COOLER_WINDOW).raw(), celsius_t(MIN_COOLING_SLOPE_DEG_COOLER).raw(), SEC_TO_MS(MIN_COOLING_SLOPE_TIME_COOLER), SEC_TO_MS(TEMP_COOLER_RESIDENCY_TIME), 0, NoPID
    #if WATCH_COOLER
      , celsius_t(WATCH_COOLER_TEMP_INCREASE).raw(), WATCH_COOLER_TEMP_PERIOD
    #endif
    > (CoolerPos, H_COOLER, COOLER_MINTEMP, COOLER_MAXTEMP),
  #endif
};

// Useful shortcut in the code below
TERN_(HAS_HEATED_BED, static Heater & bed = heaters[HeatedBedPos]);
TERN_(HAS_HEATED_CHAMBER, static Heater & chamber = heaters[HeatedChamberPos]);
TERN_(HAS_COOLER || HAS_TEMP_COOLER, static Heater & cooler = heaters[CoolerPos]);


Heater & Temperature::get_heater(const int pos) { return heaters[(heater_pos_t)pos]; }

struct Analog2Celsius {
  virtual celsius_t get(int raw) { return 0; };
};

// Convert a huge number of ifdef to a type list instead so the compiler can select by itself
struct MaxTC0 {}; struct MaxTC1 {}; struct Quarter {}; struct AD595 {}; struct AD8495 {};

template <typename Type> struct A2C: public Analog2Celsius { };
#if HAS_USER_THERMISTORS
  template <uint8_t index> struct UserThermistor : public Analog2Celsius  { celsius_t get(int raw) { return Temperature::user_thermistor_to_deg_c(index, raw); } };
#endif
#if ENABLED(TEMP_SENSOR_0_IS_MAX31865)
  template<> struct A2C<MaxTC0>  : public Analog2Celsius  { celsius_t get(int raw) { return max31865_0.temperature(MAX31865_SENSOR_OHMS_0, MAX31865_CALIBRATION_OHMS_0); } };
#endif
#if ENABLED(TEMP_SENSOR_1_IS_MAX31865)
  template<> struct A2C<MaxTC1>  : public Analog2Celsius  { celsius_t get(int raw) { return max31865_1.temperature(MAX31865_SENSOR_OHMS_1, MAX31865_CALIBRATION_OHMS_1); } };
#endif
template<> struct A2C<Quarter> : public Analog2Celsius  { celsius_t get(int raw) { return raw * 0.25; } };
#define TEMP_AD595(RAW)  ((RAW) * 5.0 * 100.0 / float(HAL_ADC_RANGE) / (OVERSAMPLENR) * (TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET)
#define TEMP_AD8495(RAW) ((RAW) * 6.6 * 100.0 / float(HAL_ADC_RANGE) / (OVERSAMPLENR) * (TEMP_SENSOR_AD8495_GAIN) + TEMP_SENSOR_AD8495_OFFSET)

template<> struct A2C<AD595>   : public Analog2Celsius  { celsius_t get(int raw) { return TEMP_AD595(raw); } };
template<> struct A2C<AD8495>  : public Analog2Celsius  { celsius_t get(int raw) { return TEMP_AD8495(raw); } };

/**
 * Bisect search for the range of the 'raw' value, then interpolate
 * proportionally between the under and over values.
 */
static float scanThermistorTable(int raw, const temp_entry_t * table, uint16_t len) {
  uint8_t l = 0, r = len, m;
  for (;;) {
    m = (l + r) >> 1;
    if (!m) return int16_t(pgm_read_word(&table[0].celsius));
    if (m == l || m == r) return int16_t(pgm_read_word(&table[len-1].celsius));
    int16_t v00 = pgm_read_word(&table[m-1].value),
            v10 = pgm_read_word(&table[m-0].value);
         if (raw < v00) r = m;
    else if (raw > v10) l = m;
    else {
      const int16_t v01 = int16_t(pgm_read_word(&table[m-1].celsius)),
                      v11 = int16_t(pgm_read_word(&table[m-0].celsius));
      return v01 + (raw - v00) * float(v11 - v01) / float(v10 - v00);
    }
  }
}

#if HAS_HOTEND_THERMISTOR
  template <uint8_t index>
  struct HEThermistorTable : public Analog2Celsius {
    celsius_t get(int raw) {
      // Thermistor with conversion table?
      const temp_entry_t(*tt)[] = (temp_entry_t(*)[])(heater_ttbl_map[index]);
      return scanThermistorTable(raw, *tt, heater_ttbllen_map[index]);
    }
  };
#endif

struct ThermistorTable : public Analog2Celsius {
  celsius_t get(int raw) {
    // Thermistor with conversion table?
    return scanThermistorTable(raw, table, len);
  }
  const temp_entry_t * table; const uint16_t len;
  ThermistorTable(const temp_entry_t * table, const uint16_t len) : table(table), len(len) {}
};




static Analog2Celsius convertionTable[] = {
  #if HAS_HOTEND
    #define DeclareA2CForHE(N)  \
      TERN(TEMP_SENSOR_## N ##_IS_CUSTOM, UserThermistor<CTI_HOTEND_## N>, \
        TERN(TEMP_SENSOR_## N ##_IS_MAX_TC, A2C<TERN(TEMP_SENSOR_## N ##_IS_MAX31865, MaxTC## N, Quarter)>, \
          TERN(TEMP_SENSOR_## N ##_IS_AD595, A2C<AD595>, \
            TERN(TEMP_SENSOR_## N ##_IS_AD8495, A2C<AD8495>, \
              TERN0(HAS_HOTEND_THERMISTOR, HEThermistorTable<N>)))))()
    LIST_BY_HOTENDS(DeclareA2CForHE(0), DeclareA2CForHE(1), DeclareA2CForHE(2), DeclareA2CForHE(3), DeclareA2CForHE(4), DeclareA2CForHE(5), DeclareA2CForHE(6), DeclareA2CForHE(7)),
  #endif

  #if HAS_TEMP_PROBE
    TERN(TEMP_SENSOR_PROBE_IS_CUSTOM, UserThermistor<CTI_PROBE>{},
      TERN(TEMP_SENSOR_PROBE_IS_THERMISTOR, (ThermistorTable{TEMPTABLE_PROBE, TEMPTABLE_PROBE_LEN}),
        TERN(TEMP_SENSOR_PROBE_IS_AD595, A2C<AD595>{},
          TERN0(TEMP_SENSOR_PROBE_IS_AD8495, A2C<AD8495>{})))),
  #endif

  #if HAS_HEATED_BED
    TERN(TEMP_SENSOR_BED_IS_CUSTOM, UserThermistor<CTI_BED>{},
      TERN(TEMP_SENSOR_BED_IS_THERMISTOR, (ThermistorTable{TEMPTABLE_BED, TEMPTABLE_BED_LEN}),
        TERN(TEMP_SENSOR_BED_IS_AD595, A2C<AD595>{},
          TERN0(TEMP_SENSOR_BED_IS_AD8495, A2C<AD8495>{})))),
  #endif

  #if HAS_TEMP_CHAMBER
    TERN(TEMP_SENSOR_CHAMBER_IS_CUSTOM, UserThermistor<CTI_CHAMBER>{},
      TERN(TEMP_SENSOR_CHAMBER_IS_THERMISTOR, (ThermistorTable{TEMPTABLE_CHAMBER, TEMPTABLE_CHAMBER_LEN}),
        TERN(TEMP_SENSOR_CHAMBER_IS_AD595, A2C<AD595>{},
          TERN0(TEMP_SENSOR_CHAMBER_IS_AD8495, A2C<AD8495>{}))))(),
  #endif

  #if HAS_TEMP_COOLER
    TERN(TEMP_SENSOR_COOLER_IS_CUSTOM, UserThermistor<CTI_COOLER>{},
      TERN(TEMP_SENSOR_COOLER_IS_THERMISTOR, (ThermistorTable{TEMPTABLE_COOLER, TEMPTABLE_COOLER_LEN}),
        TERN(TEMP_SENSOR_COOLER_IS_AD595, A2C<AD595>{},
          TERN0(TEMP_SENSOR_COOLER_IS_AD8495, A2C<AD8495>{})))),
  #endif
};

#if ANY(PID_DEBUG, PID_BED_DEBUG, PID_CHAMBER_DEBUG, PID_COOLER_DEBUG)
  // The order of the declarations below must follow the heater_pos_t order
  static const char* heatersName[] = {
    #if HAS_HOTEND
      #define DeclareHEName(N) PSTR("HOTEND" #N)
      LIST_BY_HOTENDS(DeclareHEName(0), DeclareHEName(1), DeclareHEName(2), DeclareHEName(3), DeclareHEName(4), DeclareHEName(5), DeclareHEName(6), DeclareHEName(7)),
      #undef DeclareHEName
    #endif
    #if HAS_TEMP_PROBE
      PSTR("TEMPPROBE"),
    #endif
    #if HAS_HEATED_BED
      PSTR("BED"),
    #endif
    #if HAS_HEATED_CHAMBER
      PSTR("CHAMBER"),
    #endif
    #if EITHER(HAS_COOLER, HAS_TEMP_COOLER)
      PSTR("COOLER"),
    #endif
  };
#endif

float Heater::get_pid_output(const float minPower, const float maxPower, const float bangMax) {
  const float bm = bangMax ? bangMax : maxPower;
  #if DISABLED(PIDTEMP)
      const bool is_idling = TERN0(HEATER_IDLE_HANDLER, timed_out);
      return (!is_idling && info.celsius < info.target) ? bm : 0;
  #endif

  // PID is computed as float to get the maximum possible resolution even if it's slower
  #if DISABLED(PID_OPENLOOP)
    static TERN(PID_EXTRUSION_SCALING || PID_FAN_SCALING, PIDCF_t, PID_t) work_pid; // At worst, can have 2 floats wasted here, but that's the price to pay for smaller code
    static float temp_iState = 0, temp_dState = 0;
    static bool pid_reset = true;
    float pid_output = 0;
    // Avoid multiple conversion to float here
    float temp = (float)info.celsius, target = (float)info.target;
    const float max_power_over_i_gain = maxPower / pid.getKi() - minPower, pid_error = target - temp;

    if (!target || pid_error < -(PID_FUNCTIONAL_RANGE)) {
      pid_output = 0;
      pid_reset = true;
    }
    else if (pid_error > PID_FUNCTIONAL_RANGE) {
      pid_output = bm;
      pid_reset = true;
    }
    else {
      if (pid_reset) {
        temp_iState = 0.0;
        work_pid.Kd = 0.0;
        pid_reset = false;
      }

      temp_iState = constrain(temp_iState + pid_error, 0, max_power_over_i_gain);

      work_pid.Kp = pid.getKp() * pid_error;
      work_pid.Ki = pid.getKi() * temp_iState;
      work_pid.Kd = work_pid.Kd + PID_K2 * (pid.getKd() * (temp_dState - temp) - work_pid.Kd);

      temp_dState = temp;

      pid_output = constrain(work_pid.Kp + work_pid.Ki + work_pid.Kd + minPower, 0, maxPower);
      #if ENABLED(PID_EXTRUSION_SCALING)
        work_pid.Kc = 0;
        if (id == active_extruder) { //TODO: Handle multiple extruder here
                                     // Side note: This works because heater_id and heater_pos matches for hotend position, it wouldn't match for bed and chamber and cooler
          const long e_position = stepper.position(E_AXIS);
          if (e_position > last_e_position) {
            lpq[lpq_ptr] = e_position - last_e_position;
            last_e_position = e_position;
          }
          else
            lpq[lpq_ptr] = 0;

          if (++lpq_ptr >= lpq_len) lpq_ptr = 0;
          work_pid.Kc = (lpq[lpq_ptr] * planner.steps_to_mm[E_AXIS]) * pid.getKc();
          pid_output += work_pid.Kc;
        }
      #endif
      #if ENABLED(PID_FAN_SCALING)
        if (id >= H_E0 && fan_speed[active_extruder] > PID_FAN_SCALING_MIN_SPEED) { // Heater_id is negative except for hotend where this is used
          work_pid.Kf = pid.getKf() + (PID_FAN_SCALING_LIN_FACTOR) * fan_speed[active_extruder];
          pid_output += work_pid.Kf;
        }
      #endif

      LIMIT(pid_output, 0, maxPower);
    }
  #else // PID_OPENLOOP
    const float pid_output = constrain(target, 0, maxPower);
  #endif

  #if ENABLED(PID_DEBUG)
    if (pid_debug_flag) {
      SERIAL_ECHO_MSG("PID_");
      SERIAL_ECHOPGM_P(heatersName[heaterPos]); // Progmen string here
      SERIAL_ECHOLNPAIR("_DEBUG : Input ", temp, " Output ", pid_output
        #if DISABLED(PID_OPENLOOP)
          , STR_PID_DEBUG_PTERM, work_pid.Kp
          , STR_PID_DEBUG_ITERM, work_pid.Ki
          , STR_PID_DEBUG_DTERM, work_pid.Kd
          #if ENABLED(PID_EXTRUSION_SCALING)
            , STR_PID_DEBUG_CTERM, work_pid.Kc
          #endif
        #endif
      );
    }
  #endif

  return pid_output;
}

#if ENABLED(PRINTER_EVENT_LEDS)
  // This is poor code. Would be better if event were index instead of individual functions here
  LEDColor Heater::pe_heating_start() {
    switch(id) {
      case H_PROBE:   return LEDColorOff();
      case H_BED:     return printerEventLEDs.onBedHeatingStart    ();
      case H_CHAMBER: return printerEventLEDs.onChamberHeatingStart();
      case H_COOLER:  return printerEventLEDs.onCoolerHeatingStart ();
      default:        return printerEventLEDs.onHotendHeatingStart ();
    }
  }
  void Heater::pe_heating(float start, float current, float target) {
    switch(id) {
      case H_PROBE: return;
      case H_BED:     printerEventLEDs.onBedHeating     (start, current, target); return;
      case H_CHAMBER: printerEventLEDs.onChamberHeating (start, current, target); return;
      case H_COOLER:  printerEventLEDs.onCoolerHeating  (start, current, target); return;
      default:        printerEventLEDs.onHotendHeating  (start, current, target); return;
    }
  }
  void Heater::pe_heating_done() {
    switch(id) {
      case H_PROBE: return;
      case H_BED:     printerEventLEDs.onBedHeatingDone    (); return;
      case H_CHAMBER: printerEventLEDs.onChamberHeatingDone(); return;
      case H_COOLER:  printerEventLEDs.onCoolerHeatingDone (); return;
      default:        printerEventLEDs.onHotendHeatingDone (); return;
    }
  }
#endif

bool Heater::wait_for_temperature(const uint8_t extruder, const bool no_wait_for_cooling/*=true*/
  #if G26_CLICK_CAN_CANCEL
    , const bool click_to_cancel/*=false*/
  #endif
) {
  #if ENABLED(AUTOTEMP)
    // Can't use a REMEMBER here since it depends if it's an hotend or not
    bool plannerATE = planner.autotemp_enabled;
    if (id >= H_E0) planner.autotemp_enabled = false;
  #endif

  millis_t residency_start_ms = 0;
  bool first_loop = true;
  bool ret = false;

  #if DISABLED(BUSY_WHILE_HEATING) && ENABLED(HOST_KEEPALIVE_FEATURE)
    KEEPALIVE_STATE(NOT_BUSY);
  #endif

  #if ENABLED(PRINTER_EVENT_LEDS)
    const float start_temp = deg();
    if (id >= H_BED) pe_heating_start();
  #endif

  bool wants_to_cool = false;
  celsius_t target_temp = -1.0f, old_temp = maxCValue;
  millis_t now, next_temp_ms = 0, next_cool_check_ms = 0;
  wait_for_heatup = true;
  do {
    // Target temperature might be changed during the loop
    if (target_temp != degTarget()) {
      wants_to_cool = is_cooling();
      target_temp = degTarget();

      // Exit if S<lower>, continue if S<higher>, R<lower>, or R<higher>
      if (no_wait_for_cooling && wants_to_cool) break;
    }

    now = millis();
    if (ELAPSED(now, next_temp_ms)) { // Print temp & remaining time every 1s while waiting
      next_temp_ms = now + 1000UL;
      Temperature::print_heater_states(extruder);
      if (residencyTime > 0) {
        SERIAL_ECHOPGM(" W:");
        if (residency_start_ms)
          SERIAL_ECHO(long(residencyTime - (now - residency_start_ms)) / 1000UL);
        else
          SERIAL_CHAR('?');
      }
      SERIAL_EOL();
    }

    idle();
    gcode.reset_stepper_timeout(); // Keep steppers powered

    const celsius_t temp = deg();

    #if ENABLED(PRINTER_EVENT_LEDS)
      // Gradually change LED strip from violet to red as nozzle heats up
      if (!wants_to_cool && id >= H_BED) pe_heating(start_temp, (float)temp, (float)target_temp);
    #endif

    if (residencyTime) {
      const celsius_t temp_diff = ABS(target_temp - temp);
      if (!residency_start_ms) {
        // Start the TEMP_RESIDENCY_TIME timer when we reach target temp for the first time.
        if (temp_diff < window)
          residency_start_ms = now + (first_loop ? SEC_TO_MS(TEMP_RESIDENCY_TIME) / 3 : 0);
      }
      else if (temp_diff > hysteresis) {
        // Restart the timer whenever the temperature falls outside the hysteresis.
        residency_start_ms = now;
      }

      first_loop = false;
    }

    // Prevent a wait-forever situation if R is misused i.e. M109 R0
    if (wants_to_cool) {
      // break after MIN_COOLING_SLOPE_TIME seconds
      // if the temperature did not drop at least MIN_COOLING_SLOPE_DEG
      if (!next_cool_check_ms || ELAPSED(now, next_cool_check_ms)) {
        if (old_temp - temp < minCoolingSlope) break;
        next_cool_check_ms = now + minCoolingPeriod;
        old_temp = temp;
      }
    }

    #if G26_CLICK_CAN_CANCEL
      if (click_to_cancel && ui.use_click()) {
        wait_for_heatup = false;
        ui.quick_feedback();
      }
    #endif

  } while (wait_for_heatup && temp_conditions(residency_start_ms, now, wants_to_cool));

  if (wait_for_heatup) {
    wait_for_heatup = false;
    #if ENABLED(DWIN_CREALITY_LCD)
      if (id >= H_E0) { // TODO: Move this out in the printer event API
        HMI_flag.heat_flag = 0;
        duration_t elapsed = print_job_timer.duration();  // print timer
        dwin_heat_time = elapsed.value;
      }
    #else
      ui.reset_status();
    #endif
    TERN_(PRINTER_EVENT_LEDS, if (id >= H_E0) pe_heating_done());
    ret = true;
  }

  TERN_(AUTOTEMP, if (id >= 0) planner.autotemp_enabled = plannerATE);
  return ret;
}

celsius_t Heater::analog_to_celsius(const int raw) { return convertionTable[heaterPos].get(raw); }



#if ENABLED(AUTO_POWER_E_FANS)
  uint8_t Temperature::autofan_speed[HOTENDS]; // = { 0 }
#endif

#if ENABLED(AUTO_POWER_CHAMBER_FAN)
  uint8_t Temperature::chamberfan_speed; // = 0
#endif

#if ENABLED(AUTO_POWER_COOLER_FAN)
  uint8_t Temperature::coolerfan_speed; // = 0
#endif

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
      PORT_REDIRECT(SerialMask::All);
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


#if HAS_HEATED_BED
  // Init min and max temp with extreme values to prevent false errors during startup
  int16_t Temperature::mintemp_raw_BED = TEMP_SENSOR_BED_RAW_LO_TEMP,
          Temperature::maxtemp_raw_BED = TEMP_SENSOR_BED_RAW_HI_TEMP;
  IF_DISABLED(PIDTEMPBED, millis_t Temperature::next_bed_check_ms);
#endif

#if HAS_TEMP_CHAMBER
  #if HAS_HEATED_CHAMBER
    millis_t next_cool_check_ms_2 = 0;
    float old_temp = 9999;
    int16_t Temperature::mintemp_raw_CHAMBER = TEMP_SENSOR_CHAMBER_RAW_LO_TEMP,
            Temperature::maxtemp_raw_CHAMBER = TEMP_SENSOR_CHAMBER_RAW_HI_TEMP;
    IF_DISABLED(PIDTEMPCHAMBER, millis_t Temperature::next_chamber_check_ms);
  #endif
#endif

#if HAS_TEMP_COOLER
  #if HAS_COOLER
    bool flag_cooler_state;
    //bool flag_cooler_excess = false;
    int16_t Temperature::mintemp_raw_COOLER = TEMP_SENSOR_COOLER_RAW_LO_TEMP,
            Temperature::maxtemp_raw_COOLER = TEMP_SENSOR_COOLER_RAW_HI_TEMP;
    millis_t Temperature::next_cooler_check_ms, Temperature::cooler_fan_flush_ms;
  #endif
#endif

#if HAS_TEMP_PROBE
  probe_info_t Temperature::temp_probe; // = { 0 }
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  bool Temperature::allow_cold_extrude = false;
  celsius_t Temperature::extrude_min_temp = EXTRUDE_MINTEMP;
#endif

// private:

#if EARLY_WATCHDOG
  bool Temperature::inited = false;
#endif

#if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
  celsius_t Temperature::redundant_temperature_raw = 0;
  float Temperature::redundant_temperature = 0.0;
#endif

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
  celsius_t Temperature::singlenozzle_temp[EXTRUDERS];
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
  void Temperature::PID_autotune(const celsius_t target, const heater_id_t heater_id, const int8_t ncycles, const bool set_result/*=false*/) {
    celsius_t current_temp;

    int cycles = 0;
    bool heating = true;

    millis_t next_temp_ms = millis(), t1 = next_temp_ms, t2 = next_temp_ms;
    long t_high = 0, t_low = 0;

    PIDVec tune_pid; // Smaller
    celsius_t maxT(0), minT(maxCValue);

    HEATER_LOOP() {
      if (heater.id != heater_id) continue;       // Search the heater to autotune
      if (heater.id < H_CHAMBER) return;          // Don't autotune a pseudo heater

      #if HAS_WATCH_HEATER
        const uint16_t watch_temp_period = heater.period_ms;
        const celsius_t watch_temp_increase = heater.increase;
        const celsius_t watch_temp_target = target - watch_temp_increase + heater.hysteresis + celsius_t(1);
        millis_t temp_change_ms = next_temp_ms + watch_temp_period;
        celsius_t next_watch_temp;
        bool heated = false;
      #endif

      TERN_(HAS_AUTO_FAN, next_auto_fan_check_ms = next_temp_ms + 2500UL);
      if (false
        #if BOTH(THERMAL_PROTECTION_CHAMBER, PIDTEMPCHAMBER)
          || (heater.id == H_CHAMBER && target > celsius_t(CHAMBER_MAX_TARGET))
        #endif
        #if BOTH(THERMAL_PROTECTION_BED, PIDTEMPBED)
          || (heater.id == H_BED && target > celsius_t(BED_MAX_TARGET))
        #endif
          || (target > temp_range[heater_id].maxtemp - celsius_t(HOTEND_OVERSHOOT))) {
        SERIAL_ECHOLNPGM(STR_PID_TEMP_TOO_HIGH);
        TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_TEMP_TOO_HIGH));
        return;
      }

      SERIAL_ECHOLNPGM(STR_PID_AUTOTUNE_START);

      disable_all_heaters();
      TERN_(AUTO_POWER_CONTROL, powerManager.power_on());

      long bias = heater.maxPower >> 1, d = bias;
      heater.info.soft_pwm_amount = bias;

      #if ENABLED(PRINTER_EVENT_LEDS)
        const float start_temp = (float)heater.deg();
        LEDColor color = heater.pe_heating_start();
      #endif

      TERN_(NO_FAN_SLOWING_IN_PID_TUNING, adaptive_fan_slowing = false);

      // PID Tuning loop
      wait_for_heatup = true; // Can be interrupted with M108
      while (wait_for_heatup) {

        const millis_t ms = millis();

        if (raw_temps_ready) { // temp sample ready
          updateTemperaturesFromRawValues();

          // Get the current temperature and constrain it
          current_temp = heater.deg();
          NOLESS(maxT, current_temp);
          NOMORE(minT, current_temp);

          #if ENABLED(PRINTER_EVENT_LEDS)
            heater.pe_heating(start_temp, current_temp, target);
          #endif

          #if HAS_AUTO_FAN
            if (ELAPSED(ms, next_auto_fan_check_ms)) {
              checkExtruderAutoFans();
              next_auto_fan_check_ms = ms + 2500UL;
            }
          #endif

          if (heating && current_temp > target && ELAPSED(ms, t2 + 5000UL)) {
            heating = false;
            heater.info.soft_pwm_amount = ((bias - d) >> 1);
            t1 = ms;
            t_high = t1 - t2;
            maxT = target;
          }

          if (!heating && current_temp < target && ELAPSED(ms, t1 + 5000UL)) {
            heating = true;
            t2 = ms;
            t_low = t2 - t1;
            if (cycles > 0) {
              const long max_pow = heater.maxPower;
              bias += (d * (t_high - t_low)) / (t_low + t_high);
              LIMIT(bias, 20, max_pow - 20);
              d = (bias > max_pow >> 1) ? max_pow - 1 - bias : bias;

              SERIAL_ECHOPAIR(STR_BIAS, bias, STR_D_COLON, d, STR_T_MIN, minT, STR_T_MAX, maxT);
              if (cycles > 2) {
                const float Ku = (4.0f * d) / (float(M_PI) * (maxT - minT) * 0.5f),
                            Tu = float(t_low + t_high) * 0.001f,
                            pf = heater.id >= 0 ? 0.6f : 0.2f,
                            df = heater.id >= 0 ? 1.0f / 8.0f : 1.0f / 3.0f;

                tune_pid.Kp = Ku * pf;
                tune_pid.Ki = tune_pid.Kp * 2.0f / Tu;
                tune_pid.Kd = tune_pid.Kp * Tu * df;

                SERIAL_ECHOLNPAIR(STR_KU, Ku, STR_TU, Tu);
                if (heater.id >= 0)
                  SERIAL_ECHOLNPGM(STR_CLASSIC_PID);
                else
                  SERIAL_ECHOLNPGM(" No overshoot");

                SERIAL_ECHOLNPAIR(STR_KP, tune_pid.Kp, STR_KI, tune_pid.Ki, STR_KD, tune_pid.Kd);
              }
            }
            heater.info.soft_pwm_amount = ((bias + d) >> 1);
            cycles++;
            minT = target;
          }
        }

        // Did the temperature overshoot very far?
        #ifndef MAX_OVERSHOOT_PID_AUTOTUNE
          #define MAX_OVERSHOOT_PID_AUTOTUNE 30
        #endif
        if (current_temp > target + celsius_t(MAX_OVERSHOOT_PID_AUTOTUNE)) {
          SERIAL_ECHOLNPGM(STR_PID_TEMP_TOO_HIGH);
          TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_TEMP_TOO_HIGH));
          break;
        }

        // Report heater states every 2 seconds
        if (ELAPSED(ms, next_temp_ms)) {
          #if HAS_TEMP_SENSOR
            print_heater_states(heater_id >= 0 ? heater.id : active_extruder);
            SERIAL_EOL();
          #endif
          next_temp_ms = ms + 2000UL;

          // Make sure heating is actually working
          #if HAS_WATCH_HEATER
            if ((heater.id == H_BED && ENABLED(WATCH_BED)) || (heater.id == H_CHAMBER && ENABLED(WATCH_CHAMBER)) || (heater.id >= 0 && ENABLED(WATCH_HOTENDS))) {
              if (!heated) {                                            // If not yet reached target...
                if (current_temp > next_watch_temp) {                   // Over the watch temp?
                  next_watch_temp = current_temp + watch_temp_increase; // - set the next temp to watch for
                  temp_change_ms = ms + watch_temp_period;     // - move the expiration timer up
                  if (current_temp > watch_temp_target) heated = true;  // - Flag if target temperature reached
                }
                else if (ELAPSED(ms, temp_change_ms))                   // Watch timer expired
                  _temp_error(heater_id, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
              }
              else if (current_temp < target - celsius_t(MAX_OVERSHOOT_PID_AUTOTUNE)) // Heated, then temperature fell too far?
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
            PGM_P const estring = heater.id == H_CHAMBER  ? PSTR("chamber") : heater.id == H_BED ? PSTR("bed") : NUL_STR;
            say_default_(); SERIAL_ECHOPGM_P(estring); SERIAL_ECHOLNPAIR("Kp ", tune_pid.Kp);
            say_default_(); SERIAL_ECHOPGM_P(estring); SERIAL_ECHOLNPAIR("Ki ", tune_pid.Ki);
            say_default_(); SERIAL_ECHOPGM_P(estring); SERIAL_ECHOLNPAIR("Kd ", tune_pid.Kd);
          #else
            say_default_(); SERIAL_ECHOLNPAIR("Kp ", tune_pid.Kp);
            say_default_(); SERIAL_ECHOLNPAIR("Ki ", tune_pid.Ki);
            say_default_(); SERIAL_ECHOLNPAIR("Kd ", tune_pid.Kd);
          #endif

          // Use the result? (As with "M303 U1")
          if (set_result) {
            heater.pid.scaleFrom(tune_pid);
            if (heater.id >= 0) updatePID();
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
  }

#endif // HAS_PID_HEATING

/**
 * Class and Instance Methods
 */

int16_t Temperature::getHeaterPower(const heater_id_t heater_id) {
  switch (heater_id) {
    #if HAS_HEATED_BED
      case H_BED: return heaters[HeatedBedPos].info.soft_pwm_amount;
    #endif
    #if HAS_HEATED_CHAMBER
      case H_CHAMBER: return heaters[HeatedChamberPos].info.soft_pwm_amount;
    #endif
    #if HAS_COOLER
      case H_COOLER: return heaters[CoolerPos].info.soft_pwm_amount;
    #endif
    default:
      return TERN0(HAS_HOTEND, heaters[HotEndPos0 + heater_id].info.soft_pwm_amount);
  }
}

#define _EFANOVERLAP(A,B) _FANOVERLAP(E##A,B)

#if HAS_AUTO_FAN

  // A structure that's describing the conditions required to
  // update a fan automatically.
  struct FanTemperature {
    const heater_pos_t pos;
    const celsius_t    temp;
    const bool         bit;
    const pin_t        pin;

    void autoUpdateFan(uint8_t speed) {
      if (PWM_PIN(pin) && speed < 255) analogWrite(pin, bit ? speed : 0);
      else WRITE(pin, bit);
    }
    virtual int autoUpdateFan() { autoUpdateFan(EXTRUDER_AUTO_FAN_SPEED); return 1; }

    void check() { bit |= heaters[pos].deg() >= temp; }

    FanTemperature(const heater_pos_t pos, const celsius_t temp, const pin_t pin)
      : pos(pos), temp(temp), bit(false), pin(pin) {}
  };
  template <bool autoFan, int LimitTemperature, uint8_t fanSpeed = EXTRUDER_AUTO_FAN_SPEED>
  struct FanTemperatureImpl : FanTemperature {
    virtual int autoUpdateFan() { if (autoFan) { autoUpdateFan(fanSpeed); return 1; } return 0; }
    FanTemperatureImpl(const heater_pos_t pos, const pin_t pin) : FanTemperature(pos, celsius_t(LimitTemperature, true), pin) {}
  };

  static FanTemperature fanTemperatures[] = {
    #if HAS_HOTEND
      #define DeclareFHotEnd(N) FanTemperatures<TERN0(HAS_AUTO_FAN_## N, true), celsius_t(EXTRUDER_AUTO_FAN_TEMPERATURE).raw()>{HotEndPos## N, E## N ##_AUTO_FAN_PIN },
      LIST_BY_HOTENDS(DeclareFHotEnd(0), DeclareFHotEnd(1), DeclareFHotEnd(2), DeclareFHotEnd(3), DeclareFHotEnd(4), DeclareFHotEnd(5), DeclareFHotEnd(6), DeclareFHotEnd(7)),
      #undef DeclareFHotEnd
    #endif

    #if HAS_AUTO_CHAMBER_FAN
      FanTemperatures<TERN0(HAS_AUTO_CHAMBER_FAN && !AUTO_CHAMBER_IS_E, true),  celsius_t(CHAMBER_AUTO_FAN_TEMPERATURE).raw(), CHAMBER_AUTO_FAN_SPEED>{HeatedChamberPos, CHAMBER_AUTO_FAN_PIN },
    #endif

    #if HAS_AUTO_COOLER_FAN
      FanTemperatures<true,  celsius_t(COOLER_AUTO_FAN_TEMPERATURE).raw(), 255>{CoolerPos, EO_AUTO_FAN_PIN }, // Not sure about the pin here
    #endif
  };

  void Temperature::checkExtruderAutoFans() {

    // I don't understand what this does, and I don't think it's required anymore
    // If it's used to map an index to a fan pin (and there are multiple index leading to the same pin),
    // then instead, we should store a bit index in the FanTemperature's above
    /*
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
*/
    // First step: compute all required fans
    for (int i = 0; i < COUNT(fanTemperatures); i++) {
      FanTemperature & fan = fanTemperatures[i];
      fan.check();
    }
    // Second step: switch on fans where required
    uint32 enabled = 0;
    for (int i = 0; i < COUNT(fanTemperatures); i++) {
      FanTemperature & fan = fanTemperatures[i];
      if ((enabled & _BV(i)) == 0) {
        enabled |= fan.autoUpdateFan() ? _BV(i) : 0;
      }
    }
/*
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
    */
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
    if (heater_id >= 0)
      SERIAL_ECHO(heater_id);
    else if (TERN0(HAS_HEATED_CHAMBER, heater_id == H_CHAMBER))
      SERIAL_ECHOPGM(STR_HEATER_CHAMBER);
    else if (TERN0(HAS_COOLER, heater_id == H_COOLER))
      SERIAL_ECHOPGM(STR_COOLER);
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

#if ANY(PID_DEBUG, PID_BED_DEBUG, PID_CHAMBER_DEBUG)
  bool Temperature::pid_debug_flag; // = 0
#endif

#if HAS_HOTEND
  float Temperature::get_pid_output_hotend(const uint8_t E_NAME) { return heaters[ HotEndPos0 + HOTEND_INDEX ].get_pid_output(MIN_POWER, PID_MAX, BANG_MAX); }

  void Temperature::setTargetHotend(const celsius_t celsius, const uint8_t E_NAME) {
    const uint8_t ee = HOTEND_INDEX;
    Heater & h = heaters[HotEndPos0 + ee];
    #ifdef MILLISECONDS_PREHEAT_TIME
      if (celsius == 0)
        reset_preheat_time(ee);
      else if (h.target == 0)
        start_preheat_time(ee);
    #endif
    TERN_(AUTO_POWER_CONTROL, if (celsius) powerManager.power_on());
    h.set_target(_MIN(celsius, h.maxTarget()));
    h.start_watching();
  }
#endif

#if ENABLED(PIDTEMPBED)
  float Temperature::get_pid_output_bed() { return heaters[HeatedBedPos].get_pid_output(MIN_BED_POWER, MAX_BED_POWER); }
#endif

#if ENABLED(PIDTEMPCHAMBER)
  float Temperature::get_pid_output_chamber() { return heaters[HeatedChamberPos].get_pid_output(MIN_CHAMBER_POWER, MAX_CHAMBER_POWER); }
#endif

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
    #if TEMP_SENSOR_0_IS_MAX_TC
      if (heaters[HotEndPos0].deg() > _MIN(HEATER_0_MAXTEMP, TEMP_SENSOR_0_MAX_TC_TMAX - 1.0)) max_temp_error(H_E0);
      if (heaters[HotEndPos0].deg() < _MAX(HEATER_0_MINTEMP, TEMP_SENSOR_0_MAX_TC_TMIN + .01)) min_temp_error(H_E0);
    #endif
    #if TEMP_SENSOR_1_IS_MAX_TC
      if (heaters[HotEndPos1].deg() > _MIN(HEATER_1_MAXTEMP, TEMP_SENSOR_1_MAX_TC_TMAX - 1.0)) max_temp_error(H_E1);
      if (heaters[HotEndPos1].deg() < _MAX(HEATER_1_MINTEMP, TEMP_SENSOR_1_MAX_TC_TMIN + .01)) min_temp_error(H_E1);
    #endif
  #endif

  millis_t ms = millis();

  #if HAS_HOTEND

    HOTEND_LOOP() {
      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        if (heater.deg() > temp_range[e].maxtemp) max_temp_error((heater_id_t)e);
      #endif

      TERN_(HEATER_IDLE_HANDLER, heater.update(ms));

      #if ENABLED(THERMAL_PROTECTION_HOTENDS)
        // Check for thermal runaway
        tr_state_machine[e].run(heater, 0);
      #endif

      heater.info.soft_pwm_amount = (heater.deg() > temp_range[e].mintemp || is_preheating(e)) && heater.deg() < temp_range[e].maxtemp ? (int)get_pid_output_hotend(e) >> 1 : 0;

      #if WATCH_HOTENDS
        // Make sure temperature is increasing
        if (heater.next_ms && ELAPSED(ms, heater.next_ms)) {  // Time to check this extruder?
          if (degHotend(e) < heater.target) {                          // Failed to increase enough?
            TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
            _temp_error((heater_id_t)e, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
          }
          else heater.start_watching();
        }
      #endif

      #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
        // Make sure measured temperatures are close together
        if (ABS(heater.deg() - redundant_temperature) > MAX_REDUNDANT_TEMP_SENSOR_DIFF)
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
      if (bed.deg() > bed.maxTemp) max_temp_error(H_BED);
    #endif

    #if WATCH_BED
      // Make sure temperature is increasing
      if (bed.elapsed(ms)) {        // Time to check the bed?
        if (bed.deg() < bed.degTarget()) {                              // Failed to increase enough?
          TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
          _temp_error(H_BED, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
        }
        else bed.start_watching();
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

      TERN_(HEATER_IDLE_HANDLER, bed.update(ms));

      #if HAS_THERMALLY_PROTECTED_BED
        tr_state_machine[RUNAWAY_IND_BED].run(bed, 1);
      #endif

      #if HEATER_IDLE_HANDLER
        if (bed.timed_out) {
          bed.info.soft_pwm_amount = 0;
          #if DISABLED(PIDTEMPBED)
            bed.write(LOW);
          #endif
        }
        else
      #endif
      {
        #if ENABLED(PIDTEMPBED)
          bed.info.soft_pwm_amount = WITHIN(bed.deg(), bed.minTemp, bed.maxTemp) ? (int)get_pid_output_bed() >> 1 : 0;
        #else
          // Check if temperature is within the correct band
          if (WITHIN(bed.deg(), bed.minTemp, bed.maxTemp)) {
            #if ENABLED(BED_LIMIT_SWITCHING)
              if (bed.deg() >= bed.degTarget() + bed.hysteresis)
                bed.info.soft_pwm_amount = 0;
              else if (bed.deg() <= bed.degTarget() - bed.hysteresis)
                bed.info.soft_pwm_amount = MAX_BED_POWER >> 1;
            #else // !PIDTEMPBED && !BED_LIMIT_SWITCHING
              bed.info.soft_pwm_amount = bed.deg() < bed.degTarget() ? MAX_BED_POWER >> 1 : 0;
            #endif
          }
          else {
            bed.info.soft_pwm_amount = 0;
            bed.write(LOW);
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
      if (chamber.deg() > chamber.maxTemp) max_temp_error(H_CHAMBER);
    #endif

    #if WATCH_CHAMBER
      // Make sure temperature is increasing
      if (chamber.elapsed(ms)) {              // Time to check the chamber?
        if (chamber.deg() < chamber.degTarget())    // Failed to increase enough?
          _temp_error(H_CHAMBER, str_t_heating_failed, GET_TEXT(MSG_HEATING_FAILED_LCD));
        else chamber.start_watching();              // Start again if the target is still far off
      }
    #endif

    #if EITHER(CHAMBER_FAN, CHAMBER_VENT) || DISABLED(PIDTEMPCHAMBER)
      static bool flag_chamber_excess_heat; // = false;
    #endif

    #if EITHER(CHAMBER_FAN, CHAMBER_VENT)
      static bool flag_chamber_off; // = false

      if (chamber.degTarget() > chamber.minTemp) {
        flag_chamber_off = false;

        #if ENABLED(CHAMBER_FAN)
          int16_t fan_chamber_pwm;
          #if CHAMBER_FAN_MODE == 0
            fan_chamber_pwm = CHAMBER_FAN_BASE;
          #elif CHAMBER_FAN_MODE == 1
            fan_chamber_pwm = (chamber.deg() > chamber.degTarget()) ? (CHAMBER_FAN_BASE) + (CHAMBER_FAN_FACTOR) * (chamber.deg() - chamber.degTarget()) : 0;
          #elif CHAMBER_FAN_MODE == 2
            fan_chamber_pwm = (CHAMBER_FAN_BASE) + (CHAMBER_FAN_FACTOR) * ABS(chamber.deg() - chamber.degTarget());
            if (chamber.info.soft_pwm_amount)
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
          if (!flag_chamber_excess_heat && chamber.deg() - chamber.degTarget() >= HIGH_EXCESS_HEAT_LIMIT) {
            // Open vent after MIN_COOLING_SLOPE_TIME_CHAMBER_VENT seconds if the
            // temperature didn't drop at least MIN_COOLING_SLOPE_DEG_CHAMBER_VENT
            if (next_cool_check_ms_2 == 0 || ELAPSED(ms, next_cool_check_ms_2)) {
              if (chamber.deg() - old_temp > MIN_COOLING_SLOPE_DEG_CHAMBER_VENT)
                flag_chamber_excess_heat = true; // the bed is heating the chamber too much
              next_cool_check_ms_2 = ms + SEC_TO_MS(MIN_COOLING_SLOPE_TIME_CHAMBER_VENT);
              old_temp = chamber.deg();
            }
          }
          else {
            next_cool_check_ms_2 = 0;
            old_temp = 9999;
          }
          if (flag_chamber_excess_heat && (chamber.degTarget() - chamber.deg() >= LOW_EXCESS_HEAT_LIMIT))
            flag_chamber_excess_heat = false;
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

    #if ENABLED(PIDTEMPCHAMBER)
      // PIDTEMPCHAMBER doens't support a CHAMBER_VENT yet.
      chamber.info.soft_pwm_amount = WITHIN(chamber.deg(), chamber.minTemp, CHAMBER_MAXTEMP) ? (int)get_pid_output_chamber() >> 1 : 0;
    #else
      if (ELAPSED(ms, next_chamber_check_ms)) {
        next_chamber_check_ms = ms + CHAMBER_CHECK_INTERVAL;

        if (WITHIN(chamber.deg(), chamber.minTemp, CHAMBER_MAXTEMP)) {
          if (flag_chamber_excess_heat) {
            chamber.info.soft_pwm_amount = 0;
            #if ENABLED(CHAMBER_VENT)
              if (!flag_chamber_off) MOVE_SERVO(CHAMBER_VENT_SERVO_NR, chamber.deg() <= chamber.degTarget() ? 0 : 90);
            #endif
          }
          else {
            #if ENABLED(CHAMBER_LIMIT_SWITCHING)
              if (chamber.deg() >= chamber.degTarget() + TEMP_CHAMBER_HYSTERESIS)
                chamber.info.soft_pwm_amount = 0;
              else if (chamber.deg() <= chamber.degTarget() - (TEMP_CHAMBER_HYSTERESIS))
                chamber.info.soft_pwm_amount = (MAX_CHAMBER_POWER) >> 1;
            #else
              chamber.info.soft_pwm_amount = chamber.deg() < chamber.degTarget() ? (MAX_CHAMBER_POWER) >> 1 : 0;
            #endif
            #if ENABLED(CHAMBER_VENT)
              if (!flag_chamber_off) MOVE_SERVO(CHAMBER_VENT_SERVO_NR, 0);
            #endif
          }
        }
        else {
          chamber.info.soft_pwm_amount = 0;
          chamber.write(LOW);
        }
      }
      #if ENABLED(THERMAL_PROTECTION_CHAMBER)
        tr_state_machine[RUNAWAY_IND_CHAMBER].run(chamber, 2);
      #endif
    #endif

  #endif // HAS_HEATED_CHAMBER

  #if HAS_COOLER

    #ifndef COOLER_CHECK_INTERVAL
      #define COOLER_CHECK_INTERVAL 2000UL
    #endif

    #if ENABLED(THERMAL_PROTECTION_COOLER)
      if (cooler.deg() > cooler.maxTemp) max_temp_error(H_COOLER);
    #endif

    #if WATCH_COOLER
      // Make sure temperature is decreasing
      if (cooler.elapsed(ms)) {             // Time to check the cooler?
        if (cooler.deg() > cooler.degTarget())    // Failed to decrease enough?
          _temp_error(H_COOLER, GET_TEXT(MSG_COOLING_FAILED), GET_TEXT(MSG_COOLING_FAILED));
        else cooler.start_watching();                 // Start again if the target is still far off
      }
    #endif

    static bool flag_cooler_state; // = false

    if (cooler.enabled) {
      flag_cooler_state = true; // used to allow M106 fan control when cooler is disabled
      if (cooler.degTarget() == 0) cooler.target = COOLER_MIN_TARGET;
      if (ELAPSED(ms, next_cooler_check_ms)) {
        next_cooler_check_ms = ms + COOLER_CHECK_INTERVAL;
        if (cooler.deg() > cooler.degTarget()) {
          cooler.info.soft_pwm_amount = cooler.deg() > cooler.degTarget() ? MAX_COOLER_POWER : 0;
          flag_cooler_state = cooler.info.soft_pwm_amount > 0; // used to allow M106 fan control when cooler is disabled
          #if ENABLED(COOLER_FAN)
            int16_t fan_cooler_pwm = (COOLER_FAN_BASE) + (COOLER_FAN_FACTOR) * ABS(cooler.deg() - cooler.degTarget());
            NOMORE(fan_cooler_pwm, 255);
            set_fan_speed(COOLER_FAN_INDEX, fan_cooler_pwm); // Set cooler fan pwm
            cooler_fan_flush_ms = ms + 5000;
          #endif
        }
        else {
          cooler.info.soft_pwm_amount = 0;
          #if ENABLED(COOLER_FAN)
            set_fan_speed(COOLER_FAN_INDEX, cooler.deg() > cooler.degTarget() - 2 ? COOLER_FAN_BASE : 0);
          #endif
          cooler.write(LOW);
        }
      }
    }
    else {
      cooler.info.soft_pwm_amount = 0;
      if (flag_cooler_state) {
        flag_cooler_state = false;
        thermalManager.set_fan_speed(COOLER_FAN_INDEX, 0);
      }
      cooler.write(LOW);
    }

    #if ENABLED(THERMAL_PROTECTION_COOLER)
      tr_state_machine[RUNAWAY_IND_COOLER].run(coole, 3);
    #endif

  #endif // HAS_COOLER

  #if ENABLED(LASER_COOLANT_FLOW_METER)
    cooler.flowmeter_task(ms);
    #if ENABLED(FLOWMETER_SAFETY)
      if (cutter.enabled() && cooler.check_flow_too_low()) {
        cutter.disable();
        ui.flow_fault();
      }
    #endif
  #endif

  UNUSED(ms);
}


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
        { true, 0, 0, CHAMBER_PULLUP_RESISTOR_OHMS, CHAMBER_RESISTANCE_25C_OHMS, 0, 0, CHAMBER_BETA, 0 }
      #endif
      #if TEMP_SENSOR_COOLER_IS_CUSTOM
        { true, 0, 0, COOLER_PULLUP_RESISTOR_OHMS, COOLER_RESISTANCE_25C_OHMS, 0, 0, COOLER_BETA, 0 }
      #endif
      #if TEMP_SENSOR_PROBE_IS_CUSTOM
        { true, 0, 0, PROBE_PULLUP_RESISTOR_OHMS, PROBE_RESISTANCE_25C_OHMS, 0, 0, PROBE_BETA, 0 }
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
      nullptr
    );
    SERIAL_EOL();
  }

  celsius_t Temperature::user_thermistor_to_deg_c(const uint8_t t_index, const int raw) {
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
    return _MIN(value + THERMISTOR_ABS_ZERO_C, celsius_t::maxValue);
  }
#endif

/**
 * Get the raw values into the actual temperatures.
 * The raw values are created in interrupt context,
 * and this function is called from normal context
 * as it would block the stepper routine.
 */
void Temperature::updateTemperaturesFromRawValues() {
  TERN_(TEMP_SENSOR_0_IS_MAX_TC, heaters[HotEndPos0].info.raw = READ_MAX_TC(0));
  TERN_(TEMP_SENSOR_1_IS_MAX_TC, heaters[HotEndPos1].info.raw = READ_MAX_TC(1));

  for (int i = 0; i < HeatersCount; i++)
    heaters[i].info.celsius = heaters[i].analog_to_celsius(heaters[i].info.raw);

  TERN_(TEMP_SENSOR_1_AS_REDUNDANT, redundant_temperature = analog_to_celsius_hotend(redundant_temperature_raw, 1));
  TERN_(FILAMENT_WIDTH_SENSOR, filwidth.update_measured_mm());
  TERN_(HAS_POWER_MONITOR, power_monitor.capture_values());

  // Reset the watchdog on good temperature measurement
  watchdog_refresh();

  raw_temps_ready = false;
}

#if THERMO_SEPARATE_SPI
  template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin> SoftSPI<MisoPin, MosiPin, SckPin> SPIclass<MisoPin, MosiPin, SckPin>::softSPI;
  SPIclass<MAX6675_DO_PIN, SD_MOSI_PIN, MAX6675_SCK_PIN> max_tc_spi;
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
  // Init (and disable) SPI thermocouples
  #if TEMP_SENSOR_0_IS_MAX6675 && PIN_EXISTS(MAX6675_CS)
    OUT_WRITE(MAX6675_CS_PIN, HIGH);
  #endif
  #if TEMP_SENSOR_1_IS_MAX6675 && PIN_EXISTS(MAX6675_CS2)
    OUT_WRITE(MAX6675_CS2_PIN, HIGH);
  #endif
  #if TEMP_SENSOR_0_IS_MAX6675 && PIN_EXISTS(MAX31855_CS)
    OUT_WRITE(MAX31855_CS_PIN, HIGH);
  #endif
  #if TEMP_SENSOR_1_IS_MAX6675 && PIN_EXISTS(MAX31855_CS2)
    OUT_WRITE(MAX31855_CS2_PIN, HIGH);
  #endif
  #if TEMP_SENSOR_0_IS_MAX6675 && PIN_EXISTS(MAX31865_CS)
    OUT_WRITE(MAX31865_CS_PIN, HIGH);
  #endif
  #if TEMP_SENSOR_1_IS_MAX6675 && PIN_EXISTS(MAX31865_CS2)
    OUT_WRITE(MAX31865_CS2_PIN, HIGH);
  #endif

  #if HAS_MAX31865_TEMP
    TERN_(TEMP_SENSOR_0_IS_MAX31865, max31865_0.begin(MAX31865_2WIRE)); // MAX31865_2WIRE, MAX31865_3WIRE, MAX31865_4WIRE
    TERN_(TEMP_SENSOR_1_IS_MAX31865, max31865_1.begin(MAX31865_2WIRE));
  #endif
  #if HAS_MAX31855_TEMP
    TERN_(TEMP_SENSOR_0_IS_MAX31855, max31855_0.begin());
    TERN_(TEMP_SENSOR_1_IS_MAX31855, max31855_1.begin());
  #endif
  #if HAS_MAX6675_TEMP
    TERN_(TEMP_SENSOR_0_IS_MAX6675, max6675_0.begin());
    TERN_(TEMP_SENSOR_1_IS_MAX6675, max6675_1.begin());
  #endif

  #if EARLY_WATCHDOG
    // Flag that the thermalManager should be running
    if (inited) return;
    inited = true;
  #endif

  #if MB(RUMBA)
    // Disable RUMBA JTAG in case the thermocouple extension is plugged on top of JTAG connector
    #define _AD(N) (TEMP_SENSOR_##N##_IS_AD595 || TEMP_SENSOR_##N##_IS_AD8495)
    #if _AD(0) || _AD(1) || _AD(2) || _AD(BED) || _AD(CHAMBER)
      MCUCR = _BV(JTD);
      MCUCR = _BV(JTD);
    #endif
  #endif

  // Thermistor activation by MCU pin
  #if PIN_EXISTS(TEMP_0_TR_ENABLE)
    OUT_WRITE(TEMP_0_TR_ENABLE_PIN, ENABLED(TEMP_SENSOR_0_IS_MAX_TC));
  #endif
  #if PIN_EXISTS(TEMP_1_TR_ENABLE)
    OUT_WRITE(TEMP_1_TR_ENABLE_PIN, ENABLED(TEMP_SENSOR_1_IS_MAX_TC));
  #endif

  #if BOTH(PIDTEMP, PID_EXTRUSION_SCALING)
    last_e_position = 0;
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

  TERN_(THERMO_SEPARATE_SPI, max_tc_spi.init());

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
      const celsius_t tmin = _MAX(HEATER_##NR##_MINTEMP, TERN(TEMP_SENSOR_##NR##_IS_CUSTOM, 0, pgm_read_word(&TEMPTABLE_##NR [TEMP_SENSOR_##NR##_MINTEMP_IND].celsius))); \
      temp_range[NR].mintemp = tmin; \
      while (analog_to_celsius_hotend(temp_range[NR].raw_min, NR) < tmin) \
        temp_range[NR].raw_min += TEMPDIR(NR) * (OVERSAMPLENR); \
    }while(0)
    #define _TEMP_MAX_E(NR) do{ \
      const celsius_t tmax = _MIN(HEATER_##NR##_MAXTEMP, TERN(TEMP_SENSOR_##NR##_IS_CUSTOM, 2000, pgm_read_word(&TEMPTABLE_##NR [TEMP_SENSOR_##NR##_MAXTEMP_IND].celsius) - 1)); \
      temp_range[NR].maxtemp = tmax; \
      while (analog_to_celsius_hotend(temp_range[NR].raw_max, NR) > tmax) \
        temp_range[NR].raw_max -= TEMPDIR(NR) * (OVERSAMPLENR); \
    }while(0)

    #define _MINMAX_TEST(N,M) (HOTENDS > N && TEMP_SENSOR_ ##N## THERMISTOR_ID && TEMP_SENSOR_ ##N## THERMISTOR_ID != 998 && TEMP_SENSOR_ ##N## THERMISTOR_ID != 999 && defined(HEATER_##N##_##M##TEMP))

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

  TERN_(PROBING_HEATERS_OFF, paused = false);
}

#if HAS_THERMAL_PROTECTION

  Temperature::tr_state_machine_t Temperature::tr_state_machine[NR_HEATER_RUNAWAY]; // = { { TRInactive, 0 } };

  /**
   * @brief Thermal Runaway state machine for a single heater
   * @param current          current measured temperature
   * @param target           current target temperature
   * @param heater_id        extruder index
   * @param type             the protection type
   */
  void Temperature::tr_state_machine_t::run(Heater & heater, int type) {
    constexpr static celsius_t Hysteresis[] = {
      celsius_t((int)TERN0(THERMAL_PROTECTION_HOTENDS,  THERMAL_PROTECTION_HYSTERESIS)),
      celsius_t((int)TERN0(HAS_THERMALLY_PROTECTED_BED, THERMAL_PROTECTION_BED_HYSTERESIS)),
      celsius_t((int)TERN0(THERMAL_PROTECTION_CHAMBER,  THERMAL_PROTECTION_CHAMBER_HYSTERESIS)),
      celsius_t((int)TERN0(THERMAL_PROTECTION_COOLER,   THERMAL_PROTECTION_COOLER_HYSTERESIS)),
    };
    constexpr static uint16_t Periods[] = {
      TERN0(THERMAL_PROTECTION_HOTENDS,   SEC_TO_MS(THERMAL_PROTECTION_PERIOD)),
      TERN0(HAS_THERMALLY_PROTECTED_BED,  SEC_TO_MS(THERMAL_PROTECTION_BED_PERIOD)),
      TERN0(THERMAL_PROTECTION_CHAMBER,   SEC_TO_MS(THERMAL_PROTECTION_CHAMBER_PERIOD)),
      TERN0(THERMAL_PROTECTION_COOLER,    SEC_TO_MS(THERMAL_PROTECTION_COOLER_PERIOD)),
    };

    const uint16_t period_ms = Periods[type];
    const celsius_t hysteresis_degc = Hysteresis[type];

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
      if (heater.timed_out) {
        state = TRInactive;
        running_temp = 0;
      }
      else
    #endif
    {
      // If the target temperature changes, restart
      if (running_temp != heater.degTarget()) {
        running_temp = heater.degTarget();
        state = heater.degTarget() > 0 ? TRFirstHeating : TRInactive;
      }
    }

    celsius_t current = heater.deg();
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
          if (adaptive_fan_slowing && heater.id >= 0) {
            const int fan_index = _MIN(heater.id, FAN_COUNT - 1);
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
          timer = millis() + period_ms;
          break;
        }
        else if (PENDING(millis(), timer)) break;
        state = TRRunaway;

      case TRRunaway:
        TERN_(DWIN_CREALITY_LCD, DWIN_Popup_Temperature(0));
        _temp_error(heater.id, str_t_thermal_runaway, GET_TEXT(MSG_THERMAL_RUNAWAY));
    }
  }

#endif // HAS_THERMAL_PROTECTION

void Temperature::disable_all_heaters() {

  TERN_(AUTOTEMP, planner.autotemp_enabled = false);

  // Unpause and reset everything
  TERN_(PROBING_HEATERS_OFF, pause(false));

  for (int i = 0; i < HeatersCount; i++) {
    heaters[i].set_target(0);
    heaters[i].info.soft_pwm_amount = 0;
    heaters[i].write(LOW);
  }
}

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)

  #include "printcounter.h"

  bool Temperature::auto_job_over_threshold() {
    for (Heater & heater : heaters) {
      celsius_t minTemp = heater.id >= 0 ? celsius_t(EXTRUDE_MINTEMP / 2) : heater.minTemp;
      if (heater.degTarget() > minTemp) return true;
    }
    return false;
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
        HEATER_LOOP() { heater.expire(); } // Timeout immediately
      }
      else {
        HEATER_LOOP() { heater.reset(); heater.start_watching(); }
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
    singlenozzle_temp[old_tool] = heaters[HotEndPos0].degTarget();
    if (singlenozzle_temp[new_tool] && singlenozzle_temp[new_tool] != singlenozzle_temp[old_tool]) {
      setTargetHotend(singlenozzle_temp[new_tool], 0);
      TERN_(AUTOTEMP, planner.autotemp_update());
      TERN_(HAS_STATUS_MESSAGE, set_heating_message(0));
      (void)wait_for_hotend(0, false);  // Wait for heating or cooling
    }
  }

#endif

#if HAS_MAX_TC

  #ifndef THERMOCOUPLE_MAX_ERRORS
    #define THERMOCOUPLE_MAX_ERRORS 15
  #endif

  int Temperature::read_max_tc(TERN_(HAS_MULTI_MAX_TC, const uint8_t hindex/*=0*/)) {
    #define MAX6675_HEAT_INTERVAL 250UL

    #if HAS_MAX31855_TEMP
      static uint32_t max_tc_temp = 2000;
      #define MAX_TC_ERROR_MASK    7
      #define MAX_TC_DISCARD_BITS 18
      #define MAX_TC_SPEED_BITS    3        // (_BV(SPR1)) // clock ÷ 64
    #elif HAS_MAX31865_TEMP
      static uint16_t max_tc_temp = 2000;   // From datasheet 16 bits D15-D0
      #define MAX_TC_ERROR_MASK    1        // D0 Bit not used
      #define MAX_TC_DISCARD_BITS  1        // Data is in D15-D1
      #define MAX_TC_SPEED_BITS    3        //  (_BV(SPR1)) // clock ÷ 64
    #else
      static uint16_t max_tc_temp = 2000;
      #define MAX_TC_ERROR_MASK    4
      #define MAX_TC_DISCARD_BITS  3
      #define MAX_TC_SPEED_BITS    2        // (_BV(SPR0)) // clock ÷ 16
    #endif

    #if HAS_MULTI_MAX_TC
      // Needed to return the correct temp when this is called between readings
      static celsius_t max_tc_temp_previous[MAX_TC_COUNT] = { 0 };
      #define THERMO_TEMP(I) max_tc_temp_previous[I]
      #define THERMO_SEL(A,B) (hindex ? (B) : (A))
      #define MAX6675_WRITE(V)     do{ switch (hindex) { case 1:      WRITE(MAX6675_SS2_PIN, V); break; default:      WRITE(MAX6675_SS_PIN, V); } }while(0)
      #define MAX6675_SET_OUTPUT() do{ switch (hindex) { case 1: SET_OUTPUT(MAX6675_SS2_PIN);    break; default: SET_OUTPUT(MAX6675_SS_PIN);    } }while(0)
    #else
      constexpr uint8_t hindex = 0;
      #define THERMO_TEMP(I) max_tc_temp
      #if TEMP_SENSOR_1_IS_MAX31865
        #define THERMO_SEL(A,B) B
      #else
        #define THERMO_SEL(A,B) A
      #endif
      #if TEMP_SENSOR_0_IS_MAX6675
        #define MAX6675_WRITE(V)          WRITE(MAX6675_SS_PIN, V)
        #define MAX6675_SET_OUTPUT() SET_OUTPUT(MAX6675_SS_PIN)
      #else
        #define MAX6675_WRITE(V)          WRITE(MAX6675_SS2_PIN, V)
        #define MAX6675_SET_OUTPUT() SET_OUTPUT(MAX6675_SS2_PIN)
      #endif

    #endif

    static uint8_t max_tc_errors[MAX_TC_COUNT] = { 0 };

    // Return last-read value between readings
    static millis_t next_max_tc_ms[MAX_TC_COUNT] = { 0 };
    millis_t ms = millis();
    if (PENDING(ms, next_max_tc_ms[hindex])) return int(THERMO_TEMP(hindex));
    next_max_tc_ms[hindex] = ms + MAX6675_HEAT_INTERVAL;

    //
    // TODO: spiBegin, spiRec and spiInit doesn't work when soft spi is used.
    //
    #if !THERMO_SEPARATE_SPI && NO_THERMO_TEMPS
      spiBegin();
      spiInit(MAX_TC_SPEED_BITS);
    #endif

    #if NO_THERMO_TEMPS
      MAX6675_WRITE(LOW);  // enable TT_MAX6675
      DELAY_NS(100);       // Ensure 100ns delay
    #endif

    max_tc_temp = 0;

    // Read a big-endian temperature value
    #if NO_THERMO_TEMPS
      for (uint8_t i = sizeof(max_tc_temp); i--;) {
        max_tc_temp |= TERN(THERMO_SEPARATE_SPI, max_tc_spi.receive(), spiRec());
        if (i > 0) max_tc_temp <<= 8; // shift left if not the last byte
      }
        MAX6675_WRITE(HIGH); // disable TT_MAX6675
    #endif

    #if HAS_MAX31855_TEMP
      Adafruit_MAX31855 &max855ref = THERMO_SEL(max31855_0, max31855_1);
      max_tc_temp = max855ref.readRaw32();
    #endif

    #if HAS_MAX31865_TEMP
      Adafruit_MAX31865 &max865ref = THERMO_SEL(max31865_0, max31865_1);
      #if ENABLED(LIB_USR_MAX31865)
        max_tc_temp = max865ref.readRTD_with_Fault();
      #endif
    #endif

    #if HAS_MAX6675_TEMP
      MAX6675 &max6675ref = THERMO_SEL(max6675_0, max6675_1);
      max_tc_temp = max6675ref.readRaw16();
    #endif

    #if ENABLED(LIB_ADAFRUIT_MAX31865)
      const uint8_t fault_31865 = max865ref.readFault() & 0x3FU;
    #endif

    if (DISABLED(IGNORE_THERMOCOUPLE_ERRORS)
      && TERN(LIB_ADAFRUIT_MAX31865, fault_31865, (max_tc_temp & MAX_TC_ERROR_MASK))
    ) {
      max_tc_errors[hindex]++;
      if (max_tc_errors[hindex] > THERMOCOUPLE_MAX_ERRORS) {
        SERIAL_ERROR_START();
        SERIAL_ECHOPGM("Temp measurement error! ");
        #if MAX_TC_ERROR_MASK == 7
          SERIAL_ECHOPGM("MAX31855 Fault : (", max_tc_temp & 0x7, ") >> ");
          if (max_tc_temp & 0x1)
            SERIAL_ECHOLNPGM("Open Circuit");
          else if (max_tc_temp & 0x2)
            SERIAL_ECHOLNPGM("Short to GND");
          else if (max_tc_temp & 0x4)
            SERIAL_ECHOLNPGM("Short to VCC");
        #elif HAS_MAX31865
          #if ENABLED(LIB_USR_MAX31865)
            // At the present time we do not have the ability to set the MAX31865 HIGH threshold
            // or thr LOW threshold, so no need to check for them, zero these bits out
            const uint8_t fault_31865 = max865ref.readFault() & 0x3FU;
          #endif
          max865ref.clearFault();
          if (fault_31865) {
            SERIAL_EOL();
            SERIAL_ECHOLNPAIR("\nMAX31865 Fault :(", fault_31865, ")  >>");
            if (fault_31865 & MAX31865_FAULT_HIGHTHRESH)
              SERIAL_ECHOLNPGM("RTD High Threshold");
            if (fault_31865 & MAX31865_FAULT_LOWTHRESH)
              SERIAL_ECHOLNPGM("RTD Low Threshold");
            if (fault_31865 & MAX31865_FAULT_REFINLOW)
              SERIAL_ECHOLNPGM("REFIN- > 0.85 x Bias");
            if (fault_31865 & MAX31865_FAULT_REFINHIGH)
              SERIAL_ECHOLNPGM("REFIN- < 0.85 x Bias - FORCE- open");
            if (fault_31865 & MAX31865_FAULT_RTDINLOW)
              SERIAL_ECHOLNPGM("REFIN- < 0.85 x Bias - FORCE- open");
            if (fault_31865 & MAX31865_FAULT_OVUV)
              SERIAL_ECHOLNPGM("Under/Over voltage");
          }
        #else
          SERIAL_ECHOLNPGM("MAX6675 Open Circuit");
        #endif

        // Thermocouple open
        max_tc_temp = 4 * THERMO_SEL(TEMP_SENSOR_0_MAX_TC_TMAX, TEMP_SENSOR_1_MAX_TC_TMAX);
      }
      else
        max_tc_temp >>= MAX_TC_DISCARD_BITS;
    }
    else {
      max_tc_temp >>= MAX_TC_DISCARD_BITS;
      max_tc_errors[hindex] = 0;
    }

    #if HAS_MAX31855
      if (max_tc_temp & 0x00002000) max_tc_temp |= 0xFFFFC000; // Support negative temperature
    #endif

    // Return the RTD resistance for MAX31865 for display in SHOW_TEMP_ADC_VALUES
    #if HAS_MAX31865_TEMP
      #if ENABLED(LIB_ADAFRUIT_MAX31865)
        max_tc_temp = (uint32_t(max865ref.readRTD()) * THERMO_SEL(MAX31865_CALIBRATION_OHMS_0, MAX31865_CALIBRATION_OHMS_1)) >> 16;
      #elif ENABLED(LIB_USR_MAX31865)
        max_tc_temp = (uint32_t(max_tc_temp) * THERMO_SEL(MAX31865_CALIBRATION_OHMS_0, MAX31865_CALIBRATION_OHMS_1)) >> 16;
      #endif
    #endif

    THERMO_TEMP(hindex) = max_tc_temp;

    return int(max_tc_temp);
  }

#endif // HAS_MAX_TC


// Same order as the heaters declaration
static constexpr bool hasADC[] {
  #define DeclareHEADC(N) TERN0(HAS_TEMP_ADC_## N, true)
  LIST_N(HOTENDS, TERN0(HAS_TEMP_ADC_0 && !TEMP_SENSOR_0_IS_MAX_TC, true), false, DeclareHEADC(2), DeclareHEADC(3), DeclareHEADC(4), DeclareHEADC(5), DeclareHEADC(6), DeclareHEADC(7)),
  #undef DeclareHEADC
  TERN0(HAS_TEMP_ADC_PROBE, true),
  TERN0(HAS_TEMP_ADC_BED, true),
  TERN0(HAS_TEMP_ADC_CHAMBER, true),
  TERN0(HAS_TEMP_ADC_COOLER, true),
};

/**
 * Update raw temperatures
 */
void Temperature::update_raw_temperatures() {
  for (size_t i = 0; i < COUNT(hasADC); i++) {
    if (hasADC[i]) heaters[i].info.update();
  }

  // Fix missing case for heater 1
  #if HAS_TEMP_ADC_1
    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      redundant_temperature_raw = heaters[HotEndPos1].info.acc;
    #elif !TEMP_SENSOR_1_IS_MAX_TC
      heaters[HotEndPos1].info.update();
    #endif
  #endif

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

  for (int i = 0; i < HeatersCount; i++)
    heaters[i].info.reset();

  #if HAS_HOTEND
    TERN_(TEMP_SENSOR_1_AS_REDUNDANT, heaters[1].info.reset());
  #endif

  TERN_(HAS_JOY_ADC_X, joystick.x.reset());
  TERN_(HAS_JOY_ADC_Y, joystick.y.reset());
  TERN_(HAS_JOY_ADC_Z, joystick.z.reset());

  #if HAS_HOTEND

    static constexpr int8_t temp_dir[] = {
      TERN(TEMP_SENSOR_0_IS_MAX_TC, 0, TEMPDIR(0))
      #if HAS_MULTI_HOTEND
        , TERN(TEMP_SENSOR_1_IS_MAX_TC, 0, TEMPDIR(1))
        #if HOTENDS > 2
          #define _TEMPDIR(N) , TEMPDIR(N)
          REPEAT_S(2, HOTENDS, _TEMPDIR)
        #endif
      #endif
    };

    LOOP_L_N(e, COUNT(temp_dir)) {
      const int8_t tdir = temp_dir[e];
      if (tdir) {
        Heater & heater = heaters[HotEndPos0 + e];
        const int16_t rawtemp = heater.raw() * tdir; // normal direction, +rawtemp, else -rawtemp
        const bool heater_on = (heater.degTarget() > 0 || TERN0(PIDTEMP, heater.info.soft_pwm_amount) > 0);
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

  // TODO: Move the (min/max)temp_raw into the heater class and comparison direction so we can loop here instead of duplicating the code
  #if ENABLED(THERMAL_PROTECTION_BED)
    #if TEMPDIR(BED) < 0
      #define BEDCMP(A,B) ((A)<(B))
    #else
      #define BEDCMP(A,B) ((A)>(B))
    #endif
    const bool bed_on = (bed.degTarget() > 0) || TERN0(PIDTEMPBED, heaters[HeatedBedPos].info.soft_pwm_amount > 0);
    if (BEDCMP(bed.raw(), maxtemp_raw_BED)) max_temp_error(H_BED);
    if (bed_on && BEDCMP(mintemp_raw_BED, bed.raw())) min_temp_error(H_BED);
  #endif

  #if BOTH(HAS_HEATED_CHAMBER, THERMAL_PROTECTION_CHAMBER)
    #if TEMPDIR(CHAMBER) < 0
      #define CHAMBERCMP(A,B) ((A)<(B))
    #else
      #define CHAMBERCMP(A,B) ((A)>(B))
    #endif
    const bool chamber_on = (chamber.degTarget() > 0);
    if (CHAMBERCMP(chamber.raw(), maxtemp_raw_CHAMBER)) max_temp_error(H_CHAMBER);
    if (chamber_on && CHAMBERCMP(mintemp_raw_CHAMBER, chamber.raw())) min_temp_error(H_CHAMBER);
  #endif

  #if BOTH(HAS_COOLER, THERMAL_PROTECTION_COOLER)
    #if TEMPDIR(COOLER) < 0
      #define COOLERCMP(A,B) ((A)<(B))
    #else
      #define COOLERCMP(A,B) ((A)>(B))
    #endif
    if (cutter.unitPower > 0) {
      if (COOLERCMP(cooler.raw(), maxtemp_raw_COOLER)) max_temp_error(H_COOLER);
    }
    if (COOLERCMP(mintemp_raw_COOLER, cooler.raw())) min_temp_error(H_COOLER);
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

  // Must follow heaters' declaration here to allow looping
  static SoftPWM soft_pwms[] = {
    LIST_N(HOTENDS, {0}),
    #if HAS_TEMP_PROBE
      {}, // Useless but we must keep the heaters' declaration here
    #endif
    #if HAS_HEATED_BED
      {},
    #endif
    #if HAS_HEATED_CHAMBER
      {},
    #endif
    #if HAS_COOLER
      {},
    #endif
  };

  #define WRITE_FAN(n, v) WRITE(FAN##n##_PIN, (v) ^ FAN_INVERTING)

  #if DISABLED(SLOW_PWM_HEATERS)
    /**
     * Standard heater PWM modulation
     */
    if (pwm_count_tmp >= 127) {
      pwm_count_tmp -= 127;

      for (int i = 0; i < HeatersCount; i++) {
        constexpr uint8_t pwm_mask = TERN0(SOFT_PWM_DITHER, _BV(SOFT_PWM_SCALE) - 1);
        heaters[i].write(soft_pwms[i].add(pwm_mask, heaters[i].info.soft_pwm_amount));
      }

      // This can also be array-ified as well, left as an exercise for the reader
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
      for (int i = 0; i < HeatersCount; i++)
        if (soft_pwms[i].count <= pwm_count_tmp) heaters[i].write(LOW);

      // Ditto
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
    static uint8_t slow_pwm_count = 0;

    if (slow_pwm_count == 0) {
      for (int i = 0; i < HeatersCount; i++) {
        soft_pwms[i].count = heaters[i].info.soft_pwm_amount;
        const bool on = soft_pwms[i].count > 0;
        if (soft_pwms[i].ready(on)) heaters[i].write(on);
      }
    } // slow_pwm_count == 0

    for (int i = 0; i < HeatersCount; i++)
      if (soft_pwms[i].count < slow_pwm_count && soft_pwms[i].ready(false))
        heaters[i].write(false);

    // Ditto
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

      for(int i = 0; i < HeatersCount; i++)
        soft_pwms[i].dec();
    }

  #endif // SLOW_PWM_HEATERS

  //
  // Update lcd buttons 488 times per second
  //
  static bool do_buttons;
  if ((do_buttons ^= true)) ui.update_buttons();


  ADCSensorState next_sensor_state = adc_sensor_state < SensorsReady ? (ADCSensorState)(int(adc_sensor_state) + 1) : StartSampling;
  static uint8_t heater_index = 0;
  static bool    sampling = false;

  /**
   * One sensor is sampled on every other call of the ISR.
   * Each sensor is read 16 (OVERSAMPLENR) times, taking the average.
   *
   * On each Prepare pass, ADC is started for a sensor pin.
   * On the next pass, the ADC value is read and accumulated.
   *
   * This gives each ADC 0.9765ms to charge up.
   */
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
      heater_index = 0; sampling = true;
      break;

    case HeatersSampling:
      if (heater_index < HeatersCount) next_sensor_state = adc_sensor_state; // Each heater is sampled in sequence
      if (sampling) {                                   // and it alternates between sampling
        HAL_START_ADC(heaters[heater_index].pin);
      } else {                                          // and reading each call of the loop until all heaters are done
        if (HAL_ADC_READY()) break;
        heaters[heater_index++].info.sample(HAL_READ_ADC());
      }
      sampling = !sampling;
      break;

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
  static void print_heater_state(const celsius_t c, const celsius_t t
    #if ENABLED(SHOW_TEMP_ADC_VALUES)
      , const float r
    #endif
    , const heater_id_t e=INDEX_NONE
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
      #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
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
      SERIAL_ECHOPAIR(" (", TERN(NO_THERMO_TEMPS, false, k == 'T') ? r : r * RECIPROCAL(OVERSAMPLENR));
      SERIAL_CHAR(')');
    #endif
    delay(2);
  }

  void Temperature::print_heater_states(const uint8_t target_extruder
    #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
      , const bool include_r/*=false*/
    #endif
  ) {
    {
      #if HAS_TEMP_HOTEND
        Heater & heater = heaters[HotEndPos0 + target_extruder];
        print_heater_state(heater.deg(), heater.degTarget()
          #if ENABLED(SHOW_TEMP_ADC_VALUES)
            , heater.raw()
          #endif
          , heater.id);

        #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
          if (include_r) print_heater_state(redundant_temperature, heater.degTarget()
            #if ENABLED(SHOW_TEMP_ADC_VALUES)
              , redundant_temperature_raw
            #endif
            , H_REDUNDANT
          );
        #endif
      #endif
    }
    // Yes, target extruder is output twice
    HEATER_LOOP() {
      print_heater_state(heater.deg(), heater.degTarget()
      #if ENABLED(SHOW_TEMP_ADC_VALUES)
        , heater.raw()
      #endif
      , heater.id);
    }

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
    void Temperature::AutoReportTemp::report() {
      print_heater_states(active_extruder);
      SERIAL_EOL();
    }
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
    bool Temperature::wait_for_hotend(const uint8_t target_extruder, const bool no_wait_for_cooling/*=true*/
      #if G26_CLICK_CAN_CANCEL
        , const bool click_to_cancel/*=false*/
      #endif
    ) {
      return heaters[HotEndPos0 + target_extruder].wait_for_temperature(target_extruder, no_wait_for_cooling
      #if G26_CLICK_CAN_CANCEL
        , click_to_cancel
      #endif
      );
    }

    #if ENABLED(WAIT_FOR_HOTEND)
      // TODO: It's likely possible to make this generic as well
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
      #if G26_CLICK_CAN_CANCEL
        , const bool click_to_cancel/*=false*/
      #endif
    ) {
      return heaters[HeatedBedPos].wait_for_temperature(active_extruder, no_wait_for_cooling
      #if G26_CLICK_CAN_CANCEL
        , click_to_cancel
      #endif
      );
    }
    // TODO: It's likely possible to make this generic as well
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
    bool Temperature::wait_for_probe(const_float_t target_temp, bool no_wait_for_cooling/*=true*/) {

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
    bool Temperature::wait_for_chamber(const bool no_wait_for_cooling/*=true*/) { return heaters[HeatedChamberPos].wait_for_temperature(active_extruder, no_wait_for_cooling); }
  #endif // HAS_HEATED_CHAMBER

  #if HAS_COOLER
    bool Temperature::wait_for_cooler(const bool no_wait_for_cooling/*=true*/) {  return heaters[HeatedChamberPos].wait_for_temperature(active_extruder, no_wait_for_cooling); }
  #endif // HAS_COOLER

#endif // HAS_TEMP_SENSOR
