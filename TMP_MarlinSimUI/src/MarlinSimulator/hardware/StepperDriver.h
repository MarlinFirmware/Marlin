#pragma once

#include <atomic>

#include <imgui.h>

#include "Gpio.h"
#include "../virtual_printer.h"

class StepperDriver : public VirtualPrinter::Component {
public:
  StepperDriver(pin_type enable, pin_type dir, pin_type step, std::function<void()> step_callback = [](){} ) : VirtualPrinter::Component("StepperDriver"), enable(enable), dir(dir), step(step), step_callback(step_callback) {
    Gpio::attach(step, [this](GpioEvent& ev){ this->interrupt(ev); });
  }
  ~StepperDriver() {}

  void ui_widget() {
    ImGui::Text("Steps: %ld", step_count.load());
  }

  void interrupt(GpioEvent& ev) {
    if (ev.pin_id == step && ev.event == ev.RISE && Gpio::get_pin_value(enable) == 0) {
      step_count += ((Gpio::get_pin_value(dir) * 2) - 1);
      step_callback();
    }
  }

  int64_t steps() {
    return step_count;
  }

  std::atomic_int64_t step_count = 0;
  const pin_type enable, dir, step;
  std::function<void()> step_callback;
};
