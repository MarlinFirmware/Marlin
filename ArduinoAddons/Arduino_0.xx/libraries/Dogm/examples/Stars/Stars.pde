/*

  Stars.pde
  
  Star simulation for the dogm128 library.
  
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  

  This file is part of the dogm128 Arduino library.

  The dogm128 Arduino library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 Arduino library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.

  
  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O
  
  Note:
    1) Set correct display hardware in Dogm/utility/dogm128.h
    2) Set top/bottom view (DOG_REVERSE) in Dogm/utility/dogm128.h
    See also: http://code.google.com/p/dogm128/wiki/install 
  
*/

#include <Dogm.h>

int sensorPin = 0;  // analog input, use -1 to disable analog input
int sensorValue  = 512; // default sensor value


// define a 3d point structure 
struct pt3d 
{
  int8_t x, y, z;
};

struct pt2d 
{
  int8_t x, y;
  int8_t is_visible;
};

// for the spi interface...
int a0Pin = PIN_A0_DEFAULT;      // address line a0 for the dogm module

// create an array with some stars
#define STAR_CNT 40
struct pt3d star_list[STAR_CNT] ;
struct pt2d star_on_screen_list[STAR_CNT] ;

Dogm dogm(a0Pin);

int8_t xs;			// x position on the screen
int8_t ys;			// y position on the screen
int8_t is_visible;	// is the converted point visible
int8_t zs = 13;		// eye to screen distance (fixed)

// define some additional constants
#define MX (DOG_WIDTH/2)
#define MY (DOG_HEIGHT/2)


// calculate xs and ys from a 3d value
void convert_3d_to_2d(struct pt3d *p3, struct pt2d *p2)
{
  int16_t t;
  p2->is_visible = 1;
  if ( p3->z >= zs )
  {
    t = zs;
    t *= p3->x;
    t <<=1;
    t /= p3->z;
    if ( t >= -MX && t <= MX-1 )
    {
      t += MX;
      p2->x = t;
      t = zs;
      t *= p3->y;
      t <<=1;
      t /= p3->z;
      if ( t >= -MY && t <= MY-1 )
      {
	t += MY;
	p2->y = t;
      }
      else
      {
	p2->is_visible = 0;
      }
    }
    else
    {
      p2->is_visible = 0;
    }
  }
  else
  {
    p2->is_visible = 0;
  }
}

// init star
void init_star(struct pt3d *pt)
{
    pt->z = 100-random(70);
    pt->x = random(DOG_WIDTH)-MX;
    pt->y = random(DOG_HEIGHT)-MY;
}

// init the star list
void init_star_list(void)
{
  uint8_t i;
  for( i = 0; i < STAR_CNT; i++)
  {
    init_star(star_list+i);
  }
}

// convert all stars to 2d
void convert_star_list(void)
{
  uint8_t i;
  for( i = 0; i < STAR_CNT; i++)
  {
    convert_3d_to_2d(star_list+i, star_on_screen_list+i);
  }  
}

// draw the stars
void draw_star_list(void)
{
  uint8_t i;
  for( i = 0; i < STAR_CNT; i++)
  {
    if ( star_on_screen_list[i].is_visible )
    {
      if ( star_list[i].z > (zs*3) )
      {
	dogm.setPixel(star_on_screen_list[i].x, star_on_screen_list[i].y);
      }
      else if ( star_list[i].z > (zs*2) )
      {
	dogm.setBox(star_on_screen_list[i].x, star_on_screen_list[i].y, star_on_screen_list[i].x+1, star_on_screen_list[i].y+1);
      }
      else
      {
	dogm.setBox(star_on_screen_list[i].x-1, star_on_screen_list[i].y-1, star_on_screen_list[i].x+1, star_on_screen_list[i].y+1);
      }
    }    
  }  
}

// move start list
void move_star_list(void)
{
  uint8_t i;
  int8_t dx;
  dx = sensorValue>>6;
  dx -= 8;
  for( i = 0; i < STAR_CNT; i++)
  {
    if ( star_on_screen_list[i].is_visible == 0 )
    {
      init_star(star_list+i);
    }
    else
    {
      star_list[i].z--;
      star_list[i].x-=dx;
    }
  }
}

void setup() {
  init_star_list();
}

void loop() {
  if ( sensorPin >= 0 )
    sensorValue = analogRead(sensorPin);

  convert_star_list();
  dogm.start();
  do {
    draw_star_list();
  } while( dogm.next() );
  move_star_list();
  dog_Delay(100);
}
