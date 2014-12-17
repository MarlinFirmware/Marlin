#include "ultralcd.h"

#include "Marlin.h"
#include "cardreader.h"
#include "ConfigurationStore.h"
#include "temperature.h"
#include "language.h"


/*******************************************************************************
**   Variables
*******************************************************************************/

// Extern variables
extern bool stop_buffer;
extern int stop_buffer_code;
extern uint8_t buffer_recursivity;

static float manual_feedrate[] = MANUAL_FEEDRATE;


// Configuration settings
// TODO : Review & delete if possible
int plaPreheatHotendTemp;
int plaPreheatHPBTemp;
int plaPreheatFanSpeed;

int absPreheatHotendTemp;
int absPreheatHPBTemp;
int absPreheatFanSpeed;

uint8_t prev_encoder_position;

float move_menu_scale;
//


// Display related variables
uint8_t    display_refresh_mode;
uint32_t   display_time_refresh = 0;

uint32_t   display_timeout;
bool       display_timeout_blocked;

view_t display_view_next;
view_t display_view;


// Encoder related variables
uint8_t encoder_input;
uint8_t encoder_input_last;
bool    encoder_input_blocked;
bool    encoder_input_updated;

int16_t encoder_position;


// Button related variables
uint8_t button_input;
uint8_t button_input_last;
bool    button_input_blocked;
bool    button_input_updated;

bool    button_clicked_triggered;

// Beeper related variables
bool beeper_level = false;
uint8_t beeper_duration = 0;


// ISR related variables
uint8_t lcd_timer = 0;


// Status screen drawer variables
uint8_t lcd_status_message_level;
char lcd_status_message[LCD_WIDTH+1] = WELCOME_MSG;

// View drawers variables
uint8_t display_view_menu_offset = 0;
uint8_t display_view_wizard_page = 0;


// TODO : Review & delete if possible
// Variables used when editing values.
const char* editLabel;
void* editValue;
int32_t minEditValue, maxEditValue;

#    if (SDCARDDETECT > 0)
bool lcd_oldcardstatus;
#    endif // (SDCARDDETECT > 0)
// 

// TODO : Review structure of this file to include it above.
#include "ultralcd_implementation_hitachi_HD44780.h"
//



/*******************************************************************************
**   Internal function declarations
*******************************************************************************/

// Status screen implementation functions
void draw_status_screen();
static void view_status_screen();

// Menu implementation functions
void draw_menu_main();
static void view_menu_main();

  // When the printer is stopped
  void draw_menu_sdcard();
  static void view_menu_sdcard();
    static void function_sdcard_refresh();
    static void function_menu_sdcard_updir();

  // When the printer is running
  static void function_sdcard_pause();
  static void function_sdcard_stop();
  void draw_menu_stop_confirm();
  static void lcd_stop_confirm();

  // When the printer is stopped
  void draw_menu_control();
  static void view_menu_control();

    // TODO: Review this submenu implementation
    void draw_menu_filament();
    static void view_menu_filament();
    
      void draw_menu_filament_load();
      static void view_menu_filament_load();

        void draw_menu_filament_insert();
        static void view_menu_filament_insert();

      void draw_menu_filament_unload();
      static void view_menu_filament_unload();
    //
    
    // TODO: Review this submenu implementation
    void draw_menu_move();
    static void view_menu_move();
      static void view_menu_move_jog();
        static void function_move_10mm();
        static void function_move_1mm();
        static void function_move_01mm();
          static void view_menu_move_axis();
            static void draw_picture_move_x();
            static void view_picture_move_x();
            static void draw_picture_move_y();
            static void view_picture_move_y();
            static void draw_picture_move_z();
            static void view_picture_move_z();
            static void draw_picture_move_e();
            static void view_picture_move_e();
    //

    static void function_config_level_bed();
      void draw_wizard_level_bed();
      static void view_wizard_level_bed();
      static void draw_picture_level_bed_cooling();
      static void view_picture_level_bed_cooling();

    static void function_control_preheat();
    static void function_control_cooldown();

    // When the printer is running
    static void function_prepare_change_filament();
    void draw_wizard_change_filament();
    static void view_wizard_change_filament();

    void draw_picture_set_temperature();
    static void view_picture_set_temperature();

#ifdef HEATED_BED_SUPPORT
    void draw_picture_set_temperature_bed();
    static void view_picture_set_temperature_bed();
#endif

    void draw_picture_set_feedrate();
    static void view_picture_speed_printing();

  void draw_picture_splash();
  static void view_picture_splash();



/*******************************************************************************
**   Function definitions
*******************************************************************************/

//
// General API definitions
// 

void lcd_init()
{
    // Low level init libraries for lcd & encoder
    lcd_implementation_init();

    pinMode(BTN_EN1,INPUT);
    pinMode(BTN_EN2,INPUT);
    WRITE(BTN_EN1,HIGH);
    WRITE(BTN_EN2,HIGH);

    pinMode(BTN_ENC,INPUT);
    WRITE(BTN_ENC,HIGH);

    // Init for SD card library
#  if (defined (SDSUPPORT) && defined(SDCARDDETECT) && (SDCARDDETECT > 0))
    pinMode(SDCARDDETECT,INPUT);
    WRITE(SDCARDDETECT, HIGH);
    lcd_oldcardstatus = IS_SD_INSERTED;
#  endif // (defined (SDSUPPORT) && defined(SDCARDDETECT) && (SDCARDDETECT > 0))

    // Init Timer 2 and set the OVF interrupt
    TCCR2A = 0x23;
    TCCR2B = 0x02;
    lcd_enable_interrupt();

    display_view_next = view_status_screen;
    display_view = NULL;

    display_time_refresh = millis();
    display_timeout = millis();
    lcd_enable_display_timeout();

    lcd_enable_button();
    lcd_get_button_updated();

    lcd_enable_encoder();
    lcd_get_encoder_updated();
    encoder_position = 0;

    lcd_get_button_clicked();
}


