#ifndef ULTRALCD_H
#define ULTRALCD_H

#include "Marlin.h"

#ifdef ULTRA_LCD

  void lcd_update();
  void lcd_init();
  void lcd_setstatus(const char* message);
  void lcd_setstatuspgm(const char* message);
  void lcd_setalertstatuspgm(const char* message);
  void lcd_reset_alert_level();

  #define LCD_MESSAGEPGM(x) lcd_setstatuspgm(PSTR(x))
  #define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatuspgm(PSTR(x))

  #define LCD_UPDATE_INTERVAL 100
  #define LCD_TIMEOUT_TO_STATUS 15000

  #ifdef ULTIPANEL
  void lcd_buttons_update();
  extern volatile uint8_t buttons;  //the last checked buttons in a bit array.
  #else
  FORCE_INLINE void lcd_buttons_update() {}
  #endif

  extern int plaPreheatHotendTemp;
  extern int plaPreheatHPBTemp;
  extern int plaPreheatFanSpeed;

  extern int absPreheatHotendTemp;
  extern int absPreheatHPBTemp;
  extern int absPreheatFanSpeed;
    
  #ifdef NEWPANEL
    #define EN_C (1<<BLEN_C)
    #define EN_B (1<<BLEN_B)
    #define EN_A (1<<BLEN_A)

    #define LCD_CLICKED (buttons&EN_C)
  #else
    //atomatic, do not change
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

#else //no lcd
  FORCE_INLINE void lcd_update() {}
  FORCE_INLINE void lcd_init() {}
  FORCE_INLINE void lcd_setstatus(const char* message) {}
  FORCE_INLINE void lcd_buttons_update() {}
  FORCE_INLINE void lcd_reset_alert_level() {}

  #define LCD_MESSAGEPGM(x) 
  #define LCD_ALERTMESSAGEPGM(x) 
#endif 

char *itostr2(const uint8_t &x);
char *itostr31(const int &xx);
char *itostr3(const int &xx);
char *itostr3left(const int &xx);
char *itostr4(const int &xx);

char *ftostr3(const float &x);
char *ftostr31(const float &x);
char *ftostr32(const float &x);
char *ftostr5(const float &x);
char *ftostr51(const float &x);
char *ftostr52(const float &x);

#endif //ULTRALCD
