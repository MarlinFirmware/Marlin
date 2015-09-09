#include "GuiAction.h"

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

bool raised = false;
extern bool home_all_axis;
extern bool bed_leveling;
extern const char axis_codes[NUM_AXIS];
extern bool cancel_heatup;
extern bool stop_planner_buffer;
extern bool stop_buffer;
bool change_filament = false; 
extern uint16_t stop_buffer_code;

static float manual_feedrate[] = MANUAL_FEEDRATE;



float z_offset;

extern float current_position[NUM_AXIS];
extern void clean_up_after_endstop_move();
extern void do_blocking_move_to(float x, float y, float z);
extern void setup_for_endstop_move();
extern float probe_pt(float x, float y, float z_before, int retract_action = 0);

void action_set_temperature(uint16_t degrees)
{
	TemperatureManager::single::instance().setTargetTemperature(degrees);
}

void action_preheat()
{
	TemperatureManager::single::instance().setTargetTemperature(PREHEAT_HOTEND_TEMP);
}

void action_cooldown()
{
	TemperatureManager::single::instance().setTargetTemperature(30);
}

void action_filament_unload()
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

	current_position[E_AXIS] += 50.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 5, active_extruder);
	st_synchronize();

	current_position[E_AXIS] -= 60.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 5, active_extruder);
	st_synchronize();

	current_position[E_AXIS] = lastpos[E_AXIS];
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}

void action_filament_load()
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

	current_position[E_AXIS] += 140.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 5, active_extruder);
	st_synchronize();

	current_position[E_AXIS] = lastpos[E_AXIS];
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}

void action_level_plate()
{
	static uint8_t level_plate_step = 0;

	#ifndef ABL_PROBE_PT_4_X
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

			target[X_AXIS] = ABL_PROBE_PT_1_X;
			target[Y_AXIS] = ABL_PROBE_PT_1_Y;
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

			target[X_AXIS] = ABL_PROBE_PT_2_X;
			target[Y_AXIS] = ABL_PROBE_PT_2_Y;
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

			target[X_AXIS] = ABL_PROBE_PT_3_X;
			target[Y_AXIS] = ABL_PROBE_PT_3_Y;
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

			target[Z_AXIS] = 0 + OffsetManager::single::instance().offset();
			plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
			st_synchronize();

			lcd_enable_button();

			break;

		case 3:
			#ifdef ABL_PROBE_PT_4_X
				lcd_disable_button();

				target[X_AXIS] = plan_get_axis_position(X_AXIS);
				target[Y_AXIS] = plan_get_axis_position(Y_AXIS);
				target[Z_AXIS] = plan_get_axis_position(Z_AXIS);
				target[E_AXIS] = plan_get_axis_position(E_AXIS);

				plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

				target[Z_AXIS] = 10;
				plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

				target[X_AXIS] = ABL_PROBE_PT_4_X;
				target[Y_AXIS] = ABL_PROBE_PT_4_Y;
				plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

				target[Z_AXIS] = 0 + OffsetManager::single::instance().offset();;
				plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);
				st_synchronize();

				lcd_enable_button();
			#endif // ABL_PROBE_PT_4_X

			break;

		case 4:
			lcd_disable_button();
			action_move_to_rest();
			lcd_enable_button();

			break;
	}

	level_plate_step = ++level_plate_step % max_steps;
}

void gui_action_homing()
{
	action_homing();
	action_move_to_rest();
}

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

void action_move_axis_to(uint8_t axis, float position)
{
	current_position[axis] = position;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
}

void action_move_to_rest()
{
	st_synchronize();
	current_position[X_AXIS] = plan_get_axis_position(X_AXIS);
	current_position[Y_AXIS] = plan_get_axis_position(Y_AXIS);
	current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS);

	if (current_position[Z_AXIS] < 20)
	{
		current_position[Z_AXIS] = 20;
		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 100, active_extruder);
		st_synchronize();
	}

	current_position[X_AXIS] = POSITION_REST_X;
	current_position[Y_AXIS] = POSITION_REST_Y;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 100, active_extruder);
	st_synchronize();

	current_position[Z_AXIS] = POSITION_REST_Z;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 60, active_extruder);
	st_synchronize();
}

