/****************************************************************
 
	battle.c - Noble Warfare Skirmish
 
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


/* this is the battle order with the single entry function */

void  battle_loop(battle_function func, n_unit * un, const n_uint count) {
	n_uint loop = 0;
	while(loop < count)
    {
		(*func)(&un[loop++]);
    }
}

void  battle_loop_gvar(battle_function_gvar func, n_unit * un,
                         const n_uint count , n_byte2 * gvar)
{
	n_uint loop = 0;
	while(loop < count)
		(*func)((&un[loop++]),gvar);
}

void battle_fill(n_unit * un) {
	const n_int     loc_order  = UNIT_ORDER(un);
	const n_byte    loc_wounds = GET_TYPE(un)->wounds_per_combatant;
	const n_int		loc_angle  = (un->angle);
	const n_int     loc_number = (un->number_combatants);
    n_vect2         local;

	n_int dx = (UNIT_SIZE(un)+2)/2;
	n_int dy = (UNIT_SIZE(un)+3)/2;

	n_combatant    *comb  = (un->combatants);

	n_int	        loc_width  = (un->width);
	n_int		    loc_height;

	n_int           pxx = 0, pxy = 0, pyx = 0, pyy = 0;
	n_int	        edgex, edgey, px0, px1, py0, py1;
	n_int           loop = 0;
	n_int	        line = 0;
    
    vect2_direction(&local, loc_angle, 20);
    
	if(loc_width > loc_number)
    {
		loc_width = loc_number; 
	}

	loc_height = (loc_number + loc_width - (loc_number % loc_width)) / loc_width;

	if((loc_order&1)==1) {
		if(dx == dy) {
			dx += 1;
			dy += 1;
		} else {
			dx = dy;
		}
	}
    
	/* rewrite in vectors */
	px0 = (local.x * dx);
	px1 = (local.y * dx);
	
	py0 = (local.y * dy);
	py1 = (local.x * dy);

	dx = (loc_width*dx);
	dy = (loc_height*dy);
	
	edgex = (un->average_x) - (((local.x*dx) + (local.y*dy)) >> 10);
	edgey = (un->average_y) - (((local.y*dx) - (local.x*dy)) >> 10);
	

	while(loop < loc_number) {
		n_int	pos_x = ((((pxx + pyx) >> 9) + edgex) % BATTLE_BOARD_WIDTH);
		n_int   pos_y = ((((pxy - pyy) >> 9) + edgey) % BATTLE_BOARD_HEIGHT);
        
		if(board_add(&pos_x, &pos_y)) {

			comb[loop].location_x = (n_byte2)(pos_x);
			comb[loop].location_y = (n_byte2)(pos_y);
			comb[loop].direction_facing = (n_byte)loc_angle;
			comb[loop].attacking = NUNIT_NO_ATTACK;
			comb[loop].wounds = loc_wounds;
			comb[loop].speed_current = 5;
			loop ++;
		}

		line ++;
		
		if(line == loc_width) {
			line = 0;
			pxx = 0;
			pxy = 0;
            
			pyx += py0;
			pyy += py1;
		} else {
			pxx += px0;
			pxy += px1;
		}

	}
}


static n_int battle_calc_damage(n_int wounds, n_int damage){
	wounds -= damage;
	if(wounds < 1)
    {
		wounds = 0;
    }
	return wounds;
}

#define	PROB_MEL	0
#define	PROB_MIS	1
#define	DAMA_MEL	2
#define	DAMA_MIS	3
#define	SPED_MAX	4
#define	RANG_MIS	5

