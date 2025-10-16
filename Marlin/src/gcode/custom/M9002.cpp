#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../../module/planner.h"   // <-- planner.synchronize()
#include "../../module/motion.h"    // <-- destination/prepare_move_.../feedrate_mm_s


#if HAS_X_MIN

void GcodeSuite::M9002() {
  // Параметры: D<мм> (по умолчанию 100), F<мм/мин> (по умолчанию 1200)
  const float dist_mm  = parser.seenval('D') ? parser.value_linear_units() : 100.0f;
  const float feed_mm_m = parser.seenval('F') ? parser.value_linear_units() : 1200.0f;

  planner.synchronize();

  auto x_pressed = []() -> bool {
    return bool(READ(X_MIN_PIN) ^ X_MIN_ENDSTOP_INVERTING);
  };

  // Ждать фронта нажатия: "не нажат" -> "нажат"
  bool was = x_pressed();
  for (;;) {
    const bool now = x_pressed();
    if (now && !was) break;
    was = now;
    idle();
  }

  const feedRate_t save_fr = feedrate_mm_s;
  feedrate_mm_s = MMM_TO_MMS(feed_mm_m);

  get_destination_from_command();
  destination.x += dist_mm;
  prepare_line_to_destination();

  planner.synchronize();
  feedrate_mm_s = save_fr;
}

#endif // HAS_X_MIN