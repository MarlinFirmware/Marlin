#include "ultralcd.h"


#ifdef ULTRA_LCD
extern volatile int feedmultiply;
char messagetext[LCD_WIDTH]="";

#include <LiquidCrystal.h>
LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5,LCD_PINS_D6,LCD_PINS_D7);  //RS,Enable,D4,D5,D6,D7 

unsigned long previous_millis_lcd=0;



volatile char buttons=0;  //the last checked buttons in a bit array.
int encoderpos=0;
short lastenc=0;
long blocking[8]={
  0,0,0,0,0,0,0,0};
MainMenu menu;

void lcd_status(const char* message)
{
  strncpy(messagetext,message,LCD_WIDTH);
}

long previous_millis_buttons=0;

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
  LCD_MESSAGE(fillto(LCD_WIDTH,"UltiMarlin ready."));
}


void beep()
{
#ifdef ULTIPANEL
  // [ErikDeBruijn] changed to two short beeps, more friendly
  pinMode(BEEPER,OUTPUT);
  for(int i=0;i<20;i++){
  digitalWrite(BEEPER,HIGH);
  delay(5);
  digitalWrite(BEEPER,LOW);
  delay(5);
  }
#endif
}
void beepshort()
{
#ifdef ULTIPANEL
  // [ErikDeBruijn] changed to two short beeps, more friendly
  pinMode(BEEPER,OUTPUT);
  for(int i=0;i<10;i++){
  digitalWrite(BEEPER,HIGH);
  delay(3);
  digitalWrite(BEEPER,LOW);
  delay(3);
  }
#endif  
}
void lcd_status()
{
#ifdef ULTIPANEL
  static long previous_millis_buttons=0;
  buttons_check();
  //previous_millis_buttons=millis();
  static uint8_t oldbuttons=0;
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
  pinMode(SHIFT_CLK,OUTPUT); 
  pinMode(SHIFT_LD,OUTPUT); 
  pinMode(SHIFT_EN,OUTPUT);
  pinMode(SHIFT_OUT,INPUT); 
  digitalWrite(SHIFT_OUT,HIGH);
  digitalWrite(SHIFT_LD,HIGH);  //load has inverse logic
  digitalWrite(SHIFT_EN,LOW);  //low active
}


void buttons_check()
{
  //read it from the shift register
  volatile static bool busy=false;
	if(busy) 
		return;
	busy=true;
  digitalWrite(SHIFT_LD,LOW);
  delayMicroseconds(20);
  digitalWrite(SHIFT_LD,HIGH);
  buttons=0;
	long ms=millis();
  for(short i=0;i<8;i++)
  { 
    if((blocking[i]<ms))
    {
      if(digitalRead(SHIFT_OUT))
        buttons|=(1<<i);
    }
    else
      buttons|=(1<<i);

    digitalWrite(SHIFT_CLK,HIGH);
		  delayMicroseconds(5);
    digitalWrite(SHIFT_CLK,LOW);
		delayMicroseconds(5);
  }
  buttons=~buttons; //invert it, because a pressed switch produces a logical 0
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
      //Serial.println("I fuck goats");
      ;
    }
  }
  lastenc=enc;
  busy=false;
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
void clearLcd()
{
  
}
void MainMenu::showStatus()
{ 
#if LCD_HEIGHT==4
  static int oldcurrentraw=-1;
  static int oldtargetraw=-1;
  if(force_lcd_update)  //initial display of content
  {
    encoderpos=feedmultiply;
    lcd.setCursor(0,0);lcd.print("\002123/567\001 ");
#if defined BED_USES_THERMISTOR || defined BED_USES_AD595 
    lcd.setCursor(10,0);lcd.print("B123/567\001 ");
#endif
  }
    

  if((abs(current_raw-oldcurrentraw)>3)||force_lcd_update)
  {
    lcd.setCursor(1,0);
    lcd.print(ftostr3(analog2temp(current_raw)));
    oldcurrentraw=current_raw;
  }
  if((target_raw!=oldtargetraw)||force_lcd_update)
  {
    lcd.setCursor(5,0);
    lcd.print(ftostr3(analog2temp(target_raw)));
    oldtargetraw=target_raw;
  }
  #if defined BED_USES_THERMISTOR || defined BED_USES_AD595 
 static int oldcurrentbedraw=-1;
 static int oldtargetbedraw=-1; 
 if((current_bed_raw!=oldcurrentbedraw)||force_lcd_update)
  {
    lcd.setCursor(1,0);
    lcd.print(ftostr3(analog2temp(current_bed_raw)));
    oldcurrentraw=current_raw;
  }
  if((target_bed_raw!=oldtargebedtraw)||force_lcd_update)
  {
    lcd.setCursor(5,0);
    lcd.print(ftostr3(analog2temp(target_bed_raw)));
    oldtargetraw=target_bed_raw;
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
      lcd.print(itostr2(time/60));lcd.print("h ");lcd.print(itostr2(time%60));lcd.print("m");
      oldtime=time;
    }
  }
  static int oldzpos=0;
  int currentz=current_position[2]*10;
  if((currentz!=oldzpos)||force_lcd_update)
  {
    lcd.setCursor(10,1);
    lcd.print("Z:");lcd.print(itostr31(currentz));
    oldzpos=currentz;
  }
  static int oldfeedmultiply=0;
  int curfeedmultiply=feedmultiply;
  if(encoderpos!=curfeedmultiply)
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
   lcd.print(itostr3(curfeedmultiply));lcd.print("% ");
  }
  if(messagetext[0]!='\0')
  {
    lcd.setCursor(0,LCD_HEIGHT-1);
    lcd.print(fillto(LCD_WIDTH,messagetext));
    messagetext[0]='\0';
  }
