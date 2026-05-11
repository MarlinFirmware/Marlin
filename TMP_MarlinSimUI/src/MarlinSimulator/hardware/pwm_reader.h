#pragma once

#pragma once

#include <cmath>
#include "Gpio.h"
#include "../virtual_printer.h"

class PWMReader: public VirtualPrinter::Component {
public:
  enum PwmMode {
    Inactive,
    Hardware,
    Software
  };

  PWMReader(pin_type pwm_pin);
  virtual ~PWMReader();

  void interrupt(GpioEvent& ev);
  void update();
  void ui_widget();

  pin_type pwm_pin;
  uint64_t pwm_period = 0;
  uint64_t pwm_duty = 0;
  uint64_t pwm_hightick = 0;
  uint64_t pwm_lowtick = 0;
  uint64_t pwm_last_update = 0;
  PwmMode pwm_mode = Inactive;
};
