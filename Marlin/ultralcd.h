#ifndef ULTRALCD_H
#define ULTRALCD_H

#include <inttypes.h>
#include "Marlin.h"

#ifdef ULTRA_LCD
//
// Defines
//
#  ifndef ENCODER_STEPS_PER_MENU_ITEM
#define ENCODER_STEPS_PER_MENU_ITEM 4
#  endif // ENCODER_STEPS_PER_MENU_ITEM

#define LCD_TIMEOUT_STATUS      30000
#define LCD_REFRESH              1500
#define LCD_REFRESH_LIMIT          60
#define LCD_MOVE_RESIDENCY_TIME   500

#define NO_UPDATE_SCREEN            0
#define UPDATE_SCREEN               1
#define CLEAR_AND_UPDATE_SCREEN     2

#define MAX_WINDOW_SIZE LCD_HEIGHT
#define MAX_CACHE_SIZE LCD_HEIGHT+2

//
// Typedefs
//
typedef void (*func_t)();
typedef void (*func_p)(void *);
typedef void (*view_t)();

  extern int plaPreheatHotendTemp;
  extern int plaPreheatHPBTemp;
  extern int plaPreheatFanSpeed;

  extern int absPreheatHotendTemp;
  extern int absPreheatHPBTemp;
  extern int absPreheatFanSpeed;
  
  extern bool cancel_heatup;

//
// LCD API
//
void lcd_init();

void lcd_update(bool force = false);
void lcd_buttons_update();

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
void lcd_beep_ms(uint16_t ms);
void lcd_beep_hz_ms(uint16_t frequency, uint16_t ms);
void lcd_set_refresh(uint8_t mode);
// uint8_t lcd_get_encoder_position();

#ifdef DOGLCD
	extern int lcd_contrast;
	void lcd_setcontrast(uint8_t value);
#endif

// Alert/status messages
void lcd_setstatus(const char* message);
void lcd_setstatuspgm(const char* message);
void lcd_setalertstatuspgm(const char* message);
void lcd_reset_alert_level();
    
void draw_status_screen();
void draw_wizard_change_filament();


  #ifdef NEWPANEL
    #define EN_C (1<<BLEN_C)
    #define EN_B (1<<BLEN_B)
    #define EN_A (1<<BLEN_A)

    #define LCD_CLICKED (buttons&EN_C)
    #ifdef REPRAPWORLD_KEYPAD
  	  #define EN_REPRAPWORLD_KEYPAD_F3 (1<<BLEN_REPRAPWORLD_KEYPAD_F3)
  	  #define EN_REPRAPWORLD_KEYPAD_F2 (1<<BLEN_REPRAPWORLD_KEYPAD_F2)
  	  #define EN_REPRAPWORLD_KEYPAD_F1 (1<<BLEN_REPRAPWORLD_KEYPAD_F1)
  	  #define EN_REPRAPWORLD_KEYPAD_UP (1<<BLEN_REPRAPWORLD_KEYPAD_UP)
  	  #define EN_REPRAPWORLD_KEYPAD_RIGHT (1<<BLEN_REPRAPWORLD_KEYPAD_RIGHT)
  	  #define EN_REPRAPWORLD_KEYPAD_MIDDLE (1<<BLEN_REPRAPWORLD_KEYPAD_MIDDLE)
  	  #define EN_REPRAPWORLD_KEYPAD_DOWN (1<<BLEN_REPRAPWORLD_KEYPAD_DOWN)
  	  #define EN_REPRAPWORLD_KEYPAD_LEFT (1<<BLEN_REPRAPWORLD_KEYPAD_LEFT)

  	  #define LCD_CLICKED ((buttons&EN_C) || (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_F1))
  	  #define REPRAPWORLD_KEYPAD_MOVE_Z_UP (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_F2)
  	  #define REPRAPWORLD_KEYPAD_MOVE_Z_DOWN (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_F3)
  	  #define REPRAPWORLD_KEYPAD_MOVE_X_LEFT (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_LEFT)
  	  #define REPRAPWORLD_KEYPAD_MOVE_X_RIGHT (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_RIGHT)
  	  #define REPRAPWORLD_KEYPAD_MOVE_Y_DOWN (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_DOWN)
  	  #define REPRAPWORLD_KEYPAD_MOVE_Y_UP (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_UP)
  	  #define REPRAPWORLD_KEYPAD_MOVE_HOME (buttons_reprapworld_keypad&EN_REPRAPWORLD_KEYPAD_MIDDLE)
    #endif //REPRAPWORLD_KEYPAD
  #else
    //atomic, do not change
    #define B_LE (1<<BL_LE)
    #define B_UP (1<<BL_UP)
    #define B_MI (1<<BL_MI)
    #define B_DW (1<<BL_DW)
    #define B_RI (1<<BL_RI)
    #define B_ST (1<<BL_ST)
    #define EN_B (1<<BLEN_B)
    #define EN_A (1<<BLEN_A)
    
    #define LCD_CLICKED ((buttons&B_MI)||(buttons&B_ST))
  #endif//NEWPANEL

// Macros
#define LCD_CLICKED lcd_get_button_clicked()
#define LCD_MESSAGEPGM(x) do { lcd_setstatuspgm(PSTR(x)); lcd_set_refresh(CLEAR_AND_UPDATE_SCREEN); } while (0)
#define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatuspgm(PSTR(x))


#else //no LCD
  FORCE_INLINE void lcd_update() {}
  FORCE_INLINE void lcd_init() {}
  FORCE_INLINE void lcd_setstatus(const char* message) {}
  FORCE_INLINE void lcd_buttons_update() {}
  FORCE_INLINE void lcd_reset_alert_level() {}
  FORCE_INLINE void lcd_buzz(long duration,uint16_t freq) {}
  FORCE_INLINE bool lcd_detected(void) { return true; }

  #define LCD_MESSAGEPGM(x) 
  #define LCD_ALERTMESSAGEPGM(x) 

#endif //ULTRA_LCD


char *itostr2(const uint8_t &x);
char *itostr31(const int &xx);
char *itostr3(const int &xx);
char *itostr3left(const int &xx);
char *itostr4(const int &xx);

char *ftostr3(const float &x);
char *ftostr31ns(const float &x); // float to string without sign character
char *ftostr31(const float &x);
char *ftostr32(const float &x);
char *ftostr43(const float &x);
char *ftostr12ns(const float &x); 
char *ftostr32sp(const float &x); // remove zero-padding from ftostr32
char *ftostr5(const float &x);
char *ftostr51(const float &x);
char *ftostr52(const float &x);

#endif //ULTRALCD_H
