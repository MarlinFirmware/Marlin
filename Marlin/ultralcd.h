#ifndef __ULTRALCDH
#define __ULTRALCDH
#include "Marlin.h"
#ifdef LCD_AVAILABLE

  void lcd_status();
  void lcd_init();
  void lcd_status(const char* message);
  void beep();
  void buttons_check();

  #define LCD_UPDATE_INTERVAL 100
  #define STATUSTIMEOUT 15000

  #ifdef ULTRA_LCD
    #include <LiquidCrystal.h>
    extern LiquidCrystal lcd;

    #define lcd_print(x) lcd.print(x)
    #define lcd_setCursor(column, row) lcd.setCursor(column, row)
    #define lcd_constructorInit(w, h) lcd.begin(w, h); lcd_init()
    #define lcd_lateInit()
    #define lcd_clear() lcd.clear()
    #define lcd_createChar(n, data) lcd.createChar(n+1, data)
    #define lcd_printSpecialChar(n) lcd.print(n)

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

  #elif defined SER_LCD
    #include <SoftwareSerial.h>
    #include <serLCD.h>
  
    extern serLCD lcd;
    
    #define lcd_print(x) lcd.print(x)
    #define lcd_setCursor(column, row) lcd.setCursor(row+1, column+1)
    #define lcd_constructorInit(w, h)
    #define lcd_lateInit() lcd_init()
    #define lcd_clear() lcd.clear()
    #define lcd_createChar(n, data) lcd.createChar(n+1, data)
    #define lcd_printSpecialChar(n) lcd.print(n)
    
  #elif defined KS0108
    //KS0108 library is configured in the library settings itself.
    #include <glcd.h>
    #include <fonts/SystemFont5x7.h>      // system font
       
    #define lcd_print(x) GLCD.print(x)
    #define lcd_setCursor(column, row) GLCD.CursorTo(column, row)
    #define lcd_constructorInit(w, h)
    #define lcd_lateInit() GLCD.Init(); GLCD.SelectFont(System5x7); LCD_MESSAGEPGM(WELCOME_MSG)
    #define lcd_clear() GLCD.ClearScreen()
    #define lcd_createChar(n, data) //NOT IMPLEMENTED YET
    #define lcd_printSpecialChar(n) //NOT IMPLEMENTED YET
    
  #elif defined COLOR_LCD_SHIELD
    #error COLOR_LCD_SHIELD NOT IMPLEMENTED!!
  #else
    #error Oops!  You have LCD_AVAILABLE but no LCD has been selected
  #endif

  // Scale factor from encoder to display moving?
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
        lcd_setCursor(0,activeline);lcd_print((activeline+lineoffset)?' ':' '); 
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
        lcd_setCursor(0,activeline);lcd_print((activeline+lineoffset)?'>':'\003');    
      } 
    }
    
    FORCE_INLINE void clearIfNecessary()
    {
      if(lastlineoffset!=lineoffset ||force_lcd_update)
      {
        force_lcd_update=true;
         lcd_clear();
      } 
    }
  };

  //conversion routines, could need some overworking
  //char *ftostr51(const float &x);
  //char *ftostr31(const float &x);
  //char *ftostr3(const float &x);

  // Functions used externally to send text to display
  #define LCD_MESSAGE(x) lcd_status(x);
  #define LCD_MESSAGEPGM(x) lcd_statuspgm(MYPGM(x));
  #define LCD_STATUS lcd_status();
  #define LCD_SETUP lcd_lateInit();
  
#else //no lcd
  #define LCD_STATUS
  #define LCD_MESSAGE(x)
  #define LCD_MESSAGEPGM(x)
  #define LCD_SETUP
  FORCE_INLINE void lcd_status() {};
#endif
  
#ifndef ULTIPANEL  
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
char *ftostr52(const float &x);
#endif //__ULTRALCDH

