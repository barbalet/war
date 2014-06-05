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

#include "battle.h"
#include "shared.h"

static n_byte * graphics_buffer = 0L;

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

n_byte * shared_draw(n_byte fIdentification)
{
    engine_update(1);
    
    return graphics_buffer;
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
}

n_int shared_new(n_uint seed)
{
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

#ifndef	_WIN32

n_int sim_thread_console_quit(void)
{
    return 0;
}

#endif
