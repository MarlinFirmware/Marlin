#include "../../inc/MarlinConfig.h"

#include "../gcode.h"
#include "../../module/probe.h"

#if HAS_PROBE_SETTINGS

void GcodeSuite::C001() {
    if (parser.seenval('S')) {
        probe.settings.turn_heaters_off = parser.value_bool();
    }
}

#endif