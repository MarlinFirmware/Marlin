#include "GuiAction.h"

#include "Marlin.h"
#include "cardreader.h"
#include "ConfigurationStore.h"
#include "planner.h"
#include "stepper.h"
#include "language.h"

#include "GuiManager.h"
#include "TemperatureManager.h"
#include "OffsetManager.h"
#include "AutoLevelManager.h"

extern bool cancel_heatup;
extern bool stop_planner_buffer;
extern bool stop_buffer;
bool change_filament = false; 
extern int stop_buffer_code;

static float manual_feedrate[] = MANUAL_FEEDRATE;

extern unsigned long previous_millis_cmd;

#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)

void action_set_temperature(uint16_t degrees)
{
	TemperatureManager::single::instance().setTargetTemperature(degrees);
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
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

	if (current_position[Z_AXIS] < (Z_MIN_POS+30) && change_filament == false)
	{
		#ifdef DISABLE_MAX_ENDSTOPS
			current_position[Z_AXIS] += 10;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 300/60, active_extruder);
			st_synchronize();
		#else
			current_position[Z_AXIS] += 30;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 300/60, active_extruder);
			st_synchronize();
		#endif

	}
	current_position[E_AXIS] += 50.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 300/60, active_extruder);
	st_synchronize();

	current_position[E_AXIS] -= 60.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 300/60, active_extruder);
	st_synchronize();
}

void action_filament_load()
{
	st_synchronize();
	vector_3 update_position = plan_get_position();
	current_position[X_AXIS] = update_position.x;
	current_position[Y_AXIS] = update_position.y;
	current_position[Z_AXIS] = update_position.z;
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

	if (current_position[Z_AXIS] < (Z_MIN_POS+30) && change_filament == false)
	{
		#ifdef DISABLE_MAX_ENDSTOPS
			current_position[Z_AXIS] += 10;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 300/60, active_extruder);
			st_synchronize();
		#else
			current_position[Z_AXIS] += 30;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 300/60, active_extruder);
			st_synchronize();
		#endif

	}

	current_position[E_AXIS] += 100.0;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],current_position[E_AXIS], 300/60, active_extruder);
	st_synchronize();
	current_position[E_AXIS] = lastpos[E_AXIS];

	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
	
	change_filament = false;
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

			target[X_AXIS] = ABL_PROBE_PT_1_X;
			target[Y_AXIS] = ABL_PROBE_PT_1_Y;
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

			target[X_AXIS] = ABL_PROBE_PT_2_X;
			target[Y_AXIS] = ABL_PROBE_PT_2_Y;
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

			target[X_AXIS] = ABL_PROBE_PT_3_X;
			target[Y_AXIS] = ABL_PROBE_PT_3_Y;
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

			target[X_AXIS] = Z_SAFE_HOMING_X_POINT;
			target[Y_AXIS] = Z_SAFE_HOMING_Y_POINT;
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

			target[X_AXIS] = X_MAX_POS;
			target[Y_AXIS] = Y_MAX_POS;
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
	lcd_disable_interrupt();

#ifdef Z_SAFE_HOMING
	plan_bed_level_matrix.set_to_identity();
#endif //Z_SAFE_HOMING

	float saved_feedrate = feedrate;
	int saved_feedmultiply = feedmultiply;
	feedmultiply = 100;

	enable_endstops(true);

#ifdef Z_SAFE_HOMING
	for(int8_t i=0; i < NUM_AXIS; i++)
	{
		destination[i] = plan_get_axis_position(i);
	}

	destination[Z_AXIS] += 5;
	feedrate = max_feedrate[Z_AXIS];

	plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
	st_synchronize();
#endif

	for(int8_t i=0; i < NUM_AXIS; i++)
	{
		destination[i] = current_position[i];
	}

	feedrate = 0.0;

#if Z_HOME_DIR > 0
	HOMEAXIS(Z);
#endif //Z_HOME_DIR > 0

#ifdef QUICK_HOME
	current_position[X_AXIS] = 0;current_position[Y_AXIS] = 0;

#	ifndef DUAL_X_CARRIAGE
	int x_axis_home_dir = home_dir(X_AXIS);
#	else //DUAL_X_CARRIAGE
	int x_axis_home_dir = x_home_dir(active_extruder);
	extruder_duplication_enabled = false;
#	endif //DUAL_X_CARRIAGE

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

#endif //QUICK_HOME

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

	HOMEAXIS(Y);

