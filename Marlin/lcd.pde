#include "lcd.h"
#include "pins.h"

#ifdef FANCY_LCD
extern volatile int feedmultiply;

#include <LiquidCrystal.h>
LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5,LCD_PINS_D6,LCD_PINS_D7);  //RS,Enable,D4,D5,D6,D7 

unsigned long previous_millis_lcd=0;

#include "buttons.h"

#include "menu_base.h"

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

#include "menu_base.h"
MenuBase menu;

class PageWatch:public MenuPage
{
public:
  PageWatch();

  virtual void activate();
	virtual void update();
	int lastencoder;
};

PageWatch::PageWatch()
{
  xshift=10;items=0;
}

void PageWatch::update()
{
	if(messagetext[0])
	{
		lcd.setCursor(0,1); 
		lcd.print(fillto(LCD_WIDTH,messagetext));
		messagetext[0]=0;
	}
	if(encoderpos!=lastencoder)
	{
		lcd.setCursor(0,2);
		lcd.print("Speed: ");
		if(encoderpos<5) encoderpos=5;
		if(encoderpos>600) encoderpos=600;
		feedmultiply=encoderpos;
		lcd.print(encoderpos);
		lcd.print("  ");
		lastencoder=encoderpos;
	}
	static int n=0;
	if(n++%4)
		return; //slower updates
	char line1[25];
  static char blink=0;
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

void PageWatch::activate()
{
	encoderpos=feedmultiply;
	lcd.setCursor(0,0);
  lcd.print(fillto(LCD_WIDTH," "));
#if 0
  lcd.setCursor(0, 1); 
  //copy last printed gcode line from the buffer onto the lcd
  char cline2[LCD_WIDTH];
  memset(cline2,0,LCD_WIDTH);
  strncpy(cline2,cmdbuffer[(abs(bufindr-1))%BUFSIZE],LCD_WIDTH-1); //the last processed line
  cline2[LCD_WIDTH-1]=0;
  bool print=(strlen(cline2)>0);

  if(1&&print)
  {
    lcd.print(fillto(LCD_WIDTH,cline2));
  }
  if(LCD_HEIGHT>2)
  {
    lcd.setCursor(0, 2);  
    strncpy(cline2,cmdbuffer[(abs(bufindr-2))%BUFSIZE],LCD_WIDTH-1); //the last processed line
    cline2[LCD_WIDTH-1]=0;
    bool print=(strlen(cline2)>0);
    
    if(1&&print)
    {
      lcd.print(fillto(LCD_WIDTH,cline2));
    } 
    lcd.setCursor(0,3);
		lcd.print(fillto(LCD_WIDTH,messagetext));

  }
#else
	lcd.setCursor(0,1);lcd.print(fillto(LCD_WIDTH," "));
	lcd.setCursor(0,2);lcd.print(fillto(LCD_WIDTH," "));
	lcd.setCursor(0,3);lcd.print(fillto(LCD_WIDTH," "));
#endif
	fillline();
	update();
}



class PageMove:public MenuPage
{
public:
  PageMove();

  virtual void activate();
	virtual void update();
};

//extern float current_x, current_y , current_z , current_e ;
extern float current_position[NUM_AXIS];
float target_x,target_y,target_z,target_e;



PageMove::PageMove()
{
  xshift=10;items=4;
}

int lastline=-1;
int lastencoder=0;
int step=2;
void PageMove::update()
{
	
	if(line!=lastline)
	{
		lastencoder=encoderpos;
		target_x=current_position[X_AXIS];
		target_y=current_position[Y_AXIS];
		target_z=current_position[Z_AXIS];
		target_e=current_position[E_AXIS];
	}
	else //update on the same selected line
		if(lastencoder!=encoderpos) //change in encoder
	{
		int d=encoderpos-lastencoder;
		char c=';';
		float cur=0;
		if(line==4)
		{
			step+=d;
			if(step<1) step=1;
		}
		else
		{
			switch(line)
			{
				case 0:c='X';cur=target_x+=step*d/10.;break;
				case 1:c='Y';cur=target_y+=step*d/10.;break;
				case 2:c='Z';cur=target_z+=step*d/10.;break;
				case 3:c='E';cur=target_e+=step*d/10.;break;
			}
			char com[20];
			sprintf(com,"G1 %c%i.%i",c,int(cur),int(fabs(cur*10))%10);
			enquecommand(com);
		}
		lastencoder=encoderpos;
	}
	else
	{
		//Serial.print("encoder: ");Serial.println(encoderpos);
	}
	lastline=line;
  activate();
}

void PageMove::activate()
{
 lcd.setCursor(0,0);
 lcd.print("Manual Move         ");
 lcd.setCursor(0,1);
 lcd.print(" X");lcd.print(ftostr31(current_position[X_AXIS]));lcd.print("   E");lcd.print(ftostr51(current_position[E_AXIS]));
 lcd.setCursor(0,2);
 lcd.print(" Y");lcd.print(ftostr31(current_position[Y_AXIS]));lcd.print("   St");lcd.print(ftostr31(step/10.));lcd.print(" ");
 lcd.setCursor(0,3);
 lcd.print(" Z");lcd.print(ftostr31(current_position[Z_AXIS]));lcd.print("            ");
 fillline();
}

class PageHome:public MenuPage
{
public:
  PageHome();

  virtual void activate();
	virtual void update();
};


PageHome::PageHome()
{
  xshift=10;items=5;
}

void PageHome::update()
{
	if(buttons&B_MI)
  {
    blocking[BL_MI]=millis()+blocktime;
		switch(line)
		{
			case 0:enquecommand("G28 X");break;
			case 1:enquecommand("G28 Y");break;
			case 2:enquecommand("G28 Z");break;
			case 3:enquecommand("G92 X0");break;
			case 4:enquecommand("G92 Y0");break;
			case 5:enquecommand("G92 Z0");break;
			default:
				;
		}
  }
  activate();
}

void PageHome::activate()
{
 lcd.setCursor(0,0);
 lcd.print(fillto(20,"Home"));
 lcd.setCursor(0,1);
 lcd.print(fillto(20," X         ZERO"));
 lcd.setCursor(0,2);
 lcd.print(fillto(20," Y         ZERO"));
 lcd.setCursor(0,3);
 lcd.print(fillto(20," Z         ZERO"));
	fillline();
}

#ifdef SDSUPPORT
#include "SdFat.h"

class PageSd:public MenuPage
{
public:
  PageSd();

  virtual void activate();
	virtual void update();
	int fileoffset,nrfiles;
};

PageSd::PageSd()
{
  xshift=10;items=7;firstline=0;
	fileoffset=0;
	nrfiles=0;
}

void PageSd::update()
{
	if(encoderpos!=lastencoder) //scoll through files
	{
		fileoffset=encoderpos%nrfiles;
		if(fileoffset>nrfiles-8) fileoffset=nrfiles-8;
		if(fileoffset<0) fileoffset=0;
		activate();
		lastencoder=encoderpos;
		
	}
	if(buttons&B_MI) //start print
  {
    blocking[BL_MI]=millis()+blocktime;
		
		dir_t p;

		root.rewind();
		char filename[11];
		int cnt=0;
		lastencoder=encoderpos;
		while (root.readDir(p) > 0) 
		{
			// done if past last used entry 
			if (p.name[0] == DIR_NAME_FREE) break;

			// skip deleted entry and entries for . and  ..
			if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;

			// only list subdirectories and files
			if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;

			uint8_t writepos=0;
			for (uint8_t i = 0; i < 11; i++) {

				if (p.name[i] == ' ') continue;
				if (i == 8) {
					filename[writepos++]='.';
				}
				filename[writepos++]=p.name[i];
			}
			filename[writepos++]=0;
			if(cnt==line)
				break;
			cnt++;  

		}
		char cmd[50];
		for(int i=0;i<strlen(filename);i++)
			filename[i]=tolower(filename[i]);
		sprintf(cmd,"M23 %s",filename);
		//sprintf(cmd,"M115");
		enquecommand(cmd);
		enquecommand("M24");
		beep();
			
	}
  //activate();
}

void PageSd::activate()
{
	dir_t p;

  root.rewind();
  char filename[11];
	int cnt=0;
	nrfiles=0;
	encoderpos=0;
	while (root.readDir(p) > 0) 
  {
    // done if past last used entry 
    if (p.name[0] == DIR_NAME_FREE) break;

    // skip deleted entry and entries for . and  ..
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;

    // only list subdirectories and files
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;
		nrfiles++;
	}
	root.rewind();
	cnt=0;
	int precount=0;
  while (root.readDir(p) > 0) 
  {
    // done if past last used entry 
    if (p.name[0] == DIR_NAME_FREE) break;

    // skip deleted entry and entries for . and  ..
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;

    // only list subdirectories and files
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;
		if(precount++<fileoffset)
			continue;
		

		uint8_t writepos=0;
		for (uint8_t i = 0; i < 11; i++) {

			if (p.name[i] == ' ') continue;
			if (i == 8) {
				filename[writepos++]='.';
			}
			filename[writepos++]=p.name[i];
		}
		filename[writepos++]=0;
		if(cnt>8)
			break;
		lcd.setCursor(0+10*(cnt/4),cnt%4);
		lcd.print(" ");
		lcd.print(fillto(9,filename));cnt++; 
		
      

  }
  for(;cnt<9;cnt++)
	{
		lcd.setCursor(0+10*(cnt/4),cnt%4);
		lcd.print(fillto(9," "));
	}
		
	fillline();
}

PageSd pagesd;

#endif



PageWatch pagewatch;
PageMove pagemove;
PageHome pagehome;


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
	strncpy(messagetext,message,20);
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
  if(  ((millis() - previous_millis_lcd) < LCD_UPDATE_INTERVAL)  &&  !force_lcd_update  )
    return;
	previous_millis_lcd=millis();
  force_lcd_update=false;
	menu.update();
#endif
}

