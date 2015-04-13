#include "ultralcd.h"

#include "Marlin.h"
#include "cardreader.h"
#include "ConfigurationStore.h"
#include "temperature.h"
#include "language.h"

// Only for new LCD
typedef struct {
    uint8_t type;
    char text[18];
    func_p action;
    void * data;
} cache_entry;

// Only for new LCD
typedef struct {
    char filename[13];
    char longFilename[LONG_FILENAME_LENGTH];
} cache_entry_data;

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

//Filament loading/unloading
const uint8_t NORMAL = 0;
const uint8_t LOADING = 1;
const uint8_t UNLOADING = 2;
uint8_t filamentStatus = NORMAL;


// Display related variables
uint8_t    display_refresh_mode;
uint32_t   display_time_refresh = 0;

uint32_t   display_timeout;
bool       display_timeout_blocked;

view_t display_view_next;
view_t display_view;

uint32_t refresh_interval;

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
uint32_t beeper_duration = 0;
uint8_t beep_count, frequency_ratio;

// ISR related variables
uint16_t lcd_timer = 0;

// Status screen drawer variables
uint8_t lcd_status_message_level;
char lcd_status_message[LCD_WIDTH+1] = WELCOME_MSG;


// View drawers variables
uint8_t display_view_menu_offset = 0;
uint8_t display_view_wizard_page = 0;


// SD cacheable menu variables
cache_entry cache[MAX_CACHE_SIZE];
cache_entry_data cache_data[MAX_CACHE_SIZE];
const cache_entry* cache_index = &cache[0];

bool cache_update = false;
bool cache_menu_first_time = false;
bool folder_is_root = false;

int8_t list_length;
uint8_t window_size, cache_size;
int8_t item_selected, new_item_selected;
int8_t window_min, window_max, cache_min, cache_max;

uint8_t window_offset, cursor_offset;


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
#ifdef DOGLCD
#include "dogm_lcd_implementation.h"
#else
#include "ultralcd_implementation_hitachi_HD44780.h"
#endif
//



/*******************************************************************************
**   Internal function declarations
*******************************************************************************/

// Status screen implementation functions
void draw_status_screen();
static void view_status_screen();

// Menu implementation functions
void funct_draw_menu_main(void *);
void draw_menu_main();
static void view_menu_main();

  // When the printer is stopped
  void draw_menu_sdcard();
  static void view_menu_sdcard();
    static void function_sdcard_refresh();
    static void function_menu_sdcard_updir(void *);

  // When the printer is running
  static void function_sdcard_pause();
  static void function_sdcard_stop();
  void draw_menu_stop_confirm();
  static void lcd_stop_confirm();
  static void lcd_emergency_stop();

  // When the printer is stopped
  void draw_menu_control();
  static void view_menu_control();

    // TODO: Review this submenu implementation
    void draw_menu_filament();
    static void view_menu_filament();
    
      void draw_menu_filament_load();
      static void view_menu_filament_load();
	  void pre_filament_load();
	  void pre_filament_unload();

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

    // Init Timer 5 and set the OVF interrupt (triggered every 125 us)
    TCCR5A = 0x03;
    TCCR5B = 0x19;

    OCR5AH = 0x07;
    OCR5AL = 0xD0;

    lcd_enable_interrupt();

    display_view_next = view_status_screen;
    display_view = NULL;

    display_time_refresh = millis();
    display_timeout = millis();
    refresh_interval = millis();
    lcd_enable_display_timeout();

    lcd_enable_button();
    lcd_get_button_updated();

    lcd_enable_encoder();
    lcd_get_encoder_updated();
    encoder_position = 0;

    lcd_get_button_clicked();

    SERIAL_ECHOLN("LCD initialized!");
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
    if ((button_clicked == true) || (button_pressed_count > 50)) {
        lcd_implementation_quick_feedback();
	if (button_pressed_count == 200)
	    lcd_emergency_stop();
    }

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

