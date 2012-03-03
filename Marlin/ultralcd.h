#ifndef __ULTRALCDH
#define __ULTRALCDH
#include "Marlin.h"
#ifdef ULTRA_LCD
  #include <LiquidCrystal.h>
  void lcd_status();
  void lcd_init();
  void lcd_status(const char* message);
  void beep();
  void buttons_check();

  #define LCD_UPDATE_INTERVAL 100
  #define STATUSTIMEOUT 15000
  extern LiquidCrystal lcd;
  
  #ifdef NEWPANEL
    #define EN_C (1<<BLEN_C)
    #define EN_B (1<<BLEN_B)
    #define EN_A (1<<BLEN_A)
    
    #define CLICKED (buttons&EN_C)
    #define BLOCK {blocking=millis()+blocktime;}
    #if (SDCARDDETECT > -1)
      #ifdef SDCARDDETECTINVERTED 
        #define CARDINSERTED (READ(SDCARDDETECT)!=0)
      #else
        #define CARDINSERTED (READ(SDCARDDETECT)==0)
      #endif
    #endif  //SDCARDTETECTINVERTED

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
    
  enum MainStatus{Main_Status, Main_Menu, Main_Prepare,Sub_PrepareMove, Main_Control, Main_SD,Sub_TempControl,Sub_MotionControl};

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
    void showAxisMove();
    void showSD();
    bool force_lcd_update;
    int lastencoderpos;
    int8_t lineoffset;
    int8_t lastlineoffset;
    
    bool linechanging;
    
    bool tune;
    
  private:
    FORCE_INLINE void updateActiveLines(const uint8_t &maxlines,volatile int &encoderpos)
    {
      if(linechanging) return; // an item is changint its value, do not switch lines hence
      lastlineoffset=lineoffset; 
      int curencoderpos=encoderpos;  
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

  //conversion routines, could need some overworking
  char *ftostr51(const float &x);
  char *ftostr52(const float &x);
  char *ftostr31(const float &x);
  char *ftostr3(const float &x);



  #define LCD_MESSAGE(x) lcd_status(x);
  #define LCD_MESSAGEPGM(x) lcd_statuspgm(MYPGM(x));
  #define LCD_STATUS lcd_status()
#else //no lcd
  #define LCD_STATUS
  #define LCD_MESSAGE(x)
  #define LCD_MESSAGEPGM(x)
  FORCE_INLINE void lcd_status() {};

  #define CLICKED false
  #define BLOCK ;
#endif 
  
void lcd_statuspgm(const char* message);
  
char *ftostr3(const float &x);
char *itostr2(const uint8_t &x);
char *ftostr31(const float &x);
char *ftostr32(const float &x);
char *itostr31(const int &xx);
char *itostr3(const int &xx);
char *itostr4(const int &xx);
char *ftostr51(const float &x);
#endif //ULTRALCD
