///////////////////////////////////////////////////////////////////////////////
/// \file GuiAction.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief UI dependent common functions.
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

#include "GuiAction.h"

#include "Marlin.h"
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
#include "StorageManager.h"
#include "SerialManager.h"
#include "LightManager.h"
#include "cardreader.h"

#ifdef DOGLCD
	#include "StatsManager.h"
	#include "HeatedbedManager.h"
	#include "FanManager.h"
#endif

bool raised = false;
extern bool home_all_axis;
extern bool bed_leveling;
extern const char axis_codes[NUM_AXIS];
extern bool cancel_heatup;
extern bool stop_planner_buffer;
extern bool planner_buffer_stopped;
extern bool stop_buffer;
bool change_filament = false; 
extern uint16_t stop_buffer_code;

static float manual_feedrate[] = MANUAL_FEEDRATE;

float z_offset;
float z_saved_homing;

extern float current_position[NUM_AXIS];
extern void clean_up_after_endstop_move();
extern void do_blocking_move_to(float x, float y, float z);
extern void setup_for_endstop_move();
extern float probe_pt(float x, float y, float z_before, int retract_action = 0);

void action_set_temperature(uint16_t degrees)
{
	temp::TemperatureManager::single::instance().setTargetTemperature(degrees);
}

void action_preheat()
{
	temp::TemperatureManager::single::instance().setBlowerControlState(true);
	temp::TemperatureManager::single::instance().setTargetTemperature(PREHEAT_HOTEND_TEMP);
}

void action_cooldown()
{
	temp::TemperatureManager::single::instance().setBlowerControlState(true);
	temp::TemperatureManager::single::instance().setTargetTemperature(0);
#if HEATER_BED_PIN > -1
	temp::TemperatureManager::single::instance().setBedTargetTemperature(0);
#endif
}

void action_filament_unload()
{
	st_synchronize();

	current_position[E_AXIS] += 50.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 5, active_extruder);
	st_synchronize();

	current_position[E_AXIS] -= 60.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 5, active_extruder);
	st_synchronize();
}

void action_filament_load()
{
	st_synchronize();

	current_position[E_AXIS] += 140.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 5, active_extruder);
	st_synchronize();

	current_position[E_AXIS] -= RETRACT_ON_PAUSE;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], max_feedrate[E_AXIS], active_extruder);
	st_synchronize();
}

uint8_t level_plate_step = 0;

void action_level_plate()
{
	#ifndef ABL_MANUAL_PT_4_X
		uint8_t max_steps = 4;
		uint8_t order[4] = {0,1,2,4};
	#else
		uint8_t max_steps = 5;
		uint8_t order[5] = {0,1,2,3,4};
	#endif

	switch (order[level_plate_step])
	{
		case 0:
			lcd_disable_button();

			target[X_AXIS] = plan_get_axis_position(X_AXIS);
			target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
			target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
			target[E_AXIS] = plan_get_axis_position(E_AXIS);

			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] = 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[X_AXIS] = ABL_MANUAL_PT_1_X;
			target[Y_AXIS] = ABL_MANUAL_PT_1_Y;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] = 0 + OffsetManager::single::instance().offset();
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			lcd_enable_button();

			break;

		case 1:
			lcd_disable_button();

			target[X_AXIS] = plan_get_axis_position(X_AXIS);
			target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
			target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
			target[E_AXIS] = plan_get_axis_position(E_AXIS);

			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] = 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[X_AXIS] = ABL_MANUAL_PT_2_X;
			target[Y_AXIS] = ABL_MANUAL_PT_2_Y;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] = 0 + OffsetManager::single::instance().offset();
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			lcd_enable_button();

			break;

		case 2:
			lcd_disable_button();

			target[X_AXIS] = plan_get_axis_position(X_AXIS);
			target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
			target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
			target[E_AXIS] = plan_get_axis_position(E_AXIS);

			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] = 10;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[X_AXIS] = ABL_MANUAL_PT_3_X;
			target[Y_AXIS] = ABL_MANUAL_PT_3_Y;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] = 0 + OffsetManager::single::instance().offset();
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			lcd_enable_button();

			break;

		case 3:
			#ifdef ABL_MANUAL_PT_4_X
				lcd_disable_button();

				target[X_AXIS] = plan_get_axis_position(X_AXIS);
				target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
				target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
				target[E_AXIS] = plan_get_axis_position(E_AXIS);

				plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

				target[Z_AXIS] = 10;
				plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

				target[X_AXIS] = ABL_MANUAL_PT_4_X;
				target[Y_AXIS] = ABL_MANUAL_PT_4_Y;
				plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

				target[Z_AXIS] = 0 + OffsetManager::single::instance().offset();;
				plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
				st_synchronize();

				lcd_enable_button();
			#endif // ABL_MANUAL_PT_4_X

			break;

		case 4:
			lcd_disable_button();
			lcd_enable_button();

			break;
	}

	level_plate_step = ++level_plate_step % max_steps;
}

