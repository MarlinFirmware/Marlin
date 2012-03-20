#include "ultralcd.h"
#ifdef ULTRA_LCD
#include "Marlin.h"
#include <LiquidCrystal.h>
//===========================================================================
//=============================imported variables============================
//===========================================================================

extern volatile int feedmultiply;
extern volatile bool feedmultiplychanged;

extern volatile int extrudemultiply;

extern long position[4];   
#ifdef SDSUPPORT
extern CardReader card;
#endif

//===========================================================================
//=============================public variables============================
//===========================================================================
volatile char buttons=0;  //the last checked buttons in a bit array.
int encoderpos=0;
short lastenc=0;


//===========================================================================
//=============================private  variables============================
//===========================================================================
static char messagetext[LCD_WIDTH]="";

//return for string conversion routines
static char conv[8];

LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5,LCD_PINS_D6,LCD_PINS_D7);  //RS,Enable,D4,D5,D6,D7 

static unsigned long previous_millis_lcd=0;
//static long previous_millis_buttons=0;


#ifdef NEWPANEL
 static long blocking=0;
#else
 static long blocking[8]={0,0,0,0,0,0,0,0};
#endif
 
static MainMenu menu;


void lcdProgMemprint(const char *str)
{
  char ch=pgm_read_byte(str);
  while(ch)
  {
    lcd.print(ch);
    ch=pgm_read_byte(++str);
  }
}
#define lcdprintPGM(x) lcdProgMemprint(MYPGM(x))


//===========================================================================
//=============================functions         ============================
//===========================================================================

int intround(const float &x){return int(0.5+x);}

void lcd_status(const char* message)
{
  strncpy(messagetext,message,LCD_WIDTH);
  messagetext[strlen(message)]=0;
}

void lcd_statuspgm(const char* message)
{
  char ch=pgm_read_byte(message);
  char *target=messagetext;
  uint8_t cnt=0;
  while(ch &&cnt<LCD_WIDTH)
  {
    *target=ch;
    target++;
    cnt++;
    ch=pgm_read_byte(++message);
  }
  *target=0;
}

FORCE_INLINE void clear()
{
  lcd.clear();
}


void lcd_init()
{
  //beep();
  byte Degree[8] =
  {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };
  byte Thermometer[8] =
  {
    B00100,
    B01010,
    B01010,
    B01010,
    B01010,
    B10001,
    B10001,
    B01110
  };
  byte uplevel[8]={0x04, 0x0e, 0x1f, 0x04, 0x1c, 0x00, 0x00, 0x00};//thanks joris
  byte refresh[8]={0x00, 0x06, 0x19, 0x18, 0x03, 0x13, 0x0c, 0x00}; //thanks joris
  byte folder [8]={0x00, 0x1c, 0x1f, 0x11, 0x11, 0x1f, 0x00, 0x00}; //thanks joris
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcd.createChar(1,Degree);
  lcd.createChar(2,Thermometer);
  lcd.createChar(3,uplevel);
  lcd.createChar(4,refresh);
  lcd.createChar(5,folder);
  LCD_MESSAGEPGM(WELCOME_MSG);
}


void beep()
{
  //return;
  #ifdef ULTIPANEL
	#if (BEEPER > -1)
	{
		pinMode(BEEPER,OUTPUT);
		for(int8_t i=0;i<20;i++){
		WRITE(BEEPER,HIGH);
		delay(5);
		WRITE(BEEPER,LOW);
		delay(5);
		}
	}
        #endif
  #endif
}

void beepshort()
{
  //return;
  #ifdef ULTIPANEL
	#if (BEEPER > -1)
	{
		pinMode(BEEPER,OUTPUT);
		for(int8_t i=0;i<10;i++){
		WRITE(BEEPER,HIGH);
		delay(3);
		WRITE(BEEPER,LOW);
		delay(3);
		}
	}
        #endif
  #endif  
}

void lcd_status()
{
  #ifdef ULTIPANEL
    static uint8_t oldbuttons=0;
    //static long previous_millis_buttons=0;
    //static long previous_lcdinit=0;
  //  buttons_check(); // Done in temperature interrupt
    //previous_millis_buttons=millis();
    long ms=millis();
    for(int8_t i=0; i<8; i++) {
      #ifndef NEWPANEL
      if((blocking[i]>ms))
        buttons &= ~(1<<i);
      #else
      if((blocking>ms))
        buttons &= ~(1<<i);        
      #endif
    }
    if((buttons==oldbuttons) &&  ((millis() - previous_millis_lcd) < LCD_UPDATE_INTERVAL)   )
      return;
    oldbuttons=buttons;
  #else
  
    if(((millis() - previous_millis_lcd) < LCD_UPDATE_INTERVAL)   )
      return;
  #endif
    
  previous_millis_lcd=millis();
  menu.update();
}
#ifdef ULTIPANEL  


void buttons_init()
{
  #ifdef NEWPANEL
    pinMode(BTN_EN1,INPUT);
    pinMode(BTN_EN2,INPUT); 
    pinMode(BTN_ENC,INPUT); 
    pinMode(SDCARDDETECT,INPUT);
    WRITE(BTN_EN1,HIGH);
    WRITE(BTN_EN2,HIGH);
    WRITE(BTN_ENC,HIGH);
    #if (SDCARDDETECT > -1)
    {
      WRITE(SDCARDDETECT,HIGH);
    }
    #endif
  #else
    pinMode(SHIFT_CLK,OUTPUT);
    pinMode(SHIFT_LD,OUTPUT);
    pinMode(SHIFT_EN,OUTPUT);
    pinMode(SHIFT_OUT,INPUT);
    WRITE(SHIFT_OUT,HIGH);
    WRITE(SHIFT_LD,HIGH); 
    WRITE(SHIFT_EN,LOW); 
  #endif
}