void lcd_update(bool force)
{
#  if (SDCARDDETECT > 0)
    if ((lcd_oldcardstatus != IS_SD_INSERTED)) {
        display_refresh_mode = CLEAR_AND_UPDATE_SCREEN;
        lcd_oldcardstatus = IS_SD_INSERTED;

#ifndef DOGLCD
        lcd_implementation_init(); // to maybe revive the LCD if static electricity killed it.
#endif //DOGLCD

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
    
    if ( (IS_SD_PRINTING == true) && (!force) ) {
        if (refresh_interval < millis()) {
            (*display_view)();
            refresh_interval = millis() + LCD_REFRESH_LIMIT;
        }
    } else {
        (*display_view)();
    }
}

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


void lcd_disable_buzzer()
{
    beep_count = 0;
    beeper_duration = 0;
    beeper_level = false;
    WRITE(BEEPER, beeper_level);
}

// Enable/disable function
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
    TIMSK5 |= 0x01;
    lcd_enable_button();
    lcd_enable_encoder();
}

void lcd_disable_interrupt()
{
    lcd_disable_button();
    lcd_disable_encoder();
    lcd_disable_buzzer();
    TIMSK5 &= ~(0x01);
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

void lcd_beep_ms(uint16_t ms)
{
    frequency_ratio = 0;
    beeper_duration = 8 * ms;
    while (beeper_duration) {
        lcd_update();
    }
}

void lcd_beep_hz_ms(uint16_t frequency, uint16_t ms)
{
    frequency_ratio = (4000 / frequency) - 1;
    beeper_duration = 8 * ms;
    while (beeper_duration) {
        lcd_update();
    }
}

void lcd_set_refresh(uint8_t mode)
{
    display_refresh_mode = mode;
}

uint8_t lcd_get_encoder_position()
{
    return encoder_position;
}

#ifdef DOGLCD
void lcd_setcontrast(uint8_t value) {
    pinMode(39, OUTPUT);   //Contraste = 4.5V
    digitalWrite(39, HIGH);
}
#endif // DOGLCD

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
#ifdef DOGLCD
  #define START_MENU() do {\
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {\
        lcd_implementation_clear();\
        display_refresh_mode = UPDATE_SCREEN;\
    }\
    if (lcd_get_encoder_updated() || lcd_get_button_clicked()) {\
        display_refresh_mode = UPDATE_SCREEN;\
    }\
    \
    if (display_refresh_mode == UPDATE_SCREEN) {\
        u8g.firstPage(); \
        do { \
            u8g.setFont(u8g_font_6x10_marlin);\
            if (encoder_position > 0x8000) encoder_position = 0;\
            if (encoder_position / ENCODER_STEPS_PER_MENU_ITEM < display_view_menu_offset) {\
                display_view_menu_offset = encoder_position / ENCODER_STEPS_PER_MENU_ITEM;\
                display_refresh_mode == CLEAR_AND_UPDATE_SCREEN;\
            }\
            uint8_t _lineNr = display_view_menu_offset, _menuItemNr; \
            for (uint8_t _drawLineNr = 0; _drawLineNr < LCD_HEIGHT; _drawLineNr++, _lineNr++) { \
                _menuItemNr = 0;
#else
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
        for (uint8_t _drawLineNr = 0; _drawLineNr < LCD_HEIGHT; _drawLineNr++, _lineNr++) { \
            _menuItemNr = 0;
#endif
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
#ifdef DOGLCD
#define END_MENU() \
            if (encoder_position / ENCODER_STEPS_PER_MENU_ITEM >= _menuItemNr) encoder_position = _menuItemNr * ENCODER_STEPS_PER_MENU_ITEM - 1; \
            if ((uint8_t)(encoder_position / ENCODER_STEPS_PER_MENU_ITEM) >= display_view_menu_offset + LCD_HEIGHT) { display_view_menu_offset = (encoder_position / ENCODER_STEPS_PER_MENU_ITEM) - LCD_HEIGHT + 1; _lineNr = display_view_menu_offset - 1; _drawLineNr = -1; } \
        } } while( u8g.nextPage() );\
        display_refresh_mode = NO_UPDATE_SCREEN;\
    }\
    } while(0)
#else
#define END_MENU() \
	if (encoder_position / ENCODER_STEPS_PER_MENU_ITEM >= _menuItemNr) encoder_position = _menuItemNr * ENCODER_STEPS_PER_MENU_ITEM - 1; \
	if ((uint8_t)(encoder_position / ENCODER_STEPS_PER_MENU_ITEM) >= display_view_menu_offset + LCD_HEIGHT) { display_view_menu_offset = (encoder_position / ENCODER_STEPS_PER_MENU_ITEM) - LCD_HEIGHT + 1; _lineNr = display_view_menu_offset - 1; _drawLineNr = -1; } \
	lcd_enable_encoder();\
	} } while(0)
