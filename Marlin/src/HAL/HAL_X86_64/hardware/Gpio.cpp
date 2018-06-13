#ifdef __PLAT_X86_64__
#include "Gpio.h"

pin_data Gpio::pin_map[Gpio::pin_count+1] = {};
IOLogger* Gpio::logger = nullptr;


#endif // __PLAT_X86_64__