// Interrupt-driven functions
static void lcd_update_button()
{
    static uint16_t button_pressed_count = 0;

    // Read the hardware
    button_input = lcd_implementation_update_buttons();

    // Process button click/keep-press events
    bool button_clicked = ((button_input & EN_C) && (~(button_input_last) & EN_C));
    bool button_pressed = ((button_input & EN_C) && (button_input_last & EN_C));

    if (button_pressed == true) {
        button_pressed_count++;        
    } else { 
        button_pressed_count = 0;
    }

    // Beeper feedback
    if ((button_clicked == true) || (button_pressed_count > 10))
        lcd_implementation_quick_feedback();  // Review code!!


    // Update button trigger
    if ((button_clicked == true) && (button_input_blocked == false)) {
        button_clicked_triggered = true;
    }

    if ((button_input != button_input_last) && (button_input_blocked == false)) {
        button_input_updated = true;
    }

    button_input_last = button_input;
}

static void lcd_update_encoder()
{
    // Read the input hardware
    encoder_input = lcd_implementation_update_buttons();

    // Process rotatory encoder events if they are not disabled 
    if (encoder_input != encoder_input_last && encoder_input_blocked == false) {
        prev_encoder_position = encoder_position;
        switch (encoder_input & (EN_A | EN_B)) {
        case encrot0:
            if ( (encoder_input_last & (EN_A | EN_B)) == encrot3 )
                encoder_position++;
            else if ( (encoder_input_last & (EN_A | EN_B)) == encrot1 )
                encoder_position--;
            break;
        
        case encrot1:
            if ( (encoder_input_last & (EN_A | EN_B)) == encrot0 )
                encoder_position++;
            else if ( (encoder_input_last & (EN_A | EN_B)) == encrot2 )
                encoder_position--;
            break;
        
        case encrot2:
            if ( (encoder_input_last & (EN_A | EN_B)) == encrot1 )
                encoder_position++;
            else if ( (encoder_input_last & (EN_A | EN_B)) == encrot3 )
                encoder_position--;
            break;
        
        case encrot3:
            if ( (encoder_input_last & (EN_A | EN_B)) == encrot2 )
                encoder_position++;
            else if ( (encoder_input_last & (EN_A | EN_B)) == encrot0 )
                encoder_position--;
            break;
        }

        encoder_input_updated = true;
    }

    // Update the phases
    encoder_input_last = encoder_input;
}

void lcd_update()
{
#  if (SDCARDDETECT > 0)
    if ((IS_SD_INSERTED != lcd_oldcardstatus)) {
        display_refresh_mode = CLEAR_AND_UPDATE_SCREEN;
        lcd_oldcardstatus = IS_SD_INSERTED;
        lcd_implementation_init(); // to maybe revive the LCD if static electricity killed it.

        if(lcd_oldcardstatus) {
            card.initsd();
            LCD_MESSAGEPGM(MSG_SD_INSERTED);
        } else {
            card.release();
            LCD_MESSAGEPGM(MSG_SD_REMOVED);
        }
    }
#  endif // (SDCARDDETECT > 0)

    if ( display_view == view_status_screen || display_timeout_blocked || button_input_updated || encoder_input_updated ) {
        display_timeout = millis() + LCD_TIMEOUT_STATUS;
    }

    if (display_timeout < millis())
        lcd_set_status_screen();

    if (display_view != display_view_next) {
        display_refresh_mode = CLEAR_AND_UPDATE_SCREEN;
        lcd_clear_triggered_flags();
    }

    display_view = display_view_next;
    (*display_view)();
}


//
void lcd_set_status_screen()
{
    display_view_next = view_status_screen;

    lcd_clear_triggered_flags();
}
void lcd_set_menu(view_t menu)
{
    display_view_next = menu;
    display_view_menu_offset = 0;
    
    encoder_position = 0;

    lcd_clear_triggered_flags();
}
void lcd_set_picture(view_t picture)
{
    display_view_next = picture;

    lcd_clear_triggered_flags();
}
void lcd_set_wizard(view_t wizard)
{
    display_view_next = wizard;
    lcd_wizard_set_page(0);

    lcd_clear_triggered_flags();
}

// Get and clear trigger functions
bool lcd_get_button_updated() {
    bool status = button_input_updated;
    button_input_updated = false;
    return status;
}

bool lcd_get_encoder_updated()
{
    bool status = encoder_input_updated;
    encoder_input_updated = false;
    return status;
}
bool lcd_get_button_clicked(){
    bool status = button_clicked_triggered;
    button_clicked_triggered = false;
    return status;
}
void lcd_clear_triggered_flags() {
    button_input_updated = false;
    encoder_input_updated = false;
    button_clicked_triggered = false;
}


// Enable/disable functions
void lcd_enable_button() {
    button_input = lcd_implementation_update_buttons();
    button_input_last = button_input;
    button_input_blocked = false;
}
void lcd_disable_button() {
    button_input_blocked = true;
}

void lcd_enable_encoder()
{
    encoder_input = lcd_implementation_update_buttons();
    encoder_input_last = encoder_input;
    encoder_input_blocked = false;
}
void lcd_disable_encoder()
{
    encoder_input_blocked = true;
}

void lcd_enable_display_timeout()
{
    display_timeout_blocked = false;
}
void lcd_disable_display_timeout()
{
    display_timeout_blocked = true;
}

void lcd_enable_interrupt()
{
    TIMSK2 |= 0x01;
    lcd_enable_button();
    lcd_enable_encoder();
}

void lcd_disable_interrupt()
{
    lcd_disable_button();
    lcd_disable_encoder();
    TIMSK2 &= ~(0x01);
}


// Control flow functions
void lcd_wizard_set_page(uint8_t page)
{
    display_view_wizard_page = page;
    display_refresh_mode = CLEAR_AND_UPDATE_SCREEN;
}


// Temporal
void lcd_beep()
{
    lcd_implementation_quick_feedback();
}

void lcd_set_refresh(uint8_t mode)
{
    display_refresh_mode = mode;
}

uint8_t lcd_get_encoder_position()
{
    return encoder_position;
}



// Alert/status messages
void lcd_setstatus(const char* message)
{
    if (lcd_status_message_level > 0)
        return;
    strncpy(lcd_status_message, message, LCD_WIDTH);
}