void buttons_check()
{
  
  #ifdef NEWPANEL
    uint8_t newbutton=0;
    if(READ(BTN_EN1)==0)  newbutton|=EN_A;
    if(READ(BTN_EN2)==0)  newbutton|=EN_B;
    if((blocking<millis()) &&(READ(BTN_ENC)==0))
      newbutton|=EN_C;
    buttons=newbutton;
  #else   //read it from the shift register
    uint8_t newbutton=0;
    WRITE(SHIFT_LD,LOW);
    WRITE(SHIFT_LD,HIGH);
    unsigned char tmp_buttons=0;
    for(int8_t i=0;i<8;i++)
    { 
      newbutton = newbutton>>1;
      if(READ(SHIFT_OUT))
        newbutton|=(1<<7);
      WRITE(SHIFT_CLK,HIGH);
      WRITE(SHIFT_CLK,LOW);
    }
    buttons=~newbutton; //invert it, because a pressed switch produces a logical 0
  #endif
  
  //manage encoder rotation
  char enc=0;
  if(buttons&EN_A)
    enc|=(1<<0);
  if(buttons&EN_B)
    enc|=(1<<1);
  if(enc!=lastenc)
	{
    switch(enc)
    {
    case encrot0:
      if(lastenc==encrot3)
        encoderpos++;
      else if(lastenc==encrot1)
        encoderpos--;
      break;
    case encrot1:
      if(lastenc==encrot0)
        encoderpos++;
      else if(lastenc==encrot2)
        encoderpos--;
      break;
    case encrot2:
      if(lastenc==encrot1)
        encoderpos++;
      else if(lastenc==encrot3)
        encoderpos--;
      break;
    case encrot3:
      if(lastenc==encrot2)
        encoderpos++;
      else if(lastenc==encrot0)
        encoderpos--;
      break;
    default:
      ;
    }
  }
  lastenc=enc;
}

#endif

MainMenu::MainMenu()
{
  status=Main_Status;
  displayStartingRow=0;
  activeline=0;
  force_lcd_update=true;
  #ifdef ULTIPANEL
    buttons_init();
  #endif
  lcd_init();
  linechanging=false;
  tune=false;
}

void MainMenu::showStatus()
{ 
#if LCD_HEIGHT==4
  static int olddegHotEnd0=-1;
  static int oldtargetHotEnd0=-1;
  //force_lcd_update=true;
  if(force_lcd_update)  //initial display of content
  {
    encoderpos=feedmultiply;
    clear();
    lcd.setCursor(0,0);lcdprintPGM("\002---/---\001 ");
    #if defined BED_USES_THERMISTOR || defined BED_USES_AD595 
      lcd.setCursor(10,0);lcdprintPGM("B---/---\001 ");
    #endif
  }
    
  int tHotEnd0=intround(degHotend0());
  if((tHotEnd0!=olddegHotEnd0)||force_lcd_update)
  {
    lcd.setCursor(1,0);
    lcd.print(ftostr3(tHotEnd0));
    olddegHotEnd0=tHotEnd0;
  }
  int ttHotEnd0=intround(degTargetHotend0());
  if((ttHotEnd0!=oldtargetHotEnd0)||force_lcd_update)
  {
    lcd.setCursor(5,0);
    lcd.print(ftostr3(ttHotEnd0));
    oldtargetHotEnd0=ttHotEnd0;
  }
  #if defined BED_USES_THERMISTOR || defined BED_USES_AD595 
    static int oldtBed=-1;
    static int oldtargetBed=-1; 
    int tBed=intround(degBed());
    if((tBed!=oldtBed)||force_lcd_update)
    {
      lcd.setCursor(11,0);
      lcd.print(ftostr3(tBed));
      oldtBed=tBed;
    }
    int targetBed=intround(degTargetBed());
    if((targetBed!=oldtargetBed)||force_lcd_update)
    {
      lcd.setCursor(15,0);
      lcd.print(ftostr3(targetBed));
      oldtargetBed=targetBed;
    }
  #endif
  //starttime=2;
  static uint16_t oldtime=0;
  if(starttime!=0)
  {
    lcd.setCursor(0,1);
    uint16_t time=millis()/60000-starttime/60000;
    
    if(starttime!=oldtime)
    {
      lcd.print(itostr2(time/60));lcdprintPGM("h ");lcd.print(itostr2(time%60));lcdprintPGM("m");
      oldtime=time;
    }
  }
  static int oldzpos=0;
  int currentz=current_position[2]*100;
  if((currentz!=oldzpos)||force_lcd_update)
  {
    lcd.setCursor(10,1);
    lcdprintPGM("Z:");lcd.print(ftostr52(current_position[2]));
    oldzpos=currentz;
  }
  
  static int oldfeedmultiply=0;
  int curfeedmultiply=feedmultiply;
  
  if(feedmultiplychanged == true) {
    feedmultiplychanged = false;
    encoderpos = curfeedmultiply;
  }
  
  if(encoderpos!=curfeedmultiply||force_lcd_update)
  {
   curfeedmultiply=encoderpos;
   if(curfeedmultiply<10)
     curfeedmultiply=10;
   if(curfeedmultiply>999)
     curfeedmultiply=999;
   feedmultiply=curfeedmultiply;
   encoderpos=curfeedmultiply;
  }
  
  if((curfeedmultiply!=oldfeedmultiply)||force_lcd_update)
  {
   oldfeedmultiply=curfeedmultiply;
   lcd.setCursor(0,2);
   lcd.print(itostr3(curfeedmultiply));lcdprintPGM("% ");
  }
  
  if(messagetext[0]!='\0')
  {
    lcd.setCursor(0,LCD_HEIGHT-1);
    lcd.print(messagetext);
    uint8_t n=strlen(messagetext);
    for(int8_t i=0;i<LCD_WIDTH-n;i++)
      lcd.print(" ");
    messagetext[0]='\0';
  }
#ifdef SDSUPPORT
  static uint8_t oldpercent=101;
  uint8_t percent=card.percentDone();
  if(oldpercent!=percent ||force_lcd_update)
  {
     lcd.setCursor(10,2);
    lcd.print(itostr3((int)percent));
    lcdprintPGM("%SD");
  }
#endif
#else //smaller LCDS----------------------------------
  static int olddegHotEnd0=-1;
  static int oldtargetHotEnd0=-1;
  if(force_lcd_update)  //initial display of content
  {
    encoderpos=feedmultiply;
    lcd.setCursor(0,0);lcdprintPGM("\002123/567\001 ");
    #if defined BED_USES_THERMISTOR || defined BED_USES_AD595 
    lcd.setCursor(10,0);lcdprintPGM("B123/567\001 ");
    #endif
  }
    
  int tHotEnd0=intround(degHotend0());
  int ttHotEnd0=intround(degTargetHotend0());


  if((abs(tHotEnd0-olddegHotEnd0)>1)||force_lcd_update)
  {
    lcd.setCursor(1,0);
    lcd.print(ftostr3(tHotEnd0));
    olddegHotEnd0=tHotEnd0;
  }
  if((ttHotEnd0!=oldtargetHotEnd0)||force_lcd_update)
  {
    lcd.setCursor(5,0);
    lcd.print(ftostr3(ttHotEnd0));
    oldtargetHotEnd0=ttHotEnd0;
  }

  if(messagetext[0]!='\0')
  {
    lcd.setCursor(0,LCD_HEIGHT-1);
    lcd.print(messagetext);
    uint8_t n=strlen(messagetext);
    for(int8_t i=0;i<LCD_WIDTH-n;i++)
      lcd.print(" ");
    messagetext[0]='\0';
  }

#endif
  force_lcd_update=false;
}

