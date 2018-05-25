#ifdef __PLAT_X86_64__

#include "Timer.h"
#include <stdio.h>

Timer::Timer() {
  active = false;
  compare = 0;
  frequency = 0;
  overruns = 0;
  timerid = 0;
  cbfn = nullptr;
  period = 0;
  start_time = 0;
  avg_error = 0;
}

Timer::~Timer() {
  timer_delete(timerid);
}

void Timer::init(uint32_t sig_id, uint32_t sim_freq, callback_fn* fn) {
  struct sigaction sa;
  struct sigevent sev;

  frequency = sim_freq;
  cbfn = fn;

  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = Timer::handler;
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
    return; // todo: handle error
  }

  sigemptyset(&mask);
  sigaddset(&mask, SIGRTMIN);

  disable();

  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIGRTMIN;
  sev.sigev_value.sival_ptr = (void*)this;
  if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
    return; // todo: handle error
  }
}

void Timer::start(uint32_t frequency) {
  setCompare(this->frequency / frequency);
  //printf("timer(%ld) started\n", getID());
}

void Timer::enable() {
  if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
    return; // todo: handle error
  }
  active = true;
  //printf("timer(%ld) enabled\n", getID());
}

void Timer::disable() {
  if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
    return; // todo: handle error
  }
  active = false;
}

void Timer::setCompare(uint32_t compare) {
  uint32_t nsec_offset = 0;
  if(active) {
    nsec_offset = Clock::nanos() - this->start_time; // calculate how long the timer would have been running for
    nsec_offset = nsec_offset < 1000 ? nsec_offset : 0; // constrain, this shouldn't be needed but apparently Marlin enables interrupts on the stepper timer before initialising it, todo: investigate ?bug?
  }
  this->compare = compare;
  uint64_t ns = Clock::ticksToNanos(compare, frequency) - nsec_offset;
  struct itimerspec its;
  its.it_value.tv_sec = ns / 1000000000;
  its.it_value.tv_nsec = ns % 1000000000;
  its.it_interval.tv_sec = its.it_value.tv_sec;
  its.it_interval.tv_nsec = its.it_value.tv_nsec;

  if (timer_settime(timerid, 0, &its, NULL) == -1) {
    printf("timer(%ld) failed, compare: %d(%ld)\n", getID(), compare, its.it_value.tv_nsec);
    return; // todo: handle error
  }
  //printf("timer(%ld) started, compare: %d(%d)\n", getID(), compare, its.it_value.tv_nsec);
  this->period = its.it_value.tv_nsec;
  this->start_time = Clock::nanos();
}

uint32_t Timer::getCount() {
  return Clock::nanosToTicks(Clock::nanos() - this->start_time, frequency);
}

#endif // __PLAT_X86_64__
