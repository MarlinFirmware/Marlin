#pragma once
#include <imgui.h>
#include "../virtual_printer.h"

constexpr uint16_t cmap(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
constexpr uint16_t dutycycle_to_degrees(uint16_t value) {
  return cmap((float(value) / std::numeric_limits<uint16_t>::max()) * 20000, 540, 2400, 0, 180);
}
constexpr bool is_close_enough(uint16_t value, uint16_t target, uint16_t max_error = 1) {
  return (value >= target - max_error) && (value <= target + max_error);
}

class BLTouchProbe : public VirtualPrinter::Component {
public:
  BLTouchProbe(pin_type servo_pin, pin_type probe_pin, glm::vec3 offset, glm::vec4& position, PrintBed& bed) : VirtualPrinter::Component("BLTouch") {
    m_servo_pin = servo_pin;
    m_servo = add_component<PWMReader>("PWM Control Signal", servo_pin);
    m_probe = add_component<BedProbe>("Probe Signal", probe_pin, offset, position, bed);
    Gpio::attach(servo_pin, [this](GpioEvent& event){ this->interrupt(event); });
  }
  virtual ~BLTouchProbe() {}

  void ui_widget() {
    if (m_deployed) {
      ImGui::Text("Probe Deployed");
    } else {
      ImGui::Text("Probe Stowed");
    }
    ImGui::Text("Command Angle: %d", dutycycle_to_degrees(m_servo->pwm_duty));
  }

  void interrupt(GpioEvent& ev);

private:
  bool m_deployed = false;
  pin_type m_servo_pin;
  std::shared_ptr<PWMReader> m_servo;
  std::shared_ptr<BedProbe> m_probe;
};
