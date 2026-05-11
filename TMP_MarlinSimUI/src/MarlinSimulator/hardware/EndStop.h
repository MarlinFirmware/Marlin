#pragma once

#include <atomic>

#include <imgui.h>

#include "Gpio.h"
#include "../virtual_printer.h"

class EndStop : public VirtualPrinter::Component {
public:
  EndStop(pin_type endstop, bool invert_logic, std::function<bool()> triggered) : VirtualPrinter::Component("EndStop"), endstop(endstop), invert_logic(invert_logic), triggered(triggered) {
    Gpio::attach(endstop, [this](GpioEvent& ev){ this->interrupt(ev); });
  }
  ~EndStop() {}

  void ui_widget() {
    bool value = (invert_logic ? !triggered() : triggered()) && enabled;
    ImGui::Checkbox("Triggered", &value);
    value = enabled;
    ImGui::Checkbox("Enabled", &value);
    enabled = value;
  }

  void interrupt(GpioEvent& ev) {
    if (ev.pin_id == endstop && ev.event == ev.GET_VALUE) {
      Gpio::set_pin_value(ev.pin_id, (invert_logic ? !triggered() : triggered()) && enabled);
    }
  }

private:
  const pin_type endstop;
  std::atomic_bool enabled = true;
  bool invert_logic;
  std::function<bool()> triggered;
};