void lcd_init()
{
  buttons_init();
	beep();
#ifdef FANCY_LCD
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
  lcd.clear();
  //lcd.print(fillto(20,"booting!"));
  //lcd.setCursor(0, 1);
  //lcd.print("lets Marlin!");
	LCD_MESSAGE(fillto(LCD_WIDTH,"Marlin ready."));
#endif
	menu.addMenuPage(&pagewatch);
	menu.addMenuPage(&pagemove);
		menu.addMenuPage(&pagehome);
#ifdef SDSUPPORT
		menu.addMenuPage(&pagesd);
#endif

}







void buttons_process()
{

  if(buttons&B_ST)
  {
    Serial.println("Red");
    blocking[BL_ST]=millis()+blocktime;
    enquecommand("M115\n");
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


///adds an command to the main command buffer
void enquecommand(const char *cmd)
{
  if(buflen < BUFSIZE)
  {
    //this is dangerous if a mixing of serial and this happsens
    strcpy(&(cmdbuffer[bufindw][0]),cmd);
    Serial.print("en:");Serial.println(cmdbuffer[bufindw]);
    bufindw= (bufindw + 1)%BUFSIZE;
    buflen += 1;
  }
}

void beep()
{
  pinMode(BEEPER,OUTPUT);
  digitalWrite(BEEPER,HIGH);
  delay(1000);
  digitalWrite(BEEPER,LOW);

}



#endif