/*
  spacetrash.c
  
  A game, which runs on the DOGM132 (and all other displays).
  < 512 Bytes RAM
  < 8K Bytes FlashROM

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

#include "dogm128.h"

/*
#define DOG_HEIGHT 32
#define DOG_WIDTH 128
*/

#define ST_AREA_HEIGHT (DOG_HEIGHT - 8)
#define ST_AREA_WIDTH DOG_WIDTH

#define ST_FP 4

/* object types */
struct _st_ot_struct
{
  /* 
    missle and hit: 
      bit 0: player missle and trash
      bit 1: trash, which might hit the player
  */
  
  uint8_t missle_mask;		/* this object is a missle: it might destroy something if the target is_hit_fn says so */ 
  uint8_t hit_mask;			/* if missle_mask & hit_mask is != 0  then the object can be destroyed */
  uint8_t points;
  uint8_t draw_fn;
  uint8_t move_fn;
      /* ST_MOVE_FN_NONE, ST_MOVE_FN_X_SLOW */
  uint8_t destroy_fn;		/* object can be destroyed by a missle (e.g. a missle from the space ship) */
      /* ST_DESTROY_FN_NONE, ST_DESTROY_FN_SPLIT */
  uint8_t is_hit_fn;		/* is hit procedure */
      /* ST_IS_HIT_FN_NONE, ST_IS_HIT_BBOX */
  uint8_t fire_fn;
    /* ST_FIRE_FN_NONE, ST_FIRE_FN_X_LEFT */
  
};
typedef struct _st_ot_struct st_ot;

/*
  objects, which are visible at the play area
*/
struct _st_obj_struct
{
  uint8_t ot; 			/* object type: zero means, object is not used */
  int8_t tmp;			/* generic value, used by ST_MOVE_IMPLODE */
  /* absolute position */
  /* LCD pixel position is x>>ST_FP and y>>ST_FP */
  int16_t x, y;
  int8_t x0,y0,x1,y1;	/* object outline in pixel, reference point is at 0,0 */
};
typedef struct _st_obj_struct st_obj;

#define ST_DRAW_NONE 0
#define ST_DRAW_BBOX 1
#define ST_DRAW_TRASH1 2
#define ST_DRAW_PLAYER1 3
#define ST_DRAW_TRASH2 4
#define ST_DRAW_PLAYER2 5
#define ST_DRAW_PLAYER3 6
#define ST_DRAW_GADGET 7
#define ST_DRAW_BACKSLASH 8
#define ST_DRAW_SLASH 9
#define ST_DRAW_BIG_TRASH 10

#define ST_MOVE_NONE 0
#define ST_MOVE_X_SLOW 1
#define ST_MOVE_PX_NORMAL 2
#define ST_MOVE_PX_FAST 3
#define ST_MOVE_PLAYER 4
#define ST_MOVE_PY 5
#define ST_MOVE_NY 6
#define ST_MOVE_IMPLODE 7
#define ST_MOVE_X_FAST 8
#define ST_MOVE_WALL 9
#define ST_MOVE_NXPY 10
#define ST_MOVE_NXNY 11

#define ST_IS_HIT_NONE 0
#define ST_IS_HIT_BBOX 1
#define ST_IS_HIT_WALL 2

#define ST_DESTROY_NONE 0
#define ST_DESTROY_DISAPPEAR 1
#define ST_DESTROY_TO_DUST 2
#define ST_DESTROY_GADGET 3
#define ST_DESTROY_PLAYER 4
#define ST_DESTROY_PLAYER_GADGETS 5
#define ST_DESTROY_BIG_TRASH 6

#define ST_FIRE_NONE 0
#define ST_FIRE_PLAYER1 1
#define ST_FIRE_PLAYER2 2
#define ST_FIRE_PLAYER3 3

#define ST_OT_WALL_SOLID 1
#define ST_OT_BIG_TRASH 2
#define ST_OT_MISSLE 3
#define ST_OT_TRASH1 4
#define ST_OT_PLAYER 5
#define ST_OT_DUST_PY 6
#define ST_OT_DUST_NY 7
#define ST_OT_TRASH_IMPLODE 8
#define ST_OT_TRASH2 9
#define ST_OT_PLAYER2 10
#define ST_OT_PLAYER3 11
#define ST_OT_GADGET 12
#define ST_OT_GADGET_IMPLODE 13
#define ST_OT_DUST_NXPY 14
#define ST_OT_DUST_NXNY 15

