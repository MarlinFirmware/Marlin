///////////////////////////////////////////////////////////////////////////////
/// \file Action.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief UI independent common functions.
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

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

#ifdef BED_DETECTION
void actionBedMissing()
{	
	plan_reset_position();

	current_position[X_AXIS] = plan_get_axis_position(X_AXIS);
	current_position[Y_AXIS] = plan_get_axis_position(Y_AXIS);
	current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS);
	current_position[E_AXIS] = plan_get_axis_position(E_AXIS);
	
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], target[E_AXIS], max_feedrate[E_AXIS], active_extruder);
	st_synchronize();
	
	target[X_AXIS] = POSITION_REST_X;
	target[Y_AXIS] = POSITION_REST_Y;
	target[Z_AXIS] = Z_MAX_POS/2;
	
	enable_endstops(true);

	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], target[Z_AXIS], current_position[E_AXIS], homing_feedrate[Z_AXIS], active_extruder);
	st_synchronize();
	
	current_position[Z_AXIS] = target[Z_AXIS];
	plan_buffer_line(target[X_AXIS], target[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 100 , active_extruder);
	st_synchronize();
	
	current_position[X_AXIS] = target[X_AXIS];
	current_position[Y_AXIS] = target[Y_AXIS];
	enable_endstops(false);
}

void bedDetectionHomeZ(bool firstHoming) 
{
	int axis_home_dir = home_dir(Z_AXIS);
	
	current_position[Z_AXIS] = 0;
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
	
	if(firstHoming)
	{
		feedrate = homing_feedrate[Z_AXIS];
		destination[Z_AXIS] = 1.5 * Z_MAX_POS * axis_home_dir;
	}
	else
	{
		float homing_slow_feedrate[] = HOMING_SLOW_FEEDRATE;
		feedrate = homing_slow_feedrate[Z_AXIS];
		destination[Z_AXIS] = BED_DETECTION_MAX_LOWER * axis_home_dir;
	}
	
	plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
	st_synchronize();

	if(checkZminEndstop() == false)
	{
		quickStop();
		card.sdprinting = false;
		actionBedMissing();
		lcd_enable_button();
		PrintManager::single::instance().setBedMissingFlag(true);
		return;
	}
	else
	{
		PrintManager::single::instance().setBedMissingFlag(false);
	}

	if(!firstHoming)
	{
		//Z axis is at home
		current_position[Z_AXIS] = Z_HOME_POS + add_homing[Z_AXIS];
		min_pos[Z_AXIS] = Z_MIN_POS + add_homing[Z_AXIS];
		max_pos[Z_AXIS] = Z_MAX_POS + add_homing[Z_AXIS];
					
		destination[Z_AXIS] = current_position[Z_AXIS];
		axis_known_position[Z_AXIS] = true;
	}

	endstops_hit_on_purpose();
	feedrate = 0.0;
}
#endif

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

#ifdef BED_DETECTION

		destination[X_AXIS] = round(BED_DETECTION_X_POINT);
		destination[Y_AXIS] = round(BED_DETECTION_Y_POINT);
		destination[Z_AXIS] = Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS) * (-1);    // Set destination away from bed
		feedrate = XY_TRAVEL_SPEED;
		current_position[Z_AXIS] = 0;

		plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
		plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
		st_synchronize();
		current_position[X_AXIS] = destination[X_AXIS];
		current_position[Y_AXIS] = destination[Y_AXIS];
		current_position[Z_AXIS] = destination[Z_AXIS];

		bedDetectionHomeZ(true);
		raised = false;

		plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

		destination[Z_AXIS] = BED_DETECTION_RAISE;
		feedrate = max_feedrate[Z_AXIS];
		plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
		st_synchronize();

		destination[X_AXIS] = round(Z_SAFE_HOMING_X_POINT);
		destination[Y_AXIS] = round(Z_SAFE_HOMING_Y_POINT);
		feedrate = XY_TRAVEL_SPEED;
		plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
		st_synchronize();

		current_position[X_AXIS] = destination[X_AXIS];
		current_position[Y_AXIS] = destination[Y_AXIS];
		current_position[Z_AXIS] = destination[Z_AXIS];
		
		bedDetectionHomeZ(false);
		
		if(PrintManager::single::instance().getBedMissingFlag() == true)
		{
			return;
		}

#else //no BED_DETECTION

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
#endif //BED_DETECTION

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
#ifdef DOGLCD
	PrintManager::knownPosition(true);
#endif //DOGLCD
	lcd_enable_button();
}

void action_z_homing()
{
	lcd_disable_button();

	float saved_feedrate = feedrate;
	int saved_feedmultiply = feedmultiply;
	feedmultiply = 100;

	enable_endstops(true);

	for(int8_t i=0; i < NUM_AXIS; i++)
	{
		current_position[i] = plan_get_axis_position(i);
	}

	for(int8_t i=0; i < NUM_AXIS; i++)
	{
		destination[i] = current_position[i];
	}

#if Z_HOME_DIR < 0

	destination[X_AXIS] = round(Z_SAFE_HOMING_X_POINT);
	destination[Y_AXIS] = round(Z_SAFE_HOMING_Y_POINT);
	destination[Z_AXIS] = 5 * home_dir(Z_AXIS) * (-1);    // Set destination away from bed

	feedrate = XY_TRAVEL_SPEED;
	current_position[Z_AXIS] = 0;

	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
	plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
	st_synchronize();
	current_position[X_AXIS] = destination[X_AXIS];
	current_position[Y_AXIS] = destination[Y_AXIS];

	HOMEAXIS(Z);
	raised = false;

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
#ifdef DOGLCD
	PrintManager::knownPosition(true);
#endif //DOGLCD
	lcd_enable_button();
}

