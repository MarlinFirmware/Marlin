#pragma once

#include "Gpio.h"
#include "../user_interface.h"
#include "../virtual_printer.h"

class FilamentRunoutSensor : public VirtualPrinter::Component {
public:
  FilamentRunoutSensor(pin_type runout_pin, bool runtout_trigger_value) : VirtualPrinter::Component("FilamentRunoutSensor"), runout_pin(runout_pin), runtout_trigger_value(runtout_trigger_value) {
    Gpio::attach(runout_pin, [this](GpioEvent& event){ this->interrupt(event); });
  }

  void interrupt(GpioEvent &ev) {
    if (ev.pin_id == runout_pin && ev.event == GpioEvent::GET_VALUE) {
      Gpio::set_pin_value(runout_pin, filament_present ? !runtout_trigger_value : runtout_trigger_value);
    }
  }

  void ui_widget() {
    bool value_check = filament_present.load();
    ImGui::Checkbox("Filament Present ", &value_check);
    filament_present.store(value_check);
  }

private:
  pin_type runout_pin;
  bool runtout_trigger_value;
  std::atomic_bool filament_present = true;
};
