#ifndef __ULTRALCDH
#define __ULTRALCDH
#include "Configuration.h"

#ifdef ULTRA_LCD

  void lcd_status();
  void lcd_init();
  void lcd_status(const char* message);
  void beep();
  void buttons_check();
  #define LCDSTATUSRIGHT

  #define LCD_UPDATE_INTERVAL 100
  #define STATUSTIMEOUT 15000

  #include "Configuration.h"

  #include <LiquidCrystal.h>
  extern LiquidCrystal lcd;

  //lcd display size

#ifdef NEWPANEL
 //arduino pin witch triggers an piezzo beeper
  #define BEEPER 18

  #define LCD_PINS_RS 20 
  #define LCD_PINS_ENABLE 17
  #define LCD_PINS_D4 16
  #define LCD_PINS_D5 21 
  #define LCD_PINS_D6 5
  #define LCD_PINS_D7 6
  
  //buttons are directly attached
  #define BTN_EN1 40
  #define BTN_EN2 42
  #define BTN_ENC 19  //the click
  
  #define BLEN_C 2
  #define BLEN_B 1
  #define BLEN_A 0
  
  #define SDCARDDETECT 38
  
  #define EN_C (1<<BLEN_C)
  #define EN_B (1<<BLEN_B)
  #define EN_A (1<<BLEN_A)
  
   //encoder rotation values
  #define encrot0 0
  #define encrot1 2
  #define encrot2 3
  #define encrot3 1

  
  #define CLICKED (buttons&EN_C)
  #define BLOCK {blocking=millis()+blocktime;}
  #define CARDINSERTED (READ(SDCARDDETECT)==0)
  
#else
  //arduino pin witch triggers an piezzo beeper
  #define BEEPER 18

  //buttons are attached to a shift register
  #define SHIFT_CLK 38
  #define SHIFT_LD 42
  #define SHIFT_OUT 40
  #define SHIFT_EN 17
  
  #define LCD_PINS_RS 16 
  #define LCD_PINS_ENABLE 5
  #define LCD_PINS_D4 6
  #define LCD_PINS_D5 21 
  #define LCD_PINS_D6 20
  #define LCD_PINS_D7 19
  
   //bits in the shift register that carry the buttons for:
  // left up center down right red
  #define BL_LE 7
  #define BL_UP 6
  #define BL_MI 5
  #define BL_DW 4
  #define BL_RI 3
  #define BL_ST 2

  #define BLEN_B 1
  #define BLEN_A 0

  //encoder rotation values
  #define encrot0 0
  #define encrot1 2
  #define encrot2 3
  #define encrot3 1

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
    void getfilename(const uint8_t nr);
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
    char filename[11];
    bool linechanging;
  };

  char *fillto(int8_t n,char *c);
  char *ftostr51(const float &x);
  char *ftostr31(const float &x);
  char *ftostr3(const float &x);



  #define LCD_MESSAGE(x) lcd_status(x);
  #define LCD_STATUS lcd_status()
#else //no lcd
  #define LCD_STATUS
  #define LCD_MESSAGE(x)
#endif
  
#ifndef ULTIPANEL  
 #define CLICKED false
#define BLOCK ;
#endif 
#endif //ULTRALCD

