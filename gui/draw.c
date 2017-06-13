/****************************************************************

draw.c

 =============================================================

 Copyright 1996-2017 Tom Barbalet. All rights reserved.

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

#include "battle.h"
#include "shared.h"

static n_int px = 0;
static n_int py = 0;
static n_int pz = 0;

void draw_init(void)
{
    gldraw_background_green();
}

void draw_dpx(n_double dpx)
{
    n_int int_dpx = (n_int)(dpx * 100);
    px += int_dpx;
}

void draw_dpy(n_double dpy)
{
    n_int int_dpy = (n_int)(dpy * 100);
    py += int_dpy;
}

void draw_dpz(n_double dpz)
{
    n_int int_dpz = (n_int)(dpz * 100);
    n_int total_zoom = pz + int_dpz;
    if ((total_zoom > -100) && (total_zoom < 128))
    {
        pz = total_zoom;
    }
}

void draw_combatant(n_combatant * comb, n_byte2 *gvar, void * values)
{
    if (comb->wounds != NUNIT_DEAD)
    {
        n_vect2 translation;
        
        translation.x = (comb->location.x * 800) >> 10;
        translation.y = (comb->location.y * 800) >> 10;
        
        translation.x = ((translation.x + px)*(128+pz))>>7;
        translation.y = ((translation.y + py)*(128+pz))>>7;

        gldraw_vertex(&translation);
    }
}

void draw_cycle(n_unit *un, n_general_variables * gvar)
{
    if (un->alignment)
    {
        gldraw_red();
    }
    else
    {
        gldraw_green();
    }
    gldraw_start_points();
    combatant_loop(&draw_combatant, un, gvar, NOTHING);
    gldraw_end_points();
}
