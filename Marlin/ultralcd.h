#ifndef ULTRALCD_H
#define ULTRALCD_H

#include "Marlin.h"

#ifdef ULTRA_LCD

  #if LANGUAGE_CHOICE == 6
  #include "LiquidCrystalRus.h"
  #define LCD_CLASS LiquidCrystalRus
  #endif
  
  #ifdef I2C_DISPLAY
  #include "LiquidCrystal_I2C.h"
  #define LCD_CLASS LiquidCrystal_I2C
  #else
  #include <LiquidCrystal.h>
  #define LCD_CLASS LiquidCrystal
  #endif

  void lcd_update();
  void lcd_init();
  void lcd_setstatus(const char* message);
  void lcd_setstatuspgm(const char* message);
  void lcd_setalertstatuspgm(const char* message);
  void lcd_buttons_update();
  void lcd_buttons_init();

  #define LCD_MESSAGEPGM(x) lcd_setstatuspgm(PSTR(x))
  #define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatuspgm(PSTR(x))

  #define LCD_UPDATE_INTERVAL 100
  #define LCD_TIMEOUT_TO_STATUS 15000

  extern volatile uint8_t buttons;  //the last checked buttons in a bit array.

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
    #define LCD_BLOCK {blocking=millis()+blocktime;}
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
    #define LCD_BLOCK {blocking[BL_MI]=millis()+blocktime;blocking[BL_ST]=millis()+blocktime;}
  #endif

  // blocking time for recognizing a new keypress of one key, ms
  #define blocktime 500
  #define lcdslow 5
    
  enum MainStatus{Main_Status, Main_Menu, Main_Prepare,Sub_PrepareMove, Main_Control, Main_SD,Sub_TempControl,Sub_MotionControl,Sub_RetractControl, Sub_PreheatPLASettings, Sub_PreheatABSSettings};
  
  extern LCD_CLASS lcd;
  
  class MainMenu{
  public:
    MainMenu();
    void update();
    int8_t activeline;
    MainStatus status;
    uint8_t displayStartingRow;
    
    void showStatus();
    void showMainMenu();
    void showPrepare();
    void showTune();
    void showControl();
    void showControlMotion();
    void showControlTemp();
    void showControlRetract();
    void showAxisMove();
    void showSD();
	void showPLAsettings();
	void showABSsettings();
    bool force_lcd_update;
    long lastencoderpos;
    int8_t lineoffset;
    int8_t lastlineoffset;
    
    bool linechanging;
    
    bool tune;
    
  private:
    FORCE_INLINE void updateActiveLines(const uint8_t &maxlines,volatile long &encoderpos)
    {
      if(linechanging) return; // an item is changint its value, do not switch lines hence
      lastlineoffset=lineoffset; 
      long curencoderpos=encoderpos;  
      force_lcd_update=false;
      if(  (abs(curencoderpos-lastencoderpos)<lcdslow) ) 
      { 
        lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?' ':' '); 
        if(curencoderpos<0)  
        {  
          lineoffset--; 
          if(lineoffset<0) lineoffset=0; 
          curencoderpos=lcdslow-1;
        } 
        if(curencoderpos>(LCD_HEIGHT-1+1)*lcdslow) 
        { 
          lineoffset++; 
          curencoderpos=(LCD_HEIGHT-1)*lcdslow; 
          if(lineoffset>(maxlines+1-LCD_HEIGHT)) 
            lineoffset=maxlines+1-LCD_HEIGHT; 
          if(curencoderpos>maxlines*lcdslow) 
            curencoderpos=maxlines*lcdslow; 
        } 
        lastencoderpos=encoderpos=curencoderpos;
        activeline=curencoderpos/lcdslow;
        if(activeline<0) activeline=0;
        if(activeline>LCD_HEIGHT-1) activeline=LCD_HEIGHT-1;
        if(activeline>maxlines) 
        {
          activeline=maxlines;
          curencoderpos=maxlines*lcdslow;
        }
        if(lastlineoffset!=lineoffset)
          force_lcd_update=true;
        lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?'>':'\003');    
      } 
    }
    
    FORCE_INLINE void clearIfNecessary()
    {
      if(lastlineoffset!=lineoffset ||force_lcd_update)
      {
        force_lcd_update=true;
         lcd.clear();
      } 
    }
  };
#else //no lcd
  FORCE_INLINE void lcd_update() {}
  FORCE_INLINE void lcd_init() {}
  FORCE_INLINE void lcd_setstatus(const char* message) {}
  FORCE_INLINE void lcd_buttons_init() {}
  FORCE_INLINE void lcd_buttons_update() {}

  #define LCD_MESSAGEPGM(x) 
  #define LCD_ALERTMESSAGEPGM(x) 

  #define CLICKED false
  #define BLOCK ;
#endif 

char *itostr2(const uint8_t &x);
char *itostr31(const int &xx);
char *itostr3(const int &xx);
char *itostr4(const int &xx);

char *ftostr3(const float &x);
char *ftostr31(const float &x);
char *ftostr32(const float &x);
char *ftostr51(const float &x);
char *ftostr52(const float &x);

#endif //ULTRALCD
