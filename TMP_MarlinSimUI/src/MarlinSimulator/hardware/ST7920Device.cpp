#include <mutex>
#include <fstream>
#include <cmath>
#include <random>
#include "Gpio.h"

#include <gl.h>
#include "../renderer/renderer.h"
#include "../imgui_custom.h"

#include "ST7920Device.h"
#include "ST7920DeviceROM.h"

#define DEBUG_ST7920_CMD 1
#define DEBUG_ST7920_CGRAM 2
#define DEBUG_ST7920_DDRAM 4
#define DEBUG_ST7920_CHARS 8
//#define DEBUG_ST7920 DEBUG_ST7920_CMD | DEBUG_ST7920_CGRAM | DEBUG_ST7920_DDRAM | DEBUG_ST7920_CHARS

ST7920Device::ST7920Device(pin_type clk, pin_type mosi, pin_type cs, pin_type beeper, pin_type enc1, pin_type enc2, pin_type enc_but, pin_type back, pin_type kill)
  : clk_pin(clk), mosi_pin(mosi), cs_pin(cs), beeper_pin(beeper), enc1_pin(enc1), enc2_pin(enc2), enc_but_pin(enc_but), back_pin(back), kill_pin(kill) {

  Gpio::attach(clk_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(cs_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(beeper_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc1_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc2_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc_but_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(back_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(kill_pin, [this](GpioEvent& event){ this->interrupt(event); });
}

ST7920Device::~ST7920Device() {}

void ST7920Device::process_command(Command cmd) {
  if (cmd.rs) {
    // Data write
    if (extended_instruction_set) {
      // Graphics RAM write
      graphic_ram[coordinate[1] + (coordinate[0] * (256 / 8))] = cmd.data;
      if (++coordinate[1] > 32) coordinate[1] = 0;
      dirty = true;
    } else {
      // DDRAM / CGRAM memory write
      if (in_cgram) {
        // Character Generator RAM write
        // Data is loaded as interleaved values, high byte then low byte
        // On loading data is reordered into character blocks. High bytes 0-15, low bytes 16-31
        static size_t cgram_write_index = 0;
        if (cgram_write_index < cgram_size) {
          size_t block = (cgram_write_index / 32) * 32;
          size_t n = (cgram_write_index / 2) % 16;
          size_t store_addr = block + ((cgram_write_index % 2 == 0) ? n : n + 16);
          cgram[store_addr] = cmd.data;
          #if DEBUG_ST7920 & DEBUG_ST7920_CGRAM
            printf("CGRAM[%zu] = 0x%02X\n", store_addr, cmd.data);
          #endif
          cgram_written = true;
          cgram_write_index++;
        }
        #if DEBUG_ST7920 & DEBUG_ST7920_CGRAM
          printf("CGRAM DUMP:\n");
          for (std::size_t row = 0; row < 8; row++) {
            printf("%02zX: [%p] ", row * 16, (void*)&cgram[row * 16]);
            for (std::size_t col = 0; col < 16; col++) {
              size_t idx = row * 16 + col;
              printf("0x%02X%s", cgram[idx], (col < 15) ? ", " : "\n");
            }
          }
        #endif
      } else {
        if (ddram_address_counter < ddram_size) {
          ddram[ddram_address_counter] = cmd.data;
          #if DEBUG_ST7920 & DEBUG_ST7920_DDRAM
            char ascii_char = (cmd.data >= 0x20 && cmd.data <= 0x7E) ? (char)cmd.data : '.';
            printf("DDRAM[%d] = 0x%02X('%c')\n", ddram_address_counter, cmd.data, ascii_char);
          #endif
        }
        // auto-increment / decrement (per entry mode set)
        ddram_address_counter += address_increment;
        ddram_address_counter = (ddram_address_counter + ddram_size) % ddram_size;
      }

      dirty = true;
    }
  }
  else if (extended_instruction_set) {
    if (cmd.data & (1 << 7)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [7] SET GRAPHICS RAM COORDINATE (0x%02X)\n", cmd.data);
      #endif
      coordinate[coordinate_index++] = cmd.data & 0x7F;
      if (coordinate_index == 2) {
        coordinate_index = 0;
        coordinate[1] *= 2;
        if (coordinate[1] >= 128 / 8) {
          coordinate[1] = 0;
          coordinate[0] += 32;
        }
      }
    } else if (cmd.data & (1 << 6)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [6] SET IRAM OR SCROLL ADDRESS (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 5)) {
      extended_instruction_set = cmd.data & 0b100;
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [5] EXTENDED FUNCTION SET (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 4)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [4] UNKNOWN? (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 3)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [3] UNKNOWN? (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 2)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [2] REVERSE (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 1)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [1] VERTICAL SCROLL OR RAM ADDRESS SELECT (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 0)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [0] STAND BY\n");
      #endif
    }
  } else {
    if (cmd.data & (1 << 7)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [7] SET DDRAM ADDRESS (0x%02X)\n", cmd.data);
      #endif
      ddram_address_counter = (cmd.data & 0x3f) * 2; // 7 bits but bit 6 is 0
      in_cgram = false;
    } else if (cmd.data & (1 << 6)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [6] SET CGRAM ADDRESS (0x%02X)\n", cmd.data);
      #endif
      cgram_address_counter = cmd.data & 0x3F;
      in_cgram = true;
    } else if (cmd.data & (1 << 5)) {
      extended_instruction_set = cmd.data & 0b100;
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [5] FUNCTION SET (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 4)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [4] CURSOR DISPLAY CONTROL (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 3)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [3] DISPLAY CONTROL (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 2)) {
      address_increment = (cmd.data & 0x2) ? 1 : -1;
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [2] ENTRY MODE SET (0x%02X)\n", cmd.data);
      #endif
    } else if (cmd.data & (1 << 1)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [1] RETURN HOME (0x%02X)\n", cmd.data);
      #endif
      ddram_address_counter = 0;
      cgram_address_counter = 0;
    } else if (cmd.data & (1 << 0)) {
      #if DEBUG_ST7920 & DEBUG_ST7920_CMD
        printf("cmd: [0] DISPLAY CLEAR\n");
      #endif
      address_increment = 1;
      cgram_address_counter = 0;
      ddram_address_counter = 0;
      memset(graphic_ram, 0, sizeof(graphic_ram));
      memset(ddram, 0x20, sizeof(ddram));
      //memset(cgram, 0, sizeof(cgram));
      dirty = true;
    }
  }
}

/*
 * The 16x4 LCD character layout. Each entry is 16 bits, in high / low bytes
 * Line 1:  0x80 0x81 0x82 0x83 0x84 0x85 0x86 0x87
 * Line 2:  0x90 0x91 0x92 0x93 0x94 0x95 0x96 0x97
 * Line 3:  0x88 0x89 0x8A 0x8B 0x8C 0x8D 0x8E 0x8F
 * Line 4:  0x98 0x99 0x9A 0x9B 0x9C 0x9D 0x9E 0x9F
 *
 * In the sim memory this is mappeed to bytes see physical_line_to_ddram_offset
 * Line 1:  0x00 .. 0x0F
 * Line 2:  0x20 .. 0x2F
 * Line 3:  0x10 .. 0x1F
 * Line 4:  0x30 .. 0x3F
 */

void ST7920Device::update() {
  auto now = clock.now();
  float delta = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_update).count();

  if (dirty && delta > 1.0 / 30.0) {
    last_update = now;
    dirty = false;

  static const uint8_t physical_line_to_ddram_offset[4] = { 0x00, 0x20, 0x10, 0x30 };

  #if DEBUG_ST7920 & DEBUG_ST7920_CHARS
    // Debug: print all 4 rows in one line each
    for (std::size_t char_y = 0; char_y < 4; char_y++) {
        printf("Row %zu: ", char_y);
        for (std::size_t char_x = 0; char_x < 16; char_x++) {
          std::size_t ddram_index = 0x80 + physical_line_to_ddram_offset[char_y] + char_x;
          uint8_t raw_value = ddram[ddram_index - 0x80];
          uint8_t char_code = raw_value & 0x7F;

          // Print ASCII if printable, else print as hex
          if (char_code >= 0x20 && char_code <= 0x7E) {
            printf("[0x%02zX]='%c' ", ddram_index, (char)char_code);
          } else {
            printf("[0x%02zX]=0x%02X ", ddram_index, char_code);
          }
        }
        printf("\n");
        fflush(stdout);
    }
  #endif

  // draw screen in 16x16 blocks to allow for character mode
  for (std::size_t char_y = 0; char_y < 4; char_y++) {
    for (std::size_t char_x = 0; char_x < 8; char_x++) {
      std::size_t ddram_index = 0x80 + physical_line_to_ddram_offset[char_y] + char_x * 2;
      uint8_t high_char_code = ddram[ddram_index - 0x80] & 0x7F;
      uint8_t low_char_code  = ddram[ddram_index + 1 - 0x80] & 0x7F;
      const uint8_t *high_glyph_ptr, *low_glyph_ptr;
      if ((high_char_code == 0) && (low_char_code < 8) && cgram_written) {
        high_glyph_ptr = &cgram[low_char_code * 16];
        low_glyph_ptr  = high_glyph_ptr +16;
        #if DEBUG_ST7920 & DEBUG_ST7920_CGRAM
          printf("CGRAM address: [%p]\n", (void*)&cgram[0]);
          printf("High_glyph address: [%p]\n", (void*)high_glyph_ptr);
          printf("Low_glyph address: [%p]\n", (void*)low_glyph_ptr);
          printf("Using CGRAM [%d]\n",low_char_code);
          for (std::size_t i = 0; i < 32; i++) {
            printf("DUMP [0x%02zX]=0x%02X\n", i, high_glyph_ptr[i]);
          }
        #endif
      } else {
        high_glyph_ptr = &st7920_cgrom[high_char_code * 16];
        low_glyph_ptr  = &st7920_cgrom[low_char_code * 16];
      }
      // Draw this 16x16 block
      for (std::size_t row = 0; row < 16; row++) {
        uint16_t row_bits = (high_glyph_ptr[row] << 8) | low_glyph_ptr[row];
        for (std::size_t col = 0; col < 16; col++) {
          std::size_t pixel_x = char_x * 16 + col;
          std::size_t pixel_y = char_y * 16 + row;
          std::size_t texture_index = pixel_y * 128 + pixel_x;
          bool char_pixel = TEST(row_bits, 15 - col);
          std::size_t graphic_ram_index = (pixel_y * 32) + (pixel_x / 8);
          bool gdram_pixel = TEST(graphic_ram[graphic_ram_index], 7 - (pixel_x % 8));
          bool final_pixel = char_pixel || gdram_pixel;
          texture_data[texture_index] = final_pixel ? foreground_color : background_color;
        }
      }
    }
  }
    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, texture_id);
    renderer::gl_assert_call(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA8, 128, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, 0);
  }
}

void ST7920Device::interrupt(GpioEvent& ev) {
  if (ev.pin_id == clk_pin && ev.event == GpioEvent::RISE && Gpio::get_pin_value(cs_pin)){
    incoming_byte = (incoming_byte << 1) | Gpio::get_pin_value(mosi_pin);
    if (++incoming_bit_count == 8) {
      if (incoming_byte_count == 0 && (incoming_byte & 0xF8) != 0xF8) {
        incoming_byte_count++;
      }
      incoming_cmd[incoming_byte_count++] = incoming_byte;
      incoming_byte = incoming_bit_count = 0;
      if (incoming_byte_count == 3) {
        process_command({(incoming_cmd[0] & 0b100) != 0, (incoming_cmd[0] & 0b010) != 0, uint8_t(incoming_cmd[1] | incoming_cmd[2] >> 4)});
        incoming_byte_count = 0;
      }
    }
  } else if (ev.pin_id == cs_pin && ev.event == GpioEvent::RISE) {
    incoming_bit_count = incoming_byte_count = incoming_byte = 0;
  } else if (ev.pin_id == beeper_pin) {
    if (ev.event == GpioEvent::RISE) {
      // play sound
    } else if (ev.event == GpioEvent::FALL) {
      // stop sound
    }
  } else if (ev.pin_id == kill_pin) {
    Gpio::set_pin_value(kill_pin,  !key_pressed[KeyName::KILL_BUTTON]);
  } else if (ev.pin_id == enc_but_pin) {
    Gpio::set_pin_value(enc_but_pin,  !key_pressed[KeyName::ENCODER_BUTTON]);
  } else if (ev.pin_id == back_pin) {
    Gpio::set_pin_value(back_pin,  !key_pressed[KeyName::BACK_BUTTON]);
  } else if (ev.pin_id == enc1_pin || ev.pin_id == enc2_pin) {
    const uint8_t encoder_state = encoder_position % 4;
    Gpio::set_pin_value(enc1_pin,  encoder_table[encoder_state] & 0x01);
    Gpio::set_pin_value(enc2_pin,  encoder_table[encoder_state] & 0x02);
  }
}

void ST7920Device::ui_init() {
  renderer::gl_assert_call(glGenTextures, 1, &texture_id);
  renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, texture_id);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, 0);
}

void ST7920Device::ui_widget() {
  static long int call_count = 0;
  static uint8_t up_held = 0, down_held = 0;
  call_count++;
  bool popout_begin = false;

  auto size = ImGui::GetContentRegionAvail();
  size.y = ((size.x / (width / (float)height)) * !render_popout) + 60;

  if (ImGui::BeginChild("ST7920Device", size)) {
    ImGui::GetCurrentWindow()->ScrollMax.y = 1.0f; // disable window scroll
    ImGui::Checkbox("Integer Scaling", &render_integer_scaling);
    ImGui::Checkbox("Popout", &render_popout);

    if (render_popout) {
      const imgui_custom::constraint_t constraint { width + imgui_custom::hfeat, height + imgui_custom::vfeat, (width) / (float)(height) };

      // Init the window size to contain the 2x scaled screen, margin, and window features
      ImGui::SetNextWindowSize(ImVec2(constraint.minw + width, constraint.minh + height), ImGuiCond_Once);
      ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), imgui_custom::CustomConstraints::AspectRatio, (void*)&constraint);

      popout_begin = ImGui::Begin("ST7920DeviceRender", &render_popout);
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
