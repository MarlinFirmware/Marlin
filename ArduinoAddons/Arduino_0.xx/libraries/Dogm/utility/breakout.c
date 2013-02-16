/*

  breakout.c: Breakorino C functions
  
  Arduino breakout clone for the dogm128x64 and dogs102x64 display
  
  (c) 2010 Oliver Kraus (olikraus@gmail.com)


  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.
  

*/

#define BO_BUILD "v1.06"


#include <stdio.h>
#include "dogm128.h"


/*================================================================*/
/* definitions and structures */
/*================================================================*/


typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;

#define BO_F1 font_5x7
#define BO_F2 font_6x12
#define BO_F3 font_8x13

#define BO_FP 4

/* bricks form a wall, this defines the maximum size of the wall */
//#ifdef DOGS102_HW
//#define BO_AREA_WIDTH 5
//#else
#define BO_AREA_WIDTH 7
//#endif
#define BO_AREA_HEIGHT 3

#ifdef DOGS102_HW
#define BO_BRICK_WIDTH ((s16)10<<BO_FP)
#else
#define BO_BRICK_WIDTH ((s16)12<<BO_FP)
#endif

#define BO_BRICK_HEIGHT ((s16)4<<BO_FP)

/*
  DOGM128:
    7*(12+4) + 2 = 114 --> 128-114 = 14 pixels empty on the left side
    
*/

#define BO_AREA_UNIT_X (BO_BRICK_WIDTH+((s16)4<<BO_FP))
#define BO_AREA_UNIT_Y (BO_BRICK_HEIGHT+((s16)4<<BO_FP))
#define BO_AREA_ORIG_X ((s16)2<<BO_FP)
#define BO_AREA_ORIG_Y ((s16)(DOG_HEIGHT-32)<<BO_FP)

/* dimensions of the overall game field (BO_FP units) */
#define BO_FIELD_WIDTH (BO_AREA_ORIG_X + BO_AREA_UNIT_X * BO_AREA_WIDTH)
#define BO_FIELD_HEIGHT (BO_AREA_ORIG_Y + BO_AREA_UNIT_Y * BO_AREA_HEIGHT)

/* pixel values */
#define BO_FIELD_X0 2
#define BO_FIELD_Y0 2
#define BO_FIELD_X1 ((u8)(BO_FIELD_X0+((BO_FIELD_WIDTH)>>BO_FP)))
#define BO_FIELD_Y1 ((u8)(BO_FIELD_Y0+((BO_FIELD_HEIGHT)>>BO_FP)))
#define BO_FIELD_PIX_WIDTH ((u8)((BO_FIELD_WIDTH)>>BO_FP))
#define BO_FIELD_PIX_HEIGHT ((u8)((BO_FIELD_HEIGHT)>>BO_FP))

/* brick states */
#define BO_BRICK_NONE 0
#define BO_BRICK_CLOSE8 1 
#define BO_BRICK_CLOSE7 2 
#define BO_BRICK_CLOSE6 3 
#define BO_BRICK_CLOSE5 4 
#define BO_BRICK_CLOSE4 5 
#define BO_BRICK_CLOSE3 6
#define BO_BRICK_CLOSE2 7
#define BO_BRICK_CLOSE_START 8
#define BO_BRICK_NORMAL 10
#define BO_BRICK_SOLID 11
#define BO_BRICK_BALL 12
#define BO_BRICK_NO_REFLECTION 13


struct _bo_ball
{
  s16 dx;	/* fixed point 12.4 */
  s16 dy;  /* fixed point 12.4 */
  s16 x0;	/* fixed point 12.4 */
  s16 x1;	/* fixed point 12.4 */
  s16 y0;   /* fixed point 12.4 */
  s16 y1;   /* fixed point 12.4 */
  s16 lbr_x;	/* last brick reflection */
  s16 lbr_y;	/* last brick reflection */
  unsigned is_ball_reflection;
  unsigned is_ball_lost;
};
typedef struct _bo_ball bo_ball;

bo_ball bo_ball1_obj;
bo_ball bo_ball2_obj;
bo_ball bo_ball3_obj;

