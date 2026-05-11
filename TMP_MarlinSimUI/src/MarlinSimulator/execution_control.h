#pragma once

#include <mutex>
#include <thread>
#include <functional>
#include <atomic>
#include <algorithm>
#include <cassert>
#include <map>
#include <sstream>
#include <deque>

constexpr inline uint64_t tickConvertFrequency(std::uint64_t value, std::uint64_t from, std::uint64_t to) {
  return from > to ? value / (from / to) : value * (to / from);
}

struct KernelTimer {
  KernelTimer(std::string name, void (*callback)(), uint64_t priority) : name(name), isr_function(callback), priority(priority) {}

  bool interrupt(const uint64_t source_count, const uint64_t frequency) {
    return source_count > next_interrupt(frequency);
  }

  uint64_t next_interrupt(const uint64_t source_frequency) {
    return active ? source_offset + tickConvertFrequency(compare, timer_frequency, source_frequency) : std::numeric_limits<uint64_t>::max();
  }

  void initialise(const uint64_t timer_frequency) { this->timer_frequency = timer_frequency; }
  void start(const uint64_t source_count, const uint64_t interrupt_frequency) {
    compare = timer_frequency / interrupt_frequency;
    source_offset = source_count;
  }
  void enable() { active = true; }
  bool enabled() { return active; }
  void disable() { active = false; }

  // in timer frequency
  void set_compare(const uint64_t compare) { this->compare = compare; }
  uint64_t get_compare() { return compare; }
  uint64_t get_count(const uint64_t source_count, const uint64_t source_frequency) { return tickConvertFrequency(source_count - source_offset, source_frequency, timer_frequency); }


  void set_isr(std::string name, void (*callback)()) {
    isr_function = {callback};
    this->name = name;
  }
  void execute() {
    running = true;
    isr_function();
    running = false;
  }

  std::string name;
  bool active = false;
  bool running = false;
  std::function<void()> isr_function;
  uint64_t compare = 0, source_offset = 0, timer_frequency = 0, priority = 10;
};

class Kernel {
public:
  class TimeControl {
  public:
    inline static void updateRealtime() {
      auto now = clock.now();
      auto delta = now - last_clock_read;
      uint64_t delta_uint64 = std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count();
      if(delta_uint64 > std::numeric_limits<std::uint64_t>::max() - ONE_BILLION) {
        //printf("rt info: %ld : %f\n", delta_uint64, realtime_scale.load());
        //aparently time can go backwards, thread issue?
        delta_uint64 = 0;
      }
      uint64_t delta_uint64_scaled = delta_uint64 * realtime_scale;
      if (delta_uint64_scaled != 0) {
        last_clock_read = now;
        realtime_nanos += delta_uint64_scaled;
      }
    }

    inline static void realtime_sync() {
      updateRealtime();
      if (getRealtimeTicks() > getTicks() || realtime_scale > 99.0f) {
        realtime_nanos = SimulationRuntime::nanos();
      } else while (getTicks() > getRealtimeTicks()) {
        if (quit_requested) throw (std::runtime_error("Quit Requested"));  // quit program when stuck at 0 speed
        updateRealtime();
        realtime_scale > 20.0f ?  std::this_thread::yield() : std::this_thread::sleep_for(std::chrono::nanoseconds(1));
      }
    }

    inline static uint64_t getRealtimeTicks() { return nanosToTicks(realtime_nanos); }

    inline static uint64_t getTicks() {
      return ticks.load();
    }

    inline static void setTicks(uint64_t new_ticks) {
      ticks.store(new_ticks);
    }

    inline static void addTicks(uint64_t delta_ticks) {
      ticks.store(ticks.load() + delta_ticks);
    }

    static uint64_t nanos();

    inline static uint64_t micros() {
      return nanos() / ONE_THOUSAND;
    }

    inline static uint64_t millis() {
      return nanos() / ONE_MILLION;
    }

    inline static double seconds() {
      return nanos() / double(ONE_BILLION);
    }

    constexpr static uint64_t nanosToTicks(const uint64_t value, const uint64_t freq) {
      return freq > ONE_BILLION ? value * (freq / ONE_BILLION) : value / (ONE_BILLION / freq);
    }

    inline static uint64_t nanosToTicks(uint64_t value) {
      return nanosToTicks(value, frequency);
    }

    constexpr static uint64_t ticksToNanos(const uint64_t value, const uint64_t freq) {
      return freq > ONE_BILLION ? value / (freq / ONE_BILLION) : value * (ONE_BILLION / freq);
    }

    inline static uint64_t ticksToNanos(uint64_t value) {
      return ticksToNanos(value, frequency);
    }

