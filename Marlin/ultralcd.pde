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

  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcd.createChar(1,Degree);
  lcd.createChar(2,Thermometer);
  LCD_MESSAGE(fillto(LCD_WIDTH,"UltiMarlin ready."));
}


void beep()
{
  // [ErikDeBruijn] changed to two short beeps, more friendly
  pinMode(BEEPER,OUTPUT);
  for(int i=0;i<20;i++){
  digitalWrite(BEEPER,HIGH);
  delay(5);
  digitalWrite(BEEPER,LOW);
  delay(5);
  }
  
}

void lcd_status()
{
  static long previous_millis_buttons=0;
  if(millis() - previous_millis_buttons<5)
    return;
  buttons_check();
  previous_millis_buttons=millis();

  if(  ((millis() - previous_millis_lcd) < LCD_UPDATE_INTERVAL)   )
    return;
  previous_millis_lcd=millis();
  
  menu.update();
}

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

MainMenu::MainMenu()
{
  status=Main_Status;
  displayStartingRow=0;
  activeline=0;
  force_lcd_update=true;
  buttons_init();
  lcd_init();
}
void clearLcd()
{
  
}
void MainMenu::showStatus()
{ 
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
    

  if((current_raw!=oldcurrentraw)||force_lcd_update)
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
  if(starttime!=0)
  {
    lcd.setCursor(0,1);
    uint16_t time=millis()/60000-starttime/60000;
    lcd.print(itostr2(time/60));lcd.print("h ");lcd.print(itostr2(time%60));lcd.print("m");
  }
  static int oldzpos=0;
  int currentz=current_position[3]*10;
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
    lcd.setCursor(0,3);
    lcd.print(messagetext);
    messagetext[0]='\0';
  }
  
}

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
    case 0:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Exit \"Prepare\"");
        }
        if((activeline==line) && CLICKED)
        {
          status=Main_Menu;
        }
      }break;
    case 1:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Auto Home");
        }
        if((activeline==line) && CLICKED)
        {
          enquecommand("G28 X-105 Y-105 Z0");
        }
      }break;
    case 2:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Set Origin");
          
        }
        if((activeline==line) && CLICKED)
        {
          enquecommand("G92 X0 Y0 Z0");
        }
      }break;
    case 3:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Preheat"); 
        }
        if((activeline==line) && CLICKED)
        {
          target_raw = temp2analog(170);
        }
      }break;
    case 4:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Extrude");
        }
        if((activeline==line) && CLICKED)
        {
          enquecommand("G92 E0");
          enquecommand("G1 F700 E50");
        }
      }break;
    case 5:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Disable Steppers");
        }
        if((activeline==line) && CLICKED)
        {
          enquecommand("M84");
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
   
    lcd.setCursor(0,activeline);lcd.print(' ');
    
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
     if(lineoffset>(5+1-LCD_HEIGHT))
       lineoffset=5+1-LCD_HEIGHT;
     force_lcd_update=true;
    }
    //encoderpos=encoderpos%LCD_HEIGHT;
    lastencoderpos=encoderpos;
    activeline=encoderpos/lcdslow;
    lcd.setCursor(0,activeline);lcd.print('>');   
  } 
}
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
    case 0:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Exit \"Control\"");
        }
        if((activeline==line) && CLICKED)
        {
          status=Main_Menu;
        }
      }break;
    case 1:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Nozzle:");
        }
        if((activeline==line) && CLICKED)
        {
        }
      }break;
    case 2:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" a:");
          
        }
        if((activeline==line) && CLICKED)
        {
          enquecommand("G92 X0 Y0 Z0");
        }
      }break;
    case 3:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" v-max x:"); 
        }
        if((activeline==line) && CLICKED)
        {
          //target_raw = temp2analog(170);
        }
      }break;
    case 4:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" v-max y:");
        }
        if((activeline==line) && CLICKED)
        {
          //enquecommand("G92 E0");
          //enquecommand("G1 F700 E50");
        }
      }break;
    case 5:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" v-max z:");
        }
        if((activeline==line) && CLICKED)
        {
          //enquecommand("M84");
        }
      }break;
    case 6:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" v-max e:");
        }
        if((activeline==line) && CLICKED)
        {
          //enquecommand("M84");
        }
      }break;
    
    case 7:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcd.print(" v-min:");
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
      }
    }break;
    case 8:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" a-max x:"); 
        }
        if((activeline==line) && CLICKED)
        {
          //target_raw = temp2analog(170);
        }
      }break;
    case 9:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" a-max y:");
        }
        if((activeline==line) && CLICKED)
        {
          //enquecommand("G92 E0");
          //enquecommand("G1 F700 E50");
        }
      }break;
    case 10:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" a-max z:");
        }
        if((activeline==line) && CLICKED)
        {
          //enquecommand("M84");
        }
      }break;
    case 11:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcd.print(" Store EPROM");
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
      }
    }break;
    case 12:
    {
      if(force_lcd_update)
      {
        lcd.setCursor(0,line);lcd.print(" Restore Failsafe");
      }
      if((activeline==line) && CLICKED)
      {
        //enquecommand("M84");
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
   
    lcd.setCursor(0,activeline);lcd.print(' ');
    
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
     encoderpos=3;
     if(lineoffset>(12+1-LCD_HEIGHT))
       lineoffset=12+1-LCD_HEIGHT;
     force_lcd_update=true;
    }
    //encoderpos=encoderpos%LCD_HEIGHT;
    lastencoderpos=encoderpos;
    activeline=encoderpos/lcdslow;
    lcd.setCursor(0,activeline);lcd.print('>');   
  } 
}