struct _bo_player
{
  s16 w;	/* fixed point 12.4 */
  s16 x0;	/* fixed point 12.4 */
  s16 x1;	/* fixed point 12.4 */
  s16 y0;   /* fixed point 12.4 */
  s16 y1;   /* fixed point 12.4 */
};
typedef struct _bo_player bo_player;

/*================================================================*/
/* global variables */
/*================================================================*/


u8 bo_area[BO_AREA_HEIGHT][BO_AREA_WIDTH];
s16 bo_player_brick_points; /* only written to... could  be removed */
s16 bo_remaining_bricks;
s16 bo_no_reflection_cnt = 0;

/*================================================================*/
/* declarations */
/*================================================================*/
void bo_DoBallDelta(bo_ball *b);

/*================================================================*/
/* aux functions */
/*================================================================*/


u8 bo_IsIntersection(s16 a_min, s16 a_max, s16 b_min, s16 b_max)
{
  if ( a_max < b_min )
    return 0;
  if ( a_min > b_max )
    return 0;
  return 1;
}

u8 bo_IsBallIntersection(bo_ball *b, s16 x0, s16 x1, s16 y0, s16 y1)
{
  if ( b->is_ball_lost )
    return 0;
  if ( bo_IsIntersection(b->x0, b->x1, x0, x1) == 0 )
    return 0;
  if ( bo_IsIntersection(b->y0, b->y1, y0, y1) == 0 )
    return 0;
  return 1;
}

/*================================================================*/
/* intersection and reflection */
/*================================================================*/

/*===== brick and area intersection =====*/

/*
  test versus brick at position (x,y) [12.4]
  (x,y) is the lower left point of the brick
*/
u8 bo_IsBallBrickIntersection(bo_ball *b, s16 x, s16 y)
{
  return bo_IsBallIntersection(b, x,x+BO_BRICK_WIDTH,y,y+BO_BRICK_HEIGHT);
}

/*
  (x,y) is the lower left point of the brick
*/
void bo_DoBallBrickReflection(bo_ball *b, s16 x, s16 y)
{
  /* u8 cnt; */
  s16 mx, my;
  
  if ( b->lbr_x == x && b->lbr_y == y )
    return;

  /* calculate middle of the ball */
  mx = (b->x0+b->x1)/2;
  my = (b->y0+b->y1)/2;
  if ( mx >= x && mx <= x + BO_BRICK_WIDTH )
  {
    /* top/bottom or edge reflection */
    if ( my >= y && my <= y + BO_BRICK_HEIGHT )
    {
      /* edge reflection */
      b->dx = - b->dx;
      b->dy = - b->dy;      
      b->is_ball_reflection = 1;
    }
    else
    {
      /* top/bottom or edge reflection */
      b->dy = - b->dy;      
      b->is_ball_reflection = 1;
    }
  }
  else
  {
    /* must be left or right reflection */
    b->dx = - b->dx;
    b->is_ball_reflection = 1;
  }
  /*
  bo_DoBallDelta(b);
  cnt = 6;
  while ( bo_IsBallBrickIntersection(b, x, y) != 0 )
  {
    if ( cnt == 0 )
      break;
    cnt--;
    bo_DoBallDelta(b);
  }
  */
  b->lbr_x = x;
  b->lbr_y = y;
}

