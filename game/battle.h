/****************************************************************
 
	battle.h - Noble Warfare Skirmish
 
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


#define MELEE_ATTACK_DISTANCE_SQUARED	5
#define LARGEST_DISTANCE_SQUARED 	    0xffff

#define NUNIT_NO_ATTACK          0xffff
#define NUNIT_BOUNDARIES         511
#define NUNIT_EDGE               512
#define NUNIT_AROUND             511

#define NUNIT_DEAD			         255


typedef struct n_combatant {
	n_byte speed_current;
	n_byte direction_facing;
	n_byte wounds;
	n_byte combatant_state;

	n_byte2 location_x;
	n_byte2 location_y;
	n_byte2 attacking;
	n_byte2 distance_squ;
}
n_combatant;

#define	GET_TYPE(un)		  ((n_type *)((un)->unit_type))

#define	UNIT_SIZE(un)		  ((GET_TYPE(un)->stature) >> 1)
#define	UNIT_ORDER(un)		((GET_TYPE(un)->stature) &  1)


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

	n_byte2 average_x;
	n_byte2 average_y;

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

#define GVAR_RANDOM_0                         0
#define GVAR_RANDOM_1                         1
																			  
#define GVAR_ATTACK_MELEE_DSQ                 2 /* val = 5 */

#define GVAR_DECLARE_GROUP_FACING_DSQ         3 /* val = 0x7fff */
#define GVAR_DECLARE_MAX_START_DSQ            4 /* val = 0xffff */
#define GVAR_DECLARE_ONE_TO_ONE_DSQ           5 /* val = 0xffff */
#define GVAR_DECLARE_CLOSE_ENOUGH_DSQ         6 /* val = 5 */

typedef void (*battle_function)(n_unit * un);
typedef void (*battle_function_gvar)(n_unit * un, n_byte2 * gvar);


#define L_SIZE_SCREEN      ((512*512)>>3)

#define L_SIZE_SCREEN4     (L_SIZE_SCREEN>>2)


#define NUMBER_COMBATANTS_A     1024
#define NUMBER_COMBATANTS_B     1024
#define NUMBER_COMBATANTS       (NUMBER_COMBATANTS_A + NUMBER_COMBATANTS_B)

#define	COMB_MEMORY	  (sizeof(n_combatant) * NUMBER_COMBATANTS)
#define	UNIT_MEMORY	  (sizeof(n_unit) * 2)
#define	TYPE_MEMORY	  (sizeof(n_type) * 2)

#define SIZEOF_BUFFER (COMB_MEMORY + engine_MEMORY + UNIT_MEMORY + TYPE_MEMORY)