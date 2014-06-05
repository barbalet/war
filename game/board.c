/****************************************************************
 
	board.c - Noble Warfare Skirmish
 
 =============================================================
 
 Copyright 1996-2014 Tom Barbalet. All rights reserved.
 
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

static n_byte	*board = 0L;

#define		XY_BOARD(px, py)		board[(px) | ((py) << 9)]

void    board_init(n_byte * value)
{
	board = value;
}

void board_fill(n_int px, n_int py)
{
    if (board == 0L)
    {
        (void)SHOW_ERROR("board not initialized");
        return;
    }
    
    if ((px< 0) || (px > 511))
    {
        (void)SHOW_ERROR("px out of bounds");
        return;
    }
    if ((py< 0) || (py > 511))
    {
        (void)SHOW_ERROR("py out of bounds");
        return;
    }
    
    XY_BOARD(px, py) = 255;
}

void board_clear(n_int px, n_int py)
{
    if (board == 0L)
    {
        (void)SHOW_ERROR("board not initialized");
        return;
    }
    
    if ((px< 0) || (px > 511))
    {
        (void)SHOW_ERROR("px out of bounds");
        return;
    }
    if ((py< 0) || (py > 511))
    {
        (void)SHOW_ERROR("py out of bounds");
        return;
    }
    XY_BOARD(px, py) = 0;
}

static n_int board_occupied(n_int px, n_int py)
{
    if (board == 0L)
    {
        (void)SHOW_ERROR("board not initialized");
        return 1;
    }
    
    if ((px< 0) || (px > 511))
    {
        (void)SHOW_ERROR("px out of bounds");
        return 1;
    }
    if ((py< 0) || (py > 511))
    {
        (void)SHOW_ERROR("py out of bounds");
        return 1;
    }
    return (XY_BOARD(px, py) == 255);
}

static	n_byte	board_find(n_int * ptx, n_int * pty) {
	n_int px = *ptx;
	n_int py = *pty;
	n_uint	best_dsqu = 0xffffffff;
	n_int	best_x = 0, best_y = 0;
	n_int ly = -1;

	px = (px + 512)&511;
	py = (py + 512)&511;


    
	if(board_occupied(px,py)==0) {
		*ptx = px;
		*pty = py;
		return 1;
	}
	while(ly<2) {
		n_int	lx = -1;
		n_int y_val = (py+ly+512)&511;
		while(lx<2) {
			n_int x_val = (px+lx+512)&511;
			if(board_occupied(x_val,y_val)==0) {
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
		board_fill(*ptx, *pty);
		return 1;
	}
	return 0;
}

n_byte	board_move(n_vect2 * fr, n_vect2 * pt) {
    n_int ptx = pt->x;
    n_int pty = pt->y;
	if(board_find(&ptx, &pty))
    {
		board_clear(fr->x,fr->y);
		board_fill(ptx, pty);
        pt->x = ptx;
        pt->y = pty;
		return 1;
	}
	return 0;
}