enum {ItemP_exit, ItemP_autostart,ItemP_disstep,ItemP_home, ItemP_origin, ItemP_preheat_pla, ItemP_preheat_abs, ItemP_cooldown,/*ItemP_extrude,*/ItemP_move};

//any action must not contain a ',' character anywhere, or this breaks:
#define MENUITEM(repaint_action, click_action) \
  {\
    if(force_lcd_update)  { lcd.setCursor(0,line);  repaint_action; } \
    if((activeline==line) && CLICKED) {click_action} \
  }
  
void MainMenu::showPrepare()
{
#ifdef ULTIPANEL
 uint8_t line=0;
 clearIfNecessary();
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
   //Serial.println((int)(line-lineoffset));
  switch(i)
  {
    case ItemP_exit:
      MENUITEM(  lcdprintPGM(MSG_MAIN)  ,  BLOCK;status=Main_Menu;beepshort(); ) ;
      break;
    case ItemP_autostart:
      MENUITEM(  lcdprintPGM(MSG_AUTOSTART)  ,  BLOCK;
#ifdef SDSUPPORT
          card.lastnr=0;card.setroot();card.checkautostart(true);
#endif
          beepshort(); ) ;
      break;
    case ItemP_disstep:
      MENUITEM(  lcdprintPGM(MSG_DISABLE_STEPPERS)  ,  BLOCK;enquecommand("M84");beepshort(); ) ;
      break;
    case ItemP_home:
      MENUITEM(  lcdprintPGM(MSG_AUTO_HOME)  ,  BLOCK;enquecommand("G28");beepshort(); ) ;
      break;
    case ItemP_origin:
      MENUITEM(  lcdprintPGM(MSG_SET_ORIGIN)  ,  BLOCK;enquecommand("G92 X0 Y0 Z0");beepshort(); ) ;
      break;
    case ItemP_preheat_pla:
      MENUITEM(  lcdprintPGM(MSG_PREHEAT_PLA)  ,  BLOCK;setTargetHotend0(PLA_PREHEAT_HOTEND_TEMP);setTargetBed(PLA_PREHEAT_HPB_TEMP);
      #if FAN_PIN > -1
        analogWrite(FAN_PIN, PLA_PREHEAT_FAN_SPEED);
      #endif
      beepshort(); );
      break;
    case ItemP_preheat_abs:
      MENUITEM(  lcdprintPGM(MSG_PREHEAT_ABS)  ,  BLOCK;setTargetHotend0(ABS_PREHEAT_HOTEND_TEMP);setTargetBed(ABS_PREHEAT_HPB_TEMP); 
      #if FAN_PIN > -1
        analogWrite(FAN_PIN, ABS_PREHEAT_FAN_SPEED);
      #endif
      beepshort(); );
      break;
    case ItemP_cooldown:
      MENUITEM(  lcdprintPGM(MSG_COOLDOWN)  ,  BLOCK;setTargetHotend0(0);setTargetBed(0);beepshort(); ) ;
      break;
//    case ItemP_extrude:
  //    MENUITEM(  lcdprintPGM(" Extrude")  ,  BLOCK;enquecommand("G92 E0");enquecommand("G1 F700 E50");beepshort(); ) ;
    //  break;
    case ItemP_move:
      MENUITEM(  lcdprintPGM(MSG_MOVE_AXIS) , BLOCK;status=Sub_PrepareMove;beepshort(); );
      break;
        default:   
      break;
  }
  line++;
 }
 updateActiveLines(ItemP_move,encoderpos);
#endif
}

enum {
  ItemAM_exit,
  ItemAM_X, ItemAM_Y, ItemAM_Z, ItemAM_E
};