void lcd_setstatuspgm(const char* message)
{
    if (lcd_status_message_level > 0)
        return;
    strncpy_P(lcd_status_message, message, LCD_WIDTH);
}

void lcd_setalertstatuspgm(const char* message)
{
    lcd_setstatuspgm(message);
    lcd_status_message_level = 1;

    display_view_next = view_status_screen;

}

void lcd_reset_alert_level()
{
    lcd_status_message_level = 0;
}

static void lcd_set_encoder_position(int8_t position)
{
    encoder_position = position;
}


/* Helper macros for menus */
#define START_MENU() do {\
    lcd_disable_encoder();\
    if (encoder_position > 0x8000) encoder_position = 0;\
    if (encoder_position / ENCODER_STEPS_PER_MENU_ITEM < display_view_menu_offset) {\
        display_view_menu_offset = encoder_position / ENCODER_STEPS_PER_MENU_ITEM;\
        display_refresh_mode == CLEAR_AND_UPDATE_SCREEN;\
    }\
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {\
        lcd_implementation_clear();\
        display_refresh_mode = UPDATE_SCREEN;\
    } else if (lcd_get_button_updated() || lcd_get_encoder_updated()) {\
        display_refresh_mode = UPDATE_SCREEN;\
    }\
    uint8_t _lineNr = display_view_menu_offset, _menuItemNr; \
    for(uint8_t _drawLineNr = 0; _drawLineNr < LCD_HEIGHT; _drawLineNr++, _lineNr++) { \
        _menuItemNr = 0;     
#define MENU_ITEM(type, label, args...) do { \
    if (_menuItemNr == _lineNr) { \
        if (display_refresh_mode == UPDATE_SCREEN) {\
            const char* _label_pstr = PSTR(label); \
            if ((encoder_position / ENCODER_STEPS_PER_MENU_ITEM) == _menuItemNr) { \
                lcd_implementation_drawmenu_ ## type ## _selected (_drawLineNr, _label_pstr , ## args ); \
            }else{\
                lcd_implementation_drawmenu_ ## type (_drawLineNr, _label_pstr , ## args ); \
            }\
        }\
        if (((encoder_position / ENCODER_STEPS_PER_MENU_ITEM) == _menuItemNr) && lcd_get_button_clicked()) {\
            menu_action_ ## type ( args );\
            return;\
        }\
    }\
    _menuItemNr++;\
} while(0)
#define MENU_ITEM_DUMMY() do { _menuItemNr++; } while(0)
#define MENU_ITEM_EDIT(type, label, args...) MENU_ITEM(setting_edit_ ## type, label, PSTR(label) , ## args )
#define MENU_ITEM_EDIT_CALLBACK(type, label, args...) MENU_ITEM(setting_edit_callback_ ## type, label, PSTR(label) , ## args )
#define END_MENU() \
    if (encoder_position / ENCODER_STEPS_PER_MENU_ITEM >= _menuItemNr) encoder_position = _menuItemNr * ENCODER_STEPS_PER_MENU_ITEM - 1; \
    if ((uint8_t)(encoder_position / ENCODER_STEPS_PER_MENU_ITEM) >= display_view_menu_offset + LCD_HEIGHT) { display_view_menu_offset = (encoder_position / ENCODER_STEPS_PER_MENU_ITEM) - LCD_HEIGHT + 1; _lineNr = display_view_menu_offset - 1; _drawLineNr = -1; } \
    lcd_enable_encoder();\
    } } while(0)

