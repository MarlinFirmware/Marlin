/*
  chessengine.c
  
  "Little Rook Chess" (lrc)

  Port to u8g library

  chess for embedded 8-Bit controllers

  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

  Note:
    UNIX_MAIN --> unix console executable

  Current Rule Limitation
    - no minor promotion, only "Queening" of the pawn
    - threefold repetition is not detected (same board situation appears three times)
	Note: Could be implemented, but requires tracking of the complete game
    - Fifty-move rule is not checked (no pawn move, no capture within last 50 moves)
	
  Words
    Ply		a half move
    
  General Links
    http://chessprogramming.wikispaces.com/

  Arduino specific
    http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1260055596
    
  Prefixes  
    chess_		Generic Chess Application Interface
    ce_		Chess engine, used internally, these function should not be called directly
    cu_		Chess utility function
    stack_		Internal function for stack handling

  Issues
    10.01.2011
      - castling to the right does not move the rook
	  --> done
      - castling to the left: King can only move two squares
	  --> done
      
    11.01.2011	
      Next Steps:
	- replace stack_NextCurrentPos with cu_NextPos, cleanup code according to the loop variable
	    --> done
	- Castling: Need to check for fields under attack
	    --> done
	
	- Check for WIN / LOSE situation, perhaps call ce_Eval() once on the top-level board setup
	    just after the real move
	- cleanup cu_Move
	    --> almost done
	- add some heuristics to the eval procedure
	- add right side menu
	  --> done
	- clean up chess_ManualMove
	  --> done
	- finish menu (consider is_game_end, undo move)
        - end condition: if KING is under attack and if KING can not move to a field which is under attack...
	      then the game is lost. What will be returned by the Eval procedure? is it -INF?
	    --> finished
	    
	- reduce the use of variable color, all should be reduced to board_orientation and ply&1
	
	- chess_GetNextMarked shoud make use of cu_NextPos
	    --> done
	- chess_ManualMove: again cleanup, solve draw issue (KING is not in check and no legal moves are available)
	    --> done
    22.01.2011
	- simplify eval_t ce_Eval(void)
	- position eval does not work, still moves side pawn :-(
	      maybe because all pieces are considered
	    --> done

*/

#include "u8g.h"

//#ifndef __unix__
//#else
//#include <assert.h>
//#define U8G_NOINLINE
//#endif

/*
SAN identifies each piece by a single upper case letter.  The standard English
values: pawn = "P", knight = "N", bishop = "B", rook = "R", queen = "Q", and
king = "K".
*/

/* numbers for the various pieces */
#define PIECE_NONE	0
#define PIECE_PAWN 	1
#define PIECE_KNIGHT  	2
#define PIECE_BISHOP 	3
#define PIECE_ROOK	4
#define PIECE_QUEEN 	5
#define PIECE_KING		6

/* color definitions */
#define COLOR_WHITE	0
#define COLOR_BLACK	1

/* a mask, which includes COLOR and PIECE number */
#define COLOR_PIECE_MASK 0x01f

#define CP_MARK_MASK 0x20

#define ILLEGAL_POSITION 255

/* This is the build in upper limit of the search stack */
/* This value defines the amount of memory allocated for the search stack */
/* The search depth of this chess engine can never exceed this value */
#define STACK_MAX_SIZE 5

/* chess half move stack: twice the number of undo's, a user can do */ 
#define CHM_USER_SIZE 6

/* the CHM_LIST_SIZE must be larger than the maximum search depth */
/* the overall size of ste half move stack */
#define CHM_LIST_SIZE (STACK_MAX_SIZE+CHM_USER_SIZE+2)

typedef int16_t eval_t;	/* a variable type to store results from the evaluation */ 
//#define EVAL_T_LOST -32768
#define EVAL_T_MIN -32767
#define EVAL_T_MAX 32767
//#define EVAL_T_WIN 32767

/* for maintainance of our own stack: this is the definition of one element on the stack */
struct _stack_element_struct
{
  /* the current source position which is investigated */
  uint8_t current_pos;
  uint8_t current_cp;
  uint8_t current_color;	/* COLOR_WHITE or COLOR_BLACK: must be predefines */
  
  /* the move which belongs to that value, both values are game positions */
  uint8_t best_from_pos;
  uint8_t best_to_pos;
  /* the best value, which has been dicovered so far */
  eval_t best_eval;
};
typedef struct _stack_element_struct stack_element_t;
typedef struct _stack_element_struct *stack_element_p;

/* chess half move history */
struct _chm_struct
{
  uint8_t main_cp;		/* the main piece, which is moved */
  uint8_t main_src;		/* the source position of the main piece */
  uint8_t main_dest; 	/* the destination of the main piece */
  
  uint8_t other_cp;		/* another piece: the captured one, the ROOK in case of castling or PIECE_NONE */
  uint8_t other_src;		/* the delete position of other_cp. Often identical to main_dest except for e.p. and castling */
  uint8_t other_dest;		/* only used for castling: ROOK destination pos */
  
  /* the position of the last pawn, which did a double move forward */
  /* this is required to check en passant conditions */
  /* this array can be indexed by the color of the current player */
  /* this is the condition BEFORE the move was done */
  uint8_t pawn_dbl_move[2];
  
  /* flags for the movement of rook and king; required for castling */
  /* a 1 means: castling is (still) possible */
  /* a 0 means: castling not possible */
  /*  bit 0 left side white */
  /*  bit 1 right side white */
  /*  bit 2 left side black */
  /*  bit 3 right side black */
  /* this is the condition BEFORE the move was done */
  uint8_t castling_possible;   
};

typedef struct _chm_struct chm_t;
typedef struct _chm_struct *chm_p;

/* little rook chess, main structure */
struct _lrc_struct
{  
  /* half-move (ply) counter: Counts the number of half-moves so far. Starts with 0 */
  /* the lowest bit is used to derive the color of the current player */
  /* will be set to zero in chess_SetupBoard() */
  uint8_t ply_count;
  
  /* the half move stack position counter, counts the number of elements in chm_list */
  uint8_t chm_pos;
  
  /* each element contains a colored piece, empty fields have value 0 */
  /* the field with index 0 is black (lower left) */
  uint8_t board[64];	
  /* the position of the last pawn, which did a double move forward */
  /* this is required to check en passant conditions */
  /* this array can be indexed by the color of the current player */
  uint8_t pawn_dbl_move[2]; 
  
  /* flags for the movement of rook and king; required for castling */
  /* a 1 means: castling is (still) possible */
  /* a 0 means: castling not possible */
  /*  bit 0 left side white */
  /*  bit 1 right side white */
  /*  bit 2 left side black */
  /*  bit 3 right side black */
  uint8_t castling_possible; 
  
  /* board orientation */
  /* 0: white is below COLOR_WHITE */
  /* 1: black is below COLOR_BLACK */
  /* bascially, this can be used as a color */
  uint8_t orientation;
  
  /* exchange colors of the pieces */
  /* 0: white has an empty body, use this for bright background color */
  /* 1: black has an empty body, use this for dark backround color */
  uint8_t strike_out_color;
  
  /* 0, when the game is ongoing */
  /* 1, when the game is stopped (lost or draw) */
  uint8_t is_game_end;
  /* the color of the side which lost the game */
  /* this value is only valid, when is_game_end is not 0 */
  /* values 0 and 1 represent WHITE and BLACK, 2 means a draw */
  uint8_t lost_side_color;
  
  
  
  /* checks are executed in ce_LoopRecur */
  /* these checks will put some marks on the board */
  /* this will be used by the interface to find out */
  /* legal moves */
  uint8_t check_src_pos;
  uint8_t check_mode;		/* CHECK_MODE_NONE, CHECK_MODE_MOVEABLE, CHECK_MODE_TARGET_MOVE */
  
  
  /* count of the attacking pieces, indexed by color */
  uint8_t find_piece_cnt[2];

  /* sum of the attacking pieces, indexed by color */
  uint8_t find_piece_weight[2];

  /* points to the current element of the search stack */
  /* this stack is NEVER empty. The value 0 points to the first element of the stack */
  /* actually "curr_depth" represent half-moves (plies) */
  uint8_t curr_depth;
  uint8_t max_depth;
  stack_element_p curr_element;
  
  /* allocated memory for the search stack */
  stack_element_t stack_memory[STACK_MAX_SIZE];

  /* the half move stack, used for move undo and depth search, size is stored in chm_pos */
  chm_t chm_list[CHM_LIST_SIZE];
};
typedef struct _lrc_struct lrc_t;

#define CHECK_MODE_NONE 0
#define CHECK_MODE_MOVEABLE 1
#define CHECK_MODE_TARGET_MOVE 2



/*==============================================================*/
/* global variables */
/*==============================================================*/

u8g_t *lrc_u8g;

lrc_t lrc_obj;


/*==============================================================*/
/* forward declarations */
/*==============================================================*/

/* 
  apply no inline to some of the functions:
  avr-gcc very often inlines functions, however not inline saves a lot of program memory!
  On the other hand there are some really short procedures which should be inlined (like cp_GetColor)
  These procedures are marked static to prevent the generation of the expanded procedure, which
  also saves space.
*/

uint8_t stack_Push(uint8_t color) U8G_NOINLINE;
void stack_Pop(void) U8G_NOINLINE;
void stack_InitCurrElement(void) U8G_NOINLINE;
void stack_Init(uint8_t max) U8G_NOINLINE;
void stack_SetMove(eval_t val, uint8_t to_pos) U8G_NOINLINE;
uint8_t cu_NextPos(uint8_t pos) U8G_NOINLINE;
static uint8_t cu_gpos2bpos(uint8_t gpos);
static uint8_t cp_Construct(uint8_t color, uint8_t piece);
static uint8_t cp_GetPiece(uint8_t cp);
static uint8_t cp_GetColor(uint8_t cp);
uint8_t cp_GetFromBoard(uint8_t pos) U8G_NOINLINE;
void cp_SetOnBoard(uint8_t pos, uint8_t cp) U8G_NOINLINE;

