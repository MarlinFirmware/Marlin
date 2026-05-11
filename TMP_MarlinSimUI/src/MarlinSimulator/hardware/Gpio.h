#pragma once

#include <cstdint>
#include <atomic>
#include <functional>
#include <vector>
#include <deque>

#include "../execution_control.h"
#include "src/inc/MarlinConfigPre.h"


typedef int16_t pin_type;

struct GpioEvent {
  enum Type {
    NOP,
    FALL,
    RISE,
    SET_VALUE,
    SETM,
    SETD,
    GET_VALUE
  };
  uint64_t timestamp;
  pin_type pin_id;
  GpioEvent::Type event;

  GpioEvent(uint64_t timestamp, pin_type pin_id, GpioEvent::Type event) : timestamp(timestamp), pin_id(pin_id), event(event) { }
};

class IOLogger {
public:
  virtual ~IOLogger(){};
  virtual void log(GpioEvent ev) = 0;
};

struct pin_log_data {
  uint64_t timestamp;
  uint16_t value;
};

struct pin_data {
  enum Mode {
    GPIO,
    ADC,
    SPI,
    I2C,
    UART
  };
  enum Direction {
    INPUT,
    OUTPUT
  };
  enum Pull {
    NONE,
    PULLUP,
    PULLDOWN,
    TRISTATE
  };
  enum State {
    LOW,
    HIGH
  };
  template<class... Args>
  bool attach(Args... args) {
    callbacks.push_back(std::function<void(GpioEvent&)>(args...));
    return true;
  }
  std::atomic_uint8_t pull;
  std::atomic_uint8_t dir;
  std::atomic_uint8_t mode;
  std::atomic_uint16_t value;
  std::vector<std::function<void(GpioEvent&)>> callbacks;
  std::deque<pin_log_data> event_log;
};

class Gpio {
public:

  static const pin_type pin_count = 256;

  static void set_pin_value(const pin_type pin, const uint16_t value) {
    if (!valid_pin(pin)) return;
    if (value != pin_map[pin].value) { // Optimizes for size, but misses "meaningless" sets
      pin_map[pin].value = value;
      if (logging_enabled) {
        pin_map[pin].event_log.push_back(pin_log_data{Kernel::SimulationRuntime::nanos(), pin_map[pin].value});
        if (pin_map[pin].event_log.size() > 100000) pin_map[pin].event_log.pop_front();
      }
    }
  }

  static uint16_t get_pin_value(const pin_type pin) {
    if (!valid_pin(pin)) return 0;
    return pin_map[pin].value;
  }

  static inline constexpr bool valid_pin(const pin_type pin) {
    return pin >= 0 && pin <= pin_count;
  }

  static inline void set(const pin_type pin) {
    set(pin, 1);
  }

  static void set(const pin_type pin, const uint16_t value) {
    if (!valid_pin(pin)) return;
    if (value != pin_map[pin].value) { // Optimizes for size, but misses "meaningless" sets
      GpioEvent::Type evt_type = value > 1 ? GpioEvent::SET_VALUE : value > pin_map[pin].value ? GpioEvent::RISE : value < pin_map[pin].value ? GpioEvent::FALL : GpioEvent::NOP;
      pin_map[pin].value = value;
      GpioEvent evt(Kernel::TimeControl::getTicks(), pin, evt_type);
      if (logging_enabled) {
        pin_map[pin].event_log.push_back(pin_log_data{Kernel::SimulationRuntime::nanos(), pin_map[pin].value});
        if (pin_map[pin].event_log.size() > 100000) pin_map[pin].event_log.pop_front();
      }
      for (auto callback : pin_map[pin].callbacks) callback(evt);
    }
  }

  static uint16_t get(const pin_type pin) {
    if (!valid_pin(pin)) return 0;
    GpioEvent evt(Kernel::TimeControl::getTicks(), pin, GpioEvent::GET_VALUE);
    for (auto callback : pin_map[pin].callbacks) callback(evt);
    return pin_map[pin].value;
  }

  static inline void clear(const pin_type pin) {
    set(pin, 0);
  }

  static void setMode(const pin_type pin, const uint8_t value) {
    if (!valid_pin(pin)) return;
    pin_map[pin].mode = pin_data::Mode::GPIO;

    GpioEvent evt(Kernel::TimeControl::getTicks(), pin, GpioEvent::Type::SETM);

    if (value != 1) setDir(pin, pin_data::Direction::INPUT);
    else setDir(pin, pin_data::Direction::OUTPUT);

    pin_map[pin].pull = value == 2 ? pin_data::Pull::PULLUP : value == 3 ? pin_data::Pull::PULLDOWN : pin_data::Pull::NONE;
    if (pin_map[pin].pull == pin_data::Pull::PULLUP) set(pin, pin_data::State::HIGH);

  }

  static inline uint8_t getMode(const pin_type pin) {
    if (!valid_pin(pin)) return 0;
    return pin_map[pin].mode;
  }

  static void setDir(const pin_type pin, const uint8_t value) {
    if (!valid_pin(pin)) return;
    pin_map[pin].dir = value;
    GpioEvent evt(Kernel::TimeControl::getTicks(), pin, GpioEvent::Type::SETD);
    for (auto callback : pin_map[pin].callbacks) callback(evt);
  }

  static inline uint8_t getDir(const pin_type pin) {
    if (!valid_pin(pin)) return 0;
    return pin_map[pin].dir;
  }

  static void write(const pin_type pin, const uint16_t value) {
    if (!valid_pin(pin)) return;
    pin_map[pin].value = value;
    GpioEvent evt(Kernel::TimeControl::getTicks(), pin, GpioEvent::SET_VALUE);
    for (auto callback : pin_map[pin].callbacks) callback(evt);
  }

  static uint16_t read(const pin_type pin) {
    if (!valid_pin(pin)) return 0;
    GpioEvent evt(Kernel::TimeControl::getTicks(), pin, GpioEvent::GET_VALUE);
    for (auto callback : pin_map[pin].callbacks) callback(evt);
    return pin_map[pin].value;
  }

  template<class... Args>
  static bool attach(const pin_type pin, Args... args) {
    if (!valid_pin(pin)) return false;
    return pin_map[pin].attach(args...);
  }

  static void resetLogs() {
    for (auto &pin : pin_map) {
      // Seed each pin with an initial value to ensure important edges are not the first sample.
      pin.event_log.clear();
      pin.event_log.push_back(pin_log_data{Kernel::SimulationRuntime::nanos(), pin.value});
    }
  }

  static void setLoggingEnabled(bool enable) {
    if (!logging_enabled && enable) {
      resetLogs();
    }
    logging_enabled = enable;
  }

  static bool isLoggingEnabled() {
    return logging_enabled;
  }

  static pin_data pin_map[pin_count];

private:
  static bool logging_enabled;
};