#endif
    

// Menu Drawers
static void menu_action_text();
static void menu_action_back(func_t function);
static void menu_action_submenu(func_t function);
static void menu_action_gcode(const char* pgcode);
static void menu_action_function(func_t function);
static void menu_action_wizard(func_t function);
static void funct_sdfile(void* data);
static void menu_action_sdfile(const char* filename, char* longFilename);
static void funct_sddirectory(void* data);
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
    lcd_update(true);
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
void funct_draw_menu_main(void *)
{
    draw_menu_main();
}
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
            MENU_ITEM(submenu, MSG_STOP_PRINT, draw_menu_stop_confirm);
        } else {
            MENU_ITEM(submenu, MSG_CARD_MENU, draw_menu_sdcard);

#  if (SDCARDDETECT < 1)
            MENU_ITEM(gcode, MSG_CNG_SDCARD, PSTR("M21"));  // SD-card changed by user
#  endif (SDCARDDETECT < 1)
        }

    } else {
        MENU_ITEM(text, MSG_NO_CARD);

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
    item_selected = -1;
    cache_menu_first_time = true;

    lcd_set_menu(view_menu_sdcard);
}
static void view_menu_sdcard()
{
    if (cache_menu_first_time == true) {
        // Gets the number of files on the current folder
        list_length = card.getnrfilenames();

        // Checks if the current folder is the root of the SD
        card.getWorkDirName();
        if (card.filename[0] != '/') {
            folder_is_root = false;
            list_length += 2;           // Space for "Back" & ".." options added to the list
        } else {
            folder_is_root = true;
            list_length += 1;           // Space for "Back" option added to the list
        }

        // Calculates the size of the cache and the window.
        if (list_length < MAX_WINDOW_SIZE) {
            window_size = list_length;
            cache_size = list_length;
        } else if (list_length < MAX_CACHE_SIZE) {
            window_size = MAX_WINDOW_SIZE;
            cache_size = list_length;
        } else {
            window_size = MAX_WINDOW_SIZE;
            cache_size = MAX_CACHE_SIZE;
        }

        // Reset the encoder position
        encoder_position = 0;

        // Set the edges of the windows
        window_min = 0;
        window_max = window_size - 1;
        cache_min = 0;
        cache_max = cache_size - 1;

        // The content of the cache must be updated
        cache_update = true;

        display_refresh_mode = CLEAR_AND_UPDATE_SCREEN;
    }

    // Input Adquisition

    // Gets the encoder position into a shadow register
    int16_t encoder_position_shadow = encoder_position;

    // Corrects the encoder position if it is pointing out of the list
    if (encoder_position_shadow < 0) {
        encoder_position = 0;
        encoder_position_shadow = 0;
    }
    else if (encoder_position_shadow > ENCODER_STEPS_PER_MENU_ITEM * list_length - 1) {
        encoder_position = ENCODER_STEPS_PER_MENU_ITEM * list_length - 1;
        encoder_position_shadow = ENCODER_STEPS_PER_MENU_ITEM * list_length - 1;
    }

    // Algorithm for scrolling the window and the cache along the list
    new_item_selected = encoder_position_shadow / ENCODER_STEPS_PER_MENU_ITEM;
    
    if (new_item_selected != item_selected) {
        if (new_item_selected < window_min) {
            window_min = new_item_selected;
            window_max = window_min + window_size - 1;
        } else if (new_item_selected > window_max) {
            window_max = new_item_selected;
            window_min = window_max - window_size + 1;
        }

        if (window_min < cache_min) {
            cache_max = window_max;
            cache_min = cache_max - cache_size + 1;
            if (cache_min < 0) {
                cache_min = 0;
                cache_max = cache_size - 1;
            }
            cache_update = true;
        } else if (window_max > cache_max) {
            cache_min = window_min;
            cache_max = cache_min + cache_size - 1;
            if (cache_max > list_length - 1) {
                cache_max = list_length - 1;
                cache_min = list_length - 1 - cache_size + 1;
            }
            cache_update = true;
        }

        // Calculates the relative position between cache index and window index
        window_offset = window_min - cache_min;
        // Calculates the relative position between window index and cursor
        cursor_offset = new_item_selected - window_min;

#ifdef DEBUG_DYNAMIC_MENU
        SERIAL_ECHO("List Size: "); SERIAL_ECHOLN(itostr3(list_length));
        SERIAL_ECHO("Cursor: "); SERIAL_ECHOLN(itostr3(new_item_selected));
        SERIAL_ECHO("Window Min: "); SERIAL_ECHOLN(itostr3(window_min));
        SERIAL_ECHO("Window Max: "); SERIAL_ECHOLN(itostr3(window_max));
        SERIAL_ECHO("Cache Min: "); SERIAL_ECHOLN(itostr3(cache_min));
        SERIAL_ECHO("Cache Max: "); SERIAL_ECHOLN(itostr3(cache_max));
        SERIAL_ECHOLN("----------");

        SERIAL_ECHO("Window Offset: "); SERIAL_ECHOLN(itostr3(window_offset));
        SERIAL_ECHO("Cursor Offset: "); SERIAL_ECHOLN(itostr3(cursor_offset));
        SERIAL_ECHOLN("----------");
#endif // DEBUG_DYNAMIC_MENU

        // Cache: Memory Management
        if (cache_update == true) {
            // Clears the cache content
            memset(cache, 0, sizeof(cache));
            memset(cache_data, 0, sizeof(cache_data));

#ifdef DEBUG_DYNAMIC_MENU
            SERIAL_ECHOLN("Caching:");
#endif // DEBUG_DYNAMIC_MENU
            
            // Fills the cache with new entries
            uint8_t i = 0;
            int8_t offset = 0;  // Relative position between cache index and file list

            // Cache entry for "Back" option
            if ((cache_min + i) == 0) {
                cache[i].type = 1;
                strcpy(cache[i].text, MSG_BACK);
                cache[i].action = funct_draw_menu_main;
                cache[i].data = NULL;
#ifdef DEBUG_DYNAMIC_MENU
                SERIAL_ECHO("cache["); SERIAL_ECHO(itostr3(i)); SERIAL_ECHO("]:");
                SERIAL_ECHOLN((cache_index + i)->text);
#endif // DEBUG_DYNAMIC_MENU
                i++;
            } else {
                offset--;
            }

            // Cache entry for ".." option
            if (folder_is_root == false) {
                if ((cache_min + i) == 1) {
                    cache[i].type = 2;
                    strcpy(cache[i].text, "..");
                    cache[i].action = function_menu_sdcard_updir;
                    cache[i].data = NULL;
#ifdef DEBUG_DYNAMIC_MENU
                    SERIAL_ECHO("cache["); SERIAL_ECHO(itostr3(i)); SERIAL_ECHO("]:");
                    SERIAL_ECHOLN((cache_index + i)->text);
#endif // DEBUG_DYNAMIC_MENU
                    i++;
                } else {
                    offset--;
                }
            }

            // Cache entries for the files in the current directory
            uint8_t j = 0;
            while (i < cache_size) {
                card.getfilename(cache_min + offset + j);

                cache_entry tmp;
                if (card.filenameIsDir) {
                    cache[i].type = 3;
                    strcpy(cache[i].text, card.longFilename);
                    cache[i].action = funct_sddirectory;
                    strcpy(cache_data[i].filename, card.filename);
                    strcpy(cache_data[i].longFilename, card.longFilename);
                    cache[i].data = (void*) &cache_data[i];
                } else {
                    cache[i].type = 4;
                    strcpy(cache[i].text, card.longFilename);
                    cache[i].action = funct_sdfile;
                    strcpy(cache_data[i].filename, card.filename);
                    strcpy(cache_data[i].longFilename, card.longFilename);
                    cache[i].data = (void*) &cache_data[i];
                }
#ifdef DEBUG_DYNAMIC_MENU
                SERIAL_ECHO("cache["); SERIAL_ECHO(itostr3(i)); SERIAL_ECHO("]:");
                SERIAL_ECHOLN((cache_index + i)->text);
#endif // DEBUG_DYNAMIC_MENU

                i++;
                j++;
            }
#ifdef DEBUG_DYNAMIC_MENU
            SERIAL_ECHOLN("----------");
#endif // DEBUG_DYNAMIC_MENU

            // The content of the cache is up-to-date
            cache_update = false;
        }

#ifdef DEBUG_DYNAMIC_MENU
        SERIAL_ECHOLN("Memory Cache:");
        for (uint8_t z = 0; z < cache_size; z++) {
            SERIAL_ECHO("cache["); SERIAL_ECHO(itostr3(z)); SERIAL_ECHO("]:");
            SERIAL_ECHOLN((cache_index + z)->text);
        }
        SERIAL_ECHOLN("----------");
#endif // DEBUG_DYNAMIC_MENU

        // Draw the cache content
#ifdef DOGLCD
        u8g.firstPage();
        do {
            for (uint8_t k = 0; k < window_size; k++) {
                switch (cache[window_offset+k].type) {
                case 1:
                    if (k == cursor_offset)
                        lcd_implementation_drawmenu_back_selected_R(k, cache[window_offset+k].text, NULL);
                    else

                        lcd_implementation_drawmenu_back_R(k, cache[window_offset+k].text, NULL);
                    break;
                case 2:
                    if (k == cursor_offset)
                        lcd_implementation_drawmenu_function_selected_R(k, cache[window_offset+k].text, NULL);
                    else

                        lcd_implementation_drawmenu_function_R(k, cache[window_offset+k].text, NULL);
                    break;

                case 3:
                    if (k == cursor_offset)
                        lcd_implementation_drawmenu_sddirectory_selected(k, NULL, cache_data[window_offset+k].filename, cache_data[window_offset+k].longFilename);
                    else

                        lcd_implementation_drawmenu_sddirectory(k, NULL, cache_data[window_offset+k].filename, cache_data[window_offset+k].longFilename);
                    break;
                case 4:
                    if (k == cursor_offset)
                        lcd_implementation_drawmenu_sdfile_selected(k, NULL, cache_data[window_offset+k].filename, cache_data[window_offset+k].longFilename);
                    else

                        lcd_implementation_drawmenu_sdfile(k, NULL, cache_data[window_offset+k].filename, cache_data[window_offset+k].longFilename);
                    break;
                default:
                    break;
                }
            }
        } while(u8g.nextPage());
#else
        if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
            lcd_implementation_clear();
            display_refresh_mode = NO_UPDATE_SCREEN;
        }
        for (uint8_t k = 0; k < window_size; k++) {
            switch (cache[window_offset+k].type) {
            case 1:
                if (k == cursor_offset)
                    lcd_implementation_drawmenu_back_selected_R(k, cache[window_offset+k].text, NULL);
                else

                    lcd_implementation_drawmenu_back_R(k, cache[window_offset+k].text, NULL);
                break;
            case 2:
                if (k == cursor_offset)
                    lcd_implementation_drawmenu_function_selected_R(k, cache[window_offset+k].text, NULL);
                else

                    lcd_implementation_drawmenu_function_R(k, cache[window_offset+k].text, NULL);
                break;

            case 3:
                if (k == cursor_offset)
                    lcd_implementation_drawmenu_sddirectory_selected(k, NULL, cache_data[window_offset+k].filename, cache_data[window_offset+k].longFilename);
                else

                    lcd_implementation_drawmenu_sddirectory(k, NULL, cache_data[window_offset+k].filename, cache_data[window_offset+k].longFilename);
                break;
            case 4:
                if (k == cursor_offset)
                    lcd_implementation_drawmenu_sdfile_selected(k, NULL, cache_data[window_offset+k].filename, cache_data[window_offset+k].longFilename);
                else

                    lcd_implementation_drawmenu_sdfile(k, NULL, cache_data[window_offset+k].filename, cache_data[window_offset+k].longFilename);
                break;
            default:
                break;
            }
        }
#endif // DOGLCD

        item_selected = new_item_selected;
        cache_menu_first_time = false;
    }

    // If button has been clicked, executes the action asociated to the selected item
    if (lcd_get_button_clicked()) {
        cache[window_offset+cursor_offset].action(cache[window_offset+cursor_offset].data);
    }
}

