#include "mmu2_progress_converter.h"
#include "src/core/language.h"
#ifdef __AVR__
    #include <avr/pgmspace.h>
#endif
#include "mmu_hw/progress_codes.h"
#include "messages.h"

// TODO: This needs to be replaced with whatever we are using to translate text
// #define PSTR(s) (s)
// #define PSTR(s) (s)

namespace MMU2 {
                                                                   //01234567890123456789
static const char MSG_PROGRESS_OK[] PROGMEM               = PSTR("OK"); ////MSG_PROGRESS_OK c=4
static const char MSG_PROGRESS_ENGAGE_IDLER[] PROGMEM     = PSTR("Engaging idler"); ////MSG_PROGRESS_ENGAGE_IDLER c=20
static const char MSG_PROGRESS_DISENGAGE_IDLER[] PROGMEM  = PSTR("Disengaging idler"); ////MSG_PROGRESS_DISENGAGE_IDLER c=20
static const char MSG_PROGRESS_UNLOAD_FINDA[] PROGMEM     = PSTR("Unloading to FINDA"); ////MSG_PROGRESS_UNLOAD_FINDA c=20
static const char MSG_PROGRESS_UNLOAD_PULLEY[] PROGMEM    = PSTR("Unloading to pulley"); ////MSG_PROGRESS_UNLOAD_PULLEY c=20
static const char MSG_PROGRESS_FEED_FINDA[] PROGMEM       = PSTR("Feeding to FINDA"); ////MSG_PROGRESS_FEED_FINDA c=20
static const char MSG_PROGRESS_FEED_EXTRUDER[] PROGMEM    = PSTR("Feeding to extruder"); ////MSG_PROGRESS_FEED_EXTRUDER c=20
static const char MSG_PROGRESS_FEED_NOZZLE[] PROGMEM      = PSTR("Feeding to nozzle"); ////MSG_PROGRESS_FEED_NOZZLE c=20
static const char MSG_PROGRESS_AVOID_GRIND[] PROGMEM      = PSTR("Avoiding grind"); ////MSG_PROGRESS_AVOID_GRIND c=20
static const char MSG_PROGRESS_WAIT_USER[] PROGMEM        = PSTR("ERR Wait for User"); ////MSG_PROGRESS_WAIT_USER c=20
static const char MSG_PROGRESS_ERR_INTERNAL[] PROGMEM     = PSTR("ERR Internal"); ////MSG_PROGRESS_ERR_INTERNAL c=20
static const char MSG_PROGRESS_ERR_HELP_FIL[] PROGMEM     = PSTR("ERR Help filament"); ////MSG_PROGRESS_ERR_HELP_FIL c=20
static const char MSG_PROGRESS_ERR_TMC[] PROGMEM          = PSTR("ERR TMC failed"); ////MSG_PROGRESS_ERR_TMC c=20
static const char MSG_PROGRESS_SELECT_SLOT[] PROGMEM      = PSTR("Selecting fil. slot"); ////MSG_PROGRESS_SELECT_SLOT c=20
static const char MSG_PROGRESS_PREPARE_BLADE[] PROGMEM    = PSTR("Preparing blade"); ////MSG_PROGRESS_PREPARE_BLADE c=20
static const char MSG_PROGRESS_PUSH_FILAMENT[] PROGMEM    = PSTR("Pushing filament"); ////MSG_PROGRESS_PUSH_FILAMENT c=20
static const char MSG_PROGRESS_PERFORM_CUT[] PROGMEM      = PSTR("Performing cut"); ////MSG_PROGRESS_PERFORM_CUT c=20
static const char MSG_PROGRESSPSTRETURN_SELECTOR[] PROGMEM  = PSTR("Returning selector"); ////MSG_PROGRESSPSTRETURN_SELECTOR c=20
static const char MSG_PROGRESS_PARK_SELECTOR[] PROGMEM    = PSTR("Parking selector"); ////MSG_PROGRESS_PARK_SELECTOR c=20
static const char MSG_PROGRESS_EJECT_FILAMENT[] PROGMEM   = PSTR("Ejecting filament"); ////MSG_PROGRESS_EJECT_FILAMENT c=20 //@@todo duplicate
static const char MSG_PROGRESSPSTRETRACT_FINDA[] PROGMEM    = PSTR("Retract from FINDA"); ////MSG_PROGRESSPSTRETRACT_FINDA c=20
static const char MSG_PROGRESS_HOMING[] PROGMEM           = PSTR("Homing"); ////MSG_PROGRESS_HOMING c=20
static const char MSG_PROGRESS_MOVING_SELECTOR[] PROGMEM  = PSTR("Moving selector"); ////MSG_PROGRESS_MOVING_SELECTOR c=20
static const char MSG_PROGRESS_FEED_FSENSOR[] PROGMEM     = PSTR("Feeding to FSensor"); ////MSG_PROGRESS_FEED_FSENSOR c=20

static const char * const progressTexts[] PROGMEM = {
    PSTR(MSG_PROGRESS_OK),
    PSTR(MSG_PROGRESS_ENGAGE_IDLER),
    PSTR(MSG_PROGRESS_DISENGAGE_IDLER),
    PSTR(MSG_PROGRESS_UNLOAD_FINDA),
    PSTR(MSG_PROGRESS_UNLOAD_PULLEY),
    PSTR(MSG_PROGRESS_FEED_FINDA),
    PSTR(MSG_PROGRESS_FEED_EXTRUDER),
    PSTR(MSG_PROGRESS_FEED_NOZZLE),
    PSTR(MSG_PROGRESS_AVOID_GRIND),
    PSTR(MSG_FINISHING_MOVEMENTS), //reuse from messages.cpp
    PSTR(MSG_PROGRESS_DISENGAGE_IDLER), // err disengaging idler is the same text
    PSTR(MSG_PROGRESS_ENGAGE_IDLER), // engage dtto.
    PSTR(MSG_PROGRESS_WAIT_USER),
    PSTR(MSG_PROGRESS_ERR_INTERNAL),
    PSTR(MSG_PROGRESS_ERR_HELP_FIL),
    PSTR(MSG_PROGRESS_ERR_TMC),
    PSTR(MSG_UNLOADING_FILAMENT), //reuse from messages.cpp
    PSTR(MSG_LOADING_FILAMENT), //reuse from messages.cpp
    PSTR(MSG_PROGRESS_SELECT_SLOT),
    PSTR(MSG_PROGRESS_PREPARE_BLADE),
    PSTR(MSG_PROGRESS_PUSH_FILAMENT),
    PSTR(MSG_PROGRESS_PERFORM_CUT),
    PSTR(MSG_PROGRESSPSTRETURN_SELECTOR),
    PSTR(MSG_PROGRESS_PARK_SELECTOR),
    PSTR(MSG_PROGRESS_EJECT_FILAMENT),
    PSTR(MSG_PROGRESSPSTRETRACT_FINDA),
    PSTR(MSG_PROGRESS_HOMING),
    PSTR(MSG_PROGRESS_MOVING_SELECTOR),
    PSTR(MSG_PROGRESS_FEED_FSENSOR)
};

const char * ProgressCodeToText(ProgressCode pc){
    // @@TODO ?? a better fallback option?
    return ( (uint16_t)pc <= (sizeof(progressTexts) / sizeof(progressTexts[0])) )
       ? static_cast<const char *>(pgm_read_ptr(&progressTexts[(uint16_t)pc]))
       : static_cast<const char *>(pgm_read_ptr(&progressTexts[0]));
}

} // namespace MMU2
