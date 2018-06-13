#ifndef _HARDWARE_GPIO_H_
#define _HARDWARE_GPIO_H_

#include "Clock.h"
#include "../../../inc/MarlinConfigPre.h"
#include <stdint.h>

typedef int16_t pin_type;

struct GpioEvent {
  enum Type {
    NOP,
    FALL,
    RISE,
    SET_VALUE,
    SETM,
    SETD
  };
  uint64_t timestamp;
  pin_type pin_id;
  GpioEvent::Type event;

  GpioEvent(uint64_t timestamp, pin_type pin_id, GpioEvent::Type event){
    this->timestamp = timestamp;
    this->pin_id = pin_id;
    this->event = event;
  }
};

class IOLogger {
public:
  virtual ~IOLogger(){};
  virtual void log(GpioEvent ev) = 0;
};

class Peripheral {
public:
  virtual ~Peripheral(){};
  virtual void interrupt(GpioEvent ev) = 0;
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
    GpioEvent::Type evt_type = value > 1 ? GpioEvent::SET_VALUE : value > pin_map[pin].value ? GpioEvent::RISE : value < pin_map[pin].value ? GpioEvent::FALL : GpioEvent::NOP;
    pin_map[pin].value = value;
    GpioEvent evt(Clock::nanos(), pin, evt_type);
    if(pin_map[pin].cb != nullptr) {
      pin_map[pin].cb->interrupt(evt);
    }
    if(Gpio::logger != nullptr) Gpio::logger->log(evt);
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
    GpioEvent evt(Clock::nanos(), pin, GpioEvent::Type::SETM);
    if(pin_map[pin].cb != nullptr) pin_map[pin].cb->interrupt(evt);
    if(Gpio::logger != nullptr) Gpio::logger->log(evt);
  }

  static uint8_t getMode(pin_type pin) {
    if(!valid_pin(pin)) return 0;
    return pin_map[pin].mode;
  }

  static void setDir(pin_type pin, uint8_t value) {
    if(!valid_pin(pin)) return;
    pin_map[pin].dir = value;
    GpioEvent evt(Clock::nanos(), pin, GpioEvent::Type::SETD);
    if(pin_map[pin].cb != nullptr) pin_map[pin].cb->interrupt(evt);
    if(Gpio::logger != nullptr) Gpio::logger->log(evt);
  }

  static uint8_t getDir(pin_type pin) {
    if(!valid_pin(pin)) return 0;
    return pin_map[pin].dir;
  }

  static void attachPeripheral(pin_type pin, Peripheral* per) {
    if(!valid_pin(pin)) return;
    pin_map[pin].cb = per;
  }

  static void attachLogger(IOLogger* logger) {
    Gpio::logger = logger;
  }

private:
  static IOLogger* logger;
};

#endif /* _HARDWARE_GPIO_H_ */
