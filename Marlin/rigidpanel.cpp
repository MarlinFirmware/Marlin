#include "temperature.h"
#include "rigidpanel.h"

#ifdef RIGIDPANEL

#include "Marlin.h"
#include "language.h"
#include "cardreader.h"
#include "temperature.h"
#include "stepper.h"
#include "ConfigurationStore.h"

bool lcdFastUpdate = false;					//	Used for higher update scrolling of SD card filenames
bool timeoutSuspend = false;				//	Used to disable tomeout to status screen (for leveling & filament change, etc.)
bool invertEncoderDir = false;				//	Used to invert encoder direction for menu navigation when used with up/down buttons
bool encoderCoarseEnabled = false;				//	Used to invert encoder direction for menu navigation when used with up/down buttons

/* Configuration settings */
int plaPreheatHotendTemp;
int plaPreheatHPBTemp;
int plaPreheatFanSpeed;

int absPreheatHotendTemp;
int absPreheatHPBTemp;
int absPreheatFanSpeed;
/* !Configuration settings */

//Function pointer to menu functions.
typedef void (*menuFunc_t)();

uint8_t lcd_status_message_level;
char lcd_status_message[LCD_WIDTH+1] = WELCOME_MSG;

#include "ultralcd_implementation_RigidPanel.h"

/** forward declerations **/

void copy_and_scalePID_i();
void copy_and_scalePID_d();

/* Different menus */
static void lcd_status_screen();
static void lcd_main_menu();
static void lcd_tune_menu();
static void lcd_prepare_menu();
static void lcd_heat_cool_menu();
static void lcd_bed_level_menu();
static void lcd_move_menu();
static void lcd_control_menu();
static void lcd_control_temperature_menu();
static void lcd_control_temperature_preheat_pla_settings_menu();
static void lcd_control_temperature_preheat_abs_settings_menu();
static void lcd_control_motion_menu();
static void lcd_control_retract_menu();
static void lcd_utilities_menu();
static void lcd_sdcard_menu();

static void lcd_quick_feedback();//Cause an LCD refresh, and give the user visual or audiable feedback that something has happend

