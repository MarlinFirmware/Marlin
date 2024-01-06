#pragma once
#include "src/core/macros.h"
#include <stdint.h>
// #include "../runout.h"


#if ANY(HAS_PRUSA_MMU3, HAS_PRUSA_MMU2S, MMU_EXTRUDER_SENSOR)
  #define FILAMENT_PRESENT() (READ(FIL_RUNOUT1_PIN) != FIL_RUNOUT1_STATE)
#else
  #define FILAMENT_PRESENT() true
#endif

namespace MMU2 {

/// Can be used to block printer's filament sensor handling - to avoid errorneous injecting of M600
/// while doing a toolchange with the MMU
/// In case of "no filament sensor" these methods default to an empty implementation
class FSensorBlockRunout {
public:
    FSensorBlockRunout();
    ~FSensorBlockRunout();
};

/// Possible states of filament from the perspective of presence in various parts of the printer
/// Beware, the numeric codes are important and sent into the MMU
enum class FilamentState : uint_fast8_t {
    NOT_PRESENT = 0, ///< filament sensor doesn't see the filament
    AT_FSENSOR = 1,  ///< filament detected by the filament sensor, but the nozzle has not detected the filament yet
    IN_NOZZLE = 2,   ///< filament detected by the filament sensor and also loaded in the nozzle
    UNAVAILABLE = 3  ///< sensor not available (likely not connected due broken cable)
};

FilamentState WhereIsFilament();

} // namespace MMU2
