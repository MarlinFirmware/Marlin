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
 * feature/spindle_laser.h
 * Support for Laser Power or Spindle Power & Direction
 */

#include "../inc/MarlinConfig.h"

#include "spindle_laser_types.h"

#include "../libs/buzzer.h"

// Inline laser power
#include "../module/planner.h"

#if ALL(E3S1PRO_RTS, IIC_BL24CXX_EEPROM)
    #include "../libs/BL24CXX.h"
#endif

#include "../../Configuration.h"

#define PCT_TO_PWM(X) ((X) * 255 / 100)
#define PCT_TO_SERVO(X) ((X) * 180 / 100)


// Laser/Cutter operation mode
enum CutterMode : int8_t {
  CUTTER_MODE_ERROR = -1,
  CUTTER_MODE_STANDARD,     // M3 power is applied directly and waits for planner moves to sync.
  CUTTER_MODE_CONTINUOUS,   // M3 or G1/2/3 move power is controlled within planner blocks, set with 'M3 I', cleared with 'M5 I'.
  CUTTER_MODE_DYNAMIC       // M4 laser power is proportional to the feed rate, set with 'M4 I', cleared with 'M5 I'.
};

class SpindleLaser {
public:
  static CutterMode cutter_mode;

  static constexpr uint8_t pct_to_ocr(const_float_t pct) { return uint8_t(PCT_TO_PWM(pct)); }

  // cpower = configured values (e.g., SPEED_POWER_MAX)
  // Convert configured power range to a percentage
  static constexpr cutter_cpower_t power_floor = TERN(CUTTER_POWER_RELATIVE, SPEED_POWER_MIN, 0);
  static constexpr uint8_t cpwr_to_pct(const cutter_cpower_t cpwr) {
    return cpwr ? round(100.0f * (cpwr - power_floor) / (SPEED_POWER_MAX - power_floor)) : 0;
  }

  // Convert config defines from RPM to %, angle or PWM when in Spindle mode
  // and convert from PERCENT to PWM when in Laser mode
  static constexpr cutter_power_t cpwr_to_upwr(const cutter_cpower_t cpwr) { // STARTUP power to Unit power
    return (
      #if ENABLED(SPINDLE_FEATURE)
        // Spindle configured define values are in RPM
        #if CUTTER_UNIT_IS(RPM)
          cpwr                            // to same
        #elif CUTTER_UNIT_IS(PERCENT)
          cpwr_to_pct(cpwr)               // to Percent
        #elif CUTTER_UNIT_IS(SERVO)
          PCT_TO_SERVO(cpwr_to_pct(cpwr)) // to SERVO angle
        #else
          PCT_TO_PWM(cpwr_to_pct(cpwr))   // to PWM
        #endif
      #else
        // Laser configured define values are in Percent
        #if CUTTER_UNIT_IS(PWM255)
          PCT_TO_PWM(cpwr)                // to PWM
        #else
          cpwr                            // to same
        #endif
      #endif
    );
  }

  static constexpr cutter_power_t mpower_min() { return cpwr_to_upwr(SPEED_POWER_MIN); }
  static constexpr cutter_power_t mpower_max() { return cpwr_to_upwr(SPEED_POWER_MAX); }

  #if ENABLED(LASER_FEATURE)
    static cutter_test_pulse_t testPulse;                 // (ms) Test fire pulse duration
    static uint8_t last_block_power;                      // Track power changes for dynamic power

    static feedRate_t feedrate_mm_m, last_feedrate_mm_m;  // (mm/min) Track feedrate changes for dynamic power
    static bool laser_feedrate_changed() {
      const bool changed = last_feedrate_mm_m != feedrate_mm_m;
      if (changed) last_feedrate_mm_m = feedrate_mm_m;
      return changed;
    }
  #endif

  static bool isReadyForUI;               // Ready to apply power setting from the UI to OCR
  static bool enable_state;
  static uint8_t power,
                 last_power_applied;      // Basic power state tracking

  static cutter_frequency_t frequency;  // Set PWM frequency; range: 2K-50K

  static cutter_power_t menuPower,        // Power as set via LCD menu in PWM, Percentage or RPM
                        unitPower;        // Power as displayed status in PWM, Percentage or RPM

  static void init();

  #if ENABLED(HAL_CAN_SET_PWM_FREQ) && SPINDLE_LASER_FREQUENCY
    static void refresh_frequency() { hal.set_pwm_frequency(pin_t(SPINDLE_LASER_PWM_PIN), frequency); }
  #endif

  // Modifying this function should update everywhere
  static bool enabled(const cutter_power_t opwr) { return opwr > 0; }
  static bool enabled() { return enable_state; }

