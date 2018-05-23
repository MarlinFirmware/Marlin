#ifndef _HARDWARE_LINEARAXIS_H_
#define _HARDWARE_LINEARAXIS_H_

#include "Gpio.h"

class LinearAxis: public Peripheral {
public:
  LinearAxis(pin_type enable, pin_type dir, pin_type step, pin_type end_min, pin_type end_max);
  virtual ~LinearAxis();
  void update();
  void interrupt(GpioEvent ev, pin_type pin);

  pin_type enable_pin;
  pin_type dir_pin;
  pin_type step_pin;
  pin_type min_pin;
  pin_type max_pin;

  int32_t position;
  int32_t min_position;
  int32_t max_position;

};

#endif // _HARDWARE_LINEARAXIS_H_
