#ifndef _LV_PRINTER_OPRATION_
#define _LV_PRINTER_OPRATION_

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define MIN_FILE_PRINTED	 100 //5000

extern void printer_state_polling();
extern void filament_pin_setup();
extern void filament_check();

#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif

#endif

