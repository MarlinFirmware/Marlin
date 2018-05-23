#ifndef _HARDWARE_GPIO_H_
#define _HARDWARE_GPIO_H_

#include "../../../inc/MarlinConfigPre.h"
#include <stdint.h>

typedef int16_t pin_type;

enum class GpioEvent{
  NOP,
  FALL,
  RISE,
  SET_VALUE,
  SETM,
  SETD
};

class Peripheral {
public:
  virtual ~Peripheral(){};
  virtual void interrupt(GpioEvent ev, pin_type pin) = 0;
  virtual void update() = 0;
};

struct pin_data {
  uint8_t dir;
  uint8_t mode;
  uint16_t value;
  Peripheral* cb;
};

class Gpio {
public:

  static const pin_type pin_count = 255;
  static pin_data pin_map[pin_count+1];

  static bool valid_pin(pin_type pin) {
    return pin >= 0 && pin <= pin_count;
  }

  static void set(pin_type pin) {
    set(pin, 1);
  }

  static void set(pin_type pin, uint16_t value) {
    if(!valid_pin(pin)) return;
    GpioEvent evt = value > 1 ? GpioEvent::SET_VALUE : value > pin_map[pin].value ? GpioEvent::RISE : value < pin_map[pin].value ? GpioEvent::FALL : GpioEvent::NOP;
    pin_map[pin].value = value;
    if(pin_map[pin].cb != nullptr) {
      pin_map[pin].cb->interrupt(evt, pin);
    }
  }

  static uint16_t get(pin_type pin) {
    if(!valid_pin(pin)) return 0;
    return pin_map[pin].value;
  }

  static void clear(pin_type pin) {
    set(pin, 0);
  }

  static void setMode(pin_type pin, uint8_t value) {
    if(!valid_pin(pin)) return;
    pin_map[pin].mode = value;
    if(pin_map[pin].cb != nullptr) pin_map[pin].cb->interrupt(GpioEvent::SETM, pin);
  }

  static uint8_t getMode(pin_type pin) {
    if(!valid_pin(pin)) return 0;
    return pin_map[pin].mode;
  }

  static void setDir(pin_type pin, uint8_t value) {
    if(!valid_pin(pin)) return;
    pin_map[pin].dir = value;
    if(pin_map[pin].cb != nullptr) pin_map[pin].cb->interrupt(GpioEvent::SETD, pin);
  }

  static uint8_t getDir(pin_type pin) {
    if(!valid_pin(pin)) return 0;
    return pin_map[pin].dir;
  }

  static void attachPeripheral(pin_type pin, Peripheral* per) {
    if(!valid_pin(pin)) return;
    pin_map[pin].cb = per;
  }

};

#endif /* _HARDWARE_GPIO_H_ */
