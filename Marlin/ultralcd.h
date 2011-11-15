#ifndef __ULTRALCDH
#define __ULTRALCDH
#include "Configuration.h"

#ifdef ULTRA_LCD

  void lcd_status();
  void lcd_init();
  void lcd_status(const char* message);
  void beep();
  void buttons_check();


  #define LCD_UPDATE_INTERVAL 100
  #define STATUSTIMEOUT 15000


  #include <LiquidCrystal.h>
  extern LiquidCrystal lcd;


  #ifdef NEWPANEL

    
    #define EN_C (1<<BLEN_C)
    #define EN_B (1<<BLEN_B)
    #define EN_A (1<<BLEN_A)
    
    #define CLICKED (buttons&EN_C)
    #define BLOCK {blocking=millis()+blocktime;}
    #define CARDINSERTED (READ(SDCARDDETECT)==0)
    
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
    
    #define CLICKED ((buttons&B_MI)||(buttons&B_ST))
    #define BLOCK {blocking[BL_MI]=millis()+blocktime;blocking[BL_ST]=millis()+blocktime;}
    
  #endif
    
  // blocking time for recognizing a new keypress of one key, ms
  #define blocktime 500
  #define lcdslow 5
    
  enum MainStatus{Main_Status, Main_Menu, Main_Prepare, Main_Control, Main_SD};

  class MainMenu{
  public:
    MainMenu();
    void update();
    uint8_t activeline;
    MainStatus status;
    uint8_t displayStartingRow;
    
    void showStatus();
    void showMainMenu();
    void showPrepare();
    void showControl();
    void showSD();
    bool force_lcd_update;
    int lastencoderpos;
    int8_t lineoffset;
    int8_t lastlineoffset;
    
    bool linechanging;
  };

  //conversion routines, could need some overworking
  char *fillto(int8_t n,char *c);
  char *ftostr51(const float &x);
  char *ftostr31(const float &x);
  char *ftostr3(const float &x);



  #define LCD_MESSAGE(x) lcd_status(x);
  #define LCD_MESSAGEPGM(x) lcd_statuspgm(PSTR(x));
  #define LCD_STATUS lcd_status()
#else //no lcd
  #define LCD_STATUS
  #define LCD_MESSAGE(x)
  #define LCD_MESSAGEPGM(x)
  inline void lcd_status() {};
#endif
  
#ifndef ULTIPANEL  
 #define CLICKED false
  #define BLOCK ;
#endif 
  
void lcd_statuspgm(const char* message);
  
#endif //ULTRALCD

