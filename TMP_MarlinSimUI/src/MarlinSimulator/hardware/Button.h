#pragma once

#include <atomic>

#include <imgui.h>

#include "Gpio.h"
#include "../virtual_printer.h"

class Button : public VirtualPrinter::Component {
public:
  Button(pin_type pin, bool invert_logic) : VirtualPrinter::Component("Button"), pin(pin), invert_logic(invert_logic) {
    Gpio::attach(pin, [this](GpioEvent& ev){ this->interrupt(ev); });
  }
  ~Button() {}

  void ui_widget() {
    ImGui::Button("State");
    if(ImGui::IsItemActive()) {
      active = invert_logic;
    } else {
      active = !invert_logic;
    }
  }

  void interrupt(GpioEvent& ev) {
    if (ev.pin_id == pin && ev.event == ev.GET_VALUE) {
      Gpio::set_pin_value(ev.pin_id, active);
    }
  }

private:
  const pin_type pin;
  std::atomic_bool active = true;
  bool invert_logic;
};
