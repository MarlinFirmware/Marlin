#ifdef __PLAT_X86_64__

#include "../../../inc/MarlinConfig.h"
#include "Clock.h"

std::chrono::nanoseconds Clock::startup = std::chrono::high_resolution_clock::now().time_since_epoch();
uint32_t Clock::frequency = F_CPU;

#endif // __PLAT_X86_64__
