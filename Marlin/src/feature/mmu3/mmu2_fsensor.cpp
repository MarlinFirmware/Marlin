#include "src/MarlinCore.h"
#include "src/feature/runout.h"
#include "mmu2_fsensor.h"


#if HAS_PRUSA_MMU3
namespace MMU2 {

#if HAS_FILAMENT_SENSOR
FSensorBlockRunout::FSensorBlockRunout() {
    runout.enabled = false; //suppress filament runouts while loading filament.
    // fsensor.setAutoLoadEnabled(false); //suppress filament autoloads while loading filament.
}

FSensorBlockRunout::~FSensorBlockRunout() {
    // fsensor.settings_init(); // restore filament runout state.
    runout.reset();
    runout.enabled = true;
//    SERIAL_ECHOLNPGM("FSUnBlockRunout");
}
#else // FILAMENT_SENSOR
FSensorBlockRunout::FSensorBlockRunout() { }
FSensorBlockRunout::~FSensorBlockRunout() { }
#endif // FILAMENT_SENSOR


FilamentState WhereIsFilament(){
    // return fsensor.getFilamentPresent() ? FilamentState::AT_FSENSOR : FilamentState::NOT_PRESENT;
    return FILAMENT_PRESENT() ? FilamentState::AT_FSENSOR : FilamentState::NOT_PRESENT;
}

} // namespace MMU2
#endif // HAS_PRUSA_MMU3
