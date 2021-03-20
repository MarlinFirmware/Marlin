#include "config_storedata.h"
#ifdef LONGER3D
#include "LGT_MACRO.h"
#include "../../module/planner.h"

extern Planner planner;
ARGUMENTS re_settings;
bool check_filament_disable=false;
bool list_order=false;
config_storedata::config_storedata(/* args */)
{
}

config_storedata::~config_storedata()
{
}

void config_storedata::calcAccelRates(void)
{
    int i;
	for(i = 0; i < NUM_AXIS; i++)
    {
		//axis_steps_per_sqr_second[i] = max_acceleration_units_per_sq_second[i] * axis_steps_per_unit[i];
       // planner.settings.max_acceleration_mm_per_s2=re_settings.max_acceleration_units_per_sq_second[i]*re_settings.axis_steps_per_unit[i];
      planner.settings.max_acceleration_mm_per_s2[i]=re_settings.max_acceleration_units_per_sq_second[i];
      planner.settings.axis_steps_per_mm[i]=re_settings.axis_steps_per_unit[i];
      planner.refresh_positioning();
	}
}
void config_storedata::restoreDefaultSettings(void)
{
    uint8_t i;
    float tmp1[]=DEFAULT_AXIS_STEPS_PER_UNIT;
    float tmp2[]=DEFAULT_MAX_FEEDRATE;
    long tmp3[]=DEFAULT_MAX_ACCELERATION;
    for (i=0;i<NUM_AXIS;i++) 
    {
        re_settings.axis_steps_per_unit[i]=tmp1[i];  
        re_settings.max_feedrate[i]=tmp2[i];  
        re_settings.max_acceleration_units_per_sq_second[i]=tmp3[i];
    }
	calcAccelRates();
	re_settings.acceleration=DEFAULT_ACCELERATION;
	re_settings.max_xy_jerk=DEFAULT_XJERK;     //DEFAULT_XJERK=DEFAULT_YJERK
    re_settings.max_z_jerk=DEFAULT_ZJERK;
    re_settings.max_e_jerk=DEFAULT_EJERK;
    re_settings.minimumfeedrate=DEFAULT_MINIMUMFEEDRATE;
	re_settings.mintravelfeedrate=DEFAULT_MINTRAVELFEEDRATE;
	re_settings.retract_acceleration=DEFAULT_RETRACT_ACCELERATION;	
	re_settings.is_filament_check_disable = DEFAULT_FILAMENT_CHECK_DISABLE;
    re_settings.file_order = DEFAULT_FILE_LIST_ORDER;
}
void config_storedata::settingsLoad()
{
    memset(&re_settings,0,sizeof(re_settings));
    FLASH_READ_VAR(SAVE_ADDR_SETTINGS, re_settings);
    settingsReset();
    // Serial1.print("a=");
    // Serial1.println(re_settings.acceleration);
}
void config_storedata::settingsReset()
{
    LOOP_XYZE_N(i) 
    {
        planner.settings.axis_steps_per_mm[i]          = re_settings.axis_steps_per_unit[i];
        planner.settings.max_feedrate_mm_s[i]          = re_settings.max_feedrate[i];
        planner.settings.max_acceleration_mm_per_s2[i] = re_settings.max_acceleration_units_per_sq_second[i];
    }
    planner.settings.acceleration = re_settings.acceleration;
    planner.settings.retract_acceleration = re_settings.retract_acceleration;;
    planner.settings.min_feedrate_mm_s =   re_settings.minimumfeedrate;
    planner.settings.min_travel_feedrate_mm_s = re_settings.mintravelfeedrate;
    planner.max_jerk[X_AXIS] = re_settings.max_xy_jerk;
    planner.max_jerk[Y_AXIS] = re_settings.max_xy_jerk;
    planner.max_jerk[Z_AXIS] =  re_settings.max_z_jerk;
    #if DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE)
      planner.max_jerk[E_AXIS] =  re_settings.max_e_jerk;
    #endif
    list_order=re_settings.file_order;
    check_filament_disable=re_settings.is_filament_check_disable;
    calcAccelRates();
}
#endif