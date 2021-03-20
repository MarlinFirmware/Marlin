#pragma once
#include "../inc/MarlinConfig.h"

#ifdef LONGER3D
typedef struct _ARGUMENTS
{
	float acceleration;
	float max_xy_jerk;
	float max_z_jerk;
	float max_e_jerk;
	float max_feedrate[NUM_AXIS];
	float minimumfeedrate;
	float mintravelfeedrate;
	unsigned long max_acceleration_units_per_sq_second[NUM_AXIS];
	float retract_acceleration;
	float axis_steps_per_unit[NUM_AXIS];
	bool is_filament_check_disable;
	bool file_order;
//	char flash_version[4]; 
}ARGUMENTS; 
class config_storedata
{
private:
    /* data */
public:
    config_storedata(/* args */);
    ~config_storedata();
    void calcAccelRates(void);
    void restoreDefaultSettings(void);
    void settingsLoad();
    void settingsReset();
};
#endif