#include "src/core/language.h"
#if HAS_PRUSA_MMU3
#include "mmu2_progress_converter.h"
#ifdef __AVR__
  #include <avr/pgmspace.h>
#endif
#include "mmu_hw/progress_codes.h"
#include "mmu_hw/errors_list.h"
#include "messages.h"

namespace MMU2 {
                                                                //01234567890123456789
static const char MSG_PROGRESS_OK[] PROGMEM                = ISTR("OK"); ////MSG_PROGRESS_OK c=4
static const char MSG_PROGRESS_ENGAGE_IDLER[] PROGMEM      = ISTR("Engaging idler"); ////MSG_PROGRESS_ENGAGE_IDLER c=20
static const char MSG_PROGRESS_DISENGAGE_IDLER[] PROGMEM   = ISTR("Disengaging idler"); ////MSG_PROGRESS_DISENGAGE_IDLER c=20
static const char MSG_PROGRESS_UNLOAD_FINDA[] PROGMEM      = ISTR("Unloading to FINDA"); ////MSG_PROGRESS_UNLOAD_FINDA c=20
static const char MSG_PROGRESS_UNLOAD_PULLEY[] PROGMEM     = ISTR("Unloading to pulley"); ////MSG_PROGRESS_UNLOAD_PULLEY c=20
static const char MSG_PROGRESS_FEED_FINDA[] PROGMEM        = ISTR("Feeding to FINDA"); ////MSG_PROGRESS_FEED_FINDA c=20
static const char MSG_PROGRESS_FEED_EXTRUDER[] PROGMEM     = ISTR("Feeding to extruder"); ////MSG_PROGRESS_FEED_EXTRUDER c=20
static const char MSG_PROGRESS_FEED_NOZZLE[] PROGMEM       = ISTR("Feeding to nozzle"); ////MSG_PROGRESS_FEED_NOZZLE c=20
static const char MSG_PROGRESS_AVOID_GRIND[] PROGMEM       = ISTR("Avoiding grind"); ////MSG_PROGRESS_AVOID_GRIND c=20
static const char MSG_PROGRESS_WAIT_USER[] PROGMEM         = ISTR("ERR Wait for User"); ////MSG_PROGRESS_WAIT_USER c=20
static const char MSG_PROGRESS_ERR_INTERNAL[] PROGMEM      = ISTR("ERR Internal"); ////MSG_PROGRESS_ERR_INTERNAL c=20
static const char MSG_PROGRESS_ERR_HELP_FIL[] PROGMEM      = ISTR("ERR Help filament"); ////MSG_PROGRESS_ERR_HELP_FIL c=20
static const char MSG_PROGRESS_ERR_TMC[] PROGMEM           = ISTR("ERR TMC failed"); ////MSG_PROGRESS_ERR_TMC c=20
static const char MSG_PROGRESS_SELECT_SLOT[] PROGMEM       = ISTR("Selecting fil. slot"); ////MSG_PROGRESS_SELECT_SLOT c=20
static const char MSG_PROGRESS_PREPARE_BLADE[] PROGMEM     = ISTR("Preparing blade"); ////MSG_PROGRESS_PREPARE_BLADE c=20
static const char MSG_PROGRESS_PUSH_FILAMENT[] PROGMEM     = ISTR("Pushing filament"); ////MSG_PROGRESS_PUSH_FILAMENT c=20
static const char MSG_PROGRESS_PERFORM_CUT[] PROGMEM       = ISTR("Performing cut"); ////MSG_PROGRESS_PERFORM_CUT c=20
static const char MSG_PROGRESSPSTRETURN_SELECTOR[] PROGMEM = ISTR("Returning selector"); ////MSG_PROGRESSPSTRETURN_SELECTOR c=20
static const char MSG_PROGRESS_PARK_SELECTOR[] PROGMEM     = ISTR("Parking selector"); ////MSG_PROGRESS_PARK_SELECTOR c=20
static const char MSG_PROGRESS_EJECT_FILAMENT[] PROGMEM    = ISTR("Ejecting filament"); ////MSG_PROGRESS_EJECT_FILAMENT c=20 //@@todo duplicate
static const char MSG_PROGRESSPSTRETRACT_FINDA[] PROGMEM   = ISTR("Retract from FINDA"); ////MSG_PROGRESSPSTRETRACT_FINDA c=20
static const char MSG_PROGRESS_HOMING[] PROGMEM            = ISTR("Homing"); ////MSG_PROGRESS_HOMING c=20
static const char MSG_PROGRESS_MOVING_SELECTOR[] PROGMEM   = ISTR("Moving selector"); ////MSG_PROGRESS_MOVING_SELECTOR c=20
static const char MSG_PROGRESS_FEED_FSENSOR[] PROGMEM      = ISTR("Feeding to FSensor"); ////MSG_PROGRESS_FEED_FSENSOR c=20

static const char * const progressTexts[] PROGMEM = {
  ISTR(MSG_PROGRESS_OK),
  ISTR(MSG_PROGRESS_ENGAGE_IDLER),
  ISTR(MSG_PROGRESS_DISENGAGE_IDLER),
  ISTR(MSG_PROGRESS_UNLOAD_FINDA),
  ISTR(MSG_PROGRESS_UNLOAD_PULLEY),
  ISTR(MSG_PROGRESS_FEED_FINDA),
  ISTR(MSG_PROGRESS_FEED_EXTRUDER),
  ISTR(MSG_PROGRESS_FEED_NOZZLE),
  ISTR(MSG_PROGRESS_AVOID_GRIND),
  ISTR(MSG_FINISHING_MOVEMENTS), //reuse from messages.cpp
  ISTR(MSG_PROGRESS_DISENGAGE_IDLER), // err disengaging idler is the same text
  ISTR(MSG_PROGRESS_ENGAGE_IDLER), // engage dtto.
  ISTR(MSG_PROGRESS_WAIT_USER),
  ISTR(MSG_PROGRESS_ERR_INTERNAL),
  ISTR(MSG_PROGRESS_ERR_HELP_FIL),
  ISTR(MSG_PROGRESS_ERR_TMC),
  ISTR(MSG_UNLOADING_FILAMENT), //reuse from messages.cpp
  ISTR(MSG_LOADING_FILAMENT), //reuse from messages.cpp
  ISTR(MSG_PROGRESS_SELECT_SLOT),
  ISTR(MSG_PROGRESS_PREPARE_BLADE),
  ISTR(MSG_PROGRESS_PUSH_FILAMENT),
  ISTR(MSG_PROGRESS_PERFORM_CUT),
  ISTR(MSG_PROGRESSPSTRETURN_SELECTOR),
  ISTR(MSG_PROGRESS_PARK_SELECTOR),
  ISTR(MSG_PROGRESS_EJECT_FILAMENT),
  ISTR(MSG_PROGRESSPSTRETRACT_FINDA),
  ISTR(MSG_PROGRESS_HOMING),
  ISTR(MSG_PROGRESS_MOVING_SELECTOR),
  ISTR(MSG_PROGRESS_FEED_FSENSOR)
};

const char * ProgressCodeToText(ProgressCode pc){
  // @@TODO ?? a better fallback option?
  return ( (uint16_t)pc <= (sizeof(progressTexts) / sizeof(progressTexts[0])) )
     ? static_cast<const char *>(pgm_read_ptr(&progressTexts[(uint16_t)pc]))
     : static_cast<const char *>(pgm_read_ptr(&progressTexts[0]));
}

} // namespace MMU2
#endif // HAS_PRUSA_MMU3
