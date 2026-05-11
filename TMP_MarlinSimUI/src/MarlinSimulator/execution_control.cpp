#include <limits>

#include <debugbreak.h>

#include "user_interface.h"
#include "execution_control.h"
#include "serial.h"
#include "RawSocketSerial.h"

extern RawSocketSerial net_serial;
extern MSerialT serial_stream_0;
extern MSerialT serial_stream_1;
extern MSerialT serial_stream_2;
extern MSerialT serial_stream_3;

std::chrono::steady_clock Kernel::TimeControl::clock;
std::chrono::steady_clock::time_point Kernel::TimeControl::last_clock_read(Kernel::TimeControl::clock.now());
std::atomic_uint64_t Kernel::TimeControl::ticks{0};
uint64_t Kernel::TimeControl::realtime_nanos = 0;
std::atomic<float> Kernel::TimeControl::realtime_scale = 1.0;
std::atomic_bool Kernel::debug_break_flag = false;

extern void marlin_loop();
extern "C" void TIMER0_IRQHandler();
extern "C" void TIMER1_IRQHandler();
extern void SYSTICK_IRQHandler();
std::array<KernelTimer, 4> Kernel::Timers::timers({KernelTimer{"Stepper ISR", TIMER0_IRQHandler, 1}, {"Temperature ISR", TIMER1_IRQHandler, 10}, {"SysTick", SYSTICK_IRQHandler, 5}, {"Marlin Loop", marlin_loop, 100}});

bool Kernel::timers_active = true;
std::deque<KernelTimer*> Kernel::isr_stack;
bool Kernel::quit_requested = false;
std::atomic_uint64_t Kernel::isr_timing_error = 0;

bool Kernel::is_initialized(bool known_state) {
  static bool is_running = known_state;
  is_running = is_running || known_state;
  return is_running;
}

bool Kernel::execute_loop( uint64_t max_end_ticks) {
  // Marlin often gets into reentrant loops, this is the only way to unroll out of that call stack early
  if (quit_requested) throw (std::runtime_error("Quit Requested"));
  if (debug_break_flag) { debug_break_flag = false; debug_break(); }

  //simulation time lock
  TimeControl::realtime_sync();

  static auto terminal_0 = UserInterface::getElement<SerialMonitor>("Serial Monitor(0)");
  if (terminal_0) {
    serial_stream_0.transmit_buffer.read(terminal_0->serial_buffer.in);
    terminal_0->serial_buffer.out.read(serial_stream_0.receive_buffer);
  }

  static auto terminal_1 = UserInterface::getElement<SerialMonitor>("Serial Monitor(1)");
  if (terminal_1) {
    serial_stream_1.transmit_buffer.read(terminal_1->serial_buffer.in);
    terminal_1->serial_buffer.out.read(serial_stream_1.receive_buffer);
  }

  static auto terminal_2 = UserInterface::getElement<SerialMonitor>("Serial Monitor(2)");
  if (terminal_2) {
    serial_stream_2.transmit_buffer.read(terminal_2->serial_buffer.in);
    terminal_2->serial_buffer.out.read(serial_stream_2.receive_buffer);
  }

  static auto terminal_3 = UserInterface::getElement<SerialMonitor>("Serial Monitor(3)");
  if (terminal_3) {
    serial_stream_3.transmit_buffer.read(terminal_3->serial_buffer.in);
    terminal_3->serial_buffer.out.read(serial_stream_3.receive_buffer);
  }

  if (net_serial.available()) {
    char buffer[512];
    std::scoped_lock buffer_lock(net_serial.buffer_mutex);
    auto count = net_serial.readBytes(buffer, 512);
    serial_stream_3.receive_buffer.write((uint8_t *)buffer, count);
  }


  uint64_t current_ticks = TimeControl::getTicks();
  uint64_t current_priority = std::numeric_limits<uint64_t>::max();
  auto stack_size = isr_stack.size();
  if (stack_size) {
    current_priority = isr_stack.back()->priority;
  }

  uint64_t lowest_isr = std::numeric_limits<uint64_t>::max();
  KernelTimer* next_isr = nullptr;
  for (auto& timer : Timers::timers) {
    uint64_t value = timer.next_interrupt(TimeControl::frequency);
    if (timers_active && value < lowest_isr && value < max_end_ticks && timer.enabled() && !timer.running && timer.priority < current_priority) {
      lowest_isr = value;
      next_isr = &timer;
    }
  }

  if (next_isr != nullptr ) {
    if (current_ticks > lowest_isr) {
      isr_timing_error = TimeControl::ticksToNanos(current_ticks - lowest_isr);
      next_isr->source_offset = current_ticks; // late interrupt
    } else {
      next_isr->source_offset = next_isr->next_interrupt(TimeControl::frequency); // timer was reset when the interrupt fired
      isr_timing_error = 0;
    }
    TimeControl::setTicks(next_isr->source_offset);
    isr_stack.push_back(next_isr);
    next_isr->execute();
    isr_stack.pop_back();
    return true;
  }

  return false;
}

uint64_t Kernel::TimeControl::nanos() {
  if (debug_break_flag) { debug_break_flag = false; debug_break();}  // break into debugger when stuck in time dependent loops
  if (quit_requested) throw (std::runtime_error("Quit Requested"));  // quit program when stuck in time dependent loops
  addTicks(1 + nanosToTicks(100)); // Marlin has loops that only break after x ticks, so we need to increment ticks here
  return ticksToNanos(getTicks());
}

// if a thread wants to wait, see what should be executed during that wait
void Kernel::delayCycles(uint64_t cycles) {
  if (is_initialized()) {
    auto end = TimeControl::getTicks() + cycles;
    while (execute_loop(end) && TimeControl::getTicks() < end);
    if (end > TimeControl::getTicks()) TimeControl::setTicks(end);
  }
}

// this is needed for when marlin loops idle waiting for an event with no delays (syncronize)
void Kernel::yield() {
  if (is_initialized()) {
    if(isr_stack.size() == 0) {
      // Kernel not started?
      TimeControl::addTicks(TimeControl::nanosToTicks(100));
      return;
    }
    auto max_yield = isr_stack.back()->next_interrupt(TimeControl::frequency);
    if(!execute_loop(max_yield)) { // dont wait longer than this threads exec period
      TimeControl::setTicks(max_yield);
      isr_stack.back()->source_offset = max_yield; // there was nothing to run, and we now overrun our next cycle.
    }
  }
}
