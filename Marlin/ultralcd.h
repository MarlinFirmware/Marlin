#ifndef ULTRALCD_H
#define ULTRALCD_H

#include <inttypes.h>

//
// Defines
//
#  ifndef ENCODER_STEPS_PER_MENU_ITEM
#define ENCODER_STEPS_PER_MENU_ITEM 4
#  endif // ENCODER_STEPS_PER_MENU_ITEM

#define LCD_TIMEOUT_STATUS 15000
#define LCD_REFRESH        1500

#define NO_UPDATE_SCREEN            0
#define UPDATE_SCREEN               1
#define CLEAR_AND_UPDATE_SCREEN     2


//
// Typedefs
//
typedef void (*func_t)();
typedef void (*view_t)();

//
// TODO: Review these variables
//
extern int plaPreheatHotendTemp;
extern int plaPreheatHPBTemp;
extern int plaPreheatFanSpeed;

extern int absPreheatHotendTemp;
extern int absPreheatHPBTemp;
extern int absPreheatFanSpeed;

extern bool cancel_heatup;
// END TODO

//
// LCD API
//
void lcd_init();

void lcd_update();

void lcd_set_status_screen();
void lcd_set_menu(view_t menu);
void lcd_set_picture(view_t picture);
void lcd_set_wizard(view_t wizard);

// Get and clear trigger functions
bool lcd_get_button_updated();
bool lcd_get_encoder_updated();
bool lcd_get_button_clicked();
void lcd_clear_triggered_flags();

// Enable/disable functions
void lcd_enable_button();
void lcd_disable_button();
void lcd_enable_encoder();
void lcd_disable_encoder();
void lcd_enable_display_timeout();
void lcd_disable_display_timeout();

void lcd_enable_interrupt();
void lcd_disable_interrupt();

// Control flow functions
void lcd_wizard_set_page(uint8_t page);

// Temporal
void lcd_beep();
void lcd_set_refresh(uint8_t mode);
// uint8_t lcd_get_encoder_position();

// Alert/status messages
void lcd_setstatus(const char* message);
void lcd_setstatuspgm(const char* message);
void lcd_setalertstatuspgm(const char* message);
void lcd_reset_alert_level();

// Macros
#define LCD_CLICKED lcd_get_button_clicked()
#define LCD_MESSAGEPGM(x) do { lcd_setstatuspgm(PSTR(x)); lcd_set_refresh(CLEAR_AND_UPDATE_SCREEN); } while (0)
#define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatuspgm(PSTR(x))

void draw_wizard_change_filament();



// Print utilities
char *itostr2(const uint8_t &x);
char *itostr31(const int &xx);
char *itostr3(const int &xx);
char *itostr3left(const int &xx);
char *itostr4(const int &xx);

char *ftostr3(const float &x);
char *ftostr31ns(const float &x); // float to string without sign character
char *ftostr31(const float &x);
char *ftostr32(const float &x);
char *ftostr5(const float &x);
char *ftostr51(const float &x);
char *ftostr52(const float &x);

#endif // ULTRALCD_H