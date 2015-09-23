#ifndef GUI_ACTION_H
#define GUI_ACTION_H

#include <stdint.h>

#include "Action.h"

extern void action_set_temperature(uint16_t degrees);
extern void action_preheat();
extern void action_cooldown();

extern void action_filament_unload();
extern void action_filament_load();

extern void action_level_plate();

extern void gui_action_homing();
extern void gui_action_z_homing();
extern void action_get_plane();

extern void action_move_axis_to(uint8_t axis, float position);
extern void action_move_to_rest();
extern void action_move_to_filament_change();

extern void action_start_print();
extern void action_stop_print();
extern void action_finish_print();
extern void action_pause_print();
extern void action_resume_print();
extern void action_set_feedrate_multiply(uint16_t value);
extern uint16_t action_get_feedrate_multiply();

extern void action_offset_homing();
extern void action_offset();
extern void action_set_offset(uint8_t axis, float value);
extern void action_save_offset();

extern void action_wizard_init();
extern void action_wizard_finish();
extern bool action_check_preheat_temp();
extern bool action_check_cooling();

extern void action_close_inactivity();

#endif // GUI_ACTION_H