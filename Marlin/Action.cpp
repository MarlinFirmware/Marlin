#include "Action.h"

#include "Marlin.h"
#include "cardreader.h"
#include "ConfigurationStore.h"
#include "planner.h"
#include "stepper.h"
#include "Serial.h"
#include "Language.h"

#include "GuiManager.h"
#include "TemperatureManager.h"
#include "OffsetManager.h"
#include "AutoLevelManager.h"
#include "PrintManager.h"

extern bool raised;
extern unsigned long previous_millis_cmd;

#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)

void action_homing()
{
	lcd_disable_button();

#ifdef LEVEL_SENSOR
	if (AutoLevelManager::single::instance().state() == false)
	{
		plan_bed_level_matrix.set_to_identity();
	}
#endif

	float saved_feedrate = feedrate;
	int saved_feedmultiply = feedmultiply;
	feedmultiply = 100;

	enable_endstops(true);

	home_all_axis = !((code_seen(axis_codes[X_AXIS])) || (code_seen(axis_codes[Y_AXIS])) || (code_seen(axis_codes[Z_AXIS])));

#ifdef LEVEL_SENSOR
	if (raised == false && (home_all_axis || code_seen(axis_codes[X_AXIS]) || code_seen(axis_codes[Y_AXIS])))
	{
		for(int8_t i=0; i < NUM_AXIS; i++)
		{
			destination[i] = plan_get_axis_position(i);
		}

		destination[Z_AXIS] += 5;
		feedrate = max_feedrate[Z_AXIS];

		plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
		st_synchronize();
		raised = true;
	}
#endif

	for(int8_t i=0; i < NUM_AXIS; i++)
	{
		destination[i] = current_position[i];
	}

	feedrate = 0.0;

#if Z_HOME_DIR > 0
	if((home_all_axis) || (code_seen(axis_codes[Z_AXIS]))) {
        HOMEAXIS(Z);
    }
#endif //Z_HOME_DIR > 0

#ifdef QUICK_HOME
    if((home_all_axis)||( code_seen(axis_codes[X_AXIS]) && code_seen(axis_codes[Y_AXIS])) )  
    {
		current_position[X_AXIS] = 0;current_position[Y_AXIS] = 0;

	#ifndef DUAL_X_CARRIAGE
		int x_axis_home_dir = home_dir(X_AXIS);
	#else //DUAL_X_CARRIAGE
		int x_axis_home_dir = x_home_dir(active_extruder);
		extruder_duplication_enabled = false;
	#endif //DUAL_X_CARRIAGE

		plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
		destination[X_AXIS] = 1.5 * max_length(X_AXIS) * x_axis_home_dir;destination[Y_AXIS] = 1.5 * max_length(Y_AXIS) * home_dir(Y_AXIS);
		feedrate = homing_feedrate[X_AXIS];
		if(homing_feedrate[Y_AXIS]<feedrate)
		{
			feedrate = homing_feedrate[Y_AXIS];
		}
		if (max_length(X_AXIS) > max_length(Y_AXIS))
		{
			feedrate *= sqrt(pow(max_length(Y_AXIS) / max_length(X_AXIS), 2) + 1);
		}
		else
		{
			feedrate *= sqrt(pow(max_length(X_AXIS) / max_length(Y_AXIS), 2) + 1);
		}
		plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
		st_synchronize();

		axis_is_at_home(X_AXIS);
		axis_is_at_home(Y_AXIS);
		plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
		destination[X_AXIS] = current_position[X_AXIS];
		destination[Y_AXIS] = current_position[Y_AXIS];
		plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
		feedrate = 0.0;
		st_synchronize();
		endstops_hit_on_purpose();

		current_position[X_AXIS] = destination[X_AXIS];
		current_position[Y_AXIS] = destination[Y_AXIS];
		current_position[Z_AXIS] = destination[Z_AXIS];
	}
#endif //QUICK_HOME

	if((home_all_axis) || (code_seen(axis_codes[X_AXIS])))
    {
#ifdef DUAL_X_CARRIAGE
	int tmp_extruder = active_extruder;
	extruder_duplication_enabled = false;
	active_extruder = !active_extruder;
	HOMEAXIS(X);
	inactive_extruder_x_pos = current_position[X_AXIS];
	active_extruder = tmp_extruder;
	HOMEAXIS(X);
	// reset state used by the different modes
	memcpy(raised_parked_position, current_position, sizeof(raised_parked_position));
	delayed_move_time = 0;
	active_extruder_parked = true;
#else //DUAL_X_CARRIAGE
	HOMEAXIS(X);
#endif //DUAL_X_CARRIAGE
	}

	if((home_all_axis) || (code_seen(axis_codes[Y_AXIS]))) 
	{
		HOMEAXIS(Y);
	}

#if Z_HOME_DIR < 0
	if((home_all_axis) || (axis_known_position[X_AXIS]==true && axis_known_position[Y_AXIS]==true && code_seen(axis_codes[Z_AXIS]))) 
	{
#ifndef LEVEL_SENSOR
#if defined (Z_RAISE_BEFORE_HOMING) && (Z_RAISE_BEFORE_HOMING > 0)
		destination[Z_AXIS] = Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS) * (-1);    // Set destination away from bed
		feedrate = max_feedrate[Z_AXIS];
		plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
		st_synchronize();
#endif //defined (Z_RAISE_BEFORE_HOMING) && (Z_RAISE_BEFORE_HOMING > 0)
		HOMEAXIS(Z);
#else //LEVEL_SENSOR
		destination[X_AXIS] = round(Z_SAFE_HOMING_X_POINT);
		destination[Y_AXIS] = round(Z_SAFE_HOMING_Y_POINT);
		destination[Z_AXIS] = Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS) * (-1);    // Set destination away from bed
		feedrate = XY_TRAVEL_SPEED;
		current_position[Z_AXIS] = 0;

		plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
		plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
		st_synchronize();
		current_position[X_AXIS] = destination[X_AXIS];
		current_position[Y_AXIS] = destination[Y_AXIS];

		HOMEAXIS(Z);
		raised = false;
#endif //LEVEL_SENSOR
	}
	else if (axis_known_position[X_AXIS]==true && code_seen(axis_codes[Z_AXIS]))
	{
		enquecommand("G28 Y0 Z0");
	}
	else if (axis_known_position[Y_AXIS]==true && code_seen(axis_codes[Z_AXIS]))
	{
		enquecommand("G28 X0 Z0");
	}
	else if (axis_known_position[X_AXIS]==false && axis_known_position[Y_AXIS]==false && code_seen(axis_codes[Z_AXIS]))
	{
		enquecommand("G28 X0 Y0 Z0");
	}
#endif //Z_HOME_DIR < 0

#ifdef LEVEL_SENSOR
	current_position[Z_AXIS] += zprobe_zoffset;
#endif //LEVEL_SENSOR
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

#ifdef ENDSTOPS_ONLY_FOR_HOMING
	enable_endstops(false);
#endif //ENDSTOPS_ONLY_FOR_HOMING

	feedrate = saved_feedrate;
	feedmultiply = saved_feedmultiply;
	endstops_hit_on_purpose();

	previous_millis_cmd = millis();

	PrintManager::knownPosition(true);
	lcd_enable_button();
}