#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "../user_interface.h"

#include <list>
#include <deque>

#include "../virtual_printer.h"
#include "Gpio.h"

#ifndef LCD_WIDTH
  #define LCD_WIDTH 20
#endif

#ifndef LCD_HEIGHT
  #define LCD_HEIGHT 4
#endif

#ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
  static_assert(LCD_WIDTH == 16 || LCD_WIDTH == 20, "Unsupported Character LCD Width");
  static_assert(LCD_HEIGHT == 2 || LCD_HEIGHT == 4, "Unsupported Character LCD Height");
#endif

class HD44780Device: public VirtualPrinter::Component {
public:
  enum KeyName {
    KILL_BUTTON, ENCODER_BUTTON, BACK_BUTTON, COUNT
  };

  HD44780Device(pin_type rs, pin_type en, pin_type d4, pin_type d5, pin_type d6, pin_type d7, pin_type beeper, pin_type enc1, pin_type enc2, pin_type enc_but, pin_type back, pin_type kill);
  virtual ~HD44780Device();
  void process_command(uint8_t cmd);
  void update();
  void interrupt(GpioEvent& ev);
  void ui_init();
  void ui_widget();

  struct {
    void update_address_counter(int8_t direction = 0) {
      direction = direction == 0 ? increment_direction : direction;
      if (active_address_space == DDRAM && display_lines == 2 && direction == -1 && address_counter == 0x40 ) address_counter = 0x27;
      else address_counter += direction;
      clamp_address_counter();
    };

    void clamp_address_counter() {
      if (active_address_space == DDRAM) {
        uint32_t max_address_counter = (display_lines == 1) ? 0x4F : 0x67;
        if (address_counter > max_address_counter) address_counter -= max_address_counter + 1;
        else if (display_lines == 2 && address_counter > 0x27 && address_counter < 0x40) address_counter = 0x40 + (address_counter - 0x28);
      } else address_counter &= 0x3F;
    };

    void update_display_shift(int8_t direction = 0) {
      direction = direction == 0 ? increment_direction : direction;
      display_shift += direction;
      if (display_shift == 0x50) display_shift = 0;
      else if (display_shift > 0x50) display_shift = 0x4F;
    };

    uint32_t address_counter = 0;
    enum { DDRAM, CGRAM } active_address_space = DDRAM;
    int8_t increment_direction = 1;
    bool display_shift_enabled = false;
    bool display_enabled = false;
    uint8_t display_shift = 0;
    uint8_t display_lines = 1;
    uint8_t data_width = 0;
    uint8_t character_size = 0;
    bool busy = false;
    bool cursor_on = false;
    bool cursor_blink = false;
  } state;

  pin_type rs_pin, en_pin, d4_pin, d5_pin, d6_pin, d7_pin, beeper_pin, enc1_pin, enc2_pin, enc_but_pin, back_pin, kill_pin;

  uint32_t ddram_address = 0, cgram_address = 0;
  enum { DDRAM, CGRAM } active_address_space = DDRAM;

  static constexpr uint16_t ddram_buffer_size = 104;
  uint8_t ddram_buffer[ddram_buffer_size] = {};

  static constexpr uint16_t cgram_buffer_size = 64;
  uint8_t cgram_buffer[cgram_buffer_size] = {};

  bool data_low_nibble = false, data_is_command = false;
  uint8_t data_byte = 0;

  bool key_pressed[KeyName::COUNT] = {};
  uint8_t encoder_position = 0;
  static constexpr int8_t encoder_table[4] = {1, 3, 2, 0};

  bool dirty = true;
  std::chrono::steady_clock clock;
  std::chrono::steady_clock::time_point last_update;
  float scaler;
  GLuint texture_id;

  static constexpr uint32_t display_x_char = LCD_WIDTH;
  static constexpr uint32_t display_y_char = LCD_HEIGHT;
  static constexpr uint32_t display_margin = 6;
  static constexpr uint32_t display_char_pad = 1;

  static constexpr uint32_t texture_x = 1 + (5 * display_x_char) + (display_margin * 2) + ((display_x_char - 1) * display_char_pad);
  static constexpr uint32_t texture_y = (8 * display_y_char) + (display_margin * 2) + ((display_y_char - 1) * display_char_pad);
  static constexpr uint32_t width = texture_x, height = texture_y;
  bool render_integer_scaling = false, render_popout = false;

  glm::vec<3, uint8_t> texture_data[texture_x * texture_y] = {};

  glm::ivec3 foreground_color = {0x81, 0xF2, 0xFF};
  glm::ivec3 background_color = {0x33, 0x01, 0xFC};
  glm::ivec3 display_color = {0x23, 0x2A, 0xFC};

  uint8_t* active_rom = nullptr;
};