void gui_action_homing()
{
	action_homing();
#ifdef BED_DETECTION
	if(PrintManager::single::instance().getBedMissingFlag() == true)
	{
		return;
	}
#endif // BED_DETECTION
	action_move_to_rest();
}

void gui_action_z_homing()
{
	action_z_homing();
	action_move_to_rest();
}

#ifdef LEVEL_SENSOR
static void set_bed_level_equation_3pts(float z_at_pt_1, float z_at_pt_2, float z_at_pt_3) 
{

    plan_bed_level_matrix.set_to_identity();

    vector_3 pt1 = vector_3(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, z_at_pt_1);
    vector_3 pt2 = vector_3(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, z_at_pt_2);
    vector_3 pt3 = vector_3(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, z_at_pt_3);

    vector_3 from_2_to_1 = (pt1 - pt2).get_normal();
    vector_3 from_3_to_2 = (pt2 - pt3).get_normal();
    vector_3 planeNormal = vector_3::cross(from_2_to_1, from_3_to_2).get_normal();
    planeNormal = vector_3(planeNormal.x, planeNormal.y, abs(planeNormal.z));

    plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);

    vector_3 corrected_position = plan_get_position();
    current_position[X_AXIS] = corrected_position.x;
    current_position[Y_AXIS] = corrected_position.y;
    current_position[Z_AXIS] = corrected_position.z;

    // put the bed at 0 so we don't go below it.
    current_position[Z_AXIS] = zprobe_zoffset;

    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}

