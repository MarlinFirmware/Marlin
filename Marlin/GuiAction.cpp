#include "GuiAction.h"

#include "Marlin.h"
#include "cardreader.h"
#include "ConfigurationStore.h"
#include "temperature.h"
#include "language.h"
#include "GuiManager.h"


extern uint8_t buffer_recursivity;
extern bool cancel_heatup;
extern bool stop_buffer;
extern int stop_buffer_code;

static float manual_feedrate[] = MANUAL_FEEDRATE;

void action_homing()
{
	enquecommand_P(PSTR("G28"));
	SERIAL_ECHOLN("G28");
}

void action_print()
{
	SERIAL_ECHOLN("START PRINT");
	char cmd[30];
	char* c;
	strcpy(cmd, card.longFilename);
	for (c = &cmd[0]; *c; c++)
	{
		if ((uint8_t)*c > 127)
		{
			SERIAL_ECHOLN(MSG_SD_BAD_FILENAME);
			//LCD_MESSAGEPGM(MSG_SD_BAD_FILENAME);
			//draw_status_screen();
			return;
		}
	}
	setTargetHotend0(200);
	fanSpeed = PREHEAT_FAN_SPEED;
	sprintf_P(cmd, PSTR("M23 %s"), card.filename);
	enquecommand_P(PSTR("G28"));
	enquecommand_P(PSTR("G1 Z10"));
	for(c = &cmd[4]; *c; c++)
	*c = tolower(*c);
	enquecommand(cmd);
	enquecommand_P(PSTR("M24"));
}

void action_pause_print()
{
	SERIAL_ECHOLN("PAUSA");
	lcd_disable_button();
	stop_buffer = true;
	stop_buffer_code = 1;
}

void action_stop_print()
{
	SERIAL_ECHOLN("STOP PRINT");
	if (buffer_recursivity > 0)
	{
		SERIAL_ECHOLN("Buffer recursivity");
	}
	else
	{
		SERIAL_ECHOLN("else");
		card.sdprinting = false;
		card.closefile();

		setTargetHotend(0,0);

#ifdef HEATED_BED_SUPPORT
		setTargetBed(0);
#endif // HEATED_BED_SUPPORT

		flush_commands();
		quickStop();

		plan_reset_position();

		current_position[X_AXIS] = plan_get_axis_position(X_AXIS);
		current_position[Y_AXIS] = plan_get_axis_position(Y_AXIS);
		current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS) + 10;
		current_position[E_AXIS] = plan_get_axis_position(E_AXIS) - 10;

		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

#if X_MAX_POS < 250
		current_position[X_AXIS] = X_MIN_POS;
		current_position[Y_AXIS] = 150;
		current_position[Z_AXIS] += 20;
#else // X_MAX_POS < 250
		current_position[X_AXIS] = X_MAX_POS - 15;
		current_position[Y_AXIS] = Y_MAX_POS - 15;
		current_position[Z_AXIS] = Z_MAX_POS - 15;
#endif // X_MAX_POS < 250

		if (current_position[Z_AXIS] > Z_MAX_POS)
		{
			current_position[Z_AXIS] = Z_MAX_POS;
		}

		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
		st_synchronize();

		if (SD_FINISHED_STEPPERRELEASE)
		{
			enquecommand_P(PSTR(SD_FINISHED_RELEASECOMMAND));
		}
		autotempShutdown();

		cancel_heatup = true;
	}
}