static void battle_combatant_attack(n_combatant * comb, n_combatant * comb_at, 
							 n_byte2 * gvar, n_int *av){
	/* which opponent combatant are they attacking */
	const n_byte2	loc_attacking = comb->attacking;
	/* what is the squared distance between them */
	const n_int		distance_squared = comb->distance_squ;
	/* roll the 1024 sided dice */
	n_int			dice_roll = math_random(&gvar[GVAR_RANDOM_0]) & 1023;

	/* if the attacking combatant isn't dead */
	if (comb->wounds == NUNIT_DEAD) {
		return;
	}
	/* are they attacking a unit? */
	if (loc_attacking == NUNIT_NO_ATTACK) {
	    /* if not attacking then stop */
		comb->speed_current = 0;
		return;
	}

	comb_at = &comb_at[loc_attacking];

	/* if the distance is within the melee range */
	if (distance_squared < gvar[GVAR_ATTACK_MELEE_DSQ]) {	/* val1*/
		/* stop if melee attacking */
		comb->speed_current = 0;
		/* melee hit */
		if (dice_roll < av[PROB_MEL]) {
			comb_at->wounds = (n_byte) battle_calc_damage(comb_at->wounds, av[DAMA_MEL]);
		}
		/* else if the distance is within the missle range */
	} else if (distance_squared < av[RANG_MIS]) {
		/* missile hit */
		if (dice_roll < av[PROB_MIS]) {
			comb_at->wounds = (n_byte) battle_calc_damage(comb_at->wounds,av[DAMA_MIS]);
		}
	} else {
        if (comb->speed_current <= (n_byte)av[SPED_MAX])
        {
          comb->speed_current++;
        }
	}
}


void battle_attack(n_unit *un, n_byte2 * gvar) {

	n_int		additional_variables[6];
	n_uint		loop = 0;
	/* the combatants doing the attacking */
	n_combatant  *comb = (n_combatant *)(un->combatants);
	n_combatant  *comb_at;

	/* only worthwhile if the unit is attacking something */
	if(un->unit_attacking == 0L) {
		return;
	}

	{
		/* the range of the missile weapons */
		n_int	  rang_missile = 0;
		n_type   *typ = un->unit_type;
		/* the unit that is being attacked */
		n_unit	 *un_at =  un->unit_attacking;
		n_type   *typ_at = un_at->unit_type;
		/* the combatants being attacked */
		
		comb_at = un_at->combatants;

		/* the probabilities of causing damage to the attacked unit */

		additional_variables[ PROB_MEL ] = (typ->melee_attack) * (16 - (typ_at->defence) + (typ->melee_armpie));
		additional_variables[ PROB_MIS ] = (typ->missile_attack) * (16 - (typ_at->defence) + (typ->missile_armpie));

		additional_variables[ DAMA_MEL ] = typ->melee_damage;
		additional_variables[ DAMA_MIS ] = typ->missile_damage;
	
		additional_variables[ SPED_MAX ] = typ->speed_maximum;
				
		if(un->missile_number != 0) {
			if(un->missile_timer == typ->missile_rate) {
				rang_missile = typ->missile_range;
				rang_missile *= rang_missile;
				un->missile_number--;
				un->missile_timer = 0;
			} else
				un->missile_timer++;
		}

		additional_variables[ RANG_MIS ] = rang_missile;

	}
	while (loop < un->number_combatants) {
		battle_combatant_attack(&comb[loop],comb_at,gvar,additional_variables);
		loop++;
	}
}