DOG_ROM st_ot st_object_types[] DOG_PROGMEM =
{
    /* 0: empty object type */
    { 0, 0,  0, ST_DRAW_NONE, ST_MOVE_NONE, ST_DESTROY_DISAPPEAR, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* 1: wall, player will be destroyed */
    { 2, 1, 30, ST_DRAW_BBOX, ST_MOVE_WALL, ST_DESTROY_DISAPPEAR, ST_IS_HIT_WALL, ST_FIRE_NONE },
    /* ST_OT_BIG_TRASH (2) */
    { 2, 1,  0, ST_DRAW_BIG_TRASH, ST_MOVE_X_SLOW, ST_DESTROY_BIG_TRASH, ST_IS_HIT_BBOX, ST_FIRE_NONE },
    /* 3: simple space ship (player) missle */
    { 1, 0,  0, ST_DRAW_BBOX, ST_MOVE_PX_FAST, ST_DESTROY_DISAPPEAR, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_TRASH1 (4): trash */
    { 2, 1,  0, ST_DRAW_TRASH1, ST_MOVE_X_SLOW, ST_DESTROY_TO_DUST, ST_IS_HIT_BBOX, ST_FIRE_NONE },
    /* ST_OT_PLAYER (5): player space ship */
    { 0, 2,  0, ST_DRAW_PLAYER1, ST_MOVE_PLAYER, ST_DESTROY_PLAYER, ST_IS_HIT_BBOX, ST_FIRE_PLAYER1 },
    /* ST_OT_DUST_PY (6): Last part of trash  */
    { 0, 0,  0, ST_DRAW_BBOX, ST_MOVE_PY, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_DUST_NY (7): Last part of trash  */
    { 0, 0,  0, ST_DRAW_BBOX, ST_MOVE_NY, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_TRASH_IMPLODE (8): trash was hit */
    { 0, 0,  5, ST_DRAW_TRASH1, ST_MOVE_IMPLODE, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_TRASH2 (9): trash */
    { 2, 1,  0, ST_DRAW_TRASH2, ST_MOVE_X_SLOW, ST_DESTROY_TO_DUST, ST_IS_HIT_BBOX, ST_FIRE_NONE },
    /* ST_OT_PLAYER2 (10): player space ship+1x enhancement */
    { 0, 2,  0, ST_DRAW_PLAYER2, ST_MOVE_PLAYER, ST_DESTROY_PLAYER_GADGETS, ST_IS_HIT_BBOX, ST_FIRE_PLAYER2 },
    /* ST_OT_PLAYER3 (11): player space ship+2x enhancement */
    { 0, 2,  0, ST_DRAW_PLAYER3, ST_MOVE_PLAYER, ST_DESTROY_PLAYER_GADGETS, ST_IS_HIT_BBOX, ST_FIRE_PLAYER3 },
    /* ST_OT_GADGET (12): adds enhancements */
    { 0, 1,  0, ST_DRAW_GADGET, ST_MOVE_X_FAST, ST_DESTROY_GADGET, ST_IS_HIT_BBOX, ST_FIRE_NONE },
    /* ST_OT_GADGET_IMPLODE (13) */
    { 0, 0, 20, ST_DRAW_GADGET, ST_MOVE_IMPLODE, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_DUST_NXPY (14): Last part of trash  */
    { 0, 0,  0, ST_DRAW_BACKSLASH, ST_MOVE_NXPY, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_DUST_NXNY (15): Last part of trash  */
    { 0, 0,  0, ST_DRAW_SLASH, ST_MOVE_NXNY, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    
};

/*================================================================*/
/* list of all objects on the screen */
/*================================================================*/

/* use AVR RAMEND constant to derive the number of allowed objects */

#if RAMEND < 0x300 
#define ST_OBJ_CNT 25
#else
#define ST_OBJ_CNT 45
#endif 

st_obj st_objects[ST_OBJ_CNT];

/*================================================================*/
/* about players space ship*/
/*================================================================*/

/* player position */
uint8_t st_player_pos;

/* points */
#define ST_POINTS_PER_LEVEL 25
uint16_t st_player_points;
uint16_t st_player_points_delayed;
uint16_t st_highscore = 0;

/*================================================================*/
/* overall game state */
/*================================================================*/

#define ST_STATE_PREPARE 0
#define ST_STATE_IPREPARE 1
#define ST_STATE_GAME 2
#define ST_STATE_END 3
#define ST_STATE_IEND 4

uint8_t st_state = ST_STATE_PREPARE;

/*================================================================*/
/* game difficulty */
/*================================================================*/
uint8_t st_difficulty = 1;
#define ST_DIFF_VIS_LEN 30
#define ST_DIFF_FP 5
uint16_t st_to_diff_cnt = 0;

/*================================================================*/
/* bitmaps */
/*================================================================*/

const dog_pgm_uint8_t st_bitmap_player1[] = 
{ 
  /* 01100000 */ 0x060,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 01100000 */ 0x060
};

const dog_pgm_uint8_t st_bitmap_player2[] = 
{   
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060,
  /* 11100000 */ 0x0e0,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 01100000 */ 0x060
};

const dog_pgm_uint8_t st_bitmap_player3[] = 
{   
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060,
  /* 11100000 */ 0x0e0,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 11100000 */ 0x0e0,
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060
 };

const dog_pgm_uint8_t st_bitmap_trash_5x5_1[] = 
{ 
  /* 01110000 */ 0x070,
  /* 11110000 */ 0x0f0,
  /* 11111000 */ 0x0f8,
  /* 01111000 */ 0x078,
  /* 00110000 */ 0x030,
};

const dog_pgm_uint8_t st_bitmap_trash_5x5_2[] = 
{ 
  /* 00110000 */ 0x030,
  /* 11111000 */ 0x0f8,
  /* 11111000 */ 0x0f8,
  /* 11110000 */ 0x0f0,
  /* 01110000 */ 0x070,
};

const dog_pgm_uint8_t st_bitmap_trash_7x7[] = 
{
  /* 00111000 */ 0x038,
  /* 01111100 */ 0x07c,
  /* 11111100 */ 0x0fc,
  /* 11111110 */ 0x0fe,
  /* 11111110 */ 0x0fe,
  /* 01111110 */ 0x07e,
  /* 01111000 */ 0x078,
};

const dog_pgm_uint8_t st_bitmap_gadget[] = 
{ 
  /* 01110000 */ 0x070,
  /* 11011000 */ 0x0d8,
  /* 10001000 */ 0x088,
  /* 11011000 */ 0x0d8,
  /* 01110000 */ 0x070,
};

/*================================================================*/
/* forward definitions */
/*================================================================*/
uint8_t st_rnd(void) DOG_ATTR_FN_NOINLINE;
static st_obj *st_GetObj(uint8_t objnr) DOG_ATTR_FN_NOINLINE;
uint8_t st_GetMissleMask(uint8_t objnr);
uint8_t st_GetHitMask(uint8_t objnr);
int8_t st_FindObj(uint8_t ot) DOG_ATTR_FN_NOINLINE;
void st_ClrObjs(void) DOG_ATTR_FN_NOINLINE;
int8_t st_NewObj(void) DOG_ATTR_FN_NOINLINE;
uint8_t st_CntObj(uint8_t ot);
uint8_t st_CalcXY(st_obj *o) DOG_ATTR_FN_NOINLINE;
void st_SetXY(st_obj *o, uint8_t x, uint8_t y) DOG_ATTR_FN_NOINLINE;

void st_FireStep(uint8_t is_auto_fire, uint8_t is_fire) DOG_ATTR_FN_NOINLINE;

void st_InitTrash(uint8_t x, uint8_t y, int8_t dir);
void st_NewGadget(uint8_t x, uint8_t y);
void st_NewPlayerMissle(uint8_t x, uint8_t y) ;
void st_NewTrashDust(uint8_t x, uint8_t y, int ot);
void st_NewTrashDustAreaArgs(int16_t x, int16_t y, int ot); 
void st_SetupPlayer(uint8_t objnr, uint8_t ot);


/*================================================================*/
/* utility functions */
/*================================================================*/


uint8_t st_rnd(void)
{
  return rand();
}

/*	
  for the specified index number, return the object
*/
static st_obj *st_GetObj(uint8_t objnr)
{
  return st_objects+objnr;
}


/*
  check, if this is a missle-like object (that is, can this object destroy something else)
*/ 
uint8_t st_GetMissleMask(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  return dog_pgm_read(&(st_object_types[o->ot].missle_mask));
}

/*
  check, if this is a missle-like object (that is, can this object destroy something else)
*/ 
uint8_t st_GetHitMask(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  return dog_pgm_read(&(st_object_types[o->ot].hit_mask));
}

/*
  search an empty object
*/
int8_t st_FindObj(uint8_t ot)
{
  int8_t i;
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    if ( st_objects[i].ot == ot )
      return i;
  }
  return -1;
}

/*
  delete all objects
*/

void st_ClrObjs(void)
{
  int8_t i;
  for( i = 0; i < ST_OBJ_CNT; i++ )
    st_objects[i].ot = 0;
}

/*
  search an empty object
*/
int8_t st_NewObj(void)
{
  int8_t i;
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    if ( st_objects[i].ot == 0 )
      return i;
  }
  return -1;
}

/*
  count number of objectes of the provided type
  st_CntObj(0) will return the number of empty objects, that means if
  st_CntObj(0) > 0 then st_NewObj() will return  a valid index
*/
uint8_t st_CntObj(uint8_t ot)
{
  uint8_t i;
  uint8_t cnt = 0;
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    if ( st_objects[i].ot == ot )
      cnt++;
  }
  return cnt;
}

/*
  calculate the pixel coordinates of the reference point of an object
  return rhe x value
*/
uint8_t st_px_x, st_px_y; /* pixel within area */
uint8_t st_CalcXY(st_obj *o)
{
  //st_obj *o = st_GetObj(objnr);
  st_px_y = o->y>>ST_FP;
  st_px_x = o->x>>ST_FP;
  return st_px_x;
}

void st_SetXY(st_obj *o, uint8_t x, uint8_t y)
{
  o->x = ((int16_t)x) << ST_FP;
  o->y = ((int16_t)y) << ST_FP;
}

/*
  calculate the object bounding box and place it into some global variables
*/
int16_t st_bbox_x0, st_bbox_y0, st_bbox_x1, st_bbox_y1;  

void st_CalcBBOX(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  
  st_bbox_x0 = (uint16_t)(o->x>>ST_FP);
  st_bbox_x1 = st_bbox_x0;
  st_bbox_x0 += o->x0;
  st_bbox_x1 += o->x1;

  st_bbox_y0 = (uint16_t)(o->y>>ST_FP);
  st_bbox_y1 = st_bbox_y0;
  st_bbox_y0 += o->y0;
  st_bbox_y1 += o->y1;
}

/*
  clip bbox with the view window. requires a call to st_CalcBBOX
  return 0, if the bbox is totally outside the window
*/
uint8_t st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1;
uint8_t st_ClipBBOX(void)
{
  if ( st_bbox_x0 >= ST_AREA_WIDTH )
    return 0;
  if ( st_bbox_x0 >= 0 )
    st_cbbox_x0  = (uint16_t)st_bbox_x0;
  else
    st_cbbox_x0 = 0;

  if ( st_bbox_x1 < 0 )
    return 0;
  if ( st_bbox_x1 < ST_AREA_WIDTH )
    st_cbbox_x1  = (uint16_t)st_bbox_x1;
  else
    st_cbbox_x1 = ST_AREA_WIDTH-1;

  if ( st_bbox_y0 >= ST_AREA_HEIGHT )
    return 0;
  if ( st_bbox_y0 >= 0 )
    st_cbbox_y0  = (uint16_t)st_bbox_y0;
  else
    st_cbbox_y0 = 0;

  if ( st_bbox_y1 < 0 )
    return 0;
  if ( st_bbox_y1 < ST_AREA_HEIGHT )
    st_cbbox_y1  = (uint16_t)st_bbox_y1;
  else
    st_cbbox_y1 = ST_AREA_HEIGHT-1;
  
  return 1;
}


/*================================================================*/
/* universal member functions */
/*================================================================*/


uint8_t st_IsOut(uint8_t objnr)
{
  st_CalcBBOX(objnr);
  if ( st_bbox_x0 >= ST_AREA_WIDTH )
    return 1;
  if ( st_bbox_x1 < 0 )
    return 1;
  if ( st_bbox_y0 >= ST_AREA_HEIGHT )
    return 1;
  if ( st_bbox_y1 < 0 )
    return 1;
  return 0;
}

void st_Disappear(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  st_player_points += dog_pgm_read(&(st_object_types[o->ot].points));
  o->ot = 0;
}

/*================================================================*/
/* type dependent member functions */
/*================================================================*/

void st_Move(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  switch(dog_pgm_read(&(st_object_types[o->ot].move_fn)))
  {
    case ST_MOVE_NONE:
      break;
    case ST_MOVE_X_SLOW:
      o->x -= (1<<ST_FP)/8;
      o->x -= st_difficulty;
      o->y += (int16_t)o->tmp;
      if ( o->y >= ((ST_AREA_HEIGHT-1) << ST_FP) || o->y <= 0 )
	o->tmp = - o->tmp;
      break;
    case ST_MOVE_X_FAST:
      o->x -= (1<<ST_FP)/2;
      o->y += (int16_t)o->tmp;
      if ( o->y >= ((ST_AREA_HEIGHT-1) << ST_FP) || o->y <= 0 )
	o->tmp = - o->tmp;
      break;
    case ST_MOVE_PX_NORMAL:
      o->x += (1<<ST_FP)/4;
      break;
    case ST_MOVE_PX_FAST:
      o->x += (1<<ST_FP);
      break;
    case ST_MOVE_PLAYER:
      o->y = st_player_pos<<ST_FP;
      break;
    case ST_MOVE_PY:
      o->y += 3*ST_FP;
      break;
    case ST_MOVE_NY:
      o->y -= 3*ST_FP;
      break;
    case ST_MOVE_NXPY:
      o->y += 3*ST_FP;
      o->x -= 3*ST_FP;
      break;
    case ST_MOVE_NXNY:
      o->y -= 3*ST_FP;
      o->x -= 3*ST_FP;
      break;
    case ST_MOVE_IMPLODE:
      o->tmp++;
      if ( (o->tmp & 0x03) == 0 )
      {
	if ( o->x0 != o->x1 )
	  o->x0++;
	else
	    st_Disappear(objnr);
      }
      break;
    case ST_MOVE_WALL:
      o->x -= 1;
      o->x -= (st_difficulty>>1);
      break;
  }
}

void st_DrawBBOX(uint8_t objnr)
{
  /*st_obj *o = st_GetObj(objnr);*/
  st_CalcBBOX(objnr);
  if ( st_ClipBBOX() == 0 )
    return;
  /* st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1; */

  dog_SetVLine(st_cbbox_x0, st_cbbox_y0, st_cbbox_y1);
  dog_SetVLine(st_cbbox_x1, st_cbbox_y0, st_cbbox_y1);
  dog_SetHLine(st_cbbox_x0, st_cbbox_x1, st_cbbox_y0);
  dog_SetHLine(st_cbbox_x0, st_cbbox_x1, st_cbbox_y1);
  
  //dog_SetBox(st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1);

  /*
  if ( o->ot == ST_OT_PLAYER )
  {
    dog_DrawStr(0, 26, font_4x6, dog_itoa(st_cbbox_y0));
    dog_DrawStr(10, 26, font_4x6, dog_itoa(st_cbbox_y1));
  }
  */
}

#ifdef FN_IS_NOT_IN_USE
void st_DrawFilledBox(uint8_t objnr)
{
  st_CalcBBOX(objnr);
  if ( st_ClipBBOX() == 0 )
    return;
  /* st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1; */
  dog_SetBox(st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1);
}
#endif

void st_DrawBitmap(uint8_t objnr, DOG_PGM_P bm, uint8_t w, uint8_t h)
{
  /* st_obj *o = st_GetObj(objnr); */
  st_CalcBBOX(objnr);
  /* result is here: int16_t st_bbox_x0, st_bbox_y0, st_bbox_x1, st_bbox_y1 */
  dog_SetBitmapP(st_bbox_x0,st_bbox_y1,bm,w,h);
 }

void st_DrawObj(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  switch(dog_pgm_read(&(st_object_types[o->ot].draw_fn)))
  {
    case ST_DRAW_NONE:
      break;
    case ST_DRAW_BBOX:
      st_DrawBBOX(objnr);
      break;
    case ST_DRAW_TRASH1:
      st_DrawBitmap(objnr, st_bitmap_trash_5x5_1,o->x1-o->x0+1, 5);
      break;
    case ST_DRAW_TRASH2:
      st_DrawBitmap(objnr, st_bitmap_trash_5x5_2,o->x1-o->x0+1, 5);
      break;
    case ST_DRAW_BIG_TRASH:
      st_DrawBitmap(objnr, st_bitmap_trash_7x7,o->x1-o->x0+1, 7);
      break;
    case ST_DRAW_PLAYER1:
      st_DrawBitmap(objnr, st_bitmap_player1,7,5);
      break;
    case ST_DRAW_PLAYER2:
      st_DrawBitmap(objnr, st_bitmap_player2,7,8);
      break;
    case ST_DRAW_PLAYER3:
      st_DrawBitmap(objnr, st_bitmap_player3,7,11);
      break;
    case ST_DRAW_GADGET:
      /* could use this proc, but... */
      /* st_DrawBitmap(objnr, st_bitmap_gadget,o->x1-o->x0+1, 5); */
      /* ... this one looks also funny. */
      st_DrawBitmap(objnr, st_bitmap_gadget,5,5);
      break;
    case ST_DRAW_BACKSLASH:
      {
	uint8_t x;
	uint8_t y;
	x = st_CalcXY(o);
	y = st_px_y;

	dog_SetPixel(x,y);
	x++; y--;
	dog_SetPixel(x,y);
	x++; y--;
 	dog_SetPixel(x,y);
     }      
     break;
    case ST_DRAW_SLASH:
      {
	uint8_t x;
	uint8_t y;
	x = st_CalcXY(o);
	y = st_px_y;
	
	dog_SetPixel(x,y);
	x++; y++;
	dog_SetPixel(x,y);
	x++; y++;
 	dog_SetPixel(x,y);
     }      
     break;
  }
}

uint8_t st_IsHitBBOX(uint8_t objnr, uint8_t x, uint8_t y)
{
  st_CalcBBOX(objnr);
  if ( st_ClipBBOX() == 0 ) 
    return 0;	/* obj is outside (not visible) */
  if ( x < st_cbbox_x0 )
    return 0;
  if ( x > st_cbbox_x1 )
    return 0;
  if ( y < st_cbbox_y0 )
    return 0;
  if ( y > st_cbbox_y1 )
    return 0;
  return 1;
}

void st_Destroy(uint8_t objnr)
{
  int8_t nr;
  st_obj *o = st_GetObj(objnr);
  switch(dog_pgm_read(&(st_object_types[o->ot].destroy_fn)))
  {
    case ST_DESTROY_NONE:      /* only usefull for missels or walls which stay alife */
      break;
    case ST_DESTROY_DISAPPEAR:	/* this should be the default operation */
      st_Disappear(objnr);
      break;
    case ST_DESTROY_GADGET:
      nr = st_FindObj(ST_OT_PLAYER2);
      if ( nr >= 0 )
	st_SetupPlayer(nr, ST_OT_PLAYER3);
      else
      {
	nr = st_FindObj(ST_OT_PLAYER);
	if ( nr >= 0 )
	  st_SetupPlayer(nr, ST_OT_PLAYER2);
      }
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_PY);
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_NY);
      o->ot = ST_OT_GADGET_IMPLODE;
      o->tmp = 0;
      break;
    case ST_DESTROY_TO_DUST:
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_PY);
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_NY);
      o->ot = ST_OT_TRASH_IMPLODE;
      o->tmp = 0;
      break;
    case ST_DESTROY_BIG_TRASH:
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_PY);
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_NY);
      st_InitTrash((o->x>>ST_FP)-1, (o->y>>ST_FP)+3, 2+(st_rnd()&3));
      st_InitTrash((o->x>>ST_FP)-2, (o->y>>ST_FP)-3, -2-(st_rnd()&3));
      st_Disappear(objnr);
      break;
    case ST_DESTROY_PLAYER:
      st_Disappear(objnr);
      st_state = ST_STATE_END;
      o->tmp = 0;
      break;
    case ST_DESTROY_PLAYER_GADGETS:
      /* o->ot = ST_OT_PLAYER; */
      st_SetupPlayer(objnr, ST_OT_PLAYER);
      break;
  }
}