#else //smaller LCDS----------------------------------
  static int oldcurrentraw=-1;
  static int oldtargetraw=-1;
  if(force_lcd_update)  //initial display of content
  {
    encoderpos=feedmultiply;
    lcd.setCursor(0,0);lcd.print("\002123/567\001 ");
    #if defined BED_USES_THERMISTOR || defined BED_USES_AD595 
    lcd.setCursor(10,0);lcd.print("B123/567\001 ");
    #endif
  }
    

  if((abs(current_raw-oldcurrentraw)>3)||force_lcd_update)
  {
    lcd.setCursor(1,0);
    lcd.print(ftostr3(analog2temp(current_raw)));
    oldcurrentraw=current_raw;
  }
  if((target_raw!=oldtargetraw)||force_lcd_update)
  {
    lcd.setCursor(5,0);
    lcd.print(ftostr3(analog2temp(target_raw)));
    oldtargetraw=target_raw;
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
   lcd.clear(); 
 }
 for(uint8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
   //Serial.println((int)(line-lineoffset));
  switch(i)
  {
    case ItemP_exit:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Prepare");
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
          lcd.setCursor(0,line);lcd.print(" Auto Home");
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
          lcd.setCursor(0,line);lcd.print(" Set Origin");
          
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
          lcd.setCursor(0,line);lcd.print(" Preheat"); 
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          target_raw = temp2analog(170);
          beepshort();
        }
      }break;
    case ItemP_extrude:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Extrude");
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
          lcd.setCursor(0,line);lcd.print(" Disable Steppers");
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
  ItemC_exit, ItemC_nozzle, ItemC_acc, ItemC_xyjerk, 
  ItemC_vmaxx, ItemC_vmaxy, ItemC_vmaxz, ItemC_vmaxe, 
  ItemC_vmin,  
  ItemC_amaxx, ItemC_amaxy, ItemC_amaxz, ItemC_amaxe, 
  ItemC_aret,ItemC_esteps, ItemC_store, ItemC_load,ItemC_failsafe
};

