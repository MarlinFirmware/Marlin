#include "lcd.h"
#include "pins.h"

#ifdef ULTRA_LCD
extern volatile int feedmultiply;

#include <LiquidCrystal.h>
LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5,LCD_PINS_D6,LCD_PINS_D7);  //RS,Enable,D4,D5,D6,D7 

unsigned long previous_millis_lcd=0;



#include "buttons.h"

#include "menu_base.h"

char messagetext[LCD_WIDTH]="";



bool force_lcd_update=false;




//return for string conversion routines
char conv[8];

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


#include "menu_base.h"
MenuBase menu;




void lcd_status(const char* message)
{
	strncpy(messagetext,message,LCD_WIDTH);
}

long previous_millis_buttons=0;

void lcd_status()
{
  
#ifdef FANCY_LCD
  if(millis() - previous_millis_buttons<5)
    return;
  buttons_check();
	buttons_process();
  previous_millis_buttons=millis();
#endif //FANCY_LCD
  if(  ((millis() - previous_millis_lcd) < LCD_UPDATE_INTERVAL)  &&  !force_lcd_update  )
    return;
	previous_millis_lcd=millis();
  force_lcd_update=false;
	menu.update();
}

void lcd_init()
{
#ifdef FANCY_LCD
  buttons_init();
	beep();
#endif //FANCY LCD
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



void buttons_process()
{

  if(buttons&B_ST)
  {
    //Serial.println("Red");
    blocking[BL_ST]=millis()+blocktime;
    //enquecommand("M115\n");
  }
  if(buttons&B_LE)
  {
    menu.pageUp();
    blocking[BL_LE]=millis()+blocktime;
  }
  if(buttons&B_RI)
  {
		menu.pageDown();
    blocking[BL_RI]=millis()+blocktime;
  }
  
  if(buttons&B_UP)
  {
		menu.lineUp();
    blocking[BL_UP]=millis()+blocktime;
  }
  if(buttons&B_DW)
  {
		menu.lineDown();
    blocking[BL_DW]=millis()+blocktime;
	}
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

#endif