/* Different types of actions that can be used in menuitems. */
static void menu_action_back(menuFunc_t data);
static void menu_action_submenu(menuFunc_t data);
static void menu_action_gcode(const char* pgcode);
static void menu_action_function(menuFunc_t data);
static void menu_action_sdfile(const char* filename, char* longFilename, uint8_t item);
static void menu_action_sddirectory(const char* filename, char* longFilename, uint8_t item);
static void menu_action_setting_edit_bool(const char* pstr, bool* ptr);
static void menu_action_setting_edit_int3(const char* pstr, int* ptr, int minValue, int maxValue);
static void menu_action_setting_edit_float3(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_float32(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_float5(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_float51(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_float52(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue);
static void menu_action_setting_edit_callback_bool(const char* pstr, bool* ptr, menuFunc_t callbackFunc);
static void menu_action_setting_edit_callback_int3(const char* pstr, int* ptr, int minValue, int maxValue, menuFunc_t callbackFunc);
static void menu_action_setting_edit_callback_float3(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
static void menu_action_setting_edit_callback_float32(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
static void menu_action_setting_edit_callback_float5(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
static void menu_action_setting_edit_callback_float51(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
static void menu_action_setting_edit_callback_float52(const char* pstr, float* ptr, float minValue, float maxValue, menuFunc_t callbackFunc);
static void menu_action_setting_edit_callback_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue, menuFunc_t callbackFunc);

//#define ENCODER_FEEDRATE_DEADZONE 10
#define ENCODER_FEEDRATE_DEADZONE 0
#define ENCODER_STEPS_PER_MENU_ITEM  1

/* Helper macros for menus */
#define START_MENU() do { \
	invertEncoderDir = true; \
    if (encoderPosition > 0x8000) encoderPosition = 0; \
    if (encoderPosition / ENCODER_STEPS_PER_MENU_ITEM < currentMenuViewOffset) currentMenuViewOffset = encoderPosition / ENCODER_STEPS_PER_MENU_ITEM;\
    uint8_t _lineNr = currentMenuViewOffset, _menuItemNr; \
    for(uint8_t _drawLineNr = 0; _drawLineNr < LCD_HEIGHT; _drawLineNr++, _lineNr++) { \
        _menuItemNr = 0;
#define MENU_ITEM(type, label, args...) do { \
    if (_menuItemNr == _lineNr) { \
		if (lcdDrawUpdate) { \
            const char* _label_pstr = PSTR(label); \
            if ((encoderPosition / ENCODER_STEPS_PER_MENU_ITEM) == _menuItemNr) { \
                lcd_implementation_drawmenu_ ## type ## _selected (_drawLineNr, _label_pstr , ## args ); \
            }else{\
                lcd_implementation_drawmenu_ ## type (_drawLineNr, _label_pstr , ## args ); \
            }\
        }\
        if ((LCD_CLICKED || buttons&B_RI) && (encoderPosition / ENCODER_STEPS_PER_MENU_ITEM) == _menuItemNr) {\
            lcd_quick_feedback(); \
            menu_action_ ## type ( args ); \
            return;\
        }\
    }\
    _menuItemNr++;\
} while(0)
#define MENU_ITEM_BACK_HIDDEN(type, label, args...) do { \
	if ( buttons&B_LE ) { \
		lcd_quick_feedback(); \
		menu_action_ ## type ( args ); \
		return;\
	} \
} while(0)
#ifdef HIDE_BACK_MENUS
#define MENU_ITEM_BACK	MENU_ITEM_BACK_HIDDEN
#else
#define MENU_ITEM_BACK(type, label, args...) do { \
	MENU_ITEM_BACK_HIDDEN(type, label, ## args); \
	MENU_ITEM(type, label, ## args); \
} while(0)
#endif
#define MENU_ITEM_DUMMY() do { _menuItemNr++; } while(0)
#define MENU_ITEM_EDIT(type, label, args...) MENU_ITEM(setting_edit_ ## type, label, PSTR(label) , ## args )
#define MENU_ITEM_EDIT_CALLBACK(type, label, args...) MENU_ITEM(setting_edit_callback_ ## type, label, PSTR(label) , ## args )
#define END_MENU() \
    if (encoderPosition / ENCODER_STEPS_PER_MENU_ITEM >= _menuItemNr) encoderPosition = _menuItemNr * ENCODER_STEPS_PER_MENU_ITEM - 1; \
    if ((uint8_t)(encoderPosition / ENCODER_STEPS_PER_MENU_ITEM) >= currentMenuViewOffset + LCD_HEIGHT) { currentMenuViewOffset = (encoderPosition / ENCODER_STEPS_PER_MENU_ITEM) - LCD_HEIGHT + 1; lcdDrawUpdate = 1; _lineNr = currentMenuViewOffset - 1; _drawLineNr = -1; } \
    } } while(0)

/** Used variables to keep track of the menu */
volatile uint8_t buttons;//Contains the bits of the currently pressed buttons.

uint8_t currentMenuViewOffset;              /* scroll offset in the current menu */
uint32_t blocking_enc;
uint8_t lastEncoderBits;
int8_t encoderDiff; /* encoderDiff is updated from interrupt context and added to encoderPosition every LCD update */
int8_t encoderDiff2;
uint32_t encoderPosition;
#if (SDCARDDETECT > 0)
bool lcd_oldcardstatus;
#endif

menuFunc_t currentMenu = lcd_status_screen; /* function pointer to the currently active menu */
menuFunc_t backMenu = currentMenu;
uint32_t lcd_next_update_millis;
uint32_t lcd_status_update_millis;
//uint8_t lcd_status_update_delay;
uint8_t lcdDrawUpdate = 2;                  /* Set to none-zero when the LCD needs to draw, decreased after every draw. Set to 2 in LCD routines so the LCD gets atleast 1 full redraw (first redraw is partial) */

//prevMenu and prevEncoderPosition are used to store the previous menu location when editing settings.
menuFunc_t prevMenu = NULL;
uint16_t prevEncoderPosition;
//Variables used when editing values.
const char* editLabel;
void* editValue;
int32_t minEditValue, maxEditValue;
menuFunc_t callbackFunc;

// placeholders for Ki and Kd edits
float raw_Ki, raw_Kd;

/* Main status screen. It's up to the implementation specific part to show what is needed. As this is very display dependend */
static void lcd_status_screen()
{
	static int feedLast = 0;	//	For tracking feedmultiply changes
	
    //if (lcd_status_update_delay)
        //lcd_status_update_delay--;
    //else
        //lcdDrawUpdate = 1;

	if ( millis() >= lcd_status_update_millis )
		lcdDrawUpdate = 1;

    if (lcdDrawUpdate)
    {
        lcd_implementation_status_screen();
//        lcd_status_update_delay = 10;   /* redraw the main screen every second. This is easier then trying keep track of all things that change on the screen */
		lcd_status_update_millis = millis() + LCD_STATUS_UPDATE_INTERVAL;
    }

    if (LCD_CLICKED)
    {
        currentMenu = lcd_main_menu;
        encoderPosition = 0;
        lcd_quick_feedback();
    }

    // Dead zone at 100% feedrate
    if (feedmultiply < 100 && (feedmultiply + int(encoderPosition)) > 100 ||
            feedmultiply > 100 && (feedmultiply + int(encoderPosition)) < 100)
    {
        encoderPosition = 0;
        feedmultiply = 100;
    }

    if (feedmultiply == 100 && int(encoderPosition) > ENCODER_FEEDRATE_DEADZONE)
    {
        feedmultiply += int(encoderPosition) - ENCODER_FEEDRATE_DEADZONE;
        encoderPosition = 0;
    }
    else if (feedmultiply == 100 && int(encoderPosition) < -ENCODER_FEEDRATE_DEADZONE)
    {
        feedmultiply += int(encoderPosition) + ENCODER_FEEDRATE_DEADZONE;
        encoderPosition = 0;	
    }
    else if (feedmultiply != 100)
    {
        feedmultiply += int(encoderPosition);
        encoderPosition = 0;
    }

    if (feedmultiply < 10)
        feedmultiply = 10;
    if (feedmultiply > 999)
        feedmultiply = 999;
		
	if ( feedmultiply != feedLast )
		lcd_status_update_millis = 0;	//	Force update on next cycle
		//lcd_status_update_delay = 0;	//	Force update on next cycle
	feedLast = feedmultiply;
}

//####################################################################################################
//	Main status display screen
//####################################################################################################

static void lcd_return_to_status()
{
    encoderPosition = 0;
    currentMenu = lcd_status_screen;
}

static void lcd_sdcard_pause()
{
    card.pauseSDPrint();
}
static void lcd_sdcard_resume()
{
    card.startFileprint();
}
static void lcd_sdcard_stop()
{
    card.sdprinting = false;
    card.closefile();
    quickStop();
    if(SD_FINISHED_STEPPERRELEASE)
    {
        enquecommand_P(PSTR(SD_FINISHED_RELEASECOMMAND));
    }
    autotempShutdown();
}

/* Menu implementation */
static void lcd_main_menu()
{
    START_MENU();
	MENU_ITEM_BACK(back, MSG_WATCH, lcd_status_screen);
#ifdef SDSUPPORT
    if (card.cardOK)
    {
        if (card.isFileOpen())
        {
            if (card.sdprinting)
                MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_sdcard_pause);
            else
                MENU_ITEM(function, MSG_RESUME_PRINT, lcd_sdcard_resume);
            MENU_ITEM(function, MSG_STOP_PRINT, lcd_sdcard_stop);
        }else{
            MENU_ITEM(submenu, MSG_CARD_MENU, lcd_sdcard_menu);
#if SDCARDDETECT < 1
            MENU_ITEM(gcode, MSG_CNG_SDCARD, PSTR("M21"));  // SD-card changed by user
#endif
        }
    }else{
        MENU_ITEM(submenu, MSG_NO_CARD, lcd_sdcard_menu);
#if SDCARDDETECT < 1
        MENU_ITEM(gcode, MSG_INIT_SDCARD, PSTR("M21")); // Manually initialize the SD-card via user interface
#endif
    }
#endif //SDSUPPORT

    if (movesplanned() || IS_SD_PRINTING)
    {
	    MENU_ITEM(submenu, MSG_TUNE, lcd_tune_menu);
	    }else{
	    MENU_ITEM(submenu, MSG_PREPARE, lcd_prepare_menu);
    }
    MENU_ITEM(submenu, MSG_CONTROL, lcd_control_menu);
    MENU_ITEM(submenu, MSG_UTILITIES, lcd_utilities_menu);
    END_MENU();
}

#ifdef SDSUPPORT
static void lcd_autostart_sd()
{
    card.lastnr=0;
    card.setroot();
    card.checkautostart(true);
}
#endif

//####################################################################################################
//	Bed leveling routines
//####################################################################################################
int levelStep = 0;
int levelStepLast = 0;
int levelType = 0;
bool levelHomed = false;
bool moveHead = false;
char cmd[30];

#define BED_LEVEL_Z_LIFT	5

//	Point list for bed leveling script
const uint16_t LEVEL_POINTS[][2] = {
	{ BED_LEVEL_Z_LIFT, 4 },		//	First record is { Z_LIFT, NUM_POINTS }
	{ X_MIN_POS+35, Y_CENTER_POS-50 },
	{ X_MIN_POS+35, Y_CENTER_POS+50 },
	{ X_MAX_POS-35, Y_CENTER_POS+50 },
	{ X_MAX_POS-35, Y_CENTER_POS-50 }
};

//	Point list for bed leveling extents script
const uint16_t LEVEL_EXTENTS[][2] = {
	{ BED_LEVEL_Z_LIFT, 5 },		//	First record is { Z_LIFT, NUM_POINTS }
	{ X_MIN_POS+10, Y_MIN_POS+10 },
	{ X_MIN_POS+10, Y_MAX_POS-10 },
	{ X_MAX_POS-10, Y_MAX_POS-10 },
	{ X_MAX_POS-10, Y_MIN_POS+10 },
	{ X_CENTER_POS, Y_CENTER_POS }
};

static void lcd_bed_level_doCurrent()
{
	//	Always move up a little first, and set high speed
	sprintf_P(cmd, PSTR("G0 Z%d F10000"), LEVEL_POINTS[0][0]);
	enquecommand(cmd);
	//	Home axes if not already homed
	if ( !levelHomed )
	{
		enquecommand_P(PSTR("G28"));		//	Home axes
		levelHomed = true;
		sprintf_P(cmd, PSTR("G0 Z%d F10000"), LEVEL_POINTS[0][0]);
		enquecommand(cmd);
	}
	if ( levelType == 0 )
	{
		if ( levelStep < LEVEL_POINTS[0][1] )
		{
			sprintf_P(cmd, PSTR("G0 X%d Y%d"), LEVEL_POINTS[levelStep+1][0], LEVEL_POINTS[levelStep+1][1]);
			enquecommand(cmd);
			enquecommand_P(PSTR("G0 Z0"));
		}
		else
		{
			currentMenu = &lcd_bed_level_menu;			
		}
	}
	else
	{
		if ( levelStep < LEVEL_EXTENTS[0][1] )
		{
			sprintf_P(cmd, PSTR("G0 X%d Y%d"), LEVEL_EXTENTS[levelStep+1][0], LEVEL_EXTENTS[levelStep+1][1]);
			enquecommand(cmd);
			enquecommand_P(PSTR("G0 Z0"));
		}
		else
		{
			currentMenu = &lcd_bed_level_menu;
		}
	}
}
static void lcd_bed_level_doNext()
{
	levelStep++;
	lcd_bed_level_doCurrent();
}
static void	lcd_bed_level_doLast()
{
	levelStep--;
	lcd_bed_level_doCurrent();
}
static void lcd_bed_level_moveHead()
{
	moveHead = true;
	sprintf_P(cmd, PSTR("G0 Z%d F10000"), BED_LEVEL_Z_LIFT);
	enquecommand(cmd);
	sprintf_P(cmd, PSTR("G0 X%d Y%d"), X_CENTER_POS, Y_CENTER_POS);
	enquecommand(cmd);
	encoderPosition = 0;
}
static void lcd_bed_level_moveHead_return()
{
	moveHead = false;
	lcd_bed_level_doCurrent();
}
static void lcd_bed_level_disable_steppers()
{
	levelStep--;
	enquecommand_P(PSTR("M84"));
	levelHomed = false;
	encoderPosition = 0;
}
static void	lcd_bed_level_start()
{
	levelStep = -1;
	levelType = 0;
	lcd_bed_level_doNext();
}
static void	lcd_bed_level_extents()
{
	levelStep = -1;
	levelType = 1;
	lcd_bed_level_doNext();
}
static void lcd_bed_level_cancel()
{
	lcd_bed_level_moveHead();
	lcd_bed_level_menu();
}
static void lcd_bed_level_syncZ()
{
	
}
static void lcd_bed_level_run()
{
	if ( levelStep != levelStepLast )
	{
		lcd_quick_feedback();
		encoderPosition = 0;
	}
	levelStepLast = levelStep;

	START_MENU();
	if ( moveHead || (!levelHomed && levelStep >= 0) )
	{
		if ( moveHead )
			MENU_ITEM(function, "Resume", lcd_bed_level_moveHead_return);
		else
			MENU_ITEM(function, "Resume", lcd_bed_level_doNext);
		MENU_ITEM_BACK(back, "Cancel", lcd_bed_level_menu);
	}
	else if ( levelStep == -1 )
	{
		MENU_ITEM_BACK(back, MSG_UTILITIES, lcd_utilities_menu);
		MENU_ITEM(function, "Level Bed", lcd_bed_level_start);
		MENU_ITEM(function, "Check Extents", lcd_bed_level_extents);
//		MENU_ITEM(submenu, "Sync Z Steppers", lcd_bed_level_syncZ);
	}
	else
	{
		timeoutSuspend = true;
		MENU_ITEM(function, "Continue", lcd_bed_level_doNext);
		MENU_ITEM(function, "Move Head", lcd_bed_level_moveHead);
		if ( levelStep > 0 )
		{
			MENU_ITEM(function, "Go Back", lcd_bed_level_doLast);
		}
		MENU_ITEM(function, MSG_DISABLE_STEPPERS, lcd_bed_level_disable_steppers);
		MENU_ITEM_BACK(back, "Cancel", lcd_bed_level_cancel);
	}
	END_MENU();
}

static void lcd_bed_level_menu()
{
	timeoutSuspend = false;
	levelStep = -1;
	levelStepLast = 0;
	levelType = 0;
	levelHomed = false;
	moveHead = false;
	currentMenu = &lcd_bed_level_run;
}

//####################################################################################################
//	Tune / Prepare
//####################################################################################################
//	Prepare will show when not running
static void lcd_prepare_menu()
{
	START_MENU();
	MENU_ITEM_BACK(back, MSG_MAIN, lcd_main_menu);
	MENU_ITEM(submenu, MSG_HEAT_COOL, lcd_heat_cool_menu);
	MENU_ITEM(submenu, MSG_MOVE_AXIS, lcd_move_menu);
	MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
	MENU_ITEM(gcode, MSG_DISABLE_STEPPERS, PSTR("M84"));
	//MENU_ITEM(gcode, MSG_SET_ORIGIN, PSTR("G92 X0 Y0 Z0"));
	END_MENU();
}
//	Tune will show while running
static void lcd_tune_menu()
{
    START_MENU();
    MENU_ITEM_BACK(back, MSG_MAIN, lcd_main_menu);
    MENU_ITEM_EDIT(int3, MSG_SPEED, &feedmultiply, 10, 999);
    MENU_ITEM_EDIT(int3, MSG_NOZZLE, &target_temperature[0], 0, HEATER_0_MAXTEMP - 15);
#if TEMP_SENSOR_1 != 0
    MENU_ITEM_EDIT(int3, MSG_NOZZLE1, &target_temperature[1], 0, HEATER_1_MAXTEMP - 15);
#endif
#if TEMP_SENSOR_2 != 0
    MENU_ITEM_EDIT(int3, MSG_NOZZLE2, &target_temperature[2], 0, HEATER_2_MAXTEMP - 15);
#endif
#if TEMP_SENSOR_BED != 0
    MENU_ITEM_EDIT(int3, MSG_BED, &target_temperature_bed, 0, BED_MAXTEMP - 15);
#endif
    MENU_ITEM_EDIT(int3, MSG_FAN_SPEED, &fanSpeed, 0, 255);
    MENU_ITEM_EDIT(int3, MSG_FLOW, &extrudemultiply, 10, 999);
#ifdef FILAMENTCHANGEENABLE
     MENU_ITEM(gcode, MSG_FILAMENTCHANGE, PSTR("M600"));
#endif
    END_MENU();
}

//####################################################################################################
//	Temperature control routines
//####################################################################################################

void lcd_preheat_pla()
{
	setTargetHotend0(plaPreheatHotendTemp);
	setTargetHotend1(plaPreheatHotendTemp);
	setTargetHotend2(plaPreheatHotendTemp);
	setTargetBed(plaPreheatHPBTemp);
	fanSpeed = plaPreheatFanSpeed;
	lcd_return_to_status();
	setWatch(); // heater sanity check timer
}
void lcd_preheat_abs()
{
	setTargetHotend0(absPreheatHotendTemp);
	setTargetHotend1(absPreheatHotendTemp);
	setTargetHotend2(absPreheatHotendTemp);
	setTargetBed(absPreheatHPBTemp);
	fanSpeed = absPreheatFanSpeed;
	lcd_return_to_status();
	setWatch(); // heater sanity check timer
}
static void lcd_cooldown()
{
	setTargetHotend0(0);
	setTargetHotend1(0);
	setTargetHotend2(0);
	setTargetBed(0);
	lcd_return_to_status();
}

static void lcd_heat_cool_menu()
{
	START_MENU();
	MENU_ITEM_BACK(back, MSG_PREPARE, lcd_prepare_menu);
	MENU_ITEM(function, MSG_PREHEAT_PLA, lcd_preheat_pla);
	MENU_ITEM(function, MSG_PREHEAT_ABS, lcd_preheat_abs);
	MENU_ITEM(function, MSG_COOLDOWN, lcd_cooldown);
    MENU_ITEM_EDIT(int3, MSG_NOZZLE, &target_temperature[0], 0, HEATER_0_MAXTEMP - 15);
    #if TEMP_SENSOR_1 != 0
    MENU_ITEM_EDIT(int3, MSG_NOZZLE1, &target_temperature[1], 0, HEATER_1_MAXTEMP - 15);
    #endif
//	if ( EX2_ENABLED )
//	    MENU_ITEM_EDIT(int3, MSG_NOZZLE1, &target_temperature[1], 0, HEATER_1_MAXTEMP - 15);
    #if TEMP_SENSOR_2 != 0
    MENU_ITEM_EDIT(int3, MSG_NOZZLE2, &target_temperature[2], 0, HEATER_2_MAXTEMP - 15);
    #endif
    #if TEMP_SENSOR_BED != 0
    MENU_ITEM_EDIT(int3, MSG_BED, &target_temperature_bed, 0, BED_MAXTEMP - 15);
    #endif
    MENU_ITEM_EDIT(int3, MSG_FAN_SPEED, &fanSpeed, 0, 255);
	END_MENU();
}


static void lcd_utilities_menu()
{
	START_MENU();
	MENU_ITEM_BACK(back, MSG_MAIN, lcd_main_menu);
	MENU_ITEM(submenu, "Bed Leveling", lcd_bed_level_menu);
#ifdef FILAMENTCHANGEENABLE
	MENU_ITEM(gcode, MSG_FILAMENTCHANGE, PSTR("M600"));
#endif
	END_MENU();
}


//####################################################################################################
//	Manual movement routines
//####################################################################################################
float move_menu_scale;
static void lcd_move_menu_axis();

static void lcd_move_x()
{
    if (encoderPosition != 0)
    {
        current_position[X_AXIS] += float((int)encoderPosition) * move_menu_scale;
        if (current_position[X_AXIS] < X_MIN_POS)
            current_position[X_AXIS] = X_MIN_POS;
        if (current_position[X_AXIS] > X_MAX_POS)
            current_position[X_AXIS] = X_MAX_POS;
        encoderPosition = 0;
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 600, active_extruder);
        lcdDrawUpdate = 1;
    }
    if (lcdDrawUpdate)
    {
        lcd_implementation_drawedit(PSTR("X"), ftostr31(current_position[X_AXIS]));
    }
    if (LCD_CLICKED)
    {
        lcd_quick_feedback();
        currentMenu = lcd_move_menu_axis;
        encoderPosition = 0;
    }
}
static void lcd_move_y()
{
    if (encoderPosition != 0)
    {
        current_position[Y_AXIS] += float((int)encoderPosition) * move_menu_scale;
        if (current_position[Y_AXIS] < Y_MIN_POS)
            current_position[Y_AXIS] = Y_MIN_POS;
        if (current_position[Y_AXIS] > Y_MAX_POS)
            current_position[Y_AXIS] = Y_MAX_POS;
        encoderPosition = 0;
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 600, active_extruder);
        lcdDrawUpdate = 1;
    }
    if (lcdDrawUpdate)
    {
        lcd_implementation_drawedit(PSTR("Y"), ftostr31(current_position[Y_AXIS]));
    }
    if (LCD_CLICKED)
    {
        lcd_quick_feedback();
        currentMenu = lcd_move_menu_axis;
        encoderPosition = 0;
    }
}
static void lcd_move_z()
{
    if (encoderPosition != 0)
    {
        current_position[Z_AXIS] += float((int)encoderPosition) * move_menu_scale;
        if (current_position[Z_AXIS] < Z_MIN_POS)
            current_position[Z_AXIS] = Z_MIN_POS;
        if (current_position[Z_AXIS] > Z_MAX_POS)
            current_position[Z_AXIS] = Z_MAX_POS;
        encoderPosition = 0;
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 60, active_extruder);
        lcdDrawUpdate = 1;
    }
    if (lcdDrawUpdate)
    {
        lcd_implementation_drawedit(PSTR("Z"), ftostr31(current_position[Z_AXIS]));
    }
    if (LCD_CLICKED)
    {
        lcd_quick_feedback();
        currentMenu = lcd_move_menu_axis;
        encoderPosition = 0;
    }
}
static void lcd_move_e()
{
    if (encoderPosition != 0)
    {
        current_position[E_AXIS] += float((int)encoderPosition) * move_menu_scale;
        encoderPosition = 0;
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 20, active_extruder);
        lcdDrawUpdate = 1;
    }
    if (lcdDrawUpdate)
    {
        lcd_implementation_drawedit(PSTR("Extruder"), ftostr31(current_position[E_AXIS]));
    }
    if (LCD_CLICKED)
    {
        lcd_quick_feedback();
        currentMenu = lcd_move_menu_axis;
        encoderPosition = 0;
    }
}