/*
  check area for the specified ball
*/
void bo_CheckBrickArea(bo_ball *b)
{
  s16 gx;
  s16 gy;
  u8 ix;
  u8 iy;
  u8 is_reflect = 0;
  gy = BO_AREA_ORIG_Y;
  iy = 0;
  while( iy < BO_AREA_HEIGHT )
  {
    gx = BO_AREA_ORIG_X;
    ix = 0;
    while( ix < BO_AREA_WIDTH )
    {
      switch ( bo_area[iy][ix] )
      {
	case BO_BRICK_NORMAL:
	case BO_BRICK_SOLID:
	case BO_BRICK_NO_REFLECTION:
	  if ( is_reflect == 0 )
	    if ( bo_IsBallBrickIntersection(b, gx, gy) )
	    {
	      if ( bo_area[iy][ix] == BO_BRICK_NO_REFLECTION )
		bo_no_reflection_cnt = 9*256;
	      if ( bo_no_reflection_cnt == 0 )
	      {
		bo_DoBallBrickReflection(b, gx, gy);
		is_reflect = 1;
	      }
	      bo_player_brick_points++;
	      if ( bo_area[iy][ix] != BO_BRICK_SOLID )
	      {
		bo_area[iy][ix] = BO_BRICK_CLOSE_START;
	      }
	    }
	  break;
	case BO_BRICK_BALL:
	  if ( is_reflect == 0 )
	    if ( bo_IsBallBrickIntersection(b, gx, gy) )
	    {
	      bo_ball *bb;
	      /* look for lost ball */
	      if ( bo_ball1_obj.is_ball_lost != 0 )
		bb = &bo_ball1_obj;
	      else if ( bo_ball2_obj.is_ball_lost != 0 )
		bb = &bo_ball2_obj;
	      else if ( bo_ball3_obj.is_ball_lost != 0 )
		bb = &bo_ball3_obj;
	      else
		  bb = NULL;
	      if ( bb != NULL )
		*bb = *b;		/* clone the ball */
	      /* reflection only applies to the original ball */
	      bo_DoBallBrickReflection(b, gx, gy);
	      is_reflect = 1;
	      bo_player_brick_points++;
	      bo_area[iy][ix] = BO_BRICK_CLOSE_START;
	      return;
	    }
	  break;
	case BO_BRICK_CLOSE_START:
	  bo_area[iy][ix] = BO_BRICK_CLOSE2;
	  break;
	case BO_BRICK_CLOSE2:
	  bo_area[iy][ix] = BO_BRICK_CLOSE3;
	  break;
	case BO_BRICK_CLOSE3:
	  bo_area[iy][ix] = BO_BRICK_CLOSE4;
	  break;
	case BO_BRICK_CLOSE4:
	  bo_area[iy][ix] = BO_BRICK_CLOSE5;
	  break;
	case BO_BRICK_CLOSE5:
	  bo_area[iy][ix] = BO_BRICK_CLOSE6;
	  break;
	case BO_BRICK_CLOSE6:
	  bo_area[iy][ix] = BO_BRICK_CLOSE7;
	  break;
	case BO_BRICK_CLOSE7:
	  bo_area[iy][ix] = BO_BRICK_CLOSE8;
	  break;
	case BO_BRICK_CLOSE8:
	  bo_area[iy][ix] = BO_BRICK_NONE;
	  bo_remaining_bricks--;
	  break;
	default:
	  break;
      }
      gx += BO_AREA_UNIT_X;
      ix++;
    }
    gy += BO_AREA_UNIT_Y;
    iy++;
  }  
}

/*===== field intersection =====*/

void bo_CheckField(bo_ball *b)
{
  u8 is_reflection = 0;
  if ( b->x0 <= 0 )
  {
    b->dx = -b->dx;
    is_reflection = 1;
  }
  if ( b->x1 >= BO_FIELD_WIDTH )
  {
    b->dx = -b->dx;
    is_reflection = 1;
  }
  if ( b->y0 <= 0 )
  {
    b->is_ball_lost = 1;
    b->dy = -b->dy;
    is_reflection = 1;
  }
  if ( b->y1 >= BO_FIELD_HEIGHT )
  {
    b->dy = -b->dy;
    is_reflection = 1;
  }
  if ( is_reflection != 0 )
  {
    b->is_ball_reflection = 1;
    /* reset last brick reflection */
    b->lbr_x = -1;
    b->lbr_y = -1;
  }
}

/*===== player intersection =====*/

u8 bo_IsBallPlayerIntersection(bo_ball *b, bo_player *p)
{
  if ( b->is_ball_lost )
    return 0;
  if ( bo_IsIntersection(b->x0, b->x1, p->x0, p->x1) == 0 )
    return 0;
  if ( bo_IsIntersection(b->y0, b->y1, p->y0, p->y1) == 0 )
    return 0;
  return 1;
}

