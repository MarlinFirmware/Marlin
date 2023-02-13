/**
 * Compatibility Layer  for hdsc.a library
 */

#ifdef TARGET_HC32F46x
#include "HAL.h"

void HAL_watchdog_refresh()
{
  MarlinHAL::watchdog_refresh();
}

#endif // TARGET_HC32F46x