/*
  check if the target (objnr) has been hit.
  st_IsHit() must also destroy the target.
  return value:
    0: do not destroy the missle
    1: destroy the missle
*/
uint8_t st_IsHit(uint8_t objnr, uint8_t x, uint8_t y, uint8_t missle_mask)
{
  uint8_t hit_mask = st_GetHitMask(objnr);
  st_obj *o;
  
  if ( (hit_mask & missle_mask) == 0 )
    return 0;
  
  o = st_GetObj(objnr);
  
  switch(dog_pgm_read(&(st_object_types[o->ot].is_hit_fn)))
  {
    case ST_IS_HIT_NONE:
      break;
    case ST_IS_HIT_BBOX:
      if ( st_IsHitBBOX(objnr, x, y) != 0 )
      {
	st_Destroy(objnr);
	return 1;
      }
      break;
    case ST_IS_HIT_WALL:
      if ( st_IsHitBBOX(objnr, x, y) != 0 )
      {
	o->x0++;
	if ( o->x0 < o->x1 )
	{
	  st_NewTrashDust(x, y, ST_OT_DUST_NXPY);
	  st_NewTrashDust(x, y, ST_OT_DUST_NXNY);
	}
	else
	{
	  st_Destroy(objnr);
	  st_NewTrashDust(x, y, ST_OT_DUST_NXPY);
	  st_NewTrashDust(x, y, ST_OT_DUST_NXNY);
	  st_NewTrashDust(x, y, ST_OT_DUST_NY);
	  st_NewTrashDust(x, y, ST_OT_DUST_PY);
	}
	return 1;
      }
      break;
  }
  return 0;
}



