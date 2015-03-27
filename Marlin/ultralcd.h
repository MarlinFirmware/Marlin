#ifndef ULTRALCD_H
#define ULTRALCD_H

#include "Marlin.h"

#ifdef ULTRA_LCD
  int lcd_strlen(char *s);
  int lcd_strlen_P(const char *s);
  void lcd_update();
  void lcd_init();
  void lcd_setstatus(const char* message);
  void lcd_setstatuspgm(const char* message);
  void lcd_setalertstatuspgm(const char* message);
  void lcd_reset_alert_level();
  bool lcd_detected(void);

  #ifdef DOGLCD
    extern int lcd_contrast;
    void lcd_setcontrast(uint8_t value);
    static unsigned char blink = 0; // Variable for visualization of fan rotation in GLCD
  #endif

  #define LCD_MESSAGEPGM(x) lcd_setstatuspgm(PSTR(x))
  #define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatuspgm(PSTR(x))

  #define LCD_UPDATE_INTERVAL 100
  #define LCD_TIMEOUT_TO_STATUS 15000

  #ifdef ULTIPANEL
    void lcd_buttons_update();
    extern volatile uint8_t buttons;  //the last checked buttons in a bit array.
    #ifdef REPRAPWORLD_KEYPAD
      extern volatile uint8_t buttons_reprapworld_keypad; // to store the keypad shift register values
    #endif
  #else
    FORCE_INLINE void lcd_buttons_update() {}
  #endif

  extern int plaPreheatHotendTemp;
  extern int plaPreheatHPBTemp;
  extern int plaPreheatFanSpeed;
  extern int absPreheatHotendTemp;
  extern int absPreheatHPBTemp;
  extern int absPreheatFanSpeed;

  extern bool cancel_heatup;
  
  #ifdef FILAMENT_LCD_DISPLAY
    extern unsigned long message_millis;
  #endif

  void lcd_buzz(long duration,uint16_t freq);
  bool lcd_clicked();

  void lcd_ignore_click(bool b=true);

  #ifdef NEWPANEL
    #define EN_C BIT(BLEN_C)
    #define EN_B BIT(BLEN_B)
    #define EN_A BIT(BLEN_A)

    #define LCD_CLICKED (buttons&EN_C)
    #ifdef REPRAPWORLD_KEYPAD
  	  #define EN_REPRAPWORLD_KEYPAD_F3 BIT(BLEN_REPRAPWORLD_KEYPAD_F3)
  	  #define EN_REPRAPWORLD_KEYPAD_F2 BIT(BLEN_REPRAPWORLD_KEYPAD_F2)
  	  #define EN_REPRAPWORLD_KEYPAD_F1 BIT(BLEN_REPRAPWORLD_KEYPAD_F1)
  	  #define EN_REPRAPWORLD_KEYPAD_UP BIT(BLEN_REPRAPWORLD_KEYPAD_UP)
  	  #define EN_REPRAPWORLD_KEYPAD_RIGHT BIT(BLEN_REPRAPWORLD_KEYPAD_RIGHT)
  	  #define EN_REPRAPWORLD_KEYPAD_MIDDLE BIT(BLEN_REPRAPWORLD_KEYPAD_MIDDLE)
  	  #define EN_REPRAPWORLD_KEYPAD_DOWN BIT(BLEN_REPRAPWORLD_KEYPAD_DOWN)
  	  #define EN_REPRAPWORLD_KEYPAD_LEFT BIT(BLEN_REPRAPWORLD_KEYPAD_LEFT)

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
    #define B_LE BIT(BL_LE)
    #define B_UP BIT(BL_UP)
    #define B_MI BIT(BL_MI)
    #define B_DW BIT(BL_DW)
    #define B_RI BIT(BL_RI)
    #define B_ST BIT(BL_ST)
    #define EN_B BIT(BLEN_B)
    #define EN_A BIT(BLEN_A)
    
    #define LCD_CLICKED ((buttons&B_MI)||(buttons&B_ST))
  #endif//NEWPANEL

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
