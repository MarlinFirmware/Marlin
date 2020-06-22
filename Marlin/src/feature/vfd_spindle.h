#pragma once

/**
 * feature/spindle_laser.h
 * Support for Laser Power or Spindle Power & Direction
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(SPINDLE_VFD)
#define _MSG_CUTTER(M) MSG_SPINDLE_##M
#endif
#define MSG_CUTTER(M) _MSG_CUTTER(M)

typedef uint16_t cutter_power_t;
#define CUTTER_MENU_TYPE uint16_t

// VFD spindle control for RS485 VFD's.
// 
// See VFD protocol.md for details on the messaging. 
class VFDSpindle
{
  // 50 bytes should be plenty for whatever the VFD can throw at us.
  static const int RECEIVE_BUFFER_SIZE = 50;
  static uint8_t vfd_receive_buffer[RECEIVE_BUFFER_SIZE];

  static const int SEND_BUFFER_SIZE = 20;
  static uint8_t vfd_send_buffer[SEND_BUFFER_SIZE];

  static uint16_t get_crc_value(uint8_t* data_value, uint8_t length);

  static void crc_check_value(uint8_t* data_value, uint8_t length);

  static bool validate_crc_value(uint8_t* data_value, uint8_t length);

  static void init_pins();

  static int receive_data_detail();

  static void send_data_detail(uint8_t* buffer, int length);

  static int query(int send_length);

  // Gets the current direction status of the VFD:
  // -1 = backward, 1 = forward, 0 = idle, -2 = communication error.
  static int get_direction_state();

  static uint16_t get_max_rpm();

  static uint16_t get_current_rpm();

  // direction<0: reverse
  // direction>0: forward
  // direction=0: stop
  static void set_current_direction(int direction);

  static void set_speed(uint16_t rpm);

  // Apparently some G-codes return 0-255 instead of a normal value... This
  // normalizes these numbers to RPM's.
  static uint16_t normalize_power(int32_t value);

public:
  static cutter_power_t power;
  static int direction;
  static bool enabled;

  static inline uint8_t powerPercent(const uint8_t pp) { return ui8_to_percent(pp); } // for display

  static void init();

  static void set_power(const cutter_power_t pwr);

  static inline void refresh()
  {
    apply_power(power);
  }

  static void set_enabled(const bool enable);

  static void apply_power(const cutter_power_t inpow);

  // Wait for spindle to spin up or spin down
  static void power_delay();

  static void set_direction(const bool reverse);

  static inline void disable() { set_enabled(false); }
  static inline void enable_forward() { set_direction(false); set_enabled(true); }
  static inline void enable_reverse() { set_direction(true); set_enabled(true); }
};

extern VFDSpindle cutter;
