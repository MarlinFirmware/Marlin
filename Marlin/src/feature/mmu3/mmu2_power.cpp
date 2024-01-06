#include "mmu2.h"
#include "mmu2_power.h"

#include "src/MarlinCore.h"

#include "src/core/macros.h"
#include "src/core/boards.h"
#include "src/pins/pins.h"
// #include "Configuration_var.h"
// #include "fastio.h"
// #include <util/delay.h>

namespace MMU2 {

// On MK3 we cannot do actual power cycle on HW. Instead trigger a hardware reset.
void power_on() {
#if PIN_EXISTS(MMU2_RST)
    WRITE(MMU2_RST_PIN, 1);
    SET_OUTPUT(MMU2_RST_PIN); // setup reset pin
#endif //MMU_HWRESET

    reset();
}

void power_off() {
}

void reset() {
#if PIN_EXISTS(MMU2_RST) // HW - pulse reset pin
    WRITE(MMU2_RST_PIN, 0);
    safe_delay(100);
    WRITE(MMU2_RST_PIN, 1);
#else
    mmu2.Reset(MMU2::Software); // @@TODO needs to be redesigned, this power implementation shall not know anything about the MMU itself
#endif
    // otherwise HW reset is not available
}

} // namespace MMU2
