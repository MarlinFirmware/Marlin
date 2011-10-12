#ifdef SIMPLE_LCD

extern volatile int feedmultiply;

#include <LiquidCrystal.h>
#include "Simplelcd.h"

LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5,LCD_PINS_D6,LCD_PINS_D7);  //RS,Enable,D4,D5,D6,D7 

unsigned long previous_millis_lcd=0;

char messagetext[LCD_WIDTH]="";
bool force_lcd_update=false;
extern LiquidCrystal lcd;


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



void lcd_update()
{
	if(messagetext[0])
	{
		lcd.setCursor(0,1); 
		lcd.print(fillto(LCD_WIDTH,messagetext));
		messagetext[0]=0;
	}
	static int n=0;
	if(n++%4)
		return; //slower updates
	char line1[25];
  static char blink=0;
  if (blink == 0)
  {
  	lcd.setCursor(0,0);
  	lcd.print(fillto(LCD_WIDTH,"booting..."));
  	blink++;
  } 
  else
  {
	  sprintf(line1,"%c%3i/%3i\1%c%c%c%c%c%c", ((blink++)%2==0)? (char)2:' ',
	  int(analog2temp(current_raw)),
	  int(analog2temp(target_raw)),
	  (!digitalRead(X_MIN_PIN))? 'x':' ',
	  (!digitalRead(X_MAX_PIN))? 'X':' ',
	  (!digitalRead(Y_MIN_PIN))? 'y':' ',
	  (!digitalRead(Y_MAX_PIN))? 'Y':' ',
	  (!digitalRead(Z_MIN_PIN))? 'z':' ',
	  (!digitalRead(Z_MAX_PIN))? 'Z':' ');

	lcd.setCursor(0,0); 
	lcd.print(fillto(LCD_WIDTH,line1));
  }	
}

void lcd_activate()
{
	lcd.setCursor(0,0);
  lcd.print(fillto(LCD_WIDTH," "));
	lcd.setCursor(0,1);lcd.print(fillto(LCD_WIDTH," "));
	lcd.setCursor(0,2);lcd.print(fillto(LCD_WIDTH," "));
	lcd.setCursor(0,3);lcd.print(fillto(LCD_WIDTH," "));
	//fillline();
	lcd_update();
}



void lcd_status(const char* message)
{
//   if(LCD_HEIGHT>3)
//     lcd.setCursor(0,3);
//   else
//     lcd.setCursor(0,0);
//   lcd.print(message);
//   int missing=(LCD_WIDTH-strlen(message));
//   if(missing>0)
//     for(int i=0;i<missing;i++)
//       lcd.print(" ");
	strncpy(messagetext,message,LCD_WIDTH);
}

long previous_millis_buttons=0;

void lcd_status()
{
  
  if(  ((millis() - previous_millis_lcd) < LCD_UPDATE_INTERVAL)  &&  !force_lcd_update  )
    return;
	previous_millis_lcd=millis();
  force_lcd_update=false;
	lcd_update();
}

void lcd_init()
{
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
	lcd_activate;
}

#endif