void MainMenu::showControl()
{
 uint8_t line=0;
 if(lastlineoffset!=lineoffset)
 {
   force_lcd_update=true;
   lcd.clear(); 
 }
 for(uint8_t i=lineoffset;i<lineoffset+LCD_HEIGHT;i++)
 {
  switch(i)
  {
    case ItemC_exit:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Control");
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
          lcd.setCursor(0,line);lcd.print(" \002Nozzle:");
          lcd.setCursor(13,line);lcd.print(ftostr3(analog2temp(target_raw)));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)analog2temp(target_raw);
            }
            else
            {
              target_raw = temp2analog(encoderpos);
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
    case ItemC_acc:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Acc:");
          lcd.setCursor(13,line);lcd.print(itostr3(acceleration/100));lcd.print("00");
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
            lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));lcd.print("00");
          }
        }
      }break;
    case ItemC_xyjerk: //max_xy_jerk
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Vxy-jerk: ");
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
    case ItemC_vmaxx:
    case ItemC_vmaxy:
    case ItemC_vmaxz:
    case ItemC_vmaxe:
      {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Vmax ");
          if(i==ItemC_vmaxx)lcd.print("x:");
          if(i==ItemC_vmaxy)lcd.print("y:");
          if(i==ItemC_vmaxz)lcd.print("z:");
          if(i==ItemC_vmaxe)lcd.print("e:");
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
          lcd.setCursor(0,line);lcd.print(" Vmin:");
          lcd.setCursor(13,line);lcd.print(itostr3(minimumfeedrate/60));
        }
        
        if((activeline==line) )
        {
          if(CLICKED)
          {
            linechanging=!linechanging;
            if(linechanging)
            {
               encoderpos=(int)minimumfeedrate/60;
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
    
    case ItemC_amaxx:      
    case ItemC_amaxy:
    case ItemC_amaxz:
    case ItemC_amaxe:
    {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Amax ");
          if(i==ItemC_amaxx)lcd.print("x:");
          if(i==ItemC_amaxy)lcd.print("y:");
          if(i==ItemC_amaxz)lcd.print("z:");
          if(i==ItemC_amaxe)lcd.print("e:");
          lcd.setCursor(13,line);lcd.print(itostr3(max_acceleration_units_per_sq_second[i-ItemC_amaxx]/100));lcd.print("00");
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
            lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));lcd.print("00");
          }
        }
      }break;
    case ItemC_aret://float retract_acceleration = 7000;
    {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" A-retract:");
          lcd.setCursor(13,line);lcd.print(ftostr3(retract_acceleration/100));lcd.print("00");
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
            lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));lcd.print("00");
          }
        }
      }break;
    case ItemC_esteps://axis_steps_per_unit[i] = code_value();
         {
      if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Esteps/mm:");
          lcd.setCursor(13,line);lcd.print(itostr3(axis_steps_per_unit[3]));
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
              axis_steps_per_unit[3]= encoderpos;
              encoderpos=activeline*lcdslow;
                
            }
            BLOCK;
            beepshort();
          }
          if(linechanging)
          {
            if(encoderpos<5) encoderpos=5;
            if(encoderpos>990) encoderpos=990;
            lcd.setCursor(13,line);lcd.print(itostr3(encoderpos));
          }
        }
      }break; 
    case ItemC_store:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcd.print(" Store EPROM");
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
        lcd.setCursor(0,line);lcd.print(" Load EPROM");
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
        lcd.setCursor(0,line);lcd.print(" Restore Failsafe");
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

#include "SdFat.h"

void MainMenu::getfilename(const uint8_t nr)
{
#ifdef SDSUPPORT  
  dir_t p;
  root.rewind();
  uint8_t cnt=0;
  filename[0]='\0';
  while (root.readDir(p) > 0)
  {
    if (p.name[0] == DIR_NAME_FREE) break;
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;
    if(p.name[8]!='G') continue;
    if(p.name[9]=='~') continue;
    if(cnt++!=nr) continue;
    //Serial.println((char*)p.name);
    uint8_t writepos=0;
    for (uint8_t i = 0; i < 11; i++) 
    {
      if (p.name[i] == ' ') continue;
      if (i == 8) {
        filename[writepos++]='.';
      }
      filename[writepos++]=p.name[i];
    }
    filename[writepos++]=0;
  }
#endif  
}

uint8_t getnrfilenames()
{
#ifdef SDSUPPORT
  dir_t p;
  root.rewind();
  uint8_t cnt=0;
  while (root.readDir(p) > 0)
  {
    if (p.name[0] == DIR_NAME_FREE) break;
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;
    if(p.name[8]!='G') continue;
    if(p.name[9]=='~') continue;
    cnt++;
  }
  return cnt;
#endif
}