void bo_DoBallPlayerReflection(bo_ball *b, bo_player *p)
{
  s16 mx, my;
  s16 x;
  
  if ( b->is_ball_lost )
    return;

  /* reset last brick reflection */
  b->lbr_x = -1;
  b->lbr_y = -1;  
  
  /* calculate middle of the ball */
  mx = (b->x0+b->x1)/2;
  my = (b->y0+b->y1)/2;
  if ( mx >= p->x0 && mx <= p->x1 )
  {
    /* top/bottom or edge reflection */
    if ( my >= p->y0 && my <= p->y1 )
    {
      /* edge reflection */
      b->dx = - b->dx;
      b->dy = - b->dy;
      b->is_ball_reflection = 1;
    }
    else
    {
      /* top/bottom or edge reflection */
      
      b->dy = - b->dy;      
      
      /* reduce dx, depending on how far the middle of the player paddle was hit */ 
      x = (p->x1 + p->x0)/2;
      x = x - mx;
      /*
      if ( x < 0 )
	x = -x;
      */
      /* x can be between 0 an (p->w <<BO_FP)/2 */
      /* normalize x to 2<<(BO_FP)/2 */
      x /= p->w>>(BO_FP);

      /*
      if ( b->dx < 0 )
	b->dx = -x;
      else
      */
	b->dx = -x;
      
      
      
      b->is_ball_reflection = 1;
    }
  }
  else
  {
    /* must be left or right reflection */
    b->dx = - b->dx;
    b->is_ball_reflection = 1;
  }
}

void bo_CheckPlayer(bo_ball *b, bo_player *p)
{
  if ( bo_IsBallPlayerIntersection(b, p) != 0 )
  {
    bo_DoBallPlayerReflection(b, p);
  }
}

/*================================================================*/
/* handle a single ball */
/*================================================================*/

void bo_DoDeltaLimit(bo_ball *b)
{
  
  if ( b->dx >= (1<<(BO_FP))  )
  {
    b->dx = (1<<(BO_FP));
  }
  if ( b->dy >= (1<<(BO_FP-1))  )
  {
    b->dy = (1<<(BO_FP-1));
  }
  
  if ( b->dy == 0 )
  {
    b->dy = -(1<<(BO_FP-1));
    bo_DoBallDelta(b);
  }

}

void bo_DoBallDelta(bo_ball *b)
{
  b->x0 += b->dx;
  b->x1 += b->dx;
  b->y0 += b->dy;
  b->y1 += b->dy;
}

void bo_DoBallStep(bo_ball *b, bo_player *p)
{
  bo_DoDeltaLimit(b);
  bo_DoBallDelta(b);
  
  b->is_ball_reflection = 0;
  bo_CheckBrickArea(b);
  if ( b->is_ball_reflection == 0 )
  {
    bo_CheckField(b);
    if ( b->is_ball_reflection == 0 )
    {
      if ( p != NULL )
	bo_CheckPlayer(b, p);
    }
  }  
  if ( b->is_ball_reflection != 0 )
  {
    bo_DoBallDelta(b);
  }
}

/*================================================================*/
/* setup */
/*================================================================*/

void bo_CalcRemainingBricks(void)
{
  int w,h;
  int x,y;
  bo_remaining_bricks = 0;
  w = BO_AREA_WIDTH;
  h = BO_AREA_HEIGHT;
  for( y = 0; y < h; y++)
  {
    for( x = 0; x < w; x++)
    {
      if ( bo_area[y][x] == BO_BRICK_NORMAL || bo_area[y][x] == BO_BRICK_BALL || bo_area[y][x] == BO_BRICK_NO_REFLECTION )
	bo_remaining_bricks++;
    }
  }
}

void bo_SetupLevel(u8 level)
{
  int w,h;
  int x,y;
  w = BO_AREA_WIDTH;
  h = BO_AREA_HEIGHT;
  for( y = 0; y < h; y++)
  {
    for( x = 0; x < w; x++)
    {
      bo_area[y][x] = BO_BRICK_NORMAL;
      
      if ( level == 1 )
	if ( ((x+y) & 1) == 0 )
	  bo_area[y][x] = BO_BRICK_NONE;
      
      if ( level == 2 )
	if ( y == 0 )
	  if ( (x & 1) == 0 )
	    bo_area[y][x] = BO_BRICK_SOLID;
      
      if ( level == 3 )
	if ( y == 1 )
	  if ( ( x & 1 ) == 0 )
	    bo_area[y][x] = BO_BRICK_BALL;
	  
      if ( level == 4 )
      {
	if ( y == 1 )
	  if ( x != 0 )
	    bo_area[y][x] = BO_BRICK_SOLID;
	if ( y == 0 )
	  if ( ( x & 1 ) == 0 )
	    bo_area[y][x] = BO_BRICK_NO_REFLECTION;
      }
    }
  }
  
  
  bo_CalcRemainingBricks();
}