  static void apply_power(const uint8_t inpow);

  FORCE_INLINE static void refresh() { apply_power(power); }

  #if ENABLED(SPINDLE_LASER_USE_PWM)

    private:

    static void _set_ocr(const uint8_t ocr);

    public:

    static void set_ocr(const uint8_t ocr);
    static void ocr_off();

    /**
     * Update output for power->OCR translation
     */
    static uint8_t upower_to_ocr(const cutter_power_t upwr) {
      return uint8_t(
        #if CUTTER_UNIT_IS(PWM255)
          upwr
        #elif CUTTER_UNIT_IS(PERCENT)
          pct_to_ocr(upwr)
        #else
          pct_to_ocr(cpwr_to_pct(upwr))
        #endif
      );
    }

  #endif // SPINDLE_LASER_USE_PWM

  /**
   * Correct power to configured range
   */
  static cutter_power_t power_to_range(const cutter_power_t pwr, const uint8_t pwrUnit=_CUTTER_POWER(CUTTER_POWER_UNIT)) {
    static constexpr float
      min_pct = TERN(CUTTER_POWER_RELATIVE, 0, TERN(SPINDLE_FEATURE, round(100.0f * (SPEED_POWER_MIN) / (SPEED_POWER_MAX)), SPEED_POWER_MIN)),
      max_pct = TERN(SPINDLE_FEATURE, 100, SPEED_POWER_MAX);
    if (pwr <= 0) return 0;
    cutter_power_t upwr;
    switch (pwrUnit) {
      case _CUTTER_POWER_PWM255: {  // PWM
        const uint8_t pmin = pct_to_ocr(min_pct), pmax = pct_to_ocr(max_pct);
        upwr = cutter_power_t(constrain(pwr, pmin, pmax));
      } break;
      case _CUTTER_POWER_PERCENT:   // Percent
        upwr = cutter_power_t(constrain(pwr, min_pct, max_pct));
        break;
      case _CUTTER_POWER_RPM:       // Calculate OCR value
        upwr = cutter_power_t(constrain(pwr, SPEED_POWER_MIN, SPEED_POWER_MAX));
        break;
      default: break;
    }
    return upwr;
  }

  /**
   * Enable Laser or Spindle output.
   * It's important to prevent changing the power output value during inline cutter operation.
   * Inline power is adjusted in the planner to support LASER_TRAP_POWER and CUTTER_MODE_DYNAMIC mode.
   *
   * This method accepts one of the following control states:
   *
   *  - For CUTTER_MODE_STANDARD the cutter power is either full on/off or ocr-based and it will apply
   *    SPEED_POWER_STARTUP if no value is assigned.
   *
   *  - For CUTTER_MODE_CONTINUOUS inline and power remains where last set and the cutter output enable flag is set.
   *
   *  - CUTTER_MODE_DYNAMIC is also inline-based and it just sets the enable output flag.
   *
   *  - For CUTTER_MODE_ERROR set the output enable_state flag directly and set power to 0 for any mode.
   *    This mode allows a global power shutdown action to occur.
   */
  static void set_enabled(bool enable) {
    switch (cutter_mode) {
      case CUTTER_MODE_STANDARD:
        apply_power(enable ? TERN(SPINDLE_LASER_USE_PWM, (power ?: (unitPower ? upower_to_ocr(cpwr_to_upwr(SPEED_POWER_STARTUP)) : 0)), 255) : 0);
        break;
      case CUTTER_MODE_CONTINUOUS:
      case CUTTER_MODE_DYNAMIC:
        TERN_(LASER_FEATURE, set_inline_enabled(enable));
        break;
      case CUTTER_MODE_ERROR: // Error mode, no enable and kill power.
        enable = false;
        apply_power(0);
    }
    #if PIN_EXISTS(SPINDLE_LASER_ENA)
      WRITE(SPINDLE_LASER_ENA_PIN, enable ? SPINDLE_LASER_ACTIVE_STATE : !SPINDLE_LASER_ACTIVE_STATE);
    #endif
    enable_state = enable;
  }

  static void disable() { isReadyForUI = false; set_enabled(false); }

  // Wait for spindle/laser to startup or shutdown
  static void power_delay(const bool on) {
    safe_delay(on ? SPINDLE_LASER_POWERUP_DELAY : SPINDLE_LASER_POWERDOWN_DELAY);
  }

  #if ENABLED(SPINDLE_CHANGE_DIR)
    static void set_reverse(const bool reverse);
    static bool is_reverse() { return READ(SPINDLE_DIR_PIN) == SPINDLE_INVERT_DIR; }
  #else

