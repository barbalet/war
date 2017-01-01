/****************************************************************
 
	battle.h - Noble Warfare Skirmish
 
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

#include "noble.h"

#define MELEE_ATTACK_DISTANCE_SQUARED	5
#define LARGEST_DISTANCE_SQUARED 	    0xffff

#define BATTLE_BOARD_WIDTH              (1024)
#define BATTLE_BOARD_HEIGHT             (768)

#define OUTSIDE_WIDTH(num)   (((num) < 0) || ((num) >= BATTLE_BOARD_WIDTH))
#define OUTSIDE_HEIGHT(num)  (((num) < 0) || ((num) >= BATTLE_BOARD_HEIGHT))

#define BATTLE_BOARD_SIZE               (BATTLE_BOARD_WIDTH*BATTLE_BOARD_HEIGHT)

#define NUNIT_NO_ATTACK                 0xffff

#define NUNIT_DEAD			            (255)


typedef struct n_combatant
{
    n_vect2 location;
    
	n_byte2 attacking;
	n_byte2 distance_squ;
    
    n_byte speed_current;
	n_byte direction_facing;
	n_byte wounds;
	n_byte combatant_state;
}
n_combatant;

#define	GET_TYPE(un)		  ((n_type *)((un)->unit_type))

#define	UNIT_SIZE(un)		  ((GET_TYPE(un)->stature) >> 1)
#define	UNIT_ORDER(un)		  ((GET_TYPE(un)->stature) &  1)


typedef struct n_type {
	n_byte  defence;
	n_byte  melee_attack;
	n_byte  melee_damage;
	n_byte  melee_armpie;

	n_byte  missile_attack;
	n_byte  missile_damage;
	n_byte  missile_armpie;
	n_byte  missile_rate;

	n_byte2 missile_range;
	n_byte  speed_maximum;
	n_byte  stature;
	n_byte  leadership;

	n_byte  wounds_per_combatant;
	n_byte  points_per_combatant;
}
n_type;

typedef struct n_unit {
	n_byte  morale;
	n_byte  angle;

	n_byte2 average[2];

	n_byte2 width;
	n_byte2 number_combatants;

	n_byte  alignment;
	n_byte  missile_number;

	n_byte  missile_timer;
	n_byte2 number_living;

	void *unit_type;
	void *combatants;
	void *unit_attacking;
}
n_unit;


typedef enum
{
    GVAR_RANDOM_0 = 0,
    GVAR_RANDOM_1,
																			  
    GVAR_ATTACK_MELEE_DSQ,         /* val = 5 */
    
    GVAR_DECLARE_GROUP_FACING_DSQ, /* val = 0x7fff */
    GVAR_DECLARE_MAX_START_DSQ,    /* val = 0xffff */
    GVAR_DECLARE_ONE_TO_ONE_DSQ,   /* val = 0xffff */
    GVAR_DECLARE_CLOSE_ENOUGH_DSQ  /* val = 5 */
}general_variables;

typedef void (*battle_function)(n_unit * un, n_byte2 * gvar);
typedef void (*combatant_function)(n_combatant * comb, n_byte2 * gvar, void * values);

#define NUMBER_COMBATANTS_A     1024
#define NUMBER_COMBATANTS_B     1024
#define NUMBER_COMBATANTS       (NUMBER_COMBATANTS_A + NUMBER_COMBATANTS_B)

#define	COMB_MEMORY	  (sizeof(n_combatant) * NUMBER_COMBATANTS)
#define	UNIT_MEMORY	  (sizeof(n_unit) * 2)
#define	TYPE_MEMORY	  (sizeof(n_type) * 2)

#define SIZEOF_BUFFER (COMB_MEMORY + engine_MEMORY + UNIT_MEMORY + TYPE_MEMORY)

typedef enum{
    BC_NO_COMMAND = 0,
    BC_ATTACK,
    BC_SLOW_DOWN,
    BC_HALT,
    BC_REGROUP
}battle_command;

n_byte board_clear(n_vect2 * pt);

void * engine_init(n_uint random_init);

unsigned char engine_mouse(short px, short py);

void  engine_draw(void);
n_int engine_update(void);
n_int engine_new(void);

void engine_exit();

void battle_fill(n_unit * un, n_byte2 * gvar);
void battle_move(n_unit *un, n_byte2 * gvar);
void battle_declare(n_unit *un, n_byte2 * gvar);
void battle_attack(n_unit *un, n_byte2 * gvar);
void battle_remove_dead(n_unit *un, n_byte2 * gvar);

void draw_init(void);
void draw_cycle(n_unit *un, n_byte2 * gvar);

void draw_dpx(n_double dpx);
void draw_dpy(n_double dpy);
void draw_dpz(n_double dpz);

void  combatant_loop(combatant_function func, n_unit * un, n_byte2 * gvar, void * values);
void  battle_loop(battle_function func, n_unit * un, const n_uint count, n_byte2 * gvar);
n_byte battle_opponent(n_unit * un, n_uint	num);

void board_init(n_byte * value);
n_byte board_add(n_int * ptx, n_int * pty, n_byte color);
n_byte board_move(n_vect2 * fr, n_vect2 * pt);





