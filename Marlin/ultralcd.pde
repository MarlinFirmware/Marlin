#include "ultralcd.h"
#ifdef ULTRA_LCD

//===========================================================================
//=============================imported variables============================
//===========================================================================

extern volatile int feedmultiply;
extern volatile bool feedmultiplychanged;

extern long position[4];   
extern CardReader card;

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

#include <LiquidCrystal.h>
LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5,LCD_PINS_D6,LCD_PINS_D7);  //RS,Enable,D4,D5,D6,D7 

static unsigned long previous_millis_lcd=0;
static long previous_millis_buttons=0;


#ifdef NEWPANEL
 static long blocking=0;
#else
 static long blocking[8]={0,0,0,0,0,0,0,0};
#endif
 
static MainMenu menu;

#include <avr/pgmspace.h>

void lcdProgMemprint(const char *str)
{
  char ch=pgm_read_byte(str);
  while(ch)
  {
    lcd.print(ch);
    ch=pgm_read_byte(++str);
  }
}
#define lcdprintPGM(x) lcdProgMemprint(PSTR(x))


//===========================================================================
//=============================functions         ============================
//===========================================================================

inline int intround(const float &x){return int(0.5+x);}

void lcd_status(const char* message)
{
  strncpy(messagetext,message,LCD_WIDTH);
}

void lcd_statuspgm(const char* message)
{
  char ch=pgm_read_byte(message);
  char *target=messagetext;
  while(ch)
  {
    *target=ch;
    target++;
    ch=pgm_read_byte(++message);
  }
}

inline void clear()
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
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcd.createChar(1,Degree);
  lcd.createChar(2,Thermometer);
  lcd.createChar(3,uplevel);
  lcd.createChar(4,refresh);
  LCD_MESSAGEPGM("UltiMarlin ready.");
}


void beep()
{
  //return;
  #ifdef ULTIPANEL
    pinMode(BEEPER,OUTPUT);
    for(int8_t i=0;i<20;i++){
      WRITE(BEEPER,HIGH);
      delay(5);
      WRITE(BEEPER,LOW);
      delay(5);
    }
  #endif
}

void beepshort()
{
  //return;
  #ifdef ULTIPANEL
    pinMode(BEEPER,OUTPUT);
    for(int8_t i=0;i<10;i++){
      WRITE(BEEPER,HIGH);
      delay(3);
      WRITE(BEEPER,LOW);
      delay(3);
    }
  #endif  
}

void lcd_status()
{
  #ifdef ULTIPANEL
    static uint8_t oldbuttons=0;
    static long previous_millis_buttons=0;
    static long previous_lcdinit=0;
  //  buttons_check(); // Done in temperature interrupt
    //previous_millis_buttons=millis();
    
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
    WRITE(SDCARDDETECT,HIGH);
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
}


void MainMenu::showStatus()
{ 
#if LCD_HEIGHT==4
  static int olddegHotEnd0=-1;
  static int oldtargetHotEnd0=-1;
  //force_lcd_update=true;
  if(force_lcd_update||feedmultiplychanged)  //initial display of content
  {
    feedmultiplychanged=false;
    encoderpos=feedmultiply;
    clear();
    lcd.setCursor(0,0);lcdprintPGM("\002123/567\001 ");
    #if defined BED_USES_THERMISTOR || defined BED_USES_AD595 
      lcd.setCursor(10,0);lcdprintPGM("B123/567\001 ");
    #endif
  }
    
  int tHotEnd0=intround(degHotend0());
  if((abs(tHotEnd0-olddegHotEnd0)>1)||force_lcd_update) //>1 because otherwise the lcd is refreshed to often.
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
    lcd.setCursor(1,0);
    lcd.print(ftostr3(tBed));
    olddegHotEnd0=tBed;
  }
  int targetBed=intround(degTargetBed());
  if((targetBed!=oldtargetBed)||force_lcd_update)
  {
    lcd.setCursor(5,0);
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
  int currentz=current_position[2]*10;
  if((currentz!=oldzpos)||force_lcd_update)
  {
    lcd.setCursor(10,1);
    lcdprintPGM("Z:");lcd.print(itostr31(currentz));
    oldzpos=currentz;
  }
  static int oldfeedmultiply=0;
  int curfeedmultiply=feedmultiply;
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
    lcd.print(fillto(LCD_WIDTH,messagetext));
    messagetext[0]='\0';
  }
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
    lcd.print(fillto(LCD_WIDTH,messagetext));
    messagetext[0]='\0';
  }

