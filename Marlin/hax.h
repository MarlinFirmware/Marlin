#ifndef __HAX_H
#define __HAX_H

#define manage_inactivity manage_inactivity2

void manage_inactivity2();

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define BLOCK_BUFFER_SIZE 16
#define FORCE_INLINE inline __attribute__((always_inline)) 
#define NUM_AXIS 4

#define min fmin
#define max fmax

#define CRITICAL_SECTION_START
#define CRITICAL_SECTION_END

enum AxisEnum {X_AXIS=0, Y_AXIS=1, Z_AXIS=2, E_AXIS=3};

#define enable_x()
#define enable_y()
#define enable_z()
#define enable_e()
#define enable_e0()
#define enable_e1()
#define enable_e2()

#define disable_x()
#define disable_y()
#define disable_z()
#define disable_e()
#define disable_e0()
#define disable_e1()
#define disable_e2()

#define MINIMUM_PLANNER_SPEED 0.05
// (mm/sec)

extern int fanSpeed;
extern int extrudemultiply;

#define F_CPU 16000000

#define manage_heater() 
#define lcd_update() 

#define byte uint8_t

#define square(x) ((x)*(x))

#define st_wake_up()
#define st_set_position(x,y,z,e)
#define st_set_e_position(e)

#endif