void action_get_plane()
{
	#if Z_MIN_PIN == -1
		#error "You must have a Z_MIN endstop in order to enable Auto Bed Leveling feature!!! Z_MIN_PIN must point to a valid hardware pin."
	#endif
	
#ifdef DOGLCD
#if HEATER_BED_PIN > -1
	if(HeatedbedManager::single::instance().detected())
	{
		temp::TemperatureManager::single::instance().setBedTargetTemperature(0);
		while(HeatedbedManager::single::instance().detected() && temp::TemperatureManager::single::instance().getBedCurrentTemperature() > BED_AUTOLEVEL_TEMP)
		{
			unsigned long curtime = millis();
			if(( millis() - curtime) > 1000 ) //Print Temp Reading every 1 second while heating up.
			{
				float tt=temp::TemperatureManager::single::instance().getCurrentTemperature();
				SERIAL_PROTOCOLPGM("T:");
				SERIAL_PROTOCOL(tt);
				SERIAL_PROTOCOLPGM(" E:");
				SERIAL_PROTOCOL((int)0);
				SERIAL_PROTOCOLPGM(" B:");
				SERIAL_PROTOCOL_F(temp::TemperatureManager::single::instance().getBedCurrentTemperature(),1);
				SERIAL_PROTOCOLLN("");
				curtime = millis();
			}
		};
	}
#endif // HEATER_BED_PIN > -1
#endif // DOGLCD

	// Prevent user from running a G29 without first homing in X and Y
	if (! (axis_known_position[X_AXIS] && axis_known_position[Y_AXIS]) )
	{
		LCD_MESSAGEPGM(MSG_POSITION_UNKNOWN);
		SERIAL_ECHO_START;
		SERIAL_ECHOLNPGM(MSG_POSITION_UNKNOWN);
		return; // abort G29, since we don't know where we are
	}

	#ifdef Z_PROBE_SLED
		dock_sled(false);
	#endif // Z_PROBE_SLED
	st_synchronize();

	// make sure the bed_level_rotation_matrix is identity or the planner will get it incorectly
	//vector_3 corrected_position = plan_get_position_mm();
	//corrected_position.debug("position before G29");
	plan_bed_level_matrix.set_to_identity();
	vector_3 uncorrected_position = plan_get_position();
	//uncorrected_position.debug("position durring G29");
	current_position[X_AXIS] = uncorrected_position.x;
	current_position[Y_AXIS] = uncorrected_position.y;
	current_position[Z_AXIS] = uncorrected_position.z;
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
	setup_for_endstop_move();

	feedrate = homing_feedrate[Z_AXIS];
	#ifdef AUTO_BED_LEVELING_GRID
		// probe at the points of a lattice grid

		int xGridSpacing = (RIGHT_PROBE_BED_POSITION - LEFT_PROBE_BED_POSITION) / (AUTO_BED_LEVELING_GRID_POINTS-1);
		int yGridSpacing = (BACK_PROBE_BED_POSITION - FRONT_PROBE_BED_POSITION) / (AUTO_BED_LEVELING_GRID_POINTS-1);


		// solve the plane equation ax + by + d = z
		// A is the matrix with rows [x y 1] for all the probed points
		// B is the vector of the Z positions
		// the normal vector to the plane is formed by the coefficients of the plane equation in the standard form, which is Vx*x+Vy*y+Vz*z+d = 0
		// so Vx = -a Vy = -b Vz = 1 (we want the vector facing towards positive Z

		// "A" matrix of the linear system of equations
		double eqnAMatrix[AUTO_BED_LEVELING_GRID_POINTS*AUTO_BED_LEVELING_GRID_POINTS*3];
		// "B" vector of Z points
		double eqnBVector[AUTO_BED_LEVELING_GRID_POINTS*AUTO_BED_LEVELING_GRID_POINTS];


		int probePointCounter = 0;
		bool zig = true;

		for (int yProbe=FRONT_PROBE_BED_POSITION; yProbe <= BACK_PROBE_BED_POSITION; yProbe += yGridSpacing)
		{
			int xProbe, xInc;
			if (zig)
			{
				xProbe = LEFT_PROBE_BED_POSITION;
				//xEnd = RIGHT_PROBE_BED_POSITION;
				xInc = xGridSpacing;
				zig = false;
			}
			else // zag
			{
				xProbe = RIGHT_PROBE_BED_POSITION;
				//xEnd = LEFT_PROBE_BED_POSITION;
				xInc = -xGridSpacing;
				zig = true;
			}

			for (int xCount=0; xCount < AUTO_BED_LEVELING_GRID_POINTS; xCount++)
			{
				float z_before;
				if (probePointCounter == 0)
				{
					// raise before probing
					z_before = Z_RAISE_BEFORE_PROBING;
				}
				else
				{
					// raise extruder
					z_before = current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS;
				}

				float measured_z;
				//Enhanced G29 - Do not retract servo between probes
				if (code_seen('E') || code_seen('e') )
				{
					if ((yProbe==FRONT_PROBE_BED_POSITION) && (xCount==0))
					{
						measured_z = probe_pt(xProbe, yProbe, z_before,1);
					}
					else if ((yProbe==FRONT_PROBE_BED_POSITION + (yGridSpacing * (AUTO_BED_LEVELING_GRID_POINTS-1))) && (xCount == AUTO_BED_LEVELING_GRID_POINTS-1))
					{
						measured_z = probe_pt(xProbe, yProbe, z_before,3);
					}
					else
					{
						measured_z = probe_pt(xProbe, yProbe, z_before,2);
					}
				} else {
					measured_z = probe_pt(xProbe, yProbe, z_before);
				}

				eqnBVector[probePointCounter] = measured_z;

				eqnAMatrix[probePointCounter + 0*AUTO_BED_LEVELING_GRID_POINTS*AUTO_BED_LEVELING_GRID_POINTS] = xProbe;
				eqnAMatrix[probePointCounter + 1*AUTO_BED_LEVELING_GRID_POINTS*AUTO_BED_LEVELING_GRID_POINTS] = yProbe;
				eqnAMatrix[probePointCounter + 2*AUTO_BED_LEVELING_GRID_POINTS*AUTO_BED_LEVELING_GRID_POINTS] = 1;
				probePointCounter++;
				xProbe += xInc;
			}
		}
		clean_up_after_endstop_move();

		// solve lsq problem
		double *plane_equation_coefficients = qr_solve(AUTO_BED_LEVELING_GRID_POINTS*AUTO_BED_LEVELING_GRID_POINTS, 3, eqnAMatrix, eqnBVector);

		SERIAL_PROTOCOLPGM("Eqn coefficients: a: ");
		SERIAL_PROTOCOL(plane_equation_coefficients[0]);
		SERIAL_PROTOCOLPGM(" b: ");
		SERIAL_PROTOCOL(plane_equation_coefficients[1]);
		SERIAL_PROTOCOLPGM(" d: ");
		SERIAL_PROTOCOLLN(plane_equation_coefficients[2]);


		set_bed_level_equation_lsq(plane_equation_coefficients);

		free(plane_equation_coefficients);

	#else // AUTO_BED_LEVELING_GRID not defined

		// Probe at 3 arbitrary points
		// Enhanced G29

		float z_at_pt_1, z_at_pt_2, z_at_pt_3;

		if (code_seen('E') || code_seen('e')) {
			// probe 1
			z_at_pt_1 = probe_pt(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, Z_RAISE_BEFORE_PROBING,1);
			// probe 2
			z_at_pt_2 = probe_pt(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS,2);
			// probe 3
			z_at_pt_3 = probe_pt(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS,3); 
		}
		else
		{
			// probe 1
			z_at_pt_1 = probe_pt(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, Z_RAISE_BEFORE_PROBING);
			// probe 2
			z_at_pt_2 = probe_pt(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS);
			// probe 3
			z_at_pt_3 = probe_pt(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS);
		}
		clean_up_after_endstop_move();

		set_bed_level_equation_3pts(z_at_pt_1, z_at_pt_2, z_at_pt_3);

	#endif // AUTO_BED_LEVELING_GRID
	st_synchronize();

	// The following code correct the Z height difference from z-probe position and hotend tip position.
	// The Z height on homing is measured by Z-Probe, but the probe is quite far from the hotend.
	// When the bed is uneven, this height must be corrected.
	float x_tmp, y_tmp, z_tmp, real_z;

	real_z = float(st_get_position(Z_AXIS))/axis_steps_per_unit[Z_AXIS];  //get the real Z (since the auto bed leveling is already correcting the plane)
	x_tmp = current_position[X_AXIS] + X_PROBE_OFFSET_FROM_EXTRUDER;
	y_tmp = current_position[Y_AXIS] + Y_PROBE_OFFSET_FROM_EXTRUDER;
	z_tmp = current_position[Z_AXIS];

	apply_rotation_xyz(plan_bed_level_matrix, x_tmp, y_tmp, z_tmp);         //Apply the correction sending the probe offset
	current_position[Z_AXIS] = real_z -z_tmp + current_position[Z_AXIS];   //The difference is added to current position and sent to planner.
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
	#ifdef Z_PROBE_SLED
		dock_sled(true, -SLED_DOCKING_OFFSET); // correct for over travel.
	#endif // Z_PROBE_SLED
}
#endif //LEVEL_SENSOR

