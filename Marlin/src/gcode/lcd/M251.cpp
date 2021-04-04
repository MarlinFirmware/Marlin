#include "../../inc/MarlinConfig.h"

#if HAS_LCD_BRIGHTNESS

#include "../gcode.h"
#include "../../lcd/marlinui.h"

/**
 * M251: Set the LCD brightness
 */
void GcodeSuite::M251() {
  if (parser.seen('B')) ui.set_brightness(parser.value_int());
}

#endif // HAS_LCD_BRIGHTNESS
