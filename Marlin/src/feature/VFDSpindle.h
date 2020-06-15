#pragma once

/**
 * feature/spindle_laser.h
 * Support for Laser Power or Spindle Power & Direction
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(SPINDLE_FEATURE)
#define _MSG_CUTTER(M) MSG_SPINDLE_##M
#else
#define _MSG_CUTTER(M) MSG_LASER_##M
#endif
#define MSG_CUTTER(M) _MSG_CUTTER(M)

typedef uint16_t cutter_power_t;
#define CUTTER_MENU_TYPE uint16_5

class VFDSpindle
{
  static void send(const hy_command_t& cmd);
  static void add_ModRTU_CRC(char* buf, int full_msg_len);

public:
  static cutter_power_t power;
  static int mode;

  static inline uint8_t powerPercent(const uint8_t pp) { return ui8_to_percent(pp); } // for display

  static void init();

  static inline bool enabled() { return !!power; }

  static inline void set_power(const cutter_power_t pwr) { power = pwr; }

  static inline void refresh() { apply_power(power); }

  static inline void set_enabled(const bool enable) {
    const bool was = enabled();
    set_power(enable ? SPEED_POWER_MAX : 0);
    if (was != enable) power_delay();
  }

  static void apply_power(const cutter_power_t inpow);

  static bool active();

  static void update_output();

  // Wait for spindle to spin up or spin down
  static void power_delay();
  static void set_direction(const bool reverse);

  static inline void disable() { set_enabled(false); }
  static inline void enable_forward() { set_direction(false); set_enabled(true); }
  static inline void enable_reverse() { set_direction(true); set_enabled(true); }

};

extern VFDSpindle cutter;