#endif
}

enum {ItemP_exit, ItemP_home, ItemP_origin, ItemP_preheat, ItemP_extrude, ItemP_disstep};

void MainMenu::showPrepare()
{
 uint8_t line=0;
 if(lastlineoffset!=lineoffset)
 {
   force_lcd_update=true;
   clear(); 
 }
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
   //Serial.println((int)(line-lineoffset));
  switch(i)
  {
    case ItemP_exit:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Prepare");
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          status=Main_Menu;
          beepshort();
        }
      }break;
    case ItemP_home:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Auto Home");
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          enquecommand("G28 X-105 Y-105 Z0");
          beepshort();
        }
      }break;
    case ItemP_origin:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Set Origin");
          
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          enquecommand("G92 X0 Y0 Z0");
          beepshort();
        }
      }break;
    case ItemP_preheat:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Preheat"); 
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          setTargetHotend0(170);
          beepshort();
        }
      }break;
    case ItemP_extrude:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Extrude");
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          enquecommand("G92 E0");
          enquecommand("G1 F700 E50");
          beepshort();
        }
      }break;
    case ItemP_disstep:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Disable Steppers");
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          enquecommand("M84");
          beepshort();
        }
      }break;
    default:   
      break;
  }
  line++;
 }
 lastlineoffset=lineoffset;
 if((encoderpos/lcdslow!=lastencoderpos/lcdslow)||force_lcd_update)
 {
   
    lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?' ':' ');
    
    if(encoderpos<0)
    {
     lineoffset--;
     if(lineoffset<0)
       lineoffset=0;
     encoderpos=0;
     force_lcd_update=true;
    }
    if(encoderpos/lcdslow>3)
    {
     lineoffset++;
     encoderpos=3*lcdslow;
     if(lineoffset>(ItemP_disstep+1-LCD_HEIGHT))
       lineoffset=ItemP_disstep+1-LCD_HEIGHT;
     force_lcd_update=true;
    }
    //encoderpos=encoderpos%LCD_HEIGHT;
    lastencoderpos=encoderpos;
    activeline=encoderpos/lcdslow;
    lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?'>':'\003');   
  } 
}
enum {
  ItemC_exit, ItemC_nozzle, 
  ItemC_PID_P,ItemC_PID_I,ItemC_PID_D,ItemC_PID_C,
  ItemC_fan, 
  ItemC_acc, ItemC_xyjerk, 
  ItemC_vmaxx, ItemC_vmaxy, ItemC_vmaxz, ItemC_vmaxe, 
  ItemC_vtravmin,ItemC_vmin,  
  ItemC_amaxx, ItemC_amaxy, ItemC_amaxz, ItemC_amaxe, 
  ItemC_aret,ItemC_esteps, ItemC_store, ItemC_load,ItemC_failsafe
};

