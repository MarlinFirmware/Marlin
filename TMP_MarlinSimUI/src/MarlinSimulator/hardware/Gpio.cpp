#include "Gpio.h"

pin_data Gpio::pin_map[Gpio::pin_count] = {};
bool Gpio::logging_enabled = false;