void action_correct_movement(float &x_pos, float &y_pos, float &z_pos)
{
	st_synchronize();
	vector_3 update_position = plan_get_position();

	if (checkXminEndstop() == true || checkXmaxEndstop() == true)
	{
		if (update_position.y != y_pos)
		{
			plan_set_axis_position(Y_AXIS,getRealPosAxis(Y_AXIS));
		}
		if (update_position.z != z_pos)
		{
			plan_set_axis_position(Z_AXIS,getRealPosAxis(Z_AXIS));
		}

		if (checkXminEndstop() == true)
		{
			plan_set_axis_position(X_AXIS,X_MIN_POS);
		}
		else
		{
			plan_set_axis_position(X_AXIS,X_MAX_POS);
		}
		endstops_hit_on_purpose();
	}
	if (checkYminEndstop() == true || checkYmaxEndstop() == true)
	{
		if (update_position.x != x_pos)
		{
			plan_set_axis_position(X_AXIS,getRealPosAxis(X_AXIS));
		}
		if (update_position.z != z_pos)
		{
			plan_set_axis_position(Z_AXIS,getRealPosAxis(Z_AXIS));
		}

		if (checkYminEndstop() == true)
		{
			plan_set_axis_position(Y_AXIS,Y_MIN_POS);
		}
		else
		{
			plan_set_axis_position(Y_AXIS,Y_MAX_POS);
		}
		endstops_hit_on_purpose();
	}
	if (checkZmaxEndstop() == true)
	{
		if (update_position.x != x_pos)
		{
			plan_set_axis_position(X_AXIS,getRealPosAxis(X_AXIS));
		}
		if (update_position.y != y_pos)
		{
			plan_set_axis_position(Y_AXIS,getRealPosAxis(Y_AXIS));
		}

		if (checkZmaxEndstop() == true)
		{
			plan_set_axis_position(Z_AXIS,Z_MAX_POS);
		}
		endstops_hit_on_purpose();
	}

	vector_3 update_position_2 = plan_get_position();
	current_position[X_AXIS] = update_position_2.x;
	current_position[Y_AXIS] = update_position_2.y;
	current_position[Z_AXIS] = update_position_2.z;
	st_synchronize();
}