void MainMenu::showSD()
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
    case 0:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Exit \"File\"");
        }
        if((activeline==line) && CLICKED)
        {
          status=Main_Menu;
        }
      }break;
    case 1:
      {
        if(force_lcd_update)
        {
          lcd.setCursor(0,line);lcd.print(" Refresh");
        }
        if((activeline==line) && CLICKED)
        {
        }
      }break;
    default:   
      break;
  }
  line++;
 }
 lastlineoffset=lineoffset;
 if((encoderpos!=lastencoderpos)||force_lcd_update)
 {
   
    lcd.setCursor(0,activeline);lcd.print(' ');
    
    if(encoderpos<0)
    {
     lineoffset--;
     if(lineoffset<0)
       lineoffset=0;
     encoderpos=0;
     force_lcd_update=true;
    }
    if(encoderpos>3)
    {
     lineoffset++;
     encoderpos=3*lcdslow;
     if(lineoffset>(2+1-LCD_HEIGHT))
       lineoffset=2+1-LCD_HEIGHT;
     force_lcd_update=true;
     lineoffset=0;
    }
    //encoderpos=encoderpos%LCD_HEIGHT;
    lastencoderpos=encoderpos;
    activeline=encoderpos;
    lcd.setCursor(0,activeline);lcd.print('>');   
  } 
}

void MainMenu::showMainMenu()
{
  for(short line=0;line<3;line++)
  {
    switch(line)
    { 
      case 0:
      {
        if(force_lcd_update) {lcd.setCursor(0,0);lcd.print(" Prepare \x7E");}
        if((activeline==line)&&CLICKED)
          status=Main_Prepare;
      } break;
       
      case 1:
      {
        if(force_lcd_update) {lcd.setCursor(0,1);lcd.print(" Control \x7E");}
        if((activeline==line)&&CLICKED)
          status=Main_Control;
      }break;
      case 2:    
      {
        if(force_lcd_update) {lcd.setCursor(0,2);lcd.print(" File    \x7E");}
        if((activeline==line)&&CLICKED)
          status=Main_SD;
      }break;
      default: 
      break;
    }
  }
  
  if((encoderpos!=lastencoderpos)||force_lcd_update)
  {
    lcd.setCursor(0,activeline);lcd.print(' ');
    activeline=abs(encoderpos/lcdslow)%3;
    lastencoderpos=encoderpos;
    lcd.setCursor(0,activeline);lcd.print('>');
  }

  
  
}

void MainMenu::update()
{
  static MainStatus oldstatus=Main_Menu;  //init automatically causes foce_lcd_update=true
  static long timeoutToStatus=0;

  if(status!=oldstatus)
  {
    Serial.println(status);
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
           status=Main_Menu;
           timeoutToStatus=millis()+STATUSTIMEOUT;
        }
      }break;
      case Main_Menu: 
      {
        showMainMenu(); 
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

#endif