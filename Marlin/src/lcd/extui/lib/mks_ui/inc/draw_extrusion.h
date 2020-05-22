#ifndef _LV_DRAW_EXTRUSION_
#define _LV_DRAW_EXTRUSION_




#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

extern void lv_draw_extrusion(void);
extern void lv_clear_extrusion();
extern void disp_ext_type();
extern void disp_ext_step();
extern void disp_ext_speed();
extern void disp_hotend_temp();
extern void disp_extru_amount();

//extern void disp_temp_ready_print();
#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif

#endif

