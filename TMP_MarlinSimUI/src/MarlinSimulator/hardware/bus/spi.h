#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "../Gpio.h"

struct SpiEvent {
  uint8_t *write_from, *read_into;
  size_t length;
  bool source_increment = true;
  uint8_t source_format = 1;
};

class SpiBus {
public:
  SpiBus() {}
  ~SpiBus() = default;
  SpiBus(const SpiBus&) = delete;

  void write(uint8_t value) {
    auto evt = SpiEvent{&value, nullptr, 1};
    for (auto callback : callbacks) callback(evt);
  }

  uint8_t read() {
    uint8_t value;
    auto evt = SpiEvent{nullptr, &value, 1};
    for (auto callback : callbacks) callback(evt);
    return value;
  }

  uint8_t transfer(uint8_t write_value) {
    uint8_t read_value = 0xFF;
    auto evt = SpiEvent{&write_value, &read_value, 1};
    for (auto callback : callbacks) callback(evt);
    return read_value;
  }

  template<typename DataType>
  void transfer(DataType* write_from, DataType* read_into, size_t length, bool source_increment = true) {
    uint8_t* data = nullptr;

    // uint16_t Endian swap hack
    if (sizeof(DataType) == 2) {
      if (source_increment) {
        data = new uint8_t[sizeof(DataType) * length];
        for (size_t i = 0; i < length; i ++) {
          data[(i * 2)] = (write_from[i] >> 8) & 0xFF;
          data[(i * 2) + 1] = (write_from[i] & 0xFF);
        }
      } else {
        data = new uint8_t[sizeof(DataType)];
        data[0] = (write_from[0] >> 8) & 0xFF;
        data[1] = (write_from[0] & 0xFF);
      }
    } else {
      data = (uint8_t*)write_from;
    }

    auto evt = SpiEvent{data, (uint8_t*)read_into, sizeof(DataType) * length, source_increment, sizeof(DataType)};
    for (auto callback : callbacks) callback(evt);

    if ((void*)data != (void*)write_from) delete[] data;
  }

  template<class... Args>
  void attach(Args... args) {
    callbacks.push_back(std::function<void(SpiEvent&)>(args...));
  }

  void acquire() { if(busy == true) printf("spi bus contention!\n"); busy = true; }
  void release() { busy = false; }
  bool is_busy() { return busy; }

private:
  std::vector<std::function<void(SpiEvent&)>> callbacks;
  bool busy = false;
};

extern SpiBus SpiBus0;
extern SpiBus SpiBus1;
extern SpiBus SpiBus2;
extern SpiBus SpiBus3;

template <pin_type CLK, pin_type MOSI, pin_type MISO>
SpiBus& spi_bus_by_pins();