void MainMenu::showAxisMove()
{
   uint8_t line=0;
   int oldencoderpos=0;
   clearIfNecessary();
   for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
   {
     switch(i)
      {
          case ItemAM_exit:
          MENUITEM(  lcdprintPGM(MSG_PREPARE_ALT)  ,  BLOCK;status=Main_Menu;beepshort(); ) ;
          break;
          case ItemAM_X:
          {
	 	  //oldencoderpos=0;
                  if(force_lcd_update)
                  {
                    lcd.setCursor(0,line);lcdprintPGM(" X:");
                    lcd.setCursor(11,line);lcd.print(ftostr52(current_position[X_AXIS]));
                  }
      
                  if((activeline!=line) )
                  break;
                  
                  if(CLICKED) 
                  {
                    linechanging=!linechanging;
                    if(linechanging)
                    {
			enquecommand("G91");
                    }
                    else
                    {
		      enquecommand("G90");
                      encoderpos=activeline*lcdslow;
                      beepshort();
                    }
                    BLOCK;
                  }
                  if(linechanging)
                  {
                    if (encoderpos >0) 
                   { 
		    	enquecommand("G1 F700 X0.1");
			oldencoderpos=encoderpos;
                        encoderpos=0;
		    }
		  
		    else if (encoderpos < 0)
                    {
		    	enquecommand("G1 F700 X-0.1");
			oldencoderpos=encoderpos;
                        encoderpos=0;
		    }
                    lcd.setCursor(11,line);lcd.print(ftostr52(current_position[X_AXIS]));
                  }
          }
          break;
          case ItemAM_Y:
            {
                  if(force_lcd_update)
                  {
                    lcd.setCursor(0,line);lcdprintPGM(" Y:");
                    lcd.setCursor(11,line);lcd.print(ftostr52(current_position[Y_AXIS]));
                  }
      
                  if((activeline!=line) )
                  break;
                  
                  if(CLICKED) 
                  {
                    linechanging=!linechanging;
                    if(linechanging)
                    {
			enquecommand("G91");
                    }
                    else
                    {
		      enquecommand("G90");
                      encoderpos=activeline*lcdslow;
                      beepshort();
                    }
                    BLOCK;
                  }
                  if(linechanging)
                  {
                    if (encoderpos >0) 
                   { 
		    	enquecommand("G1 F700 Y0.1");
			oldencoderpos=encoderpos;
                        encoderpos=0;
		    }
		  
		    else if (encoderpos < 0)
                    {
		    	enquecommand("G1 F700 Y-0.1");
			oldencoderpos=encoderpos;
                        encoderpos=0;
		    }
                    lcd.setCursor(11,line);lcd.print(ftostr52(current_position[Y_AXIS]));
                  }
          }
          break;
          case ItemAM_Z:
          {
                  if(force_lcd_update)
                  {
                    lcd.setCursor(0,line);lcdprintPGM(" Z:");
                    lcd.setCursor(11,line);lcd.print(ftostr52(current_position[Z_AXIS]));
                  }
      
                  if((activeline!=line) )
                  break;
                  
                   if(CLICKED) 
                  {
                    linechanging=!linechanging;
                    if(linechanging)
                    {
			enquecommand("G91");
                    }
                    else
                    {
		      enquecommand("G90");
                      encoderpos=activeline*lcdslow;
                      beepshort();
                    }
                    BLOCK;
                  }
                  if(linechanging)
                  {
                    if (encoderpos >0) 
                   { 
		    	enquecommand("G1 F70 Z0.1");
			oldencoderpos=encoderpos;
                        encoderpos=0;
		    }
		  
		    else if (encoderpos < 0)
                    {
		    	enquecommand("G1 F70 Z-0.1");
			oldencoderpos=encoderpos;
                        encoderpos=0;
		    }
                    lcd.setCursor(11,line);lcd.print(ftostr52(current_position[Z_AXIS]));
                  }
          }
          break;
          case ItemAM_E:
          MENUITEM(  lcdprintPGM(MSG_EXTRUDE)  ,  BLOCK;enquecommand("G92 E0");enquecommand("G1 F700 E5");beepshort(); ) ;
          break;
          default:
          break;
      }
      line++;
   }
   updateActiveLines(ItemAM_E,encoderpos);
}

enum {ItemT_exit,ItemT_speed,ItemT_flow,ItemT_nozzle,
#if (HEATER_BED_PIN > -1)
ItemT_bed,
#endif
ItemT_fan};

void MainMenu::showTune()
{ 
  uint8_t line=0;
  clearIfNecessary();
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
   //Serial.println((int)(line-lineoffset));
  switch(i)
  {
  case ItemT_exit:
      MENUITEM(  lcdprintPGM(MSG_MAIN)  ,  BLOCK;status=Main_Menu;beepshort(); ) ;
      break;
  case ItemT_speed:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcdprintPGM(MSG_SPEED);
        lcd.setCursor(13,line);lcd.print(ftostr3(feedmultiply));
      }
      
      if((activeline!=line) )
        break;
      
      if(CLICKED) //AnalogWrite(FAN_PIN,  fanpwm);
      {
        linechanging=!linechanging;
        if(linechanging)
        {
            encoderpos=feedmultiply;
        }
        else
        {
          encoderpos=activeline*lcdslow;
          beepshort();
        }
        BLOCK;
      }
      if(linechanging)
      {
        if(encoderpos<1) encoderpos=1;
        if(encoderpos>400) encoderpos=400;
        feedmultiply = encoderpos;
        feedmultiplychanged=true;
        lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
      }
      
    }break;
    case ItemT_nozzle:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_NOZZLE);
          lcd.setCursor(13,line);lcd.print(ftostr3(intround(degTargetHotend0())));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=intround(degTargetHotend0());
          }
          else
          {
            setTargetHotend0(encoderpos);
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>260) encoderpos=260;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
      }break;
      #if (HEATER_BED_PIN > -1)
      case ItemT_bed:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_BED);
          lcd.setCursor(13,line);lcd.print(ftostr3(intround(degTargetBed())));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=intround(degTargetBed());
          }
          else
          {
            setTargetBed(encoderpos);
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>260) encoderpos=260;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
      }break;
      #endif

      
      case ItemT_fan:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_FAN_SPEED);
          lcd.setCursor(13,line);lcd.print(ftostr3(FanSpeed));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED) //nalogWrite(FAN_PIN,  fanpwm);
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=FanSpeed;
          }
          else
          {
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>255) encoderpos=255;
          FanSpeed=encoderpos;
            analogWrite(FAN_PIN,  FanSpeed);
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;
      case ItemT_flow://axis_steps_per_unit[i] = code_value();
         {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_FLOW);
          lcd.setCursor(13,line);lcd.print(itostr4(axis_steps_per_unit[3]));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)axis_steps_per_unit[3];
          }
          else
          {
            float factor=float(encoderpos)/float(axis_steps_per_unit[3]);
            position[E_AXIS]=lround(position[E_AXIS]*factor);
            //current_position[3]*=factor;
            axis_steps_per_unit[E_AXIS]= encoderpos;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<5) encoderpos=5;
          if(encoderpos>9999) encoderpos=9999;
          lcd.setCursor(13,line);lcd.print(itostr4(encoderpos));
        }
        
      }break; 
    default:   
      break;
  }
  line++;
 }
 updateActiveLines(ItemT_fan,encoderpos);
}