void MainMenu::showControl()
{
 uint8_t line=0;
 if((lastlineoffset!=lineoffset)||force_lcd_update)
 {
   force_lcd_update=true;
   clear();
 }
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
  switch(i)
  {
    case ItemC_exit:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Control");
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          status=Main_Menu;
          beepshort();
        }
      }break;
    case ItemC_nozzle:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" \002Nozzle:");
          lcd.setCursor(13,line);lcd.print(ftostr3(intround(degHotend0())));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=intround(degHotend0());
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
        }
      }break;
      
      case ItemC_fan:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Fan speed:");
          lcd.setCursor(13,line);lcd.print(ftostr3(fanpwm));
        }
        
        if((activeline==line) )
        {
          if(CLICKED) //nalogWrite(FAN_PIN,  fanpwm);
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=fanpwm;
            }
            else
            {
              fanpwm = constrain(encoderpos,0,255);
              encoderpos=fanpwm;
              analogWrite(FAN_PIN,  fanpwm);

              beepshort();
            }
            BLOCK;
          }
          if(linechanging)
          {
            if(encoderpos<0) encoderpos=0;
            if(encoderpos>255) encoderpos=255;
            fanpwm=encoderpos;
              analogWrite(FAN_PIN,  fanpwm);
            lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
          }
        }
      }break;
    case ItemC_acc:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Acc:");
          lcd.setCursor(13,line);lcd.print(itostr3(acceleration/100));lcdprintPGM("00");
        }
        
        if((activeline==line) )
        {
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
        }
      }break;
    case ItemC_xyjerk: //max_xy_jerk
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Vxy-jerk: ");
          lcd.setCursor(13,line);lcd.print(itostr3(max_xy_jerk/60));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)max_xy_jerk/60;
            }
            else
            {
              max_xy_jerk= encoderpos*60;
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
        }
      }break;
      case ItemC_PID_P: 
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" PID-P: ");
          lcd.setCursor(13,line);lcd.print(itostr4(Kp));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)Kp/5;
            }
            else
            {
              Kp= encoderpos*5;
              encoderpos=activeline*lcdslow;
                
            }
            BLOCK;
            beepshort();
          }
          if(linechanging)
          {
            if(encoderpos<1) encoderpos=1;
            if(encoderpos>9990/5) encoderpos=9990/5;
            lcd.setCursor(13,line);lcd.print(itostr4(encoderpos*5));
          }
        }
      }break;
    case ItemC_PID_I: 
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" PID-I: ");
          lcd.setCursor(13,line);lcd.print(ftostr51(Ki));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)(Ki*10);
            }
            else
            {
              Ki= encoderpos/10.;
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
        }
      }break;
      case ItemC_PID_D: 
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" PID-D: ");
          lcd.setCursor(13,line);lcd.print(itostr4(Kd));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)Kd/5;
            }
            else
            {
              Kd= encoderpos*5;
              encoderpos=activeline*lcdslow;
                
            }
            BLOCK;
            beepshort();
          }
          if(linechanging)
          {
            if(encoderpos<0) encoderpos=0;
            if(encoderpos>9990/5) encoderpos=9990/5;
            lcd.setCursor(13,line);lcd.print(itostr4(encoderpos*5));
          }
        }
      }break;
    
    
      
    case ItemC_PID_C: 
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" PID-C: ");
          lcd.setCursor(13,line);lcd.print(itostr3(Kc));
        }
        
        if((activeline==line) )
        {
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
      }break;
    case ItemC_vmaxx:
    case ItemC_vmaxy:
    case ItemC_vmaxz:
    case ItemC_vmaxe:
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Vmax ");
          if(i==ItemC_vmaxx)lcdprintPGM("x:");
          if(i==ItemC_vmaxy)lcdprintPGM("y:");
          if(i==ItemC_vmaxz)lcdprintPGM("z:");
          if(i==ItemC_vmaxe)lcdprintPGM("e:");
          lcd.setCursor(13,line);lcd.print(itostr3(max_feedrate[i-ItemC_vmaxx]/60));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)max_feedrate[i-ItemC_vmaxx]/60;
            }
            else
            {
              max_feedrate[i-ItemC_vmaxx]= encoderpos*60;
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
        }
      }break;
    
    case ItemC_vmin:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Vmin:");
          lcd.setCursor(13,line);lcd.print(itostr3(minimumfeedrate/60));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)(minimumfeedrate/60.);
            }
            else
            {
              minimumfeedrate= encoderpos*60;
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
      }break;
    case ItemC_vtravmin:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" VTrav min:");
          lcd.setCursor(13,line);lcd.print(itostr3(mintravelfeedrate/60));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)mintravelfeedrate/60;
            }
            else
            {
              mintravelfeedrate= encoderpos*60;
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
      }break;
    
    case ItemC_amaxx:      
    case ItemC_amaxy:
    case ItemC_amaxz:
    case ItemC_amaxe:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Amax ");
          if(i==ItemC_amaxx)lcdprintPGM("x:");
          if(i==ItemC_amaxy)lcdprintPGM("y:");
          if(i==ItemC_amaxz)lcdprintPGM("z:");
          if(i==ItemC_amaxe)lcdprintPGM("e:");
          lcd.setCursor(13,line);lcd.print(itostr3(max_acceleration_units_per_sq_second[i-ItemC_amaxx]/100));lcdprintPGM("00");
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)max_acceleration_units_per_sq_second[i-ItemC_amaxx]/100;
            }
            else
            {
              max_acceleration_units_per_sq_second[i-ItemC_amaxx]= encoderpos*100;
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
        }
      }break;
    case ItemC_aret://float retract_acceleration = 7000;
    {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" A-retract:");
          lcd.setCursor(13,line);lcd.print(ftostr3(retract_acceleration/100));lcdprintPGM("00");
        }
        
        if((activeline==line) )
        {
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
        }
      }break;
    case ItemC_esteps://axis_steps_per_unit[i] = code_value();
         {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" Esteps/mm:");
          lcd.setCursor(13,line);lcd.print(itostr4(axis_steps_per_unit[3]));
        }
        
        if((activeline==line) )
        {
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
        }
      }break; 
    case ItemC_store:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcdprintPGM(" Store EPROM");
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
        beepshort();
        BLOCK;
        StoreSettings();
      }
    }break;
    case ItemC_load:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcdprintPGM(" Load EPROM");
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
        beepshort();
        BLOCK;
        RetrieveSettings();
      }
    }break;
    case ItemC_failsafe:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcdprintPGM(" Restore Failsafe");
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
        beepshort();
        BLOCK;
        RetrieveSettings(true);
      }
    }break;
    default:   
      break;
  }
  line++;
 }
 lastlineoffset=lineoffset;

 if(!linechanging &&  ((encoderpos/lcdslow!=lastencoderpos/lcdslow)||force_lcd_update))
 {
   
    lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?' ':' ');
    
    if(encoderpos<0)
    {
     lineoffset--;
     if(lineoffset<0)
       lineoffset=0;
     encoderpos=0;
     force_lcd_update=true;
    }
    if(encoderpos/lcdslow>3)
    {
     lineoffset++;
     encoderpos=3*lcdslow;
     if(lineoffset>(ItemC_failsafe+1-LCD_HEIGHT))
       lineoffset=ItemC_failsafe+1-LCD_HEIGHT;
     force_lcd_update=true;
    }
    //encoderpos=encoderpos%LCD_HEIGHT;
    lastencoderpos=encoderpos;
    activeline=encoderpos/lcdslow;
    if(activeline>3) activeline=3;
    lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?'>':'\003');   
  } 
}






