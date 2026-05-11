#include <imgui.h>

#include "pinmapping.h"
#include "NeoPixelDevice.h"

NeoPixelDevice::NeoPixelDevice(pin_type neopixel_pin, const uint8_t led_type, const uint16_t led_count) : VirtualPrinter::Component("NeoPixel"), neopixel_pin(neopixel_pin), led_type(led_type), led_count(led_count) {
  bits_per_word = led_type == NEO_GRBW ? 32 : 24;
  Gpio::attach(this->neopixel_pin, [this](GpioEvent& event){ this->interrupt(event); });
}

NeoPixelDevice::~NeoPixelDevice() {
}

void NeoPixelDevice::update() {
  if (Kernel::SimulationRuntime::nanos() - last_clock > 50000 && state != SignalState::INACTIVE) {
    if (bit_count == bits_per_word - 1) {
      if (state == SignalState::LOW_ZERO) {
          ++ bit_count;
          data_word <<= 1;
      } else if (state == SignalState::LOW_ONE) {
          ++ bit_count;
          data_word <<= 1;
          ++ data_word;
      }
      //printf("0x%08X\n", data_word);
      data_word <<= (32 - bit_count);
      update_led(data_word);
    } else {
      //printf("!TIMEOUT INACTIVE! (%d bits, 0x%08X)\n", bit_count, data_word);
    }
    state = SignalState::INACTIVE;
    bit_count = 0;
    data_word = 0;
  }
}

void NeoPixelDevice::ui_widget() {
  for (auto led : leds_display) {
    ImGui::Text("|"); ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, led);
    ImGui::Text("LED");
    ImGui::PopStyleColor();
    ImGui::SameLine(); ImGui::Text("|");
  }
}

// models energy transfer but not time lag as it tranfers through the medium.
void NeoPixelDevice::interrupt(GpioEvent& ev) {
  if (ev.pin_id == neopixel_pin && (ev.event == ev.RISE || ev.event == ev.FALL)) {
    auto timestamp = Kernel::TimeControl::ticksToNanos(ev.timestamp);
    auto data = (uint32_t)(timestamp - last_clock);
    last_clock = timestamp;

    switch (state) {
      case SignalState::INACTIVE:
        data_word = 0;
        bit_count = 0;
        if (ev.event == ev.RISE) { state = SignalState::HIGH; }
        break;
      case SignalState::HIGH:
        if (ev.event != ev.FALL) { state = SignalState::INACTIVE;}
        else if (data > 200 && data < 500) { state = SignalState::LOW_ZERO;}
        else if (data > 550 && data < 5500) { state = SignalState::LOW_ONE;}
        else { state = SignalState::INACTIVE;}
        break;
      case SignalState::LOW_ZERO:
        ++ bit_count;
        data_word <<= 1;
        if (data > 450 && data < 5000 && ev.event == ev.RISE) {
          state = SignalState::HIGH;
        } else { state = SignalState::INACTIVE; }
        break;
      case SignalState::LOW_ONE:
        ++ bit_count;
        data_word <<= 1;
        ++ data_word;
        if (data > 450 && data < 5000 && ev.event == ev.RISE) {
          state = SignalState::HIGH;
        } else { state = SignalState::INACTIVE; }
        break;
    }

    if (bit_count == bits_per_word) {
      data_word <<= (32 - bit_count);
      bit_count = 0;
      update_led(data_word);
      data_word = 0;
    }
  }
}

void NeoPixelDevice::update_led(uint32_t colour) {
  uint8_t idx = 3 - ((led_type & 0x30) >> 4);
  float red   =  (((0xFF << (idx * 8)) & colour) >> (idx * 8)) / 255.0;
  idx = 3 - ((led_type & 0x0C) >> 2);
  float green =  (((0xFF << (idx * 8)) & colour) >> (idx * 8)) / 255.0;
  idx = 3 - (led_type & 0x03);
  float blue  =  (((0xFF << (idx * 8)) & colour) >> (idx * 8)) / 255.0;
  leds_display.push_back(ImVec4(red, green, blue, 1.0f));
  if (leds_display.size() > led_count) leds_display.pop_front();
}