static void lcd_move_menu_axis()
{
    START_MENU();
    MENU_ITEM_BACK(back, MSG_MOVE_AXIS, lcd_move_menu);
    MENU_ITEM(submenu, "Move X", lcd_move_x);
    MENU_ITEM(submenu, "Move Y", lcd_move_y);
//    if (move_menu_scale < 10.0)
//    {
        MENU_ITEM(submenu, "Move Z", lcd_move_z);
        MENU_ITEM(submenu, "Extruder", lcd_move_e);
//    }
    END_MENU();
}

static void lcd_move_menu_10mm()
{
    move_menu_scale = 10.0;
    lcd_move_menu_axis();
}
static void lcd_move_menu_1mm()
{
    move_menu_scale = 1.0;
    lcd_move_menu_axis();
}
static void lcd_move_menu_01mm()
{
    move_menu_scale = 0.1;
    lcd_move_menu_axis();
}

static void lcd_move_menu()
{
    START_MENU();
    MENU_ITEM_BACK(back, MSG_PREPARE, lcd_prepare_menu);
    MENU_ITEM(submenu, "Move 10mm", lcd_move_menu_10mm);
    MENU_ITEM(submenu, "Move 1mm", lcd_move_menu_1mm);
    MENU_ITEM(submenu, "Move 0.1mm", lcd_move_menu_01mm);
    //TODO:X,Y,Z,E
    END_MENU();
}


