/****************************************************************

draw.c

 =============================================================

 Copyright 1996-2015 Tom Barbalet. All rights reserved.

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

void draw_init(void)
{
    glClearColor(0, 0.05, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void draw_cycle(n_unit *un, n_byte2 * gvar)
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
