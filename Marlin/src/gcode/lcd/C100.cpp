#include "../../inc/MarlinConfig.h"

#if HAS_PROBE_SETTINGS && ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../gcode.h"
#include "../../lcd/extui/dgus_creality/DGUSDisplay.h"
#include "../../lcd/extui/dgus_creality/DGUSScreenHandler.h"


void GcodeSuite::C100() {
    // C100 R1 (reset display)
    if (parser.seenval('R') && parser.value_bool()) {
        dgusdisplay.ResetDisplay();

        // Wait for display to come back
        safe_delay(1000);

        ScreenHandler.Buzzer(250, 250);
    }

    // C100 P[num] (switch to page)
    if (parser.seenval('P')) {
        ScreenHandler.GotoScreen(static_cast<DGUSLCD_Screens>(parser.value_byte()));

        ScreenHandler.Buzzer(250, 250);
    }
}
#endif