// Menu Drawers
static void menu_action_back(func_t function);
static void menu_action_submenu(func_t function);
static void menu_action_gcode(const char* pgcode);
static void menu_action_function(func_t function);
static void menu_action_wizard(func_t function);
static void menu_action_sdfile(const char* filename, char* longFilename);
static void menu_action_sddirectory(const char* filename, char* longFilename);
static void menu_action_setting_edit_bool(const char* pstr, bool* ptr);
static void menu_action_setting_edit_int3(const char* pstr, int* ptr, int minValue, int maxValue);
static void menu_action_setting_edit_float3(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_float32(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_float5(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_float51(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_float52(const char* pstr, float* ptr, float minValue, float maxValue);
static void menu_action_setting_edit_long5(const char* pstr, unsigned long* ptr, unsigned long minValue, unsigned long maxValue);

//
// Drawers 
//

// Status Screen Drawers

void draw_status_screen() {
    lcd_set_status_screen();
    lcd_update();
}

static void view_status_screen()
{
    // Setting the refresh mode
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_time_refresh < millis())
        display_refresh_mode = UPDATE_SCREEN;

    // Printing the view
    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_status_screen();
        display_time_refresh = millis() + LCD_REFRESH;
        display_refresh_mode = NO_UPDATE_SCREEN;
    }

    // Change conditions
    if (lcd_get_button_clicked()) {
        draw_menu_main();
    } 
}

// Views
void draw_menu_main()
{
    lcd_set_menu(view_menu_main);
}
static void view_menu_main()
{
    START_MENU();
    MENU_ITEM(back, MSG_BACK, draw_status_screen);

#ifdef SDSUPPORT
    if (card.cardOK) {
        if (card.isFileOpen()) {
            if (card.sdprinting)
		MENU_ITEM(function, MSG_PAUSE_PRINT, function_sdcard_pause);
            MENU_ITEM(function, MSG_STOP_PRINT, draw_menu_stop_confirm);
        } else {
            MENU_ITEM(submenu, MSG_CARD_MENU, draw_menu_sdcard);

#  if (SDCARDDETECT < 1)
            MENU_ITEM(gcode, MSG_CNG_SDCARD, PSTR("M21"));  // SD-card changed by user
#  endif (SDCARDDETECT < 1)
        }

    } else {
        MENU_ITEM(submenu, MSG_NO_CARD, draw_menu_sdcard);

#  if (SDCARDDETECT < 1)
        MENU_ITEM(gcode, MSG_INIT_SDCARD, PSTR("M21")); // Manually initialize the SD-card via user interface
#  endif // (SDCARDDETECT < 1)
    }
#endif // SDSUPPORT

#ifdef WITBOX
    if (movesplanned() || IS_SD_PRINTING) {

#  ifdef FILAMENTCHANGEENABLE
        MENU_ITEM(function, MSG_FILAMENTCHANGE, function_prepare_change_filament);
#  endif // FILAMENTCHANGEENABLE

       MENU_ITEM(submenu, MSG_NOZZLE, draw_picture_set_temperature);

#ifdef HEATED_BED_SUPPORT
       MENU_ITEM(submenu, MSG_BED, draw_picture_set_temperature_bed);
#endif // HEATED_BED_SUPPORT

       MENU_ITEM(submenu, MSG_SPEED, draw_picture_set_feedrate);
    } else {
        MENU_ITEM(submenu, MSG_CONTROL, draw_menu_control);
    }
#endif // WITBOX

    MENU_ITEM(wizard, "FW info", draw_picture_splash);
    END_MENU();
}

void draw_menu_sdcard()
{
    lcd_set_menu(view_menu_sdcard);
}
static void view_menu_sdcard()
{
    uint16_t fileCnt = card.getnrfilenames();
    START_MENU();
    MENU_ITEM(back, MSG_BACK, draw_menu_main);
    card.getWorkDirName();
    if(card.filename[0]=='/') {

#if (SDCARDDETECT == -1)
        MENU_ITEM(function, LCD_STR_REFRESH MSG_REFRESH, function_sdcard_refresh);
#endif // (SDCARDDETECT == -1)

    } else {
        MENU_ITEM(function, LCD_STR_FOLDER "..", function_menu_sdcard_updir);
    }

    for(uint16_t i=0;i<fileCnt;i++) {
        if (_menuItemNr == _lineNr) {

#ifndef SDCARD_RATHERRECENTFIRST
            card.getfilename(i);
#else // SDCARD_RATHERRECENTFIRST
            card.getfilename(fileCnt-1-i);
#endif // SDCARD_RATHERRECENTFIRST

            if (card.filenameIsDir) {
                MENU_ITEM(sddirectory, MSG_CARD_MENU, card.filename, card.longFilename);
            } else {
                MENU_ITEM(sdfile, MSG_CARD_MENU, card.filename, card.longFilename);
            }
        } else {
            MENU_ITEM_DUMMY();
        }
    }
    END_MENU();
}

#if (SDCARDDETECT == -1)
static void function_sdcard_refresh()
{
    card.initsd();
    display_view_menu_offset = 0;
}
#endif (SDCARDDETECT == -1)

static void function_menu_sdcard_updir()
{
    card.updir();
    display_view_menu_offset = 0;
}

static void function_sdcard_pause()
{
    LCD_MESSAGEPGM(MSG_PAUSING);
    lcd_disable_button();

    stop_buffer = true;
    stop_buffer_code = 1;
//   card.pauseSDPrint();

    draw_status_screen();
}

static void function_sdcard_stop()
{
    if (buffer_recursivity > 0) {
        display_view_next = function_sdcard_stop;
    } else {

        card.sdprinting = false;
        card.closefile();

        setTargetHotend(0,0);
#    ifdef WITBOX_DUAL
        setTargetHotend(0,1);
#    endif // WITBOX_DUAL

#ifdef HEATED_BED_SUPPORT
        setTargetBed(0);
#endif

        flush_commands();
        quickStop();

        LCD_MESSAGEPGM(WELCOME_MSG);
        draw_status_screen();
        lcd_update();

        plan_reset_position();

#    if X_MAX_POS < 250
        current_position[X_AXIS] = X_MIN_POS;
        current_position[Y_AXIS] = 150;
        current_position[Z_AXIS] += 20;
#    else // X_MAX_POS < 250
        current_position[X_AXIS] = X_MAX_POS - 15;
        current_position[Y_AXIS] = Y_MAX_POS - 15;
        current_position[Z_AXIS] = Z_MAX_POS - 15;
#    endif // X_MAX_POS < 250
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

        st_synchronize();

        if (SD_FINISHED_STEPPERRELEASE) {
            enquecommand_P(PSTR(SD_FINISHED_RELEASECOMMAND));
        }
        autotempShutdown();

        cancel_heatup = true;
    }
}

void draw_menu_stop_confirm()
{
    lcd_set_menu(lcd_stop_confirm);
}

static void lcd_stop_confirm()
{
  START_MENU();
  MENU_ITEM(back, MSG_BACK, draw_status_screen);
  MENU_ITEM(function, MSG_STOP_PRINT, function_sdcard_stop);
  END_MENU();
}

void draw_menu_control()
{
    lcd_set_menu(view_menu_control);
}

static void view_menu_control()
{
    START_MENU();

    MENU_ITEM(back, MSG_BACK, draw_menu_main);
    MENU_ITEM(submenu, MSG_FILAMENT, draw_menu_filament);
    MENU_ITEM(submenu, MSG_MOVE_AXIS, view_menu_move);
    MENU_ITEM(function, MSG_LEVEL_PLATE, function_config_level_bed); 
    if (target_temperature[0] > 10)
        MENU_ITEM(function, MSG_COOLDOWN, function_control_cooldown);
    else
        MENU_ITEM(function, MSG_PREHEAT, function_control_preheat);
    END_MENU();
}

void draw_menu_filament()
{
    lcd_enable_display_timeout();
    lcd_set_menu(view_menu_filament);
}

static void view_menu_filament()
{
    START_MENU();
    MENU_ITEM(back, MSG_BACK, draw_menu_control);
    MENU_ITEM(submenu, MSG_LOAD, draw_menu_filament_load);
    MENU_ITEM(submenu, MSG_UNLOAD, draw_menu_filament_unload);
    END_MENU();   
}

void draw_menu_filament_load()
{
    setTargetHotend0(Change_Filament_Target_Temp);
    fanSpeed = PREHEAT_FAN_SPEED;

    lcd_disable_display_timeout();

    lcd_set_menu(view_menu_filament_load);
}

static void view_menu_filament_load()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_time_refresh < millis()) {
        display_time_refresh = millis() + LCD_REFRESH;
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        START_MENU();
        MENU_ITEM(back, MSG_ABORT, draw_menu_filament);
        END_MENU();  
    
        int tHotend=int(degHotend(0) + 0.5);
        int tTarget=int(degTargetHotend(0) + 0.5);
        lcd_implementation_set_cursor(3, 2);
        lcd_implementation_print_P(PSTR(MSG_HEATING));
        lcd_implementation_set_cursor(5, 3);
        lcd_implementation_print(LCD_STR_THERMOMETER[0]);
        lcd_implementation_print(itostr3(tHotend));
        lcd_implementation_print('/');
        lcd_implementation_print(itostr3left(tTarget));
        lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
    }

    if ((degHotend(0) > degTargetHotend(0))) {
        lcd_implementation_quick_feedback();
        lcd_enable_display_timeout();
        draw_menu_filament_insert();
    }

    display_refresh_mode = NO_UPDATE_SCREEN;
}