    static constexpr uint64_t ONE_BILLION  = 1000'000'000;
    static constexpr uint64_t ONE_MILLION  = 1000'000;
    static constexpr uint64_t ONE_THOUSAND = 1000;

    static std::chrono::steady_clock clock;
    static std::chrono::steady_clock::time_point last_clock_read;
    static std::atomic_uint64_t ticks;
    static uint64_t realtime_nanos;
    static std::atomic<float> realtime_scale;
    static constexpr uint64_t frequency = 100'000'000;
  };

  class SimulationRuntime {
  public:
    static inline uint64_t nanos() { return TimeControl::ticksToNanos(TimeControl::getTicks()); }
    static inline uint64_t micros() { return TimeControl::ticksToNanos(TimeControl::getTicks()) / TimeControl::ONE_THOUSAND; }
    static inline uint64_t millis() { return TimeControl::ticksToNanos(TimeControl::getTicks()) / TimeControl::ONE_MILLION; }
    static inline double seconds() { return TimeControl::ticksToNanos(TimeControl::getTicks()) / (double)TimeControl::ONE_BILLION; }
  };

  class Timers {
  public:
    inline static void timerInit(uint8_t timer_id, uint32_t rate) {
      if (timer_id < timers.size()) {
        timers[timer_id].timer_frequency = rate;
        // printf("Timer[%d] Initialised( rate: %d )\n", timer_id, rate);
      }
    }

    inline static void timerStart(uint8_t timer_id, uint32_t interrupt_frequency) {
      if (timer_id < timers.size()) {
        timers[timer_id].compare = timers[timer_id].timer_frequency / interrupt_frequency;
        timers[timer_id].source_offset = TimeControl::getTicks();
        // printf("Timer[%d] Started( frequency: %d compare: %ld)\n", timer_id, interrupt_frequency, timers[timer_id].compare);
      }
    }

    inline static void timerEnable(uint8_t timer_id) {
      if (timer_id < timers.size()) {
        timers[timer_id].active = true;
        // printf("Timer[%d] Enabled\n", timer_id);
      }
    }

    inline static bool timerEnabled(uint8_t timer_id) {
      if (timer_id < timers.size())
        return timers[timer_id].active;
      return false;
    }

    inline static void timerDisable(uint8_t timer_id) {
      if (timer_id < timers.size()) {
        timers[timer_id].active = false;
        //printf("Timer[%d] Disabled\n", timer_id);
      }
    }

    inline static void timerSetCompare(uint8_t timer_id, uint64_t compare) {
      if (timer_id < timers.size()) {
        timers[timer_id].compare = compare;
      }
    }

    inline static uint64_t timerGetCount(uint8_t timer_id) {
      if (timer_id < timers.size()) {
        //time must pass here for the stepper isr pulse counter (time + 100ns)
        TimeControl::addTicks(1 + TimeControl::nanosToTicks(100, timers[timer_id].timer_frequency));
        return timers[timer_id].get_count(TimeControl::getTicks(), TimeControl::frequency);
      }
      return 0;
    }

    inline static uint64_t timerGetCompare(uint8_t timer_id) {
      if (timer_id < timers.size())
        return timers[timer_id].compare;
      return 0;
    }
    static std::array<KernelTimer, 4> timers;
  };

  // To avoid issues with global initialization order, this should be called with a true value
  // to enable operation of execute_loop.
  static bool is_initialized(bool known_state = false);

  //execute highest priority thread with closest interrupt, return true if something was executed
  static bool execute_loop(uint64_t max_end_ticks = std::numeric_limits<uint64_t>::max());
  // if a thread wants to wait, see what should be executed during that wait
  static void delayCycles(uint64_t cycles);
  // this was neede for when marlin loops idle waiting for an event with no delays
  static void yield();

  static void shutdown() {
    quit_requested = true;
    yield();
  }

  static void execution_break() { debug_break_flag = true; }

  //Timers
  inline static void disableInterrupts() {
    timers_active = false;
  }

  inline static void enableInterrupts() {
    timers_active = true;
  }

  inline static void delayNanos(uint64_t ns) {
    delayCycles(TimeControl::nanosToTicks(ns));
  }

  inline static void delayMicros(uint64_t micros) {
    delayCycles(TimeControl::nanosToTicks(micros * TimeControl::ONE_THOUSAND));
  }

  inline static void delayMillis(uint64_t millis) {
    delayCycles(TimeControl::nanosToTicks(millis * TimeControl::ONE_MILLION));
  }

  inline static void delaySeconds(double secs) {
    delayCycles(TimeControl::nanosToTicks(secs * TimeControl::ONE_BILLION));
  }

  static bool timers_active;
  static std::deque<KernelTimer*> isr_stack;
  static bool quit_requested;
  static std::atomic_uint64_t isr_timing_error;
  static std::atomic_bool debug_break_flag;
};