/* update all fire counters */
uint8_t st_fire_player = 0;
uint8_t st_fire_period = 51;
uint8_t st_manual_fire_delay = 20;
uint8_t st_is_fire_last_value = 0;

/*
  is_auto_fire == 1
    is_fire will be ignored, autofire enabled
  is_auto_fire == 0
    a transition from 1 to 0 on the is_fire variable will issue a missle
*/
void st_FireStep(uint8_t is_auto_fire, uint8_t is_fire)
{
  if ( is_auto_fire != 0 )
  {
    st_fire_player++;
    if ( st_fire_player >= st_fire_period )
      st_fire_player = 0;
  }
  else
  {
    if ( st_fire_player < st_manual_fire_delay )
    {
      st_fire_player++;
    }
    else
    {
      if ( st_is_fire_last_value == 0 )
	if ( is_fire != 0 )
	  st_fire_player = 0;
    }
    st_is_fire_last_value = is_fire;
  }
}

void st_Fire(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  uint8_t x;
  uint8_t y;
  
  switch(dog_pgm_read(&(st_object_types[o->ot].fire_fn)))
  {
    case ST_FIRE_NONE: 
      break;
    case ST_FIRE_PLAYER1: 
      if ( st_fire_player == 0 )
      {
	/* create missle at st_px_x and st_px_y */
	x = st_CalcXY(o);
	y = st_px_y;
	st_NewPlayerMissle(x , y );
      }
      break;
    case ST_FIRE_PLAYER2:
      if ( st_fire_player == 0 )
      {
	/* create missle at st_px_x and st_px_y */
	x = st_CalcXY(o);
	y = st_px_y;
	st_NewPlayerMissle(x , y );
	st_NewPlayerMissle(x , y+4 );
      }
      break;
    case ST_FIRE_PLAYER3:
      if ( st_fire_player == 0 )
      {
	/* create missle at st_px_x and st_px_y */
	x = st_CalcXY(o);
	y = st_px_y;
	st_NewPlayerMissle(x , y );
	st_NewPlayerMissle(x , y+4 );
	st_NewPlayerMissle(x , y-4 );
      }
      break;
  }
}