void cu_ClearBoard(void) U8G_NOINLINE;
void chess_SetupBoard(void) U8G_NOINLINE;
eval_t ce_Eval(void);

void cu_ClearMoveHistory(void) U8G_NOINLINE;
void cu_ReduceHistoryByFullMove(void) U8G_NOINLINE;
void cu_UndoHalfMove(void) U8G_NOINLINE;
chm_p cu_PushHalfMove(void) U8G_NOINLINE;


void ce_CalculatePositionWeight(uint8_t pos);
uint8_t ce_GetPositionAttackWeight(uint8_t pos, uint8_t color);

void chess_Thinking(void);
void ce_LoopPieces(void);


/*==============================================================*/
/* search stack */
/*==============================================================*/

/* get current element from stack */
stack_element_p stack_GetCurrElement(void)
{
  return lrc_obj.curr_element;
}

uint8_t stack_Push(uint8_t color)
{
  if ( lrc_obj.curr_depth == lrc_obj.max_depth )
    return 0;
  lrc_obj.curr_depth++;
  lrc_obj.curr_element = lrc_obj.stack_memory+lrc_obj.curr_depth;
  
  /* change view for the evaluation */
  color ^= 1;
  stack_GetCurrElement()->current_color = color;

  return 1;
}

void stack_Pop(void)
{
  lrc_obj.curr_depth--;
  lrc_obj.curr_element = lrc_obj.stack_memory+lrc_obj.curr_depth;
}

/* reset the current element on the stack */
void stack_InitCurrElement(void)
{
  stack_element_p e = stack_GetCurrElement();
  e->best_eval = EVAL_T_MIN;
  e->best_from_pos = ILLEGAL_POSITION;
  e->best_to_pos = ILLEGAL_POSITION;
}

/* resets the search stack (and the check mode) */
void stack_Init(uint8_t max)
{
  lrc_obj.curr_depth = 0;
  lrc_obj.curr_element = lrc_obj.stack_memory;
  lrc_obj.max_depth = max;
  lrc_obj.check_mode = CHECK_MODE_NONE;
  stack_InitCurrElement();
  stack_GetCurrElement()->current_color = lrc_obj.ply_count;
  stack_GetCurrElement()->current_color &= 1;
}

/* assign evaluation value and store the move, if this is the best move */
/* assumes, that current_pos contains the source position */
void stack_SetMove(eval_t val, uint8_t to_pos)
{
  stack_element_p e = stack_GetCurrElement();
  if ( e->best_eval < val )
  {
    e->best_eval = val;
    e->best_from_pos = e->current_pos;
    e->best_to_pos = to_pos;
  }
}

/* 
  calculate next position on a 0x88 board 
  loop is constructed in this way:
  i = 0;
  do
  {
    ...
    i = cu_NextPos(i);
  } while( i != 0 );

  next pos might be started with an illegal position like 255
*/
uint8_t cu_NextPos(uint8_t pos)
{
  /* calculate next gpos */
  pos++;
  if ( ( pos & 0x08 ) != 0 )
  {
    pos+= 0x10;
    pos&= 0xf0; 
  }
  if ( ( pos & 0x80 ) != 0 )
    pos = 0;
  return pos;
}

uint8_t cu_PrevPos(uint8_t pos)
{
  /* calculate prev gpos */
  pos--;
  if ( ( pos & 0x80 ) != 0 )
    pos = 0x077;
  else if ( ( pos & 0x08 ) != 0 )
  {
    pos &= 0xf0; 
    pos |= 0x07;
  }
  return pos;
}


/*==============================================================*/
/* position transltion */
/*==============================================================*/
/*
  there are two positions
    1. game position (gpos): BCD encoded x-y values
    2. board position (bpos): a number between 0 and 63, only used to access the board.
*/
/*
  gpos:	game position value
  returns:	board position
  note:	does not do any checks
*/
static uint8_t cu_gpos2bpos(uint8_t gpos)
{
  uint8_t bpos = gpos;
  bpos &= 0xf0;
  bpos >>= 1;
  gpos &= 0x0f;
  bpos |= gpos;
  return bpos;
}

#define gpos_IsIllegal(gpos) ((gpos) & 0x088)


/*==============================================================*/
/* colored piece handling */
/*==============================================================*/

#define cp_IsMarked(cp)  ((cp) & CP_MARK_MASK)


/*
  piece: one of PIECE_xxx
  color: COLOR_WHITE or COLOR_BLACK

  returns: A colored piece
*/
static uint8_t cp_Construct(uint8_t color, uint8_t piece)
{
  color <<= 4;
  color |= piece;
  return color;
}

/* inline is better than a macro */
static uint8_t cp_GetPiece(uint8_t cp)
{
  cp &= 0x0f;
  return cp;
}

/*
  we could use a macro:
  #define cp_GetColor(cp)	(((cp) >> 4)&1)
  however, inlined functions are sometimes much better
*/
static uint8_t cp_GetColor(uint8_t cp)
{
  cp >>= 4;
  cp &= 1;
  return cp;
}

/*
  pos: game position
  returns the colored piece at the given position
*/
uint8_t cp_GetFromBoard(uint8_t pos)
{
  return lrc_obj.board[cu_gpos2bpos(pos)];
}

/*
  pos: game position
  cp: colored piece
*/
void cp_SetOnBoard(uint8_t pos, uint8_t cp)
{
  /*printf("cp_SetOnBoard gpos:%02x cp:%02x\n", pos, cp);*/
  lrc_obj.board[cu_gpos2bpos(pos)] = cp;
}

/*==============================================================*/
/* global board access */
/*==============================================================*/

void cu_ClearBoard(void)
{
  uint8_t i;
  /* clear the board */
  for( i = 0; i < 64; i++ )
    lrc_obj.board[i] = PIECE_NONE;
  
  lrc_obj.ply_count = 0;
  lrc_obj.orientation = COLOR_WHITE;
  
  lrc_obj.pawn_dbl_move[0] = ILLEGAL_POSITION;
  lrc_obj.pawn_dbl_move[1] = ILLEGAL_POSITION;
  
  lrc_obj.castling_possible = 0x0f;
  
  lrc_obj.is_game_end = 0;
  lrc_obj.lost_side_color = 0;

  /* clear half move history */
  cu_ClearMoveHistory();

}

/*
  test setup
  white wins in one move
*/
void chess_SetupBoardTest01(void)
{
  cu_ClearBoard();
  lrc_obj.board[7+7*8] = cp_Construct(COLOR_BLACK, PIECE_KING);
  lrc_obj.board[7+5*8] = cp_Construct(COLOR_WHITE, PIECE_PAWN);
  lrc_obj.board[3] = cp_Construct(COLOR_WHITE, PIECE_KING);
  lrc_obj.board[0+7*8] = cp_Construct(COLOR_BLACK, PIECE_ROOK);
  lrc_obj.board[6] = cp_Construct(COLOR_WHITE, PIECE_QUEEN);
} 

/* setup the global board */
void chess_SetupBoard(void)
{
  uint8_t i;
  register uint8_t bp, wp;
  
  /* clear the board */
  cu_ClearBoard();
  
  /* precronstruct pawns */
  wp = cp_Construct(COLOR_WHITE, PIECE_PAWN);
  bp = cp_Construct(COLOR_BLACK, PIECE_PAWN);
  
  /* setup pawn */
  for( i = 0; i < 8; i++ )
  {
    lrc_obj.board[i+8] = wp;
    lrc_obj.board[i+6*8] = bp;
  }
  
  /* assign remaining pieces */
  
  lrc_obj.board[0] = cp_Construct(COLOR_WHITE, PIECE_ROOK);
  lrc_obj.board[1] = cp_Construct(COLOR_WHITE, PIECE_KNIGHT);
  lrc_obj.board[2] = cp_Construct(COLOR_WHITE, PIECE_BISHOP);
  lrc_obj.board[3] = cp_Construct(COLOR_WHITE, PIECE_QUEEN);
  lrc_obj.board[4] = cp_Construct(COLOR_WHITE, PIECE_KING);
  lrc_obj.board[5] = cp_Construct(COLOR_WHITE, PIECE_BISHOP);
  lrc_obj.board[6] = cp_Construct(COLOR_WHITE, PIECE_KNIGHT);
  lrc_obj.board[7] = cp_Construct(COLOR_WHITE, PIECE_ROOK);

  lrc_obj.board[0+7*8] = cp_Construct(COLOR_BLACK, PIECE_ROOK);
  lrc_obj.board[1+7*8] = cp_Construct(COLOR_BLACK, PIECE_KNIGHT);
  lrc_obj.board[2+7*8] = cp_Construct(COLOR_BLACK, PIECE_BISHOP);
  lrc_obj.board[3+7*8] = cp_Construct(COLOR_BLACK, PIECE_QUEEN);
  lrc_obj.board[4+7*8] = cp_Construct(COLOR_BLACK, PIECE_KING);
  lrc_obj.board[5+7*8] = cp_Construct(COLOR_BLACK, PIECE_BISHOP);
  lrc_obj.board[6+7*8] = cp_Construct(COLOR_BLACK, PIECE_KNIGHT);
  lrc_obj.board[7+7*8] = cp_Construct(COLOR_BLACK, PIECE_ROOK);

  //chess_SetupBoardTest01();

}