void draw_menu_filament_insert()
{
    lcd_disable_display_timeout();
    lcd_set_menu(view_menu_filament_insert);
}

static void view_menu_filament_insert()
{
    START_MENU();
    MENU_ITEM(back, MSG_BACK, draw_menu_filament);
    active_extruder = 0;
    MENU_ITEM(gcode, MSG_PRE_EXTRUD, PSTR("M701"));
    END_MENU();
}

void draw_menu_filament_unload()
{
    setTargetHotend0(Change_Filament_Target_Temp);
    fanSpeed = PREHEAT_FAN_SPEED;

    lcd_disable_display_timeout();

    lcd_set_menu(view_menu_filament_unload);
}

static void view_menu_filament_unload()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_time_refresh < millis()) {
        display_time_refresh = millis() + LCD_REFRESH;
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        START_MENU();
        MENU_ITEM(back, MSG_ABORT, draw_menu_filament);
        END_MENU();  
    
        int tHotend=int(degHotend(0) + 0.5);
        int tTarget=int(degTargetHotend(0) + 0.5);
        lcd_implementation_set_cursor(3, 2);
        lcd_implementation_print_P(PSTR(MSG_HEATING));
        lcd_implementation_set_cursor(5, 3);
        lcd_implementation_print(LCD_STR_THERMOMETER[0]);
        lcd_implementation_print(itostr3(tHotend));
        lcd_implementation_print('/');
        lcd_implementation_print(itostr3left(tTarget));
        lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
    }

    if (degHotend(0) > degTargetHotend(0))
    {
        lcd_implementation_quick_feedback();

        //-- Ejecutar gcode
        active_extruder = 0;
        enquecommand_P(PSTR("M702"));

        lcd_enable_display_timeout();
        draw_menu_filament();
    }

    display_refresh_mode = NO_UPDATE_SCREEN;
}

// MOVE AXIS
void draw_menu_move()
{
    lcd_set_menu(view_menu_move);
}

static void view_menu_move()
{
    START_MENU();

    MENU_ITEM(back, MSG_BACK, draw_menu_control);
    MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
    MENU_ITEM(gcode, MSG_DISABLE_STEPPERS, PSTR("M84"));    
    MENU_ITEM(submenu, MSG_JOG, view_menu_move_jog);

    END_MENU();
}

void draw_menu_move_jog()
{
    lcd_set_menu(view_menu_move_jog);
}

static void view_menu_move_jog()
{
    START_MENU();
    MENU_ITEM(back, MSG_BACK, draw_menu_move);
    MENU_ITEM(function, MSG_MOVE_10MM, function_move_10mm);
    MENU_ITEM(function, MSG_MOVE_1MM, function_move_1mm);
    MENU_ITEM(function, MSG_MOVE_01MM, function_move_01mm);
    END_MENU();
}

static void function_move_10mm()
{
    move_menu_scale = 10.0;
    lcd_set_menu(view_menu_move_axis);
}
static void function_move_1mm()
{
    move_menu_scale = 1.0;
    lcd_set_menu(view_menu_move_axis);
}
static void function_move_01mm()
{
    move_menu_scale = 0.1;
    lcd_set_menu(view_menu_move_axis);
}

static void view_menu_move_axis()
{
    START_MENU();
    MENU_ITEM(back, MSG_BACK, draw_menu_move_jog);
    MENU_ITEM(submenu, MSG_MOVE_X, draw_picture_move_x);
    MENU_ITEM(submenu, MSG_MOVE_Y, draw_picture_move_y);
    if (move_menu_scale < 10.0) {
        MENU_ITEM(submenu, MSG_MOVE_Z, draw_picture_move_z);
        MENU_ITEM(submenu, MSG_MOVE_E, draw_picture_move_e);
    }
    END_MENU();
}

void draw_picture_move_x()
{
    lcd_disable_display_timeout();
    display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
    lcd_set_picture(view_picture_move_x);
}
static void view_picture_move_x()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }
    if (lcd_get_encoder_updated()) {
        current_position[X_AXIS] += float((int)encoder_position) * move_menu_scale;
        if (min_software_endstops && current_position[X_AXIS] < X_MIN_POS)
            current_position[X_AXIS] = X_MIN_POS;
        if (max_software_endstops && current_position[X_AXIS] > X_MAX_POS)
            current_position[X_AXIS] = X_MAX_POS;
        encoder_position = 0;

        display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_drawedit(PSTR("X"), ftostr31(current_position[X_AXIS]));
    }

    if (display_time_refresh < millis()) {
        #ifdef DELTA
        calculate_delta(current_position);
        plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
        #else
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
        #endif
        display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
    }

    if (LCD_CLICKED) {
        lcd_set_menu(view_menu_move_axis);
        lcd_enable_display_timeout();
    }
}

void draw_picture_move_y()
{
    lcd_disable_display_timeout();
    display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
    lcd_set_picture(view_picture_move_y);
}
static void view_picture_move_y()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }
    if (lcd_get_encoder_updated()) {
        current_position[Y_AXIS] += float((int)encoder_position) * move_menu_scale;
        if (min_software_endstops && current_position[Y_AXIS] < Y_MIN_POS)
            current_position[Y_AXIS] = Y_MIN_POS;
        if (max_software_endstops && current_position[Y_AXIS] > Y_MAX_POS)
            current_position[Y_AXIS] = Y_MAX_POS;
        encoder_position = 0;

        display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_drawedit(PSTR("Y"), ftostr31(current_position[Y_AXIS]));
    }

    if (display_time_refresh < millis()) {
        #ifdef DELTA
        calculate_delta(current_position);
        plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
        #else
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
        #endif
        display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
    }

    if (LCD_CLICKED) {
        lcd_set_menu(view_menu_move_axis);
        lcd_enable_display_timeout();
    }
}

