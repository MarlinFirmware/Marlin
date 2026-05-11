#include <imgui.h>
#include "pinmapping.h"
#include "pwm_reader.h"

PWMReader::PWMReader(pin_type pwm_pin) : VirtualPrinter::Component("PWMReader"), pwm_pin(pwm_pin) {
  Gpio::attach(this->pwm_pin, [this](GpioEvent& event){ this->interrupt(event); });
}

PWMReader::~PWMReader() {
}

void PWMReader::update() {

}

void PWMReader::ui_widget() {
  if (pwm_mode == Software && Kernel::TimeControl::getTicks() - pwm_last_update > ((pwm_period * 10) % Kernel::TimeControl::nanosToTicks(Kernel::TimeControl::ONE_BILLION))) {
    pwm_mode = Inactive;
    pwm_duty = 0;
    pwm_period = 0;
    pwm_hightick = 0;
  }
  switch (pwm_mode) {
    case Inactive:
      ImGui::Text("Inactive (%s) Duty Cycle: %.2f%%",  Gpio::get_pin_value(pwm_pin) ? "High" : "Low", Gpio::get_pin_value(pwm_pin) * 100.0f);
      break;
    case Software:
      ImGui::Text("Software PWM (%.2fHz) Duty Cycle: %.2f%%", pwm_period ? float(Kernel::TimeControl::ONE_BILLION) / Kernel::TimeControl::ticksToNanos(pwm_period) : 0, pwm_period ? float(pwm_duty * 100) / pwm_period : 0);
      break;
    case Hardware:
      ImGui::Text("Hardware PWM Duty Cycle : (%ld) %.2f%%", pwm_duty, pwm_period ? float(pwm_duty * 100) / pwm_period : 0);
      break;
  }
}

void PWMReader::interrupt(GpioEvent& ev) {
  if (ev.pin_id == pwm_pin) {
    pwm_last_update = ev.timestamp;
    if (ev.event == ev.RISE) {
      if (pwm_hightick) {
        pwm_period = ev.timestamp - pwm_hightick;
        pwm_mode = Software;
      }
      pwm_hightick = ev.timestamp;
    } else if ( ev.event == ev.FALL) {
      pwm_lowtick = ev.timestamp;
      pwm_duty = ev.timestamp - pwm_hightick;
    } else if ( ev.event == ev.SET_VALUE) { // Hardware PWM
      pwm_mode = Hardware;
      pwm_duty = Gpio::get_pin_value(pwm_pin);
      pwm_period = std::numeric_limits<uint16_t>::max();
    }
  }
}
