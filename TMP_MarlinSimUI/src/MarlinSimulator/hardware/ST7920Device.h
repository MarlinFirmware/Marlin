#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "../user_interface.h"

#include <list>
#include <deque>

#include "../virtual_printer.h"
#include "Gpio.h"

class ST7920Device: public VirtualPrinter::Component {
public:
  enum KeyName {
    KILL_BUTTON, ENCODER_BUTTON, BACK_BUTTON, COUNT
  };

  struct Command {
    Command(uint8_t rw, uint8_t rs, uint8_t data) : rw(rw), rs(rs), data(data){};
    uint8_t rw = 0;
    uint8_t rs = 0;
    uint8_t data = 0;
  };

  ST7920Device(pin_type clk, pin_type mosi, pin_type cs, pin_type beeper, pin_type enc1, pin_type enc2, pin_type enc_but, pin_type back, pin_type kill);
  virtual ~ST7920Device();
  void process_command(Command cmd);
  void update();
  void interrupt(GpioEvent& ev);
  void ui_init() override;
  void ui_widget() override;

  static constexpr uint32_t width = 128, height = 64;
  bool render_integer_scaling = false, render_popout = false;

  pin_type clk_pin, mosi_pin, cs_pin, beeper_pin, enc1_pin, enc2_pin, enc_but_pin, back_pin, kill_pin;

  bool extended_instruction_set = false;

  uint8_t incoming_byte = 0;
  uint8_t incoming_bit_count = 0;
  uint8_t incoming_byte_count = 0;
  uint8_t incoming_cmd[3] = {};

  // Unused by current emulation
  // static constexpr uint8_t display_ram_size = 64 * (16 / 8);
  // uint8_t display_ram[display_ram_size] = {};  // 64 x 16bit
  // uint16_t display_ram_index = 0;

  static constexpr uint16_t graphic_ram_size = 64 * (256 / 8);
  uint8_t graphic_ram[graphic_ram_size] = {}; // 64 x 256bit
  uint16_t graphic_ram_index = 0;
  uint16_t graphic_ram_index_x = 0, graphic_ram_index_y = 0;

  static constexpr uint8_t ddram_size = 120; //  60 * 16 bits
  uint8_t ddram[ddram_size] = {};

  static constexpr uint8_t cgram_size = 128; // 1024 bits
  uint8_t cgram[cgram_size] = {};

  bool in_cgram = false; // whether address_counter refers to CGRAM
  bool cgram_written = false; // whether CGRAM has been written to

  // Separate address counters for DDRAM and CGRAM
  uint32_t ddram_address_counter = 0;
  uint32_t cgram_address_counter = 0;
  int8_t address_increment = 1;

  uint8_t coordinate[2] = {};
  uint8_t coordinate_index = 0;

  bool key_pressed[KeyName::COUNT] = {};
  uint8_t encoder_position = 0;
  static constexpr int8_t encoder_table[4] = {1, 3, 2, 0};

  bool dirty = true;
  std::chrono::steady_clock clock;
  std::chrono::steady_clock::time_point last_update;
  float scaler;
  GLuint texture_id;
  glm::vec<3, uint8_t> texture_data[128*64] = {};

  glm::ivec3 foreground_color = {0x81, 0xF2, 0xFF};
  glm::ivec3 background_color = {0x33, 0x01, 0xFC};
};