/*================================================================*/
/* object init functions */
/*================================================================*/

/*
  x,y are pixel coordinats within the play arey
*/
void st_NewGadget(uint8_t x, uint8_t y)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  st_SetXY(o, x, y);
  o->ot = ST_OT_GADGET;
  o->tmp = 8;
  //o->x = (x)<<ST_FP;
  //o->y = (y)<<ST_FP;
  o->x0 = -3;
  o->x1 = 1;
  o->y0 = -2;
  o->y1 = 2;
}

/*
  x,y are pixel coordinats within the play arey
  dir: direction
    0: random
   != 0 --> assigned
*/
void st_InitTrash(uint8_t x, uint8_t y, int8_t dir)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  if ( (st_rnd() & 1) == 0 )
    o->ot = ST_OT_TRASH1;
  else
    o->ot = ST_OT_TRASH2;
  if ( dir == 0 )
  {
    o->tmp = 0;
    if ( st_rnd() & 1 )
    {
      if ( st_rnd() & 1 )
	o->tmp++;
      else
	o->tmp--;
    }
  }
  else
  {
    o->tmp = dir;
  }
  st_SetXY(o, x, y);
  //o->x = (x)<<ST_FP;
  //o->y = (y)<<ST_FP;
  o->x0 = -3;
  o->x1 = 1;
  o->y0 = -2;
  o->y1 = 2;
  if ( st_difficulty >= 5 )
  {
    if ( (st_rnd() & 3) == 0 )
    {
      o->ot = ST_OT_BIG_TRASH;
      o->y0--;
      o->y1++;
      o->x0--;
      o->x1++;
    }
  }
}