void action_move_axis_to(uint8_t axis, float position)
{
	current_position[axis] = position;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[axis]/60, active_extruder);
}

void action_move_to_rest()
{
	st_synchronize();
	vector_3 update_position = plan_get_position();
	current_position[X_AXIS] = update_position.x;
	current_position[Y_AXIS] = update_position.y;
	current_position[Z_AXIS] = update_position.z;

	enable_endstops(true);

	if (current_position[Z_AXIS] < POSITION_REST_Z)
	{
		target[Z_AXIS] = POSITION_REST_Z;
		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], target[Z_AXIS],current_position[E_AXIS], 100, active_extruder);
		st_synchronize();
		vector_3 update_position_2 = plan_get_position();
		current_position[Z_AXIS] = update_position_2.z;
	}

	target[X_AXIS] = POSITION_REST_X;
	target[Y_AXIS] = POSITION_REST_Y;
	plan_buffer_line(target[X_AXIS], target[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 100, active_extruder);
	st_synchronize();

	action_correct_movement(target[X_AXIS], target[Y_AXIS], target[Z_AXIS]);

	plan_buffer_line(target[X_AXIS], target[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 100, active_extruder);
	st_synchronize();

	action_correct_movement(target[X_AXIS], target[Y_AXIS], target[Z_AXIS]);

	plan_buffer_line(target[X_AXIS], target[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 100, active_extruder);
	st_synchronize();

	enable_endstops(false);

	vector_3 update_position_3 = plan_get_position();
	current_position[X_AXIS] = update_position_3.x;
	current_position[Y_AXIS] = update_position_3.y;
	current_position[Z_AXIS] = update_position_3.z;
}

void action_move_to_filament_change()
{
	st_synchronize();

	vector_3 update_position = plan_get_position();
	current_position[X_AXIS] = update_position.x;
	current_position[Y_AXIS] = update_position.y;
	current_position[Z_AXIS] = update_position.z;

	if ((change_filament == false) || (current_position[Z_AXIS] < POSITION_FILAMENT_Z))
	{
		current_position[Z_AXIS] = POSITION_FILAMENT_Z;
		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 60, active_extruder);
		st_synchronize();
	}

	current_position[X_AXIS] = POSITION_FILAMENT_X;
	current_position[Y_AXIS] = POSITION_FILAMENT_Y;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 100, active_extruder);

	st_synchronize();
}

