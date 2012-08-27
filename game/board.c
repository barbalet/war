/****************************************************************
 
	board.c - Noble Warfare Skirmish
 
 =============================================================
 
 Copyright 1996-2012 Tom Barbalet. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or
 sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
 This software and Noble Ape are a continuing work of Tom Barbalet,
 begun on 13 June 1996. No apes or cats were harmed in the writing
 of this software.
 
 ****************************************************************/


#ifdef	_WIN32
#include "../noble/noble.h"
#else
#include "noble.h"
#endif

#include "battle.h"

static n_byte	*board;

void    board_init(n_byte * value);

n_byte	board_add(n_int * ptx, n_int * pty);
n_byte	board_move(n_int frx, n_int fry, n_int * ptx, n_int * pty);

void    board_remove(n_int ptx, n_int pty);

#define		XY_BOARD(px, py)		board[((px)>>3 | ((py)<<6))]

#define		FILL_BOARD(px, py)		XY_BOARD(px, py) |= 128 >> (px&7)
#define		CLEAR_BOARD(px,py)		XY_BOARD(px, py) &= (0xff ^ (128 >> (px&7)))

#define		OCCUPIED_BOARD(px,py)	(((XY_BOARD(px, py)<<(px&7))&128) == 128)


void    board_init(n_byte * value)
{
	board = value;
}

void board_remove(n_int ptx, n_int pty){
	CLEAR_BOARD(ptx, pty);
}

static	n_byte	board_find(n_int * ptx, n_int * pty) {
	n_int px = *ptx;
	n_int py = *pty;
	n_uint	best_dsqu = 0xffffffff;
	n_int	best_x = 0, best_y = 0;
	n_int ly = -1;

	px = (px + 512)&511;
	py = (py + 512)&511;

	if(OCCUPIED_BOARD(px,py)==0) {
		*ptx = px;
		*pty = py;
		return 1;
	}
	while(ly<2) {
		n_int	lx = -1;
		n_int y_val = (py+ly+512)&511;
		while(lx<2) {
			n_int x_val = (px+lx+512)&511;
			if(OCCUPIED_BOARD(x_val,y_val)==0) {
				n_int dx = (px - lx);
				n_int dy = (py - ly);
				n_uint	dsqu = (dx*dx) + (dy*dy);
				if(dsqu<best_dsqu) {
					best_dsqu = dsqu;
					best_x = x_val;
					best_y = y_val;
				}
			}
			lx++;
		}
		ly++;
	}
	if(best_dsqu != 0xffffffff) {
		*ptx = best_x;
		*pty = best_y;
		return 1;
	}
	return 0;
}

n_byte	board_add(n_int * ptx, n_int * pty) {
	if(board_find(ptx, pty)) {
		FILL_BOARD(*ptx, *pty);
		return 1;
	}
	return 0;
}

n_byte	board_move(n_int frx, n_int fry, n_int * ptx, n_int * pty) {
	if(board_find(ptx, pty)) {
		CLEAR_BOARD(frx,fry);
		FILL_BOARD(*ptx, *pty);
		return 1;
	}
	return 0;
}