//does not work
// #define MENUCHANGEITEM(repaint_action,  enter_action, accept_action,  change_action) \
//   {\
//     if(force_lcd_update)  { lcd.setCursor(0,line);  repaint_action; } \
//     if(activeline==line)  \
//     { \
//       if(CLICKED) \
//       { \
//         linechanging=!linechanging; \
//         if(linechanging)  {enter_action;} \
//         else {accept_action;} \
//       }  \
//       else \
//       if(linechanging) {change_action};}\
//   }
//   

enum {
  ItemCT_exit,ItemCT_nozzle,
#ifdef AUTOTEMP
  ItemCT_autotempactive,
  ItemCT_autotempmin,ItemCT_autotempmax,ItemCT_autotempfact,
#endif
#if (HEATER_BED_PIN > -1)
ItemCT_bed,
#endif  
  ItemCT_fan,
  ItemCT_PID_P,ItemCT_PID_I,ItemCT_PID_D,ItemCT_PID_C
};

void MainMenu::showControlTemp()
{
  uint8_t line=0;
 clearIfNecessary();
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
  switch(i)
  {
    case ItemCT_exit:
      MENUITEM(  lcdprintPGM(MSG_CONTROL)  ,  BLOCK;status=Main_Control;beepshort(); ) ;
      break;
    case ItemCT_nozzle:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_NOZZLE);
          lcd.setCursor(13,line);lcd.print(ftostr3(intround(degTargetHotend0())));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=intround(degTargetHotend0());
          }
          else
          {
            setTargetHotend0(encoderpos);
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>260) encoderpos=260;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;
      #ifdef AUTOTEMP
      case ItemCT_autotempmin:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_MIN);
          lcd.setCursor(13,line);lcd.print(ftostr3(autotemp_min));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=intround(autotemp_min);
          }
          else
          {
            autotemp_min=encoderpos;
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>260) encoderpos=260;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;  
      case ItemCT_autotempmax:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_MAX);
          lcd.setCursor(13,line);lcd.print(ftostr3(autotemp_max));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=intround(autotemp_max);
          }
          else
          {
            autotemp_max=encoderpos;
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>260) encoderpos=260;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;  
      case ItemCT_autotempfact:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_FACTOR);
          lcd.setCursor(13,line);lcd.print(ftostr32(autotemp_factor));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=intround(autotemp_factor*100);
          }
          else
          {
            autotemp_max=encoderpos;
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>99) encoderpos=99;
          lcd.setCursor(13,line);lcd.print(ftostr32(encoderpos/100.));
        }
        
      }break;
      case ItemCT_autotempactive:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_AUTOTEMP);
          lcd.setCursor(13,line);
          if(autotemp_enabled)
            lcdprintPGM(MSG_ON);
          else
            lcdprintPGM(MSG_OFF);
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          autotemp_enabled=!autotemp_enabled;
          lcd.setCursor(13,line);
          if(autotemp_enabled)
            lcdprintPGM(MSG_ON);
          else
            lcdprintPGM(MSG_OFF);
          BLOCK;
        }
        
      }break;  
      #endif //autotemp
      #if (HEATER_BED_PIN > -1)
      case ItemCT_bed:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_BED);
          lcd.setCursor(13,line);lcd.print(ftostr3(intround(degTargetBed())));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=intround(degTargetBed());
          }
          else
          {
            setTargetBed(encoderpos);
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>260) encoderpos=260;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
      }break;
      #endif
      case ItemCT_fan:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_FAN_SPEED);
          lcd.setCursor(13,line);lcd.print(ftostr3(FanSpeed));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED) //nalogWrite(FAN_PIN,  fanpwm);
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=FanSpeed;
          }
          else
          {
            encoderpos=activeline*lcdslow;
            beepshort();
          }
          BLOCK;
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>255) encoderpos=255;
          FanSpeed=encoderpos;
            analogWrite(FAN_PIN,  FanSpeed);
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;
      case ItemCT_PID_P: 
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" PID-P: ");
          lcd.setCursor(13,line);lcd.print(itostr4(Kp));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)Kp;
          }
          else
          {
            Kp= encoderpos;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<1) encoderpos=1;
          if(encoderpos>9990) encoderpos=9990;
          lcd.setCursor(13,line);lcd.print(itostr4(encoderpos));
        }
        
      }break;
    case ItemCT_PID_I: 
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_PID_I);
          lcd.setCursor(13,line);lcd.print(ftostr51(Ki/PID_dT));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)(Ki*10/PID_dT);
          }
          else
          {
            Ki= encoderpos/10.*PID_dT;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>9990) encoderpos=9990;
          lcd.setCursor(13,line);lcd.print(ftostr51(encoderpos/10.));
        }
        
      }break;
      case ItemCT_PID_D: 
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_PID_D);
          lcd.setCursor(13,line);lcd.print(itostr4(Kd*PID_dT));
        }
        
        if((activeline!=line) )
          break;
        
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)(Kd/5./PID_dT);
          }
          else
          {
            Kd= encoderpos;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>9990) encoderpos=9990;
          lcd.setCursor(13,line);lcd.print(itostr4(encoderpos));
        }
       
      }break;   
    case ItemCT_PID_C: 
      #ifdef PID_ADD_EXTRUSION_RATE
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_PID_C);
          lcd.setCursor(13,line);lcd.print(itostr3(Kc));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)Kc;
          }
          else
          {
            Kc= encoderpos;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>990) encoderpos=990;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }
      #endif
      break;
    default:   
      break;
  }
  line++;
 }
 #ifdef PID_ADD_EXTRUSION_RATE
  updateActiveLines(ItemCT_PID_C,encoderpos);
 #else
  updateActiveLines(ItemCT_PID_D,encoderpos);
 #endif
}


enum {
  ItemCM_exit, 
  ItemCM_acc, ItemCM_xyjerk, 
  ItemCM_vmaxx, ItemCM_vmaxy, ItemCM_vmaxz, ItemCM_vmaxe, 
  ItemCM_vtravmin,ItemCM_vmin,  
  ItemCM_amaxx, ItemCM_amaxy, ItemCM_amaxz, ItemCM_amaxe, 
  ItemCM_aret, ItemCM_xsteps,ItemCM_ysteps, ItemCM_zsteps, ItemCM_esteps
};



