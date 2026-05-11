#include <mutex>
#include <fstream>
#include <cmath>
#include <random>
#include "Gpio.h"

#include <gl.h>
#include "../renderer/renderer.h"

#include "../imgui_custom.h"

#include "HD44780Device.h"
#include "HD44780DeviceROM.h"

#include <src/inc/MarlinConfig.h>

HD44780Device::HD44780Device(pin_type rs, pin_type en, pin_type d4, pin_type d5, pin_type d6, pin_type d7, pin_type beeper, pin_type enc1, pin_type enc2, pin_type enc_but, pin_type back, pin_type kill)
  : rs_pin(rs), en_pin(en), d4_pin(d4), d5_pin(d5), d6_pin(d6), d7_pin(d7), beeper_pin(beeper), enc1_pin(enc1), enc2_pin(enc2), enc_but_pin(enc_but), back_pin(back), kill_pin(kill) {

  Gpio::attach(rs_pin, [this](GpioEvent& event){ this->interrupt(event); });
  data_is_command = !Gpio::get_pin_value(rs_pin); // make sure the initial state is updated
  Gpio::attach(en_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(d4_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(d5_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(d6_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(d7_pin, [this](GpioEvent& event){ this->interrupt(event); });

  Gpio::attach(beeper_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc1_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc2_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc_but_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(back_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(kill_pin, [this](GpioEvent& event){ this->interrupt(event); });

  for (auto& pixel : texture_data) pixel = display_color;

  #if DISPLAY_CHARSET_HD44780 == JAPANESE
    active_rom = hd44780_a00_rom;
  #elif DISPLAY_CHARSET_HD44780 == WESTERN
    active_rom = hd44780_a02_rom;
  #else
    #error Unavailable HD44780 Character ROM
  #endif
}

HD44780Device::~HD44780Device() {}

void HD44780Device::process_command(uint8_t cmd) {
  uint8_t cmd_id = 0, cmd_data = cmd;
  while (cmd >>= 1) { cmd_id++; } // Commands are indicated by the bit index of the highest set bit
  //printf("CMD_ID: %d\n", cmd_id);
  switch (cmd_id) {
    case 0:
      //printf("Clear Display\n");
      memset(ddram_buffer, 0x20, ddram_buffer_size);
      // fallthough intended
    case 1:
      //printf("Return Home\n");
      state.address_counter = 0;
      state.active_address_space = state.DDRAM;
      state.increment_direction = 1;
      state.display_shift = 0;
      break;
    case 2:
      state.increment_direction = (TEST(cmd_data, 1)) ? +1 : -1;
      state.display_shift_enabled = TEST(cmd_data, 0);
      //printf("Entry Mode Set(increment: %d, shift: %d)\n", state.increment_direction, state.display_shift_enabled);
      break;
    case 3:
      state.cursor_blink = TEST(cmd_data, 0);
      state.cursor_on = TEST(cmd_data, 1);
      state.display_enabled = TEST(cmd_data, 2);
      //printf("Display On/Off (Display %d, Cursor %d, Blink %d)\n", state.display_enabled, state.cursor_on, state.cursor_blink);
      break;
    case 4: {
      //printf("Dispaly/Cursor Shift\n");
      int8_t direction = (TEST(data_byte, 2)) ? +1 : -1;
      if (TEST(data_byte, 3)) state.update_display_shift(direction);
      else state.update_address_counter(direction);
      break;
    }
    case 5:
      if (state.data_width == (TEST(cmd_data, 4) ? 8 : 4)) {
        break;
      }
      state.data_width = TEST(cmd_data, 4) ? 8 : 4;
      state.display_lines = TEST(cmd_data, 3) + 1;
      state.character_size = TEST(cmd_data, 2) ? 10 : 8;
      //printf("Function Set(data width %d, lines %d, char size %d)\n", state.data_width, state.display_lines, state.character_size);
      break;
    case 6:
      state.address_counter = cmd_data & 0x3F;
      state.active_address_space = state.CGRAM;
      //printf("Set CGRAM Addr(%d)\n", cgram_address);
      break;
    case 7:
      state.address_counter = cmd_data & 0x7F;
      state.active_address_space = state.DDRAM;
      state.clamp_address_counter();
      //printf("Set DDRAM Addr(%d)\n", ddram_address);
      break;
  }
}

void HD44780Device::update() {
  auto now = clock.now();
  float delta = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_update).count();

  if (dirty && delta > 1.0 / 30.0) {
    last_update = now;
    dirty = false;

    if (state.display_lines == 2) {
      uint32_t line_length = (LCD_WIDTH * LCD_HEIGHT) / state.display_lines;
      for (std::size_t line = 0; line < state.display_lines; line ++) {
        for (std::size_t line_xindex = 0; line_xindex < line_length; line_xindex ++) {
          uint32_t index = line * 0x40 + ((line_xindex + state.display_shift) % line_length);
          uint32_t charset_base = 0;
          if (ddram_buffer[index] < 0x10) {
            charset_base = state.character_size == 8 ? (ddram_buffer[index] & 0x07) * 8 : ((ddram_buffer[index] >> 1) * 0x03) * 16;
          } else charset_base = ddram_buffer[index] * 0x10;
          uint8_t* charset = (ddram_buffer[index] < 0x10) ? cgram_buffer : active_rom;

          uint16_t x = line_xindex > LCD_WIDTH-1 ? line_xindex - LCD_WIDTH : line_xindex;
          uint16_t y = (line == 1) | ((line_xindex > LCD_WIDTH-1) << 1);

          uint16_t texture_index_x = display_margin + (5 + display_char_pad) * x;
          uint16_t texture_index_y = display_margin + (8 + display_char_pad) * y;

          for (std::size_t ty = 0; ty < 8; ty++ ) {
            for (std::size_t tx = 0; tx < 5; tx++ ) {
              texture_data[((texture_index_y + ty) * texture_x) + texture_index_x - tx + 5] = TEST(charset[charset_base + ty], tx) ? foreground_color : background_color;
            }
          }
        }
      }
    }

    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, texture_id);
    renderer::gl_assert_call(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA8, texture_x, texture_y, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, 0);
  }
}

void HD44780Device::interrupt(GpioEvent& ev) {
  if(ev.pin_id == en_pin && ev.event == GpioEvent::RISE) {
    //read the bus
    data_byte |= (Gpio::get_pin_value(d7_pin) << 3 | Gpio::get_pin_value(d6_pin) << 2 | Gpio::get_pin_value(d5_pin) << 1 |Gpio::get_pin_value(d4_pin)) << (4 * !data_low_nibble);
    data_low_nibble = !data_low_nibble;
    if (!data_low_nibble) {
      dirty = true; // can't tell when the data transmission has ended? so set dirty on all received bytes
      if (data_is_command) process_command(data_byte);
      else {
        if (state.active_address_space == state.DDRAM) {
          ddram_buffer[state.address_counter] = data_byte;
        } else {
          cgram_buffer[state.address_counter] = data_byte;
        }
        state.update_address_counter();
        if (state.display_shift_enabled) state.update_display_shift();
      }
      data_byte = 0;
    }
  } else if (ev.pin_id == rs_pin) {
    if (ev.event == GpioEvent::RISE) {
      data_is_command = false;
    } else if (ev.event == GpioEvent::FALL) {
      data_is_command = true;
    }
  } else if (ev.pin_id == beeper_pin) {
    if (ev.event == GpioEvent::RISE) {
      // play sound
    } else if (ev.event == GpioEvent::FALL) {
      // stop sound
    }
  } else if (ev.pin_id == kill_pin) {
    Gpio::set_pin_value(kill_pin, !key_pressed[KeyName::KILL_BUTTON]);
  } else if (ev.pin_id == enc_but_pin) {
    Gpio::set_pin_value(enc_but_pin, !key_pressed[KeyName::ENCODER_BUTTON]);
  } else if (ev.pin_id == back_pin) {
    Gpio::set_pin_value(back_pin, !key_pressed[KeyName::BACK_BUTTON]);
  } else if (ev.pin_id == enc1_pin || ev.pin_id == enc2_pin) {
    const uint8_t encoder_state = encoder_position % 4;
    Gpio::set_pin_value(enc1_pin, encoder_table[encoder_state] & 0x01);
    Gpio::set_pin_value(enc2_pin, encoder_table[encoder_state] & 0x02);
  }
}

void HD44780Device::ui_init() {
  renderer::gl_assert_call(glGenTextures, 1, &texture_id);
  renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, texture_id);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, 0);
}

void HD44780Device::ui_widget() {
  bool popout_begin = false;
  static long int call_count = 0;
  static uint8_t up_held = 0, down_held = 0;
  call_count++;

  auto size = ImGui::GetContentRegionAvail();
  size.y = ((size.x / (width / (float)height)) * !render_popout) + 60;

  if (ImGui::BeginChild("HD44780Device", size)) {
    ImGui::GetCurrentWindow()->ScrollMax.y = 1.0f; // disable window scroll
    ImGui::Checkbox("Integer Scaling", &render_integer_scaling);
    ImGui::Checkbox("Popout", &render_popout);

    if (render_popout) {
      const imgui_custom::constraint_t constraint { width + imgui_custom::hfeat, height + imgui_custom::vfeat, (width) / (float)(height) };
      // Init the window size to contain the 2x scaled screen, margin, and window features
      ImGui::SetNextWindowSize(ImVec2(constraint.minw + width, constraint.minh + height), ImGuiCond_Once);
      ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), imgui_custom::CustomConstraints::AspectRatio, (void*)&constraint);

      popout_begin = ImGui::Begin("HD44780DeviceRender", &render_popout);
      if (!popout_begin) {
        ImGui::End();
        return;
      }
      size = ImGui::GetContentRegionAvail();
    }

    // Apply the smallest scale that fits the window. Maintain proportions.
    size = imgui_custom::scale_proportionally(size, width, height, render_integer_scaling);

    ImGui::Image((ImTextureID)(intptr_t)texture_id, size, ImVec2(0,0), ImVec2(1,1));
    if (ImGui::IsWindowFocused()) {

      key_pressed[KeyName::KILL_BUTTON]    = ImGui::IsKeyDown(ImGuiKey_K);
      key_pressed[KeyName::ENCODER_BUTTON] = ImGui::IsKeyDown(ImGuiKey_Space) || ImGui::IsKeyDown(ImGuiKey_Enter) || ImGui::IsKeyDown(ImGuiKey_RightArrow);
      key_pressed[KeyName::BACK_BUTTON]    = ImGui::IsKeyDown(ImGuiKey_LeftArrow);

      // Turn keypresses (and repeat) into encoder clicks
      if (up_held) { up_held--; encoder_position--; }
      else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) up_held = 4;
      if (down_held) { down_held--; encoder_position++; }
      else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) down_held = 4;

      if (ImGui::IsItemHovered()) {
        key_pressed[KeyName::ENCODER_BUTTON] |= ImGui::IsMouseClicked(0);
        encoder_position += ImGui::GetIO().MouseWheel > 0 ? 1 : ImGui::GetIO().MouseWheel < 0 ? -1 : 0;
      }
    }

    if (popout_begin) ImGui::End();
  }
  ImGui::EndChild();
}
