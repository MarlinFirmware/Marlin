#ifndef _HARDWARE_CLOCK_H_
#define _HARDWARE_CLOCK_H_

#include <chrono>
#include <thread>

class Clock {
public:
  static uint64_t ticks(uint32_t frequency = Clock::frequency) {
    return (Clock::nanos() - Clock::startup.count()) / (1000000000ULL / frequency);
  }

  static void setFrequency(uint32_t freq) {
    Clock::frequency = freq;
  }

  static uint64_t nanos() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return now.count() - Clock::startup.count();
  }

  static uint64_t micros() {
    return Clock::nanos() / 1000;
  }

  static uint64_t millis() {
    return Clock::micros() / 1000;
  }

  static double seconds() {
    return Clock::nanos() / 1000000000.0;
  }

  static void delayCycles(uint64_t cycles) {
    std::this_thread::sleep_for(std::chrono::nanoseconds( (1000000000L / frequency) * cycles) );
  }

  static void delayMicros(uint64_t micros) {
    std::this_thread::sleep_for(std::chrono::microseconds( micros ));
  }

  static void delayMillis(uint64_t millis) {
    std::this_thread::sleep_for(std::chrono::milliseconds( millis ));
  }

  static void delaySeconds(double secs) {
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(secs * 1000));
  }

private:
  static std::chrono::nanoseconds startup;
  static uint32_t frequency;
};

#endif /* _HARDWARE_CLOCK_H_ */