void bo_SetupBall(bo_ball *b)
{
  b->dx = (1<<BO_FP)/2;
  b->dy = (1<<BO_FP)/2;
  b->x0 = (1<<BO_FP);
  b->y0 = (1<<BO_FP) + 2*(1<<BO_FP);
  b->x1 = 5*(1<<BO_FP);
  b->y1 = 5*(1<<BO_FP) + 2*(1<<BO_FP);
  
  b->lbr_x = -1;
  b->lbr_y = -1;
  
  b->is_ball_lost = 0;
  b->is_ball_reflection = 0;
  
}

/* pos is between 0 and 255 */
void bo_SetPlayerPos(bo_player *p, u8 pos)
{
  u8 tmp;
  tmp = ((BO_FIELD_WIDTH-p->w-BO_FP)>>BO_FP) ;
  tmp = (u8)(((u16)tmp*(u16)pos)/255);
  p->x0 = ((s16)tmp) << BO_FP;
  p->x1 = p->x0 + p->w;
  p->y0 = (s16)1 << BO_FP;
  p->y1 = (s16)3 << BO_FP;
}

void bo_SetupPlayer(bo_player *p)
{
  p->w = (s16)22*(s16)(1<<BO_FP);
  bo_SetPlayerPos(p, 128);
}

/*================================================================*/
/* dogm128 graphics */
/*================================================================*/


void draw_brick(u8 ox, u8 oy, u8 brick_status)
{
  u8 w,h;
  w = BO_BRICK_WIDTH>>BO_FP;
  h = BO_BRICK_HEIGHT>>BO_FP;
  switch( brick_status)
  {
	case BO_BRICK_NORMAL:
	  dog_SetBox(ox, oy, ox+w, oy+h);
	  return;
	case BO_BRICK_SOLID:
	  dog_SetBox(ox, oy, ox+w, oy+h);
	  dog_ClrBox(ox+1, oy+1, ox+w-1, oy+h-1);
	  return;
	case BO_BRICK_BALL:
	  dog_SetBox(ox, oy, ox+w, oy+h);
	  dog_ClrVLine(ox+2, oy+1, oy+3 );
	  dog_ClrPixel(ox+1, oy+2);
	  dog_ClrPixel(ox+3, oy+2);
	  return;
	case BO_BRICK_NO_REFLECTION:
	  {
	    const unsigned char b[1] = { 0xaa };
	    dog_SetBox(ox, oy, ox+w, oy+h);
	    dog_ClrHLine(ox, ox+4, oy);
	    dog_SetHBitmap(ox, oy, b, 5); 
	    dog_ClrHLine(ox, ox+4, oy+h);
	    dog_SetHBitmap(ox, oy+h, b, 5); 
	    dog_ClrHLine(ox+w-4, ox+w, oy);
	    dog_SetHBitmap(ox+w-4, oy, b, 5); 
	    dog_ClrHLine(ox+w-4, ox+w, oy+h);
	    dog_SetHBitmap(ox+w-4, oy+h, b, 5); 
	  }
	  return;
	case BO_BRICK_CLOSE_START:
	  dog_SetBox(ox-2, oy+1, ox+w+2, oy+h);
	  return;
	case BO_BRICK_CLOSE2:
	  dog_SetBox(ox-3, oy+1, ox+w+3, oy+h);
	  return;
	case BO_BRICK_CLOSE3:
	  dog_SetBox(ox-4, oy+1, ox+w+4, oy+h-1);
	  return;
	case BO_BRICK_CLOSE4:
	  dog_SetBox(ox-5, oy+1, ox+w+5, oy+h-1);
	  return;
	case BO_BRICK_CLOSE5:
	  dog_SetBox(ox-6, oy+1, ox+w+6, oy+h-1);
	  return;
	case BO_BRICK_CLOSE6:
	  dog_SetBox(ox-7, oy+2, ox+w+7, oy+h-1);
	  return;
	case BO_BRICK_CLOSE7:
	  dog_SetBox(ox-8, oy+2, ox+w+8, oy+h-1);
	  return;
	case BO_BRICK_CLOSE8:
	  dog_SetBox(ox-9, oy+2, ox+w+9, oy+h-1);
	  return;
	default:
	  return;
  }
}