void MainMenu::showSD()
{
#ifdef SDSUPPORT
 uint8_t line=0;
 if(lastlineoffset!=lineoffset)
 {
   force_lcd_update=true;
   lcd.clear(); 
 }
 static uint8_t nrfiles=0;
 if(force_lcd_update)
 {
  if(sdactive)
  {
    nrfiles=getnrfilenames();
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
          lcd.setCursor(0,line);lcd.print(" File");
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
          lcd.setCursor(0,line);lcd.print(" \004Refresh");
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK;
          beepshort();
          initsd();
          force_lcd_update=true;
           nrfiles=getnrfilenames();
        }
      }break;
    default:
    {
      if(i-2<nrfiles)
      {
        if(force_lcd_update)
        {
          getfilename(i-2);
          //Serial.print("Filenr:");Serial.println(i-2);
          lcd.setCursor(0,line);lcd.print(" ");lcd.print(filename);
        }
        if((activeline==line) && CLICKED)
        {
          BLOCK
          getfilename(i-2);
          char cmd[30];
          for(int i=0;i<strlen(filename);i++)
            filename[i]=tolower(filename[i]);
          sprintf(cmd,"M23 %s",filename);
          //sprintf(cmd,"M115");
          enquecommand(cmd);
          enquecommand("M24");
          beep(); 
          status=Main_Status;
          lcd_status(filename);
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
     //Serial.println((int)lineoffset);
     //Serial.println((int)(2+nrfiles+1-LCD_HEIGHT));
     if(lineoffset>(1+nrfiles+1-LCD_HEIGHT))
       lineoffset=1+nrfiles+1-LCD_HEIGHT;
     force_lcd_update=true;
     
    }
    //encoderpos=encoderpos%LCD_HEIGHT;
    lastencoderpos=encoderpos;
    activeline=encoderpos;
    if(activeline>3) activeline=3;
    if(activeline<0) activeline=0;
    if(activeline>1+nrfiles) activeline=1+nrfiles;
    if(lineoffset>1+nrfiles) lineoffset=1+nrfiles;
    lcd.setCursor(0,activeline);lcd.print((activeline+lineoffset)?'>':'\003');   
    
  }
#endif
}

enum {ItemM_watch, ItemM_prepare, ItemM_control, ItemM_file };
void MainMenu::showMainMenu()
{
   if(encoderpos/lcdslow!=lastencoderpos/lcdslow)
     force_lcd_update=true;
#ifndef ULTIPANEL
   force_lcd_update=false;
#endif
  for(short line=0;line<LCD_HEIGHT;line++)
  {
    switch(line)
    { 
      case ItemM_watch:
      {
        if(force_lcd_update) {lcd.setCursor(0,line);lcd.print(" Watch   \x7E");}
        if((activeline==line)&&CLICKED)
        {
          BLOCK;
          beepshort();
          status=Main_Status;
        }
      } break;
      case ItemM_prepare:
      {
        if(force_lcd_update) {lcd.setCursor(0,line);lcd.print(" Prepare \x7E");}
        if((activeline==line)&&CLICKED)
        {
          BLOCK;
          status=Main_Prepare;
          beepshort();
        }
      } break;
       
      case ItemM_control:
      {
        if(force_lcd_update) {lcd.setCursor(0,line);lcd.print(" Control \x7E");}
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
          if(sdmode)
            lcd.print(" Stop    \x7E");
          else
            lcd.print(" Card    \x7E");
          
        }
        if((activeline==line)&&CLICKED)
        {
          sdmode = false;

          BLOCK;
          status=Main_SD;
          beepshort();
        }
      }break;
#endif
      default: 
      break;
    }
  }
  
  if((encoderpos!=lastencoderpos)||force_lcd_update)
  {
    lcd.setCursor(0,activeline);lcd.print(activeline?' ':' ');
    if(encoderpos<0) encoderpos=0;
    if(encoderpos>3*lcdslow) encoderpos=3*lcdslow;
    activeline=abs(encoderpos/lcdslow)%LCD_HEIGHT;
    
    lastencoderpos=encoderpos;
    lcd.setCursor(0,activeline);lcd.print(activeline?'>':'\003');
  }

  
  
}

void MainMenu::update()
{
  static MainStatus oldstatus=Main_Menu;  //init automatically causes foce_lcd_update=true
  static long timeoutToStatus=0;

  if(status!=oldstatus)
  {
    //Serial.println(status);
    lcd.clear();
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




//return for string conversion routines
char conv[8];

///  convert float to string with +123.4 format
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
///  convert float to string with +123.4 format
char *ftostr31(const float &x)
{
  //sprintf(conv,"%5.1f",x);
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
  //sprintf(conv,"%5.1f",x);
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

///  convert float to string with +1234.5 format
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

#else
inline void lcd_status() {};
#endif

