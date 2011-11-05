#ifndef __MARLINH
#define __MARLINH

// Tonokip RepRap firmware rewrite based off of Hydra-mmm firmware.
// Licence: GPL
#include <WProgram.h>
#include "fastio.h"


#define ECHO(x) Serial << "echo: " << x;
#define ECHOLN(x) Serial << "echo: "<<x<<endl;

void get_command();
void process_commands();

void manage_inactivity(byte debug);

#if X_ENABLE_PIN > -1
#define  enable_x() WRITE(X_ENABLE_PIN, X_ENABLE_ON)
#define disable_x() WRITE(X_ENABLE_PIN,!X_ENABLE_ON)
#else
#define enable_x() ;
#define disable_x() ;
#endif
#if Y_ENABLE_PIN > -1
#define  enable_y() WRITE(Y_ENABLE_PIN, Y_ENABLE_ON)
#define disable_y() WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON)
#else
#define enable_y() ;
#define disable_y() ;
#endif
#if Z_ENABLE_PIN > -1
#define  enable_z() WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)
#define disable_z() WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON)
#else
#define enable_z() ;
#define disable_z() ;
#endif

#if E_ENABLE_PIN > -1

	#define  enable_e() WRITE(E_ENABLE_PIN, E_ENABLE_ON)
	#define disable_e() WRITE(E_ENABLE_PIN,!E_ENABLE_ON)

#else
#define enable_e() ;
#define disable_e() ;
#endif

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define E_AXIS 3

void FlushSerialRequestResend();
void ClearToSend();

void get_coordinates();
void prepare_move();
void kill();

//void check_axes_activity();
//void plan_init();
//void st_init();
//void tp_init();
//void plan_buffer_line(float x, float y, float z, float e, float feed_rate);
//void plan_set_position(float x, float y, float z, float e);
//void st_wake_up();
//void st_synchronize();
void enquecommand(const char *cmd);
void wd_reset();

#ifndef CRITICAL_SECTION_START
#define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
#define CRITICAL_SECTION_END    SREG = _sreg;
#endif //CRITICAL_SECTION_START

extern float homing_feedrate[];
extern bool axis_relative_modes[];

void manage_inactivity(byte debug);

#endif