/*==============================================================*/
/* checks */
/*==============================================================*/

/*
  checks if the position is somehow illegal
*/
uint8_t cu_IsIllegalPosition(uint8_t pos, uint8_t my_color)
{
  uint8_t board_cp;
  /* check, if the position is offboard */
  if ( gpos_IsIllegal(pos) != 0 )
    return 1;
  /* get the piece from the board */
  board_cp = cp_GetFromBoard(pos);
  /* check if hit our own pieces */
  if ( board_cp != 0 ) 
    if ( cp_GetColor(board_cp) == my_color )
      return 1;
  /* all ok, we could go to this position */
  return 0;
}

/*==============================================================*/
/* evaluation procedure */
/*==============================================================*/

/*
  basic idea is to return a value between EVAL_T_MIN and EVAL_T_MAX
*/

/*
  the weight table uses the PIECE number as index:
      #define PIECE_NONE	0
      #define PIECE_PAWN 	1
      #define PIECE_KNIGHT  	2
      #define PIECE_BISHOP 	3
      #define PIECE_ROOK	4
      #define PIECE_QUEEN 	5
      #define PIECE_KING		6
  the king itself is not counted
*/
uint8_t ce_piece_weight[] = { 0, 1, 3, 3, 5, 9, 0 };
uint8_t ce_pos_weight[] = { 0, 1, 1, 2, 2, 1, 1, 0};
/*
  evaluate the current situation on the global board
*/
eval_t ce_Eval(void)
{
  uint8_t cp;
  uint8_t is_my_king_present = 0;
  uint8_t is_opposit_king_present = 0;
  eval_t material_my_color = 0;
  eval_t material_opposit_color = 0;
  eval_t position_my_color = 0;
  eval_t position_opposit_color = 0;
  eval_t result;
  uint8_t pos;
  
  pos = 0;
  do
  {
    /* get colored piece from the board */
    cp = cp_GetFromBoard(pos);
    
    if ( cp_GetPiece(cp) != PIECE_NONE )
    {
      if ( stack_GetCurrElement()->current_color == cp_GetColor(cp) )
      {
	/* this is our color */
	/* check if we found our king */
	if ( cp_GetPiece(cp) == PIECE_KING  )
	  is_my_king_present = 1;
	material_my_color += ce_piece_weight[cp_GetPiece(cp)];
	if ( cp_GetPiece(cp) == PIECE_PAWN || cp_GetPiece(cp) == PIECE_KNIGHT  )
	{
	  position_my_color += ce_pos_weight[pos&7]*ce_pos_weight[(pos>>4)&7];
	}
      }
      else
      {
	/* this is the opposit color */
	if ( cp_GetPiece(cp) == PIECE_KING  )
	  is_opposit_king_present = 1;
	material_opposit_color += ce_piece_weight[cp_GetPiece(cp)];
	if ( cp_GetPiece(cp) == PIECE_PAWN || cp_GetPiece(cp) == PIECE_KNIGHT )
	{
	  position_opposit_color += ce_pos_weight[pos&7]*ce_pos_weight[(pos>>4)&7];
	}
      }
    }
    pos = cu_NextPos(pos);
  } while( pos != 0 );

    
  /* decide if we lost or won the game */
  if ( is_my_king_present == 0 )
    return EVAL_T_MIN;	/*_LOST*/
  if ( is_opposit_king_present == 0 )
    return EVAL_T_MAX;	/*_WIN*/
  
  /* here is the evaluation function */
  
  result = material_my_color - material_opposit_color;
  result <<= 3;
  result += position_my_color - position_opposit_color;
  return result;
}

/*==============================================================*/
/* move backup and restore */
/*==============================================================*/


/* this procedure must be called to keep the size as low as possible */
/* if the chm_list is large enough, it could hold the complete history */
/* but for an embedded controler... it is deleted for every engine search */
void cu_ClearMoveHistory(void)
{
  lrc_obj.chm_pos = 0;
}

void cu_ReduceHistoryByFullMove(void)
{
  uint8_t i;
  while( lrc_obj.chm_pos > CHM_USER_SIZE )
  {
    i = 0;
    for(;;)
    {
      if ( i+2 >= lrc_obj.chm_pos )
	break;
      lrc_obj.chm_list[i] = lrc_obj.chm_list[i+2];
      i++;
    }
    lrc_obj.chm_pos -= 2;
  }
}

void cu_UndoHalfMove(void)
{
  chm_p chm;
  
  if ( lrc_obj.chm_pos == 0 )
    return;
  
  lrc_obj.chm_pos--;

  chm = lrc_obj.chm_list+lrc_obj.chm_pos;
  
  lrc_obj.pawn_dbl_move[0] = chm->pawn_dbl_move[0];
  lrc_obj.pawn_dbl_move[1] = chm->pawn_dbl_move[1];
  lrc_obj.castling_possible = chm->castling_possible;
  
  cp_SetOnBoard(chm->main_src, chm->main_cp);
  cp_SetOnBoard(chm->main_dest, PIECE_NONE);
  
  if ( chm->other_src != ILLEGAL_POSITION )
    cp_SetOnBoard(chm->other_src, chm->other_cp);
  if ( chm->other_dest != ILLEGAL_POSITION )
    cp_SetOnBoard(chm->other_dest, PIECE_NONE);

}

/*
  assumes, that the following members of the returned chm structure are filled 
  uint8_t main_cp;		the main piece, which is moved
  uint8_t main_src;		the source position of the main piece
  uint8_t main_dest; 	the destination of the main piece
  
  uint8_t other_cp;		another piece: the captured one, the ROOK in case of castling or PIECE_NONE
  uint8_t other_src;		the delete position of other_cp. Often identical to main_dest except for e.p. and castling
  uint8_t other_dest;		only used for castling: ROOK destination pos

*/
chm_p cu_PushHalfMove(void)
{
  chm_p chm;
  
  chm = lrc_obj.chm_list+lrc_obj.chm_pos;
  if ( lrc_obj.chm_pos < CHM_LIST_SIZE-1)
    lrc_obj.chm_pos++;

  chm->pawn_dbl_move[0] = lrc_obj.pawn_dbl_move[0];
  chm->pawn_dbl_move[1] = lrc_obj.pawn_dbl_move[1];
  chm->castling_possible = lrc_obj.castling_possible;
  return chm;
}


char chess_piece_to_char[] = "NBRQK";

/*
  simple moves on empty field: 	Ka1-b2
  capture moves:				Ka1xb2
  castling:						0-0 or 0-0-0
*/

static void cu_add_pos(char *s, uint8_t pos) U8G_NOINLINE;

static void cu_add_pos(char *s, uint8_t pos)
{
  *s = pos;
  *s >>= 4;
  *s += 'a';
  s++;
  *s = pos;
  *s &= 15;
  *s += '1';
}

const char *cu_GetHalfMoveStr(uint8_t idx)
{
  chm_p chm;
  static char buf[7];		/*Ka1-b2*/
  char *p = buf;
  chm = lrc_obj.chm_list+idx;
  
  if ( cp_GetPiece(chm->main_cp) != PIECE_NONE )
  {
    if ( cp_GetPiece(chm->main_cp) > PIECE_PAWN )
    {
      *p++ = chess_piece_to_char[cp_GetPiece(chm->main_cp)-2];
    }
    cu_add_pos(p, chm->main_src);
    p+=2;
    if ( cp_GetPiece(chm->other_cp) == PIECE_NONE )
      *p++ = '-';
    else
      *p++ = 'x';
    cu_add_pos(p, chm->main_dest);
    p+=2;
  }
  *p = '\0';
  return buf;
}





/*==============================================================*/
/* move */
/*==============================================================*/

/*
  Move a piece from source position to a destination on the board
  This function
    - does not perform any checking
    - however it processes "en passant" and casteling
    - backup the move and allow 1x undo
  
  2011-02-05: 
    - fill pawn_dbl_move[] for double pawn moves
	--> done
    - Implement casteling 
	--> done
    - en passant
	--> done
    - pawn conversion/promotion
	--> done
    - half-move backup 
	--> done
    - cleanup everything, minimize variables
	--> done
*/

