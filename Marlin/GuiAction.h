#ifndef GUI_ACTION_H
#define GUI_ACTION_H

#include <stdint.h>

extern void action_set_temperature(uint16_t degrees);
extern void action_cooldown();

extern void action_filament_unload();
extern void action_filament_load();

extern void action_level_plate();

extern void action_homing();

extern void action_start_print();
extern void action_stop_print();
extern void action_pause_print();
extern void action_resume_print();

#endif // GUI_ACTION_H