/*
  x,y are pixel coordinats within the play arey
*/
void st_NewTrashDust(uint8_t x, uint8_t y, int ot)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  o->ot = ot;
  st_SetXY(o, x, y);
  //o->x = (x)<<ST_FP;
  //o->y = (y)<<ST_FP;
  o->x0 = 0;
  o->x1 = 0;
  o->y0 = -2;
  o->y1 = 2;
}

void st_NewTrashDustAreaArgs(int16_t x, int16_t y, int ot)
{
  st_NewTrashDust(x>>ST_FP, y>>ST_FP, ot);
}


void st_NewWall(void)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  int8_t h;
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  o->ot = ST_OT_WALL_SOLID;
  h = st_rnd();
  h &= 63;
  h = (int8_t)(((int16_t)h*(int16_t)(ST_AREA_HEIGHT/4))>>6);
  h += ST_AREA_HEIGHT/6;

  o->x0 = 0;
  o->x1 = 5;
  o->x = (ST_AREA_WIDTH-1)<<ST_FP;
 
  if ( (st_rnd() & 1) == 0 )
  {
    o->y = (ST_AREA_HEIGHT-1)<<ST_FP;
    
    o->y0 = -h;
    o->y1 = 0;
  }
  else
  {
    o->y = (0)<<ST_FP;
    o->y0 = 0;
    o->y1 = h;
  }
}

void st_NewPlayerMissle(uint8_t x, uint8_t y)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  o->ot = ST_OT_MISSLE;
  st_SetXY(o, x, y);
  //o->x = x<<ST_FP;
  //o->y = y<<ST_FP;
  o->x0 = -4;
  o->x1 = 1;
  o->y0 = 0;
  o->y1 = 0;
}