#if (SDCARDDETECT == -1)
static void function_sdcard_refresh()
{
    card.initsd();
    display_view_menu_offset = 0;
}
#endif (SDCARDDETECT == -1)

static void function_menu_sdcard_updir(void *)
{
    display_refresh_mode = CLEAR_AND_UPDATE_SCREEN;
    card.updir();

    item_selected = -1;
    cache_menu_first_time = true;
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

#ifdef HEATED_BED_SUPPORT
        setTargetBed(0);
#endif

        flush_commands();
        quickStop();

        LCD_MESSAGEPGM(WELCOME_MSG);
        draw_status_screen();
        lcd_update();

        plan_reset_position();

        current_position[X_AXIS] = plan_get_axis_position(X_AXIS);
        current_position[Y_AXIS] = plan_get_axis_position(Y_AXIS);
        current_position[Z_AXIS] = plan_get_axis_position(Z_AXIS) + 10;
        current_position[E_AXIS] = plan_get_axis_position(E_AXIS) - 10;

        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS] / 60, active_extruder);

#    if X_MAX_POS < 250
        current_position[X_AXIS] = X_MIN_POS;
        current_position[Y_AXIS] = 150;
        current_position[Z_AXIS] += 20;
#    else // X_MAX_POS < 250
        current_position[X_AXIS] = X_MAX_POS - 15;
        current_position[Y_AXIS] = Y_MAX_POS - 15;
        current_position[Z_AXIS] = Z_MAX_POS - 15;