void MainMenu::showControlMotion()
{
 uint8_t line=0;
 clearIfNecessary();
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
  switch(i)
  {
    case ItemCM_exit:
      MENUITEM(  lcdprintPGM(MSG_CONTROL)  ,  BLOCK;status=Main_Control;beepshort(); ) ;
      break;
    case ItemCM_acc:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_ACC);
          lcd.setCursor(13,line);lcd.print(itostr3(acceleration/100));lcdprintPGM("00");
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)acceleration/100;
          }
          else
          {
            acceleration= encoderpos*100;
            encoderpos=activeline*lcdslow;
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<5) encoderpos=5;
          if(encoderpos>990) encoderpos=990;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));lcdprintPGM("00");
        }
        
      }break;
    case ItemCM_xyjerk: //max_xy_jerk
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_VXY_JERK);
          lcd.setCursor(13,line);lcd.print(itostr3(max_xy_jerk));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)max_xy_jerk;
          }
          else
          {
            max_xy_jerk= encoderpos;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<1) encoderpos=1;
          if(encoderpos>990) encoderpos=990;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;
      
    case ItemCM_vmaxx:
    case ItemCM_vmaxy:
    case ItemCM_vmaxz:
    case ItemCM_vmaxe:
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_VMAX);
          if(i==ItemCM_vmaxx)lcdprintPGM(MSG_X);
          if(i==ItemCM_vmaxy)lcdprintPGM(MSG_Y);
          if(i==ItemCM_vmaxz)lcdprintPGM(MSG_Z);
          if(i==ItemCM_vmaxe)lcdprintPGM(MSG_E);
          lcd.setCursor(13,line);lcd.print(itostr3(max_feedrate[i-ItemCM_vmaxx]));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)max_feedrate[i-ItemCM_vmaxx];
          }
          else
          {
            max_feedrate[i-ItemCM_vmaxx]= encoderpos;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<1) encoderpos=1;
          if(encoderpos>990) encoderpos=990;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;
    
    case ItemCM_vmin:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_VMIN);
          lcd.setCursor(13,line);lcd.print(itostr3(minimumfeedrate));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)(minimumfeedrate);
          }
          else
          {
            minimumfeedrate= encoderpos;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>990) encoderpos=990;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;
    case ItemCM_vtravmin:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_VTRAV_MIN);
          lcd.setCursor(13,line);lcd.print(itostr3(mintravelfeedrate));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)mintravelfeedrate;
          }
          else
          {
            mintravelfeedrate= encoderpos;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<0) encoderpos=0;
          if(encoderpos>990) encoderpos=990;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
        }
        
      }break;
    
    case ItemCM_amaxx:      
    case ItemCM_amaxy:
    case ItemCM_amaxz:
    case ItemCM_amaxe:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Amax ");
          if(i==ItemCM_amaxx)lcdprintPGM(MSG_X);
          if(i==ItemCM_amaxy)lcdprintPGM(MSG_Y);
          if(i==ItemCM_amaxz)lcdprintPGM(MSG_Z);
          if(i==ItemCM_amaxe)lcdprintPGM(MSG_E);
          lcd.setCursor(13,line);lcd.print(itostr3(max_acceleration_units_per_sq_second[i-ItemCM_amaxx]/100));lcdprintPGM("00");
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)max_acceleration_units_per_sq_second[i-ItemCM_amaxx]/100;
          }
          else
          {
            max_acceleration_units_per_sq_second[i-ItemCM_amaxx]= encoderpos*100;
            encoderpos=activeline*lcdslow;
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<1) encoderpos=1;
          if(encoderpos>990) encoderpos=990;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));lcdprintPGM("00");
        }
        
      }break;
    case ItemCM_aret://float retract_acceleration = 7000;
    {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_A_RETRACT);
          lcd.setCursor(13,line);lcd.print(ftostr3(retract_acceleration/100));lcdprintPGM("00");
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)retract_acceleration/100;
          }
          else
          {
            retract_acceleration= encoderpos*100;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<10) encoderpos=10;
          if(encoderpos>990) encoderpos=990;
          lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));lcdprintPGM("00");
        }
        
      }break;
       case ItemCM_xsteps://axis_steps_per_unit[i] = code_value();
         {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_XSTEPS);
          lcd.setCursor(11,line);lcd.print(ftostr52(axis_steps_per_unit[0]));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)(axis_steps_per_unit[0]*100.0);
          }
          else
          {
            float factor=float(encoderpos)/100.0/float(axis_steps_per_unit[0]);
            position[X_AXIS]=lround(position[X_AXIS]*factor);
            //current_position[3]*=factor;
            axis_steps_per_unit[X_AXIS]= encoderpos/100.0;
            encoderpos=activeline*lcdslow;
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<5) encoderpos=5;
          if(encoderpos>32000) encoderpos=32000;//TODO: This is a problem, encoderpos is 16bit, but steps_per_unit for e can be wel over 800
          lcd.setCursor(11,line);lcd.print(ftostr52(encoderpos/100.0));
        }
        
      }break;
       case ItemCM_ysteps://axis_steps_per_unit[i] = code_value();
         {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_YSTEPS);
          lcd.setCursor(11,line);lcd.print(ftostr52(axis_steps_per_unit[1]));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)(axis_steps_per_unit[1]*100.0);
          }
          else
          {
            float factor=float(encoderpos)/100.0/float(axis_steps_per_unit[1]);
            position[Y_AXIS]=lround(position[Y_AXIS]*factor);
            //current_position[3]*=factor;
            axis_steps_per_unit[Y_AXIS]= encoderpos/100.0;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<5) encoderpos=5;
          if(encoderpos>9999) encoderpos=9999;
          lcd.setCursor(11,line);lcd.print(ftostr52(encoderpos/100.0));
        }
        
      }break;
       case ItemCM_zsteps://axis_steps_per_unit[i] = code_value();
         {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_ZSTEPS);
          lcd.setCursor(11,line);lcd.print(ftostr52(axis_steps_per_unit[2]));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)(axis_steps_per_unit[2]*100.0);
          }
          else
          {
            float factor=float(encoderpos)/100.0/float(axis_steps_per_unit[2]);
            position[Z_AXIS]=lround(position[Z_AXIS]*factor);
            //current_position[3]*=factor;
            axis_steps_per_unit[Z_AXIS]= encoderpos/100.0;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<5) encoderpos=5;
          if(encoderpos>9999) encoderpos=9999;
          lcd.setCursor(11,line);lcd.print(ftostr52(encoderpos/100.0));
        }
        
      }break;
      
    case ItemCM_esteps://axis_steps_per_unit[i] = code_value();
         {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(MSG_ESTEPS);
          lcd.setCursor(11,line);lcd.print(ftostr52(axis_steps_per_unit[3]));
        }
        
        if((activeline!=line) )
          break;
        
        if(CLICKED)
        {
          linechanging=!linechanging;
          if(linechanging)
          {
              encoderpos=(int)(axis_steps_per_unit[3]*100.0);
          }
          else
          {
            float factor=float(encoderpos)/100.0/float(axis_steps_per_unit[3]);
            position[E_AXIS]=lround(position[E_AXIS]*factor);
            //current_position[3]*=factor;
            axis_steps_per_unit[E_AXIS]= encoderpos/100.0;
            encoderpos=activeline*lcdslow;
              
          }
          BLOCK;
          beepshort();
        }
        if(linechanging)
        {
          if(encoderpos<5) encoderpos=5;
          if(encoderpos>9999) encoderpos=9999;
          lcd.setCursor(11,line);lcd.print(ftostr52(encoderpos/100.0));
        }
        
      }break; 
    default:   
      break;
  }
  line++;
 }
 updateActiveLines(ItemCM_esteps,encoderpos);
}