static void battle_combatant_declare(n_combatant * comb, n_byte2 * gvar, 
							  n_unit * un_at, n_byte reverso, n_int group_facing){

	/* if the attacking combatant isn't dead */
	/* cache the important values locally */
	n_int	loc_f = comb->direction_facing;
    n_int   loc_s = comb->speed_current;
	/* the initial condition sets up "nothing to attack" and the
	   maximum distance to attack squared */
	n_byte2 loc_attack = NUNIT_NO_ATTACK;
	n_byte2 max_distance_squared = gvar[GVAR_DECLARE_MAX_START_DSQ];	/* val3 */

	n_combatant *comb_at = un_at->combatants;
    
    n_vect2 loc, at_av, difference;

	n_int	distance_centre_squ;
    
    vect2_byte2(&loc, &comb->location_x);
    vect2_byte2(&at_av, &un_at->average_x);
    
    vect2_subtract(&difference, &loc, &at_av);
    
    distance_centre_squ = vect2_dot(&difference, &difference, 1, 1);


	if (comb->wounds == NUNIT_DEAD)
    {
		return;
	}


	if(distance_centre_squ < gvar[GVAR_DECLARE_ONE_TO_ONE_DSQ]) /* val4 */
    {
        
		/* the direction facing vector */
        
        n_vect2 facing;
        n_byte2 loop2 = 0;

        vect2_direction(&facing, loc_f, 1);

		/* if the combatant is more than half way through,
			switch the direction back on the closest-checking loop */

		while (loop2 < un_at->number_combatants)
        {
			n_byte2	loc_test = loop2;
			
			if (reverso){
				loc_test = (n_byte2) ((un_at->number_combatants) - 1 - loop2);
			}

			if (comb_at[ loc_test ].wounds != NUNIT_DEAD)
            {
                n_vect2 delta, local_test;
                n_int   distance_squared;
                
                vect2_byte2(&local_test, &comb_at[ loc_test ].location_x);
                
                vect2_subtract(&delta, &local_test, &loc);
                
                distance_squared = vect2_dot(&delta, &delta, 1, 1);
                
				/* if the combatant is closer than the previous lot AND visible (ie in front) */
				if ((distance_squared < max_distance_squared) && ((vect2_dot(&delta, &facing, 1, 1)) > 0)) {
					/* it is the prefered attacked combatant and the distance and instance is
						stored */
					max_distance_squared = (n_byte2)distance_squared;
					loc_attack = loc_test;
					/* if this combatant is within the melee attacking distance,
						the search is over, end this loop(2) */
					if (max_distance_squared < gvar[GVAR_DECLARE_CLOSE_ENOUGH_DSQ])					           /* val5 */
						loop2 += 0xffff;
				}
			}
			loop2++;
		}
	}
	
	comb->attacking    = loc_attack;
	comb->distance_squ = max_distance_squared;


	if(group_facing == -1)
    {
		if(loc_attack != NUNIT_NO_ATTACK)
        {
            n_vect2 delta, local_test;
            
            vect2_byte2(&local_test, &comb_at[ loc_attack ].location_x);
            vect2_subtract(&delta, &local_test, &loc);
	
			group_facing = math_turn_towards(&delta,comb->direction_facing, 0) ;
		}
        else
        {
            loc_s = 0;
		}
	}
  
	comb->speed_current = loc_s;
	comb->direction_facing = group_facing;
}


void battle_declare(n_unit *un, n_byte2 * gvar) {
	n_uint		 loop = 0;
	n_int      group_facing = -1;
	n_combatant *comb      = un->combatants;
	/* the unit that is being attacked */
	n_unit	    *un_at      = un->unit_attacking;
	n_byte2      loc_number = un->number_combatants;

	/* only worthwhile if the unit is attacking something */
	if (un_at == 0L) return;
	
	{
		/* the combatants being attacked */
        
        n_vect2 attacker, attackee, delta;
        
        vect2_byte2(&attacker, &un_at->average_x);
        vect2_byte2(&attackee, &un->average_x);
        
        vect2_subtract(&delta, &attacker, &attackee);
    
		if(vect2_dot(&delta, &delta, 1, 1) >= gvar[GVAR_DECLARE_GROUP_FACING_DSQ]){		                   /* val2 */
        group_facing = math_turn_towards(&delta,group_facing, 2);
    }
	}
	while (loop < loc_number) {
		n_byte  reverso = (n_byte) (loop > (n_uint)(loc_number >> 1));
		battle_combatant_declare(&comb[loop], gvar, un_at, reverso, group_facing);
		loop++;
	}
}