void MainMenu::showSD()
{
#ifdef SDSUPPORT
 uint8_t line=0;

 if(lastlineoffset!=lineoffset)
 {
   force_lcd_update=true; 
 }
 static uint8_t nrfiles=0;
 if(force_lcd_update)
 {
   clear();
  if(card.cardOK)
  {
    nrfiles=card.getnrfilenames();
  }
  else
  {
    nrfiles=0;
    lineoffset=0;
  }
  //Serial.print("Nr files:"); Serial.println((int)nrfiles);
 }
 
 for(int8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
  switch(i)
  {
    case 0:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcdprintPGM(" File");
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          status=Main_Menu;
          beepshort();
        }
      }break;
    case 1:
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
            lcdprintPGM(" \004Refresh");
          }
          else
          {
            lcdprintPGM(" \004Insert Card");
          }
          
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK;
          beepshort();
          card.initsd();
          force_lcd_update=true;
           nrfiles=card.getnrfilenames();
        }
      }break;
    default:
    {
      if(i-2<nrfiles)
      {
        if(force_lcd_update)
        {
          card.getfilename(i-2);
          //Serial.print("Filenr:");Serial.println(i-2);
          lcd.setCursor(0,line);lcdprintPGM(" ");lcd.print(card.filename);
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          card.getfilename(i-2);
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
      break;
  }
  line++;
 }
 lastlineoffset=lineoffset;
 if((encoderpos!=lastencoderpos)||force_lcd_update)
 {
   
    lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?' ':' ');
    
    if(encoderpos<0)
    {
     lineoffset--;
     if(lineoffset<0)
       lineoffset=0;
     encoderpos=0;
     force_lcd_update=true;
    }
    if(encoderpos/lcdslow>3)
    {
     lineoffset++;
     encoderpos=3*lcdslow;
     if(lineoffset>(1+nrfiles+1-LCD_HEIGHT))
       lineoffset=1+nrfiles+1-LCD_HEIGHT;
     force_lcd_update=true;
     
    }
    lastencoderpos=encoderpos;
    activeline=encoderpos;
    if(activeline>3) 
    {
      activeline=3;
    }
    if(activeline<0) 
    {
      activeline=0;
    }
    if(activeline>1+nrfiles) activeline=1+nrfiles;
    if(lineoffset>1+nrfiles) lineoffset=1+nrfiles;
    lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?'>':'\003');   
    
  }
