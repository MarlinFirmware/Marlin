#pragma once

#include "../virtual_printer.h"
#include "Gpio.h"
#include "print_bed.h"

class BedProbe : public VirtualPrinter::Component {
public:
  BedProbe(pin_type probe, glm::vec3 offset, glm::vec4& position, PrintBed& bed) : VirtualPrinter::Component("BedProbe"), probe_pin(probe), offset(offset), position(position), bed(bed) {
    Gpio::attach(probe, [this](GpioEvent& event){ this->interrupt(event); });
  }

  void interrupt(GpioEvent& event) {
    Gpio::set_pin_value(event.pin_id, triggered());
  }

  void ui_widget() {
    auto has_triggered = triggered();
    if (enabled) {
      ImGui::Checkbox("Triggered State", &has_triggered);
      ImGui::Text("Nozzle Distance Above Bed: %f", position.z - bed.calculate_z({position.x + offset.x, position.y + offset.y}));
    } else {
      ImGui::Text("Disabled");
    }
  }

  bool triggered() {
    return enabled && position.z <= bed.calculate_z({position.x + offset.x, position.y + offset.y}) - offset.z;
  }

  bool enabled = true;
  pin_type probe_pin;
  glm::vec3 offset;
  glm::vec4& position;
  PrintBed& bed;
};