static void lcd_control_menu()
{
    START_MENU();
    MENU_ITEM_BACK(back, MSG_MAIN, lcd_main_menu);
    MENU_ITEM(submenu, MSG_TEMPERATURE, lcd_control_temperature_menu);
    MENU_ITEM(submenu, MSG_MOTION, lcd_control_motion_menu);
#ifdef FWRETRACT
    MENU_ITEM(submenu, MSG_RETRACT, lcd_control_retract_menu);
#endif
#ifdef EEPROM_SETTINGS
    MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
    MENU_ITEM(function, MSG_LOAD_EPROM, Config_RetrieveSettings);
#endif
    MENU_ITEM(function, MSG_RESTORE_FAILSAFE, Config_ResetDefault);
    END_MENU();
}

static void lcd_control_temperature_menu()
{
    // set up temp variables - undo the default scaling
    raw_Ki = unscalePID_i(Ki);
    raw_Kd = unscalePID_d(Kd);

    START_MENU();
    MENU_ITEM_BACK(back, MSG_CONTROL, lcd_control_menu);
    MENU_ITEM(submenu, MSG_PREHEAT_PLA_SETTINGS, lcd_control_temperature_preheat_pla_settings_menu);
    MENU_ITEM(submenu, MSG_PREHEAT_ABS_SETTINGS, lcd_control_temperature_preheat_abs_settings_menu);
#ifdef AUTOTEMP
    MENU_ITEM_EDIT(bool, MSG_AUTOTEMP, &autotemp_enabled);
    MENU_ITEM_EDIT(float3, MSG_MIN, &autotemp_min, 0, HEATER_0_MAXTEMP - 15);
    MENU_ITEM_EDIT(float3, MSG_MAX, &autotemp_max, 0, HEATER_0_MAXTEMP - 15);
    MENU_ITEM_EDIT(float32, MSG_FACTOR, &autotemp_factor, 0.0, 1.0);
#endif
#ifdef PIDTEMP
    MENU_ITEM_EDIT(float52, MSG_PID_P, &Kp, 1, 9990);
    // i is typically a small value so allows values below 1
    MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_I, &raw_Ki, 0.01, 9990, copy_and_scalePID_i);
    MENU_ITEM_EDIT_CALLBACK(float52, MSG_PID_D, &raw_Kd, 1, 9990, copy_and_scalePID_d);