void action_start_print()
{
	char cmd[30];
	char* c;
	strcpy(cmd, card.longFilename);
	for (c = &cmd[0]; *c; c++)
	{
		if ((uint8_t)*c > 127)
		{
			SERIAL_ECHOLN(MSG_SD_BAD_FILENAME);
			return;
		}
	}
	fanSpeed = PREHEAT_FAN_SPEED;
	sprintf_P(cmd, PSTR("M23 %s"), card.filename);

#ifdef DOGLCD
	PrintManager::single::instance().state(HOMING);
#endif // DOGLCD
	action_homing();

	if (bed_leveling == true || (bed_leveling == false && AutoLevelManager::single::instance().state() == true))
	{
#ifdef DOGLCD
		PrintManager::single::instance().state(LEVELING);
#endif //DOGLCD
		action_get_plane();
	}
	enquecommand_P(PSTR("G1 Z10"));

	for(c = &cmd[4]; *c; c++)
	*c = tolower(*c);
	enquecommand(cmd);

#ifdef DOGLCD
		PrintManager::single::instance().state(READY);
#endif //DOGLCD

	enquecommand_P(PSTR("M24"));
}

void action_stop_print()
{
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

	current_position[X_AXIS] = POSITION_REST_X;
	current_position[Y_AXIS] = POSITION_REST_Y;

#if X_MAX_POS < 250
	current_position[Z_AXIS] += 20;
#else // X_MAX_POS < 250
	current_position[Z_AXIS] = Z_MAX_POS;
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
	stop_planner_buffer = true;
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

	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], current_position[Z_AXIS], lastpos[E_AXIS], 100, active_extruder); //move xy back
	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], lastpos[E_AXIS], 60, active_extruder); //move z back
	lastpos[E_AXIS] += EXTRUDE_ON_RESUME;
	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], lastpos[E_AXIS], 10, active_extruder); //extrude on resume
	st_synchronize();

	vector_3 update_position = plan_get_position();
	current_position[X_AXIS] = update_position.x;
	current_position[Y_AXIS] = update_position.y;
	current_position[Z_AXIS] = update_position.z;

	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

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

void action_offset()
{
	st_synchronize();
	// make sure the bed_level_rotation_matrix is identity or the planner will get it incorectly
	plan_bed_level_matrix.set_to_identity();
	vector_3 uncorrected_position = plan_get_position();
	current_position[X_AXIS] = uncorrected_position.x;
	current_position[Y_AXIS] = uncorrected_position.y;
	current_position[Z_AXIS] = 0;
	float z_saved_homing = current_position[Z_AXIS];
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

	current_position[Z_AXIS] = z_saved_homing;
	do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);

	plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);
	vector_3 vector_offsets = vector_3(X_PROBE_OFFSET_FROM_EXTRUDER, Y_PROBE_OFFSET_FROM_EXTRUDER, 0);
	
	apply_rotation_xyz(plan_bed_level_matrix, vector_offsets.x, vector_offsets.y, vector_offsets.z);
	z_offset = vector_offsets.z;

	plan_bed_level_matrix.set_to_identity();
	current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS);
}

void action_offset_homing()
{
	zprobe_zoffset = Z_PROBE_OFFSET_FROM_EXTRUDER;
	action_homing();
}

void action_set_offset(uint8_t axis, float value)
{
	action_move_axis_to(Z_AXIS,-value);
	zprobe_zoffset = value + z_offset;
	OffsetManager::single::instance().offset(zprobe_zoffset);
}

void action_save_offset()
{
	OffsetManager::single::instance().saveOffset();
	if(!OffsetManager::single::instance().isOffsetOnEEPROM())
	{
		OffsetManager::single::instance().offsetOnEEPROM();
	}

	action_move_to_rest();
}

bool action_check_preheat_temp()
{
	if(TemperatureManager::single::instance().getTargetTemperature() >= PREHEAT_HOTEND_TEMP)
	{
		return true;
	}
	else
	{
		return false;
	}
}