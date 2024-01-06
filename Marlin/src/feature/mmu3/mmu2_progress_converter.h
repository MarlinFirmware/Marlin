#pragma once
#ifdef HAS_PRUSA_MMU3
#include <stdint.h>
#include <stddef.h>
#include "mmu_hw/progress_codes.h"

namespace MMU2 {

const char *ProgressCodeToText(ProgressCode pc);

}
#endif // HAS_PRUSA_MMU3