#endif
}

enum {ItemM_watch, ItemM_prepare, ItemM_control, ItemM_file };
void MainMenu::showMainMenu()
{
   //if(int(encoderpos/lcdslow)!=int(lastencoderpos/lcdslow))
   //  force_lcd_update=true;
  #ifndef ULTIPANEL
    force_lcd_update=false;
  #endif
   //Serial.println((int)activeline);
   if(force_lcd_update)
     clear();
  for(int8_t line=0;line<LCD_HEIGHT;line++)
  {
    switch(line)
    { 
      case ItemM_watch:
      {
        if(force_lcd_update) {lcd.setCursor(0,line);lcdprintPGM(" Watch   \x7E");}
        if((activeline==line)&&CLICKED)
        {
          BLOCK;
          beepshort();
          status=Main_Status;
        }
      } break;
      case ItemM_prepare:
      {
        if(force_lcd_update) {lcd.setCursor(0,line);lcdprintPGM(" Prepare \x7E");}
        if((activeline==line)&&CLICKED)
        {
          BLOCK;
          status=Main_Prepare;
          beepshort();
        }
      } break;
       
      case ItemM_control:
      {
        if(force_lcd_update) {lcd.setCursor(0,line);lcdprintPGM(" Control \x7E");}
        if((activeline==line)&&CLICKED)
        {
          BLOCK;
          status=Main_Control;
          beepshort();
        }
      }break;
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
              lcdprintPGM(" Stop Print   \x7E");
            else
              lcdprintPGM(" Card Menu    \x7E");
          }
          else
          {
           lcdprintPGM(" No Card"); 
          }
        }
        #ifdef CARDINSERTED
          if(CARDINSERTED)
        #endif
        if((activeline==line)&&CLICKED)
        {
          card.sdprinting = false;
          BLOCK;
          status=Main_SD;
          beepshort();
        }
      }break;
      #endif
      default: 
        SERIAL_ERROR_START;
        SERIAL_ERRORLNPGM("Something is wrong in the MenuStructure.");
      break;
    }
  }
  if(activeline<0) 
    activeline=0;
  if(activeline>=LCD_HEIGHT) 
    activeline=LCD_HEIGHT-1;
  if((encoderpos!=lastencoderpos)||force_lcd_update)
  {
    lcd.setCursor(0,activeline);lcd.print(activeline?' ':' ');
    if(encoderpos<0) encoderpos=0;
    if(encoderpos>3*lcdslow) 
      encoderpos=3*lcdslow;
    activeline=abs(encoderpos/lcdslow)%LCD_HEIGHT;
    if(activeline<0) 
      activeline=0;
    if(activeline>=LCD_HEIGHT) 
      activeline=LCD_HEIGHT-1;
    lastencoderpos=encoderpos;
    lcd.setCursor(0,activeline);lcd.print(activeline?'>':'\003');
  }
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
        lcd_status("Card inserted");
      }
      else
      {
        card.release();
        lcd_status("Card removed");
      }
    }
  #endif
 
  if(status!=oldstatus)
  {
    //Serial.println(status);
    //clear();
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
        showPrepare(); 
      }break;
      case Main_Control:
      {
        showControl(); 
      }break;
      case Main_SD: 
      {
        showSD();
      }break;
  }
  
  if(timeoutToStatus<millis())
    status=Main_Status;
  force_lcd_update=false;
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

char *fillto(int8_t n,char *c)
{
  static char ret[25];
  bool endfound=false;
  for(int8_t i=0;i<n;i++)
  {
    ret[i]=c[i];
    if(c[i]==0)
    {
      endfound=true;
    }
    if(endfound)
    {
      ret[i]=' ';
    }
  }
  ret[n]=0;
  return ret;
}


#endif //ULTRA_LCD