void draw_bricks(void)
{
  int w,h;
  int x,y;
  int fx, fy;
  w = BO_AREA_WIDTH;
  h = BO_AREA_HEIGHT;
  for( y = 0; y < h; y++)
  {
    for( x = 0; x < w; x++)
    {
      fx = BO_AREA_ORIG_X + x*BO_AREA_UNIT_X;
      fy = BO_AREA_ORIG_Y + y*BO_AREA_UNIT_Y;
      fx >>= BO_FP;
      fy >>= BO_FP;
      fx += BO_FIELD_X0;
      fy += BO_FIELD_Y0;
      draw_brick(fx, fy, bo_area[y][x]);
    }
  }
}

void draw_ball(bo_ball *b)
{
  if ( b->is_ball_lost )
    return;

  //s16 mx, my;
  /* calculate middle of the ball */
  //mx = (b->x0+b->x1)/2;
  //my = (b->y0+b->y1)/2;

  /* convert to screen */
  //mx >>= BO_FP;
  //my >>= BO_FP;


  dog_SetBox((b->x0>>BO_FP)+BO_FIELD_X0, (b->y0>>BO_FP)+BO_FIELD_Y0, (b->x1>>BO_FP)+BO_FIELD_X0, (b->y1>>BO_FP)+BO_FIELD_Y0);
}

void draw_player(bo_player *p)
{
  u8 ox, oy, w, h;
  ox = p->x0>>BO_FP;
  oy = p->y0>>BO_FP;
  ox += BO_FIELD_X0;
  oy += BO_FIELD_Y0;
  
  w = p->w>>BO_FP;
  h = (p->y1-p->y0)>>BO_FP;
  dog_SetBox(ox, oy, ox+w, oy+h);
}

void bo_DrawFPS(uint8_t fps)
{
  if ( fps > 0 )
    dog_DrawStr(85, DOG_HEIGHT-8, BO_F1, dog_itoa(fps));
}

void draw_field(u8 level)
{
  u8 x;
  dog_SetVLine(BO_FIELD_X0, BO_FIELD_Y0, BO_FIELD_Y1);
  dog_SetVLine(BO_FIELD_X1-1, BO_FIELD_Y0, BO_FIELD_Y1);
  x = dog_DrawStr(4, DOG_HEIGHT-8, BO_F1, dog_itoa(bo_remaining_bricks));
  /* dog_DrawStr(x+2, 57, BO_F1, "left"); */
  x = dog_DrawStrP(25, DOG_HEIGHT-8, BO_F1, DOG_PSTR("level"));
  dog_DrawStr(x+2+25, DOG_HEIGHT-8, BO_F1, dog_itoa(level+1));

  if ( bo_no_reflection_cnt > 0 )
    dog_DrawStr(x+2+25+15, DOG_HEIGHT-8, BO_F1, dog_itoa((bo_no_reflection_cnt>>8)+1));
  
}



/*================================================================*/
/* API */
/*================================================================*/

bo_player bo_player_obj;
u8 bo_step_state;
u8 bo_timer;
u8 bo_level;

#define BO_STATE_IN_GAME 1
#define BO_STATE_LOST 2
#define BO_STATE_RESTART 3 
#define BO_STATE_COMLETED 4
#define BO_STATE_INTRO 5
#define BO_STATE_INTRO1 6

