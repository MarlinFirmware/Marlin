#include <mutex>
#include <fstream>
#include <cmath>
#include <random>
#include "Gpio.h"

#include <gl.h>
#include "../renderer/renderer.h"

#include "../imgui_custom.h"

#include "ST7796Device.h"

#include "../paths.h"
#include MARLIN_HAL_PATH(tft/xpt2046.h)
#include MARLIN_HAL_PATH(tft/tft_spi.h)

#define ST7796S_CASET      0x2A // Column Address Set
#define ST7796S_RASET      0x2B // Row Address Set
#define ST7796S_RAMWR      0x2C // Memory Write

ST7796Device::ST7796Device(SpiBus& spi_bus, pin_type tft_cs, SpiBus& touch_spi_bus, pin_type touch_cs, pin_type dc, pin_type beeper, pin_type enc1, pin_type enc2, pin_type enc_but, pin_type back, pin_type kill)
  : SPISlavePeripheral(spi_bus, tft_cs), dc_pin(dc), beeper_pin(beeper), enc1_pin(enc1), enc2_pin(enc2), enc_but_pin(enc_but), back_pin(back), kill_pin(kill) {
  touch = add_component<XPT2046Device>("Touch", touch_spi_bus, touch_cs);
  Gpio::attach(dc_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(beeper_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(kill_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc_but_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(back_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc1_pin, [this](GpioEvent& event){ this->interrupt(event); });
  Gpio::attach(enc2_pin, [this](GpioEvent& event){ this->interrupt(event); });
}

ST7796Device::~ST7796Device() {}

const uint32_t id_code = 0x00CB3B00;
void ST7796Device::process_command(Command cmd) {
  if (cmd.cmd == ST7796S_CASET) {
    xMin = (cmd.data[0] << 8) + cmd.data[1];
    xMax = (cmd.data[2] << 8) + cmd.data[3];
    if (xMin >= width) xMin = width - 1;
    if (xMax >= width) xMax = width - 1;
    graphic_ram_index_x = xMin;
  }
  else if (cmd.cmd == ST7796S_RASET) {
    yMin = (cmd.data[0] << 8) + cmd.data[1];
    yMax = (cmd.data[2] << 8) + cmd.data[3];
    if (yMin >= height) yMin = height - 1;
    if (yMax >= height) yMax = height - 1;
    graphic_ram_index_y = yMin;
  }
  else if (cmd.cmd == LCD_READ_ID) {
    setResponse((uint8_t*)&id_code, 4);
  }
}

void ST7796Device::update() {
  auto now = clock.now();
  float delta = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_update).count();

  if (dirty && delta > 1.0 / 30.0) {
    last_update = now;
    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, texture_id);
    renderer::gl_assert_call(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, graphic_ram);
    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, 0);
  }
}

void ST7796Device::interrupt(GpioEvent& ev) {
  if (ev.pin_id == beeper_pin) {
    if (ev.event == GpioEvent::RISE) {
      // play sound
    } else if (ev.event == GpioEvent::FALL) {
      // stop sound
    }
  } else if (ev.pin_id == dc_pin && ev.event == GpioEvent::FALL) {
    //start new command, execute last one
    process_command({command, data});
    data.clear();
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

void ST7796Device::onEndTransaction() {
  SPISlavePeripheral::onEndTransaction();
  //end of transaction, execute pending command
  process_command({command, data});
  data.clear();
};

void ST7796Device::onByteReceived(uint8_t _byte) {
  SPISlavePeripheral::onByteReceived(_byte);
  if (Gpio::get_pin_value(dc_pin)) {
    data.push_back(_byte);
    //direct write to memory, to optimize
    if (command == ST7796S_RAMWR && data.size() == 2) {
      auto pixel = (data[0] << 8) + data[1];
      graphic_ram[graphic_ram_index_x + (graphic_ram_index_y * width)] = pixel;
      if (graphic_ram_index_x >= xMax) {
        graphic_ram_index_x = xMin;
        graphic_ram_index_y++;
      }
      else {
        graphic_ram_index_x++;
      }
      if (graphic_ram_index_y >= yMax && graphic_ram_index_x >= xMax) {
        dirty = true;
      }
      if (graphic_ram_index_y >= height) graphic_ram_index_y = yMin;
      data.clear();
    }
  }
  else {
    //command
    command = _byte;
  }
}

void ST7796Device::ui_init() {
  renderer::gl_assert_call(glGenTextures, 1, &texture_id);
  renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, texture_id);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, 0);
}

void ST7796Device::ui_widget() {
  bool popout_begin = false;
  static uint8_t up_held = 0, down_held = 0;
  auto size = ImGui::GetContentRegionAvail();
  size.y = ((size.x / (width / (float)height)) * !render_popout) + 60;

  if (ImGui::BeginChild("ST7796Device", size)) {
    ImGui::GetCurrentWindow()->ScrollMax.y = 1.0f; // disable window scroll
    ImGui::Checkbox("Integer Scaling", &render_integer_scaling);
    ImGui::Checkbox("Popout", &render_popout);

    if (render_popout) {
      const imgui_custom::constraint_t constraint { width + imgui_custom::hfeat, height + imgui_custom::vfeat, (width) / (float)(height) };

      // Init the window size to contain the 1x scaled screen, margin, and window features
      ImGui::SetNextWindowSize(ImVec2(constraint.minw, constraint.minh), ImGuiCond_Once);
      ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), imgui_custom::CustomConstraints::AspectRatio, (void*)&constraint);

      popout_begin = ImGui::Begin("ST7796DeviceRender", &render_popout);
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
        encoder_position += ImGui::GetIO().MouseWheel > 0 ? 1 : ImGui::GetIO().MouseWheel < 0 ? -1 : 0;
      }
    }
    touch->ui_callback();

    if (popout_begin) ImGui::End();
  }
  ImGui::EndChild();
}
