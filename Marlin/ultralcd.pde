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
  beep();
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
bool force_lcd_update=false;

void lcd_status()
{
  static long previous_millis_buttons=0;
  if(millis() - previous_millis_buttons<5)
    return;
  buttons_check();
  previous_millis_buttons=millis();

  if(  ((millis() - previous_millis_lcd) < LCD_UPDATE_INTERVAL)  &&  !force_lcd_update  )
    return;
  previous_millis_lcd=millis();
  force_lcd_update=false;
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
  activemenu=0;
  subactive=false;
  displayStartingRow=0;
  buttonlcd_inits_init();
  lcd_init();
}

void MainMenu::showStatusRight()
{
#ifdef LCDSTATUSRIGHT
  static int oldcurrentraw=-1;
  static int oldtargetraw=-1;
  if(current_raw!=oldcurrentraw)
  {
    lcd.setCursor(LCD_WIDTH-4,0);
    lcd.print('H');
    lcd.print(ftostr3(analog2temp(current_raw)));
    oldcurrentraw=current_raw;
  }
  if(target_raw!=oldtargetraw)
  {
    lcd.setCursor(LCD_WIDTH-4,1);
    lcd.print('T');
    lcd.print(ftostr3(analog2temp(target_raw)));
    oldtargetraw=target_raw;
  }
#endif
}
void MainMenu::showSubTemperature()
{
  
}
void MainMenu::showSubTune()
{
  
}
void MainMenu::update()
{
  showStatusRight();
 if(!subactive)
 {
   
  for(short line=0;line<LCD_HEIGHT;line++)
  {
    switch(line)
    {
      case 0:    lcd.setCursor(0,line);lcd.print("Alles");break;
      case 1:    lcd.setCursor(0,line);lcd.print("bute");break;
      case 2:    lcd.setCursor(0,line);lcd.print("commt");break;
      default: break;
    }
    
  }
 }//subactive
 else
 {
   switch(activemenu)
   {
     case 0: showSubTemperature();break;
     case 1: showSubTune();break;
     default:
     break;
   }
   
 }

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