void draw_picture_move_z()
{
    lcd_disable_display_timeout();
    display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
    lcd_set_picture(view_picture_move_z);
}
static void view_picture_move_z()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }
    if (lcd_get_encoder_updated()) {
        current_position[Z_AXIS] += float((int)encoder_position) * move_menu_scale;
        if (min_software_endstops && current_position[Z_AXIS] < Z_MIN_POS)
            current_position[Z_AXIS] = Z_MIN_POS;
        if (max_software_endstops && current_position[Z_AXIS] > Z_MAX_POS)
            current_position[Z_AXIS] = Z_MAX_POS;
        encoder_position = 0;

        display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_drawedit(PSTR("Z"), ftostr31(current_position[Z_AXIS]));
    }

    if (display_time_refresh < millis()) {
        #ifdef DELTA
        calculate_delta(current_position);
        plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
        #else
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
        #endif
        display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
    }

    if (LCD_CLICKED) {
        lcd_set_menu(view_menu_move_axis);
        lcd_enable_display_timeout();
    }
}

void draw_picture_move_e()
{
    lcd_disable_display_timeout();
    display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
    lcd_set_picture(view_picture_move_e);
}
static void view_picture_move_e()
{
if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }
    if (lcd_get_encoder_updated()) {
        current_position[E_AXIS] += float((int)encoder_position) * move_menu_scale;
        encoder_position = 0;

        display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_drawedit(PSTR("Extruder"), ftostr31(current_position[E_AXIS]));
    }

    if (display_time_refresh < millis()) {
        #ifdef DELTA
        calculate_delta(current_position);
        plan_buffer_line(delta[X_AXIS], delta[Y_AXIS], delta[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
        #else
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
        #endif
        display_time_refresh = millis() + LCD_MOVE_RESIDENCY_TIME;
    }

    if (LCD_CLICKED) {
        lcd_set_menu(view_menu_move_axis);
        lcd_enable_display_timeout();
    }
}

static void function_config_level_bed()
{
    setTargetHotend(0,0);
    
    if (degHotend(0) < LEVEL_PLATE_TEMP_PROTECTION) {
        draw_wizard_level_bed();
    } else {
        draw_picture_level_bed_cooling();      
    }   
}

void draw_wizard_level_bed()
{
    enquecommand_P(PSTR("M700"));

    lcd_disable_display_timeout();
    lcd_set_wizard(view_wizard_level_bed);
}

static void view_wizard_level_bed()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        switch (display_view_wizard_page) {
             case 0:
                #ifdef MSG_WIZARD_LEVELBED_0_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_0_0));
                #endif

                #ifdef MSG_WIZARD_LEVELBED_0_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_0_1));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_0_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_0_2));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_0_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_0_3));
                #endif 
                break;

            case 1:
                #ifdef MSG_WIZARD_LEVELBED_1_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_1_0));
                #endif

                #ifdef MSG_WIZARD_LEVELBED_1_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_1_1));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_1_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_1_2));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_1_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_1_3));
                #endif 
                break;

            case 2:
                #ifdef MSG_WIZARD_LEVELBED_2_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_2_0));
                #endif

                #ifdef MSG_WIZARD_LEVELBED_2_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_2_1));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_2_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_2_2));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_2_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_2_3));
                #endif 
                break;

            case 3:
                #ifdef MSG_WIZARD_LEVELBED_3_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_3_0));
                #endif

                #ifdef MSG_WIZARD_LEVELBED_3_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_3_1));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_3_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_3_2));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_3_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_3_3));
                #endif 
                break;

            case 4:
                #ifdef MSG_WIZARD_LEVELBED_4_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_4_0));
                #endif

                #ifdef MSG_WIZARD_LEVELBED_4_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_4_1));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_4_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_4_2));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_4_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_4_3));
                #endif 
                break;

            case 5:
                #ifdef MSG_WIZARD_LEVELBED_5_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_5_0));
                #endif

                #ifdef MSG_WIZARD_LEVELBED_5_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_5_1));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_5_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_5_2));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_5_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_5_3));
                #endif 
                break;

            case 6:
                #ifdef MSG_WIZARD_LEVELBED_6_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_6_0));
                #endif

                #ifdef MSG_WIZARD_LEVELBED_6_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_6_1));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_6_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_6_2));
                #endif                

                #ifdef MSG_WIZARD_LEVELBED_6_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_LEVELBED_6_3));
                #endif 
                break;

            default:
                lcd_enable_display_timeout();
                draw_status_screen();
                break;
        }
        display_refresh_mode = NO_UPDATE_SCREEN;
    }
}

void draw_picture_level_bed_cooling()
{
    fanSpeed = COOLDOWN_FAN_SPEED;

    lcd_disable_display_timeout();
    lcd_set_picture(view_picture_level_bed_cooling);
}

static void view_picture_level_bed_cooling()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_time_refresh < millis()) {
        display_time_refresh = millis() + LCD_REFRESH;
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_set_cursor(0, 0);
        lcd_implementation_print_P(PSTR(MSG_LP_COOL_1));
        lcd_implementation_set_cursor(1, 0);
        lcd_implementation_print_P(PSTR(MSG_LP_COOL_2));
        lcd_implementation_set_cursor(1, 6);
        lcd_implementation_print(LCD_STR_THERMOMETER[0]);
        lcd_implementation_print(itostr3(int(degHotend(0))));
        lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
        lcd_implementation_set_cursor(3,0);
        lcd_implementation_print_P(PSTR(MSG_LP_COOL_3));
    }

    if (degHotend(0) < LEVEL_PLATE_TEMP_PROTECTION) {
        fanSpeed = PREHEAT_FAN_SPEED;

        lcd_enable_display_timeout();
        lcd_implementation_quick_feedback();
        
        function_config_level_bed();
        return;
    }

    if (lcd_get_button_clicked()) {
        fanSpeed = COOLDOWN_FAN_SPEED;

        lcd_enable_display_timeout();
        lcd_set_status_screen();
    }

    display_refresh_mode == NO_UPDATE_SCREEN;
    lcd_clear_triggered_flags();
}


