#ifndef GUI_ACTION_H
#define GUI_ACTION_H

#include <stdint.h>

extern void action_set_temperature(uint16_t degrees);
extern void action_cooldown();

extern void action_filament_unload();
extern void action_filament_load();

extern void action_level_plate();

extern void action_homing();

extern void action_move_axis_to(uint8_t axis, float position);

extern void action_start_print();
extern void action_stop_print();
extern void action_pause_print();
extern void action_resume_print();
extern void action_set_feedrate_multiply(uint16_t value);

extern void action_offset();
extern void action_set_offset(uint8_t axis, float value);
extern void action_save_offset();

#endif // GUI_ACTION_H