    #if ENABLED(E3S1PRO_RTS)
      static inline void set_reverse(const bool) { cutter_mode = CUTTER_MODE_DYNAMIC; }
    #else
      static void set_reverse(const bool) {}
    #endif

    static bool is_reverse() { return false; }
  #endif

  #if ENABLED(AIR_EVACUATION)
    static void air_evac_enable();     // Turn On Cutter Vacuum or Laser Blower motor
    static void air_evac_disable();    // Turn Off Cutter Vacuum or Laser Blower motor
    static void air_evac_toggle();     // Toggle Cutter Vacuum or Laser Blower motor
    static bool air_evac_state() {     // Get current state
      return (READ(AIR_EVACUATION_PIN) == AIR_EVACUATION_ACTIVE);
    }
  #endif

  #if ENABLED(AIR_ASSIST)
    static void air_assist_enable();   // Turn on air assist
    static void air_assist_disable();  // Turn off air assist
    static void air_assist_toggle();   // Toggle air assist
    static bool air_assist_state() {   // Get current state
      return (READ(AIR_ASSIST_PIN) == AIR_ASSIST_ACTIVE);
    }
  #endif

  #if HAS_MARLINUI_MENU

    #if ENABLED(SPINDLE_FEATURE)
      static void enable_with_dir(const bool reverse) {
        isReadyForUI = true;
        const uint8_t ocr = TERN(SPINDLE_LASER_USE_PWM, upower_to_ocr(menuPower), 255);
        if (menuPower)
          power = ocr;
        else
          menuPower = cpwr_to_upwr(SPEED_POWER_STARTUP);
        unitPower = menuPower;
        set_reverse(reverse);
        set_enabled(true);
      }
      FORCE_INLINE static void enable_forward() { enable_with_dir(false); }
      FORCE_INLINE static void enable_reverse() { enable_with_dir(true); }
      FORCE_INLINE static void enable_same_dir() { enable_with_dir(is_reverse()); }
    #endif // SPINDLE_FEATURE

    #if ENABLED(SPINDLE_LASER_USE_PWM)
      static void update_from_mpower() {
        if (isReadyForUI) power = upower_to_ocr(menuPower);
        unitPower = menuPower;
      }
    #endif

    #if ENABLED(LASER_FEATURE)
      // Toggle the laser on/off with menuPower. Apply SPEED_POWER_STARTUP if it was 0 on entry.
      static void menu_set_enabled(const bool state) {
        set_enabled(state);
        if (state) {
          if (!menuPower) menuPower = cpwr_to_upwr(SPEED_POWER_STARTUP);
          power = TERN(SPINDLE_LASER_USE_PWM, upower_to_ocr(menuPower), 255);
          apply_power(power);
        } else
          apply_power(0);
      }

      /**
       * Test fire the laser using the testPulse ms duration
       * Also fires with any PWM power that was previous set
       * If not set defaults to 80% power
       */
      static void test_fire_pulse() {
        BUZZ(30, 3000);
        cutter_mode = CUTTER_MODE_STANDARD; // Menu needs standard mode.
        menu_set_enabled(true);             // Laser On
        delay(testPulse);                   // Delay for time set by user in pulse ms menu screen.
        menu_set_enabled(false);            // Laser Off
      }
    #endif // LASER_FEATURE

  #endif // HAS_MARLINUI_MENU

  #if ENABLED(LASER_FEATURE)

    // Dynamic mode rate calculation
    static uint8_t calc_dynamic_power() {
      if (feedrate_mm_m > 65535) return 255;    // Too fast, go always on
      uint16_t rate = uint16_t(feedrate_mm_m);  // 16 bits from the G-code parser float input
      rate >>= 8;                               // Take the G-code input e.g. F40000 and shift off the lower bits to get an OCR value from 1-255
      return uint8_t(rate);
    }

    // Inline modes of all other functions; all enable planner inline power control
    static void set_inline_enabled(const bool enable) { planner.laser_inline.status.isEnabled = enable; }

    // Set the power for subsequent movement blocks
    static void inline_power(const cutter_power_t cpwr) {
      TERN(SPINDLE_LASER_USE_PWM, power = planner.laser_inline.power = cpwr, planner.laser_inline.power = cpwr > 0 ? 255 : 0);
    }

  #endif // LASER_FEATURE

  static void kill() { disable(); }
};

extern SpindleLaser cutter;

#if HAS_LASER_E3S1PRO

enum device_header{
  DEVICE_UNKNOWN = 0xFF,  // unknown device
  DEVICE_LASER = 1,       // laser device
  DEVICE_FDM,             // fdm device
};

enum laser_device_range{
  LASER_MIN_X = 0,
  LASER_MIN_Y,
  LASER_MAX_X,
  LASER_MAX_Y,
};

