#include "GuiAction.h"

#include "Marlin.h"
#include "cardreader.h"
#include "ConfigurationStore.h"
#include "planner.h"
#include "stepper.h"
#include "language.h"

#include "GuiManager.h"
#include "TemperatureManager.h"


extern uint8_t buffer_recursivity;
extern bool cancel_heatup;
extern bool stop_buffer;
extern int stop_buffer_code;

static float manual_feedrate[] = MANUAL_FEEDRATE;

void action_set_temperature(uint16_t degrees)
{
	TemperatureManager::single::instance().setTargetTemperature(degrees);
}

void action_cooldown()
{
	TemperatureManager::single::instance().setTargetTemperature(0);
}

void action_filament_unload()
{
	enquecommand_P(PSTR("M702"));
}

void action_filament_load()
{
	enquecommand_P(PSTR("M701"));
}

void action_level_plate()
{
	static uint8_t level_plate_step = 0;

	switch (level_plate_step)
	{
		case 0:
			target[X_AXIS] = plan_get_axis_position(X_AXIS);
			target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
			target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
			target[E_AXIS] = plan_get_axis_position(E_AXIS);

			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] += 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[X_AXIS] = 148.5;
			target[Y_AXIS] = 200;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] -= 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			break;

		case 1:
			target[X_AXIS] = plan_get_axis_position(X_AXIS);
			target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
			target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
			target[E_AXIS] = plan_get_axis_position(E_AXIS);

			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] += 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[X_AXIS] = 90;
			target[Y_AXIS] = 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] -= 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			break;

		case 2:
			target[X_AXIS] = plan_get_axis_position(X_AXIS);
			target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
			target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
			target[E_AXIS] = plan_get_axis_position(E_AXIS);

			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] += 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[X_AXIS] = 207;
			target[Y_AXIS] = 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] -= 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			break;

		case 3:
			target[X_AXIS] = plan_get_axis_position(X_AXIS);
			target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
			target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
			target[E_AXIS] = plan_get_axis_position(E_AXIS);

			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] += 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[X_AXIS] = 148.5;
			target[Y_AXIS] = 105;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] -= 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			break;

		case 4:
			target[X_AXIS] = plan_get_axis_position(X_AXIS);
			target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
			target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
			target[E_AXIS] = plan_get_axis_position(E_AXIS);

			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] += 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[X_AXIS] = 297;
			target[Y_AXIS] = 210;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] -= 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			break;
	}

	level_plate_step = ++level_plate_step % 5;
}

void action_homing()
{
	enquecommand_P(PSTR("G28"));
	SERIAL_ECHOLN("G28");
}

void action_start_print()
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
	TemperatureManager::single::instance().setTargetTemperature(200);
	fanSpeed = PREHEAT_FAN_SPEED;
	sprintf_P(cmd, PSTR("M23 %s"), card.filename);
	enquecommand_P(PSTR("G28"));
	enquecommand_P(PSTR("G1 Z10"));
	for(c = &cmd[4]; *c; c++)
	*c = tolower(*c);
	enquecommand(cmd);
	enquecommand_P(PSTR("M24"));
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

		TemperatureManager::single::instance().setTargetTemperature(0);

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
		// autotempShutdown();

		cancel_heatup = true;
	}
}

extern float target[4];
extern float lastpos[4];

void action_pause_print()
{
	lcd_disable_button();
	card.sdprinting = false;
	stop_buffer = true;
	stop_buffer_code = 1;
}

void action_resume_print()
{
	lcd_disable_button();

	plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder); //should do nothing
	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], target[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder); //move xy back
	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder); //move z back
	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], lastpos[E_AXIS], feedrate/60, active_extruder); //final untretract
	st_synchronize();

	card.sdprinting = true;

	lcd_enable_button();
	stop_buffer = false;
}