void cu_Move(uint8_t src, uint8_t dest)
{  
  /* start backup structure */
  chm_p chm = cu_PushHalfMove();

  /* these are the values from the board at the positions, provided as arguments to this function */
  uint8_t cp_src, cp_dest;
  
  /* Maybe a second position is cleared and one additional location is set */
  uint8_t clr_pos2;
  uint8_t set_pos2;
  uint8_t set_cp2;
  
  /* get values from board */
  cp_src = cp_GetFromBoard(src);
  cp_dest = cp_GetFromBoard(dest);

  /* fill backup structure */
  
  chm->main_cp = cp_src;
  chm->main_src = src;
  chm->main_dest = dest;
  
  chm->other_cp = cp_dest;		/* prepace capture backup */
  chm->other_src = dest;
  chm->other_dest = ILLEGAL_POSITION;
  
  /* setup results as far as possible with some suitable values */
  
  clr_pos2 = ILLEGAL_POSITION;	/* for en passant and castling, two positions might be cleared */
  set_pos2 = ILLEGAL_POSITION;	/* only used for castling */
  set_cp2 = PIECE_NONE;			/* ROOK for castling */
  
  /* check for PAWN */
  if ( cp_GetPiece(cp_src) == PIECE_PAWN )
  {
    
    /* double step: is the distance 2 rows */
    if ( (src - dest == 32) || ( dest - src == 32 ) )
    {
      /* remember the destination position */
      lrc_obj.pawn_dbl_move[cp_GetColor(cp_src)] = dest;
    }
    
    /* check if the PAWN is able to promote */
    else if ( (dest>>4) == 0 || (dest>>4) == 7 )
    {
      /* do simple "queening" */
      cp_src &= ~PIECE_PAWN;
      cp_src |= PIECE_QUEEN;
    }
    
    /* is it en passant capture? */
    /* check for side move */
    else if ( ((src + dest) & 1) != 0 )
    {
      /* check, if target field is empty */
      if (  cp_GetPiece(cp_dest) == PIECE_NONE )
      {
	/* this is en passant */
	/* no further checking required, because legal moves are assumed here */
	/* however... the captured pawn position must be valid */
	clr_pos2 = lrc_obj.pawn_dbl_move[cp_GetColor(cp_src) ^ 1];
	chm->other_src = clr_pos2;
	chm->other_cp = cp_GetFromBoard(clr_pos2);
      }
    }    
  }
  
  /* check for the KING */
  else if ( cp_GetPiece(cp_src) == PIECE_KING )
  {
    /* disallow castling, if the KING has moved */
    if ( cp_GetColor(cp_src) == COLOR_WHITE )
    {
      /* if white KING has moved, disallow castling for white */
      lrc_obj.castling_possible &= 0x0c;
    }
    else
    {
      /* if black KING has moved, disallow castling for black */
      lrc_obj.castling_possible &= 0x03;
    }
    
    /* has it been castling to the left? */
    if ( src - dest == 2 )
    {
      /* let the ROOK move to pos2 */
      set_pos2 = src-1;
      set_cp2 = cp_GetFromBoard(src-4);
      
      /* the ROOK must be cleared from the original position */
      clr_pos2 = src-4;
      
      chm->other_cp = set_cp2;
      chm->other_src = clr_pos2;
      chm->other_dest = set_pos2;
    }
    
    /* has it been castling to the right? */
    else if ( dest - src == 2 )
    {
      /* let the ROOK move to pos2 */
      set_pos2 = src+1;
      set_cp2 = cp_GetFromBoard(src+3);
      
      /* the ROOK must be cleared from the original position */
      clr_pos2 = src+3;
      
      chm->other_cp = set_cp2;
      chm->other_src = clr_pos2;
      chm->other_dest = set_pos2;
      
    }
    
  }
  
  /* check for the ROOK */
  else if ( cp_GetPiece(cp_src) == PIECE_ROOK )
  {
    /* disallow white left castling */
    if ( src == 0x00 )
      lrc_obj.castling_possible &= ~0x01;
    /* disallow white right castling */
    if ( src == 0x07 )
      lrc_obj.castling_possible &= ~0x02;
    /* disallow black left castling */
    if ( src == 0x70 )
      lrc_obj.castling_possible &= ~0x04;
    /* disallow black right castling */
    if ( src == 0x77 )
      lrc_obj.castling_possible &= ~0x08;
  }
  
  
  /* apply new board situation */
  
  cp_SetOnBoard(dest, cp_src);
  
  if ( set_pos2 != ILLEGAL_POSITION )
    cp_SetOnBoard(set_pos2, set_cp2);
  
  cp_SetOnBoard(src, PIECE_NONE);
  
  if ( clr_pos2 != ILLEGAL_POSITION )
    cp_SetOnBoard(clr_pos2, PIECE_NONE);
  
  
}

/*
  this subprocedure decides for evaluation of the current board situation or further (deeper) investigation
  Argument pos is the new target position if the current piece 

*/
uint8_t ce_LoopRecur(uint8_t pos)
{
  eval_t eval;
  
  /* 1. check if target position is occupied by the same player (my_color) */
  /*     of if pos is somehow illegal or not valid */
  if ( cu_IsIllegalPosition(pos, stack_GetCurrElement()->current_color) != 0 )
    return 0;

  /* 2. move piece to the specified position, capture opponent piece if required */
  cu_Move(stack_GetCurrElement()->current_pos, pos);

  
  /* 3. */
  /* if depth reached: evaluate */
  /* else: go down next level */
  /* no eval if there had been any valid half-moves, so the default value (MIN) will be returned. */
  if ( stack_Push(stack_GetCurrElement()->current_color) == 0 )
  {
    eval = ce_Eval();
  }
  else
  {
    /* init the element, which has been pushed */
    stack_InitCurrElement();
    /* start over with ntext level */
    ce_LoopPieces();
    /* get the best move from opponents view, so invert the result */
    eval = -stack_GetCurrElement()->best_eval;
    stack_Pop();
  }
  
  /* 4. store result */
  stack_SetMove(eval, pos);
  
  /* 5. undo the move */
  cu_UndoHalfMove();
  
  /* 6. check special modes */
  /* the purpose of these checks is to mark special pieces and positions on the board */
  /* these marks can be checked by the user interface to highlight special positions */
  if ( lrc_obj.check_mode != 0 )
  {
    stack_element_p e = stack_GetCurrElement();
    if ( lrc_obj.check_mode == CHECK_MODE_MOVEABLE )
    {
      cp_SetOnBoard(e->current_pos, e->current_cp | CP_MARK_MASK );
    }
    else if ( lrc_obj.check_mode == CHECK_MODE_TARGET_MOVE )
    {
      if ( e->current_pos == lrc_obj.check_src_pos )
      {
	cp_SetOnBoard(pos, cp_GetFromBoard(pos)  | CP_MARK_MASK );
      }
    }
  }
  return 1;
}

/*==============================================================*/
/* move pieces which can move one or more steps into a direction */
/*==============================================================*/

/*
  subprocedure to generate various target positions for some pieces
  special cases are handled in the piece specific sub-procedure

  Arguments:
    d: a list of potential directions
    is_multi_step: if the piece can only do one step (zero for KING and KNIGHT)
*/
static const uint8_t ce_dir_offset_rook[] PROGMEM = { 1, 16, -16, -1, 0 };
static const uint8_t ce_dir_offset_bishop[] PROGMEM = { 15, 17, -17, -15, 0 };
static const uint8_t ce_dir_offset_queen[] PROGMEM = { 1, 16, -16, -1, 15, 17, -17, -15, 0 };
static const uint8_t ce_dir_offset_knight[] PROGMEM = {14, -14, 18, -18, 31, -31, 33, -33, 0};

void ce_LoopDirsSingleMultiStep(const uint8_t *d, uint8_t is_multi_step)
{
  uint8_t loop_pos;
  
  /* with all directions */
  for(;;)
  {
    if ( u8g_pgm_read(d) == 0 )
      break;
    
    /* start again from the initial position */
    loop_pos = stack_GetCurrElement()->current_pos;
    
    /* check direction */
    do
    {
      /* check next position into one direction */
      loop_pos += u8g_pgm_read(d);
      
      /*
	go further to ce_LoopRecur()
	0 will be returned if the target position is illegal or a piece of the own color
	this is used to stop walking into one direction
      */
      if ( ce_LoopRecur(loop_pos) == 0 )
	break;
      
      /* stop if we had hit another piece */
      if ( cp_GetPiece(cp_GetFromBoard(loop_pos)) != PIECE_NONE )
	break;
    } while( is_multi_step );
    d++;
  }
}

void ce_LoopRook(void)
{
  ce_LoopDirsSingleMultiStep(ce_dir_offset_rook, 1);
}

void ce_LoopBishop(void)
{
  ce_LoopDirsSingleMultiStep(ce_dir_offset_bishop, 1);
}

void ce_LoopQueen(void)
{
  ce_LoopDirsSingleMultiStep(ce_dir_offset_queen, 1);
}

void ce_LoopKnight(void)
{
  ce_LoopDirsSingleMultiStep(ce_dir_offset_knight, 0);
}



/*==============================================================*/
/* move king */
/*==============================================================*/

uint8_t cu_IsKingCastling(uint8_t mask, int8_t direction, uint8_t cnt) U8G_NOINLINE;

/*
  checks, if the king can do castling

  Arguments:
    mask:		the bit-mask for the global "castling possible" flag
    direction:	left castling: -1, right castling 1
    cnt:		number of fields to be checked: 3 or 2
*/
uint8_t cu_IsKingCastling(uint8_t mask, int8_t direction, uint8_t cnt)
{
  uint8_t pos;
  uint8_t opponent_color;
  
  /* check if the current board state allows castling */
  if ( (lrc_obj.castling_possible & mask) == 0 )
    return 0; 	/* castling not allowed */
  
  /* get the position of the KING, could be white or black king */
  pos = stack_GetCurrElement()->current_pos;
  
  /* calculate the color of the opponent */
  opponent_color = 1;
  opponent_color -= stack_GetCurrElement()->current_color;
  
  /* if the KING itself is given check... */
  if ( ce_GetPositionAttackWeight(pos, opponent_color) > 0 )
    return 0;

  
  /* check if fields in the desired direction are emtpy */
  for(;;)
  {
    /* go to the next field */
    pos += direction;
    /* check for a piece */
    if ( cp_GetPiece(cp_GetFromBoard(pos)) != PIECE_NONE )
      return 0;		/* castling not allowed */

    /* if some of the fields are under attack */
    if ( ce_GetPositionAttackWeight(pos, opponent_color) > 0 )
      return 0;
    
    cnt--;
    if ( cnt == 0 )
      break;
  }
  return 1; /* castling allowed */
}