void st_SetupPlayer(uint8_t objnr, uint8_t ot)
{
  st_obj *o = st_GetObj(objnr);
  switch(ot)
  {
    case ST_OT_PLAYER:
      o->ot = ot;
      o->y0 = -2;
      o->y1 = 2;
      break;
    case ST_OT_PLAYER2:
      o->ot = ot;
      o->y0 = -2;
      o->y1 = 5;
      break;
    case ST_OT_PLAYER3:
      o->ot = ot;
      o->y0 = -5;
      o->y1 = 5;
      break;
  }
}

void st_NewPlayer(void)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  o->x = 6<<ST_FP;
  o->y = (ST_AREA_HEIGHT/2)<<ST_FP;
  o->x0 = -6;
  o->x1 = 0;
  st_SetupPlayer(objnr, ST_OT_PLAYER);
}

/*================================================================*/
/* trash creation */
/*================================================================*/

void st_InitDeltaWall(void)
{
  uint8_t i;
  uint8_t cnt = 0;
  uint8_t max_x = 0;
  uint8_t max_l;
  
  uint8_t min_dist_for_new = 40;
  uint8_t my_difficulty = st_difficulty;
  
  if ( st_difficulty >= 2 )
  {
    
    max_l = ST_AREA_WIDTH;
    max_l -= min_dist_for_new;
    
    if ( my_difficulty > 30 )
      my_difficulty = 30;
    min_dist_for_new -= my_difficulty;
    
    for( i = 0; i < ST_OBJ_CNT; i++ )
    {
      if ( st_objects[i].ot == ST_OT_WALL_SOLID )
      {
	cnt++;
	if ( max_x < (st_objects[i].x>>ST_FP) )
	  max_x = (st_objects[i].x>>ST_FP);
      }
    }    
    /* if ( cnt < upper_trash_limit ) */
    if ( max_x < max_l ) 
    {
      st_NewWall();
    }
  }
}


void st_InitDeltaTrash(void)
{
  uint8_t i;
  uint8_t cnt = 0;
  uint8_t max_x = 0;
  uint8_t max_l;
  
  uint8_t upper_trash_limit = ST_OBJ_CNT-7;
  uint8_t min_dist_for_new = 20;
  uint8_t my_difficulty = st_difficulty;
  
  if ( my_difficulty > 14 )
    my_difficulty = 14;
  min_dist_for_new -= my_difficulty;
  
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    if ( st_objects[i].ot == ST_OT_TRASH1 || st_objects[i].ot == ST_OT_TRASH2 || st_objects[i].ot == ST_OT_GADGET  || st_objects[i].ot == ST_OT_BIG_TRASH )
    {
      cnt++;
      if ( max_x < (st_objects[i].x>>ST_FP) )
	max_x = (st_objects[i].x>>ST_FP);
    }
  }
  
  max_l = ST_AREA_WIDTH;
  max_l -= min_dist_for_new;
  
  if ( cnt < upper_trash_limit )
    if ( max_x < max_l ) 
    {
      if (  (st_difficulty >= 3)  && ((st_rnd() & 7) == 0) )
	st_NewGadget(ST_AREA_WIDTH-1, rand() & (ST_AREA_HEIGHT-1));
      else
	st_InitTrash(ST_AREA_WIDTH-1, rand() & (ST_AREA_HEIGHT-1),0 );
    }
}

void st_InitDelta(void)
{
  st_InitDeltaTrash();
  st_InitDeltaWall();
  /*

  uint8_t cnt;
  cnt = st_CntObj(2);
  if ( cnt == 0 )
    st_InitBrick1();
  */
}

/*================================================================*/
/* API: game draw procedure */
/*================================================================*/

void st_DrawInGame(uint8_t fps)
{
  uint8_t i;
  /* draw all objects */
  for( i = 0; i < ST_OBJ_CNT; i++ )
      st_DrawObj(i);
  
  dog_ClrBox(0, ST_AREA_HEIGHT, DOG_WIDTH-1, ST_AREA_HEIGHT+3);
  dog_SetHLine(0, ST_AREA_WIDTH-1, ST_AREA_HEIGHT-1);
  dog_SetHLine(0, ST_AREA_WIDTH-1, 0);
  dog_DrawStr(0, ST_AREA_HEIGHT, font_4x6, dog_itoa(st_difficulty));
  
  dog_SetHLine(10, 10+(st_to_diff_cnt>>ST_DIFF_FP), ST_AREA_HEIGHT+3);
  dog_SetVLine(10,ST_AREA_HEIGHT+2, ST_AREA_HEIGHT+4);
  dog_SetVLine(10+ST_DIFF_VIS_LEN,ST_AREA_HEIGHT+2, ST_AREA_HEIGHT+4);

  /* player points */
  dog_DrawStr(ST_AREA_WIDTH-5*4-2,  ST_AREA_HEIGHT, font_4x6, dog_itoa(st_player_points_delayed));
  
  /* FPS output */
  if ( fps > 0 )
  {
    i = dog_DrawStr(ST_AREA_WIDTH-5*4-2-7*4, ST_AREA_HEIGHT, font_4x6, "FPS:");
    dog_DrawStr(ST_AREA_WIDTH-5*4-2-7*4+i, ST_AREA_HEIGHT, font_4x6, dog_itoa(fps));
  }
  /*dog_DrawStr(60+i, ST_AREA_HEIGHT, font_4x6, dog_itoa(st_CntObj(0)));*/
}

