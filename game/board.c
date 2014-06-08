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

#define		XY_BOARD(px, py)		board[(px) | ((py) * BATTLE_BOARD_WIDTH)]

void board_init(n_byte * value)
{
	board = value;
}

static n_int board_location_check(n_int px, n_int py)
{
    if (board == 0L)
    {
        return SHOW_ERROR("board not initialized");
    }
    
    if ((px< 0) || (px >= BATTLE_BOARD_WIDTH))
    {
        return SHOW_ERROR("px out of bounds");
    }
    if ((py< 0) || (py >= BATTLE_BOARD_HEIGHT))
    {
        return SHOW_ERROR("py out of bounds");
    }
    
    return 0;
}

static void board_fill(n_int px, n_int py, n_byte number)
{
    if (board_location_check(px, py) == -1)
    {
        return;
    }
    XY_BOARD(px, py) = number;
}

n_byte board_clear(n_int px, n_int py)
{
    n_byte value;
    if (board_location_check(px, py) == -1)
    {
        return 0;
    }
    value = XY_BOARD(px, py);
    
    XY_BOARD(px, py) = 0;
    
    return value;
}

static n_int board_occupied(n_int px, n_int py)
{
    if (board_location_check(px, py) == -1)
    {
        return 1;
    }
    return (XY_BOARD(px, py) > 127);
}

static	n_byte	board_find(n_int * ptx, n_int * pty) {
	n_int px = *ptx;
	n_int py = *pty;
	n_uint	best_dsqu = 0xffffffff;
	n_int	best_x = 0, best_y = 0;
	n_int ly = -1;
    
	px = (px + BATTLE_BOARD_WIDTH) % BATTLE_BOARD_WIDTH;
	py = (py + BATTLE_BOARD_HEIGHT) % BATTLE_BOARD_HEIGHT;
    
	if(board_occupied(px,py)==0) {
		*ptx = px;
		*pty = py;
		return 1;
	}
	while(ly < 2)
    {
		n_int	lx = -1;
		n_int y_val = (py + ly + BATTLE_BOARD_HEIGHT) % BATTLE_BOARD_HEIGHT;
		while(lx < 2)
        {
			n_int x_val = (px + lx + BATTLE_BOARD_WIDTH) % BATTLE_BOARD_WIDTH;
			if(board_occupied(x_val,y_val)==0) {
				n_int dx = (px - lx);
				n_int dy = (py - ly);
				n_uint	dsqu = (dx*dx) + (dy*dy);
				if(dsqu < best_dsqu) {
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

n_byte	board_add(n_int * ptx, n_int * pty, n_byte color) {
	if(board_find(ptx, pty))
    {
		board_fill(*ptx, *pty, color);
		return 1;
	}
	return 0;
}

n_byte	board_move(n_vect2 * fr, n_vect2 * pt) {
    n_int ptx = pt->x;
    n_int pty = pt->y;
    
    if (board_location_check(ptx, pty) == -1)
    {
        return 0;
    }
    
	if(board_find(&ptx, &pty))
    {
		n_byte color = board_clear(fr->x,fr->y);
		board_fill(ptx, pty, color);
        pt->x = ptx;
        pt->y = pty;
		return 1;
	}
	return 0;
}