/* DEPRECATED */
static void battle_combatant_move(n_combatant * comb, n_byte2 * gvar){
	n_int loc_s = comb->speed_current;
	n_int loc_f = comb->direction_facing;
    
    n_vect2 location, old_location;
    
    vect2_populate(&location, comb->location_x, comb->location_y);
    vect2_copy(&old_location, &location);
    
	if (comb->wounds == NUNIT_DEAD)
    {
		return;
	}
    
    if (loc_s)
    {
        n_vect2 facing;
        vect2_direction(&facing, loc_f, 40);
        vect2_d(&location, &facing, loc_s, 2000);
        
        if(board_move(&old_location, &location))
        {
            vect2_back_byte2(&location, &comb->location_x);
        }
    }
}


/* this is currently fudged for the skirmish testing... it will be fixed
   in the future... honest... */

/* DEPRECATED */
void battle_move(n_unit *un, n_byte2 * gvar)
{
	n_combatant *comb       = un->combatants;
	n_byte2      loc_number = un->number_combatants;
	n_uint      loop = 0;
	while (loop < loc_number) {
		battle_combatant_move(&comb[loop],gvar);
		loop++;
	}
}


/* as combatants can continue to fight for the entire battle_cycle,
   the death condition only occurs when wounds == NUNIT_DEAD */

void battle_remove_dead(n_unit *un) {

	n_combatant *comb = (n_combatant *)(un->combatants);
	n_uint sum_x = 0;
	n_uint sum_y = 0;
	n_byte2 sum = 0;
	n_byte2 loop = 0;
	while (loop < un->number_combatants)
    {
		if (comb[ loop ].wounds != NUNIT_DEAD)
        {
			if (comb[ loop ].wounds == 0)
            {
				comb[ loop ].wounds = NUNIT_DEAD;
				comb[ loop ].attacking = NUNIT_NO_ATTACK;

				board_clear(comb[ loop ].location_x, comb[ loop ].location_y);

			}
            else
            {
				sum_x += comb[ loop ].location_x;
				sum_y += comb[ loop ].location_y;
				sum++;
			}
		}
		loop++;
	}
	if(sum != 0)
    {
		un->average_x = (n_byte2)(sum_x / sum);
		un->average_y = (n_byte2)(sum_y / sum);
	}
	un->number_living = sum;
}

/* returns 1 if the alignment 0 army is all dead,
   returns 2 if the alignment 1 army is all dead,
   returns 3 if both armies are all dead, and,
   returns 0 in all other cases */

/* this performs all the "General" logic */
n_byte battle_opponent(n_unit * un, n_uint	num) {
	n_uint	loop = 0;
	n_uint	unit_count[2] = {0};
	while(loop < num)
    {
		if(un[loop].number_living > 0)
        {
			n_unit	*un_att = un[loop].unit_attacking;
			n_int	local_alignment = (un[loop].alignment) & 1;
			
			unit_count[local_alignment]++;

      /* if they are all dead, there is nothing left to attack */
			if(un_att != 0L)
            {
				if(un_att->number_living == 0)
                {
					un_att = 0L;
                }
			}

      /* this selects the attacking unit */
			if(un_att == 0L)
            {
				n_int	px = un[loop].average_x;
				n_int	py = un[loop].average_y;
				n_uint	min_dist_squ = 0xffffffff;
				n_uint	loop2 = 0;
				while(loop2 < num)
                {
					if((loop != loop2) && (un[loop2].number_living!= 0))
                    {
						if(((un[loop2].alignment)&1) != local_alignment)
                        {
							n_int	tx = un[loop2].average_x;
							n_int	ty = un[loop2].average_y;
							n_uint   dist_squ;
							tx -= px;
							ty -= py;

							dist_squ = (n_uint)((tx*tx) + (ty*ty));
							if(dist_squ < min_dist_squ)
                            {
								min_dist_squ = dist_squ;
								un_att = (n_unit *)&un[loop2];
							}
						}
					}
					loop2++;
				}
			}
			un[loop].unit_attacking = (void *)un_att;
		} else
			un[loop].unit_attacking = (void *)0L;
		loop++;
	}
	return (n_byte)((unit_count[0] == 0) + ((unit_count[1] == 0)<<1));
}