static void function_control_preheat()
{
    setTargetHotend0(PREHEAT_HOTEND_TEMP);

#ifdef HEATED_BED_SUPPORT
    setTargetBed(HBP_PREHEAT_TEMP);
#endif // HEATED_BED_SUPPORT
    
    fanSpeed = PREHEAT_FAN_SPEED;
    setWatch(); // heater sanity check timer
    
    draw_status_screen();
    lcd_update();
}

static void function_control_cooldown()
{
    setTargetHotend0(0);
    setTargetHotend1(0);
    setTargetHotend2(0);
    setTargetBed(0);

    fanSpeed = COOLDOWN_FAN_SPEED;

    draw_status_screen();
    lcd_update();
}

static void function_prepare_change_filament()
{
    stop_buffer = true;
    stop_buffer_code = 2;

    LCD_MESSAGEPGM(MSG_PAUSING);
    draw_status_screen();

    lcd_disable_button();
}

void draw_wizard_change_filament()
{
    lcd_set_wizard(view_wizard_change_filament);
    lcd_update();
}
static void view_wizard_change_filament()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        switch (display_view_wizard_page) {
            case 0:
                #ifdef MSG_WIZARD_CHANGEFILAMENT_0_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_0_0));
                #endif

                #ifdef MSG_WIZARD_CHANGEFILAMENT_0_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_0_1));
                #endif                

                #ifdef MSG_WIZARD_CHANGEFILAMENT_0_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_0_2));
                #endif                

                #ifdef MSG_WIZARD_CHANGEFILAMENT_0_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_0_3));
                #endif 
                break;

            case 1:
                #ifdef MSG_WIZARD_CHANGEFILAMENT_1_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_1_0));
                #endif

                #ifdef MSG_WIZARD_CHANGEFILAMENT_1_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_1_1));
                #endif                

                #ifdef MSG_WIZARD_CHANGEFILAMENT_1_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_1_2));
                #endif                

                #ifdef MSG_WIZARD_CHANGEFILAMENT_1_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_1_3));
                #endif 
                break;

            case 2:
                #ifdef MSG_WIZARD_CHANGEFILAMENT_2_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_2_0));
                #endif

                #ifdef MSG_WIZARD_CHANGEFILAMENT_2_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_2_1));
                #endif                

                #ifdef MSG_WIZARD_CHANGEFILAMENT_2_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_2_2));
                #endif                

                #ifdef MSG_WIZARD_CHANGEFILAMENT_2_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_2_3));
                #endif 
                break;

            case 3:
                #ifdef MSG_WIZARD_CHANGEFILAMENT_3_0
                lcd_implementation_set_cursor(0, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_3_0));
                #endif

                #ifdef MSG_WIZARD_CHANGEFILAMENT_3_1
                lcd_implementation_set_cursor(1, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_3_1));
                #endif                

                #ifdef MSG_WIZARD_CHANGEFILAMENT_3_2
                lcd_implementation_set_cursor(2, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_3_2));
                #endif                

                #ifdef MSG_WIZARD_CHANGEFILAMENT_3_3            
                lcd_implementation_set_cursor(3, 0);
                lcd_implementation_print_P(PSTR(MSG_WIZARD_CHANGEFILAMENT_3_3));
                #endif 
                break;

            default:
                lcd_enable_display_timeout();
                lcd_setstatuspgm(PSTR(MSG_PRINTING));
                draw_status_screen();
                break;
        }

        display_refresh_mode = NO_UPDATE_SCREEN;
    }
}

void draw_picture_set_temperature() 
{
    encoder_position = 0;
    lcd_set_picture(view_picture_set_temperature);
}

static void view_picture_set_temperature() 
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }
    
    if (lcd_get_encoder_updated()) {
        target_temperature[0] += int(encoder_position);
        encoder_position = 0;
        
        if (target_temperature[0] < EXTRUDE_MINTEMP) {
            target_temperature[0] = EXTRUDE_MINTEMP;
        }
        if (target_temperature[0] > (HEATER_0_MAXTEMP - 10)) {
            target_temperature[0] = (HEATER_0_MAXTEMP - 10);
        }

        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_set_cursor(1, 1);
        lcd_implementation_print_P(PSTR(MSG_NOZZLE));
        lcd_implementation_print(':');

        lcd_implementation_set_cursor(1, 17);
        lcd_implementation_print(itostr3(target_temperature[0]));    

    }

    if (lcd_get_button_clicked()) {
        draw_menu_main();
    }

    display_refresh_mode == NO_UPDATE_SCREEN;
}

#ifdef HEATED_BED_SUPPORT
void draw_picture_set_temperature_bed() 
{
    encoder_position = 0;
    lcd_set_picture(view_picture_set_temperature_bed);
}

static void view_picture_set_temperature_bed() 
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }
    
    if (lcd_get_encoder_updated()) {
        target_temperature_bed += int(encoder_position);
        encoder_position = 0;
        
        if (target_temperature_bed < (BED_MINTEMP + 10)) {
            target_temperature_bed = (BED_MINTEMP + 10);
        }
        if (target_temperature_bed > (BED_MAXTEMP - 10)) {
            target_temperature_bed = (BED_MAXTEMP - 10);
        }

        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_set_cursor(1, 1);
        lcd_implementation_print_P(PSTR(MSG_BED));
        lcd_implementation_print(':');

        lcd_implementation_set_cursor(1, 17);
        lcd_implementation_print(itostr3(target_temperature_bed));    

    }

    if (lcd_get_button_clicked()) {
        draw_menu_main();
    }

    display_refresh_mode == NO_UPDATE_SCREEN;
}
#endif