enum {
  ItemC_exit,ItemC_temp,ItemC_move,
  ItemC_store, ItemC_load,ItemC_failsafe
};

void MainMenu::showControl()
{
 uint8_t line=0;
 clearIfNecessary();
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
  switch(i)
  {
    case ItemC_exit:
      MENUITEM(  lcdprintPGM(MSG_MAIN_WIDE)  ,  BLOCK;status=Main_Menu;beepshort(); ) ;
      break;
    case ItemC_temp:
      MENUITEM(  lcdprintPGM(MSG_TEMPERATURE_WIDE)  ,  BLOCK;status=Sub_TempControl;beepshort(); ) ;
      break;
   case ItemC_move:
      MENUITEM(  lcdprintPGM(MSG_MOTION_WIDE)  ,  BLOCK;status=Sub_MotionControl;beepshort(); ) ;
      break;
    case ItemC_store:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcdprintPGM(MSG_STORE_EPROM);
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
        beepshort();
        BLOCK;
        EEPROM_StoreSettings();
      }
    }break;
    case ItemC_load:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcdprintPGM(MSG_LOAD_EPROM);
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
        beepshort();
        BLOCK;
        EEPROM_RetrieveSettings();
      }
    }break;
    case ItemC_failsafe:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcdprintPGM(MSG_RESTORE_FAILSAFE);
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
        beepshort();
        BLOCK;
        EEPROM_RetrieveSettings(true);
      }
    }break;
    default:   
      break;
  }
  line++;
 }
 updateActiveLines(ItemC_failsafe,encoderpos);
}





void MainMenu::showSD()
{
#ifdef SDSUPPORT
 uint8_t line=0;

 clearIfNecessary();
 static uint8_t nrfiles=0;
 if(force_lcd_update)
 {
  if(card.cardOK)
  {
    nrfiles=card.getnrfilenames();
  }
  else
  {
    nrfiles=0;
    lineoffset=0;
  }
 }
 bool enforceupdate=false;
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
  switch(i)
  {
    case 0:
      MENUITEM(  lcdprintPGM(MSG_MAIN)  ,  BLOCK;status=Main_Menu;beepshort(); ) ;
      break;
//     case 1:
//       {
//         if(force_lcd_update)
//         {
//           lcd.setCursor(0,line);
//            #ifdef CARDINSERTED
//           if(CARDINSERTED)
//           #else
//           if(true)
//           #endif
//           {
//             lcdprintPGM(" \004Refresh");
//           }
//           else
//           {
//             lcdprintPGM(" \004Insert Card");
//           }
//           
//         }
//         if((activeline==line) && CLICKED)
//         {
//           BLOCK;
//           beepshort();
//           card.initsd();
//           force_lcd_update=true;
//            nrfiles=card.getnrfilenames();
//         }
//       }break;
    case 1:
      MENUITEM(  lcd.print(" ");card.getWorkDirName();
	  if(card.filename[0]=='/') lcdprintPGM(MSG_REFRESH);
	  else {
		  lcd.print("\005");
		  lcd.print(card.filename);
		  lcd.print("/..");
			}  ,  
	BLOCK;
			if(SDCARDDETECT == -1) card.initsd();
			card.updir();
			enforceupdate=true;
			lineoffset=0;
			beepshort(); ) ;
      
      break;
    default:
    {
      #define FIRSTITEM 2
      if(i-FIRSTITEM<nrfiles)
      {
        if(force_lcd_update)
        {
          card.getfilename(i-FIRSTITEM);
          //Serial.print("Filenr:");Serial.println(i-2);
          lcd.setCursor(0,line);lcdprintPGM(" ");
          if(card.filenameIsDir) lcd.print("\005");
          lcd.print(card.filename);
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          card.getfilename(i-FIRSTITEM);
          if(card.filenameIsDir)
          {
            for(int8_t i=0;i<strlen(card.filename);i++)
              card.filename[i]=tolower(card.filename[i]);
            card.chdir(card.filename);
            lineoffset=0;
            enforceupdate=true;
          }
          else
          {
            char cmd[30];
            for(int8_t i=0;i<strlen(card.filename);i++)
              card.filename[i]=tolower(card.filename[i]);
            sprintf(cmd,"M23 %s",card.filename);
            //sprintf(cmd,"M115");
            enquecommand(cmd);
            enquecommand("M24");
            beep(); 
            status=Main_Status;
            lcd_status(card.filename);
          }
        } 
      }
      
    }
      break;
  }
  line++;
 }
 updateActiveLines(FIRSTITEM+nrfiles-1,encoderpos);
 if(enforceupdate)
 {
   force_lcd_update=true;
   enforceupdate=false;
 }
