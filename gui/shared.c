/****************************************************************

 shared.h

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

#import <OpenGL/gl.h>
#import <OpenGL/glext.h>
#import <OpenGL/glu.h>
#import <OpenGL/OpenGL.h>

#include "battle.h"
#include "shared.h"

static n_byte * graphics_buffer = 0L;


void battle_draw_init(void)
{
    glClearColor(0, 0.2, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void battle_draw(n_unit *un)
{
	n_combatant *comb = (n_combatant *)(un->combatants);
	n_byte2 loop = 0;

    if (un->alignment)
    {
        glColor3f(0.9, 0, 0);
    }
    else
    {
        glColor3f(0, 0, 0.9);
    }
    glBegin(GL_POINTS);

	while (loop < un->number_combatants) {
        if (comb[ loop ].wounds != NUNIT_DEAD) {
            glVertex2i(comb[loop].location_x, comb[loop].location_y);
        }
		loop++;
	}
    glEnd();
}

shared_cycle_state shared_cycle(n_uint ticks, n_byte fIdentification, n_int dim_x, n_int dim_y)
{
    return SHARED_CYCLE_OK;
}

n_int shared_init(n_byte view, n_uint random)
{
    graphics_buffer = engine_init(random);
    if (graphics_buffer)
    {
        return 0;
    }
    
    return -1;
}

void shared_close(void)
{
    engine_exit();
}

n_int shared_menu(n_int menuValue)
{
    return 0;
}

void shared_rotate(n_double num, n_byte wwind)
{
    
}

void shared_keyReceived(n_byte2 value, n_byte fIdentification)
{
    
}

void shared_keyUp(void)
{
    
}

void shared_mouseOption(n_byte option)
{
    
}

void shared_mouseReceived(n_int valX, n_int valY, n_byte fIdentification)
{
    engine_mouse(valX, valY);
}
void shared_mouseUp(void)
{
    
}

void shared_about(n_constant_string value)
{
    
}

void shared_color(n_byte2 * fit, n_int fIdentification)
{
    n_int loop = 0;
    n_int fitloop = 0;
    while (loop < 256)
    {
        fit[fitloop++] = loop << 8;
        fit[fitloop++] = loop << 8;
        fit[fitloop++] = loop << 8;
        
        loop++;
    }
    
    fit[ (128 * 3) + 0 ] = 255 << 8;
    fit[ (128 * 3) + 1 ] = 200 << 8;
    fit[ (128 * 3) + 2 ] = 200 << 8;
    
    fit[ (255 * 3) + 0 ] = 200 << 8;
    fit[ (255 * 3) + 1 ] = 200 << 8;
    fit[ (255 * 3) + 2 ] = 255 << 8;
}

void shared_draw(n_byte * outputBuffer, n_byte fIdentification, n_int dim_x, n_int dim_y)
{
    engine_update(1);
    
    
    /*
    n_int           ly = 0;
    n_int           loop = 0;
    n_int			loopColors = 0;
    n_byte2         fit[256*3];
    n_byte          colorTable[256][3];
    n_byte           * index = graphics_buffer;
    
    
    
    if (index == 0L) return;
    
    shared_color(fit, 0);
    
    while(loopColors < 256)
    {
        colorTable[loopColors][0] = fit[loop++] >> 8;
        colorTable[loopColors][1] = fit[loop++] >> 8;
        colorTable[loopColors][2] = fit[loop++] >> 8;
        loopColors++;
    }
    loop = 0;
    while(ly < dim_y)
    {
        n_int    lx = 0;
        n_byte * indexLocalX = &index[(dim_y-ly-1)*dim_x];
        while(lx < dim_x)
        {
            unsigned char value = indexLocalX[lx++] ;
            outputBuffer[loop++] = colorTable[value][0];
            outputBuffer[loop++] = colorTable[value][1];
            outputBuffer[loop++] = colorTable[value][2];
        }
        ly++;
    }
     */
}

n_int shared_new(n_uint seed)
{
    engine_update(0);
    
    return 0;
}

n_byte shared_openFileName(n_string cStringFileName, n_byte isScript)
{
    return 0;
}

void shared_saveFileName(n_string cStringFileName)
{
    
}

void shared_script_debug_handle(n_string cStringFileName)
{
    
}

n_uint shared_max_fps(void)
{
    return 30;
}



#ifndef	_WIN32

n_int sim_thread_console_quit(void)
{
    return 0;
}

#endif

