#ifndef GUI_ACTION_H
#define GUI_ACTION_H

#include <stdint.h>

extern void action_set_temperature(uint16_t degrees);

extern void action_homing();

extern void action_start_print();
extern void action_stop_print();
extern void action_pause_print();
extern void action_resume_print();

#endif // GUI_ACTION_H