# ifdef PID_ADD_EXTRUSION_RATE
    MENU_ITEM_EDIT(float3, MSG_PID_C, &Kc, 1, 9990);
# endif//PID_ADD_EXTRUSION_RATE
#endif//PIDTEMP
    END_MENU();
}

static void lcd_control_temperature_preheat_pla_settings_menu()
{
    START_MENU();
    MENU_ITEM_BACK(back, MSG_TEMPERATURE, lcd_control_temperature_menu);
    MENU_ITEM_EDIT(int3, MSG_NOZZLE, &plaPreheatHotendTemp, 0, HEATER_0_MAXTEMP - 15);
#if TEMP_SENSOR_BED != 0
    MENU_ITEM_EDIT(int3, MSG_BED, &plaPreheatHPBTemp, 0, BED_MAXTEMP - 15);
#endif
    MENU_ITEM_EDIT(int3, MSG_FAN_SPEED, &plaPreheatFanSpeed, 0, 255);
#ifdef EEPROM_SETTINGS
    MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
#endif
    END_MENU();
}

static void lcd_control_temperature_preheat_abs_settings_menu()
{
    START_MENU();
    MENU_ITEM_BACK(back, MSG_TEMPERATURE, lcd_control_temperature_menu);
    MENU_ITEM_EDIT(int3, MSG_NOZZLE, &absPreheatHotendTemp, 0, HEATER_0_MAXTEMP - 15);
#if TEMP_SENSOR_BED != 0
    MENU_ITEM_EDIT(int3, MSG_BED, &absPreheatHPBTemp, 0, BED_MAXTEMP - 15);
#endif
    MENU_ITEM_EDIT(int3, MSG_FAN_SPEED, &absPreheatFanSpeed, 0, 255);
#ifdef EEPROM_SETTINGS
    MENU_ITEM(function, MSG_STORE_EPROM, Config_StoreSettings);
#endif
    END_MENU();
}