#    endif // X_MAX_POS < 250
        if (current_position[Z_AXIS] > Z_MAX_POS) {
            current_position[Z_AXIS] = Z_MAX_POS;
        }

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

#ifdef WITBOX
static void lcd_emergency_stop()
{
    LCD_ALERTMESSAGEPGM("EMERGENCY STOP!"); //TODO
    SERIAL_ECHOLN("KILLED: Emergency stop active!");
    stop_buffer = true;
    stop_buffer_code = 999;
    if (IS_SD_PRINTING) {
        card.sdprinting = false;
        card.closefile();
    }
    quickStop();
    disable_x();
    disable_y();
    disable_z();
    disable_e0();
    disable_e1();
    disable_e2();
    cancel_heatup = true;
    function_control_cooldown();
}
#endif //WITBOX

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
    MENU_ITEM(submenu, MSG_LOAD, pre_filament_load);
    MENU_ITEM(submenu, MSG_UNLOAD, pre_filament_unload);
    END_MENU();   
}

void pre_filament_load()
{
	filamentStatus = LOADING;
	draw_picture_set_temperature();
}

void pre_filament_unload()
{
	filamentStatus = UNLOADING;
	draw_picture_set_temperature();
}

void draw_menu_filament_load()
{
	setTargetHotend0(target_temperature[0]);
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
        int tHotend=int(degHotend(0) + 0.5);
        int tTarget=int(degTargetHotend(0) + 0.5);
#ifdef DOGLCD
        u8g.firstPage();
        do {
            lcd_implementation_set_cursor(0, 0);
            lcd_implementation_print_P(PSTR(MSG_CF_HEAT_1));
            lcd_implementation_set_cursor(1, 0);
            lcd_implementation_print_P(PSTR(MSG_CF_HEAT_2));
            lcd_implementation_set_cursor(1, 6);
            lcd_implementation_print(LCD_STR_THERMOMETER[0]);
            lcd_implementation_print(itostr3(int(degHotend(0))));
            lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
            lcd_implementation_set_cursor(3,0);
            lcd_implementation_print_P(PSTR(MSG_EXIT));
        } while ( u8g.nextPage() );
#else
	START_MENU();
        MENU_ITEM(back, MSG_ABORT, draw_menu_filament);
        END_MENU();
	lcd_implementation_set_cursor(3, 2);
        lcd_implementation_print_P(PSTR(MSG_HEATING));
        lcd_implementation_set_cursor(5, 3);
        lcd_implementation_print(LCD_STR_THERMOMETER[0]);
        lcd_implementation_print(itostr3(tHotend));
        lcd_implementation_print('/');
        lcd_implementation_print(itostr3left(tTarget));
        lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
#endif
    }

    if ((degHotend(0) > degTargetHotend(0))) {
        lcd_implementation_quick_feedback();
        lcd_enable_display_timeout();
        draw_menu_filament_insert();
    }