// #include "../HAL/STM32F1/timers.h"
#include HAL_PATH(.., timers.h)

class spindle_laser_soft_pwm {
  private:
    device_header current_device=DEVICE_UNKNOWN;
    bool need_read_gcode_range = false;
    float laser_range[4]={0.0};

  public:
    const char laser_cmp_info[4][6]={"MINX:", "MINY:", "MAXX:", "MAXY:"};
    float pause_before_position_x=0, pause_before_position_y=0;
    bool  remove_card_before_is_printing = false;
    bool  already_show_warning = false;
    bool  laser_printing = false;
    uint16_t remain_time = 0;
    double laser_z_axis_high = 0;
    uint8_t power = 0;
    bool is_run_range=false;

  void quick_stop() {
    cutter.set_enabled(false);
    cutter.cutter_mode = CUTTER_MODE_STANDARD;
    cutter.apply_power(0);
  }

  void get_device_form_eeprom() {
    uint8_t buff[2]={0};
    BL24CXX::read(LASER_FDM_ADDR, &buff[0], 1);
    //SERIAL_ECHOLNPGM("get_device_form_eeprom", buff[0]);
    if ((device_header)buff[0] == DEVICE_LASER || (device_header)buff[0] == DEVICE_FDM)
      current_device = (device_header)buff[0];
    else
      current_device = DEVICE_UNKNOWN;
  }

  double get_z_axis_high_form_eeprom() {
    uint8_t buff[3]={0};
    uint16_t data=0;
    BL24CXX::read(LASER_Z_AXIS_HIGH_ADDR, &buff[0], 2);

    data = buff[0];
    data = (data<<8)|buff[1];

    if (data>LASER_Z_AXIS_HIGH_MAX*100) data = LASER_Z_AXIS_HIGH_MAX*100;

    laser_z_axis_high = data/100.0;

    return data/100.0;
  }

  void save_device_to_eeprom() {
    BL24CXX::write(LASER_FDM_ADDR, (uint8_t *)&current_device, 1);
    //SERIAL_ECHOLNPGM("save_device_to_eeprom", buff[0]);
  }

  void save_z_axis_high_to_eeprom(float data) {
    uint8_t buff[3]={0};
    uint16_t high = data*100;

    if (high >LASER_Z_AXIS_HIGH_MAX*100) high = LASER_Z_AXIS_HIGH_MAX*100;

    buff[0] = (high >> 8) & 0xFF;
    buff[1] = high&0xFF;

    BL24CXX::write(LASER_Z_AXIS_HIGH_ADDR, &buff[0], 2);
    laser_z_axis_high = data;
  }

  device_header get_current_device() {
    return current_device;
  }

  void set_current_device(device_header dev) {
    current_device = dev;
    save_device_to_eeprom();
  }

  bool is_laser_device() {
    return current_device == DEVICE_LASER;
  }

  bool is_unknown_device() {
    return current_device == DEVICE_UNKNOWN;
  }

  void set_read_gcode_range_on() {
    need_read_gcode_range = true;
  }

  void set_read_gcode_range_off() {
    need_read_gcode_range = false;
  }

  bool is_read_gcode_range_on() {
    return need_read_gcode_range==true;
  }

  void set_laser_range(laser_device_range index, float value) {
    laser_range[index] = value;
  }

  float get_laser_range(laser_device_range index) {
    return laser_range[index];
  }

  uint8_t power16_to_8(uint16_t power) {
    if (power == 0) return 0;
    double p = (uint8_t)((power / 1000.0) * 255);
    NOLESS(p, 1);
    return p;
  }

  void reset_data() {
    laser_range[LASER_MIN_X] = 0.0;
    laser_range[LASER_MIN_Y] = 0.0;
    laser_range[LASER_MAX_X] = 0.0;
    laser_range[LASER_MAX_Y] = 0.0;
  }

  void soft_pwm_init() {
    _SET_OUTPUT(LASER_SOFT_PWM_PIN);
    laser_timer_soft_pwm_init(LASER_TIMER_FREQUENCY);
  }

  void laser_power_start(const uint8_t power) { //0 - 255
    //SERIAL_ECHOLNPGM("_laser.h 424 =", power);
    laser_timer_soft_pwm_start(power);
  }

  void laser_power_stop() {
    laser_timer_soft_pwm_stop();
  }

  void laser_power_close() {
    laser_timer_soft_pwm_close();
  }

  void laser_power_open() {
    laser_power_start(1);
  }
};

extern class spindle_laser_soft_pwm laser_device;

// //set PWM
// // : power 0-255
// void laser_power_start(const uint8_t power);

// // stop pwm
// void laser_power_stop();

#endif // HAS_CUTTER