static void lcd_control_motion_menu()
{
    START_MENU();
    MENU_ITEM_BACK(back, MSG_CONTROL, lcd_control_menu);
    MENU_ITEM_EDIT(float5, MSG_ACC, &acceleration, 500, 99000);
    MENU_ITEM_EDIT(float3, MSG_VXY_JERK, &max_xy_jerk, 1, 990);
    MENU_ITEM_EDIT(float52, MSG_VZ_JERK, &max_z_jerk, 0.1, 990);
    MENU_ITEM_EDIT(float3, MSG_VE_JERK, &max_e_jerk, 1, 990);
    MENU_ITEM_EDIT(float3, MSG_VMAX MSG_X, &max_feedrate[X_AXIS], 1, 999);
    MENU_ITEM_EDIT(float3, MSG_VMAX MSG_Y, &max_feedrate[Y_AXIS], 1, 999);
    MENU_ITEM_EDIT(float3, MSG_VMAX MSG_Z, &max_feedrate[Z_AXIS], 1, 999);
    MENU_ITEM_EDIT(float3, MSG_VMAX MSG_E, &max_feedrate[E_AXIS], 1, 999);
    MENU_ITEM_EDIT(float3, MSG_VMIN, &minimumfeedrate, 0, 999);
    MENU_ITEM_EDIT(float3, MSG_VTRAV_MIN, &mintravelfeedrate, 0, 999);
    MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_X, &max_acceleration_units_per_sq_second[X_AXIS], 100, 99000, reset_acceleration_rates);
    MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_Y, &max_acceleration_units_per_sq_second[Y_AXIS], 100, 99000, reset_acceleration_rates);
    MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_Z, &max_acceleration_units_per_sq_second[Z_AXIS], 100, 99000, reset_acceleration_rates);
    MENU_ITEM_EDIT_CALLBACK(long5, MSG_AMAX MSG_E, &max_acceleration_units_per_sq_second[E_AXIS], 100, 99000, reset_acceleration_rates);
    MENU_ITEM_EDIT(float5, MSG_A_RETRACT, &retract_acceleration, 100, 99000);
    MENU_ITEM_EDIT(float52, MSG_XSTEPS, &axis_steps_per_unit[X_AXIS], 5, 9999);
    MENU_ITEM_EDIT(float52, MSG_YSTEPS, &axis_steps_per_unit[Y_AXIS], 5, 9999);
    MENU_ITEM_EDIT(float51, MSG_ZSTEPS, &axis_steps_per_unit[Z_AXIS], 5, 9999);
    MENU_ITEM_EDIT(float51, MSG_ESTEPS, &axis_steps_per_unit[E_AXIS], 5, 9999);    
#ifdef ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED
    MENU_ITEM_EDIT(bool, "Endstop abort", &abort_on_endstop_hit);
#endif
    END_MENU();
}

#ifdef FWRETRACT
static void lcd_control_retract_menu()
{
    START_MENU();
    MENU_ITEM_BACK(back, MSG_CONTROL, lcd_control_menu);
    MENU_ITEM_EDIT(bool, MSG_AUTORETRACT, &autoretract_enabled);
    MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT, &retract_length, 0, 100);
    MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACTF, &retract_feedrate, 1, 999);
    MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_ZLIFT, &retract_zlift, 0, 999);
    MENU_ITEM_EDIT(float52, MSG_CONTROL_RETRACT_RECOVER, &retract_recover_length, 0, 100);
    MENU_ITEM_EDIT(float3, MSG_CONTROL_RETRACT_RECOVERF, &retract_recover_feedrate, 1, 999);
    END_MENU();
}
#endif

#if SDCARDDETECT == -1
static void lcd_sd_refresh()
{
    card.initsd();
    currentMenuViewOffset = 0;
}
#endif
static void lcd_sd_updir()
{
    card.updir();
    currentMenuViewOffset = 0;
}

void lcd_sdcard_menu()
{
	lcdFastUpdate = true;
	
    uint16_t fileCnt = card.getnrfilenames();
    START_MENU();
    MENU_ITEM_BACK(back, MSG_MAIN, lcd_main_menu);
    card.getWorkDirName();
    if(card.filename[0]=='/')
    {
#if SDCARDDETECT == -1
        MENU_ITEM(function, LCD_STR_REFRESH MSG_REFRESH, lcd_sd_refresh);
#endif
    }else{
        MENU_ITEM(function, LCD_STR_FOLDER "..", lcd_sd_updir);
    }
    
    for(uint16_t i=0;i<fileCnt;i++)
    {
        if (_menuItemNr == _lineNr)
        {
            card.getfilename(i);
            if (card.filenameIsDir)
            {
                MENU_ITEM(sddirectory, MSG_CARD_MENU, card.filename, card.longFilename, _lineNr);
            }else{
                MENU_ITEM(sdfile, MSG_CARD_MENU, card.filename, card.longFilename, _lineNr);
            }
        }else{
            MENU_ITEM_DUMMY();
        }
    }
    END_MENU();
}

//####################################################################################################
//	Menu edit routines
//####################################################################################################
#define menu_edit_type(_type, _name, _strFunc, scale) \
    void menu_edit_ ## _name () \
    { \
		encoderCoarseEnabled = true; \
        if ((int32_t)encoderPosition < minEditValue) \
            encoderPosition = minEditValue; \
        if ((int32_t)encoderPosition > maxEditValue) \
            encoderPosition = maxEditValue; \
        if (lcdDrawUpdate) \
            lcd_implementation_drawedit(editLabel, _strFunc(((_type)encoderPosition) / scale)); \
        if (LCD_CLICKED) \
        { \
            *((_type*)editValue) = ((_type)encoderPosition) / scale; \
            lcd_quick_feedback(); \
            currentMenu = prevMenu; \
            encoderPosition = prevEncoderPosition; \
        } \
    } \
    void menu_edit_callback_ ## _name () \
    { \
		encoderCoarseEnabled = true; \
        if ((int32_t)encoderPosition < minEditValue) \
            encoderPosition = minEditValue; \
        if ((int32_t)encoderPosition > maxEditValue) \
            encoderPosition = maxEditValue; \
        if (lcdDrawUpdate) \
            lcd_implementation_drawedit(editLabel, _strFunc(((_type)encoderPosition) / scale)); \
        if (LCD_CLICKED) \
        { \
            *((_type*)editValue) = ((_type)encoderPosition) / scale; \
            lcd_quick_feedback(); \
            currentMenu = prevMenu; \
            encoderPosition = prevEncoderPosition; \
            (*callbackFunc)();\
        } \
    } \
    static void menu_action_setting_edit_ ## _name (const char* pstr, _type* ptr, _type minValue, _type maxValue) \
    { \
		encoderCoarseEnabled = true; \
        prevMenu = currentMenu; \
        prevEncoderPosition = encoderPosition; \
         \
        lcdDrawUpdate = 2; \
        currentMenu = menu_edit_ ## _name; \
         \
        editLabel = pstr; \
        editValue = ptr; \
        minEditValue = minValue * scale; \
        maxEditValue = maxValue * scale; \
        encoderPosition = (*ptr) * scale; \
    }\
    static void menu_action_setting_edit_callback_ ## _name (const char* pstr, _type* ptr, _type minValue, _type maxValue, menuFunc_t callback) \
    { \
		encoderCoarseEnabled = true; \
        prevMenu = currentMenu; \
        prevEncoderPosition = encoderPosition; \
         \
        lcdDrawUpdate = 2; \
        currentMenu = menu_edit_callback_ ## _name; \
         \
        editLabel = pstr; \
        editValue = ptr; \
        minEditValue = minValue * scale; \
        maxEditValue = maxValue * scale; \
        encoderPosition = (*ptr) * scale; \
        callbackFunc = callback;\
    }