void st_Draw(uint8_t fps)
{
  switch(st_state)
  {
    case ST_STATE_PREPARE:
    case ST_STATE_IPREPARE:
      //dog_DrawStr(0, (DOG_HEIGHT-6)/2, font_4x6, "SpaceTrash");
      dog_DrawStrP(0, (DOG_HEIGHT-6)/2, font_4x6, DOG_PSTR("SpaceTrash"));
      dog_SetHLine(DOG_WIDTH-st_to_diff_cnt-10, DOG_WIDTH-st_to_diff_cnt, (DOG_HEIGHT-6)/2-1);
      break;
    case ST_STATE_GAME:
      st_DrawInGame(fps);
      break;
    case ST_STATE_END:
    case ST_STATE_IEND:
      //dog_DrawStr(0, (DOG_HEIGHT-6)/2, font_4x6, "Game Over");
      dog_DrawStrP(0, (DOG_HEIGHT-6)/2, font_4x6, DOG_PSTR("Game Over"));
      dog_DrawStr(50, (DOG_HEIGHT-6)/2, font_4x6, dog_itoa(st_player_points));
      dog_DrawStr(75, (DOG_HEIGHT-6)/2, font_4x6, dog_itoa(st_highscore));
      dog_SetHLine(st_to_diff_cnt, st_to_diff_cnt+10, (DOG_HEIGHT-6)/2-1);
      break;
  }
}

void st_SetupInGame(void)
{
  st_player_points = 0;
  st_player_points_delayed = 0;
  st_difficulty = 1;
  st_to_diff_cnt = 0;
  st_ClrObjs();
  st_NewPlayer();
  /* st_InitBrick1(); */
}


/*================================================================*/
/* API: game setup */
/*================================================================*/

void st_Setup(void)
{
}

/*================================================================*/
/* API: game step execution */
/*================================================================*/

/*
  player_pos: 0..255
*/
void st_StepInGame(uint8_t player_pos, uint8_t is_auto_fire, uint8_t is_fire)
{
  uint8_t i, j;
  uint8_t missle_mask;
  
  /* rescale player pos */
  //st_player_pos = ((uint16_t)player_pos * (uint16_t)ST_AREA_HEIGHT)/256;
  if ( player_pos < 64 )
    st_player_pos = 0;
  else if ( player_pos >= 192 )
    st_player_pos = ST_AREA_HEIGHT-2-1;
  else 
    st_player_pos = ((uint16_t)((player_pos-64)) * (uint16_t)(ST_AREA_HEIGHT-2))/128;
  st_player_pos+=1;
  /* move all objects */
  for( i = 0; i < ST_OBJ_CNT; i++ )
    st_Move(i);
    
  /* check for objects which left the play area */
  for( i = 0; i < ST_OBJ_CNT; i++ )
    if ( st_objects[i].ot != 0 )
      if ( st_IsOut(i) != 0 )
	st_Disappear(i);
      
  /* missle and destruction handling */
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    missle_mask = st_GetMissleMask(i);
    if ( missle_mask != 0 )						/* should we apply missle handling? */
      if ( st_CalcXY(st_objects+i) != 0 )						/* yes: calculate pixel reference point (st_px_x, st_px_y) */
	for( j = 0; j < ST_OBJ_CNT; j++ )			/* has any other object been hit? */
	  if ( i != j )							/* except missle itself... */
	    if ( st_IsHit(j, st_px_x, st_px_y, missle_mask) != 0 )		/* let the member function decide */
	    {									/* let the member function destroy the object if required */
	      st_Destroy(i);
	    }
  }
  
  /* handle fire counter */
  st_FireStep(is_auto_fire, is_fire);
  
  /* fire */
  for( i = 0; i < ST_OBJ_CNT; i++ )
    st_Fire(i);
	  
  /* create new objects */
  st_InitDelta();
	
  /* increase difficulty */
	    
  st_to_diff_cnt++;
  if ( st_to_diff_cnt == (ST_DIFF_VIS_LEN<<ST_DIFF_FP) )
  {
    st_to_diff_cnt = 0;
    st_difficulty++;
    st_player_points += ST_POINTS_PER_LEVEL;
  }  
  
  /* update visible player points */
  if ( st_player_points_delayed < st_player_points )
    st_player_points_delayed++;
}
  
void st_Step(uint8_t player_pos, uint8_t is_auto_fire, uint8_t is_fire)
{
  switch(st_state)
  {
    case ST_STATE_PREPARE:
      st_to_diff_cnt = DOG_WIDTH-10;		/* reuse st_to_diff_cnt */
      st_state = ST_STATE_IPREPARE;
      break;
    case ST_STATE_IPREPARE:
      st_to_diff_cnt--;
      if ( st_to_diff_cnt == 0 )
      {
	st_state = ST_STATE_GAME;
	st_SetupInGame();
      }
      break;
    case ST_STATE_GAME:
      st_StepInGame(player_pos, is_auto_fire, is_fire);
      break;
    case ST_STATE_END:
      st_to_diff_cnt = DOG_WIDTH-10;		/* reuse st_to_diff_cnt */
      if ( st_highscore < st_player_points)
	st_highscore = st_player_points;
      st_state = ST_STATE_IEND;
      break;
    case ST_STATE_IEND:
      st_to_diff_cnt--;
      if ( st_to_diff_cnt == 0 )
	st_state = ST_STATE_PREPARE;
      break;
  }
}