void ce_LoopKing(void)
{
  /*
    there is an interessting timing problem in this procedure
    it must be checked for castling first and as second step the normal
    KING movement. If we would first check for normal moves, than
    any marks might be overwritten by the ROOK in the case of castling.
  */
  
  /* castling (this must be done before checking normal moves (see above) */
  if ( stack_GetCurrElement()->current_color == COLOR_WHITE )
  {
    /* white left castling */
    if ( cu_IsKingCastling(1, -1, 3) != 0 )
    {
      /* check for attacked fields */
      ce_LoopRecur(stack_GetCurrElement()->current_pos-2);
    }
    /* white right castling */
    if ( cu_IsKingCastling(2, 1, 2) != 0 )
    {
      /* check for attacked fields */
      ce_LoopRecur(stack_GetCurrElement()->current_pos+2);
    }
  }
  else
  {
    /* black left castling */
    if ( cu_IsKingCastling(4, -1, 3) != 0 )
    {
      /* check for attacked fields */
      ce_LoopRecur(stack_GetCurrElement()->current_pos-2);
    }
    /* black right castling */
    if ( cu_IsKingCastling(8, 1, 2) != 0 )
    {
      /* check for attacked fields */
      ce_LoopRecur(stack_GetCurrElement()->current_pos+2);
    }
  }
  
  /* reuse queen directions */
  ce_LoopDirsSingleMultiStep(ce_dir_offset_queen, 0);
}


/*==============================================================*/
/* move pawn */
/*==============================================================*/

/*
  doppelschritt: nur von der grundlinie aus, beide (!) felder vor dem bauern müssen frei sein
  en passant: nur unmittelbar nachdem ein doppelschritt ausgeführt wurde.
*/
void ce_LoopPawnSideCapture(uint8_t loop_pos)
{
  if ( gpos_IsIllegal(loop_pos) == 0 )
  {
    /* get the piece from the board */
    /* if the field is NOT empty */
    if ( cp_GetPiece(cp_GetFromBoard(loop_pos)) != PIECE_NONE )
    {
      /* normal capture */
      ce_LoopRecur(loop_pos);
      /* TODO: check for pawn conversion/promotion */
    }
    else
    {
      /* check conditions for en passant capture */
      if ( stack_GetCurrElement()->current_color == COLOR_WHITE )
      {
	if ( lrc_obj.pawn_dbl_move[COLOR_BLACK]+16 == loop_pos )
	{
	  ce_LoopRecur(loop_pos);
	  /* note: pawn conversion/promotion can not occur */
	}
      }
      else
      {
	if ( lrc_obj.pawn_dbl_move[COLOR_WHITE] == loop_pos+16 )
	{
	  ce_LoopRecur(loop_pos);
	  /* note: pawn conversion/promotion can not occur */
	}
      }
    }
  }
}

void ce_LoopPawn(void)
{
  uint8_t initial_pos = stack_GetCurrElement()->current_pos; 
  uint8_t my_color = stack_GetCurrElement()->current_color;
  
  uint8_t loop_pos;
  uint8_t line;
  
  /* one step forward */
  
  loop_pos = initial_pos;
  line = initial_pos;
  line >>= 4;
  if ( my_color == COLOR_WHITE )
    loop_pos += 16;
  else
    loop_pos -= 16;
  if ( gpos_IsIllegal(loop_pos) == 0 )
  {
    /* if the field is empty */
    if ( cp_GetPiece(cp_GetFromBoard(loop_pos)) == PIECE_NONE )
    {
      /* TODO: check for and loop through piece conversion/promotion */
      ce_LoopRecur(loop_pos);      

      /* second step forward */
      
      /* if pawn is on his starting line */
      if ( (my_color == COLOR_WHITE && line == 1) || (my_color == COLOR_BLACK && line == 6 ) )
      {
	/* the place before the pawn is not occupied, so we can do double moves, see above */
	
	if ( my_color == COLOR_WHITE )
	  loop_pos += 16;
	else
	  loop_pos -= 16;
	if ( cp_GetPiece(cp_GetFromBoard(loop_pos)) == PIECE_NONE )
	{
	  /* this is a special case, other promotions of the pawn can not occur */
	  ce_LoopRecur(loop_pos);
	}
      }
    }
  }

  /* capture */
  
  loop_pos = initial_pos;
  if ( my_color == COLOR_WHITE )
    loop_pos += 15;
  else
    loop_pos -= 15;
  ce_LoopPawnSideCapture(loop_pos);


  loop_pos = initial_pos;
  if ( my_color == COLOR_WHITE )
    loop_pos += 17;
  else
    loop_pos -= 17;
  ce_LoopPawnSideCapture(loop_pos);
}

/*==============================================================*/
/* attacked */
/*==============================================================*/

/*
  from a starting position, search for a piece, that might jump to that postion.
  return:
    the two global variables
      lrc_obj.find_piece_weight[0];
      lrc_obj.find_piece_weight[1];
  will be increased by the weight of the attacked pieces of that color.
  it is usually required to reset these global variables to zero, before using
  this function.
*/

void ce_FindPieceByStep(uint8_t start_pos, uint8_t piece, const uint8_t *d, uint8_t is_multi_step)
{
  uint8_t loop_pos, cp;
  
  /* with all directions */
  for(;;)
  {
    if ( u8g_pgm_read(d) == 0 )
      break;
    
    /* start again from the initial position */
    loop_pos = start_pos;
    
    /* check direction */
    do
    {
      /* check next position into one direction */
      loop_pos += u8g_pgm_read(d);
      
      /* check if the board boundary has been crossed */
      if ( (loop_pos & 0x088) != 0 )
	break;
      
      /* get the colored piece from the board */
      cp = cp_GetFromBoard(loop_pos);
      
      /* stop if we had hit another piece */
      if ( cp_GetPiece(cp) != PIECE_NONE )
      {
	/* if it is the piece we are looking for, then add the weight */
	if ( cp_GetPiece(cp) == piece )
	{
	  lrc_obj.find_piece_weight[cp_GetColor(cp)] += ce_piece_weight[piece];
	  lrc_obj.find_piece_cnt[cp_GetColor(cp)]++;
	}
	/* in any case, break out of the inner loop */
	break;
      }
    } while( is_multi_step );
    d++;
  }
}

void ce_FindPawnPiece(uint8_t dest_pos, uint8_t color)
{
  uint8_t cp;
  /* check if the board boundary has been crossed */
  if ( (dest_pos & 0x088) == 0 )
  {
    /* get the colored piece from the board */
    cp = cp_GetFromBoard(dest_pos);
    /* only if there is a pawn of the matching color */
    if ( cp_GetPiece(cp) == PIECE_PAWN )
    {
      if ( cp_GetColor(cp) == color )
      {
	/* the weight of the PAWN */
	lrc_obj.find_piece_weight[color] += 1;
	lrc_obj.find_piece_cnt[color]++;
      }
    }
  }
}


/*
  find out, which pieces do attack a specified field
  used to
  - check if the KING can do castling
  - check if the KING must move

  may be used in the eval procedure ... once...

  the result is stored in the global array
    uint8_t lrc_obj.find_piece_weight[2];
  which is indexed with the color.
  lrc_obj.find_piece_weight[COLOR_WHITE] is the sum of all white pieces
  which can directly move to this field.

  example:
    if the black KING is at "pos" and lrc_obj.find_piece_weight[COLOR_WHITE] is not zero 
    (after executing ce_CalculatePositionWeight(pos)) then the KING must be protected or moveed, because 
    the KING was given check.
*/

void ce_CalculatePositionWeight(uint8_t pos)
{
  
  lrc_obj.find_piece_weight[0] = 0;
  lrc_obj.find_piece_weight[1] = 0;
  lrc_obj.find_piece_cnt[0] = 0;
  lrc_obj.find_piece_cnt[1] = 0;
  
  if ( (pos & 0x088) != 0 )
    return;

  ce_FindPieceByStep(pos, PIECE_ROOK, ce_dir_offset_rook, 1);
  ce_FindPieceByStep(pos, PIECE_BISHOP, ce_dir_offset_bishop, 1);
  ce_FindPieceByStep(pos, PIECE_QUEEN, ce_dir_offset_queen, 1);
  ce_FindPieceByStep(pos, PIECE_KNIGHT, ce_dir_offset_knight, 0);
  ce_FindPieceByStep(pos, PIECE_KING, ce_dir_offset_queen, 0);

  ce_FindPawnPiece(pos+17, COLOR_BLACK);
  ce_FindPawnPiece(pos+15, COLOR_BLACK);
  ce_FindPawnPiece(pos-17, COLOR_WHITE);
  ce_FindPawnPiece(pos-15, COLOR_WHITE);
}

/*
  calculate the summed weight of pieces with specified color which can move to a specified position

  argument:
    pos: 	the position which should be analysed
    color: 	the color of those pieces which should be analysed
		e.g. if a black piece is at 'pos' and 'color' is white then this procedure returns the white atting count
*/
uint8_t ce_GetPositionAttackWeight(uint8_t pos, uint8_t color)
{
  ce_CalculatePositionWeight(pos);
  return lrc_obj.find_piece_weight[color];
}

uint8_t ce_GetPositionAttackCount(uint8_t pos, uint8_t color)
{
  ce_CalculatePositionWeight(pos);
  return lrc_obj.find_piece_cnt[color];
}


/*==============================================================*/
/* depth search starts here: loop over all pieces of the current color on the board */
/*==============================================================*/

