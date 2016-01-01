/****************************************************************

 shared.c

 =============================================================

 Copyright 1996-2016 Tom Barbalet. All rights reserved.

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

void battle_draw_init(void)
{
    glClearColor(0, 0.05, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void battle_draw(n_unit *un, n_byte2 * gvar)
{
	n_combatant *comb = (n_combatant *)(un->combatants);
	n_byte2 loop = 0;

    if (un->alignment)
    {
        glColor3f(1, 0, 0);
    }
    else
    {
        glColor3f(0.3, 0.3, 1);
    }
    glBegin(GL_POINTS);

	while (loop < un->number_combatants) {
        if (comb[ loop ].wounds != NUNIT_DEAD) {
            glVertex2i((GLint)comb[loop].location.x, (GLint)comb[loop].location.y);
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
    if (engine_init(random))
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

void shared_draw(n_byte * outputBuffer, n_byte fIdentification, n_int dim_x, n_int dim_y)
{
    engine_update();
    engine_draw();
}

n_int shared_new(n_uint seed)
{
    engine_new();
    
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
    return 60;
}



#ifndef	_WIN32

n_int sim_thread_console_quit(void)
{
    return 0;
}

#endif

