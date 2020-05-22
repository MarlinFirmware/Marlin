#ifndef _LV_DRAW_CHANGE_SPEED_
#define _LV_DRAW_CHANGE_SPEED_




#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define MIN_EXT_SPEED_PERCENT		10
#define MAX_EXT_SPEED_PERCENT		999

extern void lv_draw_change_speed(void);
extern void lv_clear_change_speed();
extern void disp_speed_step();
extern void disp_print_speed();
extern void disp_speed_type();



//extern void disp_temp_ready_print();
#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif

#endif