float action_get_height()
{
	return z_height;
}

void action_start_print()
{
	temp::TemperatureManager::single::instance().setBlowerControlState(false);

	bool serial_printing = true;

	char cmd[LONG_FILENAME_LENGTH];
	char* c;
	if(PrintManager::single::instance().state() == PRINTING)
	{
		serial_printing = false;
		
		sprintf_P(cmd, PSTR("M23 %s"), card.filename);
	}

	fanSpeed = PREHEAT_FAN_SPEED;

#ifdef DOGLCD
	PrintManager::single::instance().state(HOMING);
#endif // DOGLCD
	action_homing();
	
#ifdef BED_DETECTION
	if(PrintManager::single::instance().getBedMissingFlag() == true)
	{
		PrintManager::single::instance().state(STOPPED);
		return;
	}
#endif // BED_DETECTION

	if (bed_leveling == true || (bed_leveling == false && AutoLevelManager::single::instance().state() == true))
	{
#ifdef DOGLCD
		PrintManager::single::instance().state(LEVELING);
#endif //DOGLCD
		action_get_plane();
	}

	action_move_to_rest();

	if(serial_printing == false)
	{
		for(c = &cmd[4]; *c; c++)
		*c = tolower(*c);
		enquecommand(cmd);
	}

#ifdef DOGLCD
		PrintManager::single::instance().state(READY);
		StatsManager::single::instance().increaseTotalPrints();
#endif //DOGLCD

#ifdef FAN_BOX_PIN
	if(FanManager::single::instance().state() == true)
	{
		digitalWrite(FAN_BOX_PIN, HIGH);
	}
#endif //FAN_BOX_PIN

	enquecommand_P(PSTR("G90"));
	enquecommand_P(PSTR("G92 E0"));
	enquecommand_P(PSTR("G1 F1800")); // sets slow initial feedrate

	if(serial_printing == false)
	{
		enquecommand_P(PSTR("M24"));
	}
}

void action_stop_print()
{
	plan_bed_level_matrix.set_to_identity();

	uint8_t num_ok = flush_commands();
	stop_planner_buffer = true;
	quickStop();

	temp::TemperatureManager::single::instance().setBlowerControlState(true);
#ifdef FAN_BOX_PIN
	digitalWrite(FAN_BOX_PIN, LOW);
#endif //FAN_BOX_PIN

	if(card.isFileOpen() == true)
	{
		SERIAL_PROTOCOLLNPGM(MSG_FILE_PRINTED);
		card.sdprinting = false;
		card.closefile();
	}

	set_relative_mode(false);

	plan_reset_position();

	current_position[X_AXIS] = plan_get_axis_position(X_AXIS);
	current_position[Y_AXIS] = plan_get_axis_position(Y_AXIS);
	current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS);
	current_position[E_AXIS] = plan_get_axis_position(E_AXIS);

	target[E_AXIS] = current_position[E_AXIS] - RETRACT_ON_PAUSE;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], target[E_AXIS], max_feedrate[E_AXIS], active_extruder);
	st_synchronize();

	target[Z_AXIS] = current_position[Z_AXIS] + 10;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], target[Z_AXIS], target[E_AXIS], max_feedrate[Z_AXIS], active_extruder);
	st_synchronize();

	current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS);
	current_position[E_AXIS] = plan_get_axis_position(E_AXIS);

	target[X_AXIS] = POSITION_REST_X;
	target[Y_AXIS] = POSITION_REST_Y;

	action_preheat();

	flush_commands();
	quickStop();


#if X_MAX_POS < 250
	target[Z_AXIS] += 20;
#else // X_MAX_POS < 250*/
	target[Z_AXIS] = Z_MAX_POS;