void bo_Setup(uint8_t level)
{
  /* level = 4; */
  bo_no_reflection_cnt = 0;
  bo_level = level;
  bo_SetupBall(&bo_ball1_obj);
  bo_SetupBall(&bo_ball2_obj);
  bo_ball2_obj.is_ball_lost = 1;
  bo_SetupBall(&bo_ball3_obj);
  bo_ball3_obj.is_ball_lost = 1;
  bo_SetupPlayer(&bo_player_obj);
  bo_SetupLevel(level);
  if ( level == 0 )
    bo_step_state = BO_STATE_INTRO;
  else
    bo_step_state = BO_STATE_IN_GAME;
}

void bo_Draw(void)
{
    draw_bricks();
    draw_ball(&bo_ball1_obj);
    draw_ball(&bo_ball2_obj);
    draw_ball(&bo_ball3_obj);
    draw_player(&bo_player_obj);
    draw_field(bo_level);
    if ( bo_step_state == BO_STATE_INTRO1 )
    {
      DOG_PSTR_P s1 = DOG_PSTR("Breakorino");
      DOG_PSTR_P s2 = DOG_PSTR(BO_BUILD);
      s16 o = ((dog_sin(((s16)bo_timer)*3))/21);
      u8 w;
      
      w = dog_GetStrWidthP(BO_F2, s1);
      dog_DrawStrP(BO_FIELD_X0 + (BO_FIELD_PIX_WIDTH-w)/2, 18+o, BO_F2, s1);
      
      w = dog_GetStrWidthP(BO_F2, s2);
      dog_DrawStrP(BO_FIELD_X0 + (BO_FIELD_PIX_WIDTH-w)/2, 10+o, BO_F2, s2);
    }
    if ( bo_step_state == BO_STATE_LOST )
    {
      DOG_PSTR_P s = DOG_PSTR("Game Over");
      u8 w = dog_GetStrWidthP(BO_F3, s);
      dog_DrawStrP(BO_FIELD_X0 + (BO_FIELD_PIX_WIDTH-w)/2, 10+(bo_timer>>4), BO_F3, s);
    }
    if ( bo_step_state == BO_STATE_COMLETED )
    {
      DOG_PSTR_P s = DOG_PSTR("Completed");
      u8 w = dog_GetStrWidthP(BO_F3, s);
      dog_DrawStrP(BO_FIELD_X0 + (BO_FIELD_PIX_WIDTH-w)/2, 20-(bo_timer>>4), BO_F3, s);
    }
}

void bo_Step(void)
{
    if ( bo_no_reflection_cnt >= 3 )
    {
      bo_no_reflection_cnt-=3;
    }
    else
    {
      bo_no_reflection_cnt=0;
    }
    switch(bo_step_state)
    {
      case BO_STATE_INTRO:
	  bo_step_state = BO_STATE_INTRO1;
	  bo_timer = 127;
	break;
      case BO_STATE_INTRO1:
	bo_timer--;
	if ( bo_timer == 0 )
	  bo_step_state = BO_STATE_IN_GAME;
	break;
      case BO_STATE_IN_GAME:
	bo_DoBallStep(&bo_ball1_obj, &bo_player_obj);
	bo_DoBallStep(&bo_ball2_obj, &bo_player_obj);
	bo_DoBallStep(&bo_ball3_obj, &bo_player_obj);
	if ( bo_ball1_obj.is_ball_lost != 0 && bo_ball2_obj.is_ball_lost != 0  && bo_ball3_obj.is_ball_lost != 0 )
	{
	  bo_step_state = BO_STATE_LOST;
	  bo_timer = 80;
	}
	if ( bo_remaining_bricks == 0 )
	{
	  bo_step_state = BO_STATE_COMLETED;
	  bo_timer = 80;
	}
	break;
      case BO_STATE_LOST:
	bo_timer--;
	if ( bo_timer == 0 )
	  bo_step_state = BO_STATE_RESTART;
	break;
      case BO_STATE_RESTART:
	bo_step_state = BO_STATE_IN_GAME;
	bo_Setup(0);
	break;
      case BO_STATE_COMLETED:
	bo_timer--;
	if ( bo_timer == 0 )
	{
	  bo_step_state = BO_STATE_IN_GAME;
	  bo_Setup(bo_level+1);
	}
    }
}

void bo_SetX(uint8_t pos)
{
  bo_SetPlayerPos(&bo_player_obj, pos);
}