#ifdef DOGLCD
    if (lcd_get_button_clicked()) {
        lcd_enable_display_timeout();
        lcd_set_status_screen();
    }
#endif

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
    setTargetHotend0(target_temperature[0]);
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
        int tHotend=int(degHotend(0) + 0.5);
        int tTarget=int(degTargetHotend(0) + 0.5);
#ifdef DOGLCD
        u8g.firstPage();
        do {
            lcd_implementation_set_cursor(0, 0);
            lcd_implementation_print_P(PSTR(MSG_CF_HEAT_1));
            lcd_implementation_set_cursor(1, 0);
            lcd_implementation_print_P(PSTR(MSG_CF_HEAT_2));
            lcd_implementation_set_cursor(1, 6);
            lcd_implementation_print(LCD_STR_THERMOMETER[0]);
            lcd_implementation_print(itostr3(int(degHotend(0))));
            lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
            lcd_implementation_set_cursor(3,0);
            lcd_implementation_print_P(PSTR(MSG_EXIT));
        } while ( u8g.nextPage() );
#else
        START_MENU();
        MENU_ITEM(back, MSG_ABORT, draw_menu_filament);
        END_MENU();
	lcd_implementation_set_cursor(3, 2);
        lcd_implementation_print_P(PSTR(MSG_HEATING));
        lcd_implementation_set_cursor(5, 3);
        lcd_implementation_print(LCD_STR_THERMOMETER[0]);
        lcd_implementation_print(itostr3(tHotend));
        lcd_implementation_print('/');
        lcd_implementation_print(itostr3left(tTarget));
        lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