void ce_LoopPieces(void)
{
  stack_element_p e = stack_GetCurrElement();
  /* start with lower left position (A1) */
  e->current_pos = 0;
  do
  {
    e->current_cp = cp_GetFromBoard(e->current_pos);
    /* check if the position on the board is empty */
    if ( e->current_cp != 0 )
    {
      /* only generate moves for the current color */
      if ( e->current_color == cp_GetColor(e->current_cp) )
      {
	chess_Thinking();
	
	/* find out which piece is used */
	switch(cp_GetPiece(e->current_cp))
	{
	  case PIECE_NONE:
	    break;
	  case PIECE_PAWN:
	    ce_LoopPawn();
	    break;
	  case PIECE_KNIGHT:
	    ce_LoopKnight();
	    break;
	  case PIECE_BISHOP:
	    ce_LoopBishop();
	    break;
	  case PIECE_ROOK:
	    ce_LoopRook();
	    break;
	  case PIECE_QUEEN:
	    ce_LoopQueen();
	    break;
	  case PIECE_KING:
	    ce_LoopKing();
	    break;
	}
      }
    }    
    e->current_pos = cu_NextPos(e->current_pos);
  } while( e->current_pos != 0 );
}

/*==============================================================*/
/* user interface */
/*==============================================================*/

/*
eval_t chess_EvalCurrBoard(uint8_t color)
{
  stack_Init(0);
  stack_GetCurrElement()->current_color = color;
  ce_LoopPieces();
  return stack_GetCurrElement()->best_eval;
}
*/

/* clear any marks on the board */
void chess_ClearMarks(void)
{
  uint8_t i;
  for( i = 0; i < 64; i++ )
     lrc_obj.board[i] &= ~CP_MARK_MASK;
}

/*
  Mark all pieces which can do moves. This is done by setting flags on the global board
*/
void chess_MarkMovable(void)
{
  stack_Init(0);
  //stack_GetCurrElement()->current_color = color;
  lrc_obj.check_mode = CHECK_MODE_MOVEABLE;
  ce_LoopPieces();
}

/*
  Checks, if the piece can move from src_pos to dest_pos

  src_pos: The game position of a piece on the chess board
*/
void chess_MarkTargetMoves(uint8_t src_pos)
{
  stack_Init(0);
  stack_GetCurrElement()->current_color = cp_GetColor(cp_GetFromBoard(src_pos));
  lrc_obj.check_src_pos = src_pos;
  lrc_obj.check_mode = CHECK_MODE_TARGET_MOVE;  
  ce_LoopPieces();
}

/*
  first call should start with 255
  this procedure will return 255 if 
      - there are no marks at all
      - it has looped over all marks once
*/
uint8_t chess_GetNextMarked(uint8_t arg, uint8_t is_prev)
{
  uint8_t i;
  uint8_t pos = arg;
  for(i = 0; i < 64; i++)
  {
    if ( is_prev != 0 )
      pos = cu_PrevPos(pos);
    else
      pos = cu_NextPos(pos);
    if ( arg != 255 && pos == 0 )
      return 255;
    if ( cp_IsMarked(cp_GetFromBoard(pos)) )
      return pos;
  }
  return 255;
}


/* make a manual move: this is a little bit more than cu_Move() */
void chess_ManualMove(uint8_t src, uint8_t dest)
{
  uint8_t cp;
  
  /* printf("chess_ManualMove %02x -> %02x\n", src, dest); */
  
  /* if all other things fail, this is the place where the game is to be decided: */
  /* ... if the KING is captured */
  cp = cp_GetFromBoard(dest);
  if ( cp_GetPiece(cp) == PIECE_KING )
  {
    lrc_obj.is_game_end = 1;
    lrc_obj.lost_side_color = cp_GetColor(cp);    
  }

  /* clear ply history here, to avoid memory overflow */
  /* may be the last X moves can be kept here */
  cu_ReduceHistoryByFullMove();
  /* perform the move on the board */
  cu_Move(src, dest);
  
  /* update en passant double move positions: en passant position is removed after two half moves  */
  lrc_obj.pawn_dbl_move[lrc_obj.ply_count&1]  = ILLEGAL_POSITION;
  
  /* update the global half move counter */
  lrc_obj.ply_count++;


  /* make a small check about the end of the game */
  /* use at least depth 1, because we must know if the king can still move */
  /* this is: King moves at level 0 and will be captured at level 1 */
  /* so we check if the king can move and will not be captured at search level 1 */
  
  stack_Init(1);
  ce_LoopPieces(); 

  /* printf("chess_ManualMove/analysis best_from_pos %02x -> best_to_pos %02x\n", stack_GetCurrElement()->best_from_pos, stack_GetCurrElement()->best_to_pos); */

  /* analyse the eval result */
  
  /* check if the other player has any moves left */
  if ( stack_GetCurrElement()->best_from_pos == ILLEGAL_POSITION )
  {
    uint8_t color;
    /* conditions: */
    /* 1. no King, should never happen, opposite color has won */
    /*		this is already checked above at the beginning if this procedure */
    /* 2. King is under attack, opposite color has won */
    /* 3. King is not under attack, game is a draw */

    uint8_t i = 0;
    color = lrc_obj.ply_count;
    color &= 1;
    do
    {
      cp = cp_GetFromBoard(i);
      /* look for the King */
      if ( cp_GetPiece(cp) == PIECE_KING )
      {
	if ( cp_GetColor(cp) == color )
	{
	  /* check if  KING is attacked */
	  if ( ce_GetPositionAttackCount(i, color^1) != 0 )
	  {
	    /* KING is under attack (check) and can not move: Game is lost */
	    lrc_obj.is_game_end = 1;
	    lrc_obj.lost_side_color = color; 
	  }
	  else
	  {
	    /* KING is NOT under attack (check) but can not move: Game is a draw */
	    lrc_obj.is_game_end = 1;
	    lrc_obj.lost_side_color = 2; 
	  }
	  /* break out of the loop */
	  break;	  
	}
      }
      i = cu_NextPos(i);
    } while( i != 0 );
  }
}

/* let the computer do a move */
void chess_ComputerMove(uint8_t depth)
{
  stack_Init(depth);
  
  //stack_GetCurrElement()->current_color = lrc_obj.ply_count;
  //stack_GetCurrElement()->current_color &= 1;
  
  cu_ReduceHistoryByFullMove();
  ce_LoopPieces();

  chess_ManualMove(stack_GetCurrElement()->best_from_pos, stack_GetCurrElement()->best_to_pos);
}


/*==============================================================*/
/* unix code */
/*==============================================================*/

#ifdef UNIX_MAIN

#include <stdio.h>
#include <string.h>

char *piece_str[] = {
  /* 0x00 */
  "  ", 
  "wP", 
  "wN", 
  "wB", 
  
  /* 0x04 */
  "wR", 
  "wQ", 
  "wK", 
  "w?", 

  /* 0x08 */
  "w?", 
  "w?", 
  "w?", 
  "w?", 
  
  /* 0x0c */
  "w?", 
  "w?", 
  "w?", 
  "w?", 

  /* 0x10 */
  "b ",
  "bP", 
  "bN", 
  "bB", 
  "bR", 
  "bQ", 
  "bK", 
  "b?", 

  "b?", 
  "b?", 
  "b?", 
  "b?", 
  "b?", 
  "b?", 
  "b?", 
  "b?"
};

void chess_Thinking(void)
{
  uint8_t i;
  uint8_t cp = cp_GetPiece(stack_GetCurrElement()->current_cp);
  
  printf("Thinking:  ", piece_str[cp], stack_GetCurrElement()->current_pos);
  
  for( i = 0; i <= lrc_obj.curr_depth; i++ )
    printf("%s ", piece_str[(lrc_obj.stack_memory+i)->current_cp]);
  
  printf("    \r");
}

void board_Show(void)
{
  uint8_t i, j, cp;
  char buf[10];
  for ( i = 0; i < 8; i++ )
  {
    printf("%1d ", 7-i);
    for ( j = 0; j < 8; j++ )
    {
      /* get piece from global board */
      cp = lrc_obj.board[(7-i)*8+j];
      strcpy(buf, piece_str[cp&COLOR_PIECE_MASK]);
      
      if ( (cp & CP_MARK_MASK) != 0 )
      {
	buf[0] = '#';
      }
      
      /* mask out any bits except color and piece index */
      cp &= COLOR_PIECE_MASK;
      printf("%s %02x ", buf, cp);
      
    }
    printf("\n");
  }
}

int main(void)
{
  uint8_t depth = 3;
  chess_SetupBoard();
  board_Show();
  puts("");
    
 
 /* 
  chess_ClearMarks();
  chess_MarkMovable(COLOR_WHITE);
  board_Show();
  */
  
  chess_ManualMove(0x006, 0x066);
  
  printf("lrc_obj.is_game_end: %d\n" , lrc_obj.is_game_end);
  printf("lrc_obj.lost_side_color: %d\n" , lrc_obj.lost_side_color);

  chess_ComputerMove(2);

  printf("lrc_obj.is_game_end: %d\n" , lrc_obj.is_game_end);
  printf("lrc_obj.lost_side_color: %d\n" , lrc_obj.lost_side_color);
  
  board_Show();

}



#else

/*==============================================================*/
/* display menu */
/*==============================================================*/

//#define MNU_FONT font_5x7
#define MNU_FONT u8g_font_5x8r
//#define MNU_FONT font_6x9
#define MNU_ENTRY_HEIGHT 9

char *mnu_title = "Little Rook Chess";
char *mnu_list[] = { "New Game (White)", "New Game (Black)", "Undo Move", "Return" };
uint8_t mnu_pos = 0;
uint8_t mnu_max = 4;

void mnu_DrawHome(uint8_t is_highlight)
{
  uint8_t x = lrc_u8g->width - 35;  
  uint8_t y = (lrc_u8g->height-1);
  uint8_t t;
  
  u8g_SetFont(lrc_u8g, u8g_font_5x7r);
  u8g_SetDefaultForegroundColor(lrc_u8g);
  t = u8g_DrawStrP(lrc_u8g, x, y -1, U8G_PSTR("Options"));
    
  if ( is_highlight )
    u8g_DrawFrame(lrc_u8g, x-1, y - MNU_ENTRY_HEIGHT +1, t, MNU_ENTRY_HEIGHT);  
}

