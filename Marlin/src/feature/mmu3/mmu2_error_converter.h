#pragma once
#include <stdint.h>
#include <stddef.h>
#include "mmu_hw/buttons.h"
#include "mmu_hw/error_codes.h"
#if HAS_PRUSA_MMU3

namespace MMU2 {

/// Translates MMU2::ErrorCode into an index of Prusa-Error-Codes
/// Basically this is the way to obtain an index into all other functions in this API
uint8_t PrusaErrorCodeIndex(ErrorCode ec);

/// @returns pointer to a PROGMEM string representing the Title of the Prusa-Error-Codes error
/// @param i index of the error - obtained by calling ErrorCodeIndex
const char *PrusaErrorTitle(uint8_t i);

/// @returns pointer to a PROGMEM string representing the multi-page Description of the Prusa-Error-Codes error
/// @param i index of the error - obtained by calling ErrorCodeIndex
const char *PrusaErrorDesc(uint8_t i);

/// @returns the actual numerical value of the Prusa-Error-Codes error
/// @param i index of the error - obtained by calling ErrorCodeIndex
uint16_t PrusaErrorCode(uint8_t i);

/// @returns Btns pair of buttons for a particular Prusa-Error-Codes error
/// @param i index of the error - obtained by calling ErrorCodeIndex
uint8_t PrusaErrorButtons(uint8_t i);

/// @returns pointer to a PROGMEM string representing the Title of a button
/// @param i index of the error - obtained by calling PrusaErrorButtons + extracting low or high nibble from the Btns pair
const char *PrusaErrorButtonTitle(uint8_t bi);

/// @returns pointer to a PROGMEM string representing the "More" button
const char *PrusaErrorButtonMore();

/// Sets the selected button for later pick-up by the MMU state machine.
/// Used to save the GUI selection/decoupling
void SetButtonResponse(ButtonOperations rsp);

/// @returns button index/code based on currently processed error/screen
/// Clears the "pressed" button upon exit
Buttons ButtonPressed(ErrorCode ec);

/// @returns button index/code based on currently processed error/screen
/// Used as a subfunction of ButtonPressed.
/// Does not clear the "pressed" button upon exit
Buttons ButtonAvailable(ErrorCode ec);

} // namespace MMU2
#endif // HAS_PRUSA_MMU3
