/// @file
/// This file will not be the same on Marlin1 and Marlin2.
/// Its purpose is to unify different macros in either of Marlin incarnations.
#pragma once

#ifdef __AVR__
    #include "src/MarlinCore.h"
    // brings _O and _T macros into MMU
    #include "src/core/language.h"
    #include "../../gcode/gcode.h"
    // we don't have these in Marlin 2.x so just define them here again
    #define _O(x)                             x
    #define _T(x)                             x
    #define MARLIN_KEEPALIVE_STATE_IN_PROCESS KEEPALIVE_STATE(IN_PROCESS)
#elif defined(UNITTEST)
    #define _O(x)                             x
    #define _T(x)                             x
    #define MARLIN_KEEPALIVE_STATE_IN_PROCESS /*KEEPALIVE_STATE(IN_PROCESS) TODO*/
#else
    #include "../../gcode/gcode.h"
    #define _O(x)                             x
    #define _T(x)                             x
    #define MARLIN_KEEPALIVE_STATE_IN_PROCESS KEEPALIVE_STATE(IN_PROCESS)
#endif