menu_edit_type(int, int3, itostr3, 1)
menu_edit_type(float, float3, ftostr3, 1)
menu_edit_type(float, float32, ftostr32, 100)
menu_edit_type(float, float5, ftostr5, 0.01)
menu_edit_type(float, float51, ftostr51, 10)
menu_edit_type(float, float52, ftostr52, 100)
menu_edit_type(unsigned long, long5, ftostr5, 0.01)


/** End of menus **/

static void lcd_quick_feedback()
{
    lcdDrawUpdate = 2;
    blocking_enc = millis() + 500;
    lcd_implementation_quick_feedback();
}

/** Menu action functions **/
static void menu_action_back(menuFunc_t data)
{
    currentMenu = data;
    encoderPosition = 0;
}
static void menu_action_submenu(menuFunc_t data)
{
    currentMenu = data;
    encoderPosition = 0;
}
static void menu_action_gcode(const char* pgcode)
{
    enquecommand_P(pgcode);
}
static void menu_action_function(menuFunc_t data)
{
    (*data)();
}
static void menu_action_sdfile(const char* filename, char* longFilename, uint8_t item)
{
    char cmd[30];
    char* c;
    sprintf_P(cmd, PSTR("M23 %s"), filename);
    for(c = &cmd[4]; *c; c++)
        *c = tolower(*c);
    enquecommand(cmd);
    enquecommand_P(PSTR("M24"));
    lcd_return_to_status();
}
static void menu_action_sddirectory(const char* filename, char* longFilename, uint8_t item)
{
    card.chdir(filename);
    encoderPosition = 0;
}
static void menu_action_setting_edit_bool(const char* pstr, bool* ptr)
{
    *ptr = !(*ptr);
}

/** LCD API **/
void lcd_init()
{
    lcd_implementation_init();
  #if BTN_ENC > 0
    pinMode(BTN_ENC,INPUT); 
    WRITE(BTN_ENC,HIGH);
  #endif    
    pinMode(BTN_UP,INPUT);
    pinMode(BTN_DWN,INPUT);
    pinMode(BTN_LFT,INPUT);
    pinMode(BTN_RT,INPUT);
 //   pinMode(BTN_ENT,INPUT)

#if (SDCARDDETECT > 0)
    pinMode(SDCARDDETECT,INPUT);
    WRITE(SDCARDDETECT, HIGH);
    lcd_oldcardstatus = IS_SD_INSERTED;
#endif//(SDCARDDETECT > 0)
    lcd_buttons_update();
    encoderDiff = 0;
}

void lcd_update()
{
    static unsigned long timeoutToStatus = 0;
    
    lcd_buttons_update();
    
    #ifdef LCD_HAS_SLOW_BUTTONS
    buttons |= lcd_implementation_read_slow_buttons(); // buttons which take too long to read in interrupt context
    #endif
    
    #if (SDCARDDETECT > 0)
    if((IS_SD_INSERTED != lcd_oldcardstatus))
    {
        lcdDrawUpdate = 2;
        lcd_oldcardstatus = IS_SD_INSERTED;
        lcd_implementation_init(); // to maybe revive the lcd if static electricty killed it.
        
        if(lcd_oldcardstatus)
        {
            card.initsd();
            LCD_MESSAGEPGM(MSG_SD_INSERTED);
        }
        else
        {
            card.release();
            LCD_MESSAGEPGM(MSG_SD_REMOVED);
        }
    }
    #endif//CARDINSERTED
    
    if (lcd_next_update_millis < millis())
    {
       if (encoderDiff || encoderDiff2)
        {
			encoderDiff = invertEncoderDir?-encoderDiff:encoderDiff;	//	Check for direction inversion if set by a menu
			if ( encoderCoarseEnabled )									//	Check if left/right encoder is being used
			{
				encoderDiff2 *= 5;
				encoderDiff += encoderDiff2;
			}
			
            lcdDrawUpdate = 1;
			encoderPosition += encoderDiff;	//	Get difference, 
            encoderDiff = 0;
			encoderDiff2 = 0;
            timeoutToStatus = millis() + LCD_TIMEOUT_TO_STATUS;
        }
        if (LCD_CLICKED)
            timeoutToStatus = millis() + LCD_TIMEOUT_TO_STATUS;

		encoderCoarseEnabled = false;	//	Clear encoder coarse enable to allow a menu to set it
		invertEncoderDir = false;		//	Clear encoder inversion to allow a menu to set it
        (*currentMenu)();

#ifdef LCD_HAS_STATUS_INDICATORS
        lcd_implementation_update_indicators();
#endif

        if(timeoutToStatus < millis() && currentMenu != lcd_status_screen && !timeoutSuspend)
        {
            lcd_return_to_status();
            lcdDrawUpdate = 2;
        }
 
        if (lcdDrawUpdate == 2)
            lcd_implementation_clear();
        if (lcdDrawUpdate)
            lcdDrawUpdate--;
        lcd_next_update_millis = millis() + LCD_UPDATE_INTERVAL;

		if ( lcdFastUpdate )		//	Set up for update on next cycle
			lcdDrawUpdate = 1;
		
    }
	lcdFastUpdate = false;
}