#endif
}


enum {ItemM_watch, ItemM_prepare, ItemM_control, ItemM_file };
void MainMenu::showMainMenu()
{

  #ifndef ULTIPANEL
    force_lcd_update=false;
  #endif
  if(tune)
  {
    if(!(movesplanned() || IS_SD_PRINTING))
    {
      force_lcd_update=true;
      tune=false;
    }
  }
  else 
  {
    if(movesplanned() || IS_SD_PRINTING)
    {
      force_lcd_update=true;
      tune=true;
    }
  } 
  clearIfNecessary();
  for(int8_t line=0;line<LCD_HEIGHT;line++)
  {
    switch(line)
    { 
      case ItemM_watch:
        MENUITEM(  lcdprintPGM(MSG_WATCH)  ,  BLOCK;status=Main_Status;beepshort(); ) ;
       break;
      case ItemM_prepare:
        MENUITEM(  if(!tune) lcdprintPGM(MSG_PREPARE);else  lcdprintPGM(MSG_TUNE); ,  BLOCK;status=Main_Prepare;beepshort(); ) ;
      break;
       
      case ItemM_control:
        MENUITEM(  lcdprintPGM(MSG_CONTROL_ARROW)  ,  BLOCK;status=Main_Control;beepshort(); ) ;
      break;
      #ifdef SDSUPPORT
      case ItemM_file:    
      {
        if(force_lcd_update) 
        {
          lcd.setCursor(0,line);
          #ifdef CARDINSERTED
            if(CARDINSERTED)
          #else
            if(true)
          #endif
          {
            if(card.sdprinting)
              lcdprintPGM(MSG_STOP_PRINT);
            else
              lcdprintPGM(MSG_CARD_MENU);
          }
          else
          {
           lcdprintPGM(MSG_NO_CARD); 
          }
        }
        #ifdef CARDINSERTED
          if(CARDINSERTED)
        #endif
        if((activeline==line)&&CLICKED)
        {
          card.printingHasFinished();
          BLOCK;
          status=Main_SD;
          beepshort();
        }
      }break;
      #else
      case ItemM_file:
        break;
      #endif
      default: 
        SERIAL_ERROR_START;
        SERIAL_ERRORLNPGM(MSG_SERIAL_ERROR_MENU_STRUCTURE);
      break;
    }
  }
  updateActiveLines(3,encoderpos);
}

void MainMenu::update()
{
  static MainStatus oldstatus=Main_Menu;  //init automatically causes foce_lcd_update=true
  static long timeoutToStatus=0;
  static bool oldcardstatus=false;
  #ifdef CARDINSERTED
    if((CARDINSERTED != oldcardstatus))
    {
      force_lcd_update=true;
      oldcardstatus=CARDINSERTED;
      //Serial.println("echo: SD CHANGE");
      if(CARDINSERTED)
      {
        card.initsd();
        LCD_MESSAGEPGM(MSG_SD_INSERTED);
      }
      else
      {
        card.release();
        LCD_MESSAGEPGM(MSG_SD_REMOVED);
      }
    }
  #endif
 
  if(status!=oldstatus)
  {
    force_lcd_update=true;
    encoderpos=0;
    lineoffset=0;
    
    oldstatus=status;
  }
  if( (encoderpos!=lastencoderpos) || CLICKED)
    timeoutToStatus=millis()+STATUSTIMEOUT;

  switch(status)
  { 
      case Main_Status: 
      {  
        showStatus();
        if(CLICKED)
        {
           linechanging=false;
           BLOCK
           status=Main_Menu;
           timeoutToStatus=millis()+STATUSTIMEOUT;
        }
      }break;
      case Main_Menu: 
      {
        showMainMenu();
        linechanging=false;
      }break;
      case Main_Prepare: 
      {
        if(tune)
        {
          showTune();
        }
        else
        {
          showPrepare(); 
        }
      }break;
      case Sub_PrepareMove:
      {        
            showAxisMove();
      }break;
      case Main_Control:
      {
        showControl(); 
      }break;
      case Sub_MotionControl:
      {
        showControlMotion(); 
      }break;
      case Sub_TempControl:
      {
        showControlTemp(); 
      }break;
      case Main_SD: 
      {
        showSD();
      }break;
  }
  
  if(timeoutToStatus<millis())
    status=Main_Status;
  //force_lcd_update=false;
  lastencoderpos=encoderpos;
}






//  convert float to string with +123.4 format
char *ftostr3(const float &x)
{
  //sprintf(conv,"%5.1f",x);
  int xx=x;
  conv[0]=(xx/100)%10+'0';
  conv[1]=(xx/10)%10+'0';
  conv[2]=(xx)%10+'0';
  conv[3]=0;
  return conv;
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

char *ftostr32(const float &x)
{
  int xx=x*100;
  conv[0]=(xx>=0)?'+':'-';
  xx=abs(xx);
  conv[1]=(xx/100)%10+'0';
  conv[2]='.';
  conv[3]=(xx/10)%10+'0';
  conv[4]=(xx)%10+'0';
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

char *itostr3(const int &xx)
{
  conv[0]=(xx/100)%10+'0';
  conv[1]=(xx/10)%10+'0';
  conv[2]=(xx)%10+'0';
  conv[3]=0;
  return conv;
}

char *itostr4(const int &xx)
{
  conv[0]=(xx/1000)%10+'0';
  conv[1]=(xx/100)%10+'0';
  conv[2]=(xx/10)%10+'0';
  conv[3]=(xx)%10+'0';
  conv[4]=0;
  return conv;
}

//  convert float to string with +1234.5 format
char *ftostr51(const float &x)
{
  int xx=x*10;
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
  int xx=x*100;
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

#endif //ULTRA_LCD