#endif // X_MAX_POS < 250

	if (current_position[Z_AXIS] > Z_MAX_POS)
	{
		target[Z_AXIS] = Z_MAX_POS;
	}
	enable_endstops(true);

	plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
	st_synchronize();

	action_correct_movement(target[X_AXIS], target[Y_AXIS], target[Z_AXIS]);

	plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
	st_synchronize();

	action_correct_movement(target[X_AXIS], target[Y_AXIS], target[Z_AXIS]);

	plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
	st_synchronize();

	enable_endstops(false);

	current_position[X_AXIS] = plan_get_axis_position(X_AXIS);
	current_position[Y_AXIS] = plan_get_axis_position(Y_AXIS);
	current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS);
	current_position[E_AXIS] = plan_get_axis_position(E_AXIS);

	if (SD_FINISHED_STEPPERRELEASE)
	{
		if(PrintManager::single::instance().state() == SERIAL_CONTROL)
		{
			enquecommand_P(PSTR("M801"));
		}
		enquecommand_P(PSTR(SD_FINISHED_RELEASECOMMAND));
	}
	// autotempShutdown();
	
#if HEATER_BED_PIN > -1
	temp::TemperatureManager::single::instance().setBedTargetTemperature(0);
#endif // HEATER_BED_PIN > -1

	cancel_heatup = true;

	PrintManager::knownPosition(true);
	
	stop_buffer = false;
	
	#ifdef DOGLCD
		Time_t printTime = PrintManager::single::instance().printingTime();
		StatsManager::single::instance().updateTotalTime(printTime);
	#endif
	
	#ifdef FAN_BOX_PIN
		if(FanManager::single::instance().state() == true)
		{
			digitalWrite(FAN_BOX_PIN, LOW);
		}
	#endif //FAN_BOX_PIN
	
	for(int i=0; i != num_ok; i++)
	{
		SERIAL_PROTOCOLLNPGM(MSG_OK);
	}
}

void action_finish_print()
{
	action_stop_print();
	action_cooldown();
	#ifdef DOGLCD
		StatsManager::single::instance().increaseSuccededPrints();
	#endif
}

extern float target[4];
extern float lastpos[4];

void action_pause_print()
{
	lcd_disable_button();
	change_filament = true;
	card.sdprinting = false;
	stop_buffer = true;
	stop_buffer_code = 1;
}

void action_resume_print()
{
	lcd_disable_button();

	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 100, active_extruder); //move xy back
	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], current_position[E_AXIS], 60, active_extruder); //move z back

	st_synchronize();

	vector_3 update_position = plan_get_position();
	current_position[X_AXIS] = update_position.x;
	current_position[Y_AXIS] = update_position.y;
	current_position[Z_AXIS] = update_position.z;

	current_position[E_AXIS] += EXTRUDE_ON_RESUME;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 25, active_extruder);
	st_synchronize();

	current_position[E_AXIS] = lastpos[E_AXIS];
	plan_set_e_position(lastpos[E_AXIS]);

	card.sdprinting = true;
	lcd_enable_button();
	stop_buffer = false;
	change_filament = false;
}

void action_set_feedrate_multiply(uint16_t value)
{
	feedmultiply = value;
}

uint16_t action_get_feedrate_multiply()
{
	return feedmultiply;
}

