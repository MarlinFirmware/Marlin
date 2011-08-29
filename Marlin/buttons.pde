#include "buttons.h"
#ifdef FANCY_BUTTONS


long blocking[8]={
  0,0,0,0,0,0,0,0};

volatile int encoderpos=0; //the position of the encoder in encoder positions

volatile char buttons=0;  //the last checked buttons in a bit array.
short lastenc=0;

extern bool force_lcd_update;

void buttons_check()
{
  //read it from the shift register
  volatile static bool busy=false;
	if(busy) return;
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
	//Serial.println((int)buttons);
		#ifdef	BUTTONS_HAVEENCODER
  char enc=0;
  if(buttons&EN_A)
    enc|=(1<<0);
  if(buttons&EN_B)
    enc|=(1<<1);
	
	//Serial.print((int)enc);Serial.print(" ");
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
    //force_lcd_update=true;
	}
  lastenc=enc;
#endif
	busy=false;
}


void buttons_init()
{
  pinMode(SHIFT_CLK,OUTPUT); 
  pinMode(SHIFT_LD,OUTPUT); 
  pinMode(SHIFT_EN,OUTPUT);
  pinMode(SHIFT_OUT,INPUT); 
  digitalWrite(SHIFT_LD,HIGH);  //load has inverse logic
  digitalWrite(SHIFT_EN,LOW);  //low active
}

#endif