void lcd_setstatus(const char* message)
{
    if (lcd_status_message_level > 0)
        return;
    strncpy(lcd_status_message, message, LCD_WIDTH);
    lcdDrawUpdate = 2;
}
void lcd_setstatuspgm(const char* message)
{
    if (lcd_status_message_level > 0)
        return;
    strncpy_P(lcd_status_message, message, LCD_WIDTH);
    lcdDrawUpdate = 2;
}
void lcd_setalertstatuspgm(const char* message)
{
    lcd_setstatuspgm(message);
    lcd_status_message_level = 1;
    lcd_return_to_status();
}
void lcd_reset_alert_level()
{
    lcd_status_message_level = 0;
}

/* Warning: This function is called from interrupt context */
void lcd_buttons_update()
{
    uint8_t newbuttons=0;

	if ( millis() > blocking_enc )
	{
		if(READ(BTN_ENC)==0)
			newbuttons |= EN_C;
		if(READ(BTN_UP)==0)
			newbuttons |= B_UP;
		if(READ(BTN_DWN)==0)
			newbuttons |= B_DW;
		if(READ(BTN_RT)==0)
			newbuttons |= B_RI;
		if(READ(BTN_LFT)==0)
			newbuttons |= B_LE;
	}

	if ( newbuttons != buttons )
	{
		//	Up/down buttons form main encoder
		if(newbuttons&B_UP)
			encoderDiff++;
		if(newbuttons&B_DW)
			encoderDiff--;

		//	Left/right buttons form secondary encoder (if needed)
		if(newbuttons&B_RI)
			encoderDiff2++;
		if(newbuttons&B_LE)
			encoderDiff2--;
	}

	buttons = newbuttons;
}

void lcd_buzz(long duration, uint16_t freq)
{ 
#ifdef LCD_USE_I2C_BUZZER
  lcd.buzz(duration,freq);
#endif   
}

bool lcd_clicked() 
{ 
  return LCD_CLICKED;
}

/********************************/
/** Float conversion utilities **/
/********************************/
//  convert float to string with +123.4 format
char conv[8];
char *ftostr3(const float &x)
{
  return itostr3((int)x);
}

char *itostr2(const uint8_t &x)
{
  //sprintf(conv,"%5.1f",x);
  int xx=x;
  conv[0]=(xx/10)%10+'0';
  conv[1]=(xx)%10+'0';
  conv[2]=0;
  return conv;
}

//  convert float to string with +123.4 format
char *ftostr31(const float &x)
{
  int xx=x*10;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]=(xx/10)%10+'0';
  conv[4]='.';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

//  convert float to string with 123.4 format
char *ftostr31ns(const float &x)
{
  int xx=x*10;
  //conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[0]=(xx/1000)%10+'0';
  conv[1]=(xx/100)%10+'0';
  conv[2]=(xx/10)%10+'0';
  conv[3]='.';
  conv[4]=(xx)%10+'0';
  conv[5]=0;
  return conv;
}

char *ftostr32(const float &x)
{
  long xx=x*100;
  if (xx >= 0)
    conv[0]=(xx/10000)%10+'0';
  else
    conv[0]='-';
  xx=abs(xx);
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]='.';
  conv[4]=(xx/10)%10+'0';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

char *itostr31(const int &xx)
{
  conv[0]=(xx>=0)?'+':'-';
  conv[1]=(xx/1000)%10+'0';
  conv[2]=(xx/100)%10+'0';
  conv[3]=(xx/10)%10+'0';
  conv[4]='.';
  conv[5]=(xx)%10+'0';
  conv[6]=0;
  return conv;
}

char *itostr3(const int &xx)
{
  if (xx >= 100)
    conv[0]=(xx/100)%10+'0';
  else
    conv[0]=' ';
  if (xx >= 10)
    conv[1]=(xx/10)%10+'0';
  else
    conv[1]=' ';
  conv[2]=(xx)%10+'0';
  conv[3]=0;
  return conv;
}

char *itostr3left(const int &xx)
{
  if (xx >= 100)
  {
    conv[0]=(xx/100)%10+'0';
    conv[1]=(xx/10)%10+'0';
    conv[2]=(xx)%10+'0';
    conv[3]=0;
  }
  else if (xx >= 10)
  {
    conv[0]=(xx/10)%10+'0';
    conv[1]=(xx)%10+'0';
    conv[2]=0;
  }
  else
  {
    conv[0]=(xx)%10+'0';
    conv[1]=0;
  }
  return conv;
}

char *itostr4(const int &xx)
{
  if (xx >= 1000)
    conv[0]=(xx/1000)%10+'0';
  else
    conv[0]=' ';
  if (xx >= 100)
    conv[1]=(xx/100)%10+'0';
  else
    conv[1]=' ';
  if (xx >= 10)
    conv[2]=(xx/10)%10+'0';
  else
    conv[2]=' ';
  conv[3]=(xx)%10+'0';
  conv[4]=0;
  return conv;
}

//  convert float to string with 12345 format
char *ftostr5(const float &x)
{
  long xx=abs(x);
  if (xx >= 10000)
    conv[0]=(xx/10000)%10+'0';
  else
    conv[0]=' ';
  if (xx >= 1000)
    conv[1]=(xx/1000)%10+'0';
  else
    conv[1]=' ';
  if (xx >= 100)
    conv[2]=(xx/100)%10+'0';
  else
    conv[2]=' ';
  if (xx >= 10)
    conv[3]=(xx/10)%10+'0';
  else
    conv[3]=' ';
  conv[4]=(xx)%10+'0';
  conv[5]=0;
  return conv;
}

//  convert float to string with +1234.5 format
char *ftostr51(const float &x)
{
  long xx=x*10;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/10000)%10+'0';
  conv[2]=(xx/1000)%10+'0';
  conv[3]=(xx/100)%10+'0';
  conv[4]=(xx/10)%10+'0';
  conv[5]='.';
  conv[6]=(xx)%10+'0';
  conv[7]=0;
  return conv;
}

//  convert float to string with +123.45 format
char *ftostr52(const float &x)
{
  long xx=x*100;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/10000)%10+'0';
  conv[2]=(xx/1000)%10+'0';
  conv[3]=(xx/100)%10+'0';
  conv[4]='.';
  conv[5]=(xx/10)%10+'0';
  conv[6]=(xx)%10+'0';
  conv[7]=0;
  return conv;
}

// Callback for after editing PID i value
// grab the pid i value out of the temp variable; scale it; then update the PID driver
void copy_and_scalePID_i()
{
  Ki = scalePID_i(raw_Ki);
  updatePID();
}

// Callback for after editing PID d value
// grab the pid d value out of the temp variable; scale it; then update the PID driver
void copy_and_scalePID_d()
{
  Kd = scalePID_d(raw_Kd);
  updatePID();
}

#endif //RIGIDPANEL