void mnu_DrawEntry(uint8_t y, char *str, uint8_t is_clr_background, uint8_t is_highlight)
{
  uint8_t t, x;
  u8g_SetFont(lrc_u8g, MNU_FONT);
  t = u8g_GetStrWidth(lrc_u8g, str);
  x = u8g_GetWidth(lrc_u8g);
  x -= t;
  x >>= 1;
  
  if ( is_clr_background )
  {
    u8g_SetDefaultBackgroundColor(lrc_u8g);
    u8g_DrawBox(lrc_u8g, x-3, (lrc_u8g->height-1) - (y+MNU_ENTRY_HEIGHT-1+2), t+5, MNU_ENTRY_HEIGHT+4);
  }
  
  u8g_SetDefaultForegroundColor(lrc_u8g);
  u8g_DrawStr(lrc_u8g, x, (lrc_u8g->height-1) - y, str);
  
  if ( is_highlight )
  {
    u8g_DrawFrame(lrc_u8g, x-1, (lrc_u8g->height-1) - y -MNU_ENTRY_HEIGHT +1, t, MNU_ENTRY_HEIGHT);
  }
}

void mnu_Draw(void)
{
  uint8_t i;
  uint8_t t,y;
  /* calculate hight of the complete menu */
  y = mnu_max;
  y++; 					/* consider also some space for the title */
  y++; 					/* consider also some space for the title */
  y *= MNU_ENTRY_HEIGHT;
  
  /* calculate how much space will be left */
  t = u8g_GetHeight(lrc_u8g);			
  t -= y;
  
  /* topmost pos start half of that empty space from the top */
  t >>= 1;
  y = u8g_GetHeight(lrc_u8g);
  y -= t;
  
  y -= MNU_ENTRY_HEIGHT;
  mnu_DrawEntry(y, mnu_title, 0, 0);
  
  y -= MNU_ENTRY_HEIGHT;
  
  
  for( i = 0; i < mnu_max; i++ )
  {
    y -= MNU_ENTRY_HEIGHT;
    mnu_DrawEntry(y, mnu_list[i], 0, i == mnu_pos);
  }
}

void mnu_Step(uint8_t key_cmd)
{
    if ( key_cmd == CHESS_KEY_NEXT )
    {
      if ( mnu_pos+1 < mnu_max )
	mnu_pos++;
    }
    else if ( key_cmd == CHESS_KEY_PREV )
    {
      if ( mnu_pos > 0 )
	mnu_pos--;
    }
}




uint8_t chess_key_code = 0;
uint8_t chess_key_cmd = 0;
#define CHESS_STATE_MENU 0
#define CHESS_STATE_SELECT_START 1
#define CHESS_STATE_SELECT_PIECE 2
#define CHESS_STATE_SELECT_TARGET_POS 3
#define CHESS_STATE_THINKING 4
#define CHESS_STATE_GAME_END 5
uint8_t chess_state = CHESS_STATE_MENU;
uint8_t chess_source_pos = 255;
uint8_t chess_target_pos = 255;

const uint8_t chess_pieces_body_bm[] PROGMEM = 
{
  /* PAWN */ 		0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, /* 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x00, */ 
  /* KNIGHT */		0x00, 0x00, 0x1c, 0x2c, 0x04, 0x04, 0x0e, 0x00,
  /* BISHOP */		0x00, 0x00, 0x1c, 0x1c, 0x1c, 0x08, 0x00, 0x00, /* 0x00, 0x00, 0x08, 0x1c, 0x1c, 0x08, 0x00, 0x00, */
  /* ROOK */		0x00, 0x00, 0x00, 0x1c, 0x1c, 0x1c, 0x1c, 0x00,
  /* QUEEN */		0x00, 0x00, 0x14, 0x1c, 0x08, 0x1c, 0x08, 0x00,
  /* KING */		0x00, 0x00, 0x00, 0x08, 0x3e, 0x1c, 0x08, 0x00,
};

#ifdef NOT_REQUIRED
/* white pieces are constructed by painting black pieces and cutting out the white area */
const uint8_t chess_white_pieces_bm[] PROGMEM = 
{
  /* PAWN */ 		0x00, 0x00, 0x0c, 0x12, 0x12, 0x0c, 0x1e, 0x00, 
  /* KNIGHT */		0x00, 0x1c, 0x22, 0x52, 0x6a, 0x0a, 0x11, 0x1f,
  /* BISHOP */		0x00, 0x08, 0x14, 0x22, 0x22, 0x14, 0x08, 0x7f,
  /* ROOK */		0x00, 0x55, 0x7f, 0x22, 0x22, 0x22, 0x22, 0x7f,
  /* QUEEN */		0x00, 0x55, 0x2a, 0x22, 0x14, 0x22, 0x14, 0x7f,
  /* KING */		0x08, 0x1c, 0x49, 0x77, 0x41, 0x22, 0x14, 0x7f,
};
#endif

const uint8_t chess_black_pieces_bm[] PROGMEM = 
{
  /* PAWN */ 		0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x3c, 0x00, /* 0x00, 0x00, 0x0c, 0x1e, 0x1e, 0x0c, 0x1e, 0x00, */ 
  /* KNIGHT */		0x00, 0x1c, 0x3e, 0x7e, 0x6e, 0x0e, 0x1f, 0x1f,
  /* BISHOP */		0x00, 0x1c, 0x2e, 0x3e, 0x3e, 0x1c, 0x08, 0x7f,  /*0x00, 0x08, 0x1c, 0x3e, 0x3e, 0x1c, 0x08, 0x7f,*/
  /* ROOK */		0x00, 0x55, 0x7f, 0x3e, 0x3e, 0x3e, 0x3e, 0x7f,
  /* QUEEN */		0x00, 0x55, 0x3e, 0x3e, 0x1c, 0x3e, 0x1c, 0x7f,
  /* KING -*/		0x08, 0x1c, 0x49, 0x7f, 0x7f, 0x3e, 0x1c, 0x7f,
};


#if defined(DOGXL160_HW_GR)
#define BOXSIZE 13
#define BOXOFFSET 3
#else
#define BOXSIZE 8
#define BOXOFFSET 1
#endif

u8g_uint_t chess_low_edge;
uint8_t chess_boxsize = 8;
uint8_t chess_boxoffset = 1;


void chess_DrawFrame(uint8_t pos, uint8_t is_bold)
{
  u8g_uint_t x0, y0;

  x0 = pos;
  x0 &= 15;
  if ( lrc_obj.orientation != COLOR_WHITE )
    x0 ^= 7;

  y0 = pos;
  y0>>= 4;
  if ( lrc_obj.orientation != COLOR_WHITE )
    y0 ^= 7;
  
  x0 *= chess_boxsize;
  y0 *= chess_boxsize;
  
  u8g_SetDefaultForegroundColor(lrc_u8g);
  u8g_DrawFrame(lrc_u8g, x0, chess_low_edge - y0 - chess_boxsize+1, chess_boxsize, chess_boxsize);
  
  
  if ( is_bold )
  {
      x0--;
      y0++;
  
    u8g_DrawFrame(lrc_u8g, x0, chess_low_edge - y0 - chess_boxsize +1, chess_boxsize+2, chess_boxsize+2);
  }
}


void chess_DrawBoard(void)
{
  uint8_t i, j, cp;
  const uint8_t *ptr;  /* pointer into PROGMEM */
  
  if ( U8G_MODE_GET_BITS_PER_PIXEL(u8g_GetMode(lrc_u8g)) > 1 )
  {
    for( i = 0; i < 8; i++ )
      for( j = 0; j < 8; j++ )
      {
        uint8_t x,y;
        x = i;
        x*=chess_boxsize;
        y = j;
        y*=chess_boxsize;
        if ( ((i^j) & 1)  == 0 )
          u8g_SetDefaultMidColor(lrc_u8g);  
        else
          u8g_SetDefaultBackgroundColor(lrc_u8g);  
        u8g_DrawBox(lrc_u8g, x,chess_low_edge-y-chess_boxsize+1,chess_boxsize,chess_boxsize);
      }
    //u8g_SetDefaultForegroundColor(lrc_u8g);  
  }
  else
  {
    uint8_t x_offset = 1;
    u8g_SetDefaultForegroundColor(lrc_u8g);  
    for( i = 0; i < 8*8; i+=8 )
    {
      for( j = 0; j < 8*8; j+=8 )
      {
        if ( ((i^j) & 8)  == 0 )
        {
          u8g_DrawPixel(lrc_u8g, j+0+x_offset, chess_low_edge - i-0);
          u8g_DrawPixel(lrc_u8g, j+0+x_offset, chess_low_edge - i-2);
          u8g_DrawPixel(lrc_u8g, j+0+x_offset, chess_low_edge - i-4);
          u8g_DrawPixel(lrc_u8g, j+0+x_offset, chess_low_edge - i-6);
          u8g_DrawPixel(lrc_u8g, j+2+x_offset, chess_low_edge - i-0);
          u8g_DrawPixel(lrc_u8g, j+2+x_offset, chess_low_edge - i-6);
          u8g_DrawPixel(lrc_u8g, j+4+x_offset, chess_low_edge - i-0);
          u8g_DrawPixel(lrc_u8g, j+4+x_offset, chess_low_edge - i-6);
          u8g_DrawPixel(lrc_u8g, j+6+x_offset, chess_low_edge - i-0);
          u8g_DrawPixel(lrc_u8g, j+6+x_offset, chess_low_edge - i-2);
          u8g_DrawPixel(lrc_u8g, j+6+x_offset, chess_low_edge - i-4);
          u8g_DrawPixel(lrc_u8g, j+6+x_offset, chess_low_edge - i-6);
        }
      }
    }
  }
  
  for ( i = 0; i < 8; i++ )
  {
    for ( j = 0; j < 8; j++ )
    {
      /* get piece from global board */
      if ( lrc_obj.orientation == COLOR_WHITE )
      {
	cp =  lrc_obj.board[i*8+j];
      }
      else
      {
	cp =  lrc_obj.board[(7-i)*8+7-j];
      }
      if ( cp_GetPiece(cp) != PIECE_NONE )
      {
	ptr = chess_black_pieces_bm;
	ptr += (cp_GetPiece(cp)-1)*8;
        u8g_SetDefaultForegroundColor(lrc_u8g);
        u8g_DrawBitmapP(lrc_u8g, j*chess_boxsize+chess_boxoffset-1, chess_low_edge - (i*chess_boxsize+chess_boxsize-chess_boxoffset), 1, 8, ptr);
        
	if ( cp_GetColor(cp) == lrc_obj.strike_out_color ) 
	{
	  ptr = chess_pieces_body_bm;
	  ptr += (cp_GetPiece(cp)-1)*8;
          u8g_SetDefaultBackgroundColor(lrc_u8g);
          u8g_DrawBitmapP(lrc_u8g, j*chess_boxsize+chess_boxoffset-1, chess_low_edge - (i*chess_boxsize+chess_boxsize-chess_boxoffset), 1, 8, ptr);
	}
      }
    }
  }
  
  if ( (chess_source_pos & 0x88) == 0 )
  {
    chess_DrawFrame(chess_source_pos, 1);
  }

  if ( (chess_target_pos & 0x88) == 0 )
  {
    chess_DrawFrame(chess_target_pos, 0);
  }
  
}