void draw_picture_set_feedrate()
{
    encoder_position = 0;
    lcd_set_picture(view_picture_speed_printing); 
}
static void view_picture_speed_printing()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (lcd_get_encoder_updated()) {
        feedmultiply += int(encoder_position);
        encoder_position = 0;
        
        if (feedmultiply < 10)
            feedmultiply = 10;
        if (feedmultiply > 999)
            feedmultiply = 999;

        display_refresh_mode = UPDATE_SCREEN;
    }
    
    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_set_cursor(1, 1);
        lcd_implementation_print_P(PSTR(MSG_SPEED));
        lcd_implementation_print(':');

        lcd_implementation_set_cursor(1, 17);
        lcd_implementation_print(itostr3(feedmultiply));    

    }

    if (lcd_get_button_clicked()) {
        draw_menu_main();
    }

    display_refresh_mode == NO_UPDATE_SCREEN;
}

void draw_picture_splash()
{
    lcd_set_picture(view_picture_splash);
}

static void view_picture_splash()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }
    
    if (display_refresh_mode == UPDATE_SCREEN) {
        lcd_implementation_set_cursor(1, 6);
        lcd_implementation_print_P(PSTR(MSG_WELLCOME));
#  ifndef WITBOX_DUAL
        lcd_implementation_set_cursor(3, 8);
        lcd_implementation_print_P(PSTR(FIRMWARE_VER));
#  else // WITBOX_DUAL
        lcd_implementation_set_cursor(3, 6);
        lcd_implementation_print_P(PSTR(FIRMWARE_VER));    
#  endif // WITBOX_DUAL
    }

    if (lcd_get_button_clicked()) {
        display_view_next = view_status_screen;
    }

    display_refresh_mode == NO_UPDATE_SCREEN;
    lcd_clear_triggered_flags();
}

static void menu_action_back(func_t function)
{
    (*function)();
    lcd_clear_triggered_flags();
}
static void menu_action_submenu(func_t function)
{
    display_view_next = function;
    display_view_menu_offset = 0;
    encoder_position = 0;
    lcd_clear_triggered_flags();
}
static void menu_action_gcode(const char* pgcode)
{
    enquecommand_P(pgcode);
}
static void menu_action_function(func_t function)
{
    (*function)();
    lcd_clear_triggered_flags();
}

static void menu_action_wizard(func_t function)
{
    display_view_next = function;
    lcd_wizard_set_page(0);
}


static void menu_action_sdfile(const char* filename, char* longFilename)
{
    char cmd[30];
    char* c;

    strcpy(cmd, longFilename);
    for (c = &cmd[0]; *c; c++) {
        if ((uint8_t)*c > 127) {
            SERIAL_ECHOLN(MSG_SD_BAD_FILENAME);
            LCD_MESSAGEPGM(MSG_SD_BAD_FILENAME);
            draw_status_screen();
            lcd_update();
            return;
        }
    }

    setTargetHotend0(200);
    sprintf_P(cmd, PSTR("M23 %s"), filename);

    enquecommand_P(PSTR("G28"));
    enquecommand_P(PSTR("G1 Z10"));

    for(c = &cmd[4]; *c; c++)
        *c = tolower(*c);

    enquecommand(cmd);
    enquecommand_P(PSTR("M24"));
    draw_status_screen();
}

static void menu_action_sddirectory(const char* filename, char* longFilename)
{
    card.chdir(filename);
    encoder_position = 0;
}

static void menu_action_setting_edit_bool(const char* pstr, bool* ptr)
{
    *ptr = !(*ptr);
}

#define menu_edit_type(_type, _name, _strFunc, scale) \
    void menu_edit_ ## _name () \
    { \
        if ((int32_t)encoder_position < minEditValue) \
            encoder_position = minEditValue; \
        if ((int32_t)encoder_position > maxEditValue) \
            encoder_position = maxEditValue; \
        if (1) \
            lcd_implementation_drawedit(editLabel, _strFunc(((_type)encoder_position) / scale)); \
        if (LCD_CLICKED) \
        { \
            *((_type*)editValue) = ((_type)encoder_position) / scale; \
            lcd_implementation_quick_feedback(); \
            display_view_next = display_view; \
            encoder_position = prev_encoder_position; \
        } \
    } \
    static void menu_action_setting_edit_ ## _name (const char* pstr, _type* ptr, _type minValue, _type maxValue) \
    { \
        display_view = display_view_next; \
        prev_encoder_position = encoder_position; \
         \
        display_view_next = menu_edit_ ## _name; \
         \
        editLabel = pstr; \
        editValue = ptr; \
        minEditValue = minValue * scale; \
        maxEditValue = maxValue * scale; \
        encoder_position = (*ptr) * scale; \
    }

menu_edit_type(int, int3, itostr3, 1)
menu_edit_type(float, float3, ftostr3, 1)
menu_edit_type(float, float32, ftostr32, 100)
menu_edit_type(float, float5, ftostr5, 0.01)
menu_edit_type(float, float51, ftostr51, 10)
menu_edit_type(float, float52, ftostr52, 100)
menu_edit_type(unsigned long, long5, ftostr5, 0.01)

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

char *itostr3(const int &x)
{
  int xx = x;
  if (xx < 0) {
     conv[0]='-';
     xx = -xx;
  } else if (xx >= 100)
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
    if (xx >= 100) {
        conv[0]=(xx/100)%10+'0';
        conv[1]=(xx/10)%10+'0';
        conv[2]=(xx)%10+'0';
        conv[3]=0;
    } else if (xx >= 10) {
        conv[0]=(xx/10)%10+'0';
        conv[1]=(xx)%10+'0';
        conv[2]=0;
    } else {
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

ISR(TIMER2_OVF_vect)
{
    lcd_timer++;

#if ( defined(BEEPER) && (BEEPER > 0) )
    if (beeper_duration) {
        beeper_level = !beeper_level;
        beeper_duration--;
    } else {
        beeper_level = false;
    }
    WRITE(BEEPER, beeper_level);
#endif // ( defined(BEEPER) && (BEEPER > 0) )

    if (lcd_timer % 8 == 0)
        lcd_update_encoder();

    if (lcd_timer % 400 == 0) {
        lcd_update_button();
        lcd_timer = 0;
    }
}