#endif
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

#ifdef DOGLCD
    if (lcd_get_button_clicked()) {
        lcd_enable_display_timeout();
        lcd_set_status_screen();
    }
#endif

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
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[X_AXIS]/60, active_extruder);
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
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
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
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
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
        plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], manual_feedrate[Y_AXIS]/60, active_extruder);
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
#ifdef DOGLCD
        u8g.firstPage();
        do {
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
        } while (u8g.nextPage());
#else
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
#endif
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
#ifdef DOGLCD
        u8g.firstPage();
        do {
            lcd_implementation_set_cursor(0, 0);
            lcd_implementation_print_P(PSTR(MSG_LB_COOL_1));
            lcd_implementation_set_cursor(1, 0);
            lcd_implementation_print_P(PSTR(MSG_LB_COOL_2));
            lcd_implementation_set_cursor(1, 6);
            lcd_implementation_print(LCD_STR_THERMOMETER[0]);
            lcd_implementation_print(itostr3(int(degHotend(0))));
            lcd_implementation_print_P(PSTR(LCD_STR_DEGREE " "));
            lcd_implementation_set_cursor(3,0);
            lcd_implementation_print_P(PSTR(MSG_EXIT));
        } while ( u8g.nextPage() );
#else
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
#endif
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
    lcd_update(true);
}
static void view_wizard_change_filament()
{
    if (display_refresh_mode == CLEAR_AND_UPDATE_SCREEN) {
        lcd_implementation_clear();
        display_refresh_mode = UPDATE_SCREEN;
    }

    if (display_refresh_mode == UPDATE_SCREEN) {
#ifdef DOGLCD
        u8g.firstPage();
        do {
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
        } while (u8g.nextPage());
#else
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
#endif
        display_refresh_mode = NO_UPDATE_SCREEN;
    }
}