#ifdef LEVEL_SENSOR
void action_offset()
{
	st_synchronize();
	// make sure the bed_level_rotation_matrix is identity or the planner will get it incorectly
	plan_bed_level_matrix.set_to_identity();
	vector_3 uncorrected_position = plan_get_position();
	current_position[X_AXIS] = uncorrected_position.x;
	current_position[Y_AXIS] = uncorrected_position.y;
	current_position[Z_AXIS] = 0;
	z_saved_homing = OffsetManager::single::instance().getOffset();

	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
	setup_for_endstop_move();

	feedrate = homing_feedrate[Z_AXIS];

	// Probe at 3 arbitrary points
	// probe 1
	float z_at_pt_1 = probe_pt(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, Z_RAISE_BEFORE_PROBING);
	// probe 2
	float z_at_pt_2 = probe_pt(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS);
	// probe 3
	float z_at_pt_3 = probe_pt(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS);

	clean_up_after_endstop_move(); //Dissable endstops

	plan_bed_level_matrix.set_to_identity();

	vector_3 pt1 = vector_3(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, z_at_pt_1);
	vector_3 pt2 = vector_3(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, z_at_pt_2);
	vector_3 pt3 = vector_3(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, z_at_pt_3);

	vector_3 from_2_to_1 = (pt1 - pt2).get_normal();
	vector_3 from_3_to_2 = (pt2 - pt3).get_normal();
	vector_3 planeNormal = vector_3::cross(from_2_to_1, from_3_to_2).get_normal();

	planeNormal = vector_3(planeNormal.x, planeNormal.y, abs(planeNormal.z));

	current_position[X_AXIS] = Z_SAFE_HOMING_X_POINT;
	current_position[Y_AXIS] = Z_SAFE_HOMING_Y_POINT;
	do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], Z_RAISE_BETWEEN_PROBINGS);

	plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);
	vector_3 vector_offsets = vector_3(X_PROBE_OFFSET_FROM_EXTRUDER, Y_PROBE_OFFSET_FROM_EXTRUDER, 0);
	
	apply_rotation_xyz(plan_bed_level_matrix, vector_offsets.x, vector_offsets.y, vector_offsets.z);
	z_offset = vector_offsets.z;

	plan_bed_level_matrix.set_to_identity();

	if(z_saved_homing == 0)
	{
		current_position[Z_AXIS] = 0;
	}
	else
	{
		current_position[Z_AXIS] = -(z_saved_homing - z_offset);
	}
	do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);

	current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS);
}
#endif //LEVEL_SENSOR

void action_offset_homing()
{
	zprobe_zoffset = Z_PROBE_OFFSET_FROM_EXTRUDER;
	action_homing();
}

void action_set_offset(uint8_t axis, float value)
{
	if(z_saved_homing == 0)
	{
		action_move_axis_to(Z_AXIS,-value);
		zprobe_zoffset = value + z_offset;
	}
	else
	{
		action_move_axis_to(Z_AXIS,-(value + z_saved_homing - z_offset));
		zprobe_zoffset = value + z_saved_homing;
	}
	OffsetManager::single::instance().offset(zprobe_zoffset);
}

void action_offset_rest()
{
	action_z_homing();
	action_move_to_rest();
}

void action_save_offset()
{
	OffsetManager::single::instance().saveOffset();
}

void action_wizard_init()
{
	PrintManager::single::instance().state(INITIALIZING);
	LightManager::single::instance().state(true);
	SerialManager::single::instance().state(true);
}

void action_wizard_finish()
{
	//Set printer as initialized
	eeprom::StorageManager::single::instance().setInitialized();

	//Set default values
	PrintManager::single::instance().state(STOPPED);
	AutoLevelManager::single::instance().state(true);

	//Reset inactivity
	PrintManager::resetInactivity();
}

bool action_check_preheat_temp()
{
	temp::TemperatureManager::single::instance().setTargetTemperature(PREHEAT_HOTEND_TEMP);
	if(temp::TemperatureManager::single::instance().getCurrentTemperature() >= PREHEAT_HOTEND_TEMP)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool action_check_cooling()
{
	if(temp::TemperatureManager::single::instance().getTargetTemperature() <= temp::TemperatureManager::single::instance().getCurrentTemperature())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void action_close_inactivity()
{
	PrintManager::single::instance().state(STOPPED);
}

void action_erase_EEPROM()
{
	lcd_disable_button();
	eeprom::StorageManager::eraseEEPROM();
	RESET();
}

bool action_check_wizard()
{
	if( (eeprom::StorageManager::single::instance().getBoardType() == BOARD_BQ_CNC) && 
			(eeprom::StorageManager::single::instance().getOffset() != Z_PROBE_OFFSET_FROM_EXTRUDER) )
	{
		return true;
	}
	return false;
}

void action_reset_wizard()
{
	eeprom::StorageManager::single::instance().setUninitialized();
}

bool action_check_auto_gcode()
{
	if(card.checkAutoFile() == true)
	{
		allow_home = true;
		return true;
	}
	return false;
}

bool action_check_pause_state()
{
	if(PrintManager::single::instance().state() == PrinterState_t::PAUSED)
	{
		return true;
	}
	return false;
}