#if Z_HOME_DIR < 0
#	ifndef Z_SAFE_HOMING
#		if defined (Z_RAISE_BEFORE_HOMING) && (Z_RAISE_BEFORE_HOMING > 0)
	destination[Z_AXIS] = Z_RAISE_BEFORE_HOMING * home_dir(Z_AXIS) * (-1);    // Set destination away from bed
	feedrate = max_feedrate[Z_AXIS];
	plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate, active_extruder);
	st_synchronize();
#		endif //defined (Z_RAISE_BEFORE_HOMING) && (Z_RAISE_BEFORE_HOMING > 0)
	HOMEAXIS(Z);
#	else //Z_SAFE_HOMING
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

#	endif //Z_SAFE_HOMING
#endif //Z_HOME_DIR < 0

#ifdef Z_SAFE_HOMING
	current_position[Z_AXIS] += zprobe_zoffset;
#endif //Z_SAFE_HOMING
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

#ifdef ENDSTOPS_ONLY_FOR_HOMING
	enable_endstops(false);
#endif //ENDSTOPS_ONLY_FOR_HOMING

	feedrate = saved_feedrate;
	feedmultiply = saved_feedmultiply;
	endstops_hit_on_purpose();

	previous_millis_cmd = millis();
	lcd_enable_interrupt();
}

void action_move_axis_to(uint8_t axis, float position)
{
	current_position[axis] = position;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
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
	TemperatureManager::single::instance().setTargetTemperature(200);
	fanSpeed = PREHEAT_FAN_SPEED;
	sprintf_P(cmd, PSTR("M23 %s"), card.filename);
	enquecommand_P(PSTR("G28"));
	#ifdef ENABLE_AUTO_BED_LEVELING
		if (AutoLevelManager::single::instance().state())
		{
			enquecommand_P(PSTR("G29"));
		}
	#endif
	enquecommand_P(PSTR("G1 Z10"));
	for(c = &cmd[4]; *c; c++)
	*c = tolower(*c);
	enquecommand(cmd);
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

	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate/60, active_extruder); //move xy back
	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], current_position[E_AXIS], feedrate/60, active_extruder); //move z back
	plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], lastpos[E_AXIS], feedrate/60, active_extruder); //final untretract
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

float z_saved_homing;
vector_3 planeNormal;
vector_3 vector_offsets; 

extern float current_position[NUM_AXIS];
extern void clean_up_after_endstop_move();
extern void do_blocking_move_to(float x, float y, float z);
extern void setup_for_endstop_move();
extern float probe_pt(float x, float y, float z_before, int retract_action = 0);

void action_offset()
{
	st_synchronize();
	// make sure the bed_level_rotation_matrix is identity or the planner will get it incorectly
	plan_bed_level_matrix.set_to_identity();
	vector_3 uncorrected_position = plan_get_position();
	current_position[X_AXIS] = uncorrected_position.x;
	current_position[Y_AXIS] = uncorrected_position.y;
	current_position[Z_AXIS] = 0;
	z_saved_homing = current_position[Z_AXIS];
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
	planeNormal = vector_3::cross(from_2_to_1, from_3_to_2).get_normal();

	planeNormal = vector_3(planeNormal.x, planeNormal.y, abs(planeNormal.z));

	current_position[X_AXIS] = Z_SAFE_HOMING_X_POINT;
	current_position[Y_AXIS] = Z_SAFE_HOMING_Y_POINT;
	do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], Z_RAISE_BETWEEN_PROBINGS);

	current_position[Z_AXIS] = z_saved_homing;
	do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS]);

	plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);
	vector_offsets = vector_3(X_PROBE_OFFSET_FROM_EXTRUDER, Y_PROBE_OFFSET_FROM_EXTRUDER, 0);
	
	apply_rotation_xyz(plan_bed_level_matrix, vector_offsets.x, vector_offsets.y, vector_offsets.z);
	plan_bed_level_matrix.set_to_identity();
	current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS);
}

void action_offset_homing()
{
	zprobe_zoffset = -Z_PROBE_OFFSET_FROM_EXTRUDER;
	action_homing();
}

void action_set_offset(uint8_t axis, float value)
{
	action_move_axis_to(Z_AXIS,-value);
	zprobe_zoffset = value + vector_offsets.z;
	OffsetManager::single::instance().offset(zprobe_zoffset);
}

void action_save_offset()
{
	OffsetManager::single::instance().saveOffset();
	if(!OffsetManager::single::instance().isOffsetOnEEPROM())
	{
		OffsetManager::single::instance().offsetOnEEPROM();
	}
	do_blocking_move_to(0, current_position[Y_AXIS], 50);
}
