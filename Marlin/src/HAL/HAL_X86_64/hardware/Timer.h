#ifndef _HARDWARE_TIMER_H_
#define _HARDWARE_TIMER_H_

#include <chrono>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>

class Timer {
public:
  Timer();
  virtual ~Timer();

  typedef void (callback_fn)();

  void init(uint32_t sig_id, uint32_t sim_freq, callback_fn* fn);
  void start(uint32_t frequency);
  void enable();
  bool enabled() {return active;}
  void disable();
  void setCompare(uint32_t compare);
  uint32_t getCount();
  uint32_t getCompare() {return compare;}
  uint32_t getOverruns() {return overruns;}
  uint32_t getAvgError() {return avg_error.count();}

  intptr_t getID() {
    return (*(intptr_t*)timerid);
  }

  static void handler(int sig, siginfo_t *si, void *uc){
    Timer* _this = (Timer*)si->si_value.sival_ptr;
    _this->avg_error += (std::chrono::high_resolution_clock::now() - _this->start_time) - _this->period; //high_resolution_clock is also limited in precision, but best we have
    _this->avg_error /= 2; //very crude precision analysis (actually within +-500ns usually)
    _this->start_time = std::chrono::high_resolution_clock::now(); // wrap
    _this->cbfn();
    _this->overruns += timer_getoverrun(_this->timerid); // even at 50Khz this doesn't stay zero, again demonstrating the limitations
                                                         // using a realtime linux kernel would help somewhat
  }

  const uint64_t resolution = 1000000000; // real hardware frequency .. not really this is not an RTOS

private:
  bool active;
  uint32_t compare;
  uint32_t frequency;
  uint32_t overruns;
  timer_t timerid;
  sigset_t mask;
  callback_fn* cbfn;
  std::chrono::nanoseconds period;
  std::chrono::nanoseconds avg_error;
  std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds > start_time;
};

#endif /* _HARDWARE_TIMER_H_ */