void draw_picture_set_temperature() 
{
    encoder_position = 0;
    target_temperature[0] = FILAMENT_CHANGE_TEMP;
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
#ifdef DOGLCD
        u8g.firstPage();
        do {
            lcd_implementation_set_cursor(1, 1);
            lcd_implementation_print_P(PSTR(MSG_NOZZLE));
            lcd_implementation_print(':');

            lcd_implementation_set_cursor(1, 17);
            lcd_implementation_print(itostr3(target_temperature[0]));    
        } while ( u8g.nextPage() );
#else
	lcd_implementation_set_cursor(1, 1);
        lcd_implementation_print_P(PSTR(MSG_NOZZLE));
        lcd_implementation_print(':');

        lcd_implementation_set_cursor(1, 17);
        lcd_implementation_print(itostr3(target_temperature[0]));
#endif
    }

    if (lcd_get_button_clicked()) {
		if (filamentStatus==LOADING) {
			draw_menu_filament_load();
		}else if (filamentStatus==UNLOADING) {
			draw_menu_filament_unload();
		} else {
        	draw_menu_main();
		}
		filamentStatus = NORMAL;
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
#ifdef DOGLCD
        u8g.firstPage();
        do {
            lcd_implementation_set_cursor(1, 1);
            lcd_implementation_print_P(PSTR(MSG_BED));
            lcd_implementation_print(':');

            lcd_implementation_set_cursor(1, 17);
            lcd_implementation_print(itostr3(target_temperature_bed));    
        } while ( u8g.nextPage() );
#else
	lcd_implementation_set_cursor(1, 1);
        lcd_implementation_print_P(PSTR(MSG_BED));
        lcd_implementation_print(':');

        lcd_implementation_set_cursor(1, 17);
        lcd_implementation_print(itostr3(target_temperature_bed));
#endif
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
#ifdef DOGLCD
        u8g.firstPage();
        do {
            lcd_implementation_set_cursor(1, 1);
            lcd_implementation_print_P(PSTR(MSG_SPEED));
            lcd_implementation_print(':');

            lcd_implementation_set_cursor(1, 17);
            lcd_implementation_print(itostr3(feedmultiply));    
        } while ( u8g.nextPage() );
#else
	lcd_implementation_set_cursor(1, 1);
        lcd_implementation_print_P(PSTR(MSG_SPEED));
        lcd_implementation_print(':');

        lcd_implementation_set_cursor(1, 17);
        lcd_implementation_print(itostr3(feedmultiply));
#endif
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
#ifdef DOGLCD
        u8g.firstPage();
        do {
#endif
            lcd_implementation_set_cursor(1, 4);
            lcd_implementation_print_P(PSTR(MACHINE_NAME));
            lcd_implementation_set_cursor(3, 4);
            lcd_implementation_print_P(PSTR(FIRMWARE_VER));
            lcd_implementation_print(" ");
            lcd_implementation_print_P(PSTR(BUILD_VER));
#ifdef DOGLCD
        } while ( u8g.nextPage() );
#endif
    }

    if (lcd_get_button_clicked()) {
        display_view_next = view_status_screen;
    }

    display_refresh_mode == NO_UPDATE_SCREEN;
    lcd_clear_triggered_flags();
}

static void menu_action_text() {}

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

static void funct_sdfile(void* data)
{
    cache_entry_data* params = (cache_entry_data*) data;
    menu_action_sdfile(params->filename, params->longFilename);
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
    fanSpeed = PREHEAT_FAN_SPEED;
    sprintf_P(cmd, PSTR("M23 %s"), filename);

    enquecommand_P(PSTR("G28"));
    enquecommand_P(PSTR("G1 Z10"));

    for(c = &cmd[4]; *c; c++)
        *c = tolower(*c);

    enquecommand(cmd);
    enquecommand_P(PSTR("M24"));
    draw_status_screen();
}

static void funct_sddirectory(void* data)
{
    cache_entry_data* params = (cache_entry_data*) data;
    menu_action_sddirectory(params->filename, params->longFilename);
}

static void menu_action_sddirectory(const char* filename, char* longFilename)
{
    display_refresh_mode = CLEAR_AND_UPDATE_SCREEN;
    card.chdir(filename);
    
    item_selected = -1;
    cache_menu_first_time = true;
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

ISR(TIMER5_OVF_vect)
{
    lcd_timer++;

#if ( defined(BEEPER) && (BEEPER > 0) )
    if (beeper_duration) {
        if (beep_count == 0) {
            beeper_level = !beeper_level;
            beep_count = frequency_ratio;
        } else {
            beep_count--;
        }
        beeper_duration--;
    } else {
        beeper_level = false;
        beep_count = 0;
    }
    WRITE(BEEPER, beeper_level);    // Tone: 4 KHz (every 125 us)
#endif // ( defined(BEEPER) && (BEEPER > 0) )

    if (lcd_timer % 4 == 0)     // Every 500 us
        lcd_update_encoder();

    if (lcd_timer % 80 == 0) {  // Every 10 ms
        lcd_update_button();
        lcd_timer = 0;
    }
}