void chess_Thinking(void)
{
}

void chess_Init(u8g_t *u8g, uint8_t body_color)
{
  lrc_u8g = u8g;

  chess_low_edge = u8g_GetHeight(lrc_u8g);
  chess_low_edge--;
  

  if ( U8G_MODE_GET_BITS_PER_PIXEL(u8g_GetMode(lrc_u8g)) == 1 )
  {
  
    chess_boxsize = 8;
    chess_boxoffset = 1;
  }
  else
  {

    /*    
    if ( u8g_GetHeight(lrc_u8g) >= 12*8 )
    {
      chess_boxsize = 12;
      chess_boxoffset = 3;
    }
    else */ if ( u8g_GetHeight(lrc_u8g) >= 11*8 )
    {
      chess_boxsize = 10;
      chess_boxoffset = 2;
    }
    else
    {
      chess_boxsize = 8;
      chess_boxoffset = 1;      
    }
    
    if ( u8g_GetHeight(lrc_u8g) > 64 )
      chess_low_edge -= (u8g_GetHeight(lrc_u8g)-chess_boxsize*8) / 2;
    
  }
    
  lrc_obj.strike_out_color = body_color;
  chess_SetupBoard();
}



void chess_Draw(void)
{
  if ( chess_state == CHESS_STATE_MENU )
  {
    if ( lrc_obj.ply_count == 0)
      mnu_max = 2;
    else
      mnu_max = 4;
    mnu_Draw();
  }
  else
  {
    chess_DrawBoard();
    
    {
      uint8_t i;
      uint8_t entries = lrc_obj.chm_pos;
      if ( entries > 4 )
	entries = 4;
      
      u8g_SetFont(lrc_u8g, u8g_font_5x7);
      u8g_SetDefaultForegroundColor(lrc_u8g);
      for( i = 0; i < entries; i++ )
      {
        
#if defined(DOGXL160_HW_GR) || defined(DOGXL160_HW_BW)
	dog_DrawStr(u8g_GetWidth(lrc_u8g)-35, u8g_GetHeight(lrc_u8g)-8*(i+1), font_5x7, cu_GetHalfMoveStr(lrc_obj.chm_pos-entries+i));
#else
        u8g_DrawStr(lrc_u8g, u8g_GetWidth(lrc_u8g)-35, 8*(i+1), cu_GetHalfMoveStr(lrc_obj.chm_pos-entries+i));
#endif

      }
      
    }
    
    if ( chess_state == CHESS_STATE_SELECT_PIECE )
      mnu_DrawHome(chess_source_pos == 255);
    else if ( chess_state == CHESS_STATE_SELECT_TARGET_POS )
      mnu_DrawHome(chess_target_pos == 255);
    else
      mnu_DrawHome(0);
      
    if ( chess_state == CHESS_STATE_GAME_END )
    {
      switch( lrc_obj.lost_side_color )
      {
	case COLOR_WHITE:
	  mnu_DrawEntry(u8g_GetHeight(lrc_u8g) / 2-2, "Black wins", 1, 1);
	  break;
	case COLOR_BLACK:
	  mnu_DrawEntry(u8g_GetHeight(lrc_u8g) / 2-2, "White wins", 1, 1);
	  break;
	default:
	  mnu_DrawEntry(u8g_GetHeight(lrc_u8g) / 2-2, "Stalemate", 1, 1);
	  break;
      }  
    }
  }
}


void chess_Step(uint8_t keycode)
{
  if ( keycode == CHESS_KEY_NONE )
  {
    chess_key_cmd = chess_key_code;
    chess_key_code = CHESS_KEY_NONE;
  }
  else
  {
    chess_key_cmd = CHESS_KEY_NONE;
    chess_key_code = keycode;
  }
  //chess_ComputerMove(2);
  switch(chess_state)
  {
    case CHESS_STATE_MENU:
      mnu_Step(chess_key_cmd);
      if ( chess_key_cmd == CHESS_KEY_SELECT )
      {
	if ( mnu_pos == 0 )
	{
          chess_SetupBoard();
	  lrc_obj.orientation = 0;
	  chess_state = CHESS_STATE_SELECT_START;
	}
	else if ( mnu_pos == 1 )
	{
          chess_SetupBoard();
	  lrc_obj.orientation = 1;
	  chess_state = CHESS_STATE_THINKING;
	}
	else if ( mnu_pos == 2 )
	{
	  if ( lrc_obj.ply_count >= 2 )
	  {
	    cu_UndoHalfMove();
	    cu_UndoHalfMove();
	    lrc_obj.ply_count-=2;
	    if ( lrc_obj.ply_count == 0 )
	      mnu_pos = 0;
	  }
	  chess_state = CHESS_STATE_SELECT_START;
	}
	else if ( mnu_pos == 3 )
	{
	  chess_state = CHESS_STATE_SELECT_START;
	}
      }
      break;
    case CHESS_STATE_SELECT_START:
      chess_ClearMarks();
      chess_MarkMovable();
      chess_source_pos = chess_GetNextMarked(255, 0);
      chess_target_pos = ILLEGAL_POSITION;
      chess_state = CHESS_STATE_SELECT_PIECE;
      break;
      
    case CHESS_STATE_SELECT_PIECE:
      if ( chess_key_cmd == CHESS_KEY_NEXT )
      {
	chess_source_pos = chess_GetNextMarked(chess_source_pos, 0);
      }
      else if ( chess_key_cmd == CHESS_KEY_PREV )
      {
	chess_source_pos = chess_GetNextMarked(chess_source_pos, 1);
      }
      else if ( chess_key_cmd == CHESS_KEY_SELECT )
      {
	if ( chess_source_pos == 255 )
	{
	  chess_state = CHESS_STATE_MENU;
	}
	else
	{
	  chess_ClearMarks();
	  chess_MarkTargetMoves(chess_source_pos);
	  chess_target_pos = chess_GetNextMarked(255, 0);
	  chess_state = CHESS_STATE_SELECT_TARGET_POS;      
	}
      }
      break;
    case CHESS_STATE_SELECT_TARGET_POS:
      if ( chess_key_cmd == CHESS_KEY_NEXT )
      {
	chess_target_pos = chess_GetNextMarked(chess_target_pos, 0);
      }
      else if ( chess_key_cmd == CHESS_KEY_PREV )
      {
	chess_target_pos = chess_GetNextMarked(chess_target_pos, 1);
      }
      else if ( chess_key_cmd == CHESS_KEY_BACK )
      {
	chess_ClearMarks();
	chess_MarkMovable();
	chess_target_pos = ILLEGAL_POSITION;
	chess_state = CHESS_STATE_SELECT_PIECE;
      }
      else if ( chess_key_cmd == CHESS_KEY_SELECT )
      {
	chess_ManualMove(chess_source_pos, chess_target_pos);
	if ( lrc_obj.is_game_end != 0 )
	  chess_state = CHESS_STATE_GAME_END;
	else
	  chess_state = CHESS_STATE_THINKING;
	/* clear marks as some kind of feedback to the user... it simply looks better */
	chess_source_pos = ILLEGAL_POSITION;
	chess_target_pos = ILLEGAL_POSITION;
	chess_ClearMarks();
      }
      break;
    case CHESS_STATE_THINKING:
      chess_ComputerMove(2);
      if ( lrc_obj.is_game_end != 0 )
	chess_state = CHESS_STATE_GAME_END;
      else
	chess_state = CHESS_STATE_SELECT_START;
      break;
    case CHESS_STATE_GAME_END:
      if ( chess_key_cmd != CHESS_KEY_NONE )
      {
	chess_state = CHESS_STATE_MENU;  
	chess_SetupBoard();
      }	
      break;
  